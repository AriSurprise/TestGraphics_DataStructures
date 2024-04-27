/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  mNf.cpp
Purpose:  2D NxN, column major square matrix; inputs / outputs in row major
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_MAT320_1
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

#include "Value.h"  // Basic numerical operations: clamp, swap, min, max, etc
#include "mNf.h"  // Associated class declaration header
#include "Log.h"    // Error / warning output message / logging

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set matrix from column vectors (if col major ordered input == result)      */
dp::mNf::mNf(const std::vector<vNf>& basis, Cells2D order) :
  cell{basis}
{
  u4 max = Dimens();
  if (max == 0)
  {
    cell.resize(1);
    cell[0].Resize(1);
    cell[0][0] = 1.0f;
  }
  else
  {
    // Find dimensions of input (assume potential for ragged arrays)
    for (u4 i = 0; i < Dimens(); ++i)
    {
      if (cell[i].Dimens() > max) { max = cell[i].Dimens(); }
    }
    // Set column count to max size found in input data
    if (max > Dimens()) { cell.resize(max); }
    for (u4 i = 0; i < Dimens(); ++i) // Per column...
    {
      // Extend input given as needed
      if (cell[i].Dimens() < max) { cell[i].Resize(max); }
    }
    if (order == Cells2D::RowMajor) { Transpose(); }
  }
} // end mNf(const vector<vNf>&, Cells2D)


// Set matrix from manual input, presumably row major (needing transpose)     */
dp::mNf::mNf(std::initializer_list<std::initializer_list<f8>> data,
  Cells2D order)
{
  u4 max = static_cast<u4>(data.size()), row;
  // Scan data for requisite size (ragged input allowable, but not internally)
  for (u4 i = 0; i < data.size(); ++i)
  {
    if (static_cast<u4>(data.begin()[i].size()) < max)
    {
      max = static_cast<u4>(data.begin()[i].size());
    }
  }
  cell.resize(max);
    u4 i, j;
  if (order == Cells2D::RowMajor) for (i = 0; i < max; ++i)
  {
    cell[i].Resize(max);
    row = static_cast<u4>(data.begin()[i].size());
    for (j = 0; j < row; ++j)
    {
      cell[j][i] = static_cast<f4>(data.begin()[i].begin()[j]);
    }
    for (; j < max; ++j)
    {
      cell[j][i] = 0.0f;
    }
  }
  else for (i = 0; i < max; ++i)
  {
    cell[i].Resize(max);
    row = static_cast<u4>(data.begin()[i].size());
    for (j = 0; j < row; ++j)
    {
      cell[i][j] = static_cast<f4>(data.begin()[i].begin()[j]);
    }
    for (; j < max; ++j)
    {
      cell[i][j] = 0.0f;
    }
  }

} // end mNf(initializer_list<initializer_list<f8>>&, Cells2D)


// Create an N dimensional identity matrix: 1's in the trace, 0 elsewhere     */
dp::mNf::mNf(u1 dimens, f4 scalar) : cell(dimens)
{
  // Initialize to identity / standard basis for given dimensions
  for (size_t c = 0; c < Dimens(); ++c) // Per column...
  {
    cell[c].Resize(dimens); // Set 0 in each row entry of column elements
    cell[c][c] = scalar; // Overriding scalar identity value in trace element
  }
} // end mNf(u1)


// Create a copy of another matrix from copied row / column data values       */
dp::mNf::mNf(const mNf& source) : cell(source.cell) { }


// Create this matrix from a function result's row / column data values       */
dp::mNf::mNf(mNf&& result) noexcept : cell(std::move(result.cell)) { }



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set to unscaled (by 1/det) inverse matrix; transposed cofactor matrix      */
dp::mNf& dp::mNf::ToAdjoint(f4* determinant)
{
  CofactorMat(determinant);
  Transpose();
  return *this;

} // end mNf mNf::ToAdjoint(f4*)

