/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Mesh.cpp
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

// "./src/..."
#include "ASCIIValue.h" // string case conversion
#include "File.h"       // File read operations
#include "Log.h"        // Error / message logging management
#include "Mesh.h"       // Class declaration header file
#include "Renderer.h"   // Base interface for rendering to clear mesh from GPU


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                        Internal Class: Mesh::Vertex                        */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

dp::Mesh::Vertex::Vertex(const v3f& coords, const v2f& tex_coords,
  const v3f& normal, const v3f& local_color)
  : pos(coords), uvs(tex_coords.XYZ(0.0f)), color(local_color), tan(nullv3f),
  bitan(nullv3f)
{
  if (normal != nullv3f) { norm = normal.UnitVec(); }
  else { norm = Z_HAT; }

} // end Vertex(const v3f&, const v2f&, const v3f&, const v3f&)

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Internal Class: Mesh::Edge                         */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create a line segment to be drawn on a mesh wireframe                      */
dp::Mesh::Edge::Edge(u4 initial, u4 terminal) : i(initial), t(terminal)
{ } // end Mesh::Edge::Edge(u4, u4)


// Vertex buffer subscript of the edge connection: start (0), end (1)         */
size_t dp::Mesh::Edge::operator[](size_t s)
{ s &= 1; return (s & 1) ? i : t; } // end Mesh::Edge::operator[](u4)

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Internal Class: Mesh::Tri                          */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Constructor for defining a set of 3 points as constituting a triangle      */
dp::Mesh::Tri::Tri(u4 initial, u4 curl, u4 terminal) : i(initial), c(curl),
  t(terminal)
{ } // end Mesh::Tri::Tri(u4, u4, u4)


// Get the given tri's vertex subscripts in use: 0 (start), 1 (mid), 2 (end)  */
size_t dp::Mesh::Tri::operator[](size_t s)
{
  if (s > 2) { s = (s & 1) ? 1 : (s & 2) ? 2 : 0; }
  return (s & 1) ? c : (s == 0) ? i : t;

} // end Mesh::Tri::operator[](u4)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                 Internal Class: Mesh::RenderBufferObjects                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Defaultable setup for mesh render accessors (default as not in use)        */
dp::Mesh::RenderBufferObjects::RenderBufferObjects(u4 VAO, u4 VBO,
  u4 EdgeEBO, u4 TriEBO)
  : record(VAO), vertices(VBO), wireframe(EdgeEBO), surface(TriEBO)
{ } // end Mesh::RenderBufferObjects(u4, u4, u4, u4)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Constructors                                 */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Create mesh from supplied std::vectors to set all buffers of mesh data     */
dp::Mesh::Mesh(const std::vector<Vertex>& points,
  const std::vector<Edge>& edges, const std::vector<Tri>& tris)
  : bounds(nullv3f), centerPoint(nullv3f), vertC(0)
{
  SetVertices(points);
  SetEdges(edges);
  SetTris(tris);
  if (points.size() > 0 && points[points.size()-1].norm == nullv3f)
  {
    RecalculateNormals();
  }
  else
  {
    NormalLength(nLength);
  }

} // end Mesh(vector<v4f>|<Edge>|<Tri>|<v4f>&, vector<v4f>*)


// Default(able) constructor to make a basic unit quad mesh                   */
dp::Mesh::Mesh(f4 ulu, f4 ulv, f4 uru, f4 urv, f4 llu, f4 llv, f4 lru,
  f4 lrv)
  : bounds(nullv3f), centerPoint(nullv3f), vertC(0)
{
  // Sanitize inputs
  v2f UL(ModF(ulu,0.0f,1.0f), ModF(ulv,0.0f,1.0f));
  v2f UR(ModF(uru,0.0f,1.0f), ModF(urv,0.0f,1.0f));
  v2f LL(ModF(llu,0.0f,1.0f), ModF(llv,0.0f,1.0f));
  v2f LR(ModF(lru,0.0f,1.0f), ModF(lrv,0.0f,1.0f));
  v3f upperLeft( -0.5f, 0.5f, 0.0f );
  v3f upperRight( 0.5f, 0.5f, 0.0f );
  v3f lowerLeft( -0.5f, -0.5f, 0.0f );
  v3f lowerRight( 0.5f, -0.5f, 0.0f );

  // Set buffer data up
  std::vector<Vertex> squareP;
  squareP.push_back(Vertex( upperLeft, UL,-Z_HAT, WHITE));  // 0
  squareP.push_back(Vertex(upperRight, UR,-Z_HAT, WHITE));  // 1
  squareP.push_back(Vertex( lowerLeft, LL,-Z_HAT, WHITE));  // 2
  squareP.push_back(Vertex(lowerRight, LR,-Z_HAT, WHITE));  // 3
  // 0   1
  // 2   3
  std::vector<Edge> edges = { Edge(0, 1), Edge(0, 2), Edge(1, 3), Edge(2, 3) };
  std::vector<Tri> tris = { Tri(0, 2, 1), Tri(1, 2, 3) };

  // Load buffers with data
  SetVertices(squareP).SetEdges(edges).SetTris(tris);
  NormalLength(nLength);

} // end Mesh(f4, f4, f4, f4, f4, f4, f4, f4, f4) default / unit / square Mesh


