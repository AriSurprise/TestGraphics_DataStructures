#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <iostream>

#include "src/AABB.h"
#include "src/ASCIIPath.h"
#include "src/Blinn.h"
#include "src/BSphere.h"
#include "src/Camera.h"
#include "src/Cubemap.h"
#include "src/Framebuffer.h"
#include "src/Gbuffer.h"
#include "src/GuiAttr.h"
#include "src/Lambert.h"
#include "src/Log.h"
#include "src/m3f.h"
#include "src/mNf.h"
#include "src/Material.h"
#include "src/Mesh.h"
#include "src/OBB.h"
#include "src/Phong.h"
#include "src/Shader.h"
#include "src/ShaderFile.h"
#include "src/Texture.h"
#include "src/Value.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, dp::Camera& cam);
void ClearTo(const dp::v3f& rgbColor);
void Draw(dp::Mesh& mesh, dp::Material& material, dp::Transform* location,
  dp::Camera* view, dp::Texture* texture);
void Draw(dp::Mesh& mesh, dp::Material& material, const dp::m4f* place,
  const dp::m4f* norm, dp::Camera* view, dp::Texture* texture);
void DrawCubemap(dp::Mesh& mesh, dp::Material& material, dp::Cubemap& skybox,
  dp::Camera* view);
void ImGUIDraw(dp::GuiAttr& ui);
void ImGUIInit(GLFWwindow* win);
void ImGUIShutdown(void);
void LoadCubemap(dp::Cubemap& cube);
void LoadMesh(dp::Mesh& mesh);
void LoadTarget(dp::Framebuffer& renderTarget);
void LoadTarget(dp::Gbuffer& renderTarget);
void LoadTexture(dp::Texture& texture);
void LoadShader(dp::Shader& shaderProgram);
void PollGuiAttrs(dp::Material* setMat, dp::Lambert* lamMat,
  dp::Phong* phnMat, dp::Blinn* blnMat, dp::Material* geoMat);
void Render(dp::Gbuffer& renderTarget, dp::Mesh& NDCQuad, dp::Material&
  gbufferRender, int mode = 0);
bool Target(dp::Framebuffer* renderTarget = nullptr,
  const dp::v3f* clearColor = nullptr);
bool Target(dp::Gbuffer& renderTarget, bool lighting = false);
void UnloadCubemap(dp::Cubemap& skybox);
void UnloadMesh(dp::Mesh& mesh);
void UnloadShader(dp::Shader& program);
void UnloadTarget(dp::Framebuffer& renderTarget);
void UnloadTarget(dp::Gbuffer& renderTarget);
void UnloadTexture(dp::Texture& textureData);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// camera
dp::Camera* cam = nullptr;

dp::GuiAttr UI;

