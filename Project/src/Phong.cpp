/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Phong.cpp
Purpose:  Standard realistic diffuse / specular surface shading material model
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
#include "Log.h"    // Error / warning output message / logging
#include "Phong.h"  // Associated class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                Constructors                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Phong::Phong(Shader& phong, const v3f& diffuseColor, const v3f&
  ambientColor, const v3f& incandescentColor, const v3f& fresnelColor,
  const v3f& specularColor, f4 specularFalloff)
  : Material(Material::Type::Phong, phong), ambient(ambientColor),
 diffuse(diffuseColor), incandescence(incandescentColor), fresnel(fresnelColor),
  specular(specularColor)
{
  Falloff(specularFalloff);
} // end Phong::Phong(Shader&, v3f, v3f, v3f, v3f, v3f, f4)

dp::Phong::Phong(const Phong& source)
  : Phong(source.ShadingGroup(), source.diffuse, source.ambient,
    source.incandescence, source.fresnel, source.specular, source.rolloff)
{
  StationPoint(source.cameraPos);
} // end Phong::Phong(const Phong&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the current RGB ambient color in use by this Phong material            */
const dp::v3f& dp::Phong::Ambient(void) const
{ return ambient; } // end const v3f& Phong::Ambient(void) const


// Set the current RGB ambient color to use on this Phong material            */
dp::Phong& dp::Phong::Ambient(const v3f& ambientColor)
{
  ambient = ambientColor;
  return *this;

} // end Phong& Phong::Ambient(const v3f&)


// Get the current RGB diffuse color in use by this Phong material            */
const dp::v3f& dp::Phong::Diffuse(void) const
{ return diffuse; } // end const v3f& Phong::Diffuse(void) const


// Set the current RGB diffuse color to use on this Phong material            */
dp::Phong& dp::Phong::Diffuse(const v3f& diffuseColor)
{
  diffuse = diffuseColor;
  return *this;

} // end Phong& Phong::Diffuse(const v3f&)


// Get the rate at which distance from perfect reflection doesn't draw        */
const f4& dp::Phong::Falloff(void) const
{ return rolloff; } // end cosnt f4& Phong::Falloff(void) const


// Set the rate at which distance from perfect reflection doesn't draw        */
dp::Phong& dp::Phong::Falloff(f4 falloffRate)
{
  rolloff = (falloffRate >= 1.0f) ? falloffRate : 1.0f;
  return *this;

} // end Phong& Phong::Falloff(f4)


// Get the current RGB fresnel color in use by this Phong material            */
const dp::v3f& dp::Phong::Fresnel(void) const
{ return fresnel; } // end const v3f& Phong::Fresnel(void) const


// Set the current RGB fresnel color to use on this Phong material            */
dp::Phong& dp::Phong::Fresnel(const v3f& fresnelColor)
{
  fresnel = fresnelColor;
  return *this;

} // end Phong& Phong::Fresnel(const v3f&)


// Get the current RGB incandescent color in use by this Phong material       */
const dp::v3f& dp::Phong::Incandescence(void) const
{ return incandescence; } // end const v3f& Phong::Incandescence(void) const


// Set the current RGB incandescent color to use on this Phong material       */
dp::Phong& dp::Phong::Incandescence(const v3f& incandescentColor)
{
  incandescence = incandescentColor;
  return *this;

} // end Phong& Phong::Incandescence(const v3f&)


// Get the current RGB specular color bias in use by this Phong material      */
const dp::v3f& dp::Phong::Specular(void) const
{ return specular; } // end const v3f& Phong::Specular(void) const


// Set the current RGB specular color bias to use on this Phong material      */
dp::Phong& dp::Phong::Specular(const v3f& specularColor)
{
  specular = specularColor;
  return *this;

} // end Phong& Phong::Specular(const v3f&)


// Get the current XYZ world view coordinates in use by this Material         */
const dp::v3f& dp::Phong::StationPoint(void) const
{ return cameraPos; } // end const v3f& Phong::StationPoint(void) const


// Set the current XYZ world view coordinates for use on this Material        */
dp::Phong& dp::Phong::StationPoint(const v3f& cameraPosition)
{
  cameraPos = cameraPosition;
  return *this;

} // end Phong& Phong::StationPoint(const v3f&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Operator Overloads                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Copy a source Phong's material attribute settings into this Phong      */
dp::Phong& dp::Phong::operator=(const Phong& source)
{
  Ambient(source.ambient).Diffuse(source.diffuse).Specular(source.specular);
  Incandescence(source.incandescence).Fresnel(source.fresnel);
  Falloff(source.rolloff).StationPoint(source.cameraPos);
  return *this;
} // end Phong& Phong::operator=(const Phong&)
