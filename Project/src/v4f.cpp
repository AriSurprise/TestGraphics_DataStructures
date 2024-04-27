/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v4f.cpp
Purpose:  API agnostic, multipurpose, homogeneously promoted 3->4D float vector
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
#include "m3f.h"        // Generating matrices from vector data
#include "m4f.h"        // Generating matrices from vector data
#include "Value.h"      // NearF() float point (proximity as equality) function
#include "v2f.h"        // Swizzling operations to truncate to smaller vectors
#include "v3f.h"        // Swizzling operations to truncate to smaller vectors
#include "v4f.h"        // Class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

static const u1 SUBSCRIPTS = 4u;    //! v4f class is fixed at 4 subscripts
static const f4 PROXIMITY = _INf; //! Reasonable near offset margin

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::v4f::v4f(f4 xrs_value, f4 ygt_value, f4 zbp_value, f4 waq_value)
  : x(xrs_value), y(ygt_value), z(zbp_value), w(waq_value)
{ } // end v4f::v4f(f4, f4, f4, f4)                                           */


dp::v4f::v4f(const v4f& source)
  : v4f(source.x, source.y, source.z, source.w)
{ } // end v4f::v4f(const v4f&)                                               */


dp::v4f::v4f(v4f&& result) noexcept
{ *this = std::move(result); } // end v4f::v4f(v4f&&)                         */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::v4f& dp::v4f::A(f4 waq_value, const v2f& clamp)
{
  return A(InF(RealF(waq_value), clamp.max, clamp.min));
} // end v4f&  v4f::A(f4, const v2f&)                                         */


dp::v4f dp::v4f::AsPoint(void) const
{ return v4f(x, y, z, 1.0f); } // end v4f AsPoint(void) const                 */


dp::v4f dp::v4f::AsVector(void) const
{ return v4f(x, y, z, 0.0f); } // end v4f AsVector(void) const                */


dp::v4f dp::v4f::Angles(arc units, winding direction) const
{
  // Rotation about +y has +x right & +z down (Q1), or +z right & +x up (Q4)
  // Rotation about +x has +z left & +y up (Q1), or +y right & +z up (Q2)
  v4f result(atan2f(y, x), atan2f(-z, x), atan2f(y, -z), 0);
  if (direction == winding::clockwise) { !result; }
  switch (units)
  {
  case arc::degrees:
    result *= DEG_PER_RAD;
    break;
  case arc::gradians:
    result *= GRAD_PER_RAD;
    break;
  case arc::revolutions:
    result *= REV_PER_RAD;
    break;
  case arc::radians:
    break;
  default:
    dp::Log::Info("v4f::Angles - non-standard units used: radians out");
    break;
  }
  return result;
} // end v4f v4f::Angles(void) const                                          */


dp::v4f& dp::v4f::B(f4 zbp_value, const v2f& clamp)
{
  return B(InF(RealF(zbp_value), clamp.max, clamp.min));
} // end v4f&  v4f::B(f4, const v2f&)                                         */


dp::v4f dp::v4f::Cross(const v4f& other) const
{
  return v4f(((y * other.z) - (z * other.y)),
             ((z * other.x) - (x * other.z)),
             ((x * other.y) - (y * other.x)), Of);
} // end v4f v4f::Cross(const v4f&) const                                     */


dp::m4f dp::v4f::CrossMatrix(void) const
{
  return m4f(0.0f,    -z,    y,
               z,   0.0f,   -x,
              -y,      x, 0.0f);

} // m4f v4f::CrossMatrix(void) const                                         */


f4 dp::v4f::Dot(const v4f& other) const
{
  return (x * other.x + y * other.y + z * other.z + w * other.w);

} // end f4 v4f::Dot(const v4f&) const                                        */


f4 dp::v4f::DotH(const v4f& other) const
{
  return (x * other.x + y * other.y + z * other.z);

} // end f4 v4f::DotS(const v4f&) const                                       */


dp::m4f dp::v4f::DotTranspose(const v4f* other) const
{
  if (other)
  {
    return m4f((x*other->x), (x*other->y), (x*other->z), (x*other->w),
               (y*other->x), (y*other->y), (y*other->z), (y*other->w),
               (z*other->x), (z*other->y), (z*other->z), (z*other->w),
               (w*other->x), (w*other->y), (w*other->z), (w*other->w));
  }
  else // this * this if no other vector provided
  {
    return m4f((x*x), (x*y), (x*z), (x*w),
               (y*x), (y*y), (y*z), (y*w),
               (z*x), (z*y), (z*z), (z*w),
               (w*x), (w*y), (w*z), (w*w));
  }
} // end m4f v4f::DotTranspose(const v4f*) const                              */


