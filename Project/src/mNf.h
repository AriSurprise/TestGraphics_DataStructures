/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  mNf.h
Purpose:  2D NxN, column major square matrix; inputs / outputs in row major
Details:  Fixed size containers are not adequate for all purposes, but the 2D
  storage of vector<vector> are likely not stored contiguously, so not ideal.
  This does make returning an arbitrary length specific vector from a contained
  offset into the data simpler to return, and overall simpler to get basic
  operation working however.  Call it a first pass: sufficient for the time
  being, until performance makes an issue of it.  A refactor would do well to
  not only address the 2D elements being stored contiguously, but also to
  enforce that each vector is in fact guaranteed to be of an equal length.
  Likely this would entail an NxM matrix class, for which NxN would merely be
  a derived type which specifically enforces not only equal rows among columns,
  but equal column count to row count as well.  This would enable elimination
  processes to either return one contiguous augmented matrix, or a struct
  containing the basis as the first N columns, followed by another member of
  the solution set of vectors.  The former is likely preferable, as it is more
  succinct to perform row operations upon, but will do well to add a getter for
  the first N columns, to extract the square matrix and be able to test if the
  elimination process solution was linearly independent, spanning, and/or onto.
  Because all data is public, the matrix is simply a container, mNf is a struct
  instead of a class: using lower case, as opposed to the traditional upper
  camel case for class names in significance of this distinction, denoting the
  nature of being a primitive container (with added convenience methods).
  Multidimensional array data highlights issues in element ordering decisions:
  because input and output is left to right, top to bottom, constructors taking
  raw values in row major is more convenient to enter / read, given row driven
  text editors. On the storage and math side, column vectors are more sensible,
  therefore vNf based constructors operate column major to facilitate this, and
  raw data will still feed to GPU in column major (as expected). Though it may
  be slightly convoluted to appear inconsistent, knowing the difference, this
  may yield the best of both worlds. Additionally, from a user experience side,
  this should extend naturally to each audience who will have a pre-disposition
  to more consistently use one or the other anyhow, so consistency may well
  still be experienced by most users only concerned with one or the other.
  Subscript operators are the gray area in that choice: it is likely to confuse
  those uninitiated in graphics to use the subscript operator and suddenly get
  column major output after entering row major data. However, abstracting the
  technical fact of the column major storage to that extent may well be more
  problematic than helpful (it is at least more disingenuous). If data is
  iterated over by looping increments, it should be consistent to the memory
  position sequencing for debugging purposes.  This also gives the user an
  opportunity to intentionally output in column major without transposing.
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_MAT300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Summer 2022
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#pragma once
/* Design notes:
 2D storage of vector<vector> likely not stored contiguously, so not ideal.
 Makes returning an arbitrary length specific vector from a contained offset
 into the data simpler to return, and overall simpler to get basic operation.
 Call it a first pass, for the time being, until performance makes it an issue.
*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <stl>
#include <vector>       // Constructor with series of existing column vectors
#include <initializer_list> // Preferable variadic element list construction
// "./src/..."
#include "ang.h"        // Rotation class to use to set default angles
#include "TypeErrata.h" // Platform primitive type aliases for easier porting
#include "vNf.h"       // Member requires exposed visibility to instantiate


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  class mNf;

  struct BasisShiftNf;

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  enum class Cells2D
  {
    RowMajor, ColMajor, ColumnMajor = ColMajor
  }; // end matrix namespace definition

  enum class Elim
  {
    GaussJordan, Gaussian = GaussJordan,
    RowReducedEchelonForm, RREF = RowReducedEchelonForm, Echelon = RREF
  };

  class mNf //! Square, 2 dimensional N by N, column major matrix
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set matrix from column vectors (if input ordered in col major, copied)
    \brief
      - set matrix from column vectors (if input ordered in col major, copied)
    \details
      - if non-uniform dimensions are given, each vector will extend with null
      values to reach the longest given vector. col major data ordering
      (the default), can be transposed from row major input data if overridden
    \param basis
      - list of uniform dimension vectors comprising matrix data
    \param order
      - basis data element ordering given in row major or col major (default)
    */
    mNf(const std::vector<vNf>& basis, Cells2D order = Cells2D::ColMajor);

    /** Set matrix from manual input, presumably row major (needing transpose)
    \brief
      - set matrix from manual input, presumably row major (needing transpose)
    \details
      - if non-uniform dimensions are given, each vector will extend with null
      values to reach the longest given vector. row major data ordering
      (the default), will be transposed to col major storage unless overridden
    \param data
      - list of N dimension lists of N dimension data (perfect square data #)
    \param order
      - basis data element ordering given in col major or row major (default)
    */
    mNf(std::initializer_list<std::initializer_list<f8>> data, Cells2D order
      = Cells2D::RowMajor);

    
    /** Create an N dimensional identity matrix: 1's in the trace, 0 elsewhere
    \brief
      - create an N dimensional identity matrix: 1's in the trace, 0 elsewhere
    \param dimens
      - number of rows or columns corresponding to square matrix term count
    \param scalar
      - scalar value to be used uniformly in each trace element (col == row)
    */
    mNf(u1 dimens = 2, f4 scalar = 1.0f);


    /** Create a copy of another matrix from copied row / column data values
    \brief
      - create a copy of another matrix from copied row / column data values
    \param source
      - existing matrix to have it's row / column values copied into the new one
    */
    mNf(const mNf& source);


    /** Create this matrix from a function result's row / column data values
    \brief
      - create this matrix from a function result's row / column data values
    \param result
      - matrix function return to have it's row / column values stolen by this
    */
    mNf(mNf&& result) noexcept;

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set to unscaled (by 1/det) inverse matrix; transposed cofactor matrix
    \brief
      - set to unscaled (by 1/det) inverse matrix; transposed cofactor matrix
    \details
      - transposed cofactor sometimes preferable for showing clean values
      passing determinant something, the resultant pair is equivalent to
      inverse if used together as (1 / determinant) * adjoint
    \param determinant
      - to avoid recalculation, pass an &f4 to set det() into (of this matrix)
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& ToAdjoint(f4* determinant = nullptr);


    /** Get inverse matrix without scale by 1/det: transposed cofactor matrix
    \brief
      - get inverse matrix without scale by 1/det: transposed cofactor matrix
    \details
      - transposed cofactor sometimes preferable for showing clean values
    \param determinant
      - to avoid recalculation, pass an &f4 to set det() into (of this matrix)
    \return
      - new matrix holding sign adjusted minor determinants in each subscript
    */
    mNf AdjointMat(f4* determinant = nullptr) const;


    /** Calculate and return minor determinant matrix with oscillating signs
    \brief
      - calculate and return minor determinant matrix with oscillating signs
    \details
      - alternating negated minor det-set can calculate several notable things
      Most immediately, it is needed to obtain the inverse matrix over cofactor
      expansion (instead of rref), as well as obtaining the greater determinant
    \param determinant
      - to avoid recalculation, pass in an &f4 to have set det() into (of this)
    \return
      - new matrix holding sign adjusted minor determinants in each subscript
    */
    mNf CofactorMat(f4* determinant = nullptr) const;


    /** Calculate and return the determinant of the matrix
    \brief
      - calculate and return the determinant of the matrix
    \details
      - determinant of the matrix describes the scale of the transformation
      ie 0 flattens space by at least 1 dimension, 1 changes perspective, not
      scale, while larger would make all points be more spaced out, -1 would
      be indicative of a reflection of space but not of different scale, etc
    \return
      - determinant of the existing matrix data
    */
    f4 Det(void) const;


    /** Get the N value / square dimensions of the NxN matrix data in use
    \brief
      - get the N value / square dimensions of the NxN matrix data in use
    \return
      - element count in rows or columns for the existing matrix data
    */
    u4 Dimens(void) const;


    /** Calculate and return the opposite transformation described by the matrix
    \brief
      - calculate and return the opposite transformation described by the matrix
    \details
      - calculates the matrix which undoes the transformations of this matrix
    \param determinant
      - allows user to pass in a calculated det to shortcut attempts iff 0.0f
    \return
      - inverse transformation matrix from present transformation description
    */
    mNf Inverse(f4* determinant = nullptr) const;


    /** Set this matrix to the inverse matrix of what it was set to
    \brief
      - set this matrix to the inverse matrix of what it was set to
    \details
      - sets this to the matrix which undoes the current transformations
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& Invert(void);


    /** True if the diagonal elements are 1, while all others are 0
    \brief
      - true if the diagonal elements are 1, while all others are 0
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - true if the diagonal elements are 1, while all others are 0
    */
    bool IsIdentity(void) const;


    /** True if all elements above the trace are 0 (all data is in lower half)
    \brief
      - true if all elements above the trace are 0 (all data is in lower half)
    \details
      - is lower triangular in that upper triangle contains no data
    \return
      - true only if all elements above the trace / diagonal are 0 value
    */
    bool IsLowerTriangular(void) const;


    /** True if the matrix is non-invertible
    \brief
      - true if the matrix is non-invertible
    \param determinant
      - if you have a precalculated determinant, get whether that is 0 instead
    \return
      - whether the matrix has determinant of 0
    */
    bool IsSingular(f4* determinant = nullptr) const;


    /** True if all elements below the trace are 0 (all data is in upper half)
    \brief
      - true if all elements below the trace are 0 (all data is in upper half)
    \details
      - is only upper triangular in that lower triangle contains no data
    \return
      - true only if all elements below the trace / diagonal are 0 value
    */
    bool IsUpperTriangular(void) const;


    /** JSON formatted string containing each member value in row major format
    \brief
      - JSON formatted string containing each member value in row major format
    \return
      - string containing packed tightly ascii copy of contained matrix data
    */
    std::string JSON(void) const;


    /** Get the matrix holding this matrix's data, exculding the given row / col
    \brief
      - calculate and return the determinant of the m3f without row / col
    \param row
      - row subscript in [0,3] range to disclude from determinant calculation
    \param col
      - colunm subscript from [0,3] to disclude from determinant calculation
    \return
      - determinant of the subset matrix discluding row and col
    */
    mNf Minor(u1 row, u1 col) const;


    /** Calculate and return the determinant of the m3f without row / col
    \brief
      - calculate and return the determinant of the m3f without row / col
    \param row
      - row subscript in [0,3] range to disclude from determinant calculation
    \param col
      - colunm subscript from [0,3] to disclude from determinant calculation
    \return
      - determinant of the subset matrix discluding row and col
    */
    f4 MinorDet(u1 row, u1 col) const;


    /** Calculate and return the matrix comprised of the minor determinants
    \brief
      - calculate and return the matrix comprised of the minor determinants
    \param determinant
      - pass in an f4 to hold det() (of former values) to prevent recalculation
    \return
      - string containing packed tightly ascii copy of contained matrix data
    */
    mNf MinorMat(f4* determinant = nullptr) const;


    /** Set the matrix column / row dimensions to a new N square element count
    \brief
      - set the matrix column / row dimensions to a new N square element count
    \details
      - lower N values will truncate data; higher N values will add 0 elements
    \param squareDimensions
      - element count square matrix should have per dimension (columns / rows)
    \param traceVal
      - value to place into diagonal element subscripts (column == row)
    \param elseVal
      - value to place into non-diagonal element subscripts (column != row)
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& Resize(u4 squareDimensions, f4 traceVal = 1.0f, f4 elseVal = 0.0f);


    /** Set a matrix column / row data to another source's column / row values
    \brief
      - set a matrix column / row data to another source's column / row values
    \param source
      - matrix to have its data members copied into this matrix
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& Set(const mNf& source);


    /** Perform row elimination on the (augmentable), matrix to get a solution
    \brief
      - perform row elimination on the (augmentable), matrix to get a solution
    \param aug
      - set of any N-D (matrix's dimension) vectors to solve with basis shift
    \return
      - this matrix used as a basis, copied & solved with augmented vector set
    */
    BasisShiftNf Solution(const std::vector<vNf>& aug = emptySet, Elim method
      = Elim::GaussJordan, f4* det = nullptr) const;


    /** JSON formatted string containing each member value in row major format
    \brief
      - JSON formatted string containing each member value in row major format
    \param lineBreaks
      - if enabled / true, rows will end with new lines (undesirable in serdes)
    \return
      - string containing packed tightly ascii copy of contained matrix data
    */
    std::string ToString(bool lineBreaks = false) const;


    /** Calculate and return sum of the diagonal elements
    \brief
      - calculate and return sum of the diagonal elements
    \param op
      - if changed to Oper::Product, may alternately return the product instead
    \return
      - sum (or product), of the diagonal elements
    */
    f4 Trace(Oper op = Oper::Sum) const;


    /** Set the matrix to have swapped contents for rows and columns
    \brief
      - set the matrix to have swapped contents for rows and columns
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& Transpose(void);


    /** Get a copy of this matrix with swapped contents for rows and columns
    \brief
      - get a copy of this matrix with swapped contents for rows and columns
    \return
      - copy of this matrix with swapped row and column data fields
    */
    mNf Transposed(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set a matrix column / row data to another source's column / row values
    \brief
      - set a matrix column / row data to another source's column / row values
    \param source
      - matrix to have its data members copied into this matrix
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& operator=(const mNf& source);


    /** Set this matrix from a function result's row / column data values
    \brief
      - set this matrix from a function result's row / column data values
    \param result
      - returned matrix to have it's row / column values stolen by this
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& operator=(mNf&& result) noexcept;


    /** Get a copy of the matrix with memberwise negated elements
    \brief
      - get a copy of the matrix with memberwise negated elements
    \return
      - copy of existing matrix data with negated values memberwise
    */
    mNf operator-(void) const;


    /** Get the inverse of the current matrix data set (reverse transform)
    \brief
      - get the inverse of the current matrix data set (reverse transform)
    \return
      - matrix undergoing the reverse change of basis from this matrix
    */
    mNf operator!(void) const;


    /** Get a column vNf from the matrix's column values ([0,(N-1)])
    \brief
      - get a column vNf from the matrix's column values ([0,(N-1)])
    \param subscript
      - value in [0,(N-1)] range to access that ordered column vector
    \return
      - reference to the column vector subscript ([0,(N-1)])
    */
    const vNf& operator[](u4 subscript) const;


    /** Get a column vNf from the matrix's column values ([0,(N-1)])
    \brief
      - get a column vNf from the matrix's column values ([0,(N-1)])
    \param subscript
      - value in [0,(N-1)] range to access that ordered column vector
    \return
      - reference to the column vector subscript ([0,(N-1)])
    */
    vNf& operator[](u4 subscript);


    /** Calculate the member-wise addition of 2 matrices, returning the sum
    \brief
      - calculate the member-wise addition of 2 matrices, returning the sum
    \details
      - to allow ubiquitous function, lower dimensional matrices will be
      promoted to fit, while higher dimensional matrices will promote this
      matrix with null values, setting the memberwise results with those 0s
      (to return the overall, larger data set without truncating data).
      In keeping with mathematical standards that disallow most divergent
      dimensional matrix math, warnings will be logged accordingly.
    \param other
      - matrix to be added to this matrix's row / column data, member-wise
    \return
      - copy of this matrix with each entry added, member-wise, by other
    */
    mNf operator+(const mNf& other) const;


    /** Add an other matrix's values member-wise, setting the sum into this
    \brief
      - add an other matrix's values member-wise, setting the sum into this
    \details
      - to allow ubiquitous function, lower dimensional matrices will be
      promoted to fit, while higher dimensional matrices will promote this
      matrix with null values, setting the memberwise results with those 0s
      (to return the overall, larger data set without truncating data).
      In keeping with mathematical standards that disallow most divergent
      dimensional matrix math, warnings will be logged accordingly.
    \param other
      - matrix to be added to this matrix's row / column data, member-wise
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& operator+=(const mNf& other);


    /** Get member-wise subtraction of 2 matrices, returning the difference
    \brief
      - get member-wise subtraction of 2 matrices, returning the difference
    \details
      - to allow ubiquitous function, lower dimensional matrices will be
      promoted to fit, while higher dimensional matrices will promote this
      matrix with null values, setting the memberwise results with those 0s
      (to return the overall, larger data set without truncating data).
      In keeping with mathematical standards that disallow most divergent
      dimensional matrix math, warnings will be logged accordingly.
    \param other
      - matrix to be subtracted from this matrix's data, member-wise
    \return
      - copy of this matrix with each entry subtracted, member-wise, by other
    */
    mNf operator-(const mNf& other) const;


    /** Subtract an other matrix's values member-wise, setting the difference
    \brief
      - subtract an other matrix's values member-wise, setting the difference
    \details
      - to allow ubiquitous function, lower dimensional matrices will be
      promoted to fit, while higher dimensional matrices will promote this
      matrix with null values, setting the memberwise results with those 0s
      (to return the overall, larger data set without truncating data).
      In keeping with mathematical standards that disallow most divergent
      dimensional matrix math, warnings will be logged accordingly.
    \param other
      - matrix to be subtracted from this matrix's data, member-wise
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& operator-=(const mNf& other);


    /** Scalar multiplication multiplies each member by scalar constant
    \brief
      - scalar multiplication multiplies each member by scalar constant
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - copy of this matrix's data, scaled memberwise by scalar
    */
    mNf operator*(f4 scalar) const;


    /** Get the concatenated matrix of (this * other): both transformations
    \brief
      - get the concatenated matrix of (this * other): both transformations
    \details
      - to allow ubiquitous function, lower dimensional vectors will be
      promoted to fit, while higher dimensional vectors will promote this
      matrix with null values, setting the memberwise results with those 0s
      (to return the overall, larger data set without truncating data).
      In keeping with mathematical standards that disallow most divergent
      dimensional matrix math, warnings will be logged accordingly.
    \param other
      - matrix to be right concatenated with this matrix
    \return
      - copy of this matrix, concatenated with the other
    */
    vNf operator*(const vNf& vector) const;


    /** Get the concatenated matrix of (this * other): both transformations
    \brief
      - get the concatenated matrix of (this * other): both transformations
    \details
      - to allow ubiquitous function, lower dimensional matrices will be
      promoted to fit, while higher dimensional matrices will promote this
      matrix with null values, setting the memberwise results with those 0s
      (to return the overall, larger data set without truncating data).
      In keeping with mathematical standards that disallow most divergent
      dimensional matrix math, warnings will be logged accordingly.
    \param other
      - matrix to be right concatenated with this matrix
    \return
      - copy of this matrix, concatenated with the other
    */
    mNf operator*(const mNf& other) const;


    /** Scale each matrix element by scalar, setting the product into this
    \brief
      - scale each matrix element by scalar, setting the product into this
    \details
      - to allow ubiquitous function, lower dimensional matrices will be
      promoted to fit, while higher dimensional matrices will promote this
      matrix with null values, setting the memberwise results with those 0s
      (to return the overall, larger data set without truncating data).
      In keeping with mathematical standards that disallow most divergent
      dimensional matrix math, warnings will be logged accordingly.
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& operator*=(f4 scalar);


    /** Matrix multiplication concatenates 2 sequential transformations into 1
    \brief
      - matrix multiplication concatenates 2 sequential transformations into 1
    \details
      - to allow ubiquitous function, lower dimensional matrices will be
      promoted to fit, while higher dimensional matrices will promote this
      matrix with null values, setting the memberwise results with those 0s
      (to return the overall, larger data set without truncating data).
      In keeping with mathematical standards that disallow most divergent
      dimensional matrix math, warnings will be logged accordingly.
    \param other
      - multiplicand to use with this (cell = this[row] * other[col])
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& operator*=(const mNf& other);


    /** Perform division on all members, returning the result
    \brief
      - perform division on all members, returning the result
    \param inverseScalar
      - scalar amount by which to multiply the reciprocal of per matrix element
    \return
      - copy of this matrix, scaled up by (1.0 / inverseScalar)
    */
    mNf operator/(f4 inverseScalar) const;


    /** Division multiplies all members by reciprocal of inverseScalar
    \brief
      - division multiplies all members by reciprocal of inverseScalar
    \param inverseScalar
      - scalar amount by which to multiply the reciprocal of per matrix element
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& operator/=(f4 inverseScalar);


    /** Get string of data in matrix cells output as row major data per line
    \brief
      - get string of data in matrix cells output as row major data per line
    \return
      - multi-lined string representation of matrix data in row major output
    */
    operator std::string(void) const;



    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Calculate and set matrix to minor det matrix with oscillating signs
    \brief
      - calculate and set matrix to minor det matrix with oscillating signs
    \details
      - intermediary inverse step, setting determinant relevant to prior matrix
      Just doesn't make sense to have the set in place step, pre-mutation as
      publicly available since it only useful in adjoint / inverse matrix
      calculations, and privatizing simplifies the interface so the user
      wanting the adjoint doesn't have to remember nomenclature distinctions
      between cofactor and adjoint difference minutiae (which one has the signs
      alternately inverted from the minor determinant matrix? the cofactor.
      which one is scaled by the reciprocal of the determinant? the adjoint).
      Musch simpler for the user experience to just offer the streamlined
      output options to mitigate error: move it to public if you have reason.
    \param determinant
      - to avoid recalculation, pass in an f4 to store det() (for former values)
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    mNf& ToCofactor(f4* determinant = nullptr);



    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Column major lists of vectors comprising the basis
    std::vector<vNf> cell;

  }; // end mNf struct definitions



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                     Helper Constant Declarations                         */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Zero element / dimensional matrix; reserved 1D value for an invalid state
  static const dp::mNf nullmat(0);


  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Class Declarations                          */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Container for an N dimensional basis of column vectors with shared
    dimensional terms per row, and a set of any number of N dimensional column
    vectors which stand to represent
  */
  struct BasisShiftNf
  {
  public:

    /** Create an augmented matrix from a given basis set and optional vectors
    \brief
      - create an augmented matrix from a given basis set and optional vectors
    \details
      - design ensures the longer dimension of NxM matrix must be M >= N.
      augment could be any number of vectors at a time, including none if
      relevant, as possible in calculating the matrix determinant on its own.
      while M < N columns could be processed, it's generally less useful.
    \param basisSet
      - set of like termed vectors per row describing the problem space
    \param vectorSet
      - set of like termed vectors per row describing the equation target
    \return
      - reference to the modified matrix for easier subsequent inline action
    */
    BasisShiftNf(const mNf& basisSet, const std::vector<vNf>& vectorSet
      = emptySet);

    /** Get the number of columns contained in the augmented matrix portion
    \brief
      - get the number of columns contained in the augmented matrix portion
    \return
      - column count in the summary matrix in excess of N square dimensions
    */
    u4 AugmentDimens(void) const;

    /** Get total number of columns contained in the basis + augmented matrix
    \brief
      - get total number of columns contained in the basis + augmented matrix
    \return
      - longer dimension of matrix element count; total number of columns
    */
    u4 Columns(void) const;

    /** Get the number of rows / columns contained in the square basis matrix
    \brief
      - get the number of rows / columns contained in the square basis matrix
    \return
      - shorter dimension of matrix element count; total number of rows
    */
    u4 SquareDimens(void) const;

    /** Operate upon 2 matrix base + augment rows to swap elements per column
    \brief
      - operate upon 2 matrix base + augment rows to swap elements per column
    \details
      - operation negates basis determinant from the prior state's value
    \param swap
      - first matrix row subscript [0,N) to be swapped with another given one
    \param with
      - second matrix row subscript [0,N) to be swapped with the former
    \return
      - reference to the modified matrix augment for easier inline operations
    */
    BasisShiftNf& SwapRows(u4 swap, u4 with);

    /** Operate upon a matrix base + augment row to scale elements per column
    \brief
      - operate upon a matrix base + augment row to scale elements per column
    \details
      - operation changes basis determinant by multiplying it by the scalar
      ie, resultant basis state's determinant =: (scalar) * det(prior basis)
    \param scalar
      - value for which elements of the chosen row / dimen is to be multiplied
    \param row
      - matrix row subscript [0,N) to have the base + augment elements scaled
    \return
      - reference to the modified matrix augment for easier inline operations
    */
    BasisShiftNf& ScaleRow(f4 scalar, u4 row);

    /** Operate on a matrix base + augment row to add a scaled row into another
    \brief
      - operate on a matrix base + augment row to add a scaled row into another
    \details
      - operation will not have an effect on the basis matrix determinant
      unless from scaled row being added into the same row (net effect scalar).
      in that case, (scalar + 1.0) of that row is the product, determinant
      net change for the exceptional case then being times (scalar + 1.0)
    \param scalar
      - value which elements of the chosen from row copy is to be multiplied
    \param from
      - matrix row index [0,N) to reference in scaling and adding into another
    \param into
      - matrix row index [0,N) to have the scaled row from added into this row
    \return
      - reference to the modified matrix augment for easier inline operations
    */
    BasisShiftNf& AddRow(f4 scalar, u4 from, u4 into);

    //! NxN square dimension matrix to use in calculating pivot values
    mNf basis;

    //! Set of N-D additional vectors; other half of any equations in reference
    std::vector<vNf> augment;

  }; // end BasisShiftNf class definition


  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                     Helper Function Declarations                         */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                     Helper Operator Declarations                         */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Scale each matrix element by scalar, setting the product into this
  \brief
    - scale each matrix element by scalar, setting the product into this
  \param scalar
    - scalar amount by which to multiply each matrix element
  \param rhs
    - matrix to be scaled member-wise by the scalar amount
  \return
    - copy of the rhs matrix scaled member-wise by scalar
  */
  mNf operator*(f4 scalar, const mNf& rhs);


  /** Confirm if 2 matrix's are relatively equal across all matrix elements
  \brief
    - confirm if 2 matrix's are relatively equal across all matrix elements
  \param lhs
    - left-hand side to be compared for equality against the right-hand side
  \param rhs
    - right-hand side to be compared for equality against the left-hand side
  \return
    - true if all matrix elements are equal, false if any are not
  */
  bool operator==(const mNf& lhs, const mNf& rhs);


  /** Confirm if 2 matrix's are relatively inequal in any matrix element
  \brief
    - confirm if 2 matrix's are relatively inequal in any matrix element
  \param lhs
    - left-hand side to be compared for inequality against the right-hand side
  \param rhs
    - right-hand side to be compared for inequality against the left-hand side
  \return
    - true if any matrix elements is inequal, false if none are
  */
  bool operator!=(const mNf& lhs, const mNf& rhs);

  
  /** Output the string version of the vector data: <vector>, (point) or [clip]
  \brief
    - output the string version of the vector data: <vector>, (point) or [clip]
  \param output
    - output stream buffer to have the matrix data fed into it for display
  \param matrix
    - right-hand side to display in string representation of data contained
  \return
    - reference to the modified output stream buffer object
  */
  std::ostream& operator<<(std::ostream& output, const mNf& matrix);

} // end dp namespace
