/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  m4f.cpp
Purpose:  2D 4x4, column major matrix; inputs / outpus in row major
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_CS350_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

#include "Value.h"  // Basic numerical operations: clamp, swap, min, max, etc
#include "m3f.h"    // Associated class declaration header
#include "m4f.h"    // Associated class declaration header
#include "v2f.h"    // 2D data for doubly promoted m4f & [min, max] ranges
#include "v3f.h"    // 3D point / vector data for homogeneous matrices
#include "Log.h"    // Error / warning output message / logging

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::m4f::m4f(f4 r1c1, f4 r1c2, f4 r1c3, f4 r1c4,
             f4 r2c1, f4 r2c2, f4 r2c3, f4 r2c4,
             f4 r3c1, f4 r3c2, f4 r3c3, f4 r3c4,
             f4 r4c1, f4 r4c2, f4 r4c3, f4 r4c4, bool colMajor)
  : col{v4f(r1c1, r2c1, r3c1, r4c1),
        v4f(r1c2, r2c2, r3c2, r4c2),
        v4f(r1c3, r2c3, r3c3, r4c3),
        v4f(r1c4, r2c4, r3c4, r4c4)}
{
  if (colMajor) { Transpose(); }
} // end m4f(f4,f4,f4,f4, f4,f4,f4,f4, f4,f4,f4,f4, f4,f4,f4,(f4),(bool)) raw */


dp::m4f::m4f(f4 ux, f4 vx, f4 nx, f4 tx, f4 uy, f4 vy, f4 ny, f4 ty,
             f4 uz, f4 vz, f4 nz, f4 tz,
             f4 affine, f4 skew)
  : m4f(ux,vx,nx,tx, uy,vy,ny,ty, uz,vz,nz,tz, skew,skew,skew, affine, false)
{} // end m4f(f4,f4,f4,f4, f4,f4,f4,f4, f4,f4,f4,f4, f4, f4) affine 3D        */


dp::m4f::m4f(f4 ux, f4 vx, f4 nx, f4 uy, f4 vy, f4 ny, f4 uz, f4 vz, f4 nz,
  f4 affine, f4 skew)
  : m4f(ux,vx,nx,skew, uy,vy,ny,skew, uz,vz,nz,skew, skew,skew,skew,affine,
    false)
{ } // end m4f(f4,f4,f4, f4,f4,f4, f4,f4,f4, f4,f4) linear 3D constructor     */


dp::m4f::m4f(f8 ux, f8 vx, f8 nx, f8 uy, f8 vy, f8 ny, f8 uz, f8 vz, f8 nz,
  f8 affine, f8 skew)
{
  Set(ux,vx,nx,uy, vy,ny,uz,vz, nz, affine, skew);
} // end m4f(f8, f8, f8, f8, f8, f8, f8, f8, f8, (f8), (f8)) lazy linear 3D   */


dp::m4f::m4f(f4 ux, f4 vx, f4 tx, f4 uy, f4 vy, f4 ty, PLANE plane, f4 tz,
  f4 normal, f4 affine, f4 off, f4 skew)
{
  Set(ux,vx,tx, uy,vy,ty, plane, tz, normal, affine, off, skew);
} // end m4f(f4,f4,f4, f4,f4,f4, PLANE, (f4),(f4),(f4),(f4),(f4)) affine 2D   */


dp::m4f::m4f(f4 sx, f4 tx, f4 sy, f4 ty, f4 sz, f4 tz, f4 affine, f4 skew)
  : m4f( sx,skew,skew, tx,
       skew,  sy,skew, ty,
       skew,skew,  sz, tz,
       skew,skew,skew, affine)
{ } // end m4f(f4, f4, f4, f4, f4, f4, (f4), (f4)) 3D Scale-Translation       */


dp::m4f::m4f(f4 ux, f4 vx, f4 uy, f4 vy, PLANE plane, f4 tz, f4 normal,
  f4 affine, f4 off, f4 skew)
  : m4f(ux, vx, off,
        uy, vy, off,
        plane,   tz, normal,
        affine, off, skew)
{ } // end m4f(f4, f4, f4, f4, PLANE, (f4), (f4), (f4), (f4), (f4)) Linear 2D */


dp::m4f::m4f(f4 scale, f4 tx, f4 ty, f4 tz)
  : m4f(scale, Of,   Of, tx,
         Of,scale,   Of, ty,
         Of,   Of,scale, tz,
         Of,   Of,   Of, If)
{ } // end m4f(f4, f4, f4, f4) Uniform 3D Scale-Translate                     */


dp::m4f::m4f(f4 sx, f4 sy, f4 sz)
  : m4f(sx, Of, Of, Of,
        Of, sy, Of, Of,
        Of, Of, sz, Of,
        Of, Of, Of, If)
{ } // end m4f(f4, f4, f4) Non-Uniform Scalar                                 */


dp::m4f::m4f(f4 linear, f4 affine)
  : m4f(linear, Of, Of, Of,
        Of, linear, Of, Of,
        Of, Of, linear, Of,
        Of, Of, Of, affine)
{ } // end m4f((f4), (f4)) Uniform                                            */


