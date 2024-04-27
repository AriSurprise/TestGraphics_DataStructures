/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Transform.h
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

#include "ang.h"  // Inputs for rotation setting
#include "v3f.h"  // Constant definitions for initialization
#include "v4f.h"  // Constant definitions for initialization
#include "m4f.h"  // Output (T)SRT concatenated placement / transform matrix

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Transform //! Spatial display context in world coordinate space
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a Transform from a set of v3f descriptors for 3D placement data
    \brief
      - create a Transform from a set of v3f descriptors for 3D placement data
      -
    */
    Transform(const v3f& position = nullv3f, const v3f& dimensions = WHITE,
      const v3f& anchor = nullv3f, const v3f& orientationRads = nullv3f);


    /** Create a Transform from v3f / ang descriptors for 2.5D placement data
    \brief
      - create a Transform from v3f / ang descriptors for 2.5D placement data
    */
    Transform(const v3f& position, const v3f& dimensions, const v3f& anchor,
      ang rotation_z, ang rotation_y = nullang, ang rotation_x = nullang);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get Transform's displacement anchor for local offset from object center
    \brief
      - get Transform's displacement anchor for local offset from object center
    \return
      - reference to the contained Transform offset anchor displacement vector
    */
    const v3f& Anchor(void) const;


    /** Set Transform anchor to use displacement for local center offset
    \brief
      - set Transform anchor to use displacement for local center offset
    \param anchorOffset
      - dimensional offset vector from object center to desired local center
    \param displacement
      - if true, anchorDisplacement will be added to existing anchor offsets
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& Anchor(const v3f& anchorOffset, bool displacement = false);


    /** Set Transform anchor to use the given offset as center of transformation
    \brief
      - set Transform anchor to use the given offset as center of transformation
    \param objectCenterOffset
      - offset vector from object center to center scale / rotations about
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& CenterAround(const v3f& objectCenterOffset);


    /** Set Transform anchor to move by the given amount from current values
    \brief
      - set Transform anchor to move by the given amount from current values
    \param anchorDisplacement
      - amount by which transformation offset anchor point is to be moved
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& CenterSlide(const v3f& anchorDisplacement);


    /** Set Transform to move by given amount from current values in local Z
    \brief
      - set Transform to move by given amount from current values in local Z
    \param forwardDistance
      - amount by which the transform should move (+/-) in local Z orientation
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& Dolly(f4 forwardDistance);


    /** Get directional unit vector for local space X, or right orientation
    \brief
      - get directional unit vector for local space X, or right orientation
    \return
      - reference to the contained orientation vector of local X direction
    */
    const v3f& LocalX(void) const;


    /** Get directional unit vector for local space Y, or up orientation
    \brief
      - get directional unit vector for local space Y, or up orientation
    \return
      - reference to the contained orientation vector of local Y direction
    */
    const v3f& LocalY(void) const;


    /** Get directional unit vector for local space Z, or back orientation
    \brief
      - get directional unit vector for local space Z, or back orientation
    \return
      - reference to the contained orientation vector of local Z direction
    */
    const v3f& LocalZ(void) const;


    /** Get transformation matrix placing model space vertices into world space
    \brief
      - get transformation matrix placing model space vertices into world space
    \details
      - though a getter, must be non-constant to recalculate only as needed
    \return
      - concatenated transformation m4f into the described world coord basis
    */
    const m4f& Matrix(void);


    /** Set Transform position to use the given displacement from current value
    \brief
      - set Transform position to use the given displacement from current value
    \param displacement
      - values to offset current Transform position by (w ignored; vector math)
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& MoveBy(const v3f& displacement);


    /** Set Transform object center translation of world coords without anchor
    \brief
      - set Transform object center translation of world coords without anchor
    \param objectCenter
      - new world coordinates for Transform position (w ignored: point assumed)
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& MoveOnto(const v3f& objectCenter);


    /** Set Transform position to use the given absolute world coordinates
    \brief
      - set Transform position to use the given absolute world coordinates
    \param location
      - new world coordinates for Transform position (w ignored: point assumed)
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& MoveTo(const v3f& location);


    /** Get transformation matrix placing model space normals into world space
    \brief
      - get transformation matrix placing model space normals into world space
    \details
      - though a getter, must be non-constant to recalculate only as needed
    \return
      - concatenated transformation m4f into the described world vector basis
    */
    const m4f& NormalMatrix(void);


    /** Get euler angles of cartesian rotation per local axis
    \brief
      - get euler angles of cartesian rotation per local axis
    \return
      - reference to the contained orientation vector of radian euler angles
    */
    const v3f& Orientation(void) const;


    /** Set euler angle of cartesian rotation per local axis in <z, y, x> order
    \brief
      - set euler angle of cartesian rotation per local axis in <z, y, x> order
    \param z_rot
      - amount by which to rotate about z-axis (will be stored as f4 radians)
    \param y_rot
      - amount by which to rotate about y-axis (will be stored as f4 radians)
    \param x_rot
      - amount by which to rotate about x-axis (will be stored as f4 radians)
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& Orientation(const ang& z_rot, const ang& y_rot = nullang,
      const ang& x_rot = nullang);


    /** Set Transform to move by given amount from current values in local X
    \brief
      - set Transform to move by given amount from current values in local X
    \param rightDistance
      - amount by which the transform should move (+/-) in local X orientation
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& Pan(f4 rightDistance);


    /** Rotate the local orientation along the vertical axis (+up / -down)
    \brief
      - rotate the local orientation along the vertical axis (+up / -down)
    \param pitchChange
      - amount by which to offset (or set), rotation about the local x axis
    \param relative
      - absolute x rotation value is set if disabled instead of relative change
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& Pitch(ang pitchChange, bool relative = true);


    /** Get current affine sum world coordinates of Transform position stored
    \brief
      - get current affine sum world coordinates of Transform position stored
    \return
      - reference to the contained Transform translation (position) vector
    */
    const v3f& Position(void) const;


    /** Get current world coords of Transform object center as input position
    \brief
      - get current world coords of Transform object center as input position
    \param exact
      - ostensibly to get the displayed center point, but value doesn't matter:
      if any value is provided, this distinguishes the return type
    \return
      - reference to the contained Transform translation (position) vector
    */
    v3f Position(bool displayedCenter) const;


    /** Set position of the Transform to use the given coordinates / movement
    \brief
      - set position of the Transform to use the given coordinates / movement
    \param coords
      - values to use for Transform position / translation setting / movement
    \param displacement
      - whether to use relative offset math (if true), from current position
    \param faceValue
      - whether to apply coords as specified, ignoring anchor offsets
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& Position(const v3f& coords, bool displacement = false, 
      bool faceValue = false);


    /** Rotate the local orientation about the view normal (+clockwise / -ccw)
    \brief
      - rotate the local orientation about the view normal (+clockwise / -ccw)
    \param rollChange
      - amount by which to offset (or set), rotation about the local z axis
    \param relative
      - absolute z rotation value is set if disabled instead of relative change
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& Roll(ang rollChange, bool relative = true);


    /** Get 2D rotation of the object, presuming no other axial rotation values
    \brief
      - get 2D rotation of the object, presuming no other axial rotation values
    \return
      - reference to the contained Z value of the orientation v3f of radians
    */
    const f4& RotateZRad(void) const;


    /** Get the size (unitless) of object to be displayed, per local axis
    \brief
      - get the size (unitless) of object to be displayed, per local axis
    \return
      - reference to the contained orientation vector of radian euler angles
    */
    const v3f& Scale(void) const;


    /** Set the size (unitless) of object to be displayed, per local axis
    \brief
      - set the size (unitless) of object to be displayed, per local axis
    \param dimensionalScalars
      - local scalar ratio per axis for display relative to model dimensions
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& Scale(const v3f& dimensionalScalars);


    /** Set Transform to move by given amount from current values in local Y
    \brief
      - set Transform to move by given amount from current values in local Y
    \param upwardDistance
      - amount by which the transform should move (+/-) in local Y orientation
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& Tilt(f4 upwardDistance);


    /** Rotate the local orientation along the horizontal axis (+right / -left)
    \brief
      - rotate the local orientation along the horizontal axis (+right / -left)
    \param yawChange
      - amount by which to offset (or set), rotation about the local y axis
    \param relative
      - absolute y rotation value is set if disabled instead of relative change
    \return
      - reference to the modified Transform for easier subsequent inline action
    */
    Transform& Yaw(ang yawChange, bool relative = true);


  protected:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Protected Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Local sum (coords+offset), relative to parent space: world coords
    v3f translation;
    // TODO: refactor for nesting to local, parent coordinate spaces' transform

    //! Local unit vector for the current orientation's x-axis (u|pitch vector)
    v3f localX;

    //! Local unit vector for the current orientation's y-axis (v|yaw vector)
    v3f localY;

    //! Local unit vector for the current orientation's z-axis (n|roll vector)
    v3f localZ;

    //! Flag tracks if any settings changed; matrix/norm needs recalculation
    bool dirty;

    //! Flag tracks if position changed; moveMat needs recalculation
    bool dirtyPos;

    //! Flag tracks if rotation changed; rotMat needs recalculation
    bool dirtyRot;

    //! Flag tracks if orientation was set; local vectors need recalculation
    bool dirtyLocals;


  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Methods                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set internal local unit vectors to reflect current orientation
    \brief
      - set internal concat m4f to represent current summary transformations
    */
    void RecalculateBasis(void);

    /** Update euler orientation values to be reflective of local basis vectors
    \brief
      - update euler orientation values to be reflective of local basis vectors
    */
    void RecalculateEulers(void);

    /** Set internal concat m4f to represent current summary transformations
    \brief
      - set internal concat m4f to represent current summary transformations
    */
    void RecalculateMatrix(void);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    // TODO: refactor for ECS references (after a custom allocator?)

    //! Transformation record to convert to specified object->world dimensions
    m4f sizeMat;

    //! Transformation record to convert to affine pivot-centric space
    m4f anchorMat;

    //! Transformation record to convert to current local space orientation
    m4f rotMat;

    //! Transformation record to convert to current object->world coordinates
    m4f moveMat;

    //! Sum transformation to coordinate basis described by component v4f's
    m4f concat;
    // Translation * Rotation * Scale * Offset (right-multiplied of course)

    //! Linear inverse transpose of concatenated sum matrix (for normsals)
    m4f norm;

    //! Affine anchor vector: relative offset for scale / rotation center point
    v3f offset;

    //! Euler angle of rotation in radians about z, y, x ordering
    v3f orientRad;
    // TODO: refactor v3f -> gimbal of ang members after gimbal class is made

    //! Inverse scale setting to restore normalized dimensions (for anchor)
    v3f rescale;

    //! Size of object in x, y and z relative to object coordinate spacing
    v3f scale;

    //! Flag tracks if scale changed; sizeMat needs recalculation (+anchor)
    bool dirtyScale;

    //! Flag tracks if anchor point changed; anchorMat needs recalculation
    bool dirtyAnchor;


  }; // end Transform class

  // 4^3 per m4f, 3*4 per v3f, 6 bools w/2 trailing, +2 padding on 4-byte bounds
  static_assert(sizeof(Transform) == 120*4 + 6+2, "Transform not packed");
  // TODO: norm => m3f, bools to bitfield 486-(7+5) = 473, %4=1 => +3 padding...

} // end dp namespace
