/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Phong.h
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

  class Phong : public Material //! Diffuse lighting shading type
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
    \param phong
      - linked, phong shader program to have material attributes fed into
    \param diffuseColor
      - local color bias to use across surface diffuse shading
    \param ambientColor
      - flat color used across surface for averaged environmental light
    \param incandescentColor
      - color to render the surface's glow in the centrally facing surface
    \param fresnelColor
      - color to render the surface glow along the tangentially facing surface
    \param specularColor
      - color to render the surface highlight shading bias
    \param specularFalloff
      - degree to which highlights drop off from shading: low->big, high->small
    */
    Phong(Shader& phong, const v3f& diffuseColor = WHITE,
      const v3f& ambientColor = GRAY_10, const v3f& incandescentColor = BLACK,
      const v3f& fresnelColor = BLACK, const v3f& specularColor = WHITE,
      f4 specularFalloff = 100.0f);


    /** Create a Phong material using source's attribute settings
    \brief
      - create a Phong material using source's attribute settings
    \param source
      - material to use for copying attribute settings
    */
    Phong(const Phong& source);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Get the current RGB ambient color in use by this Phong material
    \brief
      - get the current RGB ambient color in use by this Phong material
    \return
      - float [0,1] RGB value being used for this material ambient shading
    */
    const v3f& Ambient(void) const;


    /** Set the current RGB ambient color to use on this Phong material
    \brief
      - set the current RGB ambient color to use on this Phong material
    \param ambientColor
      - float [0,1] RGB value being used for this material ambient shading
    \return
      - reference to the modified Phong for easier subsequent inline action
    */
    Phong& Ambient(const v3f& ambientColor);


    /** Get the current RGB diffuse color in use by this Phong material
    \brief
      - get the current RGB diffuse color in use by this Phong material
    \return
      - float [0,1] RGB value being used for this material diffuse shading
    */
    const v3f& Diffuse(void) const;


    /** Set the current RGB diffuse color to use on this Phong material
    \brief
      - set the current RGB diffuse color to use on this Phong material
    \param diffuseColor
      - float [0,1] RGB value being used for this material diffuse shading
    \return
      - reference to the modified Phong for easier subsequent inline action
    */
    Phong& Diffuse(const v3f& diffuseColor);


    /** Get the rate at which distance from perfect reflection doesn't draw
    \brief
      - get the rate at which distance from perfect reflection doesn't draw
    \details
      - values must be positive, with low values producing spread highlights,
      high values producing increasingly focused ones
    \return
      - float value indicative of exponential rate of highlights tapering off
    */
    const f4& Falloff(void) const;


    /** Set the rate at which distance from perfect reflection doesn't draw
    \brief
      - set the rate at which distance from perfect reflection doesn't draw
    \param falloffRate
      - roughly [1,10000] range for low->spread & high->focused highlights
    \return
      - reference to the modified Phong for easier subsequent inline action
    */
    Phong& Falloff(f4 falloffRate);


    /** Get the current RGB fresnel color in use by this Phong material
    \brief
      - get the current RGB fresnel color in use by this Phong material
    \return
      - float [0,1] RGB value being used for this material fresnel shading
    */
    const v3f& Fresnel(void) const;


    /** Set the current RGB fresnel color to use on this Phong material
    \brief
      - set the current RGB fresnel color to use on this Phong material
    \param fresnelColor
      - float [0,1] RGB value being used for this material fresnel shading
    \return
      - reference to the modified Phong for easier subsequent inline action
    */
    Phong& Fresnel(const v3f& fresnelColor);


    /** Get the current RGB incandescent color in use by this Phong material
    \brief
      - get the current RGB incandescent color in use by this Phong material
    \return
      - float [0,1] RGB value being used for this material incandescent shading
    */
    const v3f& Incandescence(void) const;


    /** Set the current RGB incandescent color to use on this Phong material
    \brief
      - set the current RGB incandescent color to use on this Phong material
    \param incandescentColor
      - float [0,1] RGB value being used for this material incandescent shading
    \return
      - reference to the modified Phong for easier subsequent inline action
    */
    Phong& Incandescence(const v3f& incandescentColor);


    /** Get the current RGB specular color bias in use by this Phong material
    \brief
      - get the current RGB specular color bias in use by this Phong material
    \return
      - float [0,1] RGB value being used for this material specular shading
    */
    const v3f& Specular(void) const;


    /** Set the current RGB specular color bias to use on this Phong material
    \brief
      - set the current RGB specular color bias to use on this Phong material
    \param specularColor
      - float [0,1] RGB value being used for this material specular shading
    \return
      - reference to the modified Phong for easier subsequent inline action
    */
    Phong& Specular(const v3f& specularColor);


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
      - reference to the modified Phong for easier subsequent inline action
    */
    Phong& StationPoint(const v3f& cameraPosition);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Operator Overloads                             */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Copy a source Phong's material attribute settings into this Phong
    \brief
      - copy a source Phong's material attribute settings into this Phong
    \param source
      - Phong material to have current attribute settings copied
    \return
      - reference to the modified Phong for easier subsequent inline action
    */
    Phong& operator=(const Phong& source);



    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    //! Cartesian world space coords of viewpoint from which a material is seen
    v3f cameraPos;

    //! Color to use in rendering this material's ambient shading
    v3f ambient;

    //! Color bias to use in rendering this material's diffuse shading
    v3f diffuse;

    //! Color to use in rendering this material's emmissive border shading
    v3f fresnel;

    //! Color to use in rendering this material's emmissive central shading
    v3f incandescence;

    //! Color bias to use in rendering this material's eccentricity shading
    v3f specular;

    //! Degree of eccentricity sharpness to use across the material's surface
    f4 rolloff;

  }; // end Phong class declaration

} // end dp namespace
