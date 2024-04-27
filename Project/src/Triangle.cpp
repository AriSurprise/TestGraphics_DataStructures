/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Triangle.cpp
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
#include "AABB.h"     // bounding box in < <min>, <max> >; collision tests
#include "AOBB.h"     // bounding box in < <mid>, <ext> >; collision tests
#include "BSphere.h"  // bounding sphere in < <norm>, sum >; collision tests
#include "Plane.h"  // bounding sphere in < <norm>, sum >; collision tests
#include "Triangle.h" // Class interface declaration header
#include "Log.h"      // System message management for errors, warnings, etc

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Triangle::Triangle(const v3f& a, const v3f& b, const v3f& c)
{
  p[1] = a.Real(); // (A)
  p[2] = b.Real(); // (B)
  p[3] = c.Real(); // (C)
  e[1] = DistV3F(p[1], p[2]); // 1:(A)->2:(B) = <AB>
  e[3] = DistV3F(p[1], p[3]); // 1:(A)->3:(C) = <AC> (to be -<AC> = <CA> later)
  // Validate u & v direction vectors are non-null (use +x, +y if needed)
  if (e[1] == nullv3f) { e[1] = X_HAT; }
  if (e[3] == nullv3f) { e[3] = Y_HAT; }
  // Validate directions are mutually unique (triangle is not degenerate)
  if (e[1].Colinear(e[3]))
  {
    // Correct with whatever world axis offset works to create triangle area
    e[3] += e[1].Colinear(X_HAT)? Y_HAT : X_HAT;
    p[3] = p[1] + e[3];
  }
  // Calculate & normalize non-point vectors (store their scalar lengths)
  e[0] = e[1].Cross(e[3]).Normalize(&l[0]); // e[0] =: ^<[1][2]>x<[1][3]> =: n
  l[0] *= AHALF; // use (n/2); (parallelogram == 2*tri) area <AB>x<AC> property 
  e[1].Normalize(&l[1]);
  e[2] = DistV3F(p[2], p[3]).Normalize(&l[2]);
  !e[3].Normalize(&l[3]); // post <AB>x<AC>, ready to invert <AC> -> <CA>
  l[5] = e[0].Dot(p[1]); // l[5] is holding origin to plane orthogonal distance

  // Check if sphere of fit volumetrically wasteful contingent to being obtuse
  if (IsObtuse())
  {
    p[0] = (p[1] + p[2] + p[3]) * ATHIRD;
    l[4] = SqrtF(MaxF(MaxF(DistSq(p[0], p[1]),
                          DistSq(p[0], p[2])),
                          DistSq(p[0], p[3])));
  }
  else // TODO: RREF to solve for sphere of fit
  {
    p[0] = (p[1] + p[2] + p[3]) * ATHIRD;
    l[4] = SqrtF(MaxF(MaxF(DistSq(p[0], p[1]),
                           DistSq(p[0], p[2])),
                           DistSq(p[0], p[3])));
  }

} // end Triangle(const v3f&, const v3f&, const v3f&, bool)                   */

dp::Triangle::Triangle(f4 x1, f4 y1, f4 z1, f4 x2, f4 y2, f4 z2, f4 x3, f4 y3,
  f4 z3)
  : Triangle(v3f(x1,y1,z1), v3f(x2,y2,z2), v3f(x3,y3,z3))
{ } // end Triangle(f4, f4, f4, f4, f4, f4, f4, f4, f4)                       */


/*dp::Triangle::Triangle(const Triangle& source)
  : p{ source.p[0], source.p[1], source.p[2], source.p[3] },
    e{ source.e[0], source.e[1], source.e[2], source.e[3] },
    l{ source.l[0], source.l[1], source.l[2], source.l[3], l[4] }
{ } // end Triangle(const Triangle&)                                          */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::v3f dp::Triangle::Barycentrics(const v3f& point) const
{
  v3f bary; // barycentric coords u=:x, v=:y, gamma=:z; 
  v3f area = AB().Cross(AC()); // Squared parallelogram area suffices mostly
  // a^2/2 / b^2/2 == a^2 / b^2 == (a/b)^2,    (a/b)^2 != a/b at end

  // Find ABP : ABC area ratio
  v3f PV = DistV3F(A(), point); // Distance from input point to triangle vertex
  v3f subarea = AB().Cross(PV);
  bary.x = area.Dot(subarea);

  // Find BCP : ABC area ratio
  PV = DistV3F(B(), point);
  subarea = BC().Cross(PV);
  bary.y = area.Dot(subarea);

  // Find CAP : ABC area ratio
  PV = DistV3F(C(), point);
  subarea = CA().Cross(PV);
  bary.z = area.Dot(subarea);

  // Normalize
  return  (bary /= area.LengthSquared()).Sqrts();

} // end v3f Triangle::Barycentrics(const v3f&) const                         */


