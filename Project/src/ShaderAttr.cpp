/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  ShaderFile.cpp
Purpose:  Generic API agnostic container for text input file to be read to GPU
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
#include "ASCIIValue.h" // TokenList parser of shader text line
#include "Log.h"        // Error / warning output message / logging
#include "ShaderAttr.h" // Associated class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                Constructors                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::ShaderAttr::ShaderAttr(std::string shaderTextLine, CodeLang shaderLang)
  : access(Qualifier::Unknown), query(s4_MAX), type(ClassType::Undefined)
{
  std::vector<std::string> tokens = TokenList(shaderTextLine, ' ');
  if (shaderLang == CodeLang::GLSL43)
  {
    if (tokens.size() > 2) // at least enough to scan, maybe comments after...
    {
      bool layoutForm = false;
      Qualifier readQ = Qualifier::Unknown; // qualifier (if valid)
      ClassType readC = ClassType::Undefined; // class (if valid)
      if (tokens[0] == "uniform") { readQ = Qualifier::Global; }
      else if (tokens[0] == "in") { readQ = Qualifier::Input; }
      else if (tokens[0] == "layout" && (tokens[1] == "(location"
        && tokens[2] == "=") || (tokens[1] == "(" && tokens[1] == "location"))
      {
        layoutForm = true; readQ = Qualifier::Input;
      }
      else if (tokens[0] == "out") { readQ = Qualifier::Output; }
      if (layoutForm && tokens.size() > 3) // further validate / extract
      {
        std::string valStr;
        size_t n = (tokens[3] == "=" && tokens.size() > 3) ? 4 : 3;
        if (tokens[n][tokens[n].length() - 1] == ')')
        {
          valStr = tokens[n].substr(tokens[n].length() - 1);
        }
        else { valStr = tokens[n]; }
        if (IsUnsignedD(valStr))
        {
          query = ToUnsigned(valStr);
          if (tokens.size() > n && tokens[n + 1] == ")") { ++n; }
          if (tokens.size() > n + 2 && tokens[n] != "//") // inline in variable
          {
            if (tokens[n] != "in") { return; }
            ClassType readC = GLSLTokenType(tokens[n + 1]);
            if (readC != ClassType::Undefined)
            {
              if (tokens[n + 2][tokens[n + 2].size() - 1] == ';')
              {
                identifier = tokens[n + 2].substr(0, tokens[n + 2].size() - 1);
              }
              else { identifier = tokens[n + 2]; }
              type = readC;
            }
            else { readQ = Qualifier::Unknown; }
          }
          else // need next line in tandem scan...requiring more input data
          {
            Log::Note("ShaderFile::Attr - multi line attribute declaration", 0);
            Log::N(" possible: AddContext required with additional code", 1);
            access = readQ;
            return;
          } // let shader detect & solve issue; finish attr construction part
        }
        else { readQ = Qualifier::Unknown; }
      }
      if (readQ == Qualifier::Unknown) { return; } // if a known, proceed
      else { readC = GLSLTokenType(tokens[1]); }
      if (readC != ClassType::Undefined)
      {
        if (tokens[2][tokens[2].size() - 1] == ';')
        {
          identifier = tokens[2].substr(0, tokens[2].size() - 1); // prune ';'
        }
        else { identifier = tokens[2]; }
        access = readQ;
        type = readC;
      } // end valid data type detected check
    } // end at least 3 tokens in code line check
  } // end CodeLang::GLSL43 validation
  else
  {
    Log::Warn("ShaderAttr - given shader language unsupported");
  }
} // end ShaderAttr::ShaderAttr(std::string, CodeLang)

// Create a shader attribute from the given string  / value elements          */
dp::ShaderAttr::ShaderAttr(std::string identifierText, Qualifier AttrType,
  ClassType className, s4 gpuVal)
  : query(gpuVal), access(AttrType), type(className),
  identifier(identifierText)
{ } // end ShaderAttr::ShaderAttr(std::string, Qualifier, ClassType, s4)

// Create a shader attribute from a validated shader attribute                */
dp::ShaderAttr::ShaderAttr(const ShaderAttr& attribute)
  : query(-1), access(attribute.access), type(attribute.type),
  identifier(attribute.identifier)
{ } // end ShaderAttr::ShaderAttr(const ShaderAttr&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get access category uniform / global, input /attribute, out, etc           */
dp::ShaderAttr::Qualifier dp::ShaderAttr::AccessType(void) const
{ return access; } // end ShaderAttr::Qualifier ShaderAttr::AccessType() const


