/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Cubemap.cpp
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
#include "Log.h"      // Error / warning output message / logging
#include "Cubemap.h"  // Associated class declaration header 


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                Constructors                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create a cubemap with flat color mapped to each side's framebuffer         */ 
dp::Cubemap::Cubemap(const v4f& color, u4 width, u4 height,
  Texture::ImageMode colorSpace)
  : use(u4_MAX), proxy(nullptr)
{
  face[0] = Texture(color, width, height, colorSpace);
  face[1] = Texture(color, width, height, colorSpace);
  face[2] = Texture(color, width, height, colorSpace);
  face[3] = Texture(color, width, height, colorSpace);
  face[4] = Texture(color, width, height, colorSpace);
  face[5] = Texture(color, width, height, colorSpace);

} // end Cubemap(const v4f&, u4, u4, Texture::ImageMode)


// Create a cubemap with image files mapped to the textures on each side      */
dp::Cubemap::Cubemap(const ASCIIPath& right, const ASCIIPath& left,
  const ASCIIPath& top, const ASCIIPath& bottom, const ASCIIPath& back,
  const ASCIIPath& front)
  : use(u4_MAX), proxy(nullptr)
{
  face[0] = Texture(right, true);
  face[1] = Texture(left, true);
  face[2] = Texture(top, true);
  face[3] = Texture(bottom, true);
  face[4] = Texture(back, true);
  face[5] = Texture(front, true);

} // end Cubemap(Path&, Path&, Path&, Path&, Path&, Path&)


// Create a cubemap from an existing cubemap's framebuffer settings           */
dp::Cubemap::Cubemap(const Cubemap& source)
  : use(u4_MAX), proxy(nullptr)
{
  face[0] = source.face[0];
  face[1] = source.face[1];
  face[2] = source.face[2];
  face[3] = source.face[3];
  face[4] = source.face[4];
  face[5] = source.face[5];

} // end Cubemap(const Cubemap&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the appropriate subscript for the given API ordering convention        */
u4 dp::Cubemap::Face(u4 number, Order convention)
{
  if (number >= FACES) { number -= FACES * (number / FACES); }
  switch (convention)
  {
  case Order::OpenGL: default: return static_cast<u4>(number);
  }
} // end u4 Cubemap::Face(u4, Order)


// Get the appropriate cubemap side subscript for OpenGL side ordering        */
u4 dp::Cubemap::GLFace(u4 number) { return Face(number, Order::OpenGL); }


// Get whether the cubemap has been uploaded onto a Renderer                  */
bool dp::Cubemap::Loaded(void) const
{
  return (use != u4_MAX && proxy != nullptr);
} // end bool Cubemap::Loaded(void) const


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Operator Overloads                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Copy a cubemap's settings, while referencing the source's shader           */
dp::Cubemap& dp::Cubemap::operator=(const Cubemap& source)
{
  for (u4 i = 0; i < FACES; ++i) { face[i] = source.face[i]; }
  return *this;

} // end Cubemap& Cubemap::operator=(const Cubemap&)


// Access a side's texture from the cubemap for the given local subscript     */
const dp::Texture& dp::Cubemap::operator[](u4 localSubscript) const
{
  return face[GLFace(localSubscript)];

} // end Framebuffer& Cubemap::operator[](u4) const


// Access a side's texture from the cubemap for the given local subscript     */
dp::Texture& dp::Cubemap::operator[](u4 localSubscript)
{
  return face[GLFace(localSubscript)];

} // end Framebuffer& Cubemap::operator[](u4)