dp::m4f::m4f(const v2f& U, const v2f& V, PLANE plane, const v2f& T,
  f4 tz, f4 normal, f4 affine, f4 off, f4 skew)
{
  // Process axis and column major preferences to delegate to base setter
  switch (plane)
  {
  case PLANE::ZX: // about y axis on the xz plane for x-down 2D world space
    Set(m4f(U.x,   off,-V.x, T.x,
            off,normal, off,  tz,
           -U.y,   off, V.y, T.y,
           skew,  skew,skew, affine));
    break;
  case PLANE::XZ: // about y axis on the xz plane for x-up 2D world space
    Set(m4f(U.x,  off,  V.x, T.x,
            off,normal, off,  tz,
            U.y,  off,  V.y, T.y,
           skew, skew, skew, affine));
    break;
  case PLANE::ZY: // about x axis on the yz plane for z-down 2D world space
    Set(m4f(normal, off, off,  tz,
               off, U.x,-V.x, T.x,
               off,-U.y, V.y, T.y,
              skew,skew,skew, affine));
    break;
  case PLANE::YZ: // about x axis on the yz plane for z-up 2D world space
    Set(m4f(normal, off, off,  tz,
               off, U.x, V.x, T.x,
               off, U.y, V.y, T.y,
              skew,skew,skew, affine));
    break;
  case PLANE::YX: // about z axis on the xy plane for y-down 2D world space
    Set(m4f(U.x, V.x,   off, T.x,
            U.y, V.y,   off, T.y,
            off, off,normal,  tz,
           skew,skew,  skew, affine));
    break;
  default: // about z axis on the xy plane for y-up 2D world space
    Set(m4f(U.x, V.x,   off, T.x,
            U.y, V.y,   off, T.y,
            off, off,normal,  tz,
           skew,skew,  skew, affine));
    break;
  } // end axis switch statement
} // end m4f(const v2f&, const v2f&, PLANE, const v2f&, f4, f4, f4, f4, f4)   */


dp::m4f::m4f(const v3f& U, const v3f& V, const v3f& N, const v3f& T, f4 affine,
  f4 skew) : m4f(U.XYZW(skew), V.XYZW(skew), N.XYZW(skew), T.XYZW(affine))
{ } // end m4f(const v3f&, const v3f&, const v3f&, const v3f&, f4, f4)        */


dp::m4f::m4f(const v4f& U, const v4f& V, const v4f& N, const v4f& T)
  : col{ U, V, N, T }
{ } // end m4f(const v4f&, const v4f&, const v4f&, const v4f&)                */


dp::m4f::m4f(const m3f& linear, const v3f& translate, f4 affine, f4 skew)
  : m4f(linear[0][0], linear[1][0], linear[2][0], skew, // no initial translate
        linear[0][1], linear[1][1], linear[2][1], skew, // use linear values to
        linear[0][2], linear[1][2], linear[2][2], skew, // make an affine T col
              skew,         skew,         skew, affine)
{ if (translate != nullv3f) { *this *= TranslateAffine(translate); } }
// end m4f(const m3f&, const v3f&, f4, f4) Promotion                          */


dp::m4f::m4f(const m4f& source)
  : m4f((source[0][0]), (source[1][0]), (source[2][0]), (source[3][0]),
        (source[0][1]), (source[1][1]), (source[2][1]), (source[3][1]),
        (source[0][2]), (source[1][2]), (source[2][2]), (source[3][2]),
        (source[0][3]), (source[1][3]), (source[2][3]), (source[3][3]))
{} // end m4f(const m4f&) Copy                                                */


dp::m4f::m4f(m4f&& result) noexcept
{
  for (u1 i = 0; i < SQUBS; ++i) { col[i] = std::move(result[i]); }
} // end m4f(m4f &&) noexcept Move                                            */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::m4f& dp::m4f::ToAdjoint(f4* determinant)
{
  CofactorMat(determinant);
  Transpose();
  return *this;

} // end m4f m4f::ToAdjoint(f4*)                                              */


dp::m4f dp::m4f::AdjointMat(f4* determinant) const
{
  m4f adjugate(*this);
  adjugate.ToAdjoint(determinant);
  return adjugate;

} // end m4f m4f::AdjointMat(f4*) const                                       */


/* Calculate and return minor determinant matrix with oscillating signs       */
dp::m4f dp::m4f::CofactorMat(f4* determinant) const
{
  m4f cofactors(MinorMat(determinant));
  // Convert minor matrix to cofactor matrix
  for (s1 i = 0; i < SQUBS; ++i)
  {
    // Start on even subscripted columns' [1] subscript; odd columns on [0]
    for (s1 j = (i & 1) ? 0 : 1; j < SQUBS; j += 2)
    {
      // Advancing by 2 subscripts, negate every (other), matrix entry
      cofactors[i][j] = -cofactors[i][j];
    }
  }
  return cofactors;

} // end m4f m4f::CofactorMat(f4* determinant) const                          */


f4 dp::m4f::Det(void) const
{
  if (IsUpperTriangular() || IsLowerTriangular())
  {
    return Trace(true);
  }
  // TODO: scan for easiest row / col to process by totaling 0 entries
  // Get determinant by first column
  return (col[0][0]*MinorDet(0,0)   // + even subscript
        - col[1][0]*MinorDet(0,1)   // - odd subscript
        + col[2][0]*MinorDet(0,2)   // + even subscript
        - col[3][0]*MinorDet(0,3)); // - odd subscript

} // end f4 m4f::Det(void) const                                              */


dp::m4f dp::m4f::Inverse(void) const
{
  f4 det; // Pass through store of determinant (to be calculated incidentally)
  // Get cofactor matrix (from minor determinant matrix: internal to cofactor)
  m4f inv(CofactorMat(&det));
  if (IsSingular(&det)) { return nullm4f; } // Confirm the matrix is invertible
  inv.Transpose(); // Convert cofactor matrix to adjugate
  // Scale adjugate matrix to inverse matrix
  f4 scalar = 1.0f / det;
  inv *= scalar;
  return inv;
} // end m4f m4f::Inverse(void)                                               */


dp::m4f& dp::m4f::Invert(void)
{ return (Set(Inverse())); } // end m4f& m4f::Invert(void)                    */


std::string dp::m4f::JSON(void) const
{
  m4f mt(col[0][0], col[0][1], col[0][2], col[0][3],
          col[1][0], col[1][1], col[1][2], col[1][3],
          col[2][0], col[2][1], col[2][2], col[2][3],
          col[3][0], col[3][1], col[3][2], col[3][3]);
  return std::string("[" + mt[0].JSON() + "," + mt[1].JSON() + ","
    + mt[2].JSON() + "," + mt[3].JSON() + "]");

} // end std::string m4f::JSON(void) const                                    */