bool dp::Triangle::In(const AABB& box) const
{
  f4 t; // Edge-Ray scalar result for box intersection
  return BSphere(p[0], l[4]).IsIn(box) // If box at least in radius of tri
    // Either simply having an extremal point inside the box (hopefully simple)
    && (box.IsIn(p[1]) || box.IsIn(p[2]) || box.IsIn(p[3])
    // Or (if necessary): having any [0,edge[#]length] t-scalar intersection with box
      || (Ray(p[1], e[1]).IsIn(box, &t) && IsInF(t, l[1], 0.0f))
      || (Ray(p[2], e[2]).IsIn(box, &t) && IsInF(t, l[2], 0.0f))
      || (Ray(p[3], e[3]).IsIn(box, &t) && IsInF(t, l[3], 0.0f)));

} // end bool Triangle::In(const AABB&) const                                 */


bool dp::Triangle::In(const BSphere& ball) const
{
  f4 t;
  return BSphere(p[0], l[4]).IsIn(ball) // If in radius of ball (radius) and
    // Either simply having an extremal point inside the ball (simpler)
    && (ball.IsIn(p[1]) || ball.IsIn(p[2]) || ball.IsIn(p[3])
    // Or (if necessary): having any [0,edge] length edge-ray ball intersection
    || (Ray(p[1], e[1]).IsIn(ball, &t) && IsInF(t, l[1], 0.0f))
    || (Ray(p[2], e[2]).IsIn(ball, &t) && IsInF(t, l[2], 0.0f))
    || (Ray(p[3], e[3]).IsIn(ball, &t) && IsInF(t, l[3], 0.0f)));

} // end bool Triangle::In(const BSphere&) const                              */


bool dp::Triangle::In(const Ray& ray) const
{
  v3f p;
  return ray.IsIn(ContainingPlane(), &p) && BarycentricIn(p);

} // end bool Triangle::In(const Ray&) const                                  */

bool dp::Triangle::In(const Triangle& other) const
{
  f4 radii = l[4] + other.l[4]; // Chech centroid/centroid distance
  if (!(DistSq(p[0], other.p[0]) < radii * radii)) { return false; }

  // If any edge length is inside other tri's plane, some part may intersect
  Plane oPlane = other.ContainingPlane();
  Ray A_B(A(), e[1]); // Infinite edge-ray from (A) through (B) (at scalar l[1])
  Ray B_C(B(), e[2]); // Infinite edge-ray from (B) through (C) (at scalar l[2])
  Ray C_A(C(), e[3]); // Infinite edge-ray from (C) through (A) (at scalar l[3])
  // Per edge (as rays), check their finite lengths against other's plane
  // Then if so, that edge-ray falls within other's subregion of that plane
  return (IsInF(A_B.Distance(oPlane), l[1], 0.0f) && other.In(A_B))
      || (IsInF(B_C.Distance(oPlane), l[2], 0.0f) && other.In(B_C))
      || (IsInF(C_A.Distance(oPlane), l[3], 0.0f) && other.In(C_A));
} // end bool Triangle::In(const Triangle&) const                             */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

bool dp::Triangle::BarycentricIn(const v3f& point) const
{
  //v3f b; // Barycentric coords u=:x, v=:y, gamma=:z; 
  v3f area = AB().Cross(AC()); // Squared parallelogram area suffices; cancels
  // Check ABP : ABC area ratio
  v3f PV = DistV3F(A(), point); // Distance from point =: P, to (A)->(B)->(C)
  v3f subarea = AB().Cross(PV);
  if ((/*b.x=*/area.Dot(subarea)) < 0) { return false; } // P right of <AB>

  // Check BCP : ABC area ratio
  PV = DistV3F(B(), point);
  subarea = BC().Cross(PV);
  if ((/*b.y=*/area.Dot(subarea)) < 0) { return false; } // P right of <BC>

  // Check CAP : ABC area ratio
  PV = DistV3F(C(), point);
  subarea = CA().Cross(PV);
  return ((/*b.z=*/0.0f <= area.Dot(subarea))); // P ? left : right of <CA>

  // (could capture b & normalize too, /area.DotSelf(); but not w/ early outs)

} // end bool Triangle::InBarycentric(const v3f&) const                       */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
