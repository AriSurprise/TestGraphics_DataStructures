/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Shader.h
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
#include "ShaderFile.h" // Validated member of file path input arg
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  class Renderer; // Definition for proxy pointer member 

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! API agnostic container of linkable shader files/attributes
  class Shader 
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Constants                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Categorical shader render target mode, allowing per-mode material sets
    enum class Rendering
    {
      Forward,  //! Default drawing to RGBA screen pixels or framebuffer
      Deferred  //! Intermediary geometrybuffer with material properties in use
    };

    //! Categorical material attribute texturing type, per-mode material sets
    enum class Sampling
    {
      Flat, //! Using default mapping to 2D texture samplers per attribute
      Cube  //! Using 6 connective 2D textures to describe environment mapping
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a composite shader using a set of vertex and fragment code files
    \brief
      - create a composite shader using a set of vertex and fragment code files
    \param vertex
      - shader text of vertex mutation code into desired coordinate data
    \param fragment
      - shader text of element interpolation code between vertices (rasterizer)
    \param targetType
      - declaration of categorical program output mode / channel expectations
    */
    Shader(ShaderFile vertex, ShaderFile fragment, Rendering targetType = 
    Rendering::Forward);


    /** Copy a shader based upon another's settings (shouldn't be necessary)
    \brief
      - Copy a shader based upon another's settings (shouldn't be necessary)
    \param source
      - shader program to be copied filewise (GPU instance not applicable)
    */
    Shader(const Shader& source);


    /** Unload a loaded shader from the GPU in preparation for deletion
    \brief
      - unload a loaded shader from the GPU in preparation for deletion
    */
    ~Shader(void);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get the fragment shader code for debugging or upload onto the GPU
    \brief
      - get the fragment shader code for debugging or upload onto the GPU
    \return
      - the c-string for the contained fragment shader code
    */
    const char* FragmentCode(void) const;


    /** Get the sanitized path string to access the fragment shader file
    \brief
      - get the sanitized path string to access the fragment shader file
    \return
      - the path string for the fragment shader file (with file name + ext)
    */
    const std::string& FragmentPath(void) const;


    /** Get a given shader global (uniform) for use in the shader program
    \brief
      - get a given shader global (uniform) for use in the shader program
    \param subscript
      - index in shader list for the desired global / uniform
    \return
      - the given (or last if greater) global (ie uniform) for the shader
    */
    ShaderAttr& Global(u4 subscript);


    /** Get the list of shader globals (uniforms) found in the shader program
    \brief
      - get the list of shader globals (uniforms) found in the shader program
    \return
      - the list of globals (ie uniforms) found in the shader code
    */
    const std::vector<ShaderAttr>& Globals(void) const;


    /** Get a given shader input (attrib) for use in the shader program
    \brief
      - get a given shader input (attrib) for use in the shader program
    \param subscript
      - index in shader list for the desired input / attrib
    \return
      - the given (or last if greater) input (ie attribs) for the shader
    */
    const ShaderAttr& Input(u4 subscript) const;


    /** Get the list of shader inputs (attribs) found in the shader program
    \brief
      - get the list of shader inputs (attribs) found in the shader program
    \return
      - the list of inputs (ie attribs) found in the shader code
    */
    const std::vector<ShaderAttr>& Inputs(void) const;


    /** Get whether the shader program has been successfully loaded for render
    \brief
      - get whether the shader program has been successfully loaded for render
    \return
      - true when GPU handle is set to a non-default value indicative of load
    */
    bool Loaded(void) const;


    /** Get the enumerated shader program code language / format type in use
    \brief
      - get the enumerated shader program code language / format type in use
    \return
      - the type enumeration value for the shader program language
    */
    ShaderFile::CodeType Type(void) const;


    /** Get the linked shader program display name as a string program label
    \brief
      - get the linked shader program display name as a string program label
    \return
      - the string of text for the sequence of the shader program files
    */
    const std::string& Name(void) const;


    /** Get a given shader output target from the shader program pipeline
    \brief
      - get a given shader output target from the shader program pipeline
    \param subscript
      - index in shader list for the desired output
    \return
      - the given (or last if greater) output (ie target) for the shader
    */
    const ShaderAttr& Output(u4 subscript) const;


    /** Get the list of shader outputs from the end of the shader pipeline
    \brief
      - get the list of shader outputs from the end of the shader pipeline
    \return
      - the list of outputs from the shader program pipeline
    */
    const std::vector<ShaderAttr>& Outputs(void) const;


    /** Get the vertex shader code for debugging or upload onto the GPU
    \brief
      - get the vertex shader code for debugging or upload onto the GPU
    \return
      - the c-string for the contained fragment shader code
    */
    const char* VertexCode(void) const;


    /** Get the sanitized path string to access the vertex shader file
    \brief
      - get the sanitized path string to access the vertex shader file
    \return
      - the path string for the vertex shader file (with file name + ext)
    */
    const std::string& VertexPath(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Operator Overloads                             */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set a shader based upon another's settings (CPU side; GPU aside)
    \brief
      - set a shader based upon another's settings (CPU side; GPU aside)
    \param source
      - shader program to be copied filewise (GPU instance not applicable)
    */
    Shader& operator=(const Shader& source);

    Renderer* proxy;

    //! Program ID value to access this shader on the GPU
    u4 handle;


  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Renderer Accessors                              */
    /* (storing GPU buffer return states on the relevant mesh being rendered) */

    //! Various VAO, VBO & EBO members exist only for Renderer access per Mesh
    friend class Renderer; // that should include the base class interface...
    friend class RendererGL4; // but derived classes don't inherit friend
    friend class Window;
    // TODO: list all other Renderers as implemented to access / load / unload

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Code lang / version enumeration for supported shader language revisions
    ShaderFile::CodeType type;

    //! Tracker for whether the shader is a forward renderer (0) or deferred (1)
    Rendering mode;

    //! Label to describe what linked files constitute the sum program
    std::string name;

    //! File path / name / extension information for vertex shader
    std::string vertPath;

    //! ASCII text of the code contained in the vertex file body
    std::string vertContents;

    //! File path / name / extension information for vertex shader
    std::string fragPath;

    //! ASCII text of the code contained in the vertex file body
    std::string fragContents;

    //! File path / name / extension information for geometry shader
    std::string geomPath;

    //! ASCII text of the code contained in the geometry file body
    std::string geomContents;

    //! list of input variables used in the shader program
    std::vector<ShaderAttr> input;

    //! list of uniform variables used in the shader program
    std::vector<ShaderAttr> global;

    //! list of output variables used in the shader program
    std::vector<ShaderAttr> output;

  }; // end Shader class declaration

} // end dp namespace
