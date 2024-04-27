/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  ASCIIPath.cpp
Purpose:  File string parser restrictive to only ascii path chars / chunks
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

#include <string>       // ASCII input processing
#include <sys/stat.h>   // System path / file validation procedures
#include "ASCIIPath.h"  // Class header declarations file
#include "ASCIIValue.h" // Character manipulation operations
#include "Log.h"        // Logging of messages expressing program operations
//#include "dirent.h"     // Directory content read; system path fallback scans

#if defined(WIN32) || defined(_WIN32)
#include <shlobj.h>     // functionality to get windows system folder path
#elif defined (__unix__)
#include <unistd.h>     // functionality to get unix system folder paths
#include <sys/types.h>  // system function return struct field extraction
#include <pwd.h>        // pwd 'path with directory' type / functionality
#endif



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Static Constants                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Initialize Path divizion character to the platform in use
#ifdef __unix__
const char dp::ASCIIPath::PD = '/';  // Unix' standard in use
#elif defined(_WIN32) || defined(WIN32)
const char dp::ASCIIPath::PD = '\\'; // Windows' standard in use
#else
const char dp::ASCIIPath::PD = '/';  // 'universal': best fallback
#endif

// Relative path to the local folder in use (not absolute, just "./")
const std::string dp::ASCIIPath::localPath = (std::string(".") + PD);


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                      Static Variable Initializations                       */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

std::string dp::ASCIIPath::defaultFilePath = localPath;
std::string dp::ASCIIPath::defaultFileName = "a";
std::string dp::ASCIIPath::defaultFileExt = "";
std::string dp::ASCIIPath::programPath = "";
std::string dp::ASCIIPath::runningPath = "";
std::string dp::ASCIIPath::startPath = "";
std::string dp::ASCIIPath::userHome = "";
std::string dp::ASCIIPath::userAppData = "";
dp::ASCIIPath::SysLang dp::ASCIIPath::langSetting = SysLang::Default;


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create a file descriptor from the given string                             */
dp::ASCIIPath::ASCIIPath(const std::string& pathName)
  : upDir(0u), drive('.')
{
  std::string input = Sanitized(pathName); // sanitized input string of path
  size_t len = input.length(); // sanitized input string length
  // Make format uniform by prepending ".\" if not present, or "..\"
  if (len < 2 || (input[0] != '.' && input[1] != ':')
    || (input[0] == '.' && input[1] != PD && input[1] != '.'))
  {
    input = localPath + input;
    len = input.length();
  }
  // Count all present path division characters
  drive = (len > 0) ? input[0] : '.';
  bool relative = (drive == '.');
  upDir = (relative && input[1] == '.') ? 1 : 0;
  for (size_t i = 0; i < len - 1; ++i)
  {
    if (IsAnyPathDivChar(input[i]))
    {
      std::string dir = input.substr(i + 1);
      size_t next = dir.find(PD);
      if (next > 0) { dir = dir.substr(0, next); }
      if (dir == "..") { ++upDir; }
      else
      {
        chunk.push_back(dir);
      }
    }
  }
  if (chunk.size() > 0 && chunk[chunk.size() - 1].find('.') == -1
    && defaultFileExt.length() > 0)
  {
    chunk[chunk.size() - 1].push_back('.');
    chunk[chunk.size() - 1] += defaultFileExt;
  }

} // end ASCIIPath::ASCIIPath(const string&)


// Create an ASCII file path from an existing ASCII file path                 */
dp::ASCIIPath::ASCIIPath(ASCIIPath const& source)
  : upDir(source.upDir), drive(source.drive)
{ chunk = source.chunk; } // end ASCIIPath(const ASCIIPath&)


// Create an ASCII file path from an existing ASCII file path                 */
dp::ASCIIPath::ASCIIPath(ASCIIPath&& result) noexcept
  : upDir(std::move(result.upDir)), drive(std::move(result.drive)),
  chunk(std::move(result.chunk))
{ } // end ASCIIPath(const ASCIIPath&&)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                           Pubilc Static Methods                            */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the default file name to be used if no usable file name is given       */
const std::string& dp::ASCIIPath::DefaultFile(void)
{
  return defaultFileName;
} // end const std::string& dp::ASCIIPath::DefaultFile(void)

