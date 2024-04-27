/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Value.h
Purpose:  Basic value manipulation functions / utilities
Notes:  A few reasons can present themselves to prefer header centralized
  implementation files: templated arguments, inline functions, et cetera, for
  which only header files containing the actual implementation are supported.
  Due to the simplicity of these value manipulation functions, inline
  definitions make sense in making these simple, palatable syntax shifts, to
  not incur any extra overhead in generating function frames in the process.
  Although the decision for inline functions does render the decision to use
  c-styled header include guarding moot (not needing a linked file at all),
  it nonetheless remains as a signifier to the desired overhead minimization.
  By extension, this decision more specifically means the functions' names use
  hungarian notation for type specificity instead of templating or overloading.
  Generally, these functions are mostly intended for primitive use.  While
  templates could offer more versatility, primitives are finite and avoiding
  templating similarly serves to lessen overhead.  Relying on overloading and
  managed name mangling would be the only reason to use a separate file for
  implementation.  This alternative being a nicety, an overloaded design would
  only stand to lower type accountability from the end user, as well as running
  counter to the above stated goals of a minimal overhead.
  The decision to still use an include guard is a nod to the designed,
  intentional omission of overloading for these simplistic functions.
  Additionally, the symmetrical lack of an implementation file in templating
  means that when it becomes desirable to target non-primitive data for these
  simple value managing functions, this design strategy extends to support it.
  The only potential downside is in an abstraction loss, but simple processes
  do not need a black box.  Quite the opposite-- the transparent details serve
  to reinforce that these functions are primarily syntactical shorthand, aren't
  intricate operations, & while insufficiently brief without, are needed often.
  Mathematical constant declarations using constexpr insted of pre-processor
  define clauses isn't strictly compatible with c standards, but are more
  compatible with wider specification standards, and the incongruity is a
  reminder to these cumulative reasonings (supporting templated classes in the
  overall design strategy, as well as the basic hungarian notation functions).
  Similarly, function parameters using references instead of pointers
  necessitating indirection operators are not strictly C compatible, but make
  the public facing implementation much easier for most audiences to read.
Project:  a.Surprise_CS350_1
Author:   Ari Surprise, a.surprise, 40000218
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Creation date:  Summer 2022
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#ifndef PRIMITIVE_VALUE_MANIP_H
#define PRIMITIVE_VALUE_MANIP_H

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <stl>
#include <cmath>            // isnan() | isinf() | signbit() float validation
#include <limits>           // "INFINITY" is too long an identifier
#include <initializer_list> // indefinite parameter count lists
#include <stdexcept>        // Set operation to prompt for input => exceptions
#include <vector>           // iterable container function parameters
// "./src/..."
#include "Log.h"        // System message management for errors, warnings, etc
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp
{
  class ang;
  union v2f;
}


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Public Constants                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

//! Constant storage of precise 32-bit float null value: 0.0
constexpr f4 Of = 0.0f;

//! Constant storage of precise 32-bit float unit value: 1.0
constexpr f4 If = 1.0f;

//! Approximation of e (Euler's number)
constexpr f4 Ef = 2.718281828459045f;

//! Approximation of 1 / e (Euler's number ; reciprocal of e)
constexpr f4 _Ef = 0.36787944117144232159552377016146f;

//! Approximation of e (Euler's number)
constexpr f8 E = 2.718281828459045235360287471352662497757247093699959574966967;

//! Approximation of 1 / e (Euler's number ; reciprocal of e)
constexpr f8 _E = 0.36787944117144232159552377016146086744581113103176783450783;

//! Floating point reserved value for positive infinity (4-byte field)
constexpr f4 INf = std::numeric_limits<f4>::infinity();

//! 0.000001 => 1/INf: Floating point infinitesimal, near 0.0f precision epsilon
constexpr f4 _INf = 0.000001f;

//! Floating point reserved value for positive infinity (8-byte field)
constexpr f8 INF = std::numeric_limits<f8>::infinity();

//! 0.00000000001 => 1/INF: Floating point infinitesimal, ~0.0 precision epsilon
constexpr f8 _INF = 0.00000000001;

//! Floating point reserved value for positive infinity (4-byte field)
constexpr f4 NaNf = std::numeric_limits<f4>::quiet_NaN();

//! Floating point reserved value for non-numeric results (8-byte field)
constexpr f8 NaN = std::numeric_limits<f8>::quiet_NaN();

//! Approximation of the circle &pi; constant
constexpr f4 PI = 3.141592653589793f;

//! Approximation of 1 / &pi (reciprocal of &pi; ; division by &pi;)
constexpr f4 _PI = 0.31830988618379067153776752674503f;

//! Approximation of &pi; / 2 (radian equivalent of 90.0 degrees)
constexpr f4 PI_2 = 1.570796326794897f;

//! Approximation of &pi; / 3 (radian equivalent of 60.0 degrees)
constexpr f4 PI_3 = 1.047197551196598f;

//! Approximation of &pi; / 4 (radian equivalent of 45.0 degrees)
constexpr f4 PI_4 = 0.785398163397448f;

//! Approximation of &pi; / 6 (radian equivalent of 30.0 degrees)
constexpr f4 PI_6 = 0.52359877559829887307710723054658f;

//! Approximation of &pi; / 12 (radian equivalent of 15.0 degrees)
constexpr f4 PI_12 = 0.26179938779914943653855361527329f;

//! Approximation of &pi; / 18 (radian equivalent of 10.0 degrees)
constexpr f4 PI_18 = 0.17453292519943295769236907684886f;

//! Approximation of &pi; / 24 (radian equivalent of 7.5 degrees)
constexpr f4 PI_24 = 0.13089969389957471826927680763665f;

//! Approximation of &pi; / 36 (radian equivalent of 5.0 degrees)
constexpr f4 PI_36 = 0.08726646259971647884618453842443f;

//! Approximation of &pi; / 45 (radian equivalent of 4.0 degrees)
constexpr f4 PI_45 = 0.06981317007977318307694763073954f;

//! Approximation of &pi; / 60 (radian equivalent of 3.0 degrees)
constexpr f4 PI_60 = 0.05235987755982988730771072305466f;

//! Approximation of &pi; / 72 (radian equivalent of 2.5 degrees)
constexpr f4 PI_72 = 0.04363323129985823942309226921222f;

//! Approximation of &pi; / 90 (radian equivalent of 2.0 degrees)
constexpr f4 PI_90 = 0.03490658503988659153847381536977f;

//! Approximation of &pi; / 180 (radian equivalent of 1 degree)
constexpr f4 PI_180 = 0.01745329251994329576923690768489f;

//! Approximation of &pi; / 360 (radian equivalent of a 1/2 degree)
constexpr f4 PI_360 = 0.00872664625997164788461845384244f;

//! Approximation of &pi; / 180 (radian equivalent of a 1/3 degree)
constexpr f4 PI_440 = 0.00713998330361316645105146223473f;

//! Approximation of &pi; / 720 (radian equivalent of a 1/4 degree)
constexpr f4 PI_720 = 0.00436332312998582394230922692122f;

//! Approximation of the square root of 2
constexpr f4 ROOT2 = 1.414213562373095f;

//! Approximation of 1 / sqrt(2) (reciprocal of sqrt(2) ; == (sqrt(2) / 2) )
constexpr f4 _ROOT2 = 0.7071067811865475f;

//! Approximation half of the square root of 2 (equivalent to 1 / sqrt(2) )
constexpr f4 ROOT2_2 = _ROOT2;

//! Approximation the square root of 3
constexpr f4 ROOT3 = 1.732050807568877f;

//! Approximation 1 / sqrt(3) ( reciprocal of square root of 3 )
constexpr f4 _ROOT3 = 0.5773502691896258f;

//! Approximation half of the square root of 3
constexpr f4 ROOT3_2 = 0.866025403784439f;

//! Approximation the square root of 5
constexpr f4 ROOT5 = 2.2360679774997896964f;

//! Approximation 1 / sqrt(5) ( reciprocal of square root of 5 )
constexpr f4 _ROOT5 = 0.44721359549995793928f;

//! Approximation the square root of 6
constexpr f4 ROOT6 = 2.4494897427831780982f;

//! Approximation 1 / sqrt(6) ( reciprocal of square root of 6 )
constexpr f4 _ROOT6 = 0.40824829046386301637f;

//! Approximation the square root of 7
constexpr f4 ROOT7 = 2.6457513110645905905f;

//! Approximation 1 / sqrt(7) ( reciprocal of square root of 7 )
constexpr f4 _ROOT7 = 0.37796447300922722721f;

//! Approximation the square root of 8
constexpr f4 ROOT8 = 2.8284271247461900976f;

//! Approximation 1 / sqrt(8) ( reciprocal of square root of 8 )
constexpr f4 _ROOT8 = 0.3535533905932737622f;

//! Approximation the square root of 10
constexpr f4 ROOT10 = 3.162277660168379332f;

//! Approximation 1 / sqrt(10) ( reciprocal of square root of 10 )
constexpr f4 _ROOT10 = 0.3162277660168379332f;

//! Approximation the square root of 11
constexpr f4 ROOT11 = 3.3166247903553998491f;

//! Approximation 1 / sqrt(11) ( reciprocal of square root of 11 )
constexpr f4 _ROOT11 = 0.30151134457776362265f;

//! Approximation the square root of 12
constexpr f4 ROOT12 = 3.4641016151377545871f;

//! Approximation 1 / sqrt(12) ( reciprocal of square root of 12 )
constexpr f4 _ROOT12 = 0.28867513459481288225f;

//! Approximation of twice the value of &pi;
constexpr f4  TAU = 6.283185307179586f;

//! Approximation of 1 / 2&pi;
constexpr f4  _TAU = 0.15915494309189533576888376337251f;

//! Note: Rotation is uncoupled from &pi; in units, so RAD_PER_REV is 2*&pi;
constexpr f4 RAD_PER_REV = TAU;

//! Premultiplied ratio of 9.0/10.0: Gradians->Degrees
constexpr f4 DEG_PER_GRAD = 0.9f;

//! Premultiplied ratio of 180.0/&pi;: Radians->Degrees
constexpr f4 DEG_PER_RAD = 57.295779513082320876798154814105f;

//! Premultiplied ratio of 360.0/1.0: Revolutions->Degrees
constexpr f4 DEG_PER_REV = 360.0f;

//! Premultiplied ratio of &pi;/180.0: Degrees->Radians
constexpr f4 RAD_PER_DEG = 0.01745329251994329576923690768489f;

//! Premultiplied ratio of &pi;/200.0: Gradians->Radians
constexpr f4 RAD_PER_GRAD = 0.0157079632679489661923132169164f;

//! Premultiplied ratio of 1.0/400.0: Gradians->Revolutions
constexpr f4 REV_PER_GRAD = 0.0025f;

//! Premultiplied ratio of 1.0/360.0: Degrees->Revolutions
constexpr f4 REV_PER_DEG = 0.00277777777777777777777777777778f;

//! Premultiplied ratio of 1.0/2&pi;: Radians->Revolutions
constexpr f4 REV_PER_RAD = _TAU;

//! Premultiplied ratio of 10.0/9.0: Degrees->Gradians
constexpr f4 GRAD_PER_DEG = 1.1111111111111111111111111111111f;

//! Premultiplied ratio of 200.0/&pi;: Radians->Gradians
constexpr f4 GRAD_PER_RAD = 63.661977236758134307553505349006f;

//! Premultiplied ratio of 200.0/&pi;: Radians->Gradians
constexpr f4 GRAD_PER_REV = 400.0f;

//! Label for 1/2, to avoid division by predictable integer constants
constexpr f4 AHALF = 0.5f;

//! Approximation of 1/3, to avoid predictable integer constant division
constexpr f4 ATHIRD = 0.333333333333333333333333333333333f;

//! Label for 1/4, to avoid division by predictable integer constants
constexpr f4 AQUARTER = 0.25f;

//! Label for 1/5, to avoid division by predictable integer constants
constexpr f4 AFIFTH = 0.2f;

//! Approximation of 1/6, to avoid predictable integer constant division
constexpr f4 ASIXTH = 0.166666666666666666666666666666667f;

//! Approximation of 1/7, to avoid predictable integer constant division
constexpr f4 ASEVENTH = 0.142857142857142857142857142857142857f;

//! Label for 1/8, to avoid division by predictable integer constants
constexpr f4 ANEIGHTH = 0.125f;

//! Approximation of 1/9, to avoid predictable integer constant division
constexpr f4 ANINTH = 0.111111111111111111111111111111111111f;

//! Label for 1/10, to avoid division by predictable integer constants
constexpr f4 ATENTH = 0.1f;

//! Approximation of 1/11, to avoid predictable integer constant division
constexpr f4 ANELEVENTH = 0.090909090909090909090909090909090909f;

//! Premultiplied ratio of 1/12
constexpr f4 PER_DOZEN = 0.08333333333333333333333333333333f;

//! Premultiplied ratio of 1/144
constexpr f4 PER_GROSS = 0.00694444444444444444444444444444f;

//! Designated sentenel value within the framework for f4 data as a bad result
constexpr f4 f4_INVALID = std::numeric_limits<f4>::signaling_NaN();

//! Largest 32-bit float finite value: ~340.282346640 duodecillions
constexpr f4 f4_MAX = std::numeric_limits<f4>::max();

//! Smallest 32-bit float value which can move maxima: ~10.1412048 nonillions
constexpr f4 f4_STEP_MAX = 2.0f * (1ull << 63) * (1ull << 63);

//! Absolute 32-bit float infinitesimal: smallest possible value: 1 / 2^{149}
constexpr f4 f4_STEP_MIN = std::numeric_limits<f4>::min() / (1 << 23);

//! General 32-bit float infinitesimal: smallest possible value: 1 / 2^{127}
constexpr f4 f4_MIN = std::numeric_limits<f4>::min();

//! Designated sentenel value within the framework for f8 data as a bad result
constexpr f8 f8_INVALID = std::numeric_limits<f8>::signaling_NaN();

//! Endianness of the system
enum class Endian
{
  Little,               //! (min,max); min < x < max; min / max disallowed
  Small = Little,      //! (min,max); min < x < max; min / max disallowed
  Low = Little,             //! (min,max); min < x < max; min / max disallowed
  // Screw middle endianness: no.  nobody likes you.
  Big,                //! [min,max]; min <= x <= max; min / max allowed
  High = Big        //! [min,max]; min <= x <= max; min / max allowed

}; // end Endian enum                                                         */

//! Interval endpoint notations: bidirectional comparison operator modes / types
enum class Cmp
{
  Disclusive,               //! (min,max); min < x < max; min / max disallowed
  Strict = Disclusive,      //! (min,max); min < x < max; min / max disallowed
  Open = Disclusive,        //! (min,max); min < x < max; min / max disallowed
  Out = Strict,             //! (min,max); min < x < max; min / max disallowed
  Inclusive,                //! [min,max]; min <= x <= max; min / max allowed
  Loose = Inclusive,        //! [min,max]; min <= x <= max; min / max allowed
  In = Loose,               //! [min,max]; min <= x <= max; min / max allowed
  Closed = Loose,           //! [min,max]; min <= x <= max; min / max allowed
  StrictAbove,              //! [min,max); min <= x < max; min allowed, not max
  LooseBelow = StrictAbove, //! [min,max); min <= x < max; min allowed, not max
  OpenAbove = LooseBelow,   //! [min,max); min <= x < max; min allowed, not max
  ClosedBelow = OpenAbove,  //! [min,max); min <= x < max; min allowed, not max
  StrictBelow,              //! (min,max]; min < x <= max; max allowed, not min
  LooseAbove = StrictBelow, //! (min,max]; min < x <= max; max allowed, not min
  ClosedAbove = LooseAbove, //! (min,max]; min < x <= max; max allowed, not min
  OpenBelow = ClosedAbove   //! (min,max]; min < x <= max; max allowed, not min

}; // end Cmp enum                                                            */


//! Overflow value setting policy possible mode typifications
enum class OFlow
{
  AskUpSetDn=-11,//! Stop for user input on overflow, set underflow to sentinel
  SetDnAskUp=-11,//! Stop for user input on overflow, set underflow to sentinel
  AskDnSetUp=-10,//! Stop for user input on underflow, set overflow to sentinel
  SetUpAskDn=-10,//! Stop for user input on underflow, set overflow to sentinel
  AskUpNullDn=-9,//! Stop for user input on overflow, reset underflow to null
  NullDnAskUp=-9,//! Stop for user input on overflow, reset underflow to null
  NullUpAskDn=-8,//! Stop for user input on underflow, reset overflow to null
  AskDnNullUp=-8,//! Stop for user input on underflow, reset overflow to null
  AskUpClampDn=-7,//! Stop for user input on overflow, clamp underflow to min
  ClampDnAskUp=-7,//! Stop for user input on overflow, clamp underflow to min
  AskDnClampUp=-6,//! Stop for user input on underflow, clamp overflow to max
  ClampUpAskDn=-6,//! Stop for user input on underflow, clamp overflow to max
  AskUpKeepDn=-5,//! Stop for user redirection on overflow, revert on underflow
  KeepDnAskUp=-5,//! Stop for user redirection on overflow, revert on underflow
  AskDnKeepUp=-4,//! Stop for user redirection on underflow, revert on overflow
  KeepUpAskDn=-4,//! Stop for user redirection on underflow, revert on overflow
  PromptUp=-3,//! Stop for user redirection on overflow, allow underflow
  PromptDn=-2,//! Stop for user redirection on underflow, allow overflow
  Prompt = -1,//! No assumptions made: stop for user redirection per overflow
  AskUp=PromptUp,//! Stop for user redirection on overflow, allow underflow
  AskDn=PromptDn,//! Stop for user redirection on underflow, allow overflow
  Ask = Prompt, //! No assumptions made: stop for user redirection per overflow
  Allow,      //! Default system behavior: wraparound any overflow / underflow
  Wrap=Allow, //! Default system behavior: wraparound any overflow / underflow
  Revert,     //! Negate value change attempts: keep pre-overflow value intact
  Keep=Revert,//! Negate value change attempts: keep pre-overflow value intact
  NoOp = Keep,//! Negate value change attempts: keep pre-overflow value intact
  RevertDn,   //! Revert underflow only: revert underflow, allow overflow
  KeepDn=RevertDn,//! Revert underflow only: revert underflow, allow overflow
  NoOpDn=KeepDn,//! Revert underflow only: revert underflow, allow overflow
  RevertUp,   //! Revert overflow only: revert overflow, allow underflow
  KeepUp=RevertUp,//! Revert overflow only: revert overflow, allow underflow
  NoOpUp=KeepUp,//! Revert overflow only: revert overflow, allow underflow
  Clamp,      //! Set to primitive specific extrema values on overflow
  ClampDn,    //! Clamp to a type minima value on underflow, but allow overflow
  ClampUp,    //! Clamp to a type maxima value on overflow, but allow underflow
  ClampDnKeepUp,//! Clamp to a type minima on underflow, revert on overflow
  KeepUpClampDn=ClampDnKeepUp,//! Clamp on underflow, revert on overflow
  ClampUpKeepDn,//! Clamp to a type maxima on overflow, revert on underflow
  KeepDnClampUp=ClampUpKeepDn,//! Clamp on overflow, revert on underflow
  Reset,      //! Reset to null value in case of any value overflow / underflow
  Null=Reset, //! Reset to null value in case of any value overflow / underflow
  ResetDn,    //! Reset to null value on underflow, but allow overflow values
  NullDn=ResetDn,//! Reset to null value on underflow, but allow overflow values
  ResetUp,    //! Reset to null value on overflow, but allow underflow values
  NullUp=ResetUp,//! Reset to null value on overflow, but allow underflow values
  NullUpKeepDn,//! Reset to null value on overflow, revert value on underflow
  KeepDnNullUp=NullUpKeepDn,//! Set null on overflow, revert value on underflow
  NullDnKeepUp,//! Reset to null value on underflow, revert value on overflow
  KeepUpNullDn=NullDnKeepUp,//! Set null on underflow, revert value on overflow
  NullUpClampDn,//! Reset to null value on overflow, clamp underflow to minima
  ClampDnNullUp=NullUpClampDn,//! Set null on overflow, clamp underflow to minima
  NullDnClampUp,//! Reset to null value on underflow, clamp overflow to maxima
  ClampUpNullDn=NullDnClampUp,//! Set null on underflow, clamp overflow to maxima
  Sentinel,   //! Set to an externally specified sentinel value on exceptions
  Set=Sentinel,//! Set to an externally specified sentinel value on exceptions
  SentinelDn, //! Set to a sentinel value on underflow, but allow overflow
  SetDn=SentinelDn,//! Set to a sentinel value on underflow, but allow overflow
  SentinelUp, //! Set to a sentinel value on overflow, but allow underflow
  SetUp=SentinelUp,//! Set to a sentinel value on overflow, but allow underflow
  SetDnKeepUp,//! Set to a sentinel value on underflow, but revert on overflow
  KeepUpSetDn=SetDnKeepUp,//! Set underflow to sentinel, revert overflow values
  SetUpKeepDn,//! Set to a sentinel value on overflow, but revert on underflow
  KeepDnSetUp=SetUpKeepDn,//! Set overflow to sentinel, revert underflow values
  SetDnClampUp,//! Set underflow to sentinel value, clamp overflow to maxima
  ClampUpSetDn=SetDnClampUp,//! Set underflow to sentinel, clamp high to maxima
  SetUpClampDn,//! Set overflow to sentinel value, clamp underflow to minima
  ClampDnSetUp=SetUpClampDn,//! Set overflow to sentinel, clamp low to minima
  SetDnNullUp,//! Set underflow to sentinel value, overflow to null value
  NullUpSetDn=SetDnNullUp,//! Set underflow to sentinel, overflow to null value
  SetUpNullDn,//! Set overflow to sentinel value, underflow to null value
  NullDnSetUp=SetUpNullDn,//! Set overflow to sentinel, underflow to null value

}; // end OFlow enum                                                          */


//! Serial operation arithmetic mode
enum class Oper
{
  Sum,          //! Iterative parts of the operation are added together
  Sigma = Sum,  //! Summation: parts of the operation are added together
  Product,      //! Iterative parts of the operation are multiplied together
  Pi = Product  //! Multiply: parts of the operation are multiplied together

}; // end Oper enum                                                           */


