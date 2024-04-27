/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  Window.cpp
Purpose:  Container for MVC decoupled classes for Viewport, Renderer & Input
Language: C++ / Eng, MSVC (needs modern x64 platforms supporting GLFW & OGL 4)
Platform: MSVC / Windows 10  x64 (needs x64 machines supporting GLFW & OpenGL 4)
Project:  a.Surprise_CS300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <../Packages/include/...>
#include <glad/glad.h>    // GLAD OGL wrangler (for GL 4.3)
#include <GLFW/glfw3.h>   // GLFW windowing package
// "./src/..."
#include "File.h"         // Read in string of shader text files
#include "InputGLFW.h"    // Instantiation of GLFW driven input manager
#include "Log.h"          // Error / warning output management to log files
#include "RendererGL4.h"  // Instantiation of GLCore 
#include "ViewportGLFW.h" // Instantiation of GLFW driven view manager
#include "Value.h"        // Common value adjustment utility functions
#include "Window.h"       // Class declaration header file


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/* Create a Window according to given settings                                */
dp::Window::Window(u2 width, u2 height, const std::string& title,
  bool fullscreen, bool maximized, bool showTitle, bool resizable,
  WindowRen::PACKAGE renderingPackage, WindowGen::PACKAGE windowingPackage)
  : renderPackage(renderingPackage), windowPackage(windowingPackage)
{
  // Validate / sanitize / convert inputs
  if (resizable)
  {
    if (showTitle) { windowStyle = WindowGen::DECORATION::RESIZABLE; }
    else { windowStyle = WindowGen::DECORATION::CLEAN; } // resizable; ~title
  } // end resizable
  else // ~resizable
  {
    if (showTitle) { windowStyle = WindowGen::DECORATION::FIXED; }
    else { windowStyle = WindowGen::DECORATION::BARE; } // ~resizable; ~title
  } // end not resizable

  if (fullscreen)
  {
    if (maximized) { windowMode = WindowGen::MODE::FSMAX; } // fs;  maximized
    else { windowMode = WindowGen::MODE::FULLSCREEN; }      // fs; ~maximized
  } // end fullscreen
  else // ~fullscreen
  {
    if (maximized) { windowMode = WindowGen::MODE::MAXIMIZED; } // ~fs;  max
    else { windowMode = WindowGen::MODE::WINDOWED; }            // ~fs; ~max
  } // end ~fullscreen

  winRes.width = (u2)InU(width, u2_MAX, WindowGen::WIN_MIN.width);
  winRes.height = (u2)InU(height, u2_MAX, WindowGen::WIN_MIN.height);
  windowTitle = title;

  // Attempt to initialize window
  switch (windowPackage)
  {
  case WindowGen::PACKAGE::GLFW:
    InitGLFW();
    // Attempt to initialize renderer within GLFW window context
    switch (renderPackage)
    {
    case WindowRen::PACKAGE::GL_CORE:
      InitGL4GLFW();
      break;

    default:
      break;
    } // end switch Rendering PACKAGE
    break;

  default:
    InitGLFW();
    Log::Warn("Window - can't initialize renderer without supported windowing"
      + std::string("API selection: GLFW only currently"));
    break;
  } // end switch Windowing PACKAGE
  quad = new Mesh();
  cube = new Mesh(Mesh::Cube(2.0f, true, true));
  centerObj = new Mesh(Mesh::pSphere());
  orbitObj = new Mesh(Mesh::pSphere()/*"./Common/models/bunny.obj"*/);
  skyTex = new Cubemap(ASCIIPath("../Common/textures/sky_right.jpg"),
    ASCIIPath("../Common/textures/sky_left.jpg"),
    ASCIIPath("../Common/textures/sky_top.jpg"),
    ASCIIPath("../Common/textures/sky_bottom.jpg"),
    ASCIIPath("../Common/textures/sky_back.jpg"),
    ASCIIPath("../Common/textures/sky_front.jpg"));
  whtTex = new Texture();
  uvTex = new Texture(ASCIIPath("../Common/textures/UV_Grid.png"));
  capTex = new Framebuffer(BLACK.Opaque(), width, height);

  activeView = Camera(v3f(0.0f, 0.0f, 3.0f));

} // end Window::Window(u2, u2, bool, bool, bool, bool, renderer, windower)


