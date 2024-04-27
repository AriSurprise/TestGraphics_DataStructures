/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v2f.cpp
Purpose:  API agnostic, multipurpose, 2 dimensional float-point element vector
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

// <stl>
#include <ostream>      // Output stream definition for operator overload
// "./src/..."
#include "ang.h"        // Unit measures for conversion in and out of arc spans
#include "ASCIIValue.h" // char range based [] operator manipulation
#include "Log.h"        // System message management for errors, warnings, etc
#include "m2f.h"        // Generating matrices from vector data
#include "m3f.h"        // Generating matrices from vector data
#include "Value.h"      // NearF() float point (proximity as equality) function
#include "v2f.h"        // Class declaration header
#include "v3f.h"        // Swizzling / promotion operations to larger vectors
#include "v4f.h"        // Swizzling / promotion operations to larger vectors


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

static const u1 SUBSCRIPTS = 2u;    //! v2f class is fixed at 2 subscripts

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::v2f::v2f(f4 xu_value, f4 yv_value) : x(xu_value), y(yv_value)
{ } // end v2f::v2f(f4, f4)                                                   */

dp::v2f::v2f(ang angle, f4 magnitude)
  : x(magnitude * CosF(angle)), y(magnitude * SinF(angle))
{ } // end v2f(ang, f4)                                                       */

dp::v2f::v2f(const dp::v2f& source) : v2f(source.x, source.y)
{ } // end v2f::v2f(const v2f&)                                               */

dp::v2f::v2f(dp::v2f&& result) noexcept
{ *this = std::move(result); } // end v2f::v2f(v2f&&)                         */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get a copy of the dimensional (x,y) values with w overwritten to be 1.0f   */
dp::v4f dp::v2f::AsPoint(f4 z_index) const
{ return v4f(x, y, z_index, 1.0f); } // end v4f AsPoint(f4) const


// Get a copy of the dimensional <x,y> values with w overwritten to be 0.0f   */
dp::v4f dp::v2f::AsVector(f4 z_index) const
{ return v4f(x, y, z_index, 0.0f); } // end v4f AsVector(f4) const


// Get angle (arctangent), of current cartesian coordinates in vector         */
dp::ang dp::v2f::Angle(void) const
{
  return ang(static_cast<f4>(atan2(y, x)), arc::radians, winding::ccw);
}


// Calculate the orthogonal cross product of (this x other) v3f               */
dp::v3f dp::v2f::Cross(const v3f& other) const
{
  return v3f((  (y * other.z)),
              ( -(x * other.z)),
        ((x * other.y) - (y * other.x)));

} // end v3f v2f::Cross(const v3f&) const


// Get m2f for getting the cross product when left multiplied with a v2f      */
dp::m3f dp::v2f::CrossMatrix(void) const
{
  return m3f(0.0f, 0.0f,   -y,
              0.0f, 0.0f,    x,
               y,     -x, 0.0f);

} // m2f v3f::CrossMatrix(void) const


// Calculate the dot product of (other &middot; this) vector (product sum)    */
f4 dp::v2f::Dot(const v2f& other) const
{
  return (x * other.x + y * other.y);
} // end f4 v2f::Dot(const v2f&) const


dp::m2f dp::v2f::DotTranspose(const v2f& other) const
{
  return  m2f((x*other.x), (x*other.y),
              (y*other.x), (y*other.y));

} // end m2f v2f::DotTranspose(const v2f&) const                              */


std::string dp::v2f::JSON(void) const
{
  return "[" + std::to_string(x) + "," + std::to_string(y) + "]";
} // end std::string v2f::JSON(void) const                                    */


dp::v2f dp::v2f::Intersect(const v2f& other) const
{
  v2f a(Range());
  v2f b(other.Range());
  if (a.max < b.min || b.max < a.min) { return nullv2f; } // Ranges divergent
  return v2f(MaxF(a.min, b.min), MinF(a.max, b.max));
} // end v2f v2f::Intersect(const v2f&) const                                 */


f4 dp::v2f::Length(void) const
{
  f4 lenSq = LengthSquared();
  if (NearF(lenSq, 1.0f)) { return 1.0f; }
  return SqrtF(lenSq);

} // end f4 v2f::Length(void) const                                           */


f4 dp::v2f::LengthSquared(void) const
{ return DotSelf(); } // end f4 v2f::LengthSquared(void) const                */


dp::v2f& dp::v2f::Normalize(void)
{
  f4 len = Length();
  if (len != 0.0f)
  {
    f4 rMag = 1.0f / len; // Reciprocal vector magnitude
    *this *= rMag;
  }
  else // null vector can't be made a unit vector: default a value of x
  {
    Log::Info("v2f::normalize - null vector input: default used");
    X(1.0f).Y(0.0f);
  }
  return *this;
} // end v2f& v2f::Normalize(void)                                            */


