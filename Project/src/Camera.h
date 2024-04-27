/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Camera.h
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

#include "Transform.h"  // Base class for inheritance
#include "v2f.h"       // Picture plane dimension parameters

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Camera : private Transform //! Spatial display context for visual output
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a Camera from the given position, orientation & view settings
    \brief
      - create a Camera from the given position, orientation & view settings
    \param eye
      - station point; camera position in world coordinates
    \param look
      - direction vector for camera view to point towards
    \param up
      - direction vector to use as relative up vector for camera local basis
    \param FoV
      - cone of vision / field of view from 0 (orthographic), to 90 degrees
    \param dimens
      - viewport world space render dimensions (default NDC in 16:9 ratio)
    \param near
      - near plane drawing clip distance from eye in look direction
    \param far
      - far plane drawing clip distance from eye in look direction
    */
    Camera(const v3f& eye = nullv3f, const v3f& look = -Z_HAT,
      const v3f& up = Y_HAT, ang FoV = ang(45.0f, arc::degrees), v2f dimens
      = v2f(3.5555555555555556f, 2.0f), f4 near = 0.000001f, f4 far = 100.0f);


    /** Create a Camera copied from a Transform's settings with camera options
    \brief
      - create a Camera copied from a Transform's settings with camera options
    \param source
      - source Camera object to copy settings from for this Camera's attributes
    */
    Camera(const Transform& source, ang FoV = ang(45.0f, arc::degrees), v2f
      dimens = v2f(3.5555555555555556f, 2.0f), f4 near = 0.1f, f4 far = 100.0f);


    /** Create a Camera copied from another source Camera's settings
    \brief
      - create a Camera copied from another source Camera's settings
    \param source
      - source Camera object to copy settings from for this Camera's attributes
    */
    Camera(const Camera& source);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Set Camera orientation basis to aim local Z axis to look in a direction
    \brief
      - set Camera orientation basis to aim local Z axis to look in a direction
    \param look
      - direction vector for camera view to point towards
    \param up
      - direction vector to use as relative up vector for camera local basis
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& Aim(v3f look = -Z_HAT, v3f up = Y_HAT);


    /** Get width : height ratio for current camera view settings 
    \brief
      - get width : height ratio for current camera view settings 
    \return
      - aspect ratio of the camera's current render width / height settings 
    */
    const f4& AspectRatio(void) const;


    /** Set Camera near and far clip distances for view depth draw range
    \brief
      - set Camera near and far clip distances for view depth draw range
    \param near
      - positive value at which objects too close to camera should not be drawn
    \param far
      - positive value at which objects too far from camera should not be drawn
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& Clip(f4 near, f4 far);


    /** Get transform matrix converting view frustum coords into render region
    \brief
      - get transform matrix converting view frustum coords into render region
    \details
      - though a getter, must be non-constant to recalculate only as needed
    \return
      - scalar matrix shrinking world scale view frustum to render region
    */
    const m4f& Device(void);


    /** Set Camera to move by given amount from current values in local Z
    \brief
      - set Camera to move by given amount from current values in local Z
    \param forwardDistance
      - amount by which the Camera should move (+/-) in local Z orientation
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& Dolly(f4 forwardDistance);


    /** Get near clipping distance from the camera at which objects are occluded
    \brief
      - get near clipping distance from the camera at which objects are occluded
    \return
      - near clip distance camera is using to determine objects are too close
    */
    const f4& FarClip(void) const;


    /** Set far clipping distance from the camera at which objects are occluded
    \brief
      - set far clipping distance from the camera at which objects are occluded
    \param farClipDistance
      - positive value at which objects too far from camera should not be drawn
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& FarClip(f4 farClipDistance);


    /** Set Camera position to use the given displacement from current value
    \brief
      - set Camera position to use the given displacement from current value
    \param displacement
      - world coordinate vector by which to offset current Camera world position
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& MoveBy(const v3f& displacement);


    /** Set Camera position to use the given absolute world coordinates
    \brief
      - set Camera position to use the given absolute world coordinates
    \param location
      - new world coordinates onto which Camera position is to be moved
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& MoveTo(const v3f& location);


    /** Get near clipping distance to the camera at which objects are occluded
    \brief
      - get near clipping distance to the camera at which objects are occluded
    \return
      - near clip distance camera is using to determine objects are too close
    */
    const f4& NearClip(void) const;


    /** Set near clipping distance from the camera at which objects are occluded
    \brief
      - set near clipping distance from the camera at which objects are occluded
    \param nearClipDistance
      - positive value at which objects too close to camera should not be drawn
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& NearClip(f4 nearClipDistance);


    /** Get whether the field of view is applying perspective distortion
    \brief
      - get whether the field of view is applying perspective distortion
    \return
      - true if the viewing angle is null; perspective distortion isn't used
    */
    bool Orthogonal(void) const;


    /** Set Camera to move by given amount from current values in local X
    \brief
      - set Camera to move by given amount from current values in local X
    \param rightDistance
      - amount by which the Camera should move (+/-) in local X orientation
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& Pan(f4 rightDistance);


    /** Get transformation matrix converting view coords into Camera frustum
    \brief
      - get transformation matrix converting view coords into Camera frustum
    \details
      - though a getter, must be non-constant to recalculate only as needed
    \return
      - distortion matrix applying perspective for the given field of view
    */
    const m4f& Persp(void);


    /** Rotate the local orientation along the vertical axis (+up / -down)
    \brief
      - rotate the local orientation along the vertical axis (+up / -down)
    \param pitchChange
      - amount by which to offset (or set), rotation about the local x axis
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& Pitch(ang pitchChange);


    /** Get current affine sum world coordinates of Camera position stored
    \brief
      - get current affine sum world coordinates of Camera position stored
    \return
      - reference to the contained Camera translation (position) vector
    */
    const v3f& Position(void) const;


    /** Set position of the Camera to use the given coordinates / movement
    \brief
      - set position of the Camera to use the given coordinates / movement
    \param coords
      - values to use for Camera position / translation setting / movement
    \param displacement
      - whether to use relative offset math (if true), from current position
    \param faceValue
      - whether to apply coords as specified, ignoring anchor offsets
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& Position(const v3f& coords, bool displacement = false);


    /** Get current resolution the camera uses for expected render dimensions 
    \brief
      - get current resolution the camera uses for expected render dimensions 
    \return
      - dimensions used for calculating internal device matrix settings
    */
    const v2f& RenderRes(void) const;


    /** Get current resolution the camera uses for expected render dimensions 
    \brief
      - get current resolution the camera uses for expected render dimensions 
    \param dimensions
      - get current resolution the camera uses for expected render dimensions 
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& RenderRes(const v2f& dimensions);


    /** Rotate the local orientation about the view normal (+clockwise / -ccw)
    \brief
      - rotate the local orientation about the view normal (+clockwise / -ccw)
    \param rollChange
      - amount by which to offset (or set), rotation about the local z axis
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& Roll(ang rollChange);


    /** Get 2D rotation of the object, presuming no other axial rotation values
    \brief
      - get 2D rotation of the object, presuming no other axial rotation values
    \return
      - reference to the contained Z value of the orientation v3f of radians
    */
    const f4& RotateZRad(void) const;


    /** Set Camera to move by given amount from current values in local Y
    \brief
      - set Camera to move by given amount from current values in local Y
    \param upwardDistance
      - amount by which the Camera should move (+/-) in local Y orientation
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& Tilt(f4 upwardDistance);


    /** Get transformation matrix placing camera visualization into world space
    \brief
      - get transformation matrix placing camera visualization into world space
    \details
      - though a getter, must be non-constant to recalculate only as needed
    \return
      - concatenated transformation m4f into the described world coord basis
    */
    const m4f& TransforMatrix(void);


    /** Get viewing angle for cone of perspective distortion from projection
    \brief
      - get viewing angle for cone of perspective distortion from projection
    \param fieldOfViewAngle
      - angle measure [0,90deg] of cone of vision spread from projection point
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    const ang& ViewAngle(void) const;


    /** Set Camera viewing angle for cone of perspective distortion
    \brief
      - set Camera viewing angle for cone of perspective distortion
    \param fieldOfViewAngle
      - angle measure [0,90deg] of cone of vision spread from projection point
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& ViewAngle(ang fieldOfViewAngle);


    /** Get transformation matrix converting world coordinates into Camera view
    \brief
      - get transformation matrix converting world coordinates into Camera view
    \details
      - though a getter, must be non-constant to recalculate only as needed
    \return
      - inverse transform matrix from model->world basis shift =: world->view
    */
    const m4f& View(void);


    /** Rotate the local orientation along the horizontal axis (+right / -left)
    \brief
      - rotate the local orientation along the horizontal axis (+right / -left)
    \param yawChange
      - amount by which to offset (or set), rotation about the local y axis
    \return
      - reference to the modified Camera for easier subsequent inline action
    */
    Camera& Yaw(ang yawChange);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Operator Overloads                             */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set this from a Transform's partial settings with default options
    \brief
      - set this from a Transform's partial settings with default options
    \param source
      - source Transform object to use for this Camera's basic attributes
    */
    Camera& operator=(const Transform& source);


    /** Set this Camera copied to match another source Camera's settings
    \brief
      - set this Camera copied to match another source Camera's settings
    \param source
      - source Camera object to copy settings from for this Camera's attributes
    */
    Camera& operator=(const Camera& source);


  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Recalculate dimensional render scale matrix based on local state flags
    \brief
      - recalculate dimensional render scale matrix based on local state flags
    */
    void RecalculateDevice(void);


    /** Recalculate frustum distortion matrix based on local state flags
    \brief
      - recalculate frustum distortion matrix based on local state flags
    */
    void RecalculatePersp(void);

    /** Recalculate view matrix as necessary based on local state change flags
    \brief
      - recalculate view matrix as necessary based on local state change flags
    */
    void RecalculateView(void);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Matrix for converting world coordinates to Camera view coordinates
    m4f viewMat;

    //! Matrix for converting view coordinates into Camera view frusum
    m4f perspMat;

    //! Matrix for scaling view frustum world coordinates into render space
    m4f devMat;

    //! Viewport internal picture plane resolution (API scale of view frustum)
    v2f viewRes;

    //! Viewport render resolution (scalar of world coords into view frustum)
    v2f renderRes;

    //! Viewport cone of vision / Field of vision angle
    ang viewAng;

    //! Viewport aspect ratio of w : h, ie height / width; rise / run
    f4 aspect;

    //! Close distance from the camera at which objects will stop being drawn
    f4 nearClip;

    //! Far distance from the camera at which objects will stop being drawn
    f4 farClip;

    //! Value between near and far clip at which picture plane is supposed
    f4 distance;
    // Arbitrary for perspective drawing; used for 2D zoom in orthogonal mode

    //! Tracker for whether view matrix is out of sync and needs recalculation
    bool dirtyView;

    //! Whether perspective matrix is out of sync and needs recalculation
    bool dirtyLook;

    //! Whether render scale device matrix is out of sync; needs recalculation
    bool dirtySlant;

  }; // end Camera class

} // end dp namespace
