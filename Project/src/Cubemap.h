/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Cubemap.h
Purpose:  Set of 6 textures with which to render a cubemap / skybox
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
#include "Texture.h"  // Textures comprising cube faces
#include "Shader.h"   // Cubemap will need a shader program
#include "v3f.h"     // Generic cubemap input default color value
#include "v4f.h"     // Generic cubemap input parameter


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{ 

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Cubemap //! Shader parameter setting base class requiring type-casting
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Constants                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Quantity of faces in a cube (6) correlating to render target count
    static constexpr u4 FACES = 6;

    //! Explicit internal subscript association in OpenGL order for upload help 
    enum class Side
    {
      Right = 0,  //! Subscript 0 looking down to +X face: -Y up, -Z right
      Left = 1,   //! Subscript 1 looking down to -X face: -Y up, +Z right
      Top = 2,    //! Subscript 2 looking down to +Y face: +Z up, +X right
      Bottom = 3, //! Subscript 3 looking down to -Y face: -Z up, +X right
      Back = 4,   //! Subscript 4 looking down to +Z face: -Y up, +X right
      Front = 5   //! Subscript 5 looking down to -Z face: -Y up, -X right

    }; // end Side enum declaration

    //! Conventional face ordering expectation per API standards
    enum class Order
    {
      OpenGL    //! Face ordering in GL standards: +X, -X, +y, -Y, +Z, -Z

    }; // end Order enum declaration

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a cubemap with flat color mapped to each side's framebuffer
    \brief
      - create a cubemap with flat color mapped to each side's framebuffer
    \param color
      - RGBA value to use as flat initial color value per pixel per texture
    \param width
      - texture pixel width to use in each of cubemap 6 side's framebuffers
    \param height
      - texture pixel height to use in each of cubemap 6 side's framebuffers
    \param colorSpace
      - image mode to use for each cubemap side's framebuffer
    */
    Cubemap(const v4f& color = WHITE.Opaque(), u4 width = 1024, u4 height
      = 1024, Texture::ImageMode colorSpace = Texture::ImageMode::RGBA);

    /** Create a cubemap with image files mapped to the textures on each side
    \brief
      - create a cubemap with image files mapped to the textures on each side
    \param right
      - path to the image file for the +X | right face of the cubemap
    \param left
      - path to the image file for the -X | left face of the cubemap
    \param top
      - path to the image file for the +Y | top face of the cubemap
    \param bottom
      - path to the image file for the -Y | bottom face of the cubemap
    \param back
      - path to the image file for the +Z | back face of the cubemap
    \param front
      - path to the image file for the -Z | front face of the cubemap
    */
    Cubemap(const ASCIIPath& right, const ASCIIPath& left, const ASCIIPath& top,
      const ASCIIPath& bottom, const ASCIIPath& back, const ASCIIPath& front);

    /** Create a cubemap from an existing cubemap's framebuffer settings
    \brief
      - create a cubemap from an existing cubemap's framebuffer settings
    \param source
      - cubemap to have current side framebuffers copied into this
    */
    Cubemap(const Cubemap& source);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get the appropriate subscript for the given API ordering convention
    \brief
      - get the appropriate subscript for the given API ordering convention
    \param number
      - the expected subscript to access faces in sequence for the convention
    \param convention
      - API in use for which to categorically adhere to a known index ordering
    \return
      - the internal face index to match the given convention's input index
    */
    static u4 Face(u4 number, Order convention = Order::OpenGL);


    /** Get the appropriate cubemap side subscript for OpenGL side ordering
    \brief
      - get the appropriate cubemap side subscript for OpenGL side ordering
    \param number
      - 0:+X;right, 1:-X;left, 2:+Y;top, 3:-Y;bottom, 4:+Z;back, 5:-Z;front
    \return
      - the internal face index to match the OpenGL index (1 to 1 currently)
    */
    static u4 GLFace(u4 number);


    /** Get whether the cubemap has been uploaded onto a Renderer
    \brief
      - get whether the cubemap has been uploaded onto a Renderer
    \return
      - true only if Renderer having loaded the cubemap is set
    */
    bool Loaded(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Operator Overloads                             */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Copy a cubemap's settings, while referencing the source's shader
    \brief
      - copy a cubemap's settings, while referencing the source's shader
    \param source
      - material to have current attribute settings copied (per derived type)
    \return
      - reference to the modified material for easier subsequent inline action
    */
    Cubemap& operator=(const Cubemap& source);


    /** Access a side's texture from the cubemap for the given local subscript
    \brief
      - access a side's texture from the cubemap for the given local subscript
    \param localSubscript
      - index of cube sides: [0]left [1]right [2]top [3]bottom [4]back [5]front
    \return
      - Framebuffer corresponding to the cube face / side for the given index
    */
    const Texture& operator[](u4 localSubscript) const;


    /** Access a side's texture from the cubemap for the given local subscript
    \brief
      - access a side's texture from the cubemap for the given local subscript
    \param localSubscript
      - index of cube sides: [0]left [1]right [2]top [3]bottom [4]back [5]front
    \return
      - Framebuffer corresponding to the cube face / side for the given index
    */
    Texture& operator[](u4 localSubscript);

    //! Handle for GPU to utilize to determine load status and render location 
    u4 use;

    //! Renderer responsible for having loaded the cubemap to the GPU
    Renderer* proxy;



    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Renderer Accessors                              */
    /* (storing GPU buffer return state of the relevant image being rendered) */

    //! Various VAO, VBO & EBO members exist only for Renderer access per Mesh
    friend class Renderer; // that should include the base class interface...
    friend class RendererGL4; // but derived classes don't inherit friend
    friend class Window;
    // TODO: list all other Renderers as implemented to access / load / unload

    //! List of cube face textures in OpenGL ordering: +x, -x, +y, -y, +z, -z
    Texture face[FACES]; // [0]left [1]right [2]top [3]botm [4]back [5]front

  }; // end Cubemap class declaration

} // end dp namespace
