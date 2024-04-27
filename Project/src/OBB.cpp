/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  OBB.cpp
Purpose:  Oriented Bounding Box (a.k.a. 3-DOP), fit to mesh's world coordinates
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_CS350_2
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
#include "AABB.h"     // World scaled & aligned box for collision tests
#include "BSphere.h"  // World scaled uniform ball for collision tests
#include "Log.h"      // System message management for errors, warnings, etc
#include "Mesh.h"     // Vertex data scanning to determine extents / orientation
#include "OBB.h"      // Class interface declaration header
#include "v2f.h"      // Class interface declaration header

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::OBB::OBB(const Mesh& mesh, const m4f& place, u4 samples, s2 step, u2 first)
{
  // Get list of vertex world coordinates to sample
  const Mesh::Vertex* v = &mesh.VertexBuffer();
  u4 verts = mesh.VertexCount();
  std::vector<v3f> wPos(verts);
  for (u4 i = 0; i < verts; ++i) { wPos[i] = place.PProd(v[i].pos); }
  // Calculate rotation & spread by sampled statistical analysis
  basis = PCA3(Covariance3D(&wPos[0], verts, samples, step, first), &ext);
  Log::Diag("OBB - Extents?" + ext.String());
  (ext *= AHALF).ToAbs();
  Log::Diag("OBB - Extents?" + ext.String());
  // Get maxima of scalar local extents
  u4 m = MaxB3F(ext.x, ext.y, ext.z); // [0,2]: max spread => major axis
  v2f spread(INf, -INf); // Record of extrema projections of data spread on axis
  f4 t; // Scalar value of projected point primary axis (unit-vec?), of spread
  bool subceded, superceded; // Projected point record of extremal highs / lows
  u4 min = 0, max = 0; // Indices of points of extremal projection
  // Project all points on that axis to find extrema
  for (u4 i = 0; i < wPos.size(); ++i)
  {
    t = basis[m].Dot(wPos[i]);
    subceded = t < spread.min;
    superceded = spread.max < t;
    SetIfF(spread.min, t, subceded);
    SetIfF(spread.max, t, superceded);
    SetIfU(min, i, subceded);
    SetIfU(max, i, superceded);
  }
  center = (wPos[max] + wPos[min]) * AHALF;
  ext[m] = (spread.max - spread.min) * AHALF; // use pop. result over samp.
  radius = ext[m];
  Log::Diag("OBB - Extents?" + ext.String());
  // Set initial state for derived data
  UpdateSurface();
  UpdateVolume();
  UpdateVert();
  UpdateProj();
  UpdateMatrix();

} // end OBB::OBB(const Mesh&, const m4f&, u4, s2, u2)                        */


dp::OBB::OBB(f4 mdX, f4 mdY, f4 mdZ, f4 scX, f4 scY, f4 scZ, bool halvedExt,
             f4 fwX, f4 fwY, f4 fwZ, f4 upX, f4 upY, f4 upZ, PLANE formed)
  : OBB(v3f(mdX,mdY,mdZ), v3f(scX,scY,scZ), halvedExt,
        v3f(fwX,fwY,fwZ), v3f(upX,upY,upZ), formed)
{}//end OBB::OBB(f4, f4, f4, f4, f4, f4, bool, f4, f4, f4, f4, f4, f4, PLANE) */

dp::OBB::OBB(const v3f& pos, const v3f& extents, bool halvedExt,
             const v3f& facing, const v3f& tilt, PLANE formed)
  : center(pos), ext(halvedExt? extents : extents*AHALF),
    basis(OrthonormalBasis(facing, tilt, PLANE::ZY))
{
  // Set initial state for derived data
  ext.ToAbs();
  radius = MaxF(MaxF(ext.x, ext.y), ext.z);
  UpdateSurface();
  UpdateVolume();
  UpdateVert();
  UpdateProj();
  UpdateMatrix();

} //end OBB::OBB(const v3f&, const v3f&, bool, const v3f&, const v3f&, PLANE) */

dp::OBB::OBB(const OBB& source)
  : wfb(source.wfb), basis(source.basis), prX(source.prX), prY(source.prY),
  prZ(source.prZ), center(source.center), ext(source.ext),
  surface(source.surface), volume(source.volume), radius(source.radius)
{ } // end OBB::OBB(const OBB& source)                                        */


dp::OBB::OBB(OBB&& result) noexcept
  : wfb(std::move(result.wfb)), prX(std::move(result.prX)),
  prY(std::move(result.prY)), prZ(std::move(result.prZ)), 
  basis(std::move(result.basis)), center(std::move(result.center)),
  ext(std::move(result.ext)), surface(std::move(result.surface)),
  volume(std::move(result.volume)), radius(std::move(result.radius))
{ } // end OBB::OBB(OBB&&) noexcept                                           */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

bool dp::OBB::IsIn(const AABB& other) const
{
  // Update self-axis projections as needed
  UpdateProj();
  return false;
}

