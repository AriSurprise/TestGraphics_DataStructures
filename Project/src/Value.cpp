/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Value.cpp
Purpose:  Basic value manipulation functions / utilities
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_CS350_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2022
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
//#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

#include <sstream>  // string stream to control output precision
#include "ang.h"    // unit semantic wrapped 4-byte datum of angles for trig
#include "Log.h"
#include "v2f.h"    // paired float point data container (pass-in values, etc)
#include "Value.h"  // function declaration header file

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                        Public Function Definitions                         */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::ang ACosF(f4 cosine, bool invert) 
{
  cosine = static_cast<f4>(acos(cosine)); // acos range only [0,&pi]
  return dp::ang((invert ? cosine + PI : cosine), dp::arc::radians);
} // end ang ASinF(f4, bool)                                                  */

dp::ang ACos2F(f4 x, f4 r)
{
  SetIfF(r, 1.0f, NearF(r, 0.0f));
  x = static_cast<f4>(asin(x / r)); // asin range only [-&pi;/2, &pi;/2]
  return dp::ang((r < 0.0f ? x + PI : x), dp::arc::radians);
} // end f4 ASin2F(f4, f4)                                                    */

dp::ang ASinF(f4 sine, bool invert)
{
  sine = static_cast<f4>(asin(sine)); // asin range only [-&pi;/2,&pi;/2]
  return dp::ang((invert ? sine + PI : sine), dp::arc::radians);
} // end f4 ASinF(f4, bool)                                                   */
dp::ang ASin2F(f4 y, f4 r)
{
  SetIfF(r, 1.0f, NearF(r, 0.0f));
  y = static_cast<f4>(asin(y / r)); // asin range only [-&pi;/2, &pi;/2]
  return dp::ang((r < 0.0f ? y + PI : y), dp::arc::radians);
} // end f4 ASin2F(f4, f4)                                                    */

dp::ang ATanF(f4 tangent, bool dNeg)
{
  tangent = static_cast<f4>(atan(tangent)); // atan range [-&pi;/2, &pi;/2]
  return dp::ang((dNeg ? tangent + PI : tangent), dp::arc::radians);
} // end f4 ATanF(f4, bool)                                                   */

dp::ang ATan2F(f4 y, f4 x)
{
  y = static_cast<f4>(atan2(y, x));
  return dp::ang(y, dp::arc::radians);
} // end f4 ATan2F(f4, f4)                                                    */


inline std::string bits(u8 val, u1 bitTotal)
{
  std::string out;
  u8 in = val; // State record of original input value
  u4 log = (1ull << (bitTotal - 1)); // 
  while (val) // While any set bits remain in input
  {
    // Read LSB upward
    out += ((val & 1) ? '1' : '0'); // Add LSB state to output: set?'1':'0'
    val >>= 1; // Truncate (now read in) LSB from remaining input value
  }
  while (log > in) // Pad remaining 'leading' 0's for unset, trailing bits
  {
    out += '0';
    log >>= 1;
  }
  std::reverse(out.begin(), out.end());
  return out;
} // end std::string bits(u8, u1)                                             */


std::string BitsB(u1 val)
{ return bits(val, 8); } // end std::string BitsB(u1)                         */


std::string BitsD(f8 val)
{
  std::string out = BitsUL(*(u8*)&val);
  if (out.length() < 64) { out = std::string(64 - out.length(), '0') + out; }
  out = ((IsNegD(val)? "-1." : "+1.") + out.substr(12, out.length())
    + "b * 10b^{" + out.substr(1, 11) + "b - 01111111111b}");
  return out;
} // end std::string BitsD(f8)                                                */


std::string BitsF(f4 val)
{
  std::string out = BitsU(*(u4*)&val);
  if (out.length() < 32) { out = std::string(32 - out.length(), '0') + out; }
  out = (out[0] == '0'? "+1." : "-1.") + out.substr(9, out.length())
    + "b * 10b^{" + out.substr(1, 8) + "b - 01111111b}";
  return out;
} // end std::string BitsF(f4)                                                */


std::string BitsU(u4 val)
{ return bits(val, 32); } // end std::string BitsU(u4)                        */


std::string BitsUL(u8 val)
{ return bits(val, 64); } // end std::string BitsUL(u8)                       */
std::string BitsUS(u2 val)
{ return bits(val, 16); } // end std::string BitsUS(u2)                       */


