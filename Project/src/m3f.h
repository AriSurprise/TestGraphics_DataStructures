/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  m3f.h
Purpose:  2D 3x3, column major matrix; inputs / outputs in row major
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
  therefore v3f based constructors operate column major to facilitate this, and
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
#include "v3f.h"        // Member requires exposed visibility to instantiate
#include "vSpace.h"     // Argument default values for planes, 2D & 3D coords


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  class m4f; // Data promotion from 3x3 to 4x4 data (using homogeneous <x,y,z>)

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class m3f //! Square 2 dimensional 3 by 3, column major matrix
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    static const u1 SQUBS = 3u; //! Square matrix has [3] vec[3] cols of f4s

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set matrix from 4 column vectors (col major ordered input -> result)
    \brief
      - set matrix from 4 column vectors (col major ordered input -> result)
    \param U
      - 1st column of transformation basis: U-axis (local x-axis)
    \param V
      - 2nd column of transformation basis: V-axis (local y-axis)
    \param NT
      - 3rd column axis of basis: N (linear z-axis) or T (affine translation)
    */
    m3f(const v3f& U, const v3f& V, const v3f& NT);

    /** Set from 3x3 matrix setting all members manually (row major ordered)
    \brief
      - set from 3x3 matrix setting all members manually (row major ordered)
    \param r1c1
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param r1c2
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param r1c3
      - X (1st) component of the n vector (3rd column): col-major [2][0] value
    \param r2c1
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param r2c2
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    \param r2c3
      - Y (2nd) component of the n vector (3rd column): col-major [2][1] value
    \param r3c1
      - Z (3rd) component of the u vector (1st column): col-major [0][2] value
    \param r3c2
      - Z (3rd) component of the v vector (2nd column): col-major [1][2] value
    \param r3c3
      - Z (3rd) component of the n vector (3rd column): col-major [2][2] value
    \param colMajor
      - if enabled, transposes the matrix data for column major input values
    */
    m3f(f4 r1c1, f4 r1c2, f4 r1c3,
         f4 r2c1, f4 r2c2, f4 r2c3,
         f4 r3c1, f4 r3c2, f4 r3c3,
         bool colMajor = false);


    /** Set 3x3 from matrix's 2D linear and affine members (row major)
    \brief
      - set 3x3 from matrix's 2D linear and affine members (row major)
    \details
      - no affine transpose option: introduces high likelihood of input error
        since <tx, ty> elements would move to bottom row, producing odd results
    \param ux
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param vx
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param tx
      - X (1st) component of the t vector (3rd column): col-major [2][0] value
    \param uy
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param vy
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    \param ty
      - Y (2nd) component of the t vector (3rd column): col-major [2][1] value
    */
    m3f (f4 ux, f4 vx, f4 tx,
          f4 uy, f4 vy, f4 ty);

    
    /** Set 3x3 from only matrix's 2D linear members (row major)
    \brief
      - set 3x3 from only matrix's 2D linear members (row major)
    \param ux
      - X (1st) component of the u vector (1st column): col-major [0][0] value
    \param vx
      - X (1st) component of the v vector (2nd column): col-major [1][0] value
    \param uy
      - Y (2nd) component of the u vector (1st column): col-major [0][1] value
    \param vy
      - Y (2nd) component of the v vector (2nd column): col-major [1][1] value
    \param colMajor
      - if enabled, transposes the matrix data for column major input values
    */
    m3f (f4 ux, f4 vx,
          f4 uy, f4 vy,
          bool colMajor = false);


    /** Set 3x3 matrix from signed, exclusively integer literals (row major)
    \brief
      - set 3x3 matrix from signed, exclusively integer literals (row major)
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
    \param colMajor
      - if enabled, transposes the matrix data for column major input values
    */
    m3f (s4 ux, s4 vx, s4 nx,
          s4 uy, s4 vy, s4 ny,
          s4 uz, s4 vz, s4 nz,
          bool colMajor = false);


    /** Set from 3x3 matrix's members with double precision truncated values
    \brief
      - set from 3x3 matrix's members with double precision truncated values
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
    \param colMajor
      - if enabled, transposes the matrix data for column major input values
    */
    m3f (f8 ux, f8 vx, f8 nx,
          f8 uy, f8 vy, f8 ny,
          f8 uz, f8 vz, f8 nz,
          bool colMajor = false);

    
    /** Uniform scalar matrix, I3 default: 1's along the trace, 0 elsewhere
    \brief
      - uniform scalar matrix, I3 default: 1's along the trace, 0 elsewhere
    \param scalar
      - sets each value along the trace (diagonal), to this value, 0 elsewhere
    */
    m3f(f4 scalar = 1.0f);

    
    /** Non-uniform scalar matrix, unique scalars along trace with 0 elsewhere
    \brief
      - non-uniform scalar matrix, unique scalars along trace with 0 elsewhere
    \details
      - unit z scalar default allowing for affine 2D scale, 4 values
    \param x_scale
      - sets scalar value in the x axis; [0][0] subscript value
    \param y_scale
      - sets scalar value in the y axis; [1][1] subscript value
    \param z_scale
      - sets scalar value in the z axis; [2][2] subscript value (unit default)
    */
    m3f(f4 x_scale, f4 y_scale, f4 z_scale = 1.0f);

    
    /** 2D transform matrix: simple 2D transform initialization
    \brief
      - 2D transform matrix: simple 2D transform initialization
    \details
      - ang argument sufficient to distinguish from non-uniform scalar matrix,
        creating the simplest distinct translate matrix initialization possible
        when initial nullang is provided before 2D movement vector values, as
        well as allowing for a default rotation matrix
    */
    m3f (ang z_rot, f4 x_move = 0.0f, f4 y_move = 0.0f,
          f4 x_scale = 1.0f, f4 y_scale = 1.0f);


    /** Create a copy of another m3f from copied row / column data values
    \brief
      - create a copy of another m3f from copied row / column data values
    \param source
      - existing m3f to have it's row / column values copied into the new m3f
    */
    m3f(const m3f& source);


    /** Create this m3f from a function result's row / column data values
    \brief
      - create this m3f from a function result's row / column data values
    \param result
      - m3f function return to have it's row / column values stolen by this
    */
    m3f(m3f&& result) noexcept;

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
      - to avoid recalculation, pass in an &f4 to set det() into (of this m3f)
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& ToAdjoint(f4* determinant = nullptr);


    /** Get inverse matrix without scale by 1/det: transposed cofactor matrix
    \brief
      - get inverse matrix without scale by 1/det: transposed cofactor matrix
    \details
      - transposed cofactor sometimes preferable for showing clean values
    \param determinant
      - to avoid recalculation, pass in an &f4 to set det() into (of this m3f)
    \return
      - new m3f holding sign adjusted minor determinants in each subscript
    */
    m3f AdjointMat(f4* determinant = nullptr) const;


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
      - new m3f holding sign adjusted minor determinants in each subscript
    */
    m3f CofactorMat(f4* determinant = nullptr) const;


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


    /** Calculate and return the opposite transformation described by the m3f
    \brief
      - calculate and return the opposite transformation described by the m3f
    \details
      - calculates the m3f which inverts the basis change of the current m3f
    \return
      - inverse transformation m3f from present transformation description
    */
    m3f Inverse(void) const;


    /** Set this matrix to the inverse matrix of what it was set to
    \brief
      - set this matrix to the inverse matrix of what it was set to
    \details
      - sets this to the m3f which undoes the current transformations
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& Invert(void);


    /** True if the bottom row is [0, 0, 1], a 2D homogeneous affine transform
    \brief
      - true if the bottom row is [0, 0, 1], a 2D homogeneous affine transform
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - true if the bottom row is [0, 0, 1], false if any member differs
    */
    inline bool IsAffine(void) const
    {
      return IsInF(col[0][2], _INf, -_INf)
          && IsInF(col[1][2], _INf, -_INf)
          && NearF(col[2][2], If);
    } // end bool m3f::IsAffine(void) const                                   */


    /** True if null above and below the trace (all data in trace; diagonal)
    \brief
      - true if null above and below the trace (all data in trace; diagonal)
    \details
      - useful for state validation and shortcut math for simpler matrix data
    \return
      - true if approximately 0 values in elements with (row != column)
    */
    inline bool IsDiagonal(void) const
    {
      return IsLowerTriangular() && IsUpperTriangular();
    } // end bool m3f::IsDiagonal(void) const                                 */


    /** True if the diagonal elements are each 1.0, while all others are 0.0
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
    } // end bool m3f::IsIdentity(void) const                                 */


    /** True if diagonal along <x,y> are 1 and others in <x,y,z> are 0
    \brief
      - true if diagonal along <x,y> are 1 and others in <x,y,z> are 0
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - true only if diagonal along <x,y,z> are 1 and others in <x,y,z> are 0
    */
    inline bool IsIdentityLinear(void) const
    {
      return       NearF(col[0][0], If)     && IsInF(col[1][0], _INf, -_INf)
          && IsInF(col[0][1], _INf, -_INf)  &&      NearF(col[1][1], If);
    } // end bool m3f::IsIdentityLinear(void) const                           */


    /** True if <T> (3rd row & column, mutually) are roughly equal to [0, 0, 1]
    \brief
      - true if <T> (3rd row & column, mutually) are roughly equal to [0, 0, 1]
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - whether the m3f is affine & <t> vector (3rd column) is [0, 0, 1]
    */
    inline bool IsLinear(void) const
    {
      return IsInF(col[2][0], _INf, -_INf)  // [*] [*] [0]
          && IsInF(col[2][1], _INf, -_INf)  // [*] [*] [0]
          && IsAffine();                    // [0] [0] [1]
    } // end bool m3f::IsLinear(void) const                                   */


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
      return IsInF(col[1][0], _INf, -_INf)  // [*] [0] [0]
          && IsInF(col[2][0], _INf, -_INf)  // [*] [*] [0]
          && IsInF(col[2][1], _INf, -_INf); // [*] [*] [*]
    } // end bool m3f::IsLowerTriangular(void) const                          */


    /** True if all columns are length 1.0 & perpendicular to the other two
    \brief
      - true if all columns are length 1.0 & perpendicular to the other two
    \details
      - validation criteria for normalized basis vectors, describing rotation
    \return
      - true only if all 3 vectors are both mutually perpendicular & length 1.0
    */
    inline bool IsOrthnormal(void) const
    {
      return NearF(col[0].LengthSquared(), If)
          && NearF(col[1].LengthSquared(), If)
          && NearF(col[2].LengthSquared(), If)
          && IsInF(col[0].Dot(col[1]), _INf, -_INf)
          && IsInF(col[0].Dot(col[2]), _INf, -_INf)
          && IsInF(col[1].Dot(col[2]), _INf, -_INf);
    } // end bool m3f::IsOrthonormal(void) const                              */


    /** True if determinant is 0, trace is less than dimensions but non 0
    \brief
      - true if determinant is 0, trace is less than dimensions but non 0
    \return
      - true only if determinant is 0, trace is less than dimensions but non 0
    */
    //bool IsProjection(void) const; // TODO: later
    
    
    /** True if no matrix cell elements contain a non-real value: no nan or inf
    \brief
      - true if no matrix cell elements contain a non-real value: no nan or inf
    \details
      - important for matrix data validation, especially in iterative processes
    \return
      - true only if all elements are storing only real number values: non-nan
    */
    inline bool IsReal(void) const
    {
      return (col[0].IsReal() && col[1].IsReal() && col[2].IsReal());
    } // end bool m3f::IsReal(void) const                                     */


    /** True if linear determinant is -1 (scale inverted)
    \brief
      - true if determinant is 0, trace is less than dimensions but non 0
    \return
      - true only if determinant is 0, trace is less than dimensions but non 0
    */
    //bool IsReflection(void) const;


    /** If true, linear determinant is 1 (scale preserved), trace =: 2*cos(t)
    \brief
      - if true, linear determinant is 1 (scale preserved), trace =: 2*cos(t)
    \details
      - computationally, better to use transpose == inverse peroperty:
        take transpose, multiply, check for identity
    \return
      - true only if linear determinant is 1 (scale preserved), trace 2*cos(t)
    */
    inline bool IsRotation(void) const
    {
      return (Transposed() * *this).IsIdentity();
    } // end bool IsRotation(void) const                                      */
    
    
    /** True if linear elements not on the trace are 0, and element [3][3] is 1
    \brief
      - true if linear elements not on the trace are 0, and element [3][3] is 1
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - true if only all elements with inequal cow & column subscripts equal 0
    */
    inline bool IsScalar(void) const
    {
      return IsInF(col[0][1], _INf, -_INf) && IsInF(col[0][2], _INf, -_INf)
          && IsInF(col[1][0], _INf, -_INf) && IsInF(col[1][2], _INf, -_INf)
          && IsInF(col[2][0], _INf, -_INf) && IsInF(col[2][1], _INf, -_INf);
    } // end bool m3f::IsScalar(void) const                                   */


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
      return (NearF((determinant ? *determinant:Det()), Of));
    } // end bool m3f::IsSingular(f4*) const                                  */


    /** True when the transpose is equal to negative of the original matrix
    \brief
      - true when the transpose is equal to negative of the original matrix
    \details
      - useful for validation and shortcut math for simpler matrix data
    \return
      - if the trace is 0 & elements across diagonal are all negative equal
    */
    inline bool IsSkewSymmetric(void) const
    {
      // Check upper triangle elements against lower triangle counterparts
      return !NearF(Trace(), Of) // Trace must also be 0 for any -x == x
          &&  NearF(col[1][0], -col[0][1])
          &&  NearF(col[2][0], -col[0][2])
          &&  NearF(col[2][1], -col[1][2]);
    } // end bool m3f::IsSkewSymmetric(void) const                            */


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
      return NearF(col[1][0], col[0][1])
          && NearF(col[2][0], col[0][2])
          && NearF(col[2][1], col[1][2]);
    } // end bool m3f::IsSymmetrical(void) const                              */


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
      return (IsAffine() && IsIdentityLinear());
    } // end bool m3f::IsTranslation(void) const                              */


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
      return  (IsInF(col[0][1], _INf, -_INf)   // [*] [*] [*]
            && IsInF(col[0][2], _INf, -_INf)   // [0] [*] [*]
            && IsInF(col[1][2], _INf, -_INf)); // [0] [0] [*]

    } // end bool m3f::IsUpperTriangular(void) const                          */


    /** JSON formatted string containing each member value in row major format
    \brief
      - JSON formatted string containing each member value in row major format
    \return
      - string containing packed tightly ascii copy of contained matrix data
    */
    std::string JSON(void) const;


    /** Create a copy of the matrix's 2D transformations set in a 3D matrix
    \brief
      - create a copy of the matrix's 2D transformations set in a 3D matrix
    \return
      - homogeneously promoted 3D transformation with null affine adjustments
    */
    m3f Linear(void) const;


    /** Promote this m3f's data to the linear <x,y,z> elements of a 4x4 matrix
    \brief
      - promote this m3f's data to the linear <x,y,z> elements of a 4x4 matrix
    \return
      - m4f with this m3f's linear data, homogeneous [3][3] =: 1 & other w =: 0
    */
    m4f M4(void) const;


    /** Calculate and return the determinant of the m3f without row / col
    \brief
      - calculate and return the determinant of the m3f without row / col
    \param row
      - row subscript in [0,3] range to disclude from determinant calculation
    \param col
      - column subscript from [0,3] to disclude from determinant calculation
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
    m3f MinorMat(f4* determinant = nullptr) const;


    /** Set a m3f column / row data to another source's column / row values
    \brief
      - set a m3f column / row data to another source's column / row values
    \param source
      - matrix to have its data members copied into this matrix
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& Set(const m3f& source);


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
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& Transpose(void);


    /** Get a copy of this m3f with swapped contents for rows and columns
    \brief
      - get a copy of this m3f with swapped contents for rows and columns
    \return
      - copy of this m3f with swapped row and column data fields
    */
    m3f Transposed(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set a m3f column / row data to another source's column / row values
    \brief
      - set a m3f column / row data to another source's column / row values
    \param source
      - matrix to have its data members copied into this matrix
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& operator=(const m3f& source);


    /** Set this m3f from a function result's row / column data values
    \brief
      - set this m3f from a function result's row / column data values
    \param result
      - returned m3f to have it's row / column values stolen by this
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& operator=(m3f&& result) noexcept;


    /** Get a copy of the matrix with memberwise negated elements
    \brief
      - get a copy of the matrix with memberwise negated elements
    \return
      - copy of existing matrix data with negated values memberwise
    */
    m3f operator-(void) const;


    /** Get the inverse of the current matrix data set (reverse transform)
    \brief
      - get the inverse of the current matrix data set (reverse transform)
    \return
      - matrix undergoing the reverse change of basis from this matrix
    */
    m3f operator!(void) const;


    /** Get a column v3f from the m3f's col[0-3] values: <0:u, 1:v, 2:n|t>
    \brief
      - get a column v3f from the m3f's col[0-3] values: <0:u, 1:v, 2:n|t>
    \param subscript
      - value in [0,2] range to access the corresponding ordered column vector
    \return
      - reference to the column vector subscript: <0:u, 1:v, 2:n|t>
    */
    const v3f& operator[](u4 subscript) const;


    /** Get a column v3f from the m3f's col[0-3] values: <0:u, 1:v, 2:n|t>
    \brief
      - get a column v3f from the m3f's col[0-3] values: <0:u, 1:v, 2:n|t>
    \param subscript
      - value in [0,2] range to access the corresponding ordered column vector
    \return
      - reference to the column vector subscript: <0:u, 1:v, 2:n|t>
    */
    v3f& operator[](u4 subscript);


    /** Calculate the member-wise addition of 2 m3f's, returning the sum
    \brief
      - calculate the member-wise addition of 2 m3f's, returning the sum
    \param other
      - m3f to be added to this matrix's row / column data, member-wise
    \return
      - copy of this m3f with each entry added, member-wise, by other
    */
    m3f operator+(const m3f& other) const;


    /** Add an other m3f's values member-wise, setting the sum into this
    \brief
      - add an other m3f's values member-wise, setting the sum into this
    \param other
      - m3f to be added to this matrix's row / column data, member-wise
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& operator+=(const m3f& other);


    /** Calculate member-wise subtraction of 2 m3f's, returning the difference
    \brief
      - calculate member-wise subtraction of 2 m3f's, returning the difference
    \param other
      - m3f to be subtracted from this matrix's row / column data, member-wise
    \return
      - copy of this m3f with each entry subtracted, member-wise, by other
    */
    m3f operator-(const m3f& other) const;


    /** Subtract an other m3f's values member-wise, setting the difference
    \brief
      - subtract an other m3f's values member-wise, setting the difference
    \param other
      - m3f to be subtracted from this matrix's row / column data, member-wise
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& operator-=(const m3f& other);


    /** Scalar multiplication multiplies each member by scalar constant
    \brief
      - scalar multiplication multiplies each member by scalar constant
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - copy of this m3f, scaled by scalar
    */
    m3f operator*(f4 scalar) const;


    /** Get the concatenated matrix of (this * other): both transformations
    \brief
      - get the concatenated matrix of (this * other): both transformations
    \param other
      - m3f to be right concatenated with this matrix
    \return
      - copy of this m3f, concatenated with the other
    */
    v3f operator*(const v3f& vector) const;


    /** Get the concatenated matrix of (this * other): both transformations
    \brief
      - get the concatenated matrix of (this * other): both transformations
    \param other
      - m3f to be right concatenated with this matrix
    \return
      - copy of this m3f, concatenated with the other
    */
    m3f operator*(const m3f& other) const;


    /** Scale each matrix element by scalar, setting the product into this
    \brief
      - scale each matrix element by scalar, setting the product into this
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& operator*=(f4 scalar);


    /** Matrix multiplication concatenates 2 sequential transformations into 1
    \brief
      - matrix multiplication concatenates 2 sequential transformations into 1
    \param scalar
      - scalar amount by which to multiply each matrix element
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& operator*=(const m3f& other);


    /** Perform division on all members, returning the result
    \brief
      - perform division on all members, returning the result
    \param divisor
      - scalar amount by which to multiply the reciprocal of per matrix element
    \return
      - copy of this m3f, scaled up by (1.0 / inverseScalar)
    */
    m3f operator/(f4 divisor) const;


    /** Division multiplies all members by reciprocal of inverseScalar
    \brief
      - division multiplies all members by reciprocal of inverseScalar
    \param divisor
      - scalar amount by which to multiply the reciprocal of per matrix element
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& operator/=(f4 divisor);


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

    //! Column major vector list of <u,v,n> 3D linear or 2D affine transform
    v3f col[SQUBS];

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Calculate and set matrix to minor det matrix with oscillating signs
    \brief
      - calculate and set matrix to minor det matrix with oscillating signs
    \details
      - intermediary inverse step, setting determinant relevant to prior m3f
      Just doesn't make sense to have the set in place premutation public:
      really only used in adjoint / inverse matrix calculations
    \param determinant
      - to avoid recalculation, pass in an f4 to store det() (for former values)
    \return
      - reference to the modified m3f for easier subsequent inline action
    */
    m3f& cofactor(f4* determinant = nullptr);



    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  }; // end m3f struct / class definition



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Constant Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Zero filled m3f in all entries; reserved value for an invalid state
  static const m3f nullm3f (Of, Of, Of, Of, Of, Of, Of, Of, Of);



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Calculate the orthonormal unit-vector set 2 non-parallel directions form
  \brief
    - calculate the orthonormal unit-vector set 2 non-parallel directions form
  \param primary
    - most relevant direction to forming the perpendicular basis orientation
  \param secondary
    - direction framing 2D plane of 3D space primary axis' orientation is in
  \param formed
    - any given plane gives intended local plane formed by primary & secondary
  \return
    - basis of 3, unit-length, perpendicular, column vectors from given input
  */
  m3f OrthonormalBasis(const v3f& primary, const v3f& secondary = dp::Y_HAT,
    PLANE formed = PLANE::UNDEFINED);


  /** Calculate the dimensional covariance symmetrical matrix of 3D point list
  \brief
    - calculate the dimensional covariance symmetrical matrix of 3D point list
  \details
    - where statistical (population) variance takes average squared distance
    from the mean, with multidimensional data we can compare 2 axes' averaged
    mean-distance products (i.e. =: row * col), instead of keeping that data's
    square term as constant, to get their relative rate of change from mean
    with respect to each other.  the 'square' covariance can be analyzed to
    find the rates of change of a dataset, including eigen value extraction per
    this framework's applications, using eigen vectors & value to find a basis
    of statistically analyzed data spread over the set by iterative use of the
    Jacobi method to diagonalize the symmetrical data.
  \param datapoint
    - address of head for 3D point sample list
  \param setSize
    - [1, u4_MAX] total available, samplable datapoint list size
  \param samples
    - [1, setSize] sampled datapoints from list to be taken in scan
  \param step
    - [-setSize/4, 0),(0, setSize/4] point samples to be skipped per scan step
  \param first
    - [1, setSize/2] point samples to offset by in first scan iteration
  \return
    - dimensional products of average point list mean differences per row * col
  */
  m3f Covariance3D(const v3f* data, u4 setSize, u4 samples, s2 step, u2
    first);


  /** Get the orthonormal basis (& scalars), for a dataset's covariance matrix
  \brief
    - get the orthonormal basis (& scalars), for a dataset's covariance matrix
  \details
    - Use the PCA method of iterative Jacobi rotations to find eigen-vectors
  \param covariance
    - mean variance changes between dimension pairs as read from 3D dataset
  \param scalars
    - pass in value to store PCA method resultant, per vector, eigen-value set
  \return
    - bounding sphere initially from statistic analysis of point spread, grown
  */
  m3f PCA3(const m3f& covariance, v3f* scalars = nullptr);


  /** Get iterative 3D rotations about world axis vectors in <z,y,x> ordering
  \brief
    - get iterative 3D rotations about world axis vectors in <z,y,x> ordering
  \param z_rot
    - rotational arc displacement about z-axis, in (column of) all XY planes
  \param y_rot
    - rotational arc displacement about y-axis, in (column of) all XZ planes
  \param x_rot
    - rotational arc displacement about x-axis, in (column of) all YZ planes
  \return
    - linear 3D rotation matrix about z (in XY), then y (in XZ), then x (in YZ)
  */
  m3f RotateEuler(ang z_rot, ang y_rot, ang x_rot);


  /** Create m3f with vector product effect creating points scaled by <x, y, z>
  \brief
    - create m3f with vector product effect creating points scaled by <x, y, z>
  \param x_scalar
    - scalar factor for scaling x-coordinates of vectors
  \param y_scalar
    - scalar factor for scaling y-coordinates of vectors
  \param z_scalar
    - scalar factor for scaling z-coordinates of vectors
  \return
    - diagonalized m3f with trace u<x_s, 0, 0>, v<0, y_s, 0>, n<0, 0, z_s>
  */
  m3f Scale3Mat(f4 x_scalar, f4 y_scalar = NAN, f4 z_scalar = NAN);


  /** Create m3f with vector product effect creating points scaled by <x, y, z>
  \brief
    - create m3f with vector product effect creating points scaled by <x, y, z>
  \param scales
    - per dimensional triplet of scalar factors for scaling multiplicands
  \return
    - diagonalized m3f with trace u<s.x, 0, 0>, v<0, s.y, 0>, n<0, 0, s.z>
  */
  inline m3f ScaleMat(const v3f& scales)
  {
    return Scale3Mat(scales.x, scales.y, scales.z);
  } // end m3f ScaleMat(const v3f&)                                           */

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
    - copy of the rhs m3f scaled member-wise by scalar
  */
  m3f operator*(f4 scalar, const m3f& rhs);


  /** Confirm if 2 m3f's are relatively equal across all matrix elements
  \brief
    - confirm if 2 m3f's are relatively equal across all matrix elements
  \param lhs
    - left-hand side to be compared for equality against the right-hand side
  \param rhs
    - right-hand side to be compared for equality against the left-hand side
  \return
    - true if all matrix elements are equal, false if any are not
  */
  bool operator==(const m3f& lhs, const m3f& rhs);


  /** Confirm if 2 m3f's are relatively inequal in any matrix element
  \brief
    - confirm if 2 m3f's are relatively inequal in any matrix element
  \param lhs
    - left-hand side to be compared for inequality against the right-hand side
  \param rhs
    - right-hand side to be compared for inequality against the left-hand side
  \return
    - true if any matrix elements is inequal, false if none are
  */
  bool operator!=(const m3f& lhs, const m3f& rhs);

  
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
  std::ostream& operator<<(std::ostream& output, const m3f& matrix);

} // end dp namespace