// Calculate inverse m4f without scale by 1/det; transposed cofactor matrix  */
dp::mNf dp::mNf::AdjointMat(f4* determinant) const
{
  mNf adjugate(*this);
  adjugate.ToAdjoint(determinant);
  return adjugate;

} // end mNf mNf::AdjointMat(f4*) const

// Calculate and return minor determinant matrix with oscillating signs       */
dp::mNf dp::mNf::CofactorMat(f4* determinant) const
{
  mNf cofactors(MinorMat(determinant));
  // Convert minor matrix to cofactor matrix
  for (u1 i = 0; i < Dimens(); ++i)
  {
    // Start on even subscripted columns' [1] subscript; odd columns on [0]
    for (u1 j = (i & 1) ? 0 : 1; j < Dimens(); j += 2)
    {
      // Advancing by 2 subscripts, negate every (other), matrix entry
      cofactors[i][j] = -cofactors[i][j];
    }
  }
  return cofactors;

} // end mNf mNf::CofactorMat(f4* determinant) const


// Calculate and return the determinant of the matrix                         */
f4 dp::mNf::Det(void) const
{
  if (IsUpperTriangular() || IsLowerTriangular())
  {
    return Trace(Oper::Product);
  }
  // TODO: scan for easiest row / col to process by totaling 0 entries
  
  // Calculate determinant...
  f4 det;
  switch (Dimens())
  {
  case 0: // degeneratively
    det = 0.0f;
    break;
  case 1: // trivially
    det = cell[0][0];
    break;
  case 2: // at base
    det = cell[0][0] * cell[1][1] - cell[0][1] * cell[1][0];
    break;
  case 3: // in 3x3 case, algebra still being managable enough
    det =
    cell[0][0] * cell[1][1] * cell[2][2] + cell[1][0] * cell[2][1]
  * cell[0][2] + cell[2][0] * cell[0][1] * cell[1][2] - cell[0][2]
  * cell[1][1] * cell[2][0] - cell[1][2] * cell[2][1] * cell[0][0]
  - cell[2][2] * cell[0][1] * cell[1][0];
    break;
  default: // In larger cases, Gaussian elim. succinct over cofactor recursion
    Solution(emptySet, Elim::Gaussian, &det);
    break;
  }
  return det;
} // end f4 mNf::Det(void) const


// Get the N value / square dimensions of the NxN matrix data in use
u4 dp::mNf::Dimens(void) const
{
  return static_cast<u4>(cell.size());

} // end size_t mNf::Dimens(void) const


// Calculate and return the opposite transformation described by the m4f     */
dp::mNf dp::mNf::Inverse(f4* determinant) const
{
  // Check if user input invalidates attempt
  if (determinant && *determinant == 0.0f) { return nullmat; }
  // Pass through storage of determinant calculation
  f4 det;
  // Don't trust user input: cofactor has to calculate it incidentally anyhow

  // Get cofactor matrix (from minor determinant matrix: internal to cofactor)
  mNf inv(CofactorMat(&det));

  // Confirm the matrix is invertible
  if (IsSingular(&det)) { return nullmat; }

  // Convert cofactor matrix to adjugate
  inv.Transpose();

  // Scale adjugate matrix to inverse matrix
  f4 scalar = 1.0f / det;
  inv *= scalar;
  return inv;

} // end mNf mNf::Inverse(void)


// Set this matrix to the inverse matrix of what it was set to                */
dp::mNf& dp::mNf::Invert(void)
{
  // No-op preferable if matrix is singular...?
  return (IsSingular() ? *this : (Set(Inverse())));
}
// end mNf& mNf::Invert(void)


// True if the diagonal elements are 1, while all others are 0                */
bool dp::mNf::IsIdentity(void) const
{
  return (Trace() == Dimens() && Trace(Oper::Product) == 1.0f
    && IsUpperTriangular() && IsLowerTriangular());

} // end bool mNf::IsIdentity(void) const