dp::v4f& dp::v4f::G(f4 ygt_value, const v2f& clamp)
{
  return G(InF(RealF(ygt_value), clamp.max, clamp.min));
} // end v4f&  v4f::G(f4, const v2f&)                                         */


bool dp::v4f::IsClip(void) const
{ return (!IsVector() && !IsPoint()); } // end bool v4f::IsClip(void) const   */


bool dp::v4f::IsPoint(void) const
{
  return NearF(w, 1.0f, _INf);
} // end bool v4f::IsPoint(void) const                                        */


bool dp::v4f::IsVector(void) const
{
  return IsInF(w, _INf, -_INf);
} // end bool v4f::IsVector(void) const                                       */


std::string dp::v4f::JSON(void) const
{
  return "[" + std::to_string(x) + "," + std::to_string(y) + ","
    + std::to_string(z) + "," + std::to_string(w) + "]";

} // end std::string v4f::json(void) const                                    */


f4 dp::v4f::Length(void) const
{
  f4 lenSq = LengthSquared();
  if (NearF(lenSq, 1.0f)) { return 1.0f; }
  return SqrtF(lenSq);

} // end f4 v4f::Length(void) const                                           */


f4 dp::v4f::LengthSquared(void) const
{ return this->Dot(*this); } // end f4 v4f::LengthSquared(void) const         */


f4 dp::v4f::Magnitude(void) const
{ return SqrtF(MagnitudeSquared()); } // end f4 v4f::Magnitude(void) const    */


f4 dp::v4f::MagnitudeSquared(void) const
{ return this->DotH(*this); } // end f4 v4f::MagnitudeSquared(void) const     */


dp::v4f& dp::v4f::MakeVector(void)
{ w = 0; return *this; } // end v4f& v4f::MakeVector(void)                    */


dp::v4f dp::v4f::Multiply(const v4f& other) const
{
  v4f product(x * other.x, y * other.y, z * other.z, w * other.w);
  return product;
} // end v4f Multiply(const v4f&) const                                       */


dp::v4f& dp::v4f::MultiplyWith(const v4f& other)
{
  Set((x * other.x), (y * other.y), (z * other.z), (w * other.w));
  return *this;
} // end v4f& MultiplyWith(const v4f&)                                        */


dp::v4f& dp::v4f::Normalize(void)
{
  this->MakeVector();
  f4 len = Length();
  if (!NearF(len, 0.0f, _INf))
  {
    f4 rMag = 1.0f / len; // Reciprocal vector magnitude
    *this *= rMag;
  }
  else // null vector can't be made a unit vector: default a value of x
  {
    Log::Info("v4f::normalize - null vector input: default used");
    Set(UNIT_VEC_X);
  }
  return *this;
} // end v4f& v4f::Normalize(void)                                            */


dp::v4f& dp::v4f::P(f4 zbp_value, const v2f& wrap)
{
  p = ModF(RealF(zbp_value), wrap.max, wrap.min); return *this;
} // end v4f& v4f::P(f4, const v2f&)                                          */


dp::v4f& dp::v4f::Q(f4 waq_value, const v2f& wrap)
{
  q = ModF(RealF(waq_value), wrap.max, wrap.min); return *this;
} // end v4f& v4f::Q(f4, const v2f&)                                          */


dp::v4f& dp::v4f::R(f4 xrs_value, const v2f& clamp)
{
  r = (InF(RealF(xrs_value), clamp.max, clamp.min)); return *this;
} // end v4f&  v4f::R(f4, const v2f&)                                         */


dp::v4f& dp::v4f::RestorePoint(f4 waq_tolerance)
{
  if (!NearF(w, 0.0f, waq_tolerance)) { w = 1.0f; }
  else if (!NearF(w, 1.0f, waq_tolerance))
  {
    x /= w;
    y /= w;
    z /= w;
    w = 1.0f;
  }
  return *this;
} // end v4f& v4f::RestorePoint(f8)                                           */


dp::v4f& dp::v4f::S(f4 xrs_value, const v2f& wrap)
{ s = ModF(RealF(xrs_value), wrap.max, wrap.min); return *this;
} // end v4f& v4f::S(f4, const v2f&)                                          */