dp::m4f dp::m4f::Linear(void) const
{
    return m4f(col[0][0], col[1][0], col[2][0], 0.0f,
                col[0][1], col[1][1], col[2][1], 0.0f,
                col[0][2], col[1][2], col[2][2], 0.0f,
                     0.0f,      0.0f,      0.0f, 1.0f);
} // end m4f m4f::Linear(void) const                                          */


f4 dp::m4f::MinorDet(u1 row, u1 col) const
{
  // TODO: scan for 0 entries to find the easiest sum to calculate
  const v4f* m = (this->col);
  s1 r1, r2, r3, c1, c2, c3;

  // Get relevant row and column subscripts (by exclusion)
  row &= v4f::VSUBS_MASK;
  switch (row)
  {
  // row [0] discluded
  case 0:   r1 = 1; r2 = 2; r3 = 3; break;
  // row [1] discluded
  case 1:   r1 = 0; r2 = 2; r3 = 3; break;
  // row [2] discluded
  case 2:   r1 = 0; r2 = 1; r3 = 3; break;
  // row [3] discluded
  default:  r1 = 0; r2 = 1; r3 = 2; break;
  }
  col &= v4f::VSUBS_MASK;
  switch (col)
  {
  // col [0] discluded
  case 0:   c1 = 1; c2 = 2; c3 = 3; break;
  // col [1] discluded
  case 1:   c1 = 0; c2 = 2; c3 = 3; break;
  // col [2] discluded
  case 2:   c1 = 0; c2 = 1; c3 = 3; break;
  // col [3] discluded
  default:  c1 = 0; c2 = 1; c3 = 2; break;
  }
  // Calculate determined component 3x3 subset's 2x2 cofactor sum: determinant
  // Sum cofactor by first row
  return (m[c1][r1] * (m[c2][r2] * m[c3][r3] - m[c3][r2] * m[c2][r3])  // + odd
        - m[c2][r1] * (m[c1][r2] * m[c3][r3] - m[c1][r3] * m[c3][r2])  // -even
        + m[c3][r1] * (m[c1][r2] * m[c2][r3] - m[c1][r3] * m[c2][r2]));// + odd
} // end f4 m4f::MinorDet(u1, u1) const                                       */


/* Calculate and return the matrix comprised of the minor determinants        */
dp::m4f dp::m4f::MinorMat(f4* determinant) const
{
  // Row-major input & (row, col) arg list format match: col-major flip
  m4f minors(MinorDet(0, 0), MinorDet(0, 1), MinorDet(0, 2), MinorDet(0, 3),
              MinorDet(1, 0), MinorDet(1, 1), MinorDet(1, 2), MinorDet(1, 3),
              MinorDet(2, 0), MinorDet(2, 1), MinorDet(2, 2), MinorDet(2, 3),
              MinorDet(3, 0), MinorDet(3, 1), MinorDet(3, 2), MinorDet(3, 3));
  // If an f4* was passed in, set the det() into it to save on recalculation
  if (determinant)
  {
    *determinant =  col[0][0]*minors[0][0]  // + even subscript
                  - col[1][0]*minors[1][0]  // - odd subscript
                  + col[2][0]*minors[2][0]  // + even subscript
                  - col[3][0]*minors[3][0]; // - odd subscript
  }
  return minors;

} // end m4f m4f::MinorMat(f4*) const                                         */


dp::v3f dp::m4f::PProd(const v3f& point) const
{
  return (*this * point.XYZW(1.0f)).XYZ();
} // end v3f m4f::PProd(const v3f&) const                                     */


dp::m4f& dp::m4f::Set(f4 r1c1, f4 r1c2, f4 r1c3, f4 r1c4, f4 r2c1, f4 r2c2,
  f4 r2c3, f4 r2c4, f4 r3c1, f4 r3c2, f4 r3c3, f4 r3c4, f4 r4c1, f4 r4c2,
  f4 r4c3, f4 r4c4, bool colMajor)
{
  if (colMajor)
  {
    col[0].Set(r1c1, r1c2, r1c3, r1c4);
    col[1].Set(r2c1, r2c2, r2c3, r2c4);
    col[2].Set(r3c1, r3c2, r3c3, r3c4);
    col[3].Set(r4c1, r4c2, r4c3, r4c4);
  }
  else
  {
    col[0].Set(r1c1, r2c1, r3c1, r4c1);
    col[1].Set(r1c2, r2c2, r3c2, r4c2);
    col[2].Set(r1c3, r2c3, r3c3, r4c3);
    col[3].Set(r1c4, r2c4, r3c4, r4c4);
  }
  return *this;
} // end m4f& m4f::Set(f4,f4,f4,f4,f4,f4,f4,f4,f4,f4,f4,f4,f4,f4,f4,f4,bool)  */


dp::m4f& dp::m4f::Set(f4 ux, f4 vx, f4 nx, f4 tx, f4 uy, f4 vy, f4 ny, f4 ty,
  f4 uz, f4 vz, f4 nz, f4 tz, f4 affine, f4 skew)
{
  col[0].Set(ux, uy, uz, skew);
  col[1].Set(vx, vy, vz, skew);
  col[2].Set(nx, ny, nz, skew);
  col[3].Set(tx, ty, tz, affine);
  return *this;
}// end m4f& m4f::Set(f4,f4,f4,f4, f4,f4,f4,f4, f4,f4,f4,f4, f4,f4) Affine 3D */


dp::m4f& dp::m4f::Set(f4 ux, f4 vx, f4 nx, f4 uy, f4 vy, f4 ny, f4 uz, f4 vz,
  f4 nz, f4 affine, f4 skew)
{
  col[0].Set( ux,  uy,  uz, skew);
  col[1].Set( vx,  vy,  vz, skew);
  col[2].Set( nx,  ny,  nz, skew);
  col[3].Set(skew,skew,skew,affine);
  return *this;
} // end m4f& m4f::Set(f4, f4, f4, f4, f4, f4, f4, f4, f4, f4, f4) linear 3D  */


