/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  ASCIIValue.cpp
Purpose:  Character evaluation functions to convert in / out of ascii values
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
#include "ASCIIValue.h" // Function declaration header file
#include "Log.h"        // Error / warning message output / logging
#include "Value.h"      // Generic numeric value manipulations

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Public Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

std::string dp::ASCIII(s4 value)
{
  std::string result;
  s4 decr;
  s4 ddct = 1000000000;
  s4 rad = 10;
  bool inNums = false;
  if (value < 0) { result = '-'; SetAbsI(value); }
  if (value >= ddct)
  {
    decr = ddct * (rad - 1);
    while (decr > ddct)
    {
      if (value >= decr) { result += '9'; value -= decr; break; }
      decr -= ddct;
    }
    ddct /= rad;
    inNums = true;
  }
  if (value >= ddct)
  {
    if (value >= 900000000) { result += "9"; value -= 900000000; }
    else if (value >= 800000000) { result += "8"; value -= 800000000; }
    else if (value >= 700000000) { result += "7"; value -= 700000000; }
    else if (value >= 600000000) { result += "6"; value -= 600000000; }
    else if (value >= 500000000) { result += "5"; value -= 500000000; }
    else if (value >= 400000000) { result += "4"; value -= 400000000; }
    else if (value >= 300000000) { result += "3"; value -= 300000000; }
    else if (value >= 200000000) { result += "2"; value -= 200000000; }
    else { result += "1"; value -= 100000000; }
    ddct /= rad;
    inNums = true;
  }
  else if (inNums) { result += "0"; }
  if (value >= ddct)
  {
    if (value >= 90000000) { result += "9"; value -= 90000000; }
    else if (value >= 80000000) { result += "8"; value -= 80000000; }
    else if (value >= 70000000) { result += "7"; value -= 70000000; }
    else if (value >= 60000000) { result += "6"; value -= 60000000; }
    else if (value >= 50000000) { result += "5"; value -= 50000000; }
    else if (value >= 40000000) { result += "4"; value -= 40000000; }
    else if (value >= 30000000) { result += "3"; value -= 30000000; }
    else if (value >= 20000000) { result += "2"; value -= 20000000; }
    else { result += "1"; value -= 10000000; }
    ddct /= rad;
    inNums = true;
  }
  else if (inNums) { result += "0"; ddct /= rad; }
  if (value >= ddct)
  {
    if (value >= 9000000) { result += "9"; value -= 9000000; }
    else if (value >= 8000000) { result += "8"; value -= 8000000; }
    else if (value >= 7000000) { result += "7"; value -= 7000000; }
    else if (value >= 6000000) { result += "6"; value -= 6000000; }
    else if (value >= 5000000) { result += "5"; value -= 5000000; }
    else if (value >= 4000000) { result += "4"; value -= 4000000; }
    else if (value >= 3000000) { result += "3"; value -= 3000000; }
    else if (value >= 2000000) { result += "2"; value -= 2000000; }
    else { result += "1"; value -= 1000000; }
    ddct /= rad;
    inNums = true;
  }
  else if (inNums) { result += "0"; ddct /= rad; }
  if (value >= ddct)
  {
    if (value >= 900000) { result += "9"; value -= 900000; }
    else if (value >= 800000) { result += "8"; value -= 800000; }
    else if (value >= 700000) { result += "7"; value -= 700000; }
    else if (value >= 600000) { result += "6"; value -= 600000; }
    else if (value >= 500000) { result += "5"; value -= 500000; }
    else if (value >= 400000) { result += "4"; value -= 400000; }
    else if (value >= 300000) { result += "3"; value -= 300000; }
    else if (value >= 200000) { result += "2"; value -= 200000; }
    else { result += "1"; value -= 100000; }
    ddct /= rad;
    inNums = true;
  }
  else if (inNums) { result += "0"; ddct /= rad; }
  if (value >= ddct)
  {
    if (value >= 90000) { result += "9"; value -= 900000; }
    else if (value >= 80000) { result += "8"; value -= 80000; }
    else if (value >= 70000) { result += "7"; value -= 70000; }
    else if (value >= 60000) { result += "6"; value -= 60000; }
    else if (value >= 50000) { result += "5"; value -= 50000; }
    else if (value >= 40000) { result += "4"; value -= 40000; }
    else if (value >= 30000) { result += "3"; value -= 30000; }
    else if (value >= 20000) { result += "2"; value -= 20000; }
    else { result += "1"; value -= 10000; }
    ddct /= rad;
    inNums = true;
  }
  else if (inNums) { result += "0"; ddct /= rad; }
  if (value >= ddct)
  {
    if (value >= 9000) { result += "9"; value -= 90000; }
    else if (value >= 8000) { result += "8"; value -= 8000; }
    else if (value >= 7000) { result += "7"; value -= 7000; }
    else if (value >= 6000) { result += "6"; value -= 6000; }
    else if (value >= 5000) { result += "5"; value -= 5000; }
    else if (value >= 4000) { result += "4"; value -= 4000; }
    else if (value >= 3000) { result += "3"; value -= 3000; }
    else if (value >= 2000) { result += "2"; value -= 2000; }
    else { result += "1"; value -= 1000; }
    ddct /= rad;
    inNums = true;
  }
  else if (inNums) { result += "0"; ddct /= rad; }
  if (value >= ddct)
  {
    if (value >= 900) { result += "9"; value -= 9000; }
    else if (value >= 800) { result += "8"; value -= 800; }
    else if (value >= 700) { result += "7"; value -= 700; }
    else if (value >= 600) { result += "6"; value -= 600; }
    else if (value >= 500) { result += "5"; value -= 500; }
    else if (value >= 400) { result += "4"; value -= 400; }
    else if (value >= 300) { result += "3"; value -= 300; }
    else if (value >= 200) { result += "2"; value -= 200; }
    else { result += "1"; value -= 100; }
    ddct /= rad;
    inNums = true;
  }
  else if (inNums) { result += "0"; ddct /= rad; }
  if (value >= ddct)
  {
    if (value >= 900) { result += "9"; value -= 900; }
    else if (value >= 800) { result += "8"; value -= 800; }
    else if (value >= 700) { result += "7"; value -= 700; }
    else if (value >= 600) { result += "6"; value -= 600; }
    else if (value >= 500) { result += "5"; value -= 500; }
    else if (value >= 400) { result += "4"; value -= 400; }
    else if (value >= 300) { result += "3"; value -= 300; }
    else if (value >= 200) { result += "2"; value -= 200; }
    else { result += "1"; value -= 100; }
    ddct /= rad;
    inNums = true;
  }
  else if (inNums) { result += "0"; ddct /= rad; }
  if (value >= ddct)
  {
    if (value >= 90) { result += "9"; value -= 90; }
    else if (value >= 80) { result += "8"; value -= 80; }
    else if (value >= 70) { result += "7"; value -= 70; }
    else if (value >= 60) { result += "6"; value -= 60; }
    else if (value >= 50) { result += "5"; value -= 50; }
    else if (value >= 40) { result += "4"; value -= 40; }
    else if (value >= 30) { result += "3"; value -= 30; }
    else if (value >= 20) { result += "2"; value -= 20; }
    else { result += "1"; value -= 10; }
    ddct /= rad;
    inNums = true;
  }
  else if (inNums) { result += "0"; ddct /= rad; }
  if (value >= ddct)
  {
    if (value >= 9) { result += "9"; value -= 9; }
    else if (value >= 8) { result += "8"; value -= 8; }
    else if (value >= 7) { result += "7"; value -= 7; }
    else if (value >= 6) { result += "6"; value -= 6; }
    else if (value >= 5) { result += "5"; value -= 5; }
    else if (value >= 4) { result += "4"; value -= 4; }
    else if (value >= 3) { result += "3"; value -= 3; }
    else if (value >= 2) { result += "2"; value -= 2; }
    else { result += "1"; value -= 1; }
    ddct /= rad;
    inNums = true;
  }
  else if (inNums) { result += "0"; ddct /= rad; }
  return result;

} // end std::string ASCIII(s4)                                               */


