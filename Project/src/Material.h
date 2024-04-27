/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Material.h
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
#include "Shader.h"     // Material must reference a shader program instance
#include "TypeErrata.h" // Platform primitive type aliases for easier porting
#include "v3f.h"       // 3D material attributes: camera pos, RGB colors, etc


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{ 

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Material //! Shader parameter setting base class requiring type-casting
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Constants                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Codification for shader program category (for Material attribute access)
    \brief
      - codification for shader program category (for Material attribute access)
    \details
      - really just acting as a contract to yourself of available attributes and
       layout pattern for accessing such when casting to the given material
       type. if you want to change what it means to qualify as that type, it
       only means the onus is on you to make all instances of that type conform
       to having and using those attributes (if you even feel the need to have
       multiple shaders using the same type: ostensibly pretty pointless to)
    */
    enum class Type
    {
      Blinn,    //! Economized reflection photorealism shading variant
      Capture,  //! Simple textured flat shader; only model->world coords to set
      Lambert,  //! Basic diffuse photorealism shading variant (only matte)
      Phong,    //! Traditional diffuse+specular photorealism shading variant
      Skybox    //! Distance drawing shading variant

    }; // end Material::Type enum class


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a shader reference instance to use it's attributes with settings
    \brief
      - create a shader reference instance to use it's attributes with settings
    \details
      - to manipulate the attributes a Shader offers, it must be categorically
      known exactly what those attributes are which can be manipulated, which
      will have to be able to vary per type of shader program. hence the type
      must be given in the base class to be able to point to those properties
      by type casting into accessing them, while allowing passing in a generic. 
      while the alternative of including those properties in the base class is
      another solution, that obligates every material to have dead functions
      without any particular signifiers of a dead state, or else to simply
      render using uniform term recycling like some kind of cross processor
      struct union, neither of which is particularly preferable
    \param subclass
      - derived class declares type for casting purposes to access shader attrs
    \param program
      - shading program group which this material will manipulate to render
    */
    Material(Material::Type subclass, Shader& program);

    /** Create a material copy of attribute settings, using the same shader
    \brief
      - create a material copy of attribute settings, using the same shader
    \param source
      - material to have current attribute settings copied (per derived type)
    */
    Material(const Material& source);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Get the shader program being referenced to render this material
    \brief
      - get the shader program being referenced to render this material
    \return
      - the type enumeration value for the shader program language
    */
    Shader& ShadingGroup(void) const;


    /** Get the enumerated shader program type for type casting purposes
    \brief
      - get the enumerated shader program type for type casting purposes
    \return
      - the type enumeration value for the shader program language
    */
    Material::Type ShadingType(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Operator Overloads                             */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Copy a material's settings, while referencing the same shader
    \brief
      - copy a material's settings, while referencing the same shader
    \param source
      - material to have current attribute settings copied (per derived type)
    \return
      - reference to the modified material for easier subsequent inline action
    */
    Material& operator=(const Material& source);



    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    //! Linked shading program data to reference when drawing this material
    Shader& shadingGroup; // non-const to allow loading of an unloaded shader

    //! Enumeration for use in derived classes to manipulate shader properties
    Type shadingType;

  }; // end Material class declaration

} // end dp namespace
