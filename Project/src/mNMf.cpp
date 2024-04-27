/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  mNMf.cpp
Purpose:  2D NxN, column major square matrix; inputs / outputs in row major
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_MAT345_3
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Fall 2022
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

#include "Log.h"    // Error / warning output message / logging
#include "mNMf.h"   // Associated class declaration header
#include "Value.h"  // Basic numerical operations: clamp, swap, min, max, etc
//#include "v2u.h"    // Dimensions access for 2D matrix read?

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::mNMf::mNMf(const vNf& basis, u2 rows)
{
  dimens.row = rows? rows : 1;
  dimens.col = (basis.Dimens() / dimens.row);
  cell.Resize(dimens.Area());
}

dp::mNMf::mNMf(const std::vector<vNf>& basis)
{
} // end mNMf(const vector<vNf>&, u2)


dp::mNMf::mNMf(std::initializer_list<f8> data, u2 rows)
{
  u4 max = static_cast<u4>(data.size());

} // end mNf(initializer_list<f8>, u2)


dp::mNMf::mNMf(const v2u& dimens, f4 scalar)
{
} // end mNf(const v2u&, f4)


dp::mNMf::mNMf(void)
{ } // end mNf(void)


dp::mNMf::mNMf(const mNMf& source) : cell(source.cell)
{ } // end mNMf(const mNMf&)                                                  */


dp::mNMf::mNMf(mNMf&& result) noexcept : cell(std::move(result.cell))
{ } // end mNMf(const mNMf&&) noexcept                                        */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::mNMf dp::mNMf::Copy(void) const
{ return mNMf(*this); }// end mNMf mNMf::Copy(void) const                     */


dp::v2u dp::mNMf::Dimens(void) const
{
  return v2u();

} // end v2u mNMf::Dimens(void) const                                         */


std::string dp::mNMf::JSON(void) const
{
  return "[matrix data]";

} // end std::string mNMf::JSON(void) const                                   */


// Set the matrix column / row dimensions to a new N square element count     */
dp::mNMf& dp::mNMf::Resize(u2 rows, u2 cols)
{
  return *this;

} // end mNMf& mNMf::Resize(u2, u2)                                           */


dp::mNMf& dp::mNMf::Set(const vNf& source)
{
  return *this;

} // end mNf& mNf::Set(const vNf&)                                            */


dp::mNMf& dp::mNMf::Set(const mNf& source)
{
  return *this;

} // end mNf& mNf::Set(const mNf&)                                            */


dp::mNMf& dp::mNMf::Set(const mNMf& source)
{
  return *this;

} // end mNf& mNf::Set(const mNMf&)                                           */


std::string dp::mNMf::ToString(bool lineBreaks) const
{
  return "[matrix string]";

} // end std::string mNf::ToString(void) const                                */


dp::mNMf& dp::mNMf::Transpose(void)
{
  return *this;

} // end mNf& mNf::Transpose(void)                                            */


dp::mNMf dp::mNMf::Transposed(void) const
{ return Copy().Transpose(); } // end mNf mNf::Transposed(void) const         */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                            Operator Overloads                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::mNMf& dp::mNMf::operator=(const vNf& source)
{
  return this->Set(source);

} // end mNf& mNf::operator=(const mNf&)                                      */

dp::mNMf& dp::mNMf::operator=(const mNf& source)
{
  return this->Set(source);

} // end mNf& mNf::operator=(const mNf&)                                      */


dp::mNMf& dp::mNMf::operator=(const mNMf& source)
{
  return this->Set(source);

} // end mNf& mNf::operator=(const mNMf&)                                     */


dp::mNMf& dp::mNMf::operator=(mNMf&& result) noexcept
{
  cell = std::move(result.cell);
  return *this;

} // end mNf& mNf::operator=(mNf&&) noexcept                                  */


dp::mNMf dp::mNMf::operator-(void) const
{
  mNMf copy(Dimens());
  for (size_t i = 0; i < cell.Dimens(); ++i)
  {
    copy.cell[i] = -cell[i]; // Copy each element, negated
  }
  return copy;

} // end mNf mNf::operator-(void) const                                       */


dp::mNMf dp::mNMf::operator!(void) const
{
  return Transposed();

} // end mNf mNf::operator!(void)                                             */


f4* dp::mNMf::operator[](u4 subscript)
{
  return &(cell[subscript]);

} // end f4* mNMf::operator[](u4)                                             */


dp::mNMf dp::mNMf::operator+(const mNMf& other) const
{
  return Copy() += other;

} // end mNMf mNMf::operator+(const mNMf&)


dp::mNMf& dp::mNMf::operator+=(const mNMf& other)
{
  for (u4 i = 0; i < cell.Dimens(); ++i) { cell[i] += other.cell[i]; }
  return *this;

} // end mNMf& mNMf::operator+=(const mNMf&)


dp::mNMf dp::mNMf::operator-(const mNMf& other) const
{
  return Copy() += -other;

} // end mNMf mNMf::operator-(const mNMf&)


dp::mNMf& dp::mNMf::operator-=(const mNMf& other)
{
  return *this += -other;

} // end mNNf& mNMf::operator-=(const mNMf&)


dp::mNMf dp::mNMf::operator*(f4 scalar) const
{
  return Copy() *= scalar;

} // end mNf mNf::operator*(f4) const

dp::mNMf& dp::mNMf::operator*=(f4 scalar)
{
  for (u4 i = 0; i < cell.Dimens(); ++i) { cell *= scalar; }
  return *this;

} // end mNf& mNf::operator*=(f4) const