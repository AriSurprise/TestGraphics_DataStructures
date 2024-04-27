/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Transform.cpp
Purpose:  Spatial display context managing placement in world coordinates
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no platform requirements)
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

// stl
#include <cmath>
// "./src/..."
#include "m3f.h"        // Homogeneous affine transform linear m3f extractions
#include "Log.h"        // Output error / warning message / logging
#include "Transform.h"  // Associated class declaration header
#include "v2f.h"        // Euler angle planar intersections


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create a Transform from a set of v3f descriptors for 3D placement data    */
dp::Transform::Transform(const v3f& position, const v3f& dimensions,
  const v3f& anchor, const v3f& orientationRads)
  : translation(position), scale(dimensions), offset(anchor),
  orientRad(orientationRads), dirty(true), dirtyPos(true),
  dirtyRot(true), dirtyLocals(true), dirtyAnchor(true), dirtyScale(true)
{ } // end Transform(const v43&, const v3f&, const v3f&, const v3f&)


// Create a Transform from v3f / ang descriptors for 2.5D placement data      */
dp::Transform::Transform(const v3f& position, const v3f& dimensions,
  const v3f& anchor, ang rotation_z, ang rotation_y, ang rotation_x)
  : Transform(position, dimensions, anchor,
    v3f(rotation_x.radRef(), rotation_y.radRef(), rotation_z.radRef()))
{ } // end Transform(const v3f&, const v3f&, const v3f&, ang, ang, ang)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

const dp::v3f& dp::Transform::Anchor(void) const
{
  return offset;
} // end const v3f& Anchor(void) const                                        */


dp::Transform& dp::Transform::Anchor(const v3f& anchorOffset,
  bool displacement)
{
  if (displacement) { return CenterSlide(anchorOffset); }
  else { return CenterAround(anchorOffset); }
} // end Transform& Anchor(const v3f&, bool)                                  */


dp::Transform& dp::Transform::CenterAround(const v3f& objectCenterOffset)
{
  if (objectCenterOffset != offset)
  {
    offset = objectCenterOffset;
    dirtyAnchor = true;
    dirty = true;
  }
  return *this;
} // end Transform& CenterAround(const v3f&)                                  */


dp::Transform& dp::Transform::CenterSlide(const v3f& anchorDisplacement)
{
  if (anchorDisplacement != nullv3f)
  {
    offset += anchorDisplacement;
    dirtyAnchor = true;
    dirty = true;
  }
  return *this;
} // end Transform& CenterSlide(const v3f&)                                   */


dp::Transform& dp::Transform::Dolly(f4 forwardDistance)
{
  translation += (localZ * forwardDistance);
  dirty = true;
  dirtyPos = true;
  return *this;

} // end Transform& Transform::Dolly(f4)                                      */


const dp::v3f& dp::Transform::LocalX(void) const
{ return localX; } // end const v3f& Transform::LocalX(void) const            */


const dp::v3f& dp::Transform::LocalY(void) const
{ return localY; } // end const v3f& Transform::LocalY(void) const            */


const dp::v3f& dp::Transform::LocalZ(void) const
{ return localZ; } // end const v3f& Transform::LocalZ(void) const            */


const dp::m4f& dp::Transform::Matrix(void)
{
  if (dirty) { RecalculateMatrix(); }
  return concat;
} // end const m4f& Matrix(void)                                              */


// Set Transform position to use the given displacement from current value    */
dp::Transform& dp::Transform::MoveBy(const v3f& displacement)
{
  if (displacement != nullv3f)
  {
    translation += displacement;
    dirtyPos = true;
    dirty = true;
  }
  return *this;
} // end Transform& MoveBy(const v3f&)                                        */


// Set Transform object center translation of world coords without anchor     */
dp::Transform& dp::Transform::MoveOnto(const v3f& objectCenter)
{
  translation = objectCenter;
  dirtyPos = true;
  dirty = true;
  return *this;
} // end Transform& MoveOnto(const v3f&)


// Set Transform position to use the given absolute world coordinates         */
dp::Transform& dp::Transform::MoveTo(const v3f& location)
{
  translation = (location + offset);
  dirtyPos = true;
  dirty = true;
  return *this;
} // end Transform& MoveTo(const v3f&)


// Get the concatenated transformation m4f into the described coord basis    */
const dp::m4f& dp::Transform::NormalMatrix(void)
{
  if (dirty) { RecalculateMatrix(); }
  return norm;
} // end const m4f& Matrix(void)


// Get euler angles of cartesian rotation per local axis                      */
const dp::v3f& dp::Transform::Orientation(void) const
{
  return orientRad;
} // end const v3f& Orientation(void) const


// Set euler angle of cartesian rotation per local axis in <z, y, x> order    */
dp::Transform& dp::Transform::Orientation(const ang& z_rot,
  const ang& y_rot, const ang& x_rot)
{
  if (orientRad != v3f(x_rot.r(), y_rot.r(), z_rot.r()))
  {
    orientRad.X(x_rot.r()).Y(y_rot.r()).Z(z_rot.r());
    dirtyLocals = true;
    dirtyRot = true;
    dirty = true;
  }
  return *this;
} // end Transform& Orientation(const ang&, const ang&, const ang&)


