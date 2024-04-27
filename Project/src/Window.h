/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  Window.h
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

// <stl>
#include <string>       // Window title field input parameter & storage member
// "./src/..."
#include "Camera.h"     // Window member Containing active camera view settings
#include "Cubemap.h"    // Testing multi-FBO capability
#include "Framebuffer.h"// Testing FBO capability
#include "GuiAttr.h"    // ImGui UI fields will manipulate member values
#include "Mesh.h"       // Loading / unloading Meshes on GPU via renderer
#include "Texture.h"    // Testing textured mesh display
#include "Shader.h"     // Testing material parameter passing for render
#include "Skybox.h"     // Testing material parameter passing for render
#include "Lambert.h"    // Testing material parameter passing for render
#include "Capture.h"    // Testing material parameter passing for render
#include "Phong.h"      // Testing material parameter passing for render
#include "Blinn.h"      // Testing material parameter passing for render
#include "TypeErrata.h" // Platform primitive type aliases for easier porting
#include "WindowGen.h"  // Generic shared window generation states & categories
#include "WindowRen.h"  // Generic shared renderer states and categories


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{
  class Viewport; // Base class for window context model / frame perpetuation
  class Renderer; // Base class for window context view / content perpetuation
  class Input;    // Base class for window context controller / input reading

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Window //! Container for processing decoupled window display & input
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a Window according to given settings
    \brief
      - create a Window according to given settings
    \param width
      - pixel dimensions of the window in the horizontal axis
    \param height
      - pixel dimensions of the window in the vertical axis
    \param title
      - displayed as the window's name at top, in the taskbar, etc
    \param fullscreen
      - whether to display the window in fullscreen mode
    \param maximized
      - whether to display the window in maximized windowed mode, when windowed
    \param showTitle
      - whether to display the window with a title bar, or hide window controls
    \param resizable
      - whether to allow the window to be dynamically scaled by the user
    \param renderingPackage
      - windowRen's enumeration for which rendering API should be used to draw
    \param windowingPackage
      - windowGen's enumeration for which windowing API should be used to draw
    */
    Window(u2 width = WindowGen::DEFAULT_RES.height, u2 height =
      WindowGen::DEFAULT_RES.height, const std::string& title = "", bool
      fullscreen = ((WindowGen::DEFAULT_WIN_MODE == WindowGen::MODE::FSMAX ||
      (WindowGen::DEFAULT_WIN_MODE == WindowGen::MODE::FULLSCREEN) ? true :
      false)), bool maximized = ((WindowGen::DEFAULT_WIN_MODE ==
      WindowGen::MODE::FSMAX || (WindowGen::DEFAULT_WIN_MODE ==
      WindowGen::MODE::MAXIMIZED) ? true : false)), bool showTitle =
      ((WindowGen::DEFAULT_WIN_DECR == WindowGen::DECORATION::RESIZABLE ||
        WindowGen::DEFAULT_WIN_DECR == WindowGen::DECORATION::FIXED) ? true :
      false), bool resizable = ((WindowGen::DEFAULT_WIN_DECR ==
      WindowGen::DECORATION::RESIZABLE || WindowGen::DEFAULT_WIN_DECR ==
      WindowGen::DECORATION::CLEAN) ? true : false),
      WindowRen::PACKAGE renderingPackage = WindowRen::DEFAULT_RENDERER,
      WindowGen::PACKAGE windowingPackage = WindowGen::DEFAULT_WINDOWER);


    /** Deallocate all window resources allocated for operation (call Shutdown)
    \brief
      - deallocate all window resources allocated for operation (call Shutdown)
    */
    ~Window(void);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get the pointer to Input in use (if any), for window control state
    \brief
      - get the pointer to Input in use (if any), for window control state
    \details
      - can return nullptr if no Windowing package set up a controller properly
    \return
      - pointer to the contained Input in use for window controller state data
    */
    Input* Control(void);


    /** Set all the Window decoration control style settings at once
    \brief
      - set all the Window decoration control style settings at once
    \param showTitle
      - whether to display the window with a title bar, or hide window controls
    \param resizable
      - whether to allow the window to be dynamically scaled by the user
    \return
      - reference to the modified Window for easier subsequent inline actions
    */
    Window& Decoration(bool showTitle, bool resizable);


    /** Set the Window decoration style and windowing mode concurrently
    \brief
      - set the Window decoration style and windowing mode concurrently
    \param fullscreen
      - whether to display the window in fullscreen mode
    \param maximized
      - whether to display the window in maximized windowed mode, when windowed
    \param showTitle
      - whether to display the window with a title bar, or hide window controls
    \param resizable
      - whether to allow the window to be dynamically scaled by the user
    \return
      - reference to the modified Window for easier subsequent inline actions
    */
    Window& DecorationMode(bool fullscreen, bool maximized, bool showTitle,
      bool resizable);


    /** Get the pixel height & width in use by the window
    \brief
      - get the pixel height & width in use by the window
    \return
      - container of pixel height & width in use by the window
    */
    const WindowGen::Dimensions& Dimensions(void) const;


    /** Set the Window's pixel height & width, preserving render scale
    \brief
      - set the Window's pixel height & width, preserving render scale
    \param winWidth
      - desired pixel width for the window
    \param winHeight
      - desired pixel height for the window
    \return
      - reference to the modified Window for easier subsequent inline actions
    */
    Window& Dimensions(u2 winWidth, u2 winHeight);


    /** Get the pixel height & width in use by the window
    \brief
      - get the pixel height & width in use by the window
    \return
      - container of pixel height & width in use by the window
    */
    const WindowGen::Dimensions& DimensionsWindowed(void) const;


    /** Get whether the window is currently set to use fullscreen mode
    \brief
      - get whether the window is currently set to use fullscreen mode
    \return
      - whether the window is currently set to fullscreen mode
    */
    bool Fullscreen(void) const;


    /** Set whether the window is currently set to use fullscreen mode
    \brief
      - set whether the window is currently set to use fullscreen mode
    \param fullscreenEnabled
      - whether to set Window to display in fullscreen (true), or windowed
    \return
      - reference to the modified Window for easier subsequent inline actions
    */
    Window& Fullscreen(bool fullscreenEnabled);


    /** Change the Window in or out of fullscreen depending on current settings
    \brief
      - change the Window in or out of fullscreen depending on current settings
    \details
      - derived class should call derived FullscreenMode with !fullscreen member
    \return
      - reference to the Viewport instance for easier subsequent inline actions
    */
    Window& FullscreenToggle(void);


    /** Perform GPU uploads / setup the mesh to be accessed for rendering
    \brief
      - perform GPU uploads / setup the mesh to be accessed for rendering
    \details
      - mesh will need VAO, VBO, EBO, etc, set with upload return values
    \param meshData
      - mesh to be uploaded to the GPU (non-const to have accessors modified)
    */
    void LoadMesh(Mesh& mesh) const;


    /** Get whether the window is currently set to maximized mode
    \brief
      - get whether the window is currently set to maximized mode
    \return
      - whether the window is currently set to maximized mode (if true)
    */
    bool Maximized(void) const;


    /** Set whether the window is currently set to use maximized mode
    \brief
      - set whether the window is currently set to use maximized mode
    \param maximizedWindow
      - whether to set the windowed display mode of the Window to maximized
    \return
      - reference to the modified Window for easier subsequent inline actions
    */
    Window& Maximized(bool maximizedWindow);


    /** Change the Window maximized mode on / off depending on current settings
    \brief
      - change the Window maximized mode on / off depending on current settings
    \details
      - derived class should call derived MaximizedMode with !maximized member
    \return
      - reference to the Viewport instance for easier subsequent inline actions
    */
    Window& MaximizedToggle(void);
    

    /** Set the Window decoration style and windowing mode concurrently
    \brief
      - set the Window decoration style and windowing mode concurrently
    \param fullscreen
      - whether to display the window in fullscreen mode (true), or not
    \param maximized
      - whether to display Window in maximized mode, when not fullscreen
    \return
      - reference to the modified Window for easier subsequent inline actions
    */
    Window& Mode(bool fullscreen, bool maximized);


    /** Get the pointer to Viewport in use (if any), for window model settings
    \brief
      - get the pointer to Viewport in use (if any), for window model settings
    \details
      - can return nullptr if no Windowing package set up context properly
    \return
      - pointer to the contained Viewport in use controlling window model data
    */
    Viewport* Frame(void);


    /** Perform required steps to put the window in an open, running state
    \brief
      - perform required steps to put the window in an open, running state
    */
    void Open(void);


    /** Get whether the window is currently allowing resizing
    \brief
      - get whether the window is currently allowing resizing
    \return
      -whether the window is currently set to allow resizing
    */
    bool Resizable(void) const;


    /** Set whether the viewport is allowed to be dynamically resized by users
    \brief
      - set whether the viewport is allowed to be dynamically resized by users
    \param resizingAllowed
      - whether to allow the window to be manually resized by the user
    \return
      - reference to the modified Window for easier subsequent inline actions
    */
    Window& Resizable(bool resizingAllowed);


    /** Set Window render dimension scale within the window (not Window pixels)
    \brief
      - set Window render dimension scale within the window (not Window pixels)
    \param width
      - desired horizontal pixel resolution for the window coordinate system
    \param height
      - desired vertical pixel resolution for the window coordinate system
    \return
      - reference to the modified Window for easier subsequent inline actions
    */
    Window& Resolution(u2 width, u2 height);


    /** Put the window in a closed, uninitialized state (deallocate resources)
    \brief
      - put the window in a closed, uninitialized state (deallocate resources)
    */
    void Shutdown(void);


    /** Set Window pixel dimensions and render scale for the window together
    \brief
      - set Window pixel dimensions and render scale for the window together
    \param winWidth
      - desired pixel width for the Window to take up in the OS environment
    \param winHeight
      Desired pixel height for the Window to take up in the OS environment
    \return
      Reference to the modified Window for easier subsequent inline actions
    */
    Window& Size(u2 winWidth, u2 winHeight);


    /** Get current state enumeration value relating to whether window can run
    \brief
      - get current state enumeration value relating to whether window can run
    \return
      - state of the Window in initialization and running capability
    */
    WindowGen::STATUS Status(void) const;


    /** Get the Window's current label / name / title text string
    \brief
      - get the Window's current label / name / title text string
    \return
      - the current title text in use for labeling the Window
    */
    const std::string& Title(void) const;


    /** Set the title field for Window name in titlebar and taskbar
    \brief
      - set the title field for Window name in titlebar and taskbar
    \param titleField
      - text string to be displayed in the window titlebar and on the taskbar
    \return
      - reference to the modified Window for easier subsequent inline actions
    */
    Window& Title(const std::string& titleField);


    /** Get state relating to whether window can run
    \brief
      - get state relating to whether window can run
    \return
      - whether the Window is currently set to display the titlebar
    */
    bool Titlebar(void) const;


    /** Set whether the viewport is allowed to be dynamically resized by users
    \brief
      - set whether the viewport is allowed to be dynamically resized by users
    \param titleVisible
      - whether the Window should display the titlebar (if true), or hide it
    \return
      - reference to the modified Window for easier subsequent inline actions
    */
    Window& Titlebar(bool titleVisible);


    /** Perform cleanup of prior rendering GPU uploads before mesh removal
    \brief
      - perform cleanup of prior rendering GPU uploads before mesh removal
    \details
      - mesh will need VAO, VBO, EBO, etc, newly invalid upload values cleared
    \param meshData
      - uploaded mesh to clear off of GPU (non-const to have accessors cleared)
    */
    void UnloadMesh(Mesh& mesh) const;


    /** Get the pointer to the Renderer in use (if any), for draw settings
    \brief
      - get the pointer to the Renderer in use (if any), for draw settings
    \details
      - can return nullptr if no Windowing package could set up a renderer
    \return
      - pointer to contained Renderer in use perpetuating display of contents
    */
    Renderer* View(void);

    bool spin = true;

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Perform all render loop steps per frame
    \brief
      - perform all render loop steps per frame
    */
    void Update(void);


    /** Check changes in settings from previous frames, update internal settings
    \brief
      - check changes in settings from previous frames, update internal settings
    */
    void PollImGuiAttrs(void);

    /** Initialize resources to put Window in an open, running state for GLFW
    \brief
      - initialize resources to put Window in an open, running state for GLFW
    */
    void InitGLFW(void);

    
    /** Perform initialization for OpenGL setup in core profile, v4.3
    \brief
      - perform initialization for OpenGL setup in core profile, v4.3
    */
    void InitGL4GLFW(void);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Pointer to the window context's input
    Input*    windowInput = nullptr;

    //! Pointer to the window context's display
    Viewport* windowView = nullptr;

    //! Pointer to the window context's display
    Renderer* renderer = nullptr;

    //! Pointer to window context needing to be cast into windowing API's type
    void* windowContext = nullptr;

    //! State independent copy of desired window title display
    std::string windowTitle;

    //! Tracker that requirements for running operations are in a valid state
    WindowGen::STATUS winState = WindowGen::STATUS::DEFAULT;

    //! Current window mode state: windowed; fullscreen; maximized; fs-max
    WindowGen::MODE windowMode;

    //! Current window control level state: resizable; title bar; frame
    WindowGen::DECORATION windowStyle;

    //! Current window API in use
    WindowGen::PACKAGE windowPackage;

    //! Current rendering API in use
    WindowRen::PACKAGE renderPackage;

    //! Tracker for current windowed resolution settings
    WindowGen::Dimensions winRes;

    //! Current camera settings to use per object draw calls
    Camera activeView;

    // Point of an input is to pull the window's strings, by accessing locals
    friend class Input;
    friend class InputGLFW;

    // Let renderer change activeMaterial...awful but had coded scene is temp
    friend class RendererGLCore;

    //! Variable container which ImGui fields will edit, to be polled per tick
    GuiAttr UI;

    //! Scene test quad to render screenColor buffer onto
    Mesh* quad = nullptr;

    //! mesh display model to render skybox textures upon
    Mesh* cube = nullptr;

    //! Scene test object to center rotation about
    Mesh* centerObj = nullptr;

    //! Scene test orbital light position demarcation objects
    Mesh* orbitObj = nullptr;

    //! Test object's position / scale / rotation in world space
    Transform place;

    //! Render Quad's position / scale / rotation in world space
    Transform pane;

    //! Orbital light demarcation balls positions
    Transform ballOrbits[16];

    //! Amount by which rotation should take place on test object each frame
    ang rotRate = ang(20.0f, arc::degrees);

    //! Blank 1 x 1 pixel white texture data to maintain appearance of no image
    Texture* whtTex = nullptr;

    //! Texture for scene contents to be stored, rendered as texture on a quad
    Framebuffer* capTex = nullptr;

    //! Textrue for unadulterated background image of scene distance
    Cubemap* skyTex = nullptr;

    //! Test grid texture to illustrate UV coordinate regions on mesh
    Texture* uvTex = nullptr;

    //! Shader program for lambert material rendering
    Shader* lamShade = nullptr;

    //! Shader program for phong material rendering
    Shader* phnShade = nullptr;

    //! Shader program for blinn material rendering
    Shader* blnShade = nullptr;

    //! Shader program for distance shader material rendering
    Shader* skyShade = nullptr;

    //! Shader program for screen shader material rendering
    Shader* capShade = nullptr;

    //! Material attribute settings container for diffuse rendering
    Material* setMat = nullptr;

    //! Material attribute settings container for HUD/FBO rendering
    Skybox* skyMat = nullptr;

    //! Material attribute settings container for HUD/FBO rendering
    Capture* capMat = nullptr;

    //! Material attribute settings container for diffuse rendering
    Lambert* lamMat = nullptr;

    //! Material attribute settings container for photorealistic rendering
    Phong* phnMat = nullptr;

    //! Material attribute settings container for lightweight realistic render
    Blinn* blnMat = nullptr;

  }; // end Window class declaration

} // end dp namespace
