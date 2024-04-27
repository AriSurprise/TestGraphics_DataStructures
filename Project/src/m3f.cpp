/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  m3f.cpp
Purpose:  2D 4x4, column major matrix; inputs / outpus in row major
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_CS350_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Summer 2022
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

#include "Log.h"    // Error / warning output message / logging
#include "m2f.h"    // Minor determinant calculation taking m2f method
#include "m3f.h"    // Associated class declaration header
#include "m4f.h"    // Data promotion from 3x3 -> 4x4 affine data
#include "Stats.h"  // Sampling struct to iterate over vertex / data lists
#include "Value.h"  // Basic numerical operations: clamp, swap, min, max, etc
#include "v2u.h"    // Index [min, max] ranges of read in vertex list data

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set matrix from 3 column vectors (col major ordered input -> result)
dp::m3f::m3f(const v3f& U, const v3f& V, const v3f& NT)
  : col{U, V, NT}
{ } // end m3f(const v3f&, const v3f&, const v3f&)

// Set 3x3 from matrix setting all members manually (row major ordered)       */
dp::m3f::m3f (f4 r1c1, f4 r1c2, f4 r1c3,
                f4 r2c1, f4 r2c2, f4 r2c3,
                f4 r3c1, f4 r3c2, f4 r3c3, bool colMajor)
  : col{v3f(r1c1, r2c1, r3c1),
        v3f(r1c2, r2c2, r3c2),
        v3f(r1c3, r2c3, r3c3)}
{
  if (colMajor) { Transpose(); }

} // end m3f(f4, f4, f4, f4, f4, f4, f4, f4, f4, bool) raw entry constructor


// Set 3x3 from matrix's 2D linear and affine members manually (row major)    */
dp::m3f::m3f (f4 ux, f4 vx, f4 tx,
                f4 uy, f4 vy, f4 ty)
// Delegate to full manual constructor to minimize chaining peculiarities
  : m3f( ux,   vx,   tx,
          uy,   vy,   ty,
          0.0f, 0.0f, 1.0f)

{} // end m3f(f4, f4, f4, f4, f4, f4) 2D affine constructor


// Set from 4x4 matrix's 3D linear members manually (row major ordered)       */
dp::m3f::m3f (f4 ux, f4 vx,
                f4 uy, f4 vy, bool colMajor)
// Delegate to full manual constructor to minimize chaining peculiarities
  : m3f(  ux,   vx,  0.0f,
           uy,   vy,  0.0f,
          0.0f, 0.0f, 1.0f)

{
  if (colMajor) { Transpose(); }

} // end m3f(f4, f4, f4, f4, bool) 2D linear constructor


// Set 3x3 matrix from signed, exclusively integer literals (row major)       */
dp::m3f::m3f (s4 ux, s4 vx, s4 nx,
                s4 uy, s4 vy, s4 ny,
                s4 uz, s4 vz, s4 nz,
                bool colMajor)
  : m3f( static_cast<f4>(ux), static_cast<f4>(vx), static_cast<f4>(nx),
          static_cast<f4>(uy), static_cast<f4>(vy), static_cast<f4>(ny),
          static_cast<f4>(uz), static_cast<f4>(vz), static_cast<f4>(nz))
{
  if (colMajor) { Transpose(); }

} // end m3f(s4, s4, s4, s4, s4, s4, s4, s4, s4) signed lazy constructor


// Set 3x3 matrix members from signed double literals (row major ordered)     */
dp::m3f::m3f (f8 ux, f8 vx, f8 nx,
                f8 uy, f8 vy, f8 ny,
                f8 uz, f8 vz, f8 nz,
                bool colMajor)
  : m3f( static_cast<f4>(ux), static_cast<f4>(vx), static_cast<f4>(nx),
          static_cast<f4>(uy), static_cast<f4>(vy), static_cast<f4>(ny),
          static_cast<f4>(uz), static_cast<f4>(vz), static_cast<f4>(nz))
{
  if (colMajor) { Transpose(); }

} // end m3f(s4, s4, s4, s4, s4, s4, s4, s4, s4) f-lazy 3D linear constructor


