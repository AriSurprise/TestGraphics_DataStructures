/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  File.cpp
Purpose:  File parser to get string data from (currently, just ascii) files
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (dirent.h for directory read presumed windows excl.)
Project:  a.Surprise_MAT320_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Fall 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <c>
#include <stdio.h>      // C File I/O is just usually better practice than C++
// "../Packages/..."
#ifdef _WIN32
#include "../../Packages/include/dirent.h"
#endif
// "./src/..."
#include "ASCIIValue.h" // string / character evaluation / corrections
#include "File.h"       // Class declaration header file
#include "Log.h"        // Error / warning output / message logging
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Static Constants                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

//! Fetch a page worth of bytes per system call: generally optimal behavior
constexpr u2 PAGE_BYTES = 4096;

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                      Static Variable Initializations                       */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                           Pubilc Static Methods                            */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

bool dp::File::Exists(const dp::ASCIIPath& fileName)
{
  std::string buffer = fileName;
  FILE* in;
  fopen_s(&in, buffer.c_str(), "r");
  if (in)
  {
    fclose(in);
    return true;
  }
  return false;

} // end bool File::Exists(const ASCIIPath&)                                  */


std::string dp::File::ReadAtOnce(const dp::ASCIIPath& fileName)
{
  std::string buffer = fileName;
  FILE* in;
  fopen_s(&in, buffer.c_str(), "r");
  if (in)
  {
    buffer.clear();
    char line[PAGE_BYTES];
    char* s = fgets(line, PAGE_BYTES, in);
    while (s)
    {
      buffer += s;
      s = fgets(line, PAGE_BYTES, in);
    }
    fclose(in);
  }
  else
  {
    Log::Warn("File::AtOnce - " + buffer + " couldn't be opened");
    buffer.clear();
  }
  return buffer;

} // end std::string File::ReadAtOnce(const ASCIIPath&)                       */


std::vector<std::string> dp::File::TextList(const std::string& searchFolder,
  const std::string& extension)
{
  std::vector<std::string> list;
#ifdef _WIN32
  struct dirent** files;
  std::string extLower = ToLower(extension);
  int contents = scandir(searchFolder.c_str(), &files, NULL, alphasort);
  if (contents > 0)
  {
    ASCIIPath current("./");
    std::string cExt;
    for (int i = 0; i < contents; ++i)
    {
      switch (files[i]->d_type)
      {
      case DT_REG:
        current = ASCIIPath(searchFolder + files[i]->d_name);
        cExt = ToLower(current.Extension());
        if (extension == "*" || cExt == extLower)
        {
          list.push_back(File::ReadAtOnce(current));
        }
        break;
      default:
        break;
      }
    } // end folder contents scan
  }// end folder exists / has contents validation check
  else
  {
    Log::Info("File::TextList - No matches in '" + searchFolder + "'");
  }
#else
  Log::Warn("File::TextList - platform not supported for directory read");
#endif
  return list;

} // end std::vector<string> TextList(const string&, const string&)           */


std::vector<dp::ShaderFile> dp::File::ShaderList(const std::string&
  searchFolder, const std::string& extension)
{
  std::vector<ShaderFile> list;
#ifdef _WIN32
  struct dirent** files;
  std::string extLower = ToLower(extension);
  int contents = scandir(searchFolder.c_str(), &files, NULL, alphasort);
  if (contents > 0)
  {
    ASCIIPath current("./");
    std::string cExt;
    for (int i = 0; i < contents; ++i)
    {
      switch (files[i]->d_type)
      {
      case DT_REG:
        current = ASCIIPath(searchFolder + files[i]->d_name);
        cExt = ToLower(current.Extension());
        if (extension == "*" || cExt == extLower)
        {
          list.push_back(ShaderFile(current));
        }
        break;
      default:
        break;
      }
    } // end folder contents scan
  }// end folder exists / has contents validation check
  else
  {
    Log::Info("File::ShaderList - No matches in '" + searchFolder + "'");
  }
#else
  Log::Warn("File::ShaderList - platform not supported for directory read");
#endif
  return list;

} // end std::vector<ShaderFile> ShaderList(const string&, const string&)     */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Pubilc Methods                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Private Static Methods                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
