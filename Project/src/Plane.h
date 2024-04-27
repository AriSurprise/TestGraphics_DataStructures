/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Plane.h
Purpose:  Infinite 2D boundary descriptor / 3D space subset in normal.xyz + sum
Details:  Member footprint kept in line with v4f use of <x,y,z,w=sum> pattern
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
#include "v3f.h"  // Normal direction member, for cartesian grouping semantic
#include "v4f.h"  // 4D <x,y,z,w> data, available for Plane I/O conversion
#include "v6f.h"  // 6D <orig,dir> data, available for Plane I/O conversion
#include "Ray.h"  // 6D <Point,normal> data, available for Plane I/O conversion


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  class AABB; // Bounding Box in extrema, for intersection testing
  class AOBB; // Bounding Box in center & extents, for intersection testing
  class BSphere; // Bounding Sphere, for intersection testing

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  //! Cartesian Plane: a boundary given in <normal, sum>; 2D subset of 3D space
  class Plane
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a Plane normal <nX,nY,nZ>, sum fitting normal (default +z plane)
    \brief
      - create a Plane normal <nX,nY,nZ>, sum fitting normal (default +z plane)
    \param nX
      - component of Plane normal unit vector along the x-axis
    \param nY
      - component of Plane normal unit vector along the y-axis
    \param nZ
      - component of Plane normal unit vector along the z-axis
    \param sum
      - where sum*(nX + nY + nZ) describes any n.XYZ() as a point in the Plane
    */
    Plane(f4 nX = 0.0f, f4 nY = 0.0f, f4 nZ = 1.0f, f4 sum = 0.0f);


    /** Create a Plane from the given input < normal, sum > values
    \brief
      - create a Plane from the given input < normal, sum > values
    \param normal
      - unit vector describing orientation perpendicular to the desired plane
    \param sum
      - where (normal*sum).Sum() describes any n.XYZ() as a point in the Plane
    */
    Plane(const v3f& normal, f4 sum = 0.0f);


    /** Create a Plane from the given input < normal(<x,y,z>), sum(w) > values
    \brief
      - create a Plane from the given input < normal(<x,y,z>), sum(w) > values
    \param source
      - 4D <x,y,z,w> float data set to use as normal(x,y,z), sum(w)
    */
    Plane(const v4f& source);


    /** Create a Plane from 6D <point, normal> data so point.Dot(normal)=:sum
    \brief
      - copies an Axis Aligned Bounding Box from given source AABB settings
    \details
      - <v3f,v3f> vector or point pairs feed to intentionally give bad results
    \param source
      - 4D <x,y,z,w> float data set to use as center(x,y,z), radius(w)
    */
    Plane(const v6f& source);


    /** Create a Bounding Sphere encompasing a given source AABB's set values
    \brief
      - create a Bounding Sphere encompasing a given source AABB's set values
    \param source
      - source AABB for setting this where: center(s.mid), radius(s.ext.length)
    */
    Plane(const Ray& source);


    /** Copy a Bounding Sphere from another given source BSphere's settings
    \brief
      - copy a Bounding Sphere from another given source BSphere's settings
    \param source
      - < center.x, center.y, center.z, radius > values to copy into this
    */
    Plane(const Plane& source);


    /** Move data into a Bounding Sphere, hijacking the given AABB r-value ref
    \brief
      - move data into a Bounding Sphere, hijacking the given AABB r-value ref
    \param result
      - < center.x, center.y, center.z, radius > data to appropriate in this
    */
    Plane(Plane&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Methods                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get unsigned orthogonal distance the given 3D point is from this plane
    \brief
      - get unsigned orthogonal distance the given 3D point is from this plane
    \param point
      - point to be evaulated against this plane to test for proximity
    \return
      - true only if the point is within given proximity to this plane
    */
    inline f4 Distance(const v3f& point) const
    {
      return AbsF(point.Dot(n) - s);

    } // end f4 Plane::Distance(const v3f&) const                             */

    /** Get whether the given bounding box has any intersection with this plane
    \brief
      - get whether the given bounding box has any intersection with this plane
    \param box
      - axis aligned bounding box to check for intersection potential with this
    \return
      - true only if any extremal box points are on opposing sides of the plane
    */
    bool IsIn(const AABB& box) const;

    /** Get whether this plane is not parallel to another (has intersection)
    \brief
      - get whether this plane is not parallel to another (has intersection)
    \param other
      - plane to use to check for intersection potential with this plane
    \return
      - true if other plane is not parallel to this (normal directions vary)
    */
    inline bool IsIn(const Plane& other) const
    {
      return !IsParallel(other);

    } // end bool Plane::In(const Plane&) const                               */

    /** Get whether the given ray has any scalar intersection with this plane
    \brief
      - get whether the given ray has any scalar intersection with this plane
    \param ray
      - ray to use to check for intersection potential with this plane
    \return
      - true if ray is not parallel to this plane, or originates in the plane
    */
    inline bool IsIn(const Ray& ray) const
    {
      return ray.IsIn(*this);

    } // end bool Plane::In(const Ray&) const                                 */

    /** Get whether the given 3D point is within proximity of this 2D plane
    \brief
      - get whether the given 3D point is within proximity of this 2D plane
    \param point
      - point to be evaulated against this plane to test for proximity
    \param proximity
      - distance in plane normal for point to be within (-/+) to this plane
    \return
      - true only if the point is within given proximity to this plane
    */
    inline bool IsIn(const v3f& point, f4 proximity = _INf) const
    {
      return NearF(point.Dot(n), s, proximity);

    } // end bool Plane::In(const v3f&, f4) const                             */


    /** Get if a plane has inverse (antiparallel) orientation to this (-normal)
    \brief
      - get if a plane has inverse (antiparallel) orientation to this (-normal)
    \param other
      - plane to be compared for negated memberwise normal equality with this
    \return
      - true only if memberwise near equality is found
    */
    inline bool IsAParallel(const Plane& other) const
    {
      // (no IsColinear() => inadequate if not unit vecs)
      return -n == other.n;
    } // end bool Plane::IsParallel(const Plane&)                             */


    /** Get if another plane is co-planar to this: same direction and position
    \brief
      - get if another plane is co-planar to this: same direction and position
    \param other
      - plane to be compared for memberwise equality with this
    \return
      - true only if memberwise near equality is found
    */
    inline bool IsCoplanar(const Plane& other) const
    {
      // Check scalar to reach plane matches, & that scalar correlates in <n>
      return NearF(Sum(), other.Sum()) && IsParallelS(other)
        || NearF(Sum(), -other.Sum()) && IsAParallel(other);
    } // end bool Plane::IsCoplanar(const Plane&)                             */


    /** Get if a plane has either parallel or anti-parallel orientation to this
    \brief
      - get if a plane has either parallel or anti-parallel orientation to this
    \param other
      - plane to be compared for general (+/-)normal equality with this
    \return
      - true only if memberwise near equality is found
    */
    inline bool IsParallel(const Plane& other) const
    {
      // (no IsColinear() => inadequate if not unit vecs)
      return IsParallelS(other) || IsAParallel(other);
    } // end bool Plane::IsParallel(const Plane&)                             */


    /** Get if a plane has the same orientation (strict parallel => <n> == <n>)
    \brief
      - get if a plane has the same orientation (strict parallel => <n> == <n>)
    \param other
      - plane to be compared for memberwise normal equality with this
    \return
      - true only if memberwise near equality is found
    */
    inline bool IsParallelS(const Plane& other) const
    {
      // (no IsColinear() => inadequate if not unit vecs)
      return n == other.n;
    } // end bool Plane::IsParallel(const Plane&)                             */


    /** Access Plane perpendicular, outward facing normal direction unit vector
    \brief
      - access Plane perpendicular, outward facing normal direction unit vector
    \return
      - unit vector in the outward facing, perpendicular direction of Plane data
    */
    inline const v3f& Normal(void) const
    { return n; } // end const v3f& Plane::Normal(void) const                 */

    /** Set Plane's cartesian orientation unit vector in parent reference frame
    \brief
      - set Plane's cartesian orientation unit vector in parent reference frame
    \param orientation
      - unit vector in Plane's perpendicular outward direction for parent frame
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Normal(f4 nX, f4 nY, f4 nZ)
    {
      return Normal(v3f(nX, nY, nZ));

    } // end Plane& Plane::Normal(f4, f4, f4)                                 */

    /** Set Plane's cartesian orientation unit vector in parent reference frame
    \brief
      - set Plane's cartesian orientation unit vector in parent reference frame
    \param orientation
      - unit vector in Plane's perpendicular outward direction for parent frame
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Normal(const v3f& orientation)
    {
      n = (orientation != nullv3f) ? orientation.UnitVec() : Z_HAT;
      return *this;

    } // end Plane& Plane::Normal(const v3f&)                                 */


    /** Set Plane normal from <nX,nY,nZ> and locational sum from given values
    \brief
      - set Plane normal from <nX,nY,nZ> and locational sum from given values
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Set(f4 nX, f4 nY, f4 nZ, f4 sum)
    {
      return Normal(nX, nY, nZ).Sum(sum);

    } // end Plane& Plane::Set(f4, f4, f4, f4)                                */


    /** Set Plane normal from <nX,nY,nZ> and locational sum from given values
    \brief
      - set Plane normal from <nX,nY,nZ> and locational sum from given values
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Set(const v3f& normal, f4 sum)
    {
      return Normal(normal).Sum(sum);

    } // end Plane& Plane::Set(const v3f&, f4)                                */


    /** Set Plane normal from <x,y,z> and sum from w axis values from the v4f
    \brief
      - set Plane normal from <x,y,z> and sum from w axis values from the v4f
    \param source
      - 6D data for <x,y,z> as any point in the PLane, <w,t,s> as normal dir
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Set(const Ray& source)
    {
      return Normal(source.Direction()).Sum(source.Coord());
    } // end Plane& Plane::Set(const Ray&)                                    */


    /** Set Plane normal from <x,y,z> and sum from w axis values from the v4f
    \brief
      - set Plane normal from <x,y,z> and sum from w axis values from the v4f
    \param source
      - 4D vector to copy the <x,y,z> data from as Plane normal, w as the sum
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Set(const v4f& source)
    {
      return Normal(source.x, source.y, source.z).Sum(source.w);

    } // end Plane& Plane::Set(const v4f&)                                    */


    /** Set Plane normal from <x,y,z> and sum from w axis values from the v4f
    \brief
      - set Plane normal from <x,y,z> and sum from w axis values from the v4f
    \param source
      - 6D data for <x,y,z> as any point in the PLane, <w,t,s> as normal dir
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Set(const v6f& source)
    {
      return Normal(source.dir).Sum(source.orig);

    } // end Plane& Plane::Set(const v4f&)                                    */


    /** Copy Plane normal and sum values from the given source Plane's data
    \brief
      - copy Plane normal and sum values from the given source Plane's data
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Set(const Plane& source)
    {
      return Normal(source.n).Sum(source.s);

    } // end Plane& Plane::Set(const Plane&)                                  */


    /** Set Plane to surrogate the result r-value reference as this data
    \brief
      - set Plane to surrogate the result r-value reference as this data
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Set(Plane&& result) noexcept
    {
      n = std::move(result.n); s = std::move(result.s); return *this;

    } // end Plane& Plane::Set(Plane&&) noexcept                              */


    /** Access sum in use placing plane in 3D (normal.Dot(pointInPlane) =: sum)
    \brief
      - access sum in use placing plane in 3D (normal.Dot(pointInPlane) =: sum)
    \return
      - sum field value equal to this->normal.Dot(pointInPlane), placing Plane
    */
    inline const f4& Sum(void) const
    { return s; } // end const f4& Plane::Sum(void) const                     */


    /** Set normal.Dot(point) sum equalling the value for where Plane exists
    \brief
      - set normal.Dot(point) sum equalling the value for where Plane exists
    \param sum
      - normal.Dot(point) equals this value, establishing where Plane exists
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Sum(f4 sum)
    {
      s = RealF(sum);
      return *this;

    } // end Plane& Plane::Sum(f4)                                            */


    /** Set normal.Dot(point) sum equalling the value for where Plane exists
    \brief
      - set normal.Dot(point) sum equalling the value for where Plane exists
    \param point
      - any point in the desired plane: normal.Dot(point) sums to set value
    \return
      - reference to the modified Plane for easier subsequent inline action
    */
    inline Plane& Sum(const v3f& point)
    {
      return Sum(n.Dot(point));

    } // end Plane& Plane::Sum(const v3f&)                                    */


    /** Set normal.Dot(point) sum equalling the value for where Plane exists
    \brief
      - set normal.Dot(point) sum equalling the value for where Plane exists
    \param localPoint
      - local coords of any point in the plane, relative to localX & <n> dirs
    \param localX
      - plane normal <n>.Cross(localX) will determine localY of 2D coord basis
    \return
      - 3D world coordinates of input point from plane, given localX
    */
    v3f WorldCoords(const v2f& localPoint, const v3f& localX) const;


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

    //! Unit vector of 3D cartesian Plane normal orientation
    v3f n;
    // not *intrinsically* global, but lacks the descriptors for a local basis

    //! Value sum where any point in the Plane dotted with n equals this
    f4 s;
    // if Planes were to support local orientations, more needed here as well

  }; // end Plane class declaration

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Get whether 2 planes are co-planar: same direction and position
  \brief
    - get whether 2 planes are co-planar: same direction and position
  \param lhs
    - plane for the left hand side of equality comparison with rhs
  \param rhs
    - plane for the right hand side of equality comparison with lhs
  \return
    - true only if memberwise near equality is found
  */
  inline bool operator==(const Plane& lhs, const Plane& rhs)
  {
    return lhs.IsCoplanar(rhs);
  } // end bool operator==(const Plane&, const Plane&)                        */

  /** Get if 2 planes aren't coplanar: different in either direction or position
  \brief
    - get if 2 planes aren't coplanar: different in either direction or position
  \details
    - if only parallel is desired, compare normals or use IsParallel()
  \param lhs
    - plane for the left hand side of inequality comparison with rhs
  \param rhs
    - plane for the right hand side of inequality comparison with lhs
  \return
    - true only if memberwise near equality is found
  */
  inline bool operator!=(const Plane& lhs, const Plane& rhs)
  {
    return !(lhs == rhs);
  } // end bool operator!=(const Plane&, const Plane&)                        */

  // Ensurance that memory footprint is as intended
  static_assert(sizeof(Plane) == 4*4, "Plane not packed");

   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

} // end dp namespace