// Uniform scalar matrix, I3 default: 1's along the trace, 0 elsewhere        */
dp::m3f::m3f(f4 scalar)
// Delegate to full manual constructor minimizing chain delegation confusion
  : m3f(scalar, 0.0f, 0.0f,
         0.0f, scalar, 0.0f,
         0.0f, 0.0f, scalar)

{} // end m3f(f4) Identity / Uniform scalar constructor


// Non-uniform scalar matrix, unique scalars along trace with 0 elsewhere     */
dp::m3f::m3f(f4 x_scale, f4 y_scale, f4 z_scale)
// Delegate to full manual constructor minimizing chain delegation confusion
  : m3f(x_scale, 0.0f, 0.0f,
         0.0f, y_scale, 0.0f,
         0.0f, 0.0f, z_scale)

{} // end m3f(f4, f4, f4) Non-uniform scalar constructor


// 2D transform matrix: simple 2D transform initialization
dp::m3f::m3f(ang z_rot, f4 x_move, f4 y_move, f4 x_scale, f4 y_scale)
{
  // Create shorthand & cached values for pre-multiplied T^(-1)*S*R*T matrices
  f4& tx = x_move, ty = y_move, sx = x_scale, sy = y_scale;
  f4 rc = cos(z_rot.r());
  f4 rs = sin(z_rot.r());
  // Set this to affine unTranslate, Rotate, Scale & reTranslate operations
  *this = (m3f((rc*sx), (-rs*sx), (rc*sx*tx-rs*sx*ty-tx),
                (rs*sy),  (rc*sy), (rc*sy*ty+rs*sy*tx-ty),
                   0.0f,     0.0f,                  1.0f));

} // end m3f(ang, f4, f4, f4, f4) 2D transform matrix constructor


// Create a copy of another m3f from copied row / column data values         */
dp::m3f::m3f(const m3f& source)
  : m3f((source[0][0]), (source[1][0]), (source[2][0]),
         (source[0][1]), (source[1][1]), (source[2][1]),
         (source[0][2]), (source[1][2]), (source[2][2]))

{} // end m3f(const m3f&) Copy constructor


// Create this m3f from a function result's row / column data values
dp::m3f::m3f(m3f&& result) noexcept
{
  for (u1 i = 0; i < SQUBS; ++i) { col[i] = std::move(result[i]); }
} // end m3f(m3f&&) noexcept Move constructor



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set to unscaled (by 1/det) inverse matrix; transposed cofactor matrix      */
dp::m3f& dp::m3f::ToAdjoint(f4* determinant)
{
  CofactorMat(determinant);
  Transpose();
  return *this;

} // end m3f m3f::ToAdjoint(f4*)

// Calculate inverse m3f without scale by 1/det; transposed cofactor matrix  */
dp::m3f dp::m3f::AdjointMat(f4* determinant) const
{
  m3f adjugate(*this);
  adjugate.ToAdjoint(determinant);
  return adjugate;

} // end m3f m3f::AdjointMat(f4*) const

// Calculate and return minor determinant matrix with oscillating signs       */
dp::m3f dp::m3f::CofactorMat(f4* determinant) const
{
  m3f cofactors(MinorMat(determinant));
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

} // end m3f m3f::CofactorMat(f4* determinant) const


// Calculate and return the determinant of the matrix                         */
f4 dp::m3f::Det(void) const
{
  if (IsUpperTriangular() || IsLowerTriangular())
  {
    return Trace(true);
  }
  // TODO: scan for easiest row / col to process by totaling 0 or 1 entries
  // Get determinant by first column
  return (col[0][0]*MinorDet(0,0)   // + even subscript
        - col[1][0]*MinorDet(0,1)   // - odd subscript
        + col[2][0]*MinorDet(0,2)); // + even subscript

} // end f4 m3f::Det(void) const                                              */