// Deallocate all window resources allocated for operation (call Shutdown)    */
dp::Window::~Window(void)
{
  if (centerObj) { delete centerObj; }
  if (orbitObj) { delete orbitObj; }
  if (cube) { delete cube; }
  if (quad) { delete quad; }
  if (uvTex) { delete uvTex; }
  if (whtTex) { delete whtTex; }
  if (capTex) { delete capTex; }
  if (skyTex) { delete skyTex; }
  if (lamMat) { delete lamMat; }
  if (phnMat) { delete phnMat; }
  if (blnMat) { delete blnMat; }
  if (capMat) { delete capMat; }
  if (lamShade) { delete lamShade; }
  if (phnShade) { delete phnShade; }
  if (blnShade) { delete blnShade; }
  if (capShade) { delete capShade; }
  if (skyShade) { delete skyShade; }
  Shutdown();
} // end Window::~Window(void)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the pointer to Input in use (if any), for window control state         */
dp::Input* dp::Window::Control(void)
{
  return windowInput;
} // end Input* Window::Control(void)


// Set all the Window decoration control style settings at once               */
dp::Window& dp::Window::Decoration(bool showTitle, bool resizable)
{
  WindowGen::DECORATION windowStyle = (showTitle) ? ((resizable) ?
    WindowGen::DECORATION::RESIZABLE : WindowGen::DECORATION::FIXED) :
    ((resizable) ? WindowGen::DECORATION::CLEAN : WindowGen::DECORATION::BARE);
  if (windowView) { windowView->DecorationMode(windowStyle); }
  return *this;
} // end Window& Window::decoration(bool, bool)


// Set the Window decoration style and windowing mode concurrently            */
dp::Window& dp::Window::DecorationMode(bool fullscreen, bool maximized,
  bool showTitle, bool resizable)
{
  windowMode = (fullscreen) ? ((maximized) ? WindowGen::MODE::FSMAX :
    WindowGen::MODE::FULLSCREEN) : ((maximized) ? WindowGen::MODE::MAXIMIZED :
      WindowGen::MODE::WINDOWED);
  windowStyle = (showTitle) ? ((resizable) ?
    WindowGen::DECORATION::RESIZABLE : WindowGen::DECORATION::FIXED) :
    ((resizable) ? WindowGen::DECORATION::CLEAN : WindowGen::DECORATION::BARE);
  if (windowView) { windowView->DecorationMode(windowStyle, windowMode); }
  return *this;
} // end Window& Window::decoration(bool, bool, bool, bool)


// Get the pixel height & width in use by the window                          */
const dp::WindowGen::Dimensions& dp::Window::Dimensions(void) const
{
  if (windowView)
  {
    return windowView->Dimensions();
  }
  return winRes;

} // end const WindowGen::Dimensions& Window::dimensions(void) const


// Set the Window's pixel height & width, preserving render scale             */
dp::Window& dp::Window::Dimensions(u2 winWidth, u2 winHeight)
{
  if (windowView)
  {
    windowView->Dimensions(winWidth, winHeight);
  }
  return *this;

} // end Window& Window::dimensions(u2, u2)


/** Get the pixel height & width in use by the window                         */
const dp::WindowGen::Dimensions& dp::Window::DimensionsWindowed(void)
const
{
  if (windowView)
  {
    return windowView->DimensionsWindowed();
  }
  return winRes;

} // end const WindowGen::Dimensions& Window::dimensions(void)


// Get whether the window is currently set to use fullscreen mode             */
bool dp::Window::Fullscreen(void) const
{
  if (windowView)
  {
    return windowView->FullscreenMode();
  }
  return (windowMode == WindowGen::MODE::FULLSCREEN || windowMode ==
    WindowGen::MODE::FSMAX) ? true : false;

} // end bool Window::fullscreen(void) const


