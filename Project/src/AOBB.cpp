/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  AOBB.cpp
Purpose:  Container for mesh center / half extent <x,y,z> <coordinate, vector>
Details: member footprint kept in line with v6f use of <mid, ext> pattern
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
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

// "./src/..."
#include "Log.h"  // System message management for errors, warnings, etc
#include "AABB.h" // AOBB variant in < min, max > extrema instead of <mid, ext>
#include "AOBB.h" // Class interface declaration header
#include "v6f.h"  // <mid, ext> capable of data abstract I/O in a 6D container

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::AOBB::AOBB(f4 xMid, f4 yMid, f4 zMid, f4 xExt, f4 yExt, f4 zExt)
  : mid(xMid, yMid, zMid), ext(xExt, yExt, zExt), mod(nullptr)
{ ValidateBounds(); } // end AOBB(f4, f4, f4, f4, f4, f4)                     */

dp::AOBB::AOBB(const v3f& center, const v3f& halfExtent)
  : mid(center), ext(halfExtent), mod(nullptr)
{ ValidateBounds(); } // end AOBB(const v3f&, const v3f&)                     */

dp::AOBB::AOBB(const AABB & source)
  : mid((source.Max()+source.Min()) * 0.5f),
  ext((source.Max()-source.Min()) * 0.5f), mod(nullptr)
{ } // end AOBB(const AABB & source)                                          */

dp::AOBB::AOBB(const v6f& source, bool inRanges)
{
  v6f sc = inRanges ? source.InExtents() : source;
  mid = sc.mid;
  ext = sc.ext;
  ValidateBounds();

} // end AOBB(const v6f&, bool)                                               */

dp::AOBB::AOBB(const AOBB& source)
  : mid(source.mid), ext(source.ext), mod(source.mod)
{ } // end AOBB(const AOBB&)                                                  */

dp::AOBB::AOBB(AOBB&& result) noexcept
  : mid(result.mid), ext(result.ext), mod(result.mod)
{ } // end AOBB(AOBB&&)                                                       */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

bool dp::AOBB::In(const AABB& box) const
{
  return !(box.Max().X() < mid.x-ext.x || mid.x+ext.x < box.Min().X())
      && !(box.Max().Y() < mid.y-ext.y || mid.y+ext.y < box.Min().Y())
      && !(box.Max().Z() < mid.z-ext.z || mid.z+ext.z < box.Min().Z());

} // end bool AOBB::In(const AABB&) const                                     */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Helper Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
