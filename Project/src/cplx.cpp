/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  cplx.h
Purpose:  Real and imaginary, 4 byte float point coordinate pair container
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no platform requirements)
Project:  a.Surprise_MAT320_1
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
#include <ostream>      // Output stream definition for operator overload
// "./src/..."
#include "ang.h"        // Unit measures for conversion in and out of arc spans
#include "ASCIIValue.h" // char range based [] operator manipulation
#include "Log.h"        // System message management for errors, warnings, etc
#include "mat4.h"       // Generating matrices from vector data
#include "Value.h"      // NearF() float point (proximity as equality) function
#include "cplx.h"       // Class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

static const u1 SUBSCRIPTS = 2u;    //! cplx class is fixed at 2 subscripts
static const f4 PROXIMITY = 0.001f; //! Reasonable near offset margin

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Creates a 2 dimensional complex number (<real, imaginary>, default <1,0>)  */
dp::cplx::cplx(f4 xu_value, f4 yv_value) : x(xu_value), y(yv_value)
{ } // end cplx::cplx(f4, f4)

// Creates a complex <real, imaginary> cartesian number from polar inputs     */
dp::cplx::cplx(ang rotation, f4 magnitude)
{
  Angle(rotation, magnitude);
} // end cplx::cplx(ang, f4)

// Creates a copy of a complex number from another source complex value       */
dp::cplx::cplx(const dp::cplx& source) : cplx(source.x, source.y)
{ } // end cplx::cplx(const cplx&)

// Creates a cplx from a cplx value reference resultant from a function       */
dp::cplx::cplx(dp::cplx&& result) noexcept
{
  *this = std::move(result);
} // end cplx::cplx(cplx&&)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get angle (arctangent), of current cartesian coordinates of this vector    */
dp::ang dp::cplx::Angle(void) const
{
  return ang(static_cast<f4>(atan2(y, x)), arc::radians, winding::ccw);

} // end v4f Angle(void) const


// Set cartesian <r,i> coordinates in vector from polar coordinate inputs     */
dp::cplx& dp::cplx::Angle(dp::ang angle_setting, f4 magnitude)
{
  X(static_cast<f4>(cos(angle_setting.r())) * magnitude);
  Y(static_cast<f4>(sin(angle_setting.r())) * magnitude);
  return *this;

} // end cplx& cplx::Angle(ang, f4)

// Complex number with negated imag value: (a+bi)(a-bi)=a^2+b^2+0i; i cancels */
dp::cplx dp::cplx::Conjugate(void) const
{
  return cplx(real, -imag);

} // end v4f Conjugate(void) const


// Get the complex number's imaginary component value                         */
const f4& dp::cplx::I(void) const
{
  return y;

} // end const f4& cplx::I(void) const


// Set the complex number's imaginary component value                         */
dp::cplx& dp::cplx::I(f4 ivy_value)
{
  y = ivy_value; return *this;

} // end cplx& cplx::I(f4)


// Get the complex number's imaginary component value                         */
const f4& dp::cplx::Imag(void) const
{
  return y;

} // end const f4& cplx::Imag(void) const


// Set the complex number's imaginary component value                         */
dp::cplx& dp::cplx::Imag(f4 ivy_value)
{
  y = ivy_value; return *this;

} // end cplx& cplx::Imag(f4)


// Get the complex number's imaginary component value                         */
const f4& dp::cplx::Imaginary(void) const
{
  return y;

} // end const f4& cplx::Imaginary(void) const


// Set the complex number's imaginary component value                         */
dp::cplx& dp::cplx::Imaginary(f4 ivy_value)
{
  y = ivy_value; return *this;

} // end cplx& cplx::Imaginary(f4)


// Format a cplx for being output as a packed vector notated string           */
std::string dp::cplx::JSON(void) const
{
  return "(" + std::to_string(x) + "," + std::to_string(y) + "*i)";

} // end std::string cplx::json(void) const


// Get the cplx diagonal length / displacement magnitude: sqrt(x^2 + y^2)     */
f4 dp::cplx::Length(void) const
{
  return static_cast<f4>(sqrt(static_cast<f8>(LengthSquared())));

} // end f4 cplx::Length(void) const


// Get squared complex displacement / diagonal length: real^2 + imag^2        */
f4 dp::cplx::LengthSquared(void) const
{
  return (real*real+imag*imag);

} // end f4 cplx::LengthSquared(void) const


// Calculate the elementwise product of (other * this) in a new v3f           */
dp::cplx dp::cplx::Multiply(const cplx& other) const
{
  cplx product(x * other.x, y * other.y);
  return product;

} // end v3f Multiply(const v3f&) const


