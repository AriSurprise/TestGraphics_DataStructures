/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  OBB.h
Purpose:  Oriented Bounding Box (a.k.a. 3-DOP), fit to mesh's world coordinates
Details:  Inheriting from AABB directly with its chosen implementation may not
  be ideal.  AABB using matrix for rendering to store midpoint & extrema non-
  redundantly ceases to be sensible once rotation is introduced, making matrix
  elements composite with rotation, translate & scale information.  The added
  extrema coordinates also don't contribute meaningfully to OBB, for which
  collision must rely on separating axis theorem (SAT), as bracketed world
  coordinates no longer suffice or apply readily.  A hierarchy may still use
  an AABB for early rejection in determining collision, as the OBB should be
  a progressively tighter fit from an AABB, which in turn should be tighter
  than a BSphere, but a class hierarchy of shared polymorphic inheritance
  doesn't facilitate that hierarchy in any meaningful capacity, least of all
  under the chosen implementation.

  The OBB will need a matrix to be drawn efficiently, and can make full use of
  a Transform's versatility (unlike the rotation restricted AABB).  Due to the
  frequent need of axis extraction for SAT however, the simplicity of a matrix
  may still suit the needs better with direct storage of that local basis of
  projection, which amounts to a rotation.  The prior scale-translation matrix
  convention could still store the other data needed, but would expand storage
  significantly.  Ultimately, a 4x4 matrix is needed to hold concatenated data
  of the 3 matrices worth of data, & while storing a scale-tranclate m4f would
  limit that concatenation to a 2 matrix product instead of 3, it would mean an
  increase from 4^3 + 3x4x(2+3) = 124 to 3x4x4x4 = 192 bytes: at over 50% extra
  storage, it is a bit hard to justify, besides assuming that no matrix product
  shortcuts could be taken for efficiency.

Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_CS350_2
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
#include "m3f.h"        // Basis local vector matrix member (for rot & proj)
#include "m4f.h"        // Display matrix member (lite; no need for Transform)
#include "v3f.h"        // Model vertex world space coordinate extrema members

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  class AABB;    // Bounding box with world aligned sides, for collision tests
  class BSphere; // Bounding sphere <center, radius>, for collision tests
  class Mesh;    // Vertex data to scan for bounding volume context
  class Plane;   // Test intersections (bool, dist...) of Boundary <normal,sum>
  class Ray;     // Intersection tests (bool, dist) of (Point),<direction> pair
  class Triangle;// Test intersection (bool, dist...) with 2D boundary subset

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Oriented Bounding Box: fitted mesh context of world pos, scale & rotation
  class OBB
  {

  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create an Oriented Bounding Box from Mesh vertex data scan of bounds
    \brief
      - create an Oriented Bounding Box from Mesh vertex data scan of bounds
    \param mesh
      - mesh to have vertex data scanned for location, scale & orientation
    \param place
      - position, scale and rotation to convert mesh data to world coordinates
    \param samples
      - [1, s =: mesh.VertexCount()] sampled points read from vertex dataset
    \param step
      - [-s, 0),(0, s] point samples to be skipped per scan step
    \param first
      - [1, s] point samples to offset by in first scan iteration
    */
    OBB(const Mesh& mesh, const m4f& place, u4 samples = u4_INVALID,
      s2 step = 1, u2 first = 0);


    /** Create an Oriented Bounding Box from defaultable, loose float values
    \brief
      - create an Oriented Bounding Box from defaultable, loose float values
    \details
      - basis from up & forward will be conformed to orthonormal unit-vectors 
    \param mdX
      - cartesian center / mid-point x-coord of bounded model
    \param mdY
      - cartesian center / mid-point y-coord of bounded model
    \param mdZ
      - cartesian center / mid-point z-coord of bounded model
    \param scX
      - scalar half-extent x-span of bounded model
    \param scY
      - scalar half-extent y-span of bounded model
    \param scZ
      - scalar half-extent z-span of bounded model
    \param halvedExt
      - given <x, y, z> extents span half of bounded model dimens, or full if 0
    \param fwX
      - bounded model -local z-axis (facing vector) x-component of orientation
    \param fwY
      - bounded model -local z-axis (facing vector) y-component of orientation
    \param fwZ
      - bounded model -local z-axis (facing vector) z-component of orientation
    \param upX
      - bounded model local y-axis (up direction) x-component of orientation
    \param upY
      - bounded model local y-axis (up direction) y-component of orientation
    \param upZ
      - bounded model local y-axis (up direction) z-component of orientation
    \param formed
      - any given plane for intended local plane formed by primary & secondary
    */
    OBB(f4 mdX = 0.0f, f4 mdY = 0.0f, f4 mdZ = 0.0f,
        f4 scX = 0.5f, f4 scY = 0.5f, f4 scZ = 0.5f, bool halvedExt = true,
        f4 fwX = 0.0f, f4 fwY = 0.0f, f4 fwZ = 1.0f,
        f4 upX = 0.0f, f4 upY = 1.0f, f4 upZ = 0.0f, PLANE formed = PLANE::ZY);


    /** Create Oriented Bounding Box from center, scale & facing directions
    \brief
      - create Oriented Bounding Box from center, scale & facing directions
    \details
      - basis from up & forward will be conformed to orthonormal unit-vectors 
    \param pos
      - cartesian center / mid-point ( x, y, z ) coords of bounded model
    \param extents
      - scalar half-extents of bounded model's <x, y, z> dimensonal spans
    \param halvedExt
      - if true, extents are in bounded model's <x, y, z> half-spans, not full
    \param scale
      - scalar half-extents of bounded model's <x, y, z> dimensonal spans
    \param forward
      - local -z-axis vector: model's basis' <x, y, z> forward direction
    \param tilt
      - local y-axis vector: model's basis' <x, y, z> given up direction
    \param formed
      - any given plane for intended local plane formed by primary & secondary
    */
    OBB(const v3f& pos, const v3f& extents, bool halvedExt = false, const v3f&
      facing = Z_HAT, const v3f& tilt = Y_HAT, PLANE formed = PLANE::ZY);


    /** Copies an Axis Aligned Bounding Box from given source OBB settings
    \brief
      - copies an Axis Aligned Bounding Box from given source OBB settings
    \param source
      - cartesian <xMin,yMin,zMin,xMax,yMax,zMax> coordinate values to copy
    */
    OBB(const OBB& source);


    /** Moves an Oriented Bounding Box hijacking the given OBB r-value ref
    \brief
      - moves an Oriented Bounding Box hijacking the given OBB r-value ref
    \param source
      - cartesian < xMin, yMin, zMin, xMax, yMax, zMax > coord values to copy
    */
    OBB(OBB&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Methods                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get bounding box's < <U>, <V>, <N> > orthonormal local basis of rotation
    \brief
      - get bounding box's < <U>, <V>, <N> > orthonormal local basis of rotation
    \return
      - triplet of perpendicular unit-vectors for local < X, Y, Z > directions
    */
    inline const m3f& Basis(void) const
    { return basis; } // end const m3f& OBB::Basis(void) const                */

    /** Get bounding box's cartesian coordinate in (x, y, z) of model mid-point
    \brief
      - get bounding box's cartesian coordinate in (x, y, z) of model mid-point
    \return
      - cartesian coordinate of model's (x, y, z) mid-point of vertex data
    */
    inline const v3f& Center(void) const
    { return center; } // end const v3f& OBB::Center(void) const              */


    /** Get an OBB that's a copy of this OBB's current coord / scale settings
    \brief
      - get an OBB that's a copy of this OBB's current coord / scale settings
    \return
      - AABB r-value created from a copy constructor call to be edited freely
    */
    inline OBB Copy(void) const
    { return OBB(*this); } // end OBB OBB::Copy(void) const                   */


    /** Get bounding box's scalar half-extent spans in <x, y, z> scales
    \brief
      - get bounding box's scalar half-extent spans in <x, y, z> scales
    \return
      - half-sized vector-spans of model extents in local <x, y, z> scales
    */
    inline v3f HalfExt(void) const
    { return ext; } // end v3f OBB::HalfExt(void) const                       */


    /** Get whether matrix data state requires recalculation prior to output
    \brief
      - get whether matrix data state requires recalculation prior to output
    \details
      - no real reason to call externally, but no reason to privatize either
    \return
      - true only if internal matrix data is out of date with other settings
    */
    inline bool IsDirtyMat(void) const
    { return IsNegF(wfb[3][3]); } // end bool OBB::IsDirtyMat(void) const     */


    /** Get whether vertex linear coord data needs recalculation before output
    \brief
      - get whether vertex linear coord data needs recalculation before output
    \details
      - no real reason to call externally, but no reason to privatize either
    \return
      - true only if vertex extent data is out of date with other settings
    */
    inline bool IsDirtyVert(void) const
    { return IsNegF(wfb[2][3]); } // end bool OBB::IsDirtyVert(void) const    */


    /** Get whether vertex projection data needs recalculation before output
    \brief
      - get whether vertex projection data needs recalculation before output
    \details
      - no real reason to call externally, but no reason to privatize either
    \return
      - true only if axis projection data is out of date with other settings
    */
    inline bool IsDirtyProj(void) const
    { return IsNegF(wfb[1][3]); } // end bool OBB::IsDirtyProj(void) const    */


    /** Get whether surface-area state requires recalculation prior to output
    \brief
      - get whether surface-area state requires recalculation prior to output
    \details
      - no real reason to call externally, but no reason to privatize either
    \return
      - true only if internal surface-area is out of date with other settings
    */
    inline bool IsDirtySurface(void) const
    { return IsNegF(surface); } // end bool OBB::IsDirtySurface(void) const   */


    /** Get whether volume data state requires recalculation prior to output
    \brief
      - get whether volume data state requires recalculation prior to output
    \details
      - no real reason to call externally, but no reason to privatize either
    \return
      - true only if internal volume data is out of date with other settings
    */
    inline bool IsDirtyVolume(void) const
    { return IsNegF(volume); } // end bool OBB::IsDirtyVolume(void) const     */

    /** Get whether another oriented bounding box is inside this box's extents
    \brief
      - get whether another oriented bounding box is inside this box's extents
    \details
      - OBB derived data mutable to change contents desipte const labels
      entails separating-axis-theorem, across each of the 2 3-DOP's 6 axes
    \param other
      - world aligned bounding box to compare with this in boolean intersection
    \return
      - true if bounding box extrema per axis have overlap on each axis
    */
    bool IsIn(const AABB& other) const;


    /** Get whether a bounding sphere is inside this box's extents
    \brief
      - get whether a bounding sphere is inside this box's extents
    \param ball
      - bounding sphere to be evaluated against this for boolean intersection
    \return
      - true if bounding sphere radius is less than distance to closest plane
    */
    bool IsIn(const BSphere& ball) const;


    /** Get whether another oriented bounding box is inside this box's extents
    \brief
      - get whether another oriented bounding box is inside this box's extents
    \details
      - OBB derived data mutable to change contents desipte const labels
      entails separating-axis-theorem, across each of the 2 3-DOP's 6 axes
    \param other
      - oriented bounding box to be compared with this for boolean intersection
    \return
      - true if bounding box extrema per axis have overlap on each axis
    */
    bool IsIn(const OBB& other) const;


    /** Get whether a boundary is intersecting (straddling) this box's extents
    \brief
      - get whether a boundary is intersecting (straddling) this box's extents
    \param plane
      - 2D boundary to be evaluated against this for boolean intersection
    \return
      - true if box extremal vertices aren't all on the same side of the plane
    */
    bool IsIn(const Plane& plane) const;


    /** Get whether direction intersects this box's extents at positive scales
    \brief
      - get whether direction intersects this box's extents at positive scales
    \param ray
      - 3D direction-point pair to evaluate in boolean intersection with this
    \return
      - true if box extremal vertices aren't all on the same side of the plane
    */
    bool IsIn(const Ray& ray) const;


    /** Get whether a triangle intersects this box's extents at positive scales
    \brief
      - get whether a triangle intersects this box's extents at positive scales
    \param tri
      - 3D point triplet / plane subset to test for intersection with this
    \return
      - true if intersecting tri plane (on both sides), & in barycentric coords
    */
    bool IsIn(const Triangle& tri) const;


    /** Get whether a cartesian point is within the bounding box's extents
    \brief
      - get whether a cartesian point is within the bounding box's extents
    \details
      - using minor separating-axis-theorem using 1 point projection per axis
    \param point
      - cartesian world space coord to be evaluated for intersection with this
    \return
      - true only if given point is between OBB extrema per axis, on all axes
    */
    bool IsIn(const v3f& point) const;


    /** Local x-axis < x, y, z > direction in world space from rotation basis
    \brief
      - local x-axis < x, y, z > direction in world space from rotation basis
    \return
      - reference to rotation basis' local x-axis' < x, y, z > world direction
    */
    inline const v3f& LocalX(void) const
    { return basis[0]; } // end const v3f& OBB::LocalX(void) const            */


    /** Local y-axis < x, y, z > direction in world space from rotation basis
    \brief
      - local y-axis < x, y, z > direction in world space from rotation basis
    \return
      - reference to rotation basis' local y-axis' < x, y, z > world direction
    */
    inline const v3f& LocalY(void) const
    { return basis[1]; } // end const v3f& OBB::LocalY(void) const            */


    /** Local z-axis < x, y, z > direction in world space from rotation basis
    \brief
      - local z-axis < x, y, z > direction in world space from rotation basis
    \return
      - reference to rotation basis' local z-axis' < x, y, z > world direction
    */
    inline const v3f& LocalZ(void) const
    { return basis[2]; } // end const v3f& OBB::LocalZ(void) const            */


    /** Get homogeneous matrix affine-scaling generic 2x2x2 box to this OBB
    \brief
      - Get homogeneous matrix affine-scaling generic 2x2x2 box to this OBB
    \details
      - non-constant getter allowing latent matrix recalculations on-demand
    \return
      - updated OBB affine transform m4f converting a 2^{3} box to this state
    */
    inline const m4f& Matrix(void) const
    { return UpdateMatrix(); } // end m4f OBB::Matrix(void) const             */


    /** Get cartesian center coordinate ( x, y, z ) mid-point of model's bounds
    \brief
      - get cartesian center coordinate ( x, y, z ) mid-point of model's bounds
    \return
      - bounding box's cartesian center ( x, y, z ) mid-point of model's bounds
    */
    inline const v3f& Mid(void) const
    { return center; } // end const v3f& OBB::Mid(void) const                 */


    /** Get local points' world scale range spread across this OBB's local-X
    \brief
      - Get local points' world scale range spread across this OBB's local-X
    \details
      - non-constant getter allowing latent projection recalculations on-demand
    \return
      - updated world spread of this box's points projected on its local-X axis
    */
    inline const v2f& ProjectedX(void) const
    { UpdateProj(); return prX; } // end const v2f& OBB::ProjectedX(void)     */


    /** Get local points' world scale range spread across this OBB's local-Y
    \brief
      - Get local points' world scale range spread across this OBB's local-Y
    \details
      - non-constant getter allowing latent projection recalculations on-demand
    \return
      - updated world spread of this box's points projected on its local-Y axis
    */
    inline const v2f& ProjectedY(void) const
    { UpdateProj(); return prY; } // end const v2f& OBB::ProjectedY(void)     */


    /** Get local points' world scale range spread across this OBB's local-X
    \brief
      - Get local points' world scale range spread across this OBB's local-X
    \details
      - non-constant getter allowing latent projection recalculations on-demand
    \return
      - updated world spread of this box's points projected on its local-X axis
    */
    inline const v2f& ProjectedZ(void) const
    { UpdateProj(); return prZ; } // end const v2f& OBB::ProjectedZ(void)     */


    /** Get world scale < x, y, z > vector span of model bounds' half extents
    \brief
      - get world scale < x, y, z > vector span of model bounds' half extents
    \return
      - reference to contained bounds minimum < x, y, z > cartesian coordinate
    */
    inline const v3f& Scale(void) const
    { return 2.0f*ext; } // end const v3f& OBB::Scale(void) const             */


    /** Set box's bounds world-coords to be moved by the given displacement
    \brief
      - set box's bounds world-coords to be moved by the given displacement
    \param center
      - new bounding box mid-point <x, y, z> coordinates of local origin
    \param halvedExt
      - new bounding box cartesian <x, y, z> dimensional scalars around center
    \param spans
      - dimens scalars are read as full extents if true; half extents if not
    \return
      - reference to the modified OBB for easier subsequent inline action
    */
    OBB& Set(const v3f& center, const v3f& extents, bool halvedExt = true,
      const v3f& facing = Z_HAT, const v3f& tilt = Y_HAT, PLANE formed
      = PLANE::UNDEFINED);


    /** Get world scale surface area of current bounding box settings in use
    \brief
      - get world scale surface area of current bounding box settings in use
    \details
      - non-constant getter allowing on-demand recalculation of surface-area
    \return
      - value of the contained member storage of surface area calculation
    */
    inline f4 Surface(void) const
    { return UpdateSurface(); } // end f4 OBB::Surface(void)                  */


    /** Get world scale surface area of current bounding box settings in use
    \brief
      - get world scale surface area of current bounding box settings in use
    \details
      - non-constant getter allowing on-demand recalculation of volume data
    \return
      - value of the contained member storage of surface area calculation
    */
    inline f4 Volume(void) const
    { return UpdateVolume(); } // end f4 OBB::Volume(void)                    */


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Operator Overloads                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get this OBB after being moved in the given direction / distance
    \brief
      - get this OBB after being moved in the given direction / distance
    \param displacement
      - center gets offset in <x, y, z> by this vector span
    \return
      - resultant OBB after being moved by the given displacement vector
    */
    inline OBB operator+(const v3f& displacement) const
    {
      return (Copy() += displacement);

    } // end OBB OBB::operator+(const v3f&) const                             */


    /** Set box's bounds world-coords to be moved by the given displacement
    \brief
      - set box's bounds world-coords to be moved by the given displacement
    \param displacement
      - mid-point, min and max will uniformly recieve this offset in <x, y, z>
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    inline OBB& operator+=(const v3f& displacement)
    {
      center += displacement; // move the center
      SetDirtyMatrix();
      SetDirtyProj();
      return *this;
    } // end OBB& OBB::operator+=(const v3f&) const                          */


    /** These bounds grown to the product with a dimensional set of scalars
    \brief
      - these bounds grown to the product with a dimensional set of scalars
    \param scale
      - dimensional scalar changes per local axis desired in bounding box size
    \return
      - reference to the modified OBB for easier subsequent inline action
    */
    inline OBB& operator*(const v3f& scale)
    { return Copy() *= scale; } // end OBB& OBB::operator*=(const v3f&)       */


    /** Grow these bounds to the product with a dimensional set of scalars
    \brief
      - grow these bounds to the product with a dimensional set of scalars
    \param scale
      - dimensional scalar changes per local axis desired in bounding box size
    \return
      - reference to the modified OBB for easier subsequent inline action
    */
    inline OBB& operator*=(const v3f& scale)
    {
      ext *= scale.Abs();
      SetDirtyScale();
      SetDirtyVert();
      SetDirtyProj();
      return *this;

    } // end OBB& OBB::operator*=(const v3f&) const                          */


    /** Grow bounds using the origin & extents of an AABB as affine scalars
    \brief
      - grow bounds using the origin & extents of an AABB as affine scalars
    \param other
      - AABB used for origin (mid-point), and dimensional scalar extents
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    OBB& operator*=(const AABB& other);


    /** Grow bounds using an OBB as affine scalars & basis rotation
    \brief
      - grow bounds using an OBB as affine scalars & basis rotation
    \details
      - OBB having a midpoint and extents like an AABB, scales & adjusts center
      as if both were unrotated AABB: other.center being affine scalar origin,
      new center from resultant extents' midpoint. Then this basis rotates by
      other.basis (*=), & then recalculate the concatenated transform matrix.
    \param other
      - AABB used for origin (mid-point), and dimensional scalar extents
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    OBB& operator*=(const OBB& other);

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Methods                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** This box's world scale point spread range on local x-axis of other OBB
    \brief
      - this box's world scale point spread range on local x-axis of other OBB
    \details
      - takes shortcuts under assumptions U => IsUniform() scalars => symmetry
      UNLESS bVert is non-null, then uses that list of 8 vertices box to
      project each onto the range
    \param axis
      - direction or point projection source from this OBB's basis
    \return
      - world scale [min, max] range of this box's points on other.LocalX()
    */
    v2f ProjSpan(v3f axis, const v3f* bVert = nullptr) const;

    /** Note when internal data changes necessitating matrix recalculation
    \brief
      - note when internal data changes necessitating matrix recalculation
    */
    inline void SetDirtyMatrix(void) const
    { wfb[3][3] = -1.0f; } // void OBB::SetDirtyMatrix(void)                  */


    /** Note when data reliant on vertex extent scalars requires recalculation
    \brief
      - note when data reliant on vertex extent scalars requires recalculation
    */
    inline void SetDirtyVert(void) const
    {
      wfb[2][3] = -0.0f;
    } // void OBB::SetDirtyProj(void)                                         */


    /** Note when data reliant on projection scalars requires recalculation
    \brief
      - note when data reliant on projection scalars requires recalculation
    */
    inline void SetDirtyProj(void) const
    {
      wfb[1][3] = -0.0f;
    } // void OBB::SetDirtyProj(void)                                         */


    /** Note when data reliant on half-extents requires recalculation
    \brief
      - note when data reliant on half-extents requires recalculation
    */
    inline void SetDirtyScale(void) const
    {
      surface = MinF(-surface, surface);
      volume = MinF(-volume, volume);
      SetDirtyMatrix();
      SetDirtyVert();
      SetDirtyProj();
    } // void OBB::SetDirtyScale(void)                                        */


    /** Correct projection scalar calculations as needed, prior to output
    \brief
      - correct projection scalar calculations as needed, prior to output
    \details
      - individual axis updates hypercompartmentalized: only ever need all 3
    */
    inline void UpdateProj(void) const
    {
      if(IsDirtyProj())
      {
        f4 mid; // Projection of midpoint per axis (extents self-parallel)
        mid = center.Dot(basis['x']); // Projected center on local-X; parallel
        prX.Set(mid-ext.x, mid+ext.x);  //    => symmetrical midpoint x-offsets
        mid = center.Dot(basis['y']); // Projected center on local-Y; parallel
        prX.Set(mid-ext.y, mid+ext.y);  //    => symmetrical midpoint y-offsets
        mid = center.Dot(basis['z']); // Projected center on local-Z; parallel
        prX.Set(mid-ext.z, mid+ext.z);  //    => symmetrical midpoint z-offsets
        wfb[2][3] = 0.0f; // reset dirty Z flag
      }
    } // end void OBB::UpdateProj(void)                                       */


    /** Correct surface area calculation as needed, prior to surface output
    \brief
      - correct surface area calculation as needed, prior to surface output
    */
    inline const f4& UpdateSurface(void) const
    {
      if(IsDirtySurface()){volume=(ext.x*ext.y+ext.x*ext.z+ext.y*ext.z)*8.0f;}
      return surface;

    } // end const f4& OBB::UpdateSurface(void)                               */


    /** Correct volumetric calculations as needed, prior to volume output
    \brief
      - correct volumetric calculations as needed, prior to volume output
    */
    inline const f4& UpdateVolume(void) const
    {
      if (IsDirtyVolume()) { volume = ext.x * ext.y * ext.z * 8.0f; }
      return volume;

    } // end const f4& OBB::UpdateVolume(void)                                */


    /** Correct the transformation matrix as needed, prior to matrix output
    \brief
      - correct the transformation matrix as needed, prior to matrix output
    */
    inline const m4f& UpdateMatrix(void) const
    {
      bool vertToo = IsDirtyVert();
      bool projToo = IsDirtyProj();
      if (IsDirtyMat())
      {
        wfb = (ScaleMat(ext)*basis).M4()*TranslateAffine(center);
        if (vertToo) { SetDirtyVert(); } // prefer over obligate recalculation
        if (projToo) { SetDirtyProj(); } // dirty flags =: calculate as needed
      }
      return wfb;
    } // end const m4f& OBB::UpdateMatrix(void)                               */


    /** Correct the vertex linear extent data as needed, prior to vector output
    \brief
      - correct the vertex linear extent data as needed, prior to vector output
    */
    inline const m4f& UpdateVert(void) const
    {
      if (IsDirtyVert())
      {
        m3f lAxis(basis['x'] * ext.x, basis['y'] * ext.y, basis['z'] * ext.z);
        point[1].Set( lAxis['x']+lAxis['y']+lAxis['z']); // Octant 1: + + +
        point[2].Set( lAxis['x']+lAxis['y']-lAxis['z']); // Octant 2: + + -
        point[3].Set( lAxis['x']-lAxis['y']-lAxis['z']); // Octant 3: + - -
        point[4].Set( lAxis['x']-lAxis['y']+lAxis['z']); // Octant 4: + - +
        point[5].Set(-lAxis['x']-lAxis['y']+lAxis['z']); // Octant 5: - - +
        point[6].Set(-lAxis['x']-lAxis['y']-lAxis['z']); // Octant 6: - - -
        point[7].Set(-lAxis['x']-lAxis['y']-lAxis['z']); // Octant 7: - + -
        point[0].Set(-lAxis['x']+lAxis['y']+lAxis['z']); // Octant 8: - + +
        wfb[1][3] = 0.0f; // Clear the set dirty flag
      }
      return wfb;
    } // end const m4f& OBB::UpdateMatrix(void)                               */


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Vertex / Element lists of 2-unit cube data for drawing any given OBB
    static const Mesh bBox;

    //! Orthogonal unit-vector triplet; local <x, y, z> directions =: rotation
    m3f basis;

    //! Cartesian center / mid-point of bounded model's world coordinates
    v3f center;

    //! World coord vector spans of scalars for bounded model's (half), extents
    v3f ext;

    //! Concatenated world coordinate transform matrix from static bounding box
    mutable m4f wfb;
    // Derived matrix's dirty flag in [3][3]MSB: affine [3][3] =: 1.0 -> -1.0
    // Dirty flag MSBs of point scales in [2][3] =: 0.0 -> -0.0 (no coeffects)
    // Dirty flag MSBs of projections in [1][3] =: 0.0 -> -0.0 (no coeffects)

    //! World scalars of box's rotated coordinates if linear (coord -= center)
    mutable v3f point[8];
    // Derived matrix's dirty flag in [3][3]MSB: affine [3][3] =: 1.0 -> -1.0
    // Dirty flag MSBs of projections [2][3] =: 0.0 -> -0.0 (no coeffects)

    //! World scale box point projections on this box's local-X basis vector
    mutable v2f prX;
    // Derived data of projection world scale set's dirty flag on wfb[2][3] MSB

    //! World scale box point projections on this box's local-Y basis vector
    mutable v2f prY;
    // Derived data of projection world scale set's dirty flag on wfb[2][3] MSB

    //! World scale box point projections on this box's local-Z basis vector
    mutable v2f prZ;
    // Derived data of projection world scale set's dirty flag on wfb[2][3] MSB

    mutable f4 surface; //! Box's world scale surface area: 2*w*h +2*w*d +2*h*d
    // Derived data's dirty flag on MSB; surface area should never be negative

    mutable f4 volume; //! Box's total world scale volume: width *height *depth
    // Derived data's dirty flag on MSB as volume should never be negative

    mutable f4 radius; //! Box's total world scale volume: width *height *depth
    // Derived data's dirty flag on MSB as volume should never be negative

  }; // end OBB class declaration

  // Ensure footprint is as intended: (((3+2)*3)+(4*4)+(3*8)+(3*2)+3) * 4-bytes
  static_assert(sizeof(OBB) == 64*4, "OBB size not packed");

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

} // end dp namespace