// Set the default file name to be used if no usable file name is given       */
void dp::ASCIIPath::DefaultFile(const std::string& fileName)
{
  std::string in;
  for (char i : fileName)
  {
    if (IsPathLegalStrict(i)) { in.push_back(i); }
  }
  if (in.length() > 0) { defaultFileName = in; }

} // end void ASCIIPath::DefaultFile(const std::string&)

const std::string& dp::ASCIIPath::DefaultExtension(void)
{
  return defaultFileExt;
} // end const std::string& ASCIIPath::DefaultExtension(void)


// Set the default file extension (if any), to use when none is specified     */
void dp::ASCIIPath::DefaultExtension(const std::string& fileExtension)
{
  std::string in;
  for (char i : fileExtension)
  {
    if (IsPathLegalStrict(i))
    {
      if (i != '.') { in.push_back(i); }
    }
  }
  defaultFileExt = in;

} // end void ASCIIPath::DefaultExtension(const std::string&)


// Get the default path for relative paths to use as the root directory       */
const std::string& dp::ASCIIPath::DefaultRoot(void)
{
  return defaultFilePath;
} // end const std::string& ASCIIPath::DefaultRoot(void)


// Set the default path for relative paths to use as the root directory       */
void dp::ASCIIPath::DefaultRoot(const std::string& filePath)
{
  std::string in = Sanitized(filePath);
  size_t len = in.length(); // sanitized input string length
  // Make format uniform by prepending ".\" if not present, or "..\"
  if (len < 2 || (in[0] != '.' && in[1] != ':')
    || (in[0] == '.' && in[1] != PD && in[1] != '.'))
  {
    in = localPath + in;
  }
  defaultFilePath = in;

} // end void ASCIIPath::DefaultRoot(const std::string&)


// Get the current user's base path for storing personal files                */
const std::string& dp::ASCIIPath::HomePath(bool allUsers)
{
  // TODO: add all users branch functionality into Init
  return (allUsers) ? userHome : userHome;
} // end const std::string& ASCIIPath::HomePath(bool)


// Get the system path programs to install to (by default, not as a given)    */
const std::string& dp::ASCIIPath::ProgramsPath(void)
{
  return programPath;
} // end const std::string& ASCIIPath::ProgramsPath(void)


// Get the current running path for relative paths to convert to absolute     */
const std::string& dp::ASCIIPath::RunningPath(void)
{
  return runningPath;
} // end const std::string& ASCIIPath::RunningPath(void)


// Get the system path for installed programs to place links (start menu, etc)*/
const std::string& dp::ASCIIPath::ShortcutPath(bool allUsers)
{
  // TODO: add all users branch functionality into Init
  return (allUsers) ? startPath : startPath;
} // end const std::string& ASCIIPath::ShortcutPath(bool)


// Get the base path for storing current user's program settings files        */
const std::string& dp::ASCIIPath::UserDataPath(void)
{
  return userAppData;
} // end const std::string& ASCIIPath::UserDataPath(void)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Pubilc Methods                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get the current file name setting for the end of the current path          */
std::string dp::ASCIIPath::File(bool extensionOn) const
{
  if (chunk.size() > 0)
  {
    std::string fileName = chunk[chunk.size() - 1];
    if (!extensionOn)
    {
      size_t last = fileName.find_last_of('.');
      if (last > 0) { return fileName.substr(0, last); }
    }
    return fileName;
  }
  return "";
} // end std::string ASCIIPath::File(bool) const


// Get the current extension setting for the end of the current file name     */
std::string dp::ASCIIPath::Extension(void) const
{
  if (chunk.size() > 0)
  {
    std::string fileName = chunk[chunk.size() - 1];
    size_t last = fileName.find_last_of('.');
    if (last > 0 && last != fileName.length())
    {
      return fileName.substr(last+1);
    }
    // else pass through to return ""
  }
  return "";
} // end std::string ASCIIPath::Extension(void) const


