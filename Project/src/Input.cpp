/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Input.cpp
Purpose:  Interface for window control / input processing, with MVC decoupling
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

#include "Input.h"  // Class declaration header file
#include "Log.h"    // Error / Warning message output / logging

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                   Consts                                   */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                Constructors                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Input::Input(Window* parent)
  : win(parent)
{ }


// Use the polled input to perform action per program settings                */
dp::Input& dp::Input::Process(void)
{
  if (On(enKey::Esc).state == kState::Released) // Stop it?
  {
    Close();
    //Log::D("Goodbye", 1);
  }
  if (win)
  {
    if (On(enKey::Space).state == kState::Pressed)
    {
      win->spin = !(win->spin);
      //Log::D(std::string("Spin ") + (win->spin ? "start" : "stop"), 1);
    }
    if (On(enKey::W).state == kState::Held)
    {
      win->activeView.Dolly(-2.5f * Dt());
      //Log::D("Dolly -" + std::to_string(Dt() * 2.5f), 1);
    }
    if (On(enKey::S).state == kState::Held)
    {
      win->activeView.Dolly(2.5f * Dt());
      //Log::D("Dolly +" + std::to_string(Dt() * 2.5f), 1);
    }
    if (On(enKey::A).state == kState::Held)
    {
      win->activeView.Pan(-2.5f * Dt());
      //Log::D("Pan -" + std::to_string(Dt() * 2.5f), 1);
    }
    if (On(enKey::D).state == kState::Held)
    {
      win->activeView.Pan(2.5f * Dt());
      //Log::D("Pan +" + std::to_string(Dt() * 2.5f), 1);
    }
    if (On(enKey::E).state == kState::Held)
    {
      win->activeView.Tilt(2.5f * Dt());
      //Log::D("Tilt +" + std::to_string(Dt() * 2.5f), 1);
    }
    if (On(enKey::X).state == kState::Held)
    {
      win->activeView.Tilt(-2.5f * Dt());
      //Log::D("Tilt -" + std::to_string(Dt() * 2.5f), 1);
    }

    if (On(enKey::Up).state == kState::Held)
    {
      win->activeView.Pitch(ang(-1.5f * Dt(), arc::rad));
      //Log::D("Pitch -" + std::to_string(AngleF(Dt()*1.5f,uArcConv::RToD)),1);
    }
    if (On(enKey::Down).state == kState::Held)
    {
      win->activeView.Pitch(ang(1.5f * Dt(), arc::rad));
      //Log::D("Pitch +" + std::to_string(AngleF(Dt()*1.5f,uArcConv::RToD)),1);
    }
    if (On(enKey::Left).state == kState::Held)
    {
      win->activeView.Yaw(ang(1.5f * Dt(), arc::rad));
      //Log::D("Yaw +" + std::to_string(AngleF(Dt()*1.5f,uArcConv::RToD)),1);
    }
    if (On(enKey::Right).state == kState::Held)
    {
      win->activeView.Yaw(ang(-1.5f * Dt(), arc::rad));
      //Log::D("Yaw -" + std::to_string(AngleF(Dt()*1.5f,uArcConv::RToD)),1);
    }

    if (On(enKey::Q).state == kState::Held)
    {
      win->activeView.Roll(ang(2.5f * Dt(), arc::rad));
      //Log::D("Roll +" + std::to_string(AngleF(Dt()*2.5f,uArcConv::RToD)),1);
    }
    if (On(enKey::Z).state == kState::Held)
    {
      win->activeView.Roll(ang(-2.5f * Dt(), arc::rad));
      //Log::D("Roll -" + std::to_string(AngleF(Dt()*2.5f,uArcConv::RToD)),1);
    }
  }
  return *this;

} // end Input& process(void)

/** Get whether the input is in a ready state to poll input (!done)           */
bool dp::Input::Running(void) const
{ return !done; } // end bool Running(void) const
