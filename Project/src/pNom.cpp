/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  vNf.cpp
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
#include <ostream>  // Output stream definition for operator overload
// "./src/..."
#include "ASCIIValue.h" // Variable label evaluation as pattern conformant
#include "Log.h"        // System message management for errors, warnings, etc
#include "nCr.h"        // Pascal triangle values
#include "Value.h"      // Basic value comparisons: min, max, clamp, near, etc
#include "pNom.h"       // Class declaration header


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Consts                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::pNom::pNom(const std::vector<pTerm>& termList,
  const std::vector<std::string>& labels)
  : term(termList)
{
  SetLabels(labels);

} // end pNom::pNom((std::vector<pTerm>&), (std::vector<std::string>&))       */

dp::pNom::pNom(const std::vector<pTerm>& termList,
  std::initializer_list<const char*> labels)
  : term(termList)
{
  SetLabels(labels);

} // end pNom::pNom(vector<pTerm>&, (initializer_list<const char*>))          */

dp::pNom::pNom(std::initializer_list<pTerm> termList,
  std::initializer_list<const char*> labels)
  : term(termList), label(labels.begin(), labels.end())
{
  // pTerms can be initialized many ways: check for oversimplified input
  if (term.size() > 0)
  {
    f4 pow;
    if (term[0].factor.size() > 0) // Some input basis given on first term...
    {
      if (term[0].factor[0].Size() > 0)
      {
        std::cout << "yeh: " << term.size() << " " << term[0].factor.size()
          << " " << term[0].factor[0].Size() << std::endl;
        pow = term[0].factor[0][0].exponent;
        std::cout << pow << std::endl;
      }
    }
    else // No variable basis given on first term: only leading coefficient
    {
      bool all = true;
      // Check if this is true across all terms
      for (u4 i = 1; i < term.size(); ++i)
      {
        if (term[i].factor.size() > 0) { all = false; break; }
      }
      // If so...
      if (all)
      {
        if (label.size() == 1) // 1 variable indicative of ascending terms
        {
          for (u4 i = 0; i < term.size(); ++i)
          {
            term[i].factor.resize(1);
            term[i].factor[0].Resize(1);
            term[i].factor[0][0] = { 1.0f, static_cast<f4>(i) };
          }
        }
        // elsewise what pattern even will fit as variables increase...?
      }
    }
  }
  SetLabels(labels);

} // end pNom::pNom(vector<pTerm>&, (initializer_list<const char*>))          */

dp::pNom::pNom(CoeffPattern type, std::initializer_list<f4> lCoeffs,
  const char* vLabel)
{
  label.resize(1);
  label[0] = (strlen(vLabel) > 0 ? vLabel : "x");
  term.resize(lCoeffs.size());
  f4 co, exp = 0.0f;
  u1 d, i, j;
  std::vector<u8> basis;
  bool osc;
  switch (type)
  {
  case CoeffPattern::Oscillating:
  case CoeffPattern::OscPascal:
  case CoeffPattern::OscBernstein:
  case CoeffPattern::OscCumlBern: osc = true; break;
  default: osc = false; break;
  }
  for (i = 0; i < lCoeffs.size(); ++i)
  {
    term[i].coeff.Resize(1);
    term[i].coeff[0] = (osc && i & 1) ? -lCoeffs.begin()[i] : lCoeffs.begin()[i];
  }
  switch (type)
  {
  case CoeffPattern::OscBernstein:
  case CoeffPattern::Bernstein:
    d = (u1)(term.size() - 1u);
    for (i = 0; i < term.size(); ++i)
    {
      basis = n::Choices(static_cast<u1>(d - i));
      term[i].factor.resize(1);
      term[i].factor[0].Resize(static_cast<u4>(basis.size()));
      exp = 0.0f;
      for (j = 0; j < term[i].factor[0].Size(); ++j)
      {
        co = (f4)(n::Cr(d, i) * basis[j]);
        term[i].factor[0][j] = { (j&1)? -co : co, i + exp++};
      }
    }
    break;
  case CoeffPattern::OscPascal:
  case CoeffPattern::Pascal:
    basis = n::Choices(static_cast<u1>(term.size() - 1));
    for (i = 0; i < term.size(); ++i)
    {
      term[i].factor.resize(1);
      term[i].factor[0].Resize(1);
      term[i].factor[0][0] = { (f4)basis[i], exp++ };
    }
    break;
  case CoeffPattern::Oscillating:
  case CoeffPattern::Constant: default:
    for (i = 0; i < term.size(); ++i)
    {
      term[i].factor.resize(1);
      term[i].factor[0].Resize(1);
      term[i].factor[0][0] = { 1.0f, exp++ };
    }
    break;
  }
}

