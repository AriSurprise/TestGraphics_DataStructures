/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  vNc.cpp
Purpose:  4-byte float-point array wrapper class for arbitrary dimensional math
Language: C++ / Eng, MSVC 19 (used, but should work with most compilers)
Platform: MSVC 19 / Windows 10 x64 (used, but should have no requirements)
Project:  a.Surprise_MAT300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Fall 2021
Copyright (C) 2021 DigiPen Institute of Technology.
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
#include "Value.h"  // Basic value comparisons: min, max, clamp, near, etc
#include "vNc.h"    // Class declaration header
#include "vNf.h"    // Inner product results in list of terms over the real


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create vector of the given dimensions initialized to 0.0f per element      */
dp::vNc::vNc(u4 dimensions)
  : dimens(dimensions)
{
  if (dimens != 0)
  {
    elem = new cplx[dimens];
    if (elem)
    {
      for (size_t i = 0; i < dimens; ++i) { elem[i] = nullcplx; }
    }
    else
    {
      Log::Error("vNc(u4) - allocation failed for "
        + std::to_string(dimens) + "element list");
      dimens = 0;
      elem = nullptr;
    }
  }
  else // null vector requested
  {
    elem = nullptr;
  }

}// end vNc(u4)


// Create an identity vector for [column] into a fourier transform's basis    */
dp::vNc::vNc(u4 dimensions, u4 column)
  : dimens(dimensions)
{
  if (dimens != 0)
  {
    elem = new cplx[dimens];
    if (elem)
    {
      //if (column >= dimens) { column %= dimens; } // not strictly necessary
      // w ('omega') =: anglular velocity (physical relevance; physics symbols)
      ang w = ang(TAU / dimens, arc::radians) * static_cast<f4>(column);
      // set each term to (e^(i*w))^k
      for (size_t k = 0; k < dimens; ++k)
      {
        elem[k] = cplx((w * static_cast<f4>(k)));
      }
    }
    else
    {
      Log::Error("vNc(u4) - allocation failed for "
        + std::to_string(dimens) + "element list");
      dimens = 0;
      elem = nullptr;
    }
  }
  else // null vector requested
  {
    elem = nullptr;
  }

}// end vNc(u4, u4)


// Creates an n-D vector of f4 to hold (up to n), f4, from 8 byte floats      */
dp::vNc::vNc(std::initializer_list<cplx> init)
  : dimens(init.size())
{
  elem = new cplx[dimens];
  if (elem)
  {
    for (size_t i = 0; i < dimens; ++i)
    {
      elem[i] = (*(init.begin() + i));
    }
  }
  else
  {
    Log::Error("vNc(initializer_list<cplx>) - allocation failed for "
      + std::to_string(dimens) + "element list");
    dimens = 0;
    elem = nullptr;
  }
}// end vNc(std::initializer_list<f4>)


// Creates an n dimensional vector from another source n dimensional vNc      */
dp::vNc::vNc(const vNc& source)
  : dimens(source.dimens)
{
  elem = new cplx[dimens];
  if (elem)
  {
    for (size_t i = 0; i < dimens; ++i) { elem[i] = source.elem[i]; }
  }
  else
  {
    Log::Error("vNc(const vNc&) - allocation failed for "
      + std::to_string(dimens) + "element list");
    dimens = 0;
    elem = nullptr;
  }
} // end vNc(const vNc&)


// Creates a vNc from a vNc value reference resultant from a function         */
dp::vNc::vNc(vNc&& result) noexcept
  : dimens(std::move(result.dimens)), elem(std::move(result.elem))
{} // end vNc(vNc&&) noexcept


// Clean up vector data before going out of scope                             */
dp::vNc::~vNc(void)
{
  if (elem)
  {
    delete[] elem;
    elem = nullptr;
  }
  dimens = 0;
  // (elem == nullptr);

} // end ~vNc(void)

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Negate each member's imaginary term and return the resultant copy          */
dp::vNc dp::vNc::Compliment(void) const
{
  vNc complement(*this);
  for (u4 i = 0; i < complement.dimens; ++i)
  {
    complement.elem[i].imag = -complement.elem[i].imag;
  }
  return complement;

} // end vNc Compliment(void) const


