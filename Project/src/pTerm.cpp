/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  pTerm.cpp
Purpose:  4-byte float-point array wrapper class for arbitrary dimensional math
Language: C++ / Eng, MSVC 19 (used, but should work with most compilers)
Platform: MSVC 19 / Windows 10 x64 (used, but should have no requirements)
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

#include <iostream>
// <stl>
#include <algorithm>// std::sort to sort term vectors
#include <list>     // easier sorted insertion container for polynomial math
#include <ostream>  // Output stream definition for operator overload
// "./src/..."
#include "nCr.h"    // Bernstein / Pascal basis coefficient sets
#include "Log.h"    // System message management for errors, warnings, etc
#include "Value.h"  // Basic value comparisons: min, max, clamp, near, etc
#include "pNom.h"   // Class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::SimpleTerm::SimpleTerm(f4 lCoeff, f4 exponentiation)
  : coeff(RealF(lCoeff)), power(RealF(exponentiation))
{} // end sStandardTerm::sStandardTerm(f4, f4)


dp::sBasisTerm::sBasisTerm(f4 lCoeff, const vNf& varExp, u4 polyExp)
  : coeff(lCoeff), exp(varExp)
{
  // Across any number of variables, scalar exponential factor applies linearly
  if (polyExp != 1.0f) { exp *= static_cast<f4>(polyExp); }

} // end sBasisVar::sBasisVar(f4, const vNf&, u4)

dp::sBasisTerm::sBasisTerm(f4 lCoeff, std::initializer_list<f4> varExp,
  u4 polyExp) : coeff(lCoeff), exp(varExp)
{
  // Across any number of variables, scalar exponential factor applies linearly
  if (polyExp != 1.0f) { exp *= static_cast<f4>(polyExp); }

} // end sBasisVar::sBasisVar(f4, std::initializer_list<f4>&, u4)

dp::variable::variable(const std::vector<SimpleTerm>& direction, u4 exponent)
  : dir({1.0f, 0.0f}) // init to 1*x^{0} = 1
{
  //exponent = RealF(exponent); // only needed if non-integers become allowed
  // If (...)^{0} or equivalent direction given, non varying in this direction
  if (exponent == 0) { dir.clear(); }
  // Otherwise if direction given in a simplified form, expand
  else if (exponent > 1)
  {
    u4 maxPow2 = Log2TruncU(exponent); // top 2^n to multiply into product
    u4 rProd = exponent; // remaining multiplicity to put into product
    // Create list of exponentially raised terms to multiply to total product
    std::vector<std::vector<SimpleTerm>> pow2(maxPow2+1);
    // ex- given x^7, instead of x*x*...*x, use x^(2^2)*x^2*x^1: O(logn) < O(n)
    // Base case initialization
    pow2[0] = direction; // Given: (0th power => 1*input => input => [units])
    std::sort(pow2[0].begin(), pow2[0].end()); // input should be ordered
    for (u4 i = 1; i <= maxPow2; ++i) // per relevant power of 2
    {
      std::list<SimpleTerm> sum;
      // Each resultant polynomial is square of the previous term
      for (u4 term = 0; term < pow2[i-1].size(); ++term) // per term of prior 2^i
      {
        // Copy the baseline of the previous, intact squared term
        std::list<SimpleTerm> pPow(pow2[i-1].begin(), pow2[i - 1].end());
        std::list<SimpleTerm>::iterator pPTermProd, termPLoc;
        // Multiply current term into the each term of the prior term sequence
        for (pPTermProd = pPow.begin();pPTermProd != pPow.end(); ++pPTermProd)
        {
          *pPTermProd *= pow2[i-1][term];
          // Coalesce those products into the sum for the i'th power of 2
          // Seek to where in the power sum sequence the multiplied entry fits
          termPLoc = sum.begin();
          while (termPLoc != sum.end() && termPLoc->power < pPTermProd->power)
          {
            ++termPLoc;
          }
          // Check for prior equally powered term matches...
          if (termPLoc != sum.end() && termPLoc->power == pPTermProd->power)
          {
            termPLoc->coeff += pPTermProd->coeff; // (to coalesce by like sums)
          }
          // Otherwise insert new powered term in ascending order
          else
          {
            sum.insert(termPLoc, *pPTermProd);
          }
        }
      }
      // Prune any summary 0 coefficient products from the chain
      for (std::list<SimpleTerm>::iterator j = sum.begin(); 0; ++j)
      {
        if (j->coeff == 0.0f) { sum.erase(j); }
      }
      // Set the sorted, coalesced sum as the i'th power of 2
      pow2[i] = std::vector<SimpleTerm>(sum.begin(), sum.end());
    }
    // Having a table of relevant powers of 2, multiply those together to total
    while (rProd > 0)
    {
      // Find the next power of 2 that doesn't exceed total multiplicity
      while (rProd < (1ull << maxPow2)) { --maxPow2; }
      // Multiply that power into the final direction vector
      // Note how much product remains afterwards
      rProd -= (1ull << maxPow2);
    }
  }

} // end variable::variable(const std::vector<SimpleTerm>&, u4)               */