// True if all elements above the trace / diagonal are 0 value elements       */
bool dp::mNf::IsLowerTriangular(void) const
{
  bool result = true;
  // Validate that each element above the trace is null
  for (size_t c = 0; c < Dimens(); ++c) // In each column vector...
  {
    for (size_t r = 0; r < c; ++r) // check each row element up to the trace
    {
      // If any non-zero element is found, shortcut out: sum result is false
      if (!NearF(cell[c][r], 0.0f, 0.0000001f)) { result = false; break; }
    }
    if (!result) { break; }
  }
  return result;

} // end bool mNf::IsLowerTriangular(void) const


// True iff the matrix is non-invertible                                      */
bool dp::mNf::IsSingular(f4* determinant) const
{
  return (NearF( ((determinant) ? *determinant: Det()), 0.0f, _INf));
} // end bool mNf::IsSingular(f4*) const


// True if all elements below the trace / diagonal are 0 value elements       */
bool dp::mNf::IsUpperTriangular(void) const
{
  bool result = true;
  // Validate that each element below the trace is null
  for (size_t c = 0; c < Dimens(); ++c) // In each column vector...
  {
    for (size_t r = c+1; r < Dimens(); ++r) // check each row below the trace
    {
      // If any non-zero element is found, shortcut out: sum result is false
      if (!NearF(cell[c][r], 0.0f, _INf)) { result = false; break; }
    }
    if (!result) { break; }
  }
  return result;

} // end bool mNf::IsUpperTriangular(void) const


// JSON formatted string containing each member value in row major format     */
std::string dp::mNf::JSON(void) const
{
  mNf mt(this->Transposed());
  std::string result("[");
  u4 last = static_cast<u4>(Dimens() - 1);
  for (u4 i = 0; i < last; ++i)
  {
    result += mt[i].CSVString() + ",";
  }
  result += mt[last].CSVString() + "]";
  return result;

} // end std::string mNf::JSON(void) const


// Get the matrix holding this matrix's data, exculding the given row / col   */
dp::mNf dp::mNf::Minor(u1 row, u1 col) const
{
  mNf result(Dimens() - 1);

  // Validate input
  row %= Dimens(); col %= Dimens();

  for (um c = 0; c < col; ++c) // Copy column data up to excluded column
  {
    // Populating row data up to the excluded row
    for (um r = 0; r < row; ++r)
    {
      result.cell[c][r] = cell[c][r];
    }
    // and then after that row
    for (um r = (um)row+1; r < Dimens(); ++r)
    {
      result.cell[c][r-1] = cell[c][r];
    }
  }
  for (um c = (um)col+1; c < Dimens(); ++c) // Then after that column
  {
    // Populating row data up to the excluded row
    for (um r = 0; r < row; ++r)
    {
      result.cell[c-1][r] = cell[c][r];
    }
    // and then after that row
    for (um r = (um)row+1; r < Dimens(); ++r)
    {
      result.cell[c-1][r-1] = cell[c][r];
    }
  }
  return result;

} // end mNf mNf::Minor(u1, u1) const


// Calculate and return the determinant of the m3f without row / col         */
f4 dp::mNf::MinorDet(u1 row, u1 col) const
{
  return Minor(row, col).Det();

} // end f4 mNf::MinorDet(u1, u1) const


// Calculate and return the matrix comprised of the minor determinants        */
dp::mNf dp::mNf::MinorMat(f4* determinant) const
{
  mNf minors(Dimens());
  // Calculate the determinant for each element's minor matrix
  for (u4 c = 0; c < Dimens(); ++c)
  {
    for (u4 r = 0; r < Dimens(); ++r)
    {
      // Row-major input: (row, col) input arg list format sees col-major flip
      minors.cell[c][r] = MinorDet(r, c);
    }
  }

  // If an f4* was given, set the det() into it to save on recalculation
  if (determinant)
  {
    *determinant = 0.0f;
    // Sum the product of matrices even subscripts
    for (u4 c = 0; c < Dimens(); c += 2)
    {
      *determinant += cell[c][0] * minors[c][0];
    }
    // Subtract the sum of products of matrices odd subscripts
    for (u4 c = 1; c < Dimens(); c += 2)
    {
      *determinant -= cell[c][0] * minors[c][0];
    }
  }
  return minors;

} // end mNf mNf::MinorMat(f4*) const


