/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Shader.cpp
Purpose:  Generic API agnostic container for shader files to be linked by GPU
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
#include "Log.h"        // Error / warning output message / logging
#include "Renderer.h"   // GPU removal prior to deletion
#include "Shader.h"     // Associated class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                Constructors                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create a composite shader using a set of vertex and fragment code files    */
dp::Shader::Shader(ShaderFile vertex, ShaderFile fragment,
  Rendering targetType)
  : handle(u4_MAX), vertPath(vertex.Path()), fragPath(fragment.Path()),
  proxy(nullptr), mode(Rendering::Forward)
{
  if (vertex.Type() == fragment.Type())
  {
    type = vertex.Type();
    vertContents = vertex.Code();
    fragContents = fragment.Code();
    name = (vertex.Name() == fragment.Name()) ? vertex.Name()
      : vertex.Name() + fragment.Name();
    std::vector<ShaderAttr> link;
    // Populate shared shader attribute lists
    for (u4 i = 0; i < vertex.Attrs().size(); ++i)
    {
      switch (vertex.Attrs()[i].AccessType())
      {
      case ShaderAttr::Qualifier::Input:
        input.push_back(vertex.Attrs()[i]);
        break;
      case ShaderAttr::Qualifier::Global:
        global.push_back(vertex.Attrs()[i]);
        break;
      case ShaderAttr::Qualifier::Output:
        // vertex output will link to fragment inputs, not program 'outputs'
        link.push_back(vertex.Attrs()[i]);
        break;
      default:
        Log::Warn("Shader - unrecognized attr '" + vertex.Attrs()[i].Text()
          + "' in " + vertex.Name());
        break;
      }
    }
    for (u4 i = 0; i < fragment.Attrs().size(); ++i)
    {
      bool notAlreadyIn = true;
      std::vector<ShaderAttr>* attrList = nullptr;
      switch (fragment.Attrs()[i].AccessType())
      {
      case ShaderAttr::Qualifier::Input:
        // fragment input = vertex output => check each linked attr for matches
        for (u4 i = 0; i < link.size(); ++i)
        {
          if (link[i].Text() == fragment.Attrs()[i].Text())
          {
            notAlreadyIn = false;
            break;
          }
        }
        if (notAlreadyIn)
        {
          //Log::Warn("Shader - no link for " + fragment.Attrs()[i].Text()
          //+ " from " + vertex.Name() + " to " + fragment.Name());
        }
        break;
      case ShaderAttr::Qualifier::Global: attrList = &global; break;
      case ShaderAttr::Qualifier::Output: attrList = &output; break;
      default:
        Log::Warn("Shader - unrecognized attr '" + fragment.Attrs()[i].Text()
          + "' in " + fragment.Name());
        break;
      }
      if (attrList)
      {
        for (u4 i = 0; i < input.size(); ++i)
        {
          if ((*attrList)[i].Text() == fragment.Attrs()[i].Text())
          {
            notAlreadyIn = false;
            break;
          }
        }
        if (notAlreadyIn) { attrList->push_back(fragment.Attrs()[i]); }
      }
    }
  }
  else
  {
    Log::Warn("Shader - file type mismatch between " + vertex.Name() + " & "
      + fragment.Name());
    type = ShaderFile::CodeType::Invalid;
  }
} // end Shader::Shader(ShaderFile, ShaderFile)


// Copy a shader based upon another's settings (shouldn't be necessary)       */
dp::Shader::Shader(const Shader& source)
  : proxy(nullptr), handle(u4_MAX), type(source.type), name(source.name),
  vertPath(source.vertPath), vertContents(source.vertContents),
  fragPath(source.fragPath), fragContents(source.fragContents),
  input(source.input), global(source.global), output(source.output),
  mode(source.mode)
{
  // Clear any erroneous dangling shader attribute pointer handle values
  for (u4 i = 0; i < input.size(); ++i) { input[i].GPUValue(u4_MAX); }
  for (u4 i = 0; i < global.size(); ++i) { global[i].GPUValue(u4_MAX); }
  for (u4 i = 0; i < output.size(); ++i) { output[i].GPUValue(u4_MAX); }

} // end Shader::Shader(const Shader&)
 