dp::variable::variable(std::initializer_list<SimpleTerm> basis, u4 exponent)
  : variable(std::vector<SimpleTerm>(basis), exponent)
{} // end variable::variable(const std::initializer_list<SimpleTerm>&, u4)

dp::variable::variable(std::initializer_list<f4> coeff,
  std::initializer_list<f4> pow, u4 exponent)
  : variable(ToSTerms(coeff, pow), exponent)
{} // end variable::variable(initializer_list<f4>, initializer_list<f4>, u4)

dp::variable::variable(u4 termCount, CoeffPattern type, u4 exponent, f4 scalar)
{
  if (termCount == 0) { dir.clear(); }
  else
  {
    std::vector<u8> b;
    u4 i;
    f4 n;
    switch (type)
    {
    case CoeffPattern::Bernstein:
      // Using termCount as dimensionality =: d; exponent as Bernstein term i
      // B(d,i) =: nCr(d,i)*(1-t)^{d-i}*t^{i} => (1-t)^{d-i} terms * scalars
      // => (1-t)^{n} =: pascal coefficients from all nCr(n) with incr powers
      if (exponent <= termCount) // assuming n =: d <= i...
      {
        n = static_cast<f4>(n::Cr(termCount, exponent));
        termCount -= (exponent - 1); // a row always having n+1 choices
        dir.resize(termCount);
        b = n::Choices(termCount - 1);
        for (i = 0; i < dir.size(); ++i)
        {
          // Bernstein terms oscillate; positive trailing (descending) term => i
          dir[i] = { (i & 1 ? -scalar : scalar) * n * static_cast<f4>(b[i]),
            static_cast<f4>(i + exponent) };
        }
      }
      else { dir.resize(1, {0.0f, 0.0f}); } // B(d,i) {d < i} =: nCr(d,i) =: 0
      break;
    case CoeffPattern::OscBernstein:
        // Using termCount as dimensionality =: d; exponent as Bernstein term i
        // B(d,i) =: nCr(d,i)*(1-t)^{d-i}*t^{i} => (1-t)^{d-i} terms * scalars
        // => (1-t)^{n} =: pascal coefficients from all nCr(n) with incr powers
      if (exponent <= termCount) // assuming n =: d <= i...
      {
        n = static_cast<f4>(n::Cr(termCount - 1, exponent));
        termCount -= (exponent); // a row always having n+1 choices
        dir.resize(termCount);
        b = n::Choices(termCount - 1);
        for (i = 0; i < dir.size(); ++i)
        {
          // General Bernstein pattern sees term oscillation => constant here
          dir[i] = { scalar * n * static_cast<f4>(b[i]),
            static_cast<f4>(i + exponent) };
        }
      }
      else { dir.resize(1, { 0.0f, 0.0f }); } // B(d,i) {d < i} =: nCr(d,i) =: 0
      break;
    case CoeffPattern::OscPascal:
      dir.resize(termCount);
      b = n::Choices(termCount - 1);
      for (i = 0; i < dir.size(); ++i)
      {
        // General pattern default sees leading term positive => termCount-i
        dir[i] = { ((termCount-i) & 1 ? scalar : -scalar) *
          static_cast<f4>(b[i]), static_cast<f4>(i) };
      }
      break;
    case CoeffPattern::Pascal:
      dir.resize(termCount);
      b = n::Choices(termCount - 1);
      for (i = 0; i < dir.size(); ++i)
      {
        dir[i] = { scalar * static_cast<f4>(b[i]), static_cast<f4>(i) };
      }
      break;
    case CoeffPattern::Oscillating:
      dir.resize(termCount);
      // General pattern default sees leading term positive => termCount-i
      for (u4 i = 0; i < dir.size(); ++i)
      {
        dir[i] = { ((termCount - i) & 1 ? scalar : -scalar),
          static_cast<f4>(i) };
      }
      break;
    case CoeffPattern::Constant: default:
      dir.resize(termCount);
      for (u4 i = 0; i < dir.size(); ++i)
      {
        dir[i] = { scalar, static_cast<f4>(i) };
      }
      break;
    }
  }
} // end variable::variable(initializer_list<f4>, initializer_list<f4>, u4)   */