// Get the first missing/erroneous subdirectory from the given path to a file */
int dp::ASCIIPath::MissingPart(bool strict) const
{
  int missing = 0;
  std::string result;
  struct stat buffer;
  bool relative = (drive == '.');

  if (relative)
  {
    if (upDir == 0)
    {
      result = defaultFilePath;
    }
    else
    {
      if (strict && runningPath != "")
      {
        ASCIIPath here(runningPath);
        if (upDir > here.chunk.size() - (here.drive == '.' ? 0 : 1))
        {
          return static_cast<int>(here.chunk.size() - upDir - 1u);
        }
      }
      std::string goUp = std::string("..") + PD;
      for (size_t i = 0; i < upDir; ++i) { result += goUp; }
    }
  }
  else
  {
    result.push_back(drive);
    result.push_back(':');
    result.push_back(PD);
    if (stat(result.c_str(), &buffer) != 0)
    {
      Log::Info(std::string("ASCIIPath::MissingPart() - drive ") + drive +
        " not found");
      return 1;
    }
  }
  for (size_t i = 0; i < chunk.size(); ++i)
  {
    if (stat(result.c_str(), &buffer) != 0)
    {
      missing += static_cast<int>(i) + ((relative) ? 0 : 1);
      break;
    }
    result += chunk[i] + PD;
  }
  return missing;
} // end int ASCIIPath::MissingPart(bool) const


// Make all missing subdirectories from the given path to the stored file     */
bool dp::ASCIIPath::MakePath(void)
{
  int missing = 0;
  std::string result;
  struct stat buffer;
  bool relative = (drive == '.');

  if (relative)
  {
    if (upDir == 0)
    {
      result = defaultFilePath;
    }
    else
    {
      std::string goUp = std::string("..") + PD;
      for (size_t i = 0; i < upDir; ++i) { result += goUp; }
    }
  }
  else
  {
    result.push_back(drive);
    result.push_back(':');
    result.push_back(PD);
    if (stat(result.c_str(), &buffer) != 0)
    {
      Log::Info(std::string("ASCIIPath::MakePath() - drive ") + drive +
        " not found");
      return false;
    }
  }
  for (size_t i = 0; i < chunk.size(); ++i)
  {
    if (stat(result.c_str(), &buffer) != 0)
    {
      CreateDirectoryA(result.c_str(), NULL);
      if (stat(result.c_str(), &buffer) != 0)
      {
        Log::Info(std::string("ASCIIPath::MakePath() - no write access in '")
          + result + '\'');
        return false;
      }
    }
    result += chunk[i] + PD;
  }
  return true;
} // end bool ASCIIPath::MakePath(void)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set an ASCII file path from another ASCII file path                        */
dp::ASCIIPath& dp::ASCIIPath::operator=(ASCIIPath const& source)
{
  upDir = source.upDir;
  drive = source.drive;
  chunk = source.chunk;
  return *this;

} // end ASCIIPath& operator=(ASCIIPath const&)


// Set an ASCII file path from a ASCII file path reference                    */
dp::ASCIIPath& dp::ASCIIPath::operator=(ASCIIPath&& result) noexcept
{
  upDir = (std::move(result.upDir));
  drive = (std::move(result.drive));
  chunk = (std::move(result.chunk));
  return *this;

} // end ASCIIPath& operator=(ASCIIPath&&) noexcept