// Set the elementwise product of (other * this) into this vector             */
dp::cplx& dp::cplx::MultiplyWith(const cplx& other)
{
  Set((x * other.x), (y * other.y));
  return *this;

} // end v3f& MultiplyWith(const v3f&)


// This cplx will be divided by its length and converted to a unit vector     */
dp::cplx& dp::cplx::Normalize(void)
{
  f4 len = Length();
  if (!Near<f4>(len, 0.0f, 0.001f))
  {
    f4 rMag = 1.0f / len; // Reciprocal vector magnitude
    *this *= rMag;
  }
  else // null vector can't be made a unit vector: default a value of x
  {
    Log::Info("cplx::Normalize - null vector input: default used");
    X(1.0f).Y(0.0f);
  }
  return *this;

} // end cplx& cplx::Normalize(void)


// Get the complex number's real component value                              */
const f4& dp::cplx::R(void) const
{
  return x;

} // end const f4& cplx::R(void) const


// Set the complex number's real component value                              */
dp::cplx& dp::cplx::R(f4 rux_value)
{
  x = rux_value; return *this;

} // end cplx& cplx::R(f4)


// Get the complex number's real component value                              */
const f4& dp::cplx::Real(void) const
{
  return x;

} // end const f4& cplx::Real(void) const


// Set the complex number's real component value                              */
dp::cplx& dp::cplx::Real(f4 rux_value)
{
  x = rux_value; return *this;

} // end cplx& cplx::Real(f4)


// Set the cplx to <x|u, y|v input values                                     */
dp::cplx& dp::cplx::Set(f4 xu_value, f4 yv_value)
{
  X(xu_value).Y(yv_value); return*this;

} // end cplx& cplx::Set(f4, f4)

// Set the complex cartesian equivalent value for the given polar components  */
dp::cplx& dp::cplx::Set(ang angle, f4 magnitude)
{
  Angle(angle, magnitude);
  return *this;

} // end cplx& cplx::Set(ang, f4)

// Set the cplx values to <source.x, source.y> values                         */
dp::cplx& dp::cplx::Set(const cplx& source)
{
  X(source.x).Y(source.y);
  return *this;

} // end cplx& cplx::Set(const cplx&)

// Set the cplx to use <result.x, result.y>                                    */
dp::cplx& dp::cplx::Set(cplx&& result) noexcept
{
  *this = std::move(result);
  return *this;

} // end cplx& cplx::Set(cplx&&) noexcept


// Format a v4f for being output as a vector notated string                   */
std::string dp::cplx::String(void) const
{
  std::string vecFormat;
  vecFormat = "[";
  if (x >= 0.0f) { vecFormat += " "; } // (add padding char for non-negatives)
  vecFormat += std::to_string(r) + ", ";
  if (y >= 0.0f) { vecFormat += " "; } // (add padding char for non-negatives)
  vecFormat += std::to_string(i) + "*i ]";
  return vecFormat;

} // end std::string cplx::String(void) const


// Get the complex number's real coordinate value under 2D texture labeling   */
const f4& dp::cplx::U(void) const
{
  return x;

} // end const f4& cplx::U(void) const


// Set the complex number's real coordinate value under 2D texture labeling   */
dp::cplx& dp::cplx::U(f4 xu_value)
{
  x = xu_value; return *this;
} // end cplx& cplx::U(f4)


// Value copy will be divided by its length and converted to a unit vector    */
dp::cplx dp::cplx::UnitVec(void) const
{
  cplx unit = *this; // Normalized copy of this
  unit.Normalize();
  return unit;

} // end cplx cplx::UnitVec(void) const


// Get the complex number's imaginary coord value under 2D texture labeling   */
const f4& dp::cplx::V(void) const
{
  return y;

} // end const f4& cplx::V(void) const


// Set the complex number's imaginary coord value under 2D texture labeling   */
dp::cplx& dp::cplx::V(f4 ivy_value)
{
  y = ivy_value;
  return *this;

} // end cplx&  cplx::V(f4)


// Get the complex number's real coordinate value under 2D cartesian labels   */
const f4& dp::cplx::X(void) const
{
  return x;

} // end const f4& cplx::X(void) const


// Set the complex number's real coordinate value under 2D cartesian labels   */
dp::cplx& dp::cplx::X(f4 rux_value)
{
  x = rux_value;
  return *this;

} // end cplx& cplx::X(f4)


// Get the complex number's imaginary coord value under 2D cartesian labels   */
const f4& dp::cplx::Y(void) const
{
  return y;

} // end const f4& cplx::Y(void) const