// Set Transform to move by given amount from current values in local X       */
dp::Transform& dp::Transform::Pan(f4 rightDistance)
{
  translation += (localX * rightDistance);
  dirty = true;
  dirtyPos = true;
  return *this;

} // end Transform& Transform::Pan(f4)


// Rotate the local orientation along the vertical axis (+up / -down)         */
dp::Transform& dp::Transform::Pitch(ang pitchChange, bool relative)
{
  ang newPitch = ang(orientRad.X(), arc::rad);
  m4f pitch;
  bool changed = false;

  // Setup value changes per mode
  if (relative)
  {
    if (pitchChange != nullang)
    {
      newPitch += pitchChange;
      orientRad.X(newPitch.r());
      pitch = RotateRodriguesAffine(pitchChange, localX);
      localY = (pitch * localY.XYZW(Of)).XYZ();
      localZ = (pitch * localZ.XYZW(Of)).XYZ();
      dirtyRot = true;
      dirty = true;
    }
  }
  else
  {
    if (pitchChange.radRef() != orientRad.X())
    {
      newPitch = pitchChange - newPitch;
      orientRad.X(pitchChange.r());
      pitch = RotateRodriguesAffine(newPitch, localX);
      localY = (pitch * localY.XYZW(Of)).XYZ();
      localZ = (pitch * localZ.XYZW(Of)).XYZ();
      dirtyRot = true;
      dirty = true;
    }
  }

  // Perpetuate mode adapted value changes
  if (changed)
  {
    // Update local unit vectors
    localY = (pitch * localY.XYZW(Of)).XYZ();
    localZ = (pitch * localZ.XYZW(Of)).XYZ();
    RecalculateEulers();

    // Update state change flags
    dirtyRot = true;
    dirty = true;
  }
  return *this;

} // end Transform& Transform::Pitch(ang, bool)


// Get current affine sum world coordinates of Transform position stored      */
const dp::v3f& dp::Transform::Position(void) const
{
  return translation;
} // end const v3f& Position(void) const


// Get current world coords of Transform object center as input position      */
dp::v3f dp::Transform::Position(bool displayedCenter) const
{
  return translation - offset;
} // end v3f Position(bool) const


// Set position of the Transform to use the given coordinates / movement      */
dp::Transform& dp::Transform::Position(const v3f& coords,
  bool displacement, bool faceValue)
{
  if (displacement) { return MoveBy(coords); }
  else if (faceValue) { return MoveTo(coords); }
  else { return MoveOnto(coords); }
} // end Transform& Position(const v3f&, bool)


// Rotate the local orientation about the view normal (+clockwise / -ccw)     */
dp::Transform& dp::Transform::Roll(ang rollChange, bool relative)
{
  ang newRoll = ang(orientRad.Z(), arc::rad);
  m4f roll;
  bool changed = false;

  // Setup value changes per mode
  if (relative)
  {
    if (rollChange != nullang)
    {
      newRoll += rollChange;
      orientRad.Z(newRoll.r());
      roll = RotateRodriguesAffine(rollChange, localZ);
      changed = true;
    }
  }
  else
  {
    if (rollChange.radRef() != orientRad.Z())
    {
      newRoll = rollChange - newRoll;
      orientRad.Z(rollChange.r());
      roll = RotateRodriguesAffine(newRoll, localZ);
      changed = true;
    }
  }

  // Perpetuate mode adapted value changes
  if (changed)
  {
    // Update local unit vectors
    localX = (roll * localX.XYZW(Of)).XYZ();
    localY = (roll * localY.XYZW(Of)).XYZ();
    RecalculateEulers();

    // Update state change flags
    dirtyRot = true;
    dirty = true;
  }
  return *this;

} // end Transform& Transform::Roll(ang, bool)


// Get 2D rotation of the object, presuming no other axial rotation values    */
const f4& dp::Transform::RotateZRad(void) const
{
  return orientRad.z;
} // end const f4& RotateZRad(void) const


// Get the size (unitless) of object to be displayed, per local axis          */
const dp::v3f& dp::Transform::Scale(void) const
{
  return scale;
} // end const v3f& Scale(void) const


// Set the size (unitless) of object to be displayed, per local axis          */
dp::Transform& dp::Transform::Scale(const v3f& dimensionalScalars)
{
  if (dimensionalScalars != scale)
  {
    scale = dimensionalScalars;
    dirtyAnchor = true; // (affine reciprocal un-scale should match units)
    dirtyScale = true;
    dirty = true;
  }
  return *this;
} // end Transform& Scale(const ang&)


// Set Transform to move by given amount from current values in local Y       */
dp::Transform& dp::Transform::Tilt(f4 upwardDistance)
{
  translation += (localY * upwardDistance);
  dirty = true;
  dirtyPos = true;
  return *this;

} // end Transform& Transform::Tilt(f4)


