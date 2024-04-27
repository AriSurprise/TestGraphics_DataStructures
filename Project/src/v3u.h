/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v3u.h
Purpose:  API agnostic, multipurpose, 3 dimensional vector of element subscripts
Details:  useful for containing any triplet of unsigned values too if small
  memory footprint is less of a concern, but array offsets are the most likely
  default usage of unsigned values.  For simplicity of footprint uniformity
  across platforms, 32-bit cap still being used, but nothing should prevent
  changing that setting if desired.
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
#include <string>       // String building methods for text value output
// "./src/..."
#include "ang.h"        // angle extraction requires arc unit parameter
#include "ASCIIValue.h" // character input validation of subscript operator
#include "Value.h"      // Value manipulation; float point approximation
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  union v3u //! 3D vector containing 3 unsigned, 4-byte elements
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    u4 vec[3];
    struct {
      u4 a; //! first / primary subscript value in v3u's subscript [0] field
      u4 b; //! second / secondary subscript value in v3u's subscript [1] field
      u4 c; //! third / tertiary subscript value in v3u's subscript [2] field
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Create a 3 dimensional value set / subscript triplet (default <0,0,0>)
    \brief
      - create a 3 dimensional value set / subscript triplet (default <0,0,0>)
    \param a_value
      - (x | a field value) - vector primary field, subscript [0] value
    \param b_value
      - (y | b field value) - vector secondary field, subscript [1] value
    \param c_value
      - (z | c field value) - vector tertiary field, subscript [2] value
    */
    v3u(u4 a_value = 0, u4 b_value = 0, u4 c_value = 0);


    /** Creates a copy of a v3f from another source v3f
    \brief
      - creates a copy of a v3f from another source v3f
    \param source
      - the v3f to used for memberwise <x|r, y|g, z|b> element value copying
    */
    v3u(const v3u& source);


    /** Creates a v3f from a v3f value reference resultant from a function
    \brief
      - creates a v3f from a v3f value reference resultant from a function
    \param result
      - the v3f value reference to surrogate into this
    */
    v3u(v3u&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Get the current primary subscript field value (subscript [0])
    \brief
      - get the current primary subscript field value (subscript [0])
    \return
      - reference to the contained subscript value in v3u's primary subscript
    */
    inline const u4& A(void) const
    { return a; } // end const f4& A(void) const                              */


    /** Set the current primary subscript field value (subscript [0])
    \brief
      - set the current primary subscript field value (subscript [0])
    \param value
      - unsigned value to set in 1st / subscript [0] / a field of the vector
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& A(u4 value)
    { a = value; return *this; } // end v3u& A(u4)                            */


    /** Set the current primary subscript field value (subscript [0])
    \brief
      - set the current primary subscript field value (subscript [0])
    \param value
      - unsigned value to set in 1st / subscript [0] / a field of the vector
    \param max
      - can clamp value into desired maximum value range
    \param min
      - can clamp value into desired minimum value range (default 0)
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& A(u4 value, u4 max, u4 min = 0)
    { a = InU(value, max, min); return *this; } // end v3u& A(u4, u4, u4)  */


    /** Get the current secondary subscript field value (subscript [1])
    \brief
      - get the current secondary subscript field value (subscript [1])
    \return
      - reference to the contained subscript value in v3u's secondary subscript
    */
    inline const u4& B(void) const
    { return b; } // end const f4& B(void) const                              */


    /** Set the current secondary subscript field value (subscript [1])
    \brief
      - set the current secondary subscript field value (subscript [1])
    \param value
      - unsigned value to set in 2nd / subscript [1] / b field of the vector
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& B(u4 value)
    { b = value; return *this; } // end v3u& B(u4)                            */


    /** Set the current secondary subscript field value (subscript [1])
    \brief
      - set the current secondary subscript field value (subscript [1])
    \param value
      - unsigned value to set in 2nd / subscript [1] / b field of the vector
    \param max
      - can clamp value into desired maximum value range
    \param min
      - can clamp value into desired minimum value range (default 0)
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& B(u4 value, u4 max, u4 min = 0)
    { b = InU(value, max, min); return *this; } // end v3u& B(u4, u4, u4)  */


    /** Get the current tertiary subscript field value (subscript [2])
    \brief
      - get the current tertiary subscript field value (subscript [2])
    \return
      - reference to the contained subscript value in v3u's tertiary subscript
    */
    inline const u4& C(void) const
    { return c; } // end const f4& C(void) const                              */


    /** Set the current tertiary subscript field value (subscript [2])
    \brief
      - set the current tertiary subscript field value (subscript [2])
    \param value
      - unsigned value to set in 3rd / [2] / c field of the vector
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& C(u4 value)
    { c = value; return *this; } // end v3u& C(u4)                            */


    /** Create a v3u with a copy of this v3u's data in each member field value
    \brief
      - create a v3u with a copy of this v3u's data in each member field value
    \return
      - new v3u containing this v3u's data, per field
    */
    inline v3u Copy(void) const { return v3u(*this); } // end v3u Copy(void)  */


    /** Set the current tertiary subscript field value (subscript [2])
    \brief
      - set the current tertiary subscript field value (subscript [2])
    \param value
      - unsigned value to set in 3rd / [2] / c field of the vector
    \param max
      - can clamp value into desired maximum value range
    \param min
      - can clamp value into desired minimum value range (default 0)
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& C(u4 value, u4 max, u4 min = 0)
    { c = InU(value, max, min); return *this; } // end v3u& C(u4, u4, u4)  */


    /** Format a v3f for being output as a packed vector notated string
    \brief
      - format a v3f for being output as a packed vector notated string
    \return
      - text formatted to [x,y,z] standards for later JSON interpretation
    */
    inline std::string JSON(void) const
    {
      return "<" + std::to_string(a) + "," + std::to_string(b) + "," +
        std::to_string(c) + ">";
    } // end std::string JSON(void) const                                     */


    /** Set the v3u to the given <a, b, c> input subscript values
    \brief
      - set the v3u to the given <a, b, c> input subscript values
    \param a_value
      - (x | a  field value) - first subscript
    \param b_value
      - (y | b field value) - second subscript
    \param c_value
      - (z | c field value) - third subscript
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& Set(u4 a_value, u4 b_value, u4 c_value)
    {
      return A(a_value).B(b_value).C(c_value);

    } // end v3u& Set(u4, u4, u4)                                             */


    /** Set the v3u to the given <a, b, c> input subscript values
    \brief
      - set the v3u to the given <a, b, c> input subscript values
    \param a_value
      - (x | a  field value) - first subscript
    \param b_value
      - (y | b field value) - second subscript
    \param c_value
      - (z | c field value) - third subscript
    \param max
      - can clamp value into desired maximum value range
    \param min
      - can clamp value into desired minimum value range (default 0)
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& Set(u4 a_value, u4 b_value, u4 c_value, u4 max, u4 min = 0)
    {
      return A(a_value, max, min).B(b_value, max, min).C(c_value, max, min);

    } // end v3u& Set(u4, u4, u4, u4, u4)                                     */


    /** Set the v3u to the given <source.a, source.b, source.c> values
    \brief
      - set the v3u to the given <source.a, source.b, source.c> values
    \param source
      - the v3u for memberwise copying of <a, b, c> input subscript values
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& Set(const v3u& source)
    {
      return A(source.a).B(source.b).C(source.c);

    } // end v3u& Set(const v3u&)                                             */


    /** Set the v3u to the given <source.a, source.b, source.c> values
    \brief
      - set the v3u to the given <source.a, source.b, source.c> values
    \param src
      - the v3u for memberwise copying of <a, b, c> input subscript values
    \param max
      - can clamp value into desired maximum value range
    \param min
      - can clamp value into desired minimum value range (default 0)
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& Set(const v3u& src, u4 max, u4 min = 0)
    {
      return A(src.a,max,min).B(src.b,max,min).C(src.c,max,min);

    } // end v3u& Set(const v3u&, u4, u4)                                     */


    /** Set the v3u to use <result.a, result.b, result.c> values (directly)
    \brief
      - set the v3u to use <result.a, result.b, result.c> values (directly)
    \param result
      - the v3u value reference from which to have values surrogated
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& Set(v3u&& result) noexcept
    {
      *vec = std::move(*result.vec);
      return *this;
    } // end v3u& Set(v3u&& result) noexcept                                  */


    /** Format a v3f for being output as a vector notated string
    \brief
      - format a v3f for being output as a vector notated string
    \details
      - adds padding for uniform spacing: JSON tightly packs CSV elements
    \return
      - text set to < x, y, z > with leading space for negative / positive sign
    */
    inline std::string String(void) const
    {
      return "<" + std::to_string(a) + ", " + std::to_string(b) + ", " +
        std::to_string(c) + ">";
    } // end std::string String(void) const                                   */

    /** 
    \brief
      - get the subset of data from the given list which subs a
    */
    template <class T>
    inline std::vector<T> Subset(const std::vector<T>& list, const v3u& subs)
    {
      std::vector<T> result(3);
      for (u4 i = 0; i < 3; ++i) { result[i] = list[subs[i]]; }
      return result;

    } // end std::vector<T> Subset(const std::vector<T>&, const v3u&)         */


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Sets the source v3u member values into this v3u value fields
    \brief
      - sets the source v3u member values into this v3u value fields
    \param source
      - the v3u which will have its values copied memberwise into this v3u
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& operator=(const v3u& source)
    { Set(source); } // end v3u& operator=(const v3u&)                        */


    /** Surrogate a v3u value reference returned from a function for this v3u
    \brief
      - surrogate a v3u value reference returned from a function for this v3u
    \param result
      - the v3u reference resultant from a function from which to steal values
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    v3u& operator=(v3u&& result) noexcept
    { Set(result); } // end v3u& operator=(v3u&&) noexcept              */


    /** Get a value from ['x|a'], ['y|b'] or ['z|c'] channels (or equivalent)
    \brief
      - get a value from ['x|a'], ['y|b'] or ['z|c'] channels (or equivalent)
    \details
      (0 | x | X | a | A) -> [0]
      (1 | y | Y | b | B) -> [1]
      (2 | z | Z | c | C) -> [2]
    \param channel
      - use (0|'x'|'a')->vec[0], (1|'y'|'b')->vec[1], (2|'z'|'c')->vec[2]
    \return
      - copy of the contained dimension member value associated to the channel
    */
    inline const u4& operator[](s1 channel) const
    {
      // clamp wildly out of range values into lowest 4 unsigned values
      if (!isascii(channel) || (channel<0x20 && channel>0xF)) { channel &= 3; }
      else if (IsDigit(channel)) { channel = ToNumeral(channel) & 3; }
      switch (channel) // Lookup array index equivalent for return
      {
      case 'z': case 'c': case 'Z': case 'C': case 2:
        return vec[2];
      case 'y': case 'b': case 'Y': case 'B': case 1:
        return vec[1];
      case 'x': case 'a': case 'X': case 'A': case 3: case 0: default:
        return vec[0];
      }
    } // end const u4& v3u::operator[](s1) const


    /** Get a value from ['x|a'], ['y|b'] or ['z|c'] channels (or equivalent)
    \brief
      - get a value from ['x|a'], ['y|b'] or ['z|c'] channels (or equivalent)
    \details
      (0 | x | X | a | A) -> [0]
      (1 | y | Y | b | B) -> [1]
      (2 | z | Z | c | C) -> [2]
    \param channel
      - use (0|'x'|'a')->vec[0], (1|'y'|'b')->vec[1], (2|'z'|'c')->vec[2]
    \return
      - copy of the contained dimension member value associated to the channel
    */
    inline u4& operator[](s1 channel)
    {
      // clamp wildly out of range values into lowest 4 unsigned values
      if (!isascii(channel) || (channel < 0x20 && channel>0xF)) { channel &= 3; }
      else if (IsDigit(channel)) { channel = ToNumeral(channel) & 3; }
      switch (channel) // Lookup array index equivalent for return
      {
      case 'z': case 'c': case 'Z': case 'C': case 2:
        return vec[2];
      case 'y': case 'b': case 'Y': case 'B': case 1:
        return vec[1];
      case 'x': case 'a': case 'X': case 'A': case 3: case 0: default:
        return vec[0];
      }
    } // end u4& v3u::operator[](s1)


    /** Calculate the member-wise addition of 2 v3u's and return the result
    \brief
      - calculate the member-wise addition of 2 v3u's and return the result
    \param other
      - source vector to have memberwise summation performed with a copy of this
    \return
      - copy of the memberwise sum of this v3f and the other into a new v3u
    */
    inline v3u operator+(const v3u& other) const
    { return Copy() += other; } // end v3u operator+(const v3u&) const        */


    /** Add an other v3u's values memberwise into this v3u's values
    \brief
      - add an other v3u's values memberwise into this v3u's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& operator+=(const v3u& other)
    {
      for (u4 i = 0; i < 3; ++i) { vec[i] += other.vec[i]; } return *this;

    } // end v3u& operator+=(const v3u&)                                      */


    /** Get the member-wise difference (Clamped to 0 minimums) of 2 v3u's
    \brief
      - get the member-wise difference (Clamped to 0 minimums) of 2 v3u's
    \param other
      - source vector to have memberwise difference calculated with this (copy)
    \return
      - copy of the memberwise difference of this and the other, in a new v3u
    */
    inline v3u operator-(const v3u& other) const
    { return Copy() -= other; } // end v3u operator-(const v3u&) const        */


    /** Subtract an other v3u's values memberwise from this v3u (Clamp to 0)
    \brief
      - subtract an other v3u's values memberwise from this v3u (Clamp to 0)
    \param other
      - source vector to have negated memberwise summation performed with this
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& operator-=(const v3u& other)
    {
      a = InU(a - other.a, a, 0); b = InU(b - other.b, b, 0);
      c = InU(c - other.c, c, 0); return *this;

    } // end v3u& operator-=(const v3u&)                                      */


    /** Scalar product multiplies each field uniformly by a scalar constant
    \brief
      - scalar product multiplies each field uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply a, b and c values
    \return
      - resultant, member-wise scaled copy of this v3u's values
    */
    inline v3u operator*(u4 scalar) const
    { return Copy() *= scalar; } // end v3u operator*(u4) const               */


    /** Scalar product sets all members to grow uniformly by a scalar constant
    \brief
      - scalar product sets all members to grow uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply a, b and c values
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& operator*=(u4 scalar)
    {
      a *= scalar; b *= scalar; c *= scalar; return *this;

    } // end v3u& operator*=(u4)                                              */


    /** Get memberwise product of this multiplied with equivalent other values
    \brief
      - get memberwise product of this multiplied with equivalent other values
    \details
      - memberwise multiplication more granular & useful for integer math than
      dot product might be conventionally: Sum() this or Dot() if sum is wanted
    \param other
      - external source vector to be multiplied memberwise with this vector
    \return
      - memberwise product of 2 vectors: < a.a * b.a, a.b * b.b, a.c * b.c >
    */
    inline v3u operator*(const v3u& other) const
    { return Copy() *= other; } // end v3u operator*(const v3u&) const        */


    /** Multiply equivalent other fields with this (memberwise multiply)
    \brief
      - multiply equivalent other fields with this (memberwise multiply)
    \param other
      - external source vector to be multiplied memberwise with this vector
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& operator*=(const v3u& other)
    {
      a *= other.a; b *= other.b; c *= other.c; return *this;

    } // end v3u& operator*=(const v3u&)                                      */


    /** Perform uniform division by divisor, returning the result
    \brief
      - perform uniform division by divisor, returning the result
    \details
      - no remainders consideredin integer division: members should be at % = 0
    \param divisor
      - amount by which <a,b,c> values are uniformly divided
    \return
      - resultant, member-wise scaled copy of this v3u's values
    */
    inline v3u operator/(u4 divisor) const
    {
      return Copy() /= divisor;
    } // end v3u operator/(f4) const                                          */


    /** Divides all members uniformly by divisor, setting the result
    \brief
      - divides all members uniformly by divisor, setting the result
    \details
      - no remainders consideredin integer division: members should be at % = 0
    \param divisor
      - amount by which <a,b,c> values are to be uniformly divided
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v3u& operator/=(u4 divisor)
    {
      a /= divisor; b /= divisor; c /= divisor; return *this;

    } // end v3u& operator/=(u4)                                              */


  }; // end v3f union / struct / class

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Generate a distance / displacement vector between two coordinate vectors
  \brief
    - generate a distance / displacement vector between two coordinate vectors
  \param initial
    - starting point (assumed), being measured for relative dimensional offset
  \param terminal
    - ending point (assumed), being measured for relative dimensional offset
  \return
    - displacement from initial to terminal (ie memberwise terminal - initial)
  */
  inline v3u Distance(const v3u& initial, const v3u& terminal)
  { return terminal - initial; } // end v3u Distance(const v3u&, const v3u&)  */


  /** Left hand scalar multiplication scales all members of rhs (commutative)
  \brief
    - left hand scalar multiplication scales all members of rhs (commutative)
  \param scalar
    - scalar amount by which to multiply x, y and z values of right-hand side
  \param rhs
    - right-hand side argument to be copied and scaled in x, y and z by scalar
  \return
    - copy of rhs v3f's values, scaled by scalar in <x,y,z>
  */
  inline v3u operator*(u4 scalar, const v3u& rhs)
  { return rhs * scalar; } // end v3u operator*(f4, const v3u&)               */


   /** Confirm if 2 v3u's are equal in all of their a, b and c field values
   \brief
     - confirm if 2 v3u's are equal in all of their a, b and c field values
   \param lhs
     - left hand side to be compared memberwise with right hand for equality
   \param rhs
     - right hand side to be compared memberwise with left hand for equality
   \return
     - true only if all members are equal to one another in equivalent fields
   */
   inline bool operator==(const v3u& lhs, const v3u& rhs)
   {
     return (lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c);

   } // end bool operator==(const v3u&, const v3u&)                           */


   /** Confirm if 2 v3u's are not equal in any of their a, b or c field values
   \brief
     - confirm if 2 v3u's are not equal in any of their a, b or c field values
   \param lhs
     - left hand side to be compared memberwise with right hand for inequality
   \param rhs
     - right hand side to be compared memberwise with left hand for inequality
   \return
     - true if any members are not equal to one another in equivalent fields
   */
   inline bool operator!=(const v3u& lhs, const v3u& rhs)
   {
     return (lhs.a != rhs.a || lhs.b != rhs.b || lhs.c != rhs.c);

   } // end bool operator!=(const v3u&, const v3u&)                           */


   /** Output the string version of the vector element data
   \brief
     - output the string version of the vector element data
   \param output
     - stream to receive the vector string and and carry contents for output
   \param vector
     - vector to have the member values be converted to text based output
   \return
     - reference to the output stream object having the vector fed in
   */
   inline std::ostream& operator<<(std::ostream& output, const v3u& vector)
   {
     return output << vector.String();

   } // end std::ostream& operator<<(std::ostream&, const v3u&)               */


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   //! Zero vector in 3 dimensions; error state reserved value
   extern const v3u& nullv3u;

   static_assert(sizeof(v3u) == 3*4, "v3u size not packed");

} // end dp namespace
