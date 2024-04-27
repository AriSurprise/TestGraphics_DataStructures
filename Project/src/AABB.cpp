/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  AABB.cpp
Purpose:  Axis Aligned Bounding Box context of mesh data spread in world coords
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
#include "AABB.h"     // Class interface declaration header
#include "BSphere.h"  // Bounding sphere <center, radius> for collision tests
#include "Log.h"      // System message management for errors, warnings, etc
#include "Mesh.h"     // Vertex data iteration to scan for appropriate bounds
#include "Plane.h"    // 2D boundary < <norm>, sum >; side output, intersection
#include "Stats.h"    // Sampling struct to iterate over mesh vertex data
#include "Transform.h"// Scaling mesh vertex data to world coordinate scales
#include "v6f.h"      // <min, max> for abstract data I/O in a 6D container

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Setup static unit cube for all AABB to recycle / scale / place in draw calls
const dp::Mesh dp::AABB::BoundingBox = dp::Mesh::Cube(2.0f);

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


dp::AABB::AABB(const Mesh& mesh, const m4f& placement, u4 samples, s2 step,
  u2 first) : projD(Of), pointD(Of)
{
  // if 1 less, may as well scan all verts: moreover, covers 0 underflow too

  // If rotationally invariant to mesh data, shortcut (no sampling required)
  if (placement.IsScalarLinear())
  {
    // Convert model space bounds to store intended world coords
    v3f scale(placement[0][0], placement[1][1], placement[2][2]);
    Set((placement * mesh.Center().XYZW(1.0f)).XYZ(),
      (scale * mesh.Dimensions()));
    return;
  }
  // TODO: check for having only 1 world axis of rotation (simplify scan)
  u4 pTotal = mesh.VertexCount();
  const Mesh::Vertex* p = &mesh.VertexBuffer();
  v3f minP(maxv3f), maxP(minv3f), cur;
  uRng subs(samples, pTotal, first, step);
  // Otherwise perform a Ritter's scan of the mesh point samples requested
  for (; subs.Scanning(); ++subs)
  {
    cur = (placement * p[subs.Index()].pos.XYZW(1.0f)).XYZ();
    SetIfF(minP.x, cur.x, (cur.x < minP.x));
    SetIfF(minP.y, cur.y, (cur.y < minP.y));
    SetIfF(minP.z, cur.z, (cur.z < minP.z));
    SetIfF(maxP.x, cur.x, (cur.x < maxP.x));
    SetIfF(maxP.y, cur.y, (cur.y < maxP.y));
    SetIfF(maxP.z, cur.z, (cur.z < maxP.z));
  }
  Set(((maxP + minP) * AHALF), (maxP - minP) * AHALF, true);

} // end AOBB(const Mesh&, Transform&, Smpl)                                  */

dp::AABB::AABB(f4 xB, f4 yB, f4 zB, f4 xE, f4 yE, f4 zE, Format input)
  : AABB(v3f(xB, yB, zB), v3f(xE, yE, zE), input)
{ ValidateBounds(); } // end AABB(f4, f4, f4, f4, f4, f4, Format)             */

dp::AABB::AABB(const v3f& begin, const v3f& end, Format input)
  : min((input == Format::Extrema? begin : begin-end)),
    max((input == Format::Extrema? end   : begin+end)), projD(Of), pointD(Of)
{ ValidateBounds(); } // end AABB(const v3f&, const v3f&, Format)             */

dp::AABB::AABB(const v6f& source, Format input) : projD(Of), pointD(Of)
{
  v6f sc = input == Format::Ranges? source.InRanges() : source;
  min = sc.min;
  max = sc.max;
  ValidateBounds();

} // end AABB(const v6f&, Format)                                             */

dp::AABB::AABB(const AABB& source)
  : extX(source.extX), extY(source.extY), extZ(source.extZ),
  projD(source.projD), pointD(source.projD), min(source.min), max(source.max)
{
  u4 i = 0;
  for (; i < PLANES; ++i)
  {
    pr[i] = source.pr[i];
    point[i] = source.point[i];
  }
  for (; i < POINTS; ++i) { point[i] = source.point[i]; }
} // end AABB(const AABB&)                                                    */

dp::AABB::AABB(AABB&& result) noexcept
  : extX(std::move(result.extX)), extY(std::move(result.extY)),
  extZ(std::move(result.extZ)), projD(std::move(result.projD)),
  pointD(std::move(result.projD)), min(std::move(result.min)),
  max(std::move(result.max))
{
  u4 i = 0;
  for (; i < PLANES; ++i)
  {
    pr[i] = result.pr[i];
    point[i] = result.point[i];
  }
  for (; i < POINTS; ++i) { point[i] = result.point[i]; }
} // end AABB(AABB&&) noexcept                                                */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

