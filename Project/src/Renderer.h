/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Renderer.h
Purpose:  Abstract base class for delegating graphics API specific rendering
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
#include "GuiAttr.h"    // Attr container prevents redundancy between API calls
#include "Camera.h"     // Draw call optionally takes one
#include "Cubemap.h"    // Draw call optionally takes one; load / unload too
#include "Framebuffer.h"// Target set operation requires render target
#include "Material.h"   // Draw call takes a material; load / unload calls too
#include "Mesh.h"       // Draw call takes a mesh; load / unload calls too
#include "Shader.h"     // Draw call takes a shader; load / unload calls too
#include "Skybox.h"     // DrawCubemap call takes a skybox shader
#include "Texture.h"    // Draw call takes a texture; load / unload calls too
#include "Transform.h"  // Draw call needs it: derived classes should have it
#include "v3f.h"       // Color / Coordinate inputs for member functions
#include "WindowGen.h"  // Windowing API enumeration lists for dependent types


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

struct GLFWwindow;  // derived GLFW view class needs base member (context) set

namespace dp //! Contents for use within class assignments
{
  union v3f;        // 3D float vector definition for coordinates and colors
  class ShaderFile; // Input files to store shader data
  class GuiAttr;    // GUI attributes' cur/prev values for frame changes
  class Shader;     // Container for accessing uniforms of a typified program
  class Material;   // Shader instance: object uniform value settings list
  class Texture;    // Base class of image data read in and uploaded to GPU
  class Framebuffer;// Derived Texture class w/ extra GPU hooks to read / write
  class Cubemap;    // Derived Texture[6] array class to store OpenGL standards
  class Mesh;       // Polygon vertex and element (tri/edge) buffer container
  class Camera;     // Derived Transform class to set Render view point / angle

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Renderer //! Abstract base class for delegating rendering API calls
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Constructor to force derived classes to specify GLFW Window is in use
    \brief
      - constructor to force derived classes to specify GLFW Window is in use
    \details
      - new constructors need to be added per windowing API to be supported
    \param context
      - GLFWwindow context signifying GLFW is being used to create the Window
    */
    Renderer(GLFWwindow* context);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set the framebuffer to be uniformly set to the rgba color value
    \brief
      - set the framebuffer to be uniformly set to the rgba color value
    \details
      - alpha will be ignored: background clear color is always opaque
    \param rgbaColor
      - [0,1] red, green, blue and alpha values for which to set pixels
    */
    virtual void ClearTo(const v3f& rgbaColor) const = 0;


    /** Push the given mesh to the next framebuffer to be drawn
    \brief
      - push all pending objects to the next framebuffer to be drawn
    \details
      - non-const operation to allow deferred render queue buffers to purge
    \param mesh
      - mesh to be rendered in the next frame at current settings
    \param location
      - settings to be used for rendering objects in and after this draw call
      omittable, but not advisable if distinct object drawing is desired
    */
    virtual void Draw(Mesh& mesh, Material& shadingProperties,
      Transform* location = nullptr, Camera* view = nullptr,
      Texture* texture = nullptr) = 0;


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
    */
    virtual void DrawCubemap(Mesh& mesh, Material& material, Cubemap& skybox,
      Camera* view = nullptr)
      = 0;


    /** Set the render mode to display (or not), mesh triangle buffer elements
    \brief
      - set the render mode to display (or not), mesh triangle buffer elements
    \param display
      - set the render step to include drawing triangles of the mesh surface
    \return
      - reference to the modified Renderer for easier subsequent inline actions
    */
    virtual Renderer& DrawShaded(bool display) = 0;


    /** Set the render mode to display (or not), mesh vertex normal edge buffer
    \brief
      - set the render mode to display (or not), mesh vertex normal edge buffer
    \param display
      - set the render step to include average normal directions of vertices
    \return
      - reference to the modified Renderer for easier subsequent inline actions
    */
    virtual Renderer& DrawVNormals(bool display) = 0;


    /** Set the render mode to display (or not), mesh edge buffer elements
    \brief
      - set the render mode to display (or not), mesh edge buffer elements
    \param display
      - set the render step to include drawing edge connections between faces
    \return
      - reference to the modified Renderer for easier subsequent inline actions
    */
    virtual Renderer& DrawWireframe(bool display) = 0;


    /** Invoke function calls for the setup to perpetuate ImGUI display
    \brief
      - invoke function calls for the setup to perpetuate ImGUI display
    \details
      - headers needing to be included will be contextual to packages used
    \param ui
      - list of attributes for UI fields to submit state modifications into
    */
    virtual void ImGUIDraw(GuiAttr& ui) = 0;


    /** Perform all setup needed for ImGUI to run as intended
    \brief
      - perform all setup needed for ImGUI to run as intended
    \details
      - headers needing to be included will be contextual to packages used
    */
    virtual void ImGUIInit(void) = 0;


    /** Perform all cleanup needed after ImGUI has finished running
    \brief
      - perform all cleanup needed after ImGUI has finished running
    \details
      - headers needing to be included will be contextual to packages used
    */
    virtual void ImGUIShutdown(void) = 0;


    /** Perform GPU uploads / setup the framebuffer set to be used for rendering
    \brief
      - perform GPU uploads / setup the framebuffer set to be used for rendering
    \details
      - each framebuffer will need to be loaded & set with upload return values
    \param skybox
      - set of framebuffers being uploaded to GPU (non-const for accessors set)
    */
    virtual void LoadCubemap(Cubemap& skybox) const = 0;


