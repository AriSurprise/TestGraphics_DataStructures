/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  BSphere.cpp
Purpose:  4D Convex bounding volume for containing a mesh by centroid and radius
Details:  member footprint kept in line with v4f use of <x,y,z,w=radius> pattern
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
#include "AABB.h"     // Bounding Box, for I/O conversions / comparisons
#include "BSphere.h"  // Class declaration header
#include "Log.h"      // System message management for errors, warnings, etc
#include "m3f.h"      // 3D matrix data: PCA method, 3D covariance matrix, etc.
#include "Mesh.h"     // Vertex data iteration to scan for appropriate bounds
#include "Plane.h"    // 2D boundary < <norm>, sum >; side output, intersection
#include "Stats.h"    // Sampling struct to iterate over mesh vertex data
#include "Transform.h"// Scaling mesh vertex data to world coordinate scales
#include "v4f.h"      // 4D 4-byte container of data without semantic for I/O

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Setup static unit ball for all BSphere to recycle/scale/place in draw calls
const dp::Mesh dp::BSphere::bBall = dp::Mesh::pSphere(16u, 12u, 1.0f);

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


dp::BSphere::BSphere(const Mesh& mesh, const m4f& placement, EPOS method,
  u4 samples, s2 step, u2 first)
{
  *this = LarsonSphere(mesh, placement, samples, step, first, method);
}


dp::BSphere::BSphere(const Mesh& mesh, const m4f& placement, u4 samples,
  s2 step, u2 first)
{
  m3f basis;
  v3f dimens;
  *this = PCASphere(mesh, placement, samples, step, first, &basis, &dimens);
}

dp::BSphere::BSphere(f4 x, f4 y, f4 z, f4 radius)
  : wfb(AbsF(radius), x, y, z)
{ } // end BSphere(f4, f4, f4, f4)                                            */

dp::BSphere::BSphere(const v3f& center, f4 radius)
  : wfb(AbsF(radius), center.x, center.y, center.z)
{ } // end BSphere(const v3f&, f4)                                            */

dp::BSphere::BSphere(const v4f& source)
  : wfb(AbsF(source.w), source.x, source.y, source.z)
{ } // end BSphere(const v4f&)                                                */

dp::BSphere::BSphere(const AABB& source)
  : BSphere(source.Mid(), source.HalfExt().Length())
{ } // end BSphere(const AABB&)                                               */

dp::BSphere::BSphere(const BSphere& source) : wfb(source.wfb)
{ } // end BSphere(const BSphere&)                                            */

