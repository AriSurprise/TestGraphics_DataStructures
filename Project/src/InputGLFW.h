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

// <./src/...>
#include "Input.h"  // Base interface definitions


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

struct GLFWwindow;

namespace dp //! Contents for use within class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class InputGLFW : public Input //! Process GLFW input decoupled from display
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Constructor to enforce subclasses to define type for external casting
    \brief
      - constructor to enforce subclasses to define type for external casting
    \param parent
      - pointer to the Window to be controlled by this input
    \param window
      - pointer to the GLFW window governing associative window display
    */
    InputGLFW(Window* parent, GLFWwindow* window);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    virtual void Close(void) override;

    /** Get the delta in time (seconds elapsed), from prior frame to now
    \brief
      - get the delta in time (seconds elapsed), from prior frame to now
    \return
      - float point representation of seconds elapsed since the last frame
    */
    virtual f4 Dt(void) override;

    
    /** Update what input is currently read in from hardware
    \brief
      - update what input is currently read in from hardware
    \return
      - reference to the modified Input for easier subsequent inline action
    */
    virtual Input& Poll(void) override;


    /** Process, then update what input is currently read in from hardware
    \brief
      - process, then update what input is currently read in from hardware
    \return
      - reference to the modified Input for easier subsequent inline action
    */
    virtual Input& Read(void) override;
    

    /** Close down and deallocate any GLFW resources owned by input
    \brief
      - close down and deallocate any GLFW resources owned by input
    */
    virtual void Shutdown(void) override;

    inline static enKey ToKey(u4 GLFW_Keycode)
    {
      switch (GLFW_Keycode)
      {
      case GLFW_KEY_SPACE: return enKey::Space;
      case GLFW_KEY_APOSTROPHE: return enKey::Apostrophe;
      case GLFW_KEY_COMMA: return enKey::Comma;
      case GLFW_KEY_MINUS: return enKey::Dash;
      case GLFW_KEY_PERIOD: return enKey::Period;
      case GLFW_KEY_SLASH: return enKey::Slash;
      case GLFW_KEY_0: return enKey::D0;
      case GLFW_KEY_1: return enKey::D1;
      case GLFW_KEY_2: return enKey::D2;
      case GLFW_KEY_3: return enKey::D3;
      case GLFW_KEY_4: return enKey::D4;
      case GLFW_KEY_5: return enKey::D5;
      case GLFW_KEY_6: return enKey::D6;
      case GLFW_KEY_7: return enKey::D7;
      case GLFW_KEY_8: return enKey::D8;
      case GLFW_KEY_9: return enKey::D9;
      case GLFW_KEY_SEMICOLON: return enKey::Semicolon;
      case GLFW_KEY_EQUAL: return enKey::Equal;
      case GLFW_KEY_A: return enKey::A;
      case GLFW_KEY_B: return enKey::B;
      case GLFW_KEY_C: return enKey::C;
      case GLFW_KEY_D: return enKey::D;
      case GLFW_KEY_E: return enKey::E;
      case GLFW_KEY_F: return enKey::F;
      case GLFW_KEY_G: return enKey::G;
      case GLFW_KEY_H: return enKey::H;
      case GLFW_KEY_I: return enKey::I;
      case GLFW_KEY_J: return enKey::J;
      case GLFW_KEY_K: return enKey::K;
      case GLFW_KEY_L: return enKey::L;
      case GLFW_KEY_M: return enKey::M;
      case GLFW_KEY_N: return enKey::N;
      case GLFW_KEY_O: return enKey::O;
      case GLFW_KEY_P: return enKey::P;
      case GLFW_KEY_Q: return enKey::Q;
      case GLFW_KEY_R: return enKey::R;
      case GLFW_KEY_S: return enKey::S;
      case GLFW_KEY_T: return enKey::T;
      case GLFW_KEY_U: return enKey::U;
      case GLFW_KEY_V: return enKey::V;
      case GLFW_KEY_W: return enKey::W;
      case GLFW_KEY_X: return enKey::X;
      case GLFW_KEY_Y: return enKey::Y;
      case GLFW_KEY_Z: return enKey::Z;
      case GLFW_KEY_LEFT_BRACKET: return enKey::LBracket;
      case GLFW_KEY_BACKSLASH: return enKey::BSlash;
      case GLFW_KEY_RIGHT_BRACKET: return enKey::RBracket;
      case GLFW_KEY_GRAVE_ACCENT: return enKey::Tilde;
      case GLFW_KEY_ESCAPE: return enKey::Escape;
      case GLFW_KEY_ENTER: return enKey::Enter;
      case GLFW_KEY_TAB: return enKey::Tab;
      case GLFW_KEY_BACKSPACE: return enKey::Backspace;
      case GLFW_KEY_INSERT: return enKey::Insert;
      case GLFW_KEY_DELETE: return enKey::Delete;
      case GLFW_KEY_RIGHT: return enKey::Right;
      case GLFW_KEY_LEFT: return enKey::Left;
      case GLFW_KEY_DOWN: return enKey::Down;
      case GLFW_KEY_UP: return enKey::Up;
      case GLFW_KEY_PAGE_UP: return enKey::PageUp;
      case GLFW_KEY_PAGE_DOWN: return enKey::PageDown;
      case GLFW_KEY_HOME: return enKey::Home;
      case GLFW_KEY_END: return enKey::End;
      case GLFW_KEY_CAPS_LOCK: return enKey::CapsLock;
      case GLFW_KEY_SCROLL_LOCK: return enKey::ScrollLock;
      case GLFW_KEY_NUM_LOCK: return enKey::NumLock;
      case GLFW_KEY_PRINT_SCREEN: return enKey::PrintScreen;
      case GLFW_KEY_PAUSE: return enKey::Pause;
      case GLFW_KEY_F1: return enKey::F1;
      case GLFW_KEY_F2: return enKey::F2;
      case GLFW_KEY_F3: return enKey::F3;
      case GLFW_KEY_F4: return enKey::F4;
      case GLFW_KEY_F5: return enKey::F5;
      case GLFW_KEY_F6: return enKey::F6;
      case GLFW_KEY_F7: return enKey::F7;
      case GLFW_KEY_F8: return enKey::F8;
      case GLFW_KEY_F9: return enKey::F9;
      case GLFW_KEY_F10: return enKey::F10;
      case GLFW_KEY_F11: return enKey::F11;
      case GLFW_KEY_F12: return enKey::F12;
      case GLFW_KEY_F13: return enKey::F13;
      case GLFW_KEY_F14: return enKey::F14;
      case GLFW_KEY_F15: return enKey::F15;
      case GLFW_KEY_F16: return enKey::F16;
      case GLFW_KEY_F17: return enKey::F17;
      case GLFW_KEY_F18: return enKey::F18;
      case GLFW_KEY_F19: return enKey::F19;
      case GLFW_KEY_F20: return enKey::F20;
      case GLFW_KEY_F21: return enKey::F21;
      case GLFW_KEY_F22: return enKey::F22;
      case GLFW_KEY_F23: return enKey::F23;
      case GLFW_KEY_F24: return enKey::F24;
      case GLFW_KEY_F25: return enKey::F25;
      case GLFW_KEY_KP_0: return enKey::N0;
      case GLFW_KEY_KP_1: return enKey::N1;
      case GLFW_KEY_KP_2: return enKey::N2;
      case GLFW_KEY_KP_3: return enKey::N3;
      case GLFW_KEY_KP_4: return enKey::N4;
      case GLFW_KEY_KP_5: return enKey::N5;
      case GLFW_KEY_KP_6: return enKey::N6;
      case GLFW_KEY_KP_7: return enKey::N7;
      case GLFW_KEY_KP_8: return enKey::N8;
      case GLFW_KEY_KP_9: return enKey::N9;
      case GLFW_KEY_KP_DECIMAL: return enKey::Decimal;
      case GLFW_KEY_KP_DIVIDE: return enKey::Divide;
      case GLFW_KEY_KP_MULTIPLY: return enKey::Multiply;
      case GLFW_KEY_KP_SUBTRACT: return enKey::Subtract;
      case GLFW_KEY_KP_ADD: return enKey::Add;
      case GLFW_KEY_KP_ENTER: return enKey::NEnter;
      case GLFW_KEY_KP_EQUAL: return enKey::NPEqual;
      case GLFW_KEY_LEFT_SHIFT: return enKey::LShift;
      case GLFW_KEY_LEFT_CONTROL: return enKey::LCtrl;
      case GLFW_KEY_LEFT_ALT: return enKey::LAlt;
      case GLFW_KEY_LEFT_SUPER: return enKey::LOS;
      case GLFW_KEY_RIGHT_SHIFT: return enKey::RShift;
      case GLFW_KEY_RIGHT_CONTROL: return enKey::RCtrl;
      case GLFW_KEY_RIGHT_ALT: return enKey::RAlt;
      case GLFW_KEY_RIGHT_SUPER: return enKey::ROS;
      case GLFW_KEY_WORLD_1: return enKey::D1;
      case GLFW_KEY_WORLD_2: return enKey::D2;
      case GLFW_KEY_UNKNOWN: default: return enKey::Invalid;
      } // end GLFW_Keycode switch statement

    } // end inline enKey ToKey(u4)

  private:

    void mouse_callback(GLFWwindow* window, f8 xpos, f8 ypos);
    void scroll_callback(GLFWwindow* window, f8 xoffset, f8 yoffset);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Shared window context pointer to read input (owned / managed by Window)
    GLFWwindow* windowContext;

    kState space = kState::Off;

    kState qKey = kState::Off;

    kState zKey = kState::Off;

    kState wKey = kState::Off;

    kState aKey = kState::Off;

    kState sKey = kState::Off;

    kState dKey = kState::Off;

    kState eKey = kState::Off;

    kState xKey = kState::Off;

    kState up = kState::Off;

    kState down = kState::Off;

    kState left = kState::Off;

    kState right = kState::Off;

    f4 lastTime;

    f4 dt;

    static const u4 GLKey[];

    static const u4 GLKeySize;

  }; // end InputGLFW class declaration

} // end dp namespace
