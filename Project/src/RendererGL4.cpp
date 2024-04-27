/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  RendererGL4.h
Purpose:  Graphics rendering implementation using OpenGL 4.3 core profile mode
Language: C++ / Eng, MSVC (used; works on modern platforms supporting OpenGL 4)
Platform: MSVC / Windows 10 x64 (used; works on modern x64 supporting OpenGL 4)
Project:  a.Surprise_CS300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <../Packages/include/...>
#include <glad/glad.h>      // GLAD OpenGL wrangler (for GL 4.3)
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <stb_image.h>
// "./src/..."
#include "Blinn.h"          // Setting Blinn material shader properties 
#include "Camera.h"         // Setting Camera matrix values in draw call
#include "Capture.h"        // Setting FBO shader attribute properties 
#include "File.h"           // Reading shader file contents 
#include "Lambert.h"        // Setting Lambert material shader properties 
#include "m3f.h"            // Matrix subset extractions / uniform updates
#include "m4f.h"            // Uniform variable matrix updates
#include "Phong.h"          // Setting Phong material shader properties 
#include "Log.h"            // Error / warning output / message logging 
#include "RendererGL4.h" // Class declaration header file 
#include "Skybox.h"         // Setting Skybox material properties
#include "v3f.h"            // reading 3D coordinate / color vector channels


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Static Constants                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                Constructors                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::RendererGL4::RendererGL4(GLFWwindow* GLFWContext,
  const std::vector<ShaderFile>& vertShaders,
  const std::vector<ShaderFile>& fragShaders)
  : Renderer(GLFWContext), drawTris(true), drawEdges(false), drawVNorms(false)
{
  
  // Initialize general render globals; not changing settings in loops
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glPointSize(8.0f);
  glLineWidth(3.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Setup ImGUI to run in this configuration profile
  ImGUIInit();

} // end RendererGL4::RendererGL4(void)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set the framebuffer to be uniformly set to the rgba color value            */
void dp::RendererGL4::ClearTo(const v3f& rgbColor) const
{
  glClearColor(rgbColor.r, rgbColor.g, rgbColor.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

} // end void RendererGL4::ClearTo(const v3f&)


// Push all pending objects to the next framebuffer to be drawn               */
void dp::RendererGL4::Draw(Mesh& mesh, Material& material,
  Transform* location, Camera* view, Texture* texture)
{
  glUseProgram(material.ShadingGroup().handle);
  if (view)
  {
    GLuint handle;
    // Set view matrix
    if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "vFw"))
      != -1)
    {
      glUniformMatrix4fv(handle, 1, false, &view->View()[0][0]);
    }
    // Sky shader takes view directly from model coords instead of world
    else if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "vFm")) != -1)
    {
      m4f pass = view->View().Linear();
      glUniformMatrix4fv(handle, 1, 0, (GLfloat*)&pass[0][0]);
    }
    // Set persp matrix
    if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "pFv")) != -1)
    {
      glUniformMatrix4fv(handle, 1, false, &view->Persp()[0][0]);
    }
  }

  glBindVertexArray(mesh.state.record);
  if (texture)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->state.image);
  }
  glDrawElements(GL_TRIANGLES, (GLsizei)mesh.TriIndices(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

} // end void RendererGL4::Draw(Mesh&, Transform*, Texture*)


// Push all pending objects to the next framebuffer to be drawn               */
void dp::RendererGL4::DrawCubemap(Mesh& mesh, Material& material,
  Cubemap& skybox, Camera* view)
{

  // Validate all arguments provided are loaded on GPU prior to attempted draw
  if (!skybox.Loaded())
  {
    Log::Info("RendererGL4::DrawCubemap - given cubemap needs load");
    LoadCubemap(skybox);
    if (!skybox.Loaded())
    {
      Log::Error("RendererGL4::DrawCubemap - cubemap couldn't be loaded");
      return;
    }
  }
  if (skybox.proxy != this)
  {
    Log::Warn("RendererGL4::DrawCubemap - cubemap wasn't loaded locally", 0);
    Log::W(", multi-load support not yet implemented", 1);
    return;
  }
  if (!material.ShadingGroup().Loaded())
  {
    Log::Info("RendererGL4::DrawCubemap - given material shader needs load");
    LoadShader(material.ShadingGroup());
    if (!material.ShadingGroup().Loaded())
    {
      Log::Error("RendererGL4::DrawCubemap - shader material couldn't load");
      return;
    }
  }
  if (material.ShadingGroup().proxy != this)
  {
    Log::Warn("RendererGL4::DrawCubemap - shader not on this renderer", 0);
    Log::W(", multi-load support not yet implemented", 1);
    return;
  }
  if (!mesh.Loaded())
  {
    Log::Info("RendererGL4::DrawCubemap - given mesh needs load");
    LoadMesh(mesh);
    if (!mesh.Loaded())
    {
      Log::Warn("RendererGL4::DrawCubemap - mesh load failed");
    }
  }
  if (mesh.state.proxy != this)
  {
    Log::Warn("RendererGL4::DrawCubemap - mesh not on this renderer", 0);
    Log::W(", multi-load support not yet implemented", 1);
    return;
  }

  // Set shader program state to reflect material settings
  glDisable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);
  glUseProgram(material.ShadingGroup().handle);
  if (view)
  {
    GLuint handle;

    // Set world->view matrix
    if ((handle
      = glGetUniformLocation(material.ShadingGroup().handle, "vFw")) != -1)
    {
      m4f pass = view->View().Linear();
      glUniformMatrix4fv(handle, 1, 0, (GLfloat*)&pass[0][0]);
    }
    // Set persp matrix
    if ((handle
      = glGetUniformLocation(material.ShadingGroup().handle, "pFv")) != -1)
    {
      glUniformMatrix4fv(handle, 1, false, &view->Persp()[0][0]);
    }

  }

  glBindVertexArray(mesh.state.record);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)skybox.use);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)mesh.state.surface);
  glDrawElements(GL_TRIANGLES, (GLsizei)mesh.TriIndices(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

} // end void RendererGL4::DrawCubemap(Mesh&, Skybox&, Cubemap&)


//  Set the render mode to render (or not), mesh triangle buffer elements     */
dp::Renderer& dp::RendererGL4::DrawShaded(bool display)
{
  drawTris = display;
  return *this;

} // end Renderer& RendererGL4::DrawShaded(bool)


//  Set the render mode to render (or not), mesh edge buffer elements         */
dp::Renderer& dp::RendererGL4::DrawVNormals(bool display)
{
  drawVNorms = display;
  return *this;

} // end Renderer& RendererGL4::DrawVNormals(bool)


//  Set the render mode to render (or not), mesh vertex normal edge buffer    */
dp::Renderer& dp::RendererGL4::DrawWireframe(bool display)
{
  drawEdges = display;
  return *this;

} // end Renderer& RendererGL4::DrawWireframe(bool)


// Invoke function calls for the setup to perpetuate ImGUI display            */
void dp::RendererGL4::ImGUIDraw(GuiAttr& ui)
{
  if (imguiSetup)
  {
    static float f;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
      ImGui::Begin("Settings");
      ImGui::Text("Running at %.1f FPS : %.3f ms/frame ",
        ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
      ImGui::Text("");
      ImGui::Combo("Shader Type", &ui.shadCurr, ui.shadeType,
        IM_ARRAYSIZE(ui.shadeType));
      ImGui::Text("Material Attributes");
      ImGui::ColorEdit3("Ambient", (float*)&ui.ambientCurrent);
      ImGui::ColorEdit3("Diffuse", (float*)&ui.diffuseCurrent);
      ImGui::ColorEdit3("Incandescence", (float*)&ui.incandescenceCurrent);
      ImGui::ColorEdit3("Fresnel", (float*)&ui.fresnelCurrent);
      if (ui.shadCurr != 0) // not on "Lambert"
      {
        ImGui::ColorEdit3("Specular", (float*)&ui.specularCurrent);
        ImGui::SliderFloat("Falloff", &ui.falloffCurrent, 1.0f, 1000.0f);
      }
      else // pad for uniform space for missing attributes on lambert
      {
        ImGui::Text("");
        ImGui::Text("");
      }
      ImGui::Text("");
      ImGui::Text("Press [space] to start / stop rotation");
      ImGui::Text("WASD: camera pan / zoom (dolly)");
      ImGui::Text("QZEX: camera roll / tilt (pan up/down)");
      ImGui::Text("Arrows: camera pitch / yaw");
      ImGui::Text("");
      ImGui::Combo("GPU UV Mapping", &ui.uvMCurrent, ui.uvMode,
        IM_ARRAYSIZE(ui.uvMode));
      ImGui::Text("      X            Y            Z");
      ImGui::SliderFloat3("UV Projection Axis", (float*)&ui.axisCurrent, -1.0f,
        1.0f, "-1   0   +1", 1.0f);
      ImGui::Text(("  " + ui.axisCurrent.String()).c_str());
      ImGui::End();
    }
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }
  else
  {
    Log::Note("RendererGL4::ImGUIDraw - setup not performed properly");
  }

} // end void RendererGL4::ImGUIDraw(void)


// Perform all setup needed for ImGUI to run as intended                      */
void dp::RendererGL4::ImGUIInit(void)
{
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(ContextGLFW(), true);
  ImGui_ImplOpenGL3_Init();
  imguiSetup = true;
} // end void RendererGL4::ImGUIInit(void)


// Perform all cleanup needed after ImGUI has finished running                */
void dp::RendererGL4::ImGUIShutdown(void)
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  imguiSetup = false;
} // end void RendererGL4::ImGUIShutdown(void)


