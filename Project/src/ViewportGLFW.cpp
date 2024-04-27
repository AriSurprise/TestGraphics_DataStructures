/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  ViewportGLFW.cpp
Purpose:  GLFW Window model context implementation (in MVC decoupling)
Language: C++ / Eng, MSVC (used, but should work with anything GLFW supports)
Platform: MSVC / Windows 10 x64 (used; works on x64 machines supporting GLFW)
Project:  a.Surprise_CS300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <../Packages/include/...>
#include <glad\glad.h>    // GLAD OGL wrangler (for GL 4.3)
#include <GLFW\glfw3.h>   // GLFW windowing package
// <./src/...>
#include "ViewportGLFW.h" // Associated class declarations
#include "Log.h"          // Error / warning message output / logging

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Parameterized constructor with defaults by which to flexibly initialize    */
dp::ViewportGLFW::ViewportGLFW(GLFWwindow*& windowContext, u2 width,
  u2 height, const std::string& title, WindowGen::MODE windowingMode,
  WindowGen::DECORATION windowDecorationStyle)
  : Viewport(WindowGen::PACKAGE::GLFW, COORD2D::QUADRANT1, width,
    height, title, windowingMode, windowDecorationStyle), window(windowContext)
{
  // Attempt to perform initialization procedures on allocation; fewer steps
  if (!Init())
  {
    // If GLFW fails to initialize, that's unrecoverable for the moment
    Log::Error("ViewportGLFW - GLFW initialization failed");
    // TODO: throw to Window to fallback to another windowing API...?
  }
} // end ViewportGLFW(GLFWwindow*&, u2, u2, const string&, MODE, DECORATION)


// Cleanup any resources allocated for running procedures prior to deletion   */
dp::ViewportGLFW::~ViewportGLFW(void)
{ Shutdown(); } // end ~ViewportGLFW(void)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Parameterized constructor with default values to initialize by flexibly    */
dp::Viewport& dp::ViewportGLFW::DecorationMode(WindowGen::DECORATION
  windowDecoration, WindowGen::MODE windowMode)
{
  // Adjust glfw window context property settings for desired mode
  switch (windowDecoration)
  {
    // TODO: actually set window context properties to inputs
    // Present no window controls; disallow resizing
  case WindowGen::DECORATION::BARE:
    break;
    // Allow window to be resized with normal controls
  case WindowGen::DECORATION::RESIZABLE:
    break;
    // Present title bar without controls or resizing
  case WindowGen::DECORATION::FIXED:
    break;
    // Present no window controls; allow resizing
  case WindowGen::DECORATION::CLEAN:
    break;
  default: // Only other defined decorations are error / bookkeeping values
    // No error: no-op / pass through makes function more versatile
    break;
  }
  // Call window mode change functions
  switch (windowMode)
  {
  case WindowGen::MODE::WINDOWED:
    // Disable both fullscreen & maximized modes to set windowed mode
    FullscreenMode(false).MaximizedMode(false);
    break;
  case WindowGen::MODE::FULLSCREEN:
    // Enable fullscreen, but disable maximize mode to set fullscreen mode
    FullscreenMode(true).MaximizedMode(false);
    break;
  case WindowGen::MODE::MAXIMIZED:
    // Disable fullscreen, but enable maximized mode to set maximized mode
    FullscreenMode(false).MaximizedMode(true);
    break;
  case WindowGen::MODE::FSMAX:
    // Enable both fullscreen & maximized modes to set maximized fullscreen mode
    FullscreenMode(true).MaximizedMode(true);
    break;
  default: // Only other defined decorations are error / bookkeeping values
    // Error value defaulted in as pass-through functionality for no-op
    break;
  }
  return *this;
} // end Viewport& DecorationMode(WindowGen::DECORATION, MODE)


