/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Log.cpp
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
#include <iostream>     // Basic STL input / output stream manipulation
// "./src/..."
#include "Log.h"        // Class declarations header file
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                      Static Variable Initializations                       */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Log::Verbosity dp::Log::outputLevel = dp::Log::Verbosity::Full;
std::string dp::Log::nteText = "Note: ";
std::string dp::Log::dgsText = "Diag: ";
std::string dp::Log::infText = "Info: ";
std::string dp::Log::wrnText = "Warning: ";
std::string dp::Log::errText = "Error: ";
std::string dp::Log::nteFile = "";
std::string dp::Log::dgsFile = "";
std::string dp::Log::infFile = "";
std::string dp::Log::wrnFile = "";
std::string dp::Log::errFile = "";
std::ofstream dp::Log::nteStrm;
std::ofstream dp::Log::dgsStrm;
std::ofstream dp::Log::infStrm;
std::ofstream dp::Log::wrnStrm;
std::ofstream dp::Log::errStrm;
std::ostream* dp::Log::nteOut = &std::cout;
std::ostream* dp::Log::dgsOut = &std::cout;
std::ostream* dp::Log::infOut = &std::cout;
std::ostream* dp::Log::wrnOut = &std::clog;
std::ostream* dp::Log::errOut = &std::cerr;
dp::Log::Verbosity dp::Log::DEFAULT_LOGGING = 
#ifdef _DEBUG
dp::Log::Verbosity::Debug;
#else
dp::Log::Verbosity::Critical;
#endif


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Pubilc Methods                                */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Add diagnostic text to the current log file in a running statement         */
void dp::Log::D(const std::string& output, bool endLine)
{
  if (dgsFile != "")
  {
    dgsStrm.open(dgsFile, std::ios_base::app);
    if (!dgsStrm.is_open())
    {
      dgsOut = &std::cout;
      Msg(Verbosity::Debug, *dgsOut, "Log::D - '" + dgsFile +
        "' open failed, reverting to std::cout", true);
      dgsFile = "";
    }
    else // File open succeeded
    {
      dgsOut = &dgsStrm; // dangling pointer readdressal per file open
    }
  }
  Msg(Verbosity::Diagnostic, *dgsOut, output, endLine);
  if (dgsStrm.is_open()) { dgsStrm.close(); }
} // end Log::D(const std::string&, bool)

// Set the prefix text to appear at the head of each level of message output  */
void dp::Log::Designations(const std::string& errorPrefix,
  const std::string& warnPrefix, const std::string& infoPrefix,
  const std::string& diagPrefix, const std::string& notePrefix)
{
  errText = errorPrefix;
  wrnText = warnPrefix;
  infText = infoPrefix;
  dgsText = diagPrefix;
  nteText = notePrefix;
} // end void Log::Designations(string&, string&, string&, string&, string&)


// Output diagnostic messages to the current log file                         */
void dp::Log::Diag(const std::string& output, bool endLine)
{
  if (dgsFile != "")
  {
    dgsStrm.open(dgsFile, std::ios_base::app);
    if (!dgsStrm.is_open())
    {
      dgsOut = &std::cout;
      Msg(Verbosity::Debug, *dgsOut, "Log::Diag - '" + dgsFile +
        "' open failed, reverting to std::cout", true);
      dgsFile = "";
    }
    else // File open succeeded
    {
      dgsOut = &dgsStrm; // dangling pointer readdressal per file open
    }
  }
  Msg(Verbosity::Diagnostic, *dgsOut, (dgsText + output), endLine);
  if (dgsStrm.is_open()) { dgsStrm.close(); }
} // end void Log::Diag(const std::string&, bool)


// Add error text to the current log file in a running statement              */
void dp::Log::E(const std::string& output, bool endLine)
{
  if (errFile != "")
  {
    errStrm.open(errFile, std::ios_base::app);
    if (!errStrm.is_open())
    {
      errOut = &std::cerr;
      Msg(Verbosity::Debug, *wrnOut, "Log::E - '" + errFile +
        "' open failed, reverting to std::cerr", true);
      errFile = "";
    }
    else // File open succeeded
    {
      errOut = &errStrm; // dangling pointer readdressal per file open
    }
  }
  Msg(Verbosity::Critical, *errOut, output, endLine);
  if (errStrm.is_open()) { errStrm.close(); }
} // end void Log::E(const std::string&, bool)