int main()
{
  std::vector<dp::vNf> data[] =
  {
    {
      {1.0f, 6.3f, 1.7f, 8176.0f,  4500.0f},
      {1.0f, 6.3f, 1.7f, 8176.0f,  4500.0f},
      {1.0f, 6.0f, 1.9f, 6699.0f,  3120.0f},
      {1.0f, 5.9f, 1.5f, 9663.0f,  6300.0f},
      {1.0f, 3.0f, 1.2f, 12837.0f, 9800.0f},
      {1.0f, 5.0f, 1.8f, 10205.0f, 4900.0f},
      {1.0f, 6.3f, 2.0f, 14890.0f, 6500.0f},
      {1.0f, 5.6f, 1.6f, 13836.0f, 8920.0f},
      {1.0f, 3.6f, 1.2f, 11628.0f, 14500.0f},
      {1.0f, 2.0f, 1.4f, 15225.0f, 14800.0f},
      {1.0f, 2.9f, 2.3f, 18691.0f, 10900.0f},
      {1.0f, 2.2f, 1.9f, 19350.0f, 16000.0f},
      {1.0f, 3.9f, 2.6f, 20638.0f, 16000.0f},
      {1.0f, 4.5f, 2.0f, 12843.0f, 7800.0f},
      {1.0f, 4.3f, 9.7f, 13384.0f, 17900.0f},
      {1.0f, 4.0f, 2.9f, 13307.0f, 10500.0f},
      {1.0f, 3.2f, 4.3f, 29855.0f, 24500.0f},
      {1.0f, 4.3f, 4.3f, 29277.0f, 30000.0f},
      {1.0f, 2.4f, 2.6f, 24651.0f, 24500.0f},
      {1.0f, 2.8f, 3.7f, 28539.0f, 34000.0f},
      {1.0f, 3.9f, 3.3f, 8085.0f,  8160.0f},
      {1.0f, 2.8f, 3.9f, 30328.0f, 35800.0f},
      {1.0f, 1.6f, 4.1f, 46172.0f, 37000.0f},
      {1.0f, 3.4f, 2.5f, 17836.0f, 19600.0f}
    }
  };
  dp::mNf dM(data[0]);
  dp::mNf tM(data[0][0].DotTranspose());
  std::cout << tM.ToString(true) << std::endl;
  system("PAUSE");
  /*
  // glfw: initialize and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CS 350", NULL, NULL);
  if (window == NULL)
  {
    dp::Log::Error("Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    dp::Log::Error("Failed to initialize GLAD");
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  // build and compile shaders
  dp::Shader skyShader(dp::ShaderFile("../shaders/vertex/Sky.glvs"),
    dp::ShaderFile("../shaders/fragment/Box.glfs"));
  LoadShader(skyShader);
  dp::Shader centerObjCS(dp::ShaderFile("../shaders/vertex/Default.glvs"),
    dp::ShaderFile("../shaders/fragment/Blinn.glfs"));
  LoadShader(centerObjCS);
  dp::Shader capShader(dp::ShaderFile("../shaders/vertex/Screen.glvs"),
    dp::ShaderFile("../shaders/fragment/Capture.glfs"));
  LoadShader(capShader);
  dp::Shader geomShader(dp::ShaderFile("../shaders/vertex/Deferred.glvs"),
    dp::ShaderFile("../shaders/fragment/Geometry.glfs"));
  LoadShader(geomShader);
  dp::Shader gLightShader(dp::ShaderFile("../shaders/vertex/Screen.glvs"),
    dp::ShaderFile("../shaders/fragment/GeoLighting.glfs"));
  LoadShader(gLightShader);
  dp::Shader lamShader(dp::ShaderFile("../shaders/vertex/Default.glvs"),
    dp::ShaderFile("../shaders/fragment/Lambert.glfs"));
  LoadShader(lamShader);
  dp::Shader phnShader(dp::ShaderFile("../shaders/vertex/Default.glvs"),
    dp::ShaderFile("../shaders/fragment/Phong.glfs"));
  LoadShader(phnShader);
  dp::Shader blnShader(dp::ShaderFile("../shaders/vertex/Default.glvs"),
    dp::ShaderFile("../shaders/fragment/Blinn.glfs"));
  LoadShader(blnShader);

  dp::Material* skyMat = new
    dp::Material(dp::Material::Type::Skybox, skyShader);
  dp::Material* cObjMat = new
    dp::Material(dp::Material::Type::Capture, centerObjCS);
  dp::Material* capMat = new dp::Material(dp::Material::Type::Capture,
    capShader);
  dp::Material* geoMat = new dp::Material(dp::Material::Type::Capture,
    geomShader);
  dp::Material* lgtMat = new dp::Material(dp::Material::Type::Blinn,
    gLightShader);

  dp::Lambert* lambert = new dp::Lambert(lamShader);
  dp::Blinn* blinn = new dp::Blinn(blnShader);
  dp::Phong* phong = new dp::Phong(phnShader);

  dp::Material* curMat = blinn;

  // set up vertex data (and buffer(s)) and configure vertex attributes
  dp::Mesh* centerObj = new dp::Mesh("../Common/models/bunny.obj");
  {
    // object mutations to fit file loaded data to scene standards
    dp::Mesh* temp = new dp::Mesh(centerObj->NormalizedCopy());
    temp->UVCylindrical(dp::Y_HAT);
    delete centerObj;
    centerObj = temp;
    temp = nullptr;
  }
  LoadMesh(*centerObj);

  dp::Mesh* quad = new dp::Mesh();
  LoadMesh(*quad);

  dp::Mesh* cube = new dp::Mesh(dp::Mesh::Cube(1.0f, false, true));
  LoadMesh(*cube);

  dp::Mesh* box = new dp::Mesh(dp::Mesh::Cube(2.0f, true, false));
  LoadMesh(*box);

  dp::Mesh* ball = new dp::Mesh(dp::Mesh::pSphere(24, 16, -1.0f));
  LoadMesh(*ball);

  // load textures
  dp::Framebuffer* sceneRender = new dp::Framebuffer(dp::BLACK.Opaque(),
    SCR_WIDTH, SCR_HEIGHT, dp::Texture::ImageMode::RGBA);
  LoadTarget(*sceneRender);
  dp::Gbuffer* deferredRender = new dp::Gbuffer(dp::BLACK.Opaque(),
    SCR_WIDTH, SCR_HEIGHT, dp::Texture::ImageMode::RGBA);
  LoadTarget(*deferredRender);
  dp::Texture* uvTex = new
    dp::Texture(dp::ASCIIPath("../Common/textures/UV_Grid.png"), false);
  LoadTexture(*uvTex);
  dp::Texture* genTex = new
    dp::Texture(dp::GRAY_70.Opaque());
  LoadTexture(*genTex);

  dp::Cubemap* skyTex = new dp::Cubemap(
    dp::ASCIIPath("../Common/textures/sky_right.jpg"),
    dp::ASCIIPath("../Common/textures/sky_left.jpg"),
    dp::ASCIIPath("../Common/textures/sky_top.jpg"),
    dp::ASCIIPath("../Common/textures/sky_bottom.jpg"),
    dp::ASCIIPath("../Common/textures/sky_front.jpg"),
    dp::ASCIIPath("../Common/textures/sky_back.jpg"));

  LoadCubemap(*skyTex);

  cam = new dp::Camera(dp::v3f(0.0f, 0.0f, 3.0f));

  dp::Transform centered;
  dp::Transform screenScale(dp::nullv3f, dp::v3f(3.556f, 2.0f, 1.0f));
  dp::AABB CenterAABB(*centerObj, centered.Matrix(), u4_MAX, 1, 0);
  dp::BSphere CenterBSphere(*centerObj, centered.Matrix(),
    centerObj->VertexCount(), 1);
  dp::OBB CenterBBox(*centerObj, centered.Matrix());

  // shader initial configuration (values not changing per frame)
  u4 handle;

  glUseProgram(skyShader.handle);
  handle = glGetUniformLocation(skyShader.handle, "skybox");
  if (handle != -1) { glUniform1i(handle, 0); }
  else { dp::Log::Warn("skybox not found in skybox shader"); }
  glUseProgram(lgtMat->ShadingGroup().handle);
  handle = glGetUniformLocation(lgtMat->ShadingGroup().handle, "gPosition");
  if (handle == -1) { dp::Log::Warn("gPosition not found in light pass"); }
  else { glUniform1i(handle, 0); }
  handle = glGetUniformLocation(lgtMat->ShadingGroup().handle, "gNormal");
  if (handle == -1) { dp::Log::Warn("gNormal not found in light pass"); }
  else { glUniform1i(handle, 1); }
  handle = glGetUniformLocation(lgtMat->ShadingGroup().handle, "gDiffuse");
  if (handle == -1) { dp::Log::Warn("gDiffuse not found in light pass"); }
  else { glUniform1i(handle, 2); }
  handle = glGetUniformLocation(lgtMat->ShadingGroup().handle, "gSpecular");
  if (handle == -1) { dp::Log::Warn("gSpecular not found in light pass"); }
  else { glUniform1i(handle, 3); }
  handle = glGetUniformLocation(lgtMat->ShadingGroup().handle, "gIncandescent");
  if (handle == 4-1) { dp::Log::Warn("gIncandescent not found in light pass"); }
  else { glUniform1i(handle, 4); }
  handle = glGetUniformLocation(lgtMat->ShadingGroup().handle, "gFresnel");
  if (handle == -1) { dp::Log::Warn("gFresnel not found in light pass"); }
  else { glUniform1i(handle, 5); }

  ImGUIInit(window);

  dp::m4f bah(CenterAABB.Matrix().Linear().Inverse().Transpose());
  dp::m4f nah(CenterBSphere.Matrix().Linear().Inverse().Transpose());
  dp::m4f gah(CenterBBox.Matrix().Linear().Inverse().Transpose());

  // render loop
  while (!glfwWindowShouldClose(window))
  {
    // perform per-frame time delta calculations & input updates
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(window, *cam);

    // render
    ClearTo(dp::BLACK);
    PollGuiAttrs(curMat, lambert, phong, blinn, lgtMat);
    //Target(*deferredRender);
    Draw(*centerObj, *geoMat, &centered, cam, genTex);
    switch (UI.cBVPrev)
    {
    case 1:
      if (UI.eposPrev != UI.eposCurrent)
      {
        UI.eposPrev = UI.eposCurrent;
        switch (UI.eposPrev)
        {
        case 0:
          CenterBSphere = dp::BSphere(*centerObj, centered.Matrix(),
            dp::EPOS::_6, UI.samplesPrev, UI.stepSPrev, UI.firstSPrev);
          break;
        case 1:
          CenterBSphere = dp::BSphere(*centerObj, centered.Matrix(),
            dp::EPOS::_14, UI.samplesPrev, UI.stepSPrev, UI.firstSPrev);
          break;
        case 2:
          CenterBSphere = dp::BSphere(*centerObj, centered.Matrix(),
            dp::EPOS::_26, UI.samplesPrev, UI.stepSPrev, UI.firstSPrev);
          break;
        case 3:
          CenterBSphere = dp::BSphere(*centerObj, centered.Matrix(),
            dp::EPOS::_50, UI.samplesPrev, UI.stepSPrev, UI.firstSPrev);
          break;
        case 4:
          CenterBSphere = dp::BSphere(*centerObj, centered.Matrix(),
            dp::EPOS::_74, UI.samplesPrev, UI.stepSPrev, UI.firstSPrev);
          break;
        case 5:
          CenterBSphere = dp::BSphere(*centerObj, centered.Matrix(),
            dp::EPOS::_98, UI.samplesPrev, UI.stepSPrev, UI.firstSPrev);
          break;
        default:
          CenterBSphere = dp::BSphere(*centerObj, centered.Matrix(),
            UI.samplesPrev, UI.stepSPrev, UI.firstSPrev);
          break;
        }
      }
      Draw(*ball, *lambert, &CenterBSphere.Matrix(), &nah, cam, genTex);
      break;
    case 2:
      Draw(*box, *lambert, &CenterBBox.Matrix(), &gah, cam, genTex);
      break;
    default:
      Draw(*box, *lambert, &CenterAABB.Matrix(), &bah, cam, genTex);
      break;
    }
    //Target();
    //Render(*deferredRender, *quad, *lgtMat, UI.outputCurrent - 1);
    DrawCubemap(*cube, *skyMat, *skyTex, cam);
    //Draw(*quad, *capMat, &screenScale, cam, deferredRender);
    ImGUIDraw(UI);
    // glfw: post rendered contents
    glfwSwapBuffers(window);
    glfwPollEvents();
  }


  // cleanup on close
  ImGUIShutdown();
  UnloadMesh(*ball);
  delete ball;
  UnloadMesh(*box);
  delete box;
  UnloadMesh(*cube);
  delete cube;
  UnloadMesh(*quad);
  delete quad;
  UnloadMesh(*centerObj);
  delete centerObj;
  UnloadTexture(*genTex);
  delete genTex;
  UnloadTexture(*uvTex);
  delete uvTex;
  UnloadTarget(*sceneRender);
  delete sceneRender;
  UnloadTarget(*deferredRender);
  delete deferredRender;
  UnloadCubemap(*skyTex);
  delete skyTex;
  UnloadShader(skyShader);
  delete skyMat;
  UnloadShader(centerObjCS);
  delete cObjMat;
  UnloadShader(geomShader);
  delete geoMat;
  UnloadShader(gLightShader);
  delete lgtMat;

  glfwTerminate();
  */
  return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);
    firstMouse = false;
  }
  float xoffset = static_cast<float>(xpos) - lastX;
  float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

  lastX = static_cast<float>(xpos);
  lastY = static_cast<float>(ypos);
  if (cam && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
  {
    //camera.ProcessMouseMovement(xoffset, yoffset);
    cam->Pitch(dp::ang(yoffset * 0.01f, dp::arc::radians));
    cam->Yaw(dp::ang(-xoffset * 0.01f, dp::arc::radians));
  }
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window, dp::Camera& cam)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    //camera.ProcessKeyboard(Camera::Movement::FORWARD, deltaTime);
    cam.Dolly(-2.5f * deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    //camera.ProcessKeyboard(Camera::Movement::BACKWARD, deltaTime);
    cam.Dolly(2.5f * deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    //camera.ProcessKeyboard(Camera::Movement::LEFT, deltaTime);
    cam.Pan(-2.5f * deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    //camera.ProcessKeyboard(Camera::Movement::RIGHT, deltaTime);
    cam.Pan(2.5f * deltaTime);
  }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  //camera.ProcessMouseScroll(yoffset);
}

void ClearTo(const dp::v3f& rgbColor)
{
  glClearColor(rgbColor.r, rgbColor.g, rgbColor.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

} // end void ClearTo(const v3f&)

void Draw(dp::Mesh& mesh, dp::Material& material,
  dp::Transform* location, dp::Camera* view, dp::Texture* texture)
{
  glUseProgram(material.ShadingGroup().handle);
  if (view)
  {
    GLuint handle;
    // Set view matrix
    if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "vFw")) != -1)
    {
      glUniformMatrix4fv(handle, 1, false, &view->View()[0][0]);
    }
    // Sky shader takes view linearly from model coords instead of affine world
    else if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "vFm")) != -1)
    {
      glUniformMatrix4fv(handle, 1, 0, (GLfloat*)&(view->View().Linear()[0][0]));
    }
    // Set persp matrix
    if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "pFv")) != -1)
    {
      glUniformMatrix4fv(handle, 1, false, &view->Persp()[0][0]);
    }
  }
  if (location)
  {
    GLuint handle = glGetUniformLocation(material.ShadingGroup().handle, "wFm");
    if (handle != -1)
    {
      glUniformMatrix4fv(handle, 1, false, &location->Matrix()[0][0]);
    }
    handle = glGetUniformLocation(material.ShadingGroup().handle, "nFm");
    if (handle == -1)
    {
      handle = glGetUniformLocation(material.ShadingGroup().handle, "wFn");
    }
    if (handle != -1)
    {
      glUniformMatrix4fv(handle, 1, false, &location->NormalMatrix()[0][0]);
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

}void Draw(dp::Mesh& mesh, dp::Material& material, const dp::m4f* place,
  const dp::m4f* norm, dp::Camera* view, dp::Texture* texture)
{
  glUseProgram(material.ShadingGroup().handle);
  if (view)
  {
    GLuint handle;
    // Set view matrix
    if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "vFw")) != -1)
    {
      glUniformMatrix4fv(handle, 1, false, &view->View()[0][0]);
    }
    // Sky shader takes view linearly from model coords instead of affine world
    else if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "vFm")) != -1)
    {
      glUniformMatrix4fv(handle, 1, 0, (GLfloat*)&(view->View().Linear()[0][0]));
    }
    // Set persp matrix
    if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "pFv")) != -1)
    {
      glUniformMatrix4fv(handle, 1, false, &view->Persp()[0][0]);
    }
  }
  if (place)
  {
    GLuint handle = glGetUniformLocation(material.ShadingGroup().handle, "wFm");
    if (handle != -1)
    {
      glUniformMatrix4fv(handle, 1, false, (GLfloat*)&(*place)[0][0]);
    }
    handle = glGetUniformLocation(material.ShadingGroup().handle, "nFm");
    if (handle == -1)
    { handle = glGetUniformLocation(material.ShadingGroup().handle, "wFn"); }
    if (norm && handle != -1)
    {
      glUniformMatrix4fv(handle, 1, false, (GLfloat*)&(*norm)[0][0]);
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

}

void DrawCubemap(dp::Mesh& mesh, dp::Material& material, dp::Cubemap&
  skybox, dp::Camera* view)
{
  glDisable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);
  glUseProgram(material.ShadingGroup().handle);
  if (view)
  {
    GLuint handle;

    // Set world->view matrix
    if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "vFw")) != -1)
    {
      glUniformMatrix4fv(handle, 1, false, &view->View().Linear()[0][0]);
    }
    // Set persp matrix
    if ((handle = glGetUniformLocation(material.ShadingGroup().handle, "pFv")) != -1)
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

} // end void DrawCubemap(Mesh&, Material&, Cubemap&, Camera*)

