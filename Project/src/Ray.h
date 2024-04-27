/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Ray.h
Purpose:  6D Container of 3D <x,y,z> < coordinate, directionVector > pair
Details: member footprint kept in line with v6f use of <min, max> pattern
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

// "./src/..."
#include "v3f.h"  // 3D cartesian coordinate / vector member definitions


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  class AABB;   // Bounding box in <min, max> extrema, for intersection testing
  class BSphere;// Bounding sphere <origin, radius>; test (closest) intersection
  class Plane;  // Boundary <normal, sum>; test intersection (bool, dist, point)
  class Triangle;// 2D Boundary subset between 3 points; test intersection
  union v6f;    // 6D vector capable of being used for abstract <v3f, v3f> I/O

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  //! 3D Coordinate with an associated 3D direction emanating infinitely outward
  class Ray
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a Ray: 3 point and 3 direction values (default <0,0,0>, <0,0,1>)
    \brief
      - create a Ray: 3 point and 3 direction values (default <0,0,0>, <0,0,1>)
    \param pX
      - point of origin component of ray in the x axis
    \param pY
      - point of origin component of ray in the y axis
    \param pZ
      - point of origin component of ray in the z axis
    \param dX
      - direction component of ray in the x axis (+right, -left facing)
    \param dY
      - direction component of ray in the y axis (+up, -down facing)
    \param dZ
      - direction component of ray in the z axis (+back, -front facing)
    */
    Ray(f4 pX=0.0f, f4 pY=0.0f, f4 pZ=0.0f, f4 dX=0.0f, f4 dY=0.0f, f4 dZ=1.0f);


    /** Creates an Axis Aligned Bounding Box from given < max, min > values
    \brief
      - creates an Axis Aligned Bounding Box from given < max, min > values
    \details
      - max before min as 0 minimum is conventional & therefore defaultable,
      counter to subscript ordering which emplaces <min, max> from <max, min>
    \param origin
      - 3D cartesian < x, y, z > coordinate from which Ray originates
    \param direction
      - 3D < x, y, z > unit vector in which Ray emenates
    */
    Ray(const v3f& origin, const v3f& direction = Z_HAT);


    /** Make a Ray from source AABB converting extrema range to AABB extents
    \brief
      - make a Ray from source AABB converting extrema range to AABB extents
    \param source
      - <mid,max> values to convert to AABB <mid,ext> to read as Ray <orig,dir>
    */
    Ray(const AABB& source);


    /** Make a Ray from plane (& point) using <orig(point), dir(plane.norm)>
    \brief
      - make a Ray from plane (& point) using <orig(point), dir(plane.norm)>
    \details
      - source plane origin presumed in normal vec passing through world origin
    \param source
      - < <normal>, sum > values to read memberwise as Ray < <orig>, <dir> >
    \param point
      - < x, y > coords in source plane to act as world coords of Ray origin
    */
    Ray(const Plane& source, const v2f& point = nullv2f);


    /** Copies an Axis Aligned Bounding Box from given source AABB settings
    \brief
      - copies an Axis Aligned Bounding Box from given source AABB settings
    \param source
      - 6D, abstract < pX, pY, pZ, dX, dY, dZ > values to use for Ray basis
    \param inRanges
      - < v3f, v3f > read as points, and average / extent calculated if true
    */
    Ray(const v6f& source, bool inRanges = false);


    /** Copies a Ray from given source Ray's given < orig, dir > settings
    \brief
      - copies a Ray from given source Ray's given < orig, dir > settings
    \param source
      - existing <orig.x, orig.y, orig.z, dir.x, dir.y, dir.z> values to copy
    */
    Ray(const Ray& source);


    /** Moves a Ray into place by hijacking the given Ray r-value ref
    \brief
      - moves a Ray into place by hijacking the given Ray r-value ref
    \param source
      - <orig.x, orig.y, orig.z, dir.x, dir.y, dir.z> r-value ref data to use
    */
    Ray(Ray&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Methods                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get the point in Ray's direction the given distance from it's origin
    \brief
      - get the point in Ray's direction the given distance from it's origin
    \details
      - just resultant point: for a Ray shifted equivalently, use scale/shift
    \param distance
      - scalar multiple of direction (unit) vector away from emanation point
    \return
      - coordinate the given distance away from Ray's center in it's direction
    */
    inline v3f At(f4 distance) const
    { return orig + RealF(distance) * dir; } // end v3f Ray::At(f4) const     */

    /** Access cartesian < x, y, z > coordinate of Ray center
    \brief
      - access cartesian < x, y, z > coordinate of Ray center
    \return
      - reference to contained Ray < x, y, z > cartesian coordinate of center
    */
    inline const v3f& Center(void) const
    { return Origin(); } // end const v3f& Ray::Center(void) const            */


    /** Set < x, y, z > coordinate of Ray's new point of emination
    \brief
      - set < x, y, z > coordinate of Ray's new point of emination
    \param center
      - world space coordinate from which the Ray should be set to eminate
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Center(const v3f& center)
    { return Origin(center); } // end Ray& Ray::Center(const v3f&)            */


    /** Set < x, y, z > coordinate components of Ray's new point of emination
    \brief
      - set < x, y, z > coordinate components of Ray's new point of emination
    \param pX
      - world space coord component in x-axis for Ray's new emination point
    \param pY
      - world space coord component in y-axis for Ray's new emination point
    \param pZ
      - world space coord component in z-axis for Ray's new emination point
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Center(f4 pX, f4 pY, f4 pZ)
    { return Origin(pX, pY, pZ); } // end Ray& Ray::Center(f4, f4, f4)        */

    /** Access cartesian < x, y, z > coordinate of Ray center
    \brief
      - access cartesian < x, y, z > coordinate of Ray center
    \return
      - reference to contained Ray < x, y, z > cartesian coordinate of center
    */
    inline const v3f& Coord(void) const
    { return Origin(); } // end const v3f& Ray::Coord(void) const             */


    /** Set < x, y, z > coordinate of Ray's new point of emination
    \brief
      - set < x, y, z > coordinate of Ray's new point of emination
    \param center
      - world space coordinate from which the Ray should be set to eminate
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Coord(const v3f& center)
    { return Origin(center); } // end Ray& Ray::Coord(const v3f&)             */


    /** Set < x, y, z > coordinate components of Ray's new point of emination
    \brief
      - set < x, y, z > coordinate components of Ray's new point of emination
    \param pX
      - world space coord component in x-axis for Ray's new emination point
    \param pY
      - world space coord component in y-axis for Ray's new emination point
    \param pZ
      - world space coord component in z-axis for Ray's new emination point
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Coord(f4 pX, f4 pY, f4 pZ)
    { return Origin(pX, pY, pZ); } // end Ray& Ray::Coord(f4, f4, f4)         */


    /** Get a copy of this Ray's < origin, direction > data
    \brief
      - get a copy of this Ray's < origin, direction > data
    \return
      - a copy of this Ray's < origin, direction > data: Ray(*this)
    */
    inline Ray Copy(void) const
    { return Ray(*this); } // end Ray& Ray::Copy(void) const                  */

    /** Access cartesian < x, y, z > coordinate of Ray center
    \brief
      - access cartesian < x, y, z > coordinate of Ray center
    \return
      - reference to contained Ray < x, y, z > cartesian coordinate of center
    */
    inline const v3f& Point(void) const
    { return Origin(); } // end const v3f& Ray::Point(void) const             */


    /** Set < x, y, z > coordinate of Ray's new point of emination
    \brief
      - set < x, y, z > coordinate of Ray's new point of emination
    \param center
      - world space coordinate from which the Ray should be set to eminate
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Point(const v3f& center)
    { return Origin(center); } // end Ray& Ray::Point(const v3f&)             */


    /** Set < x, y, z > coordinate components of Ray's new point of emination
    \brief
      - set < x, y, z > coordinate components of Ray's new point of emination
    \param pX
      - world space coord component in x-axis for Ray's new emination point
    \param pY
      - world space coord component in y-axis for Ray's new emination point
    \param pZ
      - world space coord component in z-axis for Ray's new emination point
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Point(f4 pX, f4 pY, f4 pZ)
    { return Origin(pX, pY, pZ); } // end Ray& Ray::Point(f4, f4, f4)         */


    /** Access < x, y, z > unit vector of Ray's directional emination
    \brief
      - access < x, y, z > unit vector of Ray's directional emination
    \return
      - reference to contained Ray < x, y, z > cartesian direction unit vector
    */
    inline const v3f& Direction(void) const
    { return dir; } // end const v3f& Ray::Direction(void) const              */


    /** Get scalar multiple of ray's direction to reach the given 2D boundary
    \brief
      - access < x, y, z > unit vector of Ray's directional emination
    \details
      - simplicity here is the reason parallel ray distance =: -inf, not +inf
    \param plane
      - use the given plane provided to get this ray's scalar distance
    \return
      - this ray's direction vector's scalar multiple
    */
    f4 Distance(const Plane& plane) const;


    /** Set < x, y, z > unit vector of Ray's new directional emination
    \brief
      - set < x, y, z > unit vector of Ray's new directional emination
    \param direction
      - unit vector in the direction the ray should be set to eminate
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Direction(const v3f& direction)
    {
      dir = (direction == nullv3f) ? Z_HAT : direction.UnitVec();
      return *this;

    } // end Ray& Ray::Direction(const v3f&)                                  */


    /** Set < x, y, z > unit vector of Ray's new directional emination
    \brief
      - set < x, y, z > unit vector of Ray's new directional emination
    \param dX
      - unit vector component in x-axis for Ray's new emination direction
    \param dY
      - unit vector component in y-axis for Ray's new emination direction
    \param dZ
      - unit vector component in z-axis for Ray's new emination direction
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Direction(f4 dX, f4 dY, f4 dZ)
    {
      return Direction(v3f(dX, dY, dZ));

    } // end Ray& Ray::Direction(f4, f4, f4)                                  */


    /** Get whether this ray has a point of intersection with a bounding box
    \brief
      - get whether this ray has a point of intersection with a bounding box
    \details
      - still sets a point of intersection even if false (behind / skew to ray).
      false return =: INf => "intersect at infinity" for consistency of results
      may return a true, but negative t value too, indicative of origin in box
      but ray origin being in box may still be positive too (if past center)
    \param box
      - bounding box to test for collision with this ray origin / direction
    \param tDist
      - ray length to meet box, to be set (if any; first if multiple), if given
    \return
      - true only if this has a real, non-negative scalar to reach the ball
    */
    bool IsIn(const AABB& box, f4* tDist = nullptr) const;


    /** Get whether this ray has a point of intersection with a bounding sphere
    \brief
      - get whether this ray has a point of intersection with a bounding sphere
    \details
      - still sets a point of intersection even if false (behind / skew to ray).
      false return =: INf => "intersect at infinity" for consistency of results
      may return a true, but negative t value too, indicative of origin in ball
      but ray origin being in ball may still be positive too (if past center)
    \param ball
      - bounding sphere to test for collision with this ray origin / direction
    \param tDist
      - ray length to meet ball, to be set (if any; first if multiple), if given
    \return
      - true only if this has a real, non-negative scalar to reach the ball
    */
    bool IsIn(const BSphere& ball, f4* tDist = nullptr) const;


    /** Get whether this ray has a point of intersection in the given plane
    \brief
      - get whether this ray has a point of intersection in the given plane
    \details
      - will still set point of intersection even if false (from behind ray).
      still set to "intersect at infinity" for the sake of predictable results
      simplicity here is the reason parallel ray distance =: -inf, not +inf
    \param plane
      - 2D boundary to use in testing for intersection with this ray
    \param intersection
      - point of intersection (if any, including behind), will be set if given
    \return
      - true only if this has a non-negative scalar multiple reaching the plane
    */
    bool IsIn(const Plane& plane, v3f* intersection = nullptr) const;


    /** Get whether the ray direction to the angle of a given plane
    \brief
      - get whether the triangle points align to the angle of a given plane
    \param plane
      - plane whose orientation will be tested against this triangle's plane
    \return
      - true if triangle unit-vec normal is equal to the plane's unit-vec normal
    */
    bool IsParallel(const Plane& plane) const;


    /** Get whether the triangle aligns to the same facing angle of another
    \brief
      - get whether the triangle aligns to the same facing angle of another
    \param tri
      - triangle whose orientation will be tested against this triangle's
    \return
      - true if both unit-vec normals are equal to to one another
    */
    bool IsParallel(const Triangle& tri) const;


    /** Access cartesian < x, y, z > coordinate of Ray origin
    \brief
      - access cartesian < x, y, z > coordinate of Ray origin
    \return
      - reference to contained Ray < x, y, z > cartesian coordinate of origin
    */
    inline const v3f& Origin(void) const
    { return orig; } // end const v3f& Ray::Origin(void) const                */


    /** Set < x, y, z > coordinate of Ray's new point of emination
    \brief
      - set < x, y, z > coordinate of Ray's new point of emination
    \param center
      - world space coordinate from which the Ray should be set to eminate
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Origin(const v3f& center)
    {
      orig = center;
      return *this;

    } // end Ray& Ray::Origin(const v3f&)                                     */


    /** Set < x, y, z > coordinate components of Ray's new point of emination
    \brief
      - set < x, y, z > coordinate components of Ray's new point of emination
    \param pX
      - world space coord component in x-axis for Ray's new emination point
    \param pY
      - world space coord component in y-axis for Ray's new emination point
    \param pZ
      - world space coord component in z-axis for Ray's new emination point
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Origin(f4 pX, f4 pY, f4 pZ)
    {
      return Origin(v3f(pX, pY, pZ));

    } // end Ray& Ray::Origin(f4, f4, f4)                                     */


    /** Scale point of origin to move along Ray's direction by the given scalar
    \brief
      - scale point of origin to move along Ray's direction by the given scalar
    \param scalar
      - distance along direction vector by which to move this Ray's origin
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Scale(f4 scalar)
    {
      orig += dir * RealF(scalar);
      return *this;

    } // end Ray& Ray::Scale(f4)                                              */


    /** Get a copy of this with origin shifted along direction by given scalar
    \brief
      - get a copy of this with origin shifted along direction by given scalar
    \param scalar
      - distance along direction vector by which to shift this Ray's origin
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray Scaled(f4 scalar) const
    {
      return Copy().Scale(scalar);

    } // end Ray& Ray::Scale(f4)                                              */


    /** Scale point of origin shifted on Ray's direction by the given scalar
    \brief
      - scale point of origin shifted on Ray's direction by the given scalar
    \param scalar
      - distance along direction vector by which to shift this Ray's origin
    \return
      - reference to the modified Ray for easier subsequent inline action
    */
    inline Ray& Shift(f4 scalar)
    { return Scale(scalar); } // end Ray& Ray::Shift(f4)                      */


    /** Get a copy of this with origin shifted along direction by given scalar
    \brief
      - get a copy of this with origin shifted along direction by given scalar
    \param scalar
      - distance along direction vector by which to shift this Ray's origin
    \return
      - copy with this Ray's direction, but origin += direction * scalar
    */
    inline Ray Shifted(f4 scalar)
    { return Scaled(scalar); } // end Ray& Ray::Shifted(f4)                   */


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Operator Overloads                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Methods                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Cartesian <x, y, z> world coordinate of ray origin point
    v3f orig;

    //! unit vector of direction the ray travels infinitely, from the origin on
    v3f dir;

    //! memberwise reciprocal of direction the ray travels, to save on divisions
    v3f _dir;

  }; // end Ray class declaration

  // Ensurance that memory footprint is as intended
  static_assert(sizeof(Ray) == 4 * 9, "Ray size not tightly packed");

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

} // end dp namespace