// Set whether the window is currently set to use fullscreen mode             */
dp::Window& dp::Window::Fullscreen(bool fullscreenEnabled)
{
  if (windowView)
  {
    windowView->FullscreenMode(fullscreenEnabled);
  }
  if (fullscreenEnabled)
  {
    windowMode = (windowMode == WindowGen::MODE::MAXIMIZED || windowMode ==
      WindowGen::MODE::FSMAX) ? WindowGen::MODE::FSMAX :
      WindowGen::MODE::FULLSCREEN;
  }
  else
  {
    windowMode = (windowMode == WindowGen::MODE::MAXIMIZED || windowMode ==
      WindowGen::MODE::FSMAX) ? WindowGen::MODE::MAXIMIZED :
      WindowGen::MODE::WINDOWED;
  }
  return *this;

} // end Window& Window::fullscreen(bool)

dp::Window& dp::Window::FullscreenToggle(void)
{
  if (windowView)
  {
    bool mode = windowView->FullscreenMode();
    windowView->FullscreenMode(!mode);
  }
  return *this;
} // end Window& Window::FullscreenToggle(void)


// Perform GPU uploads / setup the mesh to be accessed for rendering          */
void dp::Window::LoadMesh(Mesh& mesh) const
{
  if (renderer) { renderer->LoadMesh(mesh); }
} // end void Window::LoadMesh(Mesh&) const

// Get whether the window is currently set to maximized mode                  */
bool dp::Window::Maximized(void) const
{
  if (windowView)
  {
    return windowView->MaximizedMode();
  }
  return (windowMode == WindowGen::MODE::MAXIMIZED || windowMode ==
    WindowGen::MODE::FSMAX) ? true : false;

} // end bool Window::maximized(void) const


// Set whether the window is currently set to use maximized mode              */
dp::Window& dp::Window::Maximized(bool maximizedWindow)
{
  if (windowView)
  {
    windowView->MaximizedMode(maximizedWindow);
  }
  if (maximizedWindow)
  {
    windowMode = (windowMode == WindowGen::MODE::FULLSCREEN || windowMode ==
      WindowGen::MODE::FSMAX) ? WindowGen::MODE::FSMAX :
      WindowGen::MODE::MAXIMIZED;
  }
  else
  {
    windowMode = (windowMode == WindowGen::MODE::FULLSCREEN || windowMode ==
      WindowGen::MODE::FSMAX) ? WindowGen::MODE::FULLSCREEN :
      WindowGen::MODE::WINDOWED;
  }
  return *this;

} // end Window& Window::maximized(bool)

dp::Window& dp::Window::MaximizedToggle(void)
{
  if (windowView)
  {
    bool mode = windowView->MaximizedMode();
    windowView->MaximizedMode(!mode);
  }
  return *this;
} // end Window& Window::MaximizedToggle(void)


// Set the Window decoration style and windowing mode concurrently            */
dp::Window& dp::Window::Mode(bool fullscreen, bool maximized)
{
  windowMode = (fullscreen) ? ((maximized) ? WindowGen::MODE::FSMAX :
    WindowGen::MODE::FULLSCREEN) : ((maximized) ? WindowGen::MODE::MAXIMIZED :
      WindowGen::MODE::WINDOWED);
  if (windowView) {
    windowView->DecorationMode(WindowGen::DECORATION::UNDEFINED, windowMode);
  }
  return *this;
} // end Window& Window::decoration(bool, bool, bool, bool)


// Get the pointer to Viewport in use (if any), for window model settings     */
dp::Viewport* dp::Window::Frame(void)
{
  return windowView;
} // end Viewport* Window::Frame(void)