//! Standard integer positional number grouping terms (decimal biased latin)
enum class Radix
{
  Infinite,                                 //! Undefined: No number base
  Innumerable = Infinite,                   //! Undefined: No number base
  Base0 = Infinite,                         //! Undefined: No number base
  Unary,                                    //! Unit: n^0
  Base1 = Unary,                            //! Unit: n^0
  Tally = Unary,                            //! Unit: n^0
  Count = Tally,                            //! Unit: n^0
  Binary,                                   //! Prime: 1 * 2
  Base2 = Binary,                           //! Prime: 1 * 2
  Ternary,                                  //! Prime: 1 * 3
  Base3 = Ternary,                          //! Prime: 1 * 3
  Quaternary,                               //! Square: 2^2
  Base4 = Quaternary,                       //! Square: 2^2
  Quinary,                                  //! Prime: 1 * 5
  Base5 = Quinary,                          //! Prime: 1 * 5
  Senary,                                   //! Complex: 2 * 3
  Base6 = Senary,                           //! Complex: 2 * 3
  Septenary,                                //! Prime: 1 * 7
  Base7 = Septenary,                        //! Prime: 1 * 7
  Octal,                                    //! Cube: 2^3
  Base8 = Octal,                            //! Cube: 2^3
  Nonary,                                   //! Square: 3^2
  Base9 = Nonary,                           //! Square: 3^2
  Decimal,                                  //! Complex: 2 * 5
  Base10 = Decimal,                         //! Complex: 2 * 5
  Default = Decimal,                        //! Complex: 2 * 5
  Undecimal,                                //! Prime: 1 * 11
  Base11 = Undecimal,                       //! Prime: 1 * 11
  Duodecimal,                               //! Complex: 2^2 * 3
  Base12 = Duodecimal,                      //! Complex: 2^2 * 3
  Dozenal = Duodecimal,                     //! Complex: 2^2 * 3
  Tridecimal,                               //! Prime: 1 * 13
  Base13 = Tridecimal,                      //! Prime: 1 * 13
  Tetradecimal,                             //! Complex: 2 * 7
  Base14 = Tetradecimal,                    //! Complex: 2 * 7
  Pentadecimal,                             //! Complex: 3 * 5
  Base15 = Pentadecimal,                    //! Complex: 3 * 5
  Hexadecimal,                              //! Quartic: 2^4
  Base16 = Hexadecimal,                     //! Quartic: 2^4
  Heptadecimal,                             //! Prime: 1 * 17
  Base17 = Heptadecimal,                    //! Prime: 1 * 17
  Septadecimal = Heptadecimal,              //! Prime: 1 * 17
  Octodecimal,                              //! Complex: 2 * 3^2
  Base18 = Octodecimal,                     //! Complex: 2 * 3^2
  Enneadecimal,                             //! Prime: 1 * 19
  Base19 = Enneadecimal,                    //! Prime: 1 * 19
  Vigesimal,                                //! Complex: 2^2 * 5
  Base20 = Vigesimal,                       //! Complex: 2^2 * 5
  Unvigesimal,                              //! Complex: 3 * 7
  Base21 = Unvigesimal,                     //! Complex: 3 * 7
  Duovigesimal,                             //! Complex: 2 * 11
  Base22 = Duovigesimal,                    //! Complex: 2 * 11
  Trivigesimal,                             //! Prime: 1 * 23
  Base23 = Trivigesimal,                    //! Prime: 1 * 23
  Tetravigesimal,                           //! Complex: 2^3 * 3
  Base24 = Tetravigesimal,                  //! Complex: 2^3 * 3
  Pentavigesimal,                           //! Square: 5^2
  Base25 = Pentavigesimal,                  //! Square: 5^2
  Hexavigesimal,                            //! Complex: 2 * 13
  Base26 = Hexavigesimal,                   //! Complex: 2 * 13
  Heptavigesimal,                           //! Cube: 3^3
  Base27 = Heptavigesimal,                  //! Cube: 3^3
  Septemvigesimal = Heptavigesimal,         //! Cube: 3^3
  Octovigesimal,                            //! Complex: 2^2 * 7
  Base28 = Octovigesimal,                   //! Complex: 2^2 * 7
  Enneavigesimal,                           //! Prime: 1 * 29
  Base29 = Enneavigesimal,                  //! Prime: 1 * 29
  Trigesimal,                               //! Complex: 2 * 3 * 5
  Base30 = Trigesimal,                      //! Complex: 2 * 3 * 5
  Untrigesimal,                             //! Prime: 1 * 31
  Base31 = Untrigesimal,                    //! Prime: 1 * 31
  Duotrigesimal,                            //! Quintic: 2^5
  Base32 = Duotrigesimal,                   //! Quintic: 2^5
  Tritrigesimal,                            //! Complex: 3 * 11
  Base33 = Tritrigesimal,                   //! Complex: 3 * 11
  Tetratrigesimal,                          //! Complex: 2 * 17
  Base34 = Tetratrigesimal,                 //! Complex: 2 * 17
  Pentatrigesimal,                          //! Complex: 5 * 7
  Base35 = Pentatrigesimal,                 //! Complex: 5 * 7
  Hexatrigesimal,                           //! Complex: 2^2 * 3^2
  Base36 = Hexatrigesimal,                  //! Complex: 2^2 * 3^2
  Heptatrigesimal,                          //! Prime: 1 * 37
  Base37 = Heptatrigesimal,                 //! Prime: 1 * 37
  Septemtrigesimal = Heptatrigesimal,       //! Prime: 1 * 37
  Octotrigesimal,                           //! Complex: 2 * 19
  Base38 = Octotrigesimal,                  //! Complex: 2 * 19
  Enneatrigesimal,                          //! Complex: 3 * 13
  Base39 = Enneatrigesimal,                 //! Complex: 3 * 13
  Quatragesimal,                            //! Complex: 2^3 * 5
  Base40 = Quatragesimal,                   //! Complex: 2^3 * 5
  Unquadragesimal,                          //! Prime: 1 * 41
  Base41 = Unquadragesimal,                 //! Prime: 1 * 41
  Duoquadragesimal,                         //! Complex: 2 * 3 * 7
  Base42 = Duoquadragesimal,                //! Complex: 2 * 3 * 7
  Triquadragesimal,                         //! Prime: 1 * 43
  Base43 = Triquadragesimal,                //! Prime: 1 * 43
  Tetraquadragesimal,                       //! Complex: 2^2 * 11
  Base44 = Tetraquadragesimal,              //! Complex: 2^2 * 11
  Pentaquadragesimal,                       //! Complex: 3^2 * 5
  Base45 = Pentaquadragesimal,              //! Complex: 3^2 * 5
  Hexaquadragesimal,                        //! Complex: 2 * 23
  Base46 = Hexaquadragesimal,               //! Complex: 2 * 23
  Septaquadragesimal,                       //! Prime: 1 * 47
  Base47 = Septaquadragesimal,              //! Prime: 1 * 47
  Heptaquadragesimal = Septaquadragesimal,  //! Prime: 1 * 47
  Octoquadragesimal,                        //! Complex: 2^4 * 3
  Base48 = Octoquadragesimal,               //! Complex: 2^4 * 3
  Enneaquadragesimal,                       //! Square: 7^2
  Base49 = Enneaquadragesimal,              //! Square: 7^2
  Quinquagesimal,                           //! Complex: 2 * 5^2
  Base50 = Quinquagesimal,                  //! Complex: 2 * 5^2
  Unquiquagesimal,                          //! Complex: 3 * 17
  Base51 = Unquiquagesimal,                 //! Complex: 3 * 17
  Duoquiquagesimal,                         //! Complex: 2^2 * 13
  Base52 = Duoquiquagesimal,                //! Complex: 2^2 * 13
  Triquiquagesimal,                         //! Prime: 1 * 53
  Base53 = Triquiquagesimal,                //! Prime: 1 * 53
  Tetraquiquagesimal,                       //! Complex: 2 * 3^3
  Base54 = Tetraquiquagesimal,              //! Complex: 2 * 3^3
  Pentaquiquagesimal,                       //! Complex: 5 * 11
  Base55 = Pentaquiquagesimal,              //! Complex: 5 * 11
  Hexaquiquagesimal,                        //! Complex: 2^3 * 7
  Base56 = Hexaquiquagesimal,               //! Complex: 2^3 * 7
  Heptaquiquagesimal,                       //! Complex: 3 * 19
  Base57 = Heptaquiquagesimal,              //! Complex: 3 * 19
  Septaquiquagesimal = Heptaquiquagesimal,  //! Complex: 3 * 19
  Octoquinquagesimal,                       //! Complex: 2 * 29
  Base58 = Octoquinquagesimal,              //! Complex: 2 * 29
  Enneaquinquagesimal,                      //! Prime: 1 * 59
  Base59 = Enneaquinquagesimal,             //! Prime: 1 * 59
  Sexagesimal,                              //! Complex: 2^2 * 3 * 5
  Base60 = Sexagesimal,                     //! Complex: 2^2 * 3 * 5
  Unsexagesimal,                            //! Prime: 1 * 61
  Base61 = Unsexagesimal,                   //! Prime: 1 * 61
  Duosexagesimal,                           //! Complex: 2 * 31
  Base62 = Duosexagesimal,                  //! Complex: 2 * 31
  Trisexagesimal,                           //! Complex: 3^2 * 7
  Base63 = Trisexagesimal,                  //! Complex: 3^2 * 7
  Tetrasexagesimal,                         //! Zenzicube: 2^6
  Base64 = Tetrasexagesimal,                //! Zenzicube: 2^6
  Centesimal = 100,                         //! Square: 2^2 * 5^2
  Base100 = Centesimal,                     //! Square: 2^2 * 5^2
  Trecentosexagesimal = 360,                //! Complex: 2^3 * 3^2 * 5
  Base360 = Trecentosexagesimal,            //! Complex: 2^3 * 3^2 * 5
}; // end Radix enum                                                          */


//! Arc unit measure categories
enum class uArc
{
  Spins,
  Revolutions = Spins,
  Rev = Spins,
  S = Spins,
  Radians,
  Rad = Radians,
  R = Radians,
  Degrees,
  Deg = Degrees,
  D = Degrees,
  Gradians,
  Grad = Gradians,
  G = Gradians
}; // end uArc enum                                                           */


//! Arc unit measure conversion categories
enum class uArcConv
{
  SpinsToRadians,
  RevolutionsToRadians = SpinsToRadians,
  RevToRad = SpinsToRadians,
  SToR = SpinsToRadians,
  SpinsToDegrees,
  RevolutionsToDegrees = SpinsToDegrees,
  RevToDeg = SpinsToDegrees,
  SToD = SpinsToDegrees,
  SpinsToGradians,
  RevolutionsToGradians = SpinsToGradians,
  RevToGrad = SpinsToGradians,
  SToG = SpinsToGradians,
  RadiansToSpins,
  RadiansToRevolutions = RadiansToSpins,
  RadToRev = RadiansToSpins,
  RToS = RadiansToSpins,
  RadiansToDegrees,
  RadToDeg = RadiansToDegrees,
  RToD = RadiansToDegrees,
  RadiansToGradians,
  RadToGrad = RadiansToGradians,
  RToG = RadiansToGradians,
  DegreesToSpins,
  DegreesToRevolutions = DegreesToSpins,
  DegToRev = DegreesToSpins,
  DToS = DegreesToSpins,
  DegreesToRadians,
  DegToRad = DegreesToRadians,
  DToR = DegreesToRadians,
  DegreesToGradians,
  DegToGrad = DegreesToGradians,
  DToG = DegreesToGradians,
  GradiansToSpins,
  GradiansToRevolutions = GradiansToSpins,
  GradToRev = GradiansToSpins,
  GToS = GradiansToSpins,
  GradiansToRadians,
  GradToRad = GradiansToRadians,
  GToR = GradiansToRadians,
  GradiansToDegrees,
  GradToDeg = GradiansToDegrees,
  GToD = GradiansToDegrees
}; // end uArcConv enum                                                       */


//! Distance unit measure categories
enum class uDist
{
  //! Historic: sqrt((G*k_e*e^})/(c^4)): 1.38068*10^-36 m
  Stoney = -36,
  //! Physics: Smallest known measurable length: 1.61605*10^-35 m
  Planck = -35,
  //! Metric: smallest named metric length unit; 1/1000 zm; 1.0*10^-24 meters
  yoctometer = -24,
  //! Metric: smallest named metric length unit; 1/1000 zm; 1.0*10^-24 meters
  ym = yoctometer,
  //! Metric: 1000 ym; 1/1000 am; 1.0*10^-21 meters
  zeptometer = -21,
  //! Metric: 1000 ym; 1/1000 am; 1.0*10^-21 meters
  zm = zeptometer,
  //! Metric: 1000 zm; 1/1000 fm; 1.0*10^-18 meters
  attometer = -18,
  //! Metric: 1000 zm; 1/1000 fm; 1.0*10^-18 meters
  am = attometer,
  //! Physics: electron radius; 2.8179403227*10^-15 meters
  elecRadius,
  //! Physics: electron radius; 2.8179403227*10^-15 meters
  eRad = elecRadius,
  //! Physics: Quantum ChronoDynamics; 2.103*10^-16 meters
  quantum,
  //! Physics: Quantum ChronoDynamics; 2.103*10^-16 meters
  QCD = quantum,
  //! Metric: 1000 am; 1/1000 pm; 1.0*10^-15 meters
  femotometer = -15,
  //! Metric: 1000 am; 1/1000 pm; 1.0*10^-15 meters
  fm = femotometer,
  //! Metric: 1000 am; 1/1000 pm; 1.0*10^-15 meters
  fermi = femotometer,
  //! Physics: Reduced electron wave; 3.861592*10^-13 m
  redCompton,
  //! Physics: Reduced electron wave; 3.861592*10^-13 m
  _L_C = redCompton,
  //! Physics: Electron wavelength; 2.426310*10^-12 meters
  Compton,
  //! Physics: Electron wavelength; 2.426310*10^-12 meters
  L_C = Compton,
  //! Metric: 1000 fm; 1/1000 nm; 1.0*10^-12 meters
  picometer = -12,
  //! Metric: 1000 fm; 1/1000 nm; 1.0*10^-12 meters
  pm = picometer,
  //! Physics: hydrogen radius; 5.291772*10^-11 meters
  Bohr,
  //! Physics: hydrogen radius; 5.291772*10^-11 meters
  a_0 = Bohr,
  //! Metric: 100 pm; 1/10 nm; 1.0*10^-10 meters
  angstrom,
  //! Metric: 1000 pm; 1/1000 um; 1.0*10^-9 meters
  nanometer = -9,
  //! Metric: 1000 pm; 1/1000 um; 1.0*10^-9 meters
  nm = nanometer,
  //! Imperial: 1/1000 inch
  thou,
  //! Imperial: 1/1000 inch
  mil = thou,
  //! Imperial: 1000 mil; 1/12 foot
  inch,
  //! Imperial: 1000 mil; 1/12 foot
  in = inch,
  //! Metric: 1000 nm; 1/1000 mm; 1.0*10^-6 meters
  micrometer = -6,
  //! Metric: 1000 nm; 1/1000 mm; 1.0*10^-6 meters
  um = micrometer,
  //! Metric: 1000 nm; 1/1000 mm; 1.0*10^-6 meters
  micron = micrometer,
  //! Imperial: 12 inches; 1/3 yard
  foot,
  //! Imperial: 12 inches; 1/3 yard
  ft = foot,
  //! Metric: 1000 um; 1/10 cm; 1.0*10^-3 meters
  millimeter = -3,
  //! Metric: 1000 um; 1/10 cm; 1.0*10^-3 meters
  mm = millimeter,
  //! Metric: 10 mm; 1/10 dm; 1.0*10^-2 meters
  centimeter = -2,
  //! Metric: 10 mm; 1/10 dm; 1.0*10^-2 meters
  cm = centimeter,
  //! Metric: 10 cm; 1/10 meter; 1.0*10^-1 meters
  decimeter = -1,
  //! Metric: 10 cm; 1/10 meter; 1.0*10^-1 meters
  dm = decimeter,
  //! Metric: archetypical metric unit of length
  meter = 0,
  //! Metric: archetypical metric unit of length
  m = meter,
  //! Metric: 10 meters; 1/10 hm; 1.0*10^1 meters
  decameter = 1,
  //! Metric: 10 meters; 1/10 hm; 1.0*10^1 meters
  dam = decameter,
  //! Metric: 10 dam; 1/10 km; 1.0*10^2 meters
  hectometer = 2,
  //! Metric: 10 dam; 1/10 km; 1.0*10^2 meters
  hm = hectometer,
  //! Metric: 10 hm; 1/1000 Mm; 1.0*10^3 meters
  kilometer = 3,
  //! Metric: 10 hm; 1/1000 Mm; 1.0*10^3 meters
  Km = kilometer,
  //! Imperial: 3 feet
  yard,
  //! Imperial: 3 feet
  yd = yard,
  //! Metric: 1000 Km; 1/1000 Gm; 1.0*10^6 meters
  megameter = 6,
  //! Metric: 1000 Km; 1/1000 Gm; 1.0*10^6 meters
  Mm = megameter,
  //! Imperial: 5280 ft; 1760 yards; 1.60934 Km
  mile,
  //! Imperial: 5280 ft; 1760 yards; 1.60934 Km
  mi = mile,
  //! Imperial: distances over water; 1.15078 (statute) miles; 1.852 Km
  nauticalMile,
  //! Imperial: distances over water; 1.15078 (statute) miles; 1.852 Km
  nmi = nauticalMile,
  //! Metric: 1000 Mm; 1/1000 Tm; 1.0*10^9 meters
  gigameter = 9,
  //! Metric: 1000 Mm; 1/1000 Tm; 1.0*10^9 meters
  Gm = gigameter,
  //! Surveying: 1/100 chain; 7.92 inches; 20.1168 centimeters
  link,
  //! Imperial: water depth; 2 yd; 6 ft; 1.8288 meters
  fathom,
  //! Metric: 1000 Gm; 1/1000 Pm; 1.0*10^12 meters
  terameter = 12,
  //! Metric: 1000 Gm; 1/1000 Pm; 1.0*10^12 meters
  Tm = terameter,
  //! Surveying: 1/4 chain; sqrt(acre/160); 5.5 yards; 5.0292 meters
  rod,
  //! Surveying: 1/4 chain; sqrt(acre/160); 5.5 yards; 5.0292 meters
  pole = rod,
  //! Surveying: 1/4 chain; sqrt(acre/160); 5.5 yards; 5.0292 meters
  perch = rod,
  //! Surveying: sqrt(acre/10); 22 yards; 20.1168 meters
  chain,
  //! Metric: 1000 Tm; 1/1000 Em; 1.0*10^15 meters
  petameter = 15,
  //! Metric: 1000 Tm; 1/1000 Em; 1.0*10^15 meters
  Pm = petameter,
  //! Astronomy: Earth Radius; 6.3781370 Mm; 3,963.1906 mi
  earthRadius,
  //! Astronomy: Earth Radius; 6.3781370 Mm; 3,963.1906 mi
  R_E = earthRadius,
  //! Astronomy: Time avg dist to moon: 384.398,251,776 Mm
  lunarDist,
  //! Astronomy: Time avg dist to moon: 384.398,251,776 Mm
  R_LO = lunarDist,
  //! Metric: 1000 Pm; 1/1000 Zm; 1.0*10^18 meters
  exameter = 18,
  //! Metric: 1000 Pm; 1/1000 Zm; 1.0*10^18 meters
  Em = exameter,
  //! Astronomy: light speed per year in a vaccuum; 9.4607304725808 Pm
  lightYear,
  //! Astronomy: light speed per year in a vaccuum; 9.4607304725808 Pm
  ly = lightYear,
  //! Astronomy: distances to stars; 206,264.806247096 au; 30.856775815 Pm
  parsec,
  //! Astronomy: distances to stars; 206,264.806247096 au; 30.856775815 Pm
  pc = parsec, 
  //! Metric: 1000 Em; 1/1000 Ym; 1.0*10^21 meters
  zettameter = 21,
  //! Metric: 1000 Em; 1/1000 Ym; 1.0*10^21 meters
  Zm = zettameter,
  //! Astronomy: unit; Sun dist to Earth; 92,955,807.273026 mi; 149.5978707 Gm
  astronomic,
  //! Astronomy: unit; Sun dist to Earth; 92,955,807.273026 mi; 149.5978707 Gm
  au = astronomic,
  //! Astronomy: nearest receding galaxies; 4.55 billion pc; 14.4 billion ly
  Hubble,
  //! Astronomy: nearest receding galaxies; 4.55 billion pc; 14.4 billion ly
  D_H = Hubble,
  //! Metric: largest named length unit; 1000 Zm; 1.0*10^24 meters
  yottameter = 24,
  //! Metric: largest named length unit; 1000 Zm; 1.0*10^24 meters
  Ym = yottameter
}; // end uDist enum                                                          */


//! Unit grouping / magnitude measurements
enum class uGrp
{
  yocto = -24,
  zepto = -21,
  atto = -18,
  femto = -15,
  picto = -12,
  nano = -9,
  micro = -6,
  milli = -3,
  centi = -2,
  deci = -1,
  unit = 0,
  deca = 1,
  hecto = 2,
  kilo = 3,
  mega = 6,
  giga = 9,
  tera = 12,
  peta = 15,
  exa  = 18,
  zetta = 21,
  yotta = 24,
}; // end uGrp enum                                                           */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                          Public Static Variables                           */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

static OFlow DEFAULT_OFLOW_POLICY = OFlow::Wrap;

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                        Public Functions Declarations                       */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/** Get (and/or set) the default policy setting for handling value overflow
\brief
  - get (and/or set) the default policy setting for handling value overflow
\details
  - inline will be ignored for a function with static values, just the pattern
\param policyPreference
  - pointer to an overflow behavior value, which, if provided, becomes default
\return
  - current (or newly set), default overflow policy value declared
*/
inline OFlow OFlowPolicy(OFlow* policyPreference = nullptr);


/** Get the magnitude / absolute value of a 1-byte integer value
\brief
  - get the magnitude / absolute value of a 1-byte integer value
\param value
  - input value to be compared for negativity
\return
  - if positive, get the original value; if negative, get the negated value
*/
inline s1 AbsC(s1 value);

/** Get the magnitude / absolute value of a float-point 8-byte value
\brief
  - get the magnitude / absolute value of a float-point 8-byte value
\param value
  - input value to be compared for negativity
\return
  - if positive, get the original value; if negative, get the negated value
*/
inline f8 AbsD(const f8& value);

/** Get the magnitude / absolute value of a float-point 4-byte value
\brief
  - get the magnitude / absolute value of a float-point 4-byte value
\param value
  - input value to be compared for negativity
\return
  - if positive, get the original value; if negative, get the negated value
*/
inline f4 AbsF(f4 value);

/** Get the magnitude / absolute value of a 4-byte integer value
\brief
  - get the magnitude / absolute value of a 4-byte integer value
\param value
  - input value to be compared for negativity
\return
  - if positive, get the original value; if negative, get the negated value
*/
inline s4 AbsI(s4 value);

/** Get the magnitude / absolute value of a 8-byte integer value
\brief
  - get the magnitude / absolute value of a 8-byte integer value
\param value
  - input value to be compared for negativity
\return
  - if positive, get the original value; if negative, get the negated value
*/
inline s8 AbsL(const s8& value);

/** Get the magnitude / absolute value of a 2-byte integer value
\brief
  - get the magnitude / absolute value of a 2-byte integer value
\param value
  - input value to be compared for negativity
\return
  - if positive, get the original value; if negative, get the negated value
*/
inline s2 AbsS(s2 value);


/** Calculate the arccosine angle of input float-value-pair of rise(y), run(x)
\brief
  - calculate the arccosine angle of input float-value-pair of rise(y), run(x)
\details
  - for platform compatibility assurances it's a wrapper around pro/demotion.
  also an errata into the proper return type with units used in the framework.
  could solve instead: newtonian approximation is the best I can guess to use.
\param cosine
  - x-scalar; ratio of adj./hyp. per right triangle sides about queried angle
\param invert
  - if true, ratio had -r value => y/r or -y/r was reductive => +&pi; ang shift
\return
  - angle arc expression equivalent to the input sine value (+&pi; if invert)
*/
dp::ang ACosF(f4 cosine, bool invert = false);

/** Calculate the arcsin angle of input float-value-pair of o(x), h(scale)
\brief
  - calculate the arcsin angle of input float-value-pair of o(x), h(scale)
\details
  - for platform compatibility assurances it's a wrapper around pro/demotion.
  also an errata into the proper return type with units used in the framework.
  could solve instead: newtonian approximation is the best I can guess to use.
\param x
  - displacement in x-axis, equivalent to cos(t) of unit circle definition
\param r
  - radius =:|hypoteneuse| ; x=: r*cos(t) => x/r==a/h; (-) => quad. 3|4 => +=pi
\return
  - angle arc expression of the input y/scale sine ratio (+&pi; if scale < 0)
*/
dp::ang ACos2F(f4 x, f4 r);


/** Get the result of u1 + u1 addition, with overflow exception considerations
\brief
  - get the result of u1 + u1 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of u1 + u1 addition under the given overflow / underflow policy
*/
inline u1 AddB(u1 current, u1 plus, OFlow policy = OFlowPolicy());

/** Get the result of s1 + s1 addition, with overflow exception considerations
\brief
  - get the result of s1 + s1 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of s1 + s1 addition under the given overflow / underflow policy
*/
inline s1 AddC(s1 current, s1 plus, OFlow policy = OFlowPolicy());

/** Get the result of f8 + f8 addition, with overflow exception considerations
\brief
  - get the result of f8 + f8 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of f8 + f8 addition under the given overflow / underflow policy
*/
inline f8 AddD(f8 current, const f8& plus, OFlow policy = OFlowPolicy());

/** Get the result of f4 + f4 addition, with overflow exception considerations
\brief
  - get the result of f4 + f4 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of f4 + f4 addition under the given overflow / underflow policy
*/
inline f4 AddF(f4 current, f4 plus, OFlow policy = OFlowPolicy());

/** Get the result of s4 + s4 addition, with overflow exception considerations
\brief
  - get the result of s4 + s4 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of s4 + s4 addition under the given overflow / underflow policy
*/
inline s4 AddI(s4 current, s4 plus, OFlow policy = OFlowPolicy());

/** Get the result of s8 + s8 addition, with overflow exception considerations
\brief
  - get the result of s8 + s8 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of s8 + s8 addition under the given overflow / underflow policy
*/
inline s8 AddL(s8 current, const s8& plus, OFlow policy = OFlowPolicy());

/** Get the result of s2 + s2 addition, with overflow exception considerations
\brief
  - get the result of s2 + s2 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of s2 + s2 addition under the given overflow / underflow policy
*/
inline s2 AddS(s2 current, s2 plus, OFlow policy = OFlowPolicy());

/** Get the result of u4 + u4 addition, with overflow exception considerations
\brief
  - get the result of u4 + u4 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of u4 + u4 addition under the given overflow / underflow policy
*/
inline u4 AddU(u4 current, u4 plus, OFlow policy = OFlowPolicy());

/** Get the result of u8 + u8 addition, with overflow exception considerations
\brief
  - get the result of u8 + u8 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of u8 + u8 addition under the given overflow / underflow policy
*/
inline u8 AddUL(u8 current, const u8& plus, OFlow policy = OFlowPolicy());

/** Get the result of um + um addition, with overflow exception considerations
\brief
  - get the result of um + um addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of um + um addition under the given overflow / underflow policy
*/
inline um AddUM(um current, um plus, OFlow policy = OFlowPolicy());

/** Get the result of u2 + u2 addition, with overflow exception considerations
\brief
  - get the result of u2 + u2 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix +)
\param plus
  - value given to add upon current input value (right hand side of infix +)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - outcome of u2 + u2 addition under the given overflow / underflow policy
*/
inline u2 AddUS(u2 current, u2 plus, OFlow policy = OFlowPolicy());


/** Convert 8-byte float-point input / output angle over a unit shift
\brief
  - convert 8-byte float-point input / output angle over a unit shift
\param angle
  - quantity of arc displacement (in units to be given)
\param shift
  - arc type pairing for shift from a input unit to a desired output unit
\return
  - angle quantity as given within the specified unit input should shift into
*/
inline f8 AngleD(const f8& ang, uArcConv shift);

/** Convert 4-byte float-point input / output angle over a unit shift
\brief
  - convert 4-byte float-point input / output angle over a unit
\param angle
  - quantity of arc displacement (in units to be given)
\param shift
  - arc type pairing for shift from a input unit to a desired output unit
\return
  - angle quantity as given within the specified unit input should shift into
*/
inline f4 AngleF(f4 ang, uArcConv shift);


/** Calculate the arctangent angle of input float-value-pair of rise(y), run(x)
\brief
  - calculate the arctangent angle of input float-value-pair of rise(y), run(x)
\details
  - for platform compatibility assurances it's a wrapper around pro/demotion.
  also an errata into the proper return type with units used in the framework.
  could solve instead: newtonian approximation is the best I can guess to use.
\param sine
  - y-scalar; ratio of opp./hyp. per right triangle sides about queried angle
\param invert
  - if true, ratio had a -r component => y/r or -y/r was reductive => +&pi; ang
\return
  - angle arc expression equivalent to the input sine value (+&pi; if invert)
*/
dp::ang ASinF(f4 sine, bool invert = false);

/** Calculate the arcsin angle of input float-value-pair of o(x), h(scale)
\brief
  - calculate the arcsin angle of input float-value-pair of o(x), h(scale)
\details
  - for platform compatibility assurances it's a wrapper around pro/demotion.
  also an errata into the proper return type with units used in the framework.
  could solve instead: newtonian approximation is the best I can guess to use.
\param y
  - displacement in y-axis, equivalent to sin(t) of unit circle definition
\param r
  - radius =:|hypoteneuse| ; y=: r*sin(t) => y/r==o/h; (-) => quad. 2|3 => +=pi
\return
  - angle arc expression of the input y/scale sine ratio (+&pi; if scale < 0)
*/
dp::ang ASin2F(f4 y, f4 r);

