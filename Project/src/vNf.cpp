/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  vNf.cpp
Purpose:  4-byte float-point array wrapper class for arbitrary dimensional math
Language: C++ / Eng, MSVC 19 (used, but should work with most compilers)
Platform: MSVC 19 / Windows 10 x64 (used, but should have no requirements)
Project:  a.Surprise_MAT300_1
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

// <stl>
#include <cstdarg>  // Variadic parameter member settings
#include <ostream>  // Output stream definition for operator overload
// "./src/..."
#include "Log.h"    // System message management for errors, warnings, etc
#include "mNf.h"    // N-D vector dot transpose to get symmetrical matrix
#include "Value.h"  // Basic value comparisons: min, max, clamp, near, etc
#include "vNf.h"    // Class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::vNf::vNf(u4 dimensions, f4 value)
  : dimens(dimensions)
{
  if (dimens != 0)
  {
    elem = new f4[dimens];
    if (elem)
    {
      for (size_t i = 0; i < dimens; ++i) { elem[i] = value; }
    }
    else
    {
      Log::Error("vNf(u4) - allocation failed for "
        + std::to_string(dimens) + "element list");
      dimens = 0;
    }
  }
  else { elem = nullptr; } // null vector requested

} // end vNf::vNf(u4 dimensions)                                              */


dp::vNf::vNf(const std::vector<f4>& init)
  : dimens(static_cast<u4>(init.size()))
{
  elem = new f4[dimens];
  if (elem)
  {
    for (u4 i = 0; i < dimens; ++i)
    {
      elem[i] = static_cast<f4>(*(init.begin() + i));
    }
  }
  else
  {
    Log::Error("vNf(vector<f4>) - allocation failed for "
      + std::to_string(dimens) + " element list");
    dimens = 0;
  }
}
// end vNf(std::initializer_list<f4>)                                         */


// Creates an n-D vector of f4 to hold (up to n), f4, from 8 byte floats      */
dp::vNf::vNf(std::initializer_list<f4> init)
  : dimens(static_cast<u4>(init.size()))
{
  elem = new f4[dimens];
  if (elem)
  {
    for (u4 i = 0; i < dimens; ++i)
    {
      elem[i] = static_cast<f4>(*(init.begin() + i));
    }
  }
  else
  {
    Log::Error("vNf(initializer_list<f4>) - allocation failed for "
      + std::to_string(dimens) + " element list");
    dimens = 0;
  }
}// end vNf(std::initializer_list<f4>)


// Creates an n dimensional vector from another source n dimensional vNf      */
dp::vNf::vNf(const vNf& source)
  : dimens(source.dimens)
{
  elem = new f4[dimens];
  if (elem)
  {
    for (size_t i = 0; i < dimens; ++i) { elem[i] = source.elem[i]; }
  }
  else
  {
    Log::Error("vNf(const vNf&) - allocation failed for "
      + std::to_string(dimens) + "element list");
    dimens = 0;
  }
} // end vNf(const vNf&)


// Creates a vNf from a vNf value reference resultant from a function         */
dp::vNf::vNf(vNf&& result) noexcept
  : dimens(std::move(result.dimens)), elem(std::move(result.elem))
{} // end vNf(vNf&&) noexcept


// Clean up vector data before going out of scope                             */
dp::vNf::~vNf(void)
{
  if (elem)
  {
    delete[] elem;
    elem = nullptr;
  }
  dimens = 0;
} // end ~vNf(void)

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

std::string dp::vNf::ClipString(bool pack) const
{
  if (pack) { return "<"+CSVString(1)+">"; }
  else if (Dimens() > 0 && elem[0] < 0.0f) { return "<"+ CSVString(0) +" >"; }
  else { return "< " + CSVString(0) + " >"; }

} // end string vNf::ClipString(bool) const                                   */


