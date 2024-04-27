/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  InputGLFW.h
Purpose:  Processing of GLFW controller input decoupled from window context
Project:  a.Surprise_CS350_1
Author:   Ari Surprise, a.surprise, 40000218
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Creation date:  Summer 2022
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <../Packages/include/...>
#include <glad\glad.h>  // GLAD OGL wrangler (for GL 4.3)
#include <GLFW\glfw3.h> // GLFW windowing package
// <./src/...>
#include "InputGLFW.h"  // Class declarations header
#include "Log.h"        // Error / warning message output

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::InputGLFW::InputGLFW(Window* parent, GLFWwindow* window)
  : Input(parent), windowContext(window), lastTime(0.0f), dt(0.0f)
{
  done = (windowContext) ? false : true;
} // end InputGLFW(GLFWwindow*)                                               */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Globals                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

const u4 dp::InputGLFW::GLKey[] = 
  {
    GLFW_KEY_SPACE,
    GLFW_KEY_APOSTROPHE,
    GLFW_KEY_COMMA,
    GLFW_KEY_MINUS,
    GLFW_KEY_PERIOD,
    GLFW_KEY_SLASH,
    GLFW_KEY_0,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,
    GLFW_KEY_5,
    GLFW_KEY_6,
    GLFW_KEY_7,
    GLFW_KEY_8,
    GLFW_KEY_9,
    GLFW_KEY_SEMICOLON,
    GLFW_KEY_EQUAL,
    GLFW_KEY_A,
    GLFW_KEY_B,
    GLFW_KEY_C,
    GLFW_KEY_D,
    GLFW_KEY_E,
    GLFW_KEY_F,
    GLFW_KEY_G,
    GLFW_KEY_H,
    GLFW_KEY_I,
    GLFW_KEY_J,
    GLFW_KEY_K,
    GLFW_KEY_L,
    GLFW_KEY_M,
    GLFW_KEY_N,
    GLFW_KEY_O,
    GLFW_KEY_P,
    GLFW_KEY_Q,
    GLFW_KEY_R,
    GLFW_KEY_S,
    GLFW_KEY_T,
    GLFW_KEY_U,
    GLFW_KEY_V,
    GLFW_KEY_W,
    GLFW_KEY_X,
    GLFW_KEY_Y,
    GLFW_KEY_Z,
    GLFW_KEY_LEFT_BRACKET,
    GLFW_KEY_BACKSLASH,
    GLFW_KEY_RIGHT_BRACKET,
    GLFW_KEY_GRAVE_ACCENT,
    GLFW_KEY_WORLD_1,
    GLFW_KEY_WORLD_2,
    GLFW_KEY_ESCAPE,
    GLFW_KEY_ENTER,
    GLFW_KEY_TAB,
    GLFW_KEY_BACKSPACE,
    GLFW_KEY_INSERT,
    GLFW_KEY_DELETE,
    GLFW_KEY_RIGHT,
    GLFW_KEY_LEFT,
    GLFW_KEY_DOWN,
    GLFW_KEY_UP,
    GLFW_KEY_PAGE_UP,
    GLFW_KEY_PAGE_DOWN,
    GLFW_KEY_HOME,
    GLFW_KEY_END,
    GLFW_KEY_CAPS_LOCK,
    GLFW_KEY_SCROLL_LOCK,
    GLFW_KEY_NUM_LOCK,
    //GLFW_KEY_PRINT_SCREEN, // Fails to detect anyhow?
    GLFW_KEY_PAUSE,
    GLFW_KEY_F1,
    GLFW_KEY_F2,
    GLFW_KEY_F3,
    GLFW_KEY_F4,
    GLFW_KEY_F5,
    GLFW_KEY_F6,
    GLFW_KEY_F7,
    GLFW_KEY_F8,
    GLFW_KEY_F9,
    GLFW_KEY_F10,
    GLFW_KEY_F11,
    GLFW_KEY_F12,
    GLFW_KEY_F13,
    GLFW_KEY_F14,
    GLFW_KEY_F15,
    GLFW_KEY_F16,
    GLFW_KEY_F17,
    GLFW_KEY_F18,
    GLFW_KEY_F19,
    GLFW_KEY_F20,
    GLFW_KEY_F21,
    GLFW_KEY_F22,
    GLFW_KEY_F23,
    GLFW_KEY_F24,
    GLFW_KEY_F25,
    GLFW_KEY_KP_0,
    GLFW_KEY_KP_1,
    GLFW_KEY_KP_2,
    GLFW_KEY_KP_3,
    GLFW_KEY_KP_4,
    GLFW_KEY_KP_5,
    GLFW_KEY_KP_6,
    GLFW_KEY_KP_7,
    GLFW_KEY_KP_8,
    GLFW_KEY_KP_9,
    GLFW_KEY_KP_DECIMAL,
    GLFW_KEY_KP_DIVIDE,
    GLFW_KEY_KP_MULTIPLY,
    GLFW_KEY_KP_SUBTRACT,
    GLFW_KEY_KP_ADD,
    GLFW_KEY_KP_ENTER,
    //GLFW_KEY_KP_EQUAL, // Double detection issues with KP_Enter
    GLFW_KEY_LEFT_SHIFT,
    GLFW_KEY_LEFT_CONTROL,
    GLFW_KEY_LEFT_ALT,
    GLFW_KEY_LEFT_SUPER,
    GLFW_KEY_RIGHT_SHIFT,
    GLFW_KEY_RIGHT_CONTROL,
    GLFW_KEY_RIGHT_ALT,
    GLFW_KEY_RIGHT_SUPER,
    GLFW_KEY_MENU
  };

  const u4 dp::InputGLFW::GLKeySize = 118;



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