    /** Perform GPU uploads / setup the mesh to be accessed for rendering
    \brief
      - perform GPU uploads / setup the mesh to be accessed for rendering
    \details
      - mesh will need VAO, VBO, EBO, etc, set with upload return values
    \param meshData
      - mesh to be uploaded to the GPU (non-const to have accessors modified)
    */
    virtual void LoadMesh(Mesh& meshData) const = 0;


    /** Perform GPU uploads / setup the shader to be accessed for rendering
    \brief
      - perform GPU uploads / setup the shader to be accessed for rendering
    \details
      - shader will need match program code type across shaders; program
      handle and renderer will need to be set
    \param shaderprogram
      - mesh to be uploaded to the GPU (non-const to have accessors modified)
    \return
      - true if program compiles successfully, false if linking errors occur
    */
    virtual bool LoadShader(Shader& shaderProgram) const = 0;


    /** Perform GPU uploads / setup the framebuffer to be used for rendering
    \brief
      - perform GPU uploads / setup the framebuffer to be used for rendering
    \details
      - framebuffer will need Texture buffer set, as well as frame buffer objects
    \param textureData
      - texture being uploaded to GPU (non-const to have accessors modified)
    */
    virtual void LoadTarget(Framebuffer& imageData) const = 0;


    /** Perform GPU uploads / setup the texture to be accessed for rendering
    \brief
      - perform GPU uploads / setup the texture to be accessed for rendering
    \details
      - texture will need Texture buffer object, set with upload return values
    \param textureData
      - texture being uploaded to GPU (non-const to have accessors modified)
    */
    virtual void LoadTexture(Texture& textureData) const = 0;


    /** Deallocate all heap resources in preparation for shutdown
    \brief
      - deallocate all heap resources in preparation for shutdown
    */
    virtual void Shutdown(void) = 0;


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
    clearColor = nullptr) const = 0;


    /** Perform cleanup of prior rendering GPU uploads before cubemap removal
    \brief
      - perform cleanup of prior rendering GPU uploads before cubemap removal
    \details
      - framebuffers will need newly invalid upload accessor values reset
    \param skybox
      - framebuffer set to clear from GPU (non-const for accessor clearance)
    \param texturesToo
      - whether to clear cubemap textures' GPU state as well in the process
    */
    virtual void UnloadCubemap(Cubemap& skybox, bool texturesToo) const = 0;


    /** Perform cleanup of prior rendering GPU uploads before mesh removal
    \brief
      - perform cleanup of prior rendering GPU uploads before mesh removal
    \details
      - mesh will need VAO, VBO, EBO, etc, newly invalid upload values reset
    \param meshData
      - uploaded mesh to clear off of GPU (non-const to have accessors cleared)
    */
    virtual void UnloadMesh(Mesh& meshData) const = 0;


    /** Perform cleanup of prior rendering GPU uploads before shader removal
    \brief
      - perform cleanup of prior rendering GPU uploads before shader removal
    \details
      - shader will need both handle values & attr list handles reset
    \param program
      - shader program to be cleared from GPU (accessors clear; non-const)
    */
    virtual void UnloadShader(Shader& program) const = 0;


    /** Perform cleanup of prior GPU uploads before framebuffer removal
    \brief
      - perform cleanup of prior GPU uploads before framebuffer removal
    \details
      - framebuffer will need newly invalid upload accessor values reset
    \param imageData
      - framebuffer to clear from GPU (non-const for accessor clearance)
    \param textureToo
      - whether to clear texture GPU state as well in the process
    */
    virtual void UnloadTarget(Framebuffer& imageData, bool textureToo = true)
      const = 0;


    /** Perform cleanup of prior rendering GPU uploads before texture removal
    \brief
      - perform cleanup of prior rendering GPU uploads before texture removal
    \details
      - texture will need newly invalid upload accessor values reset
    \param textureData
      - uploaded texture to clear from GPU (non-const for accessor clearance)
    */
    virtual void UnloadTexture(Texture& textureData) const = 0;


  protected:

    /** Get window context left as a generic void pointer to pass state changes
    \brief
      - get window context left as a generic void pointer to pass state changes
    \details
      - some functions may not have an API specific struct type, but will have
      size specified elsewhere or the like for reference to context fields
    \return
      - whatever address the window context pointer currently contains
    */
    void* Context(void) const;


    /** Get the window context cast as a GLFWwindow pointer, only if using GLFW
    \brief
      - get the window context cast as a GLFWwindow pointer, only if using GLFW
    \details
      - any renderer will need access to the window context to change state
      so including the casting functionality on the base class for all derived
      classes to have access makes the most sense to allow that renderer, of
      whatever rendering API may be in use, to work with any supported windower
      in tandem use for an output window target.
    \return
      - null pointer if not using GLFW: GLFWwindow context pointer cast if so
    */
    GLFWwindow* ContextGLFW(void) const;

    //! Whether Dear ImGUI has had initialization procedures performed; can run
    bool imguiSetup = false;

  private:

    //! Which whindowing API is in use to check what to struct type to use
    WindowGen::PACKAGE windowingAPI = WindowGen::PACKAGE::UNDEFINED;

    //! Abstract pointer to the windowing context API struct type (to be cast)
    void* windowContext = nullptr;

  }; // end Renderer class declaration

} // end dp namespace
