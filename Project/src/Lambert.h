/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Lambert.h
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
#include "Material.h"   // Base class for derived material to inherit from
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Lambert : public Material //! Diffuse lighting shading type
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Constants                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a diffuse shaded material using the given attribute settings
    \brief
      - create a diffuse shaded material using the given attribute settings
    \param lambert
      - linked, lambert shader program to have material attributes fed into
    \param diffuseColor
      - local color to use across surface diffuse shading
    \param ambientColor
      - flat color used across surface for averaged environmental light
    \param incandescentColor
      - color to render the surface's glow in the centrally facing surface
    \param fresnelColor
      - color to render the surface glow along the tangentially facing surface
    */
    Lambert(Shader& lambert, const v3f& diffuseColor = WHITE,
      const v3f& ambientColor = GRAY_10, const v3f& incandescentColor = BLACK,
      const v3f& fresnelColor = BLACK);


    /** Create a Lambert material using source's attribute settings
    \brief
      - create a Lambert material using source's attribute settings
    \param source
      - material to use for copying attribute settings
    */
    Lambert(const Lambert& source);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Get the current RGB ambient color in use by this Lambert material
    \brief
      - get the current RGB ambient color in use by this Lambert material
    \return
      - float [0,1] RGB value being used for this material ambient shading
    */
    const v3f& Ambient(void) const;


    /** Set the current RGB ambient color to use on this Lambert material
    \brief
      - set the current RGB ambient color to use on this Lambert material
    \param ambientColor
      - float [0,1] RGB value being used for this material ambient shading
    \return
      - reference to the modified Lambert for easier subsequent inline action
    */
    Lambert& Ambient(const v3f& ambientColor);


    /** Get the current RGB diffuse color in use by this Lambert material
    \brief
      - get the current RGB diffuse color in use by this Lambert material
    \return
      - float [0,1] RGB value being used for this material diffuse shading
    */
    const v3f& Diffuse(void) const;


    /** Set the current RGB diffuse color to use on this Lambert material
    \brief
      - set the current RGB diffuse color to use on this Lambert material
    \param diffuseColor
      - float [0,1] RGB value being used for this material diffuse shading
    \return
      - reference to the modified Lambert for easier subsequent inline action
    */
    Lambert& Diffuse(const v3f& diffuseColor);


    /** Get the current RGB fresnel color in use by this Lambert material
    \brief
      - get the current RGB fresnel color in use by this Lambert material
    \return
      - float [0,1] RGB value being used for this material fresnel shading
    */
    const v3f& Fresnel(void) const;


    /** Set the current RGB fresnel color to use on this Lambert material
    \brief
      - set the current RGB fresnel color to use on this Lambert material
    \param fresnelColor
      - float [0,1] RGB value being used for this material fresnel shading
    \return
      - reference to the modified Lambert for easier subsequent inline action
    */
    Lambert& Fresnel(const v3f& fresnelColor);


    /** Get the current RGB incandescent color in use by this Lambert material
    \brief
      - get the current RGB incandescent color in use by this Lambert material
    \return
      - float [0,1] RGB value being used for this material incandescent shading
    */
    const v3f& Incandescence(void) const;


    /** Set the current RGB incandescent color to use on this Lambert material
    \brief
      - set the current RGB incandescent color to use on this Lambert material
    \param incandescentColor
      - float [0,1] RGB value being used for this material incandescent shading
    \return
      - reference to the modified Lambert for easier subsequent inline action
    */
    Lambert& Incandescence(const v3f& incandescentColor);


    /** Get the current XYZ world view coordinates in use by this Material
    \brief
      - get the current XYZ world view coordinates in use by this Material
    \return
      - cartesian view world coordinates being used for this material shading
    */
    const v3f& StationPoint(void) const;


    /** Set the current XYZ world view coordinates for use on this Material
    \brief
      - set the current XYZ world view coordinates for use on this Material
    \param cameraPosition
      - cartesian view world coordinates to reference in drawing this Material
    \return
      - reference to the modified Lambert for easier subsequent inline action
    */
    Lambert& StationPoint(const v3f& cameraPosition);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Operator Overloads                             */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Copy a source Lambert's material attribute settings into this Lambert
    \brief
      - copy a source Lambert's material attribute settings into this Lambert
    \param source
      - Lambert material to have current attribute settings copied
    \return
      - reference to the modified Lambert for easier subsequent inline action
    */
    Lambert& operator=(const Lambert& source);



    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    //! Cartesian world space coords of viewpoint from which a material is seen
    v3f cameraPos;

    //! Color to use in rendering this material's ambient shading
    v3f ambient;

    //! Color to use in rendering this material's diffuse shading
    v3f diffuse;

    //! Color to use in rendering this material's incandescent border shading
    v3f fresnel;

    //! Color to use in rendering this material's incandescent central shading
    v3f incandescence;

  }; // end Lambert class declaration

} // end dp namespace