dp::m4f& dp::m4f::Set(f8 ux, f8 vx, f8 nx, f8 uy, f8 vy, f8 ny, f8 uz, f8 vz,
  f8 nz, f8 affine, f8 skew)
{
  f4 skewF = static_cast<f4>(skew), affineF = static_cast<f4>(affine);
  col[0].Set(static_cast<f4>(ux), static_cast<f4>(uy), static_cast<f4>(uz),
    skewF);
  col[1].Set(static_cast<f4>(vx), static_cast<f4>(vy), static_cast<f4>(vz),
    skewF);
  col[2].Set(static_cast<f4>(nx), static_cast<f4>(ny), static_cast<f4>(nz),
    skewF);
  col[3].Set(skewF, skewF, skewF, affineF);
  return *this;
} // end m4f& m4f::Set(f8,f8,f8, f8,f8,f8, f8,f8,f8, f8, f8) lazy linear 3D   */


dp::m4f& dp::m4f::Set(f4 ux, f4 vx, f4 tx, f4 uy, f4 vy, f4 ty, PLANE plane,
  f4 tz, f4 normal, f4 affine, f4 off, f4 skew)
{
  u1 u, v, n;
  // Process axis and column major preferences to delegate to base setter
  switch (plane)
  {
  case PLANE::ZY: // about x axis on the yz plane for z-down 2D world space
    n = 0, u = 1, v = 2; uy = -uy; vx = -vx; break;
  case PLANE::YZ: // about x axis on the yz plane for z-up 2D world space
    n = 0, u = 1, v = 2; break;
  case PLANE::ZX: // about y axis on the xz plane for x-down 2D world space
    u = 0, n = 1, v = 2; uy = -uy; vx = -vx; break;
  case PLANE::XZ: // about y axis on the xz plane for x-up 2D world space
    u = 0, n = 1, v = 2; break;
  case PLANE::YX: // about z axis on the xy plane for y-down 2D world space
    u = 0, v = 1, n = 2; uy = -uy; vx = -vx; break;
  default: // about z axis on the xy plane for y-up 2D world space
    u = 0, v = 1, n = 2; break;
  } // end axis switch statement
  col[u][u] =   ux;   col[u][v] =  uy; col[u][n] = off; col[u][3] = Of;
  col[v][v] =   vy;   col[v][u] =  vx; col[v][n] = off; col[v][3] = Of;
  col[n][n] = normal; col[n][u] = off; col[n][v] = off; col[n][3] = Of;
  col[3].Set(tx, ty, tz, If);
  return *this;
} // end m4f& m4f::Set(f4, f4, f4, f4, f4, f4, PLANE, (f4), (f4)) affine 2D   */


dp::m4f& dp::m4f::Set(f4 sx,f4 tx,f4 sy,f4 ty,f4 sz, f4 tz,f4 affine,f4 skew)
{
  col[0].Set(sx, skew, skew, skew);
  col[1].Set(skew, sy, skew, skew);
  col[2].Set(skew, skew, sz, skew);
  col[3].Set(tx, ty, tz, affine);
  return *this;
} // end m4f& m4f::Set(f4, f4, f4, f4, f4, f4, (f4), (f4)) 3D Scale-Translate */


dp::m4f& dp::m4f::Set(f4 ux, f4 vx, f4 uy, f4 vy, PLANE plane, f4 tz,
  f4 normal, f4 affine, f4 off, f4 skew)
{
  return Set(ux, vx, Of, uy, vy, Of, plane, tz, normal, affine, off, skew);
} // end m4f& m4f::Set(f4,f4,f4,f4,PLANE,(f4),(f4),(f4) (f4),(f4)) Linear 2D  */


dp::m4f& dp::m4f::Set(f4 scale, f4 tx, f4 ty, f4 tz)
{
  col[0].Set(scale, Of, Of, Of);
  col[1].Set(Of, scale, Of, Of);
  col[2].Set(Of, Of, scale, Of);
  col[3].Set(tx, ty, tz, If);
  return *this;
} // end m4f& m4f::Set(f4, f4, f4, f4) Uniform 3D Scale-Translate             */


dp::m4f& dp::m4f::Set(f4 sx, f4 sy, f4 sz)
{
  return Set(sx, sy, sz, Of, Of, Of);
} // end m4f& m4f::Set(f4, f4, f4) Non-Uniform Scalar                         */


dp::m4f& dp::m4f::Set(f4 linear, f4 affine)
{
  return Set(linear, Of, linear, Of, linear, Of, affine, Of);
} // end m4f& m4f::Set((f4), (f4)) Uniform                                    */



dp::m4f& dp::m4f::Set(const v2f& U, const v2f& V, PLANE axisOut, const v2f& T,
  f4 tz, f4 normal, f4 affine, f4 off, f4 skew)
{
   return Set(U.x, V.x, T.x,
              U.y, V.y, T.y,
              axisOut, tz, normal,
              affine, off, skew);
}// end m4f& m4f::Set(ct v2f&,ct v2f&,PLANE,ct v2f&,(f4),(f4),(f4),(f4),(f4)) */


dp::m4f& dp::m4f::Set(const v4f& U, const v4f& V, const v4f& N, const v4f& T)
{
  col[0].Set(U);
  col[1].Set(V);
  col[2].Set(N);
  col[3].Set(T);
  return *this;
} // end m4f& m4f::Set(const v4f&, const v4f&, const v4f&, const v4f&)        */


dp::m4f& dp::m4f::Set(const m3f& linear, const v3f& translate, f4 affine,
  f4 skew)
{
  col[0] = linear.col[0].XYZW(skew); // no initial translate
  col[1] = linear.col[1].XYZW(skew); // use linear values to
  col[2] = linear.col[2].XYZW(skew); // make an affine T col
  col[3].Set(skew, skew, skew, affine);
  if (translate != nullv3f) { *this *= TranslateAffine(translate); }
  return *this;
}//end m4f& m4f::Set(const m3f&, const v3f&, (f4),(f4)) Homogeneous Promotion */


dp::m4f& dp::m4f::Set(const m4f& source)
{
  // Just copy each column vector: no other members should be defined in m4f
  for (s1 i = 0; i < SQUBS; ++i) { col[i] = source.col[i]; }
  return *this;

} // end m4f& m4f::Set(const m4f&)                                            */


