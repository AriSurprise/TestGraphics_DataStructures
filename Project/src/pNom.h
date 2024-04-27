/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  pNom.h
Purpose:  Polynomial container for arrays of terms using comparable variables
Details:  Not wanting to go full in on a recursive definition of a polynomial,
but wanting to support terms not only in the standard basis, nor a given linear
increasing probe of exponentiation per term, nor be limited to single variable
polynomials only, the data structure for something that open-ended quickly gets
rather convoluted.
Each term in the polynomial may have any number of variables, each capable of
being expressed in non-standard basis form (but still limited to 1 variable per
subterm basis), each of those being able to have each term raised to any power
with any coefficient.  Those subterm variables differ primarily from terms in
that the variable list of a term will be a product, while neighboring terms of
dissimilar products sum to form the total polynomial.
If a variable has a null list of exponents for basis vector
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
// "./src/..."
#include "pTerm.h"        // Constituent polynomial term struct definitions
#include "TypeErrata.h"   // Platform primitive type aliases for easier porting
#include "vNf.h"          // pNom terms use arbitrary length vectors for bases


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                      Helper Constant Delcarations                        */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  class pNom //! Polynomial container of arbitrary length term list
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Defaultable constructor using a list of polynomial terms & their labels
    \brief
      - defaultable constructor using a list of polynomial terms & their labels
    \param termList
      - list of data with leading coefficient, and basis per variable label
    \param labels
      - name list associated to termList indicies: {"x","y"} => [0]x, [1]y
    */
    pNom(const std::vector<pTerm>& termList = {},
      const std::vector<std::string>& labels = { "x" });

    /** Constructor using a list of polynomial terms (and their label literals)
    \brief
      - constructor using a list of polynomial terms (and their label literals)
    \param termList
      - list of data with leading coefficient, and basis per variable label
    \param labels
      - name list associated to termList indicies: {"x","y"} => [0]x, [1]y
    */
    pNom(const std::vector<pTerm>& termList,
      std::initializer_list<const char*> labels = { "x" });

    /** Constructor using both polynomial term (and string label) literal lists
    \brief
      - constructor using both polynomial term (and string label) literal lists
    \details
      - pTerm initializer list can take several formats: pattern implied being
      => lCoeff*(c_0*x_0^{e_0}c_1*x_1^{e_1}c_2*x_2^{e_2})^{pExp} (tb expanded)
    { lCoeff, {u4 termCount, (type=CoPat::Constant), (u4 pExp=1), f4 scl=1.0} }
        allowing generic patterned polynomial basis to be given
    { lCoeff, { c_0, c_1, c_2 }, { e_0, e_1, e_2 }, (pExp = 1u) }
        allowing shorthand for coefficients to be omitted ({}), or exponents:
          omitted coefficients will be 1; exponents will be a linear increment
          from 0 or the last given power value (if shorter than coeff list)
    { lCoeff, { { c_0, e_0 }, { c_1, e_1 }, { c_2, e_2 } }, (pExp = 1u) }
        enforcing symmetry for coefficient, exponent tuples to be grouped
    \param termList
      - list of data with leading coefficient, and basis per variable label =:
    i) {f4 lCoeff, {u4 term#, (type=CoPat::Constant), (u4 pExp=1), f4 scl=1.0}}
    ii) {f4 lCoeff, <f4>{ c_0, c_1, c_2 }, <f4>{ e_0, e_1, e_2 }, (pExp = 1u) }
    iii) {f4 lCoeff, <f4,f4>{{c_0, e_0}, {c_1, e_1}, {c_2, e_2}}, (pExp = 1u)}
    \param labels
      - name list associated to termList indicies: {"x","y"} => [0]x, [1]y
    */
    pNom(std::initializer_list<pTerm> termList,
      std::initializer_list<const char*> labels = { "x" });

    /** Constructor to infer a basis based on a typification pattern
    \brief
      - constructor to infer a basis based on a typification pattern
    \param lCoeffs
      - scalar leading coefficient per term
    \param vLabel
      - variable name associated to termList indiex [0]
    */
    pNom(CoeffPattern type, std::initializer_list<f4> lCoeffs,
      const char* vLabel = "x");

    /** Constructor to infer a basis based on a typification pattern
    \brief
      - constructor to infer a basis based on a typification pattern
    \details
      - leading coeff vectors allow for parameterization of basis in n dimens
    \param lCoeffs
      - n dimensional lead coefficient vector list (promoted given max among n)
    \param vLabel
      - variable name associated to termList indiex [0]
    */
    pNom(CoeffPattern type, std::initializer_list<vNf> lCoeffs,
      const char* vLabel = "x");

    /** Copy pNom constructor copies all vectors of polynomial variable data
    \brief
      - copy pNom constructor copies all vectors of polynomial variable data
    \param other
      - polynomial data to reference in copying when setting this pNom's values
    */
    pNom(const pNom& other);

    /** Move pNom constructor appropriates all vectors of pNom result data
    \brief
      - move pNom constructor appropriates all vectors of pNom result data
    \param result
      - polynomial data to directly take as this pNom's initialization values
    */
    pNom(pNom&& result) noexcept;

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set variable subscripts to use the new given set of ordered labels
    \brief
      - set variable subscripts to use the new given set of ordered labels
    \return
      - reference to the modified polynomial for subsequent inline operations
    */
    pNom& SetLabels(const std::vector<std::string>& labels);

    /** Set variable subscripts to use the new given set of ordered labels
    \brief
      - set variable subscripts to use the new given set of ordered labels
    \return
      - reference to the modified polynomial for subsequent inline operations
    */
    pNom& SetLabels(std::initializer_list<const char*> labels);

    /** Get the number of terms the polynomial has to be list in total
    \brief
      - get the number of terms the polynomial has to be list in total
    \return
      - number of unique factor product terms listed in the polynomial
    */
    inline u4 Terms(void) const { return static_cast<u4>(term.size()); }

    /** Get the number of terms the polynomial has to be list in total
    \brief
      - get the number of terms the polynomial has to be list in total
    \details
      - non-const to allow debug mode to clearly generate variable labels
    \return
      - number of unique factor product terms listed in the polynomial
    */
    std::string ToString(void);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a negated vector with negated copies of each element value
    \brief
      - create a negated vector with negated copies of each element value
    \return
      - copy of the vNf with negated member values
    */
    pNom operator-(void) const;


    /** Sets the source vNf member values into this vNf value fields
    \brief
      - sets the source vNf member values into this vNf value fields
    \param source
      - the vNf which will have its values copied memberwise into this vNf
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    pNom& operator=(const pNom& source);


    /** Surrogate a vNf value reference returned from a function for this vNf
    \brief
      - surrogate a vNf value reference returned from a function for this vNf

    \param result
      - the vNf reference resultant from a function from which to steal values
    \return
      - reference to the modified vNf for easier subsequent inline action
    */
    pNom& operator=(pNom&& result) noexcept;


    /** Get the value from a given [0, n-1] index subscript of the vector
    \brief
      - get the value from a given [0, n-1] index subscript of the vector
    \param index
      - element index [0, n-1] from the vector to be accessed
    \return
      - copy of the contained dimension member value associated to the index
    */
    f4 operator[](size_t index) const;


    /** Get
    \brief
      - get
    \param index
      - 
    \return
      - 
    */
    pNom& operator[](size_t index);


    /** Add
    \brief
      - add
    \param other
      - 
    \return
      - 
    */
    pNom operator+(const pNom& other) const;


    /** Add
    \brief
      - add
    \param other
      - 
    \return
      - reference to the modified polynomial for subsequent inline action
    */
    pNom& operator+=(const pNom& other);


    /** Subtract
    \brief
      - subtract
    \param other
      - 
    \return
      - 
    */
    pNom operator-(const pNom& other) const;


    /** Subtract
    \brief
      - subtract
    \param other
      - 
    \return
      - reference to the modified polynomial for subsequent inline action
    */
    pNom& operator-=(const pNom& other);


    /** Multiply
    \brief
      - multiply
    \param scalar
      - 
    \return
      - 
    */
    pNom operator*(f4 scalar) const;


    /** Multiply
    \brief
      - multiply
    \param scalar
      - 
    \return
      - reference to the modified polynomial for subsequent inline action
    */
    pNom& operator*=(f4 scalar);


    /** Multiply
    \brief
      - multiply
    \param other
      - 
    \return
      - 
    */
    pNom operator*(const pNom& other) const;


    /** Divide
    \brief
      - divide
    \param divisor
      - scalar reciprocal for multiplication, per term
    \return
      - 
    */
    pNom operator/(f4 divisor) const;


    /** Divides all members uniformly by divisor, setting the result
    \brief
      - divides all members uniformly by divisor, setting the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which each term is to have coefficients uniformly divided
    \return
      - reference to the modified polynomial for subsequent inline action
    */
    pNom& operator/=(f4 divisor);


    /** Divide
    \brief
      - divide
    \param divisor
      - scalar reciprocal for multiplication, per term
    \return
      - 
    */
    pNom operator/(const pNom& other) const;


    /** Divides this polynomial by another, setting the result
    \brief
      - divides all members uniformly by divisor, setting the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which each term is to have coefficients uniformly divided
    \return
      - reference to the modified polynomial for subsequent inline action
    */
    pNom& operator/=(const pNom& other);



    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    /** Determine if provided variable label set is conformant with defaults
    \brief
      - determine if provided variable label set is conformant with defaults
    \details
      - =: {x,y,z,w,v,u,t,s,r,q,p,o,n,m,l,k,j,i,h,g,f,e,d,c,b,a}, in that order
    \return
      - true if label set conforms to {x,y,z,w,v,u,t,s,r,...,c,b,a}, in order
    */
    void ConformBasis(void);

    /** Determine if provided variable label set is conformant with defaults
    \brief
      - determine if provided variable label set is conformant with defaults
    \details
      - =: {x,y,z,w,v,u,t,s,r,q,p,o,n,m,l,k,j,i,h,g,f,e,d,c,b,a}, in that order
    \return
      - true if label set conforms to {x,y,z,w,v,u,t,s,r,...,c,b,a}, in order
    */
    bool DefaultLabelConformant(um* offset = nullptr) const;

    /** Extend default conformant variable label set to use the new 
    \brief
      - determine if provided variable label set is conformant with defaults
    */
    void ExtendLabels(um variables, um* offset = nullptr);

    //! list of identifier label correlated to subscripts in each term variable
    std::vector<std::string> label;

    //! List of polynomial term segments with unique multiplicands
    std::vector<pTerm> term;

  }; // end pNom class declaration

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                        Helper Operator Overloads                         */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


  /** Left hand scalar multiplication scales each element of rhs uniformly
  \brief
    - left hand scalar multiplication scales each element of rhs uniformly
  \param scalar
    - scalar amount by which to multiply each term of the right-hand side
  \param rhs
    - right-hand side argument to be copied and scaled in each term scalar
  \return
    - copy of right hand side's terms, multiplied per term by scalar
  */
   pNom operator*(f4 scalar, const pNom& rhs);


   /** Confirm if 2 polynomials are equal in regards to all terms
   \brief
     - confirm if 2 polynomials are equal in regards to all terms
   \param lhs
     - left hand side to be compared memberwise with right hand for equality
   \param rhs
     - right hand side to be compared memberwise with left hand for equality
   \return
     - true only if all terms are in close proximity of one another
   */
   bool operator==(const pNom& lhs, const pNom& rhs);


   /** Confirm if 2 polynomials are not equal in regards to any term
   \brief
     - confirm if 2 polynomials are not equal in regards to any term
   \param lhs
     - left hand side to be compared memberwise with right hand for inequality
   \param rhs
     - right hand side to be compared memberwise with left hand for inequality
   \return
     - true if any terms are not in close proximity of one another
   */
   bool operator!=(const pNom& lhs, const pNom& rhs);


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! names to reference in populating through defaults as variables are added
   const std::vector<std::string> dPNomL = { "x", "y", "z", "w", "v", "u", "t",
     "s", "r", "q", "p", "o", "n", "m", "l", "k", "j", "i", "h", "g", "f", "e",
     "d", "c", "b", "a"};

} // end dp namespace
