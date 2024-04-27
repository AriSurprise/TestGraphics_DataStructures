/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Stats.cpp
Purpose:  Structs / functions to extract generalizations from larger data sets
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

#include "Log.h"        // System message management for errors, warnings, etc
#include "Stats.h"      // Function declaration header file
#include "Value.h"      // Standardized generic primitive manip. operations

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::uRng::uRng(u4 samples, u4 size, u2 first, s2 steps)
: initS(first), step(steps)
{ SetMax(size, samples); } // end uRng(u4, u4, s2, u2)                        */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                        Public Function Definitions                         */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::uRng& dp::uRng::SetMax(u4 datasetSize, u4 sampleSize, bool reset)
{
  total = IfOrU(datasetSize+1, datasetSize, datasetSize == 0);//total =:[1,MAX]
  if (sampleSize != u4_INVALID || total < samples)
  {SetIfOrU(samples,sampleSize, sampleSize<total, total);}//count =: [1, total)
  SetIfUS(initS, (u2)ModU(initS, total), total < initS); // init  =: [0, total)
  SetStep(step);
  if (!reset) { return *this; }
  return Reset();

} // end u4 Smpl::SetMax(u4)                                                  */

dp::uRng& dp::uRng::SetStep(s2 increment)
{
  // First, if as many sampled points exist as data points, ignore & simplify
  if (total <= samples) { step = 1; initS = 0; return *this; }

  // Eliminate cases of the given step size only reaching the same sample
  SetIfS(step, (s2)ModI(increment,total),(s2)total<increment-1);//-> [1, total)
  // If over half the dataset, the negated value is gets less overflow => % ops
  SetIfS(step, IfOrS(step-(s2)total, step+(s2)total, IsNegS(step)),
                          (s2)(total>>1) < AbsS(step)); // step -> (-/+)total/2
  SetIfS(step, step+1, (step == 0)); // step   =: [-total/2, 0), (0, total/2)
  // Again, check more rigorously for reaching redundant samples as needed
  if (!IsInS(step, 1, -1)) // if even divisibility is not a given, i.e. each 1
  {
    s2 prev = step;         // prior step size state record
    s2 div = total / step;  // maximum relevant divisibility for dataset
    s2 inc = IsNegS(step)? -1 : 1; // direction of increment
    // Find lowest non-cyclic step of iteration (assumes starting at (-/+)2)
    while (step <= div && step % total == 0) { step += inc; }
    // If starting larger / none found, use more solicited, refined result
    SetIfS(step, prev, step % total == 0);
  }
  return *this;

} // end Smpl& Smpl::SetStep(s2)                                              */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                        Operator Overload Definitions                       */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::uRng& dp::uRng::operator++(void)
{
  ++sampled;
  current += step;
  if (total < current) { current = ModU(current, total); }
  return *this;
} // end Smpl& Smpl::operator++(void)                                         */

dp::uRng& dp::uRng::operator=(const uRng& other)
{
  samples = other.samples;
  initS   = other.initS;
  step    = other.step;
  sampled = other.sampled;
  current = other.current;
  total   = other.total;
  return *this;

} // end u4 Smpl::operator-(const Smpl&)                                      */
