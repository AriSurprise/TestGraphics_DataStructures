/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  Blinn.cpp
Purpose:  Lightweight realistic surface shading material model
Language: C++ / Eng, MSVC 19 (used; nothing platform specific should be present)
Platform: MSVC19 / Windows 10 x64 (should work with few mods if any elsewhere)
Project:  a.Surprise_CS300_2
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// "./src/..."
#include "Log.h"    // Error / warning output message / logging
#include "Blinn.h"  // Associated class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                Constructors                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Blinn::Blinn(Shader& blinn, const v3f& diffuseColor, const v3f&
  ambientColor, const v3f& incandescentColor, const v3f& fresnelColor,
  const v3f& specularColor, f4 specularFalloff)
  : Material(Material::Type::Blinn, blinn), ambient(ambientColor),
 diffuse(diffuseColor), incandescence(incandescentColor), fresnel(fresnelColor),
  specular(specularColor)
{
  Falloff(specularFalloff);
} // end Blinn::Blinn(Shader&, v3f, v3f, v3f, v3f, v3f, f4)

dp::Blinn::Blinn(const Blinn& source)
  : Blinn(source.ShadingGroup(), source.diffuse, source.ambient,
    source.incandescence, source.fresnel)
{
  StationPoint(source.cameraPos);
} // end Blinn::Blinn(const Blinn&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the current RGB ambient color in use by this Blinn material            */
const dp::v3f& dp::Blinn::Ambient(void) const
{ return ambient; } // end const v3f& Blinn::Ambient(void) const


// Set the current RGB ambient color to use on this Blinn material            */
dp::Blinn& dp::Blinn::Ambient(const v3f& ambientColor)
{
  ambient = ambientColor;
  return *this;

} // end Blinn& Blinn::Ambient(const v3f&)


// Get the current RGB diffuse color in use by this Blinn material            */
const dp::v3f& dp::Blinn::Diffuse(void) const
{ return diffuse; } // end const v3f& Blinn::Diffuse(void) const


// Set the current RGB diffuse color to use on this Blinn material            */
dp::Blinn& dp::Blinn::Diffuse(const v3f& diffuseColor)
{
  diffuse = diffuseColor;
  return *this;

} // end Blinn& Blinn::Diffuse(const v3f&)


// Get the rate at which distance from perfect reflection doesn't draw        */
const f4& dp::Blinn::Falloff(void) const
{ return rolloff; } // end cosnt f4& Blinn::Falloff(void) const


// Set the rate at which distance from perfect reflection doesn't draw        */
dp::Blinn& dp::Blinn::Falloff(f4 falloffRate)
{
  rolloff = falloffRate;
  return *this;

} // end Blinn& Blinn::Falloff(f4)


// Get the current RGB fresnel color in use by this Blinn material            */
const dp::v3f& dp::Blinn::Fresnel(void) const
{ return fresnel; } // end const v3f& Blinn::Fresnel(void) const


// Set the current RGB fresnel color to use on this Blinn material            */
dp::Blinn& dp::Blinn::Fresnel(const v3f& fresnelColor)
{
  fresnel = fresnelColor;
  return *this;

} // end Blinn& Blinn::Fresnel(const v3f&)


// Get the current RGB incandescent color in use by this Blinn material       */
const dp::v3f& dp::Blinn::Incandescence(void) const
{ return incandescence; } // end const v3f& Blinn::Incandescence(void) const


// Set the current RGB incandescent color to use on this Blinn material       */
dp::Blinn& dp::Blinn::Incandescence(const v3f& incandescentColor)
{
  incandescence = incandescentColor;
  return *this;

} // end Blinn& Blinn::Incandescence(const v3f&)


// Get the current RGB specular color bias in use by this Blinn material      */
const dp::v3f& dp::Blinn::Specular(void) const
{ return specular; } // end const v3f& Blinn::Specular(void) const


// Set the current RGB specular color bias to use on this Blinn material      */
dp::Blinn& dp::Blinn::Specular(const v3f& specularColor)
{
  specular = specularColor;
  return *this;

} // end Blinn& Blinn::Specular(const v3f&)


// Get the current XYZ world view coordinates in use by this Material         */
const dp::v3f& dp::Blinn::StationPoint(void) const
{ return cameraPos; } // end const v3f& Blinn::StationPoint(void) const


// Set the current XYZ world view coordinates for use on this Material        */
dp::Blinn& dp::Blinn::StationPoint(const v3f& cameraPosition)
{
  cameraPos = cameraPosition;
  return *this;

} // end Blinn& Blinn::StationPoint(const v3f&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Operator Overloads                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Copy a source Blinn's material attribute settings into this Blinn      */
dp::Blinn& dp::Blinn::operator=(const Blinn& source)
{
  Ambient(source.ambient).Diffuse(source.diffuse).Specular(source.specular);
  Incandescence(source.incandescence).Fresnel(source.fresnel);
  Falloff(source.rolloff).StationPoint(source.cameraPos);
  return *this;

} // end Blinn& Blinn::operator=(const Blinn&)