// Perform GPU uploads / setup the framebuffer set to be used for rendering   */
void dp::RendererGL4::LoadCubemap(Cubemap& cube) const
{
  // TODO: obviously fragile vulnerability to non-matching image modes / res
  // cubemap should have global mode & ensure state / res ubiquity on load
  if (!cube.Loaded())
  {
    GLuint qbo;
    GLint mode = GL_RGBA, chSize = GL_UNSIGNED_BYTE;
    glGenTextures(1, &qbo);
    glBindTexture(GL_TEXTURE_CUBE_MAP, qbo);

    for (unsigned int s = 0; s < dp::Cubemap::FACES; ++s)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + s, 0, mode,
        cube[s].Width(), cube[s].Height(), 0, mode, chSize,
        cube[s].RGBAData());
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    cube.use = qbo;
    cube.proxy = (Renderer*)this;
  }
  else
  {
    Log::Warn("RendererGL4::LoadCubemap - cubemap already loaded");
  }

} // end void RendererGL4::LoadCubemap(Cubemap&) const


// Perform GPU uploads / setup the mesh to be accessed for rendering          */
void dp::RendererGL4::LoadMesh(Mesh& mesh) const
{
  if (mesh != nullmesh && !mesh.Loaded())
  {
    GLuint VAO; // temp GL VAO to assign to mesh when successfully loaded
    GLuint VBO; // temp GL value to hold and assign Vertex buffer
    GLuint EBOVNorm; // GL value to hold and assign normal element buffer
    GLuint EBOEdge; // temp GL value to hold and assign Edge element buffer
    GLuint EBOTri; // temp GL value to hold and assign Tri element buffer

    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBOVNorm); // (vertex normal orientation edge conn buffer)
    glGenBuffers(1, &EBOEdge);
    glGenBuffers(1, &EBOTri);

    glBindVertexArray(VAO);

    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.VNormVBytes(), &mesh.VNormVBuffer(),
      GL_STATIC_DRAW); // vNorms terminal vertices appended to base vertex buff

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOEdge);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.EdgeBytes(), &mesh.EdgeBuffer(),
      GL_STATIC_DRAW); // EdgeBytes() = EdgeCount() * 2(elems) * 4(sizeof(u4))

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOVNorm);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.VNormEBytes(),
      &mesh.VNormEBuffer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOTri);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.TriBytes(), &mesh.TriBuffer(),
      GL_STATIC_DRAW); // TriBytes() = TriCount() * 3(elems) * 4(sizeof(u4))

    // set the vertex shader attribute pointers
    // TODO: hard coded attribute locations should be shader specific

    glEnableVertexAttribArray(0); // vec[3] texLoc @ layout location = [0]
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(dp::Mesh::Vertex),
      (void*)offsetof(dp::Mesh::Vertex, dp::Mesh::Vertex::uvs));

    glEnableVertexAttribArray(1); // vec[3] coords @ layout location = [1]
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(dp::Mesh::Vertex),
      (void*)offsetof(dp::Mesh::Vertex, dp::Mesh::Vertex::pos));

    glEnableVertexAttribArray(2); // vec[3] vColor @ layout location = [2]
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(dp::Mesh::Vertex),
      (void*)offsetof(dp::Mesh::Vertex, dp::Mesh::Vertex::color));

    glEnableVertexAttribArray(3); // vec[3] normal @ layout location = [3]
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(dp::Mesh::Vertex),
      (void*)offsetof(dp::Mesh::Vertex, dp::Mesh::Vertex::norm));

    glEnableVertexAttribArray(4); // vec[3] tangnt @ layout location = [4]
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(dp::Mesh::Vertex),
      (void*)offsetof(dp::Mesh::Vertex, dp::Mesh::Vertex::tan));

    glEnableVertexAttribArray(5); // vec[3] bitans @ layout location = [5]
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(dp::Mesh::Vertex),
      (void*)offsetof(dp::Mesh::Vertex, dp::Mesh::Vertex::bitan));

    // Set state aside as is (by unbinding: stop recording state)
    glBindVertexArray(0);

    mesh.state.vertices = static_cast<u4>(VBO);
    mesh.state.pointFuzzEBO = static_cast<u4>(EBOVNorm);
    mesh.state.wireframe = static_cast<u4>(EBOEdge);
    mesh.state.surface = static_cast<u4>(EBOTri);
    mesh.state.record = static_cast<u4>(VAO);
    mesh.state.proxy = (Renderer*)this;

  }
  else
  {
    Log::Info("RendererGL4::LoadMesh - mesh already loaded");
  }

} // end void LoadMesh(Mesh&) const


