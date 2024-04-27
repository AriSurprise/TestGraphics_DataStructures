/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Lambert.cpp
Purpose:  Lightweight material type using diffuse shading without specular
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
#include "Log.h"        // Error / warning output message / logging
#include "Material.h"   // Associated class declaration header
#include "Lambert.h"


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                Constructors                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Lambert::Lambert(Shader& lambert, const v3f& diffuseColor, const v3f&
  ambientColor, const v3f& incandescentColor, const v3f& fresnelColor)
  : Material(Material::Type::Lambert, lambert), ambient(ambientColor),
  diffuse(diffuseColor), incandescence(incandescentColor), fresnel(fresnelColor)
{} // end Lambert::Lambert(Shader&, v3f, v3f, v3f, v3f)

dp::Lambert::Lambert(const Lambert& source)
  : Lambert(source.ShadingGroup(), source.diffuse, source.ambient,
    source.incandescence, source.fresnel)
{
  StationPoint(source.cameraPos);
} // end Lambert::Lambert(const Lambert&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the current RGB ambient color in use by this Lambert material          */
const dp::v3f& dp::Lambert::Ambient(void) const
{ return ambient; } // end const v3f& Lambert::Ambient(void) const


// Set the current RGB ambient color to use on this Lambert material          */
dp::Lambert& dp::Lambert::Ambient(const v3f& ambientColor)
{
  ambient = ambientColor;
  return *this;

} // end Lambert& Lambert::Ambient(const v3f&)


// Get the current RGB diffuse color in use by this Lambert material          */
const dp::v3f& dp::Lambert::Diffuse(void) const
{ return diffuse; } // end const v3f& Lambert::Diffuse(void) const


// Set the current RGB diffuse color to use on this Lambert material          */
dp::Lambert& dp::Lambert::Diffuse(const v3f& diffuseColor)
{
  diffuse = diffuseColor;
  return *this;

} // end Lambert& Lambert::Diffuse(const v3f&)


// Get the current RGB fresnel color in use by this Lambert material          */
const dp::v3f& dp::Lambert::Fresnel(void) const
{ return fresnel; } // end const v3f& Lambert::Fresnel(void) const


// Set the current RGB fresnel color to use on this Lambert material          */
dp::Lambert& dp::Lambert::Fresnel(const v3f& fresnelColor)
{
  fresnel = fresnelColor;
  return *this;

} // end Lambert& Lambert::Fresnel(const v3f&)


// Get the current RGB incandescent color in use by this Lambert material     */
const dp::v3f& dp::Lambert::Incandescence(void) const
{ return incandescence; } // end const v3f& Lambert::Incandescence(void) const


// Set the current RGB incandescent color to use on this Lambert material     */
dp::Lambert& dp::Lambert::Incandescence(const v3f& incandescentColor)
{
  incandescence = incandescentColor;
  return *this;

} // end Lambert& Lambert::Incandescence(const v3f&)


// Get the current XYZ world view coordinates in use by this Material         */
const dp::v3f& dp::Lambert::StationPoint(void) const
{ return cameraPos; } // end const v3f& Lambert::StationPoint(void) const


// Set the current XYZ world view coordinates for use on this Material        */
dp::Lambert& dp::Lambert::StationPoint(const v3f& cameraPosition)
{
  cameraPos = cameraPosition;
  return *this;

} // end Lambert& Lambert::StationPoint(const v3f&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Operator Overloads                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Copy a source Lambert's material attribute settings into this Lambert      */
dp::Lambert& dp::Lambert::operator=(const Lambert& source)
{
  Ambient(source.ambient).Diffuse(source.diffuse).Fresnel(source.fresnel);
  Incandescence(source.incandescence).StationPoint(source.cameraPos);
  return *this;
} // end Lambert& Lambert::operator=(const Lambert&)
