/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:  vSpace.h
Purpose:  Enums for describing features of a given vector space
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no platform requirements)
Project:  a.Surprise_CS300_1
Author:   Ari Surprise, a.surprise, 40000218
Creation date:  Spring 2021
End Header --------------------------------------------------------*/
#pragma once

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// "./src/..."
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within class assignments
{
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Consts                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    
    /* Coordinate Subdivision Ordering Explorations
    Octant ordering of 3D coordinate spaces & up aren't standardized.  Given
    each axis is divided in 2 for +/- halves, for any d there are 2^{d} subdivs
    (i.e. quad -> oct -> hexadectants, etc).  While it is conventional that
    most agree any vector space has the first division contain the all positive
    half space per axis, 2D has a convention other dimensions don't share.
    In 2D, ordering typically becomes conformant to a rotation over any probes,
    meaning instead of sequentially and exhaustively flipping each possible
    axis from lowest to highest (i.e. the most distally added dimension last),
    all quadrants listed are adjacent sequentially to their neighboring space.
    This entails that only one axis flip occurs from 1 to the next.  While in
    2D, there are only 2 ways this may occur, making a perfect match for the 2
    sequential neighbors a 1D value may have as a list or a number line, in 3D
    and upwards, many more possibilities open up supporting this concept, but
    thus not exhaustively upon making that decision of which, and instead of
    fussing over which one is best entailed at what point, several choices with
    added woes of rifts when some spaces reach a dead end without new neighbors
    before all unvisited divisions are exhausted unless planned carefully, or
    baselessly suggesting one is order may be more correct than others, instead
    it is conceded that all other spaces ought generally to conform to read
    from bitwise logic.  That is, since [0] is quadrant 1, and has positive
    coordinates in all axes, for which there are 2^{d} subdivisions, a pattern
    is used with each bit representing an axis for the chosen axis ordering,
    usually a low bit being the most supplementary axis of the space, where not
    being set implies it correlates to that axis' positive coordinate space
    half, and any set bit represents it flips to its negative space half.
    However many bits are flipped, the maxima of 2^{d}-1 having all negative
    spaces, and the minima being 0, a range of 2^{d} is exhaustively explored.
    Unlike 2D, this splits regions into a zig-zag patterning when ordered, no
    relationship to neighbor distance being considered. Combinatorically, more
    dimensions get many more potential orderings, but the chosen ordering can
    create interesting rotational properties, and be consistent across all
    lower dimensions as well.  There are however many ways to choose ones which
    do not run out of options, breaking the rotation partway through.  So the
    question is how do we reliably ensure a higher dimension explores all values
    without doing this by some algorithmic heuristic.  A marginally adequate
    solution which considers what contributes to that breaking point as the
    primary selection mechanism.

    Addressing the underlying possibility space of it, the ordering adheres to
    a pattern in which values are taken as subsets of [0,2^{d}-1] by the number
    of bits those values entail setting in binary: a pascal value row sized
    group of value groups per subset.  A step in the chosen heuristic, working
    up to 4D & sufficing to explain the variables in play, must first consider
    whether a bit will be turned on or off.  This can be determined by
    traversal from the nCr(d,0) of the initial value, given that all axes being
    positive means no bits are set, therefore the 1 way to choose 0 from among
    d choices will be selected first, decrementing that remaining pool to 0.
    Moving to values with 1 bit set after the chosen initial value of 0, the
    general process can begin.  From those values for the number of set bits,
    discounting any used to enumerate a prior division, those remaining values
    are separated into those which do only flip one bit from the current round
    of the inspection also being discluded.  Looking at the pascal row size
    remaining for the neighboring selections, it must be decided if a bit will
    flip on or off from among these values: while lesser valued pools are
    greater than 1 in list size, it must be selected from among those that are
    available and fit, which will ping-pong for a time in larger d spaces
    between 1 and 2, then proceed longer upwards.  Each value must therefore be
    selected from that number of bits (1 up or down), from the current value,
    selecting from among those that fit the current bit pattern & haven't been
    selected to include only those remaining as viable exploratory moves.
    In 5D, this is where a dead-end will crop up mid-way, likely the context
    being inadequately considered may be in looking one step further ahead,
    weighing from among choices taken to prioritize the ones which will have
    fewer branch options remaining first, preventing small pockets of
    unexplored half-spaces from forming early on.  This much adheres to a
    rotation in lower dimensions regardless, only changing the order which
    would be given in 2D standards, except that it does also switch 2D's chosen
    mathematical orientation to be a clockwise definition instead of ccw.
    
    Although clockwise 2D quadrant ordering may not seem ideal in some regards
    (& consequentially not even carried over symmetrically to 2D coords here,
    as the added time of refactoring trig. functions to consistently work in
    that system change are easily out of scope for the assigned project), this
    may yet be preferable in many regards, despite the controversy.  Our usage
    of counter-clockwise coordinates in math and sciences is often a blocking
    point for initial students' induction to the fields.  Rotation is commonly
    only observed naturally in water, clocks and the rotation of the earth,
    where the chirality is usually only observably eponymously 'clockwise'
    (ccw view of earth going east to west being relative to which pole deemws
    as a 'true' vantage point), stemming from sundials and how we rotate about
    it, the natural precession of a clock more typically makes sense to most
    new audiences.  Convincing students to accept the reversal of the familiar
    convention can be harder for some than others, and like learning to read in
    other number bases, can seem unnecessary/convoluted. Besides conventional
    familiarity or aesthetics, on a numeric modeling level counter-clockwise
    rotation also entails the next bit over from the LSB as an initial flip.
    Less of an issue in 2D where perpetuating that flip the other way simply
    uses the quadrant's MSB instead of LSB, but a medial bit from within a
    dimensional promotion would create steeper and more erratic value jumps,
    also less parallel to dimensions proceeding upwards.  The lowest bits
    changing first means the value jumps are more recognizably tangential,
    where the reverse sees less regular, proximal clusters.  However, clockwise
    rotation ordering in higher dimensions carries a uniform motion which spans
    all the subdivisions of the space, producing interesting parallels to 2D
    rotations, not only mimicking it but showing geometric relationships too.
    
    Using the same initial point and order as other conventions (+++), unless
    bitwise managment is expressly more desired, this convention changes few
    things to create any incongruencies with other ordering heuristics. In 3D,
    the order that unfolds is (by decimal & binary value given), 0:000, 1:001,
    3:011, 2:010, 6:110, 7:111, 5:101, 4:100.  Values are almost linear, but
    the interesting part is that when (as conventional), taken bitwise as 0
    =: (1) and 1 =: (-1), while an undefined, lower dimension emplaces a 0
    value for the missing axes, the rotation in the Z plane which 2D entails
    expands to the sampled points in the higher 3D space being representative
    of an unnormalized, 3D square-wave form, wrapped along the 2D rotation.
    Starting from the +X YZ half-space, viewed externally, this is to say a
    clockwise rotation begins through the positive quadrants of YZ, until 3
    octant (0>1>3>2) traversals beget 4 values, coming adjacent to completion
    in 2D by a return to 0, progressing instead to continue to the only new
    adjacent direction means the rotation flips to be counter-clockwise across
    the -Y half of XZ space (starting from the same relative upper-right
    quadrant of it), then after an additional 2 traversals (#2>6>7) before
    adjacent to where the first 2D rotation swept through already (#3; Oct5),
    finally going back through the -X half of YZ instead resumes clockwise
    rotation by exclusion of unswept regions until having exhausively spanned
    unvisited octants after another 2 traversals (#7>5>4), the same clockwise
    rotation fully traces all octants ending adjacent to the initial octant,
    which can be returned to by a comparable counter-clockwise traversal in YZ
    from -X to +X, unsetting the MSB to complete the cycle of periodicty
    spanning 2 square wave periods.  The direction reversal of apparent 2D
    rotation (viewed externally), is interesting in that it not only describes
    the wave form's switch to it's concave half, or how linear algebra changes
    axis subscript patterns of a matrix's propensity to flip signs in odd
    numbered subscripts, creating the checkering pattern many students often
    forget & adding semantic rationale to intuitively explain why this may need
    to happen, but also simply in reflecting the natural phenomena of light
    traveling simultaneously in both lines and waves to create an aesthetic
    preference.  It is also reasonable to think that spanning all space
    contiguously in one non-linear, circuitous axis of measure is apt to
    produce a polar coordinate system of more complete rotations using fewer
    dimensions to describe it.  The normal of travel along the interpolation
    needing to be a factor to adequately describe the expanded space, the
    period of that cosine circuit could then describe 2 axes of rotation in a
    plane with one value, a third axis sweep of a sine wave able to stabilize
    by reciprocal offset when parallel, get consistent tilt by phase offset,
    produce wobbling when held static (implied friction force), etc.

    Insofar as the derivation breaking for creating this effect at 5D, the
    recurrence relation is far simpler to effectively consider by the framing
    of the above description.  The 2D case as a base simply takes on 4 states,
    the initial state being a given at first from the outset contition of being
    in the ++ half of each, changing 1 bit at a time to either rotate cw/ccw,
    based on which bit flips first, and which was done over the last 2D cycle,
    thus flipping whether traversal is cw or ccw as observed to be a preferable
    result. Once all 4 positions are exhausted, the next bit up must be flipped
    to begin a comparable process in higher dimension pairs one step per
    iteration of the lower dimension pair at a time.  Setting the higher bit
    will offset the cycle of the next iteration by taking a step to set the bit
    elsewhere, and in so doing, create an opportunity to make the total set
    bits flip such that the end point bridges to have all but one bit set in
    the whole of the axes.  In doing this process, each dimension added will
    serve as a convolution of the space of the one below it, and produce this
    circuit as a result.  On clarification of one last vagueity, the 4D example
    from the subset based heuristic illustrates that while the rotation will
    flip axis pairs of bit values direction in 2D, it may not need to flip
    which bit of the pair gets flipped first as more axes are added.  It
    appears to be sufficient to simply extend from the lowest bit to change
    the next most significant bit:                1: +  + +     01: +|+  + +
    In so doing, the process also illustrates     2: +  + -     02: +|+  + -
    how much more difficult it would become apt   3: +  - -     03: +|+  - -
    to be, insisting upon flipping the MSB first  4: +  - +  -> 04: +|+  - +
    to adhere to CCW rotations.   The added axes
    appear to operate well as supersets to the    5: -  - +     05: +|-  - +
    child...'reference frame' of sorts, always    6: -  - -     06: +|-  - -
    adding the lesser to the parent axis added    7: -  + -     07: +|-  + -
    to the MSB, which gets flipped last,          8: -  + +     08: +|-__+_+
    surrogating what was the primary axis in the
    frame of the additional rotation's added convolution.       09: - -  + +
    The notable exception being the added dimension flips       10: - -  - +
    which of the directions the odd 2D rotations proceed in,    11: - -  - -
    in order to conclude at a comparable bit range for near     12: - -  + -
    symmetry by the concluding step, & creating more symmetry
    across steps in the overall process.  This may also bear    13: - +  + -
    relevance contextualizing the idea of cos / sin functions   14: - +  - -
    getting used as archetypical examples for the respective,   15: - +  - +
    categorical differences in even / odd functions' behavior.  16: - +  + +
 
    This order thusly insinuates 2D rotation functions act as linear traversal
    of a collapsed wave form from the higher dimension, collapsing periodicity
    in the added direction to an averaged line, projected down to the lower
    dimensional space.  Although harder to visualize in 4D, this pattern
    continues over 4D subsets, projected into 3D, emerging with the equivalent
    3D patterns projecting into 2D.  In 5D, the convolutions of the space
    subsets become sufficiently nuanced that achieving the proper selectivity
    of this doesn't obey the same pattern of selecting the highest valued
    subsets, but the recurrence relationship does hold.  This research being an
    aside, well outside the scope or materials of the framework, looking deeper
    into how to select the ordering which achieves this by an algorithmic
    pattern clearly isn't suitable here (deemed unfortunate or not).  It is
    safe to say however that in searching for an ordering which makes more
    consistent sense than a break from 2D one, this one comes far closer to a
    more 'correct' way of approaching it than bitwise counting.  the properties
    of patterns with neighbors have a consistant offsets, meaning spans across
    contiguous space share subscript offsets (at least if viewed cyclically,
    under modulus math; still most conformant regardless), the amount of offset
    from one to the next under this sequential ordering being suggestive of a
    neighbor to that subdivision is a clear advantage to rotational order.
    however the degree of difference / offset will still carry jumps between
    certain values proportionate to how many dividing axes divide the space, as
    these jumps can occur proportionately / represenative of shortcut traversal
    of the whole spanning circuit created by some factor of 2, per dividing
    axis breached.  The below 3D coodinate spans demonstrating how spanning X's
    halves is being split by rotation in y and z's halves, they can have any
    even value between the neighbors, while z with no axes dividing the space
    sequentially probes to neighbors with no jumps, and y pingpongs medially
    to the two with linear probes in the middle when iterating about yz, and
    more offsets the farthur out the iteration moves from the median.  the same
    logic applies to x's offsets, the smaller gaps being seen hte more the mid
    point of traversal is reached, & the latter halves being reached to bridge
    back to the first octants equally late in the circuit.
    */

    //! 3D coordinate space subset; coordinates spanning the given octants
    enum class C3D
    {
      UNDEFINED = -1, //! Coordinate space unused / undefined
      OCT0,           //! Contained in <x,y,z> - + + octant for defined origin
      OCT1,           //! Contained in <x,y,z> + + + octant for defined origin
      OCT2,           //! Contained in <x,y,z> + + - octant for defined origin
      OCT3,           //! Contained in <x,y,z> + - - octant for defined origin
      OCT4,           //! Contained in <x,y,z> + - + octant for defined origin
      OCT5,           //! Contained in <x,y,z> - - + octant for defined origin
      OCT6,           //! Contained in <x,y,z> - - - octant for defined origin
      OCT7,           //! Contained in <x,y,z> - + - octant for defined origin
      OCT8 = OCT0,    //! Octant 0=8 <x, y, z> - + + equivalent without modulo

      X18,            //! Spanning X, + + <y,z> contained for a defined origin
      X27,            //! Spanning X, + - <y,z> contained for a defined origin
      X36,            //! Spanning X, - - <y,z> contained for a defined origin
      X45,            //! Spanning X, - + <y,z> contained for a defined origin

      Y14,            //! Spans + Y + but contained in <x,z> of defined origin
      Y23,            //! Spans + Y - but contained in <x,z> of defined origin
      Y67,            //! Spans - Y - but contained in <x,z> of defined origin
      Y58,            //! Spans - Y + but contained in <x,z> of defined origin

      Z12,            //! Contained in <x,y> + +, Z spanning of defined origin
      Z34,            //! Contained in <x,y> + -, Z spanning of defined origin
      Z56,            //! Contained in <x,y> - -, Z spanning of defined origin
      Z78,            //! Contained in <x,y> - +, Z spanning of defined origin

      XY1458,         //! Spans over XY but + z contained for a defined origin
      XY2367,         //! Spans over XY but - z contained for a defined origin

      XZ1278,         //! Spans over XZ but + y contained for a defined origin
      XZ3456,         //! Spans over XZ but - y contained for a defined origin

      YZ1234,         //! Spans over YZ but + x contained for a defined origin
      YZ5678,         //! Spans over YZ but - x contained for a defined origin

      MEDIAL,         //! Spanning all octants of the subset's defined origin 
      MAX_COUNT       //! Total count of coordinate space definitions

    };  // end C3D enum


    //! 2D coordinate space; origin location type of a 2D space
    enum class COORD2D
    {
      UNDEFINED = -1, //! Coordinate space unused / undefined
      CENTER_UP,      //! Origin in window center; +y up, +x right
      NDC= CENTER_UP, //! Normalized Device Coordinates; +y up, +x right
      QUADRANT1,      //! Origin in window lower left; +y up, +x right
      QUADRANT2,      //! Origin in window lower right; +y up, +x left
      QUADRANT3,      //! Origin in window top right; +y down, +x left
      QUADRANT4,      //! Origin in window top left; +y down, +x right
      MAX_COUNT       //! Total count of coordinate space definitions

    };  // end COORD2D enum


    //! Cartesian 2D basis subset plane of concern from a 3D vector space
    enum class PLANE
    {
      UNDEFINED = -1, //! Invalid / undefined plane value
      XY,             //! Front viewpoint of right handed basis, from +Z
      XZ,             //! Top-down viewpoint of right handed basis, from +Y
      YX,             //! Back viewpoint of right handed basis, from -Z
      YZ,             //! Frustum-left view in right handed basis, from +X
      ZX,             //! Bottom-up viewpoint of right handed basis, from -Y
      ZY,             //! Frustum-right view in right handed basis, from -X
      MAX_COUNT       //! Total count of coordinate space definitions

    };  // end PLANE enum


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Public Structs                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Public Constants                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

} // end dp namespace