// Perform GPU uploads / setup the texture to be accessed for rendering       */
bool dp::RendererGL4::LoadShader(Shader& shaderProgram) const
{
  if (!shaderProgram.Loaded())
  {
    int success;
    char infoLog[512];
    const char* shaderText = shaderProgram.VertexCode();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderText, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      dp::Log::Error("RendererGL4::LoadShader - vertex compilation failed:\n"
        + std::string(infoLog));
    }
    shaderText = shaderProgram.FragmentCode();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderText, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      dp::Log::Error(
        "RendererGL4(GLFWwindow...) - fragment compilation failed:\n"
        + std::string(infoLog));
    }
    // Connect their outputs and inputs as a program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // (Check for linking errors)
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(program, 512, NULL, infoLog);
      dp::Log::Error("RendererGL4(GLFWwindow...) - linking failed:\n"
        + std::string(infoLog));
    }
    shaderProgram.handle = program;
    shaderProgram.proxy = (Renderer*)(this);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // Shader program linked successfully...


    // Get / store (set) handles for each global / uniform for later access
    for (u4 i = 0; i < shaderProgram.Globals().size(); ++i)
    {
      std::string attrText = shaderProgram.Global(i).Text();
      const char* aName = attrText.c_str();
      GLint result = glGetUniformLocation(program, aName);
      if (result == -1)
      {
        dp::Log::Warn("RendererGL4 - no match in " + shaderProgram.Name()
          + " for " + attrText +
          ": it may have been optimized away, did you use it code?");
        shaderProgram.Global(i).GPUValue(-1);
      }
      else
      {
        shaderProgram.Global(i).GPUValue(static_cast<s4>(result));
      }
    }
    return true;
  }
  else
  {
    Log::Warn("RendererGL4::LoadShader - shader already loaded");
    return false;
  }

} // end void RendererGL4::LoadShader(Shader&) const


