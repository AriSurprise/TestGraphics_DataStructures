/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v2f.cpp
Purpose:  API agnostic, multipurpose, 2 dimensional float-point element vector
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
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
#include "ASCIIValue.h" // char range based [] operator manipulation
#include "Log.h"        // System message management for errors, warnings, etc
#include "p2f.h"        // Class declaration header
#include "Value.h"      // NearF() float point (proximity as equality) function


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

static const u1 SUBSCRIPTS = 2u;    //! p2f class is fixed at 2 subscripts
static const f4 PROXIMITY = 0.001f; //! Reasonable near offset margin

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Creates a 2 dimensional vector / point (default <0,0>)                     */
dp::p2f::p2f(f4 angle, arc measure, f4 magnitude)
  : t(angle, measure), r(magnitude)
{ } // end p2f::p2f(f4, arc, f4)

// Creates a 2 dimensional vector / point (default <0,0>)                     */
dp::p2f::p2f(ang angle, f4 magnitude)
  : t(angle), r(magnitude)
{ } // end p2f::p2f(ang, f4)


// Creates a copy of a v2f from another source v2f                            */
dp::p2f::p2f(const dp::p2f& source) : p2f(source.t, source.r)
{ } // end p2f::p2f(const p2f&)


// Creates a v2f from a v2f value reference resultant from a function         */
dp::p2f::p2f(dp::p2f&& result) noexcept
{
  this->t = std::move(result.t);
  this->r = std::move(result.r);

} // end p2f::p2f(p2f&&)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get angle / direction measure currently set in polar vector                */
dp::ang dp::p2f::A(void) const
{
  return angle;

} // end p2f& p2f::A(void)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::A(f4 angle, arc measure)
{
  this->angle = ang(angle, measure);
  return *this;

} // end p2f& p2f::A(f4, arc)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::A(ang angle)
{
  dir = angle;
  return *this;

} // end p2f& p2f::A(ang)


// Get angle / direction measure currently set in polar vector                */
dp::ang dp::p2f::Ang(void) const
{
  return angle;

} // end p2f& p2f::Ang(void)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Ang(f4 angle, arc measure)
{
  this->angle = ang(angle, measure);
  return *this;

} // end p2f& p2f::Ang(f4, arc)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Ang(ang angle)
{
  dir = angle;
  return *this;
} // end p2f& p2f::Ang(ang)


// Get angle / direction measure currently set in polar vector                */
dp::ang dp::p2f::Angle(void) const
{
  return angle;

} // end p2f& p2f::Angle(void)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Angle(f4 angle, arc measure)
{
  this->angle = ang(angle, measure);
  return *this;

} // end p2f& p2f::Angle(f4, arc)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Angle(ang angle)
{
  dir = angle;
  return *this;
} // end p2f& p2f::Angle(ang)


// Get angle / direction measure currently set in polar vector                */
dp::ang dp::p2f::D(void) const
{
  return angle;

} // end p2f& p2f::D(void)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::D(f4 angle, arc measure)
{
  angle = ang(angle, measure).r();
  return *this;

} // end p2f& p2f::D(f4, arc)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::D(ang angle)
{
  dir = angle;
  return *this;

} // end p2f& p2f::D(ang)


// Get angle / direction measure currently set in polar vector                */
dp::ang dp::p2f::Dir(void) const
{
  return angle;

} // end p2f& p2f::Dir(void)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Dir(f4 angle, arc measure)
{
  angle = ang(angle, measure).r();
  return *this;

} // end p2f& p2f::Dir(f4, arc)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Dir(ang angle)
{
  dir = angle;
  return *this;
} // end p2f& p2f::Dir(ang)


// Get angle / direction measure currently set in polar vector                */
dp::ang dp::p2f::Direction(void) const
{
  return angle;

} // end p2f& p2f::Direction(void)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Direction(f4 angle, arc measure)
{
  this->angle = ang(angle, measure);
  return *this;

} // end p2f& p2f::Direction(f4, arc)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Direction(ang angle)
{
  dir = angle;
  return *this;
} // end p2f& p2f::Direction(ang)


// Format a v2f for being output as a packed vector notated string            */
std::string dp::p2f::JSON(void) const
{
  return "[" + std::to_string(t.r()) + "," + std::to_string(r) + "]";

} // end std::string v2f::JSON(void) const


// Get the displacement length / magnitude independent of angle / direction   */
f4 dp::p2f::L(void) const
{
  return len;

} // end f4 p2f::L(void) const