dp::m3f dp::m3f::Inverse(void) const
{
  f4 det; // Pass through store of determinant (to be calculated incidentally)

  // Get cofactor matrix (from minor determinant matrix: internal to cofactor)
  m3f inv(CofactorMat(&det));

  // Confirm if the matrix is invertible before proceeding
  if (IsSingular(&det)) { return nullm3f; }

  // Convert cofactor matrix to adjugate
  inv.Transpose();

  // Scale adjugate matrix to inverse matrix
  f4 scalar = 1.0f / det;
  inv *= scalar;
  return inv;

} // end m3f m3f::Inverse(void)                                               */


dp::m3f& dp::m3f::Invert(void) { return (Set(Inverse())); }
// end m3f& m3f::Invert(void)                                                 */


std::string dp::m3f::JSON(void) const
{
  m3f mt(col[0][0], col[0][1], col[0][2],
         col[1][0], col[1][1], col[1][2],
         col[2][0], col[2][1], col[2][2]);
  return std::string("["+mt[0].JSON() +","+mt[1].JSON() +","+mt[2].JSON()+"]");
} // end std::string m3f::JSON(void) const                                    */


dp::m3f dp::m3f::Linear(void) const
{
    return m3f(col[0][0], col[1][0], Of,
               col[0][1], col[1][1], Of,
                      Of,        Of, If);
} // end m3f m3f::Linear(void) const                                          */


dp::m4f dp::m3f::M4(void) const
{
    return m4f(*this);
} // end m4f m3f::M4(void) const                                              */


f4 dp::m3f::MinorDet(u1 row, u1 col) const
{
  // TODO: scan for 0 entries to find the easiest sum to calculate
  const v3f* m = (this->col);
  s1 r1=0, r2=0, c1=0, c2=0;

  // Get relevant row and column subscripts (by exclusion)
  row %= 3;
  switch (row)
  {
  case 0:   r1 = 1; r2 = 2; break; // row [0] discluded
  case 1:   r1 = 0; r2 = 2; break; // row [1] discluded
  case 2:   r1 = 0; r2 = 1; break; // row [2] discluded
  }
  col %= 3;
  switch (col)
  {
  case 0:   c1 = 1; c2 = 2; break; // col [0] discluded
  case 1:   c1 = 0; c2 = 2; break; // col [1] discluded
  case 2:   c1 = 0; c2 = 1; break; // col [2] discluded
  }

  // Calculate determined <row, col> component 2x2 subset's determinant
  return m2f(this->col[c1][r1], this->col[c2][r1],
             this->col[c1][r2], this->col[c2][r2]).Det();
} // end f4 m3f::MinorDet(u1, u1) const                                       */


// Calculate and return the matrix comprised of the minor determinants        */
dp::m3f dp::m3f::MinorMat(f4* determinant) const
{
  // Row-major input & (row, col) arg list format match: col-major flip
  m3f minors(MinorDet(0, 0), MinorDet(0, 1), MinorDet(0, 2),
              MinorDet(1, 0), MinorDet(1, 1), MinorDet(1, 2),
              MinorDet(2, 0), MinorDet(2, 1), MinorDet(2, 2));
  // If an f4* was passed in, set the det() into it to save on recalculation
  if (determinant)
  {
    *determinant =  col[0][0]*minors[0][0]  // + even subscript
                  - col[1][0]*minors[1][0]  // - odd subscript
                  + col[2][0]*minors[2][0]; // + even subscript
  }
  return minors;

} // end m3f m3f::MinorMat(f4*) const


// Set a m3f column / row data to another source's column / row values       */
dp::m3f& dp::m3f::Set(const m3f& source)
{
  // Just copy each column vector: no other members should be defined in m3f
  for (s1 i = 0; i < SQUBS; ++i) { col[i] = source.col[i]; }
  return *this;

} // end m3f& m3f::Set(const m3f&)