// Perform GPU uploads / setup the framebuffer to be used for rendering       */
void dp::RendererGL4::LoadTarget(Framebuffer& renderTarget) const
{
  if (renderTarget.use == u4_MAX && renderTarget.proxy == nullptr)
  {
    GLuint fbo, rbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    if (!renderTarget.Loaded())
    {
      GLuint textureState;
      // TODO: actually read modes to proper enum values
      u4 channelSize = GL_UNSIGNED_BYTE;
      u4 channelRead = GL_RGBA;
      u4 mode = GL_RGBA8;

      glGenTextures(1, &textureState);
      glBindTexture(GL_TEXTURE_2D, textureState);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
      glTexImage2D(GL_TEXTURE_2D, 0, mode, renderTarget.Width(),
        renderTarget.Height(), 0, channelRead, channelSize,
        renderTarget.RGBAData());
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, textureState, 0);

      renderTarget.state.image = static_cast<u4>(textureState);
      renderTarget.state.proxy = (Renderer*)(this);
    }
    else
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, renderTarget.state.image, 0);
    }

    // create a renderbuffer object for depth and stencil attachment (unsampled)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    // use a single renderbuffer object for both a depth AND stencil buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
      renderTarget.width, renderTarget.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
      GL_RENDERBUFFER, rbo);
    // check attachment succeeded / framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      Log::Error("RendererGL4 - framebuffer not complete");
    }
    else
    {
      renderTarget.use = fbo;
      renderTarget.depthStencil = rbo;
      renderTarget.proxy = (Renderer*)this;
    }
  }
  else
  {
    Log::Warn("RendererGL4::LoadTarget - framebuffer already loaded");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
} // end void RendererGL4::LoadTarget(Framebuffer&) const                     */