// Create mesh from file import (currently only supporting obj files)         */
dp::Mesh::Mesh(const std::string& relativeFilePathName, bool invertNormals)
  : bounds(nullv3f), centerPoint(nullv3f), vertC(0)
{
  ASCIIPath file(relativeFilePathName);
  if (ToLower(file.Extension()) != "obj")
  {
    Log::Warn("Mesh(string&) - only obj files supported: import aborted");
    return;
  }
  std::vector<v3f> pos;
  std::vector<v3f> col;
  std::vector<v3f> norm;
  std::vector<v2f> uv;
  std::vector<Edge> edges;
  std::vector<Tri> tris;
  std::string sanitized = file;
  std::string contents = File::ReadAtOnce(sanitized);
  size_t current = 0;
  size_t fileEnd = contents.length();

  while (current < fileEnd)
  {
    std::string line = StringLine(contents, current);
    std::vector<std::string> tok = TokenList(line);
    size_t tCount = tok.size();
    if (tCount > 0)
    {
      if (ToLower(tok[0]) == "v")
      {
        if (tCount > 3)
        {
          std::string xStr = FloatSubstr(tok[1]);
          std::string yStr = FloatSubstr(tok[2]);
          std::string zStr = FloatSubstr(tok[3]);
          if (xStr == "" || yStr == "" || zStr == "")
          {
            // FloadSubstr would have returned 0.0, not null
            Log::Info("Mesh(string) - malformed line :" + xStr + ", " + yStr
              + ", " + zStr);
            continue; // signify & abort for now: may be still useful data...?
          }
          else
          {
            f4 xVal = std::stof(tok[1]); // FloatSubStr result not accounting
            f4 yVal = std::stof(tok[2]); // for trailing ###e-### fine input
            f4 zVal = std::stof(tok[3]); // TODO: fix > use sanitized input
            pos.push_back(v3f(xVal, yVal, zVal));
            if (tCount > 4)
            {
              if (tCount > 6) // colors provided? non-standard, but why not
              {
                std::string rStr = FloatSubstr(tok[4]); // red val string read
                std::string gStr = FloatSubstr(tok[5]);// green val string read
                // yeah, yeah: haha...
                std::string bStr = FloatSubstr(tok[6]); // blue val string read
                if (rStr == "" || gStr == "" || bStr == "")
                {
                  // FloadSubstr would have returned 0.0, not null
                  Log::Info("Mesh(string) - malformed line :" + rStr + ", "
                    + gStr  + ", " + bStr +
                    " color couldn't be read, only coords used");
                  continue; // signify & abort line: pos is enough...
                }
                else
                {
                  f4 rVal = std::stof(tok[4]);
                  f4 gVal = std::stof(tok[5]);
                  f4 bVal = std::stof(tok[6]);
                  col.push_back(v3f(rVal, gVal, bVal));
                  if (tCount > 7)
                  {
                    Log::Info("Mesh(string) - no methods to interpret ", 0);
                    Log::I("more than 6 attributes in a line", true);
                  }
                }
              }
              else
              {
                Log::Info("Mesh(string) - extra attributes non-standard", 0);
                Log::I(", unable to parse " + std::to_string(tCount - 1)
                  + " numeric tokens: ambiguous how to be read", true);
              }
            } // end line has more then position attribute check
          } // end line had valid position values check
        } // end line had tokens sufficient to read 3 dimensions validation
      } // end 'v'ertex attribute line read
      else if (ToLower(tok[0]) == "f")
      {
        if (tok.size() > 3)
        {

          u4 i = 0; // initial tri (fan), token (->vert), index (won't change)
          std::string iStr = UnsignedSubstr(tok[1]);
          if (IsUnsignedD(iStr))
          {
            i = ToUnsigned(iStr) - 1;
            if (i == u4_MAX)
            {
              Log::Warn("Mesh(string) - face indexing used 0 values");
            }
          }
          else
          {
            Log::Info("Mesh(string) - face indexing not recognized");
            continue;
          }
          if (tok.size() < 6) // if 'f' + 3 tris to be defined from 5 indices
          {
            std::string cStr = UnsignedSubstr(tok[2]);
            std::string tStr = UnsignedSubstr(tok[3]);
            // Add extra edge(s) for tri / quad listing
            if (IsUnsignedD(tStr) && IsUnsignedD(cStr))
            {
              edges.push_back(Edge(i, (ToUnsigned(cStr) - 1)));
              if (tok.size() == 5) // quad terminal edge
              {
                tStr = UnsignedSubstr(tok[4]);
                if (IsUnsignedD(tStr))
                {
                  edges.push_back(Edge(ToUnsigned(tStr) - 1, i));
                }
              }
              else // tri terminal edge
              {
                edges.push_back(Edge(i, (ToUnsigned(cStr) - 1)));
              }
            }
            else
            {
              Log::Info("Mesh(string) - edge indices not recognized at "
                + std::to_string(3) + ", (or " + std::to_string(2) + ")");
              continue;
            }
          }
          for (size_t j = 3; j < tok.size(); ++j)
          {
            std::string cStr = UnsignedSubstr(tok[j-1]);
            std::string tStr = UnsignedSubstr(tok[j]);
            if (IsUnsignedD(tStr) && IsUnsignedD(cStr))
            {
              u4 c = 0; // curl vertex subscript to be used for this face tri
              u4 t = 0; // terminal vertex subscript to be used in this face tri

              // Osscilate cw & ccw vertex ordering each iteration
              if (j & 1) { c = ToUnsigned(cStr) - 1; t = ToUnsigned(tStr) - 1; }
              else { c = ToUnsigned(tStr) - 1; t = ToUnsigned(cStr) - 1; }
              if (invertNormals) { tris.push_back(Tri(i, t, c)); }
              else { tris.push_back(Tri(i, c, t)); }
              edges.push_back(Edge(c, t));
            }
            else
            {
              Log::Info("Mesh(string) - line indices not recognized at "
                + std::to_string(j) + ", (or " + std::to_string(j - 1) + ")");
              continue;
            }
          }
          if (tok.size() > 5) // add terminal fan edge
          {
            std::string cStr = UnsignedSubstr(tok[2]);
            std::string tStr = UnsignedSubstr(tok[tok.size() - 1]);
            u4 b = ToUnsigned(tStr) - 1;
            u4 e = ToUnsigned(cStr) - 1;
            edges.push_back(Edge(b, e));
          }
        } // end at least 2 numeric tokens check
      } // end 'f'ace subscript line read
    } // end tokens exist check
  } // end file contents line read iteration

  // Create vertex list from read attributes
  std::vector<Vertex> points;
  // Unify parallel attribute list sizes
  if (pos.size() > 0)
  {
    // Create generic attributes if none were provided (pos being requisite)
    if (uv.size() == 0) { uv.resize(pos.size()); } // 0,0 UVs are standard
    if (norm.size() == 0)
    {
      norm.resize(pos.size());
      for (size_t i = 0; i < norm.size(); ++i) { norm[i] = Z_HAT; }
    }
    if (col.size() == 0)
    {
      col.resize(pos.size());
      for (size_t i = 0; i < col.size(); ++i) { col[i] = WHITE; }
    }
    um max = MaxUM(MaxUM(pos.size(),norm.size()), MaxUM(uv.size(),col.size()));
    if (pos.size() < max) { pos.resize(max); } // Origin is standard
    if (uv.size() < max) { uv.resize(max); } // 0,0 UVs are standard
    if (norm.size() < max)
    {
      norm.resize(max);
      for (size_t i = 0; i < norm.size(); ++i) { norm[i] = Z_HAT; }
    }
    if (col.size() < max)
    {
      col.resize(max);
      for (size_t i = 0; i < col.size(); ++i) { col[i] = WHITE; }
    }
  }
  // (buffers should all have uniform size now, but take the least for safety)
  um size = MinUM(MinUM(pos.size(), norm.size()), MinUM(col.size(), uv.size()));
  for (u4 i = 0; i < size; ++i) // Create Vertices from attributes
  {
    points.push_back(Vertex(pos[i], uv[i], norm[i], col[i]));
  } // end Vertex list population from read in, parallel attributes

  // Set read in buffers to mesh data
  SetVertices(points);
  SetEdges(edges);
  SetTris(tris);
  RecalculateNormals(); // Use tri definitions to set normals to face

} // end Mesh(const std::string&)


// Create a new Mesh from another existing Mesh's data                        */
dp::Mesh::Mesh(const Mesh& source)
{
  point       = (source.point);
  edge        = (source.edge);
  tri         = (source.tri);
  near        = (source.near);
  vNormal     = (source.vNormal);
  bounds      = (source.bounds);
  centerPoint = (source.centerPoint);
  vertC       = (source.vertC);
  // ignore render buffer 'state' on copy: new mesh not valid on GPU

} // end Mesh(const Mesh& source)


// Create a new Mesh from a Mesh value's returned data                        */
dp::Mesh::Mesh(Mesh&& result) noexcept
{
  point       = std::move(result.point);
  edge        = std::move(result.edge);
  tri         = std::move(result.tri);
  near        = std::move(result.near);
  vNormal     = std::move(result.vNormal);
  bounds      = std::move(result.bounds);
  centerPoint = std::move(result.centerPoint);
  vertC       = std::move(result.vertC);
  // ignore render buffer 'state' on copy: new mesh not valid on GPU

} // end Mesh(Mesh&&) noexcept


// Unload from GPU by cleaning up all mesh buffer uploads prior to removal    */
dp::Mesh::~Mesh(void)
{ if (state.proxy) { state.proxy->UnloadMesh(*this); } } // end ~Mesh(void)