dp::BSphere::BSphere(BSphere&& result) noexcept : wfb(result.wfb)
{ } // end BSphere(BSphere&&) noexcept                                        */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::BSphere& dp::BSphere::ExpandedTo(const std::vector<v3f>& wPosVerts)
{
  v3f dist; // Center to point distance
  f4 d;     // distance from center (current), to point
  f4 r;     // new radius (when exceeding)

  // Scan each data point for exceeding the given radius from scan
  for (u4 i = 0; i < wPosVerts.size(); ++i)
  {
    // Check the distance from the center to the point against the radius
    dist = DistV3F(Center(), wPosVerts[i]);
    d = dist.LengthSquared();
    r = RadiusSquared();
    if (r < d && 0.0f < d) // Exceeding points grow the sphere to include it
    {
      // Convert dist to unit vector
      d = SqrtF(d);
      dist /= d;
      r = (Radius() + d) * AHALF; // Calculate new radius
      *this += dist * (r - Radius()); // Move center by rad difference
      Radius(r); // Update to use new radius
    }
  }
  return *this;

} // end BSphere& BSphere::ExpandTo(const std::vector<v3f>&)                  */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  constexpr u4 SIZE1 = 3; // Number of sqrt(1) length vectors defined =: 3
  // Unit length v3fs, directionally distinct in value combinations with 0+0+1
  const dp::v3f LEN_R1[SIZE1] = { dp::X_HAT, dp::Y_HAT, dp::Z_HAT };

  constexpr u4 CUMULATIVE_111 = SIZE1; // Axes defined over 001 sets
  constexpr u4 SIZE3 = 4; // Number of sqrt(3) length vectors defined =: 4
  // Length sqrt(3) v3f directionally distinct in signed combinations of 1+1+1
  const dp::v3f LEN_R3[SIZE3] =
  { dp::v3f(1, 1, 1), dp::v3f(1, 1,-1),
    dp::v3f(1,-1, 1), dp::v3f(1,-1,-1)};

  constexpr u4 CUMULATIVE_011 = CUMULATIVE_111+SIZE3; // Axes in 001 - 111 sets
  constexpr u4 SIZE2 = 6; // Number of sqrt(2) length vectors defined =: 6
  // Length sqrt(2) v3f directionally distinct in signed combinations of 0+1+1
  const dp::v3f LEN_R2[SIZE2] =
  { dp::v3f(1, 1, 0), dp::v3f(1, 0, 1), dp::v3f(0, 1, 1),
    dp::v3f(1,-1, 0), dp::v3f(1, 0,-1), dp::v3f(0, 1,-1) };

  constexpr u4 CUMULATIVE_012 = CUMULATIVE_011+SIZE2; // Axes in 001 - 011 sets
  constexpr u4 SIZE5 = 12; // Number of sqrt(5) length vectors defined =: 12
  // Length sqrt(5) v3f directionally distinct in signed combinations of 0+1+2
  const dp::v3f LEN_R5[SIZE5] =
  { dp::v3f(2, 1, 0), dp::v3f(2, 0, 1), dp::v3f(0, 2, 1),
    dp::v3f(2,-1, 0), dp::v3f(2, 0,-1), dp::v3f(0, 2,-1),
    dp::v3f(1, 2, 0), dp::v3f(1, 0, 2), dp::v3f(0, 1, 2),
    dp::v3f(1,-2, 0), dp::v3f(1, 0,-2), dp::v3f(0, 1,-2) };

  constexpr u4 CUMULATIVE_112 = CUMULATIVE_012+SIZE5; // Axes in 001 - 012 sets
  constexpr u4 SIZE6 = 12; // Number of sqrt(6) length vectors defined =: 12
  // Length sqrt(6) v3f directionally distinct in signed combinations of 1+1+2
  const dp::v3f LEN_R6[SIZE6] =
  { dp::v3f(2, 1, 1), dp::v3f(1, 2, 1), dp::v3f(1, 1, 2),
    dp::v3f(2, 1,-1), dp::v3f(1, 2,-1), dp::v3f(1, 1,-2),
    dp::v3f(2,-1, 1), dp::v3f(1,-2, 1), dp::v3f(1,-1, 2),
    dp::v3f(2,-1,-1), dp::v3f(1,-2,-1), dp::v3f(1,-1,-2) };

  constexpr u4 CUMULATIVE_122 = CUMULATIVE_112+SIZE6; // Axes in 001 - 112 sets
  constexpr u4 SIZE9 = 12; // Number of sqrt(9) length vectors defined =: 12
  // Length sqrt(9) v3f directionally distinct in signed combinations of 1+2+2
  const dp::v3f LEN_R9[SIZE9] =
  { dp::v3f(2, 2, 1), dp::v3f(2, 1, 2), dp::v3f(1, 2, 2),
    dp::v3f(2, 2,-1), dp::v3f(2, 1,-2), dp::v3f(1, 2,-2),
    dp::v3f(2,-2, 1), dp::v3f(2,-1, 2), dp::v3f(1,-2, 2),
    dp::v3f(2,-2,-1), dp::v3f(2,-1,-2), dp::v3f(1,-2,-2) };
  constexpr u4 CUMULATIVE_VEC = CUMULATIVE_122+SIZE9; // Axes in 001 - 122 sets


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::BSphere dp::RitterSphere(const Mesh& mesh, const m4f& placement,
  u4 samples, s2 step, u2 first)
{
  return LarsonSphere(mesh, placement, samples, step, first, EPOS::_6);

} // end BSphere RitterSphere(const Mesh&, const m4f&, u4, s2, u2)            */