dp::variable& dp::variable::operator+=(const variable& other)
{
  std::list<SimpleTerm> a(dir.begin(), dir.end());
  std::list<SimpleTerm>::iterator i = a.begin();
  u4 j;
  for (j = 0; j < other.Size(); ++j)
  {
    while (i != a.end() && i->power < other[j].power) { ++i; }
    if (i == a.end()) { break; }
    if (i->power == other[j].power) { i->coeff += other[j].coeff; ++i; }
    else { a.emplace(i, other[j]); }
  }
  while (j < other.Size()) { a.push_back(other[j]); ++j; }
  for (i = a.begin(); i != a.end(); ++i)
  {
    if (NearF(i->coefficient, 0.0f, 0.0001f)) { a.erase(i); }
  }
  dir = std::vector<SimpleTerm>(a.begin(), a.end());
  return *this;

} // end variable& variable::operator+=(const variable&)                      */

std::vector<dp::SimpleTerm> dp::variable::ToSTerms(std::initializer_list<f4>
  coeff, std::initializer_list<f4> pow)
{
  std::vector<SimpleTerm> input;
  std::initializer_list<f4>::iterator c = coeff.begin(), e = pow.begin();
  if (coeff.size() < pow.size())
  {
    for (; c != coeff.end(); ++c, ++e) { input.push_back({ *c, *e }); }
    // Coefficients not supplied for all exponents listed => 1x^n for each
    for (; e != pow.end(); ++e) { input.push_back({ 1, *e }); }
  }
  else if (coeff.size() > pow.size())
  {
    f4 p = 0.0f;
    for (; e != pow.end(); ++c, ++e) { input.push_back({ *c, *e }); p = *e; }
    // Exponents not supplied for any coefficients listed => x^(n-1)+1
    if (!IsIntegralF(p)) { p = static_cast<f4>(FloorF(p)); }
    for (; c != coeff.end(); ++c) { input.push_back({ *c, ++p }); }
  }
  else for (; c != coeff.end(); ++c, ++e) { input.push_back({ *c, *e }); }
  return input;

} // end static vector<SimpleTerm> variable::ToSTerms(init<f4>, init<f4>)     */

dp::pTerm::pTerm(f4 lCoeff, const std::vector<variable>& factorLists)
  : coeff(1, RealF(lCoeff)), factor(factorLists)
{} // end pTerm::pTerm(f4, const std::vector<variable>&)                      */

dp::pTerm::pTerm(const vNf& lCoeffv, const std::vector<variable>&factorLists)
  : coeff(lCoeffv), factor(factorLists)
{} // end pTerm::pTerm(const vNf&, const std::vector<variable>&)              */

dp::pTerm::pTerm(std::initializer_list<f4> coeffs, const std::vector<variable>&
  basis)
  : coeff(coeffs), factor(basis)
{} // end pTerm::pTerm(initializer_list<f4>, const vector<variable>&)         */

