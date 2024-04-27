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

// "../Packages/..."
#include <stb_image.h>
// "./src/..."
#include "Log.h"      // Error / message logging management
#include "Renderer.h" // Call to unload the texture from destructor as needed
#include "Texture.h"  // Class declaration header file

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Protected Statics                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Make sure user must implicitly invoke stbi functions on image load by check
bool dp::Texture::stbiSet = false;

// 4 byte intervals for stride width is standard, not a given, hence non-const
u1 dp::Texture::scanlineAlignment = 4u;

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


// Create a blank texture with pixels set to the given values                 */
dp::Texture::Texture(v4f fill, u4 imageWidth, u4 imageHeight,
  ImageMode colorDepth)
  : channel(colorDepth), image(nullptr), path("./")
{
  width = (imageWidth == 0) ? 1 : imageWidth;
  height = (imageHeight == 0) ? 1 : imageHeight;
  pixelTotal = static_cast<u8>(width) * height;
  rowDataBytes = (static_cast<u8>(PixelSize()) * width);
  u4 padCompliment = rowDataBytes % scanlineAlignment;
  u4 padding = (padCompliment == 0) ? 0 : scanlineAlignment - padCompliment;
  strideBytes = rowDataBytes + padding;
  dataBytes = strideBytes * height;
  image = new u1[dataBytes];
  if (image)
  {
    // Switch statement allocation prohibition dictates using if / else branches
    if (ChannelSize() == 1.0f)
    {
      u4 channels = ChannelCount();
      if (channels == 0)
      {
        Log::Error("Texture::Texture - no image mode support implementation");
        return;
      }
      u1* pixel = new u1[channels];
      if (pixel)
      {
        if (channels > 0)
        {
          pixel[0] =
            static_cast<u1>(InF((fill.x * u1_MAX), 0.0f, 255.0f));
          if (channels > 1)
          {
            pixel[1] =
              static_cast<u1>(InF((fill.y * u1_MAX), 0.0f, 255.0f));
            if (channels > 2)
            {
              pixel[2] =
                static_cast<u1>(InF((fill.z * u1_MAX), 0.0f, 255.0f));
              if (channels > 3)
              {
                pixel[3] =
                  static_cast<u1>(InF((fill.w * u1_MAX), 0.0f, 255.0f));
              }
            }
          }
        }
        u1* current;
        for (u4 row = 0; row < Height(); ++row)
        {
          // Advance to head of current row
          u4 rowOffset = row * strideBytes;
          current = image + rowOffset;
          for (u4 col = 0; col < Width(); ++col)
          {
            // Set each pixel within the row
            for (u4 cChannel = 0; cChannel < channels; ++cChannel)
            {
              // Set each channel per appropriate arch-pixel channel value
              *current = pixel[cChannel];
              ++current;
            }
          }
        }
        delete[] pixel;
      }
      else
      {
        Log::Error("Texture::Texture - pixel allocation failed");
      }
    }
    if (ChannelSize() == 2.0f)
    {
      u4 channels = ChannelCount();
      if (channels == 0)
      {
        Log::Error("Texture::Texture - no image mode support implementation");
        return;
      }
      u2* pixel = new u2[channels];
      if (pixel)
      {
        if (channels > 0)
        {
          pixel[0] =
            static_cast<u2>(InF((fill.x * u2_MAX), 0.0f, 65355.0f));
          if (channels > 1)
          {
            pixel[1] =
              static_cast<u2>(InF((fill.y * u2_MAX), 0.0f, 65355.0f));
            if (channels > 2)
            {
              pixel[2] =
                static_cast<u2>(InF((fill.z * u2_MAX), 0.0f, 65355.0f));
              if (channels > 3)
              {
                pixel[3] =
                  static_cast<u2>(InF((fill.w * u2_MAX), 0.0f, 65355.0f));
              }
            }
          }
        }
        u2* current;
        for (u4 row = 0; row < Height(); ++row)
        {
          // Advance to head of current row
          u4 rowOffset = row * strideBytes;
          current = (u2*)(image + rowOffset);
          for (u4 col = 0; col < Width(); ++col)
          {
            // Set each pixel within the row
            for (u4 cChannel = 0; cChannel < channels; ++cChannel)
            {
              // Set each channel per appropriate arch-pixel channel value
              *current = pixel[cChannel];
              ++current;
            }
          }
        }
        delete[] pixel;
      }
      else
      {
        Log::Error("Texture::Texture - pixel allocation failed");
      }
    }
    else if (ChannelSize() == 4.0f)
    {
      u4 channels = ChannelCount();
      if (channels == 0)
      {
        Log::Error("Texture::Texture - no image mode support implementation");
        return;
      }
      f4* pixel = new f4[channels];
      if (pixel)
      {
        if (channels > 0)
        {
          pixel[0] = fill.x;
          if (channels > 1)
          {
            pixel[1] = fill.y;
            if (channels > 2)
            {
              pixel[2] = fill.z;
              if (channels > 3)
              {
                pixel[3] = fill.w;
              }
            }
          }
        }
        f4* current;
        for (u4 row = 0; row < Height(); ++row)
        {
          // Advance to head of current row
          u4 rowOffset = row * strideBytes;
          current = (f4*)(image + rowOffset);
          for (u4 col = 0; col < Width(); ++col)
          {
            // Set each pixel within the row
            for (u4 cChannel = 0; cChannel < channels; ++cChannel)
            {
              // Set each channel per appropriate arch-pixel channel value
              *current = pixel[cChannel];
              ++current;
            }
          }
        }
        delete[] pixel;
      }
      else
      {
        Log::Error("Texture::Texture - pixel allocation failed");
      }
    }
  }
  else
  {
    Log::Error("Texture::Texture - image allocation failed");
  }
} // end Texture::Texture(v4f, ImageMode, Dimensions)


