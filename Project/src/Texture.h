/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Texture.h
Purpose:  2D raster image data to be rendered upon a polygon mesh surface
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
#include "ASCIIPath.h"  // Load image from file path parameter
#include "TypeErrata.h" // Hardware independent type aliases
#include "v4f.h"       // Fill color parameter for constructor

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  class Renderer;

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                            Public Classes                                */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Texture
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Consts                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Possible image channel color space / bit-depth combinations
    \brief
      - possible image channel color space / bit-depth combinations
    \details
      - several things are worth considering as to whether to keep a single
      enum or a pair of values for color space and bit / byte depth separately. 
      while querying separate values is more concise in some regards, as long
      as there are less than 256 values in the possible products, it uses
      less disk space to have one, making it more concise in other capacities. 
      for most conventional color spaces all bit depths are aligned to even
      byte width sizes, but some verybasic bitmaps have less than a byte per
      pixel (largely why channel bit-width is more common than byte-width),
      256 values being a confining and limited format for editing, indexed
      color modes using it for a palette for every color in the image is also
      marginally limiting, while a 9th or even 10th bit would allow 2 or 4 as
      many values / palette swithout expanding to full true color of RGBA
      values which would be far less constricting, but would require pixel
      groups to be considered at once to align to byte widths for compact
      addressal by pallete enumerations, which would be far more useful, but
      also necessitates being considered in tandem with color mode to allow. 
      just supporting indexed color mode supported at all is sufficient such
      that a pair of values could have invalid color space / depth pairings. 
      ultimately for these justifications, it was deemed that a conjoined
      enum of both being evaluated at once would mitigate the latter concern, 
      while also allowing for less redundant conjoined switch evaluations of
      the pairing for the overall pixel byte size. separately a reasonable
      result would need far more caveats, while a single switch can utilize
      pass through behavior for the vast majority, and have the exceptional
      pairings evaluate an appropriate size for the group of pixels which are
      byte aligned as a struct, allowing the measure to stay on bytes not bits
      so long as the byte count remains a unique sentinel value signifying it. 
      the unified enum value is far simpler to deal with for such special cases.
    */
    enum class ImageMode
    { // Uncomment modes as support for them is added
      //Index,    //! Enumerated 8-bit channel substitutions to a palette file
      //Index9,   //! Enumerated 9-bit channel substitutions to a palette file
      //Index9b,  //! Enumerated byte aligned 9-bit channel mapped to a palette
      //Index10,  //! Enumerated 10-bit channel substitutions to a palette file
      //Index10b, //! Enumerated byte aligned 10-bit palette mapped channel
      //GS,       //! Greyscale / Linear ramp of 8-bit mono-channel used
      //GS2,      //! Greyscale / Linear ramp of 16-bit mono-channel used
      //GS4,      //! Greyscale / Linear ramp of 32-bit mono-channel used
      //RGB,      //! Red, Green & Blue 8-bit channels for monitor output
      //RGB2,     //! Red, Green & Blue 16-bit channels for monitor output
      //RGB4,     //! Red, Green & Blue 32-bit channels for monitor output
      //HSL,      //! Hue, Chroma & Light 8-bit channels bicone color model
      //HSL2,     //! Hue, Chroma & Light 16-bit channels bicone color model
      //HSL4,     //! Hue, Chroma & Light 32-bit channels bicone color model
      //HSV,      //! Hue, Chroma & Value 8-bit channels cone color model
      //HSV2,     //! Hue, Chroma & Value 16-bit channels cone color model
      //HSV4,     //! Hue, Chroma & Value 32-bit channels cone color model
      //HSD,      //! Hue, Chroma & Dark cone 8-bit channels (HSV inverse)
      //HSD2,     //! Hue, Chroma & Dark cone 16-bit channels (HSV inverse)
      //HSD4,     //! Hue, Chroma & Dark cone 32-bit channels (HSV inverse)
      //LAB,      //! Luminance, a (r/g) & b (b/y) 8-bit chroma channels
      //LAB2,     //! Luminance, a (r/g) & b (b/y) 16-bit chroma channels
      //LAB4,     //! Luminance, a (r/g) & b (b/y) 32-bit chroma channels
      RGBA,     //! Red, Green, Blue & Alpha 8-bit channels for display
      //RGBA2,    //! Red, Green, Blue & Alpha 16-bit channels for display
      RGBA4,    //! Red, Green, Blue & Alpha 32-bit channels for display
      //HSLA,     //! Hue, Chroma, Light & Alpha 8-bit channels
      //HSLA2,    //! Hue, Chroma, Light & Alpha 16-bit channels
      //HSLA4,    //! Hue, Chroma, Light & Alpha 32-bit channels
      //HSVA,     //! Hue, Chroma, Value & Alpha 8-bit channels
      //HSVA2,    //! Hue, Chroma, Value & Alpha 16-bit channels
      //HSVA4,    //! Hue, Chroma, Value & Alpha 32-bit channels
      //HSDA,     //! Hue, Chroma, Dark & Alpha 8-bit channels
      //HSDA2,    //! Hue, Chroma, Dark & Alpha 16-bit channels
      //HSDA4,    //! Hue, Chroma, Dark & Alpha 32-bit channels
      //CMYK,     //! Cyan, Magenta, Yellow & Key 8-bit channels for print
      //CMYK2,    //! Cyan, Magenta, Yellow & Key 16-bit channels for print
      //CMYK4,    //! Cyan, Magenta, Yellow & Key 32-bit channels for print
      //LABI,     //! Luminance, a / b chroma & Incidence 8-bit channels
      //LABI2,    //! Luminance, a / b chroma & Incidence 16-bit channels
      //LABI4,    //! Luminance, a / b chroma & Incidence 32-bit channels
      Default = RGBA, //! Image mode to use as a default value (RGBA)
    }; // end Texture::ImageMode enum declaration


    /** Vertex Object Renderer value container for recalling state when drawing
    \class Texture::RenderBufferObjects
    \brief
      - vertex Object Renderer value container for recalling state when drawing
    \details
      - keeping the vertex object state records in the textuer allows it to
      be the owner to manage it's own GPU state updates as any texture changes
      come about.  Agnostic u4s will require casting for the renderer, but can
      be used by any renderer, and the storing / passing of Texture parameters
      for more intuitive external access by passing const references
    */
    class RenderBufferObjects
    {
    public:

      //! Texture Buffer Object for referring to this texture uploaded on GPU
      u4 image = u4_MAX;

      //! Pointer to Renderer which uploaded the mesh on GPU (will also unload)
      Renderer* proxy = nullptr;

    }; // end Texture::RenderBufferObjects class declaration


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Constructors                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a blank texture with pixels set to the given values
    \brief
      - create a blank texture with pixels set to the given values
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
    Texture(v4f fill = v4f(1.0f, 1.0f, 1.0f, 1.0f), u4 imageWidth = 1u,
      u4 imageHeight = 1u, ImageMode colorModel = ImageMode::Default);


    /** Load a texture from file (then convert to HDR RGBA as necessary)
    \brief
      - load a texture from file (then convert to HDR RGBA as necessary)
    \param file
      - validated path to the image file to be loaded
    \param topDown
      - true if pixel ordering should be read vertically in top down ordering
    */
    Texture(const ASCIIPath& file, bool topDown = false);


    /** Create a texture copy from another source texture image
    \brief
      - create a texture copy from another source texture image
    \param source
      - texture to use as a reference for this texture to copy the image of
    */
    Texture(const Texture& source);

    //! Destroy image heap data prior to deletion
    ~Texture(void);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Methods                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Get the count of bytes used across the full image buffer
    \brief
      - get the count of bytes used across the full image buffer
    \return
      - byte count used to hold the image: stride * height
    */
    u8 ByteSize(void) const;

    /** Get the count of channels for the given color space in use
    \brief
      - get the count of channels for the given color space in use
    \return
      - value correlating to channels defined for the current image mode
    */
    u1 ChannelCount(void) const;


    /** Get the byte width per channel for the given color space in use
    \brief
      - get the byte width per channel for the given color space in use
    \details
      - channels may not be in even bytes, hence float return value for bits
    \return
      - value correlating to bytes per channel for the current image mode
    */
    f4 ChannelSize(void) const;


    /** Get the file directory / name / extension used to load the image data
    \brief
      - get the file directory / name / extension used to load the image data
    \return
      - string validator / wrapper / parser object for the file used (if any)
    */
    const ASCIIPath& FilePath(void) const;


    /** Load new image data and set the texture to the contained image
    \brief
      - load new image data and set the texture to the contained image
    \return
      - string validator / wrapper / parser object for the file used (if any)
    */
    Texture& FilePath(const ASCIIPath& file);


    /** Get the vertical count of pixels defined in the current image data
    \brief
      - get the vertical count of pixels defined in the current image data
    \return
      - vertical measurement / pixel count of the image dimensions
    */
    u4 Height(void) const;


    /** Get the address to the beginning of image data as read in 8-bit RGBA
    \brief
      - get the address to the beginning of image data as read in 8-bit RGBA
    \return
      - pointer to the address of the first channel of the first pixel in image
    */
    u1* RGBAData(void) const;


    /** Get whether the texture has been uploaded onto a Renderer
    \brief
      - get whether the texture has been uploaded onto a Renderer
    \return
      - true only if Renderer having loaded the texture is any non-null value
    */
    bool Loaded(void) const;


    /** Get the number of pixels (width * height) in the current image data
    \brief
      - get the number of pixels (width * height) in the current image data
    \return
      - image width * height
    */
    u8 PixelCount(void) const;


    /** Get pixel byte size for the given image mode settings
    \brief
      - get pixel byte size for the given image mode settings
    \details
      - channels may not be in even bytes, hence float return value for bits
    \return
     - product of ChannelCount * ChannelSize constituting the bytes of 1 pixel
    */
    f4 PixelSize(void) const;


    /** Get the byte size of valid pixel data per row (without stride padding)
    \brief
      - get the byte size of valid pixel data per row (without stride padding)
    \return
      - pixel size * width of the image (not the full stride / scanline size)
    */
    u8 RowSize(void) const;


    /** Get the byte allotment per row of pixel image data (with padding bytes)
    \brief
      - get the byte allotment per row of pixel image data (with padding bytes)
    \return
      - pixel size * width + padding bytes to scanline alignment interval
    */
    u8 Stride(void) const;


    /** Get the horizontal count of pixels defined in the current image data
    \brief
      - get the horizontal count of pixels defined in the current image data
    \return
      - horizontal measurement / pixel count of the image dimensions
    */
    u4 Width(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Operator Overloads                             */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set this texture as a copy from another source texture image
    \brief
      - set this texture as a copy from another source texture image
    \param source
      - texture to use as a reference for this texture to copy the image of
    */
    Texture& operator=(const Texture& source);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Protected Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! State records of texture GPU buffer accessor value and loading renderer
    RenderBufferObjects state;

  protected:

    /** Perform library functions to get STBI usage in an expected state
    \brief
      - perform library functions to get STBI usage in an expected state
    \details
      - package presumes coordinates being bottom up, which needs correction
    */
    static void STBInit(void);

    //! Flag for STBI initialization having been performed already
    static bool stbiSet;

    //! Size which stride width must align to byte intervals of per scanline
    static u1 scanlineAlignment;

    //! Context for settings to use in reading image data from buffer
    ImageMode channel;

    //! Image x value; pixel resolution in the horizontal axis
    u4 width;

    //! Image y value; pixel resolution in the vertical axis
    u4 height;

    //! Total pixel resolution in the whole of the image: width * height
    u4 pixelTotal;

    //! byte size of valid pixel data in any given image row
    u4 rowDataBytes;

    //! byte size of any given image row, with padding bytes
    u4 strideBytes;

    //! byte size of entire image data buffer
    u4 dataBytes;

    //! Pointer to memory location for image data buffer to start
    u1* image;

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

    ASCIIPath path;

  }; // end Texture class declaration


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