// JSON formatted string containing each member value in row major format     */
std::string dp::m3f::ToString(bool lineBreaks) const
{
  m3f mt(col[0][0], col[0][1], col[0][2],
          col[1][0], col[1][1], col[1][2],
          col[2][0], col[2][1], col[2][2]);
  std::string result("[" + mt[0].JSON());
  for (u1 i = 1; i < SQUBS; ++i)
  {
    result += ", ";
    result += ((lineBreaks) ? "\n " : "") + mt[i].JSON();
  }
  result += "]";
  return result;

} // end std::string m3f::ToString(void) const


// Calculate and return sum (or product) of the diagonal elements             */
f4 dp::m3f::Trace(bool product) const
{
  return (product) ? (col[0][0]) * (col[1][1]) * (col[2][2]) :
                     (col[0][0]) + (col[1][1]) + (col[2][2]);

} // end f4 m3f::Trace(void) const


// Set the matrix to have swapped contents for rows and columns               */
dp::m3f& dp::m3f::Transpose(void)
{
  SwapF(col[1][0], col[0][1]);
  SwapF(col[2][0], col[0][2]);
  SwapF(col[2][1], col[1][2]);
  return *this;

} // end m3f& m3f::Transpose(void)


// Get a copy of this m3f with swapped contents for rows and columns         */
dp::m3f dp::m3f::Transposed(void) const
{
  // Quicker to just write and return the copy from existing values
  return m3f( col[0][0], col[0][1], col[0][2],
               col[1][0], col[1][1], col[1][2],
               col[2][0], col[2][1], col[2][2]);

} // end m3f m3f::Transposed(void) const



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set a m3f column / row data to another source's column / row values       */
dp::m3f& dp::m3f::operator=(const m3f& source)
{
  return this->Set(source);

} // end m3f& m3f::operator=(const m3f&)


// Set this m3f from a function result's row / column data values            */
dp::m3f& dp::m3f::operator=(m3f&& result) noexcept
{
  for (u1 i = 0; i < SQUBS; ++i) { col[i] = std::move(result.col[i]); }
  return *this;

} // end m3f& m3f::operator=(m3f&&) noexcept


// Calculate the inverse matrix and return the result (no internal change)    */
dp::m3f dp::m3f::operator-(void) const
{
  return m3f(-col[0][0],-col[0][1],-col[0][2],
              -col[1][0],-col[1][1],-col[1][2],
              -col[2][0],-col[2][1],-col[2][2]);

} // end m3f m3f::operator-(void) const

// Calculate the inverse matrix and retern the result                         */
dp::m3f dp::m3f::operator!(void) const
{
  return this->Inverse();

} // end m3f& m3f::operator!(void)


// Get a column v4f from the m3f's col[0-3] values: <0:u, 1:v, 2:n|t>        */
const dp::v3f& dp::m3f::operator[](u4 subscript) const
{
  subscript &= 3;
  if (subscript < 3) { return col[subscript]; }
  else { return col[0]; }

} // end const v3f& m3f::operator[](u4) const


// Get a column v4f from the m3f's col[0-3] values: <0:u, 1:v, 2:n|t>        */
dp::v3f& dp::m3f::operator[](u4 subscript)
{
  subscript &= 3;
  if (subscript < 3) { return col[subscript]; }
  else { return col[0]; }

} // end v3f& m3f::operator[](u4)


// Calculate the member-wise addition of 2 m3f's, returning the sum          */
dp::m3f dp::m3f::operator+(const m3f& other) const
{
  m3f sum(*this);
  for (s1 i = 0; i < SQUBS; ++i) { sum[i] += other[i]; }
  return sum;

} // end m3f m3f::operator+(const m3f&)


// Add an other m3f's values member-wise, setting the sum into this          */
dp::m3f& dp::m3f::operator+=(const m3f& other)
{
  for (s1 i = 0; i < SQUBS; ++i) { col[i] += other[i]; }
  return *this;

} // end m3f& m3f::operator+=(const m3f&)