/** Perform required steps to put the window in an open, running state        */
void dp::Window::Open(void)
{
  if (winState == WindowGen::STATUS::DEFAULT)
  {
    switch (windowPackage)
    {
    case WindowGen::PACKAGE::GLFW:
      InitGLFW();
      break;
    default:
      InitGLFW();
      break;
    }
  }
  if (winState == WindowGen::STATUS::READY)
  {
    if (renderer)
    {
      if (centerObj) { renderer->LoadMesh(*centerObj); }
      if (whtTex) { renderer->LoadTexture(*whtTex); }
      if (uvTex) { renderer->LoadTexture(*uvTex); }
      if (capTex) { renderer->LoadTarget(*capTex); }
      if (cube) { renderer->LoadMesh(*cube); }
      if (skyTex) { renderer->LoadCubemap(*skyTex); }
      if (capShade)
      {
        if (!capShade->Loaded()) { renderer->LoadShader(*capShade); }
        if (capShade->Loaded())
        {
          capMat = new Capture(*capShade);
        }
        else
        {
          Log::Warn("Window::InitGLCoreGLFW - screen shader load failed");
        }
      }
      if (skyShade)
      {
        if (!skyShade->Loaded()) { renderer->LoadShader(*skyShade); }
        if (skyShade->Loaded())
        {
          skyMat = new Skybox(*skyShade);
        }
        else
        {
          Log::Warn("Window::InitGLCoreGLFW - screen shader load failed");
        }
      }
      if (lamShade)
      {
        if (!lamShade->Loaded()) { renderer->LoadShader(*lamShade); }
        if (lamShade->Loaded())
        {
          lamMat = new Lambert(*lamShade);
        }
        else
        {
          Log::Warn("Window::InitGLCoreGLFW - lambert shader load failed");
        }
      }
      if (phnShade)
      {
        if (!phnShade->Loaded()) { renderer->LoadShader(*phnShade); }
        if (phnShade->Loaded())
        {
          phnMat = new Phong(*phnShade);
        }
        else
        {
          Log::Warn("Window::InitGLCoreGLFW - phong shader load failed");
        }
      }
      if (blnShade)
      {
        if (!blnShade->Loaded()) { renderer->LoadShader(*blnShade); }
        if (blnShade->Loaded())
        {
          blnMat = new Blinn(*blnShade);
          setMat = blnMat;
        }
        else
        {
          Log::Warn("Window::InitGLCoreGLFW - blinn shader load failed");
        }
      }
    }
    // Update the data read in from the window until solicited otherwise
    while (windowInput->Running())
    {
      Update();
    } // close render loop
  }
  else
  {
    Log::Error("Window::Open - Window not in runnable state to open");
  }
} // end Window::open(void)


// Get whether the window is currently allowing resizing                      */
bool dp::Window::Resizable(void) const
{
  return (windowStyle == WindowGen::DECORATION::RESIZABLE || windowStyle ==
    WindowGen::DECORATION::CLEAN) ? true : false;
} // end bool Window::resizable(void) const


/** Set whether the viewport is allowed to be dynamically resized by users    */
dp::Window& dp::Window::Resizable(bool resizingAllowed)
{
  if (windowView)
  {
    windowView->DecorationMode((windowStyle == WindowGen::DECORATION::FIXED ||
      windowStyle == WindowGen::DECORATION::RESIZABLE) ? ((resizingAllowed) ?
        WindowGen::DECORATION::RESIZABLE : WindowGen::DECORATION::FIXED) :
      ((resizingAllowed) ?
        WindowGen::DECORATION::CLEAN : WindowGen::DECORATION::BARE));
  }
  if (resizingAllowed)
  {
    windowStyle = (windowStyle == WindowGen::DECORATION::FIXED || windowStyle
      == WindowGen::DECORATION::RESIZABLE) ? WindowGen::DECORATION::RESIZABLE
      : WindowGen::DECORATION::CLEAN;
  }
  else
  {
    windowStyle = (windowStyle == WindowGen::DECORATION::FIXED || windowStyle
      == WindowGen::DECORATION::RESIZABLE) ? WindowGen::DECORATION::FIXED :
      WindowGen::DECORATION::BARE;
  }
  return *this;

} // end Window& Window::resizable(bool)


// Set Window render dimension scale within the window (not Window pixels)    */
dp::Window& dp::Window::Resolution(u2 width, u2 height)
{
  if (windowView)
  {
    windowView->Resolution(width, height);
  }
  return *this;

} // end Window& Window::resolution(u2, u2)