bool dp::OBB::IsIn(const OBB& other) const
{
  // Update self-axis projections as needed (a given)
  UpdateProj();
  other.UpdateProj();
  // Project OTHER's center & extents ON THIS box's basis
  const v3f* p = nullptr;
  if (!other.ext.IsUniform()) // Non-uniform box will lose symmetrical range
  {
    other.UpdateVert();
    p = &other.point[0];
  }
  // Use separating axis theorem of OTHER ON THIS to check for intersection
  if (!(other.center.Dot(LocalX()) + other.ProjSpan(LocalX(), p)).IsIn(prX)
   || !(other.center.Dot(LocalY()) + other.ProjSpan(LocalY(), p)).IsIn(prY)
   || !(other.center.Dot(LocalZ()) + other.ProjSpan(LocalZ(), p)).IsIn(prZ))
  { return false; }

  if (ext.IsUniform()) { p = nullptr; } // ext already in abs: suffices
  else
  {
    UpdateVert();
    p = &point[0];
  }
 
  // Use separating axis theorem of THIS ON OTHER to check for intersection
  return
    (!(center.Dot(other.LocalX())+ProjSpan(other.LocalX(),p)).IsIn(other.prX)
  || !(center.Dot(other.LocalY())+ProjSpan(other.LocalY(),p)).IsIn(other.prY)
  || !(center.Dot(other.LocalZ())+ProjSpan(other.LocalZ(),p)).IsIn(other.prZ));
  
} // end bool OBB::IsIn(OBB&)                                                 */


dp::OBB& dp::OBB::Set(const v3f& center, const v3f& extents, bool halvedExt,
  const v3f& facing, const v3f& tilt, PLANE formed)
{
  this->center = center;
  ext = extents;
  if (!halvedExt) { ext *= AHALF; }
  basis = OrthonormalBasis(facing, tilt, PLANE::ZY);
  SetDirtyScale();
  SetDirtyProj();
  return *this;

} //end OBB& OBB::Set(const v3f&,const v3f&,bool,const v3f&,const v3f&,PLANE) */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::OBB& dp::OBB::operator*=(const AABB& other)
{
  // Get extrema (ignore rotation: not relevant to scaling operation)
  v3f min(center - ext);
  v3f max(center + ext);
  // Find new extrema =: extremal span split at (mid), scaled by (ext) at (mid)
  min = other.Mid() - ((min - other.Mid()) * other.HalfExt());
  max = other.Mid() + ((max - other.Mid()) * other.HalfExt());
  // Reinterpret midpoint & half-extents from new extrema
  center = (max + min) * AHALF;
  ext = (max - min) * AHALF;
  // Set dirty flags for scale affected members requiring recaluclation
  SetDirtyScale();
  SetDirtyProj();
  return *this;

} // end OBB& OBB::operator*=(const AABB&)                                    */


dp::OBB& dp::OBB::operator*=(const OBB& other)
{
  // Get extrema (ignore rotation: not relevant to scaling operation)
  v3f min(center - ext);
  v3f max(center + ext);
  // Find new extrema =: extremal span split at (mid), scaled by (ext) at (mid)
  min = other.Mid() - ((min - other.Mid()) * other.HalfExt());
  max = other.Mid() + ((max - other.Mid()) * other.HalfExt());
  // Reinterpret midpoint & half-extents from new extrema
  center = (max + min) * AHALF;
  ext = (max - min) * AHALF;
  // Update basis too
  basis *= other.basis;
  // Set dirty flags for scale affected members requiring recaluclation
  SetDirtyScale();
  SetDirtyProj();
  return *this;

} // end OBB& OBB::operator*=(const OBB&)                                     */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::v2f dp::OBB::ProjSpan(v3f axis, const v3f* bVert) const
{
  // Find (normalized for external comparability), span potential of local space
  axis.Normalize(); // Validate input magnitude (hence non-const)
  if (bVert) // Implying point list of comparison is not symmetrical in range
  {
    // Needing context for pointwise projection of each
    UpdateVert(); // Update the point list as needed
    f4 current;
    v2f range(INf, -INf);
    for (u4 i = 0; i < 8; ++i) // Project each point's world scale on the axis
    {
      current = axis.Dot(point[i]);
      // Record extremal results
      SetIfF(range.min, current, current < range.min);
      SetIfF(range.max, current, range.max < current);
    }
    return range;
  }
  // Test of each local extremal directions' contribution to this given axis
  v3f range(LocalX().Dot(axis), LocalY().Dot(axis), LocalZ().Dot(axis));
  range *= ext; // scaled memberwise in those extremal directions' magnitudes
  range.ToAbs(); // -> magnitudes (simpler; half-extents spread symmetrically)

  // Find the most contributing direction from the given magnitudes
  f4 max = MaxF(MaxF(range.x, range.y), range.z);
  return v2f(-max, max); // return that symmetrical maximal offset

} // end v2f OBB::PrUSpan(v3f axis) const                                     */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