std::string dp::m4f::ToString(bool lineBreaks) const
{
  // Get col -> row major ordering for output of row driven, parsed lines
  m4f mt(col[0][0], col[0][1], col[0][2], col[0][3],
         col[1][0], col[1][1], col[1][2], col[1][3],
         col[2][0], col[2][1], col[2][2], col[2][3],
         col[3][0], col[3][1], col[3][2], col[3][3]);
  std::string result("[" + mt[0].JSON());
  std::string br((lineBreaks ? ", \n " : ", ")); // Column break formatting
  for (u1 i = 1; i < SQUBS; ++i) { result += br + mt[i].JSON(); }
  result += "]";
  return result;

} // end std::string m4f::ToString(void) const                                */


f4 dp::m4f::Trace(bool product) const
{
  f4 trace = IfOrF(If, Of, product);
  if (product) { for (u4 i = 0; i < SQUBS; ++i) { trace *= col[i][i]; } }
  else { for (u4 i = 0; i < SQUBS; ++i) { trace += col[i][i]; } }
  return trace;
} // end f4 m4f::Trace(void) const                                            */


f4 dp::m4f::TraceH(bool product) const
{
  f4 trace = IfOrF(If, Of, product);
  if (product) { for (u4 i = 0; i < SQUBS-1; ++i) { trace *= col[i][i]; } }
  else { for (u4 i = 0; i < SQUBS-1; ++i) { trace += col[i][i]; } }
  return trace;
} // end f4 m4f::TraceH(void) const                                           */


dp::m4f& dp::m4f::Transpose(void)
{
  SwapF(col[1][0], col[0][1]); SwapF(col[2][0], col[0][2]);
  SwapF(col[3][0], col[0][3]); SwapF(col[2][1], col[1][2]);
  SwapF(col[3][1], col[1][3]); SwapF(col[3][2], col[2][3]);
  return *this;

} // end m4f& m4f::Transpose(void)                                            */


dp::m4f dp::m4f::Transposed(void) const
{
  // Quicker to just write and return the copy from existing values
  return m4f( col[0][0], col[0][1], col[0][2], col[0][3],
              col[1][0], col[1][1], col[1][2], col[1][3],
              col[2][0], col[2][1], col[2][2], col[2][3],
              col[3][0], col[3][1], col[3][2], col[3][3] );
} // end m4f m4f::Transposed(void) const                                      */


dp::v3f dp::m4f::VProd(const v3f& point) const
{
  return (*this * point.XYZW()).XYZ();
} // end v3f m4f::PProd(const v3f&) const                                     */


dp::m3f dp::m4f::XXX(void) const
{ v3f X(col[0].XXX()); return m3f(X, X, X); } // end m3f m4f::XXX(void) const */


dp::m3f dp::m4f::XXY(void) const
{
  v3f X(col[0].XXY());
  return m3f(X, X, col[1].XXY());
} // end m3f m4f::XXY(void) const                                             */


dp::m3f dp::m4f::XXZ(void) const
{
  v3f X(col[0].XXZ());
  return m3f(X, col[2].XXZ(), X);
} // end m3f m4f::XXZ(void) const                                             */


dp::m3f dp::m4f::XYX(void) const
{
  v3f X(col[0].XYX());
  return m3f(X, col[1].XYX(), X);
} // end m3f m4f::XYX(void) const                                             */


dp::m3f dp::m4f::XYY(void) const
{
  v3f Y(col[1].XXY());
  return m3f(col[0].XYY(), Y, Y);
} // end m3f m4f::XYY(void) const                                             */


dp::m3f dp::m4f::XYZ(void) const
{
  return m3f(col[0].XYZ(), col[1].XYZ(), col[2].XYZ());
} // end m3f m4f::XYZ(void) const                                             */


dp::m3f dp::m4f::XZX(void) const
{
  v3f X(col[0].XZX());
  return m3f(X, col[2].XZX(), X);
} // end m3f m4f::XZX(void) const                                             */


dp::m3f dp::m4f::XZY(void) const
{
  return m3f(col[0].XZY(), col[2].XZY(), col[1].XZY());
} // end m3f m4f::XZY(void) const                                             */


dp::m3f dp::m4f::XZZ(void) const
{
  v3f Z(col[2].XZZ());
  return m3f(col[0].XZZ(), Z, Z);
} // end m3f m4f::XZZ(void) const                                             */


dp::m3f dp::m4f::YXX(void) const
{
  v3f X(col[0].YXX());
  return m3f(col[1].YXX(), X, X);
} // end m3f m4f::YXX(void) const                                             */


dp::m3f dp::m4f::YXY(void) const
{
  v3f Y(col[1].YXY());
  return m3f(Y, col[0].YXY(), Y);
} // end m3f m4f::YXY(void) const                                             */


dp::m3f dp::m4f::YXZ(void) const
{
  return m3f(col[1].YXZ(), col[0].YXZ(), col[2].YXZ());
} // end m3f m4f::YXZ(void) const                                             */


dp::m3f dp::m4f::YYX(void) const
{
  v3f Y(col[1].YYX());
  return m3f(Y, Y, col[0].YYX());
} // end m3f m4f::YYX(void) const                                             */


dp::m3f dp::m4f::YYY(void) const
{ v3f Y(col[1].YYY()); return m3f(Y, Y, Y); } // end m3f m4f::YYY(void) const */


dp::m3f dp::m4f::YYZ(void) const
{
  v3f Y(col[1].YYZ());
  return m3f(Y, Y, col[2].YYZ());
} // end m3f m4f::YYZ(void) const                                             */


dp::m3f dp::m4f::YZX(void) const
{
  return m3f(col[1].YZX(), col[2].YZX(), col[0].YZX());
} // end m3f m4f::YZX(void) const                                             */


dp::m3f dp::m4f::YZY(void) const
{
  v3f Y(col[1].YZY());
  return m3f(Y, col[2].YZY(), Y);
} // end m3f m4f::YZY(void) const                                             */