bool dp::AABB::IsIn(const AABB& other) const
{
  return !(max.x < other.min.x || other.max.x < min.x)
      && !(max.y < other.min.y || other.max.y < min.y)
      && !(max.z < other.min.z || other.max.z < min.z);

} // end bool AABB::IsIn(const AABB&) const                                   */

bool dp::AABB::IsIn(const BSphere& ball) const
{
  return IsIn(ball.Center())
    || SideBy(ball.Center()).Distance(ball.Center()) <= ball.Radius();

} // end bool AABB::IsIn(const BSphere&) const                                */


bool dp::AABB::IsIn(const Plane& plane) const
{ return plane.IsIn(*this); } // end bool AABB::IsIn(const Plane&) const      */


dp::Plane dp::AABB::SideBy(const v3f& point) const
{
  // Create normalized reference frame
  v3f mid(Mid()), ext(HalfExt());
  v3f p = (point - mid); // point localized to bounding box as origin
  v3f n(AbsF(p.x), AbsF(p.y), AbsF(p.z)); // localized & scaled to ref. frame
  n *= ext; // normalize that point to a box relative aspect ratio

  // Get the plane of largest contributing axis from normalized reference frame
  return n.x>n.y? n.x>n.z?
    p.x>0.0f? Plane(X_HAT,max.x):Plane(-X_HAT,min.x)// (x > y)> z => (-/+)x
    : p.z>0.0f? Plane(Z_HAT,max.z):Plane(-Z_HAT,min.z)// (z > x)> y => (-/+)z
    : n.y>n.z? p.y>0.0f? (Y_HAT,max.y):(-Y_HAT,min.z)// (y > x)> z => (-/+)y
    : p.z>0.0f? Plane(Z_HAT,max.z) : Plane(-Z_HAT,min.z);//(z > y)> x => (-/+)z

} // end Plane AABB::SideBy(const v3f&) const                                 */


dp::AABB& dp::AABB::Set(const v3f& center, const v3f& dimens, bool spans)
{
  v3f hExt = dimens.Abs().Real();
  if (!spans) { hExt *= AHALF; }
  extX = hExt.x;
  extY = hExt.y;
  extZ = hExt.z;
  mid = center;
  min += mid - hExt;
  max += mid + hExt;
  return *this;

} // end AABB& AABB::Set(const v3f&, const v3f&) const                        */


dp::AABB& dp::AABB::SetExt(const v3f& dimens)
{
  extX = AbsF(RealF(dimens.x));
  extY = AbsF(RealF(dimens.y));
  extZ = AbsF(RealF(dimens.z));
  v3f hExt = HalfExt();
  min += mid - hExt;
  max += mid + hExt;
  return *this;

} // end AABB& AABB::SetPos(const v3f&) const                                 */


dp::AABB& dp::AABB::SetPos(const v3f& center)
{
  v3f hExt = HalfExt();
  mid = center;
  min += Mid() - hExt;
  max += Mid() + hExt;
  return *this;

} // end AABB& AABB::SetPos(const v3f&) const                                 */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::AABB dp::AABB::operator+(const v3f& displacement) const
{
  return (Copy() += displacement);

} // end AABB AABB::operator+(const v3f&) const                               */


dp::AABB& dp::AABB::operator+=(const v3f& displacement)
{
  min += displacement; // extrema shift but scale remains intact
  max += displacement;
  mid += displacement; // midpoint too
  return *this;
} // end AABB AABB::operator+=(const v3f&)                                    */

dp::AABB& dp::AABB::operator+=(const AABB& other)
{
  min.x = MinF(min.x, other.min.x); // Get extrema defined with the 2 AABB
  min.y = MinF(min.y, other.min.y);
  min.z = MinF(min.z, other.min.z);
  max.x = MaxF(max.x, other.max.x);
  max.y = MaxF(max.y, other.max.y);
  max.z = MaxF(max.z, other.max.z);
  UpdateMatrix(); // redefine half-extents & midpoint from new extrema set
  return *this;
} // end AABB& AABB::operator+=(const AABB&)                                  */