/** Calculate the arctangent angle of input float-value-pair of rise(y), run(x)
\brief
  - calculate the arctangent angle of input float-value-pair of rise(y), run(x)
\details
  - for platform compatibility assurances it's a wrapper around pro/demotion.
  also an errata into the proper return type with units used in the framework.
  could solve instead: newtonian approximation is the best I can guess to use.
\param tangent
  - tangent (slope) ratio of y:x spans in a region; opposite:adjacent tri edges
\param dNeg
  - if true, ratio had -x denom => y/x or -y/x was reductive => +&pi; ang shift
\return
  - angle arc expression equivalent to input tangent / slope (+&pi; if dNeg)
*/
dp::ang ATanF(f4 tangent, bool dNeg = false);

/** Calculate the arctangent angle of input float-value-pair of rise(y), run(x)
\brief
  - calculate the arctangent angle of input float-value-pair of rise(y), run(x)
\details
  - for platform compatibility assurances it's a wrapper around pro/demotion.
  also an errata into the proper return type with units used in the framework.
  could solve instead: newtonian approximation is the best I can guess to use.
\param y
  - rise of the vector: displacement in y; sin(t) if <x,y> are a unit vector
\param x
  - run of the vector: displacement in x; cos(t) if <x,y> are a unit vector
\return
  - angle arc expression equivalent to input y/x tangent ratio (+&pi; if x < 0)
*/
dp::ang ATan2F(f4 y, f4 x);

/** Get the binary string representation of an 8-bit number value
\brief
  - get the binary string representation of an 8-bit number value
\param val
  - value to be converted into binary representation
\return
  - MSB leading, LSB trailing, 8 digit string of '0' and '1' (null terminated)
*/
std::string BitsB(u1 val);

/** Get the binary string representation of a 4 byte float point number value
\brief
  - get the binary string representation of a 4 byte float point number value
\details
  - direct translation of IEEE-754 binary float point binary values, for which
  two's complement is not utilized, and the mantissa's leading bit is always 1,
  as such the truncated, constant, leading 1 from scientific notation in the
  mantissa is listed ahead of the dyadic (binary decimal), fractional point for
  completeness, but is not actually read, merely part of the field separations.
  the exponent's read from memory with a constant -1023 value offset, and so
  both storage & offset are displayed.  the mantissa not using any
  bitwise differentiation between positives and negatives (generally identical
  except for the leading MSB sign bit to the whole float field), has no such
  convenient heuristic for notation of change, except for the dyadic reminder.
  IEEE-754 not being designed here, these are simply inevitable limitations.
\param val
  - value to be converted into binary representation
\return
  - space delimited fields of sign, exponent and mantissa bits' binary values
*/
std::string BitsD(f8 val);

/** Get the binary string representation of a 4 byte float point number value
\brief
  - get the binary string representation of a 4 byte float point number value
\details
  - direct translation of IEEE-754 binary float point binary values, for which
  two's complement is not utilized, and the mantissa's leading bit is always 1,
  as such the truncated, constant, leading 1 from scientific notation in the
  mantissa is listed ahead of the dyadic (binary decimal), fractional point for
  completeness, but is not actually read, merely part of the field separations.
  the exponent's sign bit is listed as a separate field since the value doesn't
  use two's complement, which makes a visual reminder that those 1:7 bits are
  read from with a constant -127 value offset.  the mantissa not using any
  bitwise differentiation between positives and negatives (generally identical
  except for the leading MSB sign bit to the whole float field), has no such
  convenient heuristic for notation of change, except for the dyadic reminder.
  IEEE-754 not being designed here, these are simply inevitable limitations.
\param val
  - value to be converted into binary representation
\return
  - space delimited fields of sign, exponent and mantissa bits' binary values
*/
std::string BitsF(f4 val);

/** Get the binary string representation of a 32-bit number value
\brief
  - get the binary string representation of a 32-bit number value
\param val
  - value to be converted into binary representation
\return
  - MSB leading, LSB trailing, 32 digit string of '0' and '1' (null terminated)
*/
std::string BitsU(u4 val);

/** Get the binary string representation of a 64-bit number value
\brief
  - get the binary string representation of a 64-bit number value
\param val
  - value to be converted into binary representation
\return
  - MSB leading, LSB trailing, 64 digit string of '0' and '1' (null terminated)
*/
std::string BitsUL(u8 val);

/** Get the binary string representation of a 16-bit number value
\brief
  - get the binary string representation of a 16-bit number value
\param val
  - value to be converted into binary representation
\return
  - MSB leading, LSB trailing, 16 digit string of '0' and '1' (null terminated)
*/
std::string BitsUS(u2 val);


/** Get the integral ceiling of the given value without fractional component
\brief
  - get the integral ceiling of the given value without fractional component
\param value
  - float point 8-byte number to have its integer value truncated
\return
  - mathematical integer cieling of the input 8-byte float point value
*/
s8 CeilD(f8 value);

/** Get the integral ceiling of the given value without fractional component
\brief
  - get the integral ceiling of the given value without fractional component
\param value
  - float point 4-byte number to have its integer value truncated
\return
  - mathematical integer cieling of the input 4-byte float point value
*/
s4 CeilF(f4 value);


/** Get the cosine (x-axis measure of tri adj./hyp.) for input 2D angle of arc
\brief
  - get the cosine (x-axis measure of tri adj./hyp.) for input 2D angle of arc
\param angle
  - arc sweep from 0 point in the x-axis of the XY plane, counter-clockwise
\return
  - right triangle base [0,1] x-axis scale, with angle as hypoteneuse length 1
*/
f4 CosF(dp::ang angle);


/** Extract the exponentiation bitfield value from the current double value
\brief
  - extract the exponentiation bitfield value from the current double value
\param value
  - input 8-byte float-point value to have the exponential (2^n) value read
\return
  - IEEE-754 64-bit float standard's 11 bit wide [-2047, 2048] exponent value
*/
inline s2 ExpD(const f8& value);

/** Extract the exponentiation bitfield value from the current float value
\brief
  - extract the exponentiation bitfield value from the current float value
\param value
  - input 4-byte float-point value to have the exponential (2^n) value read
\return
  - IEEE-754 32-bit float standard's 8 bit wide [-127, 128] exponent value
*/
inline s2 ExpF(f4 value);


/** Get integral floor portion from given value without fractional component
\brief
  - get integral floor portion from given value without fractional component
\param value
  - float point 8-byte number to have its integer value truncated
\return
  - mathematical integer floor of the input 8-byte float point value
*/
s8 FloorD(const f8& value);

/** Get integral floor portion from given value without fractional component
\brief
  - get integral floor portion from given value without fractional component
\param value
  - float point 4-byte number to have its integer value truncated
\return
  - mathematical integer floor of the input 4-byte float point value
*/
s4 FloorF(f4 value);


/** Get integral remainder portion from given value within (-1.0, 1.0) range
\brief
  - get integral remainder portion from given value within (-1.0, 1.0) range
\param value
  - float point 8-byte number to have its integer value truncation removed
\return
  - difference of input value and the floored integer equivalent
*/
inline f8 FractionalD(const f8& value);

/** Get integral remainder portion from given value within (-1.0, 1.0) range
\brief
  - get integral remainder portion from given value within (-1.0, 1.0) range
\param value
  - float point 4-byte number to have its integer value truncation removed
\return
  - difference of input value and the floored integer equivalent
*/
inline f4 FractionalF(f4 value);


