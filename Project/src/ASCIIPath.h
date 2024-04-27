/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  ASCIIPath.h
Purpose:  File string parser restrictive to only ascii path chars / chunks
Language: C++ / Eng, MSVC 19 (used, but should work with most compilers)
Platform: MSVC 19 / Windows 10 x64 (used, but should have no requirements)
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
#include <string> // Paths are just ascii string validators/ wrappers
#include <vector> // Internal member containter to list segments of file path

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Class project file usage scope restriction label
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class ASCIIPath //! String parser restricting only allowable ascii path chunks
  {
  public:
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Constants                             */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    enum class SysLang //! System language option list for system path functions
    {
      Default,  //! If language setting not given, lookup will have to happen
      Eng       //! Enlish system language: only supported / defined language
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a file descriptor from the given string
    \brief
      - create a file descriptor from the given string
    \param pathName
      - string containing the path, name and extension of the desired file
    */
    ASCIIPath(const std::string& pathName);

    /** Create an ASCII file path from an existing ASCII file path
    \brief
      - create an ASCII file path from an existing ASCII file path
    \param source
      - descriptor of an existing file path of ascii characters to be copied
    */
    ASCIIPath(ASCIIPath const& source);

    /** Create a file descriptor from a ASCII file path reference
    \brief
      - create a file descriptor from a ASCII file path reference
    \param result
      - rValue reference for a file path of ASCII characters to be copied
    */
    ASCIIPath(ASCIIPath&& result) noexcept;

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Public Static Methods                           */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Get the default file name to be used if no usable file name is given
    \brief
      - get the default file name to be used if no usable file name is given
    \return
      - current string set to be used if an ASCIIPath is made with no file name
    */
    static const std::string& DefaultFile(void);


    /** Set the default file name to be used if no usable file name is given
    \brief
      - set the default file name to be used if no usable file name is given
    \details
      - must contain path valid characters to be set successfully
    \param fileName
      - path safe ASCII character string to be used as a default file name
    */
    static void DefaultFile(const std::string& fileName);


    /** Get the default file extension (if any), to use when none is specified
    \brief
      - get the default file extension (if any), to use when none is specified
    \return
      - string set to append to a file name (no '.'), when none is specified
    */
    static const std::string& DefaultExtension(void);


    /** Set the default file extension (if any), to use when none is specified
    \brief
      - set the default file extension (if any), to use when none is specified
    \param fileExtension
      - path valid chars to be appended to files with no extension (if not "")
      not necessary to include '.'- will be added obligatorily (if not "")
    */
    static void DefaultExtension(const std::string& fileExtension);


    /** Get the default path for relative paths to use as the root directory
    \brief
      - get the default path for relative paths to use as the root directory
    \return
      - path to use as a default conversion from relative to an absolute path
    */
    static const std::string& DefaultRoot(void);


    /** Set the default path for relative paths to use as the root directory
    \brief
      - set the default path for relative paths to use as the root directory
    \details
      - default root is local ".\", path may be set as relative or absolute
      Chosen path will be prepended to relative paths at the time of output
    \param filePath
      - local directory (in ascii characters) to be used as root path for files
    */
    static void DefaultRoot(const std::string& filePath);


    /** Get the current user's base path for storing personal files
    \brief
      - get the current user's base path for storing personal files
    \param allUsers
      - retrieve the alternate path for all users data (with less write access)
    \return
      - path to place user files for profile specific personal data files
    */
    static const std::string& HomePath(bool allUsers = false);


    /** Get the system path programs to install to (by default, not as a given)
    \brief
      - get the system path programs to install to (by default, not as a given)
    \return
      - path to offer as a default for installations (without project / package)
    */
    static const std::string& ProgramsPath(void);


    /** Get the current running path for relative paths to convert to absolute
    \brief
      - get the current running path for relative paths to convert to absolute
    \return
      - path to the folder that the program is currently being run from within
    */
    static const std::string& RunningPath(void);


    /** Get the system path for installed programs to place links (start, etc)
    \brief
      - get the system path for installed programs to place links (start, etc)
    \param allUsers
      - retrieve the alternate path for all users data (with less write access)
    \return
      - path to place start menu shortcuts (Windows), opt folder (Linux), etc
    */
    static const std::string& ShortcutPath(bool allUsers = false);


    /** Get the base path for storing current user's program settings files
    \brief
      - get the base path for storing current user's program settings files
    \return
      - path to place user files for profile specific program data files
    */
    static const std::string& UserDataPath(void);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Methods                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get the current file name setting for the end of the current path
    \brief
      - get the current file name setting for the end of the current path
    \param extensionOn
      - optionally disable to have the file extension pruned, and get just name
    \return
      - text string of file name the path is cued to look up
    */
    std::string File(bool extensionOn = true) const;
    // TODO: add case augmented output (aCase:: enum or 2 bools?)


    /** Get the current extension setting for the end of the current file name
    \brief
      - get the current extension setting for the end of the current file name
    \return
      - text string of file name extension for the current file of the path
    */
    std::string Extension(void) const;


    /** Get the first missing subdirectory from the given path to a file
    \brief
      - get the first missing subdirectory from the given path to a file

    \details
      - 0 result meaning no folders are missing, 1 meaning the root is missing,
      a negative value meaning the relative path called for a number of upwards
      directory traversals which exceeded the current running folder's depth.
      The negative result will not prohibit the path from functioning and may
      not matter: in such a case, use MissingPart() instead.
    \param strict
      - if enabled, will attempt to scan if too many relative upwards traversals
      are specified for the current running directory to navigate (not an error
      producing behavior, just added semantic)
    \return
      - directories into path search for first missing path subdirectory found
    */
    int MissingPart(bool strict = false) const;


    /** Make all missing subdirectories from the given path to the stored file
    \brief
      - make all missing subdirectories from the given path to the stored file
    \return
      - true if successful, false if write permissions prohibit folder creation
    */
    bool MakePath(void);




    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set an ASCII file path from another ASCII file path
    \brief
      - set an ASCII file path from another ASCII file path
    \param result
      - file path of ASCII characters to be copied from
    */
    ASCIIPath& operator=(ASCIIPath const& source);


    /** Set an ASCII file path from a ASCII file path reference
    \brief
      - set an ASCII file path from a ASCII file path reference
    \param result
      - rValue reference for a file path of ASCII characters to be copied
    */
    ASCIIPath& operator=(ASCIIPath&& result) noexcept;


    /** Convert the file descriptor to the full path string for the platform
    \brief
      - convert the file descriptor to the full path string for the platform
    */
    operator std::string() const;

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Private Static Constants                        */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Initialization procedures performed flag: set up already done if true
    static bool envGot;

    //! Path division significant character for the platform
    const static char PD;

    //! String signifying the file is contained in the current directory
    const static std::string localPath;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Private Static Methods                          */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get the path where Windows stores program data, per language / version
    \brief
      - get the path where Windows stores program data, per language / version
    \param loggedInID
      - pass in the current user login ID (for lower code redundancy)
    \param era
      - where files were stored changes per OS and revision, making it
      ineffective to use an enum: the oldest Windows revisions (0), were single
      user, while current revisions have changed standards a few times on where
      user data gets placed.  Where those paths were in other OS eras will have
      their own dates / meanings, but no fundamental ordering semantic
    \return
      - the string, for user application data, per language / version
    */
    static std::string AppData(std::string loggedInID, int era = 3);


    /** Fetch the appropriate root path for installing packages, per OS / lang
    \brief
      - fetch the appropriate root path for installing packages, per OS / lang
    \param loggedInID
      - pass in the current user login ID (for lower code redundancy)
    \param era
      - where files were stored changes per OS and revision, making it
      ineffective to use an enum: the oldest Windows revisions (0), were single
      user, as well as 32 bit, while current revisions have changed standards a
      few times on where to install programs or user data.  Where those paths
      were in other OS eras will have their own dates / meanings, but no
      fundamental ordering semantic

    \return
      - the string for default program installations, per OS / localization
    */
    static std::string InstallFolder(std::string loggedInID, int era = 3);


    /** Fetch the appropriate root path for installed package shortcuts per OS
    \brief
      - fetch the appropriate root path for installed package shortcuts per OS
    \param loggedInID
      - pass in the current user login ID (for lower code redundancy)
    \param era
      - Where files were stored changes per OS and revision, making it
      ineffective to use an enum: the oldest Windows revisions (0), were single
      user, while current revisions have changed standards a few times on where
      user data gets placed.  Where those paths were in other OS eras will have
      their own dates / meanings, but no fundamental ordering semantic
    \return
      - the string, for user program shortcuts, per OS / language / version
    */
    static std::string InstallShortcuts(std::string loggedInID, int era = 3);


    /** Fetch the path where the OS stores users' home directories
    \brief
      - fetch the path where the OS stores users' home directories
    \details
      - retrieving root of users directory, without specific user's folder
    \return
      - the string, for the OS's user home directories, per language / version
    */
    static std::string Home(int era = 3);

    /** Correct form and remove illegal characters from input file path string
    \brief
      - correct form and remove illegal characters from input file path string
    \param unfiltered
      - input string containing the path, name and extension of the desired file
    \return
      - the unfiltered input string, stripped of illegal chars and paths
    */
    static std::string Sanitized(const std::string& unfiltered);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Private Static Members                          */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! File name to be used if none is specified
    static std::string defaultFileName;

    //! File extension to be used if none is specified
    static std::string defaultFileExt;

    //! File path to be used if solicited with none specified
    static std::string defaultFilePath;

    //! Path setting for the system location of default program installations
    static std::string programPath;

    //! Path setting for the current running location of the program
    static std::string runningPath;

    //! Path setting for the current running location of the program
    static std::string startPath;

    //! File path to the user's home directory for their personal files
    static std::string userHome;

    //! File path to the user's application data directory for the system
    static std::string userAppData;

    //! Localization stored to fetch language specific system paths
    static SysLang langSetting;

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Times the path should contain leading "..\" directory up strings
    size_t upDir;

    //! Drive letter the path should use in an absolute path ('.' if relative)
    char drive;

    //! Sequenced list of path chunks signifying directory / file names
    std::vector<std::string> chunk;

  }; // end ASCIIPath class declaration

}// end dp namespace