s8 CeilD(f8 value)
{
  constexpr u1 mBits = 52u; // Maximum bits contained in the f8 mantissa
  constexpr u8 mMask = 0xFFFFFFFFFFFFF; // Mask mantissa bits only on
  constexpr u8 lBit = 0x10000000000000; // Inverse mask; restore lead mant bit
  constexpr s1 OoBits = -11; // mBits=:53-(-11)=64 == bitsIn(s8) => overflow
  s2 fBits = mBits+1 - ExpD(value);// Sub-unit #bits for |value| ( <= 2^{-1})
  bool neg = IsNegD(value); // Overall input value was originally negatiive (?)
  if (fBits <= OoBits) { return IfOrL(s8_MIN, s8_MAX, neg); } // under/overflow
  s8 bVal = ((*(s8*)&value) & mMask) ^ lBit; // Mantissa binary absolute value
  bool big = IsNegS(fBits); // exponent > SI_In(f4) (generalizes big #'s) (?)
  bool small = (mBits < fBits); // Sub-unit place bits exist by |value| (?)
  bool fractional = !(bVal == ((bVal >> fBits) << fBits));// Sub-unit bits used
  s1 n = IfOrC(-1, 1, neg); // Value negation restorative scalar
  // (-/+)(big#? (mantissaAbs*2^{intRangeScale}) : (0:1 + mantissaAbsVal:0)
  return n * IfOrL(bVal << -fBits, (fractional + !small*(bVal >> fBits)), big);
} // end s8 CeilD(f8)                                                         */

s4 CeilF(f4 value)
{
  constexpr u1 mBits = 23u; // Maximum bits contained in the f4 mantissa
  constexpr u4 mMask = 0x7FFFFF;  // Mask mantissa bits only on, not sign | exp
  constexpr u4 lBit  = 0x800000;  // Inverse mask to restore truncated lead bit
  constexpr s1 OoBits = -8; // mBits=:24-(-8)=32 == bitsIn(s4) => overflow
  s2 fBits = mBits+1 - ExpF(value);// Sub-unit bits in |value| (2^{-1} or less)
  bool neg = IsNegF(value); // Overall input value was originally negatiive (?)
  if (fBits <= OoBits) { return IfOrI(s4_MIN, s4_MAX, neg); } // under/overflow
  s4 bVal = ((*(s4*)&value) & mMask) ^ lBit; // Mantissa binary absolute value
  bool big = IsNegS(fBits); // exponent > SI_In(f4) (generalizes big #'s) (?)
  bool small = (mBits < fBits); // Sub-unit place bits exist by |value| (?)
  bool fractional = (small&&bVal^lBit) || !(bVal == ((bVal>>fBits)<<fBits));
  s1 n = IfOrC(-1, 1, neg); // Value negation restorative scalar
  // (-/+)(big#? (mantissaAbs*2^{intRangeScale}) : (0:1 + mantissaAbsVal:0)
  return n * IfOrI(bVal << -fBits, (fractional + !small*(bVal >> fBits)), big);

} // end s4 CeilF(f4)                                                         */


f4 CosF(dp::ang angle)
{ return static_cast<f4>(cos(angle.rM())); } // end f4 CosF(ang)              */


s8 FloorD(const f8& value)
{
  constexpr u1 mBits = 52u; // Maximum bits contained in the f8 mantissa
  constexpr u8 mMask = 0xFFFFFFFFFFFFF; // Mask mantissa bits only on
  constexpr u8 lBit = 0x10000000000000; // Inverse mask; restore lead mant bit
  constexpr s1 OoBits = -11; // mBits=:53-(-11)=64 == bitsIn(s8) => overflow
  s2 fBits = mBits+1 - ExpD(value); // Floating point bits in mantissa by scale
  bool neg = IsNegD(value); // Negation restorative scalar
  if (fBits <= OoBits) { return IfOrL(s8_MIN, s8_MAX, neg); }
  s8 bVal = ((*(s8*)&value) & mMask) ^ lBit; // Absolute value of mantissa
  bool big = IsNegS(fBits); // exponent > SI_In(f8) (generalizes big #'s) (?)
  bool small = (mBits < fBits); // Sub-unit place bits exist by |value| (?)
  s1 n = IfOrC(-1, 1, neg); // Negation restorative scalar
  return IfOrL(0, n * IfOrL(bVal << -fBits, bVal >> fBits, big), small);

} // end s8 FloorD(const f8&)                                                 */

