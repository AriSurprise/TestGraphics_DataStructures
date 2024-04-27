/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  Viewport.h
Purpose:  Window context model interface (in MVC decoupling from Input & Render)
Language: C++ / Eng, MSVC 19 (used; virtual base class to decouple dependencies)
Platform: MSVC 19 / Windows 10 x64 (used; virtual class shouldn't need anything)
Project:  a.Surprise_CS300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <stl>
#include <string>       // Windows have a title field, if for nothing else
// <./src/...>
#include "WindowGen.h"  // Windowing package enumerated distinctions
#include "WindowRen.h"  // Rendering package enumerated distinctions
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Viewport
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Constructor to enforce subclasses to define type for external casting
    \brief
      - constructor to enforce subclasses to define type for external casting
    \param type
      - derived classes must declare package enumeration for casting potential
    \param packageCoordSys
      - any API has associated coordinate system it works within: declare it
     for later reference capability; external handling (WindowGen lists types)
    \param width
      - desired pixel width of the Window Viewport (relative to OS scaling)
    \param height
      - desired pixel height of the Window Viewport (relative to OS scaling)
    \param titleDisplay
      - test string to be displayed on the titlebar of the Window and taskbar
    \param windowedMode
      - initial Viewport windowing mode setting: Windowed; Fullscreen; Max
    \param title
      - control style for the Viewport: show a titlebar...? allow resizing...?
    */
    Viewport(WindowGen::PACKAGE type, COORD2D packageCoordSys,
      u2 width, u2 height, const std::string& titleDisplay,
      WindowGen::MODE windowedMode, WindowGen::DECORATION decorationStyle);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Get current render aspect ratio (height / width), in use by Viewport
    \brief
      - get current render aspect ratio (height / width), in use by Viewport
    \return
      - float-point representation of the current aspect ratio (height/width)
    */
    f4 AspectRatio(void) const;


    /** Set viewport categorical Window decoration style: titlebar, resize, etc
    \brief
      - set viewport categorical Window decoration style: titlebar, resize, etc
    \details
      - derived class manages resizable and bordered state protected members, 
      also uses undefined MODE sentinel value to leave mode unchanged, or, to
      optionally set windowing mode concurrently if defined values are given
    \return
      - reference to the Viewport instance for easier subsequent inline actions
    */
    virtual Viewport& DecorationMode(WindowGen::DECORATION windowDecoration, 
      WindowGen::MODE windowMode = WindowGen::MODE::UNDEFINED) = 0;


    /** Get the coordinate system origin for the windowing package type in use
    \brief
      - get the coordinate system origin for the windowing package type in use
    \return
      - where coord system categorically places 0 point: corners, center, etc
    */
    COORD2D DeviceOrigin(void) const;


    /** Get the pixel size of the Viewport window in a 2D dimensions container
    \brief
      - get the pixel size of the Viewport window in a 2D dimensions container
    \return
      - container of viewport dimensions height and width data fields
    */
    const WindowGen::Dimensions& Dimensions(void) const;


    /** Set the window pixel measurements (apart from the render scale)
    \brief
      - set the window pixel measurements (apart from the render scale)
    \details
      - derived class manages win and view dimensions protected members,
      the former storing dimensions when the mode is windowed, while the latter
      tracks current window dimensions with mode in mind, which may affect
      screen coordinates differently depending on the windowing API managing it
      and notably should not overwrite the view dimensions in use if maximized,
      unless also changing out of maximized state (which should be consistent
      across all Viewport derived class implementations as well)
    \param winWidth
      - desired pixel measure of Viewport's windowed screen width (x-axis)
    \param winHeight
      - desired pixel measure of Viewport's windowed screen height (y-axis)
    \return
      - reference to the Viewport instance for easier subsequent inline actions
    */
    virtual Viewport& Dimensions(u2 winWidth, u2 winHeight) = 0;


    /** Get the current settings for pixel size of the Viewport when windowed
    \brief
      - get the current settings for pixel size of the Viewport when windowed
    \return
      - container of Viewport windowed height and width dimensions data fields
    */
    const WindowGen::Dimensions& DimensionsWindowed(void) const;


    /** Get whether the Viewport is currently set to use fullscreen mode
    \brief
      - get whether the Viewport is currently set to use fullscreen mode
    \return
      - true if viewport is currently set to use fullscreen mode, false if not
    */
    bool FullscreenMode(void) const;


    /** Set whether the Viewport is in or out of fullscreen mode
    \brief
      - set whether the Viewport is in or out of fullscreen mode
    \details
      - derived class manages fullscreen and maximized protected members,
      if exiting fullscreen, the last 'windowed' mode may have been maximized,
      so not a given return to windowed dimensions when disabled, but should
      change protected view dimensions in the process of changing any dimenions
      by way of sourcing stored window dimensions to return to windowed mode,
      or by using any API given functions with which view may be set with bounds
    \param fullscreenEnabled
      - sets Viewport to fullscreen if true, returns to windowed or max if false
    \return
      - reference to the Viewport instance for easier subsequent inline actions
    */
    virtual Viewport& FullscreenMode(bool fullscreenEnabled) = 0;


    /** Call API specific set up for resources for the windowing package to run
    \brief
      - call API specific set up for resources for the windowing package to run
    \return
      - true if initialization succeeded (false return may call fallbacks?)
    */
    virtual bool Init(void) = 0;


    /** Get whether the viewport is currently set to use maximized mode
    \brief
      - get whether the viewport is currently set to use maximized mode
    \return
      - true if Viewport is currently set to use mxaimized mode, false if not
    */
    bool MaximizedMode(void) const;


    /** Set whether the viewport window in or out of maximized mode
    \brief
      - Set whether the viewport window in or out of maximized mode
    \details
      - if fullscreen, fsmax state Window will not show maximized state change
    \param maximizedWindow
      - set Window to maximized / fsmax if true, windowed / fullscreen if false
    \return
      - reference to the Viewport instance for easier subsequent inline actions
    */
    virtual Viewport& MaximizedMode(bool maximizedWindow) = 0;


    /** Get which windowing package type enumeration is in use by the Viewport
    \brief
      - get which windowing package type enumeration is in use by the Viewport
    \return
      - enumerated value from WindowGen correlated to the windowing API in use
    */
    WindowGen::PACKAGE Package(void) const;


    /** Adjust viewport dimensions on window resize events as necessary
    \brief
      - adjust viewport dimensions on window resize events as necessary
    \details
      - manage derived classes shared members for standard housekeeping tasks.
      Current view bounds dimensions must be scrubbed from windowing package.
      Aspect ratio remains fixed on resize events sans resolution changes.
      Remember to use aspect & landscape / portrait to constrain viewport size
    \param winWidth
      - desired pixel width of the Window Viewport (relative to OS scaling)
    \param winHeight
      - desired pixel height of the Window Viewport (relative to OS scaling)
    */
    virtual void Resize(u2 winWidth, u2 winHeight);


    /** Set viewport render dimensions (independent of window size)
    \brief
      - set viewport render dimensions (independent of window size)
    \details
      - change Window device and world coordinate scale: not model size
    \param renWidth
      - desired pixels scaled to fit into Window Viewport width settings
    \param renHeight
      - desired pixel height of the Window Viewport (relative to OS scaling)
    \return
      - reference to the Viewport instance for easier subsequent inline actions
    */
    virtual Viewport& Resolution(u2 renWidth, u2 renHeight) = 0;


    /** Clean up resources allocated for the windowing package to run
    \brief
      - clean up resources allocated for the windowing package to run
    */
    virtual void Shutdown(void) = 0;


    /** Get enumerated state value relating to whether Viewport can run
    \brief
      - get enumerated state value relating to whether Viewport can run
    \return
      - enumerated value from WindowGen for running state code Viewport is in:
      UNDEFINED (error) | DEFAULT (uninitialized) | READY (ready / running)
    */
    WindowGen::STATUS Status(void) const;


    /** Get the current text string in use for the window title field
    \brief
      - get the current text string in use for the window title field
    \return
      - string representation of the viewport's title text
    */
    const std::string& Title(void) const;

    /** Set the current text string in the Viewport window titlebar field
    */
    virtual Viewport& Title(const std::string& titleDisplay) = 0;

    /** Update frame-step viewport changes (by a timeframe to be calculated)
    \brief
      - update frame-step viewport changes (by a timeframe to be calculated)
    */
    virtual void Update(void) = 0;

  protected:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Class Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Track whether window is in a running state
    WindowGen::STATUS runStatus;

    //! Window name as displayed in the title bar, etc.
    std::string windowTitle;

    //! Viewport's windowed pixel dimensions context, may differ from view size
    WindowGen::Dimensions win;

    //! Viewport's pixel dimensions in active use, considering window mode
    WindowGen::Dimensions view;

    //! Whether the viewport's title / window frame decorations should display
    bool bordered;

    //! Whether the user may dynamically resize the viewport's dimensions
    bool resizable;

    //! Whether the viewport's dimensions are currently in maximized mode
    bool maximized;

    //! Whether the viewport is currently in fullscreen mode
    bool fullscreen;

    //! Whether the viewport's render dimensions have width > height
    bool landscape;

    //! Aspect ratio for intended height / width ratio
    f4 aspect;

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Category of windowing package being used to draw: subclass enumeration
    WindowGen::PACKAGE windowLib;

    //! Categorical location type the windowing API uses for an origin
    COORD2D originDev;

  }; // end Viewport class interface

} // end dp namespace