dp::pTerm::pTerm(f4 lCoeff, std::initializer_list<variable> factorLists)
  : pTerm(lCoeff, std::vector<variable>(factorLists.begin(), factorLists.end()))
{} // end pTerm::pTerm(f4, std::initializer_list<variable>)                   */

dp::pTerm::pTerm(const std::vector<f4>&lCoeffv, std::initializer_list<variable>
  factorLists)
  : coeff(lCoeffv), factor(factorLists)
{} // end pTerm::pTerm(const std::vector<f4>&, initializer_list<variable>)    */

dp::pTerm::pTerm(std::initializer_list<f4> factorList)
  : coeff(1, factorList.begin()[0]) // First entry is term's lead coefficient
{
  // Each subsequent value is the next variable's exponentiation
  factor.resize(factorList.end() - factorList.begin() - 1);
  std::initializer_list<f4>::iterator i = factorList.begin() + 1; u4 j = 0;
  for (;i != factorList.end(); ++i, ++j) { factor[j] = { { 1.0f, *i } }; }

} // end pTerm::pTerm(std::initializer_list<f4>)                              */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

std::string dp::SimpleTerm::ToString(const std::string& vLabel, bool lead) const
{
  std::string result;
  f4 cCopy = coeff;
  if (!lead)
  {
    if (cCopy > 0.0f) { result = " + "; }
    else { SetAbsF(cCopy); result = " - "; }
  }
  if (!IsIntegralF(cCopy)) { result += std::to_string(cCopy); }
  else
  {
    s4 co = FloorF(cCopy);
    if (co == 0) { return "0"; }
    if (co == 1)
    {
      if (power == 0.0) { return result + "1"; }
      if (power == 1.0) { return result + vLabel; }
    }
    else { result += std::to_string(co); }
  }
  if (!IsIntegralF(power)) { result += "^{" + std::to_string(power) + "}"; }
  else
  {
    s4 exp = FloorF(power);
    if (exp > 0)
    {
      result += vLabel;
      if (exp > 1) { result += "^{" + std::to_string(exp) + "}"; }
    }
  }
  return result;

} // end std::string SimpleTerm::ToString(const std::string&, bool) const     */