// Output error messages to the current log file                              */
void dp::Log::Error(const std::string& output, bool endLine)
{
  if (errFile != "")
  {
    errStrm.open(errFile, std::ios_base::app);
    if (!errStrm.is_open())
    {
      errOut = &std::cerr;
      Msg(Verbosity::Debug, *wrnOut, "Log::Error - '" + errFile +
        "' open failed, reverting to std::cerr", true);
      errFile = "";
    }
    else // File open succeeded
    {
      errOut = &errStrm; // dangling pointer readdressal per file open
    }
  }
  Msg(Verbosity::Critical, *errOut, (errText + output), endLine);
  if (errStrm.is_open()) { errStrm.close(); }
} // end void Log::Error(const std::string&, bool)


// Set the output file to be used for all message logs                        */
bool dp::Log::FileSetting(const ASCIIPath& file)
{
  std::string fileName = file;
  std::ofstream out(fileName);
#if defined (WIN32) || defined (_WIN32)
  if (fileName == std::string(ASCIIPath(".\\")))
#else
  if (fileName == std::string(ASCIIPath("./")))
#endif
  {
    errFile = "";
    wrnFile = "";
    infFile = "";
    dgsFile = "";
    nteFile = "";
    errOut = &std::cerr;
    wrnOut = &std::clog;
    infOut = &std::cout;
    dgsOut = &std::cout;
    nteOut = &std::cout;
    return true;
  }
  if (!out.is_open())
  {
    out.open(fileName);
  }
  if (out.is_open())
  {
    out.close();
    errFile = fileName;
    wrnFile = fileName;
    infFile = fileName;
    dgsFile = fileName;
    nteFile = fileName;
    errStrm.open(errText);
    errOut = &errStrm;
    wrnOut = &errStrm;
    infOut = &errStrm;
    dgsOut = &errStrm;
    nteOut = &errStrm;
    errStrm.close();
    return true;
  }
  else
  {
    return false;
  }
} // end bool Log::FileSetting(const ASCIIPath&)


// Set the output file to be used for all message logs                        */
bool dp::Log::FileSetting(const std::string& logFile)
{
  return (logFile == "")  ? FileSetting(ASCIIPath("./"))
                          : FileSetting(ASCIIPath(logFile));
} // end bool Log::FileSetting(const std::string&)


