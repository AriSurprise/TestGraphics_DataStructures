/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  GuiAttr.h
Purpose:  Container for all the UI values to be passed into ImGui for updates
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
#include "v3f.h"       // RGB / XYZ members


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class GuiAttr //! Container for attributes ImGuiUI updates will affect
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Tracker for UI state of what uv mode was set the previous frame
    s4 shadPrev = 2;

    //! Tracker for UI state of what uv mode is set this frame
    s4 shadCurr = 2;

    //! Tracker for UI state of what render mode was set the previous frame
    s4 outputPrev = 2;

    //! Tracker for UI state of what render mode is set this frame
    s4 outputCurrent = 2;

    //! Tracker for UI state of what uv mode was set the previous frame
    s4 uvMPrev = 0;

    //! Tracker for UI state of what uv mode is set this frame
    s4 uvMCurrent = 0;

    //! Tracker for UI state of what specular falloff was the prior frame
    f4 falloffPrev = 1000.0f;

    //! Tracker for UI state of what specular falloff is set to this frame
    f4 falloffCurrent = 1000.0f;

    //! Tracker for UI state of what ambient color is set this frame
    v3f ambientCurrent = { 0.1f, 0.1f, 0.1f };

    //! Tracker for UI state of what ambient color was set the previous frame
    v3f ambientPrev = { 0.1f, 0.1f, 0.1f };

    //! Tracker for UI state of what diffuse color is set this frame
    v3f diffuseCurrent = { 1.0f, 1.0f, 1.0f };

    //! Tracker for UI state of what diffuse color was set the previous frame
    v3f diffusePrev = { 1.0f, 1.0f, 1.0f };

    //! Tracker for UI state of what specular color is set this frame
    v3f specularCurrent = { 1.0f, 1.0f, 1.0f };

    //! Tracker for UI state of what specular color was set the previous frame
    v3f specularPrev = { 1.0f, 1.0f, 1.0f };

    //! Tracker for UI state of what incandescent color is set this frame
    v3f incandescenceCurrent = 0;

    //! Tracker for UI state of what incandescent color was set the prior frame
    v3f incandescencePrev = 0;

    //! Tracker for UI state of what fresnel color was set the previous frame
    v3f fresnelPrev = 0;

    //! Tracker for UI state of what fresnel color is set this frame
    v3f fresnelCurrent = 0;

    //! Tracker for UI state of what uv axis was set to the previous frame
    v3f axisPrev = { 0.0f, 0.0f, 1.0f };

    //! Tracker for UI state of what uv axis was set to this frame
    v3f axisCurrent = { 0.0f, 0.0f, 1.0f };

    //! UI state for center object bounding volume was in the previous frame
    s4 cBVPrev = 0;

    //! UI state for center object bounding volume is set to in this frame
    s4 cBVCurrent = 0;

    //! UI state for center object bounding sphere calculation in prior frame
    s4 eposPrev = 0;

    //! UI state for center object bounding sphere calculation in this frame
    s4 eposCurrent = 0;

    //! UI state for bounding volume data sample count in prior frame
    s4 samplesPrev = s4_MAX>>8;

    //! UI state for bounding volume data sample count in this frame
    s4 samplesCurrent = s4_MAX>>8;

    //! UI state for bounding volume data sample interval in prior frame
    s4 stepSPrev = 1;

    //! UI state for bounding volume data sample interval in this frame
    s4 stepSCurrent = 1;

    //! UI state for bounding volume sampling first sample offset in prior frame
    s4 firstSPrev = 0;

    //! UI state for bounding volume sampling first sample offset in this frame
    s4 firstSCurrent = 0;

    //! List of GPU shader program names in enumerated terms
    const char* cBVType[3] =
    { "AABB", "BSphere", "OBB" };

    //! List of GPU shader program names in enumerated terms
    const char* bSphereMethod[7] =
    { "Ritter", "Larson EPOS-14", "Larson EPOS-26", "Larson EPOS-50",
      "Larson EPOS-74", "Larson EPOS-98", "PCA" };

    //! List of GPU shader program names in enumerated terms
    const char* shadeType[3] =
    { "Lambert", "Phong", "Blinn" };

    //! List of GPU UV mapping mode names in parallel with enumerated terms
    const char* outputMode[10] =
    { "Forward", "Deferred", "Depth", "Normal", "Position", "Ambient", "Diffuse",
      "Specular", "Incandescence", "Fresnel" };

    //! List of GPU UV mapping mode names in parallel with enumerated terms
    const char* uvMode[5] =
    { "Off", "Planar", "Spherical", "Cylindrical", "Cube" };

  }; // end GuiAttr class declaration

} // end dp namespace