dp::Mesh dp::Mesh::Cube(f4 sideLength, bool hard, bool interior)
{
  f4 d = sideLength * 0.5f;
  std::vector<v3f> vertPos =
  { // separated faces in OpenGL cube ordering: +x, -x, +y, -y, +z, -z
    // (upper / lower / left / right semantic assuming an exterior object view)
    { d, d, d}, { d, d,-d}, // right face   - [00](upper-left) [01](upper-right)
    { d,-d, d}, { d,-d,-d}, // right face   - [02](lower-left) [03](lower-right)

    {-d, d,-d}, {-d, d, d}, // left face    - [04](upper-left) [05](upper-right)
    {-d,-d,-d}, {-d,-d, d}, // left face    - [06](lower-left) [07](lower-right)

    {-d, d,-d}, { d, d,-d}, // top face     - [08](upper-left) [09](upper-right)
    {-d, d, d}, { d, d, d}, // top face     - [10](lower-left) [11](lower-right)

    {-d,-d, d}, { d,-d, d}, // bottom face  - [12](upper-left) [13](upper-right)
    {-d,-d,-d}, { d,-d,-d}, // bottom face  - [14](lower-left) [15](lower-right)

    { d, d, d}, {-d, d, d}, // back face    - [16](upper-left) [17](upper-right)
    { d,-d, d}, {-d,-d, d}, // back face    - [18](lower-left) [19](lower-right)

    {-d, d,-d}, { d, d,-d}, // front face   - [20](upper-left) [21](upper-right)
    {-d,-d,-d}, { d,-d,-d}  // front face   - [22](lower-left) [23](lower-right)
  };
  std::vector<v3f> texturePos =
  { // separated faces in OpenGL cube ordering: +x, -x, +y, -y, +z, -z
    { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, // right  - [00](ul) [01](ur)
    { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, // right  - [02](ll) [03](lr)

    { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, // left   - [04](ul) [05](ur)
    { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, // left   - [06](ll) [07](lr)

    { 0.0f, 1.0f, 2.0f }, { 1.0f, 1.0f, 2.0f }, // top    - [08](ul) [09](ur)
    { 0.0f, 0.0f, 2.0f }, { 1.0f, 0.0f, 2.0f }, // top    - [10](ll) [11](lr)

    { 0.0f, 1.0f, 3.0f }, { 1.0f, 1.0f, 3.0f }, // bottom - [12](ul) [13](ur)
    { 0.0f, 0.0f, 3.0f }, { 1.0f, 0.0f, 3.0f }, // bottom - [14](ll) [15](lr)

    { 0.0f, 1.0f, 4.0f }, { 1.0f, 1.0f, 4.0f }, // back   - [16](ul) [17](ur)
    { 0.0f, 0.0f, 4.0f }, { 1.0f, 0.0f, 4.0f }, // back   - [18](ll) [19](lr)

    { 0.0f, 1.0f, 5.0f }, { 1.0f, 1.0f, 5.0f }, // front  - [20](ul) [21](ur)
    { 0.0f, 0.0f, 5.0f }, { 1.0f, 0.0f, 5.0f }  // front  - [22](ll) [23](lr)
  };
  std::vector<v3f> vertNorm;
  if (hard)
  {
    if (interior)
    {
      vertNorm = {
      {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }, // right  - [00](ul)[01](ur)
      {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }, // right  - [02](ll)[03](lr)

      { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, // left   - [04](ul)[05](ur)
      { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, // left   - [06](ll)[07](lr)

      { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f }, // top    - [08](ul)[09](ur)
      { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f }, // top    - [10](ll)[11](lr)

      { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, // bottom - [12](ul)[13](ur)
      { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, // bottom - [14](ll)[15](lr)

      { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, // back   - [16](ul)[17](ur)
      { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, // back   - [18](ll)[19](lr)

      { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, // front  - [20](ul)[21](ur)
      { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }  // front  - [22](ll)[23](lr)
      };
    } // end hard interior normal branch
    else
    {
      vertNorm = {
      { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, // right  - [00](ul)[01](ur)
      { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, // right  - [02](ll)[03](lr)

      {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }, // left   - [04](ul)[05](ur)
      {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }, // left   - [06](ll)[07](lr)

      { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, // top    - [08](ul)[09](ur)
      { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, // top    - [10](ll)[11](lr)

      { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f }, // bottom - [12](ul)[13](ur)
      { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f }, // bottom - [14](ll)[15](lr)

      { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, // back   - [16](ul)[17](ur)
      { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, // back   - [18](ll)[19](lr)

      { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, // front  - [20](ul)[21](ur)
      { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }  // front  - [22](ll)[23](lr)
      };
    } // end hard exterior normal branch
  }
  else
  {
    constexpr f4 CR3 = _ROOT3; // 1 / sqrt(3) 
    if (interior)
    {
      vertNorm = {
      {-CR3,-CR3,-CR3 }, {-CR3,-CR3, CR3 }, // right  - [00](ul)[01](ur)
      {-CR3, CR3,-CR3 }, {-CR3, CR3, CR3 }, // right  - [02](ll)[03](lr)

      { CR3,-CR3, CR3 }, { CR3,-CR3,-CR3 }, // left   - [04](ul)[05](ur)
      { CR3, CR3, CR3 }, { CR3, CR3,-CR3 }, // left   - [06](ll)[07](lr)

      { CR3,-CR3, CR3 }, {-CR3,-CR3, CR3 }, // top    - [08](ul)[09](ur)
      { CR3,-CR3,-CR3 }, {-CR3,-CR3,-CR3 }, // top    - [10](ll)[11](lr)

      { CR3, CR3,-CR3 }, {-CR3, CR3,-CR3 }, // bottom - [12](ul)[13](ur)
      { CR3, CR3, CR3 }, {-CR3, CR3, CR3 }, // bottom - [14](ll)[15](lr)

      {-CR3,-CR3,-CR3 }, { CR3,-CR3,-CR3 }, // back   - [16](ul)[17](ur)
      {-CR3, CR3,-CR3 }, { CR3, CR3,-CR3 }, // back   - [18](ll)[19](lr)

      { CR3,-CR3, CR3 }, {-CR3,-CR3, CR3 }, // front  - [20](ul)[21](ur)
      { CR3, CR3, CR3 }, {-CR3, CR3, CR3 }  // front  - [22](ll)[23](lr)
      };
    } // end soft interior normal branch
    else
    {
      vertNorm = {
      { CR3, CR3, CR3 }, { CR3, CR3,-CR3 }, // right  - [00](ul)[01](ur)
      { CR3,-CR3, CR3 }, { CR3,-CR3,-CR3 }, // right  - [02](ll)[03](lr)

      {-CR3, CR3,-CR3 }, {-CR3, CR3, CR3 }, // left   - [04](ul)[05](ur)
      {-CR3,-CR3,-CR3 }, {-CR3,-CR3, CR3 }, // left   - [06](ll)[07](lr)

      {-CR3, CR3,-CR3 }, { CR3, CR3,-CR3 }, // top    - [08](ul)[09](ur)
      {-CR3, CR3, CR3 }, { CR3, CR3, CR3 }, // top    - [10](ll)[11](lr)

      {-CR3,-CR3, CR3 }, { CR3,-CR3, CR3 }, // bottom - [12](ul)[13](ur)
      {-CR3,-CR3,-CR3 }, { CR3,-CR3,-CR3 }, // bottom - [14](ll)[15](lr)

      { CR3, CR3, CR3 }, {-CR3, CR3, CR3 }, // back   - [16](ul)[17](ur)
      { CR3,-CR3, CR3 }, {-CR3,-CR3, CR3 }, // back   - [18](ll)[19](lr)

      {-CR3, CR3,-CR3 }, { CR3, CR3,-CR3 }, // front  - [20](ul)[21](ur)
      {-CR3,-CR3,-CR3 }, { CR3,-CR3,-CR3 }  // front  - [22](ll)[23](lr)
      };
    } // end soft exterior normal branch
  }
  std::vector<Mesh::Vertex> pointList;
  pointList.resize(vertPos.size());
  for (u4 i = 0; i < vertPos.size(); ++i)
  {
    pointList[i].pos = vertPos[i];
    pointList[i].norm = vertNorm[i];
    v3f col(fabsf(vertNorm[i].r), fabsf(vertNorm[i].g), fabsf(vertNorm[i].b));
    pointList[i].color = WHITE;
    pointList[i].uvs = texturePos[i];
  }
  std::vector<Mesh::Edge> edgeList = {
    {  0,  1 }, {  0,  2 }, {  1,  3 }, {  2,  3 }, // right face borders
    {  4,  5 }, {  4,  6 }, {  5,  7 }, {  6,  7 }, // left face borders
    {  8,  9 }, {  8, 10 }, {  9, 11 }, { 10, 11 }, // top face borders
    { 12, 13 }, { 12, 14 }, { 13, 15 }, { 14, 15 }, // bottom face borders
    { 16, 17 }, { 16, 18 }, { 17, 19 }, { 18, 19 }, // back face borders
    { 20, 21 }, { 20, 22 }, { 21, 23 }, { 22, 23 }  // front face borders
  };
  std::vector<Mesh::Tri> faceList;
  if (interior)
  {
    faceList =
    {
      {  0,  2,  1 }, {  1,  2,  3 }, // ccw inward right face  - {ul} / {lr]
      {  4,  6,  5 }, {  5,  6,  7 }, // ccw inward left face   - {ul} / {lr]
      {  8, 10,  9 }, {  9, 10, 11 }, // ccw inward top face    - {ul} / {lr]
      { 12, 14, 13 }, { 13, 14, 15 }, // ccw inward bottom face - {ul} / {lr]
      { 16, 17, 18 }, { 17, 19, 18 }, // ccw inward back face   - {ul} / {lr]
      { 20, 21, 22 }, { 21, 23, 22 }  // ccw inward front face  - {ul} / {lr]
    };
  }
  else
  {
    faceList =
    {
      {  0,  1,  2 }, {  1,  3,  2 }, // ccw outward right face   - {ul} / {lr]
      {  4,  5,  6 }, {  5,  7,  6 }, // ccw outward left face    - {ul} / {lr]
      {  8,  9, 10 }, {  9, 11, 10 }, // ccw outward top face     - {ul} / {lr]
      { 12, 13, 14 }, { 13, 15, 14 }, // ccw outward bottom face  - {ul} / {lr]
      { 16, 18, 17 }, { 17, 18, 19 }, // ccw outward back face    - {ul} / {lr]
      { 20, 22, 21 }, { 21, 22, 23 }  // ccw outward front face   - {ul} / {lr]
    };
  }
  Mesh cube(pointList, edgeList, faceList);
  return cube;
} // end static Mesh Mesh::Cube(f4)

dp::Mesh dp::Mesh::Hull(u4 subdivs, f4 radius, ang offset)
{
  std::vector<Mesh::Vertex> pointList;
  std::vector<Mesh::Edge> edgeList;
  std::vector<Mesh::Tri> faceList;
  Mesh hull(pointList, edgeList, faceList);
  return hull;

} // end Mesh Mesh::Hull(u4, f4, ang)

// Create a mesh to size with a sphere shape using axial sweep subdivisions   */
dp::Mesh dp::Mesh::pSphere(u4 subdivsRadial, u4 subdivsHeight, f4 radius)
{
  // Validate inputs to meet volumetric minimums
  size_t a = (subdivsRadial < 3) ? 3 : subdivsRadial; // Axial subdivs used
  size_t a1 = a + 1; // a+1: used frequently due to 1 partition above subdivs
  size_t h = (subdivsHeight < 2) ? 2 : subdivsHeight; // Height subdivs used
  f4 r = (radius == 0.0f) ? _INf : (radius < 0.0f) ? // Model radius scale
    static_cast<f4>(abs(radius)) : radius;

  // Calculate mesh buffer size totals & contextual data
  size_t vc = (a * h) + a + h - 1;        // Sphere vertex total
  size_t ec = (2 * a * h + h + a - 2);    // Sphere edge total
  size_t fc = (2 * a * (h - 1));          // Sphere face total
  f4 us = 1.0f / static_cast<f4>(a);  // U-span in horizontal texture space
  f4 uo = us / 2.0f;                  // Sawtooth horizontal texture offset
  f4 vs = 1.0f / static_cast<f4>(h);  // V-span in vertical texture space
  size_t tpv = vc - a;          // initial subscript of top pole sawtooth verts
  size_t tcv = vc - 2 * a - 1;  // initial subscript of top end cap ring verts
  size_t tce = ec - (2 * a);    // initial subscript of top end cap ring edges
  size_t tre = ec - (3 * a);    // initial subscript of top row ring edges
  size_t tcf = fc - a;          // initial subscript of top end cap ring faces
  f4 z = r * static_cast<f4>(sin((PI * (((h / 2.0f) - 1) / h)))); // z position
  // magnitude for radius ratio for the given edge ring's vert positions
  f4 m = r * static_cast<f4>(cos((PI * (((h / 2.0f) - 1) / h))));

  // Create mesh data buffers
  std::vector<v3f> vertPos(vc);
  std::vector<v2f> texturePos(vc);
  std::vector<v3f> vertNorm(vc);
  std::vector<Mesh::Edge> edgeList(ec);
  std::vector<Mesh::Tri> faceList(fc);

  // Create tables of radial values about major axis
  f4* sinPos = new f4[a1]; // Table of precomputed sin values at radial subdivs
  f4* cosPos = new f4[a1]; // Table of precomputed cos values at radial subdivs
  if (!sinPos || !cosPos)
  {
    if (sinPos) { delete[] sinPos; }
    if (cosPos) { delete[] cosPos; }
    return nullmesh;
  } // end subdiv tables allocation validation

  // Populate radial subdivision tables for x & y coordinates
  for (size_t i = 0; i < a1; ++i)
  {
    f4 angle = ((static_cast<f4>(i) / static_cast<f4>(a))) * RAD_PER_REV;
    cosPos[i] = static_cast<f4>(cos(angle));
    sinPos[i] = static_cast<f4>(sin(angle));
  }

  // Populate mesh buffers' using trig tables: ordering verts bottom to top
  for (size_t i = 0; i < a; ++i) // set up end caps ([0,a]: ie for one rotation)
  {
    f4 x = static_cast<f4>(i); // float expression of current row iteration
    // Relative distance...of what?
    f4 relD = r * static_cast<f4>(cos(((PI * (h / 2.0f - x)) / h)));
    f4 rowX = (uo + (x * us)); // Texture space U-position for the column
    if (a1 + i < vc) // (C6385|86 warning supressions: always true)
    {
      // Bottom pole elements (sawtooth UVs at pole => multiple verts)
      texturePos[i].Set(rowX, 0.0f);        // uvs offset along bottom
      vertPos[i].Set(0.0f, 0.0f, -r);       // polar verts in uniform position
      vertNorm[i].Set(-Z_HAT);   // low polar normals all go down
      edgeList[2 * i] = Mesh::Edge((u4)(a + i), (u4)i); // Saw -> pole edge
      edgeList[2 * i + 1] = Mesh::Edge((u4)i, (u4)(a1 + i));// Pole -> saw edge
      faceList[i] = Mesh::Tri((u4)(a + i), (u4)i, (u4)(a1 + i));// polar tris

      // Top cap elements (sawtooth UVs at pole => multiple verts)
      texturePos[tpv + i].Set(rowX, 1.0f);  // offset along top
      vertPos[tpv + i].Set(0.0f, 0.0f, r);  // polar uniform position
      vertNorm[tpv + i].Set(Z_HAT);         // high polars go up
      u4 c = (u4)i;
      if (c > a) { c -= (u4)a; }
      texturePos[tcv + i].Set(us * i, (1.0f - vs));           // near texture top
      vertPos[tcv + i].Set(m * sinPos[c], m * cosPos[c], z);  // top ring
      vertNorm[tcv + i].Set(vertPos[tcv + i].UnitVec());  // facing radial angle
      edgeList[tce + 2 * i] = Mesh::Edge((u4)(tcv + i), (u4)(tpv + i));
      edgeList[tce + 2 * i + 1] = Mesh::Edge((u4)(tpv + i), (u4)(tcv + 1 + i));
      edgeList[tre + i] = Mesh::Edge((u4)(tcv + i), (u4)(tcv + 1 + i));
      faceList[tcf + i] =
        Mesh::Tri((u4)(tcv + i), (u4)(tcv + 1 + i), (u4)(tpv + i));
    }
  }
  texturePos[(tpv - 1)].Set(1.0f, 1.0f - vs);
  vertPos[(tpv - 1)].Set(m * sinPos[0], m * cosPos[0], z);
  vertNorm[tpv - 1].Set(vertPos[tpv - 1].UnitVec());

  for (size_t row = 0; row < h - 2; ++row) // set medial height subdiv components
  {
    u4 icv = (u4)(a + (row * a1));            // Initial column vert subscript
    u4 ire = (u4)(2 * a + (row * (2 * a1 - 1)));// Initial row edge subscript
    u4 ice = (u4)(ire + a);                   // Initial column edge subscript
    u4 icf = (u4)(a + (row * 2 * a));         // Initial column tri subscript
    f4 rh = static_cast<f4>(row + 1) / static_cast<f4>(h);  // Row height %
    // Distance along z from bottom / partial diameter
    z = r * static_cast<f4>(sin((PI * (((h / 2.0f) - row - 1) / h))));
    // Current row's magnitude of radial verts scale in x & y
    m = r * static_cast<f4>(cos((PI * (((h / 2.0f) - row - 1) / h))));
    if (icv < vc) // (C6385|86 warning supressions: always true)
    {
      // Set first vertex since subdiv+1 section demarcations exist per row
      // set left-most vert and edge connecting to above, adjacent orbital ring
      vertPos[icv].Set(m * sinPos[0], m * cosPos[0], -z);       // ->[.]
      texturePos[icv].Set(0.0f, vs * (row + 1));            // ->[.]
      vertNorm[icv] = vertPos[icv].UnitVec();               // ->[.]
      edgeList[ice] = Mesh::Edge((u4)icv, (u4)(icv + a1)); // ->[!]
      // Then the rest have symmetrical element counts
      for (size_t c = 1; c <= a; ++c)
      {
        u4 cre = (u4)(ire + c);   // Current row div edge segment subscript
        u4 cce = (u4)(ice + c);   // Current column div edge subscript in row
        u4 ccf = (u4)(icf + 2 * c - 1); // Current column div face subs in row
        u4 ccv = (u4)(icv + c);   // Current column vertex subscript in row
        u4 ncv = (u4)(ccv + a1);  // Next column's aligned vertex subscript
        vertPos[ccv].Set(m * sinPos[c], m * cosPos[c], -z);       // -> |  [.]
        texturePos[ccv].Set(us * c, vs * (row + 1));          // -> |  [.]
        vertNorm[ccv] = vertPos[ccv].UnitVec();               // -> |  [.]
        edgeList[cre - 1] = Mesh::Edge((u4)(ccv - 1), (u4)(ccv)); // -> |[_].
        edgeList[cce] = Mesh::Edge((u4)(ccv), (u4)(ncv));     // -> |__[|]
        faceList[ccf - 1] = Mesh::Tri((ccv - 1), (ccv), (ncv));   // -> |_/#|
        faceList[ccf] = Mesh::Tri((ccv - 1), (ncv), (ncv - 1));   // -> |#/_|
      }
    }
  }
  std::vector<Mesh::Vertex> pointList;
  pointList.resize(vertPos.size());
  for (u4 i = 0; i < vertPos.size(); ++i)
  {
    pointList[i].pos = vertPos[i];
    pointList[i].norm = vertNorm[i];
    v3f col(fabsf(vertNorm[i].r), fabsf(vertNorm[i].g), fabsf(vertNorm[i].b));
    pointList[i].color = WHITE;
    pointList[i].uvs = texturePos[i].XYZ(0.0f);
  }
  Mesh sphere(pointList, edgeList, faceList);
  delete[] sinPos;
  delete[] cosPos;
  return sphere;

} // end static Mesh Mesh::pSphere(u4, u4, f4)


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                               Public Methods                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Get reference to the object space center point of the mesh's dimensions    */
const dp::v3f& dp::Mesh::Center(void) const
{ return centerPoint; } // end const v4f& Mesh::Center(void) const


// Get reference to the object space span of the mesh's dimensions per axis   */
const dp::v3f& dp::Mesh::Dimensions(void) const
{ return bounds; } // end const v4f& Mesh::Dimensions(void) const


// Get the reference to the mesh's edge array buffer                          */
const dp::Mesh::Edge& dp::Mesh::EdgeBuffer(void) const
{ return edge[0]; } // end const Edge& Mesh::EdgeBuffer(void) const


// Get the total byte size of the mesh's edge list (2*edges*sizeof(u4))       */
u4 dp::Mesh::EdgeBytes(void) const
{
  return static_cast<u4>(edge.size() * sizeof(Edge));
} // end u4 Mesh::EdgeBytes(void) const


// Get the array size currently assigned to the mesh's edge buffer            */
u4 dp::Mesh::EdgeCount(void) const
{ return static_cast<u4>(edge.size()); } // end u4 Mesh::EdgeCount(void) const


// Get the element count of the mesh's edge list (2 elements * edge count)    */
u4 dp::Mesh::EdgeIndices(void) const
{
  return static_cast<u4>(Edge::elems * edge.size());
} // end u4 Mesh::EdgeIndices(void) const


// Get state for whether the Mesh is currently loaded on the GPU to render    */
bool dp::Mesh::Loaded(void) const
{ return (state.proxy != nullptr); } // end bool Mesh::Loaded(void) const


// Create a copy of this mesh with vertices altered to fit into a unit span   */
dp::Mesh dp::Mesh::NormalizedCopy(const v3f& view) const
{
  Mesh copy(*this);
  // Normalize points to center on origin 
  if (copy.centerPoint != nullv3f)
  {
    for (u4 i = 0; i < vertC; ++i) { copy.point[i].pos -= centerPoint; }
    copy.centerPoint = nullv3f;
  }
  // Normalize dimensional span (preserve aspect ratio)
  if (copy.bounds != nullv3f)
  {
    v3f unscale((NearF(bounds.x, 0.0f, _INf)) ? 1.0f : 1.0f / bounds.x,
                 (NearF(bounds.y, 0.0f, _INf)) ? 1.0f : 1.0f / bounds.y,
                 (NearF(bounds.z, 0.0f, _INf)) ? 1.0f : 1.0f / bounds.z);
    f4 scalar = MinF(MinF(AbsF(unscale.x), AbsF(unscale.y)), AbsF(unscale.z));
    if (scalar != 1.0f)
    {
      m4f UnitScale = Scale3Affine(scalar, scalar, scalar);
      for (u4 i = 0; i < vertC; ++i)
      {
        copy.point[i].pos = (UnitScale * point[i].pos.XYZW(If)).XYZ();
      }
      copy.bounds *= scalar;
      copy.nLength *= scalar;
    }

    // Rotate points into solicited orientation (if any) 
    if (view != nullv3f) // Parallel axis check
    {
      v4f look = -view.UnitVec().XYZW(Of);
      m4f rotBack;
      if (look == UNIT_VEC_Z)
      {
        // No-op: Z-axis has no rotation to perform
        copy.RecalculateDimens();
        copy.NormalLength(nLength);
      }
      else if (look == -UNIT_VEC_Z) // Anti-parallel axis check
      {
        rotBack = RotateRodriguesAffine(ang(180.0f, arc::degrees), X_HAT);
        for (u4 i = 0; i < vertC; ++i)
        {
          copy.point[i].pos = (rotBack * copy.point[i].pos.XYZW(If)).XYZ();
        }
        copy.RecalculateDimens();
        copy.RecalculateNormals();
      }
      else // Arbitrary axis rotation
      {
        v3f axis = look.XYZ().Cross(Z_HAT);
        ang angle = ang(acos(look.DotH(UNIT_VEC_Z)), arc::rad);
        rotBack = RotateRodriguesAffine(angle, axis);
        if (rotBack * look == -UNIT_VEC_Z)
        {
          rotBack = RotateRodriguesAffine(-angle, axis);
        }
        v4f testVec = rotBack * look;
        if (testVec != UNIT_VEC_Z)
        {
          v3f out = testVec.XYZ();
          Log::Warn("Mesh::NormalizedCopy - rotation yielded " + out.String());
        }
        else
        {
          for (u4 i = 0; i < vertC; ++i)
          {
            copy.point[i].pos = (rotBack * copy.point[i].pos.XYZW(If)).XYZ();
          }
          copy.RecalculateDimens();
          copy.RecalculateNormals();
        }
      }
    }
    else
    {
      copy.RecalculateDimens();
      copy.NormalLength(nLength);
    }
  }
  else
  {
    Log::Note("Nope");
  }
  return copy;

} // end Mesh Mesh::Normalized(const v3f&) const

// Adjust the model space length to display the mesh normals in debug         */
void dp::Mesh::NormalLength(f4 normalLength)
{
  size_t vertT = point.size(); // total verts: positions and offsets
  nLength = normalLength;
  // Populate new vertex definitions as needed
  if (2*static_cast<size_t>(vertC) < vertT) 
  {
    // Grow list to hold vertex offsets as needed
    point.resize(2*static_cast<size_t>(vertC));
    vertT = point.size();
    vNormal.clear(); // mark normal edge list for new vertex definitions
    // only necessary if points change relationship per #'s, not position
  }
  // Set offset vertices attributes to correct uniform offset length
  for (size_t n = vertC, b = 0; n < vertT; ++n, ++b)
  {
    point[n].uvs    = point[b].uvs;
    point[n].pos    = point[b].pos + (nLength * point[b].norm);
    point[n].color  = HALF_TONE; // Could be preferable as a setting...?
    point[n].norm   = point[b].norm;
    point[n].tan    = -point[b].norm;
    point[n].bitan  = -point[b].norm;
  }
  // Form edge connection visualization with the original & offset coord vertices
  if (vNormal.size() < vertC)
  {
    vNormal.resize(vertC);
    for (size_t i = 0; i < vertC; ++i)
    {
      vNormal[i] = Edge((u4)i, (u4)(i + vertC));
    }
  }
  // TODO: same for face normals with their own calculations

} // void Mesh::NormalLength(f4)


// Update the edges of a mesh to a new set of vertex index associations       */
dp::Mesh& dp::Mesh::SetEdges(const std::vector<Edge>& edgeConnections)
{
  um size = MaxUM(edge.size(), edgeConnections.size());
  for (um i = 0; i < near.size(); ++i) { near[i].wireframe.clear(); }
  if (size > edge.size())
  {
    edge.resize(size);
  }
  for (size_t cur = 0; cur < size; ++cur)
  {
    // Set the new edge
    edge[cur] = edgeConnections[cur];
    // For each vertex referenced in this edge, associate it to the vertex
    for (size_t subs = 0; subs < Edge::elems; ++subs)
    {
      if (edge[cur][subs] < (u4)near.size())
      {
        near[edge[cur][subs]].wireframe.insert(static_cast<u4>(cur));
      }
      else
      {
        Log::Warn("Mesh::SetEdges - edge index [" + std::to_string(cur) + ", "
          + std::to_string(subs) + "] out of current Vertex range "
          + std::to_string(vertC));
      }
    }
  }
  return *this;
} // end Mesh& Mesh::SetEdges(const std::vector<Edge>&)


// Update the vertices of Mesh to new sets of coordinates, normals and uvs    */
dp::Mesh& dp::Mesh::SetPositions(const std::vector<v3f>& coordList,
  const std::vector<v3f>* normalList)
{
  // Shorter potential value from the lists given: should be parallel
  um size = MinUM(coordList.size(), normalList->size());
  v3f min;
  v3f max;
  bool resized = false;

  if (size >= vertC)
  {
    // TODO: refactor to avoid platform dependent memory bitdepth detection
#ifdef _WIN64
    point.resize(2ul * static_cast<u8>(size));
#else
    point.resize(2u * size);
#endif
      near.resize(size);
      vertC = static_cast<u4>(size);
      resized = true;
  }

  // Update position data
  for (u4 i = 0; i < size; ++i) // first new data (may be all data
  {
    // new data
    point[i].pos = coordList[i];
    if (normalList) { point[i].norm = (*normalList)[i].UnitVec(); }
  }
  RecalculateDimens();

  if (resized || !normalList) { RecalculateNormals(); }

  return *this;

} // end Mesh& Mesh::SetPositions(const vector<v4f>&, const vector<v4f>*)


// Update the tris of a mesh to a new set of vertex-subscript associations    */
dp::Mesh& dp::Mesh::SetTris(const std::vector<Tri>& triList)
{
  um size = MaxUM(tri.size(), triList.size());
  for (um i = 0; i < near.size(); ++i) { near[i].surface.clear(); }
  if (size > tri.size())
  {
    tri.resize(size);
  }
  for (u4 cur = 0; cur < size; ++cur)
  {
    // Set the new tri
    tri[cur] = triList[cur];
    // For each vertex referenced by this tri, associate it to that vertex
    for (size_t subs = 0; subs < Tri::elems; ++subs)
    {
      if (tri[cur][subs] < vertC) // This Tri index is a defined Vertex
      {
        // Add this Tri to that Vertex
        near[tri[cur][subs]].surface.insert(cur);
      }
      else
      {
        Log::Warn("Mesh::SetTris - tri index [" + std::to_string(cur) + ", "
          + std::to_string(subs) + "] out of current Vertex range "
          + std::to_string(vertC));
      }
    }
  }
  return *this;

} // end Mesh& Mesh::SetTris(const std::vector<Tri>&)


// Update the vertices of a mesh to a new set of texture coordinate data      */
dp::Mesh& dp::Mesh::SetUVs(const std::vector<v2f>& textureCoords)
{
  // Can only write new UVs to existing vertices: get that size
  um size = MinUM(vertC, textureCoords.size());
  bool resized = false;
  if (size > vertC)
  {
    // TODO: refactor to avoid platform dependent memory bitdepth detection
#ifdef _WIN64
    point.resize(2ul * static_cast<u8>(size));
#else
    point.resize(2u * size);
#endif
    near.resize(size);
    resized = true;
    for (u4 i = vertC; i < size; ++i) { point[i].norm = Z_HAT; }
    vertC = static_cast<u4>(size);
  }
  for (um i = 0; i < size; ++i)
  {
    point[i].uvs = textureCoords[i].XYZ(0.0f);
  }
  if (resized) { RecalculateNormals(); }
  return *this;

} // end Mesh& Mesh::SetUVs(const std::vector<v4f>&)


// Update the vertices of Mesh to new sets of coordinates, normals and uvs    */
dp::Mesh& dp::Mesh::SetVertices(const std::vector<Vertex>& points,
  size_t offset)
{
  bool normsValid = true;
  // Grow point list as needed to scale for new data
  if (points.size() > vertC)
  {
    point.resize(2*points.size());
    near.resize(points.size());
    vNormal.clear();
    for (u4 i = vertC; i < points.size(); ++i) { point[i].norm = Z_HAT; }
    vertC = static_cast<u4>(points.size());
  }
  // And cull now extraneous data (per 'set' call; update could be different)
  if (points.size() < vertC)
  {
    point.resize(2*points.size());
    near.resize(points.size());
    vNormal.clear();
    vertC = static_cast<u4>(points.size());
  }
  // Set any listed points to use the new, provided data
  for (u4 i = 0; i < vertC; ++i)
  {
    // Easier to use full attribute set; preserve partial data later?
    point[i] = points[i];
    if (!NearF(normsValid && point[i].norm.LengthSquared(), 1.0f, _INf))
    {
      normsValid = false;
    }
  }
  if (!normsValid) { RecalculateNormals(); }
  else { NormalLength(nLength); }
  // Correct bounding / center info for new vertex context
  RecalculateDimens();

  return *this;

} // end Mesh& Mesh::SetVertices(vector<v4f>&, vector<v4f>&, vector<v4f>*)


// Get the reference to the mesh's triangle subscript array buffer            */
const dp::Mesh::Tri& dp::Mesh::TriBuffer(void) const
{ return tri[0]; } // end const Tri& Mesh::TriBuffer(void) const


// Get the total byte size of the mesh's tri array (3*triCount*sizeof(u4))    */
u4 dp::Mesh::TriBytes(void) const
{
  return static_cast<u4>(tri.size() * sizeof(Tri));
} // end u4 Mesh::TriBytes(void) const


// Get the array size currently assigned to the mesh's tri list buffer        */
u4 dp::Mesh::TriCount(void) const
{ return static_cast<u4>(tri.size()); } // end u4 Mesh::TriCount(void) const


// Get the element count of the mesh's tri list (3 elements * tri count)      */
u4 dp::Mesh::TriIndices(void) const
{
  return static_cast<u4>(Tri::elems * tri.size());
} // end u4 Mesh::TriIndices(void) const


// Project selected UVs to be mapped to the equivalent cylindrical coords     */
dp::Mesh& dp::Mesh::UVCube(const v3f& axis,
  const std::set<u4>* vertSelection)
{
  // Create mesh normalized to aspect preserved unit scale, rotated to z axis
  Mesh copy = NormalizedCopy(axis);
  v3f offset(0.5f, 0.5f, 0.0f);

  // Shift mesh copy's points from [-0.5, 0.5] to [0, 1] range for the given set
  if (vertSelection) for (u4 i : *vertSelection)
  {
    // Only setting appropriate vertices if selectivity is desired
    if (i < vertC)
    {
      // which face the vert belongs within is a factor of the largest element
      v3f current(copy.point[i].pos);
      v3f mag(abs(current.x), abs(current.y), abs(current.z));
      if (mag.z > mag.x && mag.z > mag.y) // z predominates: front / back faces
      {
        if (copy.point[i].pos.z > 0.0f) // face[4]: front; +z, using <+y, +x>
        {
          this->point[i].uvs = v3f(current.y, current.x, 4.0f) + offset;
        }
        else // face[5]: back; -z, using <+y, -x>
        {
          this->point[i].uvs = v3f(current.y, -current.x, 5.0f) + offset;
        }
      }
      if (mag.y > mag.z && mag.y > mag.x) // y predominates: top / bottom faces
      {
        if (copy.point[i].pos.y > 0.0f) // face[2]: top; +y, using <-z, +x>
        {
          this->point[i].uvs = v3f(-current.z, current.x, 2.0f) + offset;
        }
        else // face[3]: bottom; -y, using <+z, +x>
        {
          this->point[i].uvs = v3f(current.z, current.x, 3.0f) + offset;
        }
      }
      else // x component predominates or is equal: left / right faces
      {
        if (copy.point[i].pos.x > 0.0f) // face[0]: left; +x, using <+y, -z>
        {
          this->point[i].uvs = v3f(current.y, -current.z, 0.0f) + offset;
        }
        else // face[1]: right; -x, using <+y, +z>
        {
          this->point[i].uvs = v3f(current.y, current.z, 1.0f) + offset;
        }
      }
    }
    else
    {
      Log::Warn("Mesh::UVCube - vertex " + std::to_string(i)
        + "not in mesh " + std::to_string(vertC) + " range");
    }
  }
  else for (u4 i = 0; i < vertC; ++i)
  {
    // Or unilaterally to all vertices if no selectivity is solicited
    // which face the vert belongs within is a factor of the largest element
    v3f current = copy.point[i].pos;
    v3f mag(abs(current.x), abs(current.y), abs(current.z));
    if (mag.z > mag.x && mag.z > mag.y) // z predominates: front / back faces
    {
      if (copy.point[i].pos.z > 0.0f) // face[4]: front; +z, using <+y, +x>
      {
        this->point[i].uvs = v3f(current.y, current.x, 4.0f) + offset;
      }
      else // face[5]: back; -z, using <+y, -x>
      {
        this->point[i].uvs = v3f(current.y, -current.x, 5.0f) + offset;
      }
    }
    if (mag.y > mag.z && mag.y > mag.x) // y predominates: top / bottom faces
    {
      if (copy.point[i].pos.y > 0.0f) // face[2]: top; +y, using <-z, +x>
      {
        this->point[i].uvs = v3f(-current.z, current.x, 2.0f) + offset;
      }
      else // face[3]: bottom; -y, using <+z, +x>
      {
        this->point[i].uvs = v3f(current.z, current.x, 3.0f) + offset;
      }
    }
    else // x component predominates or is equal: left / right faces
    {
      if (copy.point[i].pos.x > 0.0f) // face[0]: left; +x, using <+y, -z>
      {
        this->point[i].uvs = v3f(current.y, -current.z, 0.0f) + offset;
      }
      else // face[1]: right; -x, using <+y, +z>
      {
        this->point[i].uvs = v3f(current.y, current.z, 1.0f) + offset;
      }
    }
  }
  return *this;

} // end Mesh& :Mesh::UVCube(const v3f&, const std::set<u4>*)


// Project selected UVs to be mapped to the equivalent cylindrical coords     */
dp::Mesh& dp::Mesh::UVCylindrical(const v3f& axis,
  const std::set<u4>* vertSelection)
{
  // Create mesh normalized to aspect preserved unit scale, rotated to z axis
  Mesh copy = NormalizedCopy(axis);
  f4 is = 1.0f / bounds.z; // inverse span of z range
  f4 min = -bounds.z * 0.5f; // lowest z value (normalized => -z-span / 2)

  // Shift normalized & rotated mesh points from [-0.5, 0.5] to [0, 1] range
  if (vertSelection) for (u4 i : *vertSelection)
  {
    // Only setting appropriate vertices if selectivity is desired
    if (i < vertC)
    {
      // u coord is just a rev ratio, v coord is ratio out of the whole z span
      ang t = ang(atan2f(copy.point[i].pos.y, copy.point[i].pos.x), arc::rad);
      // since copy is normalized from [-0.5, 0.5], z span is 1; +0.5 is ratio
      this->point[i].uvs = v3f(t.s(), (copy.point[i].pos.z - min) * is, 0.0f);
    }
    else
    {
      Log::Warn("Mesh::UVCylindrical - vertex " + std::to_string(i)
        + "not in mesh " + std::to_string(vertC) + " range");
    }
  }
  else for (u4 i = 0; i < vertC; ++i)
  {
    // Or unilaterally to all vertices if no selectivity is solicited
    // u coord is just a rev ratio, v coord is ratio out of the whole z span
    ang t = ang(atan2f(copy.point[i].pos.y, copy.point[i].pos.x), arc::rad);
    // since copy is normalized from [-0.5, 0.5], z span is 1; +0.5 is ratio
    this->point[i].uvs = v3f(t.s(), (copy.point[i].pos.z - min) * is, 0.0f);
  }
  return *this;

} // end Mesh& :Mesh::UVCylindrical(const v3f&, const std::set<u4>*)


dp::Mesh& dp::Mesh::UVPlanar(const v3f& axis,
  const std::set<u4>* vertSelection)
{
  // Create mesh normalized to aspect preserved unit scale, rotated to z axis
  Mesh copy = NormalizedCopy(axis);

  // Shift normalized & rotated mesh points from [-0.5, 0.5] to [0, 1] range
  if (vertSelection) for (u4 i : *vertSelection)
  {
    // Only setting appropriate vertices if selectivity is desired
    if (i < vertC)
    {
      v3f cUVs = (copy.point[i].pos.XY() + v2f(0.5f, 0.5f)).XYZ(0.0f);
      cUVs.y = 1.0f - cUVs.y; // invert per rotation
      this->point[i].uvs = cUVs;
    }
    else
    {
      Log::Warn("Mesh::UVPlanar - vertex " + std::to_string(i) + "not in mesh "
       + std::to_string(vertC) + " range");
    }
  }
  else for (u4 i = 0; i < vertC; ++i)
  {
    // Or unilaterally to all vertices if no selectivity is solicited
    this->point[i].uvs = (copy.point[i].pos.XY() + v2f(0.5f, 0.5f)).XYZ(0.0f);
  }
  return *this;

} // end Mesh& Mesh::UVPlanar(const v3f&, const std::set<u4>*)


// Project selected UVs to be mapped to the equivalent cylindrical coords     */
dp::Mesh& dp::Mesh::UVSpherical(const v3f& axis,
  const std::set<u4>* vertSelection)
{
  // Create mesh normalized to aspect preserved unit scale, rotated to z axis
  Mesh copy = NormalizedCopy(axis);

  // Calculate spherical coords per point of adjusted mesh
  if (vertSelection) for (u4 i : *vertSelection)
  {
    // Only setting appropriate vertices if selectivity is desired
    if (i < vertC)
    {
      const v3f& cP = copy.point[i].pos; // current point (of normalized copy)
      this->point[i].uvs = v3f(atan2f(cP.y, cP.x) * REV_PER_RAD,
        (PI - acos(cP.z / cP.Length()) * 2.0f * REV_PER_RAD), 0.0f);
    }
    else
    {
      Log::Warn("Mesh::UVSpherical - vertex " + std::to_string(i)
        + "not in mesh " + std::to_string(vertC) + " range");
    }
  }
  else for (u4 i = 0; i < vertC; ++i)
  {
    // Or unilaterally to all vertices if no selectivity is solicited
    const v3f& cP = copy.point[i].pos; // current point (of normalized copy)
    this->point[i].uvs = v3f(atan2f(cP.y, cP.x) * REV_PER_RAD,
      (PI - acos(cP.z / cP.Length()) * 2.0f * REV_PER_RAD), 0.0f);
  }
  return *this;

} // end Mesh& :Mesh::UVSpherical(const v3f&, const std::set<u4>*)


// Get the reference to the mesh's vertex positions buffer                    */
const dp::Mesh::Vertex& dp::Mesh::VertexBuffer(void) const
{ return point[0]; } // end const v4f& Mesh::VertexBuffer(void) const


// Get total bytes in Vertex buffer ( sizeof(Vertex) * VertexCount() )        */
u4 dp::Mesh::VertexBytes(void) const
{
  return static_cast<u4>(vertC * sizeof(Vertex));
} // end u4 Mesh::VertexBytes(void) const


// Get the array size currently assigned to the mesh's vertex buffers         */
u4 dp::Mesh::VertexCount(void) const
{
  return vertC;
} // end u4 Mesh::VertexCount(void) const


// Get total 4-byte fields in Vertex list ( (6*v3f) * verts)                  */
u4 dp::Mesh::VertexIndices(void) const
{
  return static_cast<u4>(vertC * Vertex::fields);
} // end u4 Mesh::VertexIndices(void) const


// Get the reference to the mesh's vertex normal display's edge buffer        */
const dp::Mesh::Edge& dp::Mesh::VNormEBuffer(void) const
{
  return vNormal[0];
} // end const Mesh::Edge& Mesh::VNormEBuffer(void) const


// Get total bytes in vertex normal view edge buffer (2*EdgeBytes())          */
u4 dp::Mesh::VNormEBytes(void) const
{
  return static_cast<u4>(vNormal.size() * sizeof(Edge));
} // end u4 Mesh::VNormEBytes(void) const


// Get the buffer list v3f count for edges comprising normal display         */
u4 dp::Mesh::VNormECount(void) const
{
  return static_cast<u4>(vNormal.size());
} // end u4 Mesh::VNormECount(void) const


// Get the vertex normal edge buffer count times the two elements per edge    */
u4 dp::Mesh::VNormEIndices(void) const
{
  return static_cast<u4>(vNormal.size() * Edge::elems);
} // end Mesh::u4 VNormEIndices(void) const


// Get the reference to the mesh's vertex normal display's vertex buffer      */
const dp::Mesh::Vertex& dp::Mesh::VNormVBuffer(void) const
{
  return point[0];
} // end const Mesh::Edge& Mesh::VNormVBuffer(void) const


// Get total bytes in vertex normal view vertex buffer (2*VertexBytes())      */
u4 dp::Mesh::VNormVBytes(void) const
{
  return static_cast<u4>(point.size() * sizeof(Vertex));
} // end u4 Mesh::VNormVBytes(void) const


// Get the buffer list v3f count for vertices comprising normal display      */
u4 dp::Mesh::VNormVCount(void) const
{
  return static_cast<u4>(point.size());
} // end u4 Mesh::VNormVCount(void) const


// Get the cumulative 4-byte fields in vertex normal vertex buffer list       */
u4 dp::Mesh::VNormVIndices(void) const
{
  return static_cast<u4>(point.size() * Vertex::fields);
} // end Mesh::u4 VNormVIndices(void) const



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Operator Overloads                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set Mesh to use component buffer data from another source Mesh's data      */
dp::Mesh& dp::Mesh::operator=(const Mesh& source)
{
  point       = source.point;
  edge        = source.edge;
  tri         = source.tri;
  near        = source.near;
  vNormal     = source.vNormal;
  bounds      = source.bounds;
  centerPoint = source.centerPoint;
  return *this;

} // end  Mesh& operator=(const Mesh&)


// Set Mesh to use component buffer data from another source Mesh's data      */
dp::Mesh& dp::Mesh::operator=(Mesh&& result) noexcept
{
  point       = std::move(result.point);
  edge        = std::move(result.edge);
  tri         = std::move(result.tri);
  near        = std::move(result.near);
  vNormal     = std::move(result.vNormal);
  bounds      = std::move(result.bounds);
  centerPoint = std::move(result.centerPoint);
  return *this;

} // end  Mesh& operator=(Mesh&&) noexcept



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Private Functions / Methods                        */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// Set the mesh surface normals to averaged, normalized orientations          */
void dp::Mesh::RecalculateNormals(void)
{
  // For each vertex
  for (u4 i = 0; i < vertC; ++i)
  {
    std::vector<v3f> nearNorms;
    v3f sum(nullv3f);
    // Get each adjacent face's normal direction
    v3f norm;
    if (near[i].surface.size() > 0)
    {
      for (u4 j : near[i].surface)
      {
        // Sum all the face normal directions to get the average
        v3f PQ = point[tri[j][1]].pos - point[tri[j][0]].pos;
        v3f PR = point[tri[j][2]].pos - point[tri[j][0]].pos;
        norm = PR.Cross(PQ);
        if (norm != nullv3f)
        {
          while (norm.LengthSquared() < 1.0f) { norm *= 16.0f; }
          norm.Normalize();
        }
        bool newNorm = true;
        // Invalidate against any existing nearby face norms (redundancy check)
        for (size_t k = 0; k < nearNorms.size(); ++k)
        {
          if (nearNorms[k] == norm) { newNorm = false; }
        }
        // Add only validated non-trivial normals for subsequent summation
        if (newNorm) { nearNorms.push_back(norm); }
      }
      for (v3f j : nearNorms) { sum += j; }
      if (sum != nullv3f)
      {
        point[i].norm = sum.UnitVec(); // Convert to final unit vector normal
      }
      else
      {
        //std::cout << nearNorms.size();
        point[i].norm = Z_HAT;
      }
    }
    else
    {
      point[i].norm = Z_HAT;
    }
  }
  // Adjust stored normal visualizations
  NormalLength(nLength);

} // end void Mesh::RecalculateNormals(void)


// Scan mesh vertex data for bounds & center point to be updated              */
void dp::Mesh::RecalculateDimens(void)
{
  v3f min, max;
  for (const Vertex& i : point)
  {
    if (i.pos.x < min.x) { min.x = i.pos.x; }
    if (i.pos.x > max.x) { max.x = i.pos.x; }
    if (i.pos.y < min.y) { min.y = i.pos.y; }
    if (i.pos.y > max.y) { max.y = i.pos.y; }
    if (i.pos.z < min.z) { min.z = i.pos.z; }
    if (i.pos.z > max.z) { max.z = i.pos.z; }
  }
  bounds = max - min;
  centerPoint = (max + min) * AHALF;
  if (centerPoint != nullv3f)
  {
    for (size_t i = 0; i < vertC; ++i) { point[i].pos -= centerPoint; }
    centerPoint = nullv3f;
  }
} // end void Mesh::RecalculateDimens(void)



/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Helper Functions                               */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


// Compare the contents of two meshes for equality in their components        */
bool dp::operator==(const Mesh& lhs, const Mesh& rhs)
{
  // Establish buffers are all of equal length before component iteration
  if (lhs.Center() != rhs.Center()) { return false; }
  if (lhs.Dimensions() != rhs.Dimensions()) { return false; }
  if (lhs.VertexCount() != rhs.VertexCount()) { return false; }
  if (lhs.EdgeCount() != rhs.EdgeCount()) { return false; }
  if (lhs.TriCount() != rhs.TriCount()) { return false; }
  // Easy out confirmed invalid: begin iteration per buffer per element
  for (u4 i = 0; i < lhs.VertexCount(); ++i)
  {
    if ((&lhs.VertexBuffer() + i)->pos != (&rhs.VertexBuffer() + i)->pos)
    {
      return false;
    }
    // Investigating other attributes may yield false negatives...?
  }
  for (u4 i = 0; i < lhs.EdgeCount(); ++i)
  {
    if ((&lhs.EdgeBuffer() + i)->i != (&rhs.EdgeBuffer() + i)->i
      || (&lhs.EdgeBuffer() + i)->t != (&rhs.EdgeBuffer() + i)->t)
    {
      return false;
    }
  }
  for (u4 i = 0; i < lhs.TriCount(); ++i)
  {
    if ((&lhs.TriBuffer() + i)->i != (&rhs.TriBuffer() + i)->i
      || (&lhs.TriBuffer() + i)->c != (&rhs.TriBuffer() + i)->c
      || (&lhs.TriBuffer() + i)->t != (&rhs.TriBuffer() + i)->t)
    {
      return false;
    }
  }
  // No other elements worth evaluating: normals are just debug drawing
  return true;
} // end bool operator==(const Mesh& lhs, const Mesh& rhs)


// Compare the contents of two meshes for inequality in their components      */
bool dp::operator!=(const Mesh& lhs, const Mesh& rhs)
{
  return !(lhs == rhs);
} // end bool operator!=(const Mesh& lhs, const Mesh& rhs)
