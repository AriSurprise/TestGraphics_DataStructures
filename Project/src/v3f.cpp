/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v3f.cpp
Purpose:  API agnostic, multipurpose, 3 dimensional float-point element vector
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

#include <ostream>    // Output stream definition for operator overload
#include "ang.h"      // Unit measures for conversion in and out of arc spans
#include "Log.h"      // System message management for errors, warnings, etc
#include "m3f.h"      // Generating matrices from vector data
#include "Value.h"    // NearF() float point (proximity as equality) function
#include "v2f.h"      // Swizzling truncation to lower dimension vectors
#include "v3f.h"      // Class declaration header
#include "v4f.h"      // Swizzling & promotion to larger dimension vectors


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

static const u1 SUBSCRIPTS = 3u;        //! v3f class is fixed at 3 subscripts

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Creates a 3 dimensional vector / point (default <0,0,0>)
dp::v3f::v3f(f4 xr_value, f4 yg_value, f4 zb_value)
  : x(xr_value), y(yg_value), z(zb_value)
{ } // end v3f::v3f(f4, f4, f4)                                               */


dp::v3f::v3f(ang angle, PLANE on, f4 magnitude)
{
  f4 s = magnitude * SinF(angle), c = magnitude * CosF(angle);

  switch (on)
  {
  case PLANE::XY: default:
    X(c).Y(s).Z(0.0f); break;
  case PLANE::YX:
    X(-c).Y(s).Z(0.0f); break;
  case PLANE::YZ:
    X(0.0f).Y(c).Z(s); break;
  case PLANE::ZY:
    X(0.0f).Y(-c).Z(s); break;
  case PLANE::XZ:
    X(-c).Y(0.0f).Z(s); break;
  case PLANE::ZX:
    X(c).Y(0.0f).Z(s); break;
  }
} // end v3f::v3f(ang, PLANE, f4)                                             */


dp::v3f::v3f(const v3f& source)
  : v3f(source.x, source.y, source.z)
{ } // end v3f::v3f(const v3f&)                                               */


dp::v3f::v3f(v3f&& result) noexcept
{ *this = std::move(result); } // end v3f::v3f(v3f&&)                         */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

bool dp::v3f::Colinear(const v3f& other) const
{
  // get scalar ratio of other:this, from first non-zero axis (if any, else 0)
  f4 sr = (NearF(x, 0.0f) ? other.x / x : NearF(other.x, 0.0f) ? NaNf
         : NearF(y, 0.0f) ? other.y / y : NearF(other.y, 0.0f) ? NaNf
         : NearF(z, 0.0f) ? other.z / z : NearF(other.z, 0.0f) ? NaNf : 0.0f);
  if (sr == NaNf) { return false; }
  // colinear if this, scaled to other in 1 axis, is then ~equal in all axes
  return ((*this) * sr) == other;

} // end bool v3f::Colinear(const v3f&) const                                 */

dp::m3f dp::v3f::CrossMatrix(void) const
{
  return m3f(0.0f,  -z,  y,
               z,0.0f,  -x,
              -y,   x,0.0f);

} // end m3f v3f::CrossMatrix(void) const                                     */


std::string dp::v3f::JSON(void) const
{
  return "[" + std::to_string(x) + "," + std::to_string(y) + ","
    + std::to_string(z) + "]";
} // end std::string v3f::JSON(void) const                                    */

dp::m3f dp::v3f::DotTranspose(const v3f& other) const
{
  return  m3f((x * other.x), (x * other.y), (x * other.z),
              (y * other.x), (y * other.y), (y * other.z),
              (z * other.x), (z * other.y), (z * other.z));

} // end m4f v3f::DotTranspose(const v3f&) const                              */


