/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  p2f.h
Purpose:  API agnostic, 2 dimensional float-point element polar vector
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Project:  a.Surprise_MAT320_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Fall 2021
Copyright (C) 2021 DigiPen Institute of Technology.
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
#include "ang.h"        // angle extraction requires arc unit parameter
#include "TypeErrata.h" // Platform primitive type aliases for easier porting
#include "v2f.h"        // Cartesian input / conversion return values


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  
  union p2f //! 2D vector container of 2, 4-byte float point elements
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Value range safeguard: & by 1b equivalent and faster than % by 10b
    static const u1 VSUBS_MASK = 0x1;

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Members                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    struct {
      ang t;  //! Theta measure of rotation angle in the xy plane about origin
      f4 r;   //! Radius angle scalar value, or magnitude / length of angle arc
    };
    struct {
      ang theta;  //! Measure of rotation angle in the xy plane about origin
      f4 radius;  //! Angle scalar value, or magnitude / length of angle arc
    };
    struct {
      ang a;  //! Angle of rotation equivalent to unit circle cartesian coords
      f4 m;   //! Magnitude / length of angle scalar, or radius of angle arc
    };
    struct {
      ang phi;  //! Angle of rotation; unit circle cartesian coords equivalent
      f4 mag;   //! Magnitude / length of angle scalar, or radius of angle arc
    };
    struct {
      ang angle;    //! Rotation; unit circle cartesian coords equivalent
      f4 magnitude; //! Length of angle scalar, or radius of angle arc
    };
    struct {
      ang d;  //! Angle expression of unit circle cartesian coords equivalent
      f4 l;   //! Length of angle scalar, or radius of angle arc
    };
    struct {
      ang dir;  //! Angle expression of unit circle cartesian coords equivalent
      f4 len;   //! Length of angle scalar, or radius of angle arc
    };
    struct {
      ang direction;  //! Angle expression of unit cartesian coords equivalent
      f4 length;      //! Length of direction, diagonal measurement of angle
    };

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


    /** Creates a polar 2D vector holding <angle, magnitude> of displacement
    \brief
      - creates a polar 2D vector holding <angle, magnitude> of displacement
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    */
    p2f(f4 angle, arc measure, f4 magnitude = 1.0f);


    /** Creates a polar 2D vector holding <angle, magnitude> of displacement
    \brief
      - creates a polar 2D vector holding <angle, magnitude> of displacement
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    */
    p2f(ang angle, f4 magnitude = 1.0f);


    /** Creates a copy of a p2f from another source p2f
    \brief
      - creates a copy of a p2f from another source p2f
    \param source
      - the p2f to used for memberwise <t|a, r|m> element value copying
    */
    p2f(const p2f& source);


    /** Creates a v2f from a v2f value reference resultant from a function
    \brief
      - creates a v2f from a v2f value reference resultant from a function
    \param result
      - the v2f value reference to surrogate into this
    */
    p2f(p2f&& result) noexcept;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get angle / direction measure currently set in polar vector
    \brief
      - get angle / direction measure currently set in polar vector
    \return
      - angle of rotation from <1,0> coordinate in the xy plane
    */
    ang A(void) const;


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& A(f4 angle, arc measure);


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& A(ang angle);


    /** Get angle / direction measure currently set in polar vector
    \brief
      - get angle / direction measure currently set in polar vector
    \return
      - angle of rotation from <1,0> coordinate in the xy plane
    */
    ang Ang(void) const;


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Ang(f4 angle, arc measure);


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Ang(ang angle);


    /** Get angle / direction measure currently set in polar vector
    \brief
      - get angle / direction measure currently set in polar vector
    \return
      - angle of rotation from <1,0> coordinate in the xy plane
    */
    ang Angle(void) const;


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Angle(f4 angle, arc measure);


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Angle(ang angle);


    /** Get angle / direction measure currently set in polar vector
    \brief
      - get angle / direction measure currently set in polar vector
    \return
      - angle of rotation from <1,0> coordinate in the xy plane
    */
    ang D(void) const;


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& D(f4 angle, arc measure);


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& D(ang angle);


    /** Get angle / direction measure currently set in polar vector
    \brief
      - get angle / direction measure currently set in polar vector
    \return
      - angle of rotation from <1,0> coordinate in the xy plane
    */
    ang Dir(void) const;


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Dir(f4 angle, arc measure);


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Dir(ang angle);


    /** Get angle / direction measure currently set in polar vector
    \brief
      - get angle / direction measure currently set in polar vector
    \return
      - angle of rotation from <1,0> coordinate in the xy plane
    */
    ang Direction(void) const;


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Direction(f4 angle, arc measure);


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Direction(ang angle);


    /** Format a p2f for being output as a packed vector notated string
    \brief
      - format a p2f for being output as a packed vector notated string
    \return
      - text formatted to [t,r] standards for later JSON interpretation
    */
    std::string JSON(void) const;


    /** Get displacement length / magnitude independent of angle / direction
    \brief
      - get displacement length / magnitude independent of angle / direction
    \return
      - diagonal displacement magnitude of polar vector along the set angle
    */
    f4 L(void) const;


    /** Set the p2f displacement / diagonal magnitude measure of radius
    \brief
      - set the p2f displacement / diagonal magnitude measure of radius
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& L(f4 magnitude);


    /** Get displacement length / magnitude independent of angle / direction
    \brief
      - get displacement length / magnitude independent of angle / direction
    \return
      - diagonal displacement magnitude of polar vector along the set angle
    */
    f4 Len(void) const;


    /** Set the p2f displacement / diagonal magnitude measure of radius
    \brief
      - set the p2f displacement / diagonal magnitude measure of radius
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Len(f4 magnitude);


    /** Get displacement length / magnitude independent of angle / direction
    \brief
      - get displacement length / magnitude independent of angle / direction
    \return
      - diagonal displacement magnitude of polar vector along the set angle
    */
    f4 Length(void) const;


    /** Set the p2f displacement / diagonal magnitude measure of radius
    \brief
      - set the p2f displacement / diagonal magnitude measure of radius
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Length(f4 magnitude);


    /** Get the squared displacement / diagonal length: radius^2
    \brief
      - get the squared displacement / diagonal length: radius^2
    \details
      - squares are useful for avoiding comparatively taxing square root calls
      where squared magnitude comparisons between values can suffice
    \return
      - squared length along the diagonal: x^2 + y^2 + z^2
    */
    f4 LengthSquared(void) const;


    /** Get displacement length / magnitude independent of angle / direction
    \brief
      - get displacement length / magnitude independent of angle / direction
    \return
      - diagonal displacement magnitude of polar vector along the set angle
    */
    f4 M(void) const;


    /** Set the p2f displacement / diagonal magnitude measure of radius
    \brief
      - set the p2f displacement / diagonal magnitude measure of radius
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& M(f4 magnitude);


    /** Get displacement length / magnitude independent of angle / direction
    \brief
      - get displacement length / magnitude independent of angle / direction
    \return
      - diagonal displacement magnitude of polar vector along the set angle
    */
    f4 Mag(void) const;


    /** Set the p2f displacement / diagonal magnitude measure of radius
    \brief
      - set the p2f displacement / diagonal magnitude measure of radius
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Mag(f4 magnitude);


    /** Get displacement length / magnitude independent of angle / direction
    \brief
      - get displacement length / magnitude independent of angle / direction
    \return
      - diagonal displacement magnitude of polar vector along the set angle
    */
    f4 Magnitude(void) const;


    /** Set the p2f displacement / diagonal magnitude measure of radius
    \brief
      - set the p2f displacement / diagonal magnitude measure of radius
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Magnitude(f4 magnitude);


    /** This will have its length set to 1 and be converted to a unit vector
    \brief
      - this will have its length set to 1 and be converted to a unit vector
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Normalize(void);


    /** Get angle / direction measure currently set in polar vector
    \brief
      - get angle / direction measure currently set in polar vector
    \return
      - angle of rotation from <1,0> coordinate in the xy plane
    */
    ang Phi(void) const;


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Phi(f4 angle, arc measure);


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Phi(ang angle);


    /** Get displacement length / magnitude independent of angle / direction
    \brief
      - get displacement length / magnitude independent of angle / direction
    \return
      - diagonal displacement magnitude of polar vector along the set angle
    */
    f4 R(void) const;


    /** Set the p2f displacement / diagonal magnitude measure of radius
    \brief
      - set the p2f displacement / diagonal magnitude measure of radius
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& R(f4 magnitude);


    /** Get displacement length / magnitude independent of angle / direction
    \brief
      - get displacement length / magnitude independent of angle / direction
    \return
      - diagonal displacement magnitude of polar vector along the set angle
    */
    f4 Rad(void) const;


    /** Set the p2f displacement / diagonal magnitude measure of radius
    \brief
      - set the p2f displacement / diagonal magnitude measure of radius
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Rad(f4 magnitude);


    /** Get displacement length / magnitude independent of angle / direction
    \brief
      - get displacement length / magnitude independent of angle / direction
    \return
      - diagonal displacement magnitude of polar vector along the set angle
    */
    f4 Radius(void) const;


    /** Set the p2f displacement / diagonal magnitude measure of radius
    \brief
      - set the p2f displacement / diagonal magnitude measure of radius
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Radius(f4 magnitude);


    /** Set the p2f to angle component and length input values
    \brief
      - set the p2f to angle component and length input values
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \param magnitude
      - length of displacement in the diagonal, direct route of translation
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Set(f4 angle, arc measure, f4 magnitude = 1.0f);


    /** Set the p2f from polar component input values
    \brief
      - set the p2f from polar component input values
    \param angle
      - direction of rotational offset from <1,0> in the xy plane about z
    \param magnitude
      - diagonal length / radius of displacement the vector should produce
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Set(ang angle, f4 magnitude = 1.0f);


    /** Set the p2f values to <source.theta, source.radius> values
    \brief
      - set the p2f values to <source.theta, source.radius> values
    \param source
      - the p2f to used for memberwise <angle, radius> copying
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Set(const p2f& source);


    /** Set the polar equivalent <theta,r> from cartesian <source.x, source.y> 
    \brief
      - set the polar equivalent <theta,r> from cartesian <source.x, source.y> 
    \param source
      - the v2f to used for <x|u, y|v> cartesian to <theta, r> polar conversion
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Set(const v2f& source);


    /** Set the p2f to use <result.theta, result.radius> values (directly)
    \brief
      - set the p2f to use <result.theta, result.radius> values (directly)
    \param result
      - the v2f value reference from which to have values surrogated
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& Set(p2f&& result) noexcept;


    /** Format a p2f for being output as a vector notated string
    \brief
      - format a p2f for being output as a vector notated string
    \details
      - adds padding and units: JSON tightly packs only CSV values
    \measure
      - specifies units of output measured for angle (default radians)
    \return
      - text set to < r radians, m length > with aligned padding for signs etc
    */
    std::string String(arc measure = arc::radians) const;


    /** Get angle / direction measure currently set in polar vector
    \brief
      - get angle / direction measure currently set in polar vector
    \return
      - angle of rotation from <1,0> coordinate in the xy plane
    */
    ang T(void) const;


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& T(f4 angle, arc measure);


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& T(ang angle);


    /** Convert polar 2D rotation and length to cartesian <x,y> 2D vector
    \brief
      - convert polar 2D rotation and length to cartesian <x,y> 2D vector
    \brief
      - polar <t, r> coords => <x=:r*cos(t), y=:r*sin(t)> 2D vector in xy plane
    \return
      - cartesian <x, y> vector of equivalent displacement measures to this
    */
    v2f ToCartesian(void) const;


    /** Get angle / direction measure currently set in polar vector
    \brief
      - get angle / direction measure currently set in polar vector
    \return
      - angle of rotation from <1,0> coordinate in the xy plane
    */
    ang Theta(void) const;


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \param measure
      - units of angle of displacement direction: arc::radians, degrees, etc.
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Theta(f4 angle, arc measure);


    /** Set polar direction coordinate in terms of rotation from <1, 0> in xy
    \brief
      - set polar direction coordinate in terms of rotation from <1, 0> in xy
    \param angle
      - angle equivalent to direction of displacement in the xy plane
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& Theta(ang angle);


    /** Copy will have its length set to 1 and be converted to a unit vector
    \brief
      - copy will have its length set to 1 and be converted to a unit vector
    \return
      - polar 2D vector with length 1, in the direction of this p2f
    */
    p2f UnitVec(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Sets the source v2f member values into this v2f value fields
    \brief
      - sets the source v2f member values into this v2f value fields
    \param source
      - the v2f which will have its values copied memberwise into this v2f
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& operator=(const p2f& source);
    

    /** Sets the source v2f member values into this v2f value fields
    \brief
      - sets the source v2f member values into this v2f value fields
    \param source
      - the v2f which will have its values copied memberwise into this v2f
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& operator=(const v2f& source);


    /** Surrogate a v2f value reference returned from a function for this v2f
    \brief
      - surrogate a v2f value reference returned from a function for this v2f
    \param result
      - the v2f reference resultant from a function from which to steal values
    \return
      - reference to the modified v2f for easier subsequent inline action
    */
    p2f& operator=(p2f&& result) noexcept;


    /** Polar vector produced has antiparallel direction by cw &pi; rotation
    \brief
      - polar vector produced has antiparallel direction by cw &pi; rotation
    \details
      - negating both direction and magnitude would be ostensibly identical,
        & negating direction is more pertinent, versatile and less error prone
        than negated magnitude, hence angle is changed instead of length.
        as unary (-) aligns to negation more than (!) being open to
        interpretation, and rotation being capable of winding multiple periods,
        having inverse operations makes sense.  due to (-) going in the
        mathematically negative direction, going clockwise makes most sense.
    \return
      - copy of the p2f with <t-&pi;, r> member values
    */
    p2f operator-(void) const;


    /** Polar vector produced has antiparallel direction by ccw &pi; rotation
    \brief
      - polar vector produced has antiparallel direction by ccw &pi; rotation
    \details
      - negating both direction and magnitude would be ostensibly identical,
        & negating direction is more pertinent, versatile and less error prone
        than negated magnitude, hence angle is changed instead of length.
        as unary (-) aligns to negation more than (!) being open to
        interpretation, and rotation being capable of winding multiple periods,
        having inverse operations makes sense.  due to (!) going in the
        mathematically positive direction, counterclockwise makes most sense.
    \return
      - copy of the p2f with <t+&pi;, r> member values
    */
    p2f operator!(void) const;


    /** Get value from ['d'|'a'|'t'] or ['m'|'l'|'r'] channels (or equivalent)
    \brief
      - get value from ['d'|'a'|'t'] or ['m'|'l'|'r'] channels (or equivalent)
    \details
      (0 | a | A | t | T | d | D) -> direction | angle  | theta 
      (1 | y | Y | j | J | v | V) -> magnitude | length | radius 
      since angle is a unit bearing value (ang != f4), return by f4 reference
      would open datum to convenient but undesirable value mutability, hence f4
    \param channel
      - use (0|'d'|'a'|'t')->x, (1|'m'|'l'|'r')->y to access one member value
    \return
      - copy of the contained dimension member value associated to the channel
    */
    f4 operator[](s1 channel);


    /** Get value from ['d'|'a'|'t'] or ['m'|'l'|'r'] channels (or equivalent)
    \brief
      - get value from ['d'|'a'|'t'] or ['m'|'l'|'r'] channels (or equivalent)
    \details
      (0 | a | A | t | T | d | D) -> direction | angle  | theta 
      (1 | y | Y | j | J | v | V) -> magnitude | length | radius 
      since angle is a unit bearing value (ang != f4), return by a const f4
      reference precludes convenient but undesirable value mutability
    \param channel
      - use (0|'d'|'a'|'t')->x, (1|'m'|'l'|'r')->y to access one member value
    \return
      - reference to the contained field associated to the subscript number
    */
    const f4& operator[](s1 channel) const;


    /** Calculate the member-wise addition of 2 p2f's and return the result
    \brief
      - calculate the member-wise addition of 2 p2f's and return the result
    \details
      - requires cartesian form, conversion back should not be obligatory
    \param other
      - source polar vector to be cartesian vector summed with this
    \return
      - copy of the memberwise sum of this p2f and the other into a new v2f
    */
    v2f operator+(const p2f& other) const;


    /** Add a cartesian v2f to this polar p2f, returning the result
    \brief
      - add a cartesian v2f to this polar p2f, returning the result
    \details
      - requires cartesian form, conversion back should not be obligatory
    \param other
      - source polar vector to be cartesian vector summed with this
    \return
      - copy of the memberwise sum of this p2f and the other v2f into a v2f
    */
    v2f operator+(const v2f& other) const;


    /** Calculate the member-wise addition of 2 p2f's and set the result
    \brief
      - calculate the member-wise addition of 2 p2f's and set the result
    \details
      - requires conversion to cartesian form, then back to polar form
        as such, it's not the most efficient to use polar vectors if addition
        or subtraction is required to be performed with any regularity
    \param other
      - source polar vector to be cartesian vector summed with this
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& operator+=(const p2f& other);


    /** Add a cartesian v2f to this polar p2f, setting the result
    \brief
      - add a cartesian v2f to this polar p2f, setting the result
    \details
      - requires conversion to cartesian form, then back to polar form
        as such, it's not the most efficient to use polar vectors if addition
        or subtraction is required to be performed with any regularity
    \param other
      - source cartesian vector to be memberwise summed with this
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& operator+=(const v2f& other);


    /** Calculate the member-wise difference of 2 p2f's and return the result
    \brief
      - calculate the member-wise difference of 2 p2f's and return the result
    \details
      - requires cartesian v2f form, but p2f conversion shouldn't be obligate
    \param other
      - source polar vector to be cartesian vector subtracted from this
    \return
      - copy of the memberwise difference of this and the other, in a new v2f
    */
    v2f operator-(const p2f& other) const;


    /** Subtract a cartesian v2f from this polar p2f, returning the result
    \brief
      - subtract a cartesian v2f from this polar p2f, returning the result
    \details
      - requires cartesian v2f form, but p2f conversion shouldn't be obligate
    \param other
      - source cartesian vector to be subtracted from this (polar), vector
    \return
      - copy of the memberwise difference of this and the other, in a new v2f
    */
    v2f operator-(const v2f& other) const;


    /** Subtract another p2f from this p2f's values, setting the result in this
    \brief
      - subtract another p2f from this p2f's values, setting the result in this
    \details
      - requires conversion to cartesian form, then back to polar form
        as such, it's not the most efficient to use polar vectors if addition
        or subtraction is required to be performed with any regularity
    \param other
      - source vector to have negated memberwise summation performed with this
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& operator-=(const p2f& other);


    /** Subtract a cartesian v2f from this polar p2f, set this as the result
    \brief
      - subtract a cartesian v2f from this polar p2f, set this as the result
    \details
      - requires conversion to cartesian form, then back to polar form
        as such, it's not the most efficient to use polar vectors if addition
        or subtraction is required to be performed with any regularity
    \param other
      - source vector to have negated memberwise summation performed with this
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& operator-=(const v2f& other);


    /** Scalar polar product multiplies only magnitude by the scalar constant
    \brief
      - scalar polar product multiplies only magnitude by the scalar constant
    \param scalar
      - scalar amount by which to multiply length of the vector
    \return
      - polar vector scaled in length by scalar, with direction unaffected
    */
    p2f operator*(f4 scalar) const;


    /** Scalar polar product multiplies only magnitude by the scalar constant
    \brief
      - scalar polar product multiplies only magnitude by the scalar constant
    \param scalar
      - scalar amount by which to multiply length of the vector
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& operator*=(f4 scalar);


    /** Convert to cartesian to return dot product (product sum) with other v2f
    \brief
      - convert to cartesian to return dot product (product sum) with other v2f
    \details
      - other multiplicands bear different types; may be confusing to support
    \param other
      - vector to be dot multiplied memberwise with cartesian vector of this
    \return
      - dot product of cartesian members: a.x * b.x + a.y * b.y
    */
    f4 operator*(const v2f& other) const;


    /** Multiply a polar vector by another, scaling and rotating polar coords
    \brief
      - multiply a polar vector by another, scaling and rotating polar coords
    \details
      - vector multiplication isn't strictly defined, especially in cartesian
        coordinates.  in the complex plane however, the result of multiplication
        or division including the imaginary terms results in a scaling and
        rotational effect wherein angles are summed and magnitudes multiply
        together.  although strictly, this doesn't happen in the real plane,
        extending the result to polar form real numbers is simple and desirable
        enough to mimic in lieu of any other appropriate semantic.
    \param other
      - vector to scale by, and rotate by angle addition, with this polar coord
    \return
      - sum of angles and product or radii returned in a new polar vector
    */
    p2f operator*(const p2f& other) const;


    /** Multiply a polar vector by another, scaling and rotating polar coords
    \brief
      - multiply a polar vector by another, scaling and rotating polar coords
    \details
      - vector multiplication isn't strictly defined, especially in cartesian
        coordinates.  in the complex plane however, the result of multiplication
        or division including the imaginary terms results in a scaling and
        rotational effect wherein angles are summed and magnitudes multiply
        together.  although strictly, this doesn't happen in the real plane,
        extending the result to polar form real numbers is simple and desirable
        enough to mimic in lieu of any other appropriate semantic.
    \param other
      - vector to scale by, and rotate by angle addition, with this polar coord
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& operator*=(const p2f& other);


    /** Perform division on magnitude by divisor, returning the result
    \brief
      - perform division on magnitude by divisor, returning the result
    \param divisor
      - scalar amount by which to divide the length of the vector
    \return
      - resultant polar vector copy scaled down in this polar vector's length
    */
    p2f operator/(f4 divisor) const;


    /** Divide a polar vector by another, scaling and rotating the polar coords
    \brief
      - divide a polar vector by another, scaling and rotating the polar coords
    \details
      - division by a vector isn't strictly defined, especially in cartesian
        coordinates.  but, in complex coordinates, the result of multiplication
        or division including the imaginary terms results in a scaling and
        rotational effect wherein angles are summed and magnitudes multiply
        together.  although strictly, this is only in appropriating vector form
        for complex numbers <r,i> terms, and this doesn't happen over the reals,
        extending the result to polar real numbers is simple and desirable
        enough to mimic in lieu of any other appropriate semantic.
    \param other
      - vector to scale inversely by and rotate by angle subtraction
    \return
      - resultant polar vector scaled inversely and rotated by others values
    */
    p2f operator/(const p2f& other) const;


    /** Divides length of polar vector by divisor, setting the result
    \brief
      - divides length of polar vector by divisor, setting the result
    \param divisor
      - scalar amount by which to divide the length of the vector
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& operator/=(f4 divisor);


    /** Divide a polar vector by another, scaling and rotating the polar coords
    \brief
      - divide a polar vector by another, scaling and rotating the polar coords
    \details
      - division by a vector isn't strictly defined, especially in cartesian
        coordinates.  but, in complex coordinates, the result of multiplication
        or division including the imaginary terms results in a scaling and
        rotational effect wherein angles are summed and magnitudes multiply
        together.  although strictly, this is only in appropriating vector form
        for complex numbers <r,i> terms, and this doesn't happen over the reals,
        appropriating the result to polar real numbers is simple and desirable
        enough to mimic in lieu of any other appropriate semantic.
    \param other
      - vector to scale inversely by and rotate by angle subtraction
    \return
      - reference to the modified p2f for easier subsequent inline action
    */
    p2f& operator/=(const p2f& other);


  }; // end v2f union / struct / class type declaration

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                       Helper Function Declarations                       */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


  /** Left hand scalar multiplication scales all members of rhs
  \brief
    - left hand scalar multiplication scales all members of rhs
  \param scalar
    - scalar amount by which to multiply x and y values of right-hand side
  \param rhs
    - right-hand side argument to be copied and scaled in x and y by scalar
  \return
    - copy of rhs v2f's values, scaled by scalar in <x,y>
  */
  p2f operator*(f4 scalar, const p2f& rhs);


   /** Confirm if 2 p2f's are relatively equal in both length and direction
   \brief
     - confirm if 2 p2f's are relatively equal in both length and direction
   \param lhs
     - left hand side to be compared memberwise with right hand for equality
   \param rhs
     - right hand side to be compared memberwise with left hand for equality
   \return
     - true only if all members are in close proximity of one another
   */
   bool operator==(const p2f& lhs, const p2f& rhs);


   /** Confirm if 2 p2f's are not equal in either length or direction
   \brief
     - confirm if 2 p2f's are not equal in either length or direction
   \param lhs
     - left hand side to be compared memberwise with right hand for inequality
   \param rhs
     - right hand side to be compared memberwise with left hand for inequality
   \return
     - true if any members are not in close proximity of one another
   */
   bool operator!=(const p2f& lhs, const p2f& rhs);


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
   std::ostream& operator<<(std::ostream& output, const p2f& vector);


   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
   /*                            Helper Constants                             */
   /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

   //! Polar zero vector in magnitude has indeterminate direction: 0.0*v2f(1,0)
   extern const p2f& nullp2f;

} // end dp namespace
