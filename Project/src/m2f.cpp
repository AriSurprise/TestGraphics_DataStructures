/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  m2f.cpp
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
#include "m2f.h"   // Associated class declaration header
#include "Value.h"  // Basic numerical operations: clamp, swap, min, max, etc

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::m2f::m2f(const v2f& U, const v2f& V) : col{U, V}
{ } // end m2f(const v2f&, const v2f&)                                        */


dp::m2f::m2f(f4 r1c1, f4 r1c2, f4 r2c1, f4 r2c2)
  : col{v2f(r1c1, r2c1), v2f(r1c2, r2c2)}
{ } // end m2f(f4, f4, f4, f4, bool) raw entry                                */


dp::m2f::m2f(s4 ux, s4 vx, s4 uy, s4 vy)
  : m2f(static_cast<f4>(ux), static_cast<f4>(vx),
        static_cast<f4>(uy), static_cast<f4>(vy))
{ } // end m2f(s4, s4, s4, s4) signed int, lazy input constructor             */


dp::m2f::m2f(f8 ux, f8 vx, f8 uy, f8 vy)
  : m2f( static_cast<f4>(ux), static_cast<f4>(vx),
          static_cast<f4>(uy), static_cast<f4>(vy))
{ } // end m2f(f8, f8, f8, f8) double-float literals, lazy input constructor  */


dp::m2f::m2f(void)
// Delegate to full manual constructor minimizing chain delegation confusion
  : m2f(1.0f, 0.0f, 0.0f, 1.0f)
{ } // end m2f(void) Identity                                                 */


// Create a copy of another m2f from copied row / column data values
dp::m2f::m2f(const m2f& source)
  : m2f((source[0][0]), (source[1][0]), (source[0][1]), (source[1][1]))
{ } // end m2f(const m2f&) Copy constructor                                   */


dp::m2f::m2f(m2f&& result) noexcept
{
  for (u1 i = 0; i < SQUBS; ++i) { col[i] = std::move(result[i]); }
} // end m2f(m2f&&) noexcept Move constructor                                 */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

f4 dp::m2f::Det(void) const
{
  return (col[0][0]*col[1][1] - col[0][1]*col[1][0]);
} // end f4 m2f::Det(void) const                                              */


dp::m2f dp::m2f::Inverse(void) const
{
  f4 ds = Det(); // Determinant scalar

  // Confirm the matrix is invertible (0 det has 0 scale: can't divide by 0)
  if (IsSingular(&ds)) { return nullm2f; }
  else { ds = InvF(ds); }
  // Return inverse matrix (2D base case is simple enough to...'hard code'?)
  return m2f(ds*col[1][1],-ds*col[0][1],-ds*col[1][0], ds*col[0][0]);
} // end m2f m2f::Inverse(void)                                               */


dp::m2f& dp::m2f::Invert(void) { return (Set(Inverse())); }
// end m2f& m2f::Invert(void)                                                 */


bool dp::m2f::IsIdentity(void) const
{
  return (IsScalar() && NearF(col[0][0], 1.0f, _INf)
    && NearF(col[1][1], 1.0f, _INf));
} // end bool m2f::IsIdentity(void) const                                     */


bool dp::m2f::IsLowerTriangular(void) const
{
  return  (IsInF(col[1][0], _INf, -_INf));
} // end bool m2f::IsLowerTriangular(void) const                              */


bool dp::m2f::IsScalar(void) const
{
  return (col[1][0]==0 && col[0][1]==0);
} // end bool m2f::IsScalar(void) const                                       */


bool dp::m2f::IsSingular(f4* determinant) const
{
  // Compare either cached determinant, or, if none, calculated det to 0
  return (IsInF(((determinant) ? *determinant: Det()), _INf, -_INf));
} // end bool m2f::IsSingular(f4*) const                                      */


bool dp::m2f::IsSkewSymmetric(void) const
{
  if (Trace() != 0.0f) { return false; } // trace must be 0 for any -x == x
  // Check upper triangle elements against lower triangle counterparts
  return (col[1][0] == -col[0][1]);
} // end bool m2f::IsSkewSymmetric(void) const                                */


bool dp::m2f::IsSymmetrical(void) const
{
  // Check upper triangle elements against lower triangle counterparts
  return (col[1][0] == col[0][1]);
} // end bool m2f::IsSymmetrical(void) const                                  */


bool dp::m2f::IsUpperTriangular(void) const
{
  return  (IsInF(col[0][1], _INf, -_INf));
} // end bool m2f::IsUpperTriangular(void) const                              */


std::string dp::m2f::Json(void) const
{
  m2f mt(col[0][0], col[0][1], col[1][0], col[1][1]);
  return std::string("[" + mt[0].JSON() + "," + mt[1].JSON() + "]");
} // end std::string m2f::Json(void) const                                    */


dp::m2f& dp::m2f::Set(const m2f& source)
{
  // Just copy each column vector: no other members should be defined in m2f
  for (s1 i = 0; i < SQUBS; ++i) { col[i] = source.col[i]; }
  return *this;
} // end m2f& m2f::Set(const m2f&)                                            */


std::string dp::m2f::ToString(bool lineBreaks) const
{
  m2f mt(this->Transposed());
  std::string result("[" + mt[0].JSON());
  for (u1 i = 1; i < SQUBS; ++i)
  {
    result += ", ";
    result += ((lineBreaks) ? "\n " : "") + mt[i].JSON();
  }
  result += "]";
  return result;
} // end std::string m2f::ToString(void) const                                */


f4 dp::m2f::Trace(bool product) const
{
  return (product) ? (col[0][0]) * (col[1][1]) : (col[0][0]) + (col[1][1]);
} // end f4 m2f::Trace(void) const                                            */


