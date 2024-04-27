/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  ShaderFile.h
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
#include "ASCIIPath.h"  // Validated member of file path input arg
#include "ShaderAttr.h" // Attribute list member
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class ShaderFile //! API agnostic container of shader text file / attributes
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Constants                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    enum class CodeType //! List of types of supported code
    {
      Invalid = -1, // Unsupported / unrecognized version of shader file read
      GLSL43  // OpenGL shader language 430 core (only current supported type)
    }; // end CodeType enum


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a shader from a faux file path, placing code in manually
    \brief
      - create a shader from a faux file path, placing code in manually
    \param file
      - shader name (file name gives shader name, but path irrelevant)
    \param lang
      - shader code language used in code body: GLSL revision, HLSL revision...
    \param code
      - shader language text body of tile to be uploaded to the GPU
    */
    ShaderFile(const ASCIIPath& file, CodeType lang, const std::string& code);


    /** Take a shader file's text into memory and parse it for upload to GPU
    \brief
      - take a shader file's text into memory and parse it for upload to GPU
    \param file
      - shader file path and name to be scanned, parsed and uploaded to GPU
    */
    ShaderFile(const std::string& file);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get the list of shader attributes found in the file to have handles
    \brief
      - get the list of shader attributes found in the file to have handles
    \return
      - the list of attributes found in the shader code
    */
    const std::vector<ShaderAttr>& Attrs(void) const;


    /** Get the shader code to be parsed and uploaded onto the GPU
    \brief
      - get the shader code to be parsed and uploaded onto the GPU
    \return
      - the c-string for the contained shader code to be uploaded to the GPU
    */
    const char* Code(void) const;


    /** Get the shader file extension as a text label for the shader type
    \brief
      - get the shader file extension as a text label for the shader type
    \return
      - the text for the type of the shader as read from file extension
    */
    CodeType Type(void) const;


    /** Get the shader file name as a text string label for the shader (no ext)
    \brief
      - get the shader file name as a text string label for the shader (no ext)
    \return
      - the string of text for the name of the shader as read from file path
    */
    std::string Name(void) const;


    /** Get the sanitized path string to access the file name from given input
    \brief
      - get the sanitized path string to access the file name from given input
    \return
      - the path string for the file to be opened, with file name appended
    */
    std::string Path(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    /** Extract attributes from code text, set attr list and language type
    \brief
      - extract attributes from code text, set attr list and language type
    \param fileContents
      - shader code text to interpret attibutes from (path used for code lang)
    */
    void SetAttrList(const std::string& fileContents);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! File path / name / extension information
    ASCIIPath path;
    // TODO: refactor to just use extracted name string for reduced fragility

    //! Code lang / version enumeration for supported shader language revisions
    CodeType type;

    //! ASCII text of the code contained in the file body (if any is found)
    std::string contents;

    //! list of attributes contained in the body of the shader file text
    std::vector<ShaderAttr> member;


  }; // end ShaderFile class declaration

} // end dp namespace
