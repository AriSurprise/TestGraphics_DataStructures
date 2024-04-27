/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  ang.cpp
Purpose:  Float point primitive wrapper with unit handles to track conversions
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no platform requirements)
Project:  a.Surprise_CS350_1
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

#include "Value.h"  // Type just automates unit conversions (defined within)
#include "v2f.h"    // Tangent / slope function using cartesian rise / run
#include "ang.h"    // Class declaration header file


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                      Static Variable Initializations                       */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::arc dp::ang::unitDefault = arc::radians;
f4 dp::ang::offsetRadDefault = 0.0f;
dp::winding dp::ang::directionDefault = winding::counterclockwise;

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::ang::ang(f4 rotation, arc measure, winding chirality)
{
  switch (measure)
  {
  case arc::degrees: d(rotation, chirality); break;
  case arc::revolutions: s(rotation, chirality); break;
  case arc::radians: r(rotation, chirality); break;
  case arc::gradians: g(rotation, chirality); break;
  default: // Small numbers are probably revolutions or radians...
    (rotation>-PI && rotation<PI) ?
      r(rotation, chirality) : d(rotation, chirality);
    // Odds are, either radians were fed in by a function, or degrees were
    // naively input by hand (for which less than 6.18 degrees isn't notable)
    break;
  }
} // end ang::ang(f4, measure, winding)                                       */

dp::ang::ang(const ang& source): angle(source.angle)
{ } // end ang::ang(const ang&)                                               */

dp::ang::ang(ang&& result) noexcept : angle(std::move(result.angle))
{ } // end ang::ang(ang&&) noexcept                                           */

dp::ang::ang(void) : angle(0.0f) { } // end ang::ang(void)                  *///


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                           Pubilc Static Methods                            */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

void dp::ang::ImplicitOutputDefault(arc unitMeasure, ang neutralXOffset,
  winding chirality)
{
  unitDefault = unitMeasure;
  // Store and apply the offset uniformly, not conditional to winding: clearer
  offsetRadDefault = (chirality == winding::counterclockwise) ?
    neutralXOffset.angle : -neutralXOffset.angle;
  directionDefault = chirality;

} // end static void ImplicitOutputDefault(arc, ang, winding)                 */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::ang& dp::ang::atan(f4 slope, bool otherQuadrant, int rotations)
{
  *this = ATanF(slope, otherQuadrant);
  if (rotations) { angle += TAU * rotations; }
  return *this;
} // end ang& atan(f4)                                                        */

f4 dp::ang::tan(void) const { return TanF(*this); } //end f4 tan(void) const  */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overlods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      
dp::ang dp::ang::operator-(void) const
{ return ang((angle - PI), arc::rad); } // end ang ang::operator-(void) const */

dp::ang& dp::ang::operator!(void)
{
  angle += PI;
  return *this;
} // end ang& ang::operator!(void)                                            */

f4 dp::ang::operator/(ang arc) const
{
  return (!NearF(arc.angle, 0.0f) ? angle / arc.angle
    : std::signbit(arc.angle) ? -INf : INf);
} // end ang ang::operator/(f4) const                                         */

dp::ang dp::ang::operator/(f4 divisor) const
{
  return ang((!NearF(divisor, 0.0f)? angle / divisor
            : std::signbit(divisor)? -INf : INf), arc::radians);
} // end ang ang::operator/(f4) const                                         */

inline dp::ang& dp::ang::operator/=(f4 divisor)
{
  angle = !NearF(divisor, 0.0f)? angle/divisor
        : std::signbit(divisor)? -INf : INf;
  return *this;
} // end ang& ang::operator/=(f4)                                             */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Overlods                           */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

bool dp::operator==(ang lhs, ang rhs)
{
  return (NearF(lhs.radRef(), rhs.radRef()));
} // end bool operator==(ang, ang)                                            */

bool dp::operator!=(ang lhs, ang rhs)
{
  return (!NearF(lhs.radRef(), rhs.radRef()));
} // end bool operator!=(ang, ang)                                            */