// Get the string portion which is float point conformant (if any)            */
std::string dp::FloatSubstr(const std::string& s, size_t* index)
{
  std::string validated;
  bool part = false; // An existing partition (decimal), being emplaced already
  if (s.length() == 0) { if (index) { *index = 0; } return ""; }
  else if (s[0] == '.')
  {
    if (s.length() > 1) { part = true; validated = "0."; }
    else { if (index) { *index = 0; } return ""; }
  } // end leading decimal partition check
  else if (s[0] == '-')
  {
    if (s.length() > 1) { validated = "-"; }
    else { if (index) { *index = 0; } return ""; }
  } // end leading negation check
  else if (IsDigit(s[0]))
  {
    validated.push_back(s[0]);
  }
  for (size_t i = 1; i < s.length(); ++i)
  {
    if (s[i] == '.')
    {
      if (!part)
      {
        part = true;
        if (i + 1 == s.length()) // trailing '.' check: integral float value
        {
          validated += ".0";
          if (index) { *index = i + 1; }
          break;
        } // end trailing '.' check
        else if (!IsDigit(s[i+1])) // malformed trailing '.' check
        {
          validated += ".0";
          if (index) { *index = i + 1; }
          break;
        } // end malformed '.' check
        else { validated.push_back('.'); }
      } // end partition insertion
      // else: ignore subsequent decimals; pass-through
    }
    else if (IsDigit(s[i]))
    {
      validated.push_back(s[i]);
      if (index) { *index = i+1; }
    }
    else { break; }
  }
  return validated;
} // end std::string FloatSubstr(const std::string&, size_t*)


