/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  ViewportGLFW.h
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

// <./src/...>
#include "TypeErrata.h" // Platform primitive type aliases for easier porting
#include "Viewport.h"   // Base class interface header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

struct GLFWwindow;

namespace dp //! Contents for use within class assignments
{
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Windowed media context management with GLFW (decoupled from Input)
  class ViewportGLFW : public Viewport
  {
  public:
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Parameterized constructor with defaults by which to flexibly initialize
    \brief
      - parameterized constructor with defaults by which to flexibly initialize
    \param windowContext
      - pointer to allocated GLFWwindow context for the window to use / own
    \param width
      - desired pixel width of the Window Viewport (relative to OS scaling)
    \param height
      - desired pixel height of the Window Viewport (relative to OS scaling)
    \param title
      - string of text to be displayed on titlebar of the Viewport and taskbar
    \param windowingMode
      - windowing mode for Viewport to start in (Windowed; Fullscreen; Max)
    \param windowedDecorationStyle
      - setting for window control style: showing the titlebar, resizablity
    */
    ViewportGLFW(GLFWwindow*& windowContext, u2 width =
      WindowGen::DEFAULT_RES.width, u2 height = WindowGen::DEFAULT_RES.height,
      const std::string& title = "", WindowGen::MODE windowingMode =
      WindowGen::DEFAULT_WIN_MODE, WindowGen::DECORATION windowDecorationStyle
      = WindowGen::DECORATION::RESIZABLE);


    /** Cleanup any resources allocated for running procedures prior to deletion
    \brief
      - cleanup any resources allocated for running procedures prior to deletion
    */
    ~ViewportGLFW(void);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Parameterized constructor with default values to initialize by flexibly
    \brief
      - parameterized constructor with default values to initialize by flexibly
    \param windowDecoration
      - display setting for window control style: showing the titlebar, resize
    \param windowMode
      - windowing mode for Viewport to start in (Windowed; Fullscreen; Max)
    \return
      - reference to the modified Viewport for easier subsequent inline changes
    */
    virtual Viewport& DecorationMode(WindowGen::DECORATION windowDecoration,
      WindowGen::MODE windowMode) override;


    /** Set viewport pixel dimensions for window context (not render scale)
    \brief
      - set viewport pixel dimensions for window context (not render scale)
    \param winWidth
      - desired pixel width of the Window Viewport (relative to OS scaling)
    \param winHeight
      - desired pixel height of the Window Viewport (relative to OS scaling)
    \return
      - reference to the modified Viewport for easier subsequent inline changes
    */
    virtual Viewport& Dimensions(u2 winWidth, u2 winHeight) override;


    /** Set whether the viewport in or out of fullscreen mode
    \brief
      - set whether the viewport in or out of fullscreen mode
    \param fullscreenEnabled
      - sets window to fullscreen mode if true, windowed / maximized if not
    \return
      - reference to the modified Viewport for easier subsequent inline changes
    */
    virtual Viewport& FullscreenMode(bool fullscreenEnabled) override;


    /** Set up resources required for the GLFW window to run
    \brief
      - set up resources required for the GLFW window to run
    \return
      - true if initialization succeeds, false if it fails in any way
    */
    virtual bool Init(void) override;


    /** Set viewport windowed mode to maximized (true), or windowed (false)
    \brief
      - set viewport windowed mode to maximized (true), or windowed (false)
    \details
      - if window is in fullscreen mode, windowed mode changes may not be visible
    \param maximizedWindow
      - sets the windowed mode to maximized if true, windowed if false
    \return
      - reference to the modified Viewport for easier subsequent inline changes
    */
    virtual Viewport& MaximizedMode(bool maximizedWindow) override;


    /** Adjust dimensions on window resize events (as needed)
    \brief
      - adjust dimensions on window resize events (as needed)
    \details
      - resize callback should preclude need for mapping resize event triggers
    \param winWidth
      - desired pixel width of the Window Viewport (relative to OS scaling)
    \param winHeight
      - desired pixel height of the Window Viewport (relative to OS scaling)
    */
    virtual void Resize(u2 winWidth, u2 winHeight) override;


    /** Set viewport render dimensions (independent of window size)
    \brief
      - set viewport render dimensions (independent of window size)
    \details
      - should call base Viewport class function first to manage protected members
    \param winWidth
      - desired pixel width of the Window Viewport (relative to OS scaling)
    \param winHeight
      - desired pixel height of the Window Viewport (relative to OS scaling)
    \return
      - reference to the modified Viewport for easier subsequent inline changes
    */
    virtual Viewport& Resolution(u2 winWidth, u2 winHeight) override;


    /** Clean up resources allocated for the windowing package to run
    \brief
      - clean up resources allocated for the windowing package to run
    */
    virtual void Shutdown(void) override;


    /** Set the current text string in the Viewport window titlebar field
    \brief
      - set the current text string in the Viewport window titlebar field
    \param title
      - string of text to display in viewport titlebar and taskbar as window name
    \return
      - reference to the modified Viewport for easier subsequent inline changes
    */
    virtual Viewport& Title(const std::string& titleDisplay) override;


    /** Update frame-step viewport changes (by a timeframe to be calculated)
    \brief
      - update frame-step viewport changes (by a timeframe to be calculated)
    */
    virtual void Update(void) override;

  private:
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** GLFW callback to execute on window resize (by OS or user resize)
    \brief
      - GLFW callback to execute on window resize (by OS or user resize)
    \details
      - set during init, once, as the callback designated for resize events
    \param window
      - GLFW window context pointer for the window to be manipulated
    \param width
      - desired pixel width of the Window Viewport (relative to OS scaling)
    \param height
      - desired pixel height of the Window Viewport (relative to OS scaling)
    */
    static void resize_callback(GLFWwindow* window, s4 width, s4 height);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Stored pointer to this window instance's GLFW window context
    GLFWwindow* window;

  }; // end ViewportGLFW class

} // end cs350 namespace