std::string dp::pTerm::ToString(std::vector<std::string>& label, bool lead)
const
{
  // Validate / format inputs
  label.resize(factor.size());
  u4 unlist = 0;
  for (u4 in = 0; in < label.size(); ++in)
  {
    if (label[in].length() == 0)
    {
      label[in] = "x_" + std::to_string(unlist++);
    }
  }
  // Then start the hairy business of variable specifiers in polynomial output
  std::string result;
  // Output leading coefficient
  if (!lead) // prepend sign as needed by term position in polynomial
  {
    // Only possible to subtract the term if 1-D & negative, otherwise adding
    result = (coeff.IsScalar() && coeff[0] < 0.0f) ? " - " : " + ";
  }
  // (Checking for negative scalar leading coefficient exception...)
  if (coeff.IsScalar() && coeff[0] < 0.0f) // if simple negation logged
  {
    // Take absolute value of first term to preclude double negation
    if (!IsIntegralF(coeff[0])) { result += std::to_string(AbsF(coeff[0])); }
    else { result += std::to_string(FloorF(AbsF(coeff[0]))); }
  }
  else if (!coeff.IsEmpty()) // Note no coeff if none given (1.0 implicit)
  {
    // Outputting vector formatting if non scalar
    if (coeff.IsNonScalar())
    {
      if (coeff.IsNull()) { return ""; }
      result += coeff.VecString();
    } // end leading coefficient vector check
    else // scalar leading coefficient
    {
      // If a scalar coefficient is non-integer, it always gets output
      if (!IsIntegralF(coeff[0])) { result += std::to_string(coeff[0]); }
      else // Otherwise simplify integer output even more than normal
      {
        s4 co = FloorF(coeff[0]);
        // First, if the coefficient is 0, the term is nulled out
        if (co == 0) { return ""; }
        // Then if it isn't exactly 1, the coefficient may be output
        if (co != 1.0f) { result += std::to_string(co); }
        else // But if it is exactly 1
        {
          // Check each variable until any non-zero power is found
          bool constantTerm = true;
          for (u4 i = 0; i < factor.size(); ++i)
          {
            for (u4 j = 0; j < factor[i].Size(); ++j)
            {
              if (factor[i][j].exponent != 0.0f)
              {
                // If any is found, the leading 1 is implicit and ommitted
                constantTerm = false;
                break;
              }
            }
            if (!constantTerm) { break; }
          }
          // But then if none were found, the term is constant and it is added
          if (constantTerm)
          {
            result += "1";
          }
        }
      }
    }
  } // end non-null leading coefficient branch
  std::string vLabel;

  for (u4 i = 0; i < factor.size(); ++i) // Then output factor list
  {
    vLabel = (label[i].length() == 1 ? label[i] : ("(" + label[i] + ")"));
    if (factor[i].Size() == 1) // Simpler: 1 term basis for the term's factor
    {
      if (factor[i][0].coeff != 1.0f) // Non-int coeff slightly simpler
      {
        if (!IsIntegralF(factor[i][0].coeff))
        {
          result += "(" + std::to_string(factor[i][0].coeff);
        }
        else
        {
          result += "(" + std::to_string(FloorF(factor[i][0].coeff));
        }
        if (factor[i][0].exponent != 0.0f) // If non-constant, add variable
        {
          result += vLabel;
          if (factor[i][0].exponent != 1.0f) // If non-linear, exponentiate
          {
            result += "^{";
            if (IsIntegralF(factor[i][0].exponent))
            {
              result += std::to_string(FloorF(factor[i][0].exponent));
            }
            else { result += std::to_string(factor[i][0].exponent); }
            result += "}";
          }
        }
        result += ")";
      } // end factor / variable's simple integer coefficient
      else // Integer coefficient for the term's variable basis term is present
      {
        s4 co = FloorF(factor[i][0].coeff);
        if (co == 0) { return ""; }
        if (co != 1) { result += "(" + std::to_string(co); }
        if (factor[i][0].exponent != 0.0f)
        {
          result += vLabel;
          if (factor[i][0].exponent != 1.0f) // If non-linear, exponentiate
          {
            result += "^{";
            if (IsIntegralF(factor[i][0].exponent))
            {
              result += std::to_string(FloorF(factor[i][0].exponent));
            }
            else { result += std::to_string(factor[i][0].exponent); }
            result += "}";
          }
        }
        if (co != 1) { result += ")"; }
      }
    } // end simple variable basis (1 standard basis term)
    else // Otherwise the term's basis for the variable has multiple terms
    {
      result += "(" + factor[i][0].ToString(vLabel, 1);
      for (u4 j = 1; j < factor[i].Size(); ++j)
      {
        result += factor[i][j].ToString(vLabel, 0);
      }
      result += ")";
    }
  }
  return result;

} // end std::string pTerm::ToString(std::vector<std::string>) const          */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Overloads                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

std::ostream& dp::operator<<(std::ostream& o, const std::vector<SimpleTerm>& p)
{
  if (p.size() > 0)
  {
    if (p[0].coeff != 1.0f) { o << p[0].coeff; }
    else if (p[0].power == 0.0f) { o << p[0].coeff; }
    if (p[0].power == 0.0f) {}
    else if (p[0].power == 1.0f) { o << "x"; }
    else { o << "x^{" << p[0].power << "}"; }
    for (u4 c = 1; c < p.size(); ++c)
    {
      o << (p[c].coeff > 0 ? " + " : " - ");
      f4 cAbs = AbsF(p[c].coeff);
      if (cAbs != 1.0f) { o << cAbs; }
      else if (p[c].power == 0.0f) { o << cAbs; }
      if (p[c].power == 0.0f) {}
      else if (p[c].power == 1.0f) { o << "x"; }
      else { o << "x^{" << p[c].power << "}"; }
    }
  }
  else { o << "0"; }
  return o;

} // end ostream& operator<<(std::ostream&, const std::vector<SimpleTerm>&)   */