dp::pNom::pNom(CoeffPattern type, std::initializer_list<vNf> lCoeffs,
  const char* vLabel)
{
  label.resize(1);
  label[0] = (strlen(vLabel) > 0 ? vLabel : "x");
  term.resize(lCoeffs.size());
  f4 exp = 0.0f;
  u1 i;
  std::vector<u8> basis;
  for (i = 0; i < lCoeffs.size(); ++i)
  {
    term[i].coeff = lCoeffs.begin()[i];
  }
  switch (type)
  {
  case CoeffPattern::Bernstein:
    for (i = 0; i < term.size(); ++i)
    {
    }
    break;
  case CoeffPattern::OscPascal:
    basis = n::Choices(static_cast<u1>(term.size() - 1));
    for (i = 0; i < term.size(); ++i)
    {
      term[i].factor.resize(1);
      term[i].factor[0].Resize(1, (f4)(i & 1 ? basis[i] : -(s8)basis[i]), exp++);
    }
    break;
  case CoeffPattern::Pascal:
    basis = n::Choices(static_cast<u1>(term.size() - 1));
    for (i = 0; i < term.size(); ++i)
    {
      term[i].factor.resize(1);
      term[i].factor[0].Resize(1, (f4)basis[i], exp++);
    }
    break;
  case CoeffPattern::Oscillating:
    for (i = 0; i < term.size(); ++i)
    {
      term[i].factor.resize(1);
      term[i].factor[0].Resize(1, (i & 1 ? 1.0f : -1.0f), exp++);
    }
    break;
  case CoeffPattern::Constant: default:
    for (i = 0; i < term.size(); ++i)
    {
      term[i].factor.resize(1);
      term[i].factor[0].Resize(1);
      term[i].factor[0][0] = { 1.0f, exp++ };
    }
    break;
  }
} // end pNom::pNom(CoeffPattern, std::initializer_list<vNf>, const char*)    */

dp::pNom::pNom(const pNom& other) : label(other.label), term(other.term)
{} // end pNom::pNom(const pNom&)                                             */

dp::pNom::pNom(pNom&& result) noexcept : label(std::move(result.label)),
  term(std::move(result.term))
{} // end pNom::pNom(pNom&&)                                                  */

dp::pNom& dp::pNom::SetLabels(const std::vector<std::string>& labels)
{
  label.clear();
  for (u4 i = 0; i < labels.size(); ++i)
  {
    if (labels[i].length() > 0)
    {
      bool unique = true;
      for (u4 j = 0; j < label.size(); ++j)
      {
        if (label[j] == labels[i])
        {
          unique = false;
          break;
        }
      }
      if (unique) { label.push_back(labels[i]); }
    }
  }
  um max = 0;
  for (um i = 0; i < term.size(); ++i)
  {
    if (term[i].factor.size() > max) { max = term[i].factor.size(); }
  }
  ExtendLabels(max);
  ConformBasis();
  return *this;

} // end pNom& pNom::SetLabels(std::initializer_list<const char*>)            */

dp::pNom& dp::pNom::SetLabels(std::initializer_list<const char*> labels)
{
  label.clear();
  bool unique = true;
  for (u4 i = 0; i < labels.size(); ++i)
  {
    if (strlen(labels.begin()[i]) > 0)
    {
      for (um j = 0; j < label.size(); ++j)
      {
        unique = true;
        if (label[j] == labels.begin()[i])
        {
          unique = false;
          break;
        }
      }
      if (unique) { label.push_back(labels.begin()[i]); }
    }
  }
  um max = 0;
  for (um i = 0; i < term.size(); ++i)
  {
    if (term[i].factor.size() > max) { max = term[i].factor.size(); }
  }
  ExtendLabels(max);
  ConformBasis();
  return *this;

} // end pNom& pNom::SetLabels(std::initializer_list<const char*>)            */

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