// Rotate the local orientation along the horizontal axis (+right / -left)    */
dp::Transform& dp::Transform::Yaw(ang yawChange, bool relative)
{
  ang newYaw = ang(orientRad.Y(), arc::rad);
  m4f yaw;
  bool changed = false;

  // Setup value changes per mode
  if (relative)
  {
    if (yawChange != nullang)
    {
      newYaw += yawChange;
      orientRad.Y(newYaw.r());
      yaw = RotateRodriguesAffine(yawChange, localY);
      changed = true;
    }
  }
  else
  {
    if (yawChange.radRef() != orientRad.Y())
    {
      newYaw = yawChange - newYaw;
      yaw = RotateRodriguesAffine(newYaw, localY);
      changed = true;
    }
  }

  // Perpetuate mode adapted value changes
  if (changed)
  {
    // Update local unit vectors
    localX = (yaw * localX.XYZW(Of)).XYZ();
    localZ = (yaw * localZ.XYZW(Of)).XYZ();
    RecalculateEulers();

    // Update state change flags
    dirtyRot = true;
    dirty = true;
  }
  return *this;

} // end Transform& Transform::Yaw(ang, bool)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set internal local axis unit vectors to reflect changes in orientation     */
void dp::Transform::RecalculateBasis(void)
{
  localX = X_HAT;
  localY = Y_HAT;
  localZ = Z_HAT;
  m4f roll = RotateRodriguesAffine(ang(orientRad.z, arc::rad), localZ);
  localX = (roll * localX.XYZW(Of)).XYZ();
  localY = (roll * localY.XYZW(Of)).XYZ();
  m4f yaw = RotateRodriguesAffine(ang(orientRad.y, arc::rad), localY);
  localX = (yaw * localX.XYZW(Of)).XYZ();
  localZ = (yaw * localZ.XYZW(Of)).XYZ();
  m4f pitch = RotateRodriguesAffine(ang(orientRad.x, arc::rad), localX);
  localY = (pitch * localY.XYZW(Of)).XYZ();
  localZ = (pitch * localZ.XYZW(Of)).XYZ();

} // end void Transform::RecalculateBasis(void)



// Update euler orientation values to be reflective of local basis vectors    */
void dp::Transform::RecalculateEulers(void)
{
  // TODO: only works over theta (y), when in [0,pi]...? not [-pi,pi]...?
  /* at least according to docs, but by that logic the separating case should
  jump out for one versus the other being in use as to when a transition into
  recalculation from these values occurs: if it's from values calculated in
  one branch, a jump in orientation will generally occur when switching into
  using those values, while the other branch definitely will not (whichever is
  correct). This does not appear to be strongly corellated in the present
  results, and jumps occur from values calculated from either branch...
  as such, theta values > 0 don't appear to be the delineation of errors in
  boundaries of mapping accuracy.
  */
  
  f4 sy = SqrtF(localX.x * localX.x + localX.y * localX.y);

  bool singular = sy < 1e-6;
  
  if (!singular)
  {
    f4 yVal = atan2(-localX.z, sy);
    if (yVal > 0)
    {
      //Log::N("Y");
      orientRad.x = atan2(localZ.y, localZ.z);
      orientRad.y = yVal;
      orientRad.z = atan2(localX.y, localX.x);
    }
    else
    {
      //Log::N("N");
      orientRad.x = atan2(localZ.x, localZ.y);
      orientRad.y = acos(localZ.z);
      orientRad.z = atan2(localZ.x, localZ.y);
    }
  }
  else
  {
    orientRad.x = atan2(-localZ.y, localY.y);
    orientRad.y = atan2(-localX.z, sy);
    orientRad.z = 0;
  }

} // end void Transform::RecalculateEulers(void)


// Set internal concat m4f to represent current summary transformations      */
void dp::Transform::RecalculateMatrix(void)
{
  if (dirtyScale)
  {
    rescale = v3f(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
    sizeMat = Scale3Affine(scale.x, scale.y, scale.z);
    dirtyScale = false;
  }
  if (dirtyAnchor)
  {
    v3f pivot = -offset * rescale;
    anchorMat = TranslateAffine(pivot);
    dirtyAnchor = false;
  }
  if (dirtyLocals)
  {
    RecalculateBasis();
    dirtyLocals = false;
  }
  if (dirtyRot)
  {
    rotMat = m4f(localX.x, localY.x, localZ.x,
                  localX.y, localY.y, localZ.y,
                  localX.z, localY.z, localZ.z);
    dirtyRot = false;
  }
  if (dirtyPos)
  {
    moveMat = TranslateAffine(translation);
    dirtyPos = false;
  }
  concat = anchorMat * sizeMat * rotMat * moveMat;
  norm = concat.Linear().Inverse().Transpose();
  dirty = false;
} // end void RecalculateMatrix(void)
