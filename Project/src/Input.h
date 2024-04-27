/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Input.h
Purpose:  Interface for window control / input processing, with MVC decoupling
Project:  a.Surprise_CS350_1
Author:   Ari Surprise, a.surprise, 40000218
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Creation date:  Summer 2022
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

#include "Window.h"

namespace dp //! Contents for use within class assignments
{

  class Window; // Container for pointers to the window model / view / control

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Input //! Interface for window control / input processing
  {
  public:

    //! State of a given key: on or off, with a new state change or not
    enum class kState
    {
      Off,      //! Key is not pressed, nor has it been pressed recently: 00
      Released, //! Key is not pressed, and the off state change is new: 01
      Held,     //! Key is pressed, but the state is not new: 10
      Pressed,  //! Key is pressed, and the on state change is new: 11

    }; // end kState enum

    struct Key
    {
    public:
      Key() : state(kState::Off), held(0.0f) {} 
      kState state; //! Pressed state of the given input key / button
      f4 held;      //! Duration of state held

    }; // end Input::Key inner class

    //! List of individual keys on a standard english keyboard
    /*enum class enWinKey
    {
      Modifiers = -65536, //! Bitmask to extract high field modifiers from key
      Invalid = -1,   //! Reserved error state for unrecognized keystroke
      None,           //! Default null state correlated to no keystroke
      LClick,         //! Left mouse click detection state
      RClick,         //! Right mouse click detection state
      Cancel,         //! Cancel keystroke detection state
      MClick,         //! Middle(3 button) mouse click detection state 
      X1Click,        //! First x (5 button) mouse click detection state
      X2Click,        //! Second x (5 button) mouse click detection state
      Back = 8,       //! Bksp (Backspace) keystroke detection state
      Tab,            //! Tab keystroke detection state
      Line,           //! Linefeed (numpad Enter) keystroke detection state
      Clear = 12,     //! Clear keystroke detection state
      Enter,          //! Enter keystroke detection state
      Shift = 16,     //! Shift keystroke (left or right) detection state
      Ctrl,           //! CTRL (Control) keystroke (either) detection state
      Alt,            //! Alt keystroke (left or right) detection state
      Menu = Alt,     //! Alt keystroke alias: mnemonic trigger for menu access
      Pause,          //! Pause | Break keystroke detection state
      Capital,        //! Caps Lock keystroke detection state
      CapsLock=Capital,   //! Caps Lock explicit key alias (for Capital key)
      Hangul,         //! IME Hangul mode key detection
      HanguelMode=Hangul, //! IME Hangul key archaeic alias (compatibility)
      HangulMode=Hangul,  //! IME Hangul mode explicit alias (for Hangul key)
      Junja = 23,     //! IME Junja mode key detection state
      JunjaMode=Junja,//! IME Junja mode explicit alias (for Junja key)
      FinalMode,      //! IME final mode key detection state
      Hanja,          //! IME Hanja mode key detection state
      HanjaMode=Hanja,//! IME Hanja mode explicit alias (for Hanja key)
      Esc = 27,       //! ESC (Escape) keystroke detection state
      Escape = Esc,   //! ESC (Escape) key alias (for Esc key value)
      IMEConvert,     //! IME convert keystroke detection
      IMENonconvert,  //! IME non-convert keystroke detection
      IMEAccept,      //! IME accept keystroke detection state
      IMEAceept=IMEAccept,//! IME accept key alias (obsolete: use IMEAccept)
      IMEModeChange,  //! IME mode change key detection state
      Space,          //! Space bar keystroke detection state
      Spacebar=Space, //! Space bar key alias
      PgUp,           //! Page Up keystroke detection state
      PageUp = PgUp,  //! Page Up explicit key alias (for PgUp key value)
      Prior = PgUp,   //! Page Up key alias (for PgUp key value)
      PgDn,           //! Page Down keystroke detection state
      PageDown = PgDn,//! Page Down explicit key alias (for PgDn key value)
      Next = PgDn,    //! Page Down key alias (for PgDn key value)
      End,            //! End keystroke detection state
      Home,           //! Home keystroke detection state
      Left,           //! Left arrow keystroke detection state
      Up,             //! Up arrow keystroke detection state
      Right,          //! Right arrow keystroke detection state
      Down,           //! Down arrow keystroke detection state
      Select,         //! Select keystroke detection state
      Print,          //! Print keystroke detection state (apart from PrtSc)
      Execute,        //! Execute keystroke detection state
      PrtSc,          //! Print Screen key detection state (not Print key)
      Ins,            //! Ins (Insert) key detection state
      Del,            //! Del (Delete) key detection state
      Help,           //! Help key detection state (not F1 key value)
      D0,             //! Decimal 0 numeral row keystroke detection state
      D1,             //! Decimal 1 numeral row keystroke detection state
      D2,             //! Decimal 2 numeral row keystroke detection state
      D3,             //! Decimal 3 numeral row keystroke detection state
      D4,             //! Decimal 4 numeral row keystroke detection state
      D5,             //! Decimal 5 numeral row keystroke detection state
      D6,             //! Decimal 6 numeral row keystroke detection state
      D7,             //! Decimal 7 numeral row keystroke detection state
      D8,             //! Decimal 8 numeral row keystroke detection state
      D9,             //! Decimal 9 numeral row keystroke detection state
      A = 65,         //! Alphabeic A keystroke detection state
      B,              //! Alphabeic B keystroke detection state
      C,              //! Alphabeic C keystroke detection state
      D,              //! Alphabeic D keystroke detection state
      E,              //! Alphabeic E keystroke detection state
      F,              //! Alphabeic F keystroke detection state
      G,              //! Alphabeic G keystroke detection state
      H,              //! Alphabeic H keystroke detection state
      I,              //! Alphabeic I keystroke detection state
      J,              //! Alphabeic j keystroke detection state
      K,              //! Alphabeic K keystroke detection state
      L,              //! Alphabeic L keystroke detection state
      M,              //! Alphabeic M keystroke detection state
      N,              //! Alphabeic N keystroke detection state
      O,              //! Alphabeic O keystroke detection state
      P,              //! Alphabeic P keystroke detection state
      Q,              //! Alphabeic Q keystroke detection state
      R,              //! Alphabeic R keystroke detection state
      S,              //! Alphabeic S keystroke detection state
      T,              //! Alphabeic T keystroke detection state
      U,              //! Alphabeic U keystroke detection state
      V,              //! Alphabeic V keystroke detection state
      W,              //! Alphabeic W keystroke detection state
      X,              //! Alphabeic X keystroke detection state
      Y,              //! Alphabeic Y keystroke detection state
      Z,              //! Alphabeic Z keystroke detection state
      LOS,            //! Left OS (Windows|Apple) keystroke detection state
      ROS,            //! Right OS (Windows|Apple) keystroke detection state
      Apps,           //! Application keystroke detection state (MS Natural)
      Sleep = 95,     //! Computer sleep keystroke detectino state
      Num0,           //! Numeric keypad 0 keystroke detection state
      Num1,           //! Numeric keypad 1 keystroke detection state
      Num2,           //! Numeric keypad 2 keystroke detection state
      Num3,           //! Numeric keypad 3 keystroke detection state
      Num4,           //! Numeric keypad 4 keystroke detection state
      Num5,           //! Numeric keypad 5 keystroke detection state
      Num6,           //! Numeric keypad 6 keystroke detection state
      Num7,           //! Numeric keypad 7 keystroke detection state
      Num8,           //! Numeric keypad 8 keystroke detection state
      Num9,           //! Numeric keypad 9 keystroke detection state
      Multiply,       //! Numeric multiply keystroke detection state
      Asterisk=Multiply,//! Multiply key alias: Numpad multiply key symbol
      Add,            //! Numeric keypad addition keystroke detection state
      Plus = Add,     //! Numeric keypad add keystroke alias
      Separator,      //! Separator keystroke detection state
      Subtract,       //! Numeric subtraction keystroke detection state
      Decimal,        //! Numeric decimal (period) keystroke detection state
      Divide,         //! Numeric division (slash) keystroke detection state
      F1,             //! F1 (Function row) keystroke detection state
      F2,             //! F2 (Function row) keystroke detection state
      F3,             //! F3 (Function row) keystroke detection state
      F4,             //! F4 (Function row) keystroke detection state
      F5,             //! F5 (Function row) keystroke detection state
      F6,             //! F6 (Function row) keystroke detection state
      F7,             //! F7 (Function row) keystroke detection state
      F8,             //! F8 (Function row) keystroke detection state
      F9,             //! F9 (Function row) keystroke detection state
      F10,            //! F10 (Function row) keystroke detection state
      F11,            //! F11 (Function row) keystroke detection state
      F12,            //! F12 (Function row) keystroke detection state
      F13,            //! F13 (Function row) keystroke detection state
      F14,            //! F14 (Function row) keystroke detection state
      F15,            //! F15 (Function row) keystroke detection state
      F16,            //! F16 (Function row) keystroke detection state
      F17,            //! F17 (Function row) keystroke detection state
      F18,            //! F18 (Function row) keystroke detection state
      F19,            //! F19 (Function row) keystroke detection state
      F20,            //! F20 (Function row) keystroke detection state
      F21,            //! F21 (Function row) keystroke detection state
      F22,            //! F22 (Function row) keystroke detection state
      F23,            //! F23 (Function row) keystroke detection state
      F24,            //! F24 (Function row) keystroke detection state
      NumLock = 144,  //! Num Lock keystroke detection state
      Scroll,         //! Scroll Lock keystroke detection state
      LShift = 160,   //! Left shift keystroke detection state
      RShift,         //! Right shift keystroke detection state
      LCtrl,          //! Left CTRL (Control) keystroke detection state
      RCtrl,          //! Right CTRL (Control) keystroke detection state
      LAlt,           //! Left Alt (Menu) keystroke detection state
      RAlt,           //! Right Alt (Menu) keystroke detection state
      BrowserBack,    //! Browser back keystroke detection state
      BrowserForward, //! Browser forward keystroke detection state
      BrowserRefresh, //! Browser refresh keystroke detection state
      BrowserStop,    //! Browser stop keystroke detection state
      BrowserSearch,  //! Browser search keystroke detection state
      BrowserFav,     //! Browser favorites keystroke detection state
      BrowserHome,    //! Browser home keystroke detection state
      VolumeMute,     //! Volume Mute keystroke detection state
      VolumeDown,     //! Volume Down keystroke detection state
      VolumeUp,       //! Volume Up keystroke detection state
      MediaNext,      //! Multimedia Next track keystroke detection state
      MediaPrev,      //! Multimedia Previous track keystroke detection state
      MediaStop,      //! Multimedia Stop keystroke detection state
      MediaPlay,      //! Multimedia Play / Pause keystroke detection state
      LaunchMail,     //! Launch Mail app keystroke detection state
      SelectMedia,    //! Select Media app keystroke detection state
      LaunchApp1,     //! Launch custom Application 1 key detection state
      LaunchApp2,     //! Launch custom Application 2 key detection state
      OEM1 = 186,     //! OEM 1 keystroke detection state
      OEMSemicolon=OEM1,    //! OEM Semi-colon | OEM1 key alias
      OEMPlus,        //! OEM plus (multi-region) keystroke detection state
      OEMComma,       //! OEM comma (multi-region) keystroke detection state
      OEMMinus,       //! OEM minus (multi-region) keystroke detection state
      OEMPeriod,      //! OEM period (multi-region) keystroke detection state
      OEM2,           //! OEM 2 keystroke detection state
      OEMQuestion=OEM2,     //! OEM Question mark | OEM2 key alias
      OEM3,           //! OEM 3 keystroke detection state
      OEMTilde=OEM3,  //! OEM Tilde/Backquote | OEM3 key alias
      OEM4 = 219,     //! OEM 4 keystroke detection state
      OEMOpenBrackets=OEM4, //! OEM Left (Open) Brackets | OEM4 key alias
      OEM5,           //! OEM 5 keystroke detection state
      OEMPipe = OEM5, //! OEM Backslash / Pipe | OEM5 key alias
      OEM6,           //! OEM 6 keystroke detection state
      OEMCloseBrackets=OEM6,//! OEM Right (Close) Brackets | OEM6 key alias
      OEM7,           //! OEM 7 keystroke detection state
      OEMQuotes=OEM7, //! OEM single/double quote | OEM7 key alias
      OEM8,           //! OEM 8 keystroke detection state
      OEM102,         //! OEM 102 keystroke detection state
      OEMBackslash=OEM102,  //! OEM angle bracket | backslash | 102 key alias
      ProcessKey=229, //! Process keystroke detection state
      Packet = 231,   //! Packet key value to pass unicode values as strokes
      Attn = 246,     //! ATTN (Attention) key value detection state
      Crsel,          //! CRSEL (Carousel) key value detection state
      Exsel,          //! EXSEL (Extented Selection) key value detection state
      EraseEOF,       //! Erase end of field chars (->'\0'; retain cursor pos)
      Play,           //! Play keystroke detection state
      Zoom,           //! Zoom keystroke detection state
      NoName,         //! Constant reserved for future use
      PA1,            //! PA1 keystroke detection state
      OEMClear,       //! Clear keystroke detection state
      Total = 171,    //! Book-keeping value for total valid keys defined
      All = Total,    //! Book-keeping value for total valid keys defined
      Any = All,      //! Book-keeping value for total valid keys defined
      KeyCode=65535,  //! Bitmask value to extract lower field code from key
      MShift=65536,   //! Shift modifier key detection state
      MCtrl=131072,   //! CTRL (Control) modifier key detection state
      MAlt=262144,    //! Aly (menu; mnemonic) modifier key detection state

    }; // end enWinKey enum                                                   */

    //! Individual pressable key strokes in a standard US English keyboard
    enum class enKey
    {
      // Null / Empty / Spacer keys: [-1,5]
      Invalid = -1,     //! Reserved key match error state value
      None,             //! Null key value: no keystroke detection state
      Esc,              //! Esc (Escape) keystroke detection state
      Escape = Esc,     //! Esc (Escape) keystroke detection state
      Enter,            //! Enter / Return (New Line) keystroke detection state
      Return = Enter,   //! Enter / Return (New Line) keystroke detection state
      Spacebar,         //! Space Bar keystroke detection state (Space alias)
      Space = Spacebar, //! Space Bar keystroke detection state
      Spc = Spacebar,   //! Space Bar keystroke detection state (Space alias)
      Backspace,        //! BKSP (Backspace) keystroke detection state
      Del,              //! Del (Delete) keystroke detection state
      Delete = Del,     //! Del (Delete) keystroke detection state
      // (tab notably later on to discourage use as whitespace over a modifier)
      
      // Numerical markup keys: [6,9]; precede #'s with -|+ and .|,
      Comma,            //! Comma / Lesser / Left Angle Brace key state
      Less = Comma,     //! Comma / Lesser / Left Angle Brace key state
      LessThan = Less,  //! Comma / Lesser / Left Angle Brace key state
      LAngleBr = Less,  //! Comma / Lesser / Left Angle Brace key state
      OpenAngBr = Less, //! Comma / Lesser / Left Angle Brace key state
      Period,           //! Period / Greater / Right Angle Brace key state
      More = Period,    //! Period / Greater / Right Angle Brace key state
      MoreThan = More,  //! Period / Greater / Right Angle Brace key state
      Greater = More,   //! Period / Greater / Right Angle Brace key state
      RAngleBr = More,  //! Period / Greater / Right Angle Brace key state
      CloseAngBr = More,//! Period / Greater / Right Angle Brace key state
      Equal,            //! Digit Row Equals Sign / Plus key detection state
      Plus = Equal,     //! Digit Row Equals Sign / Plus key detection state
      Dash,             //! Digit Row Dash / Hyphen / Minus / Underscore key
      Hyphen = Dash,    //! Digit Row Dash / Hyphen / Minus / Underscore key
      Underscore = Dash,//! Digit Row Dash / Hyphen / Minus / Underscore key

      // Alphanumeric keys: [10,45]; begin at base 10, moving to larger # bases
      D0,               //! Digit Row Numeral 0 / Right-Parentheses key state
      RParens = D0,     //! Digit Row Numeral 0 / Right-Parentheses key state
      RParenthesis = D0,//! Digit Row Numeral 0 / Right-Parentheses key state
      CloseParens = D0, //! Digit Row Numeral 0 / Right-Parentheses key state
      D1,               //! Digit Row Numeral 1 / Exclamation-Mark key state
      Exclamation = D1, //! Digit Row Numeral 1 / Exclamation-Mark key state
      D2,               //! Digit Row Numeral 2 / Commercial-At-Sign key state
      AtSign = D2,      //! Digit Row Numeral 2 / Commercial-At-Sign key state
      D3,               //! Digit Row Numeral 3 / Hash / Pound-Sign key state
      Hash = D3,        //! Digit Row Numeral 3 / Hash / Pound-Sign key state
      Pound = Hash,     //! Digit Row Numeral 3 / Hash / Pound-Sign key state
      D4,               //! Digit Row Numeral 4 / Dollar-Sign key state
      Dollar = D4,      //! Digit Row Numeral 4 / Dollar-Sign key state
      D5,               //! Digit Row Numeral 5 / Percent-Sign key state
      Percent = D5,     //! Digit Row Numeral 5 / Percent-Sign key state
      D6,               //! Digit Row Numeral 6 / Caret / Point-Up key state
      Caret = D6,       //! Digit Row Numeral 6 / Caret / Point-Up key state
      D7,               //! Digit Row Numeral 7 / Ampersand key state
      Ampersand = D7,   //! Digit Row Numeral 7 / Ampersand key state
      D8,               //! Digit Row Numeral 8 / Asterisk key state
      Asterisk = D8,    //! Digit Row Numeral 8 / Asterisk key state
      D9,               //! Digit Row Numeral 9 / Left-Parentheses key state
      LParens = D9,     //! Digit Row Numeral 9 / Left-Parentheses key state
      LParenthesis = D9,//! Digit Row Numeral 9 / Left-Parentheses key state
      OpenParens = D9,  //! Digit Row Numeral 9 / Left-Parentheses key state
      A,                //! Alphabetic Character A keystroke detection state
      B,                //! Alphabetic Character B keystroke detection state
      C,                //! Alphabetic Character C keystroke detection state
      D,                //! Alphabetic Character D keystroke detection state
      E,                //! Alphabetic Character E keystroke detection state
      F,                //! Alphabetic Character F keystroke detection state
      G,                //! Alphabetic Character G keystroke detection state
      H,                //! Alphabetic Character H keystroke detection state
      I,                //! Alphabetic Character I keystroke detection state
      J,                //! Alphabetic Character J keystroke detection state
      K,                //! Alphabetic Character K keystroke detection state
      L,                //! Alphabetic Character L keystroke detection state
      M,                //! Alphabetic Character M keystroke detection state
      N,                //! Alphabetic Character N keystroke detection state
      O,                //! Alphabetic Character O keystroke detection state
      P,                //! Alphabetic Character P keystroke detection state
      Q,                //! Alphabetic Character Q keystroke detection state
      R,                //! Alphabetic Character R keystroke detection state
      S,                //! Alphabetic Character S keystroke detection state
      T,                //! Alphabetic Character T keystroke detection state
      U,                //! Alphabetic Character U keystroke detection state
      V,                //! Alphabetic Character V keystroke detection state
      W,                //! Alphabetic Character W keystroke detection state
      X,                //! Alphabetic Character X keystroke detection state
      Y,                //! Alphabetic Character Y keystroke detection state
      Z,                //! Alphabetic Character Z keystroke detection state

      // Grammatical marker keys: [46,50]; kept adjacent to alphanumerics
      Apostrophe,       //! Apostrophe / Quote keystroke detection state
      Quote=Apostrophe, //! Apostrophe / Quote keystroke detection state
      LBracket,         //! Left Brackets / Curly Brackets key detection state
      LCurlyBr=LBracket,//! Left Brackets / Curly Brackets key detection state
      RBracket,         //! Right Brackets / Curly Brackets key detection state
      RCurlyBr=RBracket,//! Right Brackets / Curly Brackets key detection state
      Semicolon,        //! Semi-Colon / Colon keystroke detection state
      Colon = Semicolon,//! Semi-Colon / Colon keystroke detection state
      Slash,            //! Slash / Question Mark keystroke detection state
      Question = Slash, //! Slash / Question Mark keystroke detection state

      // Program function keys: [51,63]; closes major keys in lower field mask
      F1,               //! Function row F1 (Help) keystroke detection state
      F2,               //! Function row F2 (Edit) keystroke detection state
      F3,               //! Function row F3 (Index) keystroke detection state
      F4,               //! Function row F4 (Close) keystroke detection state
      F5,               //! Function row F5 (Reload) keystroke detection state
      F6,               //! Function row F6 (Focus) keystroke detection state
      F7,               //! Function row F7 (Check) keystroke detection state
      F8,               //! Function row F8 (Access) keystroke detection state
      F9,               //! Function row F9 (Update) keystroke detection state
      F10,              //! Function row F10 (Context) keystroke detection state
      F11,              //! Function row F11 (Toggle Fullscreen) keystroke state
      F12,              //! Function row F12 (Export) keystroke detection state
      BSlash,           //! Backslash / Pipe keystroke detection state
      Pipe = BSlash,    //! Backslash / Pipe keystroke detection state
      // irreg support => '\' best of any candidate key to align F# with D#

      // Modifier keys: [64,75]; semantic nulls at low of field >= 64 division
      Tab,              //! Tab (advance field) keystroke detection state
      Tilde,            //! Tilde / Backquote keystroke detection state
      Backquote = Tilde,//! Tilde / Backquote keystroke detection state
      LShift,           //! Left Shift keystroke detection state
      RShift,           //! Right Shift keystroke detection state
      LCtrl,            //! Left Ctrl (Control) keystroke detection state
      LCommand = LCtrl, //! Left Ctrl (Control) keystroke detection state
      RCtrl,            //! Right Ctrl (Control) keystroke detection state
      RCommand = RCtrl, //! Right Ctrl (Control) keystroke detection state
      LAlt,             //! Left Alt (Option|Menu) keystroke detection state
      LOption = LAlt,   //! Left Alt (Option|Menu) keystroke detection state
      RAlt,             //! Right Alt (Option|Menu) keystroke detection state
      ROption = RAlt,   //! Right Alt (Option|Menu) keystroke detection state
      LOS,              //! Left OS (Windows|Apple) keystroke detection state
      ROS,              //! Right OS (Windows|Apple) keystroke detection state
      Menu, //! Context (Right click menu) key: not Windows' Mnemonics Alt alias
      Context = Menu,   //! Context (Right click menu) key detection state
      Fn,               //! Fn (Function|Hardware Manipulation) keystroke state

      // Navigation keys: [76,83]; segue from modifiers to input mode changes
      Right,            //! Directional Arrow Right keystroke detection state
      Up,               //! Directional Arrow Up keystroke detection state
      Left,             //! Directional Arrow Left keystroke detection state
      Down,             //! Directional Arrow Down keystroke detection state
      Home,             //! Navigation to Home (Head) keystroke detection state
      End,              //! Navigation to End (Tail) keystroke detection state
      PageUp,           //! Page Up keystroke detection state
      PageDown,         //! Page Down keystroke detection state

      // Input mode change keys: [84,88]; pre-numpad range for NumLk continuity
      PrtSc,            //! PrtSc (Print Screen) keystroke detection state
      PrintScreen=PrtSc,//! PrtSc (Print Screen) keystroke detection state
      SysRq = PrtSc,    //! System Request alias for Print Screen key (if Alt+)
      SysRequest=PrtSc, //! System Request = PrtSc alias (dedicated key rare)
      Pause,            //! Pause | Break keystroke detection state
      Break = Pause,    //! Pause | Break keystroke detection state
      Ins,              //! Ins (Insert) keystroke detection state
      Insert = Ins,     //! Ins (Insert) keystroke detection state
      ScrLk,            //! ScrLk (Scroll Lock) keystroke detection state
      ScrollLock=ScrLk, //! ScrLk (Scroll Lock) keystroke detection state
      CapsLock,         //! Caps Lock keystroke detectino state
      Caps = CapsLock,  //! Caps Lock keystroke detection state

      // Numeric Keypad  keys: [89,105]; trailing, as 104-ext keys not universal
      NumLock,          //! NumLk (Number Lock) keystroke detection state
      NumLk = NumLock,  //! NumLk (Number Lock) keystroke detection state
      N0,               //! Numpad Numeral 0 (Ins) keystroke detection state
      N1,               //! Numpad Numeral 1 (End) keystroke detection state
      N2,               //! Numpad Numeral 2 (Down) keystroke detection state
      N3,               //! Numpad Numeral 3 (PgDn) keystroke detection state
      N4,               //! Numpad Numeral 4 (Left) keystroke detection state
      N5,               //! Numpad Numeral 5 (Sel) keystroke detection state
      N6,               //! Numpad Numeral 6 (Right) keystroke detection state
      N7,               //! Numpad Numeral 7 (Home) keystroke detection state
      N8,               //! Numpad Numeral 8 (Up) keystroke detection state
      N9,               //! Numpad Numeral 9 (PgUp) keystroke detection state
      Dot,              //! Numpad Decimal Point / Dot / Del key detection state
      Decimal = Dot,    //! Numpad Decimal Point / Dot / Del key detection state
      Minus,            //! Numpad Minus / Subtract / Hyphen key detection
      Subtract = Minus, //! Numpad Minus / Subtract / Hyphen key detection
      Add,              //! Numpad Add / Plus keystroke detection state
      Multiply,         //! Numpad Multiply / Asterisk key detection state
      Divide,           //! Numpad Divide / Slash keystroke detection state
      LineFeed,         //! Numpad Enter (Line Feed) keystroke detection state
      NEnter = LineFeed,//! Numpad Enter (Line Feed) keystroke detection state
      NPEqual = NEnter, //! Numpad Equals alias (International layout Enter key)

      // Multimedia keys: [119,125]; least ubiquitous, most modern keys trailing
      MediaStop,        //! Multimedia Stop keystroke detection state
      PlayPause,        //! Multimedia Play / Pause keystroke detection state
      MediaPlay=PlayPause,  //! Multimedia Play / Pause keystroke state
      MediaPause=PlayPause, //! Multimedia Play / Pause keystroke state
      MediaPrev,        //! Multimedia Previous track keystroke detection state
      MediaNext,        //! Multimedia Next track keystroke detection state
      VolumeUp,         //! Volume Up keystroke detection state
      VolUp=VolumeUp,   //! Volume Up keystroke detection state
      VolumeDown,       //! Volume Down keystroke detection state
      VolDn=VolumeDown, //! Volume Down keystroke detection state
      VolumeMute,       //! Volume Mute keystroke detection state
      Mute = VolumeMute,//! Volume Mute keystroke detection state
      VolMt=VolumeMute, //! Volume Mute keystroke detection state

      // Secondary function keys: [113,125]; less universal keys non-contiguous
      F13,              //! Function row F13 keystroke detection state
      F14,              //! Function row F14 keystroke detection state
      F15,              //! Function row F15 keystroke detection state
      F16,              //! Function row F16 keystroke detection state
      F17,              //! Function row F17 keystroke detection state
      F18,              //! Function row F18 keystroke detection state
      F19,              //! Function row F19 keystroke detection state
      F20,              //! Function row F20 keystroke detection state
      F21,              //! Function row F21 keystroke detection state
      F22,              //! Function row F22 keystroke detection state
      F23,              //! Function row F23 keystroke detection state
      F24,              //! Function row F24 keystroke detection state
      F25,              //! Function row F25 keystroke detection state

      // Mouse input: [126,132]; First standard input post full keyboard
      LClick,           //! Mouse Left Button Click detection state
      RClick,           //! Mouse Right Button Click detection state
      // (128 value division)
      MClick,           //! Mouse Middle (3 button) Click detection state
      MScrollDn,        //! Mouse Middle (3 button) Scroll Down (toward) state
      MScrollUp,        //! Mouse Middle (3 button) Scroll Up (away) state
      X1Click,          //! Mouse X1 (5 button) Click detection state
      X2Click,          //! Mouse X2 (5 button) Click detection state

      // Joypad XInput input: [133,228] #1 
      Pad1Joy1Left,     //! JoyPad #1 Left Joystick horizontal tilt (-X) state
      Pad1Joy1Up,       //! JoyPad #1 Left Joystick vertical tilt (+Y) state
      Pad1Joy1Right,    //! JoyPad #1 Left Joystick horizontal tilt (+X) state
      Pad1Joy1Down,     //! JoyPad #1 Left Joystick vertical tilt (-Y) state
      Pad1Joy2Left,     //! JoyPad #1 Right Joystick horizontal tilt (-X) state
      Pad1Joy2Up,       //! JoyPad #1 Right Joystick vertical tilt (+Y) state
      Pad1Joy2Right,    //! JoyPad #1 Right Joystick horizontal tilt (+X) state
      Pad1Joy2Down,     //! JoyPad #1 Right Joystick vertical tilt (-Y) state
      Pad1Left,         //! JoyPad #1 Directional Pad Left button press state
      Pad1Up,           //! JoyPad #1 Directional Pad Up button press state
      Pad1Right,        //! JoyPad #1 Directional Pad Right button press state
      Pad1Down,         //! JoyPad #1 Directional Pad Down button press state
      Pad1Mode,         //! JoyPad #1 Mode (Select) button press state
      Pad1Menu,         //! JoyPad #1 Menu (Start) button press state
      Pad1A,            //! JoyPad #1 Face A (Nintendo B) button press state
      Pad1B,            //! JoyPad #1 Face B (Nintendo A) button press state
      Pad1X,            //! JoyPad #1 Face X (Nintendo Y) button press state
      Pad1Y,            //! JoyPad #1 Face Y (Nintendo X) button press state
      Pad1L,            //! JoyPad #1 Shoulder L (Sony L1) button press state
      Pad1R,            //! JoyPad #1 Shoulder R (Sony R1) button press state
      Pad1LT,           //! JoyPad #1 Shoulder LT (Sony L2) -Z tilt state
      Pad1RT,           //! JoyPad #1 Shoulder RT (Sony R2) +Z tilt state
      Pad1L3,           //! JoyPad #1 Left Joystick Press L3 button state
      Pad1R3,           //! JoyPad #1 Right Joystick Press R3 button state
      // Joypad XInput input #2
      Pad2Joy1Left,     //! JoyPad #2 Left Joystick horizontal tilt (-X) state
      Pad2Joy1Up,       //! JoyPad #2 Left Joystick vertical tilt (+Y) state
      Pad2Joy1Right,    //! JoyPad #2 Left Joystick horizontal tilt (+X) state
      Pad2Joy1Down,     //! JoyPad #2 Left Joystick vertical tilt (-Y) state
      Pad2Joy2Left,     //! JoyPad #2 Right Joystick horizontal tilt (-X) state
      Pad2Joy2Up,       //! JoyPad #2 Right Joystick vertical tilt (+Y) state
      Pad2Joy2Right,    //! JoyPad #2 Right Joystick horizontal tilt (+X) state
      Pad2Joy2Down,     //! JoyPad #2 Right Joystick vertical tilt (-Y) state
      Pad2Left,         //! JoyPad #2 Directional Pad Left button press state
      Pad2Up,           //! JoyPad #2 Directional Pad Up button press state
      Pad2Right,        //! JoyPad #2 Directional Pad Right button press state
      Pad2Down,         //! JoyPad #2 Directional Pad Down button press state
      Pad2Mode,         //! JoyPad #2 Mode (Select) button press state
      Pad2Menu,         //! JoyPad #2 Menu (Start) button press state
      Pad2A,            //! JoyPad #2 Face A (Nintendo B) button press state
      Pad2B,            //! JoyPad #2 Face B (Nintendo A) button press state
      Pad2X,            //! JoyPad #2 Face X (Nintendo Y) button press state
      Pad2Y,            //! JoyPad #2 Face Y (Nintendo X) button press state
      Pad2L,            //! JoyPad #2 Shoulder L (Sony L1) button press state
      Pad2R,            //! JoyPad #2 Shoulder R (Sony R1) button press state
      Pad2LT,           //! JoyPad #2 Shoulder LT (Sony L2) -Z tilt state
      Pad2RT,           //! JoyPad #2 Shoulder RT (Sony R2) +Z tilt state
      Pad2L3,           //! JoyPad #2 Left Joystick Press L3 button state
      Pad2R3,           //! JoyPad #2 Right Joystick Press R3 button state
      // Joypad XInput input #3
      Pad3Joy1Left,     //! JoyPad #3 Left Joystick horizontal tilt (-X) state
      Pad3Joy1Up,       //! JoyPad #3 Left Joystick vertical tilt (+Y) state
      Pad3Joy1Right,    //! JoyPad #3 Left Joystick horizontal tilt (+X) state
      Pad3Joy1Down,     //! JoyPad #3 Left Joystick vertical tilt (-Y) state
      Pad3Joy2Left,     //! JoyPad #3 Right Joystick horizontal tilt (-X) state
      Pad3Joy2Up,       //! JoyPad #3 Right Joystick vertical tilt (+Y) state
      Pad3Joy2Right,    //! JoyPad #3 Right Joystick horizontal tilt (+X) state
      Pad3Joy2Down,     //! JoyPad #3 Right Joystick vertical tilt (-Y) state
      Pad3Left,         //! JoyPad #3 Directional Pad Left button press state
      Pad3Up,           //! JoyPad #3 Directional Pad Up button press state
      Pad3Right,        //! JoyPad #3 Directional Pad Right button press state
      Pad3Down,         //! JoyPad #3 Directional Pad Down button press state
      Pad3Mode,         //! JoyPad #3 Mode (Select) button press state
      Pad3Menu,         //! JoyPad #3 Menu (Start) button press state
      Pad3A,            //! JoyPad #3 Face A (Nintendo B) button press state
      Pad3B,            //! JoyPad #3 Face B (Nintendo A) button press state
      Pad3X,            //! JoyPad #3 Face X (Nintendo Y) button press state
      Pad3Y,            //! JoyPad #3 Face Y (Nintendo X) button press state
      Pad3L,            //! JoyPad #3 Shoulder L (Sony L1) button press state
      Pad3R,            //! JoyPad #3 Shoulder R (Sony R1) button press state
      Pad3LT,           //! JoyPad #3 Shoulder LT (Sony L2) -Z tilt state
      Pad3RT,           //! JoyPad #3 Shoulder RT (Sony R2) +Z tilt state
      Pad3L3,           //! JoyPad #3 Left Joystick Press L3 button state
      Pad3R3,           //! JoyPad #3 Right Joystick Press R3 button state
      // Joypad XInput input #4
      Pad4Joy1Left,     //! JoyPad #4 Left Joystick horizontal tilt (-X) state
      Pad4Joy1Up,       //! JoyPad #4 Left Joystick vertical tilt (+Y) state
      Pad4Joy1Right,    //! JoyPad #4 Left Joystick horizontal tilt (+X) state
      Pad4Joy1Down,     //! JoyPad #4 Left Joystick vertical tilt (-Y) state
      Pad4Joy2Left,     //! JoyPad #4 Right Joystick horizontal tilt (-X) state
      Pad4Joy2Up,       //! JoyPad #4 Right Joystick vertical tilt (+Y) state
      Pad4Joy2Right,    //! JoyPad #4 Right Joystick horizontal tilt (+X) state
      Pad4Joy2Down,     //! JoyPad #4 Right Joystick vertical tilt (-Y) state
      Pad4Left,         //! JoyPad #4 Directional Pad Left button press state
      Pad4Up,           //! JoyPad #4 Directional Pad Up button press state
      Pad4Right,        //! JoyPad #4 Directional Pad Right button press state
      Pad4Down,         //! JoyPad #4 Directional Pad Down button press state
      Pad4Mode,         //! JoyPad #4 Mode (Select) button press state
      Pad4Menu,         //! JoyPad #4 Menu (Start) button press state
      Pad4A,            //! JoyPad #4 Face A (Nintendo B) button press state
      Pad4B,            //! JoyPad #4 Face B (Nintendo A) button press state
      Pad4X,            //! JoyPad #4 Face X (Nintendo Y) button press state
      Pad4Y,            //! JoyPad #4 Face Y (Nintendo X) button press state
      Pad4L,            //! JoyPad #4 Shoulder L (Sony L1) button press state
      Pad4R,            //! JoyPad #4 Shoulder R (Sony R1) button press state
      Pad4LT,           //! JoyPad #4 Shoulder LT (Sony L2) -Z tilt state
      Pad4RT,           //! JoyPad #4 Shoulder RT (Sony R2) +Z tilt state
      Pad4L3,           //! JoyPad #4 Left Joystick Press L3 button state
      Pad4R3,           //! JoyPad #4 Right Joystick Press R3 button state
      // (end XInput device count support: 4)

      // Touch / Pen input event codes: [229,249]; keep universal inputs < 128
      Toggle,           //! Toggle absolute | relative pen coord mode state
      TabMode = Toggle, //! Toggle absolute | relative pen coord mode state
      PenMode = Toggle, //! Toggle absolute | relative pen coord mode state
      MouseMode=Toggle, //! Toggle absolute | relative pen coord mode state
      EraseHover,       //! Rear Stylus Tip Passive Hover detection state
      PenHover,         //! Pen Tip Passive Hover detection state
      PenTouch,         //! Stylus Tip contact detection state
      // (duration can differentiate tap, double tap and press events)
      Touch,            //! Finger input detection state
      MultiTouch,       //! Two / Three Finger input detection state
      PenSide1,         //! Stylus Side Switch button 1 (low end) click state
      PenSide2,         //! Stylus Side Switch button 2 (high end) click state
      // (128 key set value division delineates branded peripheral device input)
      PenSideOut,       //! Wacom Stylus Airbrush Wheel Rotate Out detection
      PenSideIn,        //! Wacom Stylus Airbrush Wheel Rotate In detection
      RingMode,         //! Wacom Touch Strip / Ring next mode detection state
      RingCW,           //! Wacom Touch Ring CW / Strip Down touch detection
      RingCCW,          //! Wacom Touch Ring CCW / Strip Up touch detection
      Express1,         //! Wacom Express Key 1 press detection state 
      Express2,         //! Wacom Express Key 2 press detection state
      Express3,         //! Wacom Express Key 3 press detection state
      Express4,         //! Wacom Express Key 4 press detection state
      Express5,         //! Wacom Express Key 5 press detection state
      Express6,         //! Wacom Express Key 6 press detection state
      Express7,         //! Wacom Express Key 7 press detection state
      Express8,         //! Wacom Express Key 8 press detection state

      // Record state values
      Total,            //! Book-keeping value for total number of keys listed
      All = Total,      //! Book-keeping value for total number of keys listed
      Any = All         //! Book-keeping value for total number of keys listed

    }; // end enKey enum                                                      */

    static constexpr um AllKeys = static_cast<um>(enKey::Total);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Constructor to force derived classes to specify Window being controlled
    \brief
      - constructor to force derived classes to specify Window being controlled
    \param parent
      - window to be controlled / given directives by this input processor
    */
    Input(Window* parent);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    
    /** Call packaging functions to post events leading to Shutdown being called
    \brief
      - get the delta in time (seconds elapsed), from prior frame to now
    \details
      - to be calculated where per frame input is polled each frame, not here
      (it's not desirable to have a new delta calculated per Dt() call)
    */
    virtual void Close(void) = 0;
    
    /** Get the delta in time (seconds elapsed), from prior frame to now
    \brief
      - get the delta in time (seconds elapsed), from prior frame to now
    \details
      - to be calculated where per frame input is polled each frame, not here
      (it's not desirable to have a new delta calculated per Dt() call)
    */
    virtual f4 Dt(void) = 0;


    /** Update what input is currently read in from hardware
    \brief
      - update what input is currently read in from hardware
    \return
      - reference to the Input instance for easier subsequent inline actions
    */
    virtual Input& Poll(void) = 0;


    /** Use the polled input to perform action per program settings
    \brief
      - use the polled input to perform action per program settings
    \return
      - reference to the modified Input for easier subsequent inline action
    */
    Input& Process(void);


    /** Process, then update what input is currently read in from hardware
    \brief
      - process, then update what input is currently read in from hardware
    \return
      - reference to the Input instance for easier subsequent inline actions
    */
    virtual Input& Read(void) = 0;


    /** Get whether the input is in a ready state to poll input (!done)
    \brief
      - get whether the input is in a ready state to poll input (!done)
    \return
      - true if done is false (derived Input classes manage protected memeber)
    */
    bool Running(void) const;


    /** Perform derived member cleanup (base class self-sufficient)
    \brief
      - perform derived member cleanup (base class self-sufficient)
    */
    virtual void Shutdown(void) = 0;


    /** Get the name of the given input EN US keyboard key
    \brief
      - get the name of the given input EN US keyboard key
    \param in
      - input keyboard key for which the label / name is desired
    \return
      - ASCII string label associated to the given input key enum value 
    */
    inline static std::string Label(enKey in)
    {
      switch (in)
      {
      case enKey::None:        //! Null key value: no keystroke
        return "{ }";
      case enKey::Escape:       //! Esc (Escape) keystroke
        return "Esc";
      case enKey::Return:       //! Enter / Return (New Line) keystroke
        return "Enter";
      case enKey::Spacebar:     //! Space Bar keystroke (Space alias)
        return "' '";
      case enKey::Backspace:    //! BKSP (Backspace) keystroke
        return "Back";
      case enKey::Del:          //! Del (Delete) keystroke
        return "Del";

      case enKey::Comma:        //! Comma / Lesser / Left Angle Brace keystroke
        return ", | <";
      case enKey::Period:       //! Period / Greater / Right Angle Brace key
        return ". | >";
      case enKey::Equal:        //! Digit Row Equals Sign / Plus keystroke
        return "= | +";
      case enKey::Dash:         //! Digit Row Hyphen / Minus / Underscore key
        return "- | _";

      case enKey::D0:           //! Digit Row Numeral 0 / Right-Parentheses key
        return "0 | )";
      case enKey::D1:           //! Digit Row Numeral 1 / Exclamation-Mark key
        return "1 | !";
      case enKey::D2:           //! Digit Row Numeral 2 / Commercial-At key
        return "2 | @";
      case enKey::D3:           //! Digit Row Numeral 3 / Hash / Pound-Sign key
        return "3 | #";
      case enKey::D4:           //! Digit Row Numeral 4 / Dollar-Sign keystroke
        return "4 | $";
      case enKey::D5:           //! Digit Row Numeral 5 / Percent-Sign key
        return "5 | %";
      case enKey::D6:           //! Digit Row Numeral 6 / Caret / Point-Up key
        return "6 | ^";
      case enKey::D7:           //! Digit Row Numeral 7 / Ampersand keystroke
        return "7 | &";
      case enKey::D8:           //! Digit Row Numeral 8 / Asterisk keystroke
        return "8 | *";
      case enKey::D9:           //! Digit Row Numeral 9 / Left-Parentheses key
        return "9 | (";
      case enKey::A:            //! Alphabetic Character A keystroke
        return "a | A";
      case enKey::B:            //! Alphabetic Character B keystroke
        return "b | B";
      case enKey::C:            //! Alphabetic Character C keystroke
        return "c | C";
      case enKey::D:            //! Alphabetic Character D keystroke
        return "d | D";
      case enKey::E:            //! Alphabetic Character E keystroke
        return "e | E";
      case enKey::F:            //! Alphabetic Character F keystroke
        return "f | F";
      case enKey::G:            //! Alphabetic Character G keystroke
        return "g | G";
      case enKey::H:            //! Alphabetic Character H keystroke
        return "h | H";
      case enKey::I:            //! Alphabetic Character I keystroke
        return "i | I";
      case enKey::J:            //! Alphabetic Character J keystroke
        return "j | J";
      case enKey::K:            //! Alphabetic Character K keystroke
        return "k | K";
      case enKey::L:            //! Alphabetic Character L keystroke
        return "l | L";
      case enKey::M:            //! Alphabetic Character M keystroke
        return "m | M";
      case enKey::N:            //! Alphabetic Character N keystroke
        return "n | N";
      case enKey::O:            //! Alphabetic Character O keystroke
        return "o | O";
      case enKey::P:            //! Alphabetic Character P keystroke
        return "p | P";
      case enKey::Q:            //! Alphabetic Character Q keystroke
        return "q | Q";
      case enKey::R:            //! Alphabetic Character R keystroke
        return "r | R";
      case enKey::S:            //! Alphabetic Character S keystroke
        return "s | S";
      case enKey::T:            //! Alphabetic Character T keystroke
        return "t | T";
      case enKey::U:            //! Alphabetic Character U keystroke
        return "u | U";
      case enKey::V:            //! Alphabetic Character V keystroke
        return "v | V";
      case enKey::W:            //! Alphabetic Character W keystroke
        return "w | W";
      case enKey::X:            //! Alphabetic Character X keystroke
        return "x | X";
      case enKey::Y:            //! Alphabetic Character Y keystroke
        return "y | Y";
      case enKey::Z:            //! Alphabetic Character Z keystroke
        return "z | Z";

      case enKey::Apostrophe:   //! Apostrophe / Quote keystroke
        return "' | \"";
      case enKey::LBracket:     //! Left Brackets / Curly Brackets keystroke
        return "[ | {";
      case enKey::RBracket:     //! Right Brackets / Curly Brackets keystroke
        return "] | {";
      case enKey::Semicolon:    //! Semi-Colon / Colon keystroke
        return "; | :";
      case enKey::Slash:        //! Slash / Question Mark keystroke
        return "/ | ?";

      case enKey::F1:           //! Function row F1 keystroke
        return "F1";
      case enKey::F2:           //! Function row F2 keystroke
        return "F2";
      case enKey::F3:           //! Function row F3 keystroke
        return "F3";
      case enKey::F4:           //! Function row F4 keystroke
        return "F4";
      case enKey::F5:           //! Function row F5 keystroke
        return "F5";
      case enKey::F6:           //! Function row F6 keystroke
        return "F6";
      case enKey::F7:           //! Function row F7 keystroke
        return "F7";
      case enKey::F8:           //! Function row F8 keystroke
        return "F8";
      case enKey::F9:           //! Function row F9 keystroke
        return "F9";
      case enKey::F10:          //! Function row F10 keystroke
        return "F10";
      case enKey::F11:          //! Function row F11 keystroke
        return "F11";
      case enKey::F12:          //! Function row F12 keystroke
        return "F12";
      case enKey::BSlash:       //! Backslash / Pipe keystroke
        return "\\ | |";

      case enKey::Tab:          //! Tab keystroke
        return "Tab";
      case enKey::Tilde:        //! Tilde / Backquote keystroke
        return "` | ~";
      case enKey::LShift:       //! Left Shift keystroke
        return "(L) Shift";
      case enKey::RShift:       //! Right Shift keystroke
        return "(R) Shift";
      case enKey::LCtrl:        //! Left Ctrl (Control) keystroke
        return "(L) Ctrl";
      case enKey::RCtrl:        //! Right Ctrl (Control) keystroke
        return "(R) Ctrl";
      case enKey::LAlt:         //! Left Alt (Menu) keystroke
        return "(L) Alt";
      case enKey::RAlt:         //! Right Alt (Menu) keystroke
        return "(R) Alt";
      case enKey::LOS:          //! Left OS (Windows|Apple) keystroke
        return "(L) OS";
      case enKey::ROS:          //! Right OS (Windows|Apple) keystroke
        return "(R) OS";
      case enKey::Context:      //! Context (Right click menu) keystroke
        return "Menu";
      case enKey::Fn:           //! Fn (Function) keystroke
        return "Fn";

      case enKey::Right:        //! Directional Arrow Right keystroke
        return "[>]";
      case enKey::Up:           //! Directional Arrow Up keystroke
        return "[^]";
      case enKey::Left:         //! Directional Arrow Left keystroke
        return "[<]";
      case enKey::Down:         //! Directional Arrow Down keystroke
        return "[v]";
      case enKey::Home:         //! Navigation to Home (Head) keystroke
        return "Home";
      case enKey::End:          //! Navigation to End (Tail) keystroke
        return "End";
      case enKey::PageUp:       //! Page Up keystroke
        return "PgUp";
      case enKey::PageDown:     //! Page Down keystroke
        return "PgDn";

      case enKey::PrtSc:        //! PrtSc (Print Screen) keystroke
        return "PrtSc";
      case enKey::Pause:        //! Pause | Break keystroke
        return "Pause | Break";
      case enKey::Ins:          //! Ins (Insert) keystroke
        return "Insert";
      case enKey::ScrLk:        //! ScrLk (Scroll Lock) keystroke
        return "ScrLk";
      case enKey::CapsLock:     //! Caps Lock keystroke detectino state
        return "CapsLock";

      case enKey::NumLock:      //! NumLk (Number Lock) keystroke
        return "NumLock";
      case enKey::N0:           //! Numpad Numeral 0 (Ins) keystroke
        return "#0 (Ins)";
      case enKey::N1:           //! Numpad Numeral 1 (End) keystroke
        return "#1 (End)";
      case enKey::N2:           //! Numpad Numeral 2 (Down) keystroke
        return "#2 [v]";
      case enKey::N3:           //! Numpad Numeral 3 (PgDn) keystroke
        return "#3 (PgDn)";
      case enKey::N4:           //! Numpad Numeral 4 (Left) keystroke
        return "#4 [<]";
      case enKey::N5:           //! Numpad Numeral 5 (Sel) keystroke
        return "#5 (Sel)";
      case enKey::N6:           //! Numpad Numeral 6 (Right) keystroke
        return "#6 [>]";
      case enKey::N7:           //! Numpad Numeral 7 (Home) keystroke
        return "#7 (Home)";
      case enKey::N8:           //! Numpad Numeral 8 (Up) keystroke
        return "#8 [^]";
      case enKey::N9:           //! Numpad Numeral 9 (PgUp) keystroke
        return "#9 (PgUp)";
      case enKey::Decimal:      //! Numpad Decimal Point / Dot / Del keystroke
        return "#.";
      case enKey::Minus:        //! Numpad Minus / Subtract / Hyphen keystroke
        return "#-";
      case enKey::Add:          //! Numpad Add / Plus keystroke
        return "#+";
      case enKey::Multiply:     //! Numpad Multiply / Asterisk keystroke
        return "#*";
      case enKey::Divide:       //! Numpad Divide / Slash keystroke
        return "#/";
      case enKey::LineFeed:     //! Numpad Enter (Line Feed) keystroke
        return "#Enter";

      case enKey::MediaStop:        //! Multimedia Stop key
        return "Stop Media";
      case enKey::MediaPause:       //! Multimedia Play / Pause key
        return "Pause Media";
      case enKey::MediaPrev:        //! Multimedia Previous track key
        return "Prev Media";
      case enKey::MediaNext:        //! Multimedia Next track key
        return "Next Media";
      case enKey::VolumeMute:       //! Volume Mute key
        return "Mute";
      case enKey::VolumeDown:       //! Volume Down key
        return "Vol Down";
      case enKey::VolumeUp:         //! Volume Up key
        return "Vol Up";

      case enKey::F13:              //! Function row F13 keystroke
        return "F13";
      case enKey::F14:              //! Function row 2 F14 keystroke
        return "F14";
      case enKey::F15:              //! Function row 2 F15 keystroke
        return "F15";
      case enKey::F16:              //! Function row 2 F16 keystroke
        return "F16";
      case enKey::F17:              //! Function row 2 F17 keystroke
        return "F17";
      case enKey::F18:              //! Function row 2 F18 keystroke
        return "F18";
      case enKey::F19:              //! Function row 2 F19 keystroke
        return "F19";
      case enKey::F20:              //! Function row 2 F20 keystroke
        return "F20";
      case enKey::F21:              //! Function row 2 F21 keystroke
        return "F21";
      case enKey::F22:              //! Function row 2 F22 keystroke
        return "F22";
      case enKey::F23:              //! Function row 2 F23 keystroke
        return "F23";
      case enKey::F24:              //! Function row 2 F24 keystroke
        return "F24";
      case enKey::F25:              //! Function row 2 F25 keystroke
        return "F25";

      case enKey::LClick:       //! Mouse Left Button Click
        return "Mouse Left Click";
      case enKey::RClick:       //! Mouse Right Button Click
        return "Mouse Right Click";
      case enKey::MClick:       //! Mouse Middle (3 button) Click
        return "Mouse Middle Click";
      case enKey::MScrollDn:    //! Mouse Middle (Wheel) Scroll Down
        return "Scroll Down";
      case enKey::MScrollUp:    //! Mouse Middle (Wheel) Scroll Up
        return "Scroll Up";
      case enKey::X1Click:      //! Mouse X1 (5 button) Click
        return "Mouse X1 Click";
      case enKey::X2Click:      //! Mouse X2 (5 button) Click
        return "Mouse X2 Click";

      case enKey::Toggle:           //! Toggle absolute | relative coord mode
        return "Pen|Mouse Mode";
      case enKey::EraseHover:       //! Rear Stylus Tip Passive Hover
        return "Eraser Hover";
      case enKey::PenHover:         //! Pen Tip Passive Hover
        return "Pen Hover";
      case enKey::PenTouch:         //! Stylus Tip contact detection state
        return "Pen Touch";
      case enKey::Touch:            //! Finger input
        return "Finger Touch";
      case enKey::MultiTouch:       //! Two / Three Finger input
        return "MultiTouch";
      case enKey::PenSide1:         //! Stylus Side Switch low end click
        return "Side Switch In";
      case enKey::PenSide2:         //! Stylus Side Switch high end click
        return "Side Switch Out";
      case enKey::PenSideOut:       //! Touch Ring CW / Strip Down touch
        return "Side Wheel In";
      case enKey::PenSideIn:        //! Touch Ring CCW / Strip Up touch
        return "Side Wheel Out";
      case enKey::RingMode:         //! Wacom Touch Strip / Ring next mode
        return "Touch Strip Mode";
      case enKey::RingCW:           //! Touch Ring CW / Strip Down touch
        return "Touch Strip Down";
      case enKey::RingCCW:          //! Touch Ring CCW / Strip Up touch
        return "Touch Strip Up";
      case enKey::Express1:         //! Wacom Express Key 1 press
        return "Express Key 1";
      case enKey::Express2:         //! Wacom Express Key 2 press
        return "Express Key 2";
      case enKey::Express3:         //! Wacom Express Key 3 press
        return "Express Key 3";
      case enKey::Express4:         //! Wacom Express Key 4 press
        return "Express Key 4";
      case enKey::Express5:         //! Wacom Express Key 5 press
        return "Express Key 5";
      case enKey::Express6:         //! Wacom Express Key 6 press
        return "Express Key 6";
      case enKey::Express7:         //! Wacom Express Key 7 press
        return "Express Key 7";
      case enKey::Express8:         //! Wacom Express Key 8 press
        return "Express Key 8";
      
      case enKey::Pad1Joy1Left: //! JoyPad #1 Left stick horizontal tilt (-X)
        return "P1 J1-Left ( < | -X1)";
      case enKey::Pad1Joy1Up:   //! JoyPad #1 Left stick vertical tilt (+Y)
        return "P1 J1-Up ( ^ | +Y1)";
      case enKey::Pad1Joy1Right://! JoyPad #1 Left stick horizontal tilt (+X)
        return "P1 J1-Right ( > | +X1)";
      case enKey::Pad1Joy1Down: //! JoyPad #1 Left stick vertical tilt (-Y)
        return "P1 J1-Down ( v | -Y1)";
      case enKey::Pad1Joy2Left: //! JoyPad #1 Right stick horizontal tilt (-X)
        return "P1 J2-Left ( < | -X2)";
      case enKey::Pad1Joy2Up:   //! JoyPad #1 Right stick vertical tilt (+Y)
        return "P1 J2-Up ( ^ | +Y2)";
      case enKey::Pad1Joy2Right://! JoyPad #1 Right stick horizontal tilt (+X)
        return "P1 J2-Right ( > | +X2)";
      case enKey::Pad1Joy2Down: //! JoyPad #1 Right stick vertical tilt (-Y)
        return "P1 J2-Down ( v | -Y2)";
      case enKey::Pad1Left:     //! JoyPad #1 Directional Pad Left button
        return "P1 D-Pad Left ( < )";
      case enKey::Pad1Up:       //! JoyPad #1 Directional Pad Up button
        return "P1 D-Pad Up ( ^ )";
      case enKey::Pad1Right:    //! JoyPad #1 Directional Pad Right button
        return "P1 D-Pad Right ( > )";
      case enKey::Pad1Down:     //! JoyPad #1 Directional Pad Down button
        return "P1 D-Pad Down ( v )";
      case enKey::Pad1Mode:     //! JoyPad #1 Mode (Select) button
        return "P1 Mode ( - )";
      case enKey::Pad1Menu:     //! JoyPad #1 Menu (Start) button
        return "P1 Menu ( + )";
      case enKey::Pad1X:        //! JoyPad #1 Face X (Nintendo Y) button
        return "P1 X ( < )";
      case enKey::Pad1Y:        //! JoyPad #1 Face Y (Nintendo X) button
        return "P1 Y ( ^ )";
      case enKey::Pad1B:        //! JoyPad #1 Face B (Nintendo A) button
        return "P1 B ( > )";
      case enKey::Pad1A:        //! JoyPad #1 Face A (Nintendo B) button
        return "P1 A ( v )";
      case enKey::Pad1L:        //! JoyPad #1 Shoulder L(B) (Sony L1) button
        return "P1 LB ( / )";
      case enKey::Pad1R:        //! JoyPad #1 Shoulder R(B) (Sony R1) button
        return "P1 RB ( \\ )";
      case enKey::Pad1LT:       //! JoyPad #1 Shoulder LT (Sony L2) button
        return "P1 LT ( // )";
      case enKey::Pad1RT:       //! JoyPad #1 Shoulder RT (Sony R2) button
        return "P1 RT ( \\\\ )";

      case enKey::Pad2Joy1Left: //! JoyPad #1 Left stick horizontal tilt (-X)
        return "P2 J1-Left ( < | -X1)";
      case enKey::Pad2Joy1Up:   //! JoyPad #1 Left stick vertical tilt (+Y)
        return "P2 J1-Up ( ^ | +Y1)";
      case enKey::Pad2Joy1Right://! JoyPad #1 Left stick horizontal tilt (+X)
        return "P2 J1-Right ( > | +X1)";
      case enKey::Pad2Joy1Down: //! JoyPad #1 Left stick vertical tilt (-Y)
        return "P2 J1-Down ( v | -Y1)";
      case enKey::Pad2Joy2Left: //! JoyPad #1 Right stick horizontal tilt (-X)
        return "P2 J2-Left ( < | -X2)";
      case enKey::Pad2Joy2Up:   //! JoyPad #1 Right stick vertical tilt (+Y)
        return "P2 J2-Up ( ^ | +Y2)";
      case enKey::Pad2Joy2Right://! JoyPad #1 Right stick horizontal tilt (+X)
        return "P2 J2-Right ( > | +X2)";
      case enKey::Pad2Joy2Down: //! JoyPad #1 Right stick vertical tilt (-Y)
        return "P2 J2-Down ( v | -Y2)";
      case enKey::Pad2Left:     //! JoyPad #2 Directional Pad Left button
        return "P2 D-Pad Left ( < )";
      case enKey::Pad2Up:       //! JoyPad #2 Directional Pad Up button
        return "P2 D-Pad Up ( ^ )";
      case enKey::Pad2Right:    //! JoyPad #2 Directional Pad Right button
        return "P2 D-Pad Right ( > )";
      case enKey::Pad2Down:     //! JoyPad #2 Directional Pad Down button
        return "P2 D-Pad Down ( v )";
      case enKey::Pad2Mode:     //! JoyPad #2 Mode (Select) button
        return "P2 Mode ( - )";
      case enKey::Pad2Menu:     //! JoyPad #2 Menu (Start) button
        return "P2 Menu ( + )";
      case enKey::Pad2X:        //! JoyPad #2 Face X (Nintendo Y) button
        return "P2 X ( < )";
      case enKey::Pad2Y:        //! JoyPad #2 Face Y (Nintendo X) button
        return "P2 Y ( ^ )";
      case enKey::Pad2B:        //! JoyPad #2 Face B (Nintendo A) button
        return "P2 B ( > )";
      case enKey::Pad2A:        //! JoyPad #2 Face A (Nintendo B) button
        return "P2 A ( v )";
      case enKey::Pad2L:        //! JoyPad #2 Shoulder L (Sony L1) button
        return "P2 LB ( / )";
      case enKey::Pad2R:        //! JoyPad #2 Shoulder R (Sony R1) button
        return "P2 RB ( \\ )";
      case enKey::Pad2LT:       //! JoyPad #2 Shoulder LT (Sony L2) button
        return "P2 LT ( // )";
      case enKey::Pad2RT:       //! JoyPad #2 Shoulder RT (Sony R2) button
        return "P2 RT ( \\\\ )";

      case enKey::Pad3Joy1Left: //! JoyPad #1 Left stick horizontal tilt (-X)
        return "P3 J1-Left ( < | -X1)";
      case enKey::Pad3Joy1Up:   //! JoyPad #1 Left stick vertical tilt (+Y)
        return "P3 J1-Up ( ^ | +Y1)";
      case enKey::Pad3Joy1Right://! JoyPad #1 Left stick horizontal tilt (+X)
        return "P3 J1-Right ( > | +X1)";
      case enKey::Pad3Joy1Down: //! JoyPad #1 Left stick vertical tilt (-Y)
        return "P3 J1-Down ( v | -Y1)";
      case enKey::Pad3Joy2Left: //! JoyPad #1 Right stick horizontal tilt (-X)
        return "P3 J2-Left ( < | -X2)";
      case enKey::Pad3Joy2Up:   //! JoyPad #1 Right stick vertical tilt (+Y)
        return "P3 J2-Up ( ^ | +Y2)";
      case enKey::Pad3Joy2Right://! JoyPad #1 Right stick horizontal tilt (+X)
        return "P3 J2-Right ( > | +X2)";
      case enKey::Pad3Joy2Down: //! JoyPad #1 Right stick vertical tilt (-Y)
        return "P3 J2-Down ( v | -Y2)";
      case enKey::Pad3Left:     //! JoyPad #3 Directional Pad Left button
        return "P3 D-Pad Left ( < )";
      case enKey::Pad3Up:       //! JoyPad #3 Directional Pad Up button
        return "P3 D-Pad Up ( ^ )";
      case enKey::Pad3Right:    //! JoyPad #3 Directional Pad Right button
        return "P3 D-Pad Right ( > )";
      case enKey::Pad3Down:     //! JoyPad #3 Directional Pad Down button
        return "P3 D-Pad Down ( v )";
      case enKey::Pad3Mode:     //! JoyPad #3 Mode (Select) button
        return "P3 Mode ( - )";
      case enKey::Pad3Menu:     //! JoyPad #3 Menu (Start) button
        return "P3 Menu ( + )";
      case enKey::Pad3X:        //! JoyPad #3 Face X (Nintendo Y) button
        return "P3 X ( < )";
      case enKey::Pad3Y:        //! JoyPad #3 Face Y (Nintendo X) button
        return "P3 Y ( ^ )";
      case enKey::Pad3B:        //! JoyPad #3 Face B (Nintendo A) button
        return "P3 B ( > )";
      case enKey::Pad3A:        //! JoyPad #3 Face A (Nintendo B) button
        return "P3 A ( v )";
      case enKey::Pad3L:        //! JoyPad #3 Shoulder L (Sony L1) button
        return "P3 LB ( / )";
      case enKey::Pad3R:        //! JoyPad #3 Shoulder R (Sony R1) button
        return "P3 RB ( \\ )";
      case enKey::Pad3LT:       //! JoyPad #3 Shoulder LT (Sony L2) button
        return "P3 LT ( // )";
      case enKey::Pad3RT:       //! JoyPad #3 Shoulder RT (Sony R2) button
        return "P3 RT ( \\\\ )";

      case enKey::Pad4Joy1Left: //! JoyPad #1 Left stick horizontal tilt (-X)
        return "P4 J1-Left ( < | -X1)";
      case enKey::Pad4Joy1Up:   //! JoyPad #1 Left stick vertical tilt (+Y)
        return "P4 J1-Up ( ^ | +Y1)";
      case enKey::Pad4Joy1Right://! JoyPad #1 Left stick horizontal tilt (+X)
        return "P4 J1-Right ( > | +X1)";
      case enKey::Pad4Joy1Down: //! JoyPad #1 Left stick vertical tilt (-Y)
        return "P4 J1-Down ( v | -Y1)";
      case enKey::Pad4Joy2Left: //! JoyPad #1 Right stick horizontal tilt (-X)
        return "P4 J2-Left ( < | -X2)";
      case enKey::Pad4Joy2Up:   //! JoyPad #1 Right stick vertical tilt (+Y)
        return "P4 J2-Up ( ^ | +Y2)";
      case enKey::Pad4Joy2Right://! JoyPad #1 Right stick horizontal tilt (+X)
        return "P4 J2-Right ( > | +X2)";
      case enKey::Pad4Joy2Down: //! JoyPad #1 Right stick vertical tilt (-Y)
        return "P4 J2-Down ( v | -Y2)";
      case enKey::Pad4Left:     //! JoyPad #4 Directional Pad Left button
        return "P4 D-Pad Left ( < )";
      case enKey::Pad4Up:       //! JoyPad #4 Directional Pad Up button
        return "P4 D-Pad Up ( ^ )";
      case enKey::Pad4Right:    //! JoyPad #4 Directional Pad Right button
        return "P4 D-Pad Right ( > )";
      case enKey::Pad4Down:     //! JoyPad #4 Directional Pad Down button
        return "P4 D-Pad Down ( v )";
      case enKey::Pad4Mode:     //! JoyPad #4 Mode (Select) button
        return "P4 Mode ( - )";
      case enKey::Pad4Menu:     //! JoyPad #4 Menu (Start) button
        return "P4 Menu ( + )";
      case enKey::Pad4X:        //! JoyPad #4 Face X (Nintendo Y) button
        return "P4 X ( < )";
      case enKey::Pad4Y:        //! JoyPad #4 Face Y (Nintendo X) button
        return "P4 Y ( ^ )";
     case enKey::Pad4B:        //! JoyPad #4 Face B (Nintendo A) button
        return "P4 B ( > )";
      case enKey::Pad4A:        //! JoyPad #4 Face A (Nintendo B) button
        return "P4 A ( v )";
      case enKey::Pad4L:        //! JoyPad #4 Shoulder L (Sony L1) button
        return "P4 LB ( / )";
      case enKey::Pad4R:        //! JoyPad #4 Shoulder R (Sony R1) button
        return "P4 RB ( \\ )";
      case enKey::Pad4LT:       //! JoyPad #4 Shoulder LT (Sony L2) button
        return "P4 LT ( // )";
      case enKey::Pad4RT:       //! JoyPad #4 Shoulder RT (Sony R2) button
        return "P4 RT ( \\\\ )";

      case enKey::Total:        //! Ttotal number of keys listed
        return "Total (" + std::to_string(AllKeys) + ")";
      case enKey::Invalid: default: //! Reserved key match error state value
        return "Undefined";

      } // end switch (input) key

    } // end std::string Input::Label(enKey)                                  */


    /** Get key from a defined enum value for all defined keys
    \brief
      - get key from a defined enum value for all defined keys
    \details
      - not great to hardcode and assume localization is using 108 qwerty
      compatible key configuration (specific key layout aside).  refactor
      should be inevitable to enumerate key listings and abstract enum in use
      to support a more universal key list, or even a localized input mode
      that allows other total key amounts
    */
    inline Key& On(enKey val) { return key[static_cast<um>(val)]; }


    /** Get key from a defined enum value for all defined keys
    \brief
      - get key from a defined enum value for all defined keys
    \details
      - not great to hardcode and assume localization is using 108 qwerty
      compatible key configuration (specific key layout aside).  refactor
      should be inevitable to enumerate key listings and abstract enum in use
      to support a more universal key list, or even a localized input mode
      that allows other total key amounts
    */
    inline Key& operator[](enKey val) { return key[static_cast<um>(val)]; }


  protected:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Protected Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Pointer to drive the parent window / conext which this Input controls 
    Window* win = nullptr;

    //! Tracker for package independent clarification of running state
    bool done = true;

    //! List of all keyboard keys present state
    Key key[AllKeys];
    // TODO: refactor to abstract localization away from US EN semantics?
    
    //! 
    bool firstMouse = true;
    f4 lastX = 0.0f;
    f4 lastY = 0.0f;

  }; // end Input class interface

} // end dp namespace