// Convert the file descriptor to a string appropriate for the platform       */
dp::ASCIIPath::operator std::string() const
{
  std::string result;
  bool relative = (drive == '.');
  if (relative)
  {
    if (upDir == 0)
    {
      result = defaultFilePath;
    }
    else
    {
      std::string goUp = std::string("..") + PD;
      for (size_t i = 0; i < upDir; ++i) { result += goUp; }
    }
  }
  else
  {
    result.push_back(drive);
    result.push_back(':');
    result.push_back(PD);
  }
  for (std::string i : chunk)
  {
    result += i + PD;
  }
  result.pop_back();
  return result;

} // end ASCIIPath::operator std::string() const


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Private Static Methods                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get a path sanitized string out of a user provided input path string       */
std::string dp::ASCIIPath::Sanitized(const std::string& unfiltered)
{
  bool unwarned = true;
  bool uninformed = true;
  bool traversing = true;
  std::string result;
  size_t inLen = unfiltered.length();
  size_t curLen = 0;
  // Check if input string has sufficient characters to process
  if (inLen < 2)
  {
    result = (std::string(".") + PD);
    if (inLen > 0)
    {
      if (IsPathLegal(unfiltered[0]))
      {
        result.push_back(unfiltered[0]);
      }
      else
      {
        Log::Info("ASCIIPath(string&) - input \"", false);
        Log::I(unfiltered);
        Log::I("\" contains invalid chars ('");
        Log::I(unfiltered[0] + std::string("' not allowed)"), true);
        unwarned = false;
        Log::Diag("ASCIIPath(string&) - no valid path specified, default used");
        uninformed = false;
        result += defaultFileName;
      }
    }
    else
    {
      Log::Info("ASCIIPath(string&) - no path specified, default used");
      uninformed = false;
      result += defaultFileName;
    }
  }
  // Construct string of legal input from unfiltered input as needed
  else
  {
    for (size_t i = 0; i < inLen; ++i)
    {
      curLen = result.length();
      if (IsPathLegal(unfiltered[i]))
      {
        if (unfiltered[i] == ':')
        {
          // Only the drive letter of an absolute path may have a colon after
          if (i == 1) { result.push_back(':'); }
          else if (unwarned)
          {
            Log::Info("ASCIIPath(string&) - ':' only valid as 2nd char");
            unwarned = false;
          }
        }
        else if (IsAnyPathDivChar(unfiltered[i]))
        {
          // Path directories must have valid identifiers between divisions
          if (curLen > 0 && !IsAnyPathDivChar(result[curLen - 1]))
          {
            // Check for bad "\.\" patterns; rollback if necessary
            if (curLen > 1 && result[curLen - 1] == '.'
              && result[curLen - 2] == PD)
            {
              result.pop_back();
            }
            else
            {
              result.push_back(PD);
            }
          }
        }
        else if (unfiltered[i] == '.')
        {
          // Check for short accepted forming input string
          if (curLen < 2)
          {
            result.push_back('.');
            // Check that head of accepted char will add path div appropriately
            if (i + 2 < inLen && unfiltered[i + 1] == '.'
              && unfiltered[i + 2] != PD)
            {
              result.push_back('.');
              result.push_back(PD);
              ++i;
            }
            // Only other problem in "_." file name is terminal '.' char
          }
          // (Given >= 2) Check for forming ".." pattern in accepted input
          else if (result[curLen - 1] == '.')
          {
            if (traversing) // Up path traversal ".." only at head of path
            {
              // Correct inconsistent head formation if needed
              if (result[0] == '.' && result[1] == PD)
              {
                result = '.' + result.substr(2);
              }
              // Must follow "\.." pattern per up traversal
              if (result[curLen - 2] == PD)
              {
                result.push_back('.');
              }
              // Up directory "\.." must be followed by path div "\..\" form
              if (i + 1 < inLen && unfiltered[i + 1] != PD)
              {
                result.push_back(PD);
              }
            }
          }
          // (Given >= 2 & !("..")) Check '.' being added would not be terminal
          else if (i < inLen - 1)
          {
            result.push_back('.');
          }
        }
        else
        {
          result.push_back(unfiltered[i]);
          if (traversing) { traversing = false; }
        }
      } // end IsPathLegal() validation
      else if (unwarned)
      {
        dp::Log::Info("ASCIIPath(string&) - input \"", false);
        dp::Log::I(unfiltered);
        dp::Log::I("\" contains invalid chars ('");
        dp::Log::I(unfiltered[i] + std::string("' not allowed)"), true);
        unwarned = false;
      }
    }
  }
  curLen = result.length();
  if (curLen != 0)
  {
    if (IsAnyPathDivChar(result[curLen - 1])) { result += defaultFileName; }
    else if (result[curLen - 1] == '.') { result += (PD + defaultFileName); }
  }
  return result;

} // end std::string ASCIIPath::Sanitized(const std::string&)


