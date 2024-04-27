/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v6f.cpp
Purpose:  API agnostic, multipurpose, 6 dimensional float-point element vector
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

#include <ostream>      // Output stream definition for operator overload
#include "ang.h"        // Unit measures for conversion in and out of arc spans
#include "ASCIIValue.h" // char range based [] operator manipulation
#include "m4f.h"        // Generating matrices from vector data
#include "Value.h"      // NearF() float point (proximity as equality) function
#include "v2f.h"        // Swizzling truncation to lower dimension vectors
#include "v3f.h"        // Swizzling / extraction to lower dimension vectors
#include "v4f.h"        // Swizzling truncation to lower dimension vectors
#include "v6f.h"        // Class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


dp::v6f::v6f(f4 x, f4 y, f4 z, f4 w, f4 t, f4 s)
  : xs(x), ys(y), zs(z), xe(w), ye(t), ze(s)
{ } // end v3f::v3f(f4, f4, f4, f4, f4, f4)                                   */

dp::v6f::v6f(const v6f& src)
  : v6f(src.x, src.y, src.z, src.w, src.t, src.s)
{ } // end v6f::v6f(const v6f&)                                               */

dp::v6f::v6f(const v3f& initial, const v3f& terminal)
  : init(initial), term(terminal)
{ } // end v6f::v6f(const v3f&, const v3f&)                                   */

dp::v6f::v6f(const v2f& origin, const v2f& v1, const v2f& v2)
  : a(origin), b(v1), c(v2)
{ } // end v6f::v6f(const v2f&, const v2f&, const v2f&)                       */

dp::v6f::v6f(v6f && result) noexcept
{ *this = std::move(result); } // end v6f(v6f&&) noexcept                     */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

std::string dp::v6f::JSON(void) const
{
  return "[" + std::to_string(x) + "," + std::to_string(y) + ","
    + std::to_string(z) + ","  + std::to_string(w) + ","  + std::to_string(t)
    + ","  + std::to_string(s) + "]";
} // end std::string v3f::json(void) const


// Format a v3f for being output as a vector notated string
std::string dp::v6f::String(void) const
{
  std::string vecFormat;
  vecFormat = "[";
  if (x >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(x) + ", ";
  if (y >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(y) + ", ";
  if (z >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(z) + " ]";
  if (w >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(w) + " ]";
  if (t >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(t) + " ]";
  if (s >= 0.0f) { vecFormat += " "; }
  vecFormat += std::to_string(s) + " ]";
  return vecFormat;
} // end std::string v3f::String(void) const                                  */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Zero vector in 3 dimensions; error state reserved value
const dp::v6f nullv6f_(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
const dp::v6f& dp::nullv6f = nullv6f_;


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