s4 FloorF(f4 value)
{
  constexpr u1 mBits = 23u; // Maximum bits contained in the f4 mantissa
  constexpr u4 mMask = 0x7FFFFF; // Mask mantissa bits only on, not sign | exp
  constexpr u4 lBit = 0x800000; // Inverse mask to restore truncated lead bit
  constexpr s1 OoBits = -8; // mBits=:24-(-8)=32 == bitsIn(s4) => overflow
  s2 fBits = mBits+1 - ExpF(value); // Floating point bits in mantissa by scale
  bool neg = IsNegF(value); // Negation restorative scalar
  if (fBits <= OoBits) { return IfOrI(s4_MIN, s4_MAX, neg); }
  s4 bVal = ((*(s4*)&value) & mMask) ^ lBit; // Absolute value of mantissa
  bool big = IsNegS(fBits); // exponent > SI_In(f4) (generalizes big #'s) (?)
  bool small = (mBits < fBits); // Sub-unit place bits exist by |value| (?)
  s1 n = IfOrC(-1, 1, neg); // Negation restorative scalar
  return IfOrI(0, n * IfOrI(bVal << -fBits, bVal >> fBits, big), small);

} // end s4 FloorF(f4)                                                        */


bool QuadraticF(f4 a, f4 b, f4 c, dp::v2f& real, dp::v2f* imag)
{
f4 discriminant = b * b - 4 * a * c; // discriminant =: sqrt(b^{2} - 4*a*c)
if (imag) { imag->Set(0.0f, 0.0f); }
if (NearF(a, 0.0f)) // ~0*x^{2} => not quadratic; can't eventually do '/2a'
{
  // x/0 => limit(+x/-0 || -x/+0)-> -INf, limit(-x/-0 || +x/+0)-> +INf
  real.x = real.y = (std::signbit(a) ^ std::signbit(b) ? -INf : INf); 
  // Never not (b^{2} - 0) > 0  => no imag factor
  return false; // no quadratic term => no solution pair
}
if (discriminant < 0.0f) // => imaginary results
{
  if (imag) // => don't shortcut
  {
    real.x = real.y = AHALF * b / a;
    discriminant = SqrtF(-discriminant); // (-) => factor out i
    imag->Set(discriminant, -discriminant);
  }
  return false; // imaginary results found
}
real.x = real.y = -b;
if (discriminant > 0.0f)
{
  discriminant = SqrtF(discriminant);
  real.x -= discriminant;
  real.y += discriminant;
}
real *= (AHALF / a); // (-b (+/-) discriminant) => / 2*a
if (real.x > real.y) SwapF(real.x, real.y); // set ordered as needed
return true;

} // end bool QuadraticF(f4, f4, f4, v2f&, v2f*)                              */


s8 RoundD(f8 value)
{
  constexpr u1 mBits = 52u; // Maximum bits contained in the f8 mantissa
  constexpr u8 mMask = 0xFFFFFFFFFFFFF; // Mask mantissa bits only on
  constexpr u8 lBit = 0x10000000000000; // Inverse mask; restore lead mant bit
  constexpr s1 OoBits = -11; // mBits=:53-(-11)=64 == bitsIn(s8) => overflow
  s2 fBits = mBits+1 - ExpD(value); // Floating point bits in mantissa by scale
  bool neg = IsNegD(value); // Negation restorative scalar
  if (fBits <= OoBits) { return IfOrL(s8_MIN,s8_MAX,neg); } // over/underflows
  s8 bVal = ((*(s8*)&value) & mMask) ^ lBit; // Absolute value of mantissa
  bool huge = IsNegS(fBits); // exponent > SI_In(f4) (generalizes big #'s) (?)
  bool small = (mBits+1 < fBits);
  bool half = !small && fBits > 0 && (1ull << (fBits-1)) & bVal;// 1/2 on (?)
  // (no bits >= 2^{0} place (?) => no iBits in value; small number
  s1 n = IfOrC(-1, 1, neg); // Value negation restorative scalar
  // (-/+)(huge#?(mantissaAbs*2^{iBitsLost}) : ((0:1) + (mantissaAbsVal:0))
  return n * IfOrL(bVal<<-fBits, (half + !small*(bVal>>fBits)), huge);

} // end s8 Round(const f8&)                                                  */