// Unload a loaded shader from the GPU in preparation for deletion            */
dp::Shader::~Shader(void)
{
  if (Loaded())
  {
    proxy->UnloadShader(*this);
  }
} // end Shader::~Shader(void)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the fragment shader code for debugging or upload onto the GPU          */
const char* dp::Shader::FragmentCode(void) const
{
  if (fragContents.empty() || fragContents == "") { return nullptr; }
  return fragContents.c_str();
} // end const char* Shader::FragmentCode(void) const


// Get the sanitized path string to access the fragment shader file           */
const std::string& dp::Shader::FragmentPath(void) const
{ return fragPath; } // end const std::string& Shader::FragmentPath(void) const


// Get a given shader global (uniform) for use in the shader program          */
dp::ShaderAttr& dp::Shader::Global(u4 subscript)
{
  if (subscript >= global.size())
  {
    subscript = (global.empty()) ? 0 : static_cast<u4>(global.size()) - 1;
  }
  return global[subscript];

} // end const ShaderAttr& Shader::Global(u4) const


// Get the list of shader globals (uniforms) found in the shader program      */
const std::vector<dp::ShaderAttr>& dp::Shader::Globals(void) const
{ return global; } // end const vector<ShaderAttr>& Shader::Globals(void) const


// Get a given shader input (attrib) for use in the shader program            */
const dp::ShaderAttr& dp::Shader::Input(u4 subscript) const
{
  if (subscript >= input.size())
  {
    subscript = (input.empty()) ? 0 : static_cast<u4>(input.size()) - 1;
  }
  return input[subscript];

} // end const ShaderAttr& Shader::Input(u4) const


// Get the list of shader inputs (attribs) found in the shader program        */
const std::vector<dp::ShaderAttr>& dp::Shader::Inputs(void) const
{ return input; } // end const vector<ShaderAttr>& Shader::Inputs(void) const


// Get whether the shader program has been successfully loaded for render     */
bool dp::Shader::Loaded(void) const
{
  return (proxy != nullptr && handle != u4_MAX);
} // end bool Shader::Loaded(void) const


// Get the enumerated shader program code language / format type in use       */
dp::ShaderFile::CodeType dp::Shader::Type(void) const
{ return type; } // end ShaderFile::CodeType Shader::Type(void) const


// Get the linked shader program display name as a string program label       */
const std::string& dp::Shader::Name(void) const
{ return name; } // end const std::string& Shader::Name(void) const


// Get a given shader output target from the shader program pipeline          */
const dp::ShaderAttr& dp::Shader::Output(u4 subscript) const
{
  if (subscript >= output.size())
  {
    subscript = (output.empty()) ? 0 : static_cast<u4>(output.size()) - 1;
  }
  return output[subscript];

} // end const ShaderAttr& Shader::Output(u4) const


// Get the list of shader outputs from the end of the shader pipeline         */
const std::vector<dp::ShaderAttr>& dp::Shader::Outputs(void) const
{ return output; } // end const vector<ShaderAttr>& Shader::Outputs(void) const


// Get the vertex shader code for debugging or upload onto the GPU            */
const char* dp::Shader::VertexCode(void) const
{
  if (vertContents.empty() || vertContents == "") { return nullptr; }
  return vertContents.c_str();
} // end const char* Shader::VertexCode(void) const


// Get the sanitized path string to access the vertex shader file             */
const std::string& dp::Shader::VertexPath(void) const
{ return vertPath; } // end const std::string& Shader::VertexPath(void) const


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Operator Overloads                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set a shader based upon another's settings (CPU side; GPU aside)           */
dp::Shader& dp::Shader::operator=(const Shader& source)
{
  proxy = nullptr;
  handle = u4_MAX;
  type = source.type;
  name = source.name;
  vertPath = source.vertPath;
  vertContents = source.vertContents;
  fragPath = source.fragPath;
  fragContents = source.fragContents;
  input.clear();
  for (const ShaderAttr& i : source.input)
  {
    input.push_back(ShaderAttr(i.Text(), i.AccessType(), i.DataClass()));
  }
  global.clear();
  for (const ShaderAttr& i : source.global)
  {
    input.push_back(ShaderAttr(i.Text(), i.AccessType(), i.DataClass()));
  }
  output.clear();
  for (const ShaderAttr& i : source.output)
  {
    input.push_back(ShaderAttr(i.Text(), i.AccessType(), i.DataClass()));
  }
  return *this;

} // end Shader& Shader::operator=(const Shader&)
