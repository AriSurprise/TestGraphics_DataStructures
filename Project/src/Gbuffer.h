/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Gbuffer.h
Purpose:  Renderbuffer to hold geometry context for a deferred lighting pass
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
#include "Texture.h"    // Base class for derived class ot inherit

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                            Public Classes                                */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Gbuffer : public Framebuffer
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Consts                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Constructors                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a blank framebuffer with pixels initialized to the given values
    \brief
      - create a blank framebuffer with pixels initialized to the given values
    \details
      - color model determines which values of fill to use, and how to do so. 
      A mono-channel image need only sample the x component, and HDR values
      will be converted to the appropriate values per channel depth settings.
      Indexed color will have to assume an initial palette with the given fill
      for each possible value, but other modes use fill components in keeping
      with whatever channel order is specified: C(x), M(y), Y(z), K(w), or L(x)
      a(y), b(z), I(w), etc.
    \param fill
      - color to be used across all texture pixels (for the given color space),
      presumed 
    \param colorModel
      - color space and channel depth to set the pixels as using
    \param order
      - width, height and pixel read order flag options
    */
    Gbuffer(v4f fill = v4f(1.0f, 1.0f, 1.0f, 1.0f), u4 imageWidth = 1u,
      u4 imageHeight = 1u, ImageMode colorModel = ImageMode::Default);


    /** Load a texture from file to initialize a framebuffer state
    \brief
      - load a texture from file to initialize a framebuffer state
    \param file
      - validated path to the image file to be loaded
    */
    Gbuffer(const ASCIIPath& file);


    /** Create a framebuffer from another source texture image
    \brief
      - create a framebuffer from another source texture image
    \details
      - could take a Framebuffer too, but copy shouldn't have GPU state
    \param source
      - texture to use as a reference for this texture to copy the image of
    */
    Gbuffer(const Gbuffer& source);


    //! Destroy image GPU data prior to deletion
    ~Gbuffer(void);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Methods                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Operator Overloads                             */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set this texture as a copy from another source texture image
    \brief
      - set this texture as a copy from another source texture image
    \param source
      - texture to use as a reference for this texture to copy the image of
    \return
      - reference to the modified Gbuffer for easier subsequent inline actions
    */
    Gbuffer& operator=(const Gbuffer& source);

    //! Handle for GPU to utilize to determine load status and render location 
    u4 gUse;

    //! Handle for GPU to process specular <r,g,b> & intensity channels location 
    u4 gSpec;

    //! Handle for GPU to process diffuse <r,g,b> & ambient r channels location 
    u4 gDiff;

    //! Handle for GPU for incandescent <r,g,b> & ambient g channels location 
    u4 gGlow;

    //! Handle for GPU to process fresnel <r,g,b> & ambient b channels location 
    u4 gFres;

    //! Handle for GPU to process geometry coordinate data channels location 
    u4 gPos;

    //! Handle for GPU to process geometry normal data channels location 
    u4 gNorm;

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Protected Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  protected:

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Renderer Accessors                              */
    /* (storing GPU buffer return state of the relevant image being rendered) */

    //! Various VAO, VBO & EBO members exist only for Renderer access per Mesh
    friend class Renderer; // that should include the base class interface...
    friend class RendererGL4; // but derived classes don't inherit friend
    friend class Window;
    // TODO: list all other Renderers as implemented to access / load / unload


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  }; // end Gbuffer class declaration


  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                            Helper Constants                              */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                            Helper Functions                              */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Operator Overloads                          */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

} // end dp namespace