// Set the matrix column / row dimensions to a new N square element count     */
dp::mNf& dp::mNf::Resize(u4 squareDimensions, f4 traceVal, f4 elseVal)
{
  if (squareDimensions != Dimens())
  {
    cell.resize(squareDimensions);
    for (u4 c = 0; c < Dimens(); ++c)
    {
      cell[c].Resize(Dimens(), elseVal);
      cell[c][c] = traceVal;
    }
  }
  return *this;
}

// Set a mNf column / row data to another source's column / row values        */
dp::mNf& dp::mNf::Set(const mNf& source)
{
  // Resize to source matrix's column count
  cell.resize(source.Dimens());
  // Copy each column vector (no other members should be defined in matrices)
  for (u1 c = 0; c < Dimens(); ++c) { cell[c] = source.cell[c]; }
  // (vNf set operator should internally maintain per column resize operation)
  // (source mNf already had to validate internally uniform size per column)

  return *this;

} // end mNf& mNf::Set(const mNf&)


// Perform row elimination on the (augmentable), matrix to get a solution set */
dp::BasisShiftNf dp::mNf::Solution(const std::vector<vNf>& aug, Elim method,
  f4* det) const
{
  // Create augmented matrix of this basis with any other N-D data sets
  BasisShiftNf result(*this, aug);
  if (det) { *det = 1.0f; }
  // Simultaneously iterate over basis + augment elements per row
  for (u4 piv = 0; piv < result.SquareDimens(); ++piv)
  {
    // If an element in a pivot location (along trace) is null...
    if (NearF(result.basis[piv][piv], 0.0f, _INf))
    {
      // Find a row to swap with whose equivalent trace elements are non null
      u4 r, b = piv;
      for (r = piv+1; r < result.SquareDimens(); ++r)
      {
        // Candidate must be non-null in the problem pivot column 
        if (!NearF(result.basis[piv][r], 0.0f, _INf))
        {
          b = r;
          // and ideally, pivot should also be non-null in the target row
          if (!NearF(result.basis[r][r], 0.0f, _INf)) { break; }
        }
      }
      // Swap rows memberwise (in both basis + augment), if suitable row found
      if (piv < b && b < result.SquareDimens())
      {
        if (det) { *det = -*det; }
        result.SwapRows(piv, b);
      }
    }
    // Scale each column element for the row to the pivot's reciprocal
    if (det) { *det *= result.basis[piv][piv]; }
    result.ScaleRow(1.0f / result.basis[piv][piv], piv);
    // Perform scalar row subtraction to cancel each row for the pivot column
    for (u4 r = piv+1; r < result.SquareDimens(); ++r)
    {
      result.AddRow(-result.basis[piv][r], piv, r);
    }
  }
  // If bringing to RREF instead of just Gauss-Jordan elimination, cancel up
  if (method == Elim::RowReducedEchelonForm)
  {
    // Perform scalar row subtraction to cancel upper rows for the pivot column
    for (u4 piv = 1; piv < result.SquareDimens(); ++piv)
    {
      if (!NearF(result.basis[piv][piv], 0.0f, _INf))
      {
        for (u4 r = 0; r < piv; ++r)
        {
          result.AddRow(-result.basis[piv][r], piv, r);
        }
      }
    }
  }
  return result;
}


// JSON formatted string containing each member value in row major format     */
std::string dp::mNf::ToString(bool lineBreaks) const
{
  mNf mt(this->Transposed());
  std::string result("[" + mt[0].CSVString(0));
  if (lineBreaks) for (u1 i = 1; i < Dimens(); ++i)
  {
    result += ", \n" + mt[i].CSVString(0);
  }
  else for (u1 i = 1; i < Dimens(); ++i)
  {
    result += ", " + mt[i].CSVString(0);
  }
  result += "]";
  return result;

} // end std::string mNf::ToString(void) const


