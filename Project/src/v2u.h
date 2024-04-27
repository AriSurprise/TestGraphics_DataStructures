/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v2u.h
Purpose:  API agnostic, multipurpose, 2 dimensional vector of unsigned elements
Details:  useful for any unsigned value pair; generally for [min, max] ranges
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
#include "ASCIIValue.h" // character input validation of subscript operator
#include "Value.h"      // Primitive basic manipulator functions; clamping
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  union v2u //! 2D vector containing 2 unsigned, 4-byte elements
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    u4 vec[2];
    struct {
      u4 x; //! first / primary unsigned value in v2u's subscript [0] field
      u4 y; //! second / secondary unsigned value in v2u's subscript [1] field
    };
    struct {
      u4 a; //! first / primary unsigned value in v2u's subscript [0] field
      u4 b; //! second / secondary unsigned value in v2u's subscript [1] field
    };
    struct {
      u4 one; //! first / primary unsigned value in v2u's [0] subscript
      u4 two; //! second / secondary unsigned value in v2u's [1] subscript
    };
    struct {
      u4 un; //! first / primary unsigned value in v2u's [0] subscript
      u4 bi; //! second / secondary unsigned value in v2u's [1] subscript
    };
    struct {
      u4 first;  //! first tuple value in v2u's [0] subscript
      u4 second; //! second tuple value in v2u's [1] subscript
    };
    struct {
      u4 i; //! Input unsigned field in v2u [0] subscript
      u4 o; //! Output unsigned field in v2u [1] subscript (NOT ONE SHORTHAND)
    };
    struct {
      u4 in;  //! Input unsigned field in v2u's [0] subscript
      u4 out; //! Output unsigned field in v2u's [1] subscript
    };
    struct {
      u4 min; //! Minimal unsigned value in v2u's [0] subscript
      u4 max; //! Maximal unsigned value in v2u's [1] subscript
    };
    struct {
      u4 row; //! 2D horizontal element count in v2u's [0] subscript
      u4 col; //! 2D vertical element count in v2u's [1] subscript
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Create a 2 dimensional value set / unsigned pair (default <0,0>)
    \brief
      - create a 2 dimensional value set / unsigned pair (default <0,0>)
    \param a_value
      - (x | a field value) - vector primary field, subscript [0] value
    \param b_value
      - (y | b field value) - vector secondary field, subscript [1] value
    */
    v2u(u4 a_value = 0, u4 b_value = 0);


    /** Creates a copy of a v2u from another source v2u
    \brief
      - creates a copy of a v2u from another source v2u
    \param source
      - the v2u to used for memberwise <x|a, y|b> element value copying
    */
    v2u(const v2u& source);


    /** Creates a v2u from a v2u value reference resultant from a function
    \brief
      - creates a v2u from a v2u value reference resultant from a function
    \param result
      - the v2u value reference to surrogate into this
    */
    v2u(v2u&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Get the current primary subscript field value (subscript [0])
    \brief
      - get the current primary subscript field value (subscript [0])
    \return
      - reference to the contained subscript value in v2u's primary subscript
    */
    inline const u4& A(void) const
    { return a; } // end const u4& v2u::A(void) const                         */


    /** Set the current primary subscript field value (subscript [0])
    \brief
      - set the current primary subscript field value (subscript [0])
    \param value
      - unsigned value to set in 1st / subscript [0] / a field of the vector
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& A(u4 value)
    { a = value; return *this; } // end v2u& v2u::A(u4)                       */


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
    inline v2u& A(u4 value, u4 max, u4 min = 0)
    { a = InU(value, max, min); return *this; } // end v2u& v2u::A(u4,u4,u4)  */


    /** Get the current primary subscript field value (subscript [0])
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
    inline u4 Area(void) const
    { return row * col; } // end v2u& v2u::A(u4,u4,u4)  */


    /** Get the current secondary subscript field value (subscript [1])
    \brief
      - get the current secondary subscript field value (subscript [1])
    \return
      - reference to the contained subscript value in v2u's secondary subscript
    */
    inline const u4& B(void) const
    { return b; } // end const u4& v2u::B(void) const                         */


    /** Set the current secondary subscript field value (subscript [1])
    \brief
      - set the current secondary subscript field value (subscript [1])
    \param value
      - unsigned value to set in 2nd / subscript [1] / b field of the vector
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& B(u4 value)
    { b = value; return *this; } // end v2u& v2u::B(u4)                       */


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
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& B(u4 value, u4 max, u4 min = 0)
    { b = InU(value, max, min); return *this; } // end v2u& v2u::B(u4,u4,u4)  */


    /** Create a v2u with a copy of this v2u's data in each member field value
    \brief
      - create a v2u with a copy of this v2u's data in each member field value
    \return
      - new v2u containing this v2u's data, per field
    */
    inline v2u Copy(void) const
    { return v2u(*this); } // end v2u v2u::Copy(void)                         */


    /** Format a v2u for being output as a packed vector notated string
    \brief
      - format a v2u for being output as a packed vector notated string
    \return
      - text formatted to (x,y) standards for later JSON interpretation
    */
    inline std::string JSON(void) const
    {
      return "(" + std::to_string(a) + "," + std::to_string(b) + ")";
    } // end std::string v2u::JSON(void) const                                */


    /** Set the v2u to the given <a, b> input unsigned values
    \brief
      - set the v2u to the given <a, b> input unsigned values
    \param a_value
      - (x | a  field value) - first element, [0] subscript
    \param b_value
      - (y | b field value) - second element, [1] subscript
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& Set(u4 a_value, u4 b_value)
    {
      return A(a_value).B(b_value);

    } // end v2u& v2u::Set(u4, u4)                                            */


    /** Set v2u to the given <a, b> input unsigned values between [min, max]
    \brief
      - set v2u to the given <a, b> input unsigned values between [min, max]
    \param a_value
      - (x | a  field value) - first element, [0] subscript
    \param b_value
      - (y | b field value) - second element, [1] subscript
    \param max
      - clamp value into desired maximum value range (inclusive)
    \param min
      - clamp value into desired minimum value range (inclusive; default 0)
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& Set(u4 a_value, u4 b_value, u4 max, u4 min = 0)
    {
      return A(a_value, max, min).B(b_value, max, min);

    } // end v2u& v2u::Set(u4, u4, u4, u4)                                    */


    /** Set the v2u to the given ( source.a, source.b ) values
    \brief
      - set the v2u to the given ( source.a, source.b ) values
    \param source
      - the v2u for memberwise copying of < a, b > input unsigned values
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& Set(const v2u& source)
    {
      return A(source.a).B(source.b);

    } // end v2u& v2u::Set(const v2u&)                                        */


    /** Set the v2u to clamped [min, max] of the given < source.a, source.b >
    \brief
      - set the v2u to clamped [min, max] of the given < source.a, source.b >
    \param src
      - the v2u for memberwise copying of < a, b > input unsigned values
    \param max
      - clamp value into desired maximum value range (inclusive)
    \param min
      - clamp value into desired minimum value range (inclusive; default 0)
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& Set(const v2u& src, u4 max, u4 min = 0)
    {
      return A(src.a,max,min).B(src.b,max,min);

    } // end v2u& v2u::Set(const v2u&, u4, u4)                                */


    /** Set the v2u to use < result.a, result.b > values (directly)
    \brief
      - set the v2u to use < result.a, result.b > values (directly)
    \param result
      - the v2u value reference from which to have values surrogated
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& Set(v2u&& result) noexcept
    {
      *vec = std::move(*result.vec);
      return *this;
    } // end v2u& v2u::Set(v2u&& result) noexcept                             */


    /** Format a v2u for being output as a point notated string
    \brief
      - format a v2u for being output as a point notated string
    \details
      - adds padding for uniform spacing: JSON tightly packs CSV elements
    \return
      - text set to < x, y > with leading space for negative / positive sign
    */
    inline std::string String(void) const
    {
      return "( " + std::to_string(a) + ", " + std::to_string(b) + " )";
    } // end std::string v2u::String(void) const                              */


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Sets the source v2u member values into this v2u value fields
    \brief
      - sets the source v2u member values into this v2u value fields
    \param source
      - the v2u which will have its values copied memberwise into this v2u
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& operator=(const v2u& source)
    { Set(source); } // end v2u& v2u::operator=(const v2u&)                   */


    /** Surrogate a v2u value reference returned from a function for this v2u
    \brief
      - surrogate a v2u value reference returned from a function for this v2u
    \param result
      - the v2u reference resultant from a function from which to steal values
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& operator=(v2u&& result) noexcept
    { Set(result); } // end v2u& v2u::operator=(v2u&&) noexcept               */


    /** Get a value from ['x|a'] or ['y|b'] channels (or equivalent)
    \brief
      - get a value from ['x|a'] or ['y|b'] channels (or equivalent)
    \details
      ( 0 | x | X | a | A | i | I | t | T ) -> [0]
      ( 1 | y | Y | b | B | o | O | u | U ) -> [1]
    \param channel
      - use (0|'x'|'a'|'i'|'u')->vec[0], (1|'y'|'b'|'o'|'t')->vec[1]
    \return
      - copy of the contained dimension member value associated to the channel
    */
    inline const u4& operator[](s1 channel) const
    {
      // clamp wildly out of range values into lowest 2 unsigned values
      if (!isascii(channel) || (channel<0x20 && channel>0xF)) { channel &= 1; }
      else if (IsDigit(channel)) { channel = ToNumeral(channel) & 1; }
      switch (channel) // Lookup array index equivalent for return
      {
      case 'y':case'b':case'o':case't':case'Y':case'B':case'O':case'T':case 1:
        return vec[1];
      case 'x':case'a':case'i':case'u':case'X':case'A':case'I':case'U':case 0:
      default:
        return vec[0];
      }
    } // end const u4& v2u::operator[](s1) const                              */


    /** Get a value from ['x|a'] or ['y|b'] channels (or equivalent)
    \brief
      - get a value from ['x|a'] or ['y|b'] channels (or equivalent)
    \details
      ( 0 | x | X | a | A | i | I | t | T ) -> [0]
      ( 1 | y | Y | b | B | o | O | u | U ) -> [1]
    \param channel
      - use (0|'x'|'a'|'i'|'u')->vec[0], (1|'y'|'b'|'o'|'t')->vec[1]
    \return
      - copy of the contained dimension member value associated to the channel
    */
    inline u4& operator[](s1 channel)
    {
      // clamp wildly out of range values into lowest 2 unsigned values
      if (!isascii(channel) || (channel<0x20 && channel>0xF)) { channel &= 1; }
      else if (IsDigit(channel)) { channel = ToNumeral(channel) & 1; }
      switch (channel) // Lookup array index equivalent for return
      {
      case 'y':case'b':case'o':case't':case'Y':case'B':case'O':case'T':
      case 1:
        return vec[1];
      case 'x':case'a':case'i':case'u':case'X':case'A':case'I':case'U':
      case 0: default:
        return vec[0];
      }
    } // end u4& v2u::operator[](s1)                                          */


    /** Calculate the member-wise addition of 2 v2u's and return the result
    \brief
      - calculate the member-wise addition of 2 v2u's and return the result
    \param other
      - source vector to have memberwise summation performed with a copy of this
    \return
      - copy of the memberwise sum of this v2u and the other into a new v2u
    */
    inline v2u operator+(const v2u& other) const
    { return Copy() += other; } // end v2u v2u::operator+(const v2u&) const   */


    /** Add an other v2u's values memberwise into this v2u's values
    \brief
      - add an other v2u's values memberwise into this v2u's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& operator+=(const v2u& other)
    {
      for (u4 i = 0; i < 2; ++i) { vec[i] += other.vec[i]; } return *this;

    } // end v2u& v2u::operator+=(const v2u&)                                 */


    /** Get the member-wise difference (Clamped to 0 minimums) of 2 v2u's
    \brief
      - get the member-wise difference (Clamped to 0 minimums) of 2 v2u's
    \param other
      - source vector to have memberwise difference calculated with this (copy)
    \return
      - copy of the memberwise difference of this and the other, in a new v2u
    */
    inline v2u operator-(const v2u& other) const
    { return Copy() -= other; } // end v2u v2u::operator-(const v2u&) const   */


    /** Subtract other v2u's values memberwise from this v2u (clamp underflow)
    \brief
      - subtract other v2u's values memberwise from this v2u (clamp underflow)
    \param other
      - source vector to have negated memberwise summation performed with this
    \return
      - reference to the modified v3u for easier subsequent inline action
    */
    inline v2u& operator-=(const v2u& other)
    {
      return A(a-other.a, a).B(b-other.b, b);

    } // end v2u& v2u::operator-=(const v2u&) */


    /** Scalar product multiplies each field uniformly by a scalar constant
    \brief
      - scalar product multiplies each field uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply a and b values
    \return
      - resultant, member-wise scaled copy of this v3u's values
    */
    inline v2u operator*(u4 scalar) const
    { return Copy() *= scalar; } // end v2u v2u::operator*(u4) const          */


    /** Scalar product sets all members to grow uniformly by a scalar constant
    \brief
      - scalar product sets all members to grow uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply both a and b values
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& operator*=(u4 scalar)
    { return A(a * scalar).B(b * scalar); } // end v2u& v2u::operator*=(u4)   */


    /** Get memberwise product of this multiplied with equivalent other values
    \brief
      - get memberwise product of this multiplied with equivalent other values
    \details
      - memberwise multiplication more granular & useful for integer math than
      dot product might be conventionally: Sum() this or Dot() if sum is wanted
    \param other
      - external source vector to be multiplied memberwise with this vector
    \return
      - memberwise product of 2 vectors: ( this.a * other.a, this.b * other.b )
    */
    inline v2u operator*(const v2u& other) const
    { return Copy() *= other; } // end v2u v2u::operator*(const v2u&) const   */


    /** Multiply equivalent other fields with this (memberwise multiply)
    \brief
      - multiply equivalent other fields with this (memberwise multiply)
    \param other
      - external source vector to be multiplied memberwise with this vector
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& operator*=(const v2u& other)
    {
      return A(a*other.a).B(b*other.b);
    } // end v2u& v2u::operator*=(const v2u&)                                 */


    /** Perform uniform division by divisor, returning the result
    \brief
      - perform uniform division by divisor, returning the result
    \details
      - no remainders in integer division: members should be at % = 0
    \param divisor
      - amount by which ( a, b ) values are uniformly divided
    \return
      - resultant, member-wise inversely scaled copy of this v2u's values
    */
    inline v2u operator/(u4 divisor) const
    { return Copy() /= divisor; } // end v2u v2u::operator/(f4) const         */


    /** Divides all members uniformly by divisor, setting the result
    \brief
      - divides all members uniformly by divisor, setting the result
    \details
      - no remainders consideredin integer division: members should be at % = 0
    \param divisor
      - amount by which ( a, b ) values are to be uniformly divided
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& operator/=(u4 divisor)
    { return A(a / divisor).B(b / divisor); } // end v2u& v2u::operator/=(u4) */


    /** Get memberwise quotient of this divided by equivalent other values
    \brief
      - get memberwise quotient of this divided by equivalent other values
    \details
      - memberwise (/) barred where conventionally (*) =: Dot(); not so here
    \param other
      - external source vector to be multiplied memberwise with this vector
    \return
      - memberwise product of 2 vectors: ( this.a * other.a, this.b * other.b )
    */
    inline v2u operator/(const v2u& other) const
    { return Copy() /= other; } // end v2u v2u::operator/(const v2u&) const   */


    /** Divide equivalent other fields with this (set memberwise quotient)
    \brief
      - Divide equivalent other fields with this (set memberwise quotient)
    \param other
      - external source vector to be the memberwise divisor to this vector
    \return
      - reference to the modified v2u for easier subsequent inline action
    */
    inline v2u& operator/=(const v2u& other)
    {
      return A(a/other.a).B(b/other.b);
    } // end v2u& v2u::operator/=(const v2u&)                                 */


  }; // end v2u union / struct / class

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
  inline v2u Distance(const v2u& initial, const v2u& terminal)
  { return terminal - initial; } // end v2u Distance(const v2u&, const v2u&)  */


  /** Left hand scalar multiplication scales all members of rhs (commutative)
  \brief
    - left hand scalar multiplication scales all members of rhs (commutative)
  \param scalar
    - scalar amount by which to multiply x and y values of right-hand side
  \param rhs
    - right-hand side argument to be copied and scaled in x and y by scalar
  \return
    - copy of rhs v2u's values, scaled by scalar in < x, y >
  */
  inline v2u operator*(u4 scalar, const v2u& rhs)
  { return rhs * scalar; } // end v2u operator*(u4, const v2u&)               */


   /** Confirm if 2 v2u's are equal in both of their a and b field values
   \brief
     - confirm if 2 v2u's are equal in both of their a and b field values
   \param lhs
     - left hand side to be compared memberwise with right hand for equality
   \param rhs
     - right hand side to be compared memberwise with left hand for equality
   \return
     - true only if both members are equal to one another in equivalent fields
   */
   inline bool operator==(const v2u& lhs, const v2u& rhs)
   {
     return (lhs.a == rhs.a && lhs.b == rhs.b);

   } // end bool operator==(const v2u&, const v2u&)                           */


   /** Confirm if 2 v2u's are not equal in either of their a or b field values
   \brief
     - confirm if 2 v2u's are not equal in either of their a or b field values
   \param lhs
     - left hand side to be compared memberwise with right hand for inequality
   \param rhs
     - right hand side to be compared memberwise with left hand for inequality
   \return
     - true if any members are not equal to one another in equivalent fields
   */
   inline bool operator!=(const v2u& lhs, const v2u& rhs)
   {
     return (lhs.a != rhs.a || lhs.b != rhs.b);

   } // end bool operator!=(const v2u&, const v2u&)                           */


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
   inline std::ostream& operator<<(std::ostream& output, const v2u& vector)
   {
     return output << vector.String();

   } // end std::ostream& operator<<(std::ostream&, const v2u&)               */


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   //! Zero vector in 3 dimensions; error state reserved value
   extern const v2u& nullv2u;

   static_assert(sizeof(v2u) == 8, "v2u size not packed");

} // end dp namespace
