/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  BSphere.h
Purpose:  4D Convex bounding volume for containing a mesh by centroid and radius
Details:  member footprint kept in line with v4f use of <x,y,z,w=radius> pattern
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
#include "v3f.h"  // Model vertex world space coordinate extrema members


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{
  class AABB; // Bounding Box using extrema, for conversion in/out of BSpheres
  union v4f;  // 4D vector capable of being used for abstract <v3f, f4> I/O

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Extremal Points Of Spread: categorical vector subsets maximizing 0/1 vals
  enum class EPOS 
  {
    Ritter, //! Use 001: world-axis unit-vectors: {1,0,0}, {0,1,0}, & {0,0,1}
    _6 = Ritter, //! EPOS-6 uses 3 001 vectors *2 to find =: 6 extremal points
    Larson3,//! Use 111: 4 added {1, +/-1, +/-1} vector combinations / sqrt(3)
    _14 = Larson3, //! EPOS-14 adding 4 111 vectors to 001 ones => 7*2 points
    Larson2,//! Use 011: 6 added {1,-/+1,0},{1,0,-/+1}, & {0,1,-/+1} / sqrt(2)
    _26 = Larson2, //! EPOS-26 adding 6 011 vectors to 111 ones => 13*2 points
    Larson5,//! Use 012: {0,1,+2},{0,2,+1},{1,0,+2},{2,0,+1},{1,+2,0},{2,+1,0}
    _50 = Larson5, //! EPOS-50 adding 12 012 vectors to 011 ones => 25*2 points
    Larson6,//! Use 112: {1,+/-1,+/-2}, {1,+/-2,+/-1}, & {2,+/-1,+/-1}/sqrt(6)
    _74 = Larson6, //! EPOS-74 adding 12 112 vectors to 012 ones => 37*2 points
    Larson9,//! Use 122: {1,+/-2,+/-2}, {2,+/-2,+/-1}, & {2,+/-1,+/-2}/sqrt(9)
    _98 = Larson9, //! EPOS-98 adding 12 122 vectors to 112 ones => 49*2 points
  };
  
  //! Bounding Sphere: <centroid, radius> bounding mesh world coords & scale
  class BSphere
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create a bounding Sphere bound by Mesh vertex data scan in EPOS method
    \brief
      - create a bounding Sphere bound by Mesh vertex data scan in EPOS method
    \param mesh
      - mesh to have vertex data read for maximal spread to use as data bounds
    \param placement
      - context to transform mesh vertex data to world coordinates
    \param scan
      - mesh scan complexity [Ritter, Larson, PCA] methods to use for bounds
    \param samples
      - [1, s =: mesh.VertexCount()] sampled points read from vertex dataset
    \param step
      - [-s, 0),(0, s] point samples to be skipped per scan step
    \param first
      - [1, s] point samples to offset by in first scan iteration
    */
    BSphere(const Mesh& mesh, const m4f& placement, EPOS scan = EPOS::Ritter,
      u4 samples = u4_INVALID, s2 step = 1, u2 first = 0u);


    /** Create a bounding sphere bound by Mesh vertex data scan in PCA method
    \brief
      - create a bounding Sphere bound by Mesh vertex data scan in PCA method
    \param mesh
      - mesh to have vertex data read for maximal spread to use as data bounds
    \param placement
      - context to transform mesh vertex data to world coordinates
    \param samples
      - [1, s =: mesh.VertexCount()] sampled points read from vertex dataset
    \param step
      - [-s, 0),(0, s] point samples to be skipped per scan step
    \param first
      - [1, s] point samples to offset by in first scan iteration
    */
    BSphere(const Mesh& mesh, const m4f& placement, u4 samples,
      s2 step, u2 first = 0u);


    /** Create a Bounding Sphere (default < centroid.xyz, radius >)
    \brief
      - create a Bounding Sphere (default < centroid.xyz, radius >)
    \param x
      - world space coord x-axis component from model's vertex data centroid
    \param y
      - world space coord y-axis component from model's vertex data centroid
    \param z
      - world space coord z-axis component from model's vertex data centroid
    \param radius
      - world space radius extent from given model's vertex data center
    */
    BSphere(f4 x = 0.0f, f4 y = 0.0f, f4 z = 0.0f, f4 radius = 0.0f);


    /** Create a Bounding Sphere from the given < center, radius > values
    \brief
      - create a Bounding Sphere from the given < center, radius > values
    \param center
      - cartesian < x, y, z > coordinate found as centroid of model vertex set
    \param radius
      - maximum radius length from given center within model's vertex data
    */
    BSphere(const v3f& center, f4 radius);


    /** Create a BSphere using v4f where <x,y,z> is the origin; w the radius
    \brief
      - create a BSphere using v4f where <x,y,z> is the origin; w the radius
    \param source
      - 4D <x,y,z,w> float data set to use as center(x,y,z), radius(w)
    */
    BSphere(const v4f& source);


    /** Create a Bounding Sphere encompasing a given source AABB's set values
    \brief
      - create a Bounding Sphere encompasing a given source AABB's set values
    \details
      - use AOBB mid as center, ext.length as radius: not a tight BSphere
    \param source
      - source AABB for setting this where: center(s.mid), radius(s.ext.length)
    */
    BSphere(const AABB& source);


    /** Copy a Bounding Sphere from another given source BSphere's settings
    \brief
      - copy a Bounding Sphere from another given source BSphere's settings
    \param source
      - < center.x, center.y, center.z, radius > values to copy into this
    */
    BSphere(const BSphere& source);


    /** Move data into a Bounding Sphere, hijacking the given r-value ref
    \brief
      - move data into a Bounding Sphere, hijacking the given r-value ref
    \param result
      - < center.x, center.y, center.z, radius > data to appropriate in this
    */
    BSphere(BSphere&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Methods                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Access bounding sphere's world space center point of model vertex data
    \brief
      - access bounding sphere's world space center point of model vertex data
    \return
      - cartesian world space center coordinate where object center lies
    */
    inline const v3f& Center(void) const
    { return (*(v3f*)&wfb[3]); }// end const v3f& BSphere::Center(void) const */

    /** Set appropriate sphere center point in world space of model vertex data
    \brief
      - set appropriate sphere center point in world space of model vertex data
    \param center
      - world space cartesian centroid <x,y,z> coord of model's vertex data
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    inline BSphere& Center(const v3f& center)
    {
      (*(v3f*)&wfb[3]) = center;
      return *this;

    } // end BSphere& BSphere::Center(const v3f&)                             */


    /** Expand BSphere to encompass any mesh data points from current settings
    \brief
      - expand BSphere to encompass any mesh data points from current settings
    \param mesh
      - mesh to use in growing bounds to encompass world coords of vertex data
    \param place
      - position, scale and rotation to convert mesh data to world coordinates
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    BSphere& ExpandedTo(const std::vector<v3f>& wPosVerts);


    /** Get whether another bounding sphere is inside this sphere's extents
    \brief
      - get whether another bounding sphere is inside this sphere's extents
    \param other
      - bounding sphere to be evaluated against this for boolean intersection
    \return
      - true if center to center distance is less than the combined radius
    */
    inline bool IsIn(const BSphere& other)
    {
      f4 cRad = R() + other.R();
      return (cRad*cRad <= DistV3F(Mid(), other.Mid()).LengthSquared());

    } // end bool BSphere::In(const BSphere&) const                           */


    /** Get whether a cartesian point is within the bounding sphere's extents
    \brief
      - get whether a cartesian point is within the bounding sphere's extents
    \param point
      - cartesian world space coord to be evaluated for intersection with this
    \return
      - true if given point is within BSphere radius' distance from the center
    */
    inline bool IsIn(const v3f& point) const
    {
      return (RadiusSquared() <= DistV3F(Mid(), point).LengthSquared());
    } // end bool BSphere::In(const v3f&) const                               */


    /** Access bounding sphere's world space center point of model vertex data
    \brief
      - access bounding sphere's world space center point of model vertex data
    \return
      - cartesian world space center coordinate where object center lies
    */
    inline const m4f& Matrix(void) const
    { return wfb; } // end const m4f& BSphere::Matrix(void) const             */


    /** Access bounding sphere's world space center point of model vertex data
    \brief
      - access bounding sphere's world space center point of model vertex data
    \return
      - cartesian world space center coordinate where object center lies
    */
    inline const v3f& Mid(void) const
    { return Center(); } // end const v3f& BSphere::Mid(void) const           */


    /** Set appropriate sphere center point in world space of model vertex data
    \brief
      - set appropriate sphere center point in world space of model vertex data
    \param center
      - world space cartesian centroid <x,y,z> coord of model's vertex data
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    inline BSphere& Mid(const v3f& center)
    { return Center(center); } // end BSphere& BSphere::Mid(const v3f&)       */


    /** Access bounding sphere's world space center point of model vertex data
    \brief
      - access bounding sphere's world space center point of model vertex data
    \return
      - cartesian world space center coordinate where object center lies
    */
    inline const v3f& Origin(void) const
    { return Center(); } // end const v3f& BSphere::Origin(void) const        */


    /** Set appropriate sphere center point in world space of model vertex data
    \brief
      - set appropriate sphere center point in world space of model vertex data
    \param center
      - world space cartesian centroid <x,y,z> coord of model's vertex data
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    inline BSphere& Origin(const v3f& center)
    { return Center(center); } // end BSphere& BSphere::Origin(const v3f&)    */


    /** Access bounding sphere's world space center point of model vertex data
    \brief
      - access bounding sphere's world space center point of model vertex data
    \return
      - cartesian world space center coordinate where object center lies
    */
    inline const v3f& Point(void) const
    { return Center(); } // end const v3f& BSphere::Point(void) const         */


    /** Set appropriate sphere center point in world space of model vertex data
    \brief
      - set appropriate sphere center point in world space of model vertex data
    \param center
      - world space cartesian centroid <x,y,z> coord of model's vertex data
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    inline BSphere& Point(const v3f& center)
    { return Center(center); } // end BSphere& BSphere::Point(const v3f&)     */


    /** Access bounding sphere's world space model vertex data extents radius
    \brief
      - access bounding sphere's world space model vertex data extents radius
    \return
      - cartesian world space center coordinate where object center lies
    */
    inline const f4& R(void) const
    { return Radius(); } // end const f4& BSphere::R(void) const              */


    /** Set appropriate sphere radius in world space for model vertex data
    \brief
      - set appropriate sphere radius in world space for model vertex data
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    inline BSphere& R(f4 radius)
    { return Radius(radius); } // end BSphere& BSphere::R(f4)                 */


    /** Access bounding sphere's world space model vertex data extents radius
    \brief
      - access bounding sphere's world space model vertex data extents radius
    \return
      - cartesian world space center coordinate where object center lies
    */
    inline const f4& Rad(void) const
    { return Radius(); } // end const f4& BSphere::Rad(void) const            */


    /** Set appropriate sphere radius in world space for model vertex data
    \brief
      - set appropriate sphere radius in world space for model vertex data
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    inline BSphere& Rad(f4 radius)
    { return Radius(radius); } // end BSphere& BSphere::Rad(f4)               */


    /** Get bounding sphere's world scale radius value, squared
    \brief
      - get bounding sphere's world scale radius value, squared
    \return
      - value or radius in the bounding sphere, times itself: r^2; R() * R()
    */
    inline f4 RadSq(void) const
    { return RadiusSquared(); } // end f4 BSphere::RadSq(void) const          */


    /** Access bounding sphere's world space model vertex data extents radius
    \brief
      - access bounding sphere's world space model vertex data extents radius
    \return
      - longest extents of vertex data in associated model from center point
    */
    inline const f4& Radius(void) const
    { return wfb[0][0]; } // end const f4& BSphere::Radius(void) const        */


    /** Set appropriate sphere radius in world space for model vertex data
    \brief
      - set appropriate sphere radius in world space for model vertex data
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    inline BSphere& Radius(f4 radius)
    {
      wfb[2][2] = wfb[1][1] = wfb[0][0] = AbsF(radius); return *this;
    } // end BSphere& BSphere::Radius(f4)                                     */


    /** Get bounding sphere's world scale radius value, squared
    \brief
      - get bounding sphere's world scale radius value, squared
    \return
      - value or radius in the bounding sphere, times itself: r^2; R() * R()
    */
    inline f4 RadiusSquared(void) const
    { return Rad() * Rad(); } // end f4 BSphere::RadiusSquared(void) const    */


    /** Get bounding sphere's world scale radius value, squared
    \brief
      - get bounding sphere's world scale radius value, squared
    \return
      - value or radius in the bounding sphere, times itself: r^2; R() * R()
    */
    inline f4 RSq(void) const
    { return RadiusSquared(); } // end f4 BSphere::RSq(void) const            */


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Operator Overloads                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set BSphere from another's settings for radius / center point data
    \brief
      - set BSphere from another's settings for radius / center point data
    \param other
      - other BSphere from which settings for radius / center point copies data
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    inline BSphere& operator=(const BSphere& other)
    {
      wfb[0][0] = other.wfb[0][0]; // Prefer to set only changed subscripts
      wfb[1][1] = other.wfb[0][0];
      wfb[2][2] = other.wfb[0][0];
      wfb[3][0] = other.wfb[3][0];
      wfb[3][1] = other.wfb[3][1];
      wfb[3][2] = other.wfb[3][2];
      return *this;
    } // end BSphere& BSphere::operator=(const BSphere&)                      */

    /** Set BSphere from another's settings for radius / center point data
    \brief
      - set BSphere from another's settings for radius / center point data
    \param displacement
      - distance vector moving the center point 
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    inline BSphere& operator+=(const v3f& displacement)
    {
      wfb[3][0] += displacement.x;
      wfb[3][1] += displacement.y;
      wfb[3][2] += displacement.z;
      return *this;
    } // end BSphere& BSphere::operator=(const v3f&)                          */

    /** Grow BSphere radius data by a given uniform constant scalar value
    \brief
      - grow BSphere radius data by a given uniform constant scalar value
    \param scalar
      - ratio by which to grow / scale radius of this BSphere
    \return
      - reference to the modified BSphere for easier subsequent inline action
    */
    inline BSphere& operator*=(f4 scalar)
    {
      scalar = AbsF(scalar);
      wfb[0][0] *= scalar;
      wfb[1][1] *= scalar;
      wfb[2][2] *= scalar;
      return *this;
    } // end BSphere& BSphere::operator*=(f4)                                 */

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Methods                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Mesh representative of any given bounding sphere in the right transform
    static const Mesh bBall;

    //! Homogeneous transform storing uniform scale (radius), and [3] (origin)
    m4f wfb;
    // redundant data needed for generic bounding sphere world coord rendering

    // Model vertices notably not noted as radius & center generalize heavily
    // could change as tight bounding sphere implementations reference extrema,
    // but no use case unlike bounding boxes, due to rotational invariance,
    // plus, extrema not finite unlike with AABB, meaning irregular sized lists

  }; // end BSphere class declaration

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                      Helper Function Declarations                        */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Create a Bounding Sphere by Mesh world axis spread of vertex bounds
  \brief
    - create a Bounding Sphere by Mesh world axis spread of vertex bounds
  \param mesh
    - mesh to have vertex data read for maximal spread to use as data bounds
  \param place
    - position, scale and rotation to convert mesh data to world coordinates
  \param samples
    - [1, s =: mesh.VertexCount()] sampled points read from vertex dataset
  \param step
    - [-s, 0),(0, s] point samples to be skipped per scan step
  \param first
    - [1, s] point samples to offset by in first scan iteration
  \return
    - bounding sphere initially created from world axis point spread, grown
  */
  BSphere RitterSphere(const Mesh& mesh, const m4f& place,
    u4 samples = u4_INVALID, s2 step = 1, u2 first = 0u);


  /** Create Bounding Sphere by presumed, predefined, local vertex spread axes
  \brief
    - create Bounding Sphere by presumed, predefined, local vertex spread axes
  \param mesh
    - mesh to have vertex data read for maximal spread to use as data bounds
  \param place
    - position, scale and rotation to convert mesh data to world coordinates
  \param samples
    - [1, s =: mesh.VertexCount()] sampled points read from vertex dataset
  \param step
    - [-s, 0),(0, s] point samples to be skipped per scan step
  \param first
    - [1, s] point samples to offset by in first scan iteration
  \param count
    - subset of categorical axes for data spread to project along in scan
  \return
    - bounding sphere initially defined by dictionary axis point spread, grown
  */
  BSphere LarsonSphere(const Mesh& mesh, const m4f& place,
    u4 samples = u4_INVALID, s2 step = 1, u2 first = 0u,
    EPOS count = EPOS::Larson3);


  /** Create a Bounding Sphere by solving for Mesh local axes of vertex spread
  \brief
    - create a Bounding Sphere by solving for Mesh local axes of vertex spread
  \param mesh
    - mesh to have vertex data read for maximal spread to use as data bounds
  \param place
    - position, scale and rotation to convert mesh data to world coordinates
  \param samples
    - [1, s =: mesh.VertexCount()] sampled points read from vertex dataset
  \param step
    - [-s, 0),(0, s] point samples to be skipped per scan step
  \param first
    - [1, s] point samples to offset by in first scan iteration
  \param basis
    - optional pass in value to store the PCA method's interim eigen-vectors
  \param basis
    - optional pass in value to store the PCA method's interim eigen-vectors
  \return
    - bounding sphere initially from statistic analysis of point spread, grown
  */
  BSphere PCASphere(const Mesh& mesh, const m4f& place,
    u4 samples = u4_INVALID, s2 step = 1, u2 first = 0,
    m3f* eigenVectors = nullptr, v3f* eigenValues = nullptr);


 /*^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
 /*                             Helper Constants                              */
 /*^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  // Ensurance that memory footprint is as intended
  static_assert(sizeof(BSphere) == 64, "BSphere not packed");

} // end dp namespace