void ImGUIDraw(dp::GuiAttr& ui)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  {
    ImGui::Begin("Settings");
    ImGui::Text("Running at %.1f FPS : %.3f ms/frame ",
      ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Text("");
    /*
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
    */
    //ImGui::Text("Press [space] to start / stop rotation");
    ImGui::Text("WASD: camera pan / zoom (dolly)");
    ImGui::Text("Alt + LMB - Camera Tumble:");
    ImGui::Text("    Horizontal - Yaw");
    ImGui::Text("    Vertical - Pitch");
    ImGui::Text("    Rotations - Roll (spin to fix up)");
    ImGui::SliderInt("Samples", &ui.samplesCurrent, 1, s4_MAX>>4);
    ImGui::SliderInt("Step", &ui.stepSCurrent, s1_MIN, s1_MAX);
    ImGui::SliderInt("First Sample", &ui.firstSCurrent, 0, s2_MAX);
    ImGui::Combo("Bounding Volume Type", &ui.cBVCurrent, ui.cBVType,
      IM_ARRAYSIZE(ui.cBVType));
    if (ui.cBVCurrent == 1) // Draw bounding sphere mode
    {
      ImGui::Combo("Scan Method", &ui.eposCurrent, ui.bSphereMethod,
        IM_ARRAYSIZE(ui.bSphereMethod));
    }
    else
    {
      ImGui::Text("");
      ImGui::Text("");
    }
    /*
    ImGui::Combo("GPU UV Mapping", &ui.uvMCurrent, ui.uvMode,
      IM_ARRAYSIZE(ui.uvMode));
    ImGui::Text("      X            Y            Z");
    ImGui::SliderFloat3("UV Projection Axis", (float*)&ui.axisCurrent, -1.0f,
      1.0f, "-1   0   +1", 1.0f);
    ImGui::Text(("  " + ui.axisCurrent.String()).c_str());
    ImGui::Combo("Deferred render mode", &ui.outputCurrent, ui.outputMode,
      IM_ARRAYSIZE(ui.outputMode));
    */
    ImGui::End();
  }
  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGUIInit(GLFWwindow* win)
{
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(win, true);
  ImGui_ImplOpenGL3_Init();

}

void ImGUIShutdown(void)
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

}