dp::v4f& dp::v4f::Set(f4 xrs_value, f4 ygt_value, f4 zbp_value, f4 waq_value)
{
  X(xrs_value).Y(ygt_value).Z(zbp_value).W(waq_value);
  return*this;
} // end v4f& v4f::Set(f4, f4, f4, f4)                                        */


dp::v4f& dp::v4f::Set(const v4f& source)
{
  X(source.x).Y(source.y).Z(source.z).W(source.w);
  return *this;
} // end v4f& v4f::Set(const v4f&)                                            */


dp::v4f& dp::v4f::Set(v4f&& result) noexcept
{
  *this = std::move(result);
  return *this;
} // end v4f& v4f::Set(v4f&&) noexcept                                        */


dp::v4f& dp::v4f::SetAngleEuler(ang z_rot, ang y_rot, ang x_rot)
{
  // Check if shortcut math can be taken for 1D rotation for early return
  if (NearF(y_rot.r(), 0.0f, _INf) && NearF(x_rot.r(), 0.0f, _INf))
  {
    // Rotation about +z has +x right & +y up (Q1)
    X(cosf(z_rot.r())).Y(sinf(z_rot.r())).Z(0).W(0);
  }
  else if (NearF(z_rot.r(), 0.0f, _INf) && NearF(x_rot.r(), 0.0f, _INf))
  {
    // Rotation about +y has +x right & +z down (Q1), or +z right & +x up (Q4)
    X(cosf(y_rot.r())).Y(0).Z(-sinf(y_rot.r())).W(0);
  }
  else if (NearF(z_rot.r(), 0.0f, _INf) && NearF(y_rot.r(), 0.0f, _INf))
  {
    // Rotation about +x has +z left & +y up (Q1), or +y right & +z up (Q2)
    X(0).Y(sinf(x_rot.r())).Z(-cosf(x_rot.r())).W(0);
  }
  // Perform rotation by matrix from input euler angles
  m3f rot = RotateEuler(ang(z, arc::R), ang(y, arc::R), ang(x, arc::R));
  *this = (rot * X_HAT).XYZW();
  return *this;
} // end v4f& v4f::SetAngleEuler(angle, angle, angle)                         */


std::string dp::v4f::String(void) const
{
  std::string vecFormat;
  if (IsVector())
  {
    vecFormat = "<" + std::to_string(x) + ", " + std::to_string(y) + ", "
      + std::to_string(z) + ">";
  }
  else if (IsPoint())
  {
    vecFormat = "(" + std::to_string(x) + ", " + std::to_string(y) + ", "
      + std::to_string(z) + ")";
  }
  else
  {
    vecFormat = "[" + std::to_string(x) + ", " + std::to_string(y) + ", "
      + std::to_string(z) + ", " + std::to_string(w) + "]";
  }
  return vecFormat;
} // end std::string v4f::String(void) const                                  */


dp::v4f& dp::v4f::T(f4 ygt_value, const v2f& wrap)
{ t = ModF(RealF(ygt_value), wrap.max, wrap.min); return *this;
} // end v4f& v4f::T(f4, const v2f&)                                          */


dp::v4f dp::v4f::UnitVec(void) const
{
  v4f unit = *this; // Normalized copy of this
  unit.Normalize();
  return unit;
} // end v4f v4f::UnitVec(void) const                                         */


dp::v4f& dp::v4f::W(f4 waq_value, const v2f& clamp)
{ w = InF(RealF(waq_value), clamp.max, clamp.min); return *this;
} // end v4f& v4f::W(f4, const v2f&)                                          */


dp::v4f& dp::v4f::X(f4 xrs_value, const v2f& clamp)
{ x = InF(RealF(xrs_value), clamp.max, clamp.min); return *this;
} // end v4f& v4f::X(f4, const v2f&)                                          */


dp::v4f& dp::v4f::X1(f4 xrs_value, const v2f& clamp)
{ x1 = InF(RealF(xrs_value), clamp.max, clamp.min); return *this;
} // end v4f& v4f::X1(f4, const v2f&)                                         */


dp::v4f& dp::v4f::X2(f4 xrs_value, const v2f& clamp)
{ x2 = InF(RealF(xrs_value), clamp.max, clamp.min); return *this;
} // end v4f& v4f::X2(f4, const v2f&)                                         */