// Set the individual files to be used for each verbosity level's output log  */
bool dp::Log::FileSettings(const ASCIIPath& errorLog,
  const ASCIIPath& warnLog, const ASCIIPath& infoLog,
  const ASCIIPath& diagLog, const ASCIIPath& noteLog)
{
  bool result = true;
  std::string fileName = errorLog;
  std::ofstream out;

  // error level log output (file) setup
#if defined (WIN32) || defined (_WIN32)
  if (fileName == std::string(ASCIIPath(".\\")))
#else
  if (fileName == std::string(ASCIIPath("./")))
#endif
  {
    errFile = "";
    errOut = &std::cerr;
  }
  else
  {
    out.open(fileName);
    if (!out.is_open())
    {
      out.open(fileName);
    }
    if (out.is_open())
    {
      out.close();
      errFile = fileName;
      errStrm.open(errFile);
      errOut = &errStrm;
      errStrm.close();
    }
    else { result = false; }
  }

  // warning level log output (file) setup
  fileName = warnLog;
#if defined (WIN32) || defined (_WIN32)
  if (fileName == std::string(ASCIIPath(".\\")))
#else
  if (fileName == std::string(ASCIIPath("./")))
#endif
  {
    wrnFile = "";
    wrnOut = &std::cerr;
  }
  else
  {
    out.open(fileName);
    if (!out.is_open())
    {
      out.open(fileName);
    }
    if (out.is_open())
    {
      out.close();
      wrnFile = fileName;
      wrnStrm.open(wrnFile);
      wrnOut = &wrnStrm;
      wrnStrm.close();
    }
    else { result = false; }
  }

  // informational level log output (file) setup
  fileName = infoLog;
#if defined (WIN32) || defined (_WIN32)
  if (fileName == std::string(ASCIIPath(".\\")))
#else
  if (fileName == std::string(ASCIIPath("./")))
#endif
  {
    infFile = "";
    infOut = &std::cerr;
  }
  else
  {
    out.open(fileName);
    if (!out.is_open())
    {
      out.open(fileName);
    }
    if (out.is_open())
    {
      out.close();
      infFile = fileName;
      infStrm.open(infFile);
      infOut = &infStrm;
      infStrm.close();
    }
    else { result = false; }
  }

  // diagnostic level log output (file) setup
  fileName = diagLog;
#if defined (WIN32) || defined (_WIN32)
  if (fileName == std::string(ASCIIPath(".\\")))
#else
  if (fileName == std::string(ASCIIPath("./")))
#endif
  {
    dgsFile = "";
    dgsOut = &std::cerr;
  }
  else
  {
    out.open(fileName);
    if (!out.is_open())
    {
      out.open(fileName);
    }
    if (out.is_open())
    {
      out.close();
      dgsFile = fileName;
      dgsStrm.open(dgsFile);
      dgsOut = &dgsStrm;
      dgsStrm.close();
    }
    else { result = false; }
  }

  // notational level log output (file) setup
  fileName = noteLog;
#if defined (WIN32) || defined (_WIN32)
  if (fileName == std::string(ASCIIPath(".\\")))
#else
  if (fileName == std::string(ASCIIPath("./")))
#endif
  {
    nteFile = "";
    nteOut = &std::cerr;
  }
  else
  {
    out.open(fileName);
    if (!out.is_open())
    {
      out.open(fileName);
    }
    if (out.is_open())
    {
      out.close();
      nteFile = fileName;
      nteStrm.open(nteFile);
      nteOut = &nteStrm;
      nteStrm.close();
    }
    else { result = false; }
  }

  return result;
} // end bool Log::FileSettings(const ASCIIPath&, ..., const ASCIIPath&)


// Set individual files to be used for each verbosity level's output log      */
bool dp::Log::FileSettings(const std::string& errorLog,
  const std::string& warnLog, const std::string& infoLog,
  const std::string& diagLog, const std::string& noteLog)
{
  return FileSettings(ASCIIPath((errorLog != "") ? errorLog : "./"),
                      ASCIIPath((warnLog != "") ? warnLog : "./"),
                      ASCIIPath((infoLog != "") ? infoLog : "./"),
                      ASCIIPath((diagLog != "") ? diagLog : "./"),
                      ASCIIPath((noteLog != "") ? noteLog : "./"));
} // end bool Log::FileSettings(string&, string&, string&, string&, string&)

// Add informational text to the current log file in a running statement      */
void dp::Log::I(const std::string& output, bool endLine)
{
  if (infFile != "")
  {
    infStrm.open(infFile, std::ios_base::app);
    if (!infStrm.is_open())
    {
      infOut = &std::cout;
      Msg(Verbosity::Debug, *infOut, "Log::I - '" + infFile +
        "' open failed, reverting to std::cout", true);
      infFile = "";
    }
    else
    {
      infOut = &infStrm;
    }
  }
  Msg(Verbosity::Inform, *infOut, output, endLine);
  if (infStrm.is_open()) { infStrm.close(); }
} // end Log::I(const std::string&, bool)


// Output informational messages to the current log file                      */
void dp::Log::Info(const std::string& output, bool endLine)
{
  if (infFile != "")
  {
    infStrm.open(infFile, std::ios_base::app);
    if (!infStrm.is_open())
    {
      infOut = &std::cout;
      Msg(Verbosity::Debug, *infOut, "Log::Info - '" + infFile +
        "' open failed, reverting to std::cout", true);
      infFile = "";
    }
    else
    {
      infOut = &infStrm;
    }
  }
  Msg(Verbosity::Inform, *infOut, (infText + output), endLine);
  if (infStrm.is_open()) { infStrm.close(); }
} // end Log::Info(const std::string&, bool)


// Output notational messages to the current log file                         */
void dp::Log::Note(const std::string& output, bool endLine)
{
  if (nteFile != "")
  {
    nteStrm.open(nteFile, std::ios_base::app);
    if (!nteStrm.is_open())
    {
      nteOut = &std::cout;
      Msg(Verbosity::Debug, *nteOut, "Log::Note - '" + nteFile +
        "' open failed, reverting to std::cout", true);
      nteFile = "";
    }
    else
    {
      nteOut = &nteStrm;
    }
  }
  Msg(Verbosity::Full, *nteOut, (nteText + output), endLine);
  if (nteStrm.is_open()) { nteStrm.close(); }
} // end Log::Note(const std::string&, bool)