// Fetch system path where programs store data for the current user settings  */
std::string dp::ASCIIPath::AppData(std::string loggedInID, int era)
{
#ifdef __unix__
  switch (era)
  {
  default:
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      // Generally, most programs just store files in a hidden home directory
      return Home(era) + loggedInID + PD + '.';
    }
  }
#elif defined(_WIN32) || defined(WIN32)
  switch (era)
  {
  case 2:
  case 1:
    // Windows historically keeps user data in a uniform hidden home subfolder
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return Home(era) + loggedInID + PD + "appData\\Roaming\\";
    }
  case 0:
  default:
    // Single user Windows era stored user data nested in system files instead
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      // Presuming %WinDir% pointed to Windows, not System32 or something else
      return "C:\\Windows\\Application Data\\";
    }
  }
#else
  // Probably good to add other OS path configurations eventually: mobile?
  switch (era)
  {
  default:
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return Home(era) + loggedInID + PD;
    }
  }
#endif
} // end std::string ASCIIPath::AppData(std::string, int)


// Fetch the appropriate root path for installing packages, per OS / lang     */
std::string dp::ASCIIPath::InstallFolder(std::string loggedInID, int era)
{
#ifdef __unix__
  switch (era)
  {
  default:
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      // all users generally need installations: needs sudo access to write in
      return "/opt/";
    }
  }
#elif defined(_WIN32) || defined(WIN32)
  switch (era)
  {
  case 3:
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return "C:\\Program Files (x86)\\";
    }
  case 4:
  case 2:
  case 1:
  case 0:
  default:
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return "C:\\Program Files\\";
    }
  }
#else
  // Probably good to add other OS path configurations eventually: mobile?
  switch (era)
  {
  default:
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return "/root/opt/";
    }
  }
#endif
  } // end std::string ASCIIPath::InstallFolder(std::string&, int)


// Fetch the system path where installed package shortcuts are placed         */
std::string dp::ASCIIPath::InstallShortcuts(std::string loggedInID, int era)
{
#ifdef __unix__
  switch (era)
  {
  default:
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      // Generally, most programs just store files in a hidden home directory
      return Home(era) + loggedInID + PD + '.';
    }
  }
#elif defined(_WIN32) || defined(WIN32)
  switch (era)
  {
  case 3:
  case 2:
    // Windows historically keeps user data in a uniform hidden home subfolder
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return AppData(loggedInID, era) + "Microsoft\\Windows\\Start Menu\\";
    }
  case 1:
    // Single user Windows era stored user data nested in system files instead
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      // Presuming %WinDir% pointed to Windows, and multiple profiles exist...
      return "C:\\Windows\\Profiles\\" + loggedInID + "\\Start Menu\\";
    }
  case 0:
  default:
    // If Windows 9x didn't have multiple users, the path didn't consider one
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return "C:\\Windows\\Start Menu\\";
    }
  }
#else
  // Probably good to add other OS path configurations eventually: mobile?
  switch (era)
  {
  default:
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return Home(era) + loggedInID + PD;
    }
  }
#endif
} // end std::string ASCIIPath::InstallShortcuts(std::string&, int)


// Fetch the root folder where user profiles are stored per OS / language     */
std::string dp::ASCIIPath::Home(int era)
{
#ifdef __unix__
  switch (era)
  {
  default:
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return "/home/";
    }
  }
#elif defined(_WIN32) || defined(WIN32)
  switch (era)
  {
  case 3:
    // Windows revisions after Vista place personal files in C:\Users\<user>
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return "C:\\Users\\";
    }
  case 2:
    // Prior to Vista, Windows revisions used C:\Documents and Settings\<user>
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return "C:\\Documents and Settings\\";
    }
  case 1:
    // Early concessions to users contingently added profiles after the 2nd one
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return "C:\\Windows\\Profiles\\";
    }
  default:
    // Windows 9x just placed user files in the root of C: if possible
    switch (langSetting)
    {
    case SysLang::Eng:
    default:
      return "C:\\";
    }
  }
#else
  // Probably good to add other OS path configurations, mobile for example
  switch (langSetting)
  {
  case SysLang::Eng:
  default:
    return "/root/home/"; // Easily an out of scope concern beyond this much
  }
#endif
} // end std::string ASCIIPath::Home(int)