// Calculate and return sum (or product) of the diagonal elements             */
f4 dp::mNf::Trace(Oper op) const
{
  return (op == Oper::Product) ? (cell[0][0]) * (cell[1][1]) * (cell[2][2])
    * (cell[3][3]) : (cell[0][0]) + (cell[1][1]) + (cell[2][2]) + (cell[3][3]);

} // end f4 mNf::Trace(void) const


// Set the matrix to have swapped contents for rows and columns               */
dp::mNf& dp::mNf::Transpose(void)
{
  // Exchange each upper triangular element with lower triangular partner
  for (size_t c = 0; c < Dimens(); ++c) // In each column vector...
  {
    for (size_t r = 0; r < c; ++r) // Swap each row element up to the trace...
    {
      SwapF(cell[c][r], cell[r][c]); // With its mirrored positional element
    }
  }
  return *this;

} // end mNf& mNf::Transpose(void)


// Get a copy of this m4f with swapped contents for rows and columns         */
dp::mNf dp::mNf::Transposed(void) const
{
  // Quicker to just write and return the copy from existing values
  return mNf(this->cell, Cells2D::RowMajor);

} // end mNf mNf::Transposed(void) const



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set a m4f column / row data to another source's column / row values       */
dp::mNf& dp::mNf::operator=(const mNf& source)
{
  return this->Set(source);

} // end mNf& mNf::operator=(const mNf&)


// Set this m4f from a function result's row / column data values            */
dp::mNf& dp::mNf::operator=(mNf&& result) noexcept
{
  cell = std::move(result.cell);
  return *this;

} // end mNf& mNf::operator=(mNf&&) noexcept


// Calculate the inverse matrix and return the result (no internal change)    */
dp::mNf dp::mNf::operator-(void) const
{
  mNf copy(Dimens());
  for (size_t c = 0; c < Dimens(); ++c) // In each column vector...
  {
    for (size_t r = 0; r < Dimens(); ++r) // Copy each row element, negated
    {
      copy.cell[c][r] = -cell[c][r];
    }
  }
  return copy;

} // end mNf mNf::operator-(void) const


// Get the inverse of the current matrix data set (reverse transform)         */
dp::mNf dp::mNf::operator!(void) const
{
  return this->Inverse();

} // end mNf mNf::operator!(void)


// Get a column v4f from the m4f's cell[0-3] values: <0:u, 1:v, 2:n, 3:t>     */
const dp::vNf& dp::mNf::operator[](u4 subscript) const
{
  subscript %= Dimens();
  return cell[subscript];

} // end const vNf& mNf::operator[](u4) const


// Get a column v4f from the m4f's cell[0-3] values: <0:u, 1:v, 2:n, 3:t>     */
dp::vNf& dp::mNf::operator[](u4 subscript)
{
  subscript %= Dimens();
  return cell[subscript];

} // end vNf& mNf::operator[](u4)


// Calculate the member-wise addition of 2 m4f's, returning the sum          */
dp::mNf dp::mNf::operator+(const mNf& other) const
{
  mNf sum(*this);
  for (u1 i = 0; i < Dimens(); ++i) { sum[i] += other[i]; }
  return sum;

} // end mNf mNf::operator+(const mNf&)


// Add an other m4f's values member-wise, setting the sum into this          */
dp::mNf& dp::mNf::operator+=(const mNf& other)
{
  for (u1 i = 0; i < Dimens(); ++i) { cell[i] += other[i]; }
  return *this;

} // end mNf& mNf::operator+=(const mNf&)


// Calculate member-wise subtraction of 2 m4f's, returning the difference    */
dp::mNf dp::mNf::operator-(const mNf& other) const
{
  mNf difference(*this);
  for (u1 i = 0; i < Dimens(); ++i) { difference[i] -= other[i]; }
  return difference;

} // end mNf mNf::operator-(const mNf&)