void LoadMesh(dp::Mesh& mesh)
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
  mesh.state.proxy = nullptr; // this pointer not available for comparable load
} // end void LoadMesh(Mesh&)

// Load framebuffer onto GPU (default framebuffer restored as current target)
void LoadTarget(dp::Framebuffer& renderTarget)
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
      renderTarget.state.proxy = nullptr; // (dp::Renderer*)(this);
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
      renderTarget.Width(), renderTarget.Height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
      GL_RENDERBUFFER, rbo);
    // check attachment succeeded / framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      dp::Log::Error("RendererGLCore - framebuffer not complete");
    }
    else
    {
      renderTarget.use = fbo;
      renderTarget.depthStencil = rbo;
      renderTarget.proxy = nullptr; // (dp::Renderer*)(this);
    }
  }
  else
  {
    dp::Log::Warn("RendererGLCore::LoadTarget - framebuffer already loaded");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

} // end void LoadTarget(Framebuffer&)

// Load geometrybuffer onto GPU (default framebuffer restored as current target)
void LoadTarget(dp::Gbuffer& renderTarget)
{
  const u4 dim = GL_TEXTURE_2D, biD = GL_FRAMEBUFFER, wrt = GL_RENDERBUFFER,
    md = GL_RGBA, sz = GL_UNSIGNED_BYTE, typ = GL_FLOAT;
  u4 gBuffer, position, normal, diffuse, specular, incandescent, fresnel;
  glGenFramebuffers(1, &gBuffer);
  glBindFramebuffer(biD, gBuffer);
  // position color buffer
  glGenTextures(1, &position);
  glBindTexture(dim, position);
  glTexImage2D(dim, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, md, typ, NULL);
  glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(biD, GL_COLOR_ATTACHMENT0, dim, position, 0);
  // normal color buffer
  glGenTextures(1, &normal);
  glBindTexture(dim, normal);
  glTexImage2D(dim, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, md, typ, NULL);
  glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(biD, GL_COLOR_ATTACHMENT1, dim, normal, 0);
  // diffuse material RGB color (& ambient R) buffer
  glGenTextures(1, &diffuse);
  glBindTexture(dim, diffuse);
  glTexImage2D(dim, 0, md, SCR_WIDTH, SCR_HEIGHT, 0, md, sz, NULL);
  glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(biD, GL_COLOR_ATTACHMENT2, dim, diffuse, 0);
  // specular material RGB color (& intensity) buffer
  glGenTextures(1, &specular);
  glBindTexture(dim, specular);
  glTexImage2D(dim, 0, md, SCR_WIDTH, SCR_HEIGHT, 0, md, sz, NULL);
  glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(biD, GL_COLOR_ATTACHMENT3, dim, specular, 0);
  // incandescent material RGB color (& ambient g) buffer
  glGenTextures(1, &incandescent);
  glBindTexture(dim, incandescent);
  glTexImage2D(dim, 0, md, SCR_WIDTH, SCR_HEIGHT, 0, md, sz, NULL);
  glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(biD, GL_COLOR_ATTACHMENT4, dim, incandescent, 0);
  // fresnel material RGB color (& ambient b) buffer
  glGenTextures(1, &fresnel);
  glBindTexture(dim, fresnel);
  glTexImage2D(dim, 0, md, SCR_WIDTH, SCR_HEIGHT, 0, md, sz, NULL);
  glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(biD, GL_COLOR_ATTACHMENT5, dim, fresnel, 0);
  // create and attach depth buffer (renderbuffer)
  unsigned int rboDepth;
  glGenRenderbuffers(1, &rboDepth);
  glBindRenderbuffer(wrt, rboDepth);
  glRenderbufferStorage(wrt, GL_DEPTH_COMPONENT, SCR_WIDTH,
    SCR_HEIGHT);
  // tell OpenGL which attachments we'll use (of this framebuffer) for rendering 
  unsigned int attachments[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5 };
  glDrawBuffers(6, attachments);
  glFramebufferRenderbuffer(biD, GL_DEPTH_ATTACHMENT, wrt, rboDepth);
  // finally check if framebuffer is complete
  if (glCheckFramebufferStatus(biD) != GL_FRAMEBUFFER_COMPLETE)
  {
    dp::Log::Warn("LoadTarget - framebuffer not complete!");
  }
  else
  {
    renderTarget.gUse = gBuffer;
    renderTarget.gPos = position;
    renderTarget.gNorm = normal;
    renderTarget.gDiff = diffuse;
    renderTarget.gSpec = specular;
    renderTarget.gGlow = incandescent;
    renderTarget.gFres = fresnel;
  }
  glBindFramebuffer(biD, 0);
  LoadTarget((dp::Framebuffer&)renderTarget);

} // end void LoadTarget(Gbuffer&)

