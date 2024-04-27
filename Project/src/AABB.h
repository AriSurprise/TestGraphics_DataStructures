/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  AABB.h
Purpose:  Axis Aligned Bounding Box context of mesh data spread in world coords

Details:  In storage & footprint decisions, most comparisons are relative to
  the extrema, while the alternative using center and half-extent relative
  offset span from it is more flexible to reuse data as objects move & rotate,
  begging the question of which model to use: extrema or midpoint.

  Extrema make sense to not recalculate (however trivial): each frame for each
  operation on each box adds up, if only to draw boxes from extrema & not even
  get into potential collision detection.  Displaying extrema would entail
  storing unique mesh vertex data, per box's unique world coords, & require a
  basic, static identity matrix to set consistent world coords as expected.

  The only alternative necessitates a static, uniform mesh & unique matrix
  storing how to scale & position that mesh (or else redundant recalculations).
  A better opportunity may be afforded there, in that with no rotation (per
  AABB definitions and restrictions to be world axis aligned at all times), the
  matrix trace & translation column store the center & extents already, without
  any convolutions.  By a simple change to an NDC box instead of a unit one,
  the matrix's data falls perfectly in line.  A matrix is already preferable to
  store over a full transform (taking extra memory for added versatility which
  categorically can't be utilized by an AABB), so of the two, a matrix is the
  more sensible member to store, per AABB.  Then, if the matrix must be stored
  to avoid re-calculation per frame regardless, & incidentally stores that
  <mid, ext> data directly, in its elements, it makes much more sense to use
  the <mid, ext> convention over the <min, max>, as they don't even need space
  dedicated if being set/read (somewhat unintuitively), directly in the matrix.

  This does not preclude the last option to use both: the downsides being in
  an increased footprint, & extra work to maintain both value sets each update.
  The bigger footprint is usually conceded to be worthwhile, if compared with
  redundant calculation arriving at consistent results.  AABB extrema will be
  redundant, if calculated more than once per frame.  The argument of entailing
  extra work is mitigated threefold, first by virtue that static data may never
  need any recalculation, then that the two are trival to switch between
  (only needing 3D addition), & lastly, extrema are needed for even basics,
  including most collision tests, even if false.  Since they get set only once
  per box per frame, & are assuredly used at least once to determine collisions
  that frame (provided at least 2 objects exist, cycles being expendable in any
  other simple case), either potential downside is thus discountable: storing
  both < min, max > & < mid, ext > (via the matrix), is thus the preferred
  footprint convention chosen, ordering given by an m4f being first since
  (4x4x4 bytes), easily aligns on most any systems in binary based addressal,
  without adding padding between members (likely no trailing bytes either,
  unless ported to a 128-bit system).

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
#include "m4f.h"        // Render matrix member (liter; no need for Transform)
#include "v2f.h"        // World space axis projection extrema scalar members
#include "v3f.h"        // Model vertex world space coordinate extrema members
//#include "v3u.h"        // Members to track mesh extrema subscripts associated
// Relevant after hill climbing is possible via convex hull support / ordering

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  class BSphere;  // Bounding sphere <center, radius>, for collision tests
  class Mesh;     // Vertex data to scan for bounding volume context
  class Plane;    // Boundary <normal, sum>; test intersection (bool, dist...)
  class Smpl;     // Sampling struct definition to iterate over mesh vertex data
  class Transform;// Spatial context chagning mesh from model to world coords
  union v6f;      // 6D vector capable of being used for <v3f, v3f> AABB I/O

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  //! Axis Aligned Bounding Box: mesh absolute world coordinates and scale
  class AABB
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                       Public Class Constants                           */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Vertex / Element lists of 2-unit cube data for drawing any given AABB
    static const Mesh BoundingBox;
    // Alernative? have each box store exhaustive extremal points: not concise

    static const u4 PLANES = 3u; //! AABB in 3D space has 3 axes: < x, y, z >

    static const u4 POINTS = 8u; //! AABB as a cuboid subset, have 8 corners

    //! Data state for <v6f> =: < <v3f>,<v3f> > read as a vector / point pair
    enum class Format
    {
      Ranges, //! Box region data defined in midpoint -/+ half-extent spans
      Extrema,//! Box region data defined in extremal [min, max] points
      Sweep   //! Box region data in <(point),<vector>> <corner, extents> Ray
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create an Axis Aligned Bounding Box by Mesh data scan of vertex bounds
    \brief
      - create an Axis Aligned Bounding Box by Mesh data scan of vertex bounds
    \param mesh
      - mesh to have vertex data read for extremal / centroid / extent details
    \param place
      - position, scale and rotation to convert mesh data to world coordinates
    \param samples
      - [1, s =: mesh.VertexCount()] sampled points read from vertex dataset
    \param step
      - [-s, 0),(0, s] point samples to be skipped per scan step
    \param first
      - [1, s] point samples to offset by in first scan iteration
    */
    AABB(const Mesh& mesh, const m4f& place, u4 samples, s2 step, u2 first);
    

    /** Creates an Axis Aligned Bounding Box (default < INF.xyz, -INF.xyz >)
    \brief
      - creates an Axis Aligned Bounding Box (default < INF.xyz, -INF.xyz >)
    \details
      - max before min as 0 minimum is conventional & therefore defaultable in
      v3f construction, therefore in keeping with that pattern here as well
      counter to subscript ordering which emplaces <min, max> from <max, min>
    \param xB
      - minimum (Extrema), or mid-point (Range), cartesian x-coord in model
    \param yB
      - minimum (Extrema), or mid-point (Range), cartesian y-coord in model
    \param zB
      - minimum (Extrema), or mid-point (Range), cartesian z-coord in model
    \param xE
      - maximum (Extrema) x-coord, or half-extent (Range) x-span, in model
    \param yE
      - maximum (Extrema) y-coord, or half-extent (Range) y-span, in model
    \param zE
      - maximum (Extrema) z-coord, or half-extent (Range) z-span, in model
    */
    AABB (f4 xB = INf, f4 yB = INf, f4 zB = INf,
          f4 xE =-INf, f4 yE =-INf, f4 zE =-INf,
          Format input = Format::Extrema);


    /** Creates an Axis Aligned Bounding Box from given < max, min > values
    \brief
      - creates an Axis Aligned Bounding Box from given < max, min > values
    \details
      - max before min as 0 minimum is conventional & therefore defaultable,
      counter to subscript ordering which emplaces <min, max> from <max, min>
    \param max
      - maximum (Extrema), or mid-point (Range), < x, y, z > coords in model
    \param min
      - minimum coords (Extrema), or half-extent spans (Range) of model <x,y,z>
    */
    AABB(const v3f& begin, const v3f& end, Format input = Format::Extrema);


    /** Copies an Axis Aligned Bounding Box from given source AABB settings
    \brief
      - copies an Axis Aligned Bounding Box from given source AABB settings
    \param source
      - cartesian <xMin,yMin,zMin,xMax,yMax,zMax> coordinate values to copy
    \param input
      - interpret v6f <v3f,v3f> members as Extrema, or <center, half-extents>
    */
    AABB(const v6f& source, Format input = Format::Extrema);


    /** Copies an Axis Aligned Bounding Box from given source AABB settings
    \brief
      - copies an Axis Aligned Bounding Box from given source AABB settings
    \param source
      - cartesian <xMin,yMin,zMin,xMax,yMax,zMax> coordinate values to copy
    */
    AABB(const AABB& source);


    /** Moves an Axis Aligned Bounding Box hijacking the given AABB r-value ref
    \brief
      - moves an Axis Aligned Bounding Box hijacking the given AABB r-value ref
    \param source
      - cartesian < xMin, yMin, zMin, xMax, yMax, zMax > coord values to copy
    */
    AABB(AABB&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Methods                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get an AABB that's a copy of this AABB's current coord / scale settings
    \brief
      - get an AABB that's a copy of this AABB's current coord / scale settings
    \return
      - AABB r-value created from a copy constructor call to be edited freely
    */
    inline AABB Copy(void) const
    { return AABB(*this); } // end AABB AABB::Copy(void) const                */

    /** Get bounding box's scalar half extent spans in <x, y, z>
    \brief
      - get bounding box's scalar half extent spans in <x, y, z>
    \return
      - true if bounding box extrema per axis have overlap on each axis
    */
    inline v3f HalfExt(void) const
    { return v3f(extX, extY, extZ); } // end v3f AABB::HalfExt(void) const    */


    /** Get whether vertex linear coord data needs recalculation before output
    \brief
      - get whether vertex linear coord data needs recalculation before output
    \details
      - no real reason to call externally, but no reason to privatize either
    \return
      - true only if vertex extent data is out of date since settings changes
    */
    inline bool IsDirtyProj(void) const
    { return IsNegF(projD); } // end bool AABB::IsDirtyProj(void) const       */


    /** Get whether vertex linear coord data needs recalculation before output
    \brief
      - get whether vertex linear coord data needs recalculation before output
    \details
      - no real reason to call externally, but no reason to privatize either
    \return
      - true only if vertex extent data is out of date since settings changes
    */
    inline bool IsDirtyVerts(void) const
    { return IsNegF(pointD); } // end bool AABB::IsDirtyVerts(void) const     */


    /** Get whether another bounding box is inside this box's extents
    \brief
      - get whether another bounding box is inside this box's extents
    \param other
      - bounding box to be evaluated against this for boolean intersection
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


    /** Get whether a boundary is intersecting (straddling) this box's extents
    \brief
      - get whether a boundary is intersecting (straddling) this box's extents
    \param plane
      - 2D boundary to be evaluated against this for boolean intersection
    \return
      - true if box extremal vertices aren't all on the same side of the plane
    */
    bool IsIn(const Plane& plane) const;


    /** Get whether a cartesian point is within the bounding box's extents
    \brief
      - get whether a cartesian point is within the bounding box's extents
    \param point
      - cartesian world space coord to be evaluated for intersection with this
    \return
      - true only if given point is between AABB extrema per axis, on all axes
    */
    inline bool IsIn(const v3f& point) const
    {return point.IsIn(max, min);}// end bool AABB::IsIn(const v3f&) const    */


    /** Access homogeneous matrix affine-scaling generic 2x2x2 box to this AABB
    \brief
      - access homogeneous matrix affine-scaling generic 2x2x2 box to this AABB
    \details
      - fragility contingent to changing m4f footprint, putting dirty flags of
      derived data in MSBs of linear 0.0 cell values, as setting those 0.0 ->
      -0.0 state changes nothing about matrix usage. But even row<->col major
      hypothetical m4f change not being a significant issue, as it means the
      worst case is needing to take a transpose, or shuffle order.  Also really
      not apt to happen or have any other footprint changes: column major is
      clearly computationally preferable given well managed row major output.
    \return
      - reference to contained bounds maximum < x, y, z > cartesian coordinate
    */
    inline const m4f& Matrix(void) const
    { return *(const m4f*)&extX; } // end const m4f& AABB::Matrix(void) const */


    /** Access maximum cartesian < x, y, z > coordinate of model bounds
    \brief
      - access maximum cartesian < x, y, z > coordinate of model bounds
    \return
      - reference to contained bounds maximum < x, y, z > cartesian coordinate
    */
    inline const v3f& Max(void) const
    { return max; } // end const v3f& AABB::Max(void) const                   */


    /** Access cartesian < x, y, z > coordinate of bounds' [min,max] mid-point
    \brief
      - access cartesian < x, y, z > coordinate of bounds' [min,max] mid-point
    \details
      - Get matrix column vector of translation, sans v4f's homogeneous w-value
        =: homogeneous affine-scale transform (no rotation), stores midpoint
    \return
      - bounding box's cartesian < x, y, z > mid-point coordinate of min & max
    */
    inline const v3f& Mid(void) const
    { return mid; } // end const v3f& AABB::Mid(void) const                   */


    /** Access minimum cartesian < x, y, z > coordinate of model bounds
    \brief
      - access minimum cartesian < x, y, z > coordinate of model bounds
    \return
      - reference to contained bounds minimum < x, y, z > cartesian coordinate
    */
    inline const v3f& Min(void) const
    { return min; } // end const v3f& AABB::Min(void) const                   */


    /** Get the plane extrapolated from box's extremal nearest to given point
    \brief
      - get the plane extrapolated from box's extremal nearest to given point
    \details
      - use cubemap logic to divide space: center to point max signed component
    \param point
      - coordinate to be compared with box extrema to find clases plane
    \return
      - plane from AABB, in a (-/+) world axis, placed in box's world coords
    */
    Plane SideBy(const v3f& point) const;


    /** Set box's bounds world-coords to be moved by the given displacement
    \brief
      - set box's bounds world-coords to be moved by the given displacement
    \param center
      - new bounding box mid-point <x, y, z> coordinates of local origin
    \param dimens
      - new bounding box cartesian <x, y, z> dimensional scalars around center
    \param spans
      - dimens scalars are read as full extents if true; half extents if not
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    AABB& Set(const v3f& center, const v3f& dimens, bool spans = false);


    /** Set box's bounds world-coords to be moved by the given displacement
    \brief
      - set box's bounds world-coords to be moved by the given displacement
    \param dimens
      - new bounding box cartesian <x, y, z> dimensional scalars
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    AABB& SetExt(const v3f& dimens);


    /** Set bounding box's world-coords to be moved by the given displacement
    \brief
      - set bounding box's world-coords to be moved by the given displacement
    \param center
      - new bounding box mid-point <x, y, z> coordinates
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    AABB& SetPos(const v3f& center);


    /** Get head of list for 8 world scale vertex spans (midpoint -> corners)
    \brief
      - get head of list for 8 world scale vertex spans (midpoint -> corners)
    \details
      - recalculates derived data latently, on-demand
    \return
      - pointer to list head of 8 AABB extrema spans from mid to vertex corners
    */
    inline const v3f* Vertices(void) const
    {
      UpdateVerts();
      return &point[0];

    } // end const const v3f* AABB::Vertices(void) const                */


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Operator Overloads                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get this AABB after being moved in the given direction / distance
    \brief
      - get this AABB after being moved in the given direction / distance
    \param displacement
      - mid-point, min and max uniformly recieving this offset in <x, y, z>
    \return
      - resultant AABB after being moved by the given displacement vector
    */
    AABB operator+(const v3f& displacement) const;


    /** Set box's bounds world-coords to be moved by the given displacement
    \brief
      - set box's bounds world-coords to be moved by the given displacement
    \param displacement
      - mid-point, min and max will uniformly recieve this offset in <x, y, z>
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    AABB& operator+=(const v3f& displacement);


    /** Get the combined bounds of a boolean union of this AABB with another
    \brief
      - get the combined bounds of a boolean union of this AABB with another
    \return
      - resultant AABB that has the combined extremal points of the given AABB
    */
    inline AABB operator+(const AABB& other) const
    {return Copy() += other;} // end AABB AABB::operator+(const AABB&) const  */


    /** Grow these bounds using boolean union of this AABB with another
    \brief
      - grow these bounds using boolean union of this AABB with another
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    AABB& operator+=(const AABB& other);


    /** Shrink bounds using boolean difference of this AABB with another
    \brief
      - shrink bounds using boolean difference of this AABB with another
    \details
      - only causes a change in very specific cases of other circumscribing all
      of this in 2 axes while (at least), a partial intersection exists in the
      last dimension: not when partial overlap exists in 2 or more dimensions,
      nor is there an effect when this reciprocally circumscribes other at all
    \param dif
      - the AABB to remove from this box's volume (then made solid again)
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    AABB& operator-=(const AABB& dif);


    /** Grow these bounds using the origin & extents of an v3f as scalars
    \brief
      - grow these bounds using the origin & extents of an v3f as scalars
    \param scale
      - dimensional scalar changes per world axis desired in bounding box size
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    AABB& operator*(const v3f& scale);


    /** Grow these bounds using the origin & extents of an v3f as scalars
    \brief
      - grow these bounds using the origin & extents of an v3f as scalars
    \param scale
      - dimensional scalar changes per world axis desired in bounding box size
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    AABB& operator*=(const v3f& scale);


    /** Grow bounds using the origin & extents of an AABB as affine scalars
    \brief
      - grow bounds using the origin & extents of an AABB as affine scalars
    \param other
      - AABB used for origin (mid-point), and dimensional scalar extents
    \return
      - reference to the modified AABB for easier subsequent inline action
    */
    AABB& operator*=(const AABB& other);

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Methods                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Note derived self-projection data as out of date; needing latent update
    \brief
      - note derived self-projection data as out of date; needing latent update
    */
    inline void SetDirtyProj(void) const
    { projD = -0.0f; } // end void SetDirtyProj(void) const                   */

    /** Note derived point extent list as out of date; needing a latent update
    \brief
      - note derived point extent list as out of date; needing a latent update
    */
    inline void SetDirtyVerts(void) const
    { pointD = -0.0f; } // end void SetDirtyVerts(void) const                 */

    /** Validate bounds for extrema that have memberwise maxima > minima
    \brief
      - validate bounds for extrema that have memberwise maxima > minima
    */
    void ValidateBounds(void);

    /** Correct mid-point coord, half-extent scalars on min/max change (redraw)
    \brief
      - correct mid-point coord, half-extent scalars on min/max change (redraw)
    */
    void UpdateMatrix(void);

    /** Correct derived self-projection data of world basis coordinate ranges
    \brief
      - correct derived self-projection data of world basis coordinate ranges
    */
    inline void UpdateProj(void) const
    {
      if (IsDirtyProj())
      {
        projD = 0.0f;
      }
    }

    /** Correct derived point list data of box corner extents on latent update
    \brief
      - correct derived point list data of box corner extents on latent update
    */
    inline void UpdateVerts(void) const
    {
      if (IsDirtyVerts())
      {
        pointD = 0.0f;
      }
    }

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Footprint in format of transform matrix from object to world coords
    // <X,|O,|O,|O>, <O,|Y,|O, O>, <O, O,|Z,|O>,|<Tx, Ty, Tz,|I>
    
    f4 extX; //! LocalX.x scale; transform matrix[0][0]; HalfExt().x
    mutable f4 projD; //! MSB dirty flag of self-projection range; matrix[0][1]
    mutable f4 pointD; //! MSB dirty flag of vertex extents; matrix[0][2]
    const v2f pX = nullv2f; // Constant matrix[0][3] & [1][0] cells
    f4 extY; //! LocalY.y scale; transform matrix[1][1]; HalfExt().y
    const v4f pZ = nullv4f; // Constant matrix[1][2], [1][3], [2][0] & [2][1]
    f4 extZ; //! LocalZ.z scale; transform matrix[2][2]; HalfExt().z
    const f4 lY = 0.0f; // Constant matrix[2][3] cell, always 0.0f
    v3f mid; //! Center point; transform matrix [3][0-2] affine translate col
    const f4 lTw = 1.0f; // Constant matrix[3][3] homogeneous coord, always 1.0f
    /* Same as an m4f affine transform, but derived data uses empty cell MSBs
     0.0 -> -0.0 has no effect on matrix math, meaning a bit from constant
     0.0 cells can get leveraged to be used as dirty flags without impacting
     overall footprint, instead introducing either fragility with respect to
     m4f footprint changes (alternatively making the whole matrix non-constant,
     not a realistic option: holds all the core state data)
    */
    
    //! Cartesian low extremal values from a model's vertex set in world coords
    v3f min;

    //! Cartesian high extremal values from a model's vertex set in world coords
    v3f max;

    //! Cartesian, linear scalar offsets of vertex world coords from mid-point
    mutable v3f point[POINTS];
    // Derived data of vertex data's dirty flag MSB of wfb[2][3]: 0.0 -> -0.0

    //! Cartesian extremal world coordinate ranges, per local=:world AABB axis
    mutable v2f pr[PLANES];

  }; // end AABB class declaration

  // Ensurance that memory footprint is as intended
  static_assert(sizeof(AABB) == (52)*4, "AABB size not packed");

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

} // end dp namespace
