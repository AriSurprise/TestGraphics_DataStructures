/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Log.h
Purpose:  Static class for logging at various levels of verbosity / severity
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
#include <fstream>      // Output stream declaration for private stream members
#include <string>       // End user availability of std::to_string(), etc
// "./src/..."
#include "ASCIIPath.h"  // File path parameters for log file setting functions

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Log //! Static methods to log messages at various levels of verbosity
  {
  public:
    
    enum class Verbosity //! Categorical levels of logging output messages
    {
      Silent,     //! Presentation mode: output no messages
      Critical,   //! Logging mode: output only error messages
      Debug,      //! Standard mode: output warning / error messages
      Inform,     //! Detailed mode: output all normal process messages
      Diagnostic, //! Diagnostic mode: output more process messages per step
      Full        //! Test mode: output all possible messages; fully document
    };

    static Verbosity DEFAULT_LOGGING;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Add diagnostic text to the current log file in a running statement
    \brief
      - add diagnostic text to the current log file in a running statement
    \details
      - verbosity must be at least Diagnostic (4 or higher) to log message
      Functions the same as Diag("") but without "Diag: " prefix, and default
      line break set to false instead (you say when you're done)
    \param output
      - string to be sent to / logged in the diagnostic output stream
    \param endLine
      - add a line break at the end of the output message (default is false)
      Use after Diag("output", false) to log more varied output, then use
      D("output", true) at the last statement to close out the line
    */
    static void D(const std::string& output, bool endLine = false);


    /** Set the prefix to appear at the head of each level of message output
    \brief
      - set the prefix to appear at the head of each level of message output
    \param errorPrefix
      - text prepended to error messages being logged
    \param warnPrefix
      - text prepended to warning messages being logged
    \param infoPrefix
      - text prepended to informative messages being logged
    \param diagPrefix
      - text prepended to diagnostic messages being logged
    \param notePrefix
      - text prepended to notational messages being logged
    */
    static void Designations(const std::string& errorPrefix = "",
      const std::string& warnPrefix = "", const std::string& infoPrefix = "",
      const std::string& diagPrefix = "", const std::string& notePrefix = "");


    /** Output diagnostic messages to the current log file
    \brief
      - output diagnostic messages to the current log file
    \details
      - verbosity must be at least Diagnostic (4 or higher) to log message
    \param output
      - string to be sent to / logged in the diagnostic output stream
    \param endLine
      - add a line break at the end of the output message (default is true)
      Set to false and in conjunction with D() to log more varied output
    */
    static void Diag(const std::string& output, bool endLine = true);


    /** Add error text to the current log file in a running statement
    \brief
      - add error text to the current log file in a running statement
    \details
      - verbosity must be at least Critical (1 or higher) to log output message
      Functions the same as Error("") but without "Error: " prefix,
      and default line break set to false instead
    \param output
      - string to be sent to / logged in the error output stream
    \param endLine
      - add a line break at the end of the output message (default is false)
      Use after Error("output", false) to log more varied output, then use
      E("output", true) at the last statement to close out the line
    */
    static void E(const std::string& output, bool endLine = false);


    /** Output error messages to the current log file
    \brief
      - output error messages to the current log file
    \details
      - verbosity must be at least Critical (1 or higher) to log output message
    \param output
      - string to be sent to / logged in the error output stream
    \param endLine
      - add a line break at the end of the output message (default is true)
      Set to false and in conjunction with E() to log more varied output
    */
    static void Error(const std::string& output, bool endLine = true);


    /** Set the output file to be used for all message logs
    \brief
      - set the output file to be used for all message logs
    \param file
      - path to the desired log file for all output
    \return
      - true if the file path can be set as requested
    */
    static bool FileSetting(const ASCIIPath& file);


    /** Set the output file to be used for all message logs
    \brief
      - set the output file to be used for all message logs
    \param file
      - path to the desired log file for all output
    \return
      - true if the file path can be set as requested
    */
    static bool FileSetting(const std::string& logFile = "");


    /** Set individual files to be used for each verbosity level's output log
    \brief
      - set individual files to be used for each verbosity level's output log
    \details
      - if any log is set to "", that will be routed to default console output
    \param errorLog
      - path to the desired log file for error output
    \param warnLog
      - path to the desired log file for warning output
    \param infoLog
      - path to the desired log file for informational output
    \param diagLog
      - path to the desired log file for diagnostic output
    \param noteLog
      - path to the desired log file for notational output
    \return
      - whether the log file set operations were successfully set
    */
    static bool FileSettings(const ASCIIPath& errorLog,
      const ASCIIPath& warnLog, const ASCIIPath& infoLog,
      const ASCIIPath& diagLog, const ASCIIPath& noteLog);


    /** Set individual files to be used for each verbosity level's output log
    \brief
      - set individual files to be used for each verbosity level's output log
    \param errorLog
      - path to the desired log file for error output
    \param warnLog
      - path to the desired log file for warning output
    \param infoLog
      - path to the desired log file for informational output
    \param diagLog
      - path to the desired log file for diagnostic output
    \param noteLog
      - path to the desired log file for notational output
    \return
      - whether the log file set operations were successfully set
    */
    static bool FileSettings(const std::string& errorLog = "",
      const std::string& warnLog = "", const std::string& infoLog = "",
      const std::string& diagLog = "", const std::string& noteLog = "");


    /** Add informative text to the current log file in a running statement
    \brief
      - add informative text to the current log file in a running statement
    \details
      - verbosity must be at least Inform (3 or higher) to log output message
      Functions the same as Info("") but without "Info: " prefix, and default
      line break set to false instead (you say when you're done)
    \param output
      - string to be sent to / logged in the informative output stream
    \param endLine
      - add a line break at the end of the output message (default is false)
      Use after Info("output", false) to log more varied output, then use
      I("output", true) at the last statement to close out the line
    */
    static void I(const std::string& output, bool endLine = false);


    /** Output informative messages to the current log file
    \brief
      - output informative messages to the current log file
    \details
      - verbosity must be at least Inform (3 or higher) to log output message
    \param output
      - string to be sent to / logged in the informative output stream
    \param endLine
      - add a line break at the end of the output message (default is true)
      Set to false and in conjunction with I() to log more varied output
    */
    static void Info(const std::string& output, bool endLine = true);


    /** Set up logging options to preferred user settings (null to ignore args)
    \brief
      - set up logging options to preferred user settings (null to ignore args)
    \details
      - any char string argument past the first may be omitted by default
      using a null value: the last provided log file name and / or message log
      prefix will then be the uniform substitute for null inputs of that type. 
      This then enables generalizations for increasingly trivial messages to a
      collective file, while more critical messages are directed elsewhere,
      without needing overly verbose, explicit input for every file or prefix.
    \param argv0
      - path the program is being run from: captured in main(int, char**) [0]
    \param output
      - level of detailed messages to output: Silent, Critical, Debug, ..., Full
    \param lang
      - language preferences for system paths to be looked up by (eng only)
    \param errorLog
      - valid file path will send errors to a log file, otherwise std cerr used
    \param errorMsg
      - string to prepend to new error messages, default used if none provided
    \param warnLog
      - valid file path will send warnings to a log file, otherwise std clog
    \param warnMsg
      - string to prepend to new warning messages, default used if none provided
    \param infoLog
      - valid file path will send new dialogs to a log file, otherwise std cout
    \param infoMsg
      - string to prepend to new informative messages, default used if not given
    \param diagLog
      - valid file path will send diagnostics to a log file, otherwise std cout
    \param diagMsg
      - string to prepend to new diagnostic messages, default used if not given
    \param noteLog
      - valid file path will send process notes to a log file, std cout if not
    \param noteMsg
      - string to prepend to new process notes, default used if not given
    */
    static void Init(const char* argv0, ASCIIPath::SysLang lang =
      ASCIIPath::SysLang::Eng, Verbosity output = DEFAULT_LOGGING,
      const char* errorLog = nullptr, const char* errorMsg = nullptr,
      const char* warnLog = nullptr, const char* warnMsg = nullptr,
      const char* infoLog = nullptr, const char* infoMsg = nullptr,
      const char* diagLog = nullptr, const char* diagMsg = nullptr,
      const char* noteLog = nullptr, const char* noteMsg = nullptr);


    /** Add notational text to the current log file in a running statement
    \brief
      - add notational text to the current log file in a running statement
    \details
      - verbosity must set to Full (5) to output the note to the log file
      Functions the same as Note("") but without "Note: " prefix, and default
      line break set to false instead (you say when you're done)
    \param output
       - string to be sent to / logged in the note output stream
    \param endLine
      - add a line break at the end of the output message (default is false)
      Use after Note("output", false) to log more varied output, then use
      N("output", true) at the last statement to close out the line
    */
    static void N(const std::string& output, bool endLine = false);


    /** Output expository process notes to the current log file
    \brief
      - output expository process notes to the current log file
    \details
      - verbosity must be set to Full (5) to output the note to the log
    \param output
      - string to be sent to / logged in the note output stream
    \param endLine
      - add a line break at the end of the output message (default is true)
      Set to false and in conjunction with N() to log more varied output
    */
    static void Note(const std::string& output, bool endLine = true);


    /** Perform cleanup (ostensibly close open files, prune empty files)
    \brief
      - perform cleanup (ostensibly close open files, prune empty files)
    \details
      - currently no files are left open by design, so only to remove null logs
    */
    static void Shutdown(void);


    /** Get the current verbosity setting in use by the Log class
    \brief
      - get the current verbosity setting in use by the Log class
    \details
      - can be useful to store current setting for local override, then reset
    \return
       - current verbosity setting for Log output level
    */
    static Log::Verbosity VerbositySetting(void);


    /** Output a different amount of process flow context / degree of minutiae
    \brief
      - output a different amount of process flow context / degree of minutiae
    \details
      - FileSetting can be used in conjunction to specify target output files
    \param sensitivity
       - set to Silent, Critical, Debug, Inform, Diagnostic or Full to display
       increasingly non-critical process detail logs for debugging purposes.
       Silent will not even log error messages in the event of system failure.
    */
    static void VerbositySetting(Verbosity sensitivity);


    /** Output a different amount of process flow context / degree of minutiae
    \brief
      - output a different amount of process flow context / degree of minutiae
    \details
      - FileSetting can be used in conjunction to specify target output files
    \param sensitivity
       - set to 0 for none, 1 for errors only, up to 5 for all messages to show
       increasingly non-critical process detail logs for debugging purposes.
       Silent will not even log error messages in the event of system failure.
    */
    static void VerbositySetting(int sensitivity);


    /** Output warning messages to the current log file
    \brief
      - output warning messages to the current log file
    \details
      - verbosity must be at least Debug (2 or higher) to log output message
    \param output
      - string to be sent to / logged in the warning output stream
    \param endLine
      - add a line break at the end of the output message (default is true)
      Set to false and in conjunction with W() to log more varied output
    */
    static void Warn(const std::string& output, bool endLine = true);


    /** Add warning text to the current log file in a running statement
    \brief
      - add warning text to the current log file in a running statement
    \details
      - verbosity must be at least Debug (2 or higher) to log output message
      Functions the same as Warning("") but without "Warning: " prefix,
      and default line break set to false instead
    \param output
      - string to be sent to / logged in the warning output stream
    \param endLine
      - add a line break at the end of the output message (default is false)
      Use after Warning("output", false) to log more varied output, then use
      W("output", true) at the last statement to close out the line
    */
    static void W(const std::string& output, bool endLine = false);


  private:

    /** Conditionally output a message to the given stream
    \brief
      - conditionally output a message to the given stream
    \details
      - essentially, a function pointer for the format other messages follow
    \param level
      - verbosity current output level setting must exceed to log the message
    \param out
      - stream to be used for output
    \param message
      - string to be sent to / logged in the error output stream
    \param endLine
      - add a line break at the end of the output message if true
    */
    static void Msg(const Verbosity level, std::ostream& out,
      const std::string& message, bool endLine);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Stored level of output verbosity dictating what messages to log
    static Verbosity outputLevel;

    //! Output file stream object to be used to log notational messages
    static std::ofstream nteStrm;

    //! Output stream object to be used to log notational messages
    static std::ostream* nteOut;

    //! Path for the file to hold logged notational messages
    static std::string nteFile;

    //! String to prepend to notational messages being logged
    static std::string nteText;

    //! Output file stream object to be used to log diagnostic messages
    static std::ofstream dgsStrm;

    //! Output stream object to be used to log diagnostic messages
    static std::ostream* dgsOut;

    //! Path for the file to hold logged diagnostic messages
    static std::string dgsFile;

    //! String to prepend to diagnostic messages being logged
    static std::string dgsText;

    //! Output file stream object to be used to log informational messages
    static std::ofstream infStrm;

    //! Output stream object to be used to log informational messages
    static std::ostream* infOut;

    //! Path for the file to hold logged informational messages
    static std::string infFile;

    //! String to prepend to informational messages being logged
    static std::string infText;

    //! Output file stream object to be used to log warning messages
    static std::ofstream wrnStrm;

    //! Output stream object to be used to log warning messages
    static std::ostream* wrnOut;

    //! Path for the file to hold logged warning messages
    static std::string wrnFile;

    //! String to prepend to warning messages being logged
    static std::string wrnText;

    //! Output file stream object to be used to log error messages
    static std::ofstream errStrm;

    //! Output stream object to be used to log error messages
    static std::ostream* errOut;

    //! Path for the file to hold logged error messages
    static std::string errFile;

    //! String to prepend to error messages being logged
    static std::string errText;

  }; // end Log class declaration

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Constant Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

} // end dp namespace