/** Put the window in a closed, uninitialized state (deallocate resources)    */
void dp::Window::Shutdown(void)
{
  if (winState == WindowGen::STATUS::READY)
  {
    if (renderer)
    {
      renderer->Shutdown();
      delete renderer;
      renderer = nullptr;
    } // end Renderer deallocation
    if (windowInput)
    {
      windowInput->Shutdown();
      delete windowInput;
      windowInput = nullptr;
    } // end Input deallocation
    if (windowView)
    {
      windowView->Shutdown();
      delete windowView;
      windowView = nullptr;
    } // end Viewport deallocation
    if (windowContext)
    {
      switch (windowPackage)
      {
      case WindowGen::PACKAGE::GLFW:
        glfwDestroyWindow((GLFWwindow*)windowContext);
        windowContext = nullptr;
        break;
      default:
        // Can't properly clean up what doesn't have a process of being freed
        break;
      }
    }
    winState = WindowGen::STATUS::DEFAULT;
  } // end READY state exit branch
  else if (winState < WindowGen::STATUS::DEFAULT)
  {
    Log::Info("Window::Shutdown - called in invalid window state");
  }
  // else DEFAULT state is fine: just redundant, so read as no-op

} // end Window::shutdown(void)


// Set Window pixel dimensions and render scale for the window together       */
dp::Window& dp::Window::Size(u2 winWidth, u2 winHeight)
{
  if (windowView)
  {
    windowView->Dimensions(winWidth, winHeight);
    windowView->Resolution(winWidth, winHeight);
  }
  winWidth = (u2)InU(winWidth, u2_MAX, WindowGen::WIN_MIN.width);
  winRes.width = winWidth;
  winHeight = (u2)InU(winHeight, u2_MAX, WindowGen::WIN_MIN.height);
  winRes.height = winHeight;
  return *this;
} // end Window& Window::size(u2, u2)


// Get current state enumeration value relating to whether window can run     */
dp::WindowGen::STATUS dp::Window::Status(void) const
{
  return winState;
} // end WindowGen::STATUS Status(void) const


// Get the Window's current label / name / title text string                  */
const std::string& dp::Window::Title(void) const
{
  if (windowView) { return windowView->Title(); }
  return windowTitle;
} //end const std::string& title(void) const


// Set the title field for Window name in titlebar and taskbar                */
dp::Window& dp::Window::Title(const std::string& titleField)
{
  if (windowView)
  {
    windowView->Title(titleField);
  }
  windowTitle = titleField;
  return *this;
} // end Window& Window::title(const std::string&)


// Get state relating to whether window can run                               */
bool dp::Window::Titlebar(void) const
{
  return (windowStyle == WindowGen::DECORATION::RESIZABLE ||
    windowStyle == WindowGen::DECORATION::FIXED) ? true : false;
} //end bool titlebar(void) const


// Set whether the viewport is allowed to be dynamically resized by users     */
dp::Window& dp::Window::Titlebar(bool titleVisible)
{
  if (titleVisible)
  {
    switch (windowStyle)
    {
    case WindowGen::DECORATION::BARE:
      if (windowView)
      {
        windowView->DecorationMode(WindowGen::DECORATION::FIXED);
      }
      break;
    case WindowGen::DECORATION::CLEAN:
      if (windowView)
      {
        windowView->DecorationMode(WindowGen::DECORATION::RESIZABLE);
      }
      break;
    case WindowGen::DECORATION::RESIZABLE: case WindowGen::DECORATION::FIXED:
    default:
      break;
    }
  }
  else
  {
    switch (windowStyle)
    {
      // TODO: clean up mess to not use windowStyle...what were you thinking...
    case WindowGen::DECORATION::BARE: case WindowGen::DECORATION::CLEAN:
    default:
      break;
    }
  }
  return *this;

} // end Window& Window::titlebar(bool)


// Perform cleanup of prior rendering GPU uploads before mesh removal         */
void dp::Window::UnloadMesh(Mesh& mesh) const
{
  if (renderer) { renderer->UnloadMesh(mesh); }
} // end void Window::UnloadMesh(Mesh&) const