// Subtract another m4f's values member-wise, setting the difference         */
dp::mNf& dp::mNf::operator-=(const mNf& other)
{
  for (u1 i = 0; i < Dimens(); ++i) { cell[i] -= other[i]; }
  return *this;

} // end mNf& mNf::operator-=(const mNf&)


// Scalar multiplication multiplies each member by scalar constant            */
dp::mNf dp::mNf::operator*(f4 scalar) const
{
  mNf scaled(*this);
  scaled *= scalar;
  return scaled;

} // end mNf mNf::operator*(f4) const


// Get the concatenated matrix of (this * other), doing both transformations  */
dp::vNf dp::mNf::operator*(const vNf& vector) const
{
  mNf mt(this->Transposed());
  vNf result(Dimens());
  u4 min = MinU(Dimens(), vector.Dimens());
  u4 max = MaxU(Dimens(), vector.Dimens());
  for (u1 r = 0; r < min; ++r) { result[r] = mt[r].Dot(vector); }
  if (min < max) { result.Resize(max); }
  return result;

} // end vNf mNf::operator*(const vNf&) const


// Scale each matrix element by scalar, setting the product into this         */
dp::mNf& dp::mNf::operator*=(f4 scalar)
{
  for (u1 i = 0; i < Dimens(); ++i) { cell[i] *= scalar; }
  return *this;
} // end mNf& mNf::operator*=(f4)


// Get the concatenated matrix of (this * other), doing both transformations  */
dp::mNf dp::mNf::operator*(const mNf& other) const
{
  mNf concat(*this);
  concat *= other;
  return concat;

} // end mNf mNf::operator*(const mNf&) const


// Matrix multiplication concatenates 2 sequential transformations into 1     */
dp::mNf& dp::mNf::operator*=(const mNf& other)
{
  // TODO: use validators to perform shortcut math and optimize
  mNf mc(*this);  // Matrix copy (to not work with shifting values)
  mNf ot = other.Transposed(); // Transposed other (for vector-wise Dot())

  for (u1 i = 0; i < Dimens(); ++i)
  {
    for (u1 j = 0; j < Dimens(); ++j) { cell[i][j] = mc[i] * ot[j]; }
  }
  return *this;

} // end mNf& mNf::operator*=(const mNf&)


// Perform division on all members, returning the result                      */
dp::mNf dp::mNf::operator/(f4 inverseScalar) const
{
  f4 scalar = (1.0f / inverseScalar);
  return ((*this) * scalar);

} // end mNf mNf::operator/(f4) const


// Division multiplies all members by reciprocal of inverseScalar             */
dp::mNf& dp::mNf::operator/=(f4 inverseScalar)
{
  f4 scalar = (1.0f / inverseScalar);
  return ((*this) *= scalar);

} // end mNf& mNf::operator/=(f4)


// Get string of data in matrix cells output as row major data per line       */
dp::mNf::operator std::string(void) const
{ return this->ToString(true); } // end operator std::string(void)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Calculate and set matrix to minor det matrix with oscillating signs        */
dp::mNf& dp::mNf::ToCofactor(f4* determinant)
{
  // Set to minor matrix, track what the determinant was
  *this = MinorMat(determinant);
  // Convert minor matrix to cofactor matrix
  for (u1 i = 0; i < Dimens(); ++i)
  {
    // Start on even subscripted columns' [1] subscript; odd columns on [0]
    for (u1 j = (i & 1) ? 0 : 1; j < Dimens(); j += 2)
    {
      // Advancing by 2 subscripts, negate every (other), matrix entry
      cell[i][j] = -cell[i][j];
    }
  }
  return *this;

} // end mNf& mNf::cofactor(f4*)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Helper Operator Functions                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


// Scale each matrix element by scalar, setting the product into this         */
dp::mNf dp::operator*(f4 scalar, const dp::mNf& rhs)
{
  return (rhs * scalar);

} // end mNf mNf::operator*(const mNf&)


