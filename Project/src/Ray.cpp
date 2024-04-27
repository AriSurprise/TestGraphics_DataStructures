/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Ray.cpp
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
#include "AABB.h"   // Bounding box in <min, max> extrema; test intersection
#include "AOBB.h"   // Bounding box in <center, halfExtent>; test intersection
#include "BSphere.h"// Bounding sphere <origin, radius>; test intersection
#include "Log.h"    // System message management for errors, warnings, etc
#include "Plane.h"  // 2D Boundary <n, s>; test intersection (dist, point, etc)
#include "Ray.h"    // Class interface declaration header
#include "v6f.h"    // <min, max> capable of data abstract I/O in 6D container

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Ray::Ray(f4 pX, f4 pY, f4 pZ, f4 dX, f4 dY, f4 dZ)
  : orig(pX, pY, pZ), dir(dX, dY, dZ)
{
  dir.Normalize();

} // end Ray(f4, f4, f4, f4, f4, f4)                                          */

dp::Ray::Ray(const v3f& origin, const v3f& direction)
  : orig(origin), dir(direction.UnitVec()), _dir(dir.Reciprocals())
{ } // end Ray(const v3f&, const v3f&)                                        */


dp::Ray::Ray(const AABB& source)
  : orig(source.Mid()), dir(source.HalfExt().UnitVec()),
  _dir(dir.Reciprocals())
{ } // end Ray(const AABB&)                                                   */

dp::Ray::Ray(const Plane& source, const v2f& point)
  : orig(source.WorldCoords(point, X_HAT)), dir(source.Normal()),
  _dir(dir.Reciprocals())
{ } // end Ray(const Plane&, const v2f&)                                      */

dp::Ray::Ray(const v6f& source, bool inRanges)
  : Ray(AABB(source, inRanges? AABB::Format::Ranges : AABB::Format::Extrema))
{ } // end Ray(const v6f&, bool)                                              */

dp::Ray::Ray(const Ray& source)
  : orig(source.orig), dir(source.dir), _dir(source._dir)
{ } // end Ray(const Ray&)                                                    */

dp::Ray::Ray(Ray&& result) noexcept : orig(std::move(result.orig)),
  dir(std::move(result.dir)), _dir(std::move(result._dir))
{ } // end Ray(Ray&&) noexcept                                                */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

f4 dp::Ray::Distance(const Plane& plane) const
{
  f4 pn = plane.Sum(); // point*norm: scalar expression of point on plane in n
  f4 on = plane.Normal().Dot(orig); // orig*norm in plane (origin's plane in n)
  // check if ray originates in plane (within +/-INf of plane)
  if (NearF(on, pn, _INf)) { return 0.0f; } // early out => origin in plane
  f4 dn = plane.Normal().Dot(dir); // dir*norm; both unit vectors => cos(theta)
  // substitute ray in plane to solve for t: (<o> + <d>*t)&(<n><x,y,z> = <n><p>)
  // => <n>(<o> + <d>*t) = n*p  =>  n*o + n*d*t = n*p  =>  t = (n*p-n*o) / n*d
  return (dn == 0.0f) ? (pn - on) / dn : -INf;
  // : (parallel: distance infinite, < 0 to lump results w/ planes behind ray)

} // end f4 Ray::Distance(const Plane&) const                                 */