// Calculate member-wise subtraction of 2 m3f's, returning the difference    */
dp::m3f dp::m3f::operator-(const m3f& other) const
{
  m3f difference(*this);
  for (s1 i = 0; i < SQUBS; ++i) { difference[i] -= other[i]; }
  return difference;

} // end m3f m3f::operator-(const m3f&)


// Subtract another m3f's values member-wise, setting the difference         */
dp::m3f& dp::m3f::operator-=(const m3f& other)
{
  for (s1 i = 0; i < SQUBS; ++i) { col[i] -= other[i]; }
  return *this;

} // end m3f& m3f::operator-=(const m3f&)


// Scalar multiplication multiplies each member by scalar constant            */
dp::m3f dp::m3f::operator*(f4 scalar) const
{
  m3f scaled(*this);
  scaled *= scalar;
  return scaled;

} // end m3f m3f::operator*(f4) const


// Get the concatenated matrix of (this * other), doing both transformations  */
dp::v3f dp::m3f::operator*(const v3f& vector) const
{
  m3f mt(this->Transposed());
  return v3f((mt[0].Dot(vector)), (mt[1].Dot(vector)), (mt[2].Dot(vector)));

} // end v3f m3f::operator*(const v3f&) const


// Scale each matrix element by scalar, setting the product into this         */
dp::m3f& dp::m3f::operator*=(f4 scalar)
{
  for (s1 i = 0; i < SQUBS; ++i) { col[i] *= scalar; }
  return *this;
} // end m3f& m3f::operator*=(f4)


// Get the concatenated matrix of (this * other), doing both transformations  */
dp::m3f dp::m3f::operator*(const m3f& other) const
{
  m3f concat(*this);
  concat *= other;
  return concat;

} // end m3f m3f::operator*(const m3f&) const


// Matrix multiplication concatenates 2 sequential transformations into 1     */
dp::m3f& dp::m3f::operator*=(const m3f& other)
{
  // TODO: use validators to perform shortcut math to preferred axis?
  m3f mc(*this);  // Matrix copy (to not work with shifting values)
  m3f ot = other.Transposed(); // Transposed other ( for vector-wise Dot() )

  for (s1 i = 0; i < SQUBS; ++i)
  {
    for (s1 j = 0; j < SQUBS; ++j) { col[i][j] = mc[i].Dot(ot[j]); }
  }
  return *this;

} // end m3f& m3f::operator*=(const m3f&)                                     */


// Perform division on all members, returning the result                      */
dp::m3f dp::m3f::operator/(f4 divisor) const
{
  f4 scalar = (1.0f / divisor);
  return ((*this) * scalar);

} // end m3f ma3::operator/(f4) const


// Division multiplies all members by reciprocal of inverseScalar             */
dp::m3f& dp::m3f::operator/=(f4 inverseScalar)
{
  f4 scalar = (1.0f / inverseScalar);
  return ((*this) *= scalar);

} // end m3f& m3f::operator/=(f4)


// Get string of data in matrix cells output as row major data per line       */
dp::m3f::operator std::string(void) const
{ return this->ToString(true); } // end operator std::string(void)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Calculate and set matrix to minor det matrix with oscillating signs        */
dp::m3f& dp::m3f::cofactor(f4* determinant)
{
  // Set to minor matrix, track what the determinant was
  *this = MinorMat(determinant);
  // Convert minor matrix to cofactor matrix
  for (s1 i = 0; i < SQUBS; ++i)
  {
    // Start on even subscripted columns' [1] subscript; odd columns on [0]
    for (s1 j = (i & 1) /* 0 if even, 1 if odd */; j < SQUBS; j += 2)
    {
      // Advancing by 2 subscripts, negate every (other), matrix entry
      col[i][j] = -col[i][j];
    }
  }
  return *this;

} // end m3f& m3f::cofactor(f4*)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Functions                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


