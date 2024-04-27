/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  vNc.h
Purpose:  4-byte float-point array wrapper class for arbitrary dimensional math
Language: C++ / Eng, MSVC 19 (used, but should work with most compilers)
Platform: MSVC 19 / Windows 10 x64 (used, but should have no requirements)
Project:  a.Surprise_MAT300_1
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
#include <initializer_list> // Preferable variadic element list construction
#include <string>           // String building methods for text value output
// "./src/..."
#include "cplx.h"       // Complex number class for base vector dimension terms
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  class vNc //! arbitrary length [v]ector of a[N]y number of [c]omplex terms
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create vector of the given dimensions initialized to 0.0f per element
    \brief
      - create vector of the given dimensions initialized to 0.0f per element
    \param dimensions
      - number of elements the vector should be able to hold in [0,n-1] indices
    */
    vNc(u4 dimensions);

    /** Create an identity vector for [column] into a fourier transform's basis
    \brief
      - create vector of the given dimensions initialized to 0.0f per element
    \param dimensions
      - number of elements the vector should be able to hold in [0,n-1] indices
    \param column
      - populates with given column vector values for fourier transform matrix
    */
    vNc(u4 dimensions, u4 column);


    /** Create N-Dimensional vector of complex values from a list of values
    \brief
      - create N-Dimensional vector of complex values from a list of values
    \details
      - convenience constructor of complex values input presumably as literals
    \param init
      - variadic list of values to populate vector with from 0 index, upwards
    */
    vNc(std::initializer_list<cplx> init);


    /** Creates an N dimensional vector from another source N dimensional vNc
    \brief
      - creates an N dimensional vector from another source N dimensional vNc
    \param source
      - the vector to used for dimensional refereance and memberwise copying
    */
    vNc(const vNc& source);


    /** Creates a vNc from a vNc value reference resultant from a function
    \brief
      - creates a vNc from a vNc value reference resultant from a function
    \param result
      - the vNc value reference to surrogate into this
    */
    vNc(vNc&& result) noexcept;


    /** Clean up vector data before going out of scope
    \brief
      - clean up vector data before going out of scope
    */
    ~vNc(void);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Negate each member's imaginary term and return the resultant copy
    \brief
      - negate each member's imaginary term and return the resultant copy
    \return
      - vector of complex terms' memberwise conjugates (this[e].r, -this[e].i)
    */
    vNc Compliment(void) const;


    /** Calculate the elementwise product of (this[i] * !other[i]) in a new vNc
    \brief
      - calculate the elementwise product of (this[i] * !other[i]) in a new vNc
    \details
      - if dimensions are inequal, 0 values are assumed to fill in the shorter.
    \param other
      - vNc to be conjugated and multiplied with this into resultant vNc
    \return
      - list of elementwise conjugate products between vectors this and other
    */
    vNc ConjugateProduct(const vNc& other) const;


    /** Format a vector for being output as a comma separated value list
    \brief
      - format a vector for being output as a comma separated value list
    \return
      - text of comma separated values holding contained float values
    */
    std::string CSV(bool pack = true) const;


    /** Get the current dimensional length / element count of the vector
    \brief
      - get the current dimensional length / element count of the vector
    \return
      - count of N dimensional elements contained in the current vector
    */
    size_t Dimens(void) const;


    /** Calculate the dot product of (other * this) vector (product sum)
    \brief
      - calculate the dot product of (other &middot; this) vector (product sum)
    \details
      - if dimensions are inequal, 0 values are assumed to fill in the shorter
    \param other
      - vNc to be dotted (memberwise multiplied and summed) with this
    \return
      - memberwise sum of products
    */
    cplx InnerProduct(const vNc& other) const;


    /** Get vector length: sqrt([0]^2 + [1]^2 + ... + [n-2]^2 + [n-1]^2)
    \brief
      - get vector length: sqrt([0]^2 + [1]^2 + ... + [n-2]^2 + [n-1]^2)
    \details
      - if dimensions are inequal, 0 values are assumed to fill in the shorter
    \return
      - combined length of the vNc's elements : sqrt([0]^2 + ... + [n-1]^2)
    */
    f4 Length(void) const;


    /** Get the squared vector length: [0]^2 + [1]^2 + ... + [n-2]^2 + [n-1]^2
    \brief
      - get the squared vector length: [0]^2 + [1]^2 + ... + [n-2]^2 + [n-1]^2
    \details
      - square root is more taxing to call if it can be avoided
    \return
      - squared length of all vNc coordinates: x^2 + y^2 + z^2 + w^2
    */
    f4 LengthSquared(void) const;


    /** This vNc will be divided by its length and converted to a unit vector
    \brief
      - this vNc will be divided by its length and converted to a unit vector
    \details
      - null vector has indeterminate direction and will default to x-axis unit
    \return
      - reference to the modified vNc for easier subsequent inline action
    */
    vNc& Normalize(void);


    /** Calculate the elementwise product of (other[i] * this[i]) in a new vNc
    \brief
      - calculate the elementwise product of (other[i] * this[i]) in a new vNc
    \details
      - if dimensions are inequal, 0 values are assumed to fill in the shorter.
      Although memberwise multiplication is 'less' useful than dot and thereby
      not the default operator result, it still has uses in multiplication of
      attribute values and the like, even if not dimensionally as pertinent
    \param other
      - vNc to be multiplied elementwise with this in the resultant product vNc
    \return
      - list of elementwise products between the vectors this and other
    */
    vNc ProductWith(const vNc& other) const;


    /** Truncate dimension elements or add new 0 values in extra dimensions
    \brief
      - truncate dimension elements or add new 0 values in extra dimensions
    \details
      - invalidates references to prior member values from subscript operator
    \return
      - reference to the modified vNc for easier subsequent inline action
    */
    vNc& Resize(size_t dimensions);


    /** Set the elementwise product of (other[i] * this[i]) into this vector
    \brief
      - set the elementwise product of (other[i] * this[i]) into this vector
    \details
      - if dimensions are inequal, 0 values are assumed to fill in the shorter.
      Although memberwise multiplication is 'less' useful than dot and thereby
      not the default operator result, it still has uses in multiplication of
      attribute values and the like, even if not dimensionally as pertinent
    \param other
      - vNc to be multiplied elementwise with this as this vNc's new values
    \return
      - reference to the modified vNc for easier subsequent inline action
    */
    vNc& SetProduct(const vNc& other);


    /** Value copy will be divided by its length and converted to a unit vector
    \brief
      - value copy will be divided by its length and converted to a unit vector
    \return
      - displacement vector with length 1, in the direction of this vNc
    */
    vNc UnitVec(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Sets the source vNc member values into this vNc value fields
    \brief
      - sets the source vNc member values into this vNc value fields
    \param source
      - the vNc which will have its values copied memberwise into this vNc
    \return
      - reference to the modified vNc for easier subsequent inline action
    */
    vNc& operator=(const vNc& source);


    /** Surrogate a vNc value reference returned from a function for this vNc
    \brief
      - surrogate a vNc value reference returned from a function for this vNc
    \param result
      - the vNc reference resultant from a function from which to steal values
    \return
      - reference to the modified vNc for easier subsequent inline action
    */
    vNc& operator=(vNc&& result) noexcept;


    /** Get the value from a given [0, n-1] index subscript of the vector
    \brief
      - get the value from a given [0, n-1] index subscript of the vector
    \param index
      - element index [0, n-1] from the vector to be accessed
    \return
      - copy of the contained dimension member value associated to the index
    */
    const cplx& operator[](size_t index) const;


    /** Get a reference to the given [0, n-1] index subscript of the vector
    \brief
      - get a reference to the given [0, n-1] index subscript of the vector
    \param index
      - element index [0, n-1] from the vector to be accessed
    \return
      - reference to contained dimension member value associated to the index
    */
    cplx& operator[](size_t channel);


    /** Negates each imaginary value: for each { v[j] = <v[j].r, -v[j].i> }
    \brief
      - negates each imaginary value: for each { v[j] = <v[j].r, -v[j].i> }
    \return
      - copy of the vNc with conjugate member values
    */
    vNc operator!(void) const;


    /** Create a negated vector with negated copies of each element value
    \brief
      - create a negated vector with negated copies of each element value
    \return
      - copy of the vNc with negated member values
    */
    vNc operator-(void) const;


    /** Calculate the member-wise difference of 2 vNc's and return the result
    \brief
      - calculate the member-wise difference of 2 vNc's and return the result
    \param other
      - source vector to have memberwise difference calculated with this (copy)
    \return
      - copy of the memberwise difference of this and the other, in a new vNc
    */
    vNc operator-(const vNc& other) const;


    /** Add an other vNc's values memberwise into this vNc's values
    \brief
      - add an other vNc's values memberwise into this vNc's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to the modified vNc for easier subsequent inline action
    */
    vNc& operator-=(const vNc& other);


    /** Calculate the member-wise addition of 2 vNc's and return the result
    \brief
      - calculate the member-wise addition of 2 vNc's and return the result
    \details
      - the larger dimensioned vector may be used if inequal vectors are given
    \param other
      - source vector to have memberwise summation performed with a copy of this
    \return
      - copy of the memberwise sum of this vNc and the other into a new vNc
    */
    vNc operator+(const vNc& other) const;


    /** Add an other vNc's values memberwise into this vNc's values
    \brief
      - add an other vNc's values memberwise into this vNc's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to the modified vNc for easier subsequent inline action
    */
    vNc& operator+=(const vNc& other);


    /** Scalar product multiplies each field uniformly by a scalar constant
    \brief
      - scalar product multiplies each field uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x, y, z and w values
    \return
      - resultant, member-wise scaled copy of this vNc's value
    */
    vNc operator*(f4 scalar) const;


    /** Take others memberwise conjugate products with this, and sum the result
    \brief
      - take others memberwise conjugate products with this, and sum the result
    \param other
      - vector for which to use the compliment (memberwise conjugates) product
    \return
      - elementwise sum of products of this and other's compliment
    */
    cplx operator*(const vNc& other) const;


    /** Scalar product sets all members to grow uniformly by a scalar constant
    \brief
      - scalar product sets all members to grow uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x, y, z and w values
    \return
      - reference to the modified vNc for easier subsequent inline action
    */
    vNc& operator*=(f4 scalar);


    /** Perform uniform division by divisor, returning the result
    \brief
      - perform uniform division by divisor, returning the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which <x,y,z,w> values are uniformly divided
    \return
      - resultant, member-wise scaled copy of this vNc's values
    */
    vNc operator/(f4 divisor) const;


    /** Divides all members uniformly by divisor, setting the result
    \brief
      - divides all members uniformly by divisor, setting the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which <x,y,z,w> values are to be uniformly divided
    \return
      - reference to the modified vNc for easier subsequent inline action
    */
    vNc& operator/=(f4 divisor);


    /** Output ordered vector element values with brackets, commas and spaces
    \brief
      - output ordered vector element values with brackets, commas and spaces
    \return
      - element values as a bracketed, comma separated list with spaces
    */
    operator std::string(void) const;

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    //! Number of elements contained in the vector
    size_t dimens;

    //! List of values for each dimension member
    cplx* elem;

  }; // end vNc class

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                        Helper Operator Overloads                         */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


  /** Left hand scalar multiplication scales each element of rhs uniformly
  \brief
    - left hand scalar multiplication scales each element of rhs uniformly
  \param scalar
    - scalar amount by which to multiply x, y and z values of right-hand side
  \param rhs
    - right-hand side argument to be copied and scaled in x, y and z by scalar
  \return
    - copy of rhs vNc's values, scaled by scalar in <x,y,z>, with <w> intact
  */
   vNc operator*(f4 scalar, const vNc& rhs);


   /** Confirm if 2 vectors are relatively equal in all elements per dimension
   \brief
     - confirm if 2 vectors are relatively equal in all elements per dimension
   \param lhs
     - left hand side to be compared memberwise with right hand for equality
   \param rhs
     - right hand side to be compared memberwise with left hand for equality
   \return
     - true only if all members are in close proximity of one another
   */
   bool operator==(const vNc& lhs, const vNc& rhs);


   /** Confirm if 2 vNc's are not equal in any of x, y, z or w values
   \brief
     - confirm if 2 vNc's are not equal in any of x, y, z or w values
   \param lhs
     - left hand side to be compared memberwise with right hand for inequality
   \param rhs
     - right hand side to be compared memberwise with left hand for inequality
   \return
     - true if any members are not in close proximity of one another
   */
   bool operator!=(const vNc& lhs, const vNc& rhs);


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

} // end dp namespace