dp::v2f& dp::v2f::Set(const v3f& source)
{ return X(source.x).Y(source.y); } // end v2f& v2f::Set(const v3f&)          */
dp::v2f& dp::v2f::Set(const v4f& source)
{ return X(source.x).Y(source.y); } // end v2f& v2f::Set(const v4f&)          */

// Format a v4f for being output as a vector notated string
std::string dp::v2f::String(void) const
{
  std::string vecFormat;
  vecFormat = "[";
  if (x >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(x) + ", ";
  if (y >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(y) + " ]";
  return vecFormat;
} // end std::string v2f::String(void) const                                  */


// Value copy will be divided by its length and converted to a unit vector 
dp::v2f dp::v2f::UnitVec(void) const
{
  v2f unit = *this; // Normalized copy of this
  unit.Normalize();
  return unit;
} // end v2f v2f::UnitVec(void) const                                         */


dp::v3f dp::v2f::XXX(void) const
{ return v3f(x, x, x); } // end v3f v2f::XXX(void) const                      */
dp::v3f dp::v2f::XXY(void) const
{ return v3f(x, x, y); } // end v3f v2f::XXY(void) const                      */
dp::v3f dp::v2f::XXZ(f4 zbp_value) const
{ return v3f(x, x, zbp_value); } // end v3f v2f::XXZ(f4) const                */
dp::v3f dp::v2f::XYX(void) const
{ return v3f(x, y, x); } // end v3f v2f::XYX(void) const                      */
dp::v3f dp::v2f::XYY(void) const
{ return v3f(x, y, y); } // end v3f v2f::XYY(void) const                      */
dp::v3f dp::v2f::XYZ(f4 zbp_value) const
{ return v3f(x, y, zbp_value); } // end v3f v2f::XYZ(f4) const                */
dp::v3f dp::v2f::XZX(f4 zbp_value) const
{ return v3f(x, zbp_value, x); } // end v3f v2f::XZX(f4) const                */
dp::v3f dp::v2f::XZY(f4 zbp_value) const
{ return v3f(x, zbp_value, y); } // end v3f v2f::XZY(f4) const                */
dp::v3f dp::v2f::XZZ(f4 zbp_value) const
{ return v3f(x, zbp_value, zbp_value); } // end v3f v2f::XZZ(f4) const        */
dp::v3f dp::v2f::XZW(f4 zbp_value, f4 waq_value) const
{ return v3f(x, zbp_value, waq_value); } // end v3f v2f::XZW(f4, f4) const    */
dp::v4f dp::v2f::XYZW(f4 zbp_value, f4 waq_value) const
{ return v4f(x, y, zbp_value, waq_value); } // end v4f v2f::XYZW(f4, f4)      */
dp::v3f dp::v2f::YXX(void) const
{ return v3f(y, x, x); } // end v3f v2f::YXX(void) const                      */
dp::v3f dp::v2f::YXY(void) const
{ return v3f(y, x, x); } // end v3f v2f::YXX(void) const                      */
dp::v3f dp::v2f::YXZ(f4 zbp_value) const
{ return v3f(y, x, zbp_value); } // end v3f v2f::YXZ(f4) const                */
dp::v3f dp::v2f::YYX(void) const
{ return v3f(y, y, x); } // end v3f v2f::YYX(void) const                      */
dp::v3f dp::v2f::YYY(void) const
{ return v3f(y, y, y); } // end v3f v2f::YYY(void) const                      */
dp::v3f dp::v2f::YYZ(f4 zbp_value) const
{ return v3f(y, y, zbp_value); } // end v3f v2f::YYZ(f4) const                */
dp::v3f dp::v2f::YZX(f4 zbp_value) const
{ return v3f(y, zbp_value, x); } // end v3f v2f::YZX(f4) const                */
dp::v3f dp::v2f::YZY(f4 zbp_value) const
{ return v3f(y, zbp_value, y); } // end v3f v2f::YZY(f4) const                */
dp::v3f dp::v2f::YZZ(f4 zbp_value) const
{ return v3f(y, zbp_value, zbp_value); } // end v3f v2f::YZZ(f4) const        */
dp::v3f dp::v2f::YZW(f4 zbp_value, f4 waq_value) const
{ return v3f(y, zbp_value, waq_value); } // end v3f v2f::YZW(f4, f4) const    */
dp::v3f dp::v2f::ZXX(f4 zbp_value) const
{ return v3f(zbp_value, x, x); } // end v3f v2f::ZXX(f4) const                */
dp::v3f dp::v2f::ZXY(f4 zbp_value) const
{ return v3f(zbp_value, x, y); } // end v3f v2f::ZXY(f4) const                */
dp::v3f dp::v2f::ZXZ(f4 zbp_value) const
{ return v3f(zbp_value, x, zbp_value); } // end v3f v2f::ZXZ(f4) const        */
dp::v3f dp::v2f::ZXW(f4 zbp_value, f4 waq_value) const
{ return v3f(zbp_value, x, waq_value); } // end v3f v2f::ZXW(f4, f4) const    */
dp::v3f dp::v2f::ZYX(f4 zbp_value) const
{ return v3f(zbp_value, y, x); } // end v3f v2f::ZYX(f4) const                */
dp::v3f dp::v2f::ZYY(f4 zbp_value) const
{ return v3f(zbp_value, y, y); } // end v3f v2f::ZYY(f4) const                */
dp::v3f dp::v2f::ZYZ(f4 zbp_value) const
{ return v3f(zbp_value, y, zbp_value); } // end v3f v2f::ZYZ(f4) const        */
dp::v3f dp::v2f::ZYW(f4 zbp_value, f4 waq_value) const
{ return v3f(zbp_value, y, waq_value); } // end v3f v2f::ZYW(f4, f4) const    */
dp::v3f dp::v2f::ZZX(f4 zbp_value) const
{ return v3f(zbp_value, zbp_value, x); } // end v3f v2f::ZZX(f4) const        */
dp::v3f dp::v2f::ZZY(f4 zbp_value) const
{ return v3f(zbp_value, zbp_value, y); } // end v3f v2f::ZZY(f4) const        */
dp::v3f dp::v2f::ZWX(f4 zbp_value, f4 waq_value) const
{ return v3f(zbp_value, waq_value, x); } // end v3f v2f::ZWX(f4, f4) const    */
dp::v3f dp::v2f::ZWY(f4 zbp_value, f4 waq_value) const
{ return v3f(zbp_value, waq_value, y); } // end v3f v2f::ZWY(f4, f4) const    */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Creates a negated copy with relevant v2f coordinate values                 */
dp::v2f dp::v2f::operator-(void) const
{ return v2f(-x, -y); } // end v2f v2f::operator-(void) const


// Sets the source v2f member values into this v2f value fields               */
dp::v2f& dp::v2f::operator=(const v2f& source)
{ this->Set(source); return *this; } // end v2f& v2f::operator=(const v2f&)


// Surrogate a v2f value reference returned from a function for this v2f      */
dp::v2f& dp::v2f::operator=(v2f&& result) noexcept
{
  x = std::move(result.x);
  y = std::move(result.y);
  return *this;
} // end v2f& v2f::operator=(const v2f&)


// Negates v2f element values to represent the inverse displacement           */
dp::v2f& dp::v2f::operator!(void)
{
  Set(-x, -y);
  return *this;
} // end v2f& v2f::operator!(void)


// Get a value from ['x'] or ['y'] channels (or equivalent)                   */
f4 dp::v2f::operator[](s1 channel) const
{
  // clamp wildly out of range values into lowest 2 unsigned values
  if (!isascii(channel) /* 'not' 'msb not set' */
    || (channel < 0x20  /* control seq */ && channel > 0xF /* hex range */))
  {
    Log::Diag(std::string("v2f::operator[] const - value out of range '")
      + char(channel) + "' read, ", false);
    channel &= 1;
    Log::W(std::string("setting to '") + std::to_string(channel) + "'", true);
  }
  else if (IsUpper(channel)) { channel = tolower(channel); }
  else if (IsDigit(channel)) { channel = ToNumeral(channel) & 1; }
  switch (channel) // Lookup array index equivalent for return
  {
  case 'x': case 'i': case 'u': case 0:
    return x;
  case 'y': case 'j': case 'v': case 1:
    return y;
  default:
    Log::Diag(std::string("v2f::operator[] const - unsupported char ")
      + char(channel) + " read: default head 'x' == 0 used");
    return x;
  }
} // end f4 v2f::operator[](s1) const


// Get a value from ['x'] or ['y'] channels (or equivalent)                   */
f4& dp::v2f::operator[](s1 channel)
{
  // clamp wildly out of range values into lowest 2 unsigned values
  if (!isascii(channel) /* 'not' 'msb not set' */
    || (channel < 0x20  /* control seq */ && channel > 0xF /* hex range */))
  {
    Log::Diag(std::string("v2f::operator[] - value out of range '")
      + char(channel) + "' read, ", false);
    channel &= 1;
    Log::D(std::string("setting to '") + std::to_string(channel) + "'", true);
  }
  else if (IsUpper(channel)) { channel = tolower(channel); }
  else if (IsDigit(channel)) { channel = ToNumeral(channel) & 1; }
  switch (channel) // Lookup array index equivalent for return
  {
  case 'x': case 'i': case 'u': case 0:
    return x;
  case 'y': case 'j': case 'v': case 1:
    return y;
  default:
    Log::Diag(std::string("v2f::operator[] - unsupported char ")
      + char(channel) + " read: default head 'x' == 0 used");
    return x;
  }
} // end f4& v2f::operator[](s1)


// Confirm if 2 v2f's are relatively equal in x and y values                  */
bool dp::v2f::operator==(const v2f& other) const
{
  return (NearF(x, other.x, _INf)  && NearF(y, other.y, _INf));

} // end bool v2f::operator==(const v2f&)


// Confirm if 2 v2f's are not equal in any of x or y values                   */
bool dp::v2f::operator!=(const v2f& other) const
{
  return(!NearF(x, other.x, _INf) || !NearF(y, other.y, _INf));

} // end bool v2f::operator!=(const v2f&)


// Calculate the member-wise addition of 2 v2f's and return the result        */
dp::v2f dp::v2f::operator+(const v2f& other) const
{
  return v2f((x + other.x), (y + other.y));

} // end v2f v2f::operator+(const v2f&) const


// Add an other v2f's values memberwise into this v2f's values                */
dp::v2f& dp::v2f::operator+=(const v2f& other)
{
  Set((x + other.x), (y + other.y));
  return *this;

} // end v2f& v2f::operator+=(const v2f&)


// Calculate the member-wise difference of 2 v2f's and return the result      */
dp::v2f dp::v2f::operator-(const v2f& other) const
{
  return v2f((x - other.x), (y - other.y));

} // end v2f v2f::operator-(const v2f&) const


// Add an other v2f's values memberwise into this v2f's values                */
dp::v2f& dp::v2f::operator-=(const v2f& other)
{
  Set((x - other.x), (y - other.y));
  return *this;

} // end v2f& v2f::operator-=(const v2f&)


// Scalar product multiplies each field uniformly by a scalar constant        */
dp::v2f dp::v2f::operator*(f4 scalar) const
{
  return v2f((scalar * x), (scalar * y));
} // end v2f v2f::operator*(f4) const


// Scalar product sets all members to grow uniformly by a scalar constant     */
dp::v2f& dp::v2f::operator*=(f4 scalar)
{
  Set((scalar * x), (scalar * y));
  return *this;
} // end v2f v2f::operator*=(f4)


// Perform uniform division by divisor, returning the result                  */
dp::v2f dp::v2f::operator/(f4 divisor) const
{
  // Get reciprocal of input: multiplication is preferable computationally
  f4 scalar = (1.0f / divisor); // Inverse divisor: a scalar value
  return ( (*this) * scalar );
} // end v2f v2f::operator/(f4) const


// Divides all members uniformly by divisor, setting the result               */
dp::v2f& dp::v2f::operator/=(f4 divisor)
{
  // Get reciprocal of input: multiplication is preferable computationally
  f4 scalar = (1.0f / divisor); // inverse divisor: a scalar value
  return ( (*this) *= scalar );
} // end v2f v2f::operator/=(f4)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

//! Zero vector in 2 dimensions; error state reserved value
const dp::v2f nullv2f_(0.0f, 0.0f);
const dp::v2f& dp::nullv2f = nullv2f_;



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Overloads                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::v2f dp::operator+(f4 initial, const dp::v2f& rhs)
{
  v2f scaled((initial + rhs.x), (initial + rhs.y));
  return scaled;
} // end v2f operator+(f4, const v2f&)                                        */


dp::v2f dp::operator-(f4 initial, const dp::v2f& rhs)
{
  v2f scaled((initial - rhs.x), (initial - rhs.y));
  return scaled;
} // end v2f operator-(f4, const v2f&)                                        */


dp::v2f dp::operator*(f4 scalar, const dp::v2f& rhs)
{
  v2f scaled((scalar * rhs.x), (scalar * rhs.y));
  return scaled;
} // end v2f operator*(f4, const v2f&)                                        */


dp::v2f dp::operator/(f4 uniform, const v2f& divisor)
{
  return v2f(uniform * InvF(divisor.x), uniform * InvF(divisor.y));
} // end v2f operator/(f4, const v2f&)                                        */


std::ostream& dp::operator<<(std::ostream& output, const v2f& vector)
{
  output << vector.String();
  return output;
} // end std::ostream& operator<<(std::ostream&, const v2f&)                  */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Generate a distance / displacement vector between two coordinate vectors   */
dp::v2f dp::Distance(const v2f& initial, const v2f& terminal)
{
  v2f distanceVec = terminal;
  distanceVec -= initial;
  return distanceVec;
} // end v2f Distance(const v2f&, const v2f&)                                 */