dp::v3f& dp::v3f::Normalize(f4* length)
{
  f4 len = LengthSquared();
  if (!NearF(len, 1.0f)) // if already unit length vector, can shortcut
  {
    if (!NearF(len, 0.0f)) 
    {
      len = SqrtF(len);
      *this /= len;
    }
    else // null vector can't be made a unit vector: default a value of +x
    {
      Log::Info("v3f::normalize - null vector input: default +x set");
      X(1.0f).Y(0.0f).Z(0.0f);
    }
  }
  if (length) { *length = len; }
  return *this;
} // end v3f& v3f::Normalize(f4*)                                             */

dp::v4f dp::v3f::Opaque(void) const
{ return XYZW(1.0f); } // end v4f v3f::Opaque(void) const                     */


dp::v3f& dp::v3f::Set(const v4f& source)
{
  X(source.x).Y(source.y).Z(source.z); return *this;
} // end v3f& v3f::Set(const v4f&)                                            */

dp::v3f& dp::v3f::SetAngleEuler(ang z_rot, ang y_rot, ang x_rot)
{
  // Check if shortcut math can be taken for 1D rotation for early return
  if (NearF(y_rot.rM(), 0.0f, _INf) && NearF(x_rot.rM(), 0.0f, _INf))
  {
    // Rotation about +z has +x right & +y up (Q1)
    X(cosf(z_rot.rM())).Y(sinf(z_rot.rM())).Z(0);
  }
  else if (NearF(z_rot.rM(), 0.0f, _INf) && NearF(x_rot.rM(), 0.0f, _INf))
  {
    // Rotation about +y has +x right & +z down (Q1), or +z right & +x up (Q4)
    X(cosf(y_rot.rM())).Y(0).Z(-sinf(y_rot.rM()));
  }
  else if (NearF(z_rot.rM(), 0.0f, _INf) && NearF(y_rot.rM(), 0.0f, _INf))
  {
    // Rotation about +x has +z left & +y up (Q1), or +y right & +z up (Q2)
    X(0).Y(sinf(x_rot.rM())).Z(-cosf(x_rot.rM()));
  }
  // Perform rotation by matrix from input euler angles
  m3f rot = RotateEuler(ang(z, arc::R), ang(y, arc::R), ang(x, arc::R));
  this->Set(rot * X_HAT);
  return *this;

} // end v3f& v3f::SetAngleEuler(angle, angle, angle)                         */


dp::v3f& dp::v3f::ToSqrts(void)
{
  f4 c; if (NearF(x, y))
  {
    c = SqrtF(x);
    if (NearF(y, z)) { return Set(c, c, c); return Set(c, c, SqrtF(z)); }
  }
  if (NearF(y, z)) { c = SqrtF(y); return Set(SqrtF(x), c, c); }
  if (NearF(x, z)) { c = SqrtF(x); return Set(c, SqrtF(y), c); }
  return Set(SqrtF(x), SqrtF(y), SqrtF(z));

} // end v3f& v3f::ToSqrts(void)                                              */