dp::BSphere dp::LarsonSphere(const Mesh& mesh, const m4f& placement,
  u4 samples, s2 step, u2 first, EPOS count)
{
  uRng subs(samples, mesh.VertexCount(), first, step);
  u4 axisIndex; // Record of which axis maximized spread (not used...yet...)
  u4 tMin = 0, tMax = 0; // Record of subscripts of total maximized spread
  u4 verts = mesh.VertexCount(); // Total vertices in mesh
  v2f axis(INF, -INF);  // point spread on the given axis
  u4 aMin = 0, aMax = 0; // extremal subscripts along axis of spread
  f4 range = -INF; // largest point spread of any axis' spread
  f4 t; // Axis projection scalar
  const Mesh::Vertex* data = &mesh.VertexBuffer(); // Mesh vertex list
  std::vector<v3f> wPos(mesh.VertexCount());

  // Get world coords of each mesh vertex
  for (u4 i = 0; i < wPos.size(); ++i) // Obligate full list over samples...?
  {
    // Doing full list & not just samples saves work over 2nd, growth stage
    wPos[i] = (placement*data[i].pos.XYZW(1.0f)).XYZ();
    ++subs;
  } // Expand will take 1 iteration, & needs to be comprehensive, not sampled
  subs.SetMax(static_cast<u4>(wPos.size()));
  // Scaling all data once, now, lets data pass to that: only sampled use here

  // Find axis of maximal spread of all sampled world coords by axis projection
  switch (count)
  {
  case EPOS::_98:
    if (2*CUMULATIVE_VEC <= verts) for (u4 a = 0; a < SIZE9; ++a) // Per 122...
    {
      // Reset from prior axis iterations
      axis.Set(INF, -INF);
      subs.Reset();
      // Get point range of spread on this axis
      while (subs.Scanning()) // Project each point onto the axis
      {
        t = wPos[subs.Index()].Dot(LEN_R9[a]); // Point-axis projection scalar
        if (t < axis.min) { axis.min = t; aMin = subs.Index(); } // Note minima
        if (axis.max < t) { axis.max = t; aMax = subs.Index(); } // Note maxima
        ++subs;
      }
      t = (axis.max - axis.min) * ATHIRD; // Get comparable range of spread
      if (range < t) // Record if exceeding prev spread
      {
        range = t; tMin = aMin; tMax = aMax; axisIndex = CUMULATIVE_122 + a;
      }
    } // pass-through
  case EPOS::_74:
    if (2*CUMULATIVE_122 <= verts) for (u4 a = 0; a < SIZE6; ++a) // Per 112...
    {
      // Reset from prior axis iterations
      axis.Set(INF, -INF);
      subs.Reset();
      // Get point range of spread on this axis
      while (subs.Scanning()) // Project each point onto the axis
      {
        t = wPos[subs.Index()].Dot(LEN_R6[a]); // Point-axis projection scalar
        if (t < axis.min) { axis.min = t; aMin = subs.Index(); } // Note minima
        if (axis.max < t) { axis.max = t; aMax = subs.Index(); } // Note maxima
        ++subs;
      }
      t = (axis.max - axis.min) * _ROOT6; // Get comparable range of spread
      if (range < t) // Record if exceeding prev spread
      {
        range = t; tMin = aMin; tMax = aMax; axisIndex = CUMULATIVE_112 + a;
      }
    } // pass-through
  case EPOS::_50:
    if (2*CUMULATIVE_112 <= verts) for (u4 a = 0; a < SIZE5; ++a) // Per 012...
    {
      // Reset from prior axis iterations
      axis.Set(INF, -INF);
      subs.Reset();
      // Get point range of spread on this axis
      while (subs.Scanning()) // Project each point onto the axis
      {
        t = wPos[subs.Index()].Dot(LEN_R5[a]); // Point-axis projection scalar
        if (t < axis.min) { axis.min = t; aMin = subs.Index(); } // Note minima
        if (axis.max < t) { axis.max = t; aMax = subs.Index(); } // Note maxima
        ++subs;
      }
      t = (axis.max - axis.min) * _ROOT5; // Get comparable range of spread
      if (range < t) // Record if exceeding prev spread
      {
        range = t; tMin = aMin; tMax = aMax; axisIndex = CUMULATIVE_012 + a;
      }
    } // pass-through
  case EPOS::_26:
    if (2*CUMULATIVE_012 <= verts) for (u4 a = 0; a < SIZE2; ++a) // Per 011...
    {
      // Reset from prior axis iterations
      axis.Set(INF, -INF);
      subs.Reset();
      // Get point range of spread on this axis
      while (subs.Scanning()) // Project each point onto the axis
      {
        t = wPos[subs.Index()].Dot(LEN_R2[a]); // Point-axis projection scalar
        if (t < axis.min) { axis.min = t; aMin = subs.Index(); } // Note minima
        if (axis.max < t) { axis.max = t; aMax = subs.Index(); } // Note maxima
        ++subs;
      }
      t = (axis.max - axis.min) * _ROOT2; // Get comparable range of spread
      if (range < t) // Record if exceeding prev spread
      {
        range = t; tMin = aMin; tMax = aMax; axisIndex = CUMULATIVE_011 + a;
      }
    } // pass-through
  case EPOS::_14:
    if (2*CUMULATIVE_011 <= verts) for (u4 a = 0; a < SIZE3; ++a) // Per 111...
    {
      // Reset from prior axis iterations
      axis.Set(INF, -INF);
      subs.Reset();
      // Get point range of spread on this axis
      while (subs.Scanning()) // Project each point onto the axis
      {
        t = wPos[subs.Index()].Dot(LEN_R3[a]); // Point-axis projection scalar
        if (t < axis.min) { axis.min = t; aMin = subs.Index(); } // Note minima
        if (axis.max < t) { axis.max = t; aMax = subs.Index(); } // Note maxima
        ++subs;
      }
      t = (axis.max - axis.min) * _ROOT3; // Get comparable range of spread
      if (range < t) // Record if exceeding prev spread
      {
        range = t; tMin = aMin; tMax = aMax; axisIndex = CUMULATIVE_111 + a;
      }
    } // pass-through
  case EPOS::_6:
    for (u4 a = 0; a < SIZE1; ++a) // For each 001...
    {
      // Reset from prior axis iterations
      axis.Set(INF, -INF);
      subs.Reset();
      // Get point range of spread on this axis
      while (subs.Scanning()) // Project each point onto the axis
      {
        t = wPos[subs.Index()].Dot(LEN_R1[a]); // Point-axis projection scalar
        if (t < axis.min) { axis.min = t; aMin = subs.Index(); } // Note minima
        if (axis.max < t) { axis.max = t; aMax = subs.Index(); } // Note maxima
        ++subs;
      }
      t = (axis.max - axis.min); // Get range of spread
      if (range < t) // Record if exceeding prev spread
      {
        range = t; tMin = aMin; tMax = aMax; axisIndex = a;
      }
    }
  }
  BSphere result((wPos[tMax] + wPos[tMin])* AHALF, range * AHALF);
  return result.ExpandedTo(wPos);

} // end BSphere LarsonSphere(const Mesh&, Transform&, u4, s2, u2, EPOS)      */