// Get the pointer to the Renderer in use (if any), for draw settings         */
dp::Renderer* dp::Window::View(void)
{
  return renderer;
} // end Renderer* Window::View(void)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Perform all render loop steps per frame                                    */
void dp::Window::Update(void)
{
  ang dRot = rotRate * windowInput->Dt();
  if (renderer && windowView && windowInput)
  {
    renderer->ClearTo(GRAY_20);
    PollImGuiAttrs();

    // Render updated scene elements
    if (centerObj)
    {
      renderer->Draw(*centerObj, *setMat, nullptr, &activeView, uvTex);
    }
    if (cube && skyMat && skyTex)
    {
      renderer->DrawCubemap(*cube, *skyMat, *skyTex, &activeView);
    }

    // Update / display the GUI
    renderer->ImGUIDraw(UI);
    // Swap display to new frame
    windowView->Update();
    windowInput->Read();
  }

} // end void Window::Update(void)


// Check changes in settings from previous frames, update internal settings   */
void dp::Window::PollImGuiAttrs(void)
{
  if (UI.shadPrev != UI.shadCurr)
  {
    switch (UI.shadCurr)
    {
    case 0: // [0] = "Lambert"
      setMat = lamMat;
      UI.ambientCurrent = lamMat->Ambient();
      UI.diffuseCurrent = lamMat->Diffuse();
      UI.incandescenceCurrent = lamMat->Incandescence();
      UI.fresnelCurrent = lamMat->Fresnel();
      break;
    case 1: // [1] = "Phong"
      setMat = phnMat;
      UI.ambientCurrent = phnMat->Ambient();
      UI.diffuseCurrent = phnMat->Diffuse();
      UI.incandescenceCurrent = phnMat->Incandescence();
      UI.fresnelCurrent = phnMat->Fresnel();
      UI.specularCurrent = phnMat->Specular();
      UI.falloffCurrent = phnMat->Falloff();
      break;
    case 2: // [2] = "Blinn"
      setMat = blnMat;
      UI.ambientCurrent = blnMat->Ambient();
      UI.diffuseCurrent = blnMat->Diffuse();
      UI.incandescenceCurrent = blnMat->Incandescence();
      UI.fresnelCurrent = blnMat->Fresnel();
      UI.specularCurrent = blnMat->Specular();
      UI.falloffCurrent = blnMat->Falloff();
      break;
    }
    UI.shadPrev = UI.shadCurr;
  }
  GLuint cProgram = setMat->ShadingGroup().handle;
  if (UI.uvMPrev != UI.uvMCurrent)
  {
    GLuint uvs = glGetUniformLocation(cProgram, "proceduralUVs");
    if (uvs != -1) { glUniform1i(uvs, UI.uvMCurrent); }
    else { Log::Info("RendererGLCore::ImGUIDraw - uv mode unavailable"); }
    UI.uvMPrev = UI.uvMCurrent;
  }
  if (UI.falloffPrev != UI.falloffCurrent)
  {
    switch (setMat->ShadingType())
    {
    case Material::Type::Phong:
      phnMat->Falloff(UI.falloffCurrent);
      break;
    case Material::Type::Blinn:
      blnMat->Falloff(UI.falloffCurrent);
      break;
    }
    UI.falloffPrev = UI.falloffCurrent;
  }
  if (UI.ambientPrev != UI.ambientCurrent)
  {
    switch (setMat->ShadingType())
    {
    case Material::Type::Lambert:
      lamMat->Ambient(UI.ambientCurrent);
      break;
    case Material::Type::Phong:
      phnMat->Ambient(UI.ambientCurrent);
      break;
    case Material::Type::Blinn:
      blnMat->Ambient(UI.ambientCurrent);
      break;
    }
    UI.ambientPrev = UI.ambientCurrent;
  }
  if (UI.diffusePrev != UI.diffuseCurrent)
  {
    switch (setMat->ShadingType())
    {
    case Material::Type::Lambert:
      lamMat->Diffuse(UI.diffuseCurrent);
      break;
    case Material::Type::Phong:
      phnMat->Diffuse(UI.diffuseCurrent);
      break;
    case Material::Type::Blinn:
      blnMat->Diffuse(UI.diffuseCurrent);
      break;
    }
    UI.diffusePrev = UI.diffuseCurrent;
  }
  if (UI.specularPrev != UI.specularCurrent)
  {
    switch (setMat->ShadingType())
    {
    case Material::Type::Phong:
      phnMat->Specular(UI.specularCurrent);
      break;
    case Material::Type::Blinn:
      blnMat->Specular(UI.specularCurrent);
      break;
    }
    UI.specularPrev = UI.specularCurrent;
  }
  if (UI.incandescencePrev != UI.incandescenceCurrent)
  {
    switch (setMat->ShadingType())
    {
    case Material::Type::Lambert:
      lamMat->Incandescence(UI.incandescenceCurrent);
      break;
    case Material::Type::Phong:
      phnMat->Incandescence(UI.incandescenceCurrent);
      break;
    case Material::Type::Blinn:
      blnMat->Incandescence(UI.incandescenceCurrent);
      break;
    }
    UI.incandescencePrev = UI.incandescenceCurrent;
  }
  if (UI.fresnelPrev != UI.fresnelCurrent)
  {
    switch (setMat->ShadingType())
    {
    case Material::Type::Lambert:
      lamMat->Fresnel(UI.fresnelCurrent);
      break;
    case Material::Type::Phong:
      phnMat->Fresnel(UI.fresnelCurrent);
      break;
    case Material::Type::Blinn:
      blnMat->Fresnel(UI.fresnelCurrent);
      break;
    }
    UI.fresnelPrev = UI.fresnelCurrent;
  }
  if (UI.axisPrev != UI.axisCurrent)
  {
    m4f rotBack;
    UI.axisCurrent.Normalize();
    GLuint uvBasis = glGetUniformLocation(cProgram, "uvB");
    if (uvBasis != -1)
    {
      // if (axisCurrent == Z_HAT) { /* identity leaves z aligned with z */ }
      if (UI.axisCurrent == -Z_HAT) // Anti-parallel axis check
      {
        rotBack = RotateRodriguesAffine(ang(180.0f, arc::degrees), X_HAT);
      }
      else // Arbitrary axis rotation
      {
        v3f axis(UI.axisCurrent.Cross(Z_HAT));
        ang angle = ang(acos(UI.axisCurrent.Dot(Z_HAT)), arc::rad);
        rotBack = RotateRodriguesAffine(angle, axis);
        if (rotBack * UI.axisCurrent.XYZW(Of) == -UNIT_VEC_Z)
        {
          rotBack = RotateRodriguesAffine(-angle, axis);
        }
        v4f testVec = rotBack * UI.axisCurrent.XYZW(Of);
        if (testVec != UNIT_VEC_Z)
        {
          v3f out = testVec.XYZ();
          Log::Warn("RendererGLCore::ImGUIDraw - rotation yielded "
            + out.String());
        }
      }
      glUniformMatrix4fv(uvBasis, 1, GL_FALSE, &rotBack[0][0]);
    }
    else { Log::Warn("RendererGLCore::ImGUIDraw - uv basis unavailable"); }
    UI.axisPrev = UI.axisCurrent;
  }
} // end Window::PollImGuiAttrs(void)


