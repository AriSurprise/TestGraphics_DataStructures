/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  RendererGL4.h
Purpose:  Graphics rendering using OpenGL 4.3 core profile mode
Language: C++ / Eng, MSVC (used; works on modern platforms supporting OpenGL 4)
Platform: MSVC / Windows 10 (used; works on modern platforms supporting OpenGL)
Project:  a.Surprise_CS300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <stl>
#include <vector>       // Members and parameters of lists of shaders
#include <string>       // Parameter inputs of GLSL vert / frag shader text
// "./src/..." 
#include "Renderer.h"   // Base class interface / class declaration
#include "TypeErrata.h" // Platform primitive type aliases for easier porting
#include "v4f.h"       // Coordinate / color value parameters


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class RendererGL4 : public Renderer //! Renders in OpenGL 4.3 Core profile
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Constructors                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a renderer using OpenGL 4.3 Core profile protocols, GLAD & GLFW
    \brief
      - create a renderer using OpenGL 4.3 Core profile protocols, GLAD & GLFW
    \details
      - other constructors will be needed to support other windowing APIs
      using the API specific struct pointer type for the window context (here
      and in base Renderer class)
    \param GLFWContext
      - window context created with GLFW for which this renderer will associate
    \param vertShaderText
      - file-at-once GLSL 430 core vertex shader text list to convert coords
    \param fragShaderText
      - file-at-once GLSL 430 core fragment shaders rendering converted coords
    */
    RendererGL4(GLFWwindow* GLFWContext, const std::vector<ShaderFile>&
      vertShaderText, const std::vector<ShaderFile>& fragShaderText);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set the framebuffer to be uniformly set to the rgba color value
    \brief
      - set the framebuffer to be uniformly set to the rgba color value
    \details
      - alpha will be ignored: background clear color is always opaque
    \param rgbColor
      - [0,1] red, green & blue values for which to set all screen pixels
    */
    virtual void ClearTo(const v3f& rgbColor) const override;


    /** Push all pending objects to the next framebuffer to be drawn
    \brief
      - push all pending objects to the next framebuffer to be drawn
    \details
      - non-const operation to allow deferred render queue buffers to purge
    \param mesh
      - mesh to be rendered in the next frame at current settings
    \param material
      - program settings to render the mesh (non-const to allow shader load)
    \param location
      - settings to be used for rendering objects in and after this draw call
    \param view
      - current camera settings to update vertex shader view / persp settings
    \param texture0
      - image to load into the material shader's default texture channel
    */
    virtual void Draw(Mesh& mesh, Material& material,
      Transform* location = nullptr, Camera* view = nullptr,
      Texture* texture0 = nullptr) override;


    /** Place a skybox into the next framebuffer to be drawn
    \brief
      - place a skybox into the next framebuffer to be drawn
    \details
      - non-const operation to allow deferred render queue buffers to purge
    \param mesh
      - mesh to display skybox / environment map onto (non-const to allow load)
    \param material
      - program settings to render a skybox (non-const to allow load)
    \param skybox
      - list of texture files holding skybox images (non-const to allow load)
    \param view
      - current camera settings to update vertex shader view / persp settings
    */
    virtual void DrawCubemap(Mesh& mesh, Material& material, Cubemap& skybox,
      Camera* view = nullptr) override;


    /** Set the render mode to display (or not), mesh triangle buffer elements
    \brief
      - set the render mode to display (or not), mesh triangle buffer elements
    \param display
      - set the render step to include drawing triangles of the mesh surface
    \return
      - reference to the modified Renderer for easier subsequent inline actions
    */
    virtual Renderer& DrawShaded(bool display) override;


    /** Set the render mode to display (or not), mesh vertex normal edge buffer
    \brief
      - set the render mode to display (or not), mesh vertex normal edge buffer
    \param display
      - set the render step to include average normal directions of vertices
    \return
      - reference to the modified Renderer for easier subsequent inline actions
    */
    virtual Renderer& DrawVNormals(bool display) override;


    /** Set the render mode to display (or not), mesh edge buffer elements
    \brief
      - set the render mode to display (or not), mesh edge buffer elements
    \param display
      - set the render step to include drawing edge connections between faces
    \return
      - reference to the modified Renderer for easier subsequent inline actions
    */
    virtual Renderer& DrawWireframe(bool display) override;


    /** Invoke function calls for the setup to perpetuate ImGUI display
    \brief
      - invoke function calls for the setup to perpetuate ImGUI display
    \param ui
      - list of attributes for UI fields to submit state modifications into
    */
    virtual void ImGUIDraw(GuiAttr& ui) override;


    /** Perform all setup needed for ImGUI to run as intended
    \brief
      - perform all setup needed for ImGUI to run as intended
    */
    virtual void ImGUIInit(void) override;


    /** Perform all cleanup needed after ImGUI has finished running
    \brief
      - perform all cleanup needed after ImGUI has finished running
    */
    virtual void ImGUIShutdown(void) override;


    /** Perform GPU uploads / setup the framebuffer set to be used for rendering
    \brief
      - perform GPU uploads / setup the framebuffer set to be used for rendering
    \details
      - each framebuffer will need to be loaded & set with upload return values
    \param skybox
      - set of framebuffers being uploaded to GPU (non-const for accessors set)
    */
    virtual void LoadCubemap(Cubemap& skybox) const override;


    /** Perform GPU uploads / setup the mesh to be accessed for rendering
    \brief
      - perform GPU uploads / setup the mesh to be accessed for rendering
    \param mesh
      - mesh to be uploaded to the GPU (non-const to have accessors modified)
    */
    virtual void LoadMesh(Mesh& mesh) const override;


    /** Perform GPU uploads / setup the shader to be accessed for rendering
    \brief
      - perform GPU uploads / setup the shader to be accessed for rendering
    \param shaderprogram
      - mesh to be uploaded to the GPU (non-const to have accessors modified)
    \return
      - true if program compiles successfully, false if linking errors occur
    */
    virtual bool LoadShader(Shader& shaderProgram) const override;


    /** Perform GPU uploads / setup the framebuffer to be used for rendering
    \brief
      - perform GPU uploads / setup the framebuffer to be used for rendering
    \param textureData
      - texture being uploaded to GPU (non-const to have accessors modified)
    */
    virtual void LoadTarget(Framebuffer& textureData) const override;


    /** Perform GPU uploads / setup the texture to be accessed for rendering
    \brief
      - perform GPU uploads / setup the texture to be accessed for rendering
    \param textureData
      - texture being uploaded to GPU (non-const to have accessors modified)
    */
    virtual void LoadTexture(Texture& textureData) const override;


    /** Deallocate all heap resources in preparation for shutdown
    \brief
      - deallocate all heap resources in preparation for shutdown
    */
    virtual void Shutdown(void) override;


    /** Set the render target to use the given image frame buffer
    \brief
      - set the render target to use the given image frame buffer
    \param renderTarget
      - image to be used as target render object for shader output
    \param clearColor
      - if provided, wipe's the render target to the given flat value
    \return
      - true if framebuffer is successfully bound as render target, else false
    */
    virtual bool Target(Framebuffer* renderTarget = nullptr, const v3f*
      clearColor = nullptr) const override;


    /** Perform cleanup of prior rendering GPU uploads before cubemap removal
    \brief
      - perform cleanup of prior rendering GPU uploads before cubemap removal
    \details
      - framebuffers will need newly invalid upload accessor values reset
    \param cube
      - framebuffer set to clear from GPU (non-const for accessor clearance)
    \param texturesToo
      - whether to clear cubemap textures' GPU state as well in the process
    */
    virtual void UnloadCubemap(Cubemap& cube, bool texturesToo) const
      override;


    /** Perform cleanup of prior rendering GPU uploads before mesh removal
    \brief
      - perform cleanup of prior rendering GPU uploads before mesh removal
    \details
      - mesh will need VAO, VBO, EBO, etc, newly invalid upload values cleared
    \param mesh
      - uploaded mesh to clear off of GPU (non-const to have accessors cleared)
    */
    virtual void UnloadMesh(Mesh& mesh) const override;


    /** Perform cleanup of prior rendering GPU uploads before shader removal
    \brief
      - perform cleanup of prior rendering GPU uploads before shader removal
    \param program
      - shader program to be cleared from GPU (accessors clear; non-const)
    */
    virtual void UnloadShader(Shader& program) const override;


    /** Perform cleanup of prior GPU uploads before framebuffer removal
    \brief
      - perform cleanup of prior GPU uploads before framebuffer removal
    \param imageData
      - framebuffer to clear from GPU (non-const for accessor clearance)
    \param textureToo
      - whether to clear texture GPU state as well in hte process
    */
    virtual void UnloadTarget(Framebuffer& imageData, bool textureToo = true)
      const override;


    /** Perform cleanup of prior rendering GPU uploads before texture removal
    \brief
      - perform cleanup of prior rendering GPU uploads before texture removal
    \details
      - texture will need newly invalid upload accessor values cleared
    \param textureData
      - uploaded texture to clear from GPU (non-const for accessor clearance)
    */
    virtual void UnloadTexture(Texture& textureData) const override;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Classes                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set shader uniform properties per material attribute type & settings
    \brief
      - set shader uniform properties per material attribute type & settings
    \param material
      - material storing type and settings to set / use prior to rendering
    */
    void SetMaterialAttributes(const Material& material);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Program in use
    u4 cProgram;
    // TODO: by this logic, ImGui will only update attributes for the last
    // shader provided; update to include loaded Shader* list & update each

    //! list of Meshes the renderer has taken in (to handle when unloading)
    std::vector<Mesh*> lMeshes;
    // possible to omit, but makes shutdown order less fragile

    //! List of shader programs compiled / loaded by the renderer to use
    std::vector<Shader*> lShaders;

    //! Toggle for whether to include edge buffer activation in draw step
    bool drawEdges;

    //! Toggle for whether to include tri buffer activation in draw step
    bool drawTris;

    //! Toggle for whether to include vertNormal buffer use in draw step
    bool drawVNorms;

  }; // end RendererGL4 declaration

} // end dp namespace