// Confirm if 2 m4f's are relatively equal across all matrix elements        */
bool dp::operator==(const mNf& lhs, const mNf& rhs)
{
  if (lhs.Dimens() != rhs.Dimens()) { return false; } // => not possibly equal
  // Check each column vector for equality
  for (u1 i = 0; i < lhs.Dimens(); ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }
  return true; // If no column was inequal, all were equal, so lhs == rhs

} // end bool operator==(const mNf&, const mNf&) const


// Confirm if 2 m4f's are relatively inequal in any matrix element           */
bool dp::operator!=(const mNf& lhs, const mNf& rhs)
{
  if (lhs.Dimens() != rhs.Dimens()) { return true; } // => must not be equal
  // Check each column vector for inequality
  for (u1 i = 0; i < lhs.Dimens(); ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }
  return false; // If no column was inequal, all were equal, so !(lhs == rhs)

} // end bool operator!=(const mNf&, const mNf&) const


// Output the string version of the vector data: <vector>, (point) or [clip]  */
std::ostream& dp::operator<<(std::ostream& output, const mNf& matrix)
{
  output << matrix.ToString(true);
  return output;

} // end std::ostream& operator<<(std::ostream&, const mNf&)

dp::BasisShiftNf::BasisShiftNf(const mNf& basisSet, const std::vector<vNf>&
  vectorSet) : basis(basisSet), augment(vectorSet)
{
  // Ensure regularity of size across columns
  u4 max = SquareDimens();
  for (u4 c = 0; c < AugmentDimens(); ++c)
  {
    if (max < augment[c].Dimens()) { max = augment[c].Dimens(); }
  }
  if (max > SquareDimens()) { basis.Resize(max); }
  for (u4 c = 0; c < AugmentDimens(); ++c)
  {
    if (max > augment[c].Dimens()) { augment[c].Resize(max); }
  }
}

u4 dp::BasisShiftNf::AugmentDimens(void) const
{
  return static_cast<u4>(augment.size());
}

u4 dp::BasisShiftNf::Columns(void) const
{
  return SquareDimens() + AugmentDimens();
}

u4 dp::BasisShiftNf::SquareDimens(void) const
{
  return basis.Dimens();
}

dp::BasisShiftNf& dp::BasisShiftNf::SwapRows(u4 swap, u4 with)
{
  if (swap >= SquareDimens()) { swap %= SquareDimens(); }
  if (with >= SquareDimens()) { with %= SquareDimens(); }
  if (swap == with) { return *this; }
  for (u4 c = 0; c < SquareDimens(); ++c)
  {
    SwapF(basis[c][swap], basis[c][with]);
  }
  for (u4 c = 0; c < AugmentDimens(); ++c)
  {
    SwapF(augment[c][swap], augment[c][with]);
  }
  return *this;
} // end BasisShiftNf& BasisShiftNf::SwapRows(u4, u4)


// 
dp::BasisShiftNf& dp::BasisShiftNf::ScaleRow(f4 scalar, u4 row)
{
  if (row >= SquareDimens()) { row %= SquareDimens(); }
  for (u4 c = 0; c < SquareDimens(); ++c)
  {
   basis[c][row] *= scalar;
  }
  for (u4 c = 0; c < AugmentDimens(); ++c)
  {
    augment[c][row] *= scalar;
  }
  return *this;
} // end BasisShiftNf& BasisShiftNf::ScaleRow(f4, u4)


// Operate on a matrix base + augment row to add a scaled row into another    */
dp::BasisShiftNf& dp::BasisShiftNf::AddRow(f4 scalar, u4 from, u4 into)
{
  if (from >= SquareDimens()) { from %= SquareDimens(); }
  if (into >= SquareDimens()) { into %= SquareDimens(); }
  for (u4 c = 0; c < SquareDimens(); ++c)
  {
    basis[c][into] += basis[c][from] * scalar;
  }
  for (u4 c = 0; c < AugmentDimens(); ++c)
  {
    augment[c][into] += augment[c][from] * scalar;
  }
  return *this;
} // end BasisShiftNf& BasisShiftNf::AddRow(f4, u4, u4)
