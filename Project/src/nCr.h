/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File:     nCr.h
Purpose:  Use dynamic programming table to get pascal values (n<=101; 0 < k < n)
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no platform requirements)
Project:  a.Surprise_MAT300_1
Author:   Ari Surprise / a.surprise / 40000218
Creation date:  Summer 2022
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#pragma once
/* Notes on the relatively low maximum of n value (101), in pick n choose k:
  C standards do not define a primitive for values wider than 8-bytes.  While
  user defined classes for larger numbers could be defined, it is overscoped
  within the constraints of the needs of this project.  Consequentially, the
  8-byte value cap is the constraint for n values above 101, as the highest
  values within nCr(102, 51), exceed the 8-byte value cap, making it better to
  simply uniformly not provide, indicating the limit and failure.
  Although it could be possible to dynamically calculate answers from serial
  definitions of nCr(n,k) = n!/ (k!(n-k)!), the result wouldn't be able to go
  beyond the bounds of a 8-byte (64-bit) value either, so the limits of the
  table's storable, cached values are the limit of output as well.
  Additionally, factorial would likley be another recursive definition anyhow,
  and while useful to have in itself after a similar fashion as performed here,
  n! also goes beyond 18,446,744,073,709,551,615 (18.4 quintillion), quickly,
  and at lower n values still than the nCr definitions.
  If larger big number classes were to be explored, a 16 byte datum could allow
  for values as high as 340,282,366,920,938,463,463,374,607,431,768,211,456
  (the 128-bit unsigned value cap: mind that's 340.28 undecillion), raising
  that limit on n somewhat, but it introduces dependencies regarding
  enforcing how to implement big numbers as well, which isn't always desirable,
  making stopping at the 8-byte bottleneck a good place for forking a branch.
  It could be preferable to have discrete datum value types of 16, 32, 64 byte
  intervals with guaranteed uniform fixed size for applications requiring
  expedient iteration over many large values, while others may want a
  serializable, more infinitely big number class that simply grows an array as
  needed to store larger numbers for applications needing a catch all solution.
  Either should be able to be translated between one and the other however, so
  it makes more sense to implement both and use the one that benefits the
  specific application, in this case the catch all being more helpful, then
  requiring the user needing fixed size numbers to validate the value being
  within bounds before using it (that being fairly par for the course and
  reasonable to expect of developers)
*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <stl>
#include <vector>       // Internal storage of n choice combination values list
// "./src/..."
#include "pTerm.h"      // Bernstein basis elements using SimpleTerm struct
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Dynamic programming table for caching Pascal values: (Choose n Pick k)
      using static accessors to read from / generate values (n<=101) as needed
  */
  class n
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Public Static Methods                           */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get the standard basis values for B[d][i] =: nCr(d,i)(1-t)^{d-i}t^{i}
    \brief
      - get the standard basis values for B[d][i] =: nCr(d,i)(1-t)^{d-i}t^{i}
    \details
      - non-constant to allow for calculation of d values on demand
    \param d
      - dimension / polynomial complexity of Bernstein basis elements to use
    \param i
      - [0,d] element offset within Bernstein basis for the given dimension
    \return
      - reference to the i'th expanded Bernstein basis element for dimension d
    */
    static const std::vector<SimpleTerm>& Bernstein(u1 d, u1 i);

    /** Get all [0,d] elements in B[d][i] =: nCr(d,i)(1-t)^{d-i}t^{i} (each i)
    \brief
      - get all [0,d] elements in B[d][i] =: nCr(d,i)(1-t)^{d-i}t^{i} (each i)
    \details
      - non-constant to allow for calculation of d values on demand
    \param d
      - dimension / polynomial complexity of Bernstein basis elements to use
    \return
      - reference to the number of ways to pick k combinations among n choices
    */
    static const std::vector<std::vector<SimpleTerm>>& BernsteinBasis(u1 d);

    /** Get the value of number of ways to pick k combinations among n choices
    \brief
      - get the value of number of ways to pick k combinations among n choices
    \details
      - non-constant to allow for calculation of large n values on demand
    \param n
      - desired number of choices available in pool to pick  from in selection
    \param k
      - desired number of candidates to pick from the selection pool of n
    \return
      - reference to the number of ways to pick k combinations among n choices
    */
    static const u8& Cr(u1 n, u1 k);


    /** Get values for all ways to pick each of k combinations among n choices
    \brief
      - get values for all ways to pick each of k combinations among 2 choices
    \details
      - non-constant to allow for calculation of large n values on demand
    \param n
      - total ways to choose k items (per subscript) out of the given n items
    \return
      - row of all Pascal values for ways to pick k combination among n choices
    */
    static std::vector<u8> Choices(u1 n);


    /** Set internal list to precalculate any of up to n of n possible choices
    \brief
      - set internal list to precalculate any of up to n of n possible choices
    \details
      - avoid sudden spikes in calculation on demand by precalculating up front
    \param n
      - desired number of choices available in pool to pick from in a selection
    */
    static void Init(u1 n);


    /** Get values for ways to pick each of k combinations up to n/2 choices
    \brief
      - get values for ways to pick each of k combinations up to n/2 choices
    \details
      - non-constant to allow for calculation of large n values on demand
    \param n
      - total ways to choose k items (per subscript) out of the given n items
    \return
      - reference to the row of all ways to pick k combinations among n choices
    */
    static const std::vector<u8>& Row(u1 n);


  private:

    /** Set the value of number of ways to pick k combinations among n choices
    \brief
      - set the value of number of ways to pick k combinations among n choices
    \details
      - from recursive definition: assuming (n-1, k-1) + (n-1, k) are available
    \param n
      - number of items which can be picked within the given row
    \param k
      - number of items out of n choices being selected
    */
    static u8 Calculate(u1 n, u1 k);


    /** Set all values for Cumulative Bernstein basis element of dimension d
    \brief
      - set all values for Cumulative Bernstein basis element of dimension d
    \param d
      - sets all Cumulative Bernstein basis elements for the given (d)imension
    */
    static void CalculateC(u1 d);


    /** Set all values for each Bernstein basis element of a given dimension
    \brief
      - set all values for each Bernstein basis element of a given dimension
    \param d
      - sets all Bernstein polynomial basis elements for the given (d)imension
    */
    static void CalculateD(u1 d);


    /** Set all values for of ways to pick each of k combination from n choices
    \brief
      - set all values for of ways to pick each of k combination from n choices
    \details
      - from recursive definition: assuming (n-1) is fully available
    \param n
      - number of items which can be picked within the given row
    */
    static void CalculateN(u1 n);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                       Private Static Members                           */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  private:

    //! Precalculated list of lists of n possible choices to be taken k ways
    static std::vector<std::vector<u8>> choose;

    //! Precalculated list of lists n::Cr(d,i)*(1-t)^{d-1}*t^{i} per i per d
    static std::vector<std::vector<std::vector<SimpleTerm>>> B;

    //! Precalculated list Bernstein basis element, sums [i,d] per i per d
    static std::vector<std::vector<std::vector<SimpleTerm>>> C;

    //! Return value for any invalid selection of choose n pick k inputs
    static const u8 invalid = 0ull;

    //! Return value for any invalid selection of choose n pick k inputs
    static const std::vector<SimpleTerm> nullBasis;


  }; // end n class declaration


} // end dp namespace