// Calculate the elementwise product of (this[i] * !other[i]) in a new vNc    */
dp::vNc dp::vNc::ConjugateProduct(const vNc& other) const
{
  size_t longer = Max<size_t>(dimens, other.dimens);
  size_t shorter = Min<size_t>(dimens, other.dimens);
  vNc result(static_cast<u4>(longer));
  for (size_t i = 0; i < shorter; ++i)
  {
    result.elem[i] = elem[i] * other.elem[i].Conjugate(); // set products
  }
  for (size_t i = shorter; i < longer; ++i)
  {
    result.elem[i] = nullcplx; // 0*x = 0 = x*0 for any length discrepancies
  }
  return result;

} // end vNc vNc::ConjugateProduct(const vNC&) const


// Format a vector for being output as a comma separated value list           */
std::string dp::vNc::CSV(bool pack) const
{
  std::string data;
  if (!elem) { data = "Empty vector"; }
  else
  {
    if (pack)
    {
      data = (elem[0].JSON());
      for (size_t i = 1; i < dimens; ++i)
      {
        data += "," + elem[i].JSON();
      }
    }
    else
    {
      data = ((elem[0].R() >= 0.0f) ? " " : "");
      data += elem[0].String();
      for (size_t i = 1; i < dimens; ++i)
      {
        data += ((elem[i].R() >= 0.0f) ? ", " : ",");
        data += elem[i].String();
      }
    }
  }
  return data;

} // end std::string CSV(bool) const


// Get the current dimensional length / element count of the vector           */
size_t dp::vNc::Dimens(void) const
{ return dimens; } // end size_t Dimens(void) const


// Calculate the dot product of (other * this) vector (product sum)           */
dp::cplx dp::vNc::InnerProduct(const vNc& other) const
{
  cplx productSum = 0.0f;
  vNc product = this->ConjugateProduct(other);
  for (size_t i = 0; i < dimens; ++i) { productSum += (product.elem[i]); }
  return productSum;

} // end f4 Dot(const vNc&) const


// Get vector length: sqrt([0]^2 + [1]^2 + ... + [n-2]^2 + [n-1]^2)           */
f4 dp::vNc::Length(void) const
{ return static_cast<f4>(sqrt(LengthSquared())); } // end f4 Length(void) const


//  Get the squared vector length: [0]^2 + [1]^2 + ... + [n-2]^2 + [n-1]^2    */
f4 dp::vNc::LengthSquared(void) const
{
  // Any complex number times its own conjugate =: ((r^2+i^2)+0i) (i's cancel)
  // sum of squares of terms is defined as squared vector length ==>
  return InnerProduct(*this).real; // imag of this->Compliment must = 0 => Real

} // end f4 LengthSquared(void) const


// This vNc will be divided by its length and converted to a unit vector      */
dp::vNc& dp::vNc::Normalize(void)
{
  *this /= Length();
  return *this;

} // end vNc& Normalize(void)


// Calculate the elementwise product of (other[i] * this[i]) in a new vNc     */
dp::vNc dp::vNc::ProductWith(const vNc& other) const
{
  size_t longer = Max<size_t>(dimens, other.dimens);
  size_t shorter = Min<size_t>(dimens, other.dimens);
  vNc result(static_cast<u4>(longer));
  for (size_t i = 0; i < shorter; ++i)
  {
    result.elem[i] = elem[i] * other.elem[i]; // set products
  }
  for (size_t i = shorter; i < longer; ++i)
  {
    result.elem[i] = 0.0f; // 0 * x = 0 = x * 0 for any length discrepancies
  }
  return result;

} // end vNc ProductWith(const vNc&) const