// utility function for loading a 2D texture from file
void LoadTexture(dp::Texture& texture)
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
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture.state.image = textureID;
    texture.state.proxy = nullptr; // this pointer not available for comparable load
  }
  else
  {
    dp::Log::Error("Texture failed to load at path: " + texture.FilePath().RunningPath());
  }
  return;
} // end void LoadTexture(Texture&)

// uploads a cubemap to the GPU
void LoadCubemap(dp::Cubemap& cube)
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
  cube.proxy = nullptr; // this pointer not available for comparable load

}

void LoadShader(dp::Shader& shaderProgram)
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
    dp::Log::Error("RendererGLCore::LoadShader - vertex compilation failed:\n"
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
      "RendererGLCore(GLFWwindow...) - fragment compilation failed:\n"
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
    dp::Log::Error("RendererGLCore(GLFWwindow...) - linking failed:\n"
      + std::string(infoLog));
  }
  shaderProgram.handle = program;
  shaderProgram.proxy = nullptr;
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
      dp::Log::Warn("RendererGLCore - no match in " + shaderProgram.Name()
        + " for " + attrText +
        ": it may have been optimized away, did you use it code?");
      shaderProgram.Global(i).GPUValue(-1);
    }
    else
    {
      shaderProgram.Global(i).GPUValue(static_cast<s4>(result));
    }
  }
} // end void LoadShader(Shader&)