// Initialize resources to put Window in an open, running state for GLFW      */
void dp::Window::InitGLFW(void)
{
  // Attempt to initialize GLFW
  if (glfwInit() != GL_TRUE)
  {
    Log::Error("Window::InitGLFW - GLFW init failed");
    winState = WindowGen::STATUS::UNDEFINED;
  }
  else
  {
    // Set OGL core profile mode
    glfwWindowHint(GLFW_SAMPLES, 1); // Anti-aliasing 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, WindowRen::GL_REV.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, WindowRen::GL_REV.minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // input manager taking care of these decisions separately
    // glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // TODO: check for settings file / hardware changes?
    // ...failing that, query available resolution range for preferred size

    // Create window context for viewport & input managers to utilize
    GLFWwindow* context;
    if ((context = glfwCreateWindow(winRes.width, winRes.height,
      windowTitle.c_str(), NULL, NULL)) == NULL)
    {
      // GLFW failed to create a window under current settings:
      // GL 4.3 not supported by hardware 
      glfwTerminate();
      // TODO: query available OGL revision on hardware to fallback to instead
      // use (OpenGL Extension Viewer | glxinfo | something)
      Log::Error("Window::InitGLFW - failed to generate GLFW window context");
      winState = WindowGen::STATUS::UNDEFINED;
    }
    else
    {
      // glewExperimental = static_cast<GLBoolean>(true);
      // (Only needed for core profile with GLEW; not GLAD)

      windowContext = (void*)context;
      windowView = new ViewportGLFW(context, winRes.width, winRes.height,
        windowTitle, windowMode, windowStyle);
      if (windowView)
      {
        windowInput = new InputGLFW(this, context);
        if (windowInput)
        {
          winState = WindowGen::STATUS::READY;
        } // end InputGLFW allocation succeeded
      } // end ViewportGLFW allocation succeeded
    } // end GLFW context creation succeeded
  } // end GLFW init succeeded

} // end Window::initGLFW(void)


