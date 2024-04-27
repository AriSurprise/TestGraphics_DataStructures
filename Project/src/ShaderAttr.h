/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  ShaderAttr.h
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

// <stl>
#include <string>       // uniform / in / out identifier member / parameters
// "./src/..."
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class ShaderAttr //! API agnostic descriptor of shader attributes
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Constants                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Types of code reading supported
    enum class CodeLang 
    {
      Unknown = -1, //! Not a known class access scope qualifier
      GLSL43,       //! Variable fed out from this shader down the pipline
    }; // end CodeLang enum

    //! Categorical shader variable access class specifiers
    enum class Qualifier 
    {
      Unknown = -1, //! Not a known class access scope qualifier
      Output,       //! Variable fed out from this shader down the pipline
      Input,        //! Variable fed in from prior shader in pipeline
      Global        //! Variable set in code and accessed uniformly in shader
    }; // end Qualifier enum

    //! Shader variable class data type storage specifiers
    enum class ClassType 
    {
      Undefined = -1, //! Not a known class type / label
      Int,        //! Single 4-byte signed integer scalar value storage
      Unsigned,   //! Single 4-byte unsigned integer scalar value storage
      Float,      //! Single 4-byte float-point scalar value storage
      Vec1,       //! Set of 1 4-byte float-point value in array storage
      Vec2,       //! Set of 2 4-byte float-point values in array storage
      Vec3,       //! Set of 3 4-byte float-point values in array storage
      Vec4,       //! Set of 4 4-byte float-point values in array storage
      bVec1,      //! Set of 1 4-byte boolean value in array storage
      bVec2,      //! Set of 2 4-byte boolean values in array storage
      bVec3,      //! Set of 3 4-byte boolean values in array storage
      bVec4,      //! Set of 4 4-byte boolean values in array storage
      iVec1,      //! Set of 1 4-byte signed integer value in array storage
      iVec2,      //! Set of 2 4-byte signed integer values in array storage
      iVec3,      //! Set of 3 4-byte signed integer values in array storage
      iVec4,      //! Set of 4 4-byte signed integer values in array storage
      uVec1,      //! Set of 1 4-byte unsigned integer in array storage
      uVec2,      //! Set of 2 4-byte unsigned integers in array storage
      uVec3,      //! Set of 3 4-byte unsigned integers in array storage
      uVec4,      //! Set of 4 4-byte unsigned integers in array storage
      dVec1,      //! Set of 1 8-byte float-point value in array storage
      dVec2,      //! Set of 2 8-byte float-point values in array storage
      dVec3,      //! Set of 3 8-byte float-point values in array storage
      dVec4,      //! Set of 4 8-byte float-point values in array storage
      Mat1,       //! Set of 1x1 (1), col-major 4-byte float in a '2D' array
      Mat2,       //! Set of 2x2 (4), col-major 4-byte floats in a 2D array
      Mat3,       //! Set of 3x3 (9), col-major 4-byte floats in a 2D array
      Mat4,       //! Set of 4x4 (16), col-major 4-byte floats in a 2D array
      Sampler2D,  //! 2D image point interpretation storage specifier
      Texture     //! Image / FBO wrapper storage specifier
    }; // end ClassType enum


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Create a shader attribute from the given shader code line
    \brief
      - create a shader attribute from the given string / value elements
    \param shaderTextLine
      - text string the shader file has to refer to the variable class
    \param shaderLang
      - language syntax logic used to scan the text line for attribute semantic
    */
    ShaderAttr(std::string shaderTextLine, CodeLang shaderLang);


    /** Create a shader attribute from the given string / value elements
    \brief
      - create a shader attribute from the given string / value elements
    \param identifierText
      - text string the shader file has to refer to the variable class
    \param className
      - text string the shader file has to refer to the variable class
    \param gpuVal
      - GPU return value for accessing / modifying the shader attribute
    */
    ShaderAttr(std::string identifierText, Qualifier AttrType,
      ClassType className, s4 gpuVal = -1);


    /** Create a shader attribute from a validated shader attribute
    \brief
      - create a shader attribute from a validated shader attribute
    \param shaderTextLine
      - text string the shader file has to refer to the variable declaration
    \param lang
      - language / revision used for shader code in shader text line argument
    */
    ShaderAttr(const ShaderAttr& attribute);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get access category uniform / global, input /attribute, out, etc
    \brief
      - get access category uniform / global, input /attribute, out, etc
    \return
      - categorical access type enumeration value for the attribute
    */
    Qualifier AccessType(void) const;


    /** Get data type of integer, unsigned, float, v2f|3|4, mat2|3|4, etc
    \brief
      - get data type of integer, unsigned, float, v2f|3|4, mat2|3|4, etc
    \return
      - categorical class data type enumeration value for the attribute
    */
    ClassType DataClass(void) const;


    /** Get value to be used in API calls to reference access of this attribute
    \brief
      - get value to be used in API calls to reference access of this attribute
    \return
      - reference to the modified attribute for easier subsequent inline action
    */
    ShaderAttr& GPUValue(s4 lookupFromGPU);


    /** Get value to be used in API calls to reference access of this attribute
    \brief
      - get value to be used in API calls to reference access of this attribute
    \return
      - GPU return value to point to housing shader's space for manipulation
    */
    s4 Handle(void) const;


    /** Get text read in from shader program code as this attribute's identifier
    \brief
      - get text read in from shader program code as this attribute's identifier
    \return
      - GPU return value to point to housing shader's space for manipulation
    */
    const std::string& Text(void) const;


    /** Supplement constructor process for incomplete Attr needing more code
    \brief
      - supplement constructor process for incomplete Attr needing more code
    \param attr
      - Attribute which was only partially set up, needing more code context
    \param shaderTextLine
      - text string the shader file has to refer to the variable declaration
    \param lang
      - language / revision used for shader code in shader text line argument
    */
    static void AddGLSLContext(ShaderAttr& attr, std::string shaderTextLine);

    /** Get the class type enumeration for the given string found in GLSL code
    \brief
      - get the class type enumeration for the given string found in GLSL code
    \param token
      - chunk of text which may be equivalent to a a class type keyword
    \return
      - class enum value correlated to the given string token (or Undefined)
    */
    static ClassType GLSLTokenType(const std::string& token);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  private:

    //! Locator returned from GPU API for changing the value of the attribute
    s4 query;

    //! Text in shader files correlating to the attribute's class
    Qualifier access;

    //! Text in shader files correlating to the attribute's class
    ClassType type;

    //! Lookup text in shader files correlating to the attribute's identifier
    std::string identifier;


    }; // end ShaderAttr class declaration

} // end dp namespace
