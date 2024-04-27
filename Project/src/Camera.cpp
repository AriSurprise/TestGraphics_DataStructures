/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Camera.cpp
Purpose:  Spatial display context managing visual output
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

// "./src/..."
#include "Camera.h" // Associated class declaration header
#include "Log.h"    // Output error / warning message / logging


const dp::ang MAX_ANG(90.0f, dp::arc::degrees);
const dp::m4f dummyPersp(1.357995f,      0.0f,       0.0f,     0.0f,
                              0.0f, 2.414213f,       0.0f,     0.0f,
                              0.0f,      0.0f, -1.002002f, -0.2002f,
                              0.0f,      0.0f,      -1.0f,     0.0f);

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Camera::Camera(const v3f& eye, const v3f& look, const v3f& up, ang FoV,
  v2f dimens, f4 near, f4 far)
{
  MoveTo(eye).Aim(look, up).RenderRes(dimens).ViewAngle(FoV).Clip(near, far);

} // Camera(const v3f&, const v3f&, const v3f&, ang, v2f, f4, f4)             */


dp::Camera::Camera(const Transform& source, ang FoV,v2f dimens,f4 near,f4 far)
  : Camera(source.Position(), -source.LocalZ(), source.LocalY(), FoV, dimens,
    near, far)
{} // Camera(const Transform&, ang, v2f, f4, f4)                              */


dp::Camera::Camera(const Camera& source)
  : Camera(source.Position(), -source.LocalZ(), source.LocalY(),
   source.ViewAngle(), source.RenderRes(), source.NearClip(), source.FarClip())
{} // Camera(const Camera&)                                                   */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Camera& dp::Camera::Aim(v3f look, v3f up)
{
  // Validate inputs
  if (look == nullv3f) { look = -Z_HAT; }
  if (up == nullv3f) { up = Y_HAT; }
  if (look == up || look == -up)
  {
    if (look == Y_HAT) { up = Z_HAT; }
    else if (look == -Y_HAT) { up = -Z_HAT; }
    else { up = Y_HAT; }
  }
  // Calculate local basis from inputs
  localZ = -look.UnitVec();
  localX = localZ.Cross(up).UnitVec();
  localY = localZ.Cross(localX);

  // Update recalculation needed states
  dirty = true;
  dirtyRot = true;
  dirtyLocals = true;
  dirtyView = true;
  return *this;

} // end Camera& Camera::Aim(v3f, v3f)                                        */


const f4& dp::Camera::AspectRatio(void) const
{ return aspect; } // end const f4& AspectRatio(void) const                   */


// Set Camera near and far clip distances for view depth draw range
dp::Camera& dp::Camera::Clip(f4 near, f4 far)
{
  if (near < 0.0f) { near = -near; }
  if (NearF(near, 0.0f, 0.000001f)) { near = 0.000001f; }
  if (far < 0.0f) { far = -far; }
  if (NearF(far, 0.0f, 0.000001f)) { far = 1.0f; }
  if (far < near) { Swap<f4>(near, far); }
  nearClip = near;
  farClip = far;
  distance = (Orthogonal()) ? near : (near + far) * 0.5f;
  dirtyLook = true;
  return *this;

} // end Camera& Camera::Clip(f4, f4)                                         */


const dp::m4f& dp::Camera::Device(void)
{
  RecalculateDevice();
  return devMat;

} // end const m4f& Camera::Device(void)                                      */


dp::Camera& dp::Camera::Dolly(f4 forwardDistance)
{
  Transform::Dolly(forwardDistance);
  dirtyView = true;
  return *this;

} // end Camera& Camera::Dolly(f4)                                            */


const f4& dp::Camera::FarClip(void) const
{ return farClip; } // end const f4& FarClip(void) const                      */


dp::Camera& dp::Camera::FarClip(f4 farClipDistance)
{
  if (farClipDistance < 0.0f) { farClipDistance = -farClipDistance; }
  if (NearF(farClipDistance, 0.0f, 0.000001f)) { farClipDistance = 1.0f; }
  if (farClipDistance < nearClip) { Swap<f4>(nearClip, farClipDistance); }
  farClip = farClipDistance;
  dirtyLook = true;
  return *this;

} // end Camera& Camera::FarClip(f4)                                          */