// Set the complex number's imaginary coord value under 2D cartesian labels   */
dp::cplx& dp::cplx::Y(f4 yv_value)
{
  y = yv_value;
  return *this;

} // end cplx&  cplx::Y(f4)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Creates a negated copy with relevant complex coordinate values             */
dp::cplx dp::cplx::operator-(void) const
{
  return cplx(-real, -imag);
} // end cplx cplx::operator-(void) const


// Sets the source cplx member values into this cplx value fields               */
dp::cplx& dp::cplx::operator=(const cplx& source)
{
  this->Set(source); return *this;
} // end cplx& cplx::operator=(const cplx&)


// Surrogate a complex value reference returned from a function for this      */
dp::cplx& dp::cplx::operator=(cplx&& result) noexcept
{
  real = std::move(result.real);
  imag = std::move(result.imag);
  return *this;
} // end cplx& cplx::operator=(const cplx&)


// Negates complex imaginary component to give the complex conjugate          */
dp::cplx dp::cplx::operator!(void) const
{
  return this->Conjugate();
} // end cplx& cplx::operator!(void)


// Get a value from ['r'] or ['i'] channels (or equivalent)                   */
f4 dp::cplx::operator[](s1 channel) const
{
  // Validate channel is either a 0-15 number, or ascii char from '  ' to '~'
  // clamp wildly out of range values into lowest 2 unsigned values
  if (!isascii(channel) /* 'not' 'msb not set' => not valid s1 range */
    || (channel > 0xF /* raw hex value input end range */
      && channel < 0x20  /* ascii control sequence end range */))
  {
    Log::Diag(std::string("cplx::operator[] const - value out of range '")
      + char(channel) + "' read, ", false);
    channel &= 1; // if clamp to bool happens, checks in elses may be skipped
    Log::W(std::string("setting to '") + std::to_string(channel) + "'", true);
  }
  else if (IsUpper(channel)) { channel = tolower(channel); }
  else if (IsDigit(channel)) { channel = ToNumeral(channel) & 1; }
  switch (channel) // Lookup array index equivalent for return
  {
  case 'r': case 'u': case 'x': case 0:
    return real;
  case 'i': case 'v': case 'y': case 1:
    return imag;
  default:
    Log::Diag(std::string("cplx::operator[] const - unsupported char ")
      + char(channel) + " read: default head 'r' == 0 used");
    return real;
  }
} // end f4 cplx::operator[](s1) const


// Get a value from ['r'] or ['i'] channels (or equivalent)                   */
f4& dp::cplx::operator[](s1 channel)
{
  // Validate channel is either a 0-15 number, or ascii char from '  ' to '~'
  // clamp wildly out of range values into lowest 2 unsigned values
  if (!isascii(channel) /* 'not' 'msb not set' => not valid s1 range */
    || (channel > 0xF /* raw hex value input end range */
      && channel < 0x20  /* ascii control sequence end range */))
  {
    Log::Diag(std::string("cplx::operator[] const - value out of range '")
      + char(channel) + "' read, ", false);
    channel &= 1; // if clamp to bool happens, checks in elses may be skipped
    Log::W(std::string("setting to '") + std::to_string(channel) + "'", true);
  }
  else if (IsUpper(channel)) { channel = tolower(channel); }
  else if (IsDigit(channel)) { channel = ToNumeral(channel) & 1; }
  switch (channel) // Lookup array index equivalent for return
  {
  case 'r': case 'u': case 'x': case 0:
    return real;
  case 'i': case 'v': case 'y': case 1:
    return imag;
  default:
    Log::Diag(std::string("cplx::operator[] - unsupported char ")
      + char(channel) + " read: default head 'r' == 0 used");
    return real;
  }
} // end f4& cplx::operator[](s1)


// Calculate the member-wise addition of 2 cplx's and return the result        */
dp::cplx dp::cplx::operator+(const cplx& other) const
{
  return cplx((x + other.x), (y + other.y));

} // end cplx cplx::operator+(const cplx&) const


// Add an other cplx's values memberwise into this cplx's values                */
dp::cplx& dp::cplx::operator+=(const cplx& other)
{
  Set((this->real + other.real), (this->imag + other.imag));
  return *this;

} // end cplx& cplx::operator+=(const cplx&)


// Calculate the member-wise difference of 2 cplx's and return the result      */
dp::cplx dp::cplx::operator-(const cplx& other) const
{
  return cplx((x - other.x), (y - other.y));

} // end cplx cplx::operator-(const cplx&) const