dp::m3f dp::m4f::YZZ(void) const
{
  v3f Z(col[2].YZZ());
  return m3f(col[1].YZZ(), Z, Z);
} // end m3f m4f::YZZ(void) const                                             */


dp::m3f dp::m4f::ZXX(void) const
{
  v3f X(col[0].ZXX());
  return m3f(col[2].ZXX(), X, X);
} // end m3f m4f::ZXX(void) const                                             */


dp::m3f dp::m4f::ZXY(void) const
{
  return m3f(col[2].ZXY(), col[0].ZXY(), col[1].ZXY());
} // end m3f m4f::ZXY(void) const                                             */


dp::m3f dp::m4f::ZXZ(void) const
{
  v3f Z(col[2].ZXZ());
  return m3f(Z, col[0].ZXZ(), Z);
} // end m3f m4f::ZXZ(void) const                                             */


dp::m3f dp::m4f::ZYX(void) const
{
  return m3f(col[2].ZYX(), col[1].ZYX(), col[0].ZYX());
} // end m3f m4f::ZYX(void) const                                             */


dp::m3f dp::m4f::ZYY(void) const
{
  v3f Y(col[1].ZYY());
  return m3f(col[2].ZYY(), Y, Y);
} // end m3f m4f::ZYY(void) const                                             */


dp::m3f dp::m4f::ZYZ(void) const
{
  v3f Z(col[2].ZYZ());
  return m3f(Z, col[1].ZYZ(), Z);
} // end m3f m4f::ZYZ(void) const                                             */


dp::m3f dp::m4f::ZZX(void) const
{
  v3f Z(col[2].ZZX());
  return m3f(Z, Z, col[0].ZZX());
} // end m3f m4f::ZZX(void) const                                             */


dp::m3f dp::m4f::ZZY(void) const
{
  v3f Z(col[2].ZZY());
  return m3f(Z, Z, col[1].ZZY());
} // end m3f m4f::ZZY(void) const                                             */


dp::m3f dp::m4f::ZZZ(void) const
{ v3f Z(col[2].ZZZ()); return m3f(Z, Z, Z); } // end m3f m4f::ZZZ(void) const */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/* Set a m4f column / row data to another source's column / row values       */
dp::m4f& dp::m4f::operator=(const m4f& source)
{
  return this->Set(source);

} // end m4f& m4f::operator=(const m4f&)


/* Set this m4f from a function result's row / column data values            */
dp::m4f& dp::m4f::operator=(m4f&& result) noexcept
{
  for (u1 i = 0; i < SQUBS; ++i) { col[i] = std::move(result.col[i]); }
  return *this;

} // end m4f& m4f::operator=(m4f&&) noexcept


/* Calculate the inverse matrix and return the result (no internal change)    */
dp::m4f dp::m4f::operator-(void) const
{
  return m4f(-col[0][0],-col[1][0],-col[2][0],-col[3][0],
              -col[0][1],-col[1][1],-col[2][1],-col[3][1],
              -col[0][2],-col[1][2],-col[2][2],-col[3][2],
              -col[0][3],-col[1][3],-col[2][3],-col[3][3] );

} // end m4f m4f::operator-(void) const


/* Get the inverse of the current matrix data set (reverse transform)         */
dp::m4f dp::m4f::operator!(void) const
{
  return this->Inverse();

} // end m4f m4f::operator!(void)


/* Get a column v4f from the m4f's col[0-3] values: <0:u, 1:v, 2:n, 3:t>     */
const dp::v4f& dp::m4f::operator[](u4 subscript) const
{
  subscript &= v4f::VSUBS_MASK; // Using v4f array and has square dimensions
  return col[subscript];

} // end const v4f& m4f::operator[](u4) const


/* Get a column v4f from the m4f's col[0-3] values: <0:u, 1:v, 2:n, 3:t>     */
dp::v4f& dp::m4f::operator[](u4 subscript)
{
  subscript &= v4f::VSUBS_MASK; // Using v4f array and has square dimensions
  return col[subscript];

} // end v4f& m4f::operator[](u4)


/* Calculate the member-wise addition of 2 m4f's, returning the sum          */
dp::m4f dp::m4f::operator+(const m4f& other) const
{
  m4f sum(*this);
  for (s1 i = 0; i < SQUBS; ++i) { sum[i] += other[i]; }
  return sum;

} // end m4f m4f::operator+(const m4f&)


/* Add an other m4f's values member-wise, setting the sum into this          */
dp::m4f& dp::m4f::operator+=(const m4f& other)
{
  for (s1 i = 0; i < SQUBS; ++i) { col[i] += other[i]; }
  return *this;

} // end m4f& m4f::operator+=(const m4f&)


/* Calculate member-wise subtraction of 2 m4f's, returning the difference    */
dp::m4f dp::m4f::operator-(const m4f& other) const
{
  m4f difference(*this);
  for (s1 i = 0; i < SQUBS; ++i) { difference[i] -= other[i]; }
  return difference;

} // end m4f m4f::operator-(const m4f&)


/* Subtract another m4f's values member-wise, setting the difference         */
dp::m4f& dp::m4f::operator-=(const m4f& other)
{
  for (s1 i = 0; i < SQUBS; ++i) { col[i] -= other[i]; }
  return *this;

} // end m4f& m4f::operator-=(const m4f&)


/* Scalar multiplication multiplies each member by scalar constant            */
dp::m4f dp::m4f::operator*(f4 scalar) const
{
  m4f scaled(*this);
  scaled *= scalar;
  return scaled;

} // end m4f m4f::operator*(f4) const


/* Get the concatenated matrix of (this * other), doing both transformations  */
dp::v4f dp::m4f::operator*(const v4f& vector) const
{
  m4f mt(this->Transposed());
  return v4f((mt[0].Dot(vector)), (mt[1].Dot(vector)),
             (mt[2].Dot(vector)), (mt[3].Dot(vector)));

} // end v4f m4f::operator*(const v4f&) const


