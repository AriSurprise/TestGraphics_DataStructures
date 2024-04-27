/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  m2f.h
Purpose:  2D 2x2, column major matrix; inputs / outputs in row major
Details:  Fixed size containers guaranteeing both contiguous storage and
  discrete size and type of matrix elements is preferable for expedient
  iteration regularity of cached elements, over the more flexible templating of
  types or variadic element counts, as those would not be able to be iterated
  over with simple memory offsets or loaded in chunks of data sets at a time.
  Because all data is public, the matrix is simply a container, m4f is a struct
  instead of a class: using lower case, as opposed to the traditional upper
  camel case for class names in significance of this distinction, denoting the
  nature of being a primitive container (with added convenience methods).
  Multidimensional array data highlights issues in element ordering decisions:
  because input and output is left to right, top to bottom, constructors taking
  raw values in row major is more convenient to enter / read, given row driven
  text editors. On the storage and math side, column vectors are more sensible,
  therefore v2f based constructors operate column major to facilitate this, and
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

// "./src/..."
#include "ang.h"        // Rotation class to use to set default angles
#include "TypeErrata.h" // Platform primitive type aliases for easier porting
#include "v2f.h"       // Member requires exposed visibility to instantiate


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class m2f //! Square 2 dimensional 2 by 2, column major matrix
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    static const u1 SQUBS = 2u; //! Square matrix has [2] vec[2] cols of f4s

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set matrix from 4 column vectors (col major ordered input -> result)
    \brief
      - set matrix from 4 column vectors (col major ordered input -> result)
    \param U
      - 1st column of homogeneous transformation basis: U-axis (local x-axis)
    \param V
      - 2nd column of homogeneous transformation basis: V-axis (local y-axis)
    \param N
      - 3rd column of homogeneous transformation basis: N-axis (local z-axis)
    \param T
      - 4th column of homogeneous transformation basis: (T)ranslation vector
    */
    m2f(const v2f& U, const v2f& V);

    /** Set from 4x4 matrix setting all members manually (row major ordered)
    \brief
      - set from 4x4 matrix setting all members manually (row major ordered)
    \param r1c1
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param r1c2
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param r2c1
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param r2c2
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    */
    m2f(f4 r1c1, f4 r1c2,
         f4 r2c1, f4 r2c2);


    /** Set lazily from 4x4 matrix's 3D linear members with integer values
    \brief
      - set lazily from 4x4 matrix's 3D linear members with integer values
    \param ux
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param vx
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param uy
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param vy
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    */
    m2f(s4 ux, s4 vx,
         s4 uy, s4 vy);


    /** Set lazily from 4x4 matrix's 3D linear members with raw literal values
    \brief
      - set lazily from 4x4 matrix's 3D linear members with raw literal values
    \param ux
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param vx
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param uy
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param vy
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    */
    m2f(f8 ux, f8 vx,
         f8 uy, f8 vy);

    
    /** Create default (identity), I2 matrix: 1's along the trace, 0 elsewhere
    \brief
      - create default (identity), I2 matrix: 1's along the trace, 0 elsewhere
    */
    m2f(void);


    /** Create a copy of another m4f from copied row / column data values
    \brief
      - create a copy of another m4f from copied row / column data values
    \param source
      - existing m2f to have it's row / column values copied into the new m2f
    */
    m2f(const m2f& source);


    /** Create this m2f from a function result's row / column data values
    \brief
      - create this m2f from a function result's row / column data values
    \param result
      - m2f function return to have it's row / column values stolen by this
    */
    m2f(m2f&& result) noexcept;

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

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


    /** Calculate and return the opposite transformation described by the m4f
    \brief
      - calculate and return the opposite transformation described by the m4f
    \details
      - calculates the m4f which undoes the transformations of the current m4f
    \return
      - inverse transformation m2f from present transformation description
    */
    m2f Inverse(void) const;


    /** Set this matrix to the inverse matrix of what it was set to
    \brief
      - set this matrix to the inverse matrix of what it was set to
    \details
      - sets this to the m4f which undoes the current transformations
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m2f& Invert(void);


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


    /** True if linear determinant is -1 (scale inverted)
    \brief
      - true if determinant is 0, trace is less than dimensions but non 0
    \return
      - true only if determinant is 0, trace is less than dimensions but non 0
    */
    //bool IsReflection(void) const;


    /** True if linear determinant is 1 (scale preserved), trace 2*cos(t)
    \brief
      - true if linear determinant is 1 (scale preserved), trace 2*cos(t)
    \return
      - true only if linear determinant is 1 (scale preserved), trace 2*cos(t)
    */
    //bool IsRotation(void) const;
    
    
    /** True if linear elements not on the trace are 0
    \brief
      - true if linear elements not on the trace are 0
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - true if only trace is non-zero
    */
    bool IsScalar(void) const;


    /** True if the matrix is non-invertible
    \brief
      - true if the matrix is non-invertible
    \param determinant
      - if you have a precalculated determinant, get whether that is 0 instead
    \return
      - whether the matrix has determinant of 0
    */
    bool IsSingular(f4* determinant = nullptr) const;


    /** True when the transpose is equal to negative of the original elements
    \brief
      - true when the transpose is equal to negative of the original elements
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - if the trace is 0 and elements across the diagonal are negative equals
    */
    bool IsSkewSymmetric(void) const;


    /** True if the transpose is equal to the original matrix
    \brief
      - true if the transpose is equal to the original matrix
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - if every element matches the one across it along the trace
    */
    bool IsSymmetrical(void) const;


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
    std::string Json(void) const;


    /** Set a m2f column / row data to another source's column / row values
    \brief
      - set a m2f column / row data to another source's column / row values
    \param source
      - matrix to have its data members copied into this matrix
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m2f& Set(const m2f& source);


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
    \param product
      - if enabled, alternately returns the product instead of sum
    \return
      - sum (or product), of the diagonal elements
    */
    f4 Trace(bool product = false) const;


    /** Set the matrix to have swapped contents for rows and columns
    \brief
      - set the matrix to have swapped contents for rows and columns
    \return
      - reference to the modified m2f for easier subsequent inline action
    */
    m2f& Transpose(void);


    /** Get a copy of this m2f with swapped contents for rows and columns
    \brief
      - get a copy of this m2f with swapped contents for rows and columns
    \return
      - copy of this m2f with swapped row and column data fields
    */
    m2f Transposed(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Set a m2f column / row data to another source's column / row values
    \brief
      - set a m2f column / row data to another source's column / row values
    \param source
      - matrix to have its data members copied into this matrix
    \return
      - reference to the modified m2f for easier subsequent inline action
    */
    m2f& operator=(const m2f& source);


    /** Set this m2f from a function result's row / column data values
    \brief
      - set this m2f from a function result's row / column data values
    \param result
      - returned m2f to have it's row / column values stolen by this
    \return
      - reference to the modified m2f for easier subsequent inline action
    */
    m2f& operator=(m2f&& result) noexcept;

    /** Get a copy of the matrix with memberwise negated elements
    \brief
      - get a copy of the matrix with memberwise negated elements
    \return
      - inverse matrix which multiplies to undo the transformations in this
    */
    m2f operator-(void) const;


    /** Get the inverse of the current matrix data set (reverse transform)
    \brief
      - get the inverse of the current matrix data set (reverse transform)
    \return
      - reference to the modified m2f for easier subsequent inline action
    */
    m2f operator!(void) const;


    /** Get a column v2f from the m2f's col[0-3] values: <0:u, 1:v, 2:n, 3:t>
    \brief
      - get a column v2f from the m2f's col[0-3] values: <0:u, 1:v, 2:n, 3:t>
    \param subscript
      - value in [0,3] range to access the corresponding ordered column vector
    \return
      - reference to the column vector subscript: <0:u, 1:v, 2:n, 3:t>
    */
    const v2f& operator[](u4 subscript) const;


    /** Get a column v2f from the m2f's col[0-3] values: <0:u, 1:v, 2:n, 3:t>
    \brief
      - get a column v2f from the m2f's col[0-3] values: <0:u, 1:v, 2:n, 3:t>
    \param subscript
      - value in [0,3] range to access the corresponding ordered column vector
    \return
      - reference to the column vector subscript: <0:u, 1:v, 2:n, 3:t>
    */
    v2f& operator[](u4 subscript);


    /** Calculate the member-wise addition of 2 m2f's, returning the sum
    \brief
      - calculate the member-wise addition of 2 m2f's, returning the sum
    \param other
      - m2f to be added to this matrix's row / column data, member-wise
    \return
      - copy of this m2f with each entry added, member-wise, by other
    */
    m2f operator+(const m2f& other) const;


    /** Add an other m2f's values member-wise, setting the sum into this
    \brief
      - add an other m2f's values member-wise, setting the sum into this
    \param other
      - m2f to be added to this matrix's row / column data, member-wise
    \return
      - reference to the modified m2f for easier subsequent inline action
    */
    m2f& operator+=(const m2f& other);


    /** Calculate member-wise subtraction of 2 m2f's, returning the difference
    \brief
      - calculate member-wise subtraction of 2 m2f's, returning the difference
    \param other
      - m2f to be subtracted from this matrix's row / column data, member-wise
    \return
      - copy of this m2f with each entry subtracted, member-wise, by other
    */
    m2f operator-(const m2f& other) const;


    /** Subtract an other m2f's values member-wise, setting the difference
    \brief
      - subtract an other m2f's values member-wise, setting the difference
    \param other
      - m2f to be subtracted from this matrix's row / column data, member-wise
    \return
      - reference to the modified m2f for easier subsequent inline action
    */
    m2f& operator-=(const m2f& other);


    /** Scalar multiplication multiplies each member by scalar constant
    \brief
      - scalar multiplication multiplies each member by scalar constant
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - memberwise copy of this m2f, with each member multiplied by the scalar
    */
    m2f operator*(f4 scalar) const;


    /** Get the concatenated matrix of (this * other): both transformations
    \brief
      - get the concatenated matrix of (this * other): both transformations
    \param other
      - m2f to be right concatenated with this matrix
    \return
      - memberwise copy of this m2f, concatenated with the other
    */
    v2f operator*(const v2f& vector) const;


    /** Get the concatenated matrix of (this * other): both transformations
    \brief
      - get the concatenated matrix of (this * other): both transformations
    \param other
      - m2f to be right concatenated with this matrix
    \return
      - copy of this m2f, concatenated with the other
    */
    m2f operator*(const m2f& other) const;


    /** Scale each matrix element by scalar, setting the product into this
    \brief
      - scale each matrix element by scalar, setting the product into this
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - reference to the modified m2f for easier subsequent inline action
    */
    m2f& operator*=(f4 scalar);


    /** Matrix multiplication concatenates 2 sequential transformations into 1
    \brief
      - matrix multiplication concatenates 2 sequential transformations into 1
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - reference to the modified m2f for easier subsequent inline action
    */
    m2f& operator*=(const m2f& other);


    /** Perform division on all members, returning the result
    \brief
      - perform division on all members, returning the result
    \param inverseScalar
      - scalar amount by which to multiply the reciprocal of per matrix element
    \return
      - copy of this m2f, scaled up by (1.0 / inverseScalar)
    */
    m2f operator/(f4 inverseScalar) const;


    /** Division multiplies all members by reciprocal of inverseScalar
    \brief
      - division multiplies all members by reciprocal of inverseScalar
    \param inverseScalar
      - scalar amount by which to multiply the reciprocal of per matrix element
    \return
      - reference to the modified m2f for easier subsequent inline action
    */
    m2f& operator/=(f4 inverseScalar);


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

    //! Row column major lists of <u,v> vectors of the 2D transformation
    v2f col[SQUBS];

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  }; // end m2f struct / class declaration



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Constant Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Zero filled m2f in all entries; reserved value for an invalid state
  static const m2f nullm2f(0.0f, 0.0f, 0.0f, 0.0f);



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Operator Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Scale each matrix element by scalar, returning the product with rhs
  \brief
    - scale each matrix element by scalar, returning the product with rhs
  \param scalar
    - scalar amount by which to multiply each matrix element
  \param rhs
    - matrix to be scaled member-wise by the scalar amount
  \return
    - copy of the rhs m2f scaled member-wise by scalar
  */
  m2f operator*(f4 scalar, const m2f& rhs);


  /** Confirm if 2 m2f's are relatively equal across all matrix elements
  \brief
    - confirm if 2 m2f's are relatively equal across all matrix elements
  \param lhs
    - left-hand side to be compared for equality against the right-hand side
  \param rhs
    - right-hand side to be compared for equality against the left-hand side
  \return
    - true if all matrix elements are equal, false if any are not
  */
  bool operator==(const m2f& lhs, const m2f& rhs);


  /** Confirm if 2 m2f's are relatively inequal in any matrix element
  \brief
    - confirm if 2 m2f's are relatively inequal in any matrix element
  \param lhs
    - left-hand side to be compared for inequality against the right-hand side
  \param rhs
    - right-hand side to be compared for inequality against the left-hand side
  \return
    - true if any matrix elements is inequal, false if none are
  */
  bool operator!=(const m2f& lhs, const m2f& rhs);

  
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
  std::ostream& operator<<(std::ostream& output, const m2f& matrix);

} // end dp namespace