dp::AABB& dp::AABB::operator-=(const AABB& dif)
{
  if (dif.min.x <= min.x)
  {
    if (max.x <= dif.max.x) // x:( o< t< t> o> ) => t.x nested in o.x values
    {
      if (dif.min.y <= min.y)
      {
        if (max.y <= dif.max.y) // y:( o< t< t> o> ) => t.xy nested in o.xy
        {
          // z:( o< o> t< t> | o< t< t> o> | t< t> o< o> ) => unchanged
          // z:( t< o< t> o> | t< o< o> t> | o< t< o> t> ) => change < | <> | >
          // t< o< t> o> | t< o< o> t> | t< t> o< o>
          if (min.z < dif.min.z && dif.min.z < max.z)
          { min.z = dif.min.z; }
          if (max.z > dif.max.z && dif.max.z > min.z)
          { max.z = dif.max.z; }
        }
        else if (dif.min.z <= min.z && max.z <= dif.max.z // o.max.y <  t.max.y
          && dif.max.y > min.y) // nested in o.xz & only max.y needs to change
        {
          max.y = dif.max.y;
        }
      }
      else if (dif.min.z <= min.z && max.z <= dif.max.z) // nested in o.xz
      {
        // y:( o< o> t< t> | o< t< t> o> | t< t> o< o> ) => no change in <|>
        // y:( t< o< t> o> | t< o< o> t> | o< t< o> t> ) => change < | <> | > y
        if (dif.min.y < max.y) { min.y = dif.min.y; }
        if (max.y > dif.max.y && dif.max.y > min.y) { max.y = dif.max.y; }
      }
    }
    else if (dif.min.y <= min.y && max.y <= dif.max.y // o.min.x <= t.min.x
          && dif.min.z <= min.z && max.z <= dif.max.z // o.max.x <  t.max.x
          && dif.max.x > min.x) // nested in o.yz & only max needs to change
    {
      max.x = dif.max.x;
    }
  }
  else if (dif.min.y <= min.y && max.y <= dif.max.y // (min.x < o.min.x)
    && dif.min.z <= min.z && max.z <= dif.max.z)  // nested in o.yz
  {
    // x:( o< o> t< t> | o< t< t> o> | t< t> o< o> ) => no change in <|>
    // x:( t< o< t> o> | t< o< o> t> | o< t< o> t> ) => change < | <> | > x
    if (dif.min.x < max.x) { min.x = dif.min.x; }
    if (max.x > dif.max.x && dif.max.x > min.x) { max.x = dif.max.x; }
  }
  UpdateMatrix(); // Recompute mid-point coord & halfExt scalars; correct draw
  return *this;
} // end AABB& AABB::operator-=(const AABB&)                                  */


dp::AABB& dp::AABB::operator*(const v3f& scale)
{ return Copy() *= scale; } // end AABB& operator*=(const v3f&)               */


dp::AABB& dp::AABB::operator*=(const v3f& scale)
{
  // Update extrema to reflace the change in scale from mid-point as origin
  v3f hExt = (HalfExt() * scale.Abs()); // half-extent scalars stay positive
  min = Mid() - hExt;
  max = Mid() + hExt;
  // Update half-extents within the matrix's linear scalar trace elements
  extX = hExt.x;
  extY = hExt.y;
  extZ = hExt.z;
  // (Mid-point remains unchanged by scaling about that local origin)
  return *this;
} // end AABB& operator*=(const v3f&)                                         */


dp::AABB& dp::AABB::operator*=(const AABB& other)
{
  // bounds = scalarOrigin + oldBoundsDifference * dimensionalScalars
  min = other.Mid() - ((min - other.Mid()) * other.HalfExt());
  max = other.Mid() + ((max - other.Mid()) * other.HalfExt());
  UpdateMatrix();
  return *this;
} // end AABB& operator*=(const AABB&)                                        */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

void dp::AABB::ValidateBounds(void)
{
  SwapIfF(min.x, max.x, min.x > max.x);
  SwapIfF(min.y, max.y, min.y > max.y);
  SwapIfF(min.z, max.z, min.z > max.z);
  UpdateMatrix();

} // end void AABB::ValidateBounds(void)                                      */


void dp::AABB::UpdateMatrix(void)
{
  // draw needs m4f => copy from the affine-scale members simplicity
  mid = ((max + min) * AHALF); // also acts as m4f affine translate col
  v3f hExt = (max - min) * AHALF; // half extents
  // Scale stored in matrix trace: not contiguous, but less redundancy
  extX = hExt.x;
  extY = hExt.y;
  extZ = hExt.z;
  // maybe a specialized, trace-major m3f is worth considering...?
  // could try it & profile to see just how slow m3f*m3f calls become...

} // end void AABB::ValidateBounds(void)                                      */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
