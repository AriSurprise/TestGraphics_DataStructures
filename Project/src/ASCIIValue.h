/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  ASCIIValue.h
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

// <stl>
#include <string> // String inputs / returns for functions parsing char groups
#include <vector> // Return value of TokenList() template definition
// "./src/..."
#include "TypeErrata.h" // Numeral types for ASCII conversion


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                            Public Functions                              */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Get the ASCII string representation of the given 4 bit integer value
  \brief
    - get the ASCII string representation of the given 4 bit integer value
  \param value
    - integer 4-byte number to have its value converted to an ASCII string
  \return
    - ASCII string representation of the given signed 4-byte integer value
  */
  std::string ASCIII(s4 value);


  /** Get the string portion which is float point conformant (if any)
  \brief
    - get the string portion which is float point conformant (if any)
  \param s
    - string to be parsed for float conformant substring
  \param index
    - subscript into string to optionally store where float conformity dropped
  \return
    - float conformant substring from input string
  */
  std::string FloatSubstr(const std::string& s, size_t* index = nullptr);


  /** Get the string portion which is float point conformant (if any)
  \brief
    - get the string portion which is float point conformant (if any)
  \param s
    - string to be parsed for float conformant substring
  \param index
    - subscript into string to optionally store where float conformity dropped
  \return
    - float conformant substring from input string
  */
  std::string SignedSubstr(const std::string& s, size_t* index = nullptr);


  /** Get the string portion which is float point conformant (if any)
  \brief
    - get the string portion which is float point conformant (if any)
  \param s
    - string to be parsed for float conformant substring
  \param index
    - subscript into string to optionally store where float conformity dropped
  \return
    - float conformant substring from input string
  */
  std::string UnsignedSubstr(const std::string& s, size_t* index = nullptr);


  /** Report whether char is an ascii alphabetic character (either case)
  \brief
    - report whether char is an ascii alphabetic character (either case)
  \param c
    - character to be scanned for alphabetic value range
  \return
    - true if the char is an alphabetic value ([A,Z] or [a,z])
  */
  bool IsAlpha(char c);


  /** Report whether a char is an ascii alphanumeric character ranges
  \brief
    - report whether a char is an ascii alphanumeric character ranges
  \param c
    - character to be scanned for alphanumeric value range match
  \return
    - true if the char is an alphanumeric value ([0,9], [A,Z] or [a,z])
  */
  bool IsAlphaNumeric(char c);


  /** Report whether a char is an ascii hexidecimal character range
  \brief
    - report whether a char is an ascii hexidecimal character range
  \param c
    - character to be scanned for hexicecimal numeric value range match
  \return
    - true if the char is a hexidecimal value ([0,9], [A,F] or [a,f])
  */
  bool IsHex(char c);


  /** Report whether char is an ascii path dividing character ('\' or '/')
  \brief
    - report whether char is an ascii path dividing character ('\' or '/')
  \param c
    - character to be scanned against path illegal entries
  \return
    - true only if the char is a reserved, path dividing value ('\' or '/')
  */
  bool IsAnyPathDivChar(char c);


  /** Report whether char is an ascii, lower case alphabetic character value
  \brief
    - report whether char is an ascii, lower case alphabetic character value
  \param c
    - character to be scanned for lower alphabetic value range
  \return
    - true only if the char is a lower case alphabetic value ([a,z] range)
  */
  bool IsLower(char c);


  /** Report whether char is an ascii, decimal, numeric character value
  \brief
    - report whether char is an ascii, decimal, numeric character value
  \param c
    - character to be scanned for decimal numeric value range
  \return
    - true only if the char is a numeric decimal value ([0,9] range)
  */
  bool IsDigit(char c);


  /** Report whether a string is only ascii, decimal, numeric character value
  \brief
    - report whether a string is only ascii, decimal, numeric character value
  \details
    - only allows unsigned int [0,9] range ascii values, not commas / decimals
  \param s
    - string to be scanned sequentially for decimal numeral ascii range
  \return
    - true only if all chars are a numeric decimal value ([0,9] range)
  */
  bool IsUnsignedD(std::string s);


  /** Report on whether a char is unbarred from being most valid characters
  \brief
    - report on whether a char is unbarred from being most valid characters
  \details
    - allows '\', '/' and ':', the 3 characters being checked externally
  \param c
    - character to be scanned against path illegal entries for path validity
  \return
    - true if the char is in ascii range and not a path illegal value
  */
  bool IsPathLegal(char c);


  /** Report on whether a char is unbarred from being any valid path character
  \brief
    - report on whether a char is unbarred from being any valid path character
  \param c
    - character to be scanned against path illegal entries for path validity
  \return
    - true if the char is in ascii range and not a path illegal value
  */
  bool IsPathLegalStrict(char c);


  /** Report whether char is an ascii, upper case alphabetic character value
  \brief
    - report whether char is an ascii, upper case alphabetic character value
  \param c
    - character to be scanned for upper alphabetic value range
  \return
    - true only if the char is a upper case alphabetic value ([A,Z] range)
  */
  bool IsUpper(char c);


  /** Get a line out of a block of text from offset onward to a break point
  \brief
    - get a line out of a block of text from offset onward to a break point
  \details
    - may return a cluster of delimited characters to signify empty lines
  \param textBody
    - block of ASCII chars spanning multiple lines worth of text to be read
  \param offset
    - chars into text body to start line break search (will be set to next + 1)
  \param limit
    - maximum allowed chars (null disclusive), for string length (0: UINT32_MAX)
  \param delim1
    - First delimiting character to break on encountering (new line by default)
  \param delim1
    - Second delimiting character to break on encountering (null by default)
  \return
    - text from given offset into textBody until a break point is encountered
  */
  std::string StringLine(const std::string& textBody, size_t& offset, size_t
  limit = 0, bool wordWrap = false, char delim1 = '\n', char delim2 = '\0');


  /** Convert an ASCII set of substrings separated by a delimiter into tokens
  \brief
    - convert an ASCII set of substrings separated by a delimiter into tokens
  \param s
    - sanitized (illegal chars removed), string to be separated into tokens
  \param delim
    - ASCII char value upon which to break tokens apart when encountered
  \return
    - value within the 1 digit hexidecimal range: [0x0, 0xF], ie [0, 15]
  */
  std::vector<std::string> TokenList(const std::string& s, char delim = ' ');


  /** Take an ascii char (presumably verified), and convert it to a numeral
  \brief
    - take an ascii char (presumably verified), and convert it to a numeral
  \param c
    - character to be converted (hexidecimal presumed highest number base used)
  \return
    - value within the 1 digit hexidecimal range: [0x0, 0xF], ie [0, 15]
  */
  char ToNumeral(char c);


  /** Convert ASCII (presumably verified), to unsigned value [0, 4,294,967,295]
  \brief
    - convert ASCII (presumably verified), to unsigned value [0, 4,294,967,295]
  \param s
    - string to be converted to an unsigned value from decimal notation
  \param radix
    - input string's positional number base system (up to hexidecimal = 16)
  \return
    - value within the unsigned range: [0, 4,294,967,295]
  */
  unsigned ToUnsigned(std::string s, unsigned radix = 10);


  /** Get an ascii string with any uppercase characters converted to lowercase
  \brief
    - get an ascii string with any uppercase characters converted to lowercase
  \param s
    - string to be converted to lowercase (non-alphabetic values preserved)
  \return
    - string with all upper case characters converted to lower case
  */
  std::string ToLower(const std::string& s);


  /** Get an ascii string with any lowercase characters converted to uppercase
  \brief
    - get an ascii string with any lowercase characters converted to uppercase
  \param s
    - string to be converted to uppercase (non-alphabetic values preserved)
  \return
    - string with all upper case characters converted to upper case
  */
  std::string ToUpper(const std::string& s);


}// end dp namespace