std::string dp::vNf::CSVString(bool pack) const
{
  std::string data;
  if (!elem) { data = "null"; }
  else
  {
    if (pack)
    {
      for (size_t i = 0; i < dimens; ++i)
      {
        data = std::to_string((IsIntegralF(elem[i])?
                               FloorF(elem[i]) : elem[i]));
      }
    }
    else // if adding padding spaces between values
    {
      // Don't pad in excess of 1 char so more fits on a line, and signs match
      data = ((elem[0] >= 0.0f) ? " " : "");
      if (IsIntegralF(elem[0])) { data += FloorF(elem[0]);  }
      else { data += std::to_string(elem[0]); }
      for (size_t i = 1; i < dimens; ++i)
      {
        // Still don't pad in excess so more fits on a line, and signs match
        data += ((elem[i] >= 0.0f) ? ", " : ",");
        if (IsIntegralF(elem[i])) { data += FloorF(elem[i]); }
        else { data += std::to_string(elem[i]); }
      }
    }
  }
  return data;

} // end std::string CSV(bool) const                                          */


f4 dp::vNf::Dot(const vNf& other) const
{
  f4 prodSum = 0.0f;
  vNf product = Multiply(other);
  for (u4 i = 0; i < dimens; ++i) { prodSum += (product.elem[i]); }
  return prodSum;

} // end f4 Dot(const vNf&) const                                             */

dp::mNf dp::vNf::DotTranspose(void) const
{
  mNf result(Dimens());
  dp::Log::Info(std::to_string(result.Dimens()));
  for (u4 c = 0; c < result.Dimens(); ++c)
  {
    for (u4 r = 0; r < result.Dimens(); ++r)
    {
      result[c][r] = elem[c] * elem[r];
    }
  }
  return result;
} // end mNf vNf::DotTranspose(void) const                                    */

bool dp::vNf::IsNull(void) const
{
  if (IsEmpty()) { return true; } // Validate / Quick out: empty set => null
  // Check for any non-zero elements: any non-zero term => vector is non-null
  for (u4 i = 0; i < dimens; ++i) { if (elem[i] != 0.0f) { return false; } }
  return true; // If no non-zero elements were found => null vector

} // end bool vNf::IsNull(void) const                                         */


f4 dp::vNf::Length(void) const
{ return SqrtF(Dot(*this)); } // end f4 Length(void) const                    */


f4 dp::vNf::LengthSquared(void) const
{
  f4 productSum = Dot(*this);
  return productSum;

} // end f4 LengthSquared(void) const                                         */


dp::vNf dp::vNf::Multiply(const vNf& other) const
{
  u4 longer = MaxU(dimens, other.dimens);
  u4 shorter = MinU(dimens, other.dimens);
  vNf result(static_cast<u4>(longer));
  for (u4 i = 0; i < shorter; ++i)
  {
    result.elem[i] = elem[i] * other.elem[i]; // set products
  }
  for (u4 i = shorter; i < longer; ++i)
  {
    result.elem[i] = 0.0f; // 0 * x = 0 = x * 0 for any length discrepancies
  }
  return result;

} // end vNf Multiply(const vNf&) const                                       */


dp::vNf& dp::vNf::MultiplyWith(const vNf& other)
{
  u4 shorter = MinU(dimens, other.dimens);
  if (dimens < other.dimens) { Resize(other.dimens); }
  for (size_t i = 0; i < shorter; ++i) { elem[i] *= (other.elem[i]); }
  return *this;

}// end vNf& MultiplyWith(const vNf&)                                         */


dp::vNf& dp::vNf::Normalize(void)
{
  *this /= Length();
  return *this;

} // end vNf& Normalize(void)                                                 */


std::string dp::vNf::PointString(bool pack) const
{
  if (pack) { return "(" + CSVString(1) + ")"; }
  else if (Dimens() > 0 && elem[0] < 0.0f) { return "("+ CSVString(0) +" )"; }
  else { return "( " + CSVString(0) + " )"; }

} // end string vNf::PointString(bool) const                                  */


