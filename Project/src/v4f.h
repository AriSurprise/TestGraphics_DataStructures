/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v4f.h
Purpose:  API agnostic, multipurpose, homogeneously promoted 3->4D float vector
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
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

// <stl>
#include <string>       // String building methods for text value output
// "./src/..."
#include "ang.h"        // angle extraction requires arc unit parameter
#include "v3f.h"        // Union read as plane of <v3f, f4>; cyclic includes
#include "Value.h"      // Value manipulation; float point approximation
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  class m4f; // Functions generating matrix data from vectors
  union v2f; // Swizzling operations to truncate data fields
  union v3f; // Swizzling operations to truncate data fields

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  union v4f //! Homogeneously promoted 3->4D vector container of 4-byte floats
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Value range safeguard: & by 111b equivalent and faster than % by 1000b
    static const u1 VSUBS_MASK = 0x3;

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    struct {
      f4 x;   //! Primary axis scale    (cartesian right if y-up; back if z-up)
      f4 y;   //! Secondary axis scale    (cartesian up if y-up; right if z-up)
      f4 z;   //! Tertiary axis scale      (cartesian back if y-up; up if z-up)
      f4 w;   //! Homogeneous dimension state distinction  (usually <0> to (1))
    };
    struct {
      f4 r;   //! 32-bit | HDR RGBA red color channel value    (exposure ratio)
      f4 g;   //! 32-bit | HDR RGBA green color channel value  (exposure ratio)
      f4 b;   //! 32-bit | HDR RGBA blue color channel value   (exposure ratio)
      f4 a;   //! 32-bit | HDR RGBA alpha color channel value   (opacity ratio)
    };
    struct {
      f4 red;  //! 32-bit|HDR RGBA red color value    (exposure ratio in [0,1])
      f4 green;//! 32-bit|HDR RGBA green color value  (exposure ratio in [0,1])
      f4 blue; //! 32-bit|HDR RGBA blue color value   (exposure ratio in [0,1])
      f4 alpha;//! 32-bit|HDR RGBA alpha color value   (opacity ratio in [0,1])
    };
    struct {
      f4 s;   //! Parameter space primary axis value     (texture space x-axis)
      f4 t;   //! Parameter space secondary axis value   (texture space y-axis)
      f4 p;   //! Parameter space primary axis ratio     (texture %/100 x-axis)
      f4 q;   //! Parameter space secondary axis ratio   (texture %/100 y-axis)
    };
    struct {
      f4 x1; //! 2D range's primary axis initial value    (also x|r|s|x1 field)
      f4 y1; //! 2D range's secondary axis initial value  (also y|g|t|y1 field)
      f4 x2; //! 2D range's primary axis terminal value   (also z|b|p|x2 field)
      f4 y2; //! 2D range's secondary axis terminal value (also w|a|q|y2 field)
    };
    struct {
      v3f norm; //! Plane normal direction value (not unit vec restricted)
      f4 sum;   //! Plane sum value: any in-plane point, dotted with normal
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Creates a homogeneous, 3D displacement vector (default <0,0,0,0>)
    \brief
      - creates a homogeneous, 3D displacement vector (default <0,0,0,0>)
    \param xrs_value
      - (x | r | s | x1 field value) - vector primary axis coordinate
    \param ygt_value
      - (y | g | t | y1 field value) - vector secondary axis coordinate
    \param zbp_value
      - (z | b | p | x2 field value) - vector tertiary axis coordinate
    \param waq_value
      - (w | a | q | y2 field value) - vector 4th / homogeneous dimension
    */
    v4f(f4 xrs_value = 0.0f, f4 ygt_value = 0.0f, f4 zbp_value = 0.0f,
         f4 waq_value = 0.0f);


    /** Creates a copy of a v4f from another source v4f
    \brief
      - creates a copy of a v4f from another source v4f
    \param source
      - the v4f to used for memberwise <x|r|s, y|g|f4, z|b|p, w|a|q> copying
    */
    v4f(const v4f& source);


    /** Creates a v4f from a v4f value reference resultant from a function
    \brief
      - creates a v4f from a v4f value reference resultant from a function
    \param result
      - the v4f value reference to surrogate into this
    */
    v4f(v4f&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Get the v4f alpha channel / 4th / homogeneous field using color labels
    \brief
      - get the v4f alpha channel / 4th / homogeneous field using color labels
    \return
      - reference to the contained opacity ratio value in quatertiary field
    */
    inline const f4& A(void) const
    { return b; } // end const f4& v4f::A(void) const                         */


    /** Set the v4f alpha channel / 4th / homogeneous field using color labels
    \brief
      - set the v4f alpha channel / 4th / homogeneous field using color labels
    \param waq_value
      - alpha value to be set in the 4th field / opacity ([0,1] suggested)
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& A(f4 waq_value)
    { a = InF(waq_value, If, Of); return *this; }// end const v4f& v4f::A(f4) */


    /** Set the v4f alpha channel / 4th / homogeneous field using color labels
    \brief
      - set the v4f alpha channel / 4th / homogeneous field using color labels
    \param waq_value
      - alpha value to be set in the 4th field / opacity ([0,1] suggested)
    \param clamp
      - blue color will to be clamped into the given exposure range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& A(f4 waq_value, const v2f& clamp);


    /** Get a copy of the dimensional (x,y,z) values with w overwritten to be 1
    \brief
      - get a copy of the dimensional (x,y,z) values with w overwritten to be 1
    \return
      - copy of this v4f's member dimensional values in [x, y, z, 1.0f]
    */
    v4f AsPoint(void) const;


    /** Get a copy of the dimensional (x,y,z) values with w overwritten to be 0
    \brief
      - get a copy of the dimensional (x,y,z) values with w overwritten to be 0
    \return
      - copy of this v4f's member dimensional values in [x, y, z, 0.0f]
    */
    v4f AsVector(void) const;


    /** [z,y,x, 0] axis rotation in sweep of cartesian members euler angles
    \brief
      - [z,y,x, 0] axis rotation in sweep of cartesian members euler angles
    \details
      - primarily useful in 2D, with 1st member in +x+y plane about +z axis:
      expanding to 3D rotations, 2nd member rotation in +x-z about +y, and
      3rd in -z+y about +x (w always being 0 since rotation is displacement)
    \param units
      - angle unit of arc sweep measure: degrees, radians, gradians, turns...
    \param direction
      - chirality of angles output: clockwise, or counter-clockwise (default)
    \return
      - <z, y, x> ordered euler angle expression of v4f planar rotation sweeps
    */
    v4f Angles(arc units, winding direction = winding::ccw) const;


    /** Get the v4f blue channel / tertiary field value using color labeling
    \brief
      - get the v4f blue channel / tertiary field value using color labeling
    \return
      - reference to the contained blue ratio in tertiary v4f coordinate
    */
    inline const f4& B(void) const
    { return b; } // end const f4& v4f::B(void) const                         */


    /** Set the v4f blue channel / tertiary field value using color labeling
    \brief
      - set the v4f blue channel / tertiary field value using color labeling
    \param zbp_value
      - blue float value to set in the 3rd field / blue channel ([0,1] range)
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& B(f4 zbp_value)
    { b = InF(zbp_value, If, Of); return *this; }// end const v4f& v4f::B(f4) */


    /** Set the v4f alpha channel / 4th / homogeneous field using color labels
    \brief
      - set the v4f alpha channel / 4th / homogeneous field using color labels
    \param zbp_value
      - blue float value to set in the 3rd field / blue channel
    \param clamp
      - blue color will to be clamped into the given exposure range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& B(f4 zbp_value, const v2f& clamp);


    /** Calculate the orthogonal cross product of (this x other) v4f
    \brief
      - calculate the orthogonal cross product of (this &#x2a2f; other) v4f
    \param other
      - v4f to be taken as the cross product's right hand: this &#x2a2f; other
    \return
      - right-handed 3D vector (0 in w|a|q), orthogonal to this and other
    */
    v4f Cross(const v4f& other) const;


    /** Get m4f for getting the cross product when left multiplied with a v4f
    \brief
      - get m4f for getting the cross product when left multiplied with a v4f
    \details
      - leaving the trace as 0, From the -z member, proceeding ccw about the
      linear, changing signs at each subscript, loop around to the bottom at x,
      and fill the linear lower triangle, then upper triangle returning back
      from x to z, still alternating signs at each subscript, until a full ccw
      circuit is reached.
    \return
      - m4f which gets this &#x2a2f (3D) other's mutually normal (3D) vector
    */
    m4f CrossMatrix(void) const;


    /** Calculate the dot product of (other * this) vector (product sum)
    \brief
      - calculate the dot product of (other &middot; this) vector (product sum)
    \details
      - gets memberwise sum of products: a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w
    \param other
      - v4f to be dotted (memberwise multiplied and summed) with this
    \return
      - memberwise sum of products: a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w
    */
    f4 Dot(const v4f& other) const;

    /** Calculate homogeneous dot product with (this * other) v4f (sans w's)
    \brief
      - calculate homogeneous dot product of this &middot; other v4f (no w's)
    \param other
      - v4f to be dotted (dimensional product sum) with this (sans w's)
    \return
      - dimensional memberwise product sum: a.x * b.x + a.y * b.y + a.z * b.z
    */
    f4 DotH(const v4f& other) const;


    /** Compute the matrix for the v4f, dotted with another v4f's transpose
    \brief
      - compute the matrix for the v4f, dotted with another v4f's transpose
    \details
      - if no other v4f is provided, this v4f is self referenced: this*this^T
    \param other
      - pointer to supplementary v4f to be transposed and multiplied with this
    \return
      - matrix of <x, y, z, w> * <other.x, other.y, other.z, other.w>^T
    */
    m4f DotTranspose(const v4f* other = nullptr) const;


    /** Get the v4f green channel / secondary field value with color labeling
    \brief
      - get the v4f green channel / secondary field value with color labeling
    \return
      - reference to the contained green ratio value of the secondary field
    */
    inline const f4& G(void) const
    { return g; } // end const f4& v4f::G(void) const                         */


    /** Set the v4f green channel / secondary field value with color labeling
    \brief
      - set the v4f green channel / secondary field value with color labeling
    \param ygt_value
      - the value to be set into the 2nd field / green channel ([0,1] range)
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& G(f4 ygt_value)
    { g = InF(ygt_value, If, Of); return *this; }// end const v4f& v4f::G(f4) */


    /** Set the v4f green channel / secondary field value with color labeling
    \brief
      - set the v4f green channel / secondary field value with color labeling
    \param ygt_value
      - the value to be set into the 2nd field / green channel ([0,1] suggested)
    \param clamp
      - green color will to be clamped into the given exposure range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& G(f4 ygt_value, const v2f& clamp);


    /** Confirm if the state of the data represents a coordinate (W != 1 or 0)
    \brief
      - confirm if the state of the data represents a coordinate (W != 1 or 0)
    \return
      - true if w is not 0 nor 1: clip coordinates; not a displacement or point
    */
    bool IsClip(void) const;


    /** Confirm if the state of the data represents a coordinate (W = 1)
    \brief
      - confirm if the state of the data represents a coordinate (W = 1)
    \return
      - true if w coordinate is about 1: a coordinate; not displacement or clip
    */
    bool IsPoint(void) const;


    /** Confirm if the state of the data represents a displacement (w = 0)
    \brief
      - confirm if the state of the data represents a displacement (w = 0)
    \return
      - true if w coordinate about 0: a displacement; not clip or point coords.
    */
    bool IsVector(void) const;


    /** Format a v4f for being output as a vector notated string
    \brief
      - format a v4f for being output as a vector notated string
    \details
      - unlike string(), output is consistently [x,y,z,w], regardless
      of dimension value: less semantic output for simpler serdes processes
    \return
      - text formatted to [x,y,z,w] standards for later JSON interpretation
    */
    std::string JSON(void) const;


    /** Get the length of the entire v4f: sqrt(x^2 + y^2 + z^2 + w^2)
    \brief
      - get the length of the entire v4f: sqrt(x^2 + y^2 + z^2 + w^2)
    \details
      - always compare with lengthSquared instead if possible, for performance, 
      if w component inclusion is undesirable, use magnitude() instead
    \return
      - combined length of the v4f's coordinates : sqrt(x^2 + y^2 + z^2 + w^2)
    */
    f4 Length(void) const;


    /** Get the squared length of the v4f as a whole: x^2 + y^2 + z^2 + w^2
    \brief
      - get the squared length of the v4f as a whole: x^2 + y^2 + z^2 + w^2
    \details
      - squares are useful for avoiding comparatively taxing square root calls, 
      if w component inclusion is undesirable, use magnitudeSquared() instead
    \return
      - squared length of all v4f coordinates: x^2 + y^2 + z^2 + w^2
    */
    f4 LengthSquared(void) const;


    /** Get the dimensional coordinates magnitude of the v4f (w|a|q ignored)
    \brief
      - get the dimensional coordinates magnitude of the v4f (w|a|q ignored)
    \details
      - compare with magnitudeSquared instead if possible, for performance, 
      if w component disclusion is undesirable, use length() instead
    \return
      - magnitude of the v4f's dimensional coordinates: sqrt(x^2 + y^2 + z^2)
    */
    f4 Magnitude(void) const;


    /** Get the squared magnitude of the dimensions of the v4f (w|a|q ignored)
    \brief
      - get the squared magnitude of the dimensions of the v4f (w|a|q ignored)
    \details
      - squares are useful for avoiding comparatively taxing square root calls, 
      if w component disclusion is undesirable, use lengthSquared() instead
    \return
      - squared magnitude of the v4f dimensional coords: x^2 + y^2 + z^2 (+0)
    */
    f4 MagnitudeSquared(void) const;


    /** Set the v4f to be treated as a vector: not a point (sets w = 0.0)
    \brief
      - set the v4f to be treated as a vector: not a point (sets w = 0.0)
    \details
      - just discarding prior w value, not making any clip space presumptions
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& MakeVector(void);


    /** Calculate the elementwise product of (other * this) in a new v4f
    \brief
      - calculate the elementwise product of (other * this) in a new v4f
    \details
      - dot products are conventional, but memberwise products are still useful
    \param other
      - v4f to be multiplied elementwise with this in a product v4f
    \return
      - list of elementwise products between this and the other vector
    */
    v4f Multiply(const v4f& other) const;


    /** Set the elementwise product of (other * this) into this vector
    \brief
      - set the elementwise product of (other * this) into this vector
    \details
      - dot products are conventional, but memberwise products are still useful
    \param other
      - v4f to be multiplied elementwise with this as this v4f's new values
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& MultiplyWith(const v4f& other);


    /** This v4f will be divided by its length and converted to a unit vector
    \brief
      - this v4f will be divided by its length and converted to a unit vector
    \details
      - null vector has indeterminate direction and will default to x-axis unit
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& Normalize(void);


    /** Get UV-space tex-sampling U-coord (X-axis) 3rd parameter-space label
    \brief
      - get UV-space tex-sampling U-coord (X-axis) 3rd parameter-space label
    \details
      - UV coordinate space extends beyond [0, 1], texture coords only repeat
    \return
      - reference to contained UV space x-value in tertiary v4f coordinate
    */
    inline const f4& P(void) const
    { return p; } // end const f4& v4f::P(void) const                         */


    /** Set UV-space U-coord (X-axis) in [0,1] of 3rd parameter-space label
    \brief
      - set UV-space U-coord (X-axis) in [0,1] of 3rd parameter-space label
    \param zbp_value
      - UV-space texture-ratio U-coord (X-axis) to set in 3rd field / P-value
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& P(f4 zbp_value)
    { p = ModF(zbp_value,If,Of); return *this; } // end const v4f& v4f::P(f4) */


    /** Set UV-space U-coord (X-axis) in given range of 3rd parameter labels
    \brief
      - set UV-space U-coord (X-axis) in given range of 3rd parameter labels
    \details
      - UV coordinate space extends beyond [0, 1], texture coords only repeat
    \param zbp_value
      - UV-space texture-ratio U-coord (X-axis) to set in 3rd field / P-value
    \param wrap
      - the value will be remapped to be within given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& P(f4 zbp_value, const v2f& wrap);


    /** Get UV-space tex-sampling V-coord (Y-axis) 4th parameter-space labels
    \brief
      - get UV-space tex-sampling V-coord (Y-axis) 4th parameter-space labels
    \details
      - UV coordinate space extends beyond [0, 1], texture coords only repeat
    \return
      - reference to contained UV space y-value in quatertiary v4f coordinate
    */
    inline const f4& Q(void) const
    { return q; } // end const f4& v4f::Q(void) const                         */


    /** Set UV-space V-coord (Y-axis) in [0,1] of 4th parameter-space labels
    \brief
      - set UV-space V-coord (Y-axis) in [0,1] of 4th parameter-space labels
    \param waq_value
      - UV-space texture-ratio V-coord (Y-axis) to set in 4th field / Q-value
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& Q(f4 waq_value)
    { q = ModF(waq_value,If,Of); return *this; } // end const v4f& v4f::Q(f4) */


    /** Set UV-space V-coord (Y-axis) in given range 4th parameter-space label
    \brief
      - set UV-space V-coord (Y-axis) in given range 4th parameter-space label
    \details
      - UV coordinate space extends beyond [0, 1], texture coords only repeat
    \param waq_value
      - UV texture space secondary V-coord to set in 4th field / Q-value
    \param wrap
      - the value will be remapped to be within given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& Q(f4 waq_value, const v2f& wrap);


    /** Get the v4f red channel / primary field value using color labeling
    \brief
      - get the v4f red channel / primary field value using color labeling
    \return
      - reference to the contained red ratio value of the primary field
    */
    inline const f4& R(void) const
    { return r; } // end const f4& v4f::R(void) const                         */


    /** Set the v4f red channel / primary field value with color labeling
    \brief
      - set the v4f red channel / primary field value with color labeling
    \param xrs_value
      - the value to be set into the 1st field / red channel ([0,1] range)
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& R(f4 xrs_value)
    { r = InF(xrs_value, If,Of); return *this; } // end const v4f& v4f::R(f4) */


    /** Set the v4f red channel / primary field value using color labeling
    \brief
      - set the v4f red channel / primary field value using color labeling
    \param xrs_value
      - red float value to set in 1st field / red channel ([0,1] suggested)
    \param clamp
      - red color will to be clamped into the given exposure range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& R(f4 xrs_value, const v2f& clamp);


    /** Set the first 3 axis vectors to 3D space by clip (w coord), division
    \brief
      - set the first 3 axis vectors to 3D space by clip (w coord), division
    \param waq_tolerance
      - how close to 0 (increasingly distorted) should a point be made from w
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& RestorePoint(f4 waq_tolerance = _INf);


    /** Get texture-space pixel-coord X-axis value in 1st parameter-space label
    \brief
      - get texture-space pixel-coord X-axis value in 1st parameter-space label
    \details
      - UV coordinate space extends beyond [0, 1], texture coords only repeat
    \return
      - reference to the contained pixel coordinate x-value in primary element
    */
    inline const f4& S(void) const
    { return s; } // end const f4& v4f::S(void) const                         */


    /** Set texture-space pixel-coord X-axis value in 1st parameter-space label
    \brief
      - set texture-space pixel-coord X-axis value in 1st parameter-space label
    \param xrs_value
      - pixel-coord float-point value to be used for x-axis coord in texture
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& S(f4 xrs_value)
    { s = RealF(xrs_value); return *this; } // end const v4f& v4f::S(f4)      */


    /** Set range-clamped pixel X-axis value in 1st parameter-space label
    \brief
      - set range-clamped pixel X-axis value in 1st parameter-space label
    \details
      - UV coordinate space extends beyond [0, 1], texture coords only repeat
      "clamped pixel" not to imply any form of value rounding to nearest pixel
    \param xrs_value
      - pixel-coord float-point value to be used for x-axis coord in texture
    \param wrap
      - the value will be remapped to be within given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& S(f4 xrs_value, const v2f& wrap);


    /** Set the v4f to <(x|r|s|x1, y|g|t|y1, (z|b|p|y2), (w|a|q|y2)> values
    \brief
      - set the v4f to <(x|r|s|x1, y|g|t|y1, (z|b|p|y2), (w|a|q|y2)> values
    \param xrs_value
      - (x | r | s | x1 field value) - vector primary axis coordinate
    \param ygt_value
      - (y | g | t | y1 field value) - vector secondary axis coordinate
    \param zbp_value
      - (z | b | p | x2 field value) - vector tertiary axis coordinate
    \param waq_value
      - (w | a | q | y2 field value) - vector 4th / homogeneous dimension
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& Set(f4 xrs_value, f4 ygt_value, f4 zbp_value, f4 waq_value);


    /** Set the v4f values to <source.x, source.y, source.z, source.w> values
    \brief
      - set the v4f values to <source.x, source.y, source.z, source.w> values
    \param source
      - the v4f to used for memberwise <x|r|s, y|g|t, z|b|p, w|a|q> copying
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& Set(const v4f& source);


    /** Set the v4f to <result.x, result.y, result.z, result.w>
    \brief
      - set the v4f to <result.x, result.y, result.z, result.w>
    \param result
      - the v4f value reference from which to have values surrogated
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& Set(v4f&& result) noexcept;


    /** Set the v4f to cartesian unit coordinate equivalents in radians
    \brief
      - set the v4f to cartesian unit coordinate equivalents in radians
    \param z_rot
      - amount to rotate counterclockwise about the z axis from 0
    \param y_rot
      - amount to rotate counterclockwise about the y axis from 0
    \param x_rot
      - amount to rotate counterclockwise about the x axis from 0
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& SetAngleEuler(ang z_rot, ang y_rot = nullang, ang x_rot = nullang);


    /** Format a v4f for being output as a vector notated string
    \brief
      - format a v4f for being output as a vector notated string
    \details
      - uses w coordinate to determine the appropriate notation for output, 
      for consistent clip output for serdes purposes, use JSON() instead
    \return
      - text set to <x,y,z> vectors, (x,y,z) points, or [x,y,z,w] clip coords
    */
    std::string String(void) const;


    /** Get texture-space pixel-coord Y-axis value in 2nd parameter-space label
    \brief
      - get texture-space pixel-coord Y-axis value in 2nd parameter-space label
    \details
      - UV coordinate space extends beyond [0, 1], texture coords only repeat
    \return
      -reference to the contained pixel coordinate y-value in secondary element
    */
    inline const f4& T(void) const
    { return t; } // end const f4& v4f::T(void) const                         */


    /** Set texture-space pixel-coord Y-axis value in 2nd parameter-space label
    \brief
      - set texture-space pixel-coord Y-axis value in 2nd parameter-space label
    \param ygt_value
      - pixel-coord float-point value to be used for y-axis coord in texture
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& T(f4 ygt_value)
    { t = RealF(ygt_value); return *this; } // end const v4f& v4f::T(f4)      */


    /** Set range-clamped pixel X-axis value in 1st parameter-space label
    \brief
      - set range-clamped pixel X-axis value in 1st parameter-space label
    \details
      - UV coordinate space extends beyond [0, 1], texture coords only repeat
      "clamped pixel" not to imply any form of value rounding to nearest pixel
    \param xrs_value
      - pixel-coord float-point value to be used for x-axis coord in texture
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    /** Set secondary-axis texture pixel coord in 2nd parameter-space label
    \brief
      - set secondary-axis texture pixel coord in 2nd parameter-space label
    \param ygt_value
      - pixel float-point coord value to be used in y-axis within texture
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& T(f4 ygt_value, const v2f& wrap);


    /** Value copy will be divided by its length and converted to a unit vector
    \brief
      - value copy will be divided by its length and converted to a unit vector
    \return
      - displacement vector with length 1, in the direction of this v4f
    */
    v4f UnitVec(void) const;


    /** Get v4f[3] W-axis homogeneous quaternary element by 3D cartesian labels
    \brief
      - get v4f[3] W-axis homogeneous quaternary element by 3D cartesian labels
    \return
      - reference to homogeneous W-axis value: v4f[3] field, quaternary element
    */
    inline const f4& W(void) const
    { return w; } // end const f4& v4f::W(void) const                         */


    /** Set v4f[3] W value homogeneous quaternary field by 3D cartesian labels
    \brief
      - set v4f[3] W value homogeneous quaternary field by 3D cartesian labels
    \param waq_value
      - W-axis float value to set in v4f [3] homogeneous field / 4th element
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& W(f4 waq_value)
    { w = RealF(waq_value); return *this; } // end const v4f& v4f::W(f4)      */


    /** Set v4f[3] clamped W value in quaternary element by 3D cartesian labels
    \brief
      - set v4f[3] clamped W value in quaternary element by 3D cartesian labels
    \param waq_value
      - W-axis homogeneous <0> or (1) float value to set in v4f [3], 4th element
    \param clamp
      - the value will be constrained to be in given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& W(f4 waq_value, const v2f& clamp);


    /** Get X-axis v4f [0] primary-element value under 3D cartesian labeling
    \brief
      - get X-axis v4f [0] primary-element value under 3D cartesian labeling
    \return
      - reference to cartesian X-axis value in v4f [0] field; primary element
    */
    inline const f4& X(void) const
    { return x; } // end const f4& v4f::X(void) const                         */


    /** Set X-axis value in v4f [0] primary element by 3D cartesian labeling
    \brief
      - set X-axis value in v4f [0] primary element by 3D cartesian labeling
    \param xrs_value
      - the float-point X-axis value to be set in v4f [0] field / 1st element
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& X(f4 xrs_value)
    { x = RealF(xrs_value); return *this; } // end const v4f& v4f::X(f4)      */


    /** Set clamped X-axis value in v4f [0] primary by 3D cartesian labeling
    \brief
      - set clamped X-axis value in v4f [0] primary by 3D cartesian labeling
    \param xrs_value
      - the float-point X-axis value to be set in v4f [0] field / 1st element
    \param clamp
      - the value will be constrained to be in given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& X(f4 xrs_value, const v2f& clamp);


    /** Get 2D-pair initial X-coordinate (1st field; [0]) by 2D-pair labels
    \brief
      - get 2D-pair initial X-coordinate (1st field; [0]) by 2D-pair labels
    \return
      - reference to cartesian initial X-value in v4f [1] / secondary element
    */
    inline const f4& X1(void) const
    { return x1; } // end const f4& v4f::X1(void) const                       */


    /** Set the 2D-pair initial X-coordinate (1st field; [0]) by 2D-pair labels
    \brief
      - set the 2D-pair initial X-coordinate (1st field; [0]) by 2D-pair labels
    \param x1s_value
      - initial-pair's X-axis float-point value to set in v4f [0] / 1st element
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& X1(f4 x1s_value)
    { x1 = RealF(x1s_value); return *this; } // end const v4f& v4f::X1(f4)    */


    /** Set clamped 2D-pair initial X-coord (1st field; [0]) by 2D-pair labels
    \brief
      - set clamped 2D-pair initial X-coord (1st field; [0]) by 2D-pair labels
    \param x1s_value
      - initial-pair's X-axis float-point value to set in v4f [0] / 1st element
    \param clamp
      - the value will be constrained to be in given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& X1(f4 x1s_value, const v2f& clamp);


    /** Get 2D-pair terminal X-coordinate (3rd field; [2]) by 2D-pair labels
    \brief
      - get 2D-pair terminal X-coordinate (3rd field; [2]) by 2D-pair labels
    \return
      - reference to cartesian terminal X-value in v4f [2] / tertiary element
    */
    inline const f4& X2(void) const
    { return x2; } // end const f4& v4f::X2(void) const                       */


    /** Set 2D-pair terminal X-coordinate (3rd field; [2]) by 2D-pair labels
    \brief
      - set 2D-pair terminal X-coordinate (3rd field; [2]) by 2D-pair labels
    \param x2p_value
      - terminal-pair's X-axis float-point value to set: v4f [2] / 3rd element
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& X2(f4 x2p_value)
    { x2 = RealF(x2p_value); return *this; } // end const v4f& v4f::X2(f4)    */


    /** Set clamped 2D-pair terminal X-coord (3th field; [2]) by 2D-pair labels
    \brief
      - set clamped 2D-pair terminal X-coord (3th field; [2]) by 2D-pair labels
    \param x2p_value
      - terminal-pair's X-axis float-point value to set in v4f[2] / 3rd element
    \param clamp
      - the value will be constrained to be in given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& X2(f4 x2p_value, const v2f& clamp);


    /** <x, y> 2D swizzling operation to get the first 2 dimensions
    \brief
      - <x, y> 2D swizzling operation to get the first 2 dimensions
    \return
      - a v2f set to use the <x, y> values of this v4f
    */
    v2f XY(void) const;

    /** <x, x, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, x, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, x, x> values of this v4f
    */
    v3f XXX(void) const;

    /** <x, x, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, x, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, x, y> values of this v4f
    */
    v3f XXY(void) const;

    /** <x, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, x, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, x, z> values of this v4f
    */
    v3f XXZ(void) const;

    /** <x, y, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, y, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, y, x> values of this v4f
    */
    v3f XYX(void) const;

    /** <x, y, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, y, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, y, y> values of this v4f
    */
    v3f XYY(void) const;

    /** <x, y, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, y, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, y, z> values of this v4f
    */
    v3f XYZ(void) const;

    /** <x, z, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, z, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, z, x> values of this v4f
    */
    v3f XZX(void) const;

    /** <x, z, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, z, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, z, y> values of this v4f
    */
    v3f XZY(void) const;

    /** <x, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, z, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, z, z> values of this v4f
    */
    v3f XZZ(void) const;


    /** Get Y-axis v4f [1] secondary-element value under 3D cartesian labeling
    \brief
      - get Y-axis v4f [1] secondary-element value under 3D cartesian labeling
    \return
      - reference to cartesian Y-axis value in v4f [1] field; secondary element
    */
    inline const f4& Y(void) const
    { return y; } // end const f4& v4f::Y(void) const                         */


    /** Set Y-axis value in v4f [1] secondary element by 3D cartesian labels
    \brief
      - set Y-axis value in v4f [1] secondary element by 3D cartesian labels
    \param ygt_value
      - the float-point Y-axis value to be set in v4f [1] field / 2nd element
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& Y(f4 ygt_value)
    { y = RealF(ygt_value); return *this; } // end const v4f& v4f::Y(f4)      */


    /** Set clamped Y-axis value in v4f [1] secondary by 3D cartesian labeling
    \brief
      - set clamped Y-axis value in v4f [1] secondary by 3D cartesian labeling
    \param ygt_value
      - the float-point Y-axis value to be set in v4f [1] field / 2nd element
    \param clamp
      - the value will be constrained to be in given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& Y(f4 ygt_value, const v2f& clamp);


    /** Get 2D-pair initial Y-coordinate (2nd field; [1]) by 2D-pair labels
    \brief
      - get 2D-pair initial Y-coordinate (2nd field; [1]) by 2D-pair labels
    \return
      - reference to cartesian initial Y-value in v4f [1] / secondary element
    */
    inline const f4& Y1(void) const
    { return y1; } // end const f4& v4f::Y1(void) const                       */


    /** Set the 2D-pair initial Y-coordinate (2nd field; [1]) by 2D-pair labels
    \brief
      - set the 2D-pair initial Y-coordinate (2nd field; [1]) by 2D-pair labels
    \param y1t_value
      - initial-pair's y-axis float-point value to set in v4f [1] / 2nd element
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& Y1(f4 y1t_value)
    { y1 = RealF(y1t_value); return *this; } // end const v4f& v4f::Y1(f4)    */


    /** Set clamped 2D-pair initial Y-coord (2nd field; [1]) by 2D-pair labels
    \brief
      - set clamped 2D-pair initial Y-coord (2nd field; [1]) by 2D-pair labels
    \param y1t_value
      - initial-pair's y-axis float-point value to set in v4f [1] / 2nd element
    \param clamp
      - the value will be constrained to be in given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& Y1(f4 y1t_value, const v2f& clamp);


    /** Get 2D-pair terminal Y-coordinate (4th field; [3]) by 2D-pair labels
    \brief
      - get 2D-pair terminal Y-coordinate (4th field; [3]) by 2D-pair labels
    \return
      - reference to cartesian terminal Y-value in v4f [3] / quaternary element
    */
    const f4& Y2(void) const
    { return y2; } // end const f4& v4f::Y2(void) const                       */


    /** Set 2D-pair terminal Y-coordinate (4th field; [3]) by 2D-pair labels
    \brief
      - set 2D-pair terminal Y-coordinate (4th field; [3]) by 2D-pair labels
    \param y2q_value
      - terminal-pair's Y-axis float-point value to set in v4f[3] / 4th element
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& Y2(f4 y2q_value)
    { y2 = RealF(y2q_value); return *this; } // end const v4f& v4f::Y2(f4)    */


    /** Set clamped 2D-pair terminal Y-coord (4th field; [3]) by 2D-pair labels
    \brief
      - set clamped 2D-pair terminal Y-coord (4th field; [3]) by 2D-pair labels
    \param y2q_value
      - terminal-pair's Y-axis float-point value to set in v4f[3] / 4th element
    \param clamp
      - the value will be constrained to be in given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& Y2(f4 y2q_value, const v2f& clamp);


    /** <y, x, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, x, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, x, x> values of this v4f
    */
    v3f YXX(void) const;


    /** <y, x, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, x, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, x, y> values of this v4f
    */
    v3f YXY(void) const;


    /** <y, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, x, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, x, z> values of this v4f
    */
    v3f YXZ(void) const;


    /** <y, y, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, y, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, y, x> values of this v4f
    */
    v3f YYX(void) const;


    /** <y, y, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, y, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, y, y> values of this v4f
    */
    v3f YYY(void) const;
    

    /** <y, y, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, y, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, y, z> values of this v4f
    */
    v3f YYZ(void) const;


    /** <y, z, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, z, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, z, x> values of this v4f
    */
    v3f YZX(void) const;


    /** <y, z, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, z, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, z, y> values of this v4f
    */
    v3f YZY(void) const;


    /** <y, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, z, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, z, z> values of this v4f
    */
    v3f YZZ(void) const;


    /** Get Z-axis v4f [2] tertiary-element value under 3D cartesian labeling
    \brief
      - get Z-axis v4f [2] tertiary-element value under 3D cartesian labeling
    \return
      - reference to cartesian Z-axis value in v4f [2] field; tertiary element
    */
    inline const f4& Z(void) const
    { return z; } // end const f4& v4f::Z(void) const                         */


    /** Set Z-axis coord value in v4f [2] tertiary field by 3D cartesian labels
    \brief
      - set Z-axis coord value in v4f [2] tertiary field by 3D cartesian labels
    \param zbp_value
      - the float-point Z-axis value to be set in v4f [2] field / 3rd element
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    inline v4f& Z(f4 zbp_value)
    { z = RealF(zbp_value); return *this; } // end const v4f& v4f::Z(f4)      */


    /** Set clamped Z-axis in v4f[2] tertiary value by 3D cartesian labeling
    \brief
      - set clamped Z-axis in v4f[2] tertiary value by 3D cartesian labeling
    \param zbp_value
      - the float-point Z-axis value to be set in v4f [2] field / 3rd element
    \param clamp
      - the value will be constrained to be in given [min, max] value range
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& Z(f4 zbp_value, const v2f& clamp);


    /** <z, x, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, x, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, x, x> values of this v4f
    */
    v3f ZXX(void) const;


    /** <z, x, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, x, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, x, y> values of this v4f
    */
    v3f ZXY(void) const;


    /** <z, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, x, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, x, z> values of this v4f
    */
    v3f ZXZ(void) const;


    /** <z, y, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, y, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, y, x> values of this v4f
    */
    v3f ZYX(void) const;


    /** <z, y, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, y, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, y, y> values of this v4f
    */
    v3f ZYY(void) const;


    /** <z, y, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, y, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, y, z> values of this v4f
    */
    v3f ZYZ(void) const;


    /** <z, z, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, z, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, z, x> values of this v4f
    */
    v3f ZZX(void) const;


    /** <z, z, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, z, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, z, y> values of this v4f
    */
    v3f ZZY(void) const;


    /** <z, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, z, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, z, z> values of this v4f
    */
    v3f ZZZ(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Creates a negated copy with relevant v4f coordinate values (not w|a|q)
    \brief
      - creates a negated copy with relevant v4f coordinate values (not w|a|q)
    \return
      - copy of the v4f with [-x, -y, -z, w] member values
    */
    v4f operator-(void) const;


    /** Sets the source v4f member values into this v4f value fields
    \brief
      - sets the source v4f member values into this v4f value fields
    \param source
      - the v4f which will have its values copied memberwise into this v4f
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& operator=(const v4f& source);


    /** Surrogate a v4f value reference returned from a function for this v4f
    \brief
      - surrogate a v4f value reference returned from a function for this v4f
    \param result
      - the v4f reference resultant from a function from which to steal values
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& operator=(v4f&& result) noexcept;


    /** Negates relevant v4f coordinate values (not w|a|q), to invert placement
    \brief
      - negates relevant v4f coordinate values (not w|a|q), to invert placement
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& operator!(void);


    /** Get a value from ['x'], ['y'], ['z'], or ['w'] channels (or equivalent)
    \brief
      - get a value from ['x'], ['y'], ['z'], or ['w'] channels (or equivalent)
    \details
      - beyond labeled values, iterators i, j k, l have ordered logic in place,
      as do m & n as the arbitrary x & y dimension aliases, since a & b are
      already in rgba, extending the aliases to cdba makes some sense for pairs,
      and by extension, cdef to the equivalent hex values, p & q have library
      standards as precedent, but u & v are arguably more intuitive aliases to
      offer as well, with the same data expected for p|u, q|v without conflicts.
      All numeral values 0-F can map cyclically to [0,3] as would be expected
      (roughly) for a subscript operator to take as standard input, and upper
      case letters can be remapped to lower case, leaving only 'o', 'h', 
      punctuation and a few control sequence values as well as other non-ascii
      values. A surprising number of letters map well to 4 channels honestly: 
      the only real disappointment / contention is that (b)lue == 10 and
      (a) == 11 instead of the other way around for hex values to be uniform,
      but it hardly seems wise to swap for the edge case uniformity at the
      expense of maintaining sequential value read order, and the alternative
      would be to reverse cd to make dcba, with looparound values to dcfe,
      which would allow the intended [0,3] range to map to a reverse byte
      chunk ordering starting from the middle of the vector instead of the head,
      which would have sequential fields and sequential labels without changing
      rgba label standards or order, but isn't more intuitive by any stretch.
    \param channel
      - use 'x'|'r'|'s'->x, 'y'|'g'|'t'->y, 'z'|'b'|'p'->z, or 'w'|'a'|'q'->w
    \return
      - copy of the contained dimension member value associated to the channel
    */
    const f4& operator[](s1 channel) const;


    /** Get a value from ['x'], ['y'], ['z'], or ['w'] channels (or equivalent)
    \brief
      - get a value from ['x'], ['y'], ['z'], or ['w'] channels (or equivalent)
    \details
      - to be more explicit and thorough in which values map to which channels:
      ( x | X | r | R | s | S | i | I | m | M | 0 | 4 |  8 | 12 | c | C ) -> x
      ( y | Y | g | G | t | T | j | J | n | N | 1 | 5 |  9 | 13 | d | D ) -> y
      ( z | Z | b | B | p | P | k | K | u | U | 2 | 6 | 10 | 14 | e | E ) -> z
      ( w | W | a | A | q | Q | l | L | v | V | 3 | 7 | 11 | 15 | f | F ) -> w
    \param channel
      - 0|'x'|'r'|'s'->x, 1|'y'|'g'|'t'->y, 2|'z'|'b'|'p'->z, 3|'w'|'a'|'q'->w
    \return
      - reference to the contained field associated to the subscript number
    */
    f4& operator[](s1 channel);


    /** Calculate the member-wise addition of 2 v4f's and return the result
    \brief
      - calculate the member-wise addition of 2 v4f's and return the result
    \param other
      - source vector to have memberwise summation performed with a copy of this
    \return
      - copy of the memberwise sum of this v4f and the other into a new v4f
    */
    v4f operator+(const v4f& other) const;


    /** Add an other v4f's values memberwise into this v4f's values
    \brief
      - add an other v4f's values memberwise into this v4f's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& operator+=(const v4f& other);


    /** Calculate the member-wise difference of 2 v4f's and return the result
    \brief
      - calculate the member-wise difference of 2 v4f's and return the result
    \param other
      - source vector to have memberwise difference calculated with this (copy)
    \return
      - copy of the memberwise difference of this and the other, in a new v4f
    */
    v4f operator-(const v4f& other) const;


    /** Subtract an other v4f's values memberwise from this v4f's values
    \brief
      - subtract an other v4f's values memberwise from this v4f's values
    \param other
      - source vector to have negated memberwise summation performed with this
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& operator-=(const v4f& other);


    /** Scalar product multiplies each field uniformly by a scalar constant
    \brief
      - scalar product multiplies each field uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x, y, z and w values
    \return
      - resultant, member-wise scaled copy of this v4f's values
    */
    v4f operator*(f4 scalar) const;


    /** Scalar product sets all members to grow uniformly by a scalar constant
    \brief
      - scalar product sets all members to grow uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x, y, z and w values
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& operator*=(f4 scalar);


    /** (Dot) multiply by the equivalent other members, and sum the result
    \brief
      - (dot) multiply by the equivalent other members, and sum the result
    \details
      - memberwise multiplication not as useful as dot product of 2 vectors
      which has the property of being ||u||*||v||*cos(angle).  If performed on
      unit vectors, this will be the cosine of the angle between them, ie their
      relative facing ratio, which has far greater applications, and
      unfortunately, 2 overloads can't have the same parameter list.
    \param other
      - external source vector to be dot multiplied with this vector
    \return
      - dot product of members: a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w
    */
    f4 operator*(const v4f& other) const;


    /** Perform uniform division by divisor, returning the result
    \brief
      - perform uniform division by divisor, returning the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which <x,y,z,w> values are uniformly divided
    \return
      - resultant, member-wise scaled copy of this v4f's values
    */
    v4f operator/(f4 divisor) const;


    /** Divides all members uniformly by divisor, setting the result
    \brief
      - divides all members uniformly by divisor, setting the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which <x,y,z,w> values are to be uniformly divided
    \return
      - reference to the modified v4f for easier subsequent inline action
    */
    v4f& operator/=(f4 divisor);


  }; // end v4f union / struct / class

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Generate a distance / displacement vector between two coordinate vectors
  \brief
    - generate a distance / displacement vector between two coordinate vectors
  \param initial
    - starting point (or vector; w ultimately wiped to 0), being measured
  \param terminal
    - ending point (or vector; w ultimately wiped to 0), being measured
  \return
    - displacement from initial to terminal (ie memberwise terminal - initial)
  */
  v4f Distance(const v4f& initial, const v4f& terminal);


  /** Left hand scalar multiplication scales only dimensional members of rhs
  \brief
    - left hand scalar multiplication scales only dimensional members of rhs
  \details
    - constants can't be divided by v4f, so w can be treated special
    due to this: the left hand operation affords an affordance in the
    asymmetry of available operations. For homogeneous vectors this should
    scale w as well, but as when one wants to scale a point, they mean to
    scale that as a vector in 3D space, not to change the fundamental
    dimensionality of that 3D point's anchor into a different 4D frame's
    volume, this can be skipped here. It may be counterintuitive that
    ordering matters, and left-multiplying may be handled differently than
    right-multiplying, but this is vector math, so losing commutativity
    (and only partly), is already par for the course. Simply change the
    implementation to scale w on your copy if you disagree with the decision.
  \param scalar
    - scalar amount by which to multiply x, y and z values of right-hand side
  \param rhs
    - right-hand side argument to be copied and scaled in x, y and z by scalar
  \return
    - copy of rhs v4f's values, scaled by scalar in <x,y,z>, with <w> intact
  */
  v4f operator*(f4 scalar, const v4f& rhs);


  /** Confirm if 2 v4f's are relatively equal in x, y, z & w values
  \brief
    - confirm if 2 v4f's are relatively equal in x, y, z & w values
  \param lhs
    - left hand side to be compared memberwise with right hand for equality
  \param rhs
    - right hand side to be compared memberwise with left hand for equality
  \return
    - true only if all members are in close proximity of one another
  */
  bool operator==(const v4f& lhs, const v4f& rhs);


  /** Confirm if 2 v4f's are not equal in any of x, y, z or w values
  \brief
    - confirm if 2 v4f's are not equal in any of x, y, z or w values
  \param lhs
    - left hand side to be compared memberwise with right hand for inequality
  \param rhs
    - right hand side to be compared memberwise with left hand for inequality
  \return
    - true if any members are not in close proximity of one another
  */
  bool operator!=(const v4f& lhs, const v4f& rhs);


  /** Output the string version of the vector data: <vector>, (point) or [clip]
  \brief
    - output the string version of the vector data: <vector>, (point) or [clip]
  \param output
    - stream to receive the vector string and and carry contents for output
  \param vector
    - vector to have the member values be converted to text based output
  \return
    - reference to the output stream object having the vector fed in
  */
  std::ostream& operator<<(std::ostream& output, const v4f& vector);


  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Helper Constants                             */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Zero vector in 4 dimensions; error state reserved value
  extern const v4f& nullv4f;

  //! Zero coordinate point definition in any given, spanning, 3D vector space
  extern const v4f& ORIGIN_POINT;

  //! Unit vector in the positive-x axis
  extern const v4f& UNIT_VEC_X;

  //! Unit vector in the positive-y axis
  extern const v4f& UNIT_VEC_Y;

  //! Unit vector in the positive-z axis
  extern const v4f& UNIT_VEC_Z;


} // end dp namespace
