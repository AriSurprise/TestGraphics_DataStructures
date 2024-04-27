/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Plane.cpp
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
#include "AABB.h"   // Bounding Box in extrema, for intersection testing
#include "AOBB.h"   // Bounding Box in center & extents, for intersection tests
#include "BSphere.h"// Bounding Sphere, for intersection tests
#include "Log.h"    // System message management for errors, warnings, etc
#include "Plane.h"  // Class declaration header
#include "v4f.h"    // Abstract 4D 4-byte container for I/O stripped of semantic
#include "v6f.h"    // Abstract 6D 4-byte container for I/O stripped of semantic

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Plane::Plane(f4 nX, f4 nY, f4 nZ, f4 sum)
{ Set(nX, nY, nZ, sum); } // end Plane(f4, f4, f4, f4)                        */

dp::Plane::Plane(const v3f& normal, f4 sum)
{ Set(normal, sum); } // end Plane(const v3f&, f4)                            */

dp::Plane::Plane(const v4f& source) { Set(source); } // end Plane(const v4f&) */

dp::Plane::Plane(const Ray& source) { Set(source); } // end Plane(const Ray&) */

dp::Plane::Plane(const v6f& source) { Set(source); } // end Plane(const v46&) */

dp::Plane::Plane(const Plane& source) : n(source.n), s(source.s)
{ } // end Plane(const Plane&)                                                */

dp::Plane::Plane(Plane&& result) noexcept
  : n(std::move(result.n)), s(std::move(result.s))
{ } // end Plane(Plane&&) noexcept                                            */

bool dp::Plane::IsIn(const AABB& box) const
{
if (box.Min() == box.Max()) { return IsIn(box.Min()); }
const v3f* v = box.Vertices();
f4 dSq = DistSq(box.Min(), box.Max()); // r^2 to plane max
f4 vD = v[0].Dot(Normal()); // vertex to plane distance in normal
bool initialSide = vD > Sum(); // if box vertices started "above" plane
if (vD * vD > dSq) { return false; }
for (um i = 1; i < AABB::POINTS; ++i)
{
  vD = v[i].Dot(Normal());
  if (vD > Sum() != initialSide) { return true; }
  if (vD * vD > dSq) { return false; }
}
return false;

} // end bool Plane::In(const AABB&) const                                    */

dp::v3f dp::Plane::WorldCoords(const v2f& localPoint, const v3f& localX) const
{
  v3f lY = (!NearF(localX.LengthSquared(), 0.0f) ? !localX.Colinear(n) ?
    localX.Cross(n) : !X_HAT.Colinear(n) ? X_HAT.Cross(n) : Y_HAT.Cross(n)
    : !X_HAT.Colinear(n) ? X_HAT.Cross(n) : Y_HAT.Cross(n));
  v3f lX = n.Cross(lY).UnitVec();
  lY.Normalize();
  v3f lOrigin = n * s; // <n> being unit vector, sum =: distance to plane
  return lOrigin;

} // end Plane& Plane::Sum(const v3f&)                                        */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

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