dp::Camera& dp::Camera::MoveBy(const v3f& displacement)
{
  Transform::MoveBy(displacement);
  dirtyView = true;
  return *this;

} // end Camera& MoveBy(const v3f&)                                           */


dp::Camera& dp::Camera::MoveTo(const v3f& location)
{
  Transform::MoveTo(location);
  dirtyView = true;
  return *this;
} // end Camera& MoveTo(const v3f&)                                           */


const f4& dp::Camera::NearClip(void) const
{ return nearClip; } // end const f4& NearClip(void) const                    */


dp::Camera& dp::Camera::NearClip(f4 nearClipDistance)
{
  if (nearClipDistance < 0.0f) { nearClipDistance = -nearClipDistance; }
  if (NearF(nearClipDistance, 0.0f, 0.000001f))
  { nearClipDistance = 0.000001f; }
  if (nearClipDistance > farClip) { Swap<f4>(farClip, nearClipDistance); }
  nearClip = nearClipDistance;
  dirtyLook = true;
  return *this;
} // end Camera& Camera::NearClip(f4)                                         */


bool dp::Camera::Orthogonal(void) const
{ return (viewAng == nullang); } // end bool Camera::Orthogonal(void) const   */


dp::Camera& dp::Camera::Pan(f4 rightDistance)
{
  Transform::Pan(rightDistance);
  dirtyView = true;
  return *this;
} // end Camera& Camera::Pan(f4)                                              */


const dp::m4f& dp::Camera::Persp(void)
{
  RecalculatePersp();
  return dummyPersp;
} // end const m4f& Camera::Persp(void)                                       */


dp::Camera& dp::Camera::Pitch(ang pitchChange)
{
  Transform::Pitch(pitchChange, true);
  dirtyView = true;
  return *this;
} // end Camera& Camera::Pitch(ang, bool)                                     */


const dp::v3f& dp::Camera::Position(void) const
{ return Transform::Position(); } // end const v3f& Position(void) const      */


dp::Camera& dp::Camera::Position(const v3f& coords, bool displacement)
{
  Transform::Position(coords, displacement, true);
  dirtyView = true;
  return *this;
} // end Camera& Position(const v3f&, bool)                                   */


const dp::v2f& dp::Camera::RenderRes(void) const
{ return renderRes; } // end const v2f& Camera::RenderRes(void) const

dp::Camera& dp::Camera::RenderRes(const v2f& dimensions)
{
  // 0 values must be impermissable to avoid device matrix 0 division error
  // negatives are merely semantically bad, simpler to avoid for debugging
  renderRes.y = (NearF(dimensions.y, 0.0f, 0.000001f)) ? 0.000001f :
    (dimensions.y < 0.0f) ? -dimensions.y :  dimensions.y;
  renderRes.x = (NearF(dimensions.x, 0.0f, 0.000001f)) ? 0.000001f :
    (dimensions.x < 0.0f) ? -dimensions.x :  dimensions.x;
  aspect = renderRes.x / renderRes.y;
  viewRes.y = viewRes.x / aspect;
  dirtyLook = true;
  dirtySlant = true;
  return *this;
} // end Camera& Camera::RenderRes(const v2f&)                                */


dp::Camera& dp::Camera::Roll(ang rollChange)
{
  Transform::Roll(rollChange, true);
  dirtyView = true;
  return *this;

} // end Camera& Camera::Roll(ang, bool)                                      */


const f4& dp::Camera::RotateZRad(void) const
{ return Transform::RotateZRad(); } // end const f4& RotateZRad(void) const   */


dp::Camera& dp::Camera::Tilt(f4 upwardDistance)
{
  Transform::Tilt(upwardDistance);
  dirtyView = true;
  return *this;

} // end Camera& Camera::Tilt(f4)                                             */


const dp::m4f& dp::Camera::TransforMatrix(void)
{
  return Transform::Matrix();

} // end const m4f& Matrix(void)                                              */


const dp::ang& dp::Camera::ViewAngle(void) const
{ return viewAng; } // end const ang& Camera::ViewAngle(void) const           */