void dp::RendererGL4::LoadTexture(Texture& texture) const
{
  unsigned int textureID;
  glGenTextures(1, &textureID);
  int width = texture.Width(), height = texture.Height(), nrComponents = 4;
  unsigned char* data = texture.RGBAData();
  if (data)
  {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
      GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture.state.image = textureID;
    texture.state.proxy = (Renderer*)this;
  }
  else
  {
    dp::Log::Error("Texture failed to load at path: "
      + texture.FilePath().RunningPath());
  }
  return;

} // end void RendererGL4::LoadTexture(Texture&) const                        */


void dp::RendererGL4::Shutdown(void)
{} // end void RendererGL4::Shutdown(void)                                    */


bool dp::RendererGL4::Target(Framebuffer* renderTarget, const v3f*
  clearColor) const
{
  if (renderTarget)
  {
    if (renderTarget->use == u4_MAX || renderTarget->proxy == nullptr)
    {
      LoadTarget(*renderTarget);
    }
    if (renderTarget->use != u4_MAX && renderTarget->proxy == this)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->use);
      glEnable(GL_DEPTH_TEST);
      glClearDepth(1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      return true;
    }
    else
    {
      Log::Warn("RendererGL4::Target - couldn't load framebuffer");
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      if (clearColor) { ClearTo(*clearColor); }
      else
      {
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      }
      return false;
    }
  }
  else
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (clearColor) { ClearTo(*clearColor); }
    else
    {
      glClearDepth(1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    glDisable(GL_DEPTH_TEST);
    return true;
  }

} // end bool RendererGL4::Target(Framebuffer*, const v3f*) const             */


void dp::RendererGL4::UnloadCubemap(Cubemap& skybox, bool texturesToo)
  const
{
  // TODO: glDeleteCubemapBuffers...?
  for (u4 i = 0; i < dp::Cubemap::FACES; ++i)
  {
    UnloadTexture(skybox[i]);
  }
  skybox.use = u4_MAX;
  skybox.proxy = nullptr;

} // end void RendererGL4::UnloadCubemap(Cubemap&) const                      */


void dp::RendererGL4::UnloadMesh(Mesh& mesh) const
{
  if (mesh.Loaded())
  {
    GLuint buff;
    buff = static_cast<GLuint>(mesh.state.record);
    glDeleteBuffers(1, &buff);
    mesh.state.record = u4_MAX;
    if (mesh.state.vertices != u4_MAX)
    {
      buff = static_cast<GLuint>(mesh.state.vertices);
      glDeleteBuffers(1, &buff);
      mesh.state.vertices = u4_MAX;
    }
    if (mesh.state.pointFuzzEBO != u4_MAX)
    {
      buff = static_cast<GLuint>(mesh.state.pointFuzzEBO);
      glDeleteBuffers(1, &buff);
      mesh.state.pointFuzzEBO = u4_MAX;
    }
    if (mesh.state.wireframe != u4_MAX)
    {
      buff = static_cast<GLuint>(mesh.state.wireframe);
      glDeleteBuffers(1, &buff);
      mesh.state.wireframe = u4_MAX;
    }
    if (mesh.state.surface != u4_MAX)
    {
      buff = static_cast<GLuint>(mesh.state.surface);
      glDeleteBuffers(1, &buff);
      mesh.state.surface = u4_MAX;
    }
    mesh.state.proxy = nullptr;
  }
} // end void UnloadMesh(Mesh&) const                                         */


