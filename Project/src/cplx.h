/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  cplx.h
Purpose:  Real and imaginary, 4 byte float point coordinate pair container
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no platform requirements)
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

// <stl>
#include <string>       // String building methods for text value output
// "./src/..."
#include "ang.h"        // angle extraction requires arc unit parameter
#include "Value.h"      // Value manipulation; float point approximation
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp  //! Contents for use within DigiPen MAT (& CS) class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


  union cplx //! real and imaginary float point component vector container
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    struct {
      f4 real; //! Complex plane real component scale   (usually horizontal: x)
      f4 imag; //! Complex imaginary component scale      (usually vertical: y)
    };
    struct {
      f4 r;   //! Primary complex plane real axis scale (usually horizontal: x)
      f4 i;   //! Secondary complex imaginary axis scale  (usually vertical: y)
    };
    struct {
      f4 u;   //! Primary texture space axis scale  (ie horizontal [0,1] space)
      f4 v;   //! Secondary texture space axis scale  (ie vertical [0,1] space)
    };
    struct {
      f4 x;   //! Primary cartesian axis scale    (right if y-up; back if z-up)
      f4 y;   //! Secondary cartesian axis scale    (up if y-up; right if z-up)
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Creates a complex cartesian number (<real, imaginary>, default <1,0>)
    \brief
      - creates a complex cartesian number (<real, imaginary>, default <1,0>)
    \param rux_value
      - (r | u | x field value) - vector real x-axis coordinate
    \param ivy_value
      - (i | v | y field value) - vector imaginary / y-axis coordinate
    */
    cplx(f4 rux_value = 1.0f, f4 ivy_value = 0.0f);


    /** Creates a complex <real, imaginary> cartesian number from polar inputs
    \brief
      - creates a complex <real, imaginary> cartesian number from polar inputs
    \param rotation
      - angle for direction of complex real (cos) & imag (sin) coordinates
    \param magnitude
      - radius to scale angle of rotation by relative to unit circle
    */
    cplx(ang rotation, f4 magnitude = 1.0f);


    /** Creates a copy of a complex number from another source complex value
    \brief
      - creates a copy of a complex number from another source complex value
    \param source
      - the complex to used for memberwise <r|u|x, i|v|y> element value copying
    */
    cplx(const cplx& source);


    /** Creates a complex number from a r-value reference function result
    \brief
      - creates a complex number from a r-value reference function result
    \param result
      - the complex value reference to surrogate into this
    */
    cplx(cplx&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Get angle (arctangent), of current cartesian coordinates of vector
    \brief
      - get angle (arctangent), of current cartesian coordinates of vector
    \return
      - angle (arctangent), of the current cartesian coordinates
    */
    ang Angle(void) const;


    /** Set cartesian <r,i> coordinates in vector from polar coordinate inputs
    \brief
      - set cartesian <r,i> coordinates in vector from polar coordinate inputs
    \param angle_setting
      - angle to set the complex <r,i> values to be equivalent to in slope
    \param magnitude
      - length of vector after being set to the given angle
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& Angle(ang angle_setting, f4 magnitude = 1.0f);


    /** Complex number with -imag value: (a+bi)(a-bi)=a^2+b^2+0i; i cancels
    \brief
      - complex number with -imag value: (a+bi)(a-bi)=a^2+b^2+0i; i cancels
    \details
      - while (ax+iby)*(ax+iby) = a^2-b^2+(2ab)i, (ax+iby)*(ax-iby) = a^2+b^2,
       which simplifies in eliminating imaginary terms & helps in division, etc
    \return
      - complex number with negated imaginary component compared to this
    */
    cplx Conjugate(void) const;


    /** Get the complex number's real component value
    \brief
      - get the complex number's real component value
    \return
      - reference to contained y-axis coord in complex's imaginary f4 field
    */
    const f4& I(void) const;


    /** Set the complex number's real component value
    \brief
      - set the complex number's real component value
    \param ivy_value
      - float-point value to be set into the imaginary / y-axis member field
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& I(f4 ivy_value);


    /** Get the complex number's imaginary component value
    \brief
      - get the complex number's imaginary component value
    \return
      - reference to contained y-axis coord in complex's imaginary f4 field
    */
    const f4& Imag(void) const;


    /** Set the complex number's imaginary component value
    \brief
      - set the complex number's imaginary component value
    \param ivy_value
      - float-point value to be set into the imaginary / y-axis member field
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& Imag(f4 ivy_value);


    /** Get the complex number's imaginary component value
    \brief
      - get the complex number's imaginary component value
    \return
      - reference to contained y-axis coord in complex's imaginary f4 field
    */
    const f4& Imaginary(void) const;


    /** Set the complex number's imaginary component value
    \brief
      - set the complex number's imaginary component value
    \param ivy_value
      - float-point value to be set into the imaginary / y-axis member field
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& Imaginary(f4 ivy_value);


    /** Format a complex for being output as a packed vector notated string
    \brief
      - format a complex for being output as a packed vector notated string
    \return
      - text formatted to (real,i*imag) standards for later JSON interpretation
    */
    std::string JSON(void) const;


    /** Get the complex displacement / diagonal magnitude: sqrt(x^2 + y^2)
    \brief
      - get the complex displacement / diagonal magnitude: sqrt(x^2 + y^2)
    \details
      - always compare with lengthSquared instead if possible, for performance
    \return
      - magnitude of the complex's total displacement: sqrt(x^2 + y^2)
    */
    f4 Length(void) const;


    /** Get squared complex displacement / diagonal length: real^2 + imag^2
    \brief
      - get squared complex displacement / diagonal length: real^2 + imag^2
    \details
      - (real+imag)(real-imag) = real^2 + (real*imag - real*imag) + imag^2
       requires multiplying by the conjugate of itself rather than typical
       dot product in order to simplify out imaginary components and get length
    \return
      - squared magnitude of complex vector along its diagonal: real^2 + imag^2
    */
    f4 LengthSquared(void) const;


    /** Calculate the elementwise product of (other * this) in a new complex
    \brief
      - calculate the elementwise product of (other * this) in a new complex
    \details
      - dot products are conventional, but memberwise products are still useful
    \param other
      - complex to be multiplied elementwise with this in a product complex
    \return
      - list of elementwise products between this and the other vector
    */
    cplx Multiply(const cplx& other) const;


    /** Set the elementwise product of (other * this) into this vector
    \brief
      - set the elementwise product of (other * this) into this vector
    \details
      - dot products are conventional, but memberwise products are still useful
    \param other
      - complex to be multiplied elementwise with this as this complex's new values
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& MultiplyWith(const cplx& other);


    /** This remaps to the complex coordinate on unit circle (with length 1)
    \brief
      - this remaps to the complex coordinate on unit circle (with length 1)
    \details
      - indeterminate null or 0 angle defaults to cos(0) + i*sin(0), ie <1,0>
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& Normalize(void);


    /** Get the complex number's real component value
    \brief
      - get the complex number's real component value
    \return
      - reference to the contained x-axis coordinate in complex's real f4 field
    */
    const f4& R(void) const;


    /** Set the complex number's real component value
    \brief
      - set the complex number's real component value
    \param rux_value
      - the float-point value to be set into the real / x-axis member field
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& R(f4 rux_value);


    /** Get the complex number's real component value
    \brief
      - get the complex number's real component value
    \return
      - reference to the contained x-axis coordinate in complex's real f4 field
    */
    const f4& Real(void) const;


    /** Set the complex number's real component value
    \brief
      - set the complex number's real component value
    \param rux_value
      - the float-point value to be set into the real / x-axis member field
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& Real(f4 rux_value);


    /** Set the complex number to <r|u|x, i|v|y> input values
    \brief
      - set the complex number to <r|u|x, i|v|y> input values
    \param xu_value
      - (r | u | x field value) - vector real component scale
    \param yv_value
      - (i | v | y field value) - vector imaginary component scale
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& Set(f4 rux_value, f4 ivy_value);


    /** Set complex cartesian (r+i) number from polar angle and scalar input
    \brief
      - set complex cartesian (r+i) number from polar angle and scalar input
    \param angle
      - (a)ngle expression of <r*cos(a), r*sin(a)*i> cartesian vector direction
    \param magnitude
      - diagonal length ((r)adius) of displacement the vector should express
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& Set(ang angle, f4 magnitude = 1.0f);


    /** Set the complex values to <source.r, source.i> values
    \brief
      - set the complex values to <source.r, source.i> values
    \param source
      - the complex number to used for memberwise <r|u|x, i|v|y> copying
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& Set(const cplx& source);


    /** Set the complex to use <result.r, result.i> values (directly)
    \brief
      - set the complex to use <result.r, result.i> values (directly)
    \param result
      - the complex value reference from which to have values surrogated
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& Set(cplx&& result) noexcept;


    /** Format a complex number for output as a vector notated string
    \brief
      - format a complex number for output as a vector notated string
    \details
      - adds padding for uniform spacing: JSON tightly packs CSV elements
    \return
      - text set to ( real, imag*i ), uniformly spaced for legibility
    */
    std::string String(void) const;


    /** Get the complex real coordinate value under 2D texture labeling
    \brief
      - get the complex real coordinate value under 2D texture labeling
    \return
      - reference to the contained complex real / x-axis coordinate f4 field
    */
    const f4& U(void) const;


    /** Set the complex real coordinate value using 2D texture labeling
    \brief
      - set the complex real coordinate value using 2D texture labeling
    \param rux_value
      - the float-point value to be set into the complex real member field
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& U(f4 rux_value);


    /** Complex number, divided by its length and converted to a unit vector
    \brief
      - Complex number, divided by its length and converted to a unit vector
    \return
      - unit displacement in the direction of complex number <r|x,i|y> values
    */
    cplx UnitVec(void) const;


    /** Get the complex imaginary coordinate value using 2D texture labeling
    \brief
      - get the complex imaginary coordinate value using 2D texture labeling
    \return
      - reference to the contained, complex imaginary f4 coordinate field
    */
    const f4& V(void) const;


    /** Set the complex imaginary coordinate value using 2D texture labeling
    \brief
      - set the complex imaginary coordinate value using 2D texture labeling
    \param yv_value
      - float point value to be set into the complex imaginary member field
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& V(f4 ivy_value);


    /** Get the complex real coordinate value using 2D cartesian labeling
    \brief
      - get the complex real coordinate value using 2D cartesian labeling
    \return
      - reference to the contained x-axis coordinate in complex real f4 field
    */
    const f4& X(void) const;


    /** Set the complex real coordinate value using 2D cartesian labeling
    \brief
      - set the complex real coordinate value using 2D cartesian labeling
    \param rux_value
      - the float point value to be set into the complex real member field
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& X(f4 rux_value);


    /** Get the complex imaginary coordinate value using 2D cartesian labeling
    \brief
      - get the complex imaginary coordinate value using 2D cartesian labeling
    \return
      - reference to the contained, complex imaginary f4 coordinate field
    */
    const f4& Y(void) const;


    /** Set the complex imaginary coordinate value using 2D cartesian labeling
    \brief
      - set the complex imaginary coordinate value using 2D cartesian labeling
    \param ivy_value
      - float point value to be set into the complex imaginary member field
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& Y(f4 ivy_value);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Creates a negated copy of all complex coordinate values: <-real, -imag>
    \brief
      - creates a negated copy of all complex coordinate values: <-real, -imag>
    \return
      - copy of the complex with <-real, -imag> member values
    */
    cplx operator-(void) const;


    /** Sets the source complex member values into this complex value fields
    \brief
      - sets the source complex member values into this complex value fields
    \param source
      - the complex number to have its values copied into this complex number
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& operator=(const cplx& source);


    /** Surrogate a complex value reference returned from a function for this
    \brief
      - surrogate a complex value reference returned from a function for this
    \param result
      - the complex reference result from a function from which to steal values
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& operator=(cplx&& result) noexcept;


    /** Negates complex imaginary component to give the complex conjugate
    \brief
      - negates complex imaginary component to give the complex conjugate
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx operator!(void) const;


    /** Get a value from ['x'] or ['y'] channels (or equivalent)
    \brief
      - get a value from ['x'] or ['y'] channels (or equivalent)
    \details
      (0 | x | X | i | I | u | U) -> x
      (1 | y | Y | j | J | v | V) -> y
    \param channel
      - use (0|'x'|'u')->x, (1|'y'|'v')->y
    \return
      - copy of the contained dimension member value associated to the channel
    */
    f4 operator[](s1 channel) const;


    /** Get a value from ['x'] or ['y'] channels (or equivalent)
    \brief
      - get a value from ['x'] or ['y'] channels (or equivalent)
    \details
      (0 | x | X | i | I | u | U) -> x
      (1 | y | Y | j | J | v | V) -> y
    \param channel
      - use (0|'x'|'u')->x, (1|'y'|'v')->y
    \return
      - reference to the contained field associated to the subscript number
    */
    f4& operator[](s1 channel);


    /** Calculate the member-wise addition of 2 complex's and return the result
    \brief
      - calculate the member-wise addition of 2 complex's and return the result
    \param other
      - source vector to have memberwise summation performed with a copy of this
    \return
      - copy of the memberwise sum of this complex and the other into a new complex
    */
    cplx operator+(const cplx& other) const;


    /** Add an other complex's values memberwise into this complex's values
    \brief
      - add an other complex's values memberwise into this complex's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& operator+=(const cplx& other);


    /** Calculate the member-wise difference of 2 complex's and return the result
    \brief
      - calculate the member-wise difference of 2 complex's and return the result
    \param other
      - source vector to have memberwise difference calculated with this (copy)
    \return
      - copy of the memberwise difference of this and the other, in a new complex
    */
    cplx operator-(const cplx& other) const;


    /** Subtract an other complex's values memberwise from this complex's values
    \brief
      - subtract an other complex's values memberwise from this complex's values
    \param other
      - source vector to have negated memberwise summation performed with this
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& operator-=(const cplx& other);


    /** Scalar product multiplies each field uniformly by a scalar constant
    \brief
      - scalar product multiplies each field uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x and y values
    \return
      - resultant, member-wise scaled copy of this complex's values
    */
    cplx operator*(f4 scalar) const;


    /** Scalar product sets all members to grow uniformly by a scalar constant
    \brief
      - scalar product sets all members to grow uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x and y values
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& operator*=(f4 scalar);


    /** Get product of this and another complex number (resolving i = sqrt(-1))
    \brief
      - get product of this and another complex number (resolving i = sqrt(-1))
    \details
      - (r_1+i_1)(r_2+i_2) = (r_1*r_2 - i_1*i_2) + (r_1*i_2 + r_2*i_1)*(i)
       or in polar: (R_1)e^(i*t_1)*(R_2)e^(i*t_2) = (R_1*R_2)e^(i*t_1+t_2)
    \param other
      - source complex number to be multiplied with this complex number
    \return
      - scaling & rotation of this by another complex number
    */
    cplx operator*(const cplx& other) const;


    /** Set product of this and another complex number (resolving i = sqrt(-1))
    \brief
      - set product of this and another complex number (resolving i = sqrt(-1))
    \details
      - (r_1+i_1)(r_2+i_2) = (r_1*r_2 - i_1*i_2) + (r_1*i_2 + r_2*i_1)*(i)
       or in polar: (R_1)e^(i*t_1)*(R_2)e^(i*t_2) = (R_1*R_2)e^(i*t_1+t_2)
    \param other
      - source complex number to be multiplied with this complex number
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& operator*=(const cplx& other);


    /** Divides real & imaginary values uniformly by divisor, returning result
    \brief
      - divides real & imaginary values uniformly by divisor, returning result
    \details
      - (real, imag)/d = (real, imag)*(1/d) = (real/d, imag/d)
    \param divisor
      - amount by which real and imaginary values are to be uniformly divided
    \return
      - resultant, member-wise scaled copy of this complex's values
    */
    cplx operator/(f4 divisor) const;


    /** Divides real and imaginary values uniformly by divisor, setting result
    \brief
      - divides real and imaginary values uniformly by divisor, setting result
    \details
      - (real, imag)/d = (real, imag)*(1/d) = (real/d, imag/d)
    \param divisor
      - amount by which real and imaginary values are to be uniformly divided
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& operator/=(f4 divisor);


    /** Divides this complex number by another complex number, returning result
    \brief
      - divides this complex number by another complex number, returning result
    \details
      - (r1 + i1)/(r2 + i2) = (r1*r2 - r1*i2 + r2*i1 - i1*i2)/(r2^2 + i2^2),
       ie intermediary results (this*other.Conjugate())/other.LengthSquared(),
       as using other's conjugate unit fraction removes imaginary components
       from the denominator, allowing for subsequent scalar division
    \param other
      - complex number by which this complex number is to be divided
    \return
      - resultant, member-wise scaled copy of this complex's values
    */
    cplx operator/(const cplx& other) const;


    /** Divides this complex number by another complex number, setting result
    \brief
      - divides this complex number by another complex number, setting result
    \details
      - (r1 + i1)/(r2 + i2) = (r1*r2 - r1*i2 + r2*i1 - i1*i2)/(r2^2 + i2^2),
       ie intermediary results (this*other.Conjugate())/other.LengthSquared(),
       as using other's conjugate unit fraction removes imaginary components
       from the denominator, allowing for subsequent scalar division
    \param other
      - complex number by which this complex number is to be divided
    \return
      - reference to this complex number for easier subsequent inline action
    */
    cplx& operator/=(const cplx& other);

  }; // end cplx union / struct / class type declaration

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Left hand scalar multiplication scales all members of rhs
  \brief
    - left hand scalar multiplication scales all members of rhs
  \param scalar
    - scalar amount by which to multiply x and y values of right-hand side
  \param rhs
    - right-hand side argument to be copied and scaled in x and y by scalar
  \return
    - copy of rhs v2f's values, scaled by scalar in <x,y>
  */
  cplx operator*(f4 scalar, const cplx& rhs);


  /** Confirm if 2 complex numbers are relatively equal in real and imag values
  \brief
    - confirm if 2 complex numbers are relatively equal in real and imag values
  \param lhs
    - left hand side to be compared memberwise with right hand for equality
  \param rhs
    - right hand side to be compared memberwise with left hand for equality
  \return
    - true only if all members are in close proximity of one another
  */
  bool operator==(const cplx& lhs, const cplx& rhs);


  /** Confirm if 2 complex numbers are not equal in either real or imag values
  \brief
    - confirm if 2 complex numbers are not equal in either real or imag values
  \param lhs
    - left hand side to be compared memberwise with right hand for inequality
  \param rhs
    - right hand side to be compared memberwise with left hand for inequality
  \return
    - true if any members are not in close proximity of one another
  */
  bool operator!=(const cplx& lhs, const cplx& rhs);


  /** Output the string version of each of the complex elements' data
  \brief
    - output the string version of each of the complex elements' data
  \param output
    - stream to receive the vector string and and carry contents for output
  \param complex
    - number to have the real & imag values be converted to text based output
  \return
    - reference to the output stream object having the vector fed in
  */
  std::ostream& operator<<(std::ostream& output, const cplx& complex);


  /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                            Helper Constants                             */
  /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Zero vector in real and imaginary dimensions; error state reserved value
  extern const cplx& nullcplx;

} // end dp namespace