dp::m2f& dp::m2f::Transpose(void)
{
  SwapF(col[1][0], col[0][1]);
  return *this;
} // end m2f& m2f::Transpose(void)                                            */


dp::m2f dp::m2f::Transposed(void) const
{
  // Quicker to just write and return the copy from existing values
  return m2f( col[0][0], col[0][1], col[1][0], col[1][1] );
} // end m2f m2f::Transposed(void) const                                      */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::m2f& dp::m2f::operator=(const m2f& source)
{ return this->Set(source); } // end m2f& m2f::operator=(const m2f&)          */


dp::m2f& dp::m2f::operator=(m2f&& result) noexcept
{
  for (u1 i = 0; i < SQUBS; ++i) { col[i] = std::move(result.col[i]); }
  return *this;
} // end m2f& m2f::operator=(m2f&&) noexcept                                  */


dp::m2f dp::m2f::operator-(void) const
{
  return m2f(-col[0][0],-col[1][0],-col[0][1],-col[1][1]);
} // end m2f m2f::operator-(void) const                                       */


dp::m2f dp::m2f::operator!(void) const
{ return this->Inverse(); } // end m2f m2f::operator!(void)                   */


const dp::v2f& dp::m2f::operator[](u4 subscript) const
{
  subscript &= v2f::VSUBS_MASK; // Using v2f array and has N^2 dimensions
  return col[subscript];
} // end const v2f& m2f::operator[](u4) const                                 */


dp::v2f& dp::m2f::operator[](u4 subscript)
{
  subscript &= v2f::VSUBS_MASK; // Using v2f array and has N^2 dimensions
  return col[subscript];
} // end v2f& m2f::operator[](u4)                                             */


dp::m2f dp::m2f::operator+(const m2f& other) const
{
  m2f sum(*this);
  for (s1 i = 0; i < SQUBS; ++i) { sum[i] += other[i]; }
  return sum;
} // end m2f m2f::operator+(const m2f&)                                       */


dp::m2f& dp::m2f::operator+=(const m2f& other)
{
  for (s1 i = 0; i < SQUBS; ++i) { col[i] += other[i]; }
  return *this;
} // end m2f& m2f::operator+=(const m2f&)                                     */


dp::m2f dp::m2f::operator-(const m2f& other) const
{
  m2f difference(*this);
  for (s1 i = 0; i < SQUBS; ++i) { difference[i] -= other[i]; }
  return difference;
} // end m2f m2f::operator-(const m2f&)                                       */


dp::m2f& dp::m2f::operator-=(const m2f& other)
{
  for (s1 i = 0; i < SQUBS; ++i) { col[i] -= other[i]; }
  return *this;
} // end m2f& m2f::operator-=(const m2f&)                                     */


dp::m2f dp::m2f::operator*(f4 scalar) const
{
  m2f scaled(*this);
  scaled *= scalar;
  return scaled;
} // end m2f m2f::operator*(f4) const                                         */


dp::v2f dp::m2f::operator*(const v2f& vector) const
{
  m2f mt(col[0][0], col[0][1], col[1][0], col[1][1]);
  return v2f((mt[0].Dot(vector)), (mt[1].Dot(vector)));
} // end v2f m2f::operator*(const v2f&) const                                 */


dp::m2f& dp::m2f::operator*=(f4 scalar)
{
  for (s1 i = 0; i < SQUBS; ++i) { col[i] *= scalar; }
  return *this;
} // end m2f& m2f::operator*=(f4)                                             */


dp::m2f dp::m2f::operator*(const m2f& other) const
{
  m2f concat(*this);
  concat *= other;
  return concat;
} // end m2f m2f::operator*(const m2f&) const                                 */


dp::m2f& dp::m2f::operator*=(const m2f& other)
{
  m2f mc(*this);  // Matrix copy (to not work with shifting values)
  m2f ot = other.Transposed(); // Transposed other (for vector-wise Dot())

  for (s1 i = 0; i < SQUBS; ++i)
  {
    for (s1 j = 0; j < SQUBS; ++j) { col[i][j] = mc[i].Dot(ot[j]); }
  }
  return *this;
} // end m2f& m2f::operator*=(const m2f&)                                     */


dp::m2f dp::m2f::operator/(f4 inverseScalar) const
{
  f4 scalar = (1.0f / inverseScalar);
  return ((*this) * scalar);
} // end m2f m2f::operator/(f4) const                                         */


dp::m2f& dp::m2f::operator/=(f4 inverseScalar)
{
  f4 scalar = (1.0f / inverseScalar);
  return ((*this) *= scalar);
} // end m2f& m2f::operator/=(f4)                                             */


dp::m2f::operator std::string(void) const
{ return this->ToString(true); } // end operator std::string(void)            */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Functions                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::m2f dp::operator*(f4 scalar, const dp::m2f& rhs)
{
  return (rhs * scalar);
} // end m2f operator*(f4, const m2f&) left hand scalar multiplication        */


bool dp::operator==(const m2f& lhs, const m2f& rhs)
{
  // Check each column vector for equality
  for (s1 i = 0; i < m2f::SQUBS; ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }
  return true; // If no column was inequal, all were equal, so lhs == rhs
} // end bool operator==(const m2f&, const m2f&) const                        */


bool dp::operator!=(const m2f& lhs, const m2f& rhs)
{
  // Check each column vector for inequality
  for (s1 i = 0; i < m2f::SQUBS; ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }
  return false; // If no column was inequal, all were equal, so !(lhs == rhs)
} // end bool operator!=(const m2f&, const m2f&) const                        */


std::ostream& dp::operator<<(std::ostream& output, const m2f& matrix)
{
  output << matrix.ToString(true);
  return output;
} // end std::ostream& operator<<(std::ostream&, const m2f&)                  */