void dp::RendererGL4::UnloadShader(Shader& program) const
{
  if (program.Loaded() && program.proxy == this)
  {
    glUseProgram(0);
    glDeleteProgram(program.handle);
    program.handle = u4_MAX;
    program.proxy = nullptr;
  }
} // end void RendererGL4::UnloadShader(Shader&) const                        */


void dp::RendererGL4::UnloadTarget(Framebuffer& imageData,
  bool textureToo) const
{
  if (imageData.use != u4_MAX && imageData.proxy == this)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &imageData.use);
    imageData.use = u4_MAX;
    imageData.depthStencil = u4_MAX;
    imageData.proxy = nullptr;
    if (textureToo && imageData.Loaded()) { UnloadTexture(imageData); }
  }
} // end void RendererGL4::UnloadTarget(Framebuffer&, bool) const             */


void dp::RendererGL4::UnloadTexture(Texture& textureData) const
{
  if (textureData.state.proxy == this)
  {
    glDeleteTextures(1, &textureData.state.image);
    textureData.state.image = u4_MAX;
    textureData.state.proxy = nullptr;
  }
  else { Log::Warn("RendererGL4::UnloadTexture - image misload detected"); }

} // end void RendererGL4::UnloadTexture(Texture&) const                      */


void dp::RendererGL4::SetMaterialAttributes(const Material& material)
{
  //GLuint pProgram = cProgram;
  cProgram = material.ShadingGroup().handle;
  glUseProgram(cProgram);
  switch (material.ShadingType())
  {
  case Material::Type::Blinn:
    glUniform3fv(glGetUniformLocation(cProgram, "Ka"), 1,
      &(((Blinn*)&material)->Ambient().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Kd"), 1,
      &(((Blinn*)&material)->Diffuse().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Ki"), 1,
      &(((Blinn*)&material)->Incandescence().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Kf"), 1,
      &(((Blinn*)&material)->Fresnel().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Ks"), 1,
      &(((Blinn*)&material)->Specular().x));
    glUniform1f(glGetUniformLocation(cProgram, "Ns"),
      (((Blinn*)&material)->Falloff()));
    break;
  case Material::Type::Phong:
    glUniform3fv(glGetUniformLocation(cProgram, "Ka"), 1,
      &(((Phong*)&material)->Ambient().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Kd"), 1,
      &(((Phong*)&material)->Diffuse().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Ki"), 1,
      &(((Phong*)&material)->Incandescence().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Kf"), 1,
      &(((Phong*)&material)->Fresnel().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Ks"), 1,
      &(((Phong*)&material)->Specular().x));
    glUniform1f(glGetUniformLocation(cProgram, "Ns"),
      (((Phong*)&material)->Falloff()));
    break;
  case Material::Type::Lambert:
    glUniform3fv(glGetUniformLocation(cProgram, "Ka"), 1,
      &(((Lambert*)&material)->Ambient().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Kd"), 1,
      &(((Lambert*)&material)->Diffuse().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Ki"), 1,
      &(((Lambert*)&material)->Incandescence().x));
    glUniform3fv(glGetUniformLocation(cProgram, "Kf"), 1,
      &(((Lambert*)&material)->Fresnel().x));
    break;
  case Material::Type::Capture:
    // no material uniform attributes defined in framebuffer to set
    break;
  case Material::Type::Skybox:
    // again, no material attributes defined (yet...?)
    break;
  default:
    Log::Info("RendererGL4::SetMaterialAttributes - shader program '"
      + material.ShadingGroup().Name() + "' attributes not considered");
    break;
  }
}
