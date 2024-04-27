/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Triangle.h
Purpose:  Container for triangle bounding information, using unit vectors
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
#include "Plane.h"  // Inline conversions (data very similar already)
#include "v3f.h"    // Defining members of Tri: 3 points of <x,y,z> values


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  class AABB; // Bounding box with <min, max>; intersection tests
  class AOBB; // Bounding box with <centroid, extents>; intersection tests
  class BSphere;// Bounding sphere <center, radius>, for collision tests
  class Plane;  // Boundary <normal, sum>; test intersection (bool, dist, point)

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  //! Triangle: 
  class Triangle
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a Triangle from a set of 3 world coords / displacement vectors
    \brief
      - create a Triangle from a set of 3 world coords / displacement vectors
    \details
      - if a, b & c colinear, b += uP to prevent degenerate triangles
    \param a
      - world space coord representing the 1st triangle point
    \param b
      - world space coord representing the 2nd triangle point
    \param c
      - world space coord representing the 3rd triangle point
    */
    Triangle(const v3f& a = nullv3f, const v3f& b = X_HAT,
      const v3f& c = Y_HAT);

    /** Create a Triangle from a 9 float point values of 3 sets of <x,y,z> data
    \brief
      - create a Triangle from a 9 float point values of 3 sets of <x,y,z> data
    \details
      - if a, b & c colinear, b += uP to prevent degenerate triangles
    \param x1
      - world space coord representing x-axis component of 1st triangle point
    \param y1
      - world space coord representing y-axis component of 1st triangle point
    \param z1
      - world space coord representing z-axis component of 1st triangle point
    \param x2
      - world space coord representing x-axis component of 2nd triangle point
    \param y2
      - world space coord representing y-axis component of 2nd triangle point
    \param z2
      - world space coord representing z-axis component of 2nd triangle point
    \param x3
      - world space coord representing x-axis component of 3rd triangle point
    \param y3
      - world space coord representing y-axis component of 3rd triangle point
    \param z3
      - world space coord representing z-axis component of 3rd triangle point
    */
    Triangle(f4 x1, f4 y1, f4 z1, f4 x2, f4 y2, f4 z2, f4 x3, f4 y3, f4 z3);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Methods                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Access triangle's 1st point's cartesian 3D world coordinates
    \brief
      - access triangle's 1st point's cartesian 3D world coordinates
    \return
      - reference to contained <x,y,z> world coordinate of triangle's 1st point
    */
    inline const v3f& A(void) const
    { return Point1(); } // end const v3f& Triangle::A(void) const            */


    /** Get triangle's upscaled edge[1] distance(A, B) vector in world coords
    \brief
      - get triangle's upscaled edge[1] distance(A, B) vector in world coords
    \return
      - world scale triangle point displacement from p[1]:(A) to p[2]:(B)
    */
    inline v3f AB(void) const
    { return Edge1(); } // end v3f Triangle::AB(void) const                   */


    /** Get triangle's upscaled -edge[3] distance(A, C) vector in world coords
    \brief
      - get triangle's upscaled -edge[3] distance(A, C) vector in world coords
    \return
      - world scale triangle point displacement from p[1]:(A) to p[3]:(C)
    */
    inline v3f AC(void) const
    { return -Edge3(); } // end v3f Triangle::AC(void) const                   */


    /** Get plane containing any barycentric combination of triangle points
    \brief
      - get plane containing any barycentric combination of triangle points
    \return
      - plane with triangle normal, using a point from the triangle for sum
    */
    inline Plane AnyP(void) const
    { return ContainingPlane(); } // end Plane Triangle::AnyP(void) const     */


    /** Get triangular, world scale area of this triangle's current settings
    \brief
      - get triangular, world scale area of this triangle's current settings
    \return
      - world scale 2D area demarcated within this triangle points
    */
    inline f4 Area(void) const
    {
      // TriArea => (2*TriArea / 2) => (ParalleloArea / 2) =>
      // (|AB|*|AC|*sin(t)) / 2 => ||ABxAC|| / 2 => n=: <e[0]> half length
      return l[0];

    } // end f4 Triangle::Area(void) const                                    */


    /** Access triangle's 2nd point's cartesian 3D world coordinates
    \brief
      - access triangle's 2nd point's cartesian 3D world coordinates
    \return
      - world coordinate of triangle's 2nd point
    */
    inline v3f B(void) const
    { return Point2(); } // end v3f Triangle::B(void) const                   */


    /** Get barycentric coordinates of a given point relative to this triangle
    \brief
      - get barycentric coordinates of a given point relative to this triangle
    \return
      - barycentric coords of point in <u=:x,v=:y,gamma=:z> of tri point ratios
    */
    v3f Barycentrics(const v3f& point) const;


    /** Get triangle's upscaled -edge[1] distance(B, A) vector in world coords
    \brief
      - get triangle's upscaled -edge[1] distance(B, A) vector in world coords
    \return
      - world scale triangle point displacement from p[2]:(B) to p[1]:(A)
    */
    inline v3f BA(void) const
    { return -Edge1(); } // end v3f Triangle::BA(void) const                  */


    /** Get triangle's upscaled edge[2] distance(B, C) vector in world coords
    \brief
      - get triangle's upscaled edge[2] distance(B, C) vector in world coords
    \return
      - world scale triangle point displacement from p[2]:(B) to p[3]:(C)
    */
    inline v3f BC(void) const
    { return Edge2(); } // end v3f Triangle::BC(void) const                   */


    /** Access triangle's 3rd point's cartesian 3D world coordinates
    \brief
      - access triangle's 3rd point's cartesian 3D world coordinates
    \return
      - world coordinate of triangle's 3rd point
    */
    inline v3f C(void) const
    { return Point3(); } // end v3f Triangle::C(void) const                   */


    /** Get triangle's upscaled edge[3] distance(C, A) vector in world scale
    \brief
      - get triangle's upscaled edge[3] distance(C, A) vector in world scale
    \return
      - world scale triangle point displacement from p[3]:(C) to p[1]:(A)
    */
    inline v3f CA(void) const
    { return Edge3(); } // end v3f Triangle::CA(void) const                  */


    /** Get triangle's upscaled -edge[2] distance(C, B) vector in world scale
    \brief
      - get triangle's upscaled -edge[2] distance(C, B) vector in world scale
    \return
      - world scale triangle point displacement from p[3]:(C) to p[2]:(B)
    */
    inline v3f CB(void) const
    { return -Edge2(); } // end v3f Triangle::CB(void) const                  */


    /** Get triangle's edge[0] <normal> at (<AB>.Cross(<AC>)/2) half world scale
    \brief
      - get triangle's edge[0] <normal> at (<AB>.Cross(<AC>)/2) half world scale
    \return
      - world scale displacement from triangle origin to 1st axis' unit point
    */
    inline v3f E(u4 n) const
    { return Edge(n); } // end v3f Triangle::E(u4) const                      */


    /** Get triangle's edge[0] <normal> at (<AB>.Cross(<AC>)/2) half world scale
    \brief
      - get triangle's edge[0] <normal> at (<AB>.Cross(<AC>)/2) half world scale
    \return
      - world scale displacement from triangle origin to 1st axis' unit point
    */
    inline v3f E0(void) const
    { return Edge0(); } // end v3f Triangle::E0(void) const                   */


    /** Get triangle's edge[1] distance(A, B) vector world scale
    \brief
      - get triangle's edge[1] distance(A, B) vector world scale
    \return
      - world scale triangle point displacement from p[1]:(A) to p[2]:(B)
    */
    inline v3f E1(void) const
    { return Edge1(); } // end v3f Triangle::E1(void) const                   */


    /** Get triangle's edge distance(B, C) vector world scale
    \brief
      - get triangle's edge distance(B, C) vector world scale
    \return
      - world scale triangle point displacement from p[2]:(B) to p[3]:(C)
    */
    inline v3f E2(void) const
    { return Edge2(); } // end v3f Triangle::E2(void) const                   */


    /** Get triangle's edge distance(C, A) vector world scale
    \brief
      - get triangle's edge distance(C, A) vector world scale
    \return
      - world scale triangle point displacement from p[3]:(C) to p[1]:(A)
    */
    inline v3f E3(void) const
    { return Edge3(); } // end v3f Triangle::E3(void) const                   */


    /** Get triangle's edge[0] <normal> at (<AB>.Cross(<AC>)/2) half world scale
    \brief
      - get triangle's edge[0] <normal> at (<AB>.Cross(<AC>)/2) half world scale
    \return
      - world scale displacement from triangle origin to 1st axis' unit point
    */
    inline v3f Edge(u4 n) const
    { return e[(n=InU(n,3))] * l[n]; } // end v3f Triangle::Edge(u4) const */


    /** Get triangle's edge[0] <normal> at (<AB>.Cross(<AC>)/2) half world scale
    \brief
      - get triangle's edge[0] <normal> at (<AB>.Cross(<AC>)/2) half world scale
    \return
      - world scale displacement from triangle origin to 1st axis' unit point
    */
    inline v3f Edge0(void) const
    { return e[0] * l[0]; } // end v3f Triangle::Edge0(void) const            */


    /** Get triangle's edge[1] distance(A, B) vector world scale
    \brief
      - get triangle's edge[1] distance(A, B) vector world scale
    \return
      - world scale triangle point displacement from p[1]:(A) to p[2]:(B)
    */
    inline v3f Edge1(void) const
    { return e[1] * l[1]; } // end v3f Triangle::Edge1(void) const            */


    /** Get triangle's edge distance(B, C) vector world scale
    \brief
      - get triangle's edge distance(B, C) vector world scale
    \return
      - world scale triangle point displacement from p[2]:(B) to p[3]:(C)
    */
    inline v3f Edge2(void) const
    { return e[2] * l[2]; } // end v3f Triangle::Edge2(void) const            */


    /** Get triangle's edge distance(C, A) vector world scale
    \brief
      - get triangle's edge distance(C, A) vector world scale
    \return
      - world scale triangle point displacement from p[3]:(C) to p[1]:(A)
    */
    inline v3f Edge3(void) const
    { return e[3] * l[3]; } // end v3f Triangle::Edge3(void) const            */


    /** Get whether a bounding box is inside this triangle's extents
    \brief
      - get whether a bounding box is inside this triangle's extents
    \param box
      - bounding box to be evaluated against this for boolean intersection
    \return
      - true if any triangle extrema (point or edge length) is inside the box
    */
    bool In(const AABB& box) const;


    /** Get whether a bounding box is inside this triangle's extents
    \brief
      - get whether a bounding box is inside this triangle's extents
    \param box
      - bounding box to be evaluated against this for boolean intersection
    \return
      - true if any triangle extrema (point or edge length) is inside the box
    */
    bool In(const AOBB& box) const;


    /** Get whether a bounding sphere is inside this triangle's extents
    \brief
      - get whether a bounding sphere is inside this triangle's extents
    \param ball
      - bounding box to be evaluated against this for boolean intersection
    \return
      - true if any triangle extrema (point or edge length) is inside the ball
    */
    bool In(const BSphere& ball) const;


    /** Get whether a plane is inside this triangle's extents
    \brief
      - get whether a plane is inside this triangle's extents
    \param plane
      - boundary to be evaluated against this triangle for boolean intersection
    \return
      - true if any tri extrema are on opposite plane side from each other
    */
    inline bool In(const Plane& plane) const
    {
      f4 dist = plane.Distance(p[1]);
      if (AbsF(dist) > 2.0f * l[4]) { return false; }
      bool initialSide = (dist > 0);
      if ((plane.Distance(p[2]) > 0) != initialSide) { return true; }
      return (plane.Distance(p[3]) > 0) != initialSide;

    } // end bool In(const Plane&) const                                      */


    /** Get whether a ray is intersecting with this triangle's extents
    \brief
      - get whether a ray is intersecting with this triangle's extents
    \param ray
      - point shot in a direction to evaluate for intersection against this tri
    \return
      - true if ray intersects plane in [0,1] of all barycentric coordinates
    */
    bool In(const Ray& ray) const;


    /** Get whether another triangle is intersecting with this triangle
    \brief
      - get whether another triangle is intersecting with this triangle
    \details
      - triangle edges are extremal, not just vertices
    \param other
      - triangle to evaluate for intersection against this triangle
    \return
      - true if any of other's extrema intersect this triangle within its plane
    */
    bool In(const Triangle& other) const;


    /** Get whether a point is within the triangle's plane and extents
    \brief
      - get whether a point is within the triangle's plane and extents
    \param point
      - world space coordinate to be evaluated for intersection with triangle
    \param proximity
      - tolerance of point planarity: half of triangle's intended prism height
    \return
      - true only if point is near the triangle plane and between edge extents
    */
    inline bool In(const v3f& point, f4 proximity = _INf) const
    {
      return ContainingPlane().IsIn(point, proximity) && BarycentricIn(point);

    } // end bool Triangle::In(const v3f&, f4) const                          */


    /** Get whether the triangle points describe an acute triangle
    \brief
      - get whether the triangle points describe an acute triangle
    \return
      - true only if no edge unit-vec pair's dotProd <= 0,  =>  pi/2 <= acos(t)
    */
    inline bool IsAcute(void) const
    { return !IsObtuse(); } // end bool Triangle::IsAcute(void) const         */


    /** Get whether the triangle points align to the angle of a given plane
    \brief
      - get whether the triangle points align to the angle of a given plane
    \param plane
      - plane whose orientation will be tested against this triangle's plane
    \return
      - true if triangle unit-vec normal is equal to the plane's unit-vec normal
    */
    inline bool IsCoplanar(const Plane& plane) const
    {
      return ContainingPlane() == plane;

    } // end bool Triangle::IsCoplanar(const Plane&) const                    */


    /** Get whether the triangle aligns to the same facing angle of another
    \brief
      - get whether the triangle aligns to the same facing angle of another
    \param tri
      - triangle whose orientation will be tested against this triangle's
    \return
      - true if both unit-vec normals are equal to to one another
    */
    inline bool IsCoplanar(const Triangle& tri) const
    {
      return NearF(NSum(), tri.NSum()) && IsParallel(tri);

    } // end bool Triangle::IsCoplanar(const Triangle&) const                 */


    /** Get whether the triangle points align to the angle of a given plane
    \brief
      - get whether the triangle points align to the angle of a given plane
    \param plane
      - plane whose orientation will be tested against this triangle's plane
    \return
      - true if triangle unit-vec normal is equal to the plane's unit-vec normal
    */
    inline bool IsParallel(const Plane& plane) const
    {
      return Normal() == plane.Normal();

    } // end bool Triangle::IsParallel(const Plane&) const                    */


    /** Get whether the triangle aligns to the same facing angle of another
    \brief
      - get whether the triangle aligns to the same facing angle of another
    \param tri
      - triangle whose orientation will be tested against this triangle's
    \return
      - true if both unit-vec normals are equal to to one another
    */
    inline bool IsParallel(const Triangle& tri) const
    {
      return Normal() == tri.Normal();

    } // end bool Triangle::IsParallel(const Triangle&) const                 */


    /** Get whether the triangle points describe a right triangle
    \brief
      - get whether the triangle points describe a right triangle
    \return
      - true if any edge unit-vec pair's dot product < 0  =>  pi/2 < acos(t)
    */
    inline bool IsRight(void) const
    {
      // (acos(^<AB>*^<AC>) || acos(^<AB>*^<BC>)|| acos(^<CA>*^<BC>)) == pi/2
      return NearF(e[1].Dot(e[2]), 0.0f)
          || NearF(e[1].Dot(e[3]), 0.0f)
          || NearF(e[2].Dot(e[3]), 0.0f);

    } // end bool Triangle::IsObtuse(void) const                              */


    /** Get whether the triangle points describe an obtuse triangle
    \brief
      - get whether the triangle points describe an obtuse triangle
    \return
      - true if any edge unit-vec pair's dot product < 0  =>  pi/2 < acos(t)
    */
    inline bool IsObtuse(void) const
    {
      // ( acos(^<AB>*^<AC>) || acos(^<BA>*^<BC>)|| acos(^<CB>*^<CA>) ) > pi/2
      return e[1].Dot(e[2]) < 0.0f
         || -e[1].Dot(e[3]) < 0.0f
         || -e[2].Dot(-e[3]) < 0.0f;

    } // end bool Triangle::IsObtuse(void) const                              */


    /** Access unit vector of triangle plane normal (perpendicular direction)
    \brief
      - access unit vector of triangle plane normal (perpendicular direction)
    \return
      - reference to unit vector perpendicular the plane of this triangle
    */
    inline const v3f& N(void) const
    { return Normal(); } // end const v3f& Triangle::N(void) const            */


    /** Access unit vector of triangle plane normal (perpendicular direction)
    \brief
      - access unit vector of triangle plane normal (perpendicular direction)
    \return
      - reference to unit vector perpendicular the plane of this triangle
    */
    inline const v3f& Norm(void) const
    { return Normal(); } // end const v3f& Triangle::Norm(void) const         */


    /** Access unit vector of triangle plane normal (perpendicular direction)
    \brief
      - access unit vector of triangle plane normal (perpendicular direction)
    \return
      - reference to unit vector perpendicular the plane of this triangle
    */
    inline const v3f& Normal(void) const
    { return e[0]; } // end const v3f& Triangle::Normal(void) const           */


    /** Get orthogonal, signed distance to triangle plane, along its normal
    \brief
      - get orthogonal, signed distance to triangle plane, along its normal
    \return
      - reference to the distance to plane of this triangle, from the origin
    */
    inline const f4& NSum(void) const
    { return l[5]; } // end const f4& Triangle::NSum(void) const              */


    /** Get plane containing any barycentric combination of triangle points
    \brief
      - get plane containing any barycentric combination of triangle points
    \param n
      - subscript for point of the triangle: 0:(centroid), 1:(A), 2:(B), 3:(C)
    \return
      - plane with triangle normal, using a point from the triangle for sum
    */
    inline Plane P(u4 n) const
    { return Point(n); } // end Plane Triangle::P(u4) const                   */


    /** Access triangle's centroid point's cartesian 3D world coordinates
    \brief
      - access triangle's centroid point's cartesian 3D world coordinates
    \return
      - reference to contained <x,y,z> world coordinate of triangle's centroid
    */
    inline const v3f& P0(void) const
    { return Point0(); } // end const v3f& Triangle::P0(void) const           */

    /** Access triangle's 1st point's cartesian 3D world coordinates
    \brief
      - access triangle's 1st point's cartesian 3D world coordinates
    \return
      - reference to contained <x,y,z> world coordinate of triangle's 1st point
    */
    inline const v3f& P1(void) const
    { return Point1(); } // end const v3f& Triangle::P1(void) const           */

    /** Get triangle's 2nd point's cartesian 3D world coordinates
    \brief
      - get triangle's 2nd point's cartesian 3D world coordinates
    \return
      - reference to contained <x,y,z> world coordinate of triangle's 2nd point
    */
    inline v3f P2(void) const
    { return Point2(); } // end const v3f& Triangle::P2(void) const           */

    /** Get triangle's 3rd point's cartesian 3D world coordinates
    \brief
      - get triangle's 3rd point's cartesian 3D world coordinates
    \return
      - reference to contained <x,y,z> world coordinate of triangle's 3rd point
    */
    inline v3f P3(void) const
    { return Point3(); } // end const v3f& Triangle::P3(void) const           */


    /** Access triangle's 1st point's cartesian 3D world coordinates
    \brief
      - access triangle's 1st point's cartesian 3D world coordinates
    \param n
      - subscript for point of the triangle: 0:(centroid), 1:(A), 2:(B), 3:(C)
    \return
      - reference to contained <x,y,z> world coordinate of triangle's PointN
    */
    inline const v3f& Point(u4 n) const
    { return p[InU(n, 3)]; } // end const v3f& Triangle::Point(u4) const   */


    /** Access triangle's centroid point's cartesian 3D world coordinates
    \brief
      - access triangle's centroid point's cartesian 3D world coordinates
    \return
      - reference to contained <x,y,z> world coordinate of triangle's centroid
    */
    inline const v3f& Point0(void) const
    { return p[0]; } // end const v3f& Triangle::Point1(void) const           */


    /** Access triangle's 1st point's cartesian 3D world coordinates
    \brief
      - access triangle's 1st point's cartesian 3D world coordinates
    \return
      - reference to contained <x,y,z> world coordinate of triangle's 1st point
    */
    inline const v3f& Point1(void) const
    { return p[1]; } // end const v3f& Triangle::Point1(void) const           */


    /** Get triangle's 2nd point's cartesian 3D world coordinates
    \brief
      - get triangle's 2nd point's cartesian 3D world coordinates
    \return
      - reference to contained <x,y,z> world coordinate of triangle's 2nd point
    */
    inline v3f Point2(void) const
    { return p[2]; } // end const v3f& Triangle::Point2(void) const           */


    /** Get triangle's 3rd point's cartesian 3D world coordinates
    \brief
      - get triangle's 3rd point's cartesian 3D world coordinates
    \return
      - reference to contained <x,y,z> world coordinate of triangle's 3rd point
    */
    inline v3f Point3(void) const
    { return p[3]; } // end const v3f& Triangle::Point3(void) const           */


    /** Get plane containing any barycentric combination of triangle's points
    \brief
      - get plane containing any barycentric combination of triangle's points
    \return
      - plane with triangle normal, using a point from the triangle for sum
    */
    inline Plane ContainingPlane(void) const
    {
      return Plane(e[0], l[5]);

    } // end Plane Triangle::ContainingPlane(void) const                      */


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Operator Overloads                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Methods                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get if barycentric coords of point (in plane) in triangle [0,1] bounds
    \brief
      - get if barycentric coords of point (in plane) in triangle [0,1] bounds
    \param point
      - point already known to be in triangle plane, to be tested with bounds
    \return
      - true if the barycentric values of point are each in [0,1] and sum to 1
    */
    bool BarycentricIn(const v3f& point) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Triangle's world coordinates of points < (centroid), (A), (B), (C) >
    v3f p[4];

    //! Triangle's edge unit vectors directions in < <n>, <AB>, <AC>, <BC> >
    v3f e[4];

    //! Stored lengths <|n|/2, |AB|, |AC|, |BC|, |centroidRadius|, nDot(p[n]) >
    v6f l;

  }; // end Triangle class declaration

  // Assurance that memory footprint is without padding bytes, as intended
  static_assert(sizeof(Triangle) == 120, "Triangle size not packed");

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

} // end dp namespace