dp::v2f dp::v4f::XY(void) const
{ return v2f(x, y); } // end v2f v4f::XY(void) const                          */


dp::v3f dp::v4f::XXX(void) const
{ return v3f(x, x, x); } // end v3f v4f::XXX(void) const                      */


dp::v3f dp::v4f::XXY(void) const
{ return v3f(x, x, y); } // end v3f v4f::XXY(void) const                      */


dp::v3f dp::v4f::XXZ(void) const
{ return v3f(x, x, z); } // end v3f v4f::XXZ(void) const                      */


dp::v3f dp::v4f::XYX(void) const
{ return v3f(x, y, x); } // end v3f v4f::XYX(void) const                      */


dp::v3f dp::v4f::XYY(void) const
{ return v3f(x, y, y); } // end v3f v4f::XYY(void) const                      */


dp::v3f dp::v4f::XYZ(void) const
{ return v3f(x, y, z); } // end v3f v4f::XYZ(void) const                      */


dp::v3f dp::v4f::XZX(void) const
{ return v3f(x, z, x); } // end v3f v4f::XZX(void) const                      */


dp::v3f dp::v4f::XZY(void) const
{ return v3f(x, z, y); } // end v3f v4f::XZY(void) const                      */


dp::v3f dp::v4f::XZZ(void) const
{ return v3f(x, z, z); } // end v3f v4f::XZZ(void) const                      */


dp::v4f& dp::v4f::Y(f4 ygt_value, const v2f& wrap)
{
  y = InF(RealF(ygt_value), wrap.max, wrap.min); return *this;
} // end v4f& v4f::Y(f4, const v2f&)                                          */


dp::v4f& dp::v4f::Y1(f4 y1t_value, const v2f& clamp)
{
  y1 = InF(RealF(y1t_value), clamp.max, clamp.min); return *this;
} // end v4f& v4f::Y1(f4, const v2f&)                                         */


dp::v4f& dp::v4f::Y2(f4 y2q_value, const v2f& clamp)
{
  y2 = InF(RealF(y2q_value), clamp.max, clamp.min); return *this;
} // end v4f& v4f::Y2(f4, const v2f&)                                         */


dp::v3f dp::v4f::YXX(void) const
{ return v3f(y, x, x); } // end v3f v4f::YXX(void) const                      */


dp::v3f dp::v4f::YXY(void) const
{ return v3f(y, x, y); } // end v3f v4f::YXY(void) const                      */


dp::v3f dp::v4f::YXZ(void) const
{ return v3f(y, x, z); } // end v3f v4f::YXZ(void) const                      */


dp::v3f dp::v4f::YYX(void) const
{ return v3f(y, y, x); } // end v3f v4f::YYX(void) const                      */


dp::v3f dp::v4f::YYY(void) const
{ return v3f(y, y, y); } // end v3f v4f::YYY(void) const                      */


dp::v3f dp::v4f::YYZ(void) const
{ return v3f(y, y, z); } // end v3f v4f::YYZ(void) const                      */


dp::v3f dp::v4f::YZX(void) const
{ return v3f(y, z, x); } // end v3f v4f::YZX(void) const                      */


dp::v3f dp::v4f::YZY(void) const
{ return v3f(y, z, y); } // end v3f v4f::YZY(void) const                      */


dp::v3f dp::v4f::YZZ(void) const
{ return v3f(y, z, z); } // end v3f v4f::YZZ(void) const                      */


dp::v4f& dp::v4f::Z(f4 zbp_value, const v2f& clamp)
{
  z = InF(RealF(zbp_value), clamp.max, clamp.min); return *this;
} // end v4f& v4f::Z(f4, const v2f&)                                          */


dp::v3f dp::v4f::ZXX(void) const
{ return v3f(z, x, x); } // end v3f v4f::ZXX(void) const                      */


dp::v3f dp::v4f::ZXY(void) const
{ return v3f(z, x, y); } // end v3f v4f::ZXY(void) const                      */


dp::v3f dp::v4f::ZXZ(void) const
{ return v3f(z, x, z); } // end v3f v4f::ZXZ(void) const                      */


dp::v3f dp::v4f::ZYX(void) const
{ return v3f(z, y, x); } // end v3f v4f::ZYX(void) const                      */


dp::v3f dp::v4f::ZYY(void) const
{ return v3f(z, y, y); } // end v3f v4f::ZYY(void) const                      */