dp::vNf& dp::vNf::Resize(u4 dimensions, f4 value)
{
  if (dimensions != dimens)
  {
    if (dimensions == 0) // make null vector
    {
      delete[] elem;
      elem = nullptr;
      dimens = 0;
    }
    else
    {
      f4* oldElem = elem;
      elem = new f4[dimensions];
      if (!elem)
      {
        Log::Error("vNf::Resize - new vector allocation failed");
        elem = oldElem; // old allocation still good...recycling is preferable?
      }
      else
      {
        if (dimens < dimensions) // elongation request
        {
          for (u4 i = 0; i < dimens; ++i)
          {
            elem[i] = oldElem[i];
          }
          for (u4 i = dimens; i < dimensions; ++i)
          {
            elem[i] = value;
          }
        }
        else // truncation request
        {
          for (size_t i = 0; i < dimensions; ++i)
          {
            elem[i] = oldElem[i];
          }
        }
        dimens = dimensions;
        delete[] oldElem;
      }
    }
  }
  return *this;

} // end vNf& Resize(size_t)                                                  */


dp::vNf dp::vNf::UnitVec(void) const
{
  return vNf(*this).Normalize();

} // end vNf UnitVec(void) const                                              */


std::string dp::vNf::VecString(bool pack) const
{
  if (pack) { return "<" + CSVString(1) + ">"; }
  else if (Dimens() > 0 && elem[0] < 0.0f) { return "<"+ CSVString(0) +" >"; }
  else { return "< " + CSVString(0) + " >"; }

} // end string vNf::VecString(bool) const                                    */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create a negated vector with negated copies of each element value          */
dp::vNf dp::vNf::operator-(void) const
{
  vNf result(*this);
  !result;
  return result;
} // end vNf operator-(void) const


// Sets the source vNf member values into this vNf value fields               */
dp::vNf& dp::vNf::operator=(f4 value)
{
  if (elem) { delete[] elem; }
  dimens = 1;
  elem = new f4[dimens];
  if (elem) { elem[0] = value; }
  else { Log::Error("vNf::operator=(f4) - bad alloc"); }
  return *this;
} // end vNf& operator=(const vNf&)


// Sets the source vNf member values into this vNf value fields               */
dp::vNf& dp::vNf::operator=(const vNf& source)
{
  if (elem) { delete[] elem; }
  dimens = source.dimens;
  elem = new f4[dimens];
  if (elem)
  {
    for (size_t i = 0; i < dimens; ++i) { elem[i] = source.elem[i]; }
  }
  return *this;
} // end vNf& operator=(const vNf&)


// Surrogate a vNf value reference returned from a function for this vNf      */
dp::vNf& dp::vNf::operator=(vNf&& result) noexcept
{
  dimens = std::move(result.dimens);
  elem = std::move(result.elem);
  return *this;
} // end vNf& operator=(vNf&& result) noexcept


// Negates each element value: for each { element[i] = -element[i]; }         */
dp::vNf& dp::vNf::operator!(void)
{
  for (size_t i = 0; i < dimens; ++i)
  {
    if (elem[i] != 0.0f) { elem[i] = -elem[i]; }
  }
  return *this;
} // end vNf& operator!(void)


// Get the value from a given [0, n-1] index subscript of the vector          */
f4 dp::vNf::operator[](size_t index) const
{
  if (index >= dimens) { index %= dimens; }
  return elem[index];
} // end f4 operator[](u2 index) const


// Get a reference to the given [0, n-1] index subscript of the vector        */
f4& dp::vNf::operator[](size_t index)
{
  if (index >= dimens) { index %= dimens; }
  return elem[index];
} // end f4& operator[](u2 index)


