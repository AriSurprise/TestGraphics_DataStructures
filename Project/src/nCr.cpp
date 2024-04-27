/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File:     nCr.cpp
Purpose:  Use dynamic programming table to get pascal values (n<=101; 0 < k < n)
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no platform requirements)
Project:  a.Surprise_MAT300_1
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

// <stl>
#include <vector>   // internal choose list pick list member initialization
// "./src/..."
#include "nCr.h"    // class declaration header file
#include "Log.h"    // Error / warning output / message logging 


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                    Public Static Variable Initializations                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

std::vector<std::vector<u8>> dp::n::choose;

const std::vector<dp::SimpleTerm> dp::n::nullBasis;

std::vector<std::vector<std::vector<dp::SimpleTerm>>> dp::n::B;

std::vector<std::vector<std::vector<dp::SimpleTerm>>> dp::n::C;

// choose 102 pick k has values go out of u8-byte range, making 102-1 the cap
u1 N_MAX = 101;

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                           Public Static Methods                            */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

const std::vector<dp::SimpleTerm>& dp::n::Bernstein(u1 d, u1 i)
{
  if (d > N_MAX)
  {
    Log::Warn("n::Bernstein - only supports n up to 101 due to u8-byte range");
    return nullBasis;
  }
  if (B.size() < d || B[d].size() < d + 1u) { CalculateD(d); }
  return B[d][i];
}

const u8& dp::n::Cr(u1 n, u1 k)
{
  if (n > N_MAX)
  {
    Log::Warn("n::Pick - only supports n up to 101 due to u8-byte range");
    return invalid;
  }
  if (k > n) { return invalid; } // n<0 | k<0  = 0; outside U_INT input range
  if (k > (n >> 1)) { k = n - k; } // nCr(n, k) = nCr(n, n-k) => fit to table
  if (choose.size() <= n) { Init(n); } // Grow list if needed
  if (choose.size() < n || choose[n].size() < k) // 
  {
    Log::Error("n::Pick - allocation failure ");
    return invalid;
  }
  return choose[n][k];

} // end u8 n::Cr(u1, u1)                                                     */


std::vector<u8> dp::n::Choices(u1 n)
{
  std::vector<u8> result = Row(n);
  result.resize(n+1, 0);
  // Populate 2nd, redundant row half of values
  for (u4 i = 0, j = n; j > i; ++i, --j) { result[j] = result[i]; }
  return result;

} // end n::Choices(u1)`                                                      */


void dp::n::Init(u1 n)
{
  if (n > N_MAX)
  {
    Log::Warn(
      "n::InitTo - only supports n up to 101 due to u8-byte range");
    n = N_MAX;
  }
  while (n >= choose.size())
  {
    u1 i = static_cast<u1>(choose.size());
    CalculateN(i);
  }

} // end u8 n::Init(u1)                                                       */


const std::vector<u8>& dp::n::Row(u1 n)
{
  if (n > N_MAX)
  {
    Log::Warn("n::Choices - only supports n up to 101 due to u8-byte range");
    return choose[0];
  }
  if (choose.size() <= n) { Init(n); } // Grow list if needed
  if (choose.size() < n)
  {
    Log::Error("n::Choices - allocation failure ");
    return choose[0];
  }
  return choose[n];

} // end n::Row(u1)`                                                          */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                           Private Static Methods                           */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

u8 dp::n::Calculate(u1 n, u1 k)
{
  if (k == 0) { return 1; }
  else if (k == 1) { return static_cast<u8>(n); }
  else if (k >= n * 0.5) { k = (n - k); }
  return choose[n-1][k-1] + choose[n-1][((k == choose[n-1].size()) ? k-1 : k)];

} // end u8 n::Calculate(u4, u4)                                              */


void dp::n::CalculateC(u1 d)
{
  u1 basisElements = d + 1;
  std::vector<u8> choices;
  dp::variable basis, sum;
  if (C.size() < basisElements) { C.resize(basisElements); }
  if (C[d].size() >= basisElements) { return; }
  if (B[d].size() < basisElements) { CalculateD(d); }
  C[d].resize(basisElements);
  C[d][0].resize(1);
  C[d][0][0] = { 1.0f, 0.0f };  // first cumulative Bernstein cancels to 1
  C[d][d] = B[d][d];            // last cumulative Bernstein just B[d][d]
  for (u1 i = d-1; i > 0; --i)
  {
    sum = C[d][i+1];
    // each other elem is C[d][i+1] (prev cumulative sum) + B[d][i]
    basis = B[d][i];
    sum += basis;
    C[d][i].resize(sum.Size());
    for (u1 j = 0; j < C[d][i].size(); ++j) { C[d][i][j] = sum[j]; }
  }

} // end u8 n::CalculateC(u1)                                                 */


void dp::n::CalculateD(u1 d)
{
  u1 basisElements = d + 1;
  f4 co, exp;
  std::vector<u8> choices;
  std::vector<SimpleTerm> basis;
  // Make sure 
  if (B.size() < basisElements) { B.resize(basisElements); }
  if (B[d].size() < basisElements) { B[d].resize(basisElements); }
  for (u1 i = 0; i < basisElements; ++i)
  {
    choices = Choices(d - i);
    basis.resize(choices.size());
    exp = 0.0f;
    co = 0.0f;
    for (u1 j = 0; j < basis.size(); ++j)
    {
      co = (f4)Cr(d, i) * choices[j];
      basis[j] = { (j&1 ? -co : co), i + exp++ };
    }
    B[d][i] = basis;
  }

} // end u8 n::CalculateD(u1)                                                 */


void dp::n::CalculateN(u1 n)
{
  u1 rowSize = ((n + 1) / 2) + ((n & 1) ? 0 : 1);
  std::vector<u8> row;
  for (u1 i = 0; i < rowSize; ++i)
  {
    u8 k = Calculate(n, i);
    row.push_back(k);
  }
  choose.push_back(row);

} // end u8 n::CalculateN(u1)                                                 */