// Set the p2f displacement / diagonal magnitude measure of radius            */
dp::p2f& dp::p2f::L(f4 magnitude)
{
  len = magnitude;
  return *this;

} // end p2f& p2f::L(f4)


// Get the displacement length / magnitude independent of angle / direction   */
f4 dp::p2f::Len(void) const
{
  return len;

} // end f4 p2f::Len(void) const


// Set the p2f displacement / diagonal magnitude measure of radius            */
dp::p2f& dp::p2f::Len(f4 magnitude)
{
  len = magnitude;
  return *this;

} // end p2f& p2f::Len(f4)


// Get the displacement length / magnitude independent of angle / direction   */
f4 dp::p2f::Length(void) const
{
  return len;

} // end f4 p2f::Length(void) const


// Set the p2f displacement / diagonal magnitude measure of radius            */
dp::p2f& dp::p2f::Length(f4 magnitude)
{
  len = magnitude;
  return *this;

} // end p2f& p2f::Length(f4)


// Get the squared displacement / diagonal length: radius^2                   */
f4 dp::p2f::LengthSquared(void) const
{
  return len * len;

} // end f4 p2f::LengthSquared(void) const


// Get the displacement length / magnitude independent of angle / direction   */
f4 dp::p2f::M(void) const
{
  return len;

} // end f4 p2f::M(void) const


// Set the p2f displacement / diagonal magnitude measure of radius            */
dp::p2f& dp::p2f::M(f4 magnitude)
{
  len = magnitude;
  return *this;

} // end p2f& p2f::M(f4)


// Get the displacement length / magnitude independent of angle / direction   */
f4 dp::p2f::Mag(void) const
{
  return len;

} // end f4 p2f::Mag(void) const


// Set the p2f displacement / diagonal magnitude measure of radius            */
dp::p2f& dp::p2f::Mag(f4 magnitude)
{
  len = magnitude;
  return *this;

} // end p2f& p2f::Mag(f4)


// Get the displacement length / magnitude independent of angle / direction   */
f4 dp::p2f::Magnitude(void) const
{
  return len;

} // end f4 p2f::Magnitude(void) const


// Set the p2f displacement / diagonal magnitude measure of radius            */
dp::p2f& dp::p2f::Magnitude(f4 magnitude)
{
  len = magnitude;
  return *this;

} // end p2f& p2f::Magnitude(f4)


// This will have its length set to 1 and be converted to a unit vector       */
dp::p2f& dp::p2f::Normalize(void)
{
  radius = 1.0f;
  return *this;
} // end p2f& p2f::Normalize(void)


// Get angle / direction measure currently set in polar vector                */
dp::ang dp::p2f::Phi(void) const
{
  return angle;

} // end p2f& p2f::Phi(void)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Phi(f4 angle, arc measure)
{
  this->angle = ang(angle, measure);
  return *this;

} // end p2f& p2f::Phi(f4, arc)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Phi(ang angle)
{
  dir = angle;
  return *this;

} // end p2f& p2f::Phi(ang)


// Get the displacement length / magnitude independent of angle / direction   */
f4 dp::p2f::R(void) const
{
  return len;

} // end f4 p2f::R(void) const


// Set the p2f displacement / diagonal magnitude measure of radius            */
dp::p2f& dp::p2f::R(f4 magnitude)
{
  len = magnitude;
  return *this;

} // end p2f& p2f::R(f4)


// Get the displacement length / magnitude independent of angle / direction   */
f4 dp::p2f::Rad(void) const
{
  return len;

} // end f4 p2f::Rad(void) const


// Set the p2f displacement / diagonal magnitude measure of radius            */
dp::p2f& dp::p2f::Rad(f4 magnitude)
{
  len = magnitude;
  return *this;

} // end p2f& p2f::Rad(f4)


// Get the displacement length / magnitude independent of angle / direction   */
f4 dp::p2f::Radius(void) const
{
  return len;

} // end f4 p2f::Radius(void) const


// Set the p2f displacement / diagonal magnitude measure of radius            */
dp::p2f& dp::p2f::Radius(f4 magnitude)
{
  len = magnitude;
  return *this;

} // end p2f& p2f::Radius(f4)


// Set the p2f to angle component and length input values                     */
dp::p2f& dp::p2f::Set(f4 angle, arc measure, f4 magnitude)
{
  Angle(angle, measure).Length(magnitude);
  return *this;

} // end p2f& p2f::Set(f4, arc, f4)


// Set the p2f from polar component input values                              */
dp::p2f& dp::p2f::Set(ang angle, f4 magnitude)
{
  Angle(angle).Length(magnitude);
  return *this;

} // end p2f& p2f::Set(ang, f4)