s4 RoundF(f4 value)
{
  constexpr u1 mBits = 23u; // Maximum bits contained in the f4 mantissa
  constexpr u4 mMask = 0x7FFFFF; // Mask mantissa bits only on, not sign | exp
  constexpr u4 lBit  = 0x800000; // Inverse mask to restore truncated lead bit
  constexpr s1 OoBits = -8; // mBits=:24-(-8)=32 == bitsIn(s4) => overflow
  s2 fBits = mBits+1 - ExpF(value); // Floating point bits in mantissa by scale
  bool neg = IsNegF(value); // Negation restorative scalar
  if (fBits <= OoBits) { return IfOrI(s4_MIN,s4_MAX,neg); } // over/underflows
  s4 bVal = ((*(s4*)&value) & mMask) ^ lBit; // Absolute value of mantissa
  bool huge = IsNegS(fBits); // exponent > SI_In(f4) (generalizes big #'s) (?)
  bool small = (mBits+1 < fBits);
  bool half = !small && fBits > 0 && (1ull << (fBits-1)) & bVal;// 1/2 on (?)
  // (no bits >= 2^{0} place (?) => no iBits in value; small number
  s1 n = IfOrC(-1, 1, neg); // Value negation restorative scalar
  // (-/+)(huge#?(mantissaAbs*2^{iBitsLost}) : ((0:1) + (mantissaAbsVal:0))
  return n * IfOrI(bVal<<-fBits, (half + !small*(bVal>>fBits)), huge);

} // end s4 RoundF(f4)                                                        */


std::string SciD(f8 val)
{
  s2 exp = (((*(u8*)&val & 0x7FF0000000000000) >> 52) - 1023);
  std::string out;                  //~FFF0000000000000
  out = std::to_string((((*(u8*)&val) & 0xFFFFFFFFFFFFF) / (f8)(1ull << 52)));
  out = (IsNegD(val) ? "-1." : "+1.") + out.substr(2, out.length()) +
    " * 2^{" + std::to_string(exp) + "}";
  return out;
} // end std::string SciD(f8)                                                 */


std::string SciF(f4 val)
{
  s2 exp = (((*(u4*)&val & 0x7F800000) >> 23) - 127);
  std::string out;
  out = std::to_string((((*(u4*)&val) & 0x7FFFFF) / (f4)(1 << 23)));
  out = (IsNegF(val) ? "-1." : "+1.") + out.substr(2, out.length()) +
    " * 2^{" + std::to_string(exp) + "}";
  return out;
} // end std::string SciF(f4)                                                 */


f4 SinF(dp::ang angle)
{ return static_cast<f4>(sin(angle.rM())); } // end f4 SinF(ang)              */


std::string StrB(u1 value)
{
  return std::to_string((u2)value);
} // end std::string StrB(u1)                                                 */


std::string StrC(s1 value)
{
  return std::to_string((s2)value);
} // end std::string StrC(s1)                                                 */


std::string StrD(const f8& value, u1 fracDigits)
{
  static std::stringstream ss;
  ss << value;
  return ss.str();
} // end std::string StrF(f4, ui)                                             */


std::string StrF(f4 value, u1 fracDigits)
{
  static std::stringstream ss;
  ss << value;
  return ss.str();
} // end std::string StrF(f4, ui)                                             */


std::string StrL(const s8& value)
{
  return std::to_string(value);
} // end std::string StrL(const s8&)                                          */


std::string StrI(s4 value)
{
  return std::to_string(value);
} // end std::string StrI(s4)                                                 */


std::string StrP(void* value)
{
  static std::stringstream ss;
  ss << std::hex << (um)value;
  return ss.str();
} // end std::string StrP(void*)                                              */


std::string StrS(s2 value)
{
  return std::to_string(value);
} // end std::string StrS(s2)                                                 */


std::string StrU(s4 value)
{
  return std::to_string(value);
} // end std::string StrU(s4)                                                 */


std::string StrUL(const u8& value)
{
  return std::to_string(value);
} // end std::string StrUL(const u8&)                                         */


std::string StrUM(um value)
{
  return std::to_string(value);
} // end std::string StrUM(um)                                                */


std::string StrUS(u2 value)
{
  return std::to_string(value);
} // end std::string StrUS(u2)                                                */


f4 TanF(dp::ang angle)
{ return static_cast<f4>(tan(angle.rM())); } // end f4 TanF(ang)              */