bool dp::Ray::IsIn(const AABB& box, f4* tDist) const
{
  // Check for easy out: ray originates / starts in box
  if (box.IsIn(orig))
  { if (tDist) { *tDist = 0.0f; } return true; } // In box => 0 distance
  // Check if ray dir is parallel with any world axes? (=> INf slope span)
  if (dir.x == 0.0f || dir.y == 0.0f || dir.z == 0.0f)
  {
    // => Can't travel in dir.x|y|z to reach box (unless starting in box)
    if (tDist) { *tDist = INf; } // (confirmed not the case above already)
    return false;
  }

  // Get t-scalars intersecting box-extrema, per axis (from x -> y -> z)
  v2f t; // t-scalar < min, max > range of intersection for checked axes
  v2f a; // t-scalar <min,max> range of axis being compared against <t>

  t.Set(box.Min().x, box.Max().x); // Use ray dir.x for extrema order
  SwapIfF(t.min, t.max, std::signbit(dir.x));// => (-)desc : (+)asc x-values
  // either - => from right, moving in -x(<) or + => from left, moving in +x(>)
  
  t -= orig.x; // set to x-axis world distance to <enter, exit> from ray origin
  t *= _dir.x; // scale (1 / dir.x) uniformly; solve for t (parallel dist in x)

  // Get comparison with y-axis t-scalars intersecting box
  a.Set(box.Min().y, box.Max().y); // Use ray dir.y for extrema order
  SwapIfF(a.min, a.max, std::signbit(dir.y)); // => (-)desc : (+)asc y-values
  // either (from top, in -y(v), downward : from bottom, in +y(^), upward)

  a -= orig.y; // set to y-axis world distance to <enter, exit> from ray origin
  a *= _dir.y; // scale (1 / dir.y) uniformly; solve for t (parallel dist in y)

  // If t-scalar of x's entry is beyond y's exit, or y's entry after x's exit
  if ((t.min > a.max) || (a.min > t.max)) // => t-skew in X:Y; no intersection
  { if (tDist) { *tDist = INf; } return false; }
  t.Set(MaxF(a.min, t.min), MinF(t.max, a.max)); // Intersect X : Y t-scalars

  a.Set(box.Min().z, box.Max().z); // Use ray dir.z for extrema order
  SwapIfF(a.min, a.max, std::signbit(dir.z)); // => (-)desc : (+)asc z-values
  // either (from front, in -z, backward : from back, in +z, forward)

  a -= orig.z; // set to z-axis world distance to <enter, exit> from ray origin
  a *= _dir.z; // scale (1 / dir.z) uniformly; solve for t (parallel dist in z)

  // If t-scalar of xy's entry is beyond z's exit, or z's entry after xy's exit
  if ((t.min > a.max) || (a.min > t.max)) // => t-skew in XY:Z; no intersection
  { if (tDist) { *tDist = INf; } return false; }
  t.Set(MaxF(a.min, t.min), MinF(t.max, a.max)); // Intersect XY:Z t-scalars

  if (tDist) { *tDist = t.min; } // Minimal distance must be first intersection
  return true;

} // end bool Ray::IsIn(const AABB&, f4*) const                               */


bool dp::Ray::IsIn(const BSphere& ball, f4* tDist) const
{
  v3f dist = DistV3F(ball.Center(), orig);
  f4 a = 1; // =: ray.dirDot(ray.dir) => squared length; unit vec ray.dir = 1
  f4 b = 2.0f * dir.Dot(dist);
  f4 c = dist.Dot(dist) - ball.RadSq();
  v2f t;
  // If (skew in euclidean space) || (ball behind ray origin) => false
  if (!QuadraticF(a, b, c, t) || (t.x < 0.0f && t.y < 0.0f))
  {
    if (tDist) { *tDist = INf; }
    return false;
  }
  if (ball.IsIn(orig)) { if (tDist) { *tDist = 0.0f; } }
  else if (tDist) { *tDist = (t.x < 0 ? t.y : t.x); }
  return true;
} // end bool Ray::IsIn(const BSphere&, f4*) const                            */

bool dp::Ray::IsIn(const Plane& plane, v3f* intersection) const
{
  f4 dist = Distance(plane);
  if (intersection) { intersection->Set(dist != -INf ? At(dist) : maxv3f); }
  return 0.0f <= dist;

} // end bool Ray::IsIn(const Plane&) const                                     */

bool dp::Ray::IsParallel(const Plane& plane) const
{
  return NearF(plane.Normal().Dot(dir), 0.0f);

} // end bool Ray::IsParallel(const Plane&) const                             */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
