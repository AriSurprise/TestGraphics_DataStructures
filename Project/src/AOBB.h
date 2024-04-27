/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  AOBB.h
Purpose:  Container for mesh center / half extent <x,y,z> <coordinate, vector>
Details: member footprint kept in line with v6f use of <mid, ext> pattern
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
#include "v3f.h"  // Defining members of AOBB: center, extents <x,y,z> values
#include "v3u.h"  // Efficiency adjustment to allow tracking mesh subscripts
#include "Mesh.h" // Reference the vertex data measured in AOBB (need nullmesh)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  class AABB; // AOBB extrema variant instead of < centroid, extents >
  class BSphere;// Bounding sphere <center, radius>, for collision tests
  class Plane;  // Boundary <normal, sum>; test intersection (bool, dist, point)
  union v6f;  // 6D vector capable of being used for <v3f, v3f> AOBB I/O

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  //! Axis Oriented Bounding Box: <mid, ext> world pos & half scale
  class AOBB
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Creates an Axis Oriented Bounding Box (default < <0,0,0>, <0,0,0> >)
    \brief
      - creates an Axis Oriented Bounding Box (default < <0,0,0>, <0,0,0> >)
    \param xMid
      - world space center cartesian x coordinate found in model vertices
    \param yMid
      - world space center cartesian y coordinate found in model vertices
    \param zMid
      - world space center cartesian z coordinate found in model vertices
    \param xExt
      - world space cartesian x axis half extent found in vertices from center
    \param yExt
      - world space cartesian y axis half extent found in vertices from center
    \param zExt
      - world space cartesian z axis half extent found in vertices from center
    */
    AOBB(f4 xMid = 0.0f, f4 yMid = 0.0f, f4 zMid = 0.0f,
      f4 xExt = 0.0f, f4 yExt = 0.0f, f4 zExt = 0.0f);


    /** Creates an Axis Oriented Bounding Box from given < max, min > values
    \brief
      - creates an Axis Oriented Bounding Box from given < max, min > values
    \details
      - max before min as 0 minimum is conventional & therefore defaultable,
      counter to subscript ordering which emplaces <min, max> from <max, min>
    \param center
      - world space middle cartesian < x, y, z > coordinates found in model
    \param halfExtent
      - axial offset from center spanned by vertices found in model
    */
    AOBB(const v3f& center, const v3f& halfExtent = nullv3f);


    /** Creates an Axis Oriented Bounding Box from given source AABB settings
    \brief
      - creates an Axis Oriented Bounding Box from given source AABB settings
    \param source
      - cartesian <xMin, yMin, zMin, xMax, yMax, zMax> coordinate values to copy
    */
    AOBB(const AABB& source);


    /** Creates an Axis Oriented Bounding Box from given source AOBB settings
    \brief
      - creates an Axis Oriented Bounding Box from given source AOBB settings
    \param source
      - cartesian <xMin,yMin,zMin,xMax,yMax,zMax> coordinate values to copy
    \param inRanges
      - if true, will interpret v6f <v3f,v3f> members as AABB for conversion
    */
    AOBB(const v6f& source, bool inRanges = false);


    /** Copies an Axis Oriented Bounding Box from given source AOBB settings
    \brief
      - copies an Axis Oriented Bounding Box from given source AOBB settings
    \param source
      - cartesian < xMid, yMid, zMid, xExt, yExt, zExt > values to copy
    */
    AOBB(const AOBB& source);


    /** Moves an Axis Oriented Bounding Box hijacking the given AOBB r-value ref
    \brief
      - moves an Axis Oriented Bounding Box hijacking the given AOBB r-value ref
    \param source
      - cartesian < xMin, yMin, zMin, xMax, yMax, zMax > values to copy
    */
    AOBB(AOBB&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Methods                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Access model vertex set half extent vector of extrema from the centroid
    \brief
      - access model vertex set half extent vector of extrema from the centroid
    \return
      - reference to contained bounds maximum < x, y, z > cartesian coordinate
    */
    inline const v3f& Ext(void) const
    { return ext; } // end const v3f& AOBB::Ext(void) const                   */


    /** Get whether another bounding box is inside this box's extents
    \brief
      - get whether another bounding box is inside this box's extents
    \param other
      - bounding box to be evaluated against this for boolean intersection
    \return
      - true only if bounding box extrema have overlap per axis, on all axes
    */
    bool In(const AABB& box) const;


    /** Get whether a cartesian point is within the bounding box's extents
    \brief
      - get whether a cartesian point is within the bounding box's extents
    \param other
      - bounding box to be evaluated against this for boolean intersection
    \return
      - true only if bounding box extrema have overlap per axis, on all axes
    */
    inline bool In(const AOBB& other) const
    {
      // AOBB should afford sphere collision logic as more efficient
      return AbsF(mid.x - other.mid.x) <= ext.x + other.ext.x
          && AbsF(mid.y - other.mid.y) <= ext.y + other.ext.y
          && AbsF(mid.z - other.mid.z) <= ext.z + other.ext.z;

    } // end bool AOBB::In(const AOBB&) const                                 */


    /** Get whether a cartesian point is within the bounding box's extents
    \brief
      - get whether a cartesian point is within the bounding box's extents
    \param point
      - cartesian world space coord to be evaluated for intersection with this
    \return
      - true only if given point is between AOBB extrema per axis, on all axes
    */
    inline bool In(const v3f& point) const
    {
      v3f min(mid - ext), max(mid + ext);
      return IsInF(point.x, min.x, max.x)
          && IsInF(point.y, min.y, max.y)
          && IsInF(point.z, min.z, max.z);
    } // end bool AOBB::In(const v3f&) const                                  */


    /** Access model's vertex set cartesian center point in world space coords
    \brief
      - access model's vertex set cartesian center point in world space coords
    \return
      - reference to model's bounds centroid < x, y, z > cartesian coordinate
    */
    inline const v3f& Mid(void) const
    { return mid; } // end const v3f& AOBB::Mid(void) const                   */


    /** Access mesh whose data AOBB is bounding / summarizing (in model coords)
    \brief
      - access mesh whose data AOBB is bounding / summarizing (in model coords)
    \return
      - reference to Mesh (if any), bounded by AOBB data (nullmesh if none)
    */
    inline const Mesh& Model(void) const
    {
      return mod ? *mod : nullmesh;

    } // end const Mesh& AOBB::Model(void) const                              */

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Operator Overloads                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Grow these bounds using boolean union of this AOBB with another
    \brief
      - grow these bounds using boolean union of this AOBB with another
    \return
      - reference to the modified AOBB for easier subsequent inline action
    */
    inline AOBB& operator+=(const AOBB& other)
    {
      // Convert both to AABB ranges
      v3f min = mid - ext;
      v3f max = mid + ext;
      v3f oMin = other.mid - other.ext;
      v3f oMax = other.mid + other.ext;

      // Get absolute extrema from both ranges
      min.x = MinF(min.x, oMin.x);    max.x = MaxF(max.x, oMax.x);
      min.y = MinF(min.y, oMin.y);    max.y = MaxF(max.y, oMax.y);
      min.z = MinF(min.z, oMin.z);    max.z = MaxF(max.z, oMax.z);

      // Convert absolute extrema back to AOBB < centroid, halfExtents >
      mid = (max + min) / 2.0f;
      ext = (max - min) / 2.0f;
      return *this;

    } // end AOBB& AOBB::operator+=(const AOBB&)                              */


    /** Move AOBB center point by given translation / displacement vector
    \brief
      - move AOBB center point by given translation / displacement vector
    \param displacement
      - amount per axis by which existing center values will be offset (+=)
    \return
      - reference to the modified AOBB for easier subsequent inline action
    */
    inline AOBB& operator+=(const v3f& displacement)
    {
      mid += displacement;
      return *this;

    } // end AOBB& AOBB::operator+=(const v3f&)                               */


    /** Scale bounds using an AOBB as a center (midpoint), and 3D scalars (ext)
    \brief
      - scale bounds using an AOBB as a center (midpoint), and 3D scalars (ext)
    \details
      - scaling takes, at most, an origin (point) and per axis scalars (vector),
      which already semantically matching what this data is, & operators
      having parameter limits, this makes more sense than adding a contrived
      data type for the purposes of allowing more dynamic scaling
    \param other
      - midpoint acting as scalar center, extents acting as 3D scalar set
    \return
      - reference to the modified AOBB for easier subsequent inline action
    */
    inline AOBB& operator*=(const AOBB& other)
    {
      // Get AABB planar extrema from AOBB (without the include, to inline it)
      v3f min = mid - ext;
      v3f max = mid + ext;

      // Adjust extrema by given scalar center distances (by extents) per axis
      min = other.mid - ((min - other.mid) * other.ext);
      max = other.mid + ((max - other.mid) * other.ext);

      // Set in terms of AOBB (< center, halfExtents >) from scaled AABB
      mid = (max + min) / 2.0f;
      ext = (max - min) / 2.0f;
      return *this;

    } // end AOBB& AOBB::operator*=(const AOBB&)                              */


    /** Scale extents memberwise using 3D scalars per axis (leave center as is)
    \brief
      - scale extents memberwise using 3D scalars per axis (leave center as is)
    \param scale
      - 3D scalar set to multiply AOBB extents by, memberwise, per axis
    \return
      - reference to the modified AOBB for easier subsequent inline action
    */
    inline AOBB& operator*=(const v3f& scale)
    {
      ext *= scale;
      return *this;

    } // end AOBB& AOBB::operator*=(const v3f&)                               */


    /** Scale extents by uniform scalar constant per axis (leave center as is)
    \brief
      - scale extents by uniform scalar constant per axis (leave center as is)
    \param scalar
      - uniform scalar quantity to multiply AOBB extents by per axis
    \return
      - reference to the modified AOBB for easier subsequent inline action
    */
    inline AOBB& operator*=(f4 scalar)
    {
      ext *= scalar;
      return *this;

    } // end AOBB& AOBB::operator*=(f4)                                       */

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Methods                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Validate bounds for extents that add to maxima; subtract to minima
    \brief
      - validate bounds for extents that add to maxima; subtract to minima
    */
    inline void ValidateBounds(void)
    {
      SetAbsF(ext.x);
      SetAbsF(ext.y);
      SetAbsF(ext.z);

    } // end void ValidateBounds(void)                                        */

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Model for whose vertex data this box represents a bounds
    Mesh const* mod;

    //! 3D cartesian coord of vertex set mid-point: < mid.x, mid.y, mid.z >
    v3f mid;

    //! 3D cartesian vector of vertex set half extents: < ext.x, ext.y, ext.z >
    v3f ext;

  }; // end AOBB class declaration

  // Ensurance that memory footprint is as intended
  static_assert(sizeof(AOBB) == 1*sizeof(um) + 4*6, "AOBB size not packed");

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

} // end dp namespace