// Set up logging options to preferred user settings (null to ignore args)    */
void dp::Log::Init(const char* argv0, ASCIIPath::SysLang lang, Verbosity
  output, const char* errorLog, const char* errorMsg, const char* warnLog,
  const char* warnMsg, const char* infoLog, const char* infoMsg, const char*
  diagLog, const char* diagMsg, const char* noteLog, const char* noteMsg)
{
  const char* uniformPrefix = "Message: ";
  const char* log = "";
  ASCIIPath basic("./");
  int provided = 0;
  bool multiple = false;
  std::vector<ASCIIPath> list;
  if (errorLog) { ++provided; log = errorLog; }
  if (warnLog)  { ++provided; log = warnLog; }
  if (infoLog)  { ++provided; log = infoLog; }
  if (diagLog)  { ++provided; log = diagLog; }
  if (noteLog)  { ++provided; log = noteLog; }
  if (provided != 1) { multiple = true; }
  VerbositySetting(output);
  if (multiple)
  {
    FileSettings((errorLog ? errorLog : log), (warnLog ? warnLog : log),
      (infoLog ? infoLog : log), (diagLog ? diagLog : log),
      (noteLog ? noteLog : log));
  }
  else
  {
    FileSetting(log);
  }
  multiple = false;
  provided = 0;
  if (errorMsg) { ++provided; uniformPrefix = errorMsg; }
  if (warnMsg)  { ++provided; uniformPrefix = warnMsg; }
  if (infoMsg)  { ++provided; uniformPrefix = infoMsg; }
  if (diagMsg)  { ++provided; uniformPrefix = diagMsg; }
  if (noteMsg)  { ++provided; uniformPrefix = noteMsg; }
  if (provided != 1)
  {
    multiple = true;
  }
  Designations(
    ((errorMsg) ? errorMsg : ((multiple) ? "Error: " : uniformPrefix)),
    ((warnMsg) ? warnMsg : ((multiple) ? "Warning: " : uniformPrefix)),
    ((infoMsg) ? infoMsg : ((multiple) ? "Info: " : uniformPrefix)),
    ((diagMsg) ? diagMsg : ((multiple) ? "Diag: " : uniformPrefix)),
    ((noteMsg) ? noteMsg : ((multiple) ? "Note: " : uniformPrefix)));
}

// Add notational text to the current log file in a running statement         */
void dp::Log::N(const std::string& output, bool endLine)
{
  if (nteFile != "")
  {
    nteStrm.open(nteFile, std::ios_base::app);
    if (!nteStrm.is_open())
    {
      nteOut = &std::cout;
      Msg(Verbosity::Debug, *nteOut, "Log::N - '" + nteFile +
        "' open failed, reverting to std::cout", true);
      nteFile = "";
    }
    else
    {
      nteOut = &nteStrm;
    }
  }
  Msg(Verbosity::Full, *nteOut, output, endLine);
  if (nteStrm.is_open()) { nteStrm.close(); }
}