void PollGuiAttrs(dp::Material* setMat, dp::Lambert* lamMat,
  dp::Phong* phnMat, dp::Blinn* blnMat, dp::Material* geoMat)
{
  if (UI.shadPrev != UI.shadCurr)
  {
    switch (UI.shadCurr)
    {
    case 0: // [0] = "Lambert"
      setMat = lamMat;
      UI.ambientCurrent = lamMat->Ambient();
      UI.diffuseCurrent = lamMat->Diffuse();
      UI.incandescenceCurrent = lamMat->Incandescence();
      UI.fresnelCurrent = lamMat->Fresnel();
      break;
    case 1: // [1] = "Phong"
      setMat = phnMat;
      UI.ambientCurrent = phnMat->Ambient();
      UI.diffuseCurrent = phnMat->Diffuse();
      UI.incandescenceCurrent = phnMat->Incandescence();
      UI.fresnelCurrent = phnMat->Fresnel();
      UI.specularCurrent = phnMat->Specular();
      UI.falloffCurrent = phnMat->Falloff();
      break;
    case 2: // [2] = "Blinn"
      setMat = blnMat;
      UI.ambientCurrent = blnMat->Ambient();
      UI.diffuseCurrent = blnMat->Diffuse();
      UI.incandescenceCurrent = blnMat->Incandescence();
      UI.fresnelCurrent = blnMat->Fresnel();
      UI.specularCurrent = blnMat->Specular();
      UI.falloffCurrent = blnMat->Falloff();
      break;
    }
    UI.shadPrev = UI.shadCurr;
  }
  GLuint cProgram = setMat->ShadingGroup().handle;
  if (UI.uvMPrev != UI.uvMCurrent)
  {
    GLuint uvs = glGetUniformLocation(cProgram, "proceduralUVs");
    if (uvs != -1) { glUniform1i(uvs, UI.uvMCurrent); }
    else { dp::Log::Info("RendererGLCore::ImGUIDraw - uv mode unavailable"); }
    UI.uvMPrev = UI.uvMCurrent;
  }
  if (UI.falloffPrev != UI.falloffCurrent)
  {
    switch (setMat->ShadingType())
    {
    case dp::Material::Type::Phong:
      phnMat->Falloff(UI.falloffCurrent);
      break;
    case dp::Material::Type::Blinn:
      blnMat->Falloff(UI.falloffCurrent);
      break;
    }
    UI.falloffPrev = UI.falloffCurrent;
  }
  if (UI.ambientPrev != UI.ambientCurrent)
  {
    switch (setMat->ShadingType())
    {
    case dp::Material::Type::Lambert:
      lamMat->Ambient(UI.ambientCurrent);
      break;
    case dp::Material::Type::Phong:
      phnMat->Ambient(UI.ambientCurrent);
      break;
    case dp::Material::Type::Blinn:
      blnMat->Ambient(UI.ambientCurrent);
      break;
    }
    UI.ambientPrev = UI.ambientCurrent;
  }
  if (UI.diffusePrev != UI.diffuseCurrent)
  {
    switch (setMat->ShadingType())
    {
    case dp::Material::Type::Lambert:
      lamMat->Diffuse(UI.diffuseCurrent);
      break;
    case dp::Material::Type::Phong:
      phnMat->Diffuse(UI.diffuseCurrent);
      break;
    case dp::Material::Type::Blinn:
      blnMat->Diffuse(UI.diffuseCurrent);
      break;
    }
    UI.diffusePrev = UI.diffuseCurrent;
  }
  if (UI.specularPrev != UI.specularCurrent)
  {
    switch (setMat->ShadingType())
    {
    case dp::Material::Type::Phong:
      phnMat->Specular(UI.specularCurrent);
      break;
    case dp::Material::Type::Blinn:
      blnMat->Specular(UI.specularCurrent);
      break;
    }
    UI.specularPrev = UI.specularCurrent;
  }
  if (UI.incandescencePrev != UI.incandescenceCurrent)
  {
    switch (setMat->ShadingType())
    {
    case dp::Material::Type::Lambert:
      lamMat->Incandescence(UI.incandescenceCurrent);
      break;
    case dp::Material::Type::Phong:
      phnMat->Incandescence(UI.incandescenceCurrent);
      break;
    case dp::Material::Type::Blinn:
      blnMat->Incandescence(UI.incandescenceCurrent);
      break;
    }
    UI.incandescencePrev = UI.incandescenceCurrent;
  }
  if (UI.fresnelPrev != UI.fresnelCurrent)
  {
    switch (setMat->ShadingType())
    {
    case dp::Material::Type::Lambert:
      lamMat->Fresnel(UI.fresnelCurrent);
      break;
    case dp::Material::Type::Phong:
      phnMat->Fresnel(UI.fresnelCurrent);
      break;
    case dp::Material::Type::Blinn:
      blnMat->Fresnel(UI.fresnelCurrent);
      break;
    }
    UI.fresnelPrev = UI.fresnelCurrent;
  }
  if (UI.axisPrev != UI.axisCurrent)
  {
    dp::m4f rotBack;
    UI.axisCurrent.Normalize();
    GLuint uvBasis = glGetUniformLocation(cProgram, "uvB");
    if (uvBasis != -1)
    {
      // if (axisCurrent == Z_HAT) { /* identity leaves z aligned with z */ }
      if (UI.axisCurrent == -dp::Z_HAT) // Anti-parallel axis check
      {
        rotBack = dp::RotateRodriguesAffine(dp::ang(180.0f,dp::arc::deg),
          dp::X_HAT);
      }
      else // Arbitrary axis rotation
      {
        dp::v3f axis = UI.axisCurrent.Cross(dp::Z_HAT);
        dp::ang angle(acos(UI.axisCurrent.Dot(dp::Z_HAT)), dp::arc::rad);
        rotBack = RotateRodriguesAffine(angle, axis);
        if (rotBack * UI.axisCurrent.XYZW(Of) == -dp::UNIT_VEC_Z)
        {
          rotBack = RotateRodriguesAffine(-angle, axis);
        }
        dp::v4f testVec = rotBack * UI.axisCurrent.XYZW(Of);
        if (testVec != dp::UNIT_VEC_Z)
        {
          dp::v3f out = testVec.XYZ();
          dp::Log::Warn("RendererGLCore::ImGUIDraw - rotation yielded "
            + out.String());
        }
      }
      glUniformMatrix4fv(uvBasis, 1, GL_FALSE, &rotBack[0][0]);
    }
    else { dp::Log::Warn("RendererGLCore::ImGUIDraw - uv basis unavailable"); }
    UI.axisPrev = UI.axisCurrent;
  }
  if (UI.outputCurrent != UI.outputPrev)
  {
    if (UI.outputCurrent != 0)
    {
      u4 outputCode = UI.outputCurrent - 1; // forward render has 1 mode
      if (geoMat)
      {
        u4 handle = glGetUniformLocation(geoMat->ShadingGroup().handle, "mode");
        if (handle != -1)
        {
          glUniform1i(handle, outputCode);
        }
      }
    }
    UI.outputPrev = UI.outputCurrent;
  }
  if (UI.cBVPrev != UI.cBVCurrent)
  {
    UI.cBVPrev = UI.cBVCurrent;
  }
  if (UI.samplesPrev != UI.samplesCurrent)
  {
    UI.samplesPrev = UI.samplesCurrent;
  }
  if (UI.stepSPrev != UI.stepSCurrent)
  {
    UI.stepSPrev = UI.stepSCurrent;
  }
  if (UI.firstSPrev != UI.firstSCurrent)
  {
    UI.firstSPrev = UI.firstSCurrent;
  }
} // end void PollGuiAttrs(void)

