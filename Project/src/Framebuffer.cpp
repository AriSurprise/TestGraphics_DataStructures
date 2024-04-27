/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  Texture.cpp
Purpose:  2D raster image data to be rendered upon a polygon mesh surface
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_CS300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// "./src/..."
#include "Log.h"        // Error / message logging management 
#include "Renderer.h"   // Call to unload the framebuffer from GPU as needed 
#include "Framebuffer.h"// Class declaration header file 

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Protected Statics                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create a blank framebuffer with pixels initialized to the given values     */
dp::Framebuffer::Framebuffer(v4f fill, u4 imageWidth, u4 imageHeight,
  ImageMode colorModel)
  : Texture(fill, imageWidth, imageHeight, colorModel), use(u4_MAX),
  depthStencil(u4_MAX), proxy(nullptr)
{} // end Framebuffer(v4f, u4, u4, ImageMode)


// Load a texture from file to initialize a framebuffer state                 */
dp::Framebuffer::Framebuffer(const ASCIIPath& file)
  : Texture(file), use(u4_MAX), depthStencil(u4_MAX), proxy(nullptr)
{} // end Framebuffer(const ASCIIPath&)


// Create a framebuffer from another source texture image                     */
dp::Framebuffer::Framebuffer(const Texture& source)
  : Texture(source), use(u4_MAX), depthStencil(u4_MAX), proxy(nullptr)
{} // end Framebuffer(const Texture&)


// Destroy image GPU data prior to deletion                                   */
dp::Framebuffer::~Framebuffer(void)
{
  if (use != u4_MAX) { state.proxy->UnloadTarget(*this); }
} // end ~Framebuffer(void) 

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Public Methods                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Protected Methods                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Framebuffer& dp::Framebuffer::operator=(const Texture& source)
{
  // TODO: an existing texture copy should be reuploaded as a framebuffer
  Texture::operator=(source);
  proxy = source.state.proxy;
  use = 0;
  depthStencil = 0;
  return *this;

} // end Framebuffer& Framebuffer::operator=(const Texture&)                  */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                        Private Functions / Methods                         */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