/* Scale each matrix element by scalar, setting the product into this         */
dp::m4f& dp::m4f::operator*=(f4 scalar)
{
  for (s1 i = 0; i < SQUBS; ++i) { col[i] *= scalar; }
  return *this;
} // end m4f& m4f::operator*=(f4)


/* Get the concatenated matrix of (this * other), doing both transformations  */
dp::m4f dp::m4f::operator*(const m4f& other) const
{
  m4f concat(*this);
  concat *= other;
  return concat;

} // end m4f m4f::operator*(const m4f&) const


/* Matrix multiplication concatenates 2 sequential transformations into 1     */
dp::m4f& dp::m4f::operator*=(const m4f& other)
{
  // TODO: use validators to perform shortcut math and optimize
  m4f mc(*this);  // Matrix copy (to not work with shifting values)
  m4f ot = other.Transposed(); // Transposed other (for vector-wise Dot())

  for (s1 i = 0; i < SQUBS; ++i)
  {
    for (s1 j = 0; j < SQUBS; ++j) { col[i][j] = mc[i] * ot[j]; }
  }
  return *this;

} // end m4f& m4f::operator*=(const m4f&)


/* Perform division on all members, returning the result                      */
dp::m4f dp::m4f::operator/(f4 inverseScalar) const
{
  f4 scalar = (1.0f / inverseScalar);
  return ((*this) * scalar);

} // end m4f m4f::operator/(f4) const


/* Division multiplies all members by reciprocal of inverseScalar             */
dp::m4f& dp::m4f::operator/=(f4 inverseScalar)
{
  f4 scalar = (1.0f / inverseScalar);
  return ((*this) *= scalar);

} // end m4f& m4f::operator/=(f4)


// Get string of data in matrix cells output as row major data per line       */
dp::m4f::operator std::string(void) const
{ return this->ToString(true); } // end operator std::string(void)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::m4f& dp::m4f::cofactor(f4* determinant)
{
  // Set to minor matrix, track what the determinant was
  *this = MinorMat(determinant);
  // Convert minor matrix to cofactor matrix
  for (s1 i = 0; i < SQUBS; ++i)
  {
    // Start on even subscripted columns' [1] subscript; odd columns on [0]
    for (s1 j = (i & 1) ? 0 : 1; j < SQUBS; j += 2)
    {
      // Advancing by 2 subscripts, negate every (other), matrix entry
      col[i][j] = -col[i][j];
    }
  }
  return *this;

} // end m4f& m4f::cofactor(f4*)                                              */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


/* Create Rodrigues rotation m4f by rotation oround axis, centered on offset */
dp::m4f dp::RotateRodriguesAffine(ang rotation, const v3f& axis,
  const v3f& affineOffset)
{
  m4f rot;
  f4 rSin = static_cast<f4>(sin(rotation.r()));
  f4 rCos = static_cast<f4>(cos(rotation.r()));
  f4 len2 = axis.LengthSquared();
  f4 len;

  // Clamp to simplify and stabilize
  if (NearF(rSin, 0.0f, _INf)) { rSin = 0.0f; }
  if (NearF(rCos, 0.0f, _INf)) { rCos = 0.0f; }

  // Check for 2D shortcut simplifications: presumably, this'll be intentional
  if (NearF(len2, 1.0f, _INf)) // ie, using unit vecs will be a lot easier
  {
    if (axis == Z_HAT || axis == -Z_HAT) // Rotation in xy plane
    {
      rot[0][0] =  rCos; rot[1][0] = -rSin;
      rot[0][1] =  rSin; rot[1][1] =  rCos;
    }
    else if (axis == Y_HAT || axis == -Y_HAT) // Rotation in xz plane
    {
      rot[0][0] =  rCos;                    rot[2][0] =  rSin;

      rot[0][2] = -rSin;                    rot[2][2] =  rCos;
    }
    else if (axis == X_HAT || axis == -X_HAT) // Rotation in yz plane
    {

                          rot[1][1] = rCos; rot[2][1] = -rSin;
                          rot[1][2] = rSin; rot[2][2] = rCos;
    }
    else // Rotation around arbitrary, normalized axis
    {
      rot = ((1 - rCos) * axis.XYZW(If).DotTranspose()
          + Scale3Affine(rCos)
          + rSin * axis.CrossMatrix());
    }
  }
  else // Rotation around (presumed), arbitrary, unnormalized axis
  {
    len = SqrtF(len2);
    rot = (((1 - rCos)/len2) * axis.XYZW(If).DotTranspose()
          + Scale3Affine(rCos)
          + (rSin/len) * axis.CrossMatrix());
  }
  if (affineOffset != nullv3f) // Modify rotation to be about any affine offset
  {
    rot = AffineMat(rot, affineOffset);
  }
  return rot;

} // end m4f RotateRodrigues(angle, const v4f&, const v4f*)


/* Create a Euler angle rotation matrix about z y and x world axes            */
dp::m4f dp::RotateEulerAffine(ang z_rotation, ang y_rotation,
  ang x_rotation, const v3f& affineOffset)
{
  f4 zSin = static_cast<f4>(sin(z_rotation.r()));
  f4 zCos = static_cast<f4>(cos(z_rotation.r()));
  f4 ySin = static_cast<f4>(sin(y_rotation.r()));
  f4 yCos = static_cast<f4>(cos(y_rotation.r()));
  f4 xSin = static_cast<f4>(sin(x_rotation.r()));
  f4 xCos = static_cast<f4>(cos(x_rotation.r()));
  m4f zRot(zCos,-zSin, 0.0f,
            zSin, zCos, 0.0f,
            0.0f, 0.0f, 1.0f);
  m4f yRot(yCos, 0.0f, ySin,
            0.0f, 1.0f, 0.0f,
           -ySin, 0.0f, yCos);
  m4f xRot(1.0f, 0.0f, 0.0f,
            0.0f, xCos,-xSin,
            0.0f, xSin, xCos);
  m4f euler = zRot * yRot * xRot;
  if (affineOffset != nullv3f) // Modify rotation to be about any affine offset
  {
    euler = AffineMat(euler, affineOffset);
  }
  return euler;

} // end m4f RotateEuler(angle, angle, angle, const v4f*)


