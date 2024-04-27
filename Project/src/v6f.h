/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  v6f.h
Purpose:  API agnostic, multipurpose, 6 dimensional float-point element vector
Details:  Contained in 1 datum for return type convenience, handling any
  combination of lesser dimension data sets returning vector or point
  combinations: pairs of 3D data may be a ray (point, vector), a bounds
  (vector, vector), or a distance (point, point), be that cartesian coords or
  attribute values (RGB color ranges or the like), and 2D data triplets may
  describe triangle point coordinates among other hypotheticals.  Clearly 6D
  data set operations are also supported, if less commonly needed in graphics
  operations, and non-uniform subsets may be useful as well (2+4D, or similar).
  No such asymmetrical applications have been considered insofar however.
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

// <stl>
#include <string>       // String building methods for text value output
// "./src/..."
#include "ASCIIValue.h" // ASCII character value interpretation for operator[]
#include "Log.h"        // System message management for errors, warnings, etc
#include "v2f.h"        // Union interpretation as set of 3 2D vectors
#include "v3f.h"        // Union interpretation as set of 2 3D vectors
#include "v4f.h"        // Extraction / Swizzle of 4D vector subsets
#include "Value.h"      // Value manipulation; float point approximation
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  union v6f //! Container for 3D vector pairs, 2D coord triplets, 6D data, etc
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    struct {
      f4 x; //! Primary (1st) axis cartesian value
      f4 y; //! Secondary (2nd) axis cartesian value
      f4 z; //! Tertiary (3rd) axis cartesian value
      f4 w; //! Quaternary (4th) axis cartesian value
      f4 t; //! Quinary (5th) axis cartesian value
      f4 s; //! Senary (6th) axis cartesian value
    };
    struct {
      f4 xMin;  //! Primary axis cartesian low point
      f4 yMin;  //! Secondary axis cartesian low point
      f4 zMin;  //! Tertiary axis cartesian low point
      f4 xMax;  //! Primary axis cartesian high point
      f4 yMax;  //! Secondary axis cartesian high point
      f4 zMax;  //! Tertiary axis cartesian high point
    };
    struct {
      f4 xs;  //! Primary axis start    (cartesian right if y-up; back if z-up)
      f4 ys;  //! Secondary axis start    (cartesian up if y-up; right if z-up)
      f4 zs;  //! Tertiary axis start      (cartesian back if y-up; up if z-up)
      f4 xe;  //! Primary axis end      (cartesian right if y-up; back if z-up)
      f4 ye;  //! Secondary axis end      (cartesian up if y-up; right if z-up)
      f4 ze;  //! Tertiary axis end        (cartesian back if y-up; up if z-up)
    };
    struct {
      f4 redIn;   //! 32-bit | HDR RGB red color      (exposure ratio in [0,1])
      f4 greenIn; //! 32-bit | HDR RGB green color    (exposure ratio in [0,1])
      f4 blueIn;  //! 32-bit | HDR RGB blue color     (exposure ratio in [0,1])
      f4 redOut;  //! 32-bit | HDR RGB red color      (exposure ratio in [0,1])
      f4 greenOut;//! 32-bit | HDR RGB green color    (exposure ratio in [0,1])
      f4 blueOut; //! 32-bit | HDR RGB blue color     (exposure ratio in [0,1])
    };
    struct {
      f4 ri;   //! 32-bit | HDR RGB red color channel input    (exposure ratio)
      f4 gi;   //! 32-bit | HDR RGB green color channel input  (exposure ratio)
      f4 bi;   //! 32-bit | HDR RGB blue color channel input   (exposure ratio)
      f4 ro;   //! 32-bit | HDR RGB red color channel output   (exposure ratio)
      f4 go;   //! 32-bit | HDR RGB green color channel output (exposure ratio)
      f4 bo;   //! 32-bit | HDR RGB blue color channel output  (exposure ratio)
    };
    struct {
      f4 xa;  //! 2D triangle primary axis 1st coordinate
      f4 ya;  //! 2D triangle secondary axis 1st coordinate
      f4 xb;  //! 2D triangle primary axis 2nd coordinate
      f4 yb;  //! 2D triangle secondary axis 1st coordinate
      f4 xc;  //! 2D triangle primary axis 3rd coordinate
      f4 yc;  //! 32-bit | HDR RGB blue color     (exposure ratio in [0,1])
    };
    struct {
      v3f start;  //! Initial 3D points [0,2] of total 6D set  
      v3f end;    //! Terminal 3D points [3,5] of total 6D set
    };
    struct {
      v3f st;  //! Initial 3D points [0,2] of total 6D set  
      v3f ed;  //! Terminal 3D points [3,5] of total 6D set
    };
    struct {
      v3f init;  //! Initial 3D points [0,2] of total 6D set  
      v3f term;  //! Terminal 3D points [3,5] of total 6D set
    };
    struct {
      v3f in;   //! Input 3D value [0,2] of total 6D set  
      v3f out;  //! Output 3D value [3,5] of total 6D set
    };
    struct {
      v3f i;  //! Input 3D value [0,2] of total 6D set  
      v3f o;  //! Output 3D value [3,5] of total 6D set
    };
    struct {
      v3f orig; //! 3D Origin point [0,2] from total 6D set  
      v3f dir;  //! 3D Ray Direction [3,5] from total 6D set
    };
    struct {
      v3f min;  //! 3D cartesian point of low extrema values; subscripts [0,2]
      v3f max;  //! 3D cartesian point of high extrema values; subscripts [3,5]
    };
    struct {
      v3f mid;  //! 3D cartesian point of model center in subscripts [0,2]
      v3f ext;  //! 3D vector of model half extents in subscripts [3,5]
    };
    struct {
      v2f a;  //! 2D Triangle labeling point 1 [0,1] from total 6D set  
      v2f b;  //! 2D Triangle labeling point 2 [2,3] from total 6D set
      v2f c;  //! 2D Triangle labeling point 3 [4,5] from total 6D set
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Creates a 6 dimensional vector / point (default <0,0,0,0,0,0>)
    \brief
      - creates a 6 dimensional vector / point (default <0,0,0,0,0,0>)
    \param x
      - (x | xs | ri | redIn | xa field value) - primary axis coordinate
    \param y
      - (y | ys | gi | greenIn | ya field value) - secondary axis coordinate
    \param z
      - (z | zs | bi | blueIn | xb field value)  - tertiary axis coordinate
    \param w
      - (w | xe | ro | redOut | yb field value) - quaternary axis coordinate
    \param t
      - (t | ye | go | greenOut | xc field value) - quinary axis coordinate
    \param s
      - (s | ze | bo | blueOut | yc field value)  - senary axis coordinate
    */
    v6f(f4 x=0.0f, f4 y=0.0f, f4 z=0.0f, f4 w=0.0f, f4 t=0.0f, f4 s=0.0f);


    /** Creates a copy of a v6f from another source v6f
    \brief
      - creates a copy of a v6f from another source v6f
    \param source
      - vector to used for memberwise <x, y, z, w, t, s> element value copying
    */
    v6f(const v6f& source);


    /** Creates a copy of a v6f from another source v6f
    \brief
      - creates a copy of a v6f from another source v6f
    \param initial
      - vector to used for subscript [0,2] <x, y, z,... > element value copying
    \param terminal
      - vector to used for subscript [3,5] <..., w, t, s> element value copying
    */
    v6f(const v3f& initial, const v3f& terminal);


    /** Creates a copy of a v6f from another source v6f
    \brief
      - creates a copy of a v6f from another source v6f
    \param origin
      - point to used for subscript [0,1] <x, y, ... > element value copying
    \param v1
      - vector for subscript [2,3] <..., z, w, ...> element value copying
    \param v2
      - vector to be used for subscript [4,5] <..., t, s> element value copying
    */
    v6f(const v2f& origin, const v2f& v1, const v2f& v2);


    /** Creates a v6f from a v6f value reference resultant from a function
    \brief
      - creates a v6f from a v6f value reference resultant from a function
    \param result
      - the v6f value reference to surrogate into this
    */
    v6f(v6f&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get first v3f blue channel / tertiary v6f field using color labeling
    \brief
      - get first v3f blue channel / tertiary v6f field using color labeling
    \return
      - reference to the contained input blue ratio in tertiary v6f coordinate
    */
    const inline f4& BIn(void) const { return in.blue; }


    /** Set first v3f blue channel / tertiary v6f field using color labeling
    \brief
      - set first v3f blue channel / tertiary v6f field using color labeling
    \param input_blue
      - first blue exposure float value to set in 3rd field ([0,1] suggested)
    \param range
      - if provided, clamps the input value between the given <min,max> range
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& BIn(f4 input_blue, v2f* range = nullptr)
    {
      bi = (range ? InF(input_blue, range->max, range->min) : input_blue);
      return *this;
    }


    /** Get second v3f blue channel / senary v6f field using color labeling
    \brief
      - get second v3f blue channel / senary v6f field using color labeling
    \return
      - reference to the contained output blue ratio in senary v6f coordinate
    */
    const inline f4& BOut(void) const { return out.blue; }


    /** Set output v3f blue channel / senary v6f field using color labeling
    \brief
      - set output v3f blue channel / senary v6f field using color labeling
    \param output_blue
      - blue float value to set in 6th field / blue channel ([0,1] suggested)
    \param range
      - if provided, clamps the input value between the given <min,max> range
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& BOut(f4 output_blue, v2f* range = nullptr)
    {
      bo = (range ? InF(output_blue, range->max, range->min) : output_blue);
      return *this;
    }


    /** Calculate the orthogonal cross product between internal v3f members
    \brief
      - calculate the orthogonal cross product between ([0,2] &#x2a2f; [3,5])
    \return
      - right-handed 3D vector, orthogonal to 3D vector pair internal members
    */
    inline v3f Cross(void) const
    {
      return v3f(i.y*o.z - i.z*o.y, i.z*o.x - i.x*o.z, i.x*o.y - i.y*o.x);
    }


    /** Calculate internal member dot product of 3D vector pairs ([0,2]*[3,5])
    \brief
      - calculate internal member dot product of 3D vector pairs ([0,2]*[3,5])
    \return
      - sum of submember products: s.x * e.x + s.y * e.y + s.z * e.z
    */
    inline f4 Dot(void) const { return (xs * xe + ys * ye + zs * ze); }


    /** Calculate the dot product of (other * this) vector (product sum)
    \brief
      - calculate the dot product of (other &middot; this) vector (product sum)
    \param other
      - v6f to be dotted (memberwise multiplied and summed) with this
    \return
      - memberwise product sum: x*v.x + y*v.y + z*v.z + w*v.w + t*v.t + s*v.s
    */
    inline f4 Dot(const v6f& v) const
    {
      return x * v.x + y * v.y + z * v.z + w * v.w + t * v.t + s * v.s;
    }


    /** Compute the v6f's square magnitude: memberwise squared value sum
    \brief
      - compute the v6f's square magnitude: memberwise squared value sum
    \return
      - sum of squared member values: x^2 + y^2 + z^2 + w^2 + t^2 + s^2
    */
    inline f4 DotSelf(void) const { return (*this * *this).Sum(); }


    /** Get first v3f green channel / secondary v6f field using color labeling
    \brief
      - get first v3f green channel / secondary v6f field using color labeling
    \return
      - reference to the contained green ratio value of the secondary field
    */
    inline const f4& GIn(void) const { return in.green; }


    /** Set the v3f green channel / secondary v6f field with color labeling
    \brief
      - set the v3f green channel / secondary v6f field with color labeling
    \param input_green
      - value to be set into the 2nd field / green input ([0,1] suggested)
    \param range
      - if provided, clamps the input value between the given <min,max> range
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& GIn(f4 input_green, v2f* range = nullptr)
    {
      gi = (range? InF(input_green,range->max,range->min) : input_green);
      return *this;
    }


    /** Get output v3f green channel / quinary v6f field using color labeling
    \brief
      - get output v3f green channel / quinary v6f field using color labeling
    \return
      - reference to the contained green ouput value of the quinary field
    */
    inline const f4& GOut(void) const { return out.green; }


    /** Set output v3f green channel / quinary v6f field with color labeling
    \brief
      - set output v3f green channel / quinary v6f field with color labeling
    \param output_green
      - value to be set into the 5th field / green output ([0,1] suggested)
    \param range
      - if provided, clamps the input value between the given <min,max> range
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& GOut(f4 output_green, v2f* range = nullptr)
    {
      go = (range? InF(output_green,range->max,range->min) : output_green);
      return *this;
    }


    /** Get <center, halfExt> converted from presumed <min, max> v6f state
    \brief
      - get <center, halfExt> converted from presumed <min, max> v6f state
    \return
      - v6f data with copy of this as < (max+min), (max-min) > / 2.0 set
    */
    inline v6f InExtents(void) const
    {
      v6f copy(*this);
      return copy.ToExtents();
    }


    /** Get <min, max> converted from presumed <center, halfExt> v6f state
    \brief
      - get <min, max> converted from presumed <center, halfExt> v6f state
    \return
      - v6f data with copy of this as < center-halfExt, center+halfExt > set
    */
    inline v6f InRanges(void) const
    {
      v6f copy(*this);
      return copy.ToRanges();
    }


    /** Format a v3f for being output as a packed vector notated string
    \brief
      - format a v3f for being output as a packed vector notated string
    \return
      - text formatted to [x,y,z,w,t,s] values for later JSON interpretation
    */
    std::string JSON(void) const;


    /** Get the v3f displacement / diagonal magnitude: sqrt(x^2 + y^2 + z^2)
    \brief
      - get the v3f displacement / diagonal magnitude: sqrt(x^2 + y^2 + z^2)
    \details
      - always compare with lengthSquared instead if possible, for performance
    \return
      - magnitude of the v3f's total displacement: sqrt(x^2 + y^2 + z^2)
    */
    inline f4 Length(void) const { return SqrtF(LengthSquared()); }


    /** Get the squared v3f displacement / diagonal length: x^2 + y^2 + z^2
    \brief
      - get the squared v3f displacement / diagonal length: x^2 + y^2 + z^2
    \details
      - squares are useful for avoiding comparatively taxing square root calls
      where squared magnitude comparisons between values can suffice
    \return
      - squared length along the diagonal: x^2 + y^2 + z^2
    */
    inline f4 LengthSquared(void) const { return DotSelf(); }


    /** This v6f will be divided by its length and converted to a unit vector
    \brief
      - this v6f will be divided by its length and converted to a unit vector
    \details
      - null vector has indeterminate direction and will default to x-axis unit
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v6f& Normalize(void)
    {
      return *this;
    }


    /** Get first v3f red channel / secondary v6f field using color labeling
    \brief
      - get first v3f red channel / secondary v6f field using color labeling
    \return
      - reference to the contained red ratio value of the secondary field
    */
    inline const f4& RIn(void) const { return in.red; }


    /** Set the v3f red channel / secondary v6f field with color labeling
    \brief
      - set the v3f red channel / secondary v6f field with color labeling
    \param input_red
      - value to be set into the 2nd field / red input ([0,1] suggested)
    \param range
      - if provided, clamps the input value between the given <min,max> range
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& RIn(f4 input_red, v2f* range = nullptr)
    { return X(input_red, range); }


    /** Get output v3f red channel / quinary v6f field using color labeling
    \brief
      - get output v3f red channel / quinary v6f field using color labeling
    \return
      - reference to the contained red ouput value of the quinary field
    */
    inline const f4& ROut(void) const { return out.red; }


    /** Set output v3f red channel / quinary v6f field with color labeling
    \brief
      - set output v3f red channel / quinary v6f field with color labeling
    \param output_green
      - value to be set into the 5th field / red output ([0,1] suggested)
    \param range
      - if provided, clamps the input value between the given <min,max> range
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& ROut(f4 output_red, v2f* range = nullptr)
    { return W(output_red, range); }


    /** Get the v6f senary-axis coordinate value under 6D cartesian labeling
    \brief
      - get the v6f senary-axis coordinate value under 6D cartesian labeling
    \return
      - reference to the contained s-axis value in senary coordinate field
    */
    inline const f4& S(void) const { return s; }


    /** Set the v6f senary-axis coordinate value under 6D cartesian labeling
    \brief
      - set the v6f senary-axis coordinate value under 6D cartesian labeling
    \param s
      - the float point value to be set into the senary axis member field
    \param range
      - pointer to <min, max> range if value clamping is desired
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& S(f4 s, v2f* range = nullptr)
    {
      this->s = (range ? InF(s, range->max, range->min) : s);
      return *this;
    }


    /** Set the v6f to the given < x, y, z, w, t, s > input values
    \brief
      - set the v6f to the given < x, y, z, w, t, s > input values
    \param x
      - (x | xs | ri | redIn | xa field value) - primary axis coordinate
    \param y
      - (y | ys | gi | greenIn | ya field value) - secondary axis coordinate
    \param z
      - (z | zs | bi | blueIn | xb field value)  - tertiary axis coordinate
    \param w
      - (w | xe | ro | redOut | yb field value) - quaternary axis coordinate
    \param t
      - (t | ye | go | greenOut | xc field value) - quinary axis coordinate
    \param s
      - (s | ze | bo | blueOut | yc field value)  - senary axis coordinate
    \param b
      - pointer to <min, max> bounds per axis if clamping is desired
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& Set(f4 x, f4 y, f4 z, f4 w, f4 t, f4 s, v2f* b = nullptr)
    {
      X(x, b).Y(y, b).Z(z, b).W(w, b).T(t, b).S(s, b);
      return *this;
    }


    /** Set the v6f to the given < x, y, z, w, t, s > source input values
    \brief
      - set the v6f to the given < x, y, z, w, t, s > source input values
    \param source
      - source vector to be used for memberwise value copying
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& Set(const v6f& source)
    {
      X(source.x).Y(source.y).Z(source.z).W(source.w).T(source.t).S(source.s);
      return *this;
    }


    /** Set the v6f to the given < x, y, z, w, t, s > source input values
    \brief
      - set the v6f to the given < x, y, z, w, t, s > source input values
    \param src
      - source vector to be used for memberwise value copying
    \param b
      - pointer to <min, max> bounds per axis if clamping is desired
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& Set(const v6f& src, v2f* b = nullptr)
    {
      X(src.x, b).Y(src.y, b).Z(src.z, b).W(src.w, b).T(src.t, b).S(src.s, b);
      return *this;
    }


    /** Set the v6f values to <s1.x, s1.y, s1.z, s2.x, s2.y, s2.z> values
    \brief
      - set the v6f values to <s1.x, s1.y, s1.z, s2.x, s2.y, s2.z> values
    \param s1
      - the v3f to used for memberwise <x|(r)ed, y|(g)reen, z|(b)lue> copying
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& Set(const v3f& s1, const v3f& s2)
    {
      st.Set(s1); ed.Set(s2); return *this;
    }


    /** Format a v6f for being output as a vector notated string
    \brief
      - format a v6f for being output as a vector notated string
    \details
      - adds padding for uniform spacing: JSON tightly packs CSV elements
    \return
      - ascii text set to < x, y, z, w, t, s > with leading spaces or a - sign
    */
    std::string String(void) const;


    /** Get the sum of all 6 axis member values of the vector
    \brief
      - get the sum of all 6 axis member values of the vector
    \return
      - sum of all 6 axis members' value: x + y + z + w + t + s
    */
    inline f4 Sum(void) const { return x + y + z + w + t + s; }


    /** Get the v6f quinary-axis coordinate value under 6D cartesian labeling
    \brief
      - get the v6f quinary-axis coordinate value under 6D cartesian labeling
    \return
      - reference to the contained t-axis value in quinary coordinate field
    */
    inline const f4& T(void) const { return t; }


    /** Set the v6f quinary-axis coordinate value under 6D cartesian labeling
    \brief
      - set the v6f quinary-axis coordinate value under 6D cartesian labeling
    \param t
      - the float point value to be set into the quinary axis member field
    \param range
      - pointer to <min, max> range if value clamping is desired
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& T(f4 t, v2f* range = nullptr)
    {
      this->t = (range ? InF(t, range->max, range->min) : t);
      return *this;
    }


    /** Convert the presumed <min,max> data to <center, halfExt> format
    \brief
      - convert the presumed <min,max> data to <center, halfExt> format
    \details
      - given data in <min,max>, set -> < (max + min), (max - min) > / 2
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& ToExtents(void)
    {
      v3f extents = (max - min);
      v3f middle = (max + min);
      mid = middle;
      ext = extents;
      *this /= 2.0f;
      return *this;
    }


    /** Convert the presumed <center, halfExt> data to <min,max> format
    \brief
      - convert the presumed <center, halfExt> data to <min,max> format
    \details
      - given data in < mid, ext >, set -> < (mid - ext), (mid + ext) >
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& ToRanges(void)
    {
      v3f low = mid - ext;
      v3f high = mid + ext;
      for (u1 i = 0; i < 3; ++i)
      {
        if (low[i] > high[i]) { SwapF(low[i], high[i]); }
      }
      min = low;
      max = high;
      return *this;
    }


    /** Value copy will be divided by its length and converted to a unit vector
    \brief
      - value copy will be divided by its length and converted to a unit vector
    \return
      - displacement vector with length 1, in the direction of this v3f
    */
    inline v6f UnitVec(void) const
    {
      f4 l = LengthSquared();
      return (NearF(l, 0.0f, 0.0001f) ? *this : (*this / SqrtF(l)));
    }


    /** Get the v6f quaternary-axis coordinate value under 6D cartesian labeling
    \brief
      - get the v6f quaternary-axis coordinate value under 6D cartesian labeling
    \return
      - reference to the contained w-axis value in quaternary coordinate field
    */
    inline const f4& W(void) const { return w; }


    /** Set the v6f quaternary-axis coordinate value under 6D cartesian labeling
    \brief
      - set the v6f quaternary-axis coordinate value under 6D cartesian labeling
    \param w
      - the float point value to be set into the quaternary axis member field
    \param range
      - pointer to <min, max> range if value clamping is desired
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& W(f4 w, v2f* range = nullptr)
    {
      this->w = (range ? InF(w, range->max, range->min) : w);
      return *this;
    }


    /** Get the v3f primary-axis coordinate value under 3D cartesian labeling
    \brief
      - get the v3f primary-axis coordinate value under 3D cartesian labeling
    \return
      - reference to the contained x-axis coordinate in primary v6f f4 field
    */
    inline const f4& X(void) const { return x; }


    /** <x, x> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <x, x> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <x, x> values of this v6f
    */
    inline v2f XX(void) const { return v2f(x, x); }


    /** <x, y> 2D swizzling operation to get the first 2 dimensions
    \brief
      - <x, y> 2D swizzling operation to get the first 2 dimensions
    \return
      - a v2f set to use the <x, y> values of this v6f
    */
    inline v2f XY(void) const { return v2f(x, y); }


    /** <x, z> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <x, z> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <x, z> values of this v6f
    */
    inline v2f XZ(void) const { return v2f(x, z); }


    /** <x, y, z, w> 4D swizzling operation to promote to 4 dimensions
    \brief
      - <x, y, z, w> 4D swizzling operation to promote to 4 dimensions
    \return
      - a v4f set to use the <x, y, , wz> values of this v6f
    */
    inline v4f XYZW(void) const { return v4f(x, y, z, w); }


    /** Set the v6f primary-axis coordinate value under 6D cartesian labeling
    \brief
      - set the v6f primary-axis coordinate value under 6D cartesian labeling
    \param x
      - the float-point value to be set into the primary-axis member field
    \param range
      - pointer to <min, max> range if value clamping is desired
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& X(f4 x, v2f* range = nullptr)
    {
      this->x = (range ? InF(x, range->max, range->min) : x);
      return *this;
    }


    /** Get the v6f secondary-axis coordinate value under 6D cartesian labeling
    \brief
      - get the v6f secondary-axis coordinate value under 6D cartesian labeling
    \return
      - reference to the contained, secondary v6f f4 coordinate field
    */
    inline const f4& Y(void) const { return y; }


    /** Set the v6f secondary-axis coordinate value with 6D cartesian labeling
    \brief
      - set the v6f secondary-axis coordinate value with 6D cartesian labeling
    \param y
      - the float point value to be set into the secondary-axis member field
    \param range
      - pointer to <min, max> range if value clamping is desired
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& Y(f4 y, v2f* range = nullptr)
    {
      this->y = (range ? InF(y, range->max, range->min) : y);
      return *this;
    }


    /** <y, x> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <y, x> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <y, x> values of this v6f
    */
    inline v2f YX(void) const { return v2f(y, x); }


    /** <y, y> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <y, y> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <y, y> values of this v6f
    */
    inline v2f YY(void) const { return v2f(y, y); }


    /** <y, z> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <y, z> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <y, z> values of this v6f
    */
    inline v2f YZ(void) const { return v2f(y, z); }


    /** Get the v6f tertiary-axis coordinate value under 6D cartesian labeling
    \brief
      - get the v6f tertiary-axis coordinate value under 6D cartesian labeling
    \return
      - reference to the contained z-axis value in tertiary coordinate field
    */
    inline const f4& Z(void) const { return z; }


    /** Set the v6f tertiary-axis coordinate value under 6D cartesian labeling
    \brief
      - set the v6f tertiary-axis coordinate value under 6D cartesian labeling
    \param z
      - the float point value to be set into the tertiary axis member field
    \param range
      - pointer to <min, max> range if value clamping is desired
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& Z(f4 z, v2f* range = nullptr)
    {
      this->z = (range ? InF(z, range->max, range->min) : z);
      return *this;
    }


    /** <z, x> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <z, x> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <z, x> values of this v6f
    */
    inline v2f ZX(void) const { return v2f(z, x); }


    /** <z, y> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <z, y> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <z, y> values of this v6f
    */
    inline v2f ZY(void) const { return v2f(z, y); }


    /** <z, z> 2D swizzling operation to get the corresponding dimensions
    \brief
      - <z, z> 2D swizzling operation to get the corresponding dimensions
    \return
      - a v2f set to use the <z, z> values of this v3f
    */
    inline v2f ZZ(void) const { return v2f(z, z); }


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Creates a negated copy with relevant v3f coordinate values
    \brief
      - creates a negated copy with relevant v3f coordinate values
    \return
      - copy of the v3f with <-x, -y, -z> member values
    */
    inline v6f operator-(void) const { return v6f(-x, -y, -z, -w, -t, -s); }


    /** Sets the source v6f member values into this v6f value fields
    \brief
      - sets the source v6f member values into this v6f value fields
    \param source
      - the v6f which will have its values copied memberwise into this v6f
    \return
      - reference to the modified v3f for easier subsequent inline action
    */
    inline v6f& operator=(const v6f& src)
    {
      Set(src.x, src.y, src.z, src.w, src.t, src.s);
      return *this;
    }


    /** Negates v6f element values to represent the inverse displacement
    \brief
      - negates v6f element values to represent the inverse displacement
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& operator!(void)
    {
      Set(-x, -y, -z, -w, -t, -s);
      return *this;
    }


    /** Get a value from ['x'], ['y'] or ['z'] channels (or equivalent)
    \brief
      - get a value from ['x'], ['y'] or ['z'] channels (or equivalent)
    \details
      (0 | x | X | r | R | i | I | u | U) -> x
      (1 | y | Y | g | G | j | J | v | V) -> y
      (2 | z | Z | b | B | k | K | n | N) -> z
    \param channel
      - use (0|'x'|'r')->x, (1|'y'|'g')->y, (2|'z'|'b')->z
    \return
      - copy of the contained dimension member value associated to the channel
    */
    inline const f4& operator[](s1 channel) const
    {
      // clamp wildly out of range values into lowest 2 unsigned values
      if (!isascii(channel) /* msb on */ || (channel < 0x20  /* control seq */
                                          && channel > 0xF /* hex range */))
      {
        Log::Diag(std::string("v6f::operator[] - value out of range '")
          + char(channel) + "' read, ", false);
        channel &= SUB_MASK;
        Log::D(std::string("setting to '") + std::to_string(channel) + "'", 1);
      }
      // Simplify numeral values to low range values in subscript range
      else if (IsDigit(channel)) { channel = ToNumeral(channel) & SUB_MASK; }
      switch (channel)
      {
      // wraparound case values from (0110b-1) to (1000b-1) for simplicity
      case 'x': case 'X': case 0: case 6: default: return x;
      case 'y': case 'Y': case 1: case 7: return y;
      case 'z': case 'Z': case 2: return z;
      case 'w': case 'W': case 3: return w;
      case 't': case 'T': case 4: return t;
      case 's': case 'S': case 5: return s;
      }
    }


    /** Get a channel value in [x=0], [y=1], [z=2], [w=3], [t=4], [s=5]
    \brief
      - get a channel value in [x=0], [y=1], [z=2], [w=3], [t=4], [s=5]
    \details
      (0 | x | X) -> x, (1 | y | Y) -> y, (2 | z | Z) -> z
      (3 | w | W) -> w, (4 | t | T) -> t, (5 | s | S) -> s
    \param channel
      - use (0|'x'|'r')->x, (1|'y'|'g')->y, (2|'z'|'b')->z
    \return
      - reference to the contained field associated to the subscript number
    */
    inline f4& operator[](s1 channel)
    {
      // clamp wildly out of range values into lowest 2 unsigned values
      if (!isascii(channel) /* msb on */ || (channel < 0x20  /* control seq */
                                          && channel > 0xF /* hex range */))
      {
        Log::Diag(std::string("v6f::operator[] - value out of range '")
          + char(channel) + "' read, ", false);
        channel &= SUB_MASK;
        Log::D(std::string("setting to '") + std::to_string(channel) + "'", 1);
      }
      // Simplify numeral values to low range values in subscript range
      else if (IsDigit(channel)) { channel = ToNumeral(channel) & SUB_MASK; }
      switch (channel)
      {
        // wraparound value cases from (0110b-1) to (1000b-1) for simplicity
      case 'x': case 'X': case 0: case 6: default: return x;
      case 'y': case 'Y': case 1: case 7: return y;
      case 'z': case 'Z': case 2: return z;
      case 'w': case 'W': case 3: return w;
      case 't': case 'T': case 4: return t;
      case 's': case 'S': case 5: return s;
      }
    }


    /** Calculate the member-wise addition of 2 v6f's and return the result
    \brief
      - calculate the member-wise addition of 2 v6f's and return the result
    \param other
      - source vector to have memberwise summation performed with a copy of this
    \return
      - copy of the memberwise sum of this v3f and the other into a new v3f
    */
    inline v6f operator+(const v6f& other) const
    {
      v6f result(*this);
      return (result += other);
    }


    /** Add an other v6f's values memberwise into this v6f's values
    \brief
      - add an other v6f's values memberwise into this v6f's values
    \param other
      - source vector to have memberwise summation performed with this
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& operator+=(const v6f& other)
    {
      x += other.x; y += other.y; z += other.z;
      w += other.w; t += other.t; s += other.s;
      return *this;
    }


    /** Calculate the member-wise difference of 2 v6f's and return the result
    \brief
      - calculate the member-wise difference of 2 v6f's and return the result
    \param other
      - source vector to have memberwise difference calculated with this (copy)
    \return
      - copy of the memberwise difference of this and the other, in a new v6f
    */
    inline v6f operator-(const v6f& other) const
    {
      v6f result(*this);
      return (result -= other);
    }


    /** Subtract an other v6f's values memberwise from this v6f's values
    \brief
      - subtract an other v6f's values memberwise from this v6f's values
    \param other
      - source vector to have negated memberwise summation performed with this
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& operator-=(const v6f& other)
    {
      x -= other.x; y -= other.y; z -= other.z;
      w -= other.w; t -= other.t; s -= other.s;
      return *this;
    }


    /** Scalar product multiplies each field uniformly by a scalar constant
    \brief
      - scalar product multiplies each field uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply x, y and z values
    \return
      - resultant, member-wise scaled copy of this v3f's values
    */
    v6f operator*(f4 scalar) const
    {
      v6f scaled(*this);
      return (scaled *= scalar);
    } // end v3f operator*(f4, const v3f&)


    /** Scalar product sets all members to grow uniformly by a scalar constant
    \brief
      - scalar product sets all members to grow uniformly by a scalar constant
    \param scalar
      - scalar amount by which to multiply each member's values
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    v6f& operator*=(f4 scalar)
    {
      x *= scalar; y *= scalar; z *= scalar;
      w *= scalar; t *= scalar; s *= scalar;
      return *this;
    }


    /** Memberwise multiply the other v6f's members (Dot or Sum for DotProduct)
    \brief
      - memberwise multiply the other v6f's members (Dot or Sum for DotProduct)
    \param other
      - external source vector to be memberwise multiplied into this vector
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& operator*(const v6f& other)
    {
      x *= other.x; y *= other.y; z *= other.z;
      w *= other.w; t *= other.t; s *= other.s;
      return *this;
    }


    /** Memberwise multiply the other v6f's members (Dot or Sum for DotProduct)
    \brief
      - memberwise multiply the other v6f's members (Dot or Sum for DotProduct)
    \details
      - Dot products inarguably useful: having ||u||*||v||*cos(angle) property,
      parallelogram area, etc. unfortunately, while conventional, 2 overloads
      can't share the same parameter list, operators are inflexible on
      parameters, and the resultant members can be summed separately, making
      the component operation of memberwise multiplication useful to retain
      granular access, as attribute multiplication is still important in non
      cartesian aspects. Additionally, type retention allows for *= operations.
    \param other
      - external source vector to be memberwise multiplied with this vector
    \return
      - vector product: < ax*bx, ay*by, az*bz, aw*bw, at*bt, as*bs >
    */
    inline v6f operator*(const v6f& other) const
    {
      v6f result(*this);
      result.x *= other.x; result.y *= other.y; result.z *= other.z;
      result.w *= other.w; result.t *= other.t; result.s *= other.s;
      return  result;
    }


    /** Perform uniform division by divisor, returning the result
    \brief
      - perform uniform division by divisor, returning the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which <x,y,z,w,t,s> values are uniformly divided
    \return
      - resultant, member-wise scaled copy of this v6f's values
    */
    inline v6f operator/(f4 divisor) const
    {
      return *this * (1.0f / divisor);
    }


    /** Divides all members uniformly by divisor, setting the result
    \brief
      - divides all members uniformly by divisor, setting the result
    \details
      - vectors aren't commutative, can have a scalar quotient; aren't divisors
    \param divisor
      - amount by which <x,y,z,w,t,s> values are to be uniformly divided
    \return
      - reference to the modified v6f for easier subsequent inline action
    */
    inline v6f& operator/=(f4 divisor)
    {
      return *this *= (1.0f / divisor);
    }
  private:


    static const u1 SUBSCRIPTS = 6u;  //! v6f class is fixed at 6 subscripts
    static const u1 SUB_MASK = 7u;  //! [0,5] requires 111b, allowing 110b, 111b


  }; // end v6f union / struct / class

  // Ensurance that simple struct only contains what it should (w/many labels)
  static_assert(sizeof(v6f) == 4*6, "sizeof(v6f) != 24");

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


  /** Left hand scalar multiplication scales all members of rhs
  \brief
    - left hand scalar multiplication scales all members of rhs
  \param scalar
    - scalar amount by which to multiply each axes values of right-hand side
  \param rhs
    - right-hand side argument to be copied and scaled memberwise by scalar
  \return
    - copy of rhs v6f's values, scaled by scalar in <x,y,z,w,t,s>
  */
  inline v6f operator*(f4 scalar, const v6f& rhs)
  {
    return rhs * scalar;

  } // end v6f operator*(f4, const v6f&)                                      */


   /** Confirm if 2 v6f's are relatively equal in all member values
   \brief
     - confirm if 2 v6f's are relatively equal in all member values
   \param lhs
     - left hand side to be compared memberwise with right hand for equality
   \param rhs
     - right hand side to be compared memberwise with left hand for equality
   \return
     - true only if all members are in close proximity of one another
   */
   inline bool operator==(const v6f& lhs, const v6f& rhs)
   {
     constexpr f4 p = _INf;
     return (NearF(lhs.x, rhs.x, p) && NearF(lhs.y, rhs.y, p)
          && NearF(lhs.z, rhs.z, p) && NearF(lhs.w, rhs.w, p)
          && NearF(lhs.t, rhs.t, p) && NearF(lhs.s, rhs.s, p));
   } // end bool operator==(const v6f&, const v6f&)                           */


   /** Confirm if 2 v6f's are not equal in any respective members' values
   \brief
     - confirm if 2 v6f's are not equal in any respective members' values
   \param lhs
     - left hand side to be compared memberwise with right hand for inequality
   \param rhs
     - right hand side to be compared memberwise with left hand for inequality
   \return
     - true if any members are not in close proximity of one another
   */
   inline bool operator!=(const v6f& lhs, const v6f& rhs)
   {
     return !(lhs == rhs);
   }


   /** Output the string version of the vector element data
   \brief
     - output the string version of the vector element data
   \param output
     - stream to receive the vector string and and carry contents for output
   \param vector
     - vector to have the member values be converted to text based output
   \return
     - reference to the output stream object having the vector fed in
   */
   inline std::ostream& operator<<(std::ostream& output, const v6f& vector)
   {
     output << vector.String();
     return output;
   }


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   //! Zero vector in 6 dimensions; error state reserved value
   extern const v6f& nullv6f;

} // end dp namespace
