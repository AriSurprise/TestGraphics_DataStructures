/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Material.cpp
Purpose:  Shader parameter manipluative base class to be derived per shader type
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
#include "Blinn.h"    // Type casting for copy setting / constrcution
#include "Lambert.h"  // Type casting for copy setting / constrcution
#include "Log.h"      // Error / warning output message / logging
#include "Phong.h"    // Type casting for copy setting / constrcution
#include "Material.h" // Associated class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                Constructors                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create a shader reference instance to use it's attributes with settings    */
dp::Material::Material(Material::Type subclass, Shader& program)
  : shadingGroup(program), shadingType(subclass)
{} // end Material::Material(Material::Type, Shader&)


// Create a material copy of attribute settings, using the same shader        */
dp::Material::Material(const Material& source)
  : shadingGroup(source.shadingGroup), shadingType(source.shadingType)
{
  Lambert* lLocal = (Lambert*)this;
  Lambert* lRemote = (Lambert*)&source;
  Phong* pLocal = (Phong*)this;
  Phong* pRemote = (Phong*)&source;
  Blinn* bLocal = (Blinn*)this;
  Blinn* bRemote = (Blinn*)&source;
  switch (shadingType)
  {
    // Add new types here & follow the pattern to copy attribute settings
  case Type::Lambert:
    *lLocal = *lRemote;
    break;
  case Type::Phong:
    *pLocal = *pRemote;
    break;
  case Type::Blinn:
    *bLocal = *bRemote;
    break;
  default:
    Log::Warn("Material - unrecognized type, copy from base Material class");
    break;
  }

} // end Material::Material(const Material&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the shader program being referenced to render this material            */
dp::Shader& dp::Material::ShadingGroup(void) const
{ return shadingGroup; } // end Shader& Material::ShadingGroup(void) const


// Get the enumerated shader program type for type casting purposes           */
dp::Material::Type dp::Material::ShadingType(void) const
{ return shadingType; } // end Material::Type Material::ShadingType(void) const


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Operator Overloads                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Copy a material's settings, while referencing the same shader              */
dp::Material& dp::Material::operator=(const Material& source)
{
  shadingGroup = source.shadingGroup;
  shadingType = source.shadingType;
  switch (shadingType)
  {
    // Add new types here (copy attribute settings as needed)
  case Type::Capture:
    // No material attribute settings to copy
    break;
  case Type::Lambert:
    // So far no need to copy materials; may be redundant in base class too?
    break;
  case Type::Phong:
    break;
  case Type::Blinn:
    break;
  default:
    Log::Warn("Material - unrecognized type, copy from base Material class");
    break;
  }
  return *this;

} // end Material& Material::operator=(const Material&)