// Perform initialization for OpenGL setup in core profile, v4.3              */
void dp::Window::InitGL4GLFW(void)
{
  if (windowContext && windowPackage == WindowGen::PACKAGE::GLFW)
  {
    const std::string vertShaderPath = "../shaders/vertex/";
    const std::string fragShaderPath = "../shaders/fragment/";
    GLFWwindow* context = static_cast<GLFWwindow*>(windowContext);
    std::vector<ShaderFile> vertShaders = File::ShaderList(vertShaderPath,
      "glvs");
    std::vector<ShaderFile> fragShaders = File::ShaderList(fragShaderPath,
      "glfs");
    if (vertShaders.size() == 0)
    {
      Log::Warn("Window::InitGLCoreGLFW - no vertex shaders found");
      // hard code > write files in any install path write privelege fragility
      vertShaders.push_back(ShaderFile(ASCIIPath("./NDC.GLVS"),
        ShaderFile::CodeType::GLSL43,
        "#version 430 core\n"
        "layout (location = 0) in v3f aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0"));
    }
    if (fragShaders.size() == 0)
    {
      Log::Warn("Window::InitGLCoreGLFW - no fragment shaders found");
      // hard code > write files in any install path write privelege fragility
      vertShaders.push_back(ShaderFile(ASCIIPath("./Flat.GLFS"),
        ShaderFile::CodeType::GLSL43,
        "#version 430 core\n"
        "out vec4 FlatColor;\n"
        "void main()\n"
        "{\n"
        "   FlatColor = vec4(1.0f, 0.0f, 0.5f, 1.0f);\n"
        "}\n\0"));
    }
    renderer = new RendererGL4(context, vertShaders, fragShaders);

    // Set up scene local shaders with specific desired shaders
    u4 sDef = 0, sScr = 0, sLam = 0, sPho = 0, sBln = 0, sCap = 0, found = 0,
      sSky = 0, sBox = 0;
    for (u4 i = 0; i < vertShaders.size(); ++i)
    {
      if (vertShaders[i].Name() == "Default") { sDef = i; ++found; }
      else if (vertShaders[i].Name() == "Screen") { sScr = i; ++found; }
      else if (vertShaders[i].Name() == "Sky") { sSky = i; ++found; }
    }
    for (u4 i = 0; i < fragShaders.size(); ++i)
    {
      if (fragShaders[i].Name() == "Lambert") { sLam = i; ++found; }
      else if (fragShaders[i].Name() == "Phong") { sPho = i; ++found; }
      else if (fragShaders[i].Name() == "Blinn") { sBln = i; ++found; }
      else if (fragShaders[i].Name() == "Capture") { sCap = i; ++found; }
      else if (fragShaders[i].Name() == "Box") { sBox = i; ++found; }
    }
    if (renderer && found == 8)
    {
      capShade = new Shader(vertShaders[sScr], fragShaders[sCap]);
      skyShade = new Shader(vertShaders[sScr], fragShaders[sCap]);
      lamShade = new Shader(vertShaders[sDef], fragShaders[sLam]);
      phnShade = new Shader(vertShaders[sDef], fragShaders[sPho]);
      blnShade = new Shader(vertShaders[sDef], fragShaders[sBln]);
    }
  }
  else
  {
    Log::Error("Window::InitGLCoreGLFW - GLFW window context not initialized");
  }
} // end Window::initGL4GLFW(void)