// Add an other cplx's values memberwise into this cplx's values                */
dp::cplx& dp::cplx::operator-=(const cplx& other)
{
  Set((x - other.x), (y - other.y));
  return *this;

} // end cplx& cplx::operator-=(const cplx&)


// Scalar product multiplies each field uniformly by a scalar constant        */
dp::cplx dp::cplx::operator*(f4 scalar) const
{
  return cplx((scalar * x), (scalar * y));
} // end cplx cplx::operator*(f4) const


// Scalar product sets all members to grow uniformly by a scalar constant     */
dp::cplx& dp::cplx::operator*=(f4 scalar)
{
  Set((scalar * x), (scalar * y));
  return *this;
} // end cplx& cplx::operator*=(f4)


// Get product of this and another complex number (resolving i = sqrt(-1))    */
dp::cplx dp::cplx::operator*(const cplx& other) const
{
  // resolve imaginary square into -real, scale linears by reciprocal reals
  return cplx( (r*other.r-i*other.i), (r*other.i+other.r*i));

} // end cplx cplx::operator*(const cplx&) const


// Set product of this and another complex number (resolving i = sqrt(-1))    */
dp::cplx& dp::cplx::operator*=(const cplx& other)
{
  // resolve imaginary square into -real, scale linears by reciprocal reals
  Set((r * other.r - i * other.i), (r * other.i + other.r * i));
  return *this;

} // end cplx& cplx::operator*=(const cplx&)


// Perform uniform division by divisor, returning the result                  */
dp::cplx dp::cplx::operator/(f4 divisor) const
{
  // Get reciprocal of input: multiplication is preferable computationally
  f4 scalar = (1.0f / divisor); // Inverse divisor: a scalar value
  return ((*this) * scalar);
} // end cplx cplx::operator/(f4) const


// Divides all members uniformly by divisor, setting the result               */
dp::cplx& dp::cplx::operator/=(f4 divisor)
{
  // Get reciprocal of input: multiplication is preferable computationally
  f4 scalar = (1.0f / divisor); // inverse divisor: a scalar value
  return ((*this) *= scalar);
} // end cplx cplx::operator/=(f4)


// Perform uniform division by divisor, returning the result                  */
dp::cplx dp::cplx::operator/(const cplx& other) const
{
  // Get reciprocal of input: multiplication is preferable computationally
  f4 scalar = (1.0f / other.LengthSquared()); // Inverse divisor: scalar value
  return (((*this) * !other) * scalar);
} // end cplx cplx::operator/(f4) const


// Divides all members uniformly by divisor, setting the result               */
dp::cplx& dp::cplx::operator/=(const cplx& other)
{
  // Get reciprocal of input: multiplication is preferable computationally
  f4 scalar = (1.0f / other.LengthSquared()); // inverse divisor: scalar value
  // length squared (||c||^2) = c*(!c), the conjugate, thus multiplying by 1:
  // a/b = (a/b)(!b/!b) = (a*!b)/(b*!b) = (a*!b)/||b||^2, where ||b||^2 is real
  return (((*this) * !other) *= scalar);
} // end cplx cplx::operator/=(f4)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


// Left hand scalar multiplication scales only dimensional members of rhs     */
dp::cplx dp::operator*(f4 scalar, const dp::cplx& rhs)
{
  cplx scaled((scalar * rhs.x), (scalar * rhs.y));
  return scaled;
} // end cplx operator*(f4, const cplx&)


// Confirm if 2 complex numbers are relatively equal in real and imag values  */
bool dp::operator==(const cplx& lhs, const cplx& rhs)
{
  return (Near<f4>(lhs.x, rhs.x, PROXIMITY)
    && Near<f4>(lhs.y, rhs.y, PROXIMITY));

} // end bool operator==(const cplx&, const cplx&)


// Confirm if 2 complex numbers are not equal in either real or imag values   */
bool dp::operator!=(const cplx& lhs, const cplx& rhs)
{
  return(!Near<f4>(lhs.x, rhs.x, PROXIMITY)
    || !Near<f4>(lhs.y, rhs.y, PROXIMITY));

} // end bool operator!=(const cplx&, const cplx&)


// Output the string version of each of the complex elements data             */
std::ostream& dp::operator<<(std::ostream& output, const cplx& complex)
{
  output << complex.String();
  return output;
} // end std::ostream& operator<<(std::ostream&, const cplx&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

//! Zero vector in real and imaginary dimensions; error state reserved value
const dp::cplx nullcplx_(0.0f, 0.0f);
const dp::cplx& dp::nullcplx = nullcplx_;
