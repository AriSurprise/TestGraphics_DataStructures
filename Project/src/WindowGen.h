/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  WindowGen.h
Purpose:  Enums for windowing and rendering classes with discrete window terms
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no platform requirements)
Project:  a.Surprise_CS300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// "./src/..."
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{
  namespace WindowGen //! Specifier to prevent conflicts from general terms
  {
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    
    enum class STATUS //! Windowing data status for running capability
    {
      UNDEFINED = -1, //! Error state designation
      DEFAULT,        //! Default windowing package state
      READY,          //! Successful resource allocation state
      MAX_COUNT       //! Total valid window states
    };  // end WindowGen::STATUS enum

    
    enum class PACKAGE //! Package type enumeration for windowing softare in use
    {
      UNDEFINED = -1, //! Virtual class error code: not to be instantiated
      //WIN32,          // Low level windowing language
      GLFW,           //! GLFW windowing package in use
      //SDL,            // Simple Directmedia Layer 2 in use
      MAX_COUNT       //! Total count of valid windowing packages defined
    };  // end WindowGen::PACKAGE enum

    //! Windowing display mode in use: windowed, fullscreen, maximized, etc
    enum class MODE
    {
      UNDEFINED = -1, //! Window display mode unused / undefined / invalid
      WINDOWED,       //! Windowed display mode
      FULLSCREEN,     //! Fullscreen display mode
      MAXIMIZED,      //! Maximized window display mode
      FSMAX,          //! Fullscreen display mode; maximized set concurrently
      MAX_COUNT       //! Total count of window mode definitions
    };  // end WindowGen::MODE enum

    //! Window control flexibility heuristic setting (title bar, resize, etc)
    enum class DECORATION
    {
      UNDEFINED = -1, //! Window decoration mode unused / undefined / invalid
      RESIZABLE,      //! Standard window controls enabled
      FIXED,          //! Window controls enabled without resize
      CLEAN,          //! No visible controls but resizable
      BARE,           //! No window controls enabled
      MAX_COUNT       //! Total count of window decoration definitions
    };  // end WindowGen::DECORATION enum


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Structs                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    
    union Dimensions //! Window dimension container
    {
      struct
      {
        u2 width;   //! Pixel width (longhand notation)
        u2 height;  //! Pixel height (longhand notation)
      };
      struct
      {
        u2 w; //! Pixel width (shorthand notation)
        u2 h; //! Pixel height (shorthand notation)
      };
    }; // end Dimensions declaration


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Constants                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Lowest supported window resolution allowed: 320x240 should be DOS safe
    static const Dimensions WIN_MIN = { 320u, 240u };

    //! A default window resolution: 720p currently ubiquitous enough
    static const Dimensions DEFAULT_RES = { 1280u, 720u };

    //! Default windowing package to use in case of initialization failures
    static const PACKAGE DEFAULT_WINDOWER = PACKAGE::GLFW;

    //! Default windowing mode setting: windowed preferred for current uses
    static const MODE DEFAULT_WIN_MODE = MODE::WINDOWED;

    //! Default window decoration setting: bare preferred for current uses
    static const DECORATION DEFAULT_WIN_DECR = DECORATION::BARE;

  } // end WindowGen namespace

} // end dp namspace