// Get the string portion which is signed decimal conformant (if any)         */
std::string dp::SignedSubstr(const std::string& s, size_t* index)
{
  std::string validated;
  if (s.length() == 0) { if (index) { *index = 0; } return ""; }
  else if (s[0] == '-')
  {
    if (s.length() > 1) { validated = "-"; }
    else { if (index) { *index = 0; } return ""; }
  } // end leading negation check
  else if (IsDigit(s[0])) {
    validated.push_back(s[0]);
  }
  for (size_t i = 1; i < s.length(); ++i)
  {
    if (IsDigit(s[i]))
    {
      validated.push_back(s[i]);
      if (index) { *index = i + 1; }
    }
    else { break; }
  }
  return validated;
} // end std::string SignedSubstr(const std::string&, size_t*)


// Get the string portion which is signed decimal conformant (if any)         */
std::string dp::UnsignedSubstr(const std::string& s, size_t* index)
{
  std::string validated = "";
  if (s.length() == 0) { if (index) { *index = 0; } return validated; }
  else if (IsDigit(s[0])) {
    validated.push_back(s[0]);
  }
  for (size_t i = 1; i < s.length(); ++i)
  {
    if (IsDigit(s[i]))
    {
      validated.push_back(s[i]);
      if (index) { *index = i + 1; }
    }
    else { break; }
  }
  return validated;
} // end std::string UnsignedSubstr(const std::string&, size_t*)

// Report whether char is an ascii alphabetic character (either case)         */
bool dp::IsAlpha(char c)
{ return (IsUpper(c) || IsLower(c)); } // end bool IsAlpha(char)


// Report whether char is an ascii alphabetic character (either case)         */
bool dp::IsAlphaNumeric(char c)
{ return (IsDigit(c) || IsAlpha(c)); } // end bool IsAlphaNumeric(char)