void Render(dp::Gbuffer& renderTarget, dp::Mesh& NDCQuad, dp::Material&
  gbufferRender, int mode)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(gbufferRender.ShadingGroup().handle);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, renderTarget.gPos);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, renderTarget.gNorm);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, renderTarget.gDiff);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, renderTarget.gSpec);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, renderTarget.gGlow);
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D, renderTarget.gFres);
  u4 handle = glGetUniformLocation(gbufferRender.ShadingGroup().handle, "mode");
  if (handle != -1)
  {
    glUniform1i(handle, mode);
  }
  glBindVertexArray(NDCQuad.state.record);
  glDrawElements(GL_TRIANGLES, (GLsizei)NDCQuad.TriIndices(), GL_UNSIGNED_INT, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, renderTarget.gUse);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT,
    GL_DEPTH_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindVertexArray(0);
}

bool Target(dp::Framebuffer* renderTarget, const dp::v3f* clearColor)
{
  if (renderTarget)
  {
    if (renderTarget->use == u4_MAX /*|| renderTarget->proxy == nullptr */)
    {
      LoadTarget(*renderTarget);
    }
    if (renderTarget->use != u4_MAX /*&& renderTarget->proxy == this*/)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->use);
      glEnable(GL_DEPTH_TEST);
      glClearDepth(1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      return true;
    }
    else
    {
      dp::Log::Warn("RendererGLCore::Target - couldn't load framebuffer");
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

} // end bool Target(Framebuffer*, const dp::v3f*)

bool Target(dp::Gbuffer& renderTarget, bool postRender)
{
  if (postRender)
  {
    //dp::Log::N("Post\n");
    glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.use);
    ClearTo(dp::BLACK);
  }
  else
  {
    //dp::Log::N("Pre\n");
    glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.gUse);
    ClearTo(dp::BLACK);
  }
  return true;
}