// Load a texture from file (then convert to HDR RGBA as necessary)           */
dp::Texture::Texture(const ASCIIPath& file, bool topDown)
  : image(nullptr), path(file)
{
  //if (!stbiSet) { STBInit(); }
  stbi_set_flip_vertically_on_load(!topDown);
  std::string path(file);
  s4 w, h, ch;
  stbi_info(path.c_str(), &w, &h, &ch);
  width = static_cast<u4>(w);
  height = static_cast<u4>(h);
  pixelTotal = static_cast<u8>(width) * height;
  channel = ImageMode::RGBA;
  rowDataBytes = (static_cast<u8>(PixelSize()) * width);
  u8 padCompliment = rowDataBytes % scanlineAlignment;
  u8 padding = (padCompliment == 0) ? 0 : scanlineAlignment - padCompliment;
  strideBytes = rowDataBytes + (u4)padding;
  dataBytes = strideBytes * height;
  u1* load = stbi_load(path.c_str(), &w, &h, &ch, 4 /* promote to rgba */);
  if (!load)
  {
    Log::Warn("Texture(file) - \"" + path
      + "\" load failed: check the file; using default");
    *this = Texture();
  }
  else
  {
    // Create buffer to hold allocation to copy read in buffer
    image = new u1[dataBytes];
    if (image)
    {
      // Copy stbi read in contents into private local pixel buffer
      // (pass procedural / file allocations uniformly to new[] / delete[] op)
      for (u8 i = 0; i < dataBytes; ++i)
      {
        *(image + i) = *(load + i);
      }
      stbi_image_free((void*)load);
    }
    else
    {
      Log::Warn("Texture(file) - copy allocation failed; using default");
      stbi_image_free((void*)load);
      *this = Texture();
    }
  }
} // end Texture::Texture(const ASCIIPath&)


// Create a texture copy from another source texture image                    */
dp::Texture::Texture(const Texture& source)
  : channel(source.channel), dataBytes(source.dataBytes), width(source.width),
  height(source.height), rowDataBytes(source.rowDataBytes), pixelTotal
  (source.pixelTotal), strideBytes(source.strideBytes), path(source.path)
{
  image = new u1[dataBytes];
  if (image)
  {
    for (u4 i = 0; i < dataBytes; ++i) { image[i] = source.image[i]; }
  }
  else { Log::Warn("Texture(Texture&) - couldn't allocate"); }
} // end Texture::Texture(const Texture&)

