/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  ShaderFile.cpp
Purpose:  Generic API agnostic container for text input file to be read to GPU
Language: C++ / Eng, MSVC 19 (used; virtual base class is to decouple language)
Platform: MSVC19 / Windows 10 x64 (used; virtual base class broadens platforms)
Project:  a.Surprise_CS300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// "./src/..."
#include "ASCIIValue.h" // ASCII char / string evaluation / conversion
#include "File.h"       // Read in string of shader text files
#include "Log.h"        // Error / warning output message / logging
#include "ShaderFile.h" // Associated class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                Constructors                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create a blank shader, to be set as a struct subsequently in code          */
dp::ShaderFile::ShaderFile(const ASCIIPath& file, CodeType lang,
  const std::string& code)
  : path(file), type(lang), contents(code)
{
  SetAttrList(contents);
} // end ShaderFile::ShaderFile(const ASCIIPath&, CodeType, const std::string&)


// Take a shader file's text into memory and parse it for upload to GPU       */
dp::ShaderFile::ShaderFile(const std::string& file)
  : path(file)
{
  contents = File::ReadAtOnce(Path());
  SetAttrList(contents);
} // end ShaderFile::ShaderFile(const std::string&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the list of shader attributes found in the file to have handles        */
const std::vector<dp::ShaderAttr>& dp::ShaderFile::Attrs(void) const
{ return member; } // end const std::vector<Attr>& ShaderFile::Attrs(void) const


// Get the sanitized path string to access the file name from given input     */
std::string dp::ShaderFile::Path(void) const
{ return path; } // end std::string ShaderFile::Path(void) const


// Get the shader file name as a text string label for the shader (no ext)    */
std::string dp::ShaderFile::Name(void) const
{
  return path.File(false);
} // end std::string ShaderFile::Name(void) const


// Get the shader file extension as a text label for the shader type          */
dp::ShaderFile::CodeType dp::ShaderFile::Type(void) const
{ return type; } // end CodeType ShaderFile::Ext(void) const


// Get the shader code to be parsedand uploaded onto the GPU                  */
const char* dp::ShaderFile::Code(void) const
{ return contents.c_str(); }// end const char* ShaderFile::Code(void) const


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Extract attributes from code text, set attr list and language type         */
void dp::ShaderFile::SetAttrList(const std::string& fileContents)
{
  if (contents.length() > 0)
  {
    size_t current = 0;
    const char* text = contents.c_str();
    std::string line;
    std::string ext = ToLower(path.Extension());
    bool versSet = false;
    bool posSet = false;
    if (ext == "glvs" || ext == "glfs" || ext == "glsl")
    {
      while (current < contents.length())
      {
        line = StringLine(contents, current);
        // Attempt to scrub first line for supported version match
        if (!versSet)
        {
          if (line == "#version 430 core") { type = CodeType::GLSL43; }
          else { type = CodeType::Invalid; }
          versSet = true;
        }
        // Scrub each line after for potential variable name / type declaration
        ShaderAttr addition(line, ShaderAttr::CodeLang::GLSL43);
        if (addition.AccessType() != ShaderAttr::Qualifier::Unknown)
        {
           // Check for partial attr from (layout...)'\n'(in...) splits
          if (addition.AccessType() == ShaderAttr::Qualifier::Input
            && addition.DataClass() == ShaderAttr::ClassType::Undefined)
          {
            line = StringLine(contents, current);
            ShaderAttr::AddGLSLContext(addition, line);
            if ( addition.Text() != ""
              && addition.AccessType() == ShaderAttr::Qualifier::Input
              && addition.DataClass() != ShaderAttr::ClassType::Undefined)
            {
              member.push_back(addition);
            }
          }
          else if (addition.Text() != "")
          {
            member.push_back(addition);
          }
        }
      }
    }
  }
} // end void ShaderFile::SetAttrList(const std::string&)