/** Set viewport pixel dimensions for window context (not render scale)       */
dp::Viewport& dp::ViewportGLFW::Dimensions(u2 winWidth, u2 winHeight)
{
  // TODO: set glfw window context to new window dimensions (resize)
  return *this;
} // end Viewport& Dimensions(u2, u2)


/** Set whether the viewport in or out of fullscreen mode                     */
dp::Viewport& dp::ViewportGLFW::FullscreenMode(bool fullscreenEnabled)
{
  fullscreen = fullscreenEnabled;
  // TODO: set glfw context to be in appropriate mode (just lower priority task)
  return *this;
} // end Viewport& FullscreenMode(bool)


/** Set up resources required for the GLFW window to run                      */
bool dp::ViewportGLFW::Init()
{
  // Set up that window for use
  glfwMakeContextCurrent(window); // Use this new window
  // Set internal resize callback event management; resize() should do nothing
  glfwSetFramebufferSizeCallback(window, resize_callback);

  // Initialize GLAD's OpenGL extensions for the revision for render use
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    Shutdown(); // Cleanup allocated resources
    runStatus = WindowGen::STATUS::UNDEFINED;
    Log::Error("ViewportGLFW::Init - GLAD initialization failed");
    return false; // GLAD initialization failed
  }

  runStatus = WindowGen::STATUS::READY;
  glViewport(0, 0, win.width, win.height); // sets OpenGL's device transform
  return true;
} // end bool Init(void)


/** Set viewport windowed mode to maximized (true), or windowed (false)       */
dp::Viewport& dp::ViewportGLFW::MaximizedMode(bool maximizedWindow)
{
  maximized = maximizedWindow;
  // TODO: set glfw context to be in appropriate maximized / windowed mode
  return *this;
} // end Viewport& MaximizedMode(bool)


/** Adjust dimensions on window resize events (as needed)                     */
void dp::ViewportGLFW::Resize(u2 winWidth, u2 winHeight)
{
  Viewport::Resize(winWidth, winHeight); // Still manage protected members
} // end void Resize(u2, u2)


/** Set viewport render dimensions (independent of window size)               */
dp::Viewport& dp::ViewportGLFW::Resolution(u2 winWidth, u2 winHeight)
{
  if (!(fullscreen || maximized))
  {
    // Update internal size trackers
    win.width = winWidth;
    win.height = winHeight;
    // Set viewport to match GLFW window (left, bottom, width, height) coords
    glViewport(0, 0, win.width, win.height); // set OGL internal device matrix
  }
  return *this;
} // end Viewport& Resolution(u2, u2)


/** Clean up resources allocated for the windowing package to run             */
void dp::ViewportGLFW::Shutdown(void)
{
  if (runStatus == WindowGen::STATUS::READY)
  {
    glfwTerminate(); // TODO: ImGui may be calling cleanup functions itself?
    // Note: does glfwTerminate just...know the active context to delete...?
    // what if there are multiple windows?
    runStatus = WindowGen::STATUS::DEFAULT;
  }
} // end void Shutdown(void)


/** Set the current text string in the Viewport window titlebar field         */
dp::Viewport& dp::ViewportGLFW::Title(const std::string& titleDisplay)
{
  windowTitle = titleDisplay;
  // TODO: update glfw window context's title
  return *this;
} // end Viewport& Title(const std::string&)


/** Update frame-step viewport changes (by a timeframe to be calculated)      */
void dp::ViewportGLFW::Update(void)
{
  // TODO: calculate dt from ticks (or compartmentalize to another header? meh)
  glfwSwapBuffers(window); // Draw the rendered things in the window
} // Viewport& Update(const std::string&)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/** GLFW callback to execute on window resize (by OS or user resize)          */
void dp::ViewportGLFW::resize_callback(GLFWwindow* window, s4 width,
  s4 height)
{
  // Set OpenGL viewport to match GLFW window in (left, bottom, width, height)
  glViewport(0, 0, width, height); // set OpenGL's internal device transform

} // end void resize_callback(GLFWwindow*, s4, s4)