// Calculate the member-wise addition of 2 vNf's and return the result        */
dp::vNf dp::vNf::operator+(const vNf& other) const
{
  vNf result(((dimens >= other.dimens) ? *this : other));
  if (result.dimens > 0)
  {
    if (result.dimens == dimens) { result += other; } // *this used as basis
    else { result += *this; } // other used as result's basis
  }
  return result;
} // end vNf operator+(const vNf&) const


// Add an other vNf's values memberwise into this vNf's values                */
dp::vNf& dp::vNf::operator+=(const vNf& other)
{
  u4 shorter = MinU(dimens, other.dimens);
  for (u4 i = 0; i < shorter; ++i) { elem[i] += other.elem[i]; }
  return *this;
} // end vNf& operator+=(const vNf&)


// Calculate the member-wise difference of 2 vNf's and return the result      */
dp::vNf dp::vNf::operator-(const vNf& other) const
{
  vNf result(((dimens >= other.dimens) ? *this : other));
  if (result.dimens > 0)
  {
    if (result.dimens == dimens) { result -= other; } // *this used as basis
    else { result -= *this; } // other used as result's basis
  }
  return result;
} // end vNf operator-(const vNf&) const


// Add an other vNf's values memberwise into this vNf's values                */
dp::vNf& dp::vNf::operator-=(const vNf& other)
{
  u4 shorter = MinU(dimens, other.dimens);
  for (u4 i = 0; i < shorter; ++i) { elem[i] -= other.elem[i]; }
  return *this;
} // end vNf& operator-=(const vNf&)


// Scalar product multiplies each field uniformly by a scalar constant        */
dp::vNf dp::vNf::operator*(f4 scalar) const
{ return (scalar * (*this)); } // end vNf& operator-=(const vNf&)


// Scalar product sets all members to grow uniformly by a scalar constant     */
dp::vNf& dp::vNf::operator*=(f4 scalar)
{
  for (size_t i = 0; i < dimens; ++i) { elem[i] *= scalar; }
  return *this;
} // end vNf& operator*=(f4)


// Take equivalent other element products (0 if none), sum the result (dot)   */
f4 dp::vNf::operator*(const vNf& other) const
{ return Dot(other); } // end f4 operator*(const vNf&) const


// Perform uniform division by divisor, returning the result                  */
dp::vNf dp::vNf::operator/(f4 divisor) const
{
  if (divisor == 0.0f)
  {
    Log::Warn("vNf::operator/= - division by 0 request ignored");
    return *this;
  }
  return *this * (1.0f / divisor);

} // end vNf operator/(f4) const


// Divides all members uniformly by divisor, setting the result               */
dp::vNf& dp::vNf::operator/=(f4 divisor)
{
  if (divisor == 0.0f)
  {
    Log::Warn("vNf::operator/= - division by 0 request ignored");
    return *this;
  }
  return *this *= (1.0f / divisor);

} // end vNf& operator/=(f4)


// Output ordered vector element values with brackets, commas and spaces      */
dp::vNf::operator std::string(void) const
{ return ClipString(0); } // end operator string(void) const


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Overloads                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Left hand scalar multiplication scales each element of rhs uniformly       */
dp::vNf dp::operator*(f4 scalar, const dp::vNf& rhs)
{
  vNf result(rhs);
  result *= scalar;
  return result;
} // end vNf operator*(f4, const dp::vNf&)


// Confirm if 2 vectors are relatively equal in all elements per dimension    */
bool dp::operator==(const vNf& lhs, const vNf& rhs)
{
  if (lhs.Dimens() == rhs.Dimens())
  {
    for (size_t i = 0; i < lhs.Dimens(); ++i)
    {
      if (!NearF(lhs[i], rhs[i], _INf)) { return false; }
    }
    return true;
  }
  return false;
} // end bool operator==(const vNf&, const vNf&)


// Confirm if 2 vectors are relatively equal in all elements per dimension    */
bool dp::operator!=(const vNf& lhs, const vNf& rhs)
{ return !(lhs == rhs); } // end bool operator!=(const vNf&, const vNf&)