dp::v3f dp::v4f::ZYZ(void) const
{ return v3f(z, y, z); } // end v3f v4f::ZYZ(void) const                      */


dp::v3f dp::v4f::ZZX(void) const
{ return v3f(z, z, x); } // end v3f v4f::ZZX(void) const                      */


dp::v3f dp::v4f::ZZY(void) const
{ return v3f(z, z, y); } // end v3f v4f::ZZY(void) const                      */


dp::v3f dp::v4f::ZZZ(void) const
{ return v3f(z, z, z); } // end v3f v4f::ZZZ(void) const                      */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::v4f& dp::v4f::operator=(const v4f& source)
{ this->Set(source); return *this; } // end v4f& v4f::operator=(const v4f&)   */


dp::v4f& dp::v4f::operator=(v4f&& result) noexcept
{
  x = std::move(result.x);
  y = std::move(result.y);
  z = std::move(result.z);
  w = std::move(result.w);
  return *this;
} // end v4f& v4f::operator=(const v4f&)                                      */


dp::v4f dp::v4f::operator-(void) const
{ return v4f(-x, -y, -z, w); } // end v4f v4f::operator-(void) const          */


dp::v4f& dp::v4f::operator!(void)
{
  return Set(-x, -y, -z, w);
} // end v4f& v4f::operator!(void)                                            */


const f4& dp::v4f::operator[](s1 channel) const
{
  // bitmask wildly out of range values into lowest 4 unsigned values
  if (!isascii(channel) /* 'not' 'msb not set' */
    || (channel < 0x20  /* control seq */ && channel > 0xF /* hex range */))
  {
    Log::Diag(std::string("v4f::operator[] const - value out of range '")
      + char(channel) + "' read, ", false);
    channel &= VSUBS_MASK; // Clamp to [0,3] int range
    Log::W(std::string("setting to '") + std::to_string(channel) + "'", true);
  }
  else if (IsUpper(channel)) { channel = tolower(channel); }
  else if (IsDigit(channel)) { channel = ToNumeral(channel) & VSUBS_MASK; }
  switch (channel) // Lookup array index equivalent for return
  {
  case 'x': case 'r': case 's': case 'i': case 'm': case 'c': case 0:
    return x;
  case 'y': case 'g': case 't': case 'j': case 'n': case 'd': case 1:
    return y;
  case 'z': case 'b': case 'p': case 'k': case 'u': case 'e': case 2:
    return z;
  case 'w': case 'a': case 'q': case 'l': case 'v': case 'f': case 3:
    return w;
  default:
    Log::Diag(std::string("v4f::operator[] const - unsupported char ")
      + char(channel) + " read: default head 'x' == 0 used");
    return x;
  }

} // end const f4& v4f::operator[](s1) const                                  */


f4& dp::v4f::operator[](s1 channel)
{
  // bitmask out of range values into lower 4 values (% (+1) equivalent)
  if ((channel > VSUBS_MASK && (channel < ' ') || channel > 'z'))
  {
    Log::Diag(std::string("v4f::operator[] - value out of range '")
      + char(channel) + " read, ", false);
    channel &= VSUBS_MASK;
    Log::W(std::string("setting to '") + std::to_string(channel) + "'", true);
  }
  switch (channel) // Lookup array index equivalent for return
  {
  case 'x': case 'r': case 's': case 'i': case 'm': case 'c': case 0:
    return x;
  case 'y': case 'g': case 't': case 'j': case 'n': case 'd': case 1:
    return y;
  case 'z': case 'b': case 'p': case 'k': case 'u': case 'e': case 2:
    return z;
  case 'w': case 'a': case 'q': case 'l': case 'v': case 'f': case 3:
    return w;
  default:
    Log::Diag(std::string("v4f::operator[] - unrecognized char ")
      + char(channel) + " read: default head 'x' == 0 used");
    return x;
  }

} // end f4& v4f::operator[](s1)                                              */


dp::v4f dp::v4f::operator+(const v4f& other) const
{
  return v4f((x + other.x), (y + other.y), (z + other.z), (w + other.w));
} // end v4f v4f::operator+(const v4f&) const                                 */


dp::v4f& dp::v4f::operator+=(const v4f& other)
{
  Set((x + other.x), (y + other.y), (z + other.z), (w + other.w));
  return *this;

} // end v4f& v4f::operator+=(const v4f&)                                     */


