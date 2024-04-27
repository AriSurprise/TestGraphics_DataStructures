/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  m4f.h
Purpose:  2D 4x4, column major matrix; inputs / outputs in row major
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
  therefore v4f based constructors operate column major to facilitate this, and
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
#include "v3f.h"        // Default values for 3D vector arguments
#include "v4f.h"        // Member requires exposed visibility to instantiate


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  class m3f; // 3x3 matrix to contain extracted 3D elements of the 4x4 data

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class m4f //! Square 2 dimensional 4 by 4, column major matrix
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    static const u1 SQUBS = 4u; //! Square matrix has [4] vec[4] cols of f4s


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set from 4x4 matrix setting all members manually (row major ordered)
    \brief
      - set from 4x4 matrix setting all members manually (row major ordered)
    \param r1c1
      - col-major [0][0] element: X value / (1st) row, (1st) column / U-vector
    \param r1c2
      - col-major [1][0] element: X value / (1st) row, (2nd) column / V-vector
    \param r1c3
      - col-major [2][0] element: X value / (1st) row, (3rd) column / N-vector
    \param r1c4
      - col-major [3][0] element: X value / (1st) row, (4th) column / T-vector
    \param r2c1
      - col-major [0][1] element: Y value / (2nd) row, (1st) column / U-vector
    \param r2c2
      - col-major [1][1] element: Y value / (2nd) row, (2nd) column / V-vector
    \param r2c3
      - col-major [2][1] element: Y value / (2nd) row, (3rd) column / N-vector
    \param r2c4
      - col-major [3][1] element: Y value / (2nd) row, (4th) column / T-vector
    \param r3c1
      - col-major [0][2] element: Z value / (3rd) row, (1st) column / U-vector
    \param r3c2
      - col-major [1][2] element: Z value / (3rd) row, (2nd) column / V-vector
    \param r3c3
      - col-major [2][2] element: Z value / (3rd) row, (3rd) column / N-vector
    \param r3c4
      - col-major [3][2] element: Z value / (3rd) row, (4th) column / T-vector
    \param r4c1
      - col-major [0][3] element: W value / (4th) row, (1st) column / U-vector
    \param r4c2
      - col-major [1][3] element: W value / (4th) row, (2nd) column / V-vector
    \param r4c3
      - col-major [2][3] element: W value / (4th) row, (3rd) column / N-vector
    \param r4c4
      - col-major [3][3] element: W value / (4th) row, (4th) column / T-vector
    \param colMajor
      - flip default row major read: if true => transpose from col major input
    */
    m4f(f4 r1c1, f4 r1c2, f4 r1c3, f4 r1c4,
        f4 r2c1, f4 r2c2, f4 r2c3, f4 r2c4,
        f4 r3c1, f4 r3c2, f4 r3c3, f4 r3c4,
        f4 r4c1, f4 r4c2, f4 r4c3, f4 r4c4 = If,
        bool colMajor = false);


    /** Set from 4x4 matrix's 3D affine members manually (row major)
    \brief
      - set from 4x4 matrix's 3D affine members manually (row major)
    \param ux
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param vx
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param nx
      - X (1st) component of the n vector (3rd column): col-major [2][0] value
    \param tx
      - X (1st) component of the t vector (4th column): col-major [3][0] value
    \param uy
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param vy
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    \param ny
      - Y (2nd) component of the n vector (3rd column): col-major [2][1] value
    \param ty
      - Y (2nd) component of the t vector (4th column): col-major [3][1] value
    \param uz
      - Z (3rd) component of the u vector (1st column): col-major [0][2] value
    \param vz
      - Z (3rd) component of the v vector (2nd column): col-major [1][2] value
    \param nz
      - Z (3rd) component of the n vector (3rd column): col-major [2][2] value
    \param tz
      - Z (3rd) component of the t vector (4th column): col-major [3][2] value
    \param affine
      - value set in row[3], column[3]: final element, homogeneous affine coord
    \param skew
      - value for affine non-diagonal elements (row != column: just last row)
    */
    m4f(f4 ux, f4 vx, f4 nx, f4 tx,
        f4 uy, f4 vy, f4 ny, f4 ty,
        f4 uz, f4 vz, f4 nz, f4 tz,
        f4 affine = If, f4 skew = Of);

    
    /** Set from 4x4 matrix's 3D linear members manually (row major ordered)
    \brief
      - set from 4x4 matrix's 3D linear members manually (row major ordered)
    \param ux
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param vx
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param nx
      - X (1st) component of the n vector (3rd column): col-major [2][0] value
    \param uy
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param vy
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    \param ny
      - Y (2nd) component of the n vector (3rd column): col-major [2][1] value
    \param uz
      - Z (3rd) component of the u vector (1st column): col-major [0][2] value
    \param vz
      - Z (3rd) component of the v vector (2nd column): col-major [1][2] value
    \param nz
      - Z (3rd) component of the n vector (3rd column): col-major [2][2] value
    \param affine
      - value set in row[3], column[3]: final element, homogeneous affine coord
    \param skew
      - value set in all non-diagonal elements (row != column), across this m4f
    */
    m4f(f4 ux, f4 vx, f4 nx,
        f4 uy, f4 vy, f4 ny,
        f4 uz, f4 vz, f4 nz,
        f4 affine = If, f4 skew = Of);


    /** Set lazily from 4x4 matrix's 3D linear members with raw literal values
    \brief
      - set lazily from 4x4 matrix's 3D linear members with raw literal values
    \param ux
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param vx
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param nx
      - X (1st) component of the n vector (3rd column): col-major [2][0] value
    \param uy
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param vy
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    \param ny
      - Y (2nd) component of the n vector (3rd column): col-major [2][1] value
    \param uz
      - Z (3rd) component of the u vector (1st column): col-major [0][2] value
    \param vz
      - Z (3rd) component of the v vector (2nd column): col-major [1][2] value
    \param nz
      - Z (3rd) component of the n vector (3rd column): col-major [2][2] value
    \param affine
      - value set in row[3], column[3]: final element, homogeneous affine coord
    \param skew
      - value set in all non-diagonal elements (row != column), across this m4f
    */
    m4f(f8 ux, f8 vx, f8 nx,
        f8 uy, f8 vy, f8 ny,
        f8 uz, f8 vz, f8 nz,
        f8 affine = 1.0, f8 skew = 0.0);

    
    /** \brief
      Set from 4x4 matrix's 2D affine members manually (row major default)
    \param ux
      - primary axis's native axial transformation contribution
    \param vx
      - secondary axis's secondary axis change contribution from primary
    \param tx
      - affine / homogeneous translation component in primary axis (x)
    \param uy
      - primary axis's secondary axis change contribution from secondary
    \param vy
      - secondary axis's native axial transformation contribution
    \param ty
      - affine / homogeneous translation component in secondary axis (y)
    \param plane
      - 2D plane perpendicular to the unaltered axis: axes correlated to u & v
    \param tz
      - translation in the tertiary-axis to allow for z-index alterations
    \param normal
      - scalar coefficient of perpendicular axis coords output relative to input
    \param affine
      - value of the last element (cell [3][3]): homogeneous promotion coord
    \param off
      - default value of unused 3D axis covariance elements (col != row)
    \param skew
      - default value of homogeneous promotion for other elements (w values)
    */
    m4f(f4 ux, f4 vx, f4 tx,
        f4 uy, f4 vy, f4 ty,
        PLANE plane,  f4 tz = Of, f4 nz = If,
        f4 affine = If, f4 off = Of, f4 skew = Of);


    /** Create a uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \brief
      - create a uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \param sx
      - value for row[0], column[0]: x-scalar of axis-aligned transform matrix
    \param tx
      - value for row[0], column[3]: x-axis center of axis-aligned transform
    \param sy
      - value for row[1], column[1]: y-scalar of axis-aligned transform matrix
    \param ty
      - value for row[1], column[3]: y-axis center of axis-aligned transform
    \param sz
      - value for row[2], column[2]: z-scalar of axis-aligned transform matrix
    \param tz
      - value for row[2], column[3]: z-axis center of axis-aligned transform
    \param affine
      - value set in row[3], column[3]: final element, homogeneous affine coord
    \param skew
      - value set in all non-diagonal elements (row != column; not last column)
    */
    m4f(f4 sx,     f4 tx,
          f4 sy,   f4 ty,
            f4 sz, f4 tz,
        f4 affine = If, f4 skew = Of);


    /** Set from 4x4 matrix's 2D linear members manually (row major default)
    \brief
      - set from 4x4 matrix's 2D linear members manually (row major default)
    \param ux
      - primary axis's native axial transformation contribution
    \param vx
      - secondary axis's secondary axis transformation contribution from primary
    \param uy
      - primary axis's secondary axis transformation contribution from secondary
    \param vy
      - secondary axis's native axial transformation contribution
    \param plane
      - 2D plane perpendicular to the unaltered axis: axes correlated to u & v
    \param tz
      - translation in the z-axis to allow for z-index conversions / flattening
    \param normal
      - scalar coefficient of perpendicular axis coords output relative to input
    \param affine
      - value of the last element (cell [3][3]): homogeneous promotion coord
    \param off
      - default value of unused 3D axis covariance elements (col != row)
    \param skew
      - default value of homogeneous promotion for other elements (w values)
    */
    m4f(f4 ux, f4 vx,
        f4 uy, f4 vy,
        PLANE plane, f4 tz = Of, f4 normal = If,
        f4 affine = If, f4 off = Of, f4 skew = Of);


    /** Create a uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \brief
      - create a uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \param scale
      - value for diagonal subscripts (row == column), barring the last one
    \param tx
      - value for row[0], column[3]: x-axis center of axis-aligned transform
    \param ty
      - value for row[1], column[3]: y-axis center of axis-aligned transform
    \param tz
      - value for row[2], column[3]: z-axis center of axis-aligned transform
    \param skew
      - value set in linear non-diagonal (row != column), elements (not last)
    */
    m4f(f4 scale, f4 tx, f4 ty, f4 tz);


    /** Create a non-uniform scalar trace (set in non-homogeneous, 3D trace)
    \brief
      - create a non-uniform scalar trace (set in non-homogeneous, 3D trace)
    \param sx
      - value for row[0], column[0]: x-scalar of axis-aligned transform matrix
    \param sy
      - value for row[1], column[1]: y-scalar of axis-aligned transform matrix
    \param sz
      - value for row[2], column[2]: z-scalar of axis-aligned transform matrix
    */
    m4f(f4 sx, f4 sy, f4 sz);


    /** m4f Uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \brief
      - m4f Uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \param linear
      - value for diagonal subscripts (row == column), barring the last one
    \param affine
      - value to set in last diagonal subscript (last element of matrix)
    */
    m4f(f4 linear = 1.0f, f4 affine = 1.0f);


    /** m4f 2D-Vector setting from column-major input (col major input stored)
    \brief
      - m4f 2D-Vector setting from column-major input (col major input stored)
    \param U
      - right axis vector of doubly homogeneous transformation basis
    \param V
      - up axis vector of doubly homogeneous transformation basis
    \param axisOut
      - 2D plane perpendicular to the unaltered axis: axes correlated to u & v
    \param T
      - 4th column of homogeneous transformation basis: (T)ranslation vector
    \param normal
      - scalar coefficient of perpendicular axis coords output relative to input
    \param affine
      - value of the last element (cell [3][3]): homogeneous promotion coord
    \param off
      - default value of unused 3D axis covariance elements (col != row)
    \param skew
      - default value of homogeneous promotion for other elements (w values)
    */
    m4f(const v2f& U, const v2f& V, PLANE axisOut, const v2f& T, f4 tz = Of,
      f4 normal = If, f4 affine = If, f4 off = Of, f4 skew = Of);


    /** m4f 3D-Vector setting from column-major input (col major input stored)
    \brief
      - m4f 3D-Vector setting from column-major input (col major input stored)
    \param U
      - 1st column of homogeneous transformation basis: U-axis (local x-axis)
    \param V
      - 2nd column of homogeneous transformation basis: V-axis (local y-axis)
    \param N
      - 3rd column of homogeneous transformation basis: N-axis (local z-axis)
    \param T
      - 4th column of homogeneous transformation basis: (T)ranslation vector
    \param affine
      - value of the last element (cell [3][3]): homogeneous promotion coord
    \param skew
      - default value of homogeneous promotion for other elements (w values)
    */
    m4f(const v3f& U, const v3f& V, const v3f& N, const v3f& T = nullv3f,
      f4 affine = If, f4 skew = Of);


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
    m4f(const v4f& U, const v4f& V, const v4f& N, const v4f& T);


    /** m4f homogeneous promotion from m3f copying row / column data values
    \brief
      - m4f homogeneous promotion from m3f copying row / column data values
    \param linear
      - use row / column values copied into <x, y, z> elements of the new m4f
    \param translate
      - matrix multiply in <x, y, z> elements of the last column of new m4f
    */
    m4f(const m3f& linear, const v3f& translate = nullv3f,
      f4 affine = If, f4 skew = Of);


    /** Create a copy of another m4f from copied row / column data values
    \brief
      - create a copy of another m4f from copied row / column data values
    \param source
      - existing m4f to have it's row / column values copied into the new m4f
    */
    m4f(const m4f& source);


    /** Create this m4f from a function result's row / column data values
    \brief
      - create this m4f from a function result's row / column data values
    \param result
      - m4f function return to have it's row / column values stolen by this
    */
    m4f(m4f&& result) noexcept;

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
      - to avoid recalculation, pass in an &f4 to set det() into (of this m4f)
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& ToAdjoint(f4* determinant = nullptr);


    /** Get inverse matrix without scale by 1/det: transposed cofactor matrix
    \brief
      - get inverse matrix without scale by 1/det: transposed cofactor matrix
    \details
      - transposed cofactor sometimes preferable for showing clean values
    \param determinant
      - to avoid recalculation, pass in an &f4 to set det() into (of this m4f)
    \return
      - new m4f holding sign adjusted minor determinants in each subscript
    */
    m4f AdjointMat(f4* determinant = nullptr) const;


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
      - new m4f holding sign adjusted minor determinants in each subscript
    */
    m4f CofactorMat(f4* determinant = nullptr) const;


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
      - inverse transformation m4f from present transformation description
    */
    m4f Inverse(void) const;


    /** Set this matrix to the inverse matrix of what it was set to
    \brief
      - set this matrix to the inverse matrix of what it was set to
    \details
      - sets this to the m4f which undoes the current transformations
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& Invert(void);


    /** True if the bottom row is [0, 0, 0, 1]
    \brief
      - true if the bottom row is [0, 0, 0, 1]
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - true if the bottom row is [0, 0, 0, 1], false if any member differs
    */
    inline bool IsAffine(void) const
    {
      return IsInF(col[0][3], _INf, -_INf) && IsInF(col[1][3], _INf, -_INf)
          && IsInF(col[2][3], _INf, -_INf) && NearF(col[3][3], If);
    } // end bool m4f::IsAffine(void) const                                   */


    /** True if the diagonal elements are 1, while all others are 0
    \brief
      - true if the diagonal elements are 1, while all others are 0
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - true if the diagonal elements are 1, while all others are 0
    */
    inline bool IsIdentity(void) const
    {
      return (IsTranslation() && IsLinear());
    } // end bool m4f::IsIdentity(void) const                                 */


    /** True if diagonal along <x,y,z> are 1 and others in <x,y,z> are 0
    \brief
      - true if diagonal along <x,y,z> are 1 and others in <x,y,z> are 0
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - true only if diagonal along <x,y,z> are 1 and others in <x,y,z> are 0
    */
    inline bool IsIdentityLinear(void) const
    {
      return NearF(col[0][0],If) && NearF(col[1][1],If) && NearF(col[2][2],If)
        && IsInF(col[1][0], _INf, -_INf) && IsInF(col[0][1], _INf, -_INf)
        && IsInF(col[2][0], _INf, -_INf) && IsInF(col[0][2], _INf, -_INf)
        && IsInF(col[2][1], _INf, -_INf) && IsInF(col[1][2], _INf, -_INf);
    } // end bool m4f::IsIdentityLinear(void) const                           */


    /** True if the <t> vector (4th column) is [0, 0, 0, 1]
    \brief
      - true if the <t> vector (4th column) is [0, 0, 0, 1]
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - whether the m4f is affine & <t> vector (4th column) is [0, 0, 0, 1]
    */
    inline bool IsLinear(void) const
    {
      return IsAffine() && IsInF(col[3][0], _INf, -_INf)
        && IsInF(col[3][1], _INf, -_INf) && IsInF(col[3][2], _INf, -_INf);
    } // end bool m4f::IsLinear(void) const                                   */


    /** True if all elements above the trace are 0 (all data is in lower half)
    \brief
      - true if all elements above the trace are 0 (all data is in lower half)
    \details
      - is lower triangular in that upper triangle contains no data
    \return
      - true only if all elements above the trace / diagonal are 0 value
    */
    inline bool IsLowerTriangular(void) const
    {
      return IsInF(col[1][0], _INf, -_INf) && IsInF(col[2][0], _INf, -_INf)
          && IsInF(col[2][1], _INf, -_INf) && IsInF(col[3][0], _INf, -_INf)
          && IsInF(col[3][1], _INf, -_INf) && IsInF(col[3][2], _INf, -_INf);

    } // end bool m4f::IsLowerTriangular(void) const                          */


    /** True if determinant is 0, trace is less than dimensions but non 0
    \brief
      - true if determinant is 0, trace is less than dimensions but non 0
    \return
      - true only if determinant is 0, trace is less than dimensions but non 0
    */
    /*inline bool IsProjection(void) const
    {
      return false;
    } // end bool m4f::IsProjection(void) const                              */


    /** True if linear determinant is -1 (scale inverted)
    \brief
      - true if determinant is 0, trace is less than dimensions but non 0
    \return
      - true only if determinant is 0, trace is less than dimensions but non 0
    */
    /*inline bool IsReflection(void) const
    {
      return false;
    } // end bool m4f::IsReflection(void) const                              */


    /** If true, determinant is 1 (scale preserved), & trace 2*cos(t)
    \brief
      - if true, determinant is 1 (scale preserved), & trace 2*cos(t)
    \return
      - true only if transpoaed matrix is the inverse of the current m4f
    */
    inline bool IsRotation(void) const
    {
      return (Transposed() * *this).IsIdentity();
    } // end bool m4f::IsRotation(void) const                                */
    
    
    /** True if any non-diagonal elements (column != row), not on trace are 0
    \brief
      - true if any non-diagonal elements (column != row), not on trace are 0
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - true if only trace is non-zero
    */
    inline bool IsScalar(void) const
    {
      return IsScalarLinear()
        && IsInF(col[3][0], _INf, -_INf) && IsInF(col[0][3], _INf, -_INf)
        && IsInF(col[3][1], _INf, -_INf) && IsInF(col[1][3], _INf, -_INf)
        && IsInF(col[3][2], _INf, -_INf) && IsInF(col[2][3], _INf, -_INf);
    } // end bool m4f::IsScalar(void) const                                   */


    /** True if 0 in all linear non-trace elements (no affine element checks)
    \brief
      - true if 0 in all linear non-trace elements (no affine element checks)
    \details
      - useful in shortcut math detecting scalar-translation => no rotation
    \return
      - true if only trace is non-zero
    */
    inline bool IsScalarLinear(void) const
    {
      return IsInF(col[1][0], _INf, -_INf) && IsInF(col[2][0], _INf, -_INf)
          && IsInF(col[0][1], _INf, -_INf) && IsInF(col[2][1], _INf, -_INf)
          && IsInF(col[0][2], _INf, -_INf) && IsInF(col[1][2], _INf, -_INf);
    } // end bool m4f::IsScalarLinear(void) const                             */


    /** True if the matrix is non-invertible
    \brief
      - true if the matrix is non-invertible
    \param determinant
      - if you have a precalculated determinant, get whether that is 0 instead
    \return
      - whether the matrix has determinant of 0
    */
    inline bool IsSingular(f4* determinant = nullptr) const
    {
      return (IsInF((determinant? *determinant: Det()), _INf, -_INf));
    } // end bool m4f::IsSingular(f4*) const                                  */


    /** True when the transpose is equal to negative of the original matrix
    \brief
      - true when the transpose is equal to negative of the original matrix
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - if the trace is 0 & elements across the diagonal are all negative equals
    */
    inline bool IsSkewSymmetric(void) const
    {
      // Check upper triangle elements against lower triangle counterparts
      return !IsInF(Trace(), _INf, -_INf)
        && NearF(col[1][0], -col[0][1]) && NearF(col[2][0], -col[0][2])
        && NearF(col[3][0], -col[0][3]) && NearF(col[2][1], -col[1][2])
        && NearF(col[3][1], -col[1][3]) && NearF(col[3][2], -col[2][3]);
    } // end bool m4f::IsSkewSymmetric(void) const


    /** True if the transpose is equal to the original matrix
    \brief
      - true if the transpose is equal to the original matrix
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - if every element matches the one across it along the trace
    */
    inline bool IsSymmetrical(void) const
    {
      // Check upper triangle elements against lower triangle counterparts
      return NearF(col[1][0], col[0][1]) && NearF(col[2][0], col[0][2])
          && NearF(col[3][0], col[0][3]) && NearF(col[2][1], col[1][2])
          && NearF(col[3][1], col[1][3]) && NearF(col[3][2], col[2][3]);
    } // end bool m4f::IsSymmetrical(void) const                              */


    /** True if identity is in linear <x,y,z> and [0,0,0,1] in bottom row
    \brief
      - true if identity is in linear <x,y,z> and [0,0,0,1] in bottom row
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - true only if identity is in linear <x,y,z> and [0,0,0,1] in bottom row
    */
    inline bool IsTranslation(void) const
    {
      return IsAffine() && IsIdentityLinear();
    } // end bool m4f::IsTranslation(void) const                              */


    /** True if all elements below the trace are 0 (all data is in upper half)
    \brief
      - true if all elements below the trace are 0 (all data is in upper half)
    \details
      - is only upper triangular in that lower triangle contains no data
    \return
      - true only if all elements below the trace / diagonal are 0 value
    */
    inline bool IsUpperTriangular(void) const
    {
      return  IsInF(col[0][1], _INf, -_INf) && IsInF(col[0][2], _INf, -_INf)
           && IsInF(col[0][3], _INf, -_INf) && IsInF(col[1][2], _INf, -_INf)
           && IsInF(col[1][3], _INf, -_INf) && IsInF(col[2][3], _INf, -_INf);
    } // end bool m4f::IsUpperTriangular(void) const                          */


    /** JSON formatted string containing each member value in row major format
    \brief
      - JSON formatted string containing each member value in row major format
    \return
      - string containing packed tightly ascii copy of contained matrix data
    */
    std::string JSON(void) const;


    /** Create a copy of the matrix's 3D transformations set in a 4D matrix
    \brief
      - create a copy of the matrix's 3D transformations set in a 4D matrix
    \return
      - homogeneously promoted 3D transformation with null affine adjustments
    */
    m4f Linear(void) const;


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
    m4f MinorMat(f4* determinant = nullptr) const;


    /** Get homogeneously promoted product of m4f * [(x, y, z), 1] => (x,y,z)
    \brief
      - get homogeneously promoted product of m4f * [(x, y, z), 1] => (x,y,z)
    \param point
      - vector data representative of a point (not vector span), in 3D space
    \return
      - matrix transformed product v3f result from homogeneously promoted point
    */
    v3f PProd(const v3f& point) const;

    /** Set from 4x4 matrix setting all members manually (row major ordered)
    \brief
      - set from 4x4 matrix setting all members manually (row major ordered)
    \param r1c1
      - col-major [0][0] element: X value / (1st) row, (1st) column / U-vector
    \param r1c2
      - col-major [1][0] element: X value / (1st) row, (2nd) column / V-vector
    \param r1c3
      - col-major [2][0] element: X value / (1st) row, (3rd) column / N-vector
    \param r1c4
      - col-major [3][0] element: X value / (1st) row, (4th) column / T-vector
    \param r2c1
      - col-major [0][1] element: Y value / (2nd) row, (1st) column / U-vector
    \param r2c2
      - col-major [1][1] element: Y value / (2nd) row, (2nd) column / V-vector
    \param r2c3
      - col-major [2][1] element: Y value / (2nd) row, (3rd) column / N-vector
    \param r2c4
      - col-major [3][1] element: Y value / (2nd) row, (4th) column / T-vector
    \param r3c1
      - col-major [0][2] element: Z value / (3rd) row, (1st) column / U-vector
    \param r3c2
      - col-major [1][2] element: Z value / (3rd) row, (2nd) column / V-vector
    \param r3c3
      - col-major [2][2] element: Z value / (3rd) row, (3rd) column / N-vector
    \param r3c4
      - col-major [3][2] element: Z value / (3rd) row, (4th) column / T-vector
    \param r4c1
      - col-major [0][3] element: W value / (4th) row, (1st) column / U-vector
    \param r4c2
      - col-major [1][3] element: W value / (4th) row, (2nd) column / V-vector
    \param r4c3
      - col-major [2][3] element: W value / (4th) row, (3rd) column / N-vector
    \param r4c4
      - col-major [3][3] element: W value / (4th) row, (4th) column / T-vector
    \param colMajor
      - flip default row major read: if true => transpose from col major input
    */
    m4f& Set(f4 r1c1, f4 r1c2, f4 r1c3, f4 r1c4,
             f4 r2c1, f4 r2c2, f4 r2c3, f4 r2c4,
             f4 r3c1, f4 r3c2, f4 r3c3, f4 r3c4,
             f4 r4c1, f4 r4c2, f4 r4c3, f4 r4c4 = If,
             bool colMajor = false);


    /** Set from 4x4 matrix's 3D affine members manually (row major)
    \brief
      - set from 4x4 matrix's 3D affine members manually (row major)
    \param ux
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param vx
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param nx
      - X (1st) component of the n vector (3rd column): col-major [2][0] value
    \param tx
      - X (1st) component of the t vector (4th column): col-major [3][0] value
    \param uy
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param vy
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    \param ny
      - Y (2nd) component of the n vector (3rd column): col-major [2][1] value
    \param ty
      - Y (2nd) component of the t vector (4th column): col-major [3][1] value
    \param uz
      - Z (3rd) component of the u vector (1st column): col-major [0][2] value
    \param vz
      - Z (3rd) component of the v vector (2nd column): col-major [1][2] value
    \param nz
      - Z (3rd) component of the n vector (3rd column): col-major [2][2] value
    \param tz
      - Z (3rd) component of the t vector (4th column): col-major [3][2] value
    \param affine
      - value set in row[3], column[3]: final element, homogeneous affine coord
    \param skew
      - value for affine non-diagonal elements (row != column: just last row)
    */
    m4f& Set(f4 ux, f4 vx, f4 nx, f4 tx,
             f4 uy, f4 vy, f4 ny, f4 ty,
             f4 uz, f4 vz, f4 nz, f4 tz,
             f4 affine = If, f4 skew = Of);


    /** Set from 4x4 matrix's 3D linear members manually (row major ordered)
    \brief
      - set from 4x4 matrix's 3D linear members manually (row major ordered)
    \param ux
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param vx
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param nx
      - X (1st) component of the n vector (3rd column): col-major [2][0] value
    \param uy
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param vy
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    \param ny
      - Y (2nd) component of the n vector (3rd column): col-major [2][1] value
    \param uz
      - Z (3rd) component of the u vector (1st column): col-major [0][2] value
    \param vz
      - Z (3rd) component of the v vector (2nd column): col-major [1][2] value
    \param nz
      - Z (3rd) component of the n vector (3rd column): col-major [2][2] value
    \param affine
      - value set in row[3], column[3]: final element, homogeneous affine coord
    \param skew
      - value set in all non-diagonal elements (row != column), across this m4f
    */
    m4f& Set(f4 ux, f4 vx, f4 nx,
             f4 uy, f4 vy, f4 ny,
             f4 uz, f4 vz, f4 nz,
             f4 affine = If, f4 skew = Of);


    /** Set lazily from 4x4 matrix's 3D linear members with raw literal values
    \brief
      - set lazily from 4x4 matrix's 3D linear members with raw literal values
    \param ux
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param vx
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param nx
      - X (1st) component of the n vector (3rd column): col-major [2][0] value
    \param uy
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param vy
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    \param ny
      - Y (2nd) component of the n vector (3rd column): col-major [2][1] value
    \param uz
      - Z (3rd) component of the u vector (1st column): col-major [0][2] value
    \param vz
      - Z (3rd) component of the v vector (2nd column): col-major [1][2] value
    \param nz
      - Z (3rd) component of the n vector (3rd column): col-major [2][2] value
    \param affine
      - value set in row[3], column[3]: final element, homogeneous affine coord
    \param skew
      - value set in all non-diagonal elements (row != column), across this m4f
    */
    m4f& Set(f8 ux, f8 vx, f8 nx,
             f8 uy, f8 vy, f8 ny,
             f8 uz, f8 vz, f8 nz,
             f8 affine = 1.0, f8 skew = 0.0);


    /** \brief
      Set from 4x4 matrix's 2D affine members manually (row major default)
    \param ux
      - primary axis's native axial transformation contribution
    \param vx
      - secondary axis's secondary axis change contribution from primary
    \param tx
      - affine / homogeneous translation component in primary axis (x)
    \param uy
      - primary axis's secondary axis change contribution from secondary
    \param vy
      - secondary axis's native axial transformation contribution
    \param ty
      - affine / homogeneous translation component in secondary axis (y)
    \param plane
      - 2D plane perpendicular to the unaltered axis: axes correlated to u & v
    \param tz
      - translation in the tertiary-axis to allow for z-index alterations
    \param normal
      - scalar coefficient of perpendicular axis coords output relative to input
    \param affine
      - value of the last element (cell [3][3]): homogeneous promotion coord
    \param off
      - default value of unused 3D axis covariance elements (col != row)
    \param skew
      - default value of homogeneous promotion for other elements (w values)
    */
    m4f& Set(f4 ux, f4 vx, f4 tx,
             f4 uy, f4 vy, f4 ty,
             PLANE plane, f4 tz = Of, f4 nz = If,
             f4 affine = If, f4 off = Of, f4 skew = Of);


    /** Create a uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \brief
      - create a uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \param sx
      - value for row[0], column[0]: x-scalar of axis-aligned transform matrix
    \param tx
      - value for row[0], column[3]: x-axis center of axis-aligned transform
    \param sy
      - value for row[1], column[1]: y-scalar of axis-aligned transform matrix
    \param ty
      - value for row[1], column[3]: y-axis center of axis-aligned transform
    \param sz
      - value for row[2], column[2]: z-scalar of axis-aligned transform matrix
    \param tz
      - value for row[2], column[3]: z-axis center of axis-aligned transform
    \param affine
      - value set in row[3], column[3]: final element, homogeneous affine coord
    \param skew
      - value set in all non-diagonal elements (row != column; not last column)
    */
    m4f& Set(f4 sx, f4 tx,
             f4 sy, f4 ty,
             f4 sz, f4 tz,
             f4 affine = If, f4 skew = Of);


    /** Set from 4x4 matrix's 2D linear members manually (row major default)
    \brief
      - set from 4x4 matrix's 2D linear members manually (row major default)
    \param ux
      - primary axis's native axial transformation contribution
    \param vx
      - secondary axis's secondary axis transformation contribution from primary
    \param uy
      - primary axis's secondary axis transformation contribution from secondary
    \param vy
      - secondary axis's native axial transformation contribution
    \param plane
      - 2D plane perpendicular to the unaltered axis: axes correlated to u & v
    \param tz
      - translation in the z-axis to allow for z-index conversions / flattening
    \param normal
      - scalar coefficient of perpendicular axis coords output relative to input
    \param affine
      - value of the last element (cell [3][3]): homogeneous promotion coord
    \param off
      - default value of unused 3D axis covariance elements (col != row)
    \param skew
      - default value of homogeneous promotion for other elements (w values)
    */
    m4f& Set(f4 ux, f4 vx,
             f4 uy, f4 vy,
             PLANE plane, f4 tz = Of, f4 normal = If,
             f4 affine = If, f4 off = Of, f4 skew = Of);


    /** Create a uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \brief
      - create a uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \param scale
      - value for diagonal subscripts (row == column), barring the last one
    \param tx
      - value for row[0], column[3]: x-axis center of axis-aligned transform
    \param ty
      - value for row[1], column[3]: y-axis center of axis-aligned transform
    \param tz
      - value for row[2], column[3]: z-axis center of axis-aligned transform
    \param skew
      - value set in linear non-diagonal (row != column), elements (not last)
    */
    m4f& Set(f4 scale, f4 tx, f4 ty, f4 tz);


    /** Create a non-uniform scalar trace (set in non-homogeneous, 3D trace)
    \brief
      - create a non-uniform scalar trace (set in non-homogeneous, 3D trace)
    \param sx
      - value for row[0], column[0]: x-scalar of axis-aligned transform matrix
    \param sy
      - value for row[1], column[1]: y-scalar of axis-aligned transform matrix
    \param sz
      - value for row[2], column[2]: z-scalar of axis-aligned transform matrix
    */
    m4f& Set(f4 sx, f4 sy, f4 sz);


    /** m4f Uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \brief
      - m4f Uniform linear scalar trace (set in non-homogeneous, 3D trace)
    \param linear
      - value for diagonal subscripts (row == column), barring the last one
    \param affine
      - value to set in last diagonal subscript (last element of matrix)
    */
    m4f& Set(f4 linear = 1.0f, f4 affine = 1.0f);


    /** m4f 2D-Vector setting from column-major input (col major input stored)
    \brief
      - m4f 2D-Vector setting from column-major input (col major input stored)
    \param U
      - right axis vector of doubly homogeneous transformation basis
    \param V
      - up axis vector of doubly homogeneous transformation basis
    \param axisOut
      - 2D plane perpendicular to the unaltered axis: axes correlated to u & v
    \param T
      - 4th column of homogeneous transformation basis: (T)ranslation vector
    \param normal
      - scalar coefficient of perpendicular axis coords output relative to input
    \param affine
      - value of the last element (cell [3][3]): homogeneous promotion coord
    \param off
      - default value of unused 3D axis covariance elements (col != row)
    \param skew
      - default value of homogeneous promotion for other elements (w values)
    */
    m4f& Set(const v2f& U, const v2f& V, PLANE axisOut, const v2f& T,
      f4 tz = Of, f4 normal = If, f4 affine = If, f4 off = Of, f4 skew = Of);


    /** m4f 3D-Vector setting from column-major input (col major input stored)
    \brief
      - m4f 3D-Vector setting from column-major input (col major input stored)
    \param U
      - 1st column of homogeneous transformation basis: U-axis (local x-axis)
    \param V
      - 2nd column of homogeneous transformation basis: V-axis (local y-axis)
    \param N
      - 3rd column of homogeneous transformation basis: N-axis (local z-axis)
    \param T
      - 4th column of homogeneous transformation basis: (T)ranslation vector
    \param affine
      - value of the last element (cell [3][3]): homogeneous promotion coord
    \param skew
      - default value of homogeneous promotion for other elements (w values)
    */
    inline m4f& Set(const v3f& U, const v3f& V, const v3f& N,
      const v3f& T = nullv3f, f4 affine = If, f4 skew = Of)
    {
      return Set(U.XYZW(skew), V.XYZW(skew), N.XYZW(skew), T.XYZW(affine));
    }// end m4f& m4f::Set(cnst v3f&,cnst v3f&,cnst v3f&,cnst v3f&,(f4),(f4)) */


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
    m4f& Set(const v4f& U, const v4f& V, const v4f& N, const v4f& T);


    /** m4f homogeneous promotion from m3f copying row / column data values
    \brief
      - m4f homogeneous promotion from m3f copying row / column data values
    \param linear
      - use row / column values copied into <x, y, z> elements of the new m4f
    \param translate
      - matrix multiply in <x, y, z> elements of the last column of new m4f
    */
    m4f& Set(const m3f& linear, const v3f& translate = nullv3f,
      f4 affine = If, f4 skew = Of);


    /** Set a m4f column / row data to another source's column / row values
    \brief
      - set a m4f column / row data to another source's column / row values
    \param source
      - matrix to have its data members copied into this matrix
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& Set(const m4f& source);


    /** JSON formatted string containing each member value in row major format
    \brief
      - JSON formatted string containing each member value in row major format
    \param lineBreaks
      - if enabled / true, rows will end with new lines (undesirable in serdes)
    \return
      - string containing packed tightly ascii copy of contained matrix data
    */
    std::string ToString(bool lineBreaks = false) const;


    /** Calculate the sum (or product alternatively), of diagonal elements
    \brief
      - calculate the sum (or product alternatively), of diagonal elements
    \param product
      - if enabled, alternately returns the product instead of sum
    \return
      - sum (or product), of the diagonal (row == col) elements
    */
    f4 Trace(bool product = false) const;


    /** Get the homogeneous sum (or product), of the diagonal <x,y,z> elements
    \brief
      - get the homogeneous sum (or product), of the diagonal <x,y,z> elements
    \param product
      - if enabled, alternately returns the product instead of sum
    \return
      - sum (or product), of the diagonal (row == col) elements, without w
    */
    f4 TraceH(bool product = false) const;


    /** Set the matrix to have swapped contents for rows and columns
    \brief
      - set the matrix to have swapped contents for rows and columns
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& Transpose(void);


    /** Get a copy of this m4f with swapped contents for rows and columns
    \brief
      - get a copy of this m4f with swapped contents for rows and columns
    \return
      - copy of this m4f with swapped row and column data fields
    */
    m4f Transposed(void) const;


    /** Get homogeneously promoted product of m4f * [<x, y, z>, 0] => <x,y,z>
    \brief
      - get homogeneously promoted product of m4f * [<x, y, z>, 0] => <x,y,z>
    \param point
      - vector data representative of a vector span (not point), in 3D space
    \return
      - matrix transformed product v3f result from homogeneously promoted v3f
    */
    v3f VProd(const v3f& vector) const;


    /** Extract this matrix's square 3D <x, x, x> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <x, x, x> vector elements set in an m3f
    \return
      - <x, x, x> column like vector elements in <x, x, x> columns of a new m3f
    */
    m3f XXX(void) const;


    /** Extract this matrix's square 3D <x, x, y> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <x, x, y> vector elements set in an m3f
    \return
      - <x, x, y> column like vector elements in <x, x, y> columns of a new m3f
    */
    m3f XXY(void) const;


    /** Extract this matrix's square 3D <x, x, z> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <x, x, z> vector elements set in an m3f
    \return
      - <x, x, z> column like vector elements in <x, x, z> columns of a new m3f
    */
    m3f XXZ(void) const;


    /** Extract this matrix's square 3D <x, y, x> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <x, y, x> vector elements set in an m3f
    \return
      - <x, y, x> column like vector elements in <x, y, x> columns of a new m3f
    */
    m3f XYX(void) const;


    /** Extract this matrix's square 3D <x, y, y> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <x, y, y> vector elements set in an m3f
    \return
      - <x, y, y> column like vector elements in <x, y, y> columns of a new m3f
    */
    m3f XYY(void) const;


    /** Extract this matrix's square 3D <x, y, z> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <x, y, z> vector elements set in an m3f
    \return
      - <x, y, z> column like vector elements in <x, y, z> columns of a new m3f
    */
    m3f XYZ(void) const;


    /** Extract this matrix's square 3D <x, z, x> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <x, z, x> vector elements set in an m3f
    \return
      - <x, z, x> column like vector elements in <x, z, x> columns of a new m3f
    */
    m3f XZX(void) const;


    /** Extract this matrix's square 3D <x, z, y> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <x, z, y> vector elements set in an m3f
    \return
      - <x, z, y> column like vector elements in <x, z, y> columns of a new m3f
    */
    m3f XZY(void) const;


    /** Extract this matrix's square 3D <x, z, z> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <x, z, z> vector elements set in an m3f
    \return
      - <x, z, z> column like vector elements in <x, z, z> columns of a new m3f
    */
    m3f XZZ(void) const;


    /** Extract this matrix's square 3D <y, x, x> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <y, x, x> vector elements set in an m3f
    \return
      - <y, x, x> column like vector elements in <y, x, x> columns of a new m3f
    */
    m3f YXX(void) const;


    /** Extract this matrix's square 3D <y, x, y> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <y, x, y> vector elements set in an m3f
    \return
      - <y, x, y> column like vector elements in <y, x, y> columns of a new m3f
    */
    m3f YXY(void) const;


    /** Extract this matrix's square 3D <y, x, z> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <y, x, z> vector elements set in an m3f
    \return
      - <y, x, z> column like vector elements in <y, x, z> columns of a new m3f
    */
    m3f YXZ(void) const;


    /** Extract this matrix's square 3D <y, y, x> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <y, y, x> vector elements set in an m3f
    \return
      - <y, y, x> column like vector elements in <y, y, x> columns of a new m3f
    */
    m3f YYX(void) const;


    /** Extract this matrix's square 3D <y, y, y> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <y, y, y> vector elements set in an m3f
    \return
      - <y, y, y> column like vector elements in <y, y, y> columns of a new m3f
    */
    m3f YYY(void) const;


    /** Extract this matrix's square 3D <y, y, z> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <y, y, z> vector elements set in an m3f
    \return
      - <y, y, z> column like vector elements in <y, y, z> columns of a new m3f
    */
    m3f YYZ(void) const;


    /** Extract this matrix's square 3D <y, z, x> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <y, z, x> vector elements set in an m3f
    \return
      - <y, z, x> column like vector elements in <y, z, x> columns of a new m3f
    */
    m3f YZX(void) const;


    /** Extract this matrix's square 3D <y, z, y> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <y, z, y> vector elements set in an m3f
    \return
      - <y, z, y> column like vector elements in <y, z, y> columns of a new m3f
    */
    m3f YZY(void) const;


    /** Extract this matrix's square 3D <y, z, z> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <y, z, z> vector elements set in an m3f
    \return
      - <y, z, z> column like vector elements in <y, z, z> columns of a new m3f
    */
    m3f YZZ(void) const;


    /** Extract this matrix's square 3D <z, x, x> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <z, x, x> vector elements set in an m3f
    \return
      - <z, x, x> column like vector elements in <z, x, x> columns of a new m3f
    */
    m3f ZXX(void) const;


    /** Extract this matrix's square 3D <z, x, y> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <z, x, y> vector elements set in an m3f
    \return
      - <z, x, y> column like vector elements in <z, x, y> columns of a new m3f
    */
    m3f ZXY(void) const;


    /** Extract this matrix's square 3D <z, x, z> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <z, x, z> vector elements set in an m3f
    \return
      - <z, x, z> column like vector elements in <z, x, z> columns of a new m3f
    */
    m3f ZXZ(void) const;


    /** Extract this matrix's square 3D <z, y, x> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <z, y, x> vector elements set in an m3f
    \return
      - <z, y, x> column like vector elements in <z, y, x> columns of a new m3f
    */
    m3f ZYX(void) const;


    /** Extract this matrix's square 3D <z, y, y> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <z, y, y> vector elements set in an m3f
    \return
      - <z, y, y> column like vector elements in <z, y, y> columns of a new m3f
    */
    m3f ZYY(void) const;


    /** Extract this matrix's square 3D <z, y, z> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <z, y, z> vector elements set in an m3f
    \return
      - <z, y, z> column like vector elements in <z, y, z> columns of a new m3f
    */
    m3f ZYZ(void) const;


    /** Extract this matrix's square 3D <z, z, x> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <z, z, x> vector elements set in an m3f
    \return
      - <z, z, x> column like vector elements in <z, z, x> columns of a new m3f
    */
    m3f ZZX(void) const;


    /** Extract this matrix's square 3D <z, z, y> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <z, z, y> vector elements set in an m3f
    \return
      - <z, z, y> column like vector elements in <z, z, y> columns of a new m3f
    */
    m3f ZZY(void) const;


    /** Extract this matrix's square 3D <z, z, z> vector elements set in an m3f
    \brief
      - extract this matrix's square 3D <z, z, z> vector elements set in an m3f
    \return
      - <z, z, z> column like vector elements in <z, z, z> columns of a new m3f
    */
    m3f ZZZ(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set a m4f column / row data to another source's column / row values
    \brief
      - set a m4f column / row data to another source's column / row values
    \param source
      - matrix to have its data members copied into this matrix
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& operator=(const m4f& source);


    /** Set this m4f from a function result's row / column data values
    \brief
      - set this m4f from a function result's row / column data values
    \param result
      - returned m4f to have it's row / column values stolen by this
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& operator=(m4f&& result) noexcept;


    /** Get a copy of the matrix with memberwise negated elements
    \brief
      - get a copy of the matrix with memberwise negated elements
    \return
      - copy of existing matrix data with negated values memberwise
    */
    m4f operator-(void) const;


    /** Get the inverse of the current matrix data set (reverse transform)
    \brief
      - get the inverse of the current matrix data set (reverse transform)
    \return
      - matrix undergoing the reverse change of basis from this matrix
    */
    m4f operator!(void) const;


    /** Get a column v4f from the m4f's col[0-3] values: <0:u, 1:v, 2:n, 3:t>
    \brief
      - get a column v4f from the m4f's col[0-3] values: <0:u, 1:v, 2:n, 3:t>
    \param subscript
      - value in [0,3] range to access the corresponding ordered column vector
    \return
      - reference to the column vector subscript: <0:u, 1:v, 2:n, 3:t>
    */
    const v4f& operator[](u4 subscript) const;


    /** Get a column v4f from the m4f's col[0-3] values: <0:u, 1:v, 2:n, 3:t>
    \brief
      - get a column v4f from the m4f's col[0-3] values: <0:u, 1:v, 2:n, 3:t>
    \param subscript
      - value in [0,3] range to access the corresponding ordered column vector
    \return
      - reference to the column vector subscript: <0:u, 1:v, 2:n, 3:t>
    */
    v4f& operator[](u4 subscript);


    /** Calculate the member-wise addition of 2 m4f's, returning the sum
    \brief
      - calculate the member-wise addition of 2 m4f's, returning the sum
    \param other
      - m4f to be added to this matrix's row / column data, member-wise
    \return
      - copy of this m4f with each entry added, member-wise, by other
    */
    m4f operator+(const m4f& other) const;


    /** Add an other m4f's values member-wise, setting the sum into this
    \brief
      - add an other m4f's values member-wise, setting the sum into this
    \param other
      - m4f to be added to this matrix's row / column data, member-wise
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& operator+=(const m4f& other);


    /** Calculate member-wise subtraction of 2 m4f's, returning the difference
    \brief
      - calculate member-wise subtraction of 2 m4f's, returning the difference
    \param other
      - m4f to be subtracted from this matrix's row / column data, member-wise
    \return
      - copy of this m4f with each entry subtracted, member-wise, by other
    */
    m4f operator-(const m4f& other) const;


    /** Subtract an other m4f's values member-wise, setting the difference
    \brief
      - subtract an other m4f's values member-wise, setting the difference
    \param other
      - m4f to be subtracted from this matrix's row / column data, member-wise
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& operator-=(const m4f& other);


    /** Scalar multiplication multiplies each member by scalar constant
    \brief
      - scalar multiplication multiplies each member by scalar constant
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - copy of this m4f, scaled by scalar
    */
    m4f operator*(f4 scalar) const;


    /** Get the concatenated matrix of (this * other): both transformations
    \brief
      - get the concatenated matrix of (this * other): both transformations
    \param other
      - m4f to be right concatenated with this matrix
    \return
      - copy of this m4f, concatenated with the other
    */
    v4f operator*(const v4f& vector) const;


    /** Get the concatenated matrix of (this * other): both transformations
    \brief
      - get the concatenated matrix of (this * other): both transformations
    \param other
      - m4f to be right concatenated with this matrix
    \return
      - copy of this m4f, concatenated with the other
    */
    m4f operator*(const m4f& other) const;


    /** Scale each matrix element by scalar, setting the product into this
    \brief
      - scale each matrix element by scalar, setting the product into this
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& operator*=(f4 scalar);


    /** Matrix multiplication concatenates 2 sequential transformations into 1
    \brief
      - matrix multiplication concatenates 2 sequential transformations into 1
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& operator*=(const m4f& other);


    /** Perform division on all members, returning the result
    \brief
      - perform division on all members, returning the result
    \param inverseScalar
      - scalar amount by which to multiply the reciprocal of per matrix element
    \return
      - copy of this m4f, scaled up by (1.0 / inverseScalar)
    */
    m4f operator/(f4 inverseScalar) const;


    /** Division multiplies all members by reciprocal of inverseScalar
    \brief
      - division multiplies all members by reciprocal of inverseScalar
    \param inverseScalar
      - scalar amount by which to multiply the reciprocal of per matrix element
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& operator/=(f4 inverseScalar);


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

    //! Row column major lists of <u,v,n,t> vectors of 3D the affine transform
    v4f col[SQUBS];

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Calculate and set matrix to minor det matrix with oscillating signs
    \brief
      - calculate and set matrix to minor det matrix with oscillating signs
    \details
      - intermediary inverse step, setting determinant relevant to prior m4f
      Just doesn't make sense to have the set in place premutation public:
      really only used in adjoint / inverse matrix calculations
    \param determinant
      - to avoid recalculation, pass in an f4 to store det() (for former values)
    \return
      - reference to the modified m4f for easier subsequent inline action
    */
    m4f& cofactor(f4* determinant = nullptr);



    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  }; // end m4f struct



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Constant Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Zero filled m4f in all entries; reserved value for an invalid state
  static const m4f nullm4f(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Create Rodrigues rotation m4f around axis, centered on offset
  \brief
    - create Rodrigues rotation m4f around axis, centered on offset
  \details
    - requires 4 floats to describe a relative rotation process. 
    Not well suited to storing absolute rotation transformations relative to
    the origin, but less subject to gimbal lock than a Euler rotation, and
    easier to conceive of than quaternions.
  \param rotation
    - arc of displacement about the given axis
  \param axis
    - direction about which rotation takes place
  \param offset
    - pointer to the v4f which can take the m4f to desired affine center
  \return
    - new m4f transformation of the requested rotation about the axis
  */
  m4f RotateRodriguesAffine(ang rotation, const v3f& axis = Z_HAT,
    const v3f& affineOffset = nullv3f);


  /** Create a Euler angle rotation matrix about z y and x world axes
  \brief
    - create a Euler angle rotation matrix about z y and x world axes
  \details
    - simpler than quaternions for handling rotations, but more subject to
    issues regarding consistent interpolation like gimbal lock. Does allow for
    a relatively accessible base model under which to see rotation happen.
    Alternatively, rodrigues formula is nice for iterative rotation increments
    about a given axis, but doesn't quite allow for a transform the same way
  \param z_rotation
    - rotation from 0, counter-clockwise, in the z-axis
  \param y_rotation
    - rotation from 0, counter-clockwise, in the y-axis
  \param x_rotation
    - rotation from 0, counter-clockwise, in the x-axis
  \param affineOffset
    - pointer to the v4f which can take the m4f to desired affine center
  \return
    - new m4f describing the requested sum of rotations
  */
  m4f RotateEulerAffine(ang z_rotation, ang y_rotation = nullang,
    ang x_rotation = nullang, const v3f& affineOffset = nullv3f);

  
  /** Create m4f which vector multiplies to scale points in <x,y,z> by scalars
  \brief
    - create m4f which vector multiplies to scale points in <x,y,z> by scalars
  \param x_scalar
    - scalar factor for scaling x-coordinates of vectors
  \param y_scalar
    - scalar factor for scaling y-coordinates of vectors
  \param z_scalar
    - scalar factor for scaling z-coordinates of vectors
  \param offset
    - pointer to the v4f which can take the m4f to desired affine center
  \return
    - m4f with only linear trace having u<x_s>,v<y_s>,n<z_s>,t<1.0>, and 0's
  */
  m4f Scale3Affine(f4 x_scalar, f4 y_scalar = NAN, f4 z_scalar = NAN, const
    v3f& affineOffset = nullv3f);

  
  /** Create m4f which vector multiplies to scale points in <x,y,z> by scalars
  \brief
    - create m4f which vector multiplies to scale points in <x,y,z> by scalars
  \param affineOffset
    - vector which can take the m4f to desired affine center
  \param scales
    - per dimensional triplet of scalar factors for scaling multiplicands
  \return
    - m4f with only linear trace having u<x_s>,v<y_s>,n<z_s>,t<1.0>, and 0's
  */
  m4f ScaleAffine(const v3f& scales, const v3f& affineOffset = nullv3f);

  
  /** Create m4f which vector multiplies to translate points by the move v4f
  \brief
    - create m4f which vector multiplies to translate points by the move v4f
  \details
    - if displacement is not a vector (0 in w), this is ignored and simplified
  \param displacement
    - reference to v4f describing the desired translation vector
  \return
    - m4f with linear <x|u,y|v,z|n> identity, and <w|t> col of displacement
  */
  m4f TranslateAffine(const v3f& displacement);

  
  /** Get a new m4f from an existing one with only the linear part intact
  \brief
    - get a new m4f from an existing one with only the linear part intact
  \param source
    - existing, presumably affine, transformation to discard translation from
  \return
    - linear 3D <x,y,z> components of eaxh matrix row / col set into a new m4f
  */
  m4f LinearAffine(const m4f& source);

  
  /** Take an existing linear matrix and displace the center of transformation
  \brief
    - take an existing linear matrix and displace the center of transformation
  \details
    - you are expected to put a linear transformation in linear: not validated
  \param linear
    - the transformation happening presently, centered in world coordinates
  \param displacement
    - offset between the world origin and the desired center of transformation
  \return
    - affine m4f with linear transformation, centered about the displacement
  */
  m4f AffineMat(const m4f& linear, const v3f& displacement);

  
  /** Concatenate an existing set of scale, rotate and translation matrices
  \brief
    - concatenate an existing set of scale, rotate and translation matrices
  \param scale
    - affine scale matrix describing the dimensions of the transformation
  \param rotate
    - affine rotation matrix describing the orientation of the transformation
  \param translate
    - affine movement matrix describing the position of the transformation
  \return
    - concatenated transform matrix placing objects according to the sum inputs
  */
  m4f TransformAffine(const m4f& scale, const m4f& rotate,
    const m4f& translate);



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Operator Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Scale each matrix element by scalar, setting the product into this
  \brief
    - scale each matrix element by scalar, setting the product into this
  \param scalar
    - scalar amount by which to multiply each matrix element
  \param rhs
    - matrix to be scaled member-wise by the scalar amount
  \return
    - copy of the rhs m4f scaled member-wise by scalar
  */
  m4f operator*(f4 scalar, const m4f& rhs);


  /** Confirm if 2 m4f's are relatively equal across all matrix elements
  \brief
    - confirm if 2 m4f's are relatively equal across all matrix elements
  \param lhs
    - left-hand side to be compared for equality against the right-hand side
  \param rhs
    - right-hand side to be compared for equality against the left-hand side
  \return
    - true if all matrix elements are equal, false if any are not
  */
  bool operator==(const m4f& lhs, const m4f& rhs);


  /** Confirm if 2 m4f's are relatively inequal in any matrix element
  \brief
    - confirm if 2 m4f's are relatively inequal in any matrix element
  \param lhs
    - left-hand side to be compared for inequality against the right-hand side
  \param rhs
    - right-hand side to be compared for inequality against the left-hand side
  \return
    - true if any matrix elements is inequal, false if none are
  */
  bool operator!=(const m4f& lhs, const m4f& rhs);

  
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
  std::ostream& operator<<(std::ostream& output, const m4f& matrix);

  static_assert(sizeof(m4f) == 16*4, "m4f size not packed");

} // end dp namespace
