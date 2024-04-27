/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  TypeErrata.cpp
Purpose:  Primitive byte-width aliases to simplify hardware / platform ports
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_CS350_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Summer 2022
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#ifndef TYPE_PRIMITIVE_ERRATA_H
#define TYPE_PRIMITIVE_ERRATA_H
#include <assert.h>

static_assert(sizeof(unsigned char) == 1, "sizeof(unsigned char) != 1");
typedef unsigned char u1;       //! Unsigned 1-byte; 8-bit datum
static_assert(sizeof(unsigned short) == 2, "sizeof(unsigned short) != 2");
typedef unsigned short u2;      //! Unsigned 2-byte; 16-bit datum
static_assert(sizeof(unsigned int) == 4, "sizeof(unsigned int) != 4");
typedef unsigned int u4;        //! Unsigned 4-byte; 32-bit datum
static_assert(sizeof(unsigned long long)==8,"sizeof(unsigned long long) != 8");
typedef unsigned long long u8;  //! Unsigned 8-byte; 64-bit datum
typedef size_t um; //! Unsigned OS bit-depth (x86:4 | x64:8 bytes) driven datum
static_assert(sizeof(signed char) == 1, "sizeof(signed char) != 1");
typedef signed char s1;         //! Signed 1-byte; 8-bit datum
static_assert(sizeof(signed short) == 2, "sizeof(signed short) != 2");
typedef signed short s2;        //! Signed 2-byte; 16-bit datum
static_assert(sizeof(signed int) == 4, "sizeof(signed int) != 4");
typedef signed int s4;          //! Signed 4-byte; 32-bit datum
static_assert(sizeof(signed long long) == 8, "sizeof(signed long long) != 8");
typedef signed long long s8;    //! Signed 8-byte; 64-bit datum
static_assert(sizeof(float) == 4, "sizeof(float) != 4");
typedef float f4;               //! Floating-point 4-byte; 32-bit datum
static_assert(sizeof(double) == 8, "sizeof(double) != 8");
typedef double f8;              //! Floating-point 8-byte; 64-bit datum

//! Number of bits per byte: 8 (by definition)
constexpr u1 BYTEBITS = 8u;

//! Highest possible unsigned 8-bit value: 0xFF, ie 255
constexpr u1 u1_MAX = (1u << (sizeof(u1) * BYTEBITS)) - 1;

//! Highest possible unsigned 16-bit value: 0xFFFF, ie 65535
constexpr u2 u2_MAX = (1u << (sizeof(u2) * BYTEBITS)) - 1;

//! Highest possible unsigned 32-bit value: 0xFFFFFFFF, ie 4,294,967,295
constexpr u4 u4_MAX = (1ull << (sizeof(u4) * BYTEBITS)) - 1;

//! Highest possible unsigned 64-bit value: 18,446,744,073,709,551,615
constexpr u8 u8_MAX = u8(0xFFFFFFFFFFFFFFFFUL);

//! Highest possible subscript value: u2|4|8_MAX depending on platform
constexpr um um_MAX = (sizeof(um)==2? u2_MAX : sizeof(um)==4? u4_MAX : u8_MAX);

//! Lowest possible signed 8-bit value: 0x80, ie -128
constexpr s1 s1_MIN = (1ull << (sizeof(s1) * BYTEBITS - 1));

//! Highest possible signed 8-bit value: 0x7F, ie 127
constexpr s1 s1_MAX = (1ull << (sizeof(s1) * BYTEBITS - 1)) - 1;

//! Lowest possible signed 16-bit value: 0x8000, ie -32768
constexpr s2 s2_MIN = (1ull << (sizeof(s2) * BYTEBITS - 1));

//! Highest possible signed 16-bit value: 0x7FFF, ie 32767
constexpr s2 s2_MAX = (1ull << (sizeof(s2) * BYTEBITS - 1)) - 1;

//! Lowest possible signed 32-bit value: 0x80000000, ie -2,147,483,648
constexpr s4 s4_MIN = (1ull << (sizeof(s4) * BYTEBITS - 1));

//! Highest possible signed 32-bit value: 0x7FFFFFFF, ie 2,147,483,647
constexpr s4 s4_MAX = (1ull << (sizeof(s4) * BYTEBITS - 1)) - 1;

//! Lowest possible signed 64-bit value: ie -9,223,372,036,854,775,808
constexpr s8 s8_MIN = s8(0x8000000000000000ll);

//! Highest possible signed 32-bit value: ie 9,223,372,036,854,775,807
constexpr s8 s8_MAX = s8(0x7FFFFFFFFFFFFFFFll);

//! u1_MAX =: sentinel value within the framework for u1 data as a bad result
constexpr u1 u1_INVALID = u1_MAX;

//! u2_MAX =: sentinel value within the framework for u2 data as a bad result
constexpr u2 u2_INVALID = u2_MAX;

//! u4_MAX =: sentinel value within the framework for u4 data as a bad result
constexpr u4 u4_INVALID = u4_MAX;

//! um_MAX =: sentinel value within the framework for um data as a bad result
constexpr um um_INVALID = (sizeof(um) == 2 ? u2_MAX : sizeof(um) == 4 ? u4_MAX
  : u8_MAX);

//! u8_MAX =: sentinel value  within the frameworkfor u8 data as a bad result
constexpr u8 u8_INVALID = u8_MAX;
// need something, so u#_MAX since comparable to s#_MIN
// static_assert(((u4)s4_MAX)+s4_MIN == u4_MAX, "like so");
// sN MSB is just -(2^{8*N-1}) place to uN MSB being +(2^{8*N-1})

//! Designated sentinel value within the framework for s1 data as a bad result
constexpr s1 s1_INVALID = s1_MIN;

//! Designated sentinel value within the framework for s2 data as a bad result
constexpr s2 s2_INVALID = s2_MIN;

//! Designated sentinel value within the framework for s4 data as a bad result
constexpr s4 s4_INVALID = s4_MIN;

//! Designated sentinel value within the framework for s8 data as a bad result
constexpr s8 s8_INVALID = s8_MIN;
// need something, and s#_MIN makes symmetrical valid ranges [-s#_MAX,s#_MAX]

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENV_64
#else
#define ENV_32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENV_64
#else
#define ENV_32
#endif
#endif


#endif // end TYPE_PRIMITIVE_ERRATA_H
