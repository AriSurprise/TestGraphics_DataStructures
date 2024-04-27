/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  pTerm.h
Purpose:  Constituent chunks of data listed / summed in a pNom polynomial

Details:  Not wanting to go full in on a recursive definition of a polynomial,
but wanting to support terms not only in the standard basis, nor limited to
linear increasing probes of exponentiation per term, as well as allowing for
more than single variable polynomials, the data structure for something that
open-ended quickly gets rather convoluted.
Each term in the polynomial may have any number of variables, each capable of
being expressed in non-standard basis form (but still limited to 1 variable per
subterm basis), each of those being able to have each term raised to any power
with any coefficient (ie 4(x^{2}-2x+1)(y^{0.25}+8y^{0.5}+16)).  Those subterms'
variables differ primarily from terms in that the variable list of a term will
be a product, while neighboring terms of dissimilar products sum to form the
total polynomial.

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
#include "TypeErrata.h"   // Platform primitive type aliases for easier porting
#include "Value.h"        // Manipulate values in simple inline setters
#include "vNf.h"          // pNom terms use arbitrary length vectors for bases


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Typification of coefficient patterns for initialization / recognition
  enum class CoeffPattern
  {
    Constant,     //! All terms have coefficient 1
    Oscillating,  //! Terms alternate between having coefficient 1 and -1
    Pascal,       //! Coefficients align to a given n over all k in nCr(n,k)
    OscPascal,    //! Coefficients alternate signs {+,-} over all k in nCr(n,k)
    Bernstein,    //! Coefficients use osc. lesser pascal basis sums per term
    OscBernstein, //! Bernstein basis with alternating term signs (basis too)
    CumlBernstein,//! Cumulative Bernstein basis term sums used per term
    OscCumlBern,  //! Cumulative Bernstein basis with osc. term signs (+ basis)
  };

  //! Standard basis single variable term: leading coefficient raised to a power
  union SimpleTerm
  {
    /** Initialize a standard basis variable product with an exponentiation set
    \brief
      - initialize a standard basis variable product with an exponentiation set
    \param lCoeff
      - leading coefficient for which variable has that scalar multiplicity
    \param exponentiation
      - exponentiation (power raised; dimensional multiplicity) for the term
    */
    SimpleTerm(f4 lCoeff = 1.0f, f4 exponentiation = 1.0);

    /** Output the string representation of the standard basis term
    \brief
      - output the string representation of the standard basis term
    \param vLabel
      - formatted variable identifier label for multiplicity of coeff & power
    \param lead
      - leading terms don't display positive signs or space signs out
    \return
      - string version of value, ie {2,3}("v", 1) => " + 2v^{3}"
    */
    std::string ToString(const std::string& vLabel, bool lead) const;


    /** Multiplication operator for 2 polynomial terms <coeff, power> tuples
    \brief
      - multiplication operator for 2 polynomial terms <coeff, power> tuples
    \param other
      - right hand term to use in editing *this = < c * other.c, e + other.e >
    \return
      - true only if other has a greater exponent value than this term's power
    */
    inline SimpleTerm& operator*=(const SimpleTerm& other)
    { coeff *= other.coeff; power += other.power; return *this; }

    /** Lesser comparison evaluating lesser terms as those with low exponents
    \brief
      - lesser comparison evaluating lesser terms as those with low exponents
    \param other
      - right hand simple term for (exponent) comparison with this term
    \return
      - true only if other has a greater exponent value than this term's power
    */
    inline bool operator<(const SimpleTerm& other) const
    { return power < other.power; }

    /** Greater comparison evaluating lesser terms as those with low exponents
    \brief
      - lesser comparison evaluating lesser terms as those with low exponents
    \param other
      - right hand simple term for (exponent) comparison with this term
    \return
      - true only if other has a greater exponent value than this term's power
    */
    inline bool operator>(const SimpleTerm& other) const
    { return power > other.power; }

    /** Equality comparison checks for exponential equality (coeffs coalescable)
    \brief
      - equality comparison checks for exponential equality (coeffs coalescable)
    \details
      - equality only values exponentiation because matching coefficients group
      which does presume that the variable being raised to that power matches
      which is an external reference this can not determine intrinsically
    \param other
      - right hand term used in checking that powers match: ( e == other.e )
    \return
      - true if exponent values match, false if power values differ
    */
    inline bool operator==(const SimpleTerm& other) const
    { return power == other.power; }

    /** Inequality comparison operator checks coeff & power for inequality
    \brief
      - Inequality comparison operator checks coeff & power for inequality
    \param other
      - right hand term used in checking that ( c != other.c || e != other.e )
    \return
      - false only if coefficient and exponent values both match
    */
    inline bool operator!=(const SimpleTerm& other) const
    { return power != other.power; }

    struct {
      f4 coefficient; //! Leading coefficient for mutiplicity of the variable
      f4 exponent; //! Exponentiation of variable (name given on parent pNom)
    };

    struct {
      f4 coeff; //! Leading coefficient for mutiplicity of the variable
      f4 power; //! Exponentiation of variable (name given on the parent pNom)
    };

    struct {
      f4 c; //! Leading coefficient for mutiplicity of the variable
      f4 e; //! Exponentiation of variable (name given on the parent pNom)
    };

  }; // end SimpleTerm struct definition

  //! Container of a standard basis element (exp per var) & leading coefficient
  struct sBasisTerm
  {
    /** Initialize a standard basis variable product with an exponentiation set
    \brief
      - initialize a standard basis variable product with an exponentiation set
    \param lCoeff
      - leading coefficient (scalar multiple) shared across the polynomial term
    \param varExp
      - set of variable exponentiations: {0.5,3,0,1}{x,y,z,w} => x^{0.5}y^{3}w
    \param polyExp
      - shared exponentiation variable product has factored from all terms
    */
    sBasisTerm(f4 lCoeff = 0.0f, const vNf& varExp = {}, u4 polyExp = 1);

    /** Initialize a standard basis variable product with an exponentiation set
    \brief
      - initialize a standard basis variable product with an exponentiation set
    \param lCoeff
      - leading scalar coefficient shared by each variable in a polynomial term
    \param varExp
      - variable exponentiation set: {0.5,3,0,1}, {x,y,z,w} => x^{0.5}y^{3}w
    \param polyExp
      - shared exponentiation variable product has factored from all terms
    */
    sBasisTerm(f4 lCoeff, std::initializer_list<f4> varExp, u4 polyExp = 1);

    
    f4 coeff; //! Leading coefficient for mutiplicity of the variable product
    vNf exp;  //! Exponentiation per variable by index (pNom sets index label)

  }; // end sBasisTerm struct definition


  //! Container for a variable from a polynomial term (vector<{coeff, power}>)
  struct variable
  {
    /** Defaultable constructor for a variable in a term of a polynomial
    \brief
      - defaultable constructor for a variable in a term of a polynomial
    \details
      - can be specified per variable in a non-standard basis, meaning for each
      variable in a parent polynomial, that may have a term not simply in 3x or
      2x^{2}, but also 5(6 + x^{0.5} - 2x + 4x^{2})^{0.5}, which could be given
      in many variables that could form a product of each taking this form.
      each variable must only reference one 
    \param basis
      - list of <exponentiation, leading coefficient>, for the basis direction
    \param exponent
      - power the variable (the summary direction vector) is raised by
    */
    variable(const std::vector<SimpleTerm>& basis, u4 exponent = 1);
    
    /** Paired list constructor for a variable in a term of a polynomial
    \brief
      - paired list constructor for a variable in a term of a polynomial
    \param basis
      - list of <exponentiation, leading coefficient>, for the basis direction
    \param exponent
      - exponentiation of the basis (from shorthand if greater than 1)
    */
    variable(std::initializer_list<SimpleTerm> basis, u4 exponent = 1);
    
    /** Simple list constructor for a variable in a term of a polynomial
    \brief
      - simple list constructor for a variable in a term of a polynomial
    \details
      - if exponent list is not given, exponents will probe linearly up from 0
      if coefficient list is not specified, coefficients will all be 1
    \param coeff
      - standard basis coefficients values for variable's direction basis
    \param pow
      - standard basis exponent values for variable's direction basis
    \param exponent
      - power by which to raise the whole of the subterm by factored expansion
    */
    variable(std::initializer_list<f4> coeff, std::initializer_list<f4> pow,
      u4 exponent = 1);
    
    /** Simple list constructor for a variable in a term of a polynomial
    \brief
      - simple list constructor for a variable in a term of a polynomial
    \param termCount
      - elements with coeff 1 & linear incremental exponent comprising basis
    \param exponent
      - power by which to raise the whole of the subterm by factored expansion
    \param scalar
      - amount by which each term in the basis may be uniformly scaled
    */
    variable(u4 termCount = 1, CoeffPattern type = CoeffPattern::Constant,
      u4 exponent = 1, f4 scalar = 1.0f);

    /** Adjust variable basis term count with linear power increment from last
    \brief
      - adjust variable basis term count with linear power increment from last
    \param termCount
      - set variable basis to have this many terms (min 1) =: <coeff,++lastExp>
    \return
      - reference to the modified variable for subsequent inline access
    */
    inline variable& Resize(u4 termCount, f4 coeff = 1.0f, f4 power = NAN)
    {
      if (termCount == 0)
      {
        dir = { 1.0f, 0.0f };
      }
      else
      {
        dir.resize(termCount);
        um size = dir.size();
        if (std::isnan(power))
        {
          power = static_cast<f4>(FloorF(dir[size - 1].exponent));
          for (um i = size; i < termCount; ++i) { dir[0] = { coeff, ++power }; }
        }
        else 
        {
          for (um i = size; i < termCount; ++i) { dir[0] = { coeff, power }; }
        }
      }
      return *this;
    }

    /** Adjust variable basis term count with linear power increment from last
    \brief
      - adjust variable basis term count with linear power increment from last
    \param termCount
      - set variable basis to have this many terms (min 1) =: <coeff,++lastExp>
    \return
      - number of standard basis terms defined in the term's variable's basis
    */
    inline u4 Size(void) const { return static_cast<u4>(dir.size()); }

    /** Set operator to simplify variable basis direction to an exponentiation
    \brief
      - set operator to simplify variable basis direction to an exponentiation
    \param term
      - <1.0f, exponent> value to set as the variable's basis direction
    \return
      - reference to the modified variable for subsequent inline access
    */
    inline variable& operator=(f4 exponent)
    {
      dir.resize(1);
      dir[0] = {1.0f, exponent};
      return *this;
    }

    /** Set operator to simplify variable's basis direction to one term
    \brief
      - set operator to simplify variable's basis direction to one term
    \param term
      - <coefficient, exponent> pair to set as the variable's basis direction
    \return
      - reference to the modified variable for subsequent inline access
    */
    inline variable& operator=(const SimpleTerm& term)
    {
      dir.resize(1);
      dir[0] = term;
      return *this;
    }

    /** Set operator to make a variable's basis direction from a list
    \brief
      - set operator to make a variable's basis direction from a list
    \param term
      - <coefficient, exponent> list to set as the variable's basis direction
    \return
      - reference to the modified variable for subsequent inline access
    */
    inline variable& operator=(std::vector<SimpleTerm> term)
    {
      dir = term;
      return *this;
    }

    /** Set operator to make a variable's basis direction from a list
    \brief
      - set operator to make a variable's basis direction from a list
    \param term
      - <coefficient, exponent> list to set as the variable's basis direction
    \return
      - reference to the modified variable for subsequent inline access
    */
    inline variable& operator=(std::initializer_list<SimpleTerm> term)
    {
      dir = term;
      return *this;
    }

    /** Set operator to make a variable's basis direction from a list
    \brief
      - set operator to make a variable's basis direction from a list
    \param term
      - <coefficient, exponent> list to set as the variable's basis direction
    \return
      - reference to the modified variable for subsequent inline operations
    */
    inline variable& operator=(std::initializer_list<f4> coeffs)
    {
      dir.resize(coeffs.size());
      f4 exp = 0.0f;
      for (u4 i = 0; i < dir.size(); ++i)
      {
        dir[i] = { coeffs.begin()[i], ++exp };
      }
      return *this;
    }

    /** Subscript operator accessor to access basis direction elements
    \brief
      - subscript operator accessor to access basis direction elements
    \param subscript
      - offset to access the variable's basis <coeff, power> element term list
    \return
      - standard basis element from variable's basis term list
    */
    inline SimpleTerm& operator[](u4 subscript) { return dir[subscript]; }

    /** Subscript operator accessor to access basis direction elements
    \brief
      - subscript operator accessor to access basis direction elements
    \param subscript
      - offset to access the variable's basis <coeff, power> element term list
    \return
      - standard basis element from variable's basis term list
    */
    inline const SimpleTerm& operator[](u4 subscript) const
    {
      return dir[subscript];
    }

    /** Addition set operator to add another set of basis terms to these terms
    \brief
      - Addition set operator to add another set of basis terms to these terms
    \param other
      - variable basis terms to be added with this set of variable basis terms
    \return
      - reference to the modified variable for subsequent inline operations
    */
    variable& operator+=(const variable& other);

  private:

    /** Combine parallel coefficient & exponent lists into a tuple array
    \brief
      - combine parallel coefficient & exponent lists into a tuple array
    \details
      - if inequal length, default coeff =: 1.0, pow =: ++floor(pow[n-1])
      ie - from the last given value, increment from the next integer
    \param coeff
      - list of coefficients per variable sourcing equivalent subscripts
    \param pow
      - list of exponentiation per variable sourcing equivalent subscripts
    \return
      - list of interleaved values from parallel component lists
    */
    static std::vector<SimpleTerm> ToSTerms(std::initializer_list<f4> coeff,
      std::initializer_list<f4> pow);

    std::vector<SimpleTerm> dir; //! variable's lCoeff * variable^{power} list

  }; // end variable struct definition


  //! Term within a polynomial, each capable of referencing several variables
  struct pTerm
  {
    /** Defaultable constructor for a leading coefficient and basis of factors
    \brief
      - defaultable constructor for a leading coefficient and basis of factors
    \param lCoeff
      - leading coefficient: scalar multiplicity for the term with this basis
    \param factorList
      - standard basis tuple vectors of <coeff, exponent> sets per variable
    */
    pTerm(f4 lCoeff = 1.0f, const std::vector<variable>& factorLists = {});

    /** Make a pTerm from standard basis leading coefficient vector and factors
    \brief
      - make a pTerm from standard basis leading coefficient vector and factors
    \param lCoeffv
      - vector of coefficients associated as product with variable basis set
    \param factorList
      - standard basis tuple vectors of <coeff, exponent> sets per variable
    */
    pTerm(const vNf& lCoeffv, const std::vector<variable>& factorLists = {});

    /** Constructor for a standard basis leading coefficient vector and factors
    \brief
      - constructor for a standard basis leading coefficient vector and factors
    \param coeffs
      - vector of coefficients associated as product with variable basis set
    \param basis
      - standard basis tuple vectors of <coeff, exponent> sets per variable
    */
    pTerm(std::initializer_list<f4> coeffs, const std::vector<variable>& basis);

    /** List constructor for a leading coefficient and basis of factor literals
    \brief
      - list constructor for a leading coefficient and basis of factor literals
    \param lCoeff
      - leading coefficient: scalar multiplicity for the term with this basis
    \param factorList
      - standard basis tuple vectors of <coeff, exponent> sets per variable
      ie- (4.0, {{1,2},{-2,1},{8,0}},{{3,4},{4,1}}) => 4(x^{2}-2x+8)(3y^{4}+4y),
      or- (6.5, {{1,-3,7},{-4,1,2}}, {}) => 6.5(7x^{2}-3x+1)(2y^{2}+y-4),
      or- (2.1, {{3,Pascal},{4,Osccilating}}) => 2.1(x^{2}+2x+1)(y{3}-y{2}+y-1)
    */
    pTerm(f4 lCoeff, std::initializer_list<variable> factorLists);

    /** List constructor of leading coefficient vector & factor basis literals
    \brief
      - list constructor of leading coefficient vector & factor basis literals
    \param lCoeffv
      - vector of coefficients associated as product with variable basis set
    \param factorList
      - standard basis tuple vectors of <coeff, exponent> sets per variable
      ie- (4.0, {{1,2},{-2,1},{8,0}},{{3,4},{4,1}}) => 4(x^{2}-2x+8)(3y^{4}+4y),
      or- (6.5, {{1,-3,7},{-4,1,2}}, {}) => 6.5(7x^{2}-3x+1)(2y^{2}+y-4),
      or- (2.1, {{3,Pascal},{4,Osccilating}}) => 2.1(x^{2}+2x+1)(y{3}-y{2}+y-1)
    */
    pTerm(const std::vector<f4>& lCoeffv, std::initializer_list<variable>
      factorLists);

    /** Constructor using coefficient and a basis with only 1 term per variable
    \brief
      - constructor using coefficient and a basis with only 1 term per variable
    \param factorList
      - leading coefficient and exponent set per variable index for the term
    */
    pTerm(std::initializer_list<f4> factorList);

    /** Get the string representation for a term contained within a polynomial
    \brief
      - get the string representation for a term contained within a polynomial
    \details
      - if label list is omitted, default will assume series default values x_n
      for n = [0, m = (factor.size - label.size)], ie { x_0, x_1, ..., x_m-1 }
      while a longer list than factors given will simply ignore 0 value output
    \param label
      - array of labels for each variable in factor list in sequential order
    \param leads
      - polynomial leading term has compact / truncated +sign output (if true)
    \return
      - string output for typical mathematical notation of a polynomial term
    */
    std::string ToString(std::vector<std::string>& label, bool lead = false)
      const;

    /** Get the sum of two terms with like pNom variable subscript references
    \brief
      - get the sum of two terms with like pNom variable subscript references
    \details
      - if label list is omitted, default will assume series default values x_n
      for n = [0, m = (factor.size - label.size)], ie { x_0, x_1, ..., x_m-1 }
      while a longer list than factors given will simply ignore 0 value output
    \param label
      - array of labels for each variable in factor list in sequential order
    \param leads
      - polynomial leading term has compact / truncated +sign output (if true)
    \return
      - string output for typical mathematical notation of a polynomial term
    */
    inline pTerm operator+(const pTerm& other) const
    {
      pTerm result(*this);
      result.factor.resize(MaxUM(factor.size(), other.factor.size()));
      for (um i = 0; i < other.factor.size(); ++i)
      {
        result.factor[i] += other.factor[i];
      }
      return result;
    }

    vNf coeff; //! leading coefficient of the term

    std::vector<variable> factor; //! multiplicand series per term

    // pNom tracks specific variable identifier label applied per term index

  }; // end pTerm 

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                        Helper Operator Overloads                         */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  std::ostream& operator<<(std::ostream& o, const std::vector<SimpleTerm>& p);


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


} // end dp namespace