/* Create a m4f which vector multiplies to scale points in <x,y,z> by scalar */
dp::m4f dp::Scale3Affine(f4 x_scalar, f4 y_scalar, f4 z_scalar,
  const v3f& affineOffset)
{
  // Validate whether input scalars were 1, 2, or 3D
  if (std::isnan(z_scalar) || std::isinf(z_scalar))
  {
    if (std::isnan(y_scalar) || std::isinf(y_scalar))
    {
      if (std::isnan(x_scalar) || std::isinf(x_scalar))
      {
        x_scalar = 1.0f; // Given no inputs...identity is the best default?
      } // end all invalid branch
      // Given 2 default / useless arguments, set for uniform scaling
      y_scalar = x_scalar;
      z_scalar = x_scalar;
    } // end y_scalar + z_scalar both invalid branch
    else if (std::isnan(x_scalar) || std::isinf(x_scalar))
    {
      // Given 2 default / useless arguments, set for uniform scaling
      x_scalar = y_scalar;
      z_scalar = y_scalar;
    } // end z_scalar + x_scalar both invalid; y_scalar is not branch
    else
    {
      // x & y being good but z not is indicative of 2D scales:
      z_scalar = 1.0f; // Simply preserve z-values for model->world scalars
    } // end x_scalar and y_scalar both good; z_scalar is not branch
  } // end z_scalar invalid branch
  else
  {
    if (std::isnan(y_scalar) || std::isinf(y_scalar))
    {
      if (std::isnan(x_scalar) || std::isinf(x_scalar))
      {
        // Given 2 default / useless arguments, set for uniform scaling
        x_scalar = z_scalar;
        y_scalar = z_scalar;
      } // end only z_scalar valid branch
      else
      {
        y_scalar = 1.0f; // Simply preserve y-values for debugging convenience
      } // end x_scalar + z_scalar both valid branch
    }
    else if (std::isnan(x_scalar) || std::isinf(x_scalar))
    {
      x_scalar = 1.0f; // Simply preserve x-values for debugging convenience
    } // end both y_scalar + z_scalar are valid; x_scalar is not branch
    // else valid 3D scalars provided, no issues remain
  } // end z_scalar valid branch
  m4f scalarMat(x_scalar,  0.0f,     0.0f, 0.0f,
                 0.0f, y_scalar,     0.0f, 0.0f,
                 0.0f,     0.0f, z_scalar, 0.0f,
                 0.0f,     0.0f,     0.0f, 1.0f);
  if (affineOffset != nullv3f) // Modify scale around any offset
  {
    scalarMat = AffineMat(scalarMat, affineOffset);
  }
  return scalarMat;

} // end m4f ScaleMat(f4, f4, f4, const v3f&)                                 */

dp::m4f dp::ScaleAffine(const v3f& scales, const v3f& affineOffset)
{
  return Scale3Affine(scales.x, scales.y, scales.z, affineOffset);
} // end m4f ScaleAffine(const v3f&, const v3f&)                              */


dp::m4f dp::TranslateAffine(const v3f& displacement)
{
  return m4f(1.0f, 0.0f, 0.0f, displacement.x,
             0.0f, 1.0f, 0.0f, displacement.y,
             0.0f, 0.0f, 1.0f, displacement.z,
             0.0f, 0.0f, 0.0f,     1.0f); // Needs to be 1.0, regardless of w
} // end m4f Translate(const v4f&)                                            */


dp::m4f dp::LinearAffine(const m4f& source)
{
  if (source.IsLinear()) { return source; }
  return m4f(source[0][0], source[1][0], source[2][0], 0.0f,
             source[0][1], source[1][1], source[2][1], 0.0f,
             source[0][2], source[1][2], source[2][2], 0.0f,
                     0.0f,         0.0f,         0.0f, 1.0f);

} // end m4f LinearMat(const m4f&)                                            */


/* Take an existing linear matrix and displace the center of transformation   */
dp::m4f dp::AffineMat(const m4f& linear, const v3f& displacement)
{
  // TODO: validate the linearity of linear; optimize in general
  v3f backwards = -displacement;
  m4f offset = TranslateAffine(backwards);        // Move world frame
  offset *= linear;                               // Perform linear transform
  m4f lazyTrans = TranslateAffine(displacement);  // Shift world frame back
  return offset * lazyTrans;

} // end m4f AffineMat(const m4f&)


/* Concatenate an existing set of scale, rotate and translation matrices      */
dp::m4f dp::TransformAffine(const m4f& scale, const m4f& rotate,
  const m4f& translate)
{
  return scale * rotate * translate;

} // end m4f TransformMat(const m4f&, const m4f&, const m4f&)

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Functions                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


/* Scale each matrix element by scalar, setting the product into this         */
dp::m4f dp::operator*(f4 scalar, const dp::m4f& rhs)
{
  return (rhs * scalar);

} // end m4f m4f::operator*(const m4f&)


/* Confirm if 2 m4f's are relatively equal across all matrix elements        */
bool dp::operator==(const m4f& lhs, const m4f& rhs)
{
  // Check each column vector for equality
  for (s1 i = 0; i < m4f::SQUBS; ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }
  return true; // If no column was inequal, all were equal, so lhs == rhs

} // end bool operator==(const m4f&, const m4f&) const


/* Confirm if 2 m4f's are relatively inequal in any matrix element           */
bool dp::operator!=(const m4f& lhs, const m4f& rhs)
{
  // Check each column vector for inequality
  for (s1 i = 0; i < m4f::SQUBS; ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }
  return false; // If no column was inequal, all were equal, so !(lhs == rhs)

} // end bool operator!=(const m4f&, const m4f&) const


/* Output the string version of the vector data: <vector>, (point) or [clip]  */
std::ostream& dp::operator<<(std::ostream& output, const m4f& matrix)
{
  output << matrix.ToString(true);
  return output;

} // end std::ostream& operator<<(std::ostream&, const m4f&)