// Get data type of integer, unsigned, float, v2f|3|4, mat2|3|4, etc         */
dp::ShaderAttr::ClassType dp::ShaderAttr::DataClass(void) const
{ return type; } // end ShaderAttr::Qualifier ShaderAttr::DataClass(void) const


// Set value to be retrieved in API calls to reference access of this attribute    */
dp::ShaderAttr& dp::ShaderAttr::GPUValue(s4 lookupFromGPU)
{
  query = lookupFromGPU;
  return *this;

} // end ShaderAttr& ShaderAttr::GPUValue(s4) const


// Get value to be used in API calls to reference access of this attribute    */
s4 dp::ShaderAttr::Handle(void) const
{ return query; } // end s4 ShaderAttr::Handle(void) const


// Get text read in from shader program code as this attribute's identifier   */
const std::string& dp::ShaderAttr::Text(void) const
{ return identifier; } // end std::string ShaderAttr::Text(void) const


// Supplement constructor process for incomplete Attr needing more code       */
void dp::ShaderAttr::AddGLSLContext(ShaderAttr& attr,
  std::string shaderTextLine)
{
  if (attr.access == ShaderAttr::Qualifier::Input
    && attr.type == ShaderAttr::ClassType::Undefined)
  {
    std::vector<std::string> tokens = TokenList(shaderTextLine);
    if (tokens.size() > 2 && tokens[0] == "in")
    {
      ClassType readC = GLSLTokenType(tokens[1]);
      if (readC != ShaderAttr::ClassType::Undefined)
      {
        if (tokens[2][tokens[2].size() - 1] == ';')
        {
          attr.identifier = tokens[2].substr(0, tokens[2].size() - 1); // -';'
        }
        else { attr.identifier = tokens[2]; }
        attr.type = readC;
      }
      else { attr.access = ShaderAttr::Qualifier::Unknown; return; }
    }
    else { attr.access = ShaderAttr::Qualifier::Unknown; return; }
  }
} // end void ShaderFile::Attr::AddContext(Attr&, std::string, CodeType)


// Get the class type enumeration for the given string found in GLSL code     */
dp::ShaderAttr::ClassType dp::ShaderAttr::GLSLTokenType(const
  std::string& token)
{
  // Order classes by shader file frequency for slightly better load speeds
  if (token == "vec4") { return ShaderAttr::ClassType::Vec4; }
  else if (token == "mat4") { return ShaderAttr::ClassType::Mat4; }
  else if (token == "sampler2D") { return ShaderAttr::ClassType::Sampler2D; }
  else if (token == "Texture") { return ShaderAttr::ClassType::Texture; }
  else if (token == "vec3") { return ShaderAttr::ClassType::Vec3; }
  else if (token == "vec2") { return ShaderAttr::ClassType::Vec2; }
  else if (token == "int") { return ShaderAttr::ClassType::Int; }
  else if (token == "float") { return ShaderAttr::ClassType::Float; }
  else if (token == "mat3") { return ShaderAttr::ClassType::Mat3; }
  else if (token == "mat2") { return ShaderAttr::ClassType::Mat2; }
  else if (token == "vec1") { return ShaderAttr::ClassType::Vec1; }
  else if (token == "ivec4") { return ShaderAttr::ClassType::iVec4; }
  else if (token == "ivec3") { return ShaderAttr::ClassType::iVec3; }
  else if (token == "ivec2") { return ShaderAttr::ClassType::iVec2; }
  else if (token == "ivec1") { return ShaderAttr::ClassType::iVec1; }
  else if (token == "uvec4") { return ShaderAttr::ClassType::uVec4; }
  else if (token == "uvec3") { return ShaderAttr::ClassType::uVec3; }
  else if (token == "uvec2") { return ShaderAttr::ClassType::uVec2; }
  else if (token == "uvec1") { return ShaderAttr::ClassType::uVec1; }
  else if (token == "bvec4") { return ShaderAttr::ClassType::bVec4; }
  else if (token == "bvec3") { return ShaderAttr::ClassType::bVec3; }
  else if (token == "bvec2") { return ShaderAttr::ClassType::bVec2; }
  else if (token == "bvec1") { return ShaderAttr::ClassType::bVec1; }
  else if (token == "dvec4") { return ShaderAttr::ClassType::dVec4; }
  else if (token == "dvec3") { return ShaderAttr::ClassType::dVec3; }
  else if (token == "dvec2") { return ShaderAttr::ClassType::dVec2; }
  else if (token == "dvec1") { return ShaderAttr::ClassType::dVec1; }
  else if (token == "mat1") { return ShaderAttr::ClassType::Mat1; }
  else { return ShaderAttr::ClassType::Undefined; }

} // end ShaderAttr::ClassType ShaderAttr::GLSLTokenType(const std::string&)

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