dp::v4f dp::v4f::operator-(const v4f& other) const
{
  return v4f((x - other.x), (y - other.y), (z - other.z),
    (w - other.w));

} // end v4f v4f::operator-(const v4f&) const


dp::v4f& dp::v4f::operator-=(const v4f& other)
{
  Set((x - other.x), (y - other.y), (z - other.z), (w - other.w));
  return *this;

} // end v4f& v4f::operator-=(const v4f&)                                     */


dp::v4f dp::v4f::operator*(f4 scalar) const
{
  return v4f((scalar * x), (scalar * y), (scalar * z), (scalar * w));
} // end v4f v4f::operator*(f4) const                                         */


dp::v4f& dp::v4f::operator*=(f4 scalar)
{
  Set((scalar * x), (scalar * y), (scalar * z), (scalar * w));
  return *this;

} // end v4f v4f::operator*=(f4)                                              */


f4 dp::v4f::operator*(const v4f& other) const
{ return Dot(other); } // end f4 v4f::operator*(const v4f&) const             */


dp::v4f dp::v4f::operator/(f4 divisor) const
{
  // Get reciprocal of input: multiplication is preferable computationally
  f4 scalar = (1.0f / divisor); // Inverse divisor: a scalar value
  return ( (*this) * scalar );

} // end v4f v4f::operator/(f4) const                                         */


dp::v4f& dp::v4f::operator/=(f4 divisor)
{
  // Get reciprocal of input: multiplication is preferable computationally
  f4 scalar = (1.0f / divisor); // inverse divisor: a scalar value
  return ( (*this) *= scalar );

} // end v4f v4f::operator/=(f4)                                              */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

//! Zero vector in 4 dimensions; error state reserved value
const dp::v4f nullv4f_(0.0f, 0.0f, 0.0f, 0.0f);
const dp::v4f& dp::nullv4f = nullv4f_;

  //! Zero coordinate point definition in any given, spanning, 3D vector space
const dp::v4f ORIGIN_POINT_(0.0f, 0.0f, 0.0f, 1.0f);
const dp::v4f& dp::ORIGIN_POINT = ORIGIN_POINT_;

//! Unit vector in the positive-x axis
const dp::v4f UNIT_VEC_X_(1.0f, 0.0f, 0.0f, 0.0f);
const dp::v4f& dp::UNIT_VEC_X = UNIT_VEC_X_;

//! Unit vector in the positive-y axis
const dp::v4f UNIT_VEC_Y_(0.0f, 1.0f, 0.0f, 0.0f);
const dp::v4f& dp::UNIT_VEC_Y = UNIT_VEC_Y_;

//! Unit vector in the positive-z axis
const dp::v4f UNIT_VEC_Z_(0.0f, 0.0f, 1.0f, 0.0f);
const dp::v4f& dp::UNIT_VEC_Z = UNIT_VEC_Z_;

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::v4f dp::Distance(const v4f& initial, const v4f& terminal)
{
  v4f distanceVec = terminal;
  distanceVec -= initial;
  return distanceVec.MakeVector();

} // end v4f Distance(const v4f&, const v4f&)                                 */


dp::v4f dp::operator*(f4 scalar, const dp::v4f& rhs)
{
  v4f scaled((scalar * rhs.x), (scalar * rhs.y), (scalar * rhs.z), rhs.w);
  return scaled;

} // end v4f operator*(f4, const v4f&)                                        */


bool dp::operator==(const v4f& lhs, const v4f& rhs)
{
  return (NearF(lhs.x, rhs.x, PROXIMITY) &&
    NearF(lhs.y, rhs.y, PROXIMITY) && NearF(lhs.z, rhs.z, PROXIMITY)
    && NearF(lhs.w, rhs.w, PROXIMITY));

} // end bool operator==(const v4f&, const v4f&)                              */


bool dp::operator!=(const v4f& lhs, const v4f& rhs)
{
  return(!NearF(lhs.x, rhs.x, PROXIMITY) ||
    !NearF(lhs.y, rhs.y, PROXIMITY) || !NearF(lhs.z, rhs.z, PROXIMITY)
    || !NearF(lhs.w, rhs.w, PROXIMITY));

} // end bool operator!=(const v4f&, const v4f&)                              */


std::ostream& dp::operator<<(std::ostream& output, const v4f& vector)
{
  output << vector.String();
  return output;

} // end std::ostream& operator<<(std::ostream&, const v4f&)                  */