dp::m3f dp::OrthonormalBasis(const v3f& primary, const v3f& secondary, PLANE
  formed)
{
  // Get initial 3 axes
  v3f U = primary;
  v3f V = secondary;
  v3f N = U.Cross(V);
  if (N == nullv3f) // If inputs were null or parallel, data needs correction
  {
    // Repair degenerate inputs: get 2 non-parallel, non-null directions
    if (U == nullv3f) // First get at least U to be non-null
    {
      U = X_HAT;
      V = V.Cross(U); // Was secondary null or parallel to x?
      if (V == nullv3f) { V = Y_HAT; } // If so, use y
      // IF primary was null, U & V are non-null & non-parallel
    } // If primary was null, U no longer is: (now =: primary or X)
    // Secondary may still have been null or parallel
    if (V == nullv3f) // If V is still null (=> primary was non-null)
    {
      // Primary having been non-null => parallel, or null input
      V = U.Cross(Z_HAT); // U (now =: primary or X), given as Z parallel...?
      if (V == nullv3f) { V = Y_HAT; } // if U is Z parallel => Y isn't
      // else V is non-parallel now =: Cross(primary, Z)
    } // Both U & V can no longer be null or parallel
    // Re-establish N as a valid independent direction
    N = U.Cross(V);
  }
  // Establish orthonormality of U & N -> V
  N.Normalize();  // N is perpendicular to U & V => unitize
  U.Normalize();  // U is perpendicular to N => unitize
  V = N.Cross(U); // V is unitized & perpendicular (N & U being unit-vecs)

  // Establish local XYZ ordering for current U =: +x, V =: +y, N =: -z
  u4 m;
  switch (formed)
  {
    // Given primary =: U => x =: U;   secondary =: V => y =: V
  case PLANE::XY: // UxV right-handed basis =>  +x in U, +y in V & -z in N
    break; // intended input order currently +x,+y,-z -> +x,+y,-z (no-op)
    // Given primary =: U => y -> V;   secondary =: V => x -> U
  case PLANE::YX: // VxU right-handed basis =>  -x in U, +y in V & +z in N
    !N; (!V).Swap(U); // intended input order currently +y,-x,+z -> +x,+y,-z
    break;
    // Given primary =: U => x =: U;   secondary =: V => z -> N
  case PLANE::XZ: // UxN right-handed basis =>  +x in U, +y in V & +z in N
    (!V).Swap(N); // intended input order currently +x,+z,+y -> +x,+y,-z
    break;
    // Given primary =: U => z -> N;   secondary =: V => x -> U
  case PLANE::ZX: // NxU right-handed basis =>  +x in U, -y in V & +z in N
    // intended input order currently +z,+x,-y -> +x,+y,-z (pass through)
    // Given primary =: U => y -> V;   secondary =: V => z -> N
  case PLANE::YZ: // VxN right-handed basis =>  -x in U, +y in V & +z in N
    (!N).Swap((!V)); // intended input order currently +y,+z,-x -> +x,+y,-z
    U.Swap(V);
    break;
    // Given primary =: U => z -> N;   secondary =: V => y -> V
  case PLANE::ZY: // NxV right-handed basis =>  +x in U, +y in V & +z in N
    (!N).Swap(U); // intended input order currently +z,+y,+x -> +x,+y,-z
    break;
  case PLANE::UNDEFINED: default: // No context => max component =: axis
    m = MaxB3F(AbsF(U.z), AbsF(V.z), AbsF(N.z)); // max Z should be N axis
    switch (m) {
    case 0: N.Swap(U); break; // If U has max z, swap into N
    case 1: N.Swap(V); break; // If V has max z, swap into N
    case 2: default: break;   // If N has max z, good
    }
    m = MaxB3F(AbsF(U.y), AbsF(V.y), AbsF(N.y)); // max Y should be V axis
    switch (m) {
    case 1: U.Swap(V); break; // If U has max y, swap into V
    case 2: break; // If N has max y & z...?
    case 0: default: break; // If V has max y, good
    }
    break;
  }

  return m3f(U, V, N);
}

