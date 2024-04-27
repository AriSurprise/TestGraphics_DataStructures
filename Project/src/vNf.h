/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  vNf.h
Purpose:  4-byte float-point array wrapper class for arbitrary dimensional math
Language: C++ / Eng, MSVC 19 (used, but should work with most compilers)
Platform: MSVC 19 / Windows 10 x64 (used, but should have no requirements)
Project:  a.Surprise_MAT300_1
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
#include <initializer_list> // Preferable variadic element list construction
#include <string>           // String building methods for text value output
#include <vector>           // EmptySet definition (null vector of vNf)
// "./src/..."
#include "TypeErrata.h"   // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  class mNf; // Make symmetrical matrix from vector dot transpose method

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  class vNf //! arbitrary length / dimension [v]ector of a[N]y 4-byte [f]loats 
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
    vNf(u4 dimensions = 0, f4 value = 0.0f);


    /** Create N-Dimensional vector of f4 values to hold from a list of values
    \brief
      - create N-Dimensional vector of f4 values to hold from a list of values
    \details
      - convenience of f8 promotion for freer input prioritized over efficiency
      of consistent float literal enforcement: convenience is the point here
    \param init
      - variadic list of values to populate vector with from 0 index, upwards
    */
    vNf(const std::vector<f4>& init);


    /** Create N-Dimensional vector of f4 values to hold from a list of values
    \brief
      - create N-Dimensional vector of f4 values to hold from a list of values
    \details
      - convenience of f8 promotion for freer input prioritized over efficiency
      of consistent float literal enforcement: convenience is the point here
    \param init
      - variadic list of values to populate vector with from 0 index, upwards
    */
    vNf(std::initializer_list<f4> init);


    /** Creates an N dimensional vector from another source N dimensional vNf
    \brief
      - creates an N dimensional vector from another source N dimensional vNf
    \param source
      - the vector to used for dimensional refereance and memberwise copying
    */
    vNf(const vNf& source);


    /** Creates a vNf from a vNf value reference resultant from a function
    \brief
      - creates a vNf from a vNf value reference resultant from a function
    \param result
      - the vNf value reference to surrogate into this
    */
    vNf(vNf&& result) noexcept;


    /** Clean up vector data before going out of scope
    \brief
      - clean up vector data before going out of scope
    */
    ~vNf(void);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Format a vector output as a comma separated value string in brackets
    \brief
      - format a vector output as a comma separated value string in brackets
    \param pack
      - separate values with spaces for legibility if false; save space if true
    \return
      - ASCII text clip ( [] enclosed ), notation of comma separated values
    */
    std::string ClipString(bool pack = false) const;


    /** Format a vector for being output as a comma separated value string
    \brief
      - format a vector for being output as a comma separated value string
    \return
      - ASCII text of packed, comma delimited values holding contained floats
    */
    std::string CSVString(bool pack = true) const;


    /** Get the current dimensional length / element count of the vector
    \brief
      - get the current dimensional length / element count of the vector
    \return
      - count of N dimensional elements contained in the current vector
    */
    inline u4 Dimens(void) const { return dimens; }


    /** Calculate the dot product of (other * this) vector (product sum)
    \brief
      - calculate the dot product of (other &middot; this) vector (product sum)
    \details
      - if dimensions are inequal, 0 values are assumed to fill in the shorter
    \param other
      - vNf to be dotted (memberwise multiplied and summed) with this
    \return
      - memberwise sum of products
    */
    f4 Dot(const vNf& other) const;


    /** Get the square matrix of symmetrical memberwise cell product data
    \brief
      - get the square matrix of symmetrical memberwise cell product data
    \return
      - symmetrical square matrix with cells of vector data memberwise products
    */
    mNf DotTranspose(void) const;


    /** Check if the dimens are 0: subset of null vectors with no 0 values
    \brief
      - check if the dimens are 0: subset of null vectors with no 0 values
    \return
      - false if any element is present; dimensions exist, otherwise true
    */
    inline bool IsEmpty(void) const { return !elem; }


    /** Check if multiple dimensions are present on the vector: 2D or above
    \brief
      - check if multiple dimensions are present on the vector: 2D or above
    \details
      - exists to help preclude !IsScalar() yielding positives on empty sets
      as well as to achieve !IsEmpty() && !IsScalar() in 1 comparison
    \return
      - false if dimensions are less than 2, otherwise true
    */
    inline bool IsNonScalar(void) const { return dimens > 1; }


    /** Check if all vector elements are 0, or if the dimens are 0 (nullvec)
    \brief
      - check if all vector elements are 0, or if the dimens are 0 (nullvec)
    \return
      - false if any non-zero value is present in any dimension, otherwise true
    */
    bool IsNull(void) const;


    /** Check if dimensions 1: 1D is scalar; 0D (empty) & 2+D vectors are not
    \brief
      - check if dimensions 1: 1D is scalar; 0D (empty) & 2+D vectors are not
    \details
      - note: negation does not imply that the non-scalar vector is non-empty
      situations where 1D matters primarily prioritizing the precise value
    \return
      - false if dimensions are not exactly equal to 1, otherwise true
    */
    inline bool IsScalar(void) const { return dimens == 1; }


    /** Get vector length: sqrt([0]^2 + [1]^2 + ... + [n-2]^2 + [n-1]^2)
    \brief
      - get vector length: sqrt([0]^2 + [1]^2 + ... + [n-2]^2 + [n-1]^2)
    \details
      - if dimensions are inequal, 0 values are assumed to fill in the shorter
    \return
      - combined length of the vNf's elements : sqrt([0]^2 + ... + [n-1]^2)
    */
    f4 Length(void) const;


    /** Get the squared vector length: [0]^2 + [1]^2 + ... + [n-2]^2 + [n-1]^2
    \brief
      - get the squared vector length: [0]^2 + [1]^2 + ... + [n-2]^2 + [n-1]^2
    \details
      - square root is more taxing to call if it can be avoided
    \return
      - squared length of all vNf coordinates: x^2 + y^2 + z^2 + w^2
    */
    f4 LengthSquared(void) const;


    /** Calculate the elementwise product of (other[i] * this[i]) in a new vNf
    \brief
      - calculate the elementwise product of (other[i] * this[i]) in a new vNf
    \details
      - if dimensions are inequal, 0 values are assumed to fill in the shorter.
      Although memberwise multiplication is 'less' useful than dot and thereby
      not the default operator result, it still has uses in multiplication of
      attribute values and the like, even if not dimensionally as pertinent
    \param other
      - vNf to be multiplied elementwise with this in the resultant product vNf
    \return
      - list of elementwise products between the vectors this and other
    */
    vNf Multiply(const vNf& other) const;


    /** Set the elementwise product of (other[i] * this[i]) into this vector
    \brief
      - set the elementwise product of (other[i] * this[i]) into this vector
    \details
      - if dimensions are inequal, 0 values are assumed to fill in the shorter.
      Although memberwise multiplication is 'less' useful than dot and thereby
      not the default operator result, it still has uses in multiplication of
      attribute values and the like, even if not dimensionally as pertinent
    \param other
      - vNf to be multiplied elementwise with this as this vNf's new values
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& MultiplyWith(const vNf& other);


    /** This vNf will be divided by its length and converted to a unit vector
    \brief
      - this vNf will be divided by its length and converted to a unit vector
    \details
      - null vector has indeterminate direction and will default to x-axis unit
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& Normalize(void);


    /** Format a vector output as a comma separated value string in brackets
    \brief
      - format a vector output as a comma separated value string in brackets
    \param pack
      - separate values with spaces for legibility if false; save space if true
    \return
      - ASCII text point ( () enclosed ), notation of comma separated values
    */
    std::string PointString(bool pack = false) const;


    /** Truncate dimension elements or add new 0 values in extra dimensions
    \brief
      - truncate dimension elements or add new 0 values in extra dimensions
    \details
      - invalidates references to prior member values from subscript operator
    \param dimensions
      - new element count or dimensional length the vector should have
    \param value
      - value to emplace into any new elements resultant of an increased size
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& Resize(u4 dimensions, f4 value = 0.0f);


    /** Value copy will be divided by its length and converted to a unit vector
    \brief
      - value copy will be divided by its length and converted to a unit vector
    \return
      - displacement vector with length 1, in the direction of this vNf
    */
    vNf UnitVec(void) const;


    /** Format a vector output as a comma separated value string in braces
    \brief
      - format a vector output as a comma separated value string in braces
    \param pack
      - separate values with spaces for legibility if false; save space if true
    \return
      - ASCII text vector ( <> enclosed ), notation of comma separated values
    */
    std::string VecString(bool pack = false) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a negated vector with negated copies of each element value
    \brief
      - create a negated vector with negated copies of each element value
    \return
      - copy of the vNf with negated member values
    */
    vNf operator-(void) const;


    /** Sets the source vNf to a 1D vNf with the given value set in subscript 0
    \brief
      - sets the source vNf to a 1D vNf with the given value set in subscript 0
    \param vale
      - the scalar value to be used in subscript 0 as the entire vNf
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& operator=(f4 value);


    /** Sets the source vNf member values into this vNf value fields
    \brief
      - sets the source vNf member values into this vNf value fields
    \param source
      - the vNf which will have its values copied memberwise into this vNf
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& operator=(const vNf& source);


    /** Surrogate a vNf value reference returned from a function for this vNf
    \brief
      - surrogate a vNf value reference returned from a function for this vNf

    \param result
      - the vNf reference resultant from a function from which to steal values
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& operator=(vNf&& result) noexcept;


    /** Negates each element value: for each { element[i] = -element[i]; }
    \brief
      - negates each element value: for each { element[i] = -element[i]; }
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& operator!(void);


    /** Get the value from a given [0, n-1] index subscript of the vector
    \brief
      - get the value from a given [0, n-1] index subscript of the vector
    \param index
      - element index [0, n-1] from the vector to be accessed
    \return
      - copy of the contained dimension member value associated to the index
    */
    f4 operator[](size_t index) const;


    /** Get a reference to the given [0, n-1] index subscript of the vector
    \brief
      - get a reference to the given [0, n-1] index subscript of the vector
    \param index
      - element index [0, n-1] from the vector to be accessed
    \return
      - reference to contained dimension member value associated to the index
    */
    f4& operator[](size_t channel);


    /** Calculate the member-wise addition of 2 vNf's and return the result
    \brief
      - calculate the member-wise addition of 2 vNf's and return the result
    \details
      - the larger dimensioned vector may be used if inequal vectors are given
    \param other
      - source vector to have memberwise summation performed with a copy of this
    \return
      - copy of the memberwise sum of this vNf and the other into a new vNf
    */
    vNf operator+(const vNf& other) const;


    /** Add an other vNf's values memberwise into this vNf's values
    \brief
      - add an other vNf's values memberwise into this vNf's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& operator+=(const vNf& other);


    /** Calculate the member-wise difference of 2 vNf's and return the result
    \brief
      - calculate the member-wise difference of 2 vNf's and return the result
    \param other
      - source vector to have memberwise difference calculated with this (copy)
    \return
      - copy of the memberwise difference of this and the other, in a new vNf
    */
    vNf operator-(const vNf& other) const;


    /** Add an other vNf's values memberwise into this vNf's values
    \brief
      - add an other vNf's values memberwise into this vNf's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& operator-=(const vNf& other);


    /** Scalar product multiplies each field uniformly by a scalar constant
    \brief
      - scalar product multiplies each field uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x, y, z and w values
    \return
      - resultant, member-wise scaled copy of this vNf's valuce
    */
    vNf operator*(f4 scalar) const;


    /** Scalar product sets all members to grow uniformly by a scalar constant
    \brief
      - scalar product sets all members to grow uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x, y, z and w values
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& operator*=(f4 scalar);


    /** Take equivalent other element product (0 if none), sum the result (dot)
    \brief
      - take equivalent other element product (0 if none), sum the result (dot)
    \param other
      - external source vector to be dot multiplied with this vector
    \return
      - dot product of members: a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w
    */
    f4 operator*(const vNf& other) const;


    /** Perform uniform division by divisor, returning the result
    \brief
      - perform uniform division by divisor, returning the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which <x,y,z,w> values are uniformly divided
    \return
      - resultant, member-wise scaled copy of this vNf's values
    */
    vNf operator/(f4 divisor) const;


    /** Divides all members uniformly by divisor, setting the result
    \brief
      - divides all members uniformly by divisor, setting the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which <x,y,z,w> values are to be uniformly divided
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    vNf& operator/=(f4 divisor);


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
    u4 dimens;

    //! List of values for each dimension member
    f4* elem;

  }; // end vNf class

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
    - copy of rhs vNf's values, scaled by scalar in <x,y,z>, with <w> intact
  */
   vNf operator*(f4 scalar, const vNf& rhs);


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
   bool operator==(const vNf& lhs, const vNf& rhs);


   /** Confirm if 2 vNf's are not equal in any of x, y, z or w values
   \brief
     - confirm if 2 vNf's are not equal in any of x, y, z or w values
   \param lhs
     - left hand side to be compared memberwise with right hand for inequality
   \param rhs
     - right hand side to be compared memberwise with left hand for inequality
   \return
     - true if any members are not in close proximity of one another
   */
   bool operator!=(const vNf& lhs, const vNf& rhs);


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   const std::vector<vNf> emptySet;

} // end dp namespace
