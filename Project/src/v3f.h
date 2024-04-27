/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v3f.h
Purpose:  API agnostic, multipurpose, 3 dimensional float-point element vector
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
#include "ang.h"        // Angle extraction requires arc unit parameter
#include "ASCIIValue.h" // char range based inline operator[] manipulation
#include "TypeErrata.h" // Platform primitive type aliases for easier porting
#include "Value.h"      // Value manipulation; float point approximation
#include "vSpace.h"     // Cartesian plane definitions for various operatons

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  class m3f;
  union v2f;
  union v4f;

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  union v3f //! 3D vector container of 3, 4-byte float point elements
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    f4 v[3];  //! Sequential value array (equivalent to prefer operator[] use)
    struct {
      f4 x;   //! Primary axis scale    (cartesian right if y-up; back if z-up)
      f4 y;   //! Secondary axis scale    (cartesian up if y-up; right if z-up)
      f4 z;   //! Tertiary axis scale      (cartesian back if y-up; up if z-up)
    };
    struct {
      f4 r;   //! 32-bit | HDR RGB red color channel value    (exposure ratio)
      f4 g;   //! 32-bit | HDR RGB green color channel value  (exposure ratio)
      f4 b;   //! 32-bit | HDR RGB blue color channel value   (exposure ratio)
    };
    struct {
      f4 red;  //! 32-bit | HDR RGB red color value   (exposure ratio in [0,1])
      f4 green;//! 32-bit | HDR RGB green color value (exposure ratio in [0,1])
      f4 blue; //! 32-bit | HDR RGB blue color value  (exposure ratio in [0,1])
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Create a 3 dimensional vector / point (default <0,0,0>)
    \brief
      - create a 3 dimensional vector / point (default <0,0,0>)
    \param xr_value
      - (x | r | red field value)   - vector primary axis coordinate
    \param yg_value
      - (y | g | green field value) - vector secondary axis coordinate
    \param zb_value
      - (z | b | blue field value)  - vector tertiary axis coordinate
    */
    v3f(f4 xr_value = 0.0f, f4 yg_value = 0.0f, f4 zb_value = 0.0f);


    /** Create a 3D vector in a 2D plane of world axes from polar-form inputs
    \brief
      - create a 3D vector in a 2D plane of world axes from polar-form inputs
    \param angle
      - arc of displacement in the given world axis plane from angle origin
    \param on
      - world plane (at origin) the mesh hull is aligned to be contained in
    \param magnitude
      - diagonal length of the intended vector displacement sqrt(x^2+y^2+z^2)
    */
    v3f(ang angle, PLANE on = PLANE::XY, f4 magnitude = 1.0f);


    /** Creates a copy of a v3f from another source v3f
    \brief
      - creates a copy of a v3f from another source v3f
    \param source
      - the v3f to used for memberwise <x|r, y|g, z|b> element value copying
    */
    v3f(const v3f& source);


    /** Creates a v3f from a v3f value reference resultant from a function
    \brief
      - creates a v3f from a v3f value reference resultant from a function
    \param result
      - the v3f value reference to surrogate into this
    */
    v3f(v3f&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get a v3f with the magnitudes of this v3f's <x,y,z> memberwise values
    \brief
      - get a v3f with the magnitudes of this v3f's <x,y,z> memberwise values
      \return
      - this v3f's data's memberwise absolute magnitues copied into a new v3f
    */
    inline v3f Abs(void) const
    { return v3f(AbsF(x), AbsF(y), AbsF(z)); } // end v3f v3f::Abs() const    */

    /** Get a copy of this v3f (x,y,z) values with implicit point semantic
    \brief
      - get a copy of this v3f (x,y,z) values with implicit point semantic
    \return
      - copy of this v3f (x, y, z) as a Point3 with implicit w=[1] class value
    */
    //Point3 AsPoint(void) const;


    /** Get a copy of this v3f <x,y,z> values with implicit vector semantic
    \brief
      - get a copy of this v3f <x,y,z> values with implicit vector semantic
    \details
      - TODO: Vec3 type as distinct v3f wrapper with implicit homogeneous coord
    \return
      - copy of this v3f <x, y, z> as a Vec3 with implicit w=[0] class value
    */
    //Vec3 AsVector(void) const;


    /** <z,y,x> axis rotation in sweep of cartesian members euler angles
    \brief
      - <z,y,x> axis rotation in sweep of cartesian members euler angles
    \details
      - primarily useful in 2D, with 1st member in +x+y plane about +z axis:
      expanding to 3D rotations, 2nd member rotation in +x-z about +y, and
      3rd in -z+y about +x (w always being 0 since rotation is displacement)
    \param units
      - angle unit of arc sweep measure: degrees, radians, gradians, turns...
    \param direction
      - chirality of angles output: clockwise, or counter-clockwise (default)
    \return
      - <z, y, x> ordered euler angle expression of v3f planar rotation sweeps
    */
    inline v3f Angles(arc units, winding direction = winding::ccw) const
    {
      // Rotation about +y has +x > & +z v (Q1), or +z > & +x ^ (Q4)
      // Rotation about +x has +z < & +y ^ (Q1), or +y > & +z v (Q2)
      ang rX(ATan2F(y, x)), rY = ATan2F(-z, x), rZ = ATan2F(y, -z);
      return direction==winding::ccw?
        units == arc::radians?        v3f(rX.r(), rY.r(), rZ.r())
        : units == arc::degrees?      v3f(rX.d(), rY.d(), rZ.d())
        : units == arc::revolutions?  v3f(rX.s(), rY.s(), rZ.s())
        : /* => ccw gradians */       v3f(rX.g(), rY.g(), rZ.g())
        : units == arc::radians?     v3f(-rX.r(),-rY.r(),-rZ.r())
        : units == arc::degrees?     v3f(-rX.d(),-rY.d(),-rZ.d())
        : units == arc::revolutions? v3f(-rX.s(),-rY.s(),-rZ.s())
        : /* => cw gradians */       v3f(-rX.g(),-rY.g(),-rZ.g());
    } // end v3f v3f::Angles(void) const                                      */


    /** Get the v3f blue channel / tertiary field value using color labeling
    \brief
      - get the v3f blue channel / tertiary field value using color labeling
    \return
      - reference to the contained blue ratio in tertiary v3f coordinate
    */
    inline const f4& B(void) const
    { return Z(); } // end const f4& v3f::B(void) const                       */


    /** Set the v3f blue channel / tertiary field value using color labeling
    \brief
      - set the v3f blue channel / tertiary field value using color labeling
    \param zb_value
      - blue float value to set in 3rd field / blue channel ([0,1] suggested)
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& B(f4 zb_value)
    { return Z(zb_value); } // end v3f& v3f::B(f4)                            */


    /** Set the v3f blue channel / tertiary field value using color labeling
    \brief
      - set the v3f blue channel / tertiary field value using color labeling
    \param zb_value
      - blue float value to set in 3rd field / blue channel ([0,1] suggested)
    \param clamp
      - if given, value will to be clamped into given, real [min,max] range
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& B(f4 zb_value, const v2f& clamp)
    { return Z(zb_value, clamp); } // end v3f& v3f::B(f4, const v2f&)         */


    /** Get whether each vector member is between [min,max] on each axis
    \brief
      - get whether each vector member is between [min,max] on each axis
    \param min
      - per axis minimal values allowed in memberwise dimensional fields
    \param max
      - per axis maximal values allowed in memberwise dimensional fields
    \param mode
      - comparitor operator extrema preferences for <= / < and >= / > at ends
    \return
      - true only if all member values are between memberwise [min,max] range
    */
    inline bool IsIn(const v3f& max, const v3f& min)
    const
    {
      return IsInF(x, max.x, min.x)
          && IsInF(y, max.y, min.y)
          && IsInF(z, max.z, min.z);
    } // end bool v3f::Between(const v3f&, const v3f&)                        */


    /** Get whether each vector member is between [min,max] on each axis
    \brief
      - get whether each vector member is between [min,max] on each axis
    \param min
      - per axis minimal values allowed in memberwise dimensional fields
    \param max
      - per axis maximal values allowed in memberwise dimensional fields
    \param mode
      - comparitor operator extrema preferences for <= / < and >= / > at ends
    \return
      - true only if all member values are between memberwise [min,max] range
    */
    inline bool IsInS(const v3f& max, const v3f& min)
    const
    {
      return IsInSF(x, max.x, min.x)
          && IsInSF(y, max.y, min.y)
          && IsInSF(z, max.z, min.z);
    } // end bool v3f::BetweenS(const v3f&, const v3f&)                       */


    /** Get whether each vector member is between [min,max] on each axis
    \brief
      - get whether each vector member is between [min,max] on each axis
    \param min
      - per axis minimal values allowed in memberwise dimensional fields
    \param max
      - per axis maximal values allowed in memberwise dimensional fields
    \param mode
      - comparitor operator extrema preferences for <= / < and >= / > at ends
    \return
      - true only if all member values are between memberwise [min,max] range
    */
    inline bool IsInSA(const v3f& max, const v3f& min)
    const
    {
      return IsInSAF(x, max.x, min.x)
          && IsInSAF(y, max.y, min.y)
          && IsInSAF(z, max.z, min.z);
    } // end bool v3f::BetweenSA(const v3f&, const v3f&)                      */


    /** Get whether each vector member is between [min,max] on each axis
    \brief
      - get whether each vector member is between [min,max] on each axis
    \param min
      - per axis minimal values allowed in memberwise dimensional fields
    \param max
      - per axis maximal values allowed in memberwise dimensional fields
    \param mode
      - comparitor operator extrema preferences for <= / < and >= / > at ends
    \return
      - true only if all member values are between memberwise [min,max] range
    */
    inline bool IsInSB(const v3f& max, const v3f& min)
    const
    {
      return IsInSBF(x, max.x, min.x)
          && IsInSBF(y, max.y, min.y)
          && IsInSBF(z, max.z, min.z);
    } // end bool v3f::BetweenSB(const v3f&, const v3f&)                      */


    /** Get if another vector is a dimensional scalar multiple of this vector
    \brief
      - get if another vector is a dimensional scalar multiple of this vector
    \details
      - presumably both v3f have vector data: points are only origin relevant
    \param other
      - vector to be compared with this to test for scalar equivalency
    \return
      - true only if other is a scalar multiple of this v3f's data
    */
    bool Colinear(const v3f& other) const;


    /** Get a memberwise copy of this vector's data
    \brief
      - get a memberwise copy of this vector's data
    \return
      - v3f(*this): a convenience wrapper to copy constructor of existing data
    */
    inline v3f Copy(void) const
    { return v3f(*this); } // end v3f::v3f Copy(void) const                   */


    /** Calculate the orthogonal cross product of (this x other) v3f
    \brief
      - calculate the orthogonal cross product of (this &#x2a2f; other) v3f
    \param other
      - v3f to be taken as the cross product's right hand: this &#x2a2f; other
    \return
      - right-handed 3D vector (0 in w|a|q), orthogonal to this and other
    */
    inline v3f Cross(const v3f& other) const
    {
      return v3f( ((y * other.z) - (z * other.y)),
                  ((z * other.x) - (x * other.z)),
                  ((x * other.y) - (y * other.x)));

    } // end v3f v3f::Cross(const v3f&) const                                 */


    /** Get m3f for getting the cross product when left multiplied with a v3f
    \brief
      - get m3f for getting the cross product when left multiplied with a v3f
    \details
      - leaving the trace as 0, From the -z member, proceeding ccw about the
      linear, changing signs at each subscript, loop around to the bottom at x,
      and fill the linear lower triangle, then upper triangle returning back
      from x to z, still alternating signs at each subscript, until a full ccw
      circuit is reached.
    \return
      - m4f which gets this &#x2a2f (3D) other's mutually normal (3D) vector
    */
    m3f CrossMatrix(void) const;


    /** Calculate the dot product of (other * this) vector (product sum)
    \brief
      - calculate the dot product of (other &middot; this) vector (product sum)
    \details
      - gets memberwise sum of products: a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w
    \param other
      - v3f to be dotted (memberwise multiplied and summed) with this
    \return
      - memberwise sum of products: a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w
    */
    inline f4 Dot(const v3f& other) const
    {
      return (*this * other).Sum();
    } // end f4 v3f::Dot(const v3f&) const                                    */


    /** Calculate the dot product of (this * this) vector (product sum)
    \brief
      - calculate the dot product of (this * this) vector (product sum)
    \return
      - memberwise sum of squared products: x^2 + y^2 + z^2
    */
    inline f4 DotSelf(void) const
    { return Dot(*this); } // end f4 DotSelf(void) const                      */


    /** Compute the matrix for the v3f, dotted with another v3f's transpose
    \brief
      - compute the matrix for the v3f, dotted with another v3f's transpose
    \param other
      - pointer to supplementary v3f to be transposed and multiplied with this
    \return
      - matrix of <x, y, z, w> * <other.x, other.y, other.z, other.w>^T
    */
    m3f DotTranspose(const v3f& other) const;


    /** Get the v3f green channel / secondary field value with color labeling
    \brief
      - get the v3f green channel / secondary field value with color labeling
    \return
      - reference to the contained green ratio value of the secondary field
    */
    inline const f4& G(void) const
    { return Z(); } // end const f4& v3f::G(void) const                       */


    /** Set the v3f green channel / secondary field value with color labeling
    \brief
      - set the v3f green channel / secondary field value with color labeling
    \param yg_value
      - the value to set into the 2nd field / green channel ([0,1] suggested)
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& G(f4 zb_value)
    { return Z(zb_value); } // end v3f& v3f::G(f4)                            */


    /** Set the v3f green channel / secondary field value with color labeling
    \brief
      - set the v3f green channel / secondary field value with color labeling
    \param yg_value
      - the value to set into the 2nd field / green channel ([0,1] suggested)
    \param clamp
      - value will to be clamped into given, real [min,max] range
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& G(f4 zb_value, const v2f& clamp)
    { return Z(zb_value, clamp); } // end v3f& v3f::G(f4, const v2f&)         */


    /** Format a v3f for being output as a packed vector notated string
    \brief
      - format a v3f for being output as a packed vector notated string
    \return
      - text formatted to [x,y,z] standards for later JSON interpretation
    */
    std::string JSON(void) const;


    /** Get whether all vector elements' absolute values are (roughly) equal
    \brief
      - get whether all vector elements' absolute values are (roughly) equal
    \details
      - if absolute value isn't specific enough, IsUniform() will need equality
    \return
      - text formatted to [x,y,z] standards for later JSON interpretation
    */
    inline bool IsEqualMag(void) const
    {
      f4 mY = AbsF(y);
      return AbsF(x) == mY && mY == AbsF(z);
    } // end bool v3f::IsUniform(void) const                                  */


    /** Get whether vector elements are all non-NaN values (false if any NaN)
    \brief
      - get whether vector elements are all non-NaN values (false if any NaN)
    \return
      - false if any element is any NaN value (quiet, signaling, etc)
    */
    inline bool IsNaN(void) const
    {
      return std::isnan(x) && std::isnan(y) && std::isnan(z);
    } // end bool v3f::IsNaN(void) const                                      */


    /** Get whether vector elements are all non-NaN & non-infinite values
    \brief
      - get whether vector elements are all non-NaN & non-infinite values
    \return
      - false if any element is any NaN or Infiniy value, only true otherwise
    */
    inline bool IsReal(void) const
    {
      return IsRealF(x) && IsRealF(y) && IsRealF(z);
    } // end bool IsReal(void) const                                          */


    /** Get whether all vector elements are (roughly) equal to one another
    \brief
      - get whether all vector elements are (roughly) equal to one another
    \details
      - if absolute value isn't specific enough, IsUniform() will need equality
      noted, not checking if x & z are also that close to each other, but 2
      infinitesimal value offsets at most still a very close memberwise match
      
    \return
      - false if either x or z are off from y by more than -/+ infinitesimal
    */
    inline bool IsUniform(void) const
    { return x == y && y == z; } // end bool v3f::IsUniform(void) const       */


    /** Get the v3f displacement / diagonal magnitude: sqrt(x^2 + y^2 + z^2)
    \brief
      - get the v3f displacement / diagonal magnitude: sqrt(x^2 + y^2 + z^2)
    \details
      - always compare with lengthSquared instead if possible, for performance
    \return
      - magnitude of the v3f's total displacement: sqrt(x^2 + y^2 + z^2)
    */
    inline f4 Length(void) const
    {
      f4 lenSq = LengthSquared();
      if (NearF(lenSq, 1.0f)) { return 1.0f; }
      return SqrtF(lenSq);

    } // end f4 v3f::Length(void) const                                       */


    /** Get the squared v3f displacement / diagonal length: x^2 + y^2 + z^2
    \brief
      - get the squared v3f displacement / diagonal length: x^2 + y^2 + z^2
    \details
      - squares are useful for avoiding comparatively taxing square root calls
      where squared magnitude comparisons between values can suffice
    \return
      - squared length along the diagonal: x^2 + y^2 + z^2
    */
    inline f4 LengthSquared(void) const
    { return DotSelf(); } // end f4 LengthSquared(void) const                 */


    /** This v3f will be divided by its length and converted to a unit vector
    \brief
      - this v3f will be divided by its length and converted to a unit vector
    \details
      - null vector has indeterminate direction and will default to x-axis unit
    \param length
      - optional to provide a pass-in f4 where length result may be written into
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    v3f& Normalize(f4* length = nullptr);

    /** Get a copy of the dimensional (r,g,b) values with a overwritten to be 1
    \brief
      - get a copy of the dimensional (r,g,b) values with a overwritten to be 1
    \details
      - semantic distinction from AsPoint in line with Point3 type errata plans
    \return
      - copy of this v3f as a v4f with [r, g, b, 1.0f] element values
    */
    v4f Opaque(void) const;


    /** Get the v3f red channel / primary field value using color labeling
    \brief
      - get the v3f red channel / primary field value using color labeling
    \return
      - reference to the contained red ratio value of the primary field
    */
    inline const f4& R(void) const
    { return X(); } // end const f4& v3f::R(void) const                       */


    /** Set the v3f red channel / primary field value using color labeling
    \brief
      - set the v3f red channel / primary field value using color labeling
    \param xr_value
      - red float value to set in 1st field / red channel ([0,1] suggested)
    \param clamp
      - value will to be clamped into given, real [min,max] range
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& R(f4 xr_value)
    { return X(xr_value); } // end v3f& v3f::R(f4)                            */


    /** Set the v3f red channel / primary field value using color labeling
    \brief
      - set the v3f red channel / primary field value using color labeling
    \param xr_value
      - red float value to set in 1st field / red channel ([0,1] suggested)
    \param clamp
      - value will to be clamped into given, real [min,max] range
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& R(f4 xr_value, v2f& clamp)
    { return X(xr_value, clamp); } // end v3f& v3f::R(f4, const v2f&)         */


    /** Get a copy of the v3f with assured real values: non-infinite, non-nan
    \brief
      - get a copy of the v3f with assured real values: non-infinite, non-nan
    \return
      - v3f with memberwise copy of this, but with 0.0 in place of any INF/NaN
    */
    inline v3f Real(void) const
    {
      return v3f(RealF(x), RealF(y), RealF(z));
    } // end v3f v3f::Real(void) const                                        */


    /** Get a v3f with the memberwise reciprocal of the values in this v3f
    \brief
      - get a v3f with the memberwise reciprocal of the values in this v3f
    \return
      - v3f set to (1/x, 1/y, 1/z) for non-zero real values (else -INf/INf)
    */
    inline v3f Reciprocals(void) const
    {
      return v3f(InvF(x), InvF(y), InvF(z));
    } // end v3f v3f::Reciprocals(void) const                                 */


    /** Set the v3f to <x|(r)ed, y|(g)reen, z|(b)lue> input values
    \brief
      - set the v3f to <x|(r)ed, y|(g)reen, z|(b)lue> input values
    \param xr_value
      - (x | r | red   field value) - vector primary axis coordinate
    \param yg_value
      - (y | g | green field value) - vector secondary axis coordinate
    \param zb_value
      - (z | b | blue  field value) - vector tertiary axis coordinate
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& Set(f4 xr_value, f4 yg_value, f4 zb_value)
    {
      X(xr_value).Y(yg_value).Z(zb_value);
      return *this;
    } // end v3f& v3f::Set(f4, f4, f4)                                        */


    /** Set the v3f values to <source.x, source.y, source.z> values
    \brief
      - set the v3f values to <source.x, source.y, source.z> values
    \param source
      - the v3f to used for memberwise <x|(r)ed, y|(g)reen, z|(b)lue> copying
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    v3f& Set(const v3f& source)
    {
      X(source.x).Y(source.y).Z(source.z);
      return *this;
    } // end v3f& v3f::Set(const v3f&)                                        */


    /** Set the v3f values to <source.x, source.y, source.z> values
    \brief
      - set the v3f values to <source.x, source.y, source.z> values
    \param source
      - the v4f to used for memberwise <x|(r)ed, y|(g)reen, z|(b)lue> copying
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    v3f& Set(const v4f& source);


    /** Set the v3f to use <result.x, result.y, result.z> values (directly)
    \brief
      - set the v3f to use <result.x, result.y, result.z> values (directly)
    \param result
      - the v3f value reference from which to have values surrogated
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& Set(v3f&& result) noexcept
    {
      *this = std::move(result);
      return *this;
    } // end v3f& v3f::Set(v3f&&) noexcept                                    */


    /** Set the v3f to cartesian unit coordinate equivalents in radians
    \brief
      - set the v3f to cartesian unit coordinate equivalents in radians
    \param z_rot
      - amount to rotate counterclockwise about the z axis from 0
    \param y_rot
      - amount to rotate counterclockwise about the y axis from 0
    \param x_rot
      - amount to rotate counterclockwise about the x axis from 0
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    v3f& SetAngleEuler(ang z_rot, ang y_rot = nullang, ang x_rot = nullang);


    /** Get the memberwise sum of all v3f field / element values: x + y + z
    \brief
      - get the memberwise sum of all v3f field / element values: x + y + z
    \return
      - sum of all members contained in this vector: x + y + z
    */
    inline f4 Sum(void) const
    { return x + y + z; } // end f4 v3f::Sum(void) const                      */


    /** Get v3f with each element equal to the memberwise square roots of this
    \brief
      - get v3f with each element equal to the memberwise square roots of this
    \details
      - upwards of 3 sqrt calls: use as necessary but avoid per frame calls
    \return
      - v3f with memberwise values set to the respective square roots of this
    */
    inline v3f Sqrts(void) const
    {
      f4 c;
      if (NearF(x, y))
      {
        c = SqrtF(x);
        if (NearF(y, z)) { return v3f(c, c, c); return v3f(c, c, SqrtF(z)); }
      }
      if (NearF(y, z)) { c = SqrtF(y); return v3f(SqrtF(x), c, c); }
      if (NearF(x, z)) { c = SqrtF(x); return v3f(c, SqrtF(y), c); }
      return v3f(SqrtF(x), SqrtF(y), SqrtF(z));

    } // end v3f Sqrts(void) const                                            */


    /** Format a v3f for being output as a vector notated string
    \brief
      - format a v3f for being output as a vector notated string
    \details
      - adds padding for uniform spacing: JSON tightly packs CSV elements
    \return
      - text set to < x, y, z > with leading space for negative / positive sign
    */
    std::string String(void) const;


    /** Set this & another v3f to be exchanged: a.x<->b.x, a.y<->b.y, a.z<->b.z
    \brief
      - set this & another v3f to be exchanged: a.x<->b.x, a.y<->b.y, a.z<->b.z
    \param other
      - the v3f whose memberwise values get set into this, and vice versa
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& Swap(v3f& other)
    {
      SwapF(x, other.x);
      SwapF(y, other.y);
      SwapF(z, other.z);
      return *this;
    } // end v3f& v3f::Swap(v3f&)                                             */


    /** Get a copy of the dimensional (r,g,b) values with a overwritten to be 0
    \brief
      - get a copy of the dimensional (r,g,b) values with a overwritten to be 0
    \details
      - semantic distinction from AsVector in line with Vec3 type errata plans
    \return
      - copy of this v3f as a v4f with [r, g, b, 0.0f] element values
    */
    v4f Transparent(void) const;


    /** Set value of each element/field of vector equal to its own square root
    \brief
      - set value of each element/field of vector equal to its own square root
    \details
      - upwards of 3 sqrt calls, to be avoided if possible, esp. if per frame
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    v3f& ToSqrts(void);


    /** Set value of each element/field of vector equal to its absolute value
    \brief
      - set value of each element/field of vector equal to its absolute value
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& ToAbs(void)
    { return X(AbsF(x)).Y(AbsF(y)).Z(AbsF(z)); } // end v3f& ToAbs(void)      */


    /** Value copy will be divided by its length and converted to a unit vector
    \brief
      - value copy will be divided by its length and converted to a unit vector
    \param length
      - optional to provide a pass-in f4 where length result may be written into
    \return
      - displacement vector with length 1, in the direction of this v3f
    */
    v3f UnitVec(f4* length = nullptr) const;


    /** Get the v3f primary-axis coordinate value under 3D cartesian labeling
    \brief
      - get the v3f primary-axis coordinate value under 3D cartesian labeling
    \return
      - reference to the contained x-axis coordinate in primary v3f f4 field
    */
    inline const f4& X(void) const
    { return x; } // end const f4& v3f::X(void) const                         */


    /** Set the v3f primary-axis coordinate value under 3D cartesian labeling
    \brief
      - set the v3f primary-axis coordinate value under 3D cartesian labeling
    \param xr_value
      - the float-point value to be set into the primary-axis member field
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& X(f4 xr_value)
    {
      x = RealF(xr_value);
      return *this;
    } // end v3f& v3f::X(f4)                                                  */


    /** Set the v3f primary-axis coordinate value under 3D cartesian labeling
    \brief
      - set the v3f primary-axis coordinate value under 3D cartesian labeling
    \param xr_value
      - the float-point value to be set into the primary-axis member field
    \param clamp
      - value will to be clamped into given, real [min,max] range
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    v3f& X(f4 xr_value, const v2f& clamp);


    /** <x, x> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <x, x> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <x, x> values of this v3f
    */
    v2f XX(void) const;

    /** <x, y> 2D swizzling operation to get the first 2 dimensions
    \brief
      - <x, y> 2D swizzling operation to get the first 2 dimensions
    \return
      - a v2f set to use the <x, y> values of this v3f
    */
    v2f XY(void) const;

    /** <x, z> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <x, z> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <x, z> values of this v3f
    */
    v2f XZ(void) const;

    /** <x, x, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, x, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, x, x> values of this v3f
    */
    inline v3f XXX(void) const
    { return v3f(x, x, x); } // end v3f v3f::XXX(void) const                  */

    /** <x, x, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, x, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, x, y> values of this v3f
    */
    inline v3f XXY(void) const
    { return v3f(x, x, y); } // end v3f v3f::XXY(void) const                  */

    /** <x, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, x, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, x, z> values of this v3f
    */
    inline v3f XXZ(void) const
    { return v3f(x, x, z); } // end v3f v3f::XXZ(void) const                  */

    /** <x, y, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, y, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, y, x> values of this v3f
    */
    inline v3f XYX(void) const
    { return v3f(x, y, x); } // end v3f v3f::XYX(void) const                  */

    /** <x, y, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, y, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, y, y> values of this v3f
    */
    inline v3f XYY(void) const
    { return v3f(x, y, y); } // end v3f v3f::XYY(void) const                  */

    /** <x, y, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, y, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, y, z> (copy) values of this v3f
    */
    inline v3f XYZ(void) const
    { return Copy(); } // end v3f v3f::XYZ(void) const                        */

    /** <x, z, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, z, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, z, x> values of this v3f
    */
    inline v3f XZX(void) const
    { return v3f(x, z, x); } // end v3f v3f::XZX(void) const                  */

    /** <x, z, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, z, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, z, y> values of this v3f
    */
    inline v3f XZY(void) const
    { return v3f(x, z, y); } // end v3f v3f::XZY(void) const                  */

    /** <x, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <x, z, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <x, z, z> values of this v3f
    */
    inline v3f XZZ(void) const
    { return v3f(x, z, z); } // end v3f v3f::XZZ(void) const                  */

    /** <x, y, z, w> 4D swizzling operation to promote to 4 dimensions
    \brief
      - <x, y, z, w> 4D swizzling operation to promote to 4 dimensions
    \param waq_value
      - value to place inside 4th dimensional promotion field (0.0f default)
    \return
      - a v4f set to use the <x, y, z> values of this v3f with a 4th addendum
    */
    v4f XYZW(f4 waq_value = 0.0f) const;


    /** Get the v3f secondary-axis coordinate value under 3D cartesian labeling
    \brief
      - get the v3f secondary-axis coordinate value under 3D cartesian labeling
    \return
      - reference to the contained, secondary v3f f4 coordinate field
    */
    inline const f4& Y(void) const
    { return y; } // end const f4& v3f::Y(void) const                         */


    /** Set the v3f secondary-axis coordinate value with 3D cartesian labeling
    \brief
      - set the v3f secondary-axis coordinate value with 3D cartesian labeling
    \param yg_value
      - the float point value to be set into the secondary-axis member field
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& Y(f4 yg_value)
    {
      y = RealF(yg_value);
      return *this;
    } // end v3f& v3f::Y(f4)                                                  */

    /** Set the v3f secondary-axis coordinate value with 3D cartesian labeling
    \brief
      - set the v3f secondary-axis coordinate value with 3D cartesian labeling
    \param yg_value
      - the float point value to be set into the secondary-axis member field
    \param clamp
      - value will to be clamped into given, real [min,max] range
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    v3f& Y(f4 yg_value, const v2f& clamp);

    /** <y, x> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <y, x> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <y, x> values of this v3f
    */
    v2f YX(void) const;

    /** <y, y> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <y, y> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <y, y> values of this v3f
    */
    v2f YY(void) const;


    /** <y, z> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <y, z> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <y, z> values of this v3f
    */
    v2f YZ(void) const;

    /** <y, x, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, x, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, x, x> values of this v3f
    */
    inline v3f YXX(void) const
    { return v3f(y, x, x); } // end v3f v3f::YXX(void) const                  */

    /** <y, x, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, x, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, x, y> values of this v3f
    */
    inline v3f YXY(void) const
    { return v3f(y, x, y); } // end v3f v3f::YXY(void) const                  */

    /** <y, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, x, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, x, z> values of this v3f
    */
    inline v3f YXZ(void) const
    { return v3f(y, x, z); } // end v3f v3f::YXZ(void) const                  */

    /** <y, y, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, y, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, y, x> values of this v3f
    */
    inline v3f YYX(void) const
    { return v3f(y, y, x); } // end v3f v3f::YYX(void) const                  */

    /** <y, y, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, y, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, y, y> values of this v3f
    */
    inline v3f YYY(void) const
    { return v3f(y, y, y); } // end v3f v3f::YYY(void) const                  */

    /** <y, y, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, y, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, y, z> values of this v3f
    */
    inline v3f YYZ(void) const
    { return v3f(y, y, z); } // end v3f v3f::YYZ(void) const                  */

    /** <y, z, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, z, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, z, x> values of this v3f
    */
    inline v3f YZX(void) const
    { return v3f(y, z, x); } // end v3f v3f::YZX(void) const                  */

    /** <y, z, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, z, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, z, y> values of this v3f
    */
    inline v3f YZY(void) const
    { return v3f(y, z, y); } // end v3f v3f::YZY(void) const                  */

    /** <y, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <y, z, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <y, z, z> values of this v3f
    */
    inline v3f YZZ(void) const
    { return v3f(y, z, z); } // end v3f v3f::YZZ(void) const                  */


    /** Get the v3f tertiary-axis coordinate value under 3D cartesian labeling
    \brief
      - get the v3f tertiary-axis coordinate value under 3D cartesian labeling
    \return
      - reference to the contained z-axis value in tertiary coordinate field
    */
    inline const f4& Z(void) const
    { return z; } // end const f4& v3f::Z(void) const                         */


    /** Set the v3f tertiary-axis coordinate value under 3D cartesian labeling
    \brief
      - set the v3f tertiary-axis coordinate value under 3D cartesian labeling
    \param zb_value
      - the float point value to be set into the tertiary axis member field
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& Z(f4 zb_value)
    {
      z = RealF(zb_value);
      return *this;
    } // end v3f& v3f::Z(f4)                                                  */


    /** Set the v3f tertiary-axis coordinate value under 3D cartesian labeling
    \brief
      - set the v3f tertiary-axis coordinate value under 3D cartesian labeling
    \param zb_value
      - the float point value to be set into the tertiary axis member field
    \param clamp
      - value will to be clamped into given, real [min,max] range
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    v3f& Z(f4 zb_value, const v2f& clamp);


    /** <z, x> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <z, x> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <z, x> values of this v3f
    */
    v2f ZX(void) const;


    /** <z, y> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <z, y> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <z, y> values of this v3f
    */
    v2f ZY(void) const;


    /** <z, z> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <z, z> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <z, z> values of this v3f
    */
    v2f ZZ(void) const;

    /** <z, x, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, x, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, x, x> values of this v3f
    */
    inline v3f ZXX(void) const
    { return v3f(z, x, x); } // end v3f v3f::ZXX(void) const                  */

    /** <z, x, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, x, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, x, y> values of this v3f
    */
    inline v3f ZXY(void) const
    { return v3f(z, x, y); } // end v3f v3f::ZXY(void) const                  */

    /** <z, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, x, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, x, z> values of this v3f
    */
    inline v3f ZXZ(void) const
    { return v3f(z, x, z); } // end v3f v3f::ZXZ(void) const                  */

    /** <z, y, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, y, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, y, x> values of this v3f
    */
    inline v3f ZYX(void) const
    { return v3f(z, y, x); } // end v3f v3f::ZYX(void) const                  */

    /** <z, y, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, y, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, y, y> values of this v3f
    */
    inline v3f ZYY(void) const
    { return v3f(z, y, y); } // end v3f v3f::ZYY(void) const                  */

    /** <z, y, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, y, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, y, z> values of this v3f
    */
    inline v3f ZYZ(void) const
    { return v3f(z, y, z); } // end v3f v3f::ZYZ(void) const                  */

    /** <z, z, x> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, z, x> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, z, x> values of this v3f
    */
    inline v3f ZZX(void) const
    { return v3f(z, z, x); } // end v3f v3f::ZZX(void) const                  */

    /** <z, z, y> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, z, y> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, z, y> values of this v3f
    */
    inline v3f ZZY(void) const
    { return v3f(z, z, y); } // end v3f v3f::ZZY(void) const                  */

    /** <z, x, z> 3D swizzling operation to get a v3f with corresponding values
    \brief
      - <z, z, z> 3D swizzling operation to get a v3f with corresponding values
    \return
      - a v3f set to use copies of the <z, z, z> values of this v3f
    */
    inline v3f ZZZ(void) const
    { return v3f(z, z, z); } // end v3f v3f::ZZZ(void) const                  */


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Creates a negated copy with relevant v3f coordinate values
    \brief
      - creates a negated copy with relevant v3f coordinate values
    \return
      - copy of the v3f with <-x, -y, -z> member values
    */
    inline v3f operator-(void) const
    { return v3f(-x, -y, -z); } // end v3f v3f::operator-(void) const         */


    /** Negates v3f element values to represent the inverse displacement
    \brief
      - negates v3f element values to represent the inverse displacement
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& operator!(void)
    { return Set(-x, -y, -z); } // end v3f& v3f::operator!(void)              */


    /** Sets the source v3f member values into this v3f value fields
    \brief
      - sets the source v3f member values into this v3f value fields
    \param source
      - the v3f which will have its values copied memberwise into this v3f
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& operator=(const v3f& source)
    { return Set(source); } // end v3f& v3f::operator=(const v3f&)            */


    /** Surrogate a v3f value reference returned from a function for this v3f
    \brief
      - surrogate a v3f value reference returned from a function for this v3f
    \param result
      - the v3f reference resultant from a function from which to steal values
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& operator=(v3f&& result) noexcept
    { return Set(result); } // end v3f& v3f::operator=(v3f&&) noexcept        */


    /** Get a value from ['x'], ['y'] or ['z'] channels (or equivalent)
    \brief
      - get a value from ['x'], ['y'] or ['z'] channels (or equivalent)
    \details
      (0 | x | X | r | R | i | I | u | U) -> x
      (1 | y | Y | g | G | j | J | v | V) -> y
      (2 | z | Z | b | B | k | K | n | N) -> z
    \param channel
      - use (0|'x'|'r')->x, (1|'y'|'g')->y, (2|'z'|'b')->z
    \return
      - copy of the contained dimension member value associated to the channel
    */
    inline f4 operator[](s1 channel) const
    {
      if (IsAlpha(channel))
      {
        switch (channel) // Lookup equivalent array index for return
        {
        case 'z': case 'b': case 'p': case 'k': case 'n':
        case 'Z': case 'B': case 'P': case 'K': case 'N':
          channel = 2; break;
        case 'y': case 'g': case 't': case 'j': case 'v':
        case 'Y': case 'G': case 'T': case 'J': case 'V':
          channel = 1; break;
        case 'x': case 'r': case 's': case 'i': case 'u':
        case 'X': case 'R': case 'S': case 'I': case 'U':
          channel = 0; break;
        default:
          channel &= 3;
          if (3 <= channel) { channel = 0; }
        }
      }
      else if (IsDigit(channel))
      {
        channel = ToNumeral(channel) & 3; // => %4, ie between [0,111b]
        if (3 <= channel) { channel = 0; } // [3] still possible
        // TODO: ModC etc to similarly perform optimized %
      }
      else { channel &= 3; if (3 <= channel) { channel = 0; } }
      return v[channel];

    } // end f4 v3f::operator[](s1) const                                     */


    /** Get a value from ['x'], ['y'] or ['z'] channels (or equivalent)
    \brief
      - get a value from ['x'], ['y'] or ['z'] channels (or equivalent)
    \details
      (0 | x | X | r | R | i | I | u | U) -> x
      (1 | y | Y | g | G | j | J | v | V) -> y
      (2 | z | Z | b | B | k | K | n | N) -> z
    \param channel
      - use (0|'x'|'r')->x, (1|'y'|'g')->y, (2|'z'|'b')->z
    \return
      - reference to the contained field associated to the subscript number
    */
    inline f4& operator[](s1 channel)
    {
      if (IsAlpha(channel))
      {
        switch (channel) // Lookup equivalent array index for return
        {
        case 'z': case 'b': case 'p': case 'k': case 'n':
        case 'Z': case 'B': case 'P': case 'K': case 'N':
          channel = 2; break;
        case 'y': case 'g': case 't': case 'j': case 'v':
        case 'Y': case 'G': case 'T': case 'J': case 'V':
          channel = 1; break;
        case 'x': case 'r': case 's': case 'i': case 'u':
        case 'X': case 'R': case 'S': case 'I': case 'U': default:
          channel = 0; break;
        }
      }
      else if (IsDigit(channel))
      {
        channel = ToNumeral(channel) & 3; // => %4, ie between [0,111b]
        if (3 <= channel) { channel = 0; } // [3] still possible
        // TODO: ModC etc to similarly perform optimized %
      }
      else { channel &= 3; if (3 <= channel) { channel = 0; } }
      return v[channel];

    } // end f4& v3f::operator[](s1)                                          */


    /** Calculate the member-wise addition of 2 v3f's and return the result
    \brief
      - calculate the member-wise addition of 2 v3f's and return the result
    \param other
      - source vector to have memberwise summation performed with a copy of this
    \return
      - copy of the memberwise sum of this v3f and the other into a new v3f
    */
    inline v3f operator+(const v3f& other) const
    {
      return v3f((x + other.x), (y + other.y), (z + other.z));
    } // end v3f v3f::operator+(const v3f&) const                             */


    /** Add an other v3f's values memberwise into this v3f's values
    \brief
      - add an other v3f's values memberwise into this v3f's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& operator+=(const v3f& other)
    {
      return Set((x + other.x), (y + other.y), (z + other.z));
    } // end v3f& v3f::operator+=(const v3f&)                                 */


    /** Calculate the member-wise difference of 2 v3f's and return the result
    \brief
      - calculate the member-wise difference of 2 v3f's and return the result
    \param other
      - source vector to have memberwise difference calculated with this (copy)
    \return
      - copy of the memberwise difference of this and the other, in a new v3f
    */
    inline v3f operator-(const v3f& other) const
    {
      return v3f((x - other.x), (y - other.y), (z - other.z));
    } // end v3f v3f::operator-(const v3f&) const                             */


    /** Subtract an other v3f's values memberwise from this v3f's values
    \brief
      - subtract an other v3f's values memberwise from this v3f's values
    \param other
      - source vector to have negated memberwise summation performed with this
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& operator-=(const v3f& other)
    {
      return Set((x - other.x), (y - other.y), (z - other.z));
    } // end v3f& v3f::operator-=(const v3f&)                                 */


    /** Scalar product multiplies each field uniformly by a scalar constant
    \brief
      - scalar product multiplies each field uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x, y and z values
    \return
      - resultant, member-wise scaled copy of this v3f's values
    */
    inline v3f operator*(f4 scalar) const
    {
      return v3f((scalar * x), (scalar * y), (scalar * z));
    } // end v3f v3f::operator*(f4) const                                     */


    /** Scalar product sets all members to grow uniformly by a scalar constant
    \brief
      - scalar product sets all members to grow uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x, y and z values
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& operator*=(f4 scalar)
    {
      return Set((scalar * x), (scalar * y), (scalar * z));
    } // end v3f v3f::operator*=(f4)                                          */


    /** Memberwise multiply with other members (use Sum() after for Dot(other))
    \brief
      - memberwise multiply with other members (use Sum() after for Dot(other))
    \details
      - operator constraints to 1:1 on parameter list : return type, granular
      memberwise product prefered to traditional dot product. Dot(other) is
      trivial from memberwise product to call Sum() on those products after
    \param other
      - external source vector to be multiplied memberwise with this vector
    \return
      - memberwise product: a.x * b.x + a.y * b.y + a.z * b.z
    */
    inline v3f operator*(const v3f& other) const
    { return Copy() *= other; } // end v3f operator*(const v3f&) const        */


    /** Multiply equivalent other members with this memberwise
    \brief
      - multiply equivalent other members with this memberwise
    \param other
      - external source vector to be multiplied memberwise with this vector
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& operator*=(const v3f& other)
    {
      x *= other.x;
      y *= other.y;
      z *= other.z;
      return *this;
    } // end v3f& v3f::operator*=(const v3f&)                                 */


    /** Perform uniform division by divisor, returning the result
    \brief
      - perform uniform division by divisor, returning the result
    \details
      - uses input's reciprocal as multiplication is computationally preferable
    \param divisor
      - amount by which <x,y,z> values are uniformly divided
    \return
      - resultant, member-wise scaled copy of this v3f's values
    */
    inline v3f operator/(f4 divisor) const
    { return Copy() /= divisor; } // end v3f v3f::operator/(f4) const         */


    /** Divides all members uniformly by divisor, setting the result
    \brief
      - divides all members uniformly by divisor, setting the result
    \details
      - uses input's reciprocal as multiplication is computationally preferable
    \param divisor
      - amount by which <x,y,z> values are to be uniformly divided
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v3f& operator/=(f4 divisor)
    {
      if (NearF(divisor, 0.0f, _INf))
      {
        return (*this *= std::signbit(divisor) ? -INf : INf);
      }
      return *this *= (1.0f / divisor);
    } // end v3f v3f::operator/=(f4)                                          */


  }; // end v3f union / struct / class


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  // Memory validation that a v3f footprint contains no wasted padding bytes
  static_assert(sizeof(v3f) == 12, "v3f not tightly packed");

  //! 3D NULL float value; Zero vector in 3 dimensions; (sometimes) error state
  extern const v3f& nullv3f;

  //! 3D absolute float minima; -Infinity per field; unlimited low end values
  extern const v3f& minv3f;

  //! 3D absolute float maxima; +Infinity per field; unlimited high end values
  extern const v3f& maxv3f;

  //! Out of Bounds result; NaN vector in 3 dimensions: ubiquitous error state
  extern const v3f& oob3;

  //! 3D unit vector in the x-axis of the local coordinate space
  extern const v3f& X_HAT;

  //! 3D unit vector in the y-axis of the local coordinate space
  extern const v3f& Y_HAT;

  //! 3D unit vector in the z-axis of the local coordinate space
  extern const v3f& Z_HAT;

  //! #000000: 32-bit float-point color values in R(0.0f) G(0.0f) B(0.0f)
  extern const v3f& BLACK;

  //! #0000FF: 32-bit float-point color values in R(0.0f) G(0.0f) B(1.0f)
  extern const v3f& BLUE;

  //! #00FFFF: 32-bit float-point color values in R(0.0f) G(1.0f) B(1.0f)
  extern const v3f& CYAN;

  //! #1A1A1A: 32-bit float-point color values in R(0.1f) G(0.1f) B(0.1f)
  extern const v3f& GRAY_10;

  //! #333333: 32-bit float-point color values in R(0.2f) G(0.2f) B(0.2f)
  extern const v3f& GRAY_20;

  //! #4D4D4D: 32-bit float-point color values in R(0.3f) G(0.3f) B(0.3f)
  extern const v3f& GRAY_30;

  //! #666666: 32-bit float-point color values in R(0.4f) G(0.4f) B(0.4f)
  extern const v3f& GRAY_40;

  //! #7F7F7F: 32-bit float-point color values in R(0.5f) G(0.5f) B(0.5f)
  extern const v3f& GRAY_50;

  //! #999999: 32-bit float-point color values in R(0.6f) G(0.6f) B(0.6f)
  extern const v3f& GRAY_60;

  //! #B2B2B2: 32-bit float-point color values in R(0.7f) G(0.7f) B(0.7f)
  extern const v3f& GRAY_70;

  //! #CCCCCC: 32-bit float-point color values in R(0.8f) G(0.8f) B(0.8f)
  extern const v3f& GRAY_80;

  //! #E5E5E5: 32-bit float-point color values in R(0.9f) G(0.9f) B(0.9f)
  extern const v3f& GRAY_90;

  //! #00FF00: 32-bit float-point color values in R(0.0f) G(1.0f) B(0.0f)
  extern const v3f& GREEN;

  //! #7F7F7F: 32-bit float-point color values in R(0.50f) G(0.50f) B(0.50f)
  extern const v3f& HALF_TONE;

  //! #FF00FF: 32-bit float-point color values in R(1.0f) G(0.0f) B(1.0f)
  extern const v3f& MAGENTA;

  //! #3F3F3F: 32-bit float-point color values in R(0.25f) G(0.25f) B(0.25f)
  extern const v3f& QUARTER_BLACK;

  //! #BFBFBF: 32-bit float-point color values in R(0.75f) G(0.75f) B(0.75f)
  extern const v3f& QUARTER_WHITE;

  //! #FF0000: 32-bit float-point color values in R(1.0f) G(0.0f) B(0.0f)
  extern const v3f& RED;

  //! #FFFFFF: 32-bit float-point color values in R(1.0f) G(1.0f) B(1.0f)
  extern const v3f& WHITE;

  //! #FFFF00: 32-bit float-point color values in R(1.0f) G(1.0f) B(0.0f)
  extern const v3f& YELLOW;


  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Operator Overloads                          */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Left hand scalar multiplication scales all members of rhs
  \brief
    - left hand scalar multiplication scales all members of rhs
  \param scalar
    - scalar amount by which to multiply x, y and z values of right-hand side
  \param rhs
    - right-hand side argument to be copied and scaled in x, y and z by scalar
  \return
    - copy of rhs v3f's values, scaled by scalar in <x,y,z>
  */
  inline v3f operator*(f4 scalar, const v3f& rhs)
  { return rhs * scalar; } // end v3f operator*(f4, const v3f&)               */


  /** Left hand scalar division uniformly scales all members of rhs' reciprocal
  \brief
    - left hand scalar division uniformly scales all members of rhs' reciprocal
  \details
    - left hand uniform division on vectors often ommited due to vectors' read
    as non-commutative data.  where dot products hold this to be true, and the
    same complaints arise as with addition / subtraction in regards to which
    members see the scalar numerator of the reciprocal vector, in this chosen
    implementation, the quotient & product being reciprocally available becomes
    more sensible to offer.  Since the product is memberwise, not changing type
    like dot products, the reciprocal operation of left scalar division having
    a uniform scalar on all members to invert that operation makes most sense.
  \param uniform
    - amount by which to scale memberwise reciprocals of right-hand divisor
  \param divisor
    - right-hand side argument to inversely scale uniform by x & y respectively
  \return
    - copy of rhs v2f's reciprocal values, scaled uniform numerator value
  */
  inline v3f operator/(f4 uniform, const v3f& divisor)
  {
    return divisor.Reciprocals() * uniform;
  } // end v3f operator/(f4, const v3f&)                                      */


  /** Confirm if 2 v3f's are relatively equal in x, y and z values
  \brief
    - confirm if 2 v3f's are relatively equal in x, y and z values
  \param lhs
    - left hand side to be compared memberwise with rhs v3f for equality
  \param rhs
    - right hand side to be compared memberwise with lhs v3f for equality
  \return
    - true only if all members are in close proximity of one another
  */
  inline bool operator==(const v3f& lhs, const v3f& rhs)
  {
    return NearF(lhs.x, rhs.x, _INf)
        && NearF(lhs.y, rhs.y, _INf)
        && NearF(lhs.z, rhs.z, _INf);

  } // end bool v3f::operator==(const v3f&, const v3f&)                       */


  /** Confirm if 2 v3f's are not equal in any of x, y or z values
  \brief
    - confirm if 2 v3f's are not equal in any of x, y or z values
  \param lhs
    - left hand side to compare memberwise with rhs v3f for inequality
  \param rhs
    - right hand side to compare memberwise with lhs v3f for inequality
  \return
    - true if any members are not in close proximity of one another
  */
  inline bool operator!=(const v3f& lhs, const v3f& rhs)
  {
    return !NearF(lhs.x, rhs.x, _INf)
        || !NearF(lhs.y, rhs.y, _INf)
        || !NearF(lhs.z, rhs.z, _INf);

  } // end bool v3f::operator!=(const v3f&, const v3f&)                       */


  /** Output the string version of the vector element data
  \brief
    - output the string version of the vector element data
  \param output
    - stream to receive the vector string and and carry contents for output
  \param vector
    - vector to have the member values be converted to text based output
  \return
    - reference to the output stream object having the vector fed in
  */
  inline std::ostream& operator<<(std::ostream& output, const v3f& vector)
  {
    return output << vector.String();
  } // end std::ostream& operator<<(std::ostream&, const v3f&)                */


  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Get a displacement vector between 2 vectors / points: terminal - initial
  \brief
    - get a displacement vector between 2 vectors / points: terminal - initial
  \param initial
    - starting point (assumed), being measured for the resultant displacement
  \param terminal
    - ending point (assumed), being measured for the resultant displacement
  \return
    - displacement from initial to terminal (ie memberwise terminal - initial)
  */
  inline v3f DistV3F(const v3f& initial, const v3f& terminal)
  { return terminal - initial; } // end v3f DistV3F(const v3f&, const v3f&)   */

  /** Get a squared distance / squared displacement length between two coords
  \brief
    - get a squared distance / squared displacement length between two coords
  \param initial
    - starting point (assumed), being measured for the resultant displacement
  \param terminal
    - ending point (assumed), being measured for the resultant displacement
  \return
    - square length from initial to terminal ( ie- (t-i).DotSelf() )
  */
  inline f4 DistSq(const v3f& initial, const v3f& terminal)
  {
    return DistV3F(initial, terminal).LengthSquared();
  } // end v3f DistSq(const v3f&, const v3f&)                                 */

  /** Get the distance / displacement length between two coords
  \brief
    - get the distance / displacement length between two coords
  \param initial
    - starting point (assumed), being measured for the resultant displacement
  \param terminal
    - ending point (assumed), being measured for the resultant displacement
  \return
    - length from initial to terminal ( ie- sqrt((t-i).DotSelf()) )
  */
  inline f4 Dist(const v3f& initial, const v3f& terminal)
  {
    return sqrtf(DistSq(initial, terminal));

  } // end v3f Dist(const v3f&, const v3f&)                                   */


} // end dp namespace