dp::m3f dp::Covariance3D(const v3f* data, u4 setSize, u4 samples, s2 step,
  u2 first)
{
  uRng subs(samples, setSize, first, step);
  // check for null / uniform spread over [0,1] samples: shortcut w/I3
  if (!data || subs.Dataset() < 2) { return m3f(); }
  v3f mu; // Mean of data per dimension (conventionally greek small mu)
  v3f s;  // Sample point 
  v3f sq; // Square mean difference from the mean: <(x-mu)^2,(y-mu)^2,(z-mu)^2>
  f4 xy = 0.0f, xz = 0.0f, yz = 0.0f; // 2D mean diff. products: x*y, x*z, y*z
  f4 rate = InvU(subs.Samples()); // divisor's inverse ratio of sampled points

  while (subs.Scanning()) // Get the dimensional dataset sum
  {
    mu += data[subs.Index()];
    ++subs;
  }
  mu *= rate; // Average sum to get the mean
  subs.Reset();
  while (subs.Scanning()) // Get the dimensional dataset sum
  {
    s = data[subs.Index()] - mu;
    sq += (s * s);
    xy += s.x * s.y;
    xz += s.x * s.z;
    yz += s.y * s.z;
    ++subs;
  }
  // Average sums to get the mean differences from the mean
  sq *= rate;
  xy *= rate;
  xz *= rate;
  yz *= rate;

  // Return symmetrical matrix of those averages
  return m3f(sq.x, xy, xz, xy, sq.y, yz, xz, yz, sq.z);

} // end :m3f dp::Covariance3D(const v3f*, u4, u4, s2, u2)                    */


dp::m3f dp::PCA3(const m3f& covariance, v3f* scalars)
{
  if (!covariance.IsReal()) { return nullm3f; } // ???...least harmful result?
  m3f basis; // Orthonormal eigen-vector basis (rotated from I3 identity m3f)
  m3f rot; // Rotation matrix of jacobi iteration (initially I3 identity m3f)
  m3f spread(covariance); // Context for rotations (from dataset's covariance)
  v2u a; // Rotation's (excluded) axis subscripts (rot plane of 2 others)
  f4 cosT, sinT, tanT; // Trig values extrapolated from remaining covariance
  u4 i = 0;

  // Iteratively perform basis rotations to isolate covariance -> spread in V
  if (!spread.IsDiagonal()) while (true)
  {
    // Get optimal rotation => max covariance (subscripts p!=q excl. rot. axis)
    switch (MaxB3F(AbsF(spread['y']['z']),
                   AbsF(spread['x']['z']),
                   AbsF(spread['x']['y'])))
    {
    case 0: a.Set(1, 2); break; // => yz was maximal; exclude ['x'] =: [0]
    case 1: a.Set(0, 2); break; // => xz was maximal; exclude ['y'] =: [1]
    case 2: default: a.Set(0, 1); break;//=> xy maxima; excl. ['z'] =: [2]
    }

    // Extract trig values defining rotation matrix about that world axis
    tanT = (spread[a.one][a.one] - spread[a.two][a.two]);// (cos^2-sin^2)
    tanT /= (2 * spread[a.one][a.two]);// 1st =>  B =:    / (2*cos*sin); -> tan
    //tan =: sign(B) / (|B| + sqrt(B^2+1))
    tanT = (IsNegF(tanT) ? -1 : 1) / (AbsF(tanT) + SqrtF((tanT * tanT) + 1));
    cosT = InvF(SqrtF((tanT * tanT) + 1)); // cos =: 1 / sqrt(tan^2+1)
    sinT = cosT * tanT; // sin =: tan*cos = (sin/cos)*cos = sin*(cos/cos) = sin
    /*
    if (TanT == Of || !NearF((cosT * cosT + sinT * sinT), If))
    {
      // tan(0) => no rotation; cos^2(t) + sin^2(t) =: 1
      Log::Diag("m3f PCA3 - probably not extracting sin / cos correctly");
    }
    //else { Log::D("."); }
    */
    // Set Jacobi matrix by I3 differences to create world axis rotation
    rot[a.one][a.one] = cosT;     rot[a.two][a.one] = sinT;
    rot[a.one][a.two] = -sinT;    rot[a.two][a.two] = cosT;

    // Rotate matrices by Jacobi rotation matrix
    basis *= rot; // Rotate eigen-vector orthonormal basis by Jacobi rotation
    spread *= rot; // Rotate spread (covariance remainder) equivalently
     // Get inverse rot.: transpose =: rotation matrix^{-1}; only 2 skew elems.
    SwapF(rot[a.one][a.two], rot[a.two][a.one]);
    spread = rot * spread; // Rotate back (rot^{-1}); Aggregately isolate spread
    // Reached termination case?
    if (++i == 500) { Log::Error("m3f PCA3 - rotation inconclusive"); break; }
    if (spread.IsDiagonal()) {break;}

    // Reset Jacobi matrix to a clean I3 matrix for the next iteration
    rot[a.one][a.one] = 1.0f;     rot[a.two][a.one] = 0.0f;
    rot[a.one][a.two] = 0.0f;     rot[a.two][a.two] = 1.0f;
    // (only set 4/9 elements, better ratio still in higher dimens)
  }

  // Get Eigen-values from diagonalized m3f; find max => axis of spread
  if (scalars)
  {
    // Probably shouldn't be able to be negative anymore? better to be certain
    scalars->Set(AbsF(spread[0][0]), AbsF(spread[1][1]), AbsF(spread[2][2]));
  }
  return basis;

} // end m3f PCA3(m3f, v3f*)                                                  */


