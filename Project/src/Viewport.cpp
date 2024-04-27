/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  Viewport.cpp
Purpose:  Window context model interface (in MVC decoupling from Input & Render)
Language: C++ / Eng, MSVC (used; virtual base class is to decouple language)
Platform: MSVC / Windows 10 (used; virtual base class is to broaden platforms)
Project:  a.Surprise_CS300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <./src/...>
#include "Viewport.h"

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Constructor to enforce subclasses to define type for external casting      */
dp::Viewport::Viewport(WindowGen::PACKAGE type, COORD2D packageCoordSys,
  u2 width, u2 height, const std::string& titleDisplay,
  WindowGen::MODE windowedMode, WindowGen::DECORATION decorationStyle)
  : windowLib(type), originDev(packageCoordSys),
    runStatus(WindowGen::STATUS::DEFAULT), windowTitle(titleDisplay)
{
  win.width = (width > WindowGen::WIN_MIN.width) ?
    width : WindowGen::WIN_MIN.width;
  win.height = (height > WindowGen::WIN_MIN.height) ?
    height : WindowGen::WIN_MIN.height;
  switch (windowedMode)
  {
  case WindowGen::MODE::FULLSCREEN:
    view.width = WindowGen::WIN_MIN.width;
    view.height = WindowGen::WIN_MIN.height;
    fullscreen = true;
    maximized = false;
    break;
  case WindowGen::MODE::MAXIMIZED:
    view.width = WindowGen::WIN_MIN.width;
    view.height = WindowGen::WIN_MIN.height;
    fullscreen = false;
    maximized = true;
    break;
  case WindowGen::MODE::FSMAX:
    view.width = WindowGen::WIN_MIN.width;
    view.height = WindowGen::WIN_MIN.height;
    fullscreen = true;
    maximized = true;
    break;
  case WindowGen::MODE::WINDOWED: default:
    view.width = win.width;
    view.height = win.height;
    fullscreen = false;
    maximized = false;
    break;
  }
  landscape = (win.width > win.height) ? true : false;
  aspect = static_cast<float>(win.height) / static_cast<float>(win.width);
  switch (decorationStyle)
  {
  case WindowGen::DECORATION::FIXED:
    bordered = true; resizable = false;
    break;
  case WindowGen::DECORATION::CLEAN:
    bordered = false; resizable = true;
    break;
  case WindowGen::DECORATION::RESIZABLE:
    bordered = true; resizable = true;
    break;
  case WindowGen::DECORATION::BARE: default:
    bordered = false; resizable = false;
    break;
  }
} // end Viewport(PACKAGE, COORDS, u2, u2, string&, MODE, DECORATION)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get current render aspect ratio (height / width), in use by the Viewport   */
f4 dp::Viewport::AspectRatio(void) const
{ return aspect; } // end f4 AspectRatio(void) const


// Get the coordinate system origin for the windowing package type in use     */
dp::COORD2D dp::Viewport::DeviceOrigin(void) const
{ return originDev; } // end WindowRen::COORDS DeviceOrigin(void) const


// Get the pixel size of the Viewport window in a dimensions container        */
const dp::WindowGen::Dimensions& dp::Viewport::Dimensions(void) const
{
  if (fullscreen || maximized) { return view; }
  else { return win; }
} // end const WindowGen::Dimensions& Dimensions(void) const


// Get the current settings for pixel size of the Viewport when windowed      */
const dp::WindowGen::Dimensions& dp::Viewport::DimensionsWindowed(void)
  const { return win; } // end WindowGen::Dimensions& DimensionsWindowed(void)


// Get whether the viewport is currently set to use fullscreen mode           */
bool dp::Viewport::FullscreenMode(void) const
{ return fullscreen; } // end bool FullscreenMode(void) const


// Get whether the viewport is currently set to use maximized mode            */
bool dp::Viewport::MaximizedMode(void) const
{ return maximized; } // end bool MaximizedMode(void) const


// Get which windowing package type enumeration is in use by the Viewport     */
dp::WindowGen::PACKAGE dp::Viewport::Package(void) const
{ return windowLib; } // end WindowGen::PACKAGE Package(void) const


// Adjust viewport dimensions on window resize events as necessary            */
void dp::Viewport::Resize(u2 winWidth, u2 winHeight)
{ win.h = winHeight; win.w = winWidth; } // end void Resize(u2, u2)


// Get enumerated state value relating to whether Viewport can run            */
dp::WindowGen::STATUS dp::Viewport::Status(void) const
{ return runStatus; } // end WindowGen::STATUS Status(void) const


// Get the current text string in use for the window title field              */
const std::string& dp::Viewport::Title(void) const
{ return windowTitle; } // end const std::string& Title(void) const