// Destroy image heap data prior to deletion                                  */
dp::Texture::~Texture(void)
{
  if (this->Loaded()) { state.proxy->UnloadTexture(*this); }
  if (image) { delete[] image; }
} // end Texture::~Texture(void)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Public Methods                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the byte allotment per row of pixel image data (with padding bytes)    */
u8 dp::Texture::ByteSize(void) const
{ return dataBytes; } // end u4 Texture::ByteSize(void) const


// Get the count of channels for the given color space in use                 */
u1 dp::Texture::ChannelCount(void) const
{
  switch (channel)
  {
  case ImageMode::RGBA:
  case ImageMode::RGBA4: return 4u;
  default: return 0;
  }
} // end u4 Texture::ChannelCount(void) const


// Get the byte width per channel for the given color space in use            */
f4 dp::Texture::ChannelSize(void) const
{
  switch (channel)
  {
  case ImageMode::RGBA:     return 1.0f;
  case ImageMode::RGBA4:    return 4.0f;
  default: return 0;
  }
} // end u4 Texture::ChannelSize(void) const


// Get the file directory / name / extension used to load the image data      */
const dp::ASCIIPath& dp::Texture::FilePath(void) const
{ return path; } // end const ASCIIPath& Texture::FilePath(void) const


// Get the file directory / name / extension used to load the image data      */
dp::Texture& dp::Texture::FilePath(const ASCIIPath& file)
{
  *this = Texture(file);
  return *this;

} // end Texture& Texture::FilePath(const ASCIIPath&)


// Get the vertical count of pixels defined in the current image data         */
u4 dp::Texture::Height(void) const
{ return height; } // end u4 Texture::Height(void) const


// Get the address to the beginning of image data as read in 8-bit RGBA       */
u1* dp::Texture::RGBAData(void) const
{ return &image[0]; } // end u1* Texture::RGBAData(void) const


// Get whether the texture has been uploaded onto a Renderer                  */
bool dp::Texture::Loaded(void) const
{ return (state.proxy != nullptr); } // end bool Texture::Loaded(void) const


// Get the number of pixels (width * height) in the current image data        */
u8 dp::Texture::PixelCount(void) const
{ return pixelTotal; } // end u4 Texture::PixelCount(void) const


// Get the byte allotment per pixel in the image data                         */
f4 dp::Texture::PixelSize(void) const
{
  switch (channel)
  {
  case ImageMode::RGBA:     return 4.0f;
  case ImageMode::RGBA4:    return 16.0f;
  default: return 0;
  }
} // end u4 Texture::PixelSize(void) const


// Get the byte size of valid pixel data per row (without stride padding)     */
u8 dp::Texture::RowSize(void) const
{ return rowDataBytes; } // end u4 Texture::RowSize(void) const


// Get the byte allotment per row of pixel image data (with padding bytes)    */
u8 dp::Texture::Stride(void) const
{ return strideBytes; } // end u4 Texture::Stride(void) const


// Get the horizontal count of pixels defined in the current image data       */
u4 dp::Texture::Width(void) const
{ return width; } // end u4 Texture::Width(void) const


// Set this texture as a copy from another source texture image               */
dp::Texture& dp::Texture::operator=(const Texture& source)
{
  u1* temp = new u1[source.dataBytes];
  if (temp)
  {
    path = source.path;
    channel = source.channel;
    width = source.width;
    height = source.height;
    pixelTotal = source.pixelTotal;
    dataBytes = source.dataBytes;
    rowDataBytes = source.rowDataBytes;
    strideBytes = source.strideBytes;
    for (u8 i = 0; i < dataBytes; ++i) { temp[i] = source.image[i]; }
    if (image) { delete[] image; }
    image = temp;
    temp = nullptr;
  }
  else { Log::Warn("Texture::operator=(Texture&) - couldn't allocate"); }
  return *this;

} // end Texture::operator=(const Texture&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Protected Methods                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Perform library functions to get STBI usage in an expected state           */
void dp::Texture::STBInit(void)
{
  stbi_set_flip_vertically_on_load(true);
  stbiSet = true;

} // end void Texture::STBInit(void)

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                        Private Functions / Methods                         */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