dp::m3f dp::RotateEuler(ang x_rot, ang y_rot, ang z_rot)
{
  f4  zs = SinF(z_rot);
  f4  zc = CosF(z_rot);
  f4  ys = SinF(y_rot);
  f4  yc = CosF(y_rot);
  f4  xs = SinF(x_rot);
  f4  xc = CosF(x_rot);

  m3f zRot(zc, -zs, 0.0f,
    zs, zc, 0.0f,
    0.0f, 0.0f, 1.0f);
  m3f yRot(yc, 0.0f, ys,
    0.0f, 1.0f, 0.0f,
    -ys, 0.0f, yc);
  m3f xRot(1.0f, 0.0f, 0.0f,
    0.0f, xc, -xs,
    0.0f, xs, xc);
  return zRot * yRot * xRot;
} // end m3f RotateEuler(ang, ang, ang)                                       */


dp::m3f dp::Scale3Mat(f4 x_scalar, f4 y_scalar, f4 z_scalar)
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

  return m3f(x_scalar, y_scalar, z_scalar);

} // end m3f ScaleMat(f4, f4, f4)                                             */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                        Helper Operator Definitions                         */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Scale each matrix element by scalar, setting the product into this         */
dp::m3f dp::operator*(f4 scalar, const dp::m3f& rhs)
{
  return (rhs * scalar);

} // end m3f m3f::operator*(const m3f&)


// Confirm if 2 m3f's are relatively equal across all matrix elements        */
bool dp::operator==(const m3f& lhs, const m3f& rhs)
{
  // Check each column vector for equality
  for (s1 i = 0; i < m3f::SQUBS; ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }
  return true; // If no column was inequal, all were equal, so lhs == rhs

} // end bool operator==(const m3f&, const m3f&) const


// Confirm if 2 m3f's are relatively inequal in any matrix element           */
bool dp::operator!=(const m3f& lhs, const m3f& rhs)
{
  // Check each column vector for inequality
  for (s1 i = 0; i < m3f::SQUBS; ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }
  return false; // If no column was inequal, all were equal, so !(lhs == rhs)

} // end bool operator!=(const m3f&, const m3f&) const


// Output the string version of the vector data: <vector>, (point) or [clip]  */
std::ostream& dp::operator<<(std::ostream& output, const m3f& matrix)
{
  output << matrix.ToString(true);
  return output;

} // end std::ostream& operator<<(std::ostream&, const m3f&)
