/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v2f.h
Purpose:  API agnostic, multipurpose, 2 dimensional float-point element vector
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
#include "Value.h"      // Value manipulation; float point approximation
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  class m2f;  // Functions generating matrix data from vectors
  class m3f;  // Functions generating cross matrix data (uniquely 3D)
  union v3f;  // Promotion to higher dimensions of real numbers
  union v4f;  // Promotion to (still) higher dimensions of real numbers

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  union v2f //! 2D vector container of 2, 4-byte float point elements
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Value range safeguard: & by 1b equivalent and faster than % by 10b
    static const u1 VSUBS_MASK = 0x1;

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    struct {
      f4 x;   //! Primary cartesian axis scale  (right vector; horizontal axis)
      f4 y;   //! Secondary cartesian axis scale     (up vector; vertical axis)
    };
    struct {
      f4 u;   //! Primary texture space axis scale  (ie horizontal [0,1] space)
      f4 v;   //! Secondary texture space axis scale  (ie vertical [0,1] space)
    };
    struct {
      f4 min;   //! Value range minimum scale
      f4 max;   //! Value range maximum scale
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Creates a 2 dimensional vector / point (default <0,0>)
    \brief
      - creates a 2 dimensional vector / point (default <0,0>)
    \param xu_value
      - v2f [0] element (u | min| x) axis field value - primary coordinate
    \param yv_value
      - v2f [1] element (v | max| y) axis field value - secondary coordinate
    */
    v2f(f4 xu_value = 0.0f, f4 yv_value = 0.0f);


    /** Creates a copy of a v2f from another source v2f
    \brief
      - creates a copy of a v2f from another source v2f
    \param angle
      - (a)ngle expression of <r*cos(a), r*sin(a)*i> cartesian vector direction
    \param magnitude
      - diagonal length ((r)adius) of displacement the vector should express
    \param source
      - the v2f to used for memberwise <x|r, y|g, z|b> element value copying
    */
    v2f(ang angle, f4 magnitude = 1.0f);


    /** Creates a copy of a v2f from another source v2f
    \brief
      - creates a copy of a v2f from another source v2f
    \param source
      - the v2f to used for memberwise <x|r, y|g, z|b> element value copying
    */
    v2f(const v2f& source);


    /** Creates a v2f from a v2f value reference resultant from a function
    \brief
      - creates a v2f from a v2f value reference resultant from a function
    \param result
      - the v2f value reference to surrogate into this
    */
    v2f(v2f&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get a copy of the dimensional (x,y) values with w overwritten to be 1
    \brief
      - get a copy of the dimensional (x,y) values with w overwritten to be 1
    \param z_index
      - point in z-axis (y-up depth axis), in which the coordinate is placed
    \return
      - copy of this v2f as a v4f with [x, y, z_index, 1.0f] element values
    */
    v4f AsPoint(f4 z_index = 0.0f) const;


    /** Get a copy of the dimensional <x,y> values with w overwritten to be 0
    \brief
      - get a copy of the dimensional <x,y> values with w overwritten to be 0
    \param z_index
      - point in z-axis (y-up depth axis), in which the coordinate is placed
    \return
      - copy of this v2f as a v4f with [x, y, z_index, 0.0f] element values
    */
    v4f AsVector(f4 z_index = 0.0f) const;


    /** Get angle (arctangent), of current cartesian coordinates in vector
    \brief
      - get angle (arctangent), of current cartesian coordinates in vector
    \return
      - angle (arctangent), of the current cartesian coordinates
    */
    ang Angle(void) const;


    /** Calculate the orthogonal cross product of (this x other) v3f
    \brief
      - calculate the orthogonal cross product of (this &#x2a2f; other) v3f
    \details
      - a v2f cross would always be z-up, but it's not a given for a v3f
    \param other
      - v3f to be taken as the cross product's right hand: this &#x2a2f; other
    \return
      - right-handed 3D vector, orthogonal to this and other
    */
    v3f Cross(const v3f& other) const;


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
      - v4f to be dotted (memberwise multiplied and summed) with this
    \return
      - memberwise sum of products: a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w
    */
    f4 Dot(const v2f& other) const;


    /** Compute the matrix for the v2f, dotted with another v2f's transpose
    \brief
      - compute the matrix for the v2f, dotted with another v2f's transpose
    \param other
      - pointer to supplementary v2f to be transposed and multiplied with this
    \return
      - matrix of <x, y, z, w> * <other.x, other.y, other.z, other.w>^T
    */
    m2f DotTranspose(const v2f& other) const;


    /** Get the dot product of this with itself: sum of memberwise square terms
    \brief
      - compute the matrix for the v2f, dotted with another v2f's transpose
    \return
      - matrix of <x, y, z, w> * <other.x, other.y, other.z, other.w>^T
    */
    inline f4 DotSelf(void) const
    { return (*this * *this).Sum(); } // end f4 DotSelf(void) const           */


    /** Get whether the ranges of an other & this v2f have coincident values
    \brief
      - get whether the ranges of an other & this v2f have coincident values
    \details
      - assuming values are of a colinear range; not a 2D coordinate or vector
      mostly redundant to Intersect(v): use based on need of specifics, not both
    \param other
      - other v2f to have its range evaluated against this v2f's range
    \return
      - true only if other's ordered members intersect with this' ordered values
    */
    inline bool IsIn(const v2f& other) const
    {
      if (!IsNaN() || !other.IsNaN()) { return false; } // Not even colinear
      bool a(min < max), b(other.min < other.max); // check for vector reversal
      // TODO: Range class for ordered pair, wouldn't need this much validation

      const f4& aBegin(a? min : max), aEnd(a? max : min),
        bBegin(b? other.min: other.max), bEnd(b? other.max: other.min);
      return bBegin < aEnd || aBegin < bEnd; // b overflows or underflows in a
    } // end f4 v2f::IsIn(void) const                                         */


    /** Get whether either of this v2f's members are set to any NaN values
    \brief
      - get whether either of this v2f's members are set to any NaN values
    \return
      - true if either member is set to NaN, false as long as neither is
    */
    inline bool IsNaN(void) const
    {
      return std::isnan(min) && std::isnan(max);
    } // end bool v2f::IsNaN(void) const                                      */


    /** Get whether either of this v2f's members are set to any NaN values
    \brief
      - get whether either of this v2f's members are set to any NaN values
    \return
      - true if either member is set to NaN, false as long as neither is
    */
    inline bool IsReal(void) const
    {
      return IsRealF(min) && IsRealF(max);
    } // end bool v2f::IsReal(void) const                                     */


    /** Format a v2f for being output as a packed vector notated string
    \brief
      - format a v2f for being output as a packed vector notated string
    \return
      - text formatted to [x,y] standards for later JSON interpretation
    */
    std::string JSON(void) const;


    /** Get the intersection of 2 value ranges
    \brief
      - get the v2f displacement / diagonal magnitude: sqrt(x^2 + y^2)
    \details
      - always compare with lengthSquared instead if possible, for performance
    \return
      - magnitude of the v2f's total displacement: sqrt(x^2 + y^2)
    */
    v2f Intersect(const v2f& other) const;


    /** Get the v2f displacement / diagonal magnitude: sqrt(x^2 + y^2)
    \brief
      - get the v2f displacement / diagonal magnitude: sqrt(x^2 + y^2)
    \details
      - always compare with lengthSquared instead if possible, for performance
    \return
      - magnitude of the v2f's total displacement: sqrt(x^2 + y^2)
    */
    f4 Length(void) const;


    /** Get the squared v2f displacement / diagonal length: x^2 + y^2
    \brief
      - get the squared v2f displacement / diagonal length: x^2 + y^2
    \details
      - squares are useful for avoiding comparatively taxing square root calls
      where squared magnitude comparisons between values can suffice
    \return
      - squared length along the diagonal: x^2 + y^2 + z^2
    */
    f4 LengthSquared(void) const;


    /** This v2f will be divided by its length and converted to a unit vector
    \brief
      - this v2f will be divided by its length and converted to a unit vector
    \details
      - null vector has indeterminate direction and will default to x-axis unit
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& Normalize(void);


    /** Get a the ordered contents of this v2f: min < max (or 0 range if NaN)
    \brief
      - get a the ordered contents of this v2f: min < max (or 0 range if NaN)
    \return
      - ascending, ordered copy of this, but with 0.0 in place of any NaN value
    */
    inline v2f Range(void) const
    {
      if (std::isnan(min)) { return ((std::isnan(max)? 0 : v2f(max, max))); }
      if (std::isnan(max)) { return v2f(min, min); }
      return (min < max? v2f(min, max) : v2f(max, min));
    } // end v2f v2f::Range(void) const                                       */


    /** Get a copy of the v2f with assured real values: non-infinite, non-nan
    \brief
      - get a copy of the v2f with assured real values: non-infinite, non-nan
    \return
      - v2f with memberwise copy of this, but with 0.0 in place of any INF/NaN
    */
    inline v2f Real(void) const
    {
      return v2f(RealF(x), RealF(y));
    } // end v2f v2f::Real(void) const                                        */


    /** Get a v2f with the memberwise reciprocals of the values in this v2f
    \brief
      - get a v2f with the memberwise reciprocals of the values in this v2f
    \details
      - if 1/0 entailed in any field, goes to (+/-)INf => non-real results
      which could then be made to get Real results =: 0 in an infinite cycle
    \return
      - v3f set to (1/x, 1/y, 1/z) for non-zero real values (else -INf/INf)
    */
    inline v2f Reciprocals(void) const
    { return v2f(InvF(x), InvF(y)); } // end v2f v2f::Reciprocals(void) const */


    /** Set the v2f to <x|u, y|v> input values
    \brief
      - set the v2f to <x|u, y|v> input values
    \param xu_value
      - (x | u   field value) - vector primary axis coordinate
    \param yv_value
      - (y | v field value) - vector secondary axis coordinate
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& Set(f4 xr_value, f4 yg_value)
    { return X(xr_value).Y(yg_value); } // end v2f& v2f::Set(const v4f&)      */


    /** Set the v2f to equivalent <x|u, y|v> cartesian values from polar inputs
    \brief
      - set the v2f to equivalent <x|u, y|v> cartesian values from polar inputs
    \param angle
      - (a)ngle expression of <r*cos(a), r*sin(a)*i> cartesian vector direction
    \param magnitude
      - diagonal length ((r)adius) of displacement the vector should express
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    inline v2f& Set(ang angle, f4 magnitude = 1.0f)
    {
      return Set(magnitude*CosF(angle), magnitude*SinF(angle));
    } // end v2f& Set(ang, f4)                                                */


    /** Set the v2f values to <source.x, source.y> values
    \brief
      - set the v2f values to <source.x, source.y> values
    \param source
      - the v2f to used for memberwise <x|u, y|v> copying
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    inline v2f& Set(const v2f& source)
    { return X(source.x).Y(source.y); } // end v2f& v2f::Set(const v2f&)      */


    /** Set the v2f values to <source.x, source.y> values, discarding source.z
    \brief
      - set the v2f values to <source.x, source.y> values, discarding source.z
    \param source
      - the v3f to used for memberwise <x|u, y|v> value copying
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& Set(const v3f& source);


    /** Set the v2f values to <source.x, source.y> values, discarding z & w
    \brief
      - set the v2f values to <source.x, source.y> values, discarding z & w
    \param source
      - the v4f to used for memberwise <x|(r)ed, y|(g)reen> copying
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& Set(const v4f& source);


    /** Set the v2f to use <result.x, result.y> values (directly)
    \brief
      - set the v2f to use <result.x, result.y> values (directly)
    \param result
      - the v2f value reference from which to have values surrogated
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& Set(v2f&& result) noexcept
    {
      *this = std::move(result);
      return *this;

    } // end v2f& v2f::Set(v2f&&) noexcept                                    */


    /** Get the sum of all member element values from the vector's components
    \brief
      - get the sum of all member element values from the vector's components
    \return
      - memberwise sum of all element / field values from the vector: x + y
    */
    inline f4 Sum(void) const
    { return x + y; } // end f4 Sum(void) const                               */


    /** Format a v2f for being output as a vector notated string
    \brief
      - format a v2f for being output as a vector notated string
    \details
      - adds padding for uniform spacing: JSON tightly packs CSV elements
    \return
      - text set to < x, y > with leading space for negative / positive sign
    */
    std::string String(void) const;


    /** Get the v2f primary-axis coordinate value under 2D texture labeling
    \brief
      - get the v2f primary-axis coordinate value under 2D texture labeling
    \return
      - reference to the contained x-axis coordinate in primary v2f f4 field
    */
    inline const f4& U(void) const
    { return X(); } // end const f4& U(void) const                            */


    /** Set the v2f primary-axis coordinate value under 2D texture labeling
    \brief
      - set the v2f primary-axis coordinate value under 2D texture labeling
    \param xu_value
      - the float-point value to be set into the primary-axis member field
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    inline v2f& U(f4 xu_value)
    { return X(xu_value); } // end v2f& v2f::U(f4)                            */


    /** Value copy will be divided by its length and converted to a unit vector
    \brief
      - value copy will be divided by its length and converted to a unit vector
    \return
      - displacement vector with length 1, in the direction of this v2f
    */
    v2f UnitVec(void) const;


    /** Get the v2f secondary axis coordinate value under 2D texture labeling
    \brief
      - get the v2f secondary axis coordinate value under 2D texture labeling
    \return
      - reference to the contained, secondary v2f f4 coordinate field
    */
    inline const f4& V(void) const
    { return Y(); } // end const f4& V(void) const                            */


    /** Set the v2f secondary axis coordinate value with 3D texture labeling
    \brief
      - set the v2f secondary axis coordinate value with 3D texture labeling
    \param yv_value
      - the float point value to be set into the secondary-axis member field
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    inline v2f& V(f4 yv_value)
    { return Y(yv_value); } // end v2f& v2f::V(f4)                            */


    /** Get the v2f primary-axis coordinate value under 2D cartesian labeling
    \brief
      - get the v2f primary-axis coordinate value under 2D cartesian labeling
    \return
      - reference to the contained x-axis coordinate in primary v2f f4 field
    */
    const f4& X(void) const
    { return x; } // end const f4& v2f::X(void) const                         */


    /** Set the v2f primary-axis coordinate value under 2D cartesian labeling
    \brief
      - set the v2f primary-axis coordinate value under 2D cartesian labeling
    \param xu_value
      - the float-point value to be set into the primary-axis member field
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& X(f4 xu_value)
    { x = xu_value; return *this; } // end v2f& v2f::X(f4)                    */

    /** <x, x, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <x, x, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \return
      - a v3f set to redundantly use the <x, x, x> values of this
    */
    v3f XXX(void) const;

    /** <x, x, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <x, x, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \return
      - a v3f set to redundantly use the <x, x, y> values of this
    */
    v3f XXY(void) const;

    /** <x, x, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <x, x, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in tertiary field of the new v3f (0.0f by default)
    \return
      - a v3f set to use the <x, y> values of this v2f with a tertiary value
    */
    v3f XXZ(f4 zbp_value = 0.0f) const;

    /** <x, y, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <x, y, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \return
      - a v3f set to redundantly use the <x, y, x> values of this
    */
    v3f XYX(void) const;

    /** <x, y, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <x, y, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \return
      - a v3f set to redundantly use the <x, y, y> values of this
    */
    v3f XYY(void) const;

    /** <x, y, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <x, y, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in tertiary field of the new v3f (0.0f by default)
    \return
      - a v3f set to use the <x, y> values of this v2f with a tertiary value
    */
    v3f XYZ(f4 zbp_value = 0.0f) const;

    /** <x, (z), x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <x, (z), x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in secondary field of the new v3f (0.0f by default)
    \return
      - a v3f set to the x value of this in 1st & 3rd with a given 2nd value
    */
    v3f XZX(f4 zbp_value = 0.0f) const;

    /** <x, (z), y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <x, (z), y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in secondary field of the new v3f (0.0f by default)
    \return
      - a v3f with the x value of this, a given 2nd value, and this y as a 3rd
    */
    v3f XZY(f4 zbp_value = 0.0f) const;

    /** <x, (z), (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <x, (z), (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 2nd & 3rd fields of the new v3f (0.0f by default)
    \return
      - a v3f with the x value of this, a given value in the 2nd & 3rd elements
    */
    v3f XZZ(f4 zbp_value = 0.0f) const;

    /** <x, (z), (w)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <x, (z), (w)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in secondary field of the new v3f (0.0f by default)
    \param waq_value
      - value to emplace in tertiary field of the new v3f (0.0f by default)
    \return
      - a v3f with the x value of this, given values in the 2nd & 3rd elements
    */
    v3f XZW(f4 zbp_value = 0.0f, f4 waq_value = 0.0f) const;

    /** <x, y, (z), (w)> 4D swizzling operation to promote to 4 dimensions
    \brief
      - <x, y, (z), (w)> 4D swizzling operation to promote to 4 dimensions
    \param zbp_value
      - value to emplace in tertiary field of the new v4f (0.0f by default)
    \param waq_value
      - value to emplace in quatertnary field of the new v4f (0.0f default)
    \return
      - a v4f set to use the <x, y> values of this v2f with 3rd & 4th addendums
    */
    v4f XYZW(f4 zbp_value = 0.0f, f4 waq_value = 0.0f) const;

    /** Get the v2f secondary-axis coordinate value under 2D cartesian labeling
    \brief
      - get the v2f secondary-axis coordinate value under 2D cartesian labeling
    \return
      - reference to the contained, secondary v2f f4 coordinate field
    */
    const f4& Y(void) const
    { return y; } // end const f4& v2f::Y(void) const                         */

    /** Set the v2f secondary-axis coordinate value with 3D cartesian labeling
    \brief
      - set the v2f secondary-axis coordinate value with 3D cartesian labeling
    \param yv_value
      - the float point value to be set into the secondary-axis member field
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& Y(f4 yv_value)
    { y = yv_value; return *this; } // end v2f&  v2f::Y(f4)                   */

    /** <y, x, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <y, x, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \return
      - a v3f set to redundantly use the <y, x, x> values of this
    */
    v3f YXX(void) const;

    /** <y, x, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <y, x, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \return
      - a v3f set to redundantly use the <y, x, y> values of this
    */
    v3f YXY(void) const;

    /** <y, x, z> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <y, x, z> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in tertiary field of the new v3f (0.0f by default)
    \return
      - a v3f set to use the <y, x> values of this v2f with a tertiary value
    */
    v3f YXZ(f4 zbp_value = 0.0f) const;

    /** <y, y, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <y, y, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \return
      - a v3f set to redundantly use the <y, y, x> values of this
    */
    v3f YYX(void) const;

    /** <y, y, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <y, y, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \return
      - a v3f set to redundantly use the <y, y, y> value of this
    */
    v3f YYY(void) const;

    /** <y, y, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <y, y, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in tertiary field of the new v3f (0.0f by default)
    \return
      - a v3f set to use the <y, y> values of this v2f with a tertiary value
    */
    v3f YYZ(f4 zbp_value = 0.0f) const;

    /** <y, (z), x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <y, (z), x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in secondary field of the new v3f (0.0f by default)
    \return
      - a v3f set to use the x & y values of this with a given 2nd value
    */
    v3f YZX(f4 zbp_value = 0.0f) const;

    /** <y, (z), y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <y, (z), y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in secondary field of the new v3f (0.0f by default)
    \return
      - a v3f set to redundantly use the y value of this with a given 2nd value
    */
    v3f YZY(f4 zbp_value = 0.0f) const;

    /** <y, (z), (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <y, (z), (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 2nd & 3rd fields of the new v3f (0.0f by default)
    \return
      - a v3f with y value of this in x, and a given value in 2nd & 3rd elements
    */
    v3f YZZ(f4 zbp_value = 0.0f) const;

    /** <y, (z), (w)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <y, (z), (w)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in secondary field of the new v3f (0.0f by default)
    \param waq_value
      - value to emplace in tertiary field of the new v3f (0.0f by default)
    \return
      - a v3f with the y value in x, and given values in the 2nd & 3rd elements
    */
    v3f YZW(f4 zbp_value = 0.0f, f4 waq_value = 0.0f) const;

    /** <(z), x, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), x, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st field of the new v3f (0.0f by default)
    \return
      - a v3f with x value of this in y & z, and a given value in 1st element
    */
    v3f ZXX(f4 zbp_value = 0.0f) const;

    /** <(z), x, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), x, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st field of the new v3f (0.0f by default)
    \return
      - a v3f with a given value in 1st element, x value in y, and y value in z 
    */
    v3f ZXY(f4 zbp_value = 0.0f) const;

    /** <(z), x, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), x, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st & 3rd fields of the new v3f (0.0f by default)
    \return
      - a v3f with a given value in 1st & 3rd elements, and this x value in y 
    */
    v3f ZXZ(f4 zbp_value = 0.0f) const;

    /** <(z), x, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), x, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st element / field of the new v3f (0.0f default)
    \param waq_value
      - value to emplace in 3rd element / field of the new v3f (0.0f default)
    \return
      - a v3f with a given value in 1st & 3rd elements, and this x value in y 
    */
    v3f ZXW(f4 zbp_value = 0.0f, f4 waq_value = 0.0f) const;

    /** <(z), y, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), y, x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st element / field of the new v3f (0.0f default)
    \return
      - a v3f with a given value in 1st element, y in 2nd & x value in 3rd
    */
    v3f ZYX(f4 zbp_value = 0.0f) const;

    /** <(z), y, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), y, y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st element / field of the new v3f (0.0f default)
    \return
      - a v3f with a given value in 1st element, and this y value in 2nd & 3rd
    */
    v3f ZYY(f4 zbp_value = 0.0f) const;

    /** <(z), y, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), y, (z)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st & 3rd fields of the new v3f (0.0f by default)
    \return
      - a v3f with a given value in 1st & 3rd elements, and this y value in y 
    */
    v3f ZYZ(f4 zbp_value = 0.0f) const;

    /** <(z), y, (w)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), y, (w)> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st element / field of the new v3f (0.0f default)
    \param waq_value
      - value to emplace in 3rd element / field of the new v3f (0.0f default)
    \return
      - a v3f with given values in 1st & 3rd elements, and this y value in y 
    */
    v3f ZYW(f4 zbp_value = 0.0f, f4 waq_value = 0.0f) const;

    /** <(z), (z), x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), (z), x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st & 2nd fields of the new v3f (0.0f by default)
    \return
      - a v3f with a given value in 1st & 2nd elements, and this x value in z
    */
    v3f ZZX(f4 zbp_value = 0.0f) const;

    /** <(z), (z), y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), (z), y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st & 2nd fields of the new v3f (0.0f by default)
    \return
      - a v3f with a given value in 1st & 2nd elements, and this y value in z
    */
    v3f ZZY(f4 zbp_value = 0.0f) const;

    /** <(z), (w), x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), (w), x> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st element / field of the new v3f (0.0f default)
    \param waq_value
      - value to emplace in 2nd element / field of the new v3f (0.0f default)
    \return
      - a v3f with given value in 1st & 2nd elements, and this x value in z 
    */
    v3f ZWX(f4 zbp_value = 0.0f, f4 waq_value = 0.0f) const;

    /** <(z), (w), y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \brief
      - <(z), (w), y> 3D pseudo-swizzling operation to promote to 3 dimensions
    \param zbp_value
      - value to emplace in 1st element / field of the new v3f (0.0f default)
    \param waq_value
      - value to emplace in 2nd element / field of the new v3f (0.0f default)
    \return
      - a v3f with given value in 1st & 2nd elements, and this y value in z 
    */
    v3f ZWY(f4 zbp_value = 0.0f, f4 waq_value = 0.0f) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Creates a negated copy with relevant v2f coordinate values
    \brief
      - creates a negated copy with relevant v2f coordinate values
    \return
      - copy of the v2f with <-x, -y> member values
    */
    v2f operator-(void) const;


    /** Negates v2f element values to represent the inverse displacement
    \brief
      - negates v2f element values to represent the inverse displacement
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& operator!(void);


    /** Sets the source v2f member values into this v2f value fields
    \brief
      - sets the source v2f member values into this v2f value fields
    \param source
      - the v2f which will have its values copied memberwise into this v2f
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& operator=(const v2f& source);


    /** Surrogate a v2f value reference returned from a function for this v2f
    \brief
      - surrogate a v2f value reference returned from a function for this v2f
    \param result
      - the v2f reference resultant from a function from which to steal values
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& operator=(v2f&& result) noexcept;


    /** Get a value from ['x'] or ['y'] channels (or equivalent)
    \brief
      - get a value from ['x'] or ['y'] channels (or equivalent)
    \details
      (0 | x | X | i | I | u | U) -> x
      (1 | y | Y | j | J | v | V) -> y
    \param channel
      - use (0|'x'|'u')->x, (1|'y'|'v')->y
    \return
      - copy of the contained dimension member value associated to the channel
    */
    f4 operator[](s1 channel) const;


    /** Get a value from ['x'] or ['y'] channels (or equivalent)
    \brief
      - get a value from ['x'] or ['y'] channels (or equivalent)
    \details
      (0 | x | X | i | I | u | U) -> x
      (1 | y | Y | j | J | v | V) -> y
    \param channel
      - use (0|'x'|'u')->x, (1|'y'|'v')->y
    \return
      - reference to the contained field associated to the subscript number
    */
    f4& operator[](s1 channel);


    /** Confirm if 2 v2f's are relatively equal in x and y values
    \brief
      - confirm if 2 v2f's are relatively equal in x and y values
    \param other
      - right hand side to be compared memberwise with left hand for equality
    \return
      - true only if all members are in close proximity of one another
    */
    bool operator==(const v2f& other) const;


    /** Confirm if 2 v2f's are not equal in any of x or y values
    \brief
      - confirm if 2 v2f's are not equal in any of x or y values
    \param other
      - right hand side to be compared memberwise with left hand for inequality
    \return
      - true if any members are not in close proximity of one another
    */
    bool operator!=(const v2f& other) const;


    /** Calculate the member-wise addition of this v2f and a uniform constant
    \brief
      - calculate the member-wise addition of this v2f and a uniform constant
    \param scalar
      - value to be added into each member uniformly as a constant offset
    \return
      - copy of the memberwise sum of this v2f with the constant value offset
    */
    inline v2f operator+(f4 scalar) const
    { return v2f(x + scalar, y + scalar); } // end v2f operator+(f4) const    */


    /** Calculate the member-wise addition of 2 v2f's and return the result
    \brief
      - calculate the member-wise addition of 2 v2f's and return the result
    \param other
      - source vector to have memberwise summation performed with a copy of this
    \return
      - copy of the memberwise sum of this v2f and the other into a new v2f
    */
    v2f operator+(const v2f& other) const;


    /** Uniformly add the constant offset into this v2f's element values
    \brief
      - uniformly add the constant offset into this v2f's element values
    \param scalar
      - value to be added into each member uniformly as a constant offset
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    inline v2f& operator+=(f4 scalar)
    { return X(x + scalar).Y(y + scalar); } // end v2f& operator+=(f4) const  */


    /** Add an other v2f's values memberwise into this v2f's values
    \brief
      - add an other v2f's values memberwise into this v2f's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& operator+=(const v2f& other);


    /** Calculate the member-wise addition of this v2f and a uniform constant
    \brief
      - calculate the member-wise addition of this v2f and a uniform constant
    \param scalar
      - value to be added into each member uniformly as a constant offset
    \return
      - copy of the memberwise sum of this v2f with the constant value offset
    */
    inline v2f operator-(f4 scalar) const
    { return v2f(x - scalar, y - scalar); } // end v2f operator-(f4) const    */


    /** Calculate the member-wise difference of 2 v2f's and return the result
    \brief
      - calculate the member-wise difference of 2 v2f's and return the result
    \param other
      - source vector to have memberwise difference calculated with this (copy)
    \return
      - copy of the memberwise difference of this and the other, in a new v2f
    */
    v2f operator-(const v2f& other) const;


    /** Uniformly subtract the constant -offset into this v2f's element values
    \brief
      - uniformly subtract the constant -offset into this v2f's element values
    \param scalar
      - value to be added into each member uniformly as a constant offset
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    inline v2f& operator-=(f4 scalar)
    { return X(x - scalar).Y(y - scalar); } // end v2f& operator-=(f4) const  */


    /** Subtract an other v2f's values memberwise from this v2f's values
    \brief
      - subtract an other v2f's values memberwise from this v2f's values
    \param other
      - source vector to have negated memberwise summation performed with this
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& operator-=(const v2f& other);


    /** Scalar product multiplies each field uniformly by a scalar constant
    \brief
      - scalar product multiplies each field uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x and y values
    \return
      - resultant, member-wise scaled copy of this v2f's values
    */
    v2f operator*(f4 scalar) const;


    /** Scalar product sets all members to grow uniformly by a scalar constant
    \brief
      - scalar product sets all members to grow uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x and y values
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& operator*=(f4 scalar);


    /** Memberwise multiply by the equivalent other members (not Dot summed)
    \brief
      - memberwise multiply by the equivalent other members (not Dot summed)
    \details
      - dot product is useful more frequently, but also just this result's sum
      the more granular (*) operation still has applications: just because the
      more complex operation is prevalently useful doesn't mean it semantically
      overtakes the meaning of the base definition of multiplication, as dot
      for the default operator result means memberwise multiplication has to be
      a named method not on the operator (& a long name at that).
    \param other
      - external source vector to be memberwise multiplied with this vector
    \return
      - memberwise product with other: < a.x * b.x, a.y * b.y >
    */
    inline v2f operator*(const v2f& other) const
    {
      return v2f(x * other.x, y * other.y);
    } // end v2f operator*(const v2f&) const                                  */


    /** Memberwise multiply by the equivalent other members (not Dot summed)
    \brief
      - memberwise multiply by the equivalent other members (not Dot summed)
    \details
      - dot product is useful more frequently, but also just this result's sum
      the more granular (*) operation still has applications: just because the
      more complex operation is prevalently useful doesn't mean it semantically
      overtakes the meaning of the base definition of multiplication, as dot
      for the default operator result means memberwise multiplication has to be
      a named method not on the operator (& a long name at that).
    \param other
      - external source vector to be memberwise multiplied with this vector
    \return
      - memberwise product with other: < a.x * b.x, a.y * b.y >
    */
    inline v2f& operator*=(const v2f& other)
    {
      x *= other.x; y *= other.y;
      return *this;
    } // end v2f operator*(const v2f&) const                                  */


    /** Perform uniform division by divisor, returning the result
    \brief
      - perform uniform division by divisor, returning the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which <x,y> values are uniformly divided
    \return
      - resultant, member-wise scaled copy of this v2f's values
    */
    v2f operator/(f4 divisor) const;


    /** Divides all members uniformly by divisor, setting the result
    \brief
      - divides all members uniformly by divisor, setting the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which <x,y> values are to be uniformly divided
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    v2f& operator/=(f4 divisor);


  }; // end v2f union / struct / class type declaration


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   //! Zero vector in 2 dimensions; error state reserved value
   extern const v2f& nullv2f;



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Operator Overloads                          */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Left hand sum adds a uniform constant into each axis / member of rhs
  \brief
    - left hand sum adds a uniform constant into each axis / member of rhs
  \details
    - uniform addition / subtraction on vectors often considered indeterminate
    in interpretation: applied to all dimensions? one?  which one(s)? & get
    ommitted accordingly.  Here 2D interpretations include co-linear ranges, as
    well as multidimensional data, where the constant offset is only vague to
    the latter datasets.  Only to be applied assuming that uniform offset is
    sensible (i.e.- co-linear data, or strictly wanting uniform offsets).  If
    less generic containers only supporting the latterwere made, it ought not
    extend support of these overloads at all.
  \param initial
    - value *each* vector element has for an initial value (not just [0])
  \param offset
    - right-hand side argument to add to initial in x and y respectively
  \return
    - copy of rhs v2f's negated values, added to a uniform constant in each
  */
  v2f operator+(f4 initial, const v2f& offset);


  /** Left hand range difference adds memberwise rhs negation to initial value
  \brief
    - left hand range difference adds memberwise rhs negation to initial value
  \details
    - uniform addition / subtraction on vectors often considered indeterminate
    in interpretation: applied to all dimensions? one?  which one(s)? & get
    ommitted accordingly.  Here 2D interpretations include co-linear ranges, as
    well as multidimensional data, where the constant offset is only vague to
    the latter datasets.  Only to be applied assuming that uniform offset is
    sensible (i.e.- co-linear data, or strictly wanting uniform offsets).  If
    less generic containers only supporting the latterwere made, it ought not
    extend support of these overloads at all.
  \param initial
    - value *each* vector element has for an initial value (not just [0])
  \param offset
    - right-hand side argument to subtract from initial x and y respectively
  \return
    - copy of rhs v2f's negated values, added to a uniform constant in each
  */
  v2f operator-(f4 initial, const v2f& offset);


  /** Left hand scalar multiplication scales all members of rhs
  \brief
    - left hand scalar multiplication scales all members of rhs
  \param scalar
    - scalar amount by which to multiply x and y values of right-hand side
  \param rhs
    - right-hand side argument to be copied and scaled in x and y by scalar
  \return
    - copy of rhs v2f's values, scaled by scalar in <x,y>
  */
  v2f operator*(f4 scalar, const v2f& vec);


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
  v2f operator/(f4 uniform, const v2f& divisor);


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
   std::ostream& operator<<(std::ostream& output, const v2f& vector);


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                      Helper Function Declarations                       */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Generate a distance / displacement vector between two coordinate vectors
  \brief
    - generate a distance / displacement vector between two coordinate vectors
  \param initial
    - starting point (assumed), being measured for the resultant displacement
  \param terminal
    - ending point (assumed), being measured for the resultant displacement
  \return
    - displacement from initial to terminal (ie memberwise terminal - initial)
  */
  v2f Distance(const v2f& initial, const v2f& terminal);

} // end dp namespace
