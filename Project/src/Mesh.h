/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Mesh.h
Purpose:  Polygonal geometric / attribute container with buffers of vert data
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

// <stl>
#include <set>          // Proximal vert subscript members with subset lists
#include <string>       // File parsing based constructor argument
#include <vector>       // Containers for passing / holding mesh buffer data
// "./src/..."
#include "v3f.h"       // Buffer list members for coord / attribute context
#include "v2f.h"       // Buffer list members for texture coord attributes
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{

  class Renderer;

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                            Public Classes                                */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  class Mesh
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Internal Classes                              */
    /* (classes without specific semantic outside the context of the class)   */

    /** Container for polygon mesh attributes which can be varied per point
    \class Mesh::Vertex
    \brief
      - container for polygon mesh attributes which can be varied per point
    \details
      - points have cartesian positions, as well as placement ratio within a
      texture (uv coordinates: [0,1] in x & y, to be independent of resolution
      as source images may change, may have (RBG), color to interpolate across
      the surface, as well as averaged surface facing orientation (which can
      include surface normal direction, as well as tangent & bitangent surface
      directions which can form an orthonomal basis for any surface point.
    */
    class Vertex
    {
    public:

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                       Mesh::Vertex Consts                            */
      /*                  (class wide constant values)                        */

      //! (6) Number of vertex attribute vector definitions available
      static const u1 elems = 6u;

      //! (18) Sum of f4 vertex attribute fields / dimensions (v3f*6)
      static const u1 fields = 18u;

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                     Mesh::Vertex Constructors                        */
      /*              (struct of attributes varying per point)                */

      /** Create a data point within the mesh about which Edges and Tris form
      \brief
        - create a data point within the mesh about which Edges and Tris form
      \param coords
        - cartesian 3D <x, y, z> coordinates for the mesh point in model space
      \param tex_coords
        - UV [0,1] coordinates for 2D mesh point texture ratio position
      \param tex_coords
        - UV [0,1] coordinates for 2D mesh point texture ratio position
      */
      Vertex(const v3f& coords = nullv3f, const v2f& tex_coords = nullv2f,
        const v3f& normal = Z_HAT, const v3f& local_color = WHITE);


      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                     Mesh::Vertex Public Members                      */
      /*   (no particular reason to restrict access to any attribute, yet)    */

      //! Unit vector buffer for outward facing direction in cartesian coords
      v3f uvs;
      // <x,y> values hold <u,v> texture ratio coords, z holding uv set index

      //! Coordinate buffer holding a vertex <x,y,z> values in model space
      v3f pos;

      //! RGB [0,1] value buffer for unique coloring to display per vertex
      v3f color;

      //! Unit vector buffer for outward facing direction in cartesian coords
      v3f norm;

      //! Unit vector buffer for first surface axis direction cartesian coords
      v3f tan; // not in use: defines per vertex local basis with norm & bitan

      //! Unit vector buffer for second surface axis direction cartesian coords
      v3f bitan; // not in use: defines per vertex local basis with norm & tan

    }; // end Mesh::Vertex declaration


    /** Container for context regarding adjacent elements to each Vertex
    \class Mesh::Vertex
    \brief
      - container for context regarding adjacent Vertices to each Vertex
    \details
      - Each vertex also can hold information not for direct vizualization or 
      having buffers of data uploaded to the GPU for rendering, for example
      regarding adjacent surface details in order to succinctly edit portions as they change without
      having to update the entire mesh buffer, detecting relative locations of
      neighboring points via incoming edge connections, as well as vertex
      contribution to the surface (hull edges will be attached to the vertex
      but only have 1 face, and complex edges will have 3 faces in common, but
      internal edges will have exactly 2 faces using them).  Knowing proximal
      Vertex information can do much to automatically clean up a mesh as well,
      pruning vertices which don't have at least two edges or one tri using
      them which can't contribute to any geometric surface description.
    */
    class VertContext
    {

    public:

      //! Defined Edge buffer indices which are adjacent to (using), this point
      std::set<u4> wireframe;

      //! Defined Tri buffer indices which are adjacent to (using), this point
      std::set<u4> surface;

    }; // end VertContext class / struct declaration



    /** Container for vertex buffer associations for drawing a line segment
    \class Mesh::Edge
    \brief
      - container for vertex buffer associations for drawing a line segment
    \details
      - line segments are comprised of initial and terminal subscripts
    */
    class Edge
    {
    public:

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                        Mesh::Edge Consts                             */
      /*                  (class wide constant values)                        */

      //! Number of vertex subscript definitions to be referenced per edge
      static const u1 elems = 2u;

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                       Mesh::Edge Constructors                        */
      /*              (simple struct with 2 complicit values)                 */

      /** Create a line segment to be drawn on a mesh wireframe
      \brief
        - create a line segment to be drawn on a mesh wireframe
      \param initial
        - subscript in vertex buffer array to associate to index 0 of the edge
      \param jterminal
        - subscript in vertex buffer array to associate to index 1 of the edge
      */
      Edge(u4 initial = 0u, u4 terminal = 0u);

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                    Mesh::Edge Operator Overloads                     */
      /* (a struct of 2 subscript indices needs no math process: just access) */

      /** Vertex buffer subscript of the edge connection: start (0), end (1)
      \brief
        - vertex buffer subscript of the edge connection: start (0), end (1)
      \param s
        - index subscript to associated vertex position subscript in edge
        ie- vert position[edge[s][0]] and position[edge[s][1]] share an edge
      */
      size_t operator[](size_t s);

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                      Mesh::Edge Public Members                       */
      /* (subscript operator exists for convenience, not access restriction)  */

      //! (i)nitial vertex buffer subscript of the edge connection: start (0)
      u4 i;
      //! (t)erminal vertex buffer subscript of the edge connection: end (1)
      u4 t;

    }; // end Mesh::Edge declaration

    
    /** Vertex buffer subscript associations in drawing a triangle
    \class Mesh::Tri
    \brief
      - vertex buffer subscript associations in drawing a triangle
    \details
      - indices are presumed to be given in counter-clockwise ordering, 
      due to backface culling, clockwise ordered vertices may not display
      properly (or at all, depending upon rendering settings)
    */
    class Tri
    {
    public:

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                         Mesh::Tri Consts                             */
      /*                  (class wide constant values)                        */

      //! Number of vertex subscript definitions to be referenced per tri
      static const u1 elems = 3u;

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                        Mesh::Tri Constructors                        */
      /*              (simple struct with 3 complicit values)                 */

      /** Define a set of 3 points as constituting a counter-clockwise triangle
      \brief
        - define a set of 3 points as constituting a counter-clockwise triangle
      \details
        - indices should be in ccw order relative to the outward direction
      \param initial
        - subscript in vertex position buffer to associate to Tri's index[0]
      \param curl
        - subscript in vertex position buffer to associate to Tri's index[1]
      \param terminal
        - subscript in vertex position buffer to associate to Tri's index[2]
      */
      Tri(u4 initial = 0u, u4 curl = 0u, u4 terminal = 0u);

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                    Mesh::Tri Operator Overloads                      */
      /* (a struct of 2 subscript indices needs no math process: just access) */

      /** Get tri's vertex index subscripts in use: 0 (start), 1 (mid), 2 (end)
      \brief
        - get tri's vertex index subscripts in use: 0 (start), 1 (mid), 2 (end)
      \param s
        - index s to associated vertex position subscript in tri definition
      */
      size_t operator[](size_t s);

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                     Mesh::Tri Public Members                         */
      /* (subscript operator exists for convenience, not access restriction)  */

      //! (i)nitial vertex buffer subscript associated to the triangle
      u4 i;
      //! (c)url / medial vertex buffer subscript associated to the triangle
      u4 c;
      //! (t)erminal vertex buffer subscript associated to the triangle
      u4 t;

    }; // end Mesh::Tri declaration


    /** Vertex Object Renderer value container for recalling state when drawing
    \class Mesh::RenderBufferObjects
    \brief
      - vertex Object Renderer value container for recalling state when drawing
    \details
      - keeping the vertex object state records in the mesh allows the mesh to
      be the owner to manage it's own GPU state updates as any mesh changes
      come about.  Agnostic u4s will require casting for the renderer, but can
      be used by any renderer
    */
    class RenderBufferObjects
    {
    public:

      /*^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
      /*                Mesh::RenderBufferObjects Constructors                */
      /*   (setup for various renderer buffer state record recall values)     */

      /** Defaultable setup for mesh render accessors (default as not in use)
      \brief
        - defaultable setup for mesh render accessors (default as not in use)
      \param VAO
        - Vertex Array Object: GPU recall value for record of this mesh's state
      \param VBO
        - Buffer state recall value for using this mesh's Vertices for input
      \param EdgeEBO
        - Element Buffer state recall value for using this mesh's edge indices
      \param TriEBO
        - Element Buffer state recall value for using this mesh's tri indices
      */
      RenderBufferObjects(u4 VAO = u4_MAX, u4 VBO = u4_MAX, u4 EdgeEBO = u4_MAX,
        u4 TriEBO = u4_MAX);

      //! Vertex array object for Mesh GPU bound buffer array state for recall
      u4 record = u4_MAX;

      //! Vertex position buffer object for the mesh GPU buffer state
      u4 vertices = u4_MAX;

      //! EBO for normal visualization edge buffer for GPU state binding
      u4 pointFuzzEBO = u4_MAX;

      //! Edge element buffer object for the mesh GPU buffer state
      u4 wireframe = u4_MAX;

      //! Tri element buffer object for the mesh GPU buffer state
      u4 surface = u4_MAX;

      //! Pointer to Renderer which uploaded the mesh on GPU (will also unload)
      const Renderer* proxy = nullptr;

    };


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Create mesh from supplied std::vectors to set all buffers of mesh data
    \brief
      - create mesh from supplied std::vectors to set all buffers of mesh data
    \param vertexCoords
      - list of (model space), vertex coords, order relevant for edges & tris
    \param edges
      - list of edge connection vertex subscript pairs between which edges form
    \param tris
      - vertex coord index triples (in outward ccw winding) to draw as tris
    \param uvs
      - uv parameter coordinates in texture space for associated indices
    \param normals
      - optional list of surface normal vectors in parallel to coordinate
      indices.  Parallel arrays that is: non-parallel vectors (wouldn't make
      sense): if not provided, will be calculated procedurally automatically
    */
    Mesh(const std::vector<Vertex>& points, const std::vector<Edge>& edges,
      const std::vector<Tri>& tris);


    /** Default(able) constructor to make a basic unit quad mesh
    \brief
      - default(able) constructor to make a basic unit quad mesh
    \param ulu
      - texture horizontal (u|x|p) coord ratio for upper-left vertex in [0, 1]
    \param ulv
      - texture vertical (v|y|q) coord ratio for upper-left vertex in [0, 1]
    \param uru
      - texture horizontal (u|x|p) coord ratio for upper-right vertex in [0, 1]
    \param urv
      - texture vertical (v|y|q) coord ratio for upper-right vertex in [0, 1]
    \param llu
      - texture horizontal (u|x|p) coord ratio for lower-left vertex in [0, 1]
    \param llv
      - texture vertical (v|y|q) coord ratio for lower-left vertex in [0, 1]
    \param lru
      - texture horizontal (u|x|p) coord ratio for lower-right vertex in [0, 1]
    \param lrv
      - texture vertical (v|y|q) coord ratio for lower-right vertex in [0, 1]
    */
    Mesh(f4 ulu = 0.0f, f4 ulv = 1.0f, f4 uru = 1.0f, f4 urv = 1.0f,
         f4 llu = 0.0f, f4 llv = 0.0f, f4 lru = 1.0f, f4 lrv = 0.0f);


    /** Create mesh from file import (currently only supporting obj files)
    \brief
      - create mesh from file import (currently only supporting obj files)
    \param relativeFilePathName
      - relative (or absolute), ASCII path with file name for file to be opened
    \param invertNormals
      - treat vertex subscript indexing as cw instead of ccw ordering if true
    */
    Mesh(const std::string& relativeFilePathName, bool invertNormals = false);


    /** Create a new Mesh from another existing Mesh's data
    \brief
      - create a new Mesh from another existing Mesh's data
    \param source
      - mesh for which the contained component buffers will be sourced / copied
    */
    Mesh(const Mesh& source);


    /** Create a new Mesh from a Mesh value's returned data
    \brief
      - create a new Mesh from a Mesh value's returned data
    \param result
      - mesh value for which the contained component buffers will be hijacked
    */
    Mesh(Mesh&& result) noexcept;


    /** Unload from GPU by cleaning up all mesh buffer uploads prior to removal
    \brief
      - unload from GPU by cleaning up all mesh buffer uploads prior to removal
    */
    ~Mesh(void);


    /** Create a mesh to size using a uniform cube shape
    \brief
      - create a mesh to size using a uniform cube shape
    \param sideLength
      - model coordinate distance uniformly spanning space between adjacent verts
    \param hard
      - vertex normals will be set to face normals if true, averaged if false
    \param interior
      - if set to true, normals will be reversed to face inside of cube
    \return
      - resultant Mesh data representing a cube
    */
    static Mesh Cube(f4 sideLength = 1.0f, bool hard = true, bool interior = 0);


    /** Create a 2D regular polygon shaped mesh using evenly spaced subdivisions
    \brief
      - create a 2D regular polygon shaped mesh using evenly spaced subdivisions
    \param subdivs
      - [2,inf] arc subdivisions (equivalent to points) around the perimeter
    \param radius
      - object space vertex spacing scale of sphere points / positions
    \param offset
      - angle for initial point placement, from a null angle (cartesian <0,1>)
    \return
      - resultant Mesh data approximating a sphere
    */
    static Mesh Hull(u4 subdivs = 16u, f4 radius = 0.5f, ang offset = nullang);


    /** Create a mesh to size with a sphere shape using axial sweep subdivisions
    \brief
      - create a mesh to size with a sphere shape using axial sweep subdivisions
    \param subdivsRadial
      - number of arc subtensions divisions (polar perpendicular), around sphere
    \param subdivsHeight
      - number of dividing polygon edge rings along (parallel to) the polar axis
    \param radius
      - object space vertex spacing scale of sphere points / positions
    \return
      - resultant Mesh data approximating a sphere
    */
    static Mesh pSphere(u4 subdivsRadial = 16u, u4 subdivsHeight = 12u,
      f4 radius = 0.5f);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get reference to the object space center point of the mesh's dimensions
    \brief
      - get reference to the object space center point of the mesh's dimensions
    \return
      - reference to the Mesh's cartesian model space center point
    */
    const v3f& Center(void) const;


    /** Get reference to the object space span of the mesh's dimensions per axis
    \brief
      - get reference to the object space span of the mesh's dimensions per axis
    \return
      - reference to the Mesh's cartesian model space dimensional span
    */
    const v3f& Dimensions(void) const;


    /** Get the reference to the mesh's edge array buffer's initial subscript
    \brief
      - get the reference to the mesh's edge array buffer
    \return
      - reference to the mesh edge buffer; nulledge if unavailable
    */
    const Edge& EdgeBuffer(void) const;


    /** Get the total byte size of the mesh's edge list (2*edges*sizeof(u4))
    \brief
      - get the total byte size of the mesh's edge list (2*edges*sizeof(u4))
    \return
      - product of edges defined in buffer * size of u4 * elems (2), per edge
    */
    u4 EdgeBytes(void) const;


    /** Get the array size currently assigned to the mesh's edge buffer
    \brief
      - get the array size currently assigned to the mesh's edge buffer
    \return
      - number of edge definitions in the mesh's edge buffer
    */
    u4 EdgeCount(void) const;


    /** Get the element count of the mesh's edge list (2 elements * edge count)
    \brief
      - get the element count of the mesh's edge list (2 elements * edge count)
    \return
      - product of number of edges in buffer * elements per edge (2)
    */
    u4 EdgeIndices(void) const;


    /** Get state for whether the Mesh is currently loaded on the GPU to render
    \brief
      - get state for whether the Mesh is currently loaded on the GPU to render
    \return
      - true if the mesh is available to be rendered on the GPU
    */
    bool Loaded(void) const;


    /** Create a copy of this mesh with vertices altered to fit into a unit span
    \brief
      - create a copy of this mesh with vertices altered to fit into a unit span
    \param view
      - mesh may be reoriented to have verts aligned to use z as this direction
    \return
      - mesh with modified object space vertices fit to fill [-0.5, 0.5] span
    */
    Mesh NormalizedCopy(const v3f& view = nullv3f) const;


    /** Adjust the model space length to display the mesh normals in debug
    \brief
      - adjust the model space length to display the mesh normals in debug
    \param normalLength
      - the length to set normals displayed buffers to use in debug draw
    */
    void NormalLength(f4 normalLength);


    /** Update the edges of a mesh to a new set of vertex index associations
    \brief
      - update the edges of a mesh to a new set of vertex index associations
    \param edgeList
      - list of edge connection vertex-subscript pairs to draw edges between
    \param edgeCount
      - number of edge connection definitions in edgeList
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& SetEdges(const std::vector<Edge>& edgeConnections);


    /** Update the vertices of Mesh to new sets of coordinates, normals and uvs
    \brief
      - update the vertices of Mesh to new sets of coordinates, normals and uvs
    \param positions
      - list of vertex coordinates of relevant surface points to draw about
    \param normals
      - optional list of precalculated vertex surface orientation unit vectors
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& SetPositions(const std::vector<v3f>& coordList,
      const std::vector<v3f>* normalList = nullptr);


    /** Update the tris of a mesh to a new set of vertex-subscript associations
    \brief
      Update the tris of a mesh to a new set of vertex-subscript associations
    \details
      - subsets of vertex subscripts must be in counterclockwise winding
      to display, due to default backface culling settings being enabled
    \param triList
      - list of ccw tri vertex-subscript subsets to draw surfaces about
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& SetTris(const std::vector<Tri>& triList);


    /** Update the vertices of a mesh to a new set of texture coordinate data
    \brief
      - update the vertices of a mesh to a new set of texture coordinate data
    \details
      - textureCoords subscripts must write to matching, existing vertices
    \param textureCoords
      - list of uv [0,1] space coordinates in <x,y> members
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& SetUVs(const std::vector<v2f>& textureCoords);


    /** Update the vertices of Mesh to new sets of coordinates, normals and uvs
    \brief
      - update the vertices of Mesh to new sets of coordinates, normals and uvs
    \param points
      - list of vertex attributes for relevant surface points to draw around
    \param offset
      - index into the existing mesh Vertex subscripts to begin setting data
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& SetVertices(const std::vector<Vertex>& points, size_t offset = 0u);


    /** Get the reference to the mesh's triangle subscript array buffer
    \brief
      - get the reference to the mesh's triangle subscript array buffer
    \return
      - reference to the mesh triangle buffer; nulltri if unavailable
    */
    const Tri& TriBuffer(void) const;


    /** Get the total byte size of the mesh's tri array (3*triCount*sizeof(u4))
    \brief
      - get the total byte size of the mesh's tri array (3*triCount*sizeof(u4))
    \return
      - product of tris defined in buffer * size of u4 * elements (3), per tri
    */
    u4 TriBytes(void) const;


    /** Get the definition count currently assigned to the mesh's tri list
    \brief
      - get the definition count currently assigned to the mesh's tri list
    \return
      - number of tri definitions in the mesh's face / triangle buffer
    */
    u4 TriCount(void) const;


    /** Get the element count of the mesh's tri list (3 elements * tri count)
    \brief
      - get the element count of the mesh's tri list (3 elements * tri count)
    \return
      - product of number of tris in buffer * elements per tri (3)
    */
    u4 TriIndices(void) const;


    /** Project UVs to be mapped to a cube mapping in the given orientation
    \brief
      - project UVs to be mapped to a cube mapping in the given orientation
    \details
      - rotates axis into z comparably to other projections, then uses z coord
      of uv values to assign which face uvs belong within by float truncation
      to unsigned vlaues of using remaining axis coords in GL face standards:
      0(+x |  left  | <+y,-z>), 1(-x | right | <+y,+z>), 2(+y | top | <-z,+x>),
      3(-y | bottom | <+z,+x>), 4(+z | front | <+y,+x>), 5(-z | back | <+y,-x>)
    \param axis
      - axis vector to be used as model space front face of projection
    \param vertSelection
      - if provided, will limit affected Uvs to only include verts from the set
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& UVCube(const v3f& axis = Z_HAT,
      const std::set<u4>* vertSelection = nullptr);


    /** Project selected UVs to be mapped to the equivalent cylindrical coords
    \brief
      - project selected UVs to be mapped to the equivalent cylindrical coords
    \details
      - rotates a normalized mesh copy as axis arg, aligned into the z axis to
      create an arbitrary cylindrical alignment axis for all points to map upon
    \param axis
      - axis vector of cylindrical rotation for which angles project about
    \param vertSelection
      - if provided, will limit affected Uvs to only include verts from the set
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& UVCylindrical(const v3f& axis = Z_HAT,
      const std::set<u4>* vertSelection = nullptr);


    /** Project all selected UVs to be mapped to the equivalent UV plane coords
    \brief
      - project all selected UVs to be mapped to the equivalent UV plane coords
    \details
      - rotates a normalized mesh copy as axis arg, aligned into the z axis to
      create an arbitrary xy projection plane for all points to map upon
    \param axis
      - normal vector acting as the axis to the desired plane of projection
    \param vertSelection
      - if provided, will limit affected Uvs to only include verts from the set
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& UVPlanar(const v3f& axis = Z_HAT,
      const std::set<u4>* vertSelection = nullptr);


    /** Project selected UVs to be mapped to the equivalent cylindrical coords
    \brief
      - project selected UVs to be mapped to the equivalent cylindrical coords
    \details
      - rotates a normalized mesh copy as axis arg, aligned into the z axis to
      create an arbitrary cylindrical alignment axis for all points to map upon
    \param axis
      - axis vector of cylindrical rotation for which angles project about
    \param vertSelection
      - if provided, will limit affected Uvs to only include verts from the set
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& UVSpherical(const v3f& axis = Z_HAT,
      const std::set<u4>* vertSelection = nullptr);


    /** Get the reference to the mesh's vertex positions buffer
    \brief
      - get the reference to the mesh's vertex positions buffer
    \return
      - reference to the mesh vertex buffer
    */
    const Vertex& VertexBuffer(void) const;


    /** Get total bytes in Vertex buffer ( sizeof(Vertex) * VertexCount() )
    \brief
      - get the element count of the mesh's tri list (3 elements * tri count)
    \return
      - product of bytes taken by a Mesh::Vertex and the total VertexCount()
    */
    u4 VertexBytes(void) const;


    /** Get the definition count currently assigned to the mesh's vertex list
    \brief
      - get the definition count currently assigned to the mesh's vertex list
    \return
      - v3f count in the mesh's Vertex attribute buffer (pos, uvs, norms, etc)
    */
    u4 VertexCount(void) const;


    /** Get total 4-byte fields in Vertex list ( (6*v3f) * verts)
    \brief
      - get the element count of the mesh's tri list (3 elements * tri count)
    \details
      - should be 1/4 of VertexBytes(), ie ( sum-list-size / sizeof(f4) )
    \return
      - product of f4 element fields in a Vertex and the total VertexCount()
    */
    u4 VertexIndices(void) const;


    /** Get the reference to the mesh's vertex normal display's edge buffer
    \brief
      - get the reference to the mesh's vertex normal display's edge buffer
    \return
      - reference to the mesh vertex normal display's edge list buffer
    */
    const Edge& VNormEBuffer(void) const;


    /** Get total bytes in vertex normal view edge buffer (2*EdgeBytes())
    \brief
      - get total bytes in vertex normal view edge buffer (2*EdgeBytes())
    \return
      - product of bytes taken by a Mesh::Vertex and the total VNormVCount()
    */
    u4 VNormEBytes(void) const;


    /** Get the buffer list v3f count for edges comprising normal display
    \brief
      - get the buffer list v3f count for edges comprising normal display
    \return
      - v3f count in vertex normal display's edge buffer
    */
    u4 VNormECount(void) const;


    /** Get the vertex normal edge buffer count times the two elements per edge
    \brief
      - get the vertex normal edge buffer count times the two elements per edge
    \return
      - product of edge elements (2) & vertex normal display's edge count
    */
    u4 VNormEIndices(void) const;


    /** Get the reference to the mesh's vertex normal display's vertex buffer
    \brief
      - get the reference to the mesh's vertex normal display's vertex buffer
    \return
      - reference to the mesh vertex normal display's vertex list buffer
    */
    const Vertex& VNormVBuffer(void) const;


    /** Get total bytes in vertex normal view vertex buffer (2*VertexBytes())
    \brief
      - get total bytes in vertex normal view vertex buffer (2*VertexBytes())
    \return
      - product of bytes taken by a Mesh::Vertex and the total VNormVCount()
    */
    u4 VNormVBytes(void) const;


    /** Get the buffer list v3f count for vertices comprising normal display
    \brief
      - get the buffer list v3f count for vertices comprising normal display
    \return
      - v3f count in normal vertex buffer (should be twice the vertex count)
    */
    u4 VNormVCount(void) const;


    /** Get the cumulative 4-byte fields in vertex normal vertex buffer list
    \brief
      - get the cumulative 4-byte fields in vertex normal vertex buffer list
    \return
      - product of f4 element fields in a vertex normals verts & VertexCount()
    */
    u4 VNormVIndices(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set Mesh to use component buffer data from another source Mesh's data
    \brief
      - set Mesh to use component buffer data from another source Mesh's data
    \param source
      - set Mesh to use component buffer data from another source Mesh's data
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& operator=(const Mesh& source);


    /** Set Mesh to use component buffers from another returned Mesh directly
    \brief
      - set Mesh to use component buffers from another returned Mesh directly
    \param result
      - Mesh return value whose buffer data will be repurposed for this Mesh
    \return
      - reference to the modified Mesh for easier subsequent inline action
    */
    Mesh& operator=(Mesh&& result) noexcept;


    /** Output text string of all Mesh buffer contents
    \brief
      - output text string of all Mesh buffer contents
    \return
      - string of all vertex buffer data, line separated per buffer and entry
    */
    operator std::string(void) const;

    RenderBufferObjects state;  //! State records of mesh GPU buffer accessors
    // may be sensible to refactor to a list, with VAO/VBO/EBO per renderer

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Renderer Accessors                              */
    /* (storing GPU buffer return states on the relevant mesh being rendered) */

    //! Various VAO, VBO & EBO members exist only for Renderer access per Mesh
    friend class Renderer; // that should include the base class interface...
    friend class RendererGL4; // but derived classes don't inherit friend
    friend class Window;
    // TODO: list all other Renderers as implemented to access / load / unload

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set the mesh surface normals to averaged, normalized orientations
    \brief
      - set the mesh surface normals to averaged, normalized orientations
    \details
      - cs350 project requirements stipulate that vertex normals should be set
      to face normals, or creating a simplified universal hard shading group
      for edges and tris, but this doesn't need extra parameters as this is
      accomplished through redundant vertex listings and tris created using
      those broken apart surface segments.  This goes the extra measure and
      averages all unique adjacent tri normals, each vertex being broken apart
      means that there will only be one tri normal to average per Vertex.
    */
    void RecalculateNormals(void);

    /** Scan mesh vertex data for bounds & center point to be updated
    \brief
      - scan mesh vertex data for bounds & center point to be updated
    */
    void RecalculateDimens(void);

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Private Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    std::vector<Vertex> point;  //! Model space attributes per mesh point

    std::vector<Edge> edge;     //! Position subscripts forming wireframe

    std::vector<Edge> vNormal;  //! vOrth subscripts forming normal view

    std::vector<Tri> tri;       //! CCW position subscripts forming surface

    std::vector<VertContext> near;  //! Adjacenty context per mesh point
    // must be kept parallel to point array: using same size and indices

    v3f bounds;            //! Dimensional vector of mesh size in x, y and z

    v3f centerPoint;       //! Object's local origin (will stay the origin)

    u4 vertC; //! Vertex count defined in point array buffer (point.size() / 2)
    // Vertex normal offset points are in the same buffer & don't count

    f4 nLength = 0.01f; //! Length being used to draw normals in model space

  }; // end Mesh class declaration 


  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                            Helper Constants                              */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  //! Empty mesh definition
  static const Mesh nullmesh;

  //! Initial value attached to itself; used as an invalid edge state 
  static const Mesh::Edge nulledge(0, 0);

  //! Initial value circling itself; used as an invalid tri state 
  static const Mesh::Tri nulltri(0, 0, 0);


  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                            Helper Functions                              */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Operator Overloads                          */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Compare the contents of two meshes for equality in their components
  \brief
    - compare the contents of two meshes for equality in their components
  \param lhs
    - left-hand Mesh being wieghed in comparison against rhs (right-hand side)
  \param rhs
    - right-hand Mesh being wieghed in comparison against lhs (left-hand side)
  \return
    - true if all buffer components in each Vertex / element  list are the same
  */
  bool operator==(const Mesh& lhs, const Mesh& rhs);


  /** Compare the contents of two meshes for inequality in their components
  \brief
    - compare the contents of two meshes for inequality in their components
  \param lhs
    - left-hand Mesh being wieghed in comparison against rhs (right-hand side)
  \param rhs
    - right-hand Mesh being wieghed in comparison against lhs (left-hand side)
  \return
    - true if any buffer components in any Vertex / element  list are inequal
  */
  bool operator!=(const Mesh& lhs, const Mesh& rhs);

} // end dp namespace