dp::Camera& dp::Camera::ViewAngle(ang fieldOfViewAngle)
{
  if (fieldOfViewAngle < nullang) { !fieldOfViewAngle; }
  if (NearF(fieldOfViewAngle.r(), 0.0f, 0.000001f))
  {
    viewRes.x = 2.0f;
    viewAng = nullang;
  }
  else
  {
    if (fieldOfViewAngle > MAX_ANG) { viewAng = MAX_ANG; }
    else { viewAng = fieldOfViewAngle; }
    viewRes.x = 2.0f * tanf((viewAng.r() * 0.5f));
  }
  viewRes.y = viewRes.x / aspect;
  dirtyLook = true;
  return *this;

} // end Camera& Camera::ViewAngle(ang)                                       */


const dp::m4f& dp::Camera::View(void)
{
  RecalculateView();
  return viewMat;

} // end const m4f& Camera::ViewMatrix(void)                                  */


dp::Camera& dp::Camera::Yaw(ang yawChange)
{
  Transform::Yaw(yawChange, true);
  dirtyView = true;
  return *this;

} // end Camera& Camera::Yaw(ang, bool)                                       */



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Operator Overloads                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Camera& dp::Camera::operator=(const Transform& source)
{
  MoveTo(source.Position()).Aim(-source.LocalZ(), source.LocalY());
  RenderRes(v2f(3.5555555555555556f, 2.0f));
  ViewAngle(ang(45.0f, arc::degrees)).Clip(0.000001f, 100.0f);
  return *this;
} // end Camera& Camera::operator=(const Transform&)                          */


dp::Camera& dp::Camera::operator=(const Camera& source)
{
  MoveTo(source.Position()).Aim(-source.LocalZ(), source.LocalY());
  RenderRes(source.RenderRes()).ViewAngle(source.ViewAngle());
  Clip(source.NearClip(), source.FarClip());
  return *this;
} // end Camera& Camera::operator=(const Camera&)                             */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Private Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

void dp::Camera::RecalculateDevice(void)
{
  if (dirtySlant)
  {
    f4 dif = (farClip - nearClip);
    devMat = Scale3Affine(InvF(renderRes.x), InvF(renderRes.y), InvF(dif));
    dirtySlant = false;
  }
} // end void Camera::RecalculateDevice(void)                                 */

// Recalculate frustum distortion matrix based on local state flags           */
void dp::Camera::RecalculatePersp(void)
{
  if (dirtyLook)
  {
    f4& N = nearClip; // Near plane distance
    f4& F = farClip;  // Far plane distance
    f4 id = 1.0f / (N - F); // Inverse ratio of near to far plane distance
    f4 iw = 1.0f / viewRes.x; // Inverse viewport width ratio
    f4 ih = 1.0f / viewRes.y; // Inverse viewport height ratio
    if (Orthogonal())
    {
      // TODO: factor distance into each term to utilize any kind of 2D zoom
      perspMat = m4f(2.0f*iw,    0.0f,     0.0f,     0.0f,
                         0.0f, 2.0f*ih,     0.0f,     0.0f,
                         0.0f,    0.0f, -2.0f*id,     0.0f,
                         0.0f,    0.0f,-(N+F)*id,     1.0f);
      //Log::D(perspMat.ToString(true));
    }
    else
    {
      f4 d2 = 0.5f * 2.0f; // Twice the distance to the picture plane
      perspMat = m4f(d2*iw,   0.0f,        0.0f,        0.0f,
                       0.0f,  d2*ih,        0.0f,        0.0f,
                       0.0f,   0.0f,    (N+F)*id, 2.0f*N*F*id,
                       0.0f,   0.0f,       -1.0f,        0.0f);
      //Log::D(perspMat.ToString(true));
    }
    dirtyLook = false;
  }
} // end void Camera::RecalculatePersp(void)

// Recalculate view matrix as necessary based on local state change flags     */
void dp::Camera::RecalculateView(void)
{
  if (dirtyView)
  {
    viewMat = TransforMatrix().Inverse(); // model->world inverse =: world->view
    //Log::D("view\n" + viewMat.ToString(true));
    dirtyView = false;
  }
} // end void Camera::RecalculateView(void)