/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/ template <class T>
inline T IfOr(const T& givenVal, const T& otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline u1 IfOrB(u1 givenVal, u1 otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline s1 IfOrC(s1 givenVal, s1 otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline f8 IfOrD(const f8& givenVal, const f8& otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline f4 IfOrF(f4 givenVal, f4 otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline s4 IfOrI(s4 givenVal, s4 otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline s8 IfOrL(const s8& givenVal, const s8& otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline s2 IfOrS(s2 givenVal, s2 otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline u4 IfOrU(u4 givenVal, u4 otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline u8 IfOrUL(const u8& givenVal, const u8& otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline um IfOrUM(um givenVal, um otherVal, bool given);

/** Get ternary result of given value (if given is true), or other value if not
\brief
  - get ternary result of given value (if given is true), or other value if not
\param givenVal
  - value returned only if given condition resolves to be true (ternary lhs)
\param otherVal
  - value returned only if given condition resolves to be false (ternary rhs)
\return
  - ternary result of (given? givenVal : otherVal): 1 given input or the other
*/
inline u2 IfOrUS(u2 givenVal, u2 otherVal, bool given);


/** Get clamped copy of Typed value, set between a minimum & maximum range
\brief
  - Get clamped copy of Typed value, set between a minimum & maximum range
\details
  - can be used for any comparable class, but primarily for enum class values
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/ template <class T>
inline T In(const T& value, const T& max, const T& min);

/** Get clamped copy of unsigned value, set between a minimum & maximum range
\brief
  - Get clamped copy of unsigned value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline u1 InB(u1 value, u1 max, u1 min = 0u);

/** Get clamped copy of integer value, set between a minimum & maximum range
\brief
  - Get clamped copy of integer value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline s1 InC(s1 value, s1 max, s1 min = 0);

/** Get clamped copy of float value, set between a minimum & maximum range
\brief
  - Get clamped copy of float value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline f8 InD(const f8& value, const f8& max, const f8& min = 0.0);

/** Get clamped copy of float value, set between a minimum & maximum range
\brief
  - Get clamped copy of float value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline f4 InF(f4 value, f4 max, f4 min = 0.0f);

/** Get clamped copy of integer value, set between a minimum & maximum range
\brief
  - Get clamped copy of integer value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline s4 InI(s4 value, s4 max, s4 min = 0);

/** Get clamped copy of integer value, set between a minimum & maximum range
\brief
  - Get clamped copy of integer value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline s8 InL(const s8& value, const s8& max, const s8& min = 0l);

/** Get clamped copy of integer value, set between a minimum & maximum range
\brief
  - Get clamped copy of integer value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline s2 InS(s2 value, s2 max, s2 min = 0);

/** Get clamped copy of unsigned value, set between a minimum & maximum range
\brief
  - Get clamped copy of unsigned value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline u4 InU(u4 value, u4 max, u4 min = 0u);

/** Get clamped copy of unsigned value, set between a minimum & maximum range
\brief
  - Get clamped copy of unsigned value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline u8 InUL(const u8& value, const u8& max, const u8& min = 0ull);

/** Get clamped copy of subscript value, set between a minimum & maximum range
\brief
  - Get clamped copy of subscript value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline um InUM(um value, um max, um min = 0u);

/** Get clamped copy of unsigned value, set between a minimum & maximum range
\brief
  - Get clamped copy of unsigned value, set between a minimum & maximum range
\param value
  - original, input value to be evaluated against the allowed min, max range
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - input value clamped between minimum & maximum
*/
inline u2 InUS(u2 value, u2 max, u2 min = 0u);


/** Check if value of the given Type is in a minimum & maximum range
\brief
  - check if value of the given Type is in a minimum & maximum range
\param val
  - comparable input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
template <class T>
inline bool IsIn(const T& value, const T& max, const T& min);

/** Check if value of the given unsigned is in a minimum & maximum range
\brief
  - check if value of the given unsigned is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInB(u1 value, u1 max, u1 min);

/** Check if value of the given int/char is in a minimum & maximum range
\brief
  - check if value of the given int/char is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInC(s1 value, s1 max, s1 min);

/** Check if value of the given float is in a minimum & maximum range
\brief
  - check if value of the given float is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInD(const f8& value, const f8& max, const f8& min);

/** Check if value of the given float is in a minimum & maximum range
\brief
  - check if value of the given float is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInF(f4 value, f4 max, f4 min);

/** Check if value of the given integer is in a minimum & maximum range
\brief
  - check if value of the given integer is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInI(s4 value, s4 max, s4 min);

/** Check if value of the given integer is in a minimum & maximum range
\brief
  - check if value of the given integer is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInL(const s8& value, const s8& max, const s8& min);

/** Check if value of the given integer is in a minimum & maximum range
\brief
  - check if value of the given integer is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInS(s2 value, s2 max, s2 min);

/** Check if value of the given unsigned is in a minimum & maximum range
\brief
  - check if value of the given unsigned is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInU(u4 value, u4 max, u4 min);

/** Check if value of the given unsigned is in a minimum & maximum range
\brief
  - check if value of the given unsigned is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInUL(const u8& value, const u8& max, const u8& min);

/** Check if value of the given subscript is in a minimum & maximum range
\brief
  - check if value of the given subscript is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInUM(um value, um max, um min);

/** Check if value of the given unsigned is in a minimum & maximum range
\brief
  - check if value of the given unsigned is in a minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between minimum & maximum
*/
inline bool IsInUS(u2 value, u2 max, u2 min);

/** Check if value of the given Type is in a strict minimum & maximum range
\brief
  - check if value of the given Type is in a strict minimum & maximum range
\param val
  - comparable input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & maximum
*/
template <class T>
inline bool IsInS(const T& value, const T& max, const T& min);

/** Check if value of the given unsigned is in a strict minimum & maximum range
\brief
  - check if value of the given unsigned is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSB(u1 value, u1 max, u1 min);

/** Check if value of the given int/char is in a strict minimum & maximum range
\brief
  - check if value of the given int/char is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSC(s1 value, s1 max, s1 min);

/** Check if value of the given float is in a strict minimum & maximum range
\brief
  - check if value of the given float is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSD(const f8& value, const f8& max, const f8& min);

/** Check if value of the given float is in a strict minimum & maximum range
\brief
  - check if value of the given float is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSF(f4 value, f4 max, f4 min);

/** Check if value of the given integer is in a strict minimum & maximum range
\brief
  - check if value of the given integer is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSI(s4 value, s4 max, s4 min);

/** Check if value of the given integer is in a strict minimum & maximum range
\brief
  - check if value of the given integer is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSL(const s8& value, const s8& max, const s8& min);

/** Check if value of the given integer is in a strict minimum & maximum range
\brief
  - check if value of the given integer is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSS(s2 value, s2 max, s2 min);

/** Check if value of the given unsigned is in a strict minimum & maximum range
\brief
  - check if value of the given unsigned is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSU(u4 value, u4 max, u4 min);

/** Check if value of the given unsigned is in a strict minimum & maximum range
\brief
  - check if value of the given unsigned is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSUL(const u8& value, const u8& max, const u8& min);

/** Check if value of the given subscript is in a strict minimum & maximum range
\brief
  - check if value of the given subscript is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSUM(um value, um max, um min);

/** Check if value of the given unsigned is in a strict minimum & maximum range
\brief
  - check if value of the given unsigned is in a strict minimum & maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between strict minimum & maximum
*/
inline bool IsInSUS(u2 value, u2 max, u2 min);

/** Check if value of the given Type is in a minimum & strict maximum range
\brief
  - check if value of the given Type is in a minimum & strict maximum range
\param val
  - comparable input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
template <class T>
inline bool IsInSA(const T& value, const T& max, const T& min);

/** Check if value of the given unsigned is in a minimum & strict maximum range
\brief
  - check if value of the given unsigned is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAB(u1 value, u1 max, u1 min);

/** Check if value of the given int/char is in a minimum & strict maximum range
\brief
  - check if value of the given int/char is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAC(s1 value, s1 max, s1 min);

/** Check if value of the given float is in a minimum & strict maximum range
\brief
  - check if value of the given float is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAD(const f8& value, const f8& max, const f8& min);

/** Check if value of the given float is in a minimum & strict maximum range
\brief
  - check if value of the given float is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAF(f4 value, f4 max, f4 min);

/** Check if value of the given integer is in a minimum & strict maximum range
\brief
  - check if value of the given integer is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAI(s4 value, s4 max, s4 min);

/** Check if value of the given integer is in a minimum & strict maximum range
\brief
  - check if value of the given integer is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAL(const s8& value, const s8& max, const s8& min);

/** Check if value of the given integer is in a minimum & strict maximum range
\brief
  - check if value of the given integer is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAS(s2 value, s2 max, s2 min);

/** Check if value of the given unsigned is in a minimum & strict maximum range
\brief
  - check if value of the given unsigned is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAU(u4 value, u4 max, u4 min);

/** Check if value of the given unsigned is in a minimum & strict maximum range
\brief
  - check if value of the given unsigned is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAUL(const u8& value, const u8& max, const u8& min);

/** Check if value of the given subscript is in a minimum & strict maximum range
\brief
  - check if value of the given subscript is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAUM(um value, um max, um min);

/** Check if value of the given unsigned is in a minimum & strict maximum range
\brief
  - check if value of the given unsigned is in a minimum & strict maximum range
\param val
  - input value to be compared
\param max
  - maximum value should be equal to or below
\param min
  - minimum value should be strictly above
\return
  - whether value lies between minimum & strict maximum
*/
inline bool IsInSAUS(u2 value, u2 max, u2 min);

/** Check if value of the given Type is in a maximum & strict minimum range
\brief
  - check if value of the given Type is in a maximum & strict minimum range
\param val
  - comparable input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
template <class T>
inline bool IsInSB(const T& value, const T& max, const T& min);

/** Check if value of the given unsigned is in a maximum & strict minimum range
\brief
  - check if value of the given unsigned is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBB(u1 value, u1 max, u1 min);

/** Check if value of the given int/char is in a maximum & strict minimum range
\brief
  - check if value of the given int/char is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBC(s1 value, s1 max, s1 min);

/** Check if value of the given float is in a maximum & strict minimum range
\brief
  - check if value of the given float is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBD(const f8& value, const f8& max, const f8& min);

/** Check if value of the given float is in a maximum & strict minimum range
\brief
  - check if value of the given float is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBF(f4 value, f4 max, f4 min);

/** Check if value of the given integer is in a maximum & strict minimum range
\brief
  - check if value of the given integer is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBI(s4 value, s4 max, s4 min);

/** Check if value of the given integer is in a maximum & strict minimum range
\brief
  - check if value of the given integer is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBL(const s8& value, const s8& max, const s8& min);

/** Check if value of the given integer is in a maximum & strict minimum range
\brief
  - check if value of the given integer is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBS(s2 value, s2 max, s2 min);

/** Check if value of the given unsigned is in a maximum & strict minimum range
\brief
  - check if value of the given unsigned is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBU(u4 value, u4 max, u4 min);

/** Check if value of the given unsigned is in a maximum & strict minimum range
\brief
  - check if value of the given unsigned is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBUL(const u8& value, const u8& max, const u8& min);

/** Check if value of the given subscript is in a maximum & strict minimum range
\brief
  - check if value of the given subscript is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBUM(um value, um max, um min);

/** Check if value of the given unsigned is in a maximum & strict minimum range
\brief
  - check if value of the given unsigned is in a maximum & strict minimum range
\param val
  - input value to be compared
\param max
  - maximum value should be strictly below
\param min
  - minimum value should be equal to or above
\return
  - whether value lies between maximum & strict minimum
*/
inline bool IsInSBUS(u2 value, u2 max, u2 min);


/** Get reciprocal scalar (inverse =: 1 / value) of input value (-/+INf if 0.0)
\brief
  - get reciprocal scalar (inverse =: 1 / value) of input value (-/+INf if 0.0)
\param value
  - float point 8-byte number for which to calculate the reciprocal (inverse)
\return
  - 1.0 / value input given, unless value exactly 0 (in which case infinity)
*/
inline f8 InvD(const f8& value);

/** Get reciprocal scalar (inverse =: 1 / value) of input value (-/+INf if 0.0)
\brief
  - get reciprocal scalar (inverse =: 1 / value) of input value (-/+INf if 0.0)
\param value
  - float point 4-byte number for which to calculate the reciprocal (inverse)
\return
  - 1.0f / value input given, unless value exactly 0 (in which case infinity)
*/
inline f4 InvF(f4 value);

/** Get reciprocal scalar (inverse =: 1 / value) of input value (INf if 0)
\brief
  - get reciprocal scalar (inverse =: 1 / value) of input value (INf if 0)
\param value
  - 4-byte integer value for which to calculate the reciprocal (inverse)
\return
  - unless value is exactly 0, (1.0f / (f4)value) given (otherwise infinity)
*/
inline f4 InvI(s4 value);

/** Get reciprocal scalar (inverse =: 1 / value) of input value (INf if 0)
\brief
  - get reciprocal scalar (inverse =: 1 / value) of input value (INf if 0)
\param value
  - 8-byte integer value to use for denominator of (1 / value) fraction
\return
  - 1.0 / value input given, unless value is 0 (in which case infinity)
*/
inline f8 InvL(const s8& value);

/** Get reciprocal scalar (inverse =: 1 / value) of input value (INf if 0)
\brief
  - get reciprocal scalar (inverse =: 1 / value) of input value (INf if 0)
\param value
  - 4-byte unsigned value for which to calculate the reciprocal (inverse)
\return
  - unless value is exactly 0, (1.0f / (f4)value) given (otherwise infinity)
*/
inline f4 InvU(u4 value);

/** Get reciprocal scalar (inverse =: 1 / value) of input value (INf if 0)
\brief
  - get reciprocal scalar (inverse =: 1 / value) of input value (INf if 0)
\param value
  - 8-byte unsigned value to use for denominator of (1 / value) fraction
\return
  - 1.0 / value input given, unless value is 0 (in which case infinity)
*/
inline f8 InvUL(const u8& value);


/** Get whether the given value is strictly equal to an integer value
\brief
  - get whether the given value is strictly equal to an integer value
\param value
  - float-point 8-byte number to be evaluated as an integer value
\return
  - true only if value mantissa has no bits of fractional precision set on (1)
*/
inline bool IsIntD(const f8& value);

/** Get whether the given value is strictly equal to an integer value
\brief
  - get whether the given value is strictly equal to an integer value
\param value
  - float-point 4-byte number to be evaluated as an integer value
\return
  - true only if value mantissa has no bits of fractional precision set on (1)
*/
inline bool IsIntF(f4 value);


/** Get whether the given value is approximately equal to an integer value
\brief
  - get whether the given value is approximately equal to an integer value
\param value
  - float-point 8-byte number to be evaluated as an integer value
\param within
  - threshold for accepted distance (+/-) from integer values to consider true
\return
  - true if the value is within 0.0001 of an integer value
*/
inline bool IsIntegralD(const f8& value, const f8& within = _INF);

/** Get whether the given value is approximately equal to an integer value
\brief
  - get whether the given value is approximately equal to an integer value
\param value
  - float-point 4-byte number to be evaluated as an integer value
\param within
  - threshold for accepted distance (+/-) from integer values to consider true
\return
  - true if the value is within 0.0001 of an integer value
*/
inline bool IsIntegralF(f4 value, f4 within = _INf);

/** Get whether the 1-byte integer value is negative
\brief
  - get whether the 1-byte integer value is negative
\param value
  - input value to be checked for negativity
\return
  - true if MSB is set in value, implying value < 0
*/
inline bool IsNegC(s1 value);

/** Get whether the float-point 8-byte value is negative
\brief
  - get whether the float-point 8-byte value is negative
\param value
  - input value to be checked for negativity
\return
  - true if MSB is set in value, implying value < 0
*/
inline bool IsNegD(const f8& value);

/** Get whether the float-point 4-byte value is negative
\brief
  - get whether the float-point 4-byte value is negative
\param value
  - input value to be checked for negativity
\return
  - true if MSB is set in value, implying value < 0
*/
inline bool IsNegF(f4 value);

/** Get whether the 4-byte integer value is negative
\brief
  - get whether the 4-byte integer value is negative
\param value
  - input value to be checked for negativity
\return
  - true if MSB is set in value, implying value < 0
*/
inline bool IsNegI(s4 value);

/** Get whether the 8-byte integer value is negative
\brief
  - get whether the 8-byte integer value is negative
\param value
  - input value to be checked for negativity
\return
  - true if MSB is set in value, implying value < 0
*/
inline bool IsNegL(const s8& value);

/** Get whether the 2-byte integer value is negative
\brief
  - get whether the 2-byte integer value is negative
\param value
  - input value to be checked for negativity
\return
  - true if MSB is set in value, implying value < 0
*/
inline bool IsNegS(s2 value);


/** Get whether the given value is a non-NaN and non-infinite one
\brief
  - get whether the given value is a non-NaN and non-infinite one
\param value
  - float point 8-byte value to be checked for a real number value
\return
  - true if value is neither set to a NaN or Infinite reserved designation
*/
inline bool IsRealD(const f8& value);

/** Get whether the given value is a non-NaN and non-infinite one
\brief
  - get whether the given value is a non-NaN and non-infinite one
\param value
  - float point 4-byte value to be checked for a real number value
\return
  - true if value is neither set to a NaN or Infinite reserved designation
*/
inline bool IsRealF(f4 value);


/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the closest integer to mixed ratio from initial to terminal values
*/
inline u1 LerpB(u1 from, u1 to, f4 mix);

/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the closest integer to mixed ratio from initial to terminal values
*/
inline s1 LerpC(s1 from, s1 to, f4 mix);

/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the mixed ratio of initial and terminal: initial + mix * (terminal-initial)
*/
inline f8 LerpD(const f8& from, const f8& to, const f8& mix);

/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the mixed ratio of initial and terminal: initial + mix * (terminal-initial)
*/
inline f4 LerpF(f4 from, f4 to, f4 mix);

/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the closest integer to mixed ratio from initial to terminal values
*/
inline s4 LerpI(s4 from, s4 to, f4 mix);

/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the closest integer to mixed ratio from initial to terminal values
*/
inline s8 LerpL(const s8& from, const s8& to, const f8& mix);

/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the closest integer to mixed ratio from initial to terminal values
*/
inline s2 LerpS(s2 from, s2 to, f4 mix);

/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the closest integer to mixed ratio from initial to terminal values
*/
inline u4 LerpU(u4 from, u4 to, f4 mix);

/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the closest integer to mixed ratio from initial to terminal values
*/
inline u8 LerpUL(const u8& from, const u8& to, const f8& mix);

/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the closest integer to mixed ratio from initial to terminal values
*/
inline um LerpUM(um from, um to, f4 mix);

/** Get linearly interpolated mixed ratio value from intial to terminal inputs
\brief
  - get linearly interpolated mixed ratio value from intial to terminal inputs
\details
  - both for versatiliy and performance, no [0,1] value clamping consideration
\param initial
  - the (untested) low end of input values (may be greater than terminal)
\param terminal
  - the (untested) high end of input values (may be less than initial)
\param mix
  - the ratio of how far from initial to terminal the final output should be
\return
  - the closest integer to mixed ratio from initial to terminal values
*/
inline u2 LerpUS(u2 from, u2 to, f4 mix);


/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncB(u1 value);

/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncC(s1 value);

/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncD(const f8& value);

/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncF(f4 value);

/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncI(s4 value);

/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncL(const s8& value);

/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncS(s2 value);

/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncU(u4 value);

/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncUL(const u8& value);

/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncUM(um value);

/** Get the largest power of 2 used to find value as a sum of powers of 2
\brief
  - get the largest power of 2 used to find value as a sum of powers of 2
\param value
  - input to get the floor of the log base 2 for; highest enabled bit
\return
  - the floor of the largest power of 2 used == highest (non-sign) bit on
*/
inline u2 Log2TruncUS(u2 value);


/** Get the lowest value from a list of comparable values
\brief
  - get the lowest value from a list of comparable values
\details
  - usable with any comparable class, but intended for enum class comparisons
\param list
  - reference to the list of values to be scanned for a high value
\return
  - the highest value among the given values
*/
template <class T> inline T Max(std::initializer_list<T> list);

/** Get the lowest value from a list of comparable values
\brief
  - get the lowest value from a list of comparable values
\details
  - usable with any comparable class, but intended for enum class comparisons
\param list
  - reference to the list of values to be scanned for a high value
\return
  - the highest value among the given values listed
*/
template <class T> inline T Max(const std::vector<T>& list);

/** Get the highest value from 2 comparable values of the given unsigned values
\brief
  - get the highest value from 2 comparable values of the given unsigned values
\param a
  - first potential value for comparison as maximum of the set / pair
\param b
  - second potential value for comparison as maximum of the set / pair
\return
  - the highest value out of the given 2 values
*/
inline u1 MaxB(u1 a, u1 b);

/** Get the highest value from 2 comparable values of the given integer values
\brief
  - get the highest value from 2 comparable values of the given integer values
\param a
  - first potential value for comparison as maximum of the set / pair
\param b
  - second potential value for comparison as maximum of the set / pair
\return
  - the highest value out of the given 2 values
*/
inline s1 MaxC(s1 a, s1 b);

/** Get the highest value from 2 comparable values of the given floats
\brief
  - get the highest value from 2 comparable values of the given floats
\param a
  - first potential value for comparison as maximum of the set / pair
\param b
  - second potential value for comparison as maximum of the set / pair
\return
  - the highest value out of the given 2 values
*/
inline f8 MaxD(const f8& a, const f8& b);

/** Get the highest value from 2 comparable values of the given floats
\brief
  - get the highest value from 2 comparable values of the given floats
\param a
  - first potential value for comparison as maximum of the set / pair
\param b
  - second potential value for comparison as maximum of the set / pair
\return
  - the highest value out of the given 2 values
*/
inline f4 MaxF(f4 a, f4 b);

/** Get the highest value from 2 comparable values of the given integer values
\brief
  - get the highest value from 2 comparable values of the given integer values
\param a
  - first potential value for comparison as maximum of the set / pair
\param b
  - second potential value for comparison as maximum of the set / pair
\return
  - the highest value out of the given 2 values
*/
inline s4 MaxI(s4 a, s4 b);

/** Get the highest value from 2 comparable values of the given integer values
\brief
  - get the highest value from 2 comparable values of the given integer values
\param a
  - first potential value for comparison as maximum of the set / pair
\param b
  - second potential value for comparison as maximum of the set / pair
\return
  - the highest value out of the given 2 values
*/
inline s8 MaxL(const s8& a, const s8& b);

/** Get the highest value from 2 comparable values of the given integer values
\brief
  - get the highest value from 2 comparable values of the given integer values
\param a
  - first potential value for comparison as maximum of the set / pair
\param b
  - second potential value for comparison as maximum of the set / pair
\return
  - the highest value out of the given 2 values
*/
inline s2 MaxS(s2 a, s2 b);

/** Get the highest value from 2 comparable values of unsigned integers
\brief
  - get the highest value from 2 comparable values of unsigned integers
\param a
  - first potential value for comparison as maximum of the set
\param b
  - second potential value for comparison as maximum of the set
\return
  - the highest value out of the given 2 values
*/
inline u4 MaxU(u4 a, u4 b);

/** Get the highest value from 2 comparable values of unsigned integers
\brief
  - get the highest value from 2 comparable values of unsigned integers
\param a
  - first potential value for comparison as maximum of the set
\param b
  - second potential value for comparison as maximum of the set
\return
  - the highest value out of the given 2 values
*/
inline u8 MaxUL(const u8& a, const u8& b);

/** Get the highest value from 2 comparable values of unsigned integers
\brief
  - get the highest value from 2 comparable values of unsigned integers
\param a
  - first potential value for comparison as maximum of the set
\param b
  - second potential value for comparison as maximum of the set
\return
  - the highest value out of the given 2 values
*/
inline um MaxUM(um a, um b);

/** Get the highest value from 2 comparable values of unsigned integers
\brief
  - get the highest value from 2 comparable values of unsigned integers
\param a
  - first potential value for comparison as maximum of the set
\param b
  - second potential value for comparison as maximum of the set
\return
  - the highest value out of the given 2 values
*/
inline u2 MaxUS(u2 a, u2 b);


/** Get the 3D axis subscript about which the maximal float value is found
\brief
  - get the 3D axis subscript about which the maximal float value is found
\param x
  - x axis | yz plane value => [0] if the maximum of the 3 given values
\param y
  - y axis | xz plane value => [1] if the maximum of the 3 given values
\param z
  - z axis | xy plane value => [2] if the maximum of the 3 given values
\return
  - [0] if x is max; [1] if y is max; [2] if z is the maximum given value
*/
inline u1 MaxB3F(f4 x, f4 y, f4 z);


/** Get the lowest value from a list of comparable values
\brief
  - get the lowest value from a list of comparable values
\details
  - can be used for any comparable class, but primarily for enum class values
\param list
  - reference to the list of values to be scanned for a low value
\return
  - the lowest of the given values in the list
*/
template <class T> inline T Min(const std::vector<T>& list);

/** Get the lowest value from a list of comparable values
\brief
  - get the lowest value from a list of comparable values
\details
  - can be used for any comparable class, but primarily for enum class values
\param list
  - reference to the list of values to be scanned for a low value
\return
  - the lowest of the given values in the list
*/
template <class T> inline T Min(std::initializer_list<T> list);

/** Get the lowest value from 2 unsigned values
\brief
  - get the lowest value from 2 unsigned values
\param a
  - first potential unsigned value for comparison as minimum of the two
\param b
  - second potential unsigned value for comparison as minimum of the two
\return
  - the lowest of the given 2 unsigned values
*/
inline u1 MinB(u1 a, u1 b);

/** Get the lowest value from 2 integer / ascii char values
\brief
  - get the lowest value from 2 integer / ascii char values
\param a
  - first potential integer / ascii char value for comparison as minima
\param b
  - second potential integer / ascii char value for comparison as minima
\return
  - the lowest of the given 2 integer / ascii char values
*/
inline s1 MinC(s1 a, s1 b);

/** Get the lowest value from 2 float-point values
\brief
  - get the lowest value from 2 float-point values
\param a
  - first potential float-point value for comparison as minimm of the two
\param b
  - second potential float-point value for comparison as minimm of the two
\return
  - the lowest of the given 2 float-point values
*/
inline f8 MinD(const f8& a, const f8& b);

/** Get the lowest value from 2 float-point values
\brief
  - get the lowest value from 2 float-point values
\param a
  - first potential float-point value for comparison as minimm of the two
\param b
  - second potential float-point value for comparison as minimm of the two
\return
  - the lowest of the given 2 float-point values
*/
inline f4 MinF(f4 a, f4 b);

/** Get the lowest value from 2 integer values
\brief
  - get the lowest value from 2 integer values
\param a
  - first potential integer value for comparison as minimm of the two
\param b
  - second potential integer value for comparison as minimm of the two
\return
  - the lowest of the given 2 integer values
*/
inline s4 MinI(s4 a, s4 b);

/** Get the lowest value from 2 integer values
\brief
  - get the lowest value from 2 integer values
\param a
  - first potential integer value for comparison as minimm of the two
\param b
  - second potential integer value for comparison as minimm of the two
\return
  - the lowest of the given 2 integer values
*/
inline s8 MinL(const s8& a, const s8& b);

/** Get the lowest value from 2 integer values
\brief
  - get the lowest value from 2 integer values
\param a
  - first potential integer value for comparison as minimm of the two
\param b
  - second potential integer value for comparison as minimm of the two
\return
  - the lowest of the given 2 integer values
*/
inline s2 MinS(s2 a, s2 b);

/** Get the lowest value from 2 unsigned values
\brief
  - get the lowest value from 2 unsigned values
\param a
  - first potential unsigned value for comparison as minimum of the two
\param b
  - second potential unsigned value for comparison as minimum of the two
\return
  - the lowest of the given 2 unsigned values
*/
inline u4 MinU(u4 a, u4 b);

/** Get the lowest value from 2 unsigned values
\brief
  - get the lowest value from 2 unsigned values
\param a
  - first potential unsigned value for comparison as minimum of the two
\param b
  - second potential unsigned value for comparison as minimum of the two
\return
  - the lowest of the given 2 unsigned values
*/
inline u8 MinUL(const u8& a, const u8& b);

/** Get the lowest value from 2 subscript values
\brief
  - get the lowest value from 2 subscript values
\param a
  - first potential subscript value for comparison as minimum of the two
\param b
  - second potential subscript value for comparison as minimum of the two
\return
  - the lowest of the given 2 subscript values
*/
inline um MinUM(um a, um b);

/** Get the lowest value from 2 unsigned values
\brief
  - get the lowest value from 2 unsigned values
\param a
  - first potential unsigned value for comparison as minimum of the two
\param b
  - second potential unsigned value for comparison as minimum of the two
\return
  - the lowest of the given 2 unsigned values
*/
inline u2 MinUS(u2 a, u2 b);


/** Gets the equivalent of current value on a looping range between [min, max)
\brief
  - gets the equivalent of current value on a looping range between [min, max)
\details
  - temporary wrapper around later preferable implementation
\param val
  - input value to be remapped to a value between [min, max] (inclusive below)
\param max
  - upper bounds of acceptable value range (disclusive end value)
\param min
  - lower bounds of acceptable value range (inclusive, default 0)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline u1 ModB(u1 current, u1 max, u1 min = 0);

/** Gets the equivalent of current value on a looping range between [min, max)
\brief
  - gets the equivalent of current value on a looping range between [min, max)
\details
  - temporary wrapper around later preferable implementation
\param val
  - input value to be remapped to a value between [min, max] (inclusive below)
\param max
  - upper bounds of acceptable value range (disclusive end value)
\param min
  - lower bounds of acceptable value range (inclusive, default 0)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline s1 ModC(s1 current, s1 max, s1 min = 0);

/** Gets the equivalent of current value on a looping range between [min, max]
\brief
  - gets the equivalent of current value on a looping range between [min, max]
\param current
  - input value to be remapped to a value between [min, max] (inclusive)
\param max
  - upper bounds of acceptable value range
\param min
  - lower bounds of acceptable value range (default 0.0)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline f8 ModD(const f8& current, const f8& max, const f8& min = 0.0);

/** Gets the equivalent of current value on a looping range between [min, max]
\brief
  - gets the equivalent of current value on a looping range between [min, max]
\param current
  - input value to be remapped to a value between [min, max] (inclusive)
\param max
  - upper bounds of acceptable value range
\param min
  - lower bounds of acceptable value range (default 0.0f)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline f4 ModF(f4 current, f4 max, f4 min = 0.0f);

/** Gets the equivalent of current value on a looping range between [min, max)
\brief
  - gets the equivalent of current value on a looping range between [min, max)
\details
  - temporary wrapper around later preferable implementation
\param val
  - input value to be remapped to a value between [min, max] (inclusive below)
\param max
  - upper bounds of acceptable value range (disclusive end value)
\param min
  - lower bounds of acceptable value range (inclusive, default 0)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline s4 ModI(s4 current, s4 max, s4 min = 0);

/** Gets the equivalent of current value on a looping range between [min, max)
\brief
  - gets the equivalent of current value on a looping range between [min, max)
\details
  - temporary wrapper around later preferable implementation
\param val
  - input value to be remapped to a value between [min, max] (inclusive below)
\param max
  - upper bounds of acceptable value range (disclusive end value)
\param min
  - lower bounds of acceptable value range (inclusive, default 0)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline s8 ModL(const s8& current, const s8& max, const s8& min = 0);

/** Gets the equivalent of current value on a looping range between [min, max)
\brief
  - gets the equivalent of current value on a looping range between [min, max)
\details
  - temporary wrapper around later preferable implementation
\param val
  - input value to be remapped to a value between [min, max] (inclusive below)
\param max
  - upper bounds of acceptable value range (disclusive end value)
\param min
  - lower bounds of acceptable value range (inclusive, default 0)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline s2 ModS(s2 current, s2 max, s2 min = 0);

/** Gets the equivalent of current value on a looping range between [min, max)
\brief
  - gets the equivalent of current value on a looping range between [min, max)
\details
  - temporary wrapper around later preferable implementation
\param val
  - input value to be remapped to a value between [min, max] (inclusive below)
\param max
  - upper bounds of acceptable value range (disclusive end value)
\param min
  - lower bounds of acceptable value range (inclusive, default 0)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline u4 ModU(u4 current, u4 max, u4 min = 0);

/** Gets the equivalent of current value on a looping range between [min, max)
\brief
  - gets the equivalent of current value on a looping range between [min, max)
\details
  - temporary wrapper around later preferable implementation
\param val
  - input value to be remapped to a value between [min, max] (inclusive below)
\param max
  - upper bounds of acceptable value range (disclusive end value)
\param min
  - lower bounds of acceptable value range (inclusive, default 0)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline u8 ModUL(const u8& current, const u8& max, const u8& min = 0);

/** Gets the equivalent of current value on a looping range between [min, max)
\brief
  - gets the equivalent of current value on a looping range between [min, max)
\details
  - temporary wrapper around later preferable implementation
\param val
  - input value to be remapped to a value between [min, max] (inclusive below)
\param max
  - upper bounds of acceptable value range (disclusive end value)
\param min
  - lower bounds of acceptable value range (inclusive, default 0)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline um ModUM(um current, um max, um min = 0);

/** Gets the equivalent of current value on a looping range between [min, max)
\brief
  - gets the equivalent of current value on a looping range between [min, max)
\details
  - temporary wrapper around later preferable implementation
\param val
  - input value to be remapped to a value between [min, max] (inclusive below)
\param max
  - upper bounds of acceptable value range (disclusive end value)
\param min
  - lower bounds of acceptable value range (inclusive, default 0)
\return
  - remapped input value of current to be wrapped between min and max range
*/
inline u2 ModUS(u2 current, u2 max, u2 min = 0);


/** Check if value is within range of target (give or take tolerance)
\brief
  - check if value is within range of target (give or take tolerance)
\param value
  - input value to be compared with target value (within tolerance)
\param target
  - value which input val should be within tolerance of (give or take)
\param within
  - tolerance input value must be in range of (-/+), relative to target value
\return
  Whether value is nearly equal to target, within the tolerated margins
  */
inline bool NearD(const f8& value, const f8& target, const f8& within = _INf);

/** Check if value is within range of target (give or take tolerance)
\brief
  - check if value is within range of target (give or take tolerance)
\param value
  - input value to be compared with target value (within tolerance)
\param target
  - value which input val should be within tolerance of (give or take)
\param within
  - tolerance input value must be in range of (-/+), relative to target value
\return
  Whether value is nearly equal to target, within the tolerated margins
  */
inline bool NearF(const f4& value, const f4& target, const f4& within = _INf);

/** Get the quadratic solution for given triplet of coefficients for ax^2+bx+c
\brief
  - get the quadratic solution for given triplet of coefficients for ax^2+bx+c
\details
  - not providing pass-in imag values signifies shortcut behavior,
  skipping calculations of complex results as immaterial: complex results
  always return false as a means for differentiation in behavioral follow up,
  to distinguish finding real results from only finding complex ones
\param a
  - scalar coefficient for the given variable x's squared term  (ie a * x^2)
\param b
  - scalar coefficient for the given variable x's linear term   (ie b * x^1)
\param c
  - scalar coefficient for the given variable x's constant term (ie c * x^0)
\param real
  - pass in for real values of complex solution <+discriminant, -discriminant>
\param imag
  - imaginary values of complex solution (shortcut if -discr && not given)
\return
  - true if real solutions exist; not complex results; b^{2} > 4*a*c
*/
bool QuadraticF(f4 a, f4 b, f4 c, dp::v2f& real, dp::v2f* imag = nullptr);


/** Get the real validated value, discarding NaN or inf states & resetting to 0
\brief
  - Get the real validated value, discarding NaN or inf states & resetting to 0
  - input value to be set to neither infinity, nor a non-number value if it is
\return
  - input value as entered if a normal value state, 0 if in was infinite or NaN
*/
inline f8 RealD(const f8& value);

/** Get the real validated value, discarding NaN or inf states & resetting to 0
\brief
  - Get the real validated value, discarding NaN or inf states & resetting to 0
\param value
  - input value to be set to neither infinity, nor a non-number value if it is
\return
  - input value as entered if a normal value state, 0 if in was infinite or NaN
*/
inline f4 RealF(f4 value);


/** Get the nearest integral equivalent of input from fractional magnitude
\brief
  - get the nearest integral equivalent of input from fractional magnitude
\param value
  - float point 8-byte number to have its nearest integer equivalent found
\return
  - input ceiling (if fractional absolute value above 0.5) or floor (elsewise)
*/
s8 RoundD(f8 value);

/** Get the nearest integral equivalent of input from fractional magnitude
\brief
  - get the nearest integral equivalent of input from fractional magnitude
\param value
  - float point 4-byte number to have its nearest integer equivalent found
\return
  - input ceiling (if fractional absolute value above 0.5) or floor (elsewise)
*/
s4 RoundF(f4 value);


/** Get the scientific notation string representation of a 8-byte float value
\brief
  - get the scientific notation string representation of a 8-byte float value
\details
  - direct translation of IEEE-754 float point binary field values, for which
  two's complement is not utilized, and the mantissa's leading bit is always 1,
  as such the truncated, constant, leading 1 from scientific notation in the
  mantissa is listed ahead of the dyadic (binary decimal), fractional point for
  completeness, but is not actually read, merely part of the field separations.
  Exponent read bitwise with a constant -1023 bias offset value, displayed as
  read, not as stored.  sign bitfield output as leading sign instead of 1 : 0
  for general legibility, + output when 0 is read for more uniform width.
\param val
  - value to be converted into scientific notation representation
\return
  - string with mantissa & exponent (still in binary base) values from fields
*/
std::string SciD(f8 val);


/** Get the scientific notation string representation of a 4-byte float value
\brief
  - get the scientific notation string representation of a 4-byte float value
\details
  - direct translation of IEEE-754 float point binary field values, for which
  two's complement is not utilized, and the mantissa's leading bit is always 1,
  as such the truncated, constant, leading 1 from scientific notation in the
  mantissa is listed ahead of the dyadic (binary decimal), fractional point for
  completeness, but is not actually read, merely part of the field separations.
  Exponent read bitwise with a constant -127 bias offset value, displayed as
  read, not as stored.  sign bitfield output as leading sign instead of 1 : 0
  for general legibility, + output when 0 is read for more uniform width.
\param val
  - value to be converted into scientific notation representation
\return
  - string of signed-mantissa & exponent field (still in binary base) values
*/
std::string SciF(f4 val);


/* // Depricated due to static <=> inline incompatibility, for esoteric goals
inline f4 SentinelF(f4* sentinelValue); // F(loat) (common semantic float-point)
inline f8 SentinelD(f8* sentinelValue); // D(ouble) (common semantic 2*register)
inline s1 SentinelC(s1* sentinelValue); // C(har)     (common semantic of sByte)
inline s2 SentinelS(s2* sentinelValue); // S(hort)    (common semantic of int/2)
inline s4 SentinelI(s4* sentinelValue); // I(nteger)  (common semantic abbr int)
inline s8 SentinelW(s8* sentinelValue); // L  -> W(wide)  (UL-> L collision)
inline u1 SentinelB(u1* sentinelValue); // B(yte) (semantic label for u1)
inline u2 SentinelU(u2* sentinelValue); // US -> U(nsigned) (prune 2 char label)
inline u4 SentinelN(u4* sentinelValue); // U  -> N(umber) (-> U -: u2 : S -: s2)
inline u8 SentinelL(u8* sentinelValue); // UL -> L(arge)  (prune 2 char labels)
inline um SentinelM(um* sentinelValue); // UM -> M(emory) (prune 2 char labels)
*/ // User decisions for custom error states apt to create incompatibilities too


/** Set the magnitude / absolute value of a signed 1-byte integer value
\brief
  - set the magnitude / absolute value of a signed 1-byte integer value
\param value
  - input value to have the absolute value of it's current value set
\return
  - reference to the passed in value to continue operations upon inline
*/
inline s1& SetAbsC(s1& value);

/** Set the magnitude / absolute value of a float-point 8-byte value
\brief
  - set the magnitude / absolute value of a float-point 8-byte value
\param value
  - input value to have the absolute value of it's current value set
\return
  - reference to the passed in value to continue operations upon inline
*/
inline f8& SetAbsD(f8& value);

/** Set the magnitude / absolute value of a float-point 4-byte value
\brief
  - set the magnitude / absolute value of a float-point 4-byte value
\param value
  - input value to have the absolute value of it's current value set
\return
  - reference to the passed in value to continue operations upon inline
*/
inline f4& SetAbsF(f4& value);

/** Set the magnitude / absolute value of a signed 4-byte integer value
\brief
  - set the magnitude / absolute value of a signed 4-byte integer value
\param value
  - input value to have the absolute value of it's current value set
\return
  - reference to the passed in value to continue operations upon inline
*/
inline s4& SetAbsI(s4& value);

/** Set the magnitude / absolute value of a signed 8-byte integer value
\brief
  - set the magnitude / absolute value of a signed 8-byte integer value
\param value
  - input value to have the absolute value of it's current value set
\return
  - reference to the passed in value to continue operations upon inline
*/
inline s8& SetAbsL(s8& value);

/** Set the magnitude / absolute value of a signed 2-byte integer value
\brief
  - set the magnitude / absolute value of a signed 2-byte integer value
\param value
  - input value to have the absolute value of it's current value set
\return
  - reference to the passed in value to continue operations upon inline
*/
inline s2& SetAbsS(s2& value);


/** Set the result of u1 + u1 addition, with overflow exception considerations
\brief
  - set the result of u1 + u1 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddB(u1& current, u1 plus, OFlow policy = OFlowPolicy());

/** Set the result of s1 + s1 addition, with overflow exception considerations
\brief
  - set the result of s1 + s1 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddC(s1& current, s1 plus, OFlow policy = OFlowPolicy());

/** Set the result of f8 + f8 addition, with overflow exception considerations
\brief
  - set the result of f8 + f8 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddD(f8& current, const f8& plus, OFlow policy = OFlowPolicy());

/** Set the result of f4 + f4 addition, with overflow exception considerations
\brief
  - set the result of f4 + f4 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddF(f4& current, f4 plus, OFlow policy = OFlowPolicy());

/** Set the result of s4 + s4 addition, with overflow exception considerations
\brief
  - set the result of s4 + s4 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddI(s4& current, s4 plus, OFlow policy = OFlowPolicy());

/** Set the result of s8 + s8 addition, with overflow exception considerations
\brief
  - set the result of s8 + s8 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddL(s8& current, const s8& plus, OFlow policy = OFlowPolicy());

/** Set the result of s2 + s2 addition, with overflow exception considerations
\brief
  - set the result of s2 + s2 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddS(s2& current, s2 plus, OFlow policy = OFlowPolicy());

/** Set the result of u4 + u4 addition, with overflow exception considerations
\brief
  - set the result of u4 + u4 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddU(u4& current, u4 plus, OFlow policy = OFlowPolicy());

/** Set the result of u2 + u2 addition, with overflow exception considerations
\brief
  - set the result of u2 + u2 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddUL(u8& current, const u8& plus, OFlow policy=OFlowPolicy());

/** Set the result of um + um addition, with overflow exception considerations
\brief
  - set the result of um + um addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddUM(um& current, um plus, OFlow policy = OFlowPolicy());

/** Set the result of u2 + u2 addition, with overflow exception considerations
\brief
  - set the result of u2 + u2 addition, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value given prior to addition operation (left hand side of infix += )
\param plus
  - value given to add upon current input value (right hand side of infix += )
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true when exception didn't occur, false if overflow / underflow was handled
*/
inline bool SetAddUS(u2& current, u2 plus, OFlow policy = OFlowPolicy());


/** Set the result of u1 operations, with underflow exception considerations
\brief
  - set the result of u1 operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnB(u1& set, u1 to, OFlow policy = OFlowPolicy());

/** Set the result of s1 operations, with underflow exception considerations
\brief
  - set the result of s1 operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnC(s1& set, s1 to, OFlow policy = OFlowPolicy());

/** Set the result of f8 operations, with underflow exception considerations
\brief
  - set the result of f8 operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnD(f8& set, const f8& to, OFlow policy = OFlowPolicy());

/** Set the result of f4 operations, with underflow exception considerations
\brief
  - set the result of f4 operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnF(f4& set, f4 to, OFlow policy = OFlowPolicy());

/** Set the result of s4 operations, with underflow exception considerations
\brief
  - set the result of s4 operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnI(s4& set, s4 to, OFlow policy = OFlowPolicy());

/** Set the result of s8 operations, with underflow exception considerations
\brief
  - set the result of s8 operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnL(s8& set, const s8& to, OFlow policy = OFlowPolicy());

/** Set the result of s2 operations, with underflow exception considerations
\brief
  - set the result of s2 operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnS(s2& set, s2 to, OFlow policy = OFlowPolicy());

/** Set the result of u4 operations, with underflow exception considerations
\brief
  - set the result of u4 operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnU(u4& set, u4 to, OFlow policy = OFlowPolicy());

/** Set the result of u8 operations, with underflow exception considerations
\brief
  - set the result of u8 operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnUL(u8& set, const u8& to, OFlow policy = OFlowPolicy());

/** Set the result of um operations, with underflow exception considerations
\brief
  - set the result of um operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnUM(um& set, um to, OFlow policy = OFlowPolicy());

/** Set the result of u2 operations, with underflow exception considerations
\brief
  - set the result of u2 operations, with underflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & underflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process underflow exceptions
\return
  - true when no exception occured, false if underflow was handled
*/
inline bool SetDnUS(u2& set, u2 to, OFlow policy = OFlowPolicy());


/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/ template <class T>
inline T& SetIf(T& set, const T& to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline u1& SetIfB(u1& set, u1 to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline s1& SetIfC(s1& set, s1 to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline f8& SetIfD(f8& set, const f8& to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline f4& SetIfF(f4& set, f4 to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline s4& SetIfI(s4& set, s4 to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline s8& SetIfL(s8& set, const s8& to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline s2& SetIfS(s2& set, s2 to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline u4& SetIfU(u4& set, u4 to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline u8& SetIfUL(u8& set, const u8& to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline um& SetIfUM(um& set, um to, bool given);

/** Conditionally set = to value, only if given a true boolean
\brief
  - conditionally set = to value, only if given a true boolean
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - new value to set as current input value (right hand side of infix (=) op)
\param given
  - boolean condition to determine whether to change set value (only if true)
\return
  - pass through of set (new or original), reference for inline continuation
*/
inline u2& SetIfUS(u2& set, u2 to, bool given);


/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/ template <class T>
inline bool SetIfP(T* set, const T& to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPB(u1* set, u1 to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPC(s1* set, s1 to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPD(f8* set, const f8& to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPF(f4* set, f4 to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPI(s4* set, s4 to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPL(s8* set, const s8& to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPS(s2* set, s2 to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPU(u4* set, u4 to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPUL(u8* set, const u8& to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPUM(um* set, um to, bool given);

/** Set set's value (*set) = to value, if given is true set and is non-null
\brief
  - set set's value (*set) = to value, if given is true set and is non-null
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\param given
  - boolean condition that must be fulfilled for set to be dereferenced and set
\return
  - true if set points to a valid u1, false if set was null and not settable
*/
inline bool SetIfPUS(u2* set, u2 to, bool given);


/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/ template <class T>
inline T& SetIfOr(T& set, const T& to, bool given, const T& orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline u1& SetIfOrB(u1& set, u1 to, bool given, u1 orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline s1& SetIfOrC(s1& set, s1 to, bool given, s1 orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline f8& SetIfOrD(f8& set, const f8& to, bool given, const f8& orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline f4& SetIfOrF(f4& set, f4 to, bool given, f4 orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline s4& SetIfOrI(s4& set, s4 to, bool given, s4 orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline s8& SetIfOrL(s8& set, const s8& to, bool given, const s8& orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline s2& SetIfOrS(s2& set, s2 to, bool given, s2 orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline u4& SetIfOrU(u4& set, u4 to, bool given, u4 orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline u8& SetIfOrUL(u8& set, const u8& to, bool given, const u8& orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline um& SetIfOrUM(um& set, um to, bool given, um orTo);

/** Ternary set = to value given boolean condition is true, orTo if false
\brief
  - ternary set = to value given boolean condition is true, orTo if false
\param set
  - input value given prior to set operation (left hand side of infix (=) op)
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean to determine whether to change set value to : orTo conditionally
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - pass through of set (to : orTo), value reference for inline continuation
*/
inline u2& SetIfOrUS(u2& set, u2 to, bool given, u2 orTo);


/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/ template <class T>
inline bool SetIfOrP(T* set, const T& to, bool given, const T& orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPB(u1* set, u1 to, bool given, u1 orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPC(s1* set, s1 to, bool given, s1 orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPD(f8* set, const f8& to, bool given, const f8& orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPF(f4* set, f4 to, bool given, f4 orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPI(s4* set, s4 to, bool given, s4 orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPL(s8* set, const s8& to, bool given, const s8& orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPS(s2* set, s2 to, bool given, s2 orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPU(u4* set, u4 to, bool given, u4 orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPUL(u8* set, const u8& to, bool given, const u8& orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPUM(um* set, um to, bool given, um orTo);

/** Ternary *set = to, given boolean is true, orTo if false, if set is non-null
\brief
  - ternary *set = to, given boolean is true, orTo if false, if set is non-null
\details
  - wrapper around inlined basics for profiled, framework wide replacement
  branchless implementation chosen for current compilation expedience
\param set
  - input value pointer to dereference and set => *set = given? to : orTo
\param to
  - default to set new input value (given?) (right side of (=); true of (?) op)
\param given
  - boolean condition to determine whether to change set value to : orTo
\param orTo
  - alternate to set input value (!given?) (right side of (=); false of (?) op)
\return
  - true if set is non-null, and either value is set, false elsewise on no-op
*/
inline bool SetIfOrPUS(u2* set, u2 to, bool given, u2 orTo);


/** Clamp given float value between an allowed minimum & maximum range
\brief
  - clamp given float value between an allowed minimum & maximum range
\details
  - can be used for any comparable class, but primarily for enum class values
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
template <class T> inline T& SetIn(T& value, const T& max, const T& min);

/** Clamp input unsigned value between [min, max] inclusive range
\brief
  - clamp input unsigned value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline u1& SetInB(u1& value, u1 max, u1 min);

/** Clamp input integer / char value between [min, max] inclusive range
\brief
  - clamp input integer / char value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline s1& SetInC(s1& value, s1 max, s1 min);

/** Clamp input float-point value between [min, max] inclusive range
\brief
  - clamp input float-point value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline f8& SetInD(f8& value, const f8& max, const f8& min);

/** Clamp input float-point value between [min, max] inclusive range
\brief
  - clamp input float-point value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline f4& SetInF(f4& value, f4 max, f4 min);

/** Clamp input integer value between [min, max] inclusive range
\brief
  - clamp input integer value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline s4& SetInI(s4& value, s4 max, s4 min);

/** Clamp input integer value between [min, max] inclusive range
\brief
  - clamp input integer value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline s8& SetInL(s8& value, const s8& max, const s8& min);

/** Clamp input integer value between [min, max] inclusive range
\brief
  - clamp input integer value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline s2& SetInS(s2& value, s2 max, s2 min);

/** Clamp input unsigned value between [min, max] inclusive range
\brief
  - clamp input unsigned value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline u4& SetInU(u4& value, u4 max, u4 min);

/** Clamp input unsigned value between [min, max] inclusive range
\brief
  - clamp input unsigned value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline u8& SetInUL(u8& value, const u8& max, const u8& min);

/** Clamp input subscript value between [min, max] inclusive range
\brief
  - clamp input subscript value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline um& SetInUM(um& value, um max, um min);

/** Clamp input unsigned value between [min, max] inclusive range
\brief
  - clamp input unsigned value between [min, max] inclusive range
\param value
  - input value to be modified (as required, if outside of range)
\param max
  - maximum to be allowed in result compared to input value
\param min
  - minimum to be allowed in result compared to input value
\return
  - reference to the passed in value to continue with inline operations
*/
inline u2& SetInUS(u2& value, u2 max, u2 min);


/** Set input value to inverse (reciprocal) of it's current value (INF if 0)
\brief
  - set input value to inverse (reciprocal) of it's current value (INF if 0)
\param toReciprocal
  - input value to be set to 1 / toReciprocal if non-zero, else (+/-) INF 
\return
  - pass through of set reciprocal value reference for inline continuation
*/
inline f8& SetInvD(f8& toReciprocal);
/** Set input value to inverse (reciprocal) of it's current value (INf if 0)
\brief
  - set input value to inverse (reciprocal) of it's current value (INf if 0)
\param toReciprocal
  - input value to be set to 1 / toReciprocal if non-zero, else (+/-) INF 
\return
  - pass through of set reciprocal value reference for inline continuation
*/
inline f4& SetInvF(f4& toReciprocal);


/** Set value of set (*set) = to value, if set is a valid (non-null), T pointer
\brief
  - set value of set (*set) = to value, if set is a valid (non-null), T pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid T datum, and was set, false if not possible
*/ template <class T>
inline bool SetP(T* set, const T& to);

/** Set set's value (*set) = to value, if set is a valid (non-null), u1 pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), u1 pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid u1 datum, and was set, false if not possible
*/
inline bool SetPB(u1* set, u1 to);

/** Set set's value (*set) = to value, if set is a valid (non-null), s1 pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), s1 pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid s1 datum, and was set, false if not possible
*/
inline bool SetPC(s1* set, s1 to);

/** Set set's value (*set) = to value, if set is a valid (non-null), f8 pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), f8 pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid f8 datum, and was set, false if not possible
*/
inline bool SetPD(f8* set, const f8& to);

/** Set set's value (*set) = to value, if set is a valid (non-null), f4 pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), f4 pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid f4 datum, and was set, false if not possible
*/
inline bool SetPF(f4* set, f4 to);

/** Set set's value (*set) = to value, if set is a valid (non-null), s4 pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), s4 pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid s4 datum, and was set, false if not possible
*/
inline bool SetPI(s4* set, s4 to);

/** Set set's value (*set) = to value, if set is a valid (non-null), s8 pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), s8 pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid s8 datum, and was set, false if not possible
*/
inline bool SetPL(s8* set, const s8& to);

/** Set set's value (*set) = to value, if set is a valid (non-null), s2 pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), s2 pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid s2 datum, and was set, false if not possible
*/
inline bool SetPS(s2* set, s2 to);

/** Set set's value (*set) = to value, if set is a valid (non-null), u4 pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), u4 pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid u4 datum, and was set, false if not possible
*/
inline bool SetPU(u4* set, u4 to);

/** Set set's value (*set) = to value, if set is a valid (non-null), u8 pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), u8 pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid u8 datum, and was set, false if not possible
*/
inline bool SetPUL(u8* set, const u8& to);

/** Set set's value (*set) = to value, if set is a valid (non-null), um pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), um pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid um datum, and was set, false if not possible
*/
inline bool SetPUM(um* set, um to);

/** Set set's value (*set) = to value, if set is a valid (non-null), u2 pointer
\brief
  - set set's value (*set) = to value, if set is a valid (non-null), u2 pointer
\param set
  - input pointer to set (*set = to), if valid (left hand side of infix (=) op)
\param to
  - value to use as input's set value after (right hand side of infix (=) op)
\return
  - true if set points to a valid u2 datum, and was set, false if not possible
*/
inline bool SetPUS(u2* set, u2 to);


/** Set u1 - u1 subtraction results, with overflow exception considerations
\brief
  - set u1 - u1 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubB(u1& current, u1 plus, OFlow policy = OFlowPolicy());

/** Set s1 - s1 subtraction results, with overflow exception considerations
\brief
  - set s1 - s1 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubC(s1& current, s1 minus, OFlow policy = OFlowPolicy());

/** Set f8 - f8 subtraction results, with overflow exception considerations
\brief
  - set f8 - f8 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubD(f8& current, const f8& minus, OFlow policy=OFlowPolicy());

/** Set f4 - f4 subtraction results, with overflow exception considerations
\brief
  - set f4 - f4 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubF(f4& current, f4 minus, OFlow policy = OFlowPolicy());

/** Set s4 - s4 subtraction results, with overflow exception considerations
\brief
  - set s4 - s4 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubI(s4& current, s4 minus, OFlow policy = OFlowPolicy());

/** Set s8 - s8 subtraction results, with overflow exception considerations
\brief
  - set s8 - s8 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubL(s8& current, const s8& minus, OFlow policy=OFlowPolicy());

/** Set s2 - s2 subtraction results, with overflow exception considerations
\brief
  - set s2 - s2 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubS(s2& current, s2 minus, OFlow policy = OFlowPolicy());

/** Set u4 - u4 subtraction results, with overflow exception considerations
\brief
  - set u4 - u4 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubU(u4& current, u4 minus, OFlow policy = OFlowPolicy());

/** Set u8 - u8 subtraction results, with overflow exception considerations
\brief
  - set u8 - u8 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubUL(u8& current, const u8& minus, OFlow policy=OFlowPolicy());

/** Set um - um subtraction results, with overflow exception considerations
\brief
  - set um - um subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubUM(um& current, um minus, OFlow policy = OFlowPolicy());

/** Set u2 - u2 subtraction results, with overflow exception considerations
\brief
  - set u2 - u2 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input value prior to subtraction operation (left hand side of infix -=)
\param minus
  - value given to subtract from current input (right hand side of infix -=)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - true if the expected value was set; false if overflow / underflow occurred
*/
inline bool SetSubUS(u2& current, u2 minus, OFlow policy = OFlowPolicy());


/** Set the result of u1 operations, with overflow exception considerations
\brief
  - set the result of u1 operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpB(u1& set, u1 to, OFlow policy = OFlowPolicy());

/** Set the result of s1 operations, with overflow exception considerations
\brief
  - set the result of s1 operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpC(s1& set, s1 to, OFlow policy = OFlowPolicy());

/** Set the result of f8 operations, with overflow exception considerations
\brief
  - set the result of f8 operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpD(f8& set, const f8& to, OFlow policy = OFlowPolicy());

/** Set the result of f4 operations, with overflow exception considerations
\brief
  - set the result of f4 operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpF(f4& set, f4 to, OFlow policy = OFlowPolicy());

/** Set the result of s4 operations, with overflow exception considerations
\brief
  - set the result of s4 operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpI(s4& set, s4 to, OFlow policy = OFlowPolicy());

/** Set the result of s8 operations, with overflow exception considerations
\brief
  - set the result of s8 operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpL(s8& set, const s8& to, OFlow policy);

/** Set the result of s2 operations, with overflow exception considerations
\brief
  - set the result of s2 operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpS(s2& set, s2 to, OFlow policy = OFlowPolicy());

/** Set the result of u4 operations, with overflow exception considerations
\brief
  - set the result of u4 operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpU(u4& set, u4 to, OFlow policy = OFlowPolicy());

/** Set the result of u8 operations, with overflow exception considerations
\brief
  - set the result of u8 operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpUL(u8& set, const u8& to, OFlow policy = OFlowPolicy());

/** Set the result of um operations, with overflow exception considerations
\brief
  - set the result of um operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpUM(um& set, um to, OFlow policy = OFlowPolicy());

/** Set the result of u2 operations, with overflow exception considerations
\brief
  - set the result of u2 operations, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & overflow occurs
\param val
  - input value given prior to set operation (left hand side of infix (=) op)
\param result
  - new value to set as current input value (right hand side of infix (=) op)
\param policy
  - defaultable typification of how to process overflow exceptions
\return
  - true when no exception occured, false if overflow was handled
*/
inline bool SetUpUS(u2& set, u2 to, OFlow policy = OFlowPolicy());


/** Get the sine (y-axis measure of tri opp./hyp.) for input 2D angle of arc
\brief
  - get the sine (y-axis measure of tri opp./hyp.) for input 2D angle of arc
\param angle
  - arc sweep from 0 point in the x-axis of the XY plane, counter-clockwise
\return
  - right triangle height [0,1] y-axis size, with angle as hypoteneuse length 1
*/
f4 SinF(dp::ang angle);


/** Calculate the square root of a given input float point value
\brief
  - calculate the square root of a given input float point value
\details
  - for platform compatibility assurances it's a wrapper around pro/demotion.
  could be a place to solve with something fancier in the future too, if a new
  fast inverse square root shows that's not just a pre-emptive optimization
  this is also an opportunity to specifically not do that in a place where one
  might think to add it. because you shouldn't anymore: it's not faster.
\param value
  - input value for which to calculate the square root
\return
  - the square root of the input value given
*/
inline f4 SqrtF(f4 value);


/** Get ASCII std::string equivalent for input 1-byte unsigned [0,255] value
\brief
  - get ASCII std::string equivalent for input 1-byte unsigned [0,255] value
\details
  - mostly a string stream wrapper family, also for int output instead of char
\param value
  - input 1-byte unsigned datum to convert to numeric ASCII std::string output
\return
  - unsigned integer value [0, 255] of input value, as an ASCII std::string
*/
std::string StrB(u1 value);


/** Get ASCII std::string equivalent for input 1-byte signed [-128,127] value
\brief
  - get ASCII std::string equivalent for input 1-byte signed [-128,127] value
\details
  - mostly a string stream wrapper family, also for int output instead of char
\param value
  - input 1-byte signed datum to convert to numeric ASCII std::string output
\return
  - signed integer value [-128,127] of input value, as an ASCII std::string
*/
std::string StrC(s1 value);


/** Get ASCII std::string equivalent for input 8-byte float-point value nuances
\brief
  - get ASCII std::string equivalent for input 8-byte float-point value nuances
\details
  - mostly a standard string stream wrapper to facilitate output consistency
\param value
  - input 8-byte float datum to convert to numeric ASCII std::string output
\param fracDigits
  - optional decimal precision format output contraint (carry last value over)
\return
  - ASCII std::string of input float value, adding preferential specificity
*/
std::string StrD(const f8& value, u1 fracDigits = u1_INVALID);


/** Get ASCII std::string equivalent for input 4-byte float-point value nuances
\brief
  - get ASCII std::string equivalent for input 4-byte float-point value nuances
\details
  - mostly a standard string stream wrapper to facilitate output consistency
\param value
  - input 4-byte float datum to convert to numeric ASCII std::string output
\param fracDigits
  - optional decimal precision format output contraint (carry last value over)
\return
  - ASCII std::string of input float value, adding preferential specificity
*/
std::string StrF(f4 value, u1 fracDigits = u1_INVALID);


/** Get ASCII std::string equivalent for input 8-byte [-2^{63}, +2^{63}) value
\brief
  - get ASCII std::string equivalent for input 8-byte [-2^{63}, +2^{63}) value
\details
  - mostly a string stream wrapper family: primitive conversion syntactic sugar
\param value
  - input 8-byte signed datum to convert to numeric ASCII std::string output
\return
  - ASCII std::string representation of integer [-2^{63}, +2^{63}) input value
*/
std::string StrL(const s8& value);


/** Get ASCII std::string equivalent for input 4-byte [-2^{31}, +2^{31}) value
\brief
  - get ASCII std::string equivalent for input 4-byte [-2^{31}, +2^{31}) value
\details
  - mostly a string stream wrapper family: primitive conversion syntactic sugar
\param value
  - input 4-byte signed datum to convert to numeric ASCII std::string output
\return
  - ASCII std::string representation of integer [-2^{31}, +2^{31}) input value
*/
std::string StrI(s4 value);


/** Get hexadecimal ASCII std::string equivalent of a given address input value
\brief
  - get hexadecimal ASCII std::string equivalent of a given address input value
\details
  - mostly a string stream wrapper family: primitive conversion syntactic sugar
\param value
  - any pointer used as input to convert to hexadecimal ASCII std::string output
\return
  - ASCII std::string representation of a given memory address input value
*/
std::string StrP(void* value);


/** Get ASCII std::string equivalent for 2-byte [-32768, 32,767] input value
\brief
  - get ASCII std::string equivalent for 2-byte [-32768, 32,767] input value
\details
  - mostly a string stream wrapper family: primitive conversion syntactic sugar
\param value
  - input 2-byte signed datum to convert to numeric ASCII std::string output
\return
  - ASCII std::string representation of integer [-32768, 32,767] input value
*/
std::string StrS(s2 value);


/** Get ASCII std::string equivalent for 4-byte [0, 4,294,967,296] input value
\brief
  - get ASCII std::string equivalent for 4-byte [0, 4,294,967,296] input value
\details
  - mostly a string stream wrapper family: primitive conversion syntactic sugar
\param value
  - input 4-byte unsigned datum to convert to numeric ASCII std::string output
\return
  - ASCII std::string representation of integer [0, 4,294,967,296] input value
*/
std::string StrU(s4 value);


/** Get the ASCII std::string equivalent for 8-byte [0, +2^{64}] input value
\brief
  - get the ASCII std::string equivalent for 8-byte [0, +2^{64}] input value
\details
  - mostly a string stream wrapper family: primitive conversion syntactic sugar
\param value
  - input 8-byte unsigned datum to convert to numeric ASCII std::string output
\return
  - ASCII std::string representation of integer [0, +2^{64}] input value
*/
std::string StrUL(const u8& value);


/** Get ASCII std::string equivalent of a given unsigned subscript input value
\brief
  - get ASCII std::string equivalent of a given unsigned subscript input value
\details
  - mostly a string stream wrapper family: primitive conversion syntactic sugar
\param value
  - unsigned subscript input to convert to numeric ASCII std::string output
\return
  - ASCII std::string representation of unsigned integer subscript input value
*/
std::string StrUM(um value);


/** Get ASCII std::string equivalent of 2-byte [0, 65,535] unsigned input value
\brief
  - get ASCII std::string equivalent of 2-byte [0, 65,535] unsigned input value
\details
  - mostly a string stream wrapper family: primitive conversion syntactic sugar
\param value
  - input 2-byte unsigned datum to convert to numeric ASCII std::string output
\return
  - ASCII std::string representation of unsigned int [0, 65,535] input value
*/
std::string StrUS(u2 value);


/** Get u1 - u1 subtraction results, with overflow exception considerations
\brief
  - get u1 - u1 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (u1 - u1) subtraction results under the given overflow / underflow policy
*/
inline u1 SubB(u1 current, u1 minus, OFlow policy = OFlowPolicy());


/** Get s1 - s1 subtraction results, with overflow exception considerations
\brief
  - get s1 - s1 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (s1 - s1) subtraction results under the given overflow / underflow policy
*/
inline s1 SubC(s1 current, s1 minus, OFlow policy = OFlowPolicy());

/** Get f8 - f8 subtraction results, with overflow exception considerations
\brief
  - get f8 - f8 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (f8 - f8) subtraction results under the given overflow / underflow policy
*/
inline f8 SubD(f8 current, const f8& minus, OFlow policy = OFlowPolicy());

/** Get f4 - f4 subtraction results, with overflow exception considerations
\brief
  - get f4 - f4 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (f4 - f4) subtraction results under the given overflow / underflow policy
*/
inline f4 SubF(f4 current, f4 minus, OFlow policy = OFlowPolicy());

/** Get s8 - s8 subtraction results, with overflow exception considerations
\brief
  - get s8 - s8 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (s8 - s8) subtraction results under the given overflow / underflow policy
*/
inline s8 SubL(s8 current, const s8& minus, OFlow policy = OFlowPolicy());

/** Get s4 - s4 subtraction results, with overflow exception considerations
\brief
  - get s4 - s4 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (s4 - s4) subtraction results under the given overflow / underflow policy
*/
inline s4 SubI(s4 current, s4 minus, OFlow policy = OFlowPolicy());

/** Get s2 - s2 subtraction results, with overflow exception considerations
\brief
  - get s2 - s2 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (s2 - s2) subtraction results under the given overflow / underflow policy
*/
inline s2 SubS(s2 current, s2 minus, OFlow policy = OFlowPolicy());

/** Get u4 - u4 subtraction results, with overflow exception considerations
\brief
  - get u4 - u4 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (u4 - u4) subtraction results under the given overflow / underflow policy
*/
inline u4 SubU(u4 current, u4 minus, OFlow policy = OFlowPolicy());

/** Get u8 - u8 subtraction results, with overflow exception considerations
\brief
  - get u8 - u8 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (u8 - u8) subtraction results under the given overflow / underflow policy
*/
inline u8 SubUL(u8 current, const u8& minus, OFlow policy = OFlowPolicy());

/** Get um - um subtraction results, with overflow exception considerations
\brief
  - get um - um subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (um - um) subtraction results under the given overflow / underflow policy
*/
inline um SubUM(um current, um minus, OFlow policy = OFlowPolicy());

/** Get u2 - u2 subtraction results, with overflow exception considerations
\brief
  - get u2 - u2 subtraction results, with overflow exception considerations
\details
  - exceptions thrown if policy prompts for user input & over/underflow occurs
\param current
  - input given prior to subtraction operation (left hand side of infix -)
\param minus
  - value to take away from current input value (right hand side of infix -)
\param policy
  - defaultable typification of how to process overflow / underflow exceptions
\return
  - (u2 - u2) subtraction results under the given overflow / underflow policy
*/
inline u2 SubUS(u2 current, u2 minus, OFlow policy = OFlowPolicy());


/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
template <class T> inline void Swap(T& a, T& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapB(u1& a, u1& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapC(s1& a, s1& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapD(f8& a, f8& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapF(f4& a, f4& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapI(s4& a, s4& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapL(s8& a, s8& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapS(s2& a, s2& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapU(u4& a, u4& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapUL(u8& a, u8& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapUM(um& a, um& b);

/** Exchange two values with one another
\brief
  - exchange two values with one another
\param a
  - reference to variable a to be set to b
\param b
  - reference to variable b to be set to a
*/
inline void SwapUS(u2& a, u2& b);


/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
template <class T> inline void SwapIf(T& a, T& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfB(u1& a, u1& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfC(s1& a, s1& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfD(f8& a, f8& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfF(f4& a, f4& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfI(s4& a, s4& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfL(s8& a, s8& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfS(s2& a, s2& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfU(u4& a, u4& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfUL(u8& a, u8& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfUM(um& a, um& b, bool given);

/** Exchange or preserve two values' stored locations based the given condition
\brief
  - exchange or preserve two values' stored locations based the given condition
\param a
  - reference to variable a to be set to b if given is true; left as a if not
\param b
  - reference to variable b to be set to a if given is true; left as b if not
\param given
  - swap a with b if true: expression to resolve inline, guiding operation flow
*/
inline void SwapIfUS(u2& a, u2& b, bool given);

/** Get the tangent (slope measure of tri opp./adj.) for input 2D angle of arc
\brief
  - get the tangent (slope measure of tri opp./adj.) for input 2D angle of arc.
\param angle
  - arc sweep from 0 point in the x-axis of the XY plane, counter-clockwise
\return
  - right triangle slope ratio: y/x; rise/run; sin(t)/cos(t); opposite/adjacent
*/
f4 TanF(dp::ang angle);


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*               Public Template / Inline Function Definitions                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

inline OFlow OFlowPolicy(OFlow* policyPreference)
{
  static OFlow policy = DEFAULT_OFLOW_POLICY;
  if (policyPreference) { policy = *policyPreference; }
  return policy;
} // end OFlow OFlowPolicy(OFlow*)                                            */


inline s1 AbsC(s1 value)
{ return MaxC(value, -value); } // end s1 AbsI(s1)                            */
inline f8 AbsD(const f8& value)
{ return MaxD(value, -value); } // end f8 AbsD(const f8&)                     */
inline f4 AbsF(f4 value)
{ return MaxF(value, -value); } // end f4 AbsF(f4)                            */
inline s4 AbsI(s4 value)
{ return MaxI(value, -value); } // end s4 AbsI(s4)                            */
inline s8 AbsL(const s8& value)
{ return MaxL(value, -value); } // end s8 AbsL(const s8&)                     */
inline s2 AbsS(s2 value)
{ return MaxS(value, -value); } // end s2 AbsS(s2)                            */


inline u1 AddB(u1 current, u1 plus, OFlow policy)
{
  u1 prior = current;
  try
  {
    if (!SetAddB(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end u1 AddB(u1, u1, OFlow)                                               */
inline s1 AddC(s1 current, s1 plus, OFlow policy)
{
  s1 prior = current;
  try
  {
    if (!SetAddC(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end s1 AddC(s1, s1, OFlow)                                               */
inline f8 AddD(f8 current, const f8& plus, OFlow policy)
{
  f8 prior = current;
  try
  {
    if (!SetAddD(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end f8 AddD(f8, const f8&, OFlow)                                        */
inline f4 AddF(f4 current, f4 plus, OFlow policy)
{
  f4 prior = current;
  try
  {
    if (!SetAddF(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end f4 AddF(f4, f4, OFlow)                                               */
inline s4 AddI(s4 current, s4 plus, OFlow policy)
{
  s4 prior = current;
  try
  {
    if (!SetAddI(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end s4 AddI(s4, s4, OFlow)                                               */
inline s8 AddL(s8 current, const s8& plus, OFlow policy)
{
  s8 prior = current;
  try
  {
    if (!SetAddL(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end s8 AddL(s8, const s8&, OFlow)                                        */
inline s2 AddS(s2 current, s2 plus, OFlow policy)
{
  s2 prior = current;
  try
  {
    if (!SetAddS(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end s2 AddS(s2, s2, OFlow)                                               */
inline u4 AddU(u4 current, u4 plus, OFlow policy)
{
  u4 prior = current;
  try
  {
    if (!SetAddU(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end u4 AddU(u4, u4, OFlow)                                               */
inline u8 AddUL(u8 current, const u8& plus, OFlow policy)
{
  u8 prior = current;
  try
  {
    if (!SetAddUL(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end u8 AddUL(u8, const u8&, OFlow)                                       */
inline um AddUM(um current, um plus, OFlow policy)
{
  um prior = current;
  try
  {
    if (!SetAddUM(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end um AddUM(um, um, OFlow)                                              */
inline u2 AddUS(u2 current, u2 plus, OFlow policy)
{
  u2 prior = current;
  try
  {
    if (!SetAddUS(current, plus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " + " + std::to_string(plus), 0);
      dp::Log::I( " != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end u2 AddUS(u2, u2, OFlow)                                              */


inline f8 AngleD(const f8& angle, uArcConv shift)
{
  switch (shift)
  {
    case uArcConv::RevolutionsToRadians: return angle * RAD_PER_REV;
    case uArcConv::RevolutionsToDegrees: return angle * DEG_PER_REV;
    case uArcConv::RevolutionsToGradians: return angle * GRAD_PER_REV;
    case uArcConv::RadiansToRevolutions: return angle * REV_PER_RAD;
    case uArcConv::RadiansToDegrees: return angle * DEG_PER_RAD;
    case uArcConv::RadiansToGradians: return angle * GRAD_PER_RAD;
    case uArcConv::DegreesToRevolutions: return angle * REV_PER_DEG;
    case uArcConv::DegreesToRadians: return angle * RAD_PER_DEG;
    case uArcConv::DegreesToGradians: return angle * GRAD_PER_DEG;
    case uArcConv::GradiansToRevolutions: return angle * REV_PER_GRAD;
    case uArcConv::GradiansToRadians: return angle * RAD_PER_GRAD;
    case uArcConv::GradiansToDegrees: return angle * DEG_PER_GRAD;
    default: return angle;
  }

} // end f8 AngleD(const f8&, uArcConv)                                       */

inline f4 AngleF(f4 angle, uArcConv shift)
{
  switch (shift)
  {
  case uArcConv::RevolutionsToRadians: return angle * RAD_PER_REV;
  case uArcConv::RevolutionsToDegrees: return angle * DEG_PER_REV;
  case uArcConv::RevolutionsToGradians: return angle * GRAD_PER_REV;
  case uArcConv::RadiansToRevolutions: return angle * REV_PER_RAD;
  case uArcConv::RadiansToDegrees: return angle * DEG_PER_RAD;
  case uArcConv::RadiansToGradians: return angle * GRAD_PER_RAD;
  case uArcConv::GradiansToRevolutions: return angle * REV_PER_GRAD;
  case uArcConv::GradiansToRadians: return angle * RAD_PER_GRAD;
  case uArcConv::GradiansToDegrees: return angle * DEG_PER_GRAD;
  case uArcConv::DegreesToRevolutions: return angle * REV_PER_DEG;
  case uArcConv::DegreesToRadians: return angle * RAD_PER_DEG;
  case uArcConv::DegreesToGradians: return angle * GRAD_PER_DEG;
  default: return angle;
  }

} // end f4 AngleF(f4, uArcConv)                                              */

inline s2 ExpD(const f8& value)
{
  // IEEE-754 standards of 8-byte float-point scientific notation bitfield
  constexpr s2 eBias = 0x3FF;// Unsigned -> intended value range offset (1023)
  constexpr u1 eShft = 52u; // Truncate f8's 1:11:[52] sign:exp:[mantissa]bits
  constexpr u8 eMask = 0x7FF; // 0111,1111,1111b, Isolate 11 exp from sign bit
  // Need: getF8BitsAsU8Field-> moveDnToU2AsS2-> isolateExp-> -1'sCompOffset
  return ((s2)(((*(u8*)&value) >> eShft) & eMask) - eBias);// => [-2048,2048]

} // end s2 ExpD(f4)                                                          */

inline s2 ExpF(f4 value)
{
  // IEEE-754 standards of 4-byte float-point scientific notation bitfield
  constexpr s2 eBias = 0x7E; // Unsigned -> intended value range offset (126)
  constexpr u1 eShft = 23u; // Truncate f4's 1:8:[23] sign:exp:[mantissa bits]
  constexpr u4 eMask = 0xFF; // 0000,1111,1111b, Isolate 8 exp from sign bit
  // Need: getF4BitsAsU4Field-> moveDnToU1AsS2-> isolateExp-> -1'sCompOffset
  return ((s2)(((*(u4*)&value) >> eShft) & eMask) - eBias);// => [-128,128)

} // end s2 ExpF(f4)                                                          */


inline f8 FractionalD(const f8& value)
{
  return IfOrD(value - FloorD(value), 0.0, AbsD(value) < s8_MAX);

} // end f8 FractionalD(const f8&)                                            */

inline f4 FractionalF(f4 value)
{
  return IfOrF(value - FloorF(value), 0.0f, AbsF(value) < s4_MAX);

} // end f4 FractionalF(f4)                                                   */


template <class T>
inline T IfOr(const T& givenVal, const T& otherVal, bool given)
{ return given*givenVal + !given*otherVal; }// end T IfOr<T>(c T&,c T&, bool) */
inline u1 IfOrB(u1 givenVal, u1 otherVal, bool given)
{ return given*givenVal + !given*otherVal; } // end u1 IfOrB(u1, u1, bool)    */
inline s1 IfOrC(s1 givenVal, s1 otherVal, bool given)
{ return given*givenVal + !given*otherVal; } // end s1 IfOrC(s1, s1, bool)    */
inline f8 IfOrD(const f8& givenVal, const f8& otherVal, bool given)
{ return given? givenVal : otherVal; } // end f8 IfOrD(c f8&,c f8&,bool)      */
// Branchless preferred, but NaN always inequal to NaN, even 0.0 * NaN
inline f4 IfOrF(f4 givenVal, f4 otherVal, bool given)
{ return given? givenVal : otherVal; } // end f4 IfOrF(f4, f4, bool)          */
inline s4 IfOrI(s4 givenVal, s4 otherVal, bool given)
{ return given*givenVal + !given*otherVal; } // end s4 IfOrI(s4, s4, bool)    */
inline s8 IfOrL(const s8& givenVal, const s8& otherVal, bool given)
{ return given*givenVal + !given*otherVal; }// end s8 IfOrL(c s8&,c s8&,bool) */
inline s2 IfOrS(s2 givenVal, s2 otherVal, bool given)
{ return given*givenVal + !given*otherVal; } // end s2 IfOrS(s2, s2, bool)    */
inline u4 IfOrU(u4 givenVal, u4 otherVal, bool given)
{ return given*givenVal + !given*otherVal; } // end u4 IfOrU(u4, u4, bool)    */
inline u8 IfOrUL(const u8& givenVal, const u8& otherVal, bool given)
{ return given*givenVal +!given*otherVal; }// end u8 IfOrUL(c u8&,c u8&,bool) */
inline um IfOrUM(um givenVal, um otherVal, bool given)
{ return given*givenVal + !given*otherVal; } // end um IfOrUM(um, um, bool)   */
inline u2 IfOrUS(u2 givenVal, u2 otherVal, bool given)
{ return given*givenVal + !given*otherVal; } // end u2 IfOrUS(u2, u2, bool)   */


template <class T> inline T In(const T& value, const T& max, const T& min)
{
  bool order = (min <= max);
  const T& maxima = order ? max : min, minima = order ? min : max;
  bool over = (value > maxima), under = (value < minima);
  return !(over || under) * value + under * min + over * max;

} // end <T>  In<T>(const <T>&, const <T>&, const <T>&)                       */

inline u1  InB(u1 value, u1 max, u1 min)
{
  SwapIfB(max, min, (max < min));
  bool over = (value > max), under = (value < min);
  return !(over || under) * value + under * min + over * max;

} // end u1  InB(u1, u1, u1)                                                  */

inline s1  InC(s1 value, s1 max, s1 min)
{
  SwapIfC(max, min, (max < min));
  bool over = (value > max), under = (value < min);
  return !(over || under) * value + under * min + over * max;

} // end s1  InC(s1, s1, s1)                                                  */

inline f8  InD(const f8& value, const f8& max, const f8& min)
{
  bool order = (min <= max);
  const f8& maxima = order ? max : min, minima = order ? min : max;
  bool over = (value > maxima), under = (value < minima);
  return !(over || under) * value + under * min + over * max;

} // end f8  InD(const f8&, const f8&, const f8&)                             */

inline f4  InF(f4 value, f4 max, f4 min)
{
  SwapIfF(max, min, (max < min));
  bool over = (value > max), under = (value < min);
  return !(over || under) * value + under * min + over * max;

} // end f4  InF(f4, f4, f4)                                                  */

inline s4  InI(s4 value, s4 max, s4 min)
{
  SwapIfI(max, min, (max < min));
  bool over = (value > max), under = (value < min);
  return !(over || under) * value + under * min + over * max;

} // end s4  InI(s4, s4, s4)                                                  */

inline s8  InL(const s8& value, const s8& max, const s8& min)
{
  bool order = (min <= max);
  const s8& maxima = order ? max : min, minima = order ? min : max;
  bool over = (value > maxima), under = (value < minima);
  return !(over || under) * value + under * min + over * max;

} // end s8  InL(const s8&, const s8&, const s8&)                             */

inline s2  InS(s2 value, s2 max, s2 min)
{
  SwapIfS(max, min, (max < min));
  bool over = (value > max), under = (value < min);
  return !(over || under) * value + under * min + over * max;

} // end s2  InS(s2, s2, s2)                                                  */

inline u4  InU(u4 value, u4 max, u4 min)
{
  SwapIfU(max, min, (max < min));
  bool over = (value > max), under = (value < min);
  return !(over || under) * value + under * min + over * max;

} // end u4  InU(u4, u4, u4)                                                  */

inline u8  InUL(const u8& value, const u8& max, const u8& min)
{
  bool order = (min <= max);
  const u8& maxima = order ? max : min, minima = order ? min : max;
  bool over = (value > maxima), under = (value < minima);
  return !(over || under) * value + under * min + over * max;

} // end u8  InUL(const u8&, const u8&, const u8&)                            */

inline um  InUM(um value, um max, um min)
{
  SwapIfUM(max, min, (max < min));
  bool high = (value > max), low = (value < min);
  return !(high || low) * value + low * min + high * max;

} // end um  InUM(um, um, um)                                                 */

inline u2  InUS(u2 value, u2 max, u2 min)
{
  SwapIfUS(max, min, (max < min));
  bool over = (value > max), under = (value < min);
  return !(over || under) * value + under * min + over * max;

} // end u2  InUS(u2, u2, u2)                                                 */


inline f8 InvD(const f8& value)
{
  return value != 0.0? (1.0 / value) : IfOrD(-INF, INF, IsNegD(value));
} // end f8 InvD(const f8&)                                                   */
inline f4 InvF(f4 value)
{
  return value != Of? (1.0f/value) : IfOrF(-INf, INf, IsNegF(value));
} // end f4 InvF(f4)                                                          */
inline f4 InvI(s4 value)
{ return InvF(static_cast<f4>(value)); } // end f4 InvI(s4)                   */
inline f8 InvL(const s8& value)
{ return InvD(static_cast<f8>(value)); } // end f4 InvL(const s8&)            */
inline f4 InvU(u4 value)
{ return InvF(static_cast<f4>(value)); } // end f4 InvU(u4)                   */
inline f8 InvUL(const u8& value)
{ return InvD(static_cast<f8>(value)); } // end f4 InvUL(const u8&)           */


template <class T>
inline bool IsIn(const T& value, const T& max, const T& min)
{
  bool order = (min <= max);
  const T& maxima = order? max : min, minima = order? min : max;
  return (minima <= value && value <= maxima);

} // end bool IsIn<T>(const <T>&, const <T>&, const <T>&)                     */

inline bool IsInB(u1 value, u1 max, u1 min)
{
  SwapIfB(min, max, max < min);
  return (min <= value && value <= max);

} // end bool IsInB(u1, u1, u1)                                               */

inline bool IsInC(s1 value, s1 max, s1 min)
{
  SwapIfC(min, max, max < min);
  return (min <= value && value <= max);

} // end bool IsInC(s1, s1, s1)                                               */

inline bool IsInD(const f8& value, const f8& max, const f8& min)
{
  bool order = (min <= max);
  const f8& maxima = order ? max : min, minima = order ? min : max;
  return (minima <= value && value <= maxima);

} // end bool IsInD(const f8&, const f8&, const f8&)                          */

inline bool IsInF(f4 value, f4 max, f4 min)
{
  SwapIfF(min, max, max < min);
  return (min <= value && value <= max);

} // end bool IsInF(f4, f4, f4)                                               */

inline bool IsInI(s4 value, s4 max, s4 min)
{
  SwapIfI(min, max, max < min);
  return (min <= value && value <= max);

} // end bool IsInI(s4, s4, s4)                                               */

inline bool IsInL(const s8& value, const s8& max, const s8& min)
{
  bool order = (min <= max);
  const s8& maxima = order ? max : min, minima = order ? min : max;
  return (minima <= value && value <= maxima);

} // end bool IsInL(const s8&, const s8&, const s8&)                          */

inline bool IsInS(s2 value, s2 max, s2 min)
{
  SwapIfS(min, max, max < min);
  return (min <= value && value <= max);

} // end bool IsInS(s2, s2, s2)                                               */

inline bool IsInU(u4 value, u4 max, u4 min)
{
  SwapIfU(min, max, max < min);
  return (min <= value && value <= max);

} // end bool IsInU(u4, u4, u4)                                               */

inline bool IsInUL(const u8& value, const u8& max, const u8& min)
{
  bool order = (min <= max);
  const u8& maxima = order ? max : min, minima = order ? min : max;
  return (minima <= value && value <= maxima);

} // end bool IsInUL(const u8&, const u8&, const u8&)                         */
inline bool IsInUM(um value, um max, um min)
{
  SwapIfUM(min, max, max < min);
  return (min <= value && value <= max);

} // end bool IsInUM(um, um, um)                                              */

inline bool IsInUS(u2 value, u2 max, u2 min)
{
  SwapIfUS(min, max, max < min);
  return (min <= value && value <= max);

} // end bool IsInUS(u2, u2, u2)                                              */

template <class T>
inline bool IsInS(const T& value, const T& max, const T& min)
{
  bool order = (min <= max);
  const T& maxima = order ? max : min, minima = order ? min : max;
  return (minima < value&& value < maxima);
  
} // end bool IsInS<T>(const <T>&, const <T>&, const <T>&)                    */

inline bool IsInSB(u1 value, u1 max, u1 min)
{
  SwapIfB(min, max, max < min);
  return (min < value&& value < max);

} // end bool IsInSB(u1, u1, u1)                                              */

inline bool IsInSC(s1 value, s1 max, s1 min)
{
  SwapIfC(min, max, max < min);
  return (min < value&& value < max);

} // end bool IsInSC(s1, s1, s1)                                              */

inline bool IsInSD(const f8& value, const f8& max, const f8& min)
{
  bool order = (min <= max);
  const f8& maxima = order ? max : min, minima = order ? min : max;
  return (minima < value&& value < maxima);

} // end bool IsInSD(const f8&, const f8&, const f8&)                         */

inline bool IsInSF(f4 value, f4 max, f4 min)
{
  SwapIfF(min, max, max < min);
  return (min < value&& value < max);

} // end bool IsInSF(f4, f4, f4)                                              */

inline bool IsInSI(s4 value, s4 max, s4 min)
{
  SwapIfI(min, max, max < min);
  return (min < value&& value < max);

} // end bool IsInSI(s4, s4, s4)                                              */

inline bool IsInSL(const s8& value, const s8& max, const s8& min)
{
  bool order = (min <= max);
  const s8& maxima = order ? max : min, minima = order ? min : max;
  return (minima < value&& value < maxima);

} // end bool IsInSL(const s8&, const s8&, const s8&)                         */

inline bool IsInSS(s2 value, s2 max, s2 min)
{
  SwapIfS(min, max, max < min);
  return (min < value&& value < max);

} // end bool IsInSS(s2, s2, s2)                                              */

inline bool IsInSU(u4 value, u4 max, u4 min)
{
  SwapIfU(min, max, max < min);
  return (min < value&& value < max);

} // end bool IsInSU(u4, u4, u4)                                              */

inline bool IsInSUL(const u8& value, const u8& max, const u8& min)
{
  bool order = (min <= max);
  const u8& maxima = order ? max : min, minima = order ? min : max;
  return (minima < value&& value < maxima);

} // end bool IsInSUL(const u8&, const u8&, const u8&)                        */
inline bool IsInSUM(um value, um max, um min)
{
  SwapIfUM(min, max, max < min);
  return (min < value&& value < max);


} // end bool IsInSUM(um, um, um)                                             */

inline bool IsInSUS(u2 value, u2 max, u2 min)
{
  SwapIfUS(min, max, max < min);
  return (min < value&& value < max);

} // end bool IsInSUS(u2, u2, u2)                                             */

template <class T>
inline bool IsInSA(const T& value, const T& max, const T& min)
{
  bool order = (min <= max);
  const T& maxima = order ? max : min, minima = order ? min : max;
  return (minima <= value && value < maxima);
  
} // end bool IsInSA<T>(const <T>&, const <T>&, const <T>&)                   */

inline bool IsInSAB(u1 value, u1 max, u1 min)
{
  SwapIfB(min, max, max < min);
  return (min <= value && value < max);

} // end bool IsInSAB(u1, u1, u1)                                             */

inline bool IsInSAC(s1 value, s1 max, s1 min)
{
  SwapIfC(min, max, max < min);
  return (min <= value && value < max);

} // end bool IsInSAC(s1, s1, s1)                                             */

inline bool IsInSAD(const f8& value, const f8& max, const f8& min)
{
  bool order = (min <= max);
  const f8& maxima = order ? max : min, minima = order ? min : max;
  return (minima <= value && value < maxima);

} // end bool IsInSAD(const f8&, const f8&, const f8&)                        */

inline bool IsInSAF(f4 value, f4 max, f4 min)
{
  SwapIfF(min, max, max < min);
  return (min <= value && value < max);

} // end bool IsInSAF(f4, f4, f4)                                             */

inline bool IsInSAI(s4 value, s4 max, s4 min)
{
  SwapIfI(min, max, max < min);
  return (min <= value && value < max);

} // end bool IsInSAI(s4, s4, s4)                                             */

inline bool IsInSAL(const s8& value, const s8& max, const s8& min)
{
  bool order = (min <= max);
  const s8& maxima = order ? max : min, minima = order ? min : max;
  return (minima <= value && value < maxima);

} // end bool IsInSAL(const s8&, const s8&, const s8&)                        */

inline bool IsInSAS(s2 value, s2 max, s2 min)
{
  SwapIfS(min, max, max < min);
  return (min <= value && value < max);

} // end bool IsInSAS(s2, s2, s2)                                             */

inline bool IsInSAU(u4 value, u4 max, u4 min)
{
  SwapIfU(min, max, max < min);
  return (min <= value && value < max);

} // end bool IsInSAU(u4, u4, u4)                                             */

inline bool IsInSAUL(const u8& value, const u8& max, const u8& min)
{
  bool order = (min <= max);
  const u8& maxima = order ? max : min, minima = order ? min : max;
  return (minima <= value && value < maxima);

} // end bool IsInSAUL(const u8&, const u8&, const u8&)                       */
inline bool IsInSAUM(um value, um max, um min)
{
  SwapIfUM(min, max, max < min);
  return (min <= value && value < max);

} // end bool IsInSAUM(um, um, um)                                            */

inline bool IsInSAUS(u2 value, u2 max, u2 min)
{
  SwapIfUS(min, max, max < min);
  return (min <= value && value < max);

} // end bool IsInSAUS(u2, u2, u2)                                            */

template <class T>
inline bool IsInSB(const T& value, const T& max, const T& min)
{
  bool order = (min <= max);
  const T& maxima = order ? max : min, minima = order ? min : max;
  return (minima < value&& value <= maxima);

} // end bool IsInSB<T>(const <T>&, const <T>&, const <T>&)                   */

inline bool IsInSBB(u1 value, u1 max, u1 min)
{
  SwapIfB(min, max, max < min);
  return (min < value&& value <= max);

} // end bool IsInSBB(u1, u1, u1)                                             */

inline bool IsInSBC(s1 value, s1 max, s1 min)
{
  SwapIfC(min, max, max < min);
  return (min < value&& value <= max);

} // end bool IsInSBC(s1, s1, s1)                                             */

inline bool IsInSBD(const f8& value, const f8& max, const f8& min)
{
  bool order = (min <= max);
  const f8& maxima = order ? max : min, minima = order ? min : max;
  return (minima < value&& value <= maxima);

} // end bool IsInSBD(const f8&, const f8&, const f8&)                        */

inline bool IsInSBF(f4 value, f4 max, f4 min)
{
  SwapIfF(min, max, max < min);
  return (min < value&& value <= max);

} // end bool IsInSBF(f4, f4, f4)                                             */

inline bool IsInSBI(s4 value, s4 max, s4 min)
{
  SwapIfI(min, max, max < min);
  return (min < value&& value <= max);

} // end bool IsInSBI(s4, s4, s4)                                             */

inline bool IsInSBL(const s8& value, const s8& max, const s8& min)
{
  bool order = (min <= max);
  const s8& maxima = order ? max : min, minima = order ? min : max;
  return (minima < value&& value <= maxima);

} // end bool IsInSBL(const s8&, const s8&, const s8&)                        */

inline bool IsInSBS(s2 value, s2 max, s2 min)
{
  SwapIfS(min, max, max < min);
  return (min < value&& value <= max);

} // end bool IsInSBS(s2, s2, s2)                                             */

inline bool IsInSBU(u4 value, u4 max, u4 min)
{
  SwapIfU(min, max, max < min);
  return (min < value&& value <= max);

} // end bool IsInSBU(u4, u4, u4)                                             */

inline bool IsInSBUL(const u8& value, const u8& max, const u8& min)
{
  bool order = (min <= max);
  const u8& maxima = order ? max : min, minima = order ? min : max;
  return (minima < value&& value <= maxima);

} // end bool IsInSBUL(const u8&, const u8&, const u8&)                       */
inline bool IsInSBUM(um value, um max, um min)
{
  SwapIfUM(min, max, max < min);
  return (min < value&& value <= max);

} // end bool IsInSBUM(um, um, um)                                            */

inline bool IsInSBUS(u2 value, u2 max, u2 min)
{
  SwapIfUS(min, max, max < min);
  return (min < value&& value <= max);

} // end bool IsInSBUS(u2, u2, u2)                                            */


inline bool IsIntegralD(const f8& value, const f8& within)
{
  return NearD((f8)FloorD(value), value, within);
} // end bool IsIntegralD(const f8&, const f8&)                               */
inline bool IsIntegralF(f4 value, f4 within)
{
  return NearD((f8)FloorD(value), value, within);
} // end bool IsIntegralF(f4, f4)                                             */


inline bool IsIntD(const f8& value)
{
  constexpr u1 mBits = 53u; // Maximum bits contained in the f8 mantissa
  s2 fBits = mBits - ExpD(value); // Floating point bits in mantissa by scale
  u8 bVal = (*(u8*)&value); // Float-point bits read as an unsigned / bitfield
  return (fBits <= 0 || bVal == ((bVal >> fBits) << fBits));

} // end bool IsIntD(const f8&)                                               */
inline bool IsIntF(f4 value)
{
  constexpr u1 mBits = 24u; // Maximum bits contained in the f4 mantissa
  s2 fBits = mBits - ExpF(value); // Floating point bits in mantissa by scale
  u4 bVal = (*(u4*)&value); // Float-point bits read as an unsigned / bitfield
  return (fBits <= 0 || bVal == ((bVal >> fBits) << fBits));

} // end bool IsIntF(f4)                                                      */


inline bool IsNegC(s1 value)
{
  constexpr s4 MSB = 1 << 7; // only s1 Most Significant Bit position on (1)
  return value & MSB; // 100...b & value == 100...b iff value < 0; 0 otherwise
} // end bool IsNegC(s1)                                                      */
inline bool IsNegD(const f8& value)
{
  constexpr s8 MSB = 1ll << 63; // only f8 Most Significant Bit position on (1)
  return *(s8*)&value & MSB;
} // end bool IsNegD(f8)                                                      */
inline bool IsNegF(f4 value)
{
  constexpr s4 MSB = 1 << 31; // only f4 Most Significant Bit position on (1)
  return *(s4*)&value & MSB;
} // end bool IsNegF(f4)                                                      */
inline bool IsNegI(s4 value)
{
  constexpr s4 MSB = 1 << 31; // only s4 Most Significant Bit position on (1)
  return value & MSB; // 100...b & value == 100...b iff value < 0; 0 otherwise
} // end bool IsNegI(s4)                                                      */
inline bool IsNegL(const s8& value)
{
  constexpr s8 MSB = 1ll << 63; // only s8 Most Significant Bit position on (1)
  return value & MSB;
} // end bool IsNegL(s8)                                                      */
inline bool IsNegS(s2 value)
{
  constexpr s4 MSB = 1 << 15; // only s2 Most Significant Bit position on (1)
  return value & MSB; // 100...b & value == 100...b iff value < 0; 0 otherwise
} // end bool IsNegS(s2)                                                      */


inline bool IsRealD(const f8& value)
{
  return !(std::isnan(value) || std::isinf(value));
} // end bool IsRealD(const f8&)                                              */
inline bool IsRealF(f4 value)
{ return !(std::isnan(value) || std::isinf(value)); } // end bool IsRealF(f4) */


inline u1 LerpB(u1 initial, u1 terminal, f4 mix)
{
  return initial + RoundF(mix * static_cast<f4>(terminal - initial));
} // end u1 LerpB(u1, u1, f4)                                                 */
inline s1 LerpC(s1 initial, s1 terminal, f4 mix)
{
  return initial + RoundF(mix * static_cast<f4>(terminal - initial));
} // end s1 LerpC(s1, s1, f4)                                                 */
inline f8 LerpD(const f8& initial, const f8& terminal, const f8& mix)
{
  return initial + (mix * (terminal - initial));
} // end f8 LerpD(const f8&, const f8&, const f8&)                            */
inline f4 LerpF(f4 initial, f4 terminal, f4 mix)
{
  return initial + (mix * (terminal - initial));
} // end f4 LerpF(f4, f4, f4)                                                 */
inline s4 LerpI(s4 initial, s4 terminal, f4 mix)
{
  return initial + RoundF(mix * static_cast<f4>(terminal - initial));
} // end s4 LerpI(s4, s4, f4)                                                 */
inline s8 LerpL(const s8& initial, const s8& terminal, const f8& mix)
{
  return initial + RoundD(mix * static_cast<f8>(terminal - initial));
} // end s8 LerpL(const s8&, const s8&, const s8&)                            */
inline s2 LerpS(s2 initial, s2 terminal, f4 mix)
{
  return initial + RoundF(mix * static_cast<f4>(terminal - initial));
} // end s2 LerpS(s2, s2, f4)                                                 */
inline u4 LerpU(u4 initial, u4 terminal, f4 mix)
{
  return initial + RoundF(mix * static_cast<f4>(terminal - initial));
} // end u4 LerpU(u4, u4, f4)                                                 */
inline u8 LerpUL(const u8& initial, const u8& terminal, const f8& mix)
{
  return initial + RoundD(mix * static_cast<f8>(terminal - initial));
} // end u8 LerpL(const u8&, const u8&, const u8&)                            */
inline um LerpUM(um initial, um terminal, f4 mix)
{
  return initial +
#ifdef ENV_64
    RoundD(mix * static_cast<f8>(terminal - initial));
#else
    RoundF(mix * static_cast<f4>(terminal - initial));
#endif
} // end um LerpUM(um, um, f4)                                                */
inline u2 LerpUS(u2 initial, u2 terminal, f4 mix)
{
  return initial + RoundF(mix * static_cast<f4>(terminal - initial));
} // end u2 LerpUS(u2, u2, f4)                                                */


inline u2 Log2TruncB(u1 value)
{
  for (u2 i = 7; i > 0; --i) 
  {
    if (value & (1ull<<i)) { return i; }
  }
  return 0;

} // end u2 Log2TruncB(u1)                                                    */
inline u2 Log2TruncC(s1 value)
{
  SetAbsC(value);
  for(u2 i = 7; i > 0; --i)
  {
    if (value & (1ull << i)) { return i; }
  }
  return 0;

} // end u2 Log2TruncC(s1)                                                    */
inline u2 Log2TruncD(const f8& value)
{
  s8 aMag = static_cast<s8>(AbsD(value));
  for(u2 i = 63; i > 0; --i)
  {
    if (aMag & (1ull << i)) { return i; }
  }
  return 0;

} // end u2 Log2TruncD(const f8&)                                             */
inline u2 Log2TruncF(f4 value)
{
  s4 aMag = static_cast<s4>(AbsF(value));
  for(u2 i = 31; i > 0; --i)
  {
    if (aMag & (1ull << i)) { return i; }
  }
  return 0;

} // end u2 Log2TruncF(f4)                                                    */
inline u2 Log2TruncI(s4 value)
{
  SetAbsI(value);
  for(u2 i = 31; i > 0; --i)
  {
    if (value & (1ull << i)) { return i; }
  }
  return 0;

} // end u2 Log2TruncI(s4)                                                    */
inline u2 Log2TruncL(const s8& value)
{
  s8 mag = AbsL(value);
  for(u2 i = 63; i > 0; --i)
  {
    if (mag & (1ull << i)) {return i; }
  }
  return 0;

} // end u2 Log2TruncL(const s8&)                                             */
inline u2 Log2TruncS(s2 value)
{
  SetAbsS(value);
  for(u2 i = 15; i > 0; --i)
  {
    if (value & (1ull << i)) { return i; }
  }
  return 0;

} // end u2 Log2TruncI(s2)                                                    */
inline u2 Log2TruncU(u4 value)
{
  for(u2 i = 31; i > 0; --i)
  {
    if (value & (1ull << i)) { return i; }
  }
  return 0;

} // end u2 Log2TruncU(u4)                                                    */
inline u2 Log2TruncUL(const u8& value)
{
  for(u2 i = 63; i > 0; --i)
  {
    if (value & (1ull << i)) { return i; }
  }
  return 0;

} // end u2 Log2TruncUL(const u8&)                                            */
inline u2 Log2TruncUM(um value)
{
  constexpr u2 MSB = (sizeof(um) * BYTEBITS) - 1;// top bit position [0, 31||63]
  for(u2 i = MSB; i > 0; --i)
  {
    if (value & (1ull << i)) { return i; }
  }
  return 0;

} // end u2 Log2TruncUM(um)                                                   */
inline u2 Log2TruncUS(u2 value)
{
  for(u2 i = 15; i > 0; --i)
  {
    if (value & (1ull << i)) { return i; }
  }
  return 0;

} // end u2 Log2TruncUS(u2)                                                   */


template <class T> inline T Max(std::initializer_list<T> list)
{
  if (list.size() < 1) { return 0; }
  T result = list.begin()[0];
  for (T i : list) { result = IfOr<T>(i, result, i > result); }
  return result;

} // end <T> Max<T>(std::initializer_list<T>)                                 */
template <class T> inline T Max(const std::vector<T>& list)
{
  if (list.size() < 1) { return 0; }
  f8 result = list[0];
  for (f8 i : list) { result = IfOr<T>(result, i, result > i); }
  return result;

} // end <T> Max<T>(const std::vector<T>&)                                    */
inline u1 MaxB(u1 a, u1 b)
{ return IfOrB(a, b, b < a); } // end u1 MaxB(u1, u1)                         */
inline s1 MaxC(s1 a, s1 b)
{ return IfOrC(a, b, b < a); } // end s1 MaxC(s1, s1)                         */
inline f8 MaxD(const f8& a, const f8& b)
{ return IfOrD(a, b, b < a); } // end f8 MaxD(const f8&, const f8&)           */
inline f4 MaxF(f4 a, f4 b)
{ return IfOrF(a, b, b < a); } // end f4 MaxF(f4, f4)                         */
inline s4 MaxI(s4 a, s4 b)
{ return IfOrI(a, b, b < a); } // end s4 MaxI(s4, s4)                         */
inline s8 MaxL(const s8& a, const s8& b)
{ return IfOrL(a, b, b < a); } // end s8 MaxL(const s8&, const s8&)           */
inline s2 MaxS(s2 a, s2 b)
{ return IfOrS(a, b, b < a); } // end s2 MaxI(s2, s2)                         */
inline u4 MaxU(u4 a, u4 b)
{ return IfOrU(a, b, b < a); } // end u4 MaxU(u4, u4)                         */
inline u8 MaxUL(const u8& a, const u8& b)
{ return IfOrUL(a, b, b < a); } // end u8 MaxUL(const u8&, const u8&)         */
inline um MaxUM(um a, um b)
{ return IfOrUM(a, b, b < a); } // end um MaxUM(um, um)                       */
inline u2 MaxUS(u2 a, u2 b)
{ return IfOrUS(a, b, b < a); } // end u2 MaxUS(u2, u2)                       */

inline u1 MaxB3F(f4 x, f4 y, f4 z)
{return IfOrU(IfOrU(2,0,x<z),IfOrU(1,0,x<y),y<z);}// end u4 Max3U(f4, f4, f4) */

template <class T> inline T Min(const std::vector<T>& list)
{
  if (list.size() < 1) { return 0; }
  T result = list[0];
  for (T i : list) { result = IfOr<T>(i, result, i < result); }
  return result;

} // end <T> Min<T>(const std::vector<T>&)                                    */
template <class T> inline T Min(std::initializer_list<T> list)
{
  if (list.size() < 1) { return 0; }
  T result = list.begin()[0];
  for (T i : list) { result = IfOr<T>(i, result, i < result); }
  return result;

} // end <T> Min<T>(std::initializer_list<T>)                                 */
inline u1 MinB(u1 a, u1 b)
{ return IfOrB(a, b, a < b); } // end u1 MinB(u1, u1)                         */
inline s1 MinC(s1 a, s1 b)
{ return IfOrC(a, b, a < b); } // end s1 MinC(s1, s1)                         */
inline f8 MinD(const f8& a, const f8& b)
{ return (a < b) ? a : b; } // end f8 MinD(const f8&, const f8&)              */
inline f4 MinF(f4 a, f4 b)
{ return IfOrF(a, b, a < b); } // end f4 MinF(f4, f4)                         */
inline s4 MinI(s4 a, s4 b)
{ return IfOrI(a, b, a < b); } // end s4 MinI(s4, s4)                         */
inline s8 MinL(const s8& a, const s8& b)
{ return IfOrL(a, b, a < b); } // end s8 MinL(const s8&, const s8&)           */
inline s2 MinS(s2 a, s2 b)
{ return IfOrS(a, b, a < b); } // end s2 MinS(s2, s2)                         */
inline u4 MinU(u4 a, u4 b)
{ return IfOrU(a, b, a < b); } // end u4 MinU(u4, u4)                         */
inline u8 MinUL(const u8& a, const u8& b)
{ return IfOrUL(a, b, a < b); } // end u8 MinUL(const u8&, const u8&)         */
inline um MinUM(um a, um b)
{ return IfOrUM(a, b, a < b); } // end um MinUM(um, um)                       */
inline u2 MinUS(u2 a, u2 b)
{ return IfOrUS(a, b, a < b); } // end u2 MinUS(u2, u2)                       */


inline u1 ModB(u1 val, u1 max, u1 min)
{
  if (IsInSAB(val, max, min)) { return val; } // avoid (/)
  return ((val - min) % max) + min;
} // end u1 ModB(u1, u1, u1)                                                  */
inline s1 ModC(s1 val, s1 max, s1 min)
{
  if (IsInSAC(val, max, min)) { return val; } // avoid (/)
  return ((val - min) % max) + min;
} // end s1 ModC(s1, s1, s1)                                                  */
inline f8 ModD(const f8& val, const f8& max, const f8& min)
{
  if (IsInD(val, max, min)) { return val; }
  // No number exists if either extrema is non-numerical
  if (std::isnan(max) || std::isnan(min)) { return NaN; }
  // Confirm inputs are ordered to ascending size
  bool order = (min <= max);
  const f8& maxima = order ? max : min, minima = order ? min : max;
  // Can only place new values into finite ranges
  if (std::isinf(maxima))
  {
    return (std::isinf(minima) || minima < val) ? RealD(val) :
      (IsNegD(minima) && minima == 0.0) ? 0.0 : minima;
  }
  else if (std::isinf(minima))
  {
    return maxima > val ? RealD(val) :
      (IsNegD(maxima) && maxima == 0.0) ? 0.0 : maxima;
  }
  f8 range = maxima - minima; // range of remapping: min to max distance
  // Negative ranges precluded by swap; null range returns either boundary
  if (range <= 0.0) { return minima; }
  if (!IsRealD(val)) { return minima; } // can only map real numbers

  f8 ratio = InvD(range); // ratio of total range: cycles =: dist / range
  f8 underBy = minima - val; // local subset (current to min distance)
  f8 overBy = val - maxima; // local subset (max to current distance)
  underBy = (underBy > 0.0) ? CeilD(underBy * ratio) : 0.0;
  overBy = (overBy > 0.0) ? CeilD(overBy * ratio) : 0.0;
  return val + ((underBy - overBy) * range);

} // end f4 ModD(const f8&, const f8&, const f8&)                             */
inline f4 ModF(f4 current, f4 max, f4 min)
{
  if (IsInF(current, max, min)) { return current; }
  // No number exists if either extrema is non-numerical
  if (std::isnan(max) || std::isnan(min)) { return NaNf; }
  // Confirm inputs are ordered to ascending size
  SwapIfF(min, max, (max < min));
  // Can only place new values into finite ranges
  if (std::isinf(max))
  {
    return (IsNegD(min) || min < current) ? RealF(current) :
      (IsNegD(min) && min == 0.0f) ? 0.0f : min;
  }
  else if (std::isinf(min))
  {
    return max > current ? RealF(current) :
      (IsNegD(max) && max == 0.0f) ? 0.0f : max;
  }
  f4 range = max - min; // overall range of remapping, min to max distance
  // Negative ranges precluded by swap; null range returns either boundary
  if (range <= 0.0f) { return min; }
  if (!IsRealF(current)) { return min; } // can only map real numbers
  f4 ratio = 1.0f / range; // ratio of total range: cycles =: dist / range
  f4 underBy = min - current; // local subset (current to min distance)
  f4 overBy = current - max; // local subset (max to current distance)
  underBy = (underBy > 0.0f) ? CeilF(underBy * ratio) : 0.0f;
  overBy = (overBy > 0.0f) ? CeilF(overBy * ratio) : 0.0f;
  return current + ((underBy - overBy) * range);

} // end f4 ModF(f4, f4, f4)                                                  */
inline s4 ModI(s4 val, s4 max, s4 min)
{
  if (IsInSAI(val, max, min)) { return val; } // avoid (/)
  return ((val - min) % max) + min;
} // end s4 ModI(s4, s4, s4)                                                  */

inline s8 ModL(const s8& val, const s8& max, const s8& min)
{
  if (IsInSAL(val, max, min)) { return val; } // avoid (/)
  return ((val - min) % max) + min;
} // end s8 ModL(const s8&, const s8&, const s8&)                             */

inline s2 ModS(s2 val, s2 max, s2 min)
{
  if (IsInSAS(val, max, min)) { return val; } // avoid (/)
  return ((val - min) % max) + min;
} // end s2 ModS(s2, s2, s2)                                                  */

inline u4 ModU(u4 val, u4 max, u4 min)
{
  if (IsInSAU(val, max, min)) { return val; } // avoid (/)
  return ((val - min) % max) + min;
} // end u4 ModU(u4, u4, u4)                                                  */

inline u8 ModUL(const u8& val, const u8& max, const u8& min)
{
  if (IsInSAUL(val, max, min)) { return val; } // avoid (/)
  return ((val - min) % max) + min;
} // end u8 ModUL(const u8&, const u8&, const u8&)                            */

inline um ModUM(um val, um max, um min)
{
  if (IsInSAUM(val, max, min)) { return val; } // avoid (/)
  return ((val - min) % max) + min;
} // end um ModUM(um, um, um)                                                 */

inline u2 ModUS(u2 val, u2 max, u2 min)
{
  if (IsInSAUS(val, max, min)) { return val; } // avoid (/)
  return ((val - min) % max) + min;
} // end u2 ModUS(u2, u2, u2)                                                 */


inline bool NearD(const f8& value, const f8& target, const f8& within)
{
  return IsInD(value, target + within, target - within);
}// end bool NearD(const f8&, const f8&, const f8&)                           */
inline bool NearF(const f4& value, const f4& target, const f4& within)
{
  return IsInF(value, target + within, target - within);
}// end bool NearF(const f4&, const f4&, const f4&)                           */


inline f8 RealD(const f8& value)
{
  return (std::isnan(value) || std::isinf(value)) ? 0.0 : value;

} // end f8 RealD(const f8&)                                                  */
inline f4 RealF(f4 value)
{
  return (std::isnan(value) || std::isinf(value)) ? 0.0f : value;

} // end f4 RealF(f4)                                                         */


/** Get (and/or set) the overflow sentinel value for 1-byte signed data
\brief
  - get (and/or set) the overflow sentinel value for 1-byte signed data
\details
  - inline will be ignored for a function with static values, just the pattern
\param sentinelValue
  - pointer to value, which, if provided, becomes overflow value sentinel value
\return
  - current (or newly set), value declared as the f4 sentinel error value
*/
/*inline s1 SentinelC(s1* sentinelValue)
{
  static s1 sentinelC = s1_MIN;
  if (sentinelValue) { sentinelC = *sentinelValue; }
  return sentinelC;
} // end s1 SentinelC(s1*)                                                    */


/** Get (and/or set) the overflow sentinel value for 8-byte float point data
\brief
  - get (and/or set) the overflow sentinel value for 8-byte float point data
\details
  - inline will be ignored for a function with static values, just the pattern
\param sentinelValue
  - pointer to 8-byte float, which, if provided, sets overflow sentinel value
\return
  - current (or newly set), value declared as the f8 sentinel error value
*/
/*inline f8 SentinelD(f8* sentinelValue)
{
  static f8 sentinelD = NaN;
  if (sentinelValue) { sentinelD = *sentinelValue; }
  return sentinelD;
} // end f8 SentinelD(f8*)                                                    */


/** Get (and/or set) the overflow sentinel value for 4-byte float-point data
\brief
  - get (and/or set) the overflow sentinel value for 4-byte float-point data
\details
  - inline will be ignored for a function with static values, just the pattern
\param sentinelValue
  - pointer to value, which, if provided, becomes overflow value sentinel value
\return
  - current (or newly set), value declared as the f4 sentinel error value
*/
/*inline f4 SentinelF(f4* sentinelValue)
{
  static f4 sentinelF = NaNf;
  return SetIfF(sentinelF, *sentinelValue, sentinelValue);
} // end f8 SentinelF(f4*)                                                    */


inline s1& SetAbsC(s1& value)
{ return SetIfC(value, -value, IsNegC(value)); } // end s1& SetAbsC(s1&)      */
inline f8& SetAbsD(f8& value)
{ return SetIfD(value, -value, IsNegD(value)); } // end f8& SetAbsD(f8&)      */
inline f4& SetAbsF(f4& value)
{ return SetIfF(value, -value, IsNegF(value)); } // end s4& SetAbsF(f4&)      */
inline s4& SetAbsI(s4& value)
{ return SetIfI(value, -value, IsNegI(value)); } // end s4& SetAbsI(s4&)      */
inline s2& SetAbsS(s2& value)
{ return SetIfS(value, -value, IsNegS(value)); } // end s2& SetAbsS(s2&)      */
inline s8& SetAbsL(s8& value)
{ return SetIfL(value, -value, IsNegL(value)); } // end s8& SetAbsL(s8&)      */


inline bool SetAddB(u1& current, u1 plus, OFlow policy)
{
  return SetUpB(current, (current + plus), policy);
} // end bool SetAddB(u1&, u1, OFlow)                                         */
inline bool SetAddC(s1& current, s1 plus, OFlow policy)
{
  return SetUpC(current, (current + plus), policy);
} // end bool SetAddC(s1&, s1, OFlow)                                         */
inline bool SetAddD(f8& current, const f8& plus, OFlow policy)
{
  f8 sum = current + plus;
  return IsNegD(plus)? SetUpD(current,sum,policy) : SetDnD(current,sum,policy);
} // end bool SetAddD(f8&, const f8&, OFlow)                                  */
inline bool SetAddF(f4& current, f4 plus, OFlow policy)
{
  return SetUpF(current, (current + plus), policy);
} // end bool SetAddF(f4&, f4, OFlow)                                         */
inline bool SetAddI(s4& current, s4 plus, OFlow policy)
{
  return SetUpI(current, (current + plus), policy);
} // end bool SetAddI(s4&, s4, OFlow)                                         */
inline bool SetAddL(s8& current, const s8& plus, OFlow policy)
{
  s8 sum = current + plus;
  return IsNegL(plus)? SetUpL(current,sum,policy) : SetDnL(current,sum,policy);
} // end bool SetAddL(s8&, const s8&, OFlow)                                  */
inline bool SetAddS(s2& current, s2 plus, OFlow policy)
{
  return SetUpS(current, (current + plus), policy);
} // end bool SetAddS(s2&, s2, OFlow)                                         */
inline bool SetAddU(u4& current, u4 plus, OFlow policy)
{
  return SetUpU(current, (current + plus), policy);
} // end bool SetAddU(u4&, u4, OFlow)                                         */
inline bool SetAddUL(u8& current, const u8& plus, OFlow policy)
{
  return SetUpUL(current, (current + plus), policy);
} // end bool SetAddUL(u8&, const u8&, OFlow)                                 */
inline bool SetAddUM(um& current, um plus, OFlow policy)
{
  return SetUpUM(current, (current + plus), policy);
} // end bool SetAddUM(um&, um, OFlow)                                        */
inline bool SetAddUS(u2& current, u2 plus, OFlow policy)
{
  return SetUpUS(current, (current + plus), policy);
} // end bool SetAddUS(u2&, u2, OFlow)                                        */



template <class T> inline T& SetIn(T& value, const T& max, const T& min)
{
  return (value =  In<T>(value, max, min));
} // end <T>& SetIn<T>(<T>&, const <T>&, const <T>&)                       */
inline u1& SetInB(u1& value, u1 max, u1 min)
{
  return (value =  InB(value, max, min));
} // end u1& SetInB(u1&, u1, u1)                                              */
inline s1& SetInC(s1& value, s1 max, s1 min)
{
  return (value =  InC(value, max, min));
} // end s1& SetInC(s1&, s1, s1)                                              */
inline f8& SetInD(f8& value, const f8& max, const f8& min)
{
  return (value =  InD(value, max, min));
} // end f8& SetInD(f8&, const f8&, const f8&)                                */
inline f4& SetInF(f4& value, f4 max, f4 min)
{
  return (value =  InF(value, max, min));
} // end f4& SetInF(f4&, f4, f4)                                              */
inline s4& SetInI(s4& value, s4 max, s4 min)
{
  return (value =  InI(value, max, min));
} // end s4& SetInI(s4&, s4, s4)                                              */
inline s8& SetInL(s8& value, const s8& max, const s8& min)
{
  return (value =  InL(value, max, min));
} // end s8& SetInL(s8&, const s8&, const s8&)                                */
inline s2& SetInS(s2& value, s2 max, s2 min)
{
  return (value =  InS(value, max, min));
} // end s2& SetInS(s2&, s2, s2)                                              */
inline u4& SetInU(u4& value, u4 max, u4 min)
{
  return (value =  InU(value, max, min));
} // end u4& SetInU(u4&, u4, u4)                                              */
inline u8& SetInUL(u8& value, const u8& max, const u8& min)
{
  return (value =  InUL(value, max, min));
} // end u8& SetInUL(u8&, const u8&, const u8&)                               */
inline um& SetInUM(um& value, um max, um min)
{
  return (value =  InUM(value, max, min));
} // end um& SetInUM(um&, um, um)                                             */
inline u2& SetInUS(u2& value, u2 max, u2 min)
{
  return (value =  InUS(value, max, min));
} // end u2& SetInUS(u2&, u2, u2)                                             */


inline bool SetDnB(u1& val, u1 result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("u1 underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val = 0u; break;  // Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0u; break;  // Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = u1_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnB(u1&, u1, OFlow)                                          */
inline bool SetDnC(s1& val, s1 result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("s1 underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val = 0; break;   // Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0; break;   // Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = s1_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnC(s1&, s1, OFlow)                                          */
inline bool SetDnD(f8& val, const f8& result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("f8 underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val = -INF; break;// Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0.0; break; // Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = f8_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnD(f8&, const f8&, OFlow)                                   */
inline bool SetDnF(f4& val, f4 result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("f4 underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val = -INf; break;// Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0.0f; break;// Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = f4_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnF(f4&, f4, OFlow)                                          */
inline bool SetDnI(s4& val, s4 result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("s4 underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val=s4_MIN; break;// Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0; break;   // Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = s4_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnI(s4&, s4, OFlow)                                          */
inline bool SetDnL(s8& val, const s8& result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("s8 underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val=s8_MIN; break;// Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0; break;   // Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = s8_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnL(s8&, const s8&, OFlow)                                   */
inline bool SetDnS(s2& val, s2 result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("s2 underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val=s2_MIN; break;// Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0; break;   // Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = s2_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnS(s2&, s2, OFlow)                                          */
inline bool SetDnU(u4& val, u4 result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("u4 underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val = 0u; break;  // Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0u; break;  // Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = u4_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnU(u4&, u4, OFlow)                                          */
inline bool SetDnUL(u8& val, const u8& result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("u8 underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val = 0u; break;  // Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0u; break;  // Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = u8_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnUL(u8&, const u8&, OFlow)                                  */
inline bool SetDnUM(um& val, um result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("um underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val = 0u; break;  // Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0u; break;  // Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = um_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnUM(um&, um, OFlow)                                         */
inline bool SetDnUS(u2& val, u2 result, OFlow policy)
{
  if (result <= val) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("u2 underflow"); }
  switch (policy) // else underflow exception occurred
  {
  case OFlow::Wrap:       case OFlow::KeepUp:         case OFlow::ClampUp:
  case OFlow::NullUp:     case OFlow::SetUp: default: // underflow vWrap cases
    val = result;     // Set underflow to wrapped remainder (typical underflow)
  case OFlow::Keep:       case OFlow::ClampUpKeepDn:  case OFlow::NullUpKeepDn:
  case OFlow::KeepDn:     case OFlow::SetUpKeepDn:    // underflow vKeep cases
    break;            // Set underflow value as input given: ignore attempt
  case OFlow::Clamp:      case OFlow::KeepUpClampDn:  case OFlow::NullUpClampDn:
  case OFlow::ClampDn:    case OFlow::SetUpClampDn:   // underflow vClamp cases
    val = 0u; break;  // Set underflow as clamped to typed minima value
  case OFlow::Null:       case OFlow::KeepUpNullDn:   case OFlow::ClampUpNullDn:
  case OFlow::NullDn:     case OFlow::SetUpNullDn:    // underflow vNull cases
    val = 0u; break;  // Set underflow to neutral / null value
  case OFlow::Set:        case OFlow::KeepUpSetDn:    case OFlow::ClampUpSetDn:
  case OFlow::SetDn:      case OFlow::NullUpSetDn:    // underflow vSet cases
    val = u2_INVALID; // Set underflow to externally defined sentinel
  }
  return false;

} // end bool SetDnUS(u2&, u2, OFlow)                                         */

template <class T> inline T& SetIf(T& set, const T& to, bool given)
{
  return set = IfOr<T>(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end T& SetIf<T>(T&, const T&, bool)                                      */
inline u1& SetIfB(u1& set, u1 to, bool given)
{
  return set = IfOrB(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end u1& SetIfB(u1&, u1, bool)                                            */
inline s1& SetIfC(s1& set, s1 to, bool given)
{
  return set = IfOrC(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end s1& SetIfC(s1&, s1, bool)                                            */
inline f8& SetIfD(f8& set, const f8& to, bool given)
{
  return set = IfOrD(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end f8& SetIfD(f8&, const f8&, bool)                                     */
inline f4& SetIfF(f4& set, f4 to, bool given)
{
  return set = IfOrF(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end f4& SetIfF(f4&, f4, bool)                                            */
inline s4& SetIfI(s4& set, s4 to, bool given)
{
  return set = IfOrI(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end s4& SetIfI(s4&, s4, bool)                                            */
inline s8& SetIfL(s8& set, const s8& to, bool given)
{
  return set = IfOrL(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end s8& SetIfL(s8&, const s8&, bool)                                     */
inline s2& SetIfS(s2& set, s2 to, bool given)
{
  return set = IfOrS(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end s2& SetIfS(s2&, s2, bool)                                            */
inline u4& SetIfU(u4& set, u4 to, bool given)
{
  return set = IfOrU(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end u4& SetIfU(u4&, u4, bool)                                            */
inline u8& SetIfUL(u8& set, const u8& to, bool given)
{
  return set = IfOrUL(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end u8& SetIfUL(u8&, const u8&, bool)                                    */
inline um& SetIfUM(um& set, um to, bool given)
{
  return set = IfOrUM(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end um& SetIfUM(um&, um, bool)                                            */
inline u2& SetIfUS(u2& set, u2 to, bool given)
{
  return set = IfOrUS(to, set, given); // given? 1*to + 0*set : 0*to + 1*set
} // end u2& SetIfUS(u2&, u2, bool)                                            */


template<class T> inline bool SetIfP(T* set, const T& to, bool given)
{
  if (!set) { return false; } SetIf<T>(*set, to, given); return true;
} // end bool SetIfP<T>(T*, const T&, bool)                                   */
inline bool SetIfPB(u1* set, u1 to, bool given)
{
  if (!set) { return false; } SetIfB(*set, to, given); return true;
} // end bool SetIfPB(u1*, u1, bool)                                          */
inline bool SetIfPC(s1* set, s1 to, bool given)
{
  if (!set) { return false; } SetIfC(*set, to, given); return true;
} // end bool SetIfPC(s1*, s1, bool)                                          */
inline bool SetIfPD(f8* set, const f8& to, bool given)
{
  if (!set) { return false; } SetIfD(*set, to, given); return true;
} // end bool SetIfPD(f8*, const f8&, bool)                                   */
inline bool SetIfPF(f4* set, f4 to, bool given)
{
  if (!set) { return false; } SetIfF(*set, to, given); return true;
} // end bool SetIfPF(f4*, f4, bool)                                          */
inline bool SetIfPI(s4* set, s4 to, bool given)
{
  if (!set) { return false; } SetIfI(*set, to, given); return true;
} // end bool SetIfPI(s4*, s4, bool)                                          */
inline bool SetIfPL(s8* set, const s8& to, bool given)
{
  if (!set) { return false; } SetIfL(*set, to, given); return true;
} // end bool SetIfPL(s8*, const s8&, bool)                                   */
inline bool SetIfPS(s2* set, s2 to, bool given)
{
  if (!set) { return false; } SetIfS(*set, to, given); return true;
} // end bool SetIfPS(s2*, s2, bool)                                          */
inline bool SetIfPU(u4* set, u4 to, bool given)
{
  if (!set) { return false; } SetIfU(*set, to, given); return true;
} // end bool SetIfPU(u4*, u4, bool)                                          */
inline bool SetIfPUL(u8* set, const u8& to, bool given)
{
  if (!set) { return false; } SetIfUL(*set, to, given); return true;
} // end bool SetIfPUL(u8*, const u8&, bool)                                  */
inline bool SetIfPUM(um* set, um to, bool given)
{
  if (!set) { return false; } SetIfUM(*set, to, given); return true;
} // end bool SetIfPUM(um*, um, bool)                                         */
inline bool SetIfPUS(u2* set, u2 to, bool given)
{
  if (!set) { return false; } SetIfUS(*set, to, given); return true;
} // end bool SetIfPUS(u2*, u2, bool)                                         */


template <class T>
inline T& SetIfOr(T& set, const T& to, bool given, const T& orTo)
{
  return set = IfOr<T>(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end T& SetIfOr<T>(T&,const T&,bool,const T&)                             */
inline u1& SetIfOrB(u1& set, u1 to, bool given, u1 orTo)
{
  return set = IfOrB(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end u1& SetIfOrB(u1&, u1, bool, u1)                                      */
inline s1& SetIfOrC(s1& set, s1 to, bool given, s1 orTo)
{
  return set = IfOrC(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end s1& SetIfOrC(s1&, s1, bool, s1)                                      */
inline f8& SetIfOrD(f8& set, const f8& to, bool given, const f8& orTo)
{
  return set = IfOrD(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end f8& SetIfOrD(f8&, const f8&, bool, const f8&)                        */
inline f4& SetIfOrF(f4& set, f4 to, bool given, f4 orTo)
{
  return set = IfOrF(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end f4& SetIfOrF(f4&, f4, bool, f4)                                      */
inline s4& SetIfOrI(s4& set, s4 to, bool given, s4 orTo)
{
  return set = IfOrI(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end s4& SetIfOrI(s4&, s4, bool, s4)                                      */
inline s8& SetIfOrL(s8& set, const s8& to, bool given, const s8& orTo)
{
  return set = IfOrL(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end s8& SetIfOrL(s8&, const s8&, bool, const s8&)                        */
inline s2& SetIfOrS(s2& set, s2 to, bool given, s2 orTo)
{
  return set = IfOrS(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end s2& SetIfOrS(s2&, s2, bool, s2)                                      */
inline u4& SetIfOrU(u4& set, u4 to, bool given, u4 orTo)
{
  return set = IfOrU(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end u4& SetIfOrU(u4&, u4, bool, u4)                                      */
inline u8& SetIfOrUL(u8& set, const u8& to, bool given, const u8& orTo)
{
  return set = IfOrUL(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end u8& SetIfOrUL(u8&, const u8&, bool, const u8&)                       */
inline um& SetIfOrUM(um& set, um to, bool given, um orTo)
{
  return set = IfOrUM(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end um& SetIfOrUM(um&, um, bool, um)                                     */
inline u2& SetIfOrUS(u2& set, u2 to, bool given, u2 orTo)
{
  return set = IfOrUS(to, orTo, given); // given? 1*to+0*orTo : 0*to+1*orTo
} // end u2& SetIfOrUS(u2&, u2, bool, u2)                                     */


template <class T>
inline bool SetIfOrP(T* set, const T& to, bool given, const T& orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrP<T>(T*, const T&, bool, const T&)                       */
inline bool SetIfOrPB(u1* set, u1 to, bool given, u1 orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPB(u1*, u1, bool, u1)                                    */
inline bool SetIfOrPC(s1* set, s1 to, bool given, s1 orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPC(s1*, s1, bool, s1)                                    */
inline bool SetIfOrPD(f8* set, const f8& to, bool given, const f8& orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPF(f8*, const f8&, bool, const f8&)                      */
inline bool SetIfOrPF(f4* set, f4 to, bool given, f4 orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPF(f4*, f4, bool, f4)                                    */
inline bool SetIfOrPI(s4* set, s4 to, bool given, s4 orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPI(s4*, s4, bool, s4)                                    */
inline bool SetIfOrPL(s8* set, const s8& to, bool given, const s8& orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPF(s8*, const s8&, bool, const s8&)                      */
inline bool SetIfOrPS(s2* set, s2 to, bool given, s2 orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPS(s2*, s2, bool, s2)                                    */
inline bool SetIfOrPU(u4* set, u4 to, bool given, u4 orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPU(u4*, u4, bool, u4)                                    */
inline bool SetIfOrPUL(u8* set, const u8& to, bool given, const u8& orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPF(u8*, const u8&, bool, const u8&)                      */
inline bool SetIfOrPUM(um* set, um to, bool given, um orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPUM(um*, um, bool, um)                                   */
inline bool SetIfOrPUS(u2* set, u2 to, bool given, u2 orTo)
{
  if (!set) { return false; }
  *set = given * to + !given * orTo; // given? 1*to + 0*orTo : 0*to + 1*orTo
  return true;
} // end bool SetIfOrPUS(u2*, u2, bool, u2)                                   */


inline f8& SetInvD(f8& toReciprocal)
{ return toReciprocal = InvD(toReciprocal); } // end f8& SetInvD(f8&)         */
inline f4& SetInvF(f4& toReciprocal)
{ return toReciprocal = InvF(toReciprocal); } // end f4& SetInvF(f4&)         */


template<class T> inline bool SetP(T* set, const T& to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetP<T>(T*, const T&)                                           */
inline bool SetPB(u1* set, u1 to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPB(u1*, u1)                                                  */
inline bool SetPC(s1* set, s1 to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPC(s1*, s1)                                                  */
inline bool SetPD(f8* set, const f8& to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPD(f8*, const f8&)                                           */
inline bool SetPF(f4* set, f4 to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPF(f4*, f4)                                                  */
inline bool SetPI(s4* set, s4 to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPI(s4*, s4)                                                  */
inline bool SetPL(s8* set, const s8& to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPL(s8*, const s8&)                                           */
inline bool SetPS(s2* set, s2 to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPS(s2*, s2)                                                  */
inline bool SetPU(u4* set, u4 to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPU(u4*, u4)                                                  */
inline bool SetPUL(u8* set, const u8& to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPUL(u8*, const u8&)                                          */
inline bool SetPUM(um* set, um to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPUM(um*, um)                                                 */
inline bool SetPUS(u2* set, u2 to)
{
  if (!set) { return false; } *set = to; return true;
} // end bool SetPUS(u2*, u2)                                                 */


inline bool SetSubB(u1& current, u1 minus, OFlow policy)
{
  return SetDnB(current, current - minus, policy);
} // end bool SetSubB(u1&, u1, OFlow)                                         */
inline bool SetSubC(s1& current, s1 minus, OFlow policy)
{
  s1 dif = current - minus;
  return IsNegC(minus)? SetUpC(current,dif,policy):SetDnC(current,dif,policy);
} // end bool SetSubC(s1&, s1, OFlow)                                         */
inline bool SetSubD(f8& current, const f8& minus, OFlow policy)
{
  f8 dif = current - minus;
  return IsNegD(minus)? SetUpD(current,dif,policy):SetDnD(current,dif,policy);
} // end bool SetSubD(f8&, const f8&, OFlow)                                  */
inline bool SetSubF(f4& current, f4 minus, OFlow policy)
{
  f4 dif = current - minus;
  return IsNegF(minus)? SetUpF(current,dif,policy):SetDnF(current,dif,policy);
} // end bool SetSubF(f4&, f4, OFlow)                                         */
inline bool SetSubI(s4& current, s4 minus, OFlow policy)
{
  s4 dif = current - minus;
  return IsNegI(minus)? SetUpI(current,dif,policy):SetDnI(current,dif,policy);
} // end bool SetSubI(s4&, s4, OFlow)                                         */
inline bool SetSubL(s8& current, const s8& minus, OFlow policy)
{
  s8 dif = current - minus;
  return IsNegL(minus)? SetUpL(current,dif,policy):SetDnL(current,dif,policy);
} // end bool SetSubL(s8&, const s8&, OFlow)                                  */
inline bool SetSubS(s2& current, s2 minus, OFlow policy)
{
  s2 dif = current - minus;
  return IsNegS(minus)? SetUpS(current,dif,policy):SetDnS(current,dif,policy);
} // end bool SetSubS(s2&, s2, OFlow)                                         */
inline bool SetSubU(u4& current, u4 minus, OFlow policy)
{
  return SetDnU(current, current - minus, policy);
} // end bool SetSubU(u4&, u4, OFlow)                                         */
inline bool SetSubUL(u8& current, const u8& minus, OFlow policy)
{
  return SetDnUL(current, current - minus, policy);
} // end bool SetSubUL(u8&, const u8&, OFlow)                                 */
inline bool SetSubUM(um& current, um minus, OFlow policy)
{
  return SetDnUM(current, current - minus, policy);
} // end bool SetSubUM(um&, um, OFlow)                                        */
inline bool SetSubUS(u2& current, u2 minus, OFlow policy)
{
  return SetDnUS(current, current - minus, policy);
} // end bool SetSubUS(u2&, u2, OFlow)                                        */


inline bool SetUpB(u1& val, u1 result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("s2 overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val=u1_MAX; break;// Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0; break;   // Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = u1_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpB(u1&, u1, OFlow)                                          */
inline bool SetUpC(s1& val, s1 result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("s2 overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val=s1_MAX; break;// Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0; break;   // Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = s1_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpC(s1&, s1, OFlow)                                          */
inline bool SetUpD(f8& val, const f8& result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("f8 overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val = INF; break; // Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0.0; break; // Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = f8_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpD(f8&, const f8&, OFlow)                                   */
inline bool SetUpF(f4& val, f4 result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("f4 overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val = INf; break; // Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0.0f; break;// Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = f4_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpF(f4&, f4, OFlow)                                          */
inline bool SetUpI(s4& val, s4 result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("s4 overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val=s4_MAX; break;// Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0; break;   // Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = s4_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpI(s4&, s4, OFlow)                                          */
inline bool SetUpL(s8& val, const s8& result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("s8 overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val=s8_MAX;break; // Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0; break;   // Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = s8_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpL(s8&, const s8&, OFlow)                                   */
inline bool SetUpS(s2& val, s2 result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("s2 overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val=s2_MAX; break;// Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0; break;   // Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = s2_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpS(s2&, s2, OFlow)                                          */
inline bool SetUpU(u4& val, u4 result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("u4 overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val=u4_MAX; break;// Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0; break;   // Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = u4_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpU(u4&, u4, OFlow)                                          */
inline bool SetUpUL(u8& val, const u8& result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("u8 overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val=u8_MAX;break; // Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0u; break;  // Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = u8_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpUL(u8&, const u8&, OFlow)                                  */
inline bool SetUpUM(um& val, um result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("um overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val=um_MAX; break;// Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0u; break;  // Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = um_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpUM(um&, um, OFlow)                                         */
inline bool SetUpUS(u2& val, u2 result, OFlow policy)
{
  if (val <= result) { val = result; return true; } // typical (=) outcome
  if (policy <= OFlow::Ask) { throw std::invalid_argument("u2 overflow"); }
  switch (policy) // else overflow exception occurred
  {
  case OFlow::Wrap:     case OFlow::KeepDn:         case OFlow::ClampDn:
  case OFlow::NullDn:   case OFlow::SetDn: default: // overflow ^Wrap cases
    val = result;     // Set overflow wrapped remainder (typical overflow)
  case OFlow::Keep:     case OFlow::ClampDnKeepUp:  case OFlow::NullDnKeepUp:
  case OFlow::KeepUp:   case OFlow::SetDnKeepUp:    // overflow ^Keep cases
    break;            // Set overflow value as input given: ignore attempt
  case OFlow::Clamp:    case OFlow::KeepDnClampUp:  case OFlow::NullDnClampUp:
  case OFlow::ClampUp:  case OFlow::SetDnClampUp:   // overflow ^Clamp cases
    val=u2_MAX; break;// Set overflow clamped to typed maxima value
  case OFlow::Null:     case OFlow::KeepDnNullUp:   case OFlow::ClampDnNullUp:
  case OFlow::NullUp:   case OFlow::SetDnNullUp:    // overflow ^Null cases
    val = 0; break;   // Set overflow to neutral / null value
  case OFlow::Set:      case OFlow::KeepDnSetUp:    case OFlow::ClampDnSetUp:
  case OFlow::SetUp:    case OFlow::NullDnSetUp:    // overflow ^Set cases
    val = u2_INVALID; // Set ^ to reserved sentinel value
  }
  return false;

} // end bool SetUpUS(u2&, u2, OFlow)                                         */


inline f4 SqrtF(f4 value)
{
  return static_cast<f4>(sqrt(static_cast<f8>(value)));
} // end f4 SqrtF(f4)                                                         */


inline u1 SubB(u1 current, u1 minus, OFlow policy)
{
  u1 prior = current;
  try
  {
    if (!SetSubB(current, minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end u1 SubB(u1, u1, OFlow)                                               */
inline s1 SubC(s1 current, s1 minus, OFlow policy)
{
  s1 prior = current;
  try
  {
    if (!SetSubC(current, minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end s1 SubC(s1, s1, OFlow)                                               */
inline f8 SubD(f8 current, const f8& minus, OFlow policy)
{
  f8 prior = current;
  try
  {
    if (!SetAddD(current, -minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end f8 SubD(const f8&, const f8&, OFlow)                                 */
inline f4 SubF(f4 current, f4 minus, OFlow policy)
{
  f4 prior = current;
  try
  {
    if (!SetSubF(current, minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end f4 SubF(f4, f4, OFlow)                                               */
inline s4 SubI(s4 current, s4 minus, OFlow policy)
{
  s4 prior = current;
  try
  {
    if (!SetSubI(current, minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end s4 SubI(s4, s4, OFlow)                                               */
inline s8 SubL(s8 current, const s8& minus, OFlow policy)
{
  s8 prior = current;
  try
  {
    if (!SetSubL(current, minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end s8 SubL(const s8&, const s8&, OFlow)                                 */
inline s2 SubS(s2 current, s2 minus, OFlow policy)
{
  s2 prior = current;
  try
  {
    if (!SetSubS(current, minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end s2 SubS(s2, s2, OFlow)                                               */
inline u4 SubU(u4 current, u4 minus, OFlow policy)
{
  u4 prior = current;
  try
  {
    if (!SetSubU(current, minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end u4 SubU(u4, u4, OFlow)                                               */
inline u8 SubUL(u8 current, const u8& minus, OFlow policy)
{
  u8 prior = current;
  try
  {
    if (!SetSubUL(current, minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end u8 SubUL(const u8&, const u8&, OFlow)                                */
inline um SubUM(um current, um minus, OFlow policy)
{
  um prior = current;
  try
  {
    if (!SetSubUM(current, minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end um SubUM(um, um, OFlow)                                              */
inline u2 SubUS(u2 current, u2 minus, OFlow policy)
{
  u2 prior = current;
  try
  {
    if (!SetSubUS(current, minus, policy))
    {
      // Informative as the intended function of overflow handling stays quiet
      dp::Log::Info(std::to_string(prior) + " - " + std::to_string(minus), 0);
      dp::Log::I(" != " + std::to_string(current), true);
    }
  }
  catch (std::invalid_argument const& ex) { throw ex; }
  return current;

} // end u2 SubUS(u2, u2, OFlow)                                              */


template <class T> inline void Swap(T& a, T& b)
{
  T temp = a; a = b; b = temp;

} // end void Swap<T>(T&, T&)                                                 */
inline void SwapB(u1& a, u1& b)
{
  u1 temp = a; a = b; b = temp;
} // end void SwapB(u1&, u1&)                                                 */
inline void SwapC(s1& a, s1& b)
{
  s1 temp = a; a = b; b = temp;
} // end void SwapC(s1&, s1&)                                                 */
inline void SwapD(f8& a, f8& b)
{
  f8 temp = a; a = b; b = temp;
} // end void SwapD(f8&, f8&)                                                 */
inline void SwapF(f4& a, f4& b)
{
  f4 temp = a; a = b; b = temp;
} // end void SwapF(f4&, f4&)                                                 */
inline void SwapI(s4& a, s4& b)
{
  s4 temp = a; a = b; b = temp;
} // end void SwapF(s4&, s4&)                                                 */
inline void SwapL(s8& a, s8& b)
{
  s8 temp = a; a = b; b = temp;
} // end void SwapL(s8&, s8&)                                                 */
inline void SwapS(s2& a, s2& b)
{
  s2 temp = a; a = b; b = temp;
} // end void SwapS(s2&, s2&)                                                 */
inline void SwapU(u4& a, u4& b)
{
  u4 temp = a; a = b; b = temp;
} // end void SwapU(u4&, u4&)                                                 */
inline void SwapUL(u8& a, u8& b)
{
  u8 temp = a; a = b; b = temp;
} // end void SwapUL(u8&, u8&)                                                */
inline void SwapUM(um& a, um& b)
{
  um temp = a; a = b; b = temp;
} // end void SwapUM(um&, um&)                                                */
inline void SwapUS(u2& a, u2& b)
{
  u2 temp = a; a = b; b = temp;
} // end void SwapUS(u2&, u2&)                                                */


template <class T> inline void SwapIf(T& a, T& b, bool given)
{
  T temp = IfOr<T>(a, b, given); a = IfOr<T>(b, a, given); b = temp;
} // end void SwapIf<T>(T&, T&, bool)                                         */
inline void SwapIfB(u1& a, u1& b, bool given)
{
  u1 temp = IfOrB(a, b, given); a = IfOrB(b, a, given); b = temp;
} // end void SwapIfB(u1&, u1&, bool)                                         */
inline void SwapIfC(s1& a, s1& b, bool given)
{
 s1 temp = IfOrC(a, b, given); a = IfOrC(b, a, given); b = temp;
} // end void SwapIfC(s1&, s1&, bool)                                         */
inline void SwapIfD(f8& a, f8& b, bool given)
{
  f8 temp = IfOrD(a, b, given); a = IfOrD(b, a, given); b = temp;
} // end void SwapIfD(f8&, f8&, bool)                                         */
inline void SwapIfF(f4& a, f4& b, bool given)
{
  f4 temp = IfOrF(a, b, given); a = IfOrF(b, a, given); b = temp;
} // end void SwapIfF(f4&, f4&, bool)                                         */
inline void SwapIfI(s4& a, s4& b, bool given)
{
  s4 temp = IfOrI(a, b, given); a = IfOrI(b, a, given); b = temp;
} // end void SwapIfI(s4&, s4&, bool)                                         */
inline void SwapIfL(s8& a, s8& b, bool given)
{
  s8 temp = IfOrL(a, b, given); a = IfOrL(b, a, given); b = temp;
} // end void SwapIfL(s8&, s8&, bool)                                         */
inline void SwapIfS(s2& a, s2& b, bool given)
{
  s2 temp = IfOrS(a, b, given); a = IfOrS(b, a, given); b = temp;
} // end void SwapIfS(s2&, s2&, bool)                                         */
inline void SwapIfU(u4& a, u4& b, bool given)
{
  u4 temp = IfOrU(a, b, given); a = IfOrU(b, a, given); b = temp;
} // end void SwapIfU(u4&, u4&, bool)                                         */
inline void SwapIfUL(u8& a, u8& b, bool given)
{
  u8 temp = IfOrUL(a, b, given); a = IfOrUL(b, a, given); b = temp;
} // end void SwapULIf(u8&, u8&, bool)                                        */
inline void SwapIfUM(um& a, um& b, bool given)
{
  um temp = IfOrUM(a, b, given); a = IfOrUM(b, a, given); b = temp;
} // end void SwapIfUM(um&, um&, bool)                                        */
inline void SwapIfUS(u2& a, u2& b, bool given)
{
  u2 temp = IfOrUS(a, b, given); a = IfOrUS(b, a, given); b = temp;
} // end void SwapIfUS(u2&, u2&, bool)                                        */

#endif