void dp::InputGLFW::Close(void)
{
  if (windowContext)
  {
    done = true;
    glfwSetWindowShouldClose(windowContext, true);
  }
  else
  {
    Log::Warn("InputGLFW::Close - no window allocated");
  }
} // end void InputGLFW::Close(void)                                          */

// Get the delta in time (seconds elapsed), from prior frame to now
f4 dp::InputGLFW::Dt(void) { return dt; } // end f4 InputGLFW::Dt(void)


// Update what input is currently read in from hardware                       */
dp::Input& dp::InputGLFW::Poll(void)
{
  if (windowContext)
  {
    // Calculate dt for the frame
    f4 current = static_cast<f4>(glfwGetTime());
    dt = current - lastTime;
    lastTime = current;

    // Poll for new GLFW input events
    glfwPollEvents();
    for (u4 i = 0; i < GLKeySize; ++i)
    {
      if (glfwGetKey(windowContext, GLKey[i]) == GLFW_PRESS)
      {
        if (On(ToKey(GLKey[i])).state == kState::Off)
        {
          On(ToKey(GLKey[i])).state = kState::Pressed;
        }
        else if (On(ToKey(GLKey[i])).state == kState::Pressed)
        {
          On(ToKey(GLKey[i])).state = kState::Held;
        }
        On(ToKey(GLKey[i])).held += dt;
      }
      else
      {
        if (On(ToKey(GLKey[i])).state == kState::Pressed
         || On(ToKey(GLKey[i])).state == kState::Held)
        {
          On(ToKey(GLKey[i])).state = kState::Released;
          On(ToKey(GLKey[i])).held = 0.0f;
        }
        else if (On(ToKey(GLKey[i])).state == kState::Released)
        {
          On(ToKey(GLKey[i])).state = kState::Off;
          On(ToKey(GLKey[i])).held = 0.0f;
        }
      }
    }
  }
  return *this;
} // end Input& poll(void)


// Process, then update what input is currently read in from hardware         */
dp::Input& dp::InputGLFW::Read(void)
{
  Process().Poll();
  return *this;
} // end Input& read(void)


// Close down and deallocate any GLFW resources owned by input                */
void dp::InputGLFW::Shutdown(void)
{
  // Just null out the window pointer; it's not owned by input to clean here
  done = false;
  windowContext = nullptr;

} // end void shutdown(void)

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void dp::InputGLFW::mouse_callback(GLFWwindow* window, f8 xpos, f8 ypos)
{
  if (firstMouse)
  {
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);
    firstMouse = false;
  }
  float xoffset = static_cast<f4>(xpos) - lastX;
  // reversed since y-coordinates go from bottom to top
  float yoffset = lastY - static_cast<f4>(ypos);

  lastX = static_cast<float>(xpos);
  lastY = static_cast<float>(ypos);
  if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
  {
    if (win)
    {
      //win->activeView.ProcessMouseMovement(xoffset, yoffset);
      win->activeView.Pitch(ang(yoffset * 0.01f, arc::radians));
      win->activeView.Yaw(ang(yoffset * 0.01f, arc::radians));
    }
  }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void dp::InputGLFW::scroll_callback(GLFWwindow* window, f8 xoffset, f8 yoffset)
{
  //if (win) { win->activeView.ProcessMouseScroll(yoffset); }

} // end void InputGLFW::scroll_callback(GLFWwindow*, f8, f8)