std::string dp::pNom::ToString(void)
{
  std::string result;
  if (Terms() < 1) { return "0"; }
  result += term[0].ToString(label, true);
  for (u4 i=1; i < term.size(); ++i)
  {
    result += term[i].ToString(label, false);
  }
  return result;

} // end std::string pNom::ToString(void) const                               */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::pNom dp::pNom::operator-(void) const
{
  pNom copy(*this);
  // Negate the leading coefficient of each term in the polynomial copy
  for (u4 i = 0; i < Terms(); ++i)
  {
    copy.term[i].coeff = -copy.term[i].coeff;
  }
  // Return the result
  return copy;
} // end pNom pNom::operator-(void) const                                     */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Overloads                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

void dp::pNom::ConformBasis(void)
{
  um lSize = label.size(), prevSize, i, j; // Get listed variable size
  // Go over each term in the polynomial
  for (i = 0; i < term.size(); ++i)
  {
    // Conform each variable in the term to size for given variable list
    if (term[i].factor.size() != lSize)
    {
      // Either by simple truncation of extraneous data
      if (term[i].factor.size() > lSize) { term[i].factor.resize(lSize); }
      // Or by inserting zero values per variable
      else
      {
        prevSize = term[i].factor.size();
        term[i].factor.resize(lSize);
        for (j = prevSize; j < term[i].factor.size(); ++j)
        {
          term[i].factor[j].Resize(1, 1.0f);
        }
      }
    }
  }
} // end void pNom::ConformBasis(void)                                        */

bool dp::pNom::DefaultLabelConformant(um* offset) const
{
  if (label.size() <= dPNomL.size())
  {
    um i = 0;
    um offs; // buffer for offset to use if none provided to pass in pointer
    if (!offset) { offset = &offs; }
    *offset = dPNomL.size() - i;
    // If the labels don't begin with the expected start, check for an offset
    if (label[0] != "x")
    {
      std::string id = ToLower(label[0]);
      um len = id.length();
      if (len > 1 || (len == 1 && !IsLower(id[0]))) { return false; }
      // Cycling ahead to wherever a match occurs
      for (; i < dPNomL.size(); ++i)
      {
        if (label[0] == dPNomL[i]) { break; }
      }
      if (i == dPNomL.size()) { return false; }
    }
    *offset = i;
    // Advancing through the remainder of the default set from any offset
    for (i = 0; i < label.size() && i + *offset < dPNomL.size(); ++i)
    {
      if (label[i] != dPNomL[i+*offset]) { return false; }
    }
    // And again, from the beginning up to that offset point (if any)
    for (u4 j = 0; i < label.size() && j < *offset; ++i, ++j)
    {
      if (label[i] != dPNomL[j]) { return false; }
    }
  }
  else
  {
    // ...find some iterative perpetuation of the pattern...? out of scope
    return false;
    for (u4 i = 0; i < dPNomL.size(); ++i)
    {
      if (label[i] != dPNomL[i]) { return false; }
    }
  }
  return true;

} // end pNom::DefaultLabelConformant(void) const

void dp::pNom::ExtendLabels(um variables, um* offset)
{
  um off, prev = label.size(), i;
  bool defaults = (offset == nullptr);
  if (!offset) // If no offset was calculated from existing values, find it
  {
    offset = &off;
    defaults = DefaultLabelConformant(offset);
  }
  if (defaults)
  {
    label.resize(variables); // conform basis labels to new count
    if (!prev) // If no previous label list exists, copy the defaults
    {
      for (i = 0; i < variables; ++i) { label[i] = dPNomL[i]; }
      return;
    }
    // Iterate from prior size to assign default variable labels for new ones
    for (i = variables - 1; i >= prev; --i)
    {
      label[i] = dPNomL[((i + *offset) % dPNomL.size())];
    }
  }
  else
  {
    u4 unlist = 0;
    std::string v = "x_";
    bool found = false;
    for (i = label.size()-1; i > 0; --i)
    {
      if (label[i].length() == 1 || label[i].find('_') == 1)
      {
        v = label[i].substr(0, 1) + "_";
        label[i] = label[i].substr(0,1) + "_" + std::to_string(unlist++);
        found = true;
        break;
      }
    }
    if (!found && label.size() > 0)
    {
      if (label[0].length() == 1 || label[0].find('_') == 1)
      {
        v = label[0].substr(0, 1) + "_";
        label[0] = label[0].substr(0,1) + "_" + std::to_string(unlist++);
      }
    }
    label.resize(variables); // conform basis labels to new count
    for (u4 in = 0; in < label.size(); ++in)
    {
      if (label[in].length() == 0)
      {
        label[in] = v + std::to_string(unlist++);
      }
    }
  }

} // end void pNom::ExtendDefaultLabel(s4, u4*)