dp::vNc& dp::vNc::Resize(size_t dimensions)
{
  if (dimensions != dimens)
  {
    if (dimensions == 0) // make null vector
    {
      delete[] elem;
      dimens = 0;
      elem = nullptr;
    }
    else
    {
      cplx* oldElem = elem;
      elem = new cplx[dimensions];
      if (!elem)
      {
        Log::Error("vNc::Resize - new vector allocation failed");
        elem = oldElem; // old allocation still good...recycling is preferable?
      }
      else
      {
        if (dimens < dimensions) // elongation request
        {
          for (size_t i = 0; i < dimens; ++i)
          {
            elem[i] = oldElem[i];
          }
          for (size_t i = dimens; i < dimensions; ++i)
          {
            elem[i] = 0.0f;
          }
        }
        else // truncation request
        {
          // TODO: downscaling resize shouldn't require reallocaction or copy
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
} // end vNc& Resize(size_t)


// Set the elementwise product of (other[i] * this[i]) into this vector       */
dp::vNc& dp::vNc::SetProduct(const vNc& other)
{
  size_t shorter = Min<size_t>(dimens, other.dimens);
  if (dimens < other.dimens) { Resize(other.dimens); }
  for (size_t i = 0; i < shorter; ++i) { elem[i] *= (other.elem[i]); }
  return *this;

} // end vNc& vNc::SetProduct(const vNc&)


// Value copy will be divided by its length and converted to a unit vector    */
dp::vNc dp::vNc::UnitVec(void) const
{
  return vNc(*this).Normalize();
} // end vNc UnitVec(void) const

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Sets the source vNc member values into this vNc value fields               */
dp::vNc& dp::vNc::operator=(const vNc& source)
{
  if (elem) { delete[] elem; }
  dimens = source.dimens;
  elem = new cplx[dimens];
  if (elem)
  {
    for (size_t i = 0; i < dimens; ++i) { elem[i] = source.elem[i]; }
  }
  return *this;
} // end vNc& operator=(const vNc&)


// Surrogate a vNc value reference returned from a function for this vNc      */
dp::vNc& dp::vNc::operator=(vNc&& result) noexcept
{
  dimens = std::move(result.dimens);
  elem = std::move(result.elem);
  return *this;
} // end vNc& operator=(vNc&& result) noexcept


// Get the value from a given [0, n-1] index subscript of the vector          */
const dp::cplx& dp::vNc::operator[](size_t index) const
{
  if (index >= dimens) { index %= dimens; }
  return elem[index];
} // end f4 operator[](u2 index) const


// Get a reference to the given [0, n-1] index subscript of the vector        */
dp::cplx& dp::vNc::operator[](size_t index)
{
  if (index >= dimens) { index %= dimens; }
  return elem[index];
} // end f4& operator[](u2 index)


// Negates each imaginary value: for each { v[j] = <v[j].r, -v[j].i> }        */
dp::vNc dp::vNc::operator!(void) const
{
  vNc result(*this);
  for (size_t i = 0; i < dimens; ++i)
  {
    if (elem[i] != 0.0f) { result.elem[i].imag = -result.elem[i].imag; }
  }
  return result;

} // end vNc& operator!(void)


// Create a negated vector with negated copies of each element value          */
dp::vNc dp::vNc::operator-(void) const
{
  vNc result(*this);
  for (size_t i = 0; i < dimens; ++i)
  {
    if (elem[i] != 0.0f) { result.elem[i].real = -result.elem[i].real; }
    if (elem[i] != 0.0f) { result.elem[i].imag = -result.elem[i].imag; }
  }
  return result;

} // end vNc operator-(void) const


// Calculate the member-wise difference of 2 vNc's and return the result      */
dp::vNc dp::vNc::operator-(const vNc& other) const
{
  vNc result(((dimens >= other.dimens) ? *this : other));
  if (result.dimens > 0)
  {
    if (result.dimens == dimens) { result -= other; } // *this used as basis
    else { result -= *this; } // other used as result's basis
  }
  return result;
} // end vNc operator-(const vNc&) const


// Add an other vNc's values memberwise into this vNc's values                */
dp::vNc& dp::vNc::operator-=(const vNc& other)
{
  size_t shorter = Min<size_t>(dimens, other.dimens);
  for (size_t i = 0; i < shorter; ++i) { elem[i] -= other.elem[i]; }
  return *this;
} // end vNc& operator-=(const vNc&)


// Calculate the member-wise addition of 2 vNc's and return the result        */
dp::vNc dp::vNc::operator+(const vNc& other) const
{
  vNc result(((dimens >= other.dimens) ? *this : other));
  if (result.dimens > 0)
  {
    if (result.dimens == dimens) { result += other; } // *this used as basis
    else { result += *this; } // other used as result's basis
  }
  return result;
} // end vNc operator+(const vNc&) const


// Add an other vNc's values memberwise into this vNc's values                */
dp::vNc& dp::vNc::operator+=(const vNc& other)
{
  size_t shorter = Min<size_t>(dimens, other.dimens);
  for (size_t i = 0; i < shorter; ++i) { elem[i] += other.elem[i]; }
  return *this;
} // end vNc& operator+=(const vNc&)


// Scalar product multiplies each field uniformly by a scalar constant        */
dp::vNc dp::vNc::operator*(f4 scalar) const
{
  vNc product(*this);
  for (u4 i = 0; i < product.dimens; ++i) { product.elem[i] *= scalar; }
  return (scalar * (*this));

} // end vNc& operator-=(const vNc&)


// Scalar product sets all members to grow uniformly by a scalar constant     */
dp::vNc& dp::vNc::operator*=(f4 scalar)
{
  for (size_t i = 0; i < dimens; ++i) { elem[i] *= scalar; }
  return *this;

} // end vNc& operator*=(f4)


// Take others memberwise conjugate products with this, and sum the result    */
dp::cplx dp::vNc::operator*(const vNc& other) const
{
  cplx productSum = 0.0f;
  vNc product = this->ProductWith(other.Compliment());
  for (size_t i = 0; i < dimens; ++i) { productSum += (product.elem[i]); }
  return productSum;

} // end cplx operator*(const vNc&) const


// Perform uniform division by divisor, returning the result                  */
dp::vNc dp::vNc::operator/(f4 divisor) const
{
  if (divisor == 0.0f)
  {
    Log::Warn("vNc::operator/= - division by 0 request ignored");
    return *this;
  }
  return *this * (1.0f / divisor);

} // end vNc operator/(f4) const


// Divides all members uniformly by divisor, setting the result               */
dp::vNc& dp::vNc::operator/=(f4 divisor)
{
  if (divisor == 0.0f)
  {
    Log::Warn("vNc::operator/= - division by 0 request ignored");
    return *this;
  }
  return *this *= (1.0f / divisor);

} // end vNc& operator/=(f4)


// Output ordered vector element values with brackets, commas and spaces      */
dp::vNc::operator std::string(void) const
{
  return ("[" + CSV(false) + "]");

} // end operator string(void) const


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Overloads                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Left hand scalar multiplication scales each element of rhs uniformly       */
dp::vNc dp::operator*(f4 scalar, const dp::vNc& rhs)
{
  vNc result(rhs);
  result *= scalar;
  return result;

} // end vNc operator*(f4, const dp::vNc&)


// Confirm if 2 vectors are relatively equal in all elements per dimension    */
bool dp::operator==(const vNc& lhs, const vNc& rhs)
{
  if (lhs.Dimens() == rhs.Dimens())
  {
    for (size_t i = 0; i < lhs.Dimens(); ++i)
    {
      if (!Near<f4>(lhs[i].real, rhs[i].real, 0.001f)
       || !Near<f4>(lhs[i].imag, rhs[i].imag, 0.001f)) { return false; }
    }
    return true;
  }
  return false;

} // end bool operator==(const vNc&, const vNc&)


// Confirm if 2 vectors are relatively equal in all elements per dimension    */
bool dp::operator!=(const vNc& lhs, const vNc& rhs)
{
  return !(lhs == rhs);

} // end bool operator!=(const vNc&, const vNc&)