dp::BSphere dp::PCASphere(const Mesh& mesh, const m4f& placement, u4 samples,
  s2 step, u2 first, m3f* eigenVectors, v3f* eigenValues)
{
  uRng s(samples, (u4)mesh.VertexCount(), first, step);
  const Mesh::Vertex* data = &mesh.VertexBuffer();
  std::vector<v3f> wPos(mesh.VertexCount()); // World coordinates of mesh data
  for (u4 i = 0; i < mesh.VertexCount(); ++i)
  { wPos[i] = placement.PProd(data[i].pos); }
  // Sampled world coordinate dimensional covariance matrix to diagonalize
  m3f A(Covariance3D(&wPos[0], s.Dataset(), s.Samples(), s.Step(), s.First()));
  m3f E; // Eigen-vector set in column vectors
  m3f& V = eigenVectors? *eigenVectors : E; // Orthonormal eigen-vectors basis
  v3f L; // Eigen-values correlating to column vectors
  v3f& S = eigenValues? *eigenValues : L; // Magnitude of eigen-vectors spread

  // (Iteratively perform basis rotations to minimize covariance)
  V = PCA3(A, &S);// Get eigen-vector basis; pass in to get eigen-value scalars

  // Find magnitude of spread, along major, eigen-vector axis of data spread
  u4 m = MaxB3F(S.x, S.y, S.z); // [0,2]: major axis associated => max spread
  v2f dataS(INf, -INf); // Record of extrema projections of data spread on axis
  f4 t; // Scalar value of projected point primary axis (unit-vec?), of spread
  bool subceded, superceded; // Projected point record of extremal highs / lows
  u4 min = 0, max = 0; // Indices of points of extremal projection
  // Project all points on that axis to find extrema
  for (u4 i = 0; i < wPos.size(); ++i)
  {
    t = V[m].Dot(wPos[i]);
    subceded = t < dataS.min;
    superceded = dataS.max < t;
    dataS.min = IfOrF(t, dataS.min, subceded);
    dataS.max = IfOrF(t, dataS.max, superceded);
    SetIfU(min, i, subceded);
    SetIfU(max, i, superceded);
  }

  BSphere result((wPos[max]+wPos[min])*AHALF, (dataS.max-dataS.min)*AHALF);
  return result.ExpandedTo(wPos);

} // end BSphere PCASphere(const Mesh&, const m4f&, u4, s2, u2, m3f*, v3f*)   */