// Report whether a char is an ascii hexidecimal character range              */
bool dp::IsHex(char c)
{
  return (IsDigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
} // end bool IsHex(char)


// Report whether a given char is an ascii path dividing character (\ or /)   */
bool dp::IsAnyPathDivChar(char c)
{ return (c == '/' || c == '\\'); } // end bool IsAnyPathDivChar(char)


// Report whether char is an ascii, lower case alphabetic character value     */
bool dp::IsLower(char c)
{ return (c >= 'a' && c <= 'z'); } // end bool IsLower(char)


// Report whether char is an ascii, decimal, numeric character value          */
bool dp::IsDigit(char c)
{ return (c >= '0' && c <= '9'); } // end bool IsDigit(char)

bool dp::IsUnsignedD(std::string s)
{
  for (char c : s) { if (!IsDigit(c)) { return false; } }
  return true; // else: all digits
} // end bool IsUnsignedD(std::string)


// Report on whether a char is unbarred from being most valid characters      */
bool dp::IsPathLegal(char c)
{
  // Allowed path characters work by blacklists: whitelists non-viable
  if (c < ' ' || c > '~') { return false; }
  switch (c)
  {
    // '\', '/' and ':' considered valid for these tests' functions
  case '?': case '*': case '|': case '"': case '<': case '>':
    return false;
  default:
    return true;
  }
} // end bool IsPathLegal(const char)


// Report on whether a char is unbarred from being any valid path character   */
bool dp::IsPathLegalStrict(char c)
{
  // Allowed path characters work by blacklists: whitelists non-viable
  if (c < ' ' || c > '~') { return false; }
  switch (c)
  {
  case '\\':
  case '/':
  case ':':
    return false;
  default:
    return IsPathLegal(c);
  }
} // end bool IsPathLegal(const char)


// Report whether char is an ascii, upper case alphabetic character value     */
bool dp::IsUpper(char c)
{ return (c >= 'A' && c <= 'Z'); } // end bool IsUpper(char)


// Get a line out of a block of text from offset onward to a break point      */
std::string dp::StringLine(const std::string& textBody, size_t& offset,
  size_t limit, bool wordWrap, char delim1, char delim2)
{
  std::string subs(textBody.c_str() + offset);
  size_t end = 0;
  // Consume empty lines (return at all once to signify the consumed lines)
  if (end < subs.length() && (subs[end] == delim1 || subs[end] == delim2))
  {
    do { ++end; }
    while (end < subs.length() && (subs[end]==delim1 || subs[end]==delim2));
    if (end >= subs.length()) { offset += subs.length(); }
    else
    {
      offset += end;
      subs = subs.substr(0, end);
      return subs;
    }
  }
  if (limit == 0)
  {
    if (delim2 == '\0')
    {
      for (size_t i = 0; i < subs.length(); ++i)
      {
        if (subs[i] == '\0' || subs[i] == delim1) { end = i; break; }
      }
    }
    else
    {
      for (size_t i = 0; i < subs.length(); ++i)
      {
        if(subs[i]=='\0' || subs[i]==delim1 || subs[i]==delim2) {end=i;break;}
      }
    }
  }
  else
  {
    if (delim2 == '\0')
    {
      for (size_t i = 0; i < subs.length(); ++i)
      {
        if (i >= limit || subs[i]=='\0' || subs[i]==delim1) { end=i; break; }
      }
    }
    else
    {
      for (size_t i = 0; i < subs.length(); ++i)
      {
        if (i >= limit || subs[i]=='\0' || subs[i]==delim1 || subs[i]==delim2)
        { end = i;  break; }
      }
    }
  }
  if (end == 0) { offset += subs.length(); }
  else
  {
    if (wordWrap)
    {
      // for partial words: rewind from found end point to last ' ' as needed
      for (size_t i = end; i != 0; --i)
      {
        if (subs[i] == ' ') { end = i; break; }
      }
    }
    offset += end+1; // increment offset past current delimiter into next line
    if (end > subs.length()) { return subs; }
    subs = subs.substr(0, end);
  }
  return subs;
} // end std::string StringLine(const std::string&, size_t&, size_t, char, char)


// Convert an ASCII set of substrings separated by a delimiter into tokens    */
std::vector<std::string> dp::TokenList(const std::string& s, char delim)
{
  const std::string str(s + delim);
  std::vector<std::string> list;
  for (size_t i = 0; i < str.length(); ++i)
  {
    if (str[i] != delim)
    {
      std::string token;
      for (size_t j = i; j < str.length(); ++j)
      {
        if (str[j] == delim)
        {
          list.push_back(token);
          i = j;
          break;
        }
        else
        {
          token.push_back(str[j]);
        }
      }
    }
  }
  return list;
}// end std::vector<std::string> TokenList(const std::string& char)


// Take an ascii char(presumably verified), and convert it to a numeral
char dp::ToNumeral(char c)
{ return (c - '0') & 0xF; } // end char ToNumeral(char c)


// Convert ASCII (presumably verified), to unsigned value [0, 4,294,967,295]  */
unsigned dp::ToUnsigned(std::string s, unsigned radix)
{
  if (radix == 0) { radix = 1; }
  else if (radix > 16) { radix = 16; }
  unsigned sum = 0;
  if (s.length() > 10) { return UINT32_MAX; }
  else if (s.length() != 0)
  {
    for (size_t i = 0; i < s.length(); ++i)
    {
      size_t c = s.length() - 1 - i;
      sum += static_cast<unsigned>(ToNumeral(s[c])) * static_cast<unsigned>
        (pow(static_cast<double>(radix), static_cast<double>((i))));
    }
  }
  return sum;
} // end unsigned ToUnsignedDec(std::string)


std::string dp::ToLower(const std::string& s)
{
  std::string lower;
  for (char i : s)
  {
    if (IsUpper(i)) { lower.push_back(std::tolower(i)); }
    else { lower.push_back(i); }
  }
  return lower;
} // end std::string ToLower(const std::string&)                              */


std::string dp::ToUpper(const std::string& s)
{
  std::string upper;
  for (char i : s)
  {
    if (IsLower(i)) { upper.push_back(std::toupper(i)); }
    else { upper.push_back(i); }
  }
  return upper;
} // end std::string ToUpper(const std::string&)                              */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