// Perform cleanup: prune empty files, close file streams                     */
void dp::Log::Shutdown(void)
{
  if (errFile != "")
  {
    std::ifstream i;
    i.open(errFile, std::ifstream::ate);
    if (i.is_open() && i.tellg() == 0)
    {
      i.close();
      remove(errFile.c_str());
    }
  }
  if (wrnFile != "" && wrnFile != errFile)
  {
    std::ifstream i;
    i.open(wrnFile, std::ifstream::ate);
    if (i.is_open() && i.tellg() == 0)
    {
      i.close();
      remove(wrnFile.c_str());
    }
  }
  if (infFile != "" && infFile != wrnFile && infFile != errFile)
  {
    std::ifstream i;
    i.open(infFile, std::ifstream::ate);
    if (i.is_open() && i.tellg() == 0)
    {
      i.close();
      remove(infFile.c_str());
    }
  }
  if (dgsFile != "" && dgsFile != infFile && dgsFile != wrnFile
    && dgsFile != errFile)
  {
    std::ifstream i;
    i.open(dgsFile, std::ifstream::ate);
    if (i.is_open() && i.tellg() == 0)
    {
      i.close();
      remove(dgsFile.c_str());
    }
  }
  if (nteFile != "" && nteFile != dgsFile && nteFile != infFile
    && nteFile != wrnFile && nteFile != errFile)
  {
    std::ifstream i;
    i.open(nteFile, std::ifstream::ate);
    if (i.is_open() && i.tellg() == 0)
    {
      i.close();
      remove(nteFile.c_str());
    }
  }
  if (errFile != "") { errFile.clear(); }
  if (wrnFile != "") { wrnFile.clear(); }
  if (infFile != "") { infFile.clear(); }
  if (dgsFile != "") { dgsFile.clear(); }
  if (nteFile != "") { nteFile.clear(); }
  if (errStrm.is_open()) { errStrm.close(); }
  if (wrnStrm.is_open()) { wrnStrm.close(); }
  if (infStrm.is_open()) { infStrm.close(); }
  if (dgsStrm.is_open()) { dgsStrm.close(); }
  if (nteStrm.is_open()) { nteStrm.close(); }
}// end void Log::Shutdown(void)


// Get the current verbosity setting in use by the Log class                  */
dp::Log::Verbosity dp::Log::VerbositySetting(void)
{
  return outputLevel;
} // end void Log::VerbositySetting(void)


// Output a different amount of process flow context / degree of minutiae     */
void dp::Log::VerbositySetting(dp::Log::Verbosity sensitivity)
{
  outputLevel = sensitivity;
} // end void Log::VerbositySetting(Log::Verbosity)


// Output a different amount of process flow context / degree of minutiae     */
void dp::Log::VerbositySetting(int sensitivity)
{
  Verbosity setting;
  switch (sensitivity) // static cast should work- just safer to be explicit
  {
  case 0:
    setting = Verbosity::Silent;
    break;
  case 1:
    setting = Verbosity::Critical;
    break;
  case 2:
    setting = Verbosity::Debug;
    break;
  case 3:
    setting = Verbosity::Inform;
    break;
  case 4:
    setting = Verbosity::Diagnostic;
    break;
  default:
    setting = Verbosity::Full;
    break;
  }
  outputLevel = setting;
} // end void Log::VerbositySetting(int)


// Output warning messages to the current log file                            */
void dp::Log::Warn(const std::string& output, bool endLine)
{
  if (wrnFile != "")
  {
    wrnStrm.open(wrnFile, std::ios_base::app);
    if (!wrnStrm.is_open())
    {
      wrnOut = &std::clog;
      Msg(Verbosity::Debug, *wrnOut, "Log::Warning - '" + wrnFile +
        "' open failed, reverting to std::clog", true);
      wrnFile = "";
    }
    else
    {
      wrnOut = &wrnStrm;
    }
  }
  Msg(Verbosity::Debug, *wrnOut, (wrnText + output), endLine);
  if (wrnStrm.is_open()) { wrnStrm.close(); }
} // end void Log::Warn(const std::string&, bool)


// Add warning text to the current log file in a running statement            */
void dp::Log::W(const std::string& output, bool endLine)
{
  if (wrnFile != "")
  {
    wrnStrm.open(wrnFile, std::ios_base::app);
    if (!wrnStrm.is_open()) // Revert & signal output file open error
    {
      wrnOut = &std::clog;
      Msg(Verbosity::Debug, *wrnOut, "Log::W - '" + wrnFile +
        "' open failed, reverting to std::clog", true);
      wrnFile = "";
    }
    else
    {
      wrnOut = &wrnStrm;
    }
  }
  Msg(Verbosity::Debug, *wrnOut, output, endLine);
  if (wrnStrm.is_open()) { wrnStrm.close(); }
} // end void Log::W(const std::string&, bool)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Conditionally output a message to the given stream                         */
void dp::Log::Msg(const Verbosity level, std::ostream& out,
  const std::string& message, bool endLine)
{
  if (outputLevel >= level)
  {
    out << message;
    if (endLine) { out << "\n"; }
  }
} // end void Log::Msg(const Verbosity, ostream&, const string&, bool)