void UnloadCubemap(dp::Cubemap& skybox)
{
  for (u4 i = 0; i < dp::Cubemap::FACES; ++i)
  {
    UnloadTexture(skybox[i]);
  }
  skybox.use = u4_MAX;
}

void UnloadMesh(dp::Mesh& mesh)
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
}

void UnloadShader(dp::Shader& program)
{
  glUseProgram(0);
  glDeleteProgram(program.handle);
  program.handle = u4_MAX;
}

void UnloadTarget(dp::Framebuffer& imageData)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &imageData.use);
  imageData.use = u4_MAX;
  imageData.depthStencil = u4_MAX;
  imageData.proxy = nullptr;
  UnloadTexture(imageData);

} // end void UnloadTarget(Framebuffer&)

void UnloadTarget(dp::Gbuffer& imageData)
{
  if (imageData.gDiff != u4_MAX)
  {
    glDeleteTextures(1, &imageData.gDiff);
    imageData.gDiff = u4_MAX;
  }
  if (imageData.gSpec != u4_MAX)
  {
    glDeleteTextures(1, &imageData.gSpec);
    imageData.gSpec = u4_MAX;
  }
  if (imageData.gGlow != u4_MAX)
  {
    glDeleteTextures(1, &imageData.gGlow);
    imageData.gGlow = u4_MAX;
  }
  if (imageData.gFres != u4_MAX)
  {
    glDeleteTextures(1, &imageData.gFres);
    imageData.gFres = u4_MAX;
  }
  if (imageData.gPos != u4_MAX)
  {
    glDeleteTextures(1, &imageData.gPos);
    imageData.gPos = u4_MAX;
  }
  if (imageData.gNorm != u4_MAX)
  {
    glDeleteTextures(1, &imageData.gNorm);
    imageData.gNorm = u4_MAX;
  }
  if (imageData.gUse != u4_MAX)
  {
    glDeleteTextures(1, &imageData.gUse);
    imageData.gUse = u4_MAX;
  }
  UnloadTarget((dp::Framebuffer&)imageData);

} // end void UnloadTarget(Gbuffer&)

void UnloadTexture(dp::Texture& textureData)
{
  glDeleteTextures(1, &textureData.state.image);
  textureData.state.image = u4_MAX;
  textureData.state.proxy = nullptr;
}