// Set the p2f values to <source.theta, source.radius> values                 */
dp::p2f& dp::p2f::Set(const p2f& source)
{
  Angle(source.angle).Length(source.length);
  return *this;

} // end p2f& p2f::Set(const p2f&)


// Set the polar equivalent <theta,r> from cartesian <source.x, source.y>     */
dp::p2f& dp::p2f::Set(const v2f& source)
{
  length = source.Length();
  f4 d = 1.0f / length;
  angle = ang(atan((source.y*d) / (source.x*d)), arc::radians);
  return *this;

} // end p2f& p2f::Set(const v2f&)


// Set the p2f to use <result.theta, result.radius> values (directly)         */
dp::p2f& dp::p2f::Set(p2f&& result) noexcept
{
  this->direction = std::move(result.angle);
  this->length = std::move(result.magnitude);
  return *this;

} // end p2f& p2f::Set(p2f&&) noexcept


// Format a p2f for being output as a vector notated string                   */
std::string dp::p2f::String(arc measure) const
{
  std::string vecFormat;
  vecFormat = "[";
  if (theta.r() >= 0.0f) { vecFormat += " "; }
  switch (measure)
  {
  case arc::degrees: vecFormat += std::to_string(theta.d()) + " degrees, ";
    break;
  case arc::turns: vecFormat += std::to_string(theta.s()) + " turns, ";
    break;
  case arc::gradians: vecFormat += std::to_string(theta.g()) + " gradians, ";
    break;
  case arc::rad: default: vecFormat += std::to_string(theta.r())+" radians, ";
    break;
  }
  if (radius >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(radius) + " long ]";
  return vecFormat;
} // end std::string p2f::String(void) const


// Get angle / direction measure currently set in polar vector                */
dp::ang dp::p2f::T(void) const
{
  return angle;

} // end p2f& p2f::T(void)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::T(f4 angle, arc measure)
{
  this->angle = ang(angle, measure);
  return *this;

} // end p2f& p2f::T(f4, arc)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::T(ang angle)
{
  dir = angle;
  return *this;

} // end p2f& p2f::T(ang)


// Convert polar 2D rotation and length to cartesian <x,y> 2D vector          */
dp::v2f dp::p2f::ToCartesian(void) const
{
  // Scale unit vector trig directions of theta angle by radius magnitude
  return v2f(r*cos(t.r()), r*sin(t.r()));

} // end v2f p2f::ToCartesian(void) const


// Get angle / direction measure currently set in polar vector                */
dp::ang dp::p2f::Theta(void) const
{
  return angle;

} // end p2f& p2f::Theta(void)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Theta(f4 angle, arc measure)
{
  this->angle = ang(angle, measure);
  return *this;

} // end p2f& p2f::Theta(f4, arc)


// Set polar direction coordinate in terms of rotation from <1, 0> in xy      */
dp::p2f& dp::p2f::Theta(ang angle)
{
  dir = angle;
  return *this;
} // end p2f& p2f::Theta(ang)


// Copy will have its length set to 1 and be converted to a unit vector       */
dp::p2f dp::p2f::UnitVec(void) const
{
  p2f unit = *this; // Normalized copy of this
  unit.Normalize();
  return unit;
} // end p2f p2f::UnitVec(void) const


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Sets the source v2f member values into this v2f value fields               */
dp::p2f& dp::p2f::operator=(const p2f& source)
{ this->Set(source); return *this; } // end p2f& p2f::operator=(const p2f&)


// Sets the source v2f member values into this v2f value fields               */
dp::p2f& dp::p2f::operator=(const v2f& source)
{ this->Set(source); return *this; } // end p2f& p2f::operator=(const v2f&)


// Surrogate a v2f value reference returned from a function for this v2f      */
dp::p2f& dp::p2f::operator=(p2f&& result) noexcept
{
  theta = std::move(result.theta);
  radius = std::move(result.radius);
  return *this;
} // end p2f& p2f::operator=(p2f&&)


// Creates a negated copy with relevant v2f coordinate values                 */
dp::p2f dp::p2f::operator-(void) const
{
  return p2f(t - ang(PI, arc::radians), r);

} // end p2f p2f::operator-(void) const


// Negates v2f element values to represent the inverse displacement           */
dp::p2f dp::p2f::operator!(void) const
{
  return p2f(t + ang(PI, arc::radians), r);

} // end v2f& v2f::operator!(void)


// Get value from ['d'|'a'|'t'] or ['m'|'l'|'r'] channels (or equivalent)     */
const f4& dp::p2f::operator[](s1 channel) const
{
  // clamp wildly out of range values into lowest 2 unsigned values
  if (!isascii(channel) /* 'not' 'msb not set' */
    || (channel < 0x20  /* control seq */ && channel > 0xF /* hex range */))
  {
    Log::Diag(std::string("p2f::operator[] - value out of range '")
      + char(channel) + "' read, ", false);
    channel &= 1;
    Log::W(std::string("setting to '") + std::to_string(channel) + "'", true);
  }
  else if (IsUpper(channel)) { channel = tolower(channel); }
  else if (IsDigit(channel)) { channel = ToNumeral(channel) & 1; }
  switch (channel) // Lookup array index equivalent for return
  {
  case 'd': case 'a': case 't': case 0:
    return t.radRef();
  case 'm': case 'l': case 'r': case 1:
    return r;
  default:
    Log::Diag(std::string("p2f::operator[] - unsupported char ")
      + char(channel) + " read: default head 't' == 0 used");
    return t.radRef();
  }
} // end const f4& p2f::operator[](s1) const


// Get value from ['d'|'a'|'t'] or ['m'|'l'|'r'] channels (or equivalent)     */
f4 dp::p2f::operator[](s1 channel)
{
  // clamp wildly out of range values into lowest 2 unsigned values
  if (!isascii(channel) /* 'not' 'msb not set' */
    || (channel < 0x20  /* control seq */ && channel > 0xF /* hex range */))
  {
    Log::Diag(std::string("p2f::operator[] const - value out of range '")
      + char(channel) + "' read, ", false);
    channel &= 1;
    Log::W(std::string("setting to '") + std::to_string(channel) + "'", true);
  }
  else if (IsUpper(channel)) { channel = tolower(channel); }
  else if (IsDigit(channel)) { channel = ToNumeral(channel) & 1; }
  switch (channel) // Lookup array index equivalent for return
  {
  case 'd': case 'a': case 't': case 0:
    return t.r();
  case 'm': case 'l': case 'r': case 1:
    return r;
  default:
    Log::Diag(std::string("p2f::operator[] const - unsupported char ")
      + char(channel) + " read: default head 't' == 0 used");
    return t.r();
  }
} // end f4 p2f::operator[](s1)


// Calculate the member-wise addition of 2 p2f's and return the result        */
dp::v2f dp::p2f::operator+(const p2f& other) const
{

  return (ToCartesian() + other.ToCartesian());

} // end v2f p2f::operator+(const p2f&) const


// Add a cartesian v2f to this polar p2f, returning the result                */
dp::v2f dp::p2f::operator+(const v2f& other) const
{

  return (ToCartesian() + other);

} // end v2f p2f::operator+(const v2f&) const


// Calculate the member-wise addition of 2 p2f's and set the result           */
dp::p2f& dp::p2f::operator+=(const p2f& other)
{
  // Gross.  This is why no one uses polar...
  Set(ToCartesian() + other.ToCartesian());
  return *this;

} // end p2f& p2f::operator+=(const p2f&)


// Add a cartesian v2f to this polar p2f, setting the result                  */
dp::p2f& dp::p2f::operator+=(const v2f& other)
{
  Set(ToCartesian() + other);
  return *this;

} // end p2f& p2f::operator+=(const v2f&)


// Calculate the member-wise difference of 2 p2f's and return the result      */
dp::v2f dp::p2f::operator-(const p2f& other) const
{
  return (ToCartesian() - other.ToCartesian());

} // end v2f p2f::operator-(const p2f&) const


// Subtract a cartesian v2f from this polar p2f, returning the result         */
dp::v2f dp::p2f::operator-(const v2f& other) const
{
  return (ToCartesian() - other);

} // end v2f p2f::operator-(const v2f&) const


// ubtract another p2f from this p2f's values, setting the result in this     */
dp::p2f& dp::p2f::operator-=(const p2f& other)
{
  Set(ToCartesian() - other.ToCartesian());
  return *this;

} // end p2f& p2f::operator-=(const p2f&)


// Subtract a cartesian v2f from this polar p2f, set this as the result       */
dp::p2f& dp::p2f::operator-=(const v2f& other)
{
  Set(ToCartesian() - other);
  return *this;

} // end p2f& p2f::operator-=(const v2f&)


// Scalar product multiplies each field uniformly by a scalar constant        */
dp::p2f dp::p2f::operator*(f4 scalar) const
{
  return p2f(theta, r*scalar);

} // end p2f p2f::operator*(f4) const


// Scalar product sets all members to grow uniformly by a scalar constant     */
dp::p2f& dp::p2f::operator*=(f4 scalar)
{
  Radius(radius*scalar);
  return *this;

} // end p2f p2f::operator*=(f4)


// Convert to cartesian to return dot product (product sum) with other v2f    */
f4 dp::p2f::operator*(const v2f& other) const
{
  return ToCartesian().Dot(other);

} // end f4 p2f::operator*(const v2f&) const


// Multiply a polar vector by another, scaling and rotating polar coords      */
dp::p2f dp::p2f::operator*(const p2f& other) const
{
  return p2f(theta + other.theta, radius * other.radius);

} // end p2f p2f::operator*(const p2f&) const


// Multiply a polar vector by another, scaling and rotating polar coords      */
dp::p2f& dp::p2f::operator*=(const p2f& other)
{
  Set(theta + other.theta, radius * other.radius);
  return *this;

} // end p2f& p2f::operator*=(const p2f&)


// Perform division on magnitude by divisor, returning the result             */
dp::p2f dp::p2f::operator/(f4 divisor) const
{
  return p2f(theta, r / divisor);

} // end p2f p2f::operator/(f4) const


// Divide a polar vector by another, scaling and rotating the polar coords    */
dp::p2f dp::p2f::operator/(const p2f& other) const
{
  return p2f(theta - other.theta, radius / other.radius);

} // end p2f p2f::operator/(const p2f&) const


// Divides length of polar vector by divisor, setting the result              */
dp::p2f& dp::p2f::operator/=(f4 divisor)
{
  Radius(radius / divisor);
  return *this;

} // end p2f& p2f::operator/=(f4)


// Divide a polar vector by another, scaling and rotating the polar coords    */
dp::p2f& dp::p2f::operator/=(const p2f& other)
{
  Set(theta - other.theta, radius / other.radius);
  return *this;

} // end p2f& p2f::operator/=(const p2f&)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

//! Polar zero vector in magnitude has indeterminate direction: 0.0*v2f(1,0)
const dp::p2f nullp2f_(dp::nullang, 0.0f);
const dp::p2f& dp::nullp2f = nullp2f_;
// (really any 0 magnitude vector will be problematic, regardless of direction)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


// Left hand scalar multiplication scales only magnitude of rhs               */
dp::p2f dp::operator*(f4 scalar, const dp::p2f& rhs)
{
  return p2f(rhs.theta, (scalar * rhs.radius));
} // end p2f operator*(f4, const p2f&)


// Confirm if 2 p2f's are relatively equal in both length and direction       */
bool dp::operator==(const p2f& lhs, const p2f& rhs)
{
  // Recall angles are tricksy: with their dirty double tangent secrets,
  // negative magnitudes, anti-parallels, multiple revolutions, etc...
  if (Near<f4>(Abs<f4>(lhs.radius), Abs<f4>(rhs.radius), PROXIMITY))
  {
    f4 lha = WrappedRange<f4>(lhs.a.r(), TAU, 0.0f),
      rha = WrappedRange<f4>(rhs.a.r(), TAU, 0.0f);
    if ((lhs.radius >= 0.0f && rhs.radius >= 0.0f)
      || (lhs.radius < 0.0f && rhs.radius < 0.0f))
    {
      return (lha == rha);
    }
    // If the magnitudes of of opposing signs, test anti-parallel direction
    else { return (lha == -rha) || (-lha == rha); }
  }
  return false;

} // end bool operator==(const p2f&, const p2f&)


// Confirm if 2 p2f's are not equal in either length or direction             */
bool dp::operator!=(const p2f& lhs, const p2f& rhs)
{
  // Recall angles are tricksy: with their dirty double tangent secrets,
  // negative magnitudes, anti-parallels, multiple revolutions, etc...
  if (Near<f4>(Abs<f4>(lhs.radius), Abs<f4>(rhs.radius), PROXIMITY))
  {
    f4 lha = WrappedRange<f4>(lhs.a.r(), TAU, 0.0f),
       rha = WrappedRange<f4>(rhs.a.r(), TAU, 0.0f);
    if ((lhs.radius > 0.0f && rhs.radius > 0.0f)
     || (lhs.radius < 0.0f && rhs.radius < 0.0f))
    {
      return (lha != rha);
    }
    else { return (lha != -rha) || (-lha != rha); }
  }
  return true;

} // end bool operator!=(const p2f&, const p2f&)


// Output the string version of the vector element data                       */
std::ostream& dp::operator<<(std::ostream& output, const p2f& vector)
{
  output << vector.String();
  return output;
} // end std::ostream& operator<<(std::ostream&, const p2f&)