std::string dp::v3f::String(void) const
{
  std::string vecFormat;
  vecFormat = "[";
  if (x >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(x) + ", ";
  if (y >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(y) + ", ";
  if (z >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(z) + " ]";
  return vecFormat;
} // end std::string v3f::String(void) const                                  */


dp::v4f dp::v3f::Transparent(void) const
{ return XYZW(0.0f); } // end v4f Transparent(void) const                     */


dp::v3f dp::v3f::UnitVec(f4* length) const
{
  f4 l = LengthSquared();
  if (NearF(l, 1.0f))
  {
    if (length) { *length = l; }
    return Copy();
  }
  if (length) { *length = l; }
  SetInvF(l);
  return v3f(x*l, y*l, z*l);
} // end v3f v3f::UnitVec(void) const                                         */

dp::v3f& dp::v3f::X(f4 xr_value, const dp::v2f& clamp)
{
  x = InF(xr_value, clamp.max, clamp.min);
  return *this;
} // end v3f& v3f::X(f4, const v2f&)                                          */

dp::v2f dp::v3f::XX(void) const
{ return v2f(x, x); } // end v2f v3f::XX(void) const                          */

dp::v2f dp::v3f::XY(void) const
{ return v2f(x, y); } // end v2f v3f::XY(void) const                          */

dp::v2f dp::v3f::XZ(void) const
{ return v2f(x, z); } // end v2f v3f::XZ(void) const                          */

dp::v4f dp::v3f::XYZW(f4 waq_value) const
{ return v4f(x, y, z, waq_value); } // end v2f v3f::XYZW(void) const          */

dp::v3f& dp::v3f::Y(f4 yg_value, const dp::v2f& clamp)
{
  y = InF(yg_value, clamp.max, clamp.min);
  return *this;
} // end v3f& v3f::Y(f4, const v2f&)                                          */

dp::v2f dp::v3f::YX(void) const
{ return v2f(y, x); } // end v2f v3f::YX(void) const                          */

dp::v2f dp::v3f::YY(void) const
{ return v2f(y, y); } // end v2f v3f::YY(void) const                          */

dp::v2f dp::v3f::YZ(void) const
{ return v2f(y, z); } // end v2f v3f::YZ(void) const                          */

dp::v3f& dp::v3f::Z(f4 zb_value, const dp::v2f& clamp)
{
  z = InF(zb_value, clamp.max, clamp.min);
  return *this;
} // end v3f& v3f::Z(f4, const v2f&)                                          */

dp::v2f dp::v3f::ZX(void) const
{ return v2f(z, x); } // end v2f v3f::ZX(void) const                          */

dp::v2f dp::v3f::ZY(void) const
{ return v2f(z, y); } // end v2f v3f::ZY(void) const                          */

dp::v2f dp::v3f::ZZ(void) const
{ return v2f(z, z); } // end v2f v3f::ZZ(void) const                          */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Zero vector in 3 dimensions; error state reserved value
const dp::v3f nullv3f_(0.0f, 0.0f, 0.0f);
const dp::v3f& dp::nullv3f = nullv3f_;

// -Infinity vector in 3 dimensions
const dp::v3f minv3f_( -INf, -INf, -INf);
const dp::v3f& dp::minv3f = minv3f_;

// +Infinity vector in 3 dimensions
const dp::v3f maxv3f_(  INf,  INf,  INf);
const dp::v3f& dp::maxv3f = maxv3f_;

// Out of Bounds result; NaN vector in 3 dimensions: ubiquitous error state
const dp::v3f oob3_( NaNf, NaNf, NaNf);
const dp::v3f& dp::oob3 = oob3_;

// 3D unit vector in the x-axis of the local coordinate space
const dp::v3f X_HAT_(1.0f, 0.0f, 0.0f);
const dp::v3f& dp::X_HAT = X_HAT_;

// 3D unit vector in the y-axis of the local coordinate space
const dp::v3f Y_HAT_(0.0f, 1.0f, 0.0f);
const dp::v3f& dp::Y_HAT = Y_HAT_;

// 3D unit vector in the z-axis of the local coordinate space
const dp::v3f Z_HAT_(0.0f, 0.0f, 1.0f);
const dp::v3f& dp::Z_HAT = Z_HAT_;

// #000000: 32-bit float-point color values in R(0.0f) G(0.0f) B(0.0f)
const dp::v3f& dp::BLACK = nullv3f_;

// #0000FF: 32-bit float-point color values in R(0.0f) G(0.0f) B(1.0f)
const dp::v3f BLUE_(0.0f, 0.0f, 1.0f);
const dp::v3f& dp::BLUE = BLUE_;

// #00FFFF: 32-bit float-point color values in R(0.0f) G(1.0f) B(1.0f)
const dp::v3f CYAN_(0.0f, 1.0f, 1.0f);
const dp::v3f& dp::CYAN = CYAN_;

// #1A1A1A: 32-bit float-point color values in R(0.1f) G(0.1f) B(0.1f)
const dp::v3f GRAY10_(0.1f, 0.1f, 0.1f);
const dp::v3f& dp::GRAY_10 = GRAY10_;

// #333333: 32-bit float-point color values in R(0.2f) G(0.2f) B(0.2f)
const dp::v3f GRAY20_(0.2f, 0.2f, 0.2f);
const dp::v3f& dp::GRAY_20 = GRAY20_;

// #4D4D4D: 32-bit float-point color values in R(0.3f) G(0.3f) B(0.3f)
const dp::v3f GRAY30_(0.3f, 0.3f, 0.3f);
const dp::v3f& dp::GRAY_30 = GRAY30_;

// #666666: 32-bit float-point color values in R(0.4f) G(0.4f) B(0.4f)
const dp::v3f GRAY40_(0.4f, 0.4f, 0.4f);
const dp::v3f& dp::GRAY_40 = GRAY40_;

// #999999: 32-bit float-point color values in R(0.6f) G(0.6f) B(0.6f)
const dp::v3f GRAY60_(0.6f, 0.6f, 0.6f);
const dp::v3f& dp::GRAY_60 = GRAY60_;

// #B2B2B2: 32-bit float-point color values in R(0.7f) G(0.7f) B(0.7f)
const dp::v3f GRAY70_(0.7f, 0.7f, 0.7f);
const dp::v3f& dp::GRAY_70 = GRAY70_;

// #CCCCCC: 32-bit float-point color values in R(0.8f) G(0.8f) B(0.8f)
const dp::v3f GRAY80_(0.8f, 0.8f, 0.8f);
const dp::v3f& dp::GRAY_80 = GRAY80_;

// #E5E5E5: 32-bit float-point color values in R(0.9f) G(0.9f) B(0.9f)
const dp::v3f GRAY90_(0.9f, 0.9f, 0.9f);
const dp::v3f& dp::GRAY_90 = GRAY90_;

// #00FF00: 32-bit float-point color values in R(0.0f) G(1.0f) B(0.0f)
const dp::v3f GREEN_(0.0f, 1.0f, 0.0f);
const dp::v3f& dp::GREEN = GREEN_;

// 7F7F7F: 32-bit float-point color values in R(0.50f) G(0.50f) B(0.50f)
const dp::v3f HALF_TONE_(0.5f, 0.5f, 0.5f);
const dp::v3f& dp::HALF_TONE = HALF_TONE_;
const dp::v3f& dp::GRAY_50 = HALF_TONE_;

// #FF00FF: 32-bit float-point color values in R(1.0f) G(0.0f) B(1.0f)
const dp::v3f MAGENTA_(1.0f, 0.0f, 1.0f);
const dp::v3f& dp::MAGENTA = MAGENTA_;

// #3F3F3F: 32-bit float-point color values in R(0.25f) G(0.25f) B(0.25f)
const dp::v3f QUARTER_BLACK_(0.25f, 0.25f, 0.25f);
const dp::v3f& QUARTER_BLACK = QUARTER_BLACK_;

// #BFBFBF: 32-bit float-point color values in R(0.75f) G(0.75f) B(0.75f)
const dp::v3f QUARTER_WHITE_(0.75f, 0.75f, 0.75f);
const dp::v3f& QUARTER_WHITE = QUARTER_WHITE_;

// #FF0000: 32-bit float-point color values in R(1.0f) G(0.0f) B(0.0f)
const dp::v3f RED_(1.0f, 0.0f, 0.0f);
const dp::v3f& dp::RED = RED_;

// #FFFFFF: 32-bit float-point color values in R(1.0f) G(1.0f) B(1.0f)
const dp::v3f WHITE_(1.0f, 1.0f, 1.0f);
const dp::v3f& dp::WHITE = WHITE_;

// #FFFF00: 32-bit float-point color values in R(1.0f) G(1.0f) B(0.0f)
const dp::v3f YELLOW_(1.0f, 1.0f, 0.0f);
const dp::v3f& dp::YELLOW = YELLOW_;


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Overloads                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// hm...all simple semantic sugar corrections & inlined...helper functions too

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
