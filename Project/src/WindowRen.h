/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  WindowRen.h
Purpose:  Enums for windowing and rendering classes with discrete common terms
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no platform requirements)
Project:  a.Surprise_CS350_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Summer 2022
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// "./src/..."
#include "TypeErrata.h" // Platform primitive type aliases for easier porting
#include "vSpace.h"     // vector coordinate space enum / constant definitions


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{
  namespace WindowRen  //! Specifier to prevent conflicts from general terms
  {
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Package type enumeration for windowing softare in use
    enum class PACKAGE
    {
      UNDEFINED = -1, //! Virtual class error code: not to be instantiated
      //GL_SOFT,        // Using OpenGL protocols to ouput software rendering
      GL_CORE,        //! Modern OpenGL revision in use in core profile mode
      //DX_11,          // DirectX renderer in use with classic protocols
      //DX_VUL          // DirectX Vulkan renderer in use with modern overhead
      MAX_COUNT       //! Total count of valid windowing packages defined
    };  // end WindowRen::PACKAGE enum


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Structs                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! OpenGL / GLSL language revision in use for rendering calls
    struct OpenGLRevision
    {
      u2 major; //! Major (Major.x) OpenGL version number in use
      u2 minor; //! Minor (Rev.Minor) OpenGL version number in use
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Constants                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! GLSL 4.3 core profile mode is the only supported renderer for the class
    static const PACKAGE DEFAULT_RENDERER = PACKAGE::GL_CORE;

    //! Using GLSL 4.3: requires other GLAD packages to be installed to change
    static const OpenGLRevision GL_REV = { 4, 3 };

  } // end WindowRen namespace

} // end dp namespace
