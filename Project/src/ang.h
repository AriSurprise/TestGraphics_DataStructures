/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  ang.h
Purpose:  Float point primitive wrapper with unit handles to track conversions
Details:  Could meaningfully specialize to refactor using fixed-point data:
  cyclical data means range is better expressed in fixed scale: [-PI/2,PI/2],
  for example containing the half cycle of all unique values, but only mantissa
  bits are being useful to us. In equal precision, exponent/sign bits being used
  for perhaps signed cycle count, or something with less conflated semantic than
  just "more radians" as an abstract float may allow for more operator options
  (integer operators like bitshift being quite useful in some situations),
  preclude float-point discrepancy errors with exact steps of a fraction of arc
  within the given datum's bit depth, & expedite modulus for removing full
  revolutions to get more precise answers into a bitshift+bitmask operation with
  the intended output-float's exponent | sign bits a constant replacement.
  A sign-bit being 'useless' also suggests a domain move [-PI/2,PI/2 <->[0,PI],
  par for cos & sin domain discrepancy anyhow, but that's only half cycles,
  meaning sign & exponent bits would store those as a units field, which then
  would need bitshifting (>> 1, ie- * 1/2), to output full cycle count (lossily),
  or conversely in higher transparency, output given cycles in halves instead.
  Could use [0,TAU] instead to circumvent that, but either precision or cycles
  would be halved for that mantissa-bit moving or not (cycles/2 likely better),
  which would dictate those offsets be baked into bitmask / bitshift constants.
  Bear in mind this is either or, & adding a 24th(+1) mantissa-bit, going down
  to cycles+sign bits of 7+1 would seem simpler (being along byte boundaries),
  but does add complications in that IEEE-754 doesn't use 2's compliment, which
  also bolsters arguments for semantic of [0,PI|TAU] internal unsigned range,
  as it's then uniform (or nearly), to bitmask a float-mantissa field in place.
  Ultimately the domain of all trig functions are all real numbers, so where
  cycles begin and end is a non-issue for library calls in processing efficacy,
  but custom coordinates remain a complication to the final float-point output.
  Generally fixed point entails using q-values with a set number of bits used
  to represent the sign, integer place values, and fractional ones, under the
  general name convention of q.n#.m# for those 2 fields' bit depths.  Like
  float-point but discarding the exponent field, and usually having slight
  bitwise storage differences with concern to negative values or sentinel
  values for non-real values like Inf & NaN being stored equivalently still,
  if at all.  Data of the same magnitude is more expedient and reliable to work
  with as it falls to becoming integer math instead.  Floats store negatives in
  1's compliment unlike integers generally using 2's comp, meaning negative
  value bits are inverted to make a lot of arithmetic easier, while 1's comp
  needs more consideration how two numbers of differing signs come together
  (but it already must do that when aligning values of varying magnitude, so it
  makes more sense to leave uninverted for simplifying other operations like
  mantissa multiplication which is done in almost every operation.  Nearly all
  of these complications are removed by having no byte dedicated to signs
  however, as unsigned integer math has no need of 2's complement, field
  alignments or many such discrepancy issues.  These follow the same convention
  with the added naming convention of uq.n#.m#, where n+m sum to the bit depth
  of the field with no padding / remainder for the sign bit's prior offset.
  It also eliminates the peculiarity signed data carries of double defining a
  positive and negative 0 value, meaning increments are uniform (unlike floats)
  and continuous in the number line (unlike 1's compliment or floats), so we
  get specific fractions with given min/max values, and easier to perform & more
  reliable operations with exact results.  Either q or uq-values are restrictive
  compared to floats to this end, and while the exponent values can be tied to a
  class type to simplify tracking how a q-value's value is by automating range &
  input/output conversion, the exponent can not move quite as liberally far as
  a float point one can.  Complications arise when exponent magnitudes exceed
  the mantissa bit depth of the data, so some values aren't technically able to
  be defined with q-values as float point can due to continuity errors over the
  domain.  Consequentially, uq-values are also able to address and solve those
  shortcomings.  They are however, traditionally used across the whole data,
  where this would be a bit of a subfield. over 3 bytes instead of a full 2|4|8.
  This all speaks to [0, TAU), suggesting a uq3.21 value with [0, 8.0] range
  over the alternative [0, PI) halves which then metes to a q2.22, [0,4] range.
  Realistically, likely entailing a signed q2.21 (-PI,PI] getting a +PI offset
  unless unsigned, 3-byte q-values are implemented, which may be an format too
  specialized to merit developing compared to just primitives using powers of 2.
  Presuming accord on that much, the main issue becomes that either q-value of
  base 2 will have some remainder (excess of 2pi), as invalid data which would
  need to be bitmasked away and have modulus math done in the case of pseudo
  over/underflow, to properly track the purported rotation 'units' while
  leaving the lower bits as radians under 1 revolution.  Alternatively and
  perhaps more reliably, it would be possible to use those bits as a [-1,1)
  q-value / [0,2) uq-value, requiring float output to be multiplied by a
  constant of pi; taken in as 1/2 revolutions instead of radians (or simply
  revolutions).  This would simplify the need of any modulus math on the
  adjustment step, however slighly complicating the in/output step.  No longer
  a simple bitmask of fixed size as hoped, but better capitalizing on the
  optimization all the same.  The trade off still bodes better for the
  compressed range, as modulus entails a more computationally intensive
  division operation, while multiplication of stored revolution ratios instead
  of radians only requires the multiplication step already entailed in the
  other non-radian arc unit conversions prior, thus only moving which unit
  requires no conversion from radians to revolutions in / output (this step
  could also be mitigated by writing custom trig functions taking revolutions,
  which would stand to have some increased precision anyhow).
  If the output step will generally require multiplication by coefficients
  greater than 1, the lower bits of what would be the final output wouldn't
  store precision that could be used once in radians, so it makes sense to
  stick to a q.m.21, as the multiplication will be equivalent to a bitshift
  left by log_2(2pi) =: ~2.65149612947, meaning q.3.20 is all the final output
  precision will effectively hold, thus the slight step up of precision in only
  considering it a bitshift of 2 means math libraries will read the floats at
  full precision of the type.  Arguments could be made to scale back further
  for "preferred units" like degrees or gradians, but that would come at the
  expense of ultimate precision and likely not be preferable: floats are most
  precise when they are smallest.  The attraction in even considering those
  merits however is that every integer bit conserved allows for tracking twice
  as many revolutions across the full 4-byte field, but even at a conservative
  q.10.21 or uq.11.21, that still allows for a [-1024,1023} or a [0,2047) range
  respectively.  Either is arguably far more than is likely to have need of
  outside of astronomical bodies that may spin quickly and indefinitely.
  Besides that, neither degrees nor gradians are  a power of 2, which devalues
  the gain of change of units as different overflow (albeit at least not
  irrational), is reintroduced in that change.
  Within the use of a rotational unit of even power of 2^0, overflow is simply
  placed evenly into the integer bits  at the units place, providing the exact
  semantic desired.  Also, aside from 1's complement (if not using a uq-value),
  being slightly different over the negatives, this mean while the higher bits
  don't semantically align to a float's exponent, they do align semantically to
  what the q-value attaches to the value: an integral number of rotations and
  partial ones about the binary radix (decimal) point, which means that a more
  standard width q-value of 2|4|8 bytes with the appropriate 23 fractional bits
  are all we are storing, meaning no special width implementation need be of
  concern to implement.  Even if not n=:23 bits, it would only need a bitshift
  at most.  The beauty in using this contrivance is also in how it can extend
  to support more rotations without requiring any significant architectureal
  designs: the final precision of angle need not change for the final output,
  so if more rotations are needed, they can just be a larger m# q value in the
  same way.  This means an 8-byte angle could then hold 4.398 trillion
  (unsigned) rotations (2^{32+10}) before reaching under/overflow, and still
  support precision of 2^{-21} (approx. 0.4768 millionths of revolutions;
  2.996 millionths of radians; 171.66 millionths of degrees; 190.73 millionths
  of gradians), just as a float could, but with the consistency they rarely are
  able to deliver on and greater ease of calcualtion to adjust.
  An 8-byte datum focused on angles could have increased rotational precision
  from there, but unless astronomical distances are being needed this already
  suffices.  At this granularity, an angle as shallow as these is already only
  netting a sin value of ~7.49014056585e-7 by 1 increment, meaning ~7.49014mm
  shifted in y per 10,000km, or ~4.745753 inches per 100 miles diagonally on
  the shallowest angle definable.  Of course trig functions are non-linear, so
  not per step: others as small as 5.2963311703e-7 by pi/4, or ~8.544854572e-17
  by pi/2, meaning only ~8.54485457175367123mm in -x per Pm moved diagonally on
  that same lowest angle possible.  Likely already adequate granularity for
  most purposes, but if the 23 bits of a float aren't enough, a double's
  mantissa would hold 52(+1; 19 beyond a float's) bits which would mean a sin
  of 1.3951473992e-15 at step 1, 1.110223e-15 by pi/4, & only 2.96459196489e-34
  by pi/2, or equivalently 1-cos(step 1).
  By equivalent measure of granularity, that first increment would be shallow
  enough to mete out to ~69.75736996 cm per Pm, ~6.59955675669297783 m per
  light year, or ~3.6831891 feet per trillion miles diagonal travel of vertical
  displacement.  Despite that being a significant improvement, one may consider
  the number of light years between things in astronomic bodies & the difference
  a few meters can add up to at about 6.6 each, this is likely still far from
  precise enough for astrophysics, but it is quite literally the most precision
  standard primitives up to 64 bits can deliver on, meaning more precise results
  than that would require custom trig functions to be written as well. A pure
  uq.0.64 value simply measuring the [0,1] space could improve this further to
  ~1.7670345097099352e-41 & ~5.94480362957421e-21 for the first cos & sin pair
  respectively without requiring the jump to 128-bit pseudo-primitive types
  being defined.  The modest improvement may have some worthwhile applications,
  as it would be as precise as ~1.83437472861456513mm / 100 parsecs (again, by
  comparable measure of scaling sin(2pi*2^(-64)), but 128-bit types would still
  likely be preferable for astronomical applications where safety and travel
  are of concern (realistically, the alternative makes more sense to just be
  conscious of the architectural limitation built in whereever that granularity
  is, and plan for periodic recalculations at increasingly closer distances in
  nearing a target).  Likely, 64 bits of pure fractional precision would work
  for angles in astronomical simulations of less than intergalactic scales,
  although going intergalactic distances at very exact bearings and sleeping to
  conserve energy between (not recalibrating periodically), the offsets may be
  large enough to still be of some concern to point and aim at a target that
  small and also that far away (independent of accuracy of sensory input from
  that remote a distance to get those bearings from something and so much else).
  Of course these concerns are predicated on fixed point constraints too, and
  floating point stores data magnitudes independent of the mantissa bit depth to
  some increase in precision over specific value ranges where some assumptions
  may be made about the fixed point data's redundancy in large or small numbers.
  For example, a float's exponent has many values above e+23 (10^{23}), as well
  as ones below e-23 (10^{-23}) which can go in that field, signifying how a
  significantly more precise result can be obtained in a float-point datum than
  a fixed point one may be able to offer: the issue is both one of variable
  precision as values are adjusted in magnitude more automatically, meaning one
  won't get that precision consistently as mentioned above, but also that those
  increasingly precise values and large numbers use some generalizations about
  the data, meaning those magnitudes aren't available over the whole range of
  the number line for any given exponent field value.  In order for a float to
  store a number at a 2^{-24} with only 23 bits of mantissa precision, the
  exponent must of course be at an e-24 value, which entails a range that those
  23 bits can only describe over signed number ranges with an introduction of
  discontinuities.  That 2^{-1} place then has no representative bit in the
  mantissa, meaning there is no place for the value to be set in a 2^{-1} place,
  meaning the 1/2's value is implicitly always either on, or off (in float or
  fixed point data really).  Essentially, this is what the truncated unit place
  bit already did in less extreme magnitudes, which implies by extension the
  1/2's bit would be on instead of the units bit, which would mean the number
  range is from (-1,-0.5],[0.5,1), meaning an all 0 bit value would hold -/+0.5,
  and neither the 0 value nor anything between could be set while at that
  magnitude.  In the next step down this becomes true for the 1/4's bit being
  inplied on, the values from (-0.5,-0.25][0.25,0.5) being in a similar place,
  but closer to 0 being in range by 1 order of magnitude (in base 2), which
  would continue down to asymptoting 0 with smaller jumps around 0 until the
  smallest magnitude in the exponent was reached, when it could get no closer
  to 0 (which, incidentally, serves to explain the float's propensity to have
  arithmetic operations generally not result in 0 values exactly, but more and
  more low exponent values with some mantissa that seems like garbage to the
  average user: also not ideal behavior, but is resultant to not erasing
  mantissa bits when magnitude changes can suffice instead, and shrinking
  values rarely has cause to truncate lower bits of precision, as you're just
  adding leading 0's).  But while less than intuitive in terms of range holes,
  this extends better than the alternative to have it be implicitly off.
  Notably, this process served to rarify exact 0 values with floats, which was
  generally a desirable property for avoiding 0 division errors, so while angle
  division is rarely useful, this propensity for fixed-point to generally allow
  for more 0 values as a result of more operations should be observed.  The bit
  alternative to be off would also still insinuate this discontinuity, only
  over larger scales, the leading truncated bit still being somewhere, & it
  makes less sense to lose that bit of precision since the semantic behind the
  truncated bit still always remains true of any representation (the leading bit
  in binary must be 1, & so will be redundant to explicitly state is on): this
  then implies the units place remains on instead and the exponent field simply
  doesn't correlate to which bit is the highest set place.  The range would then
  simply be from (-1.5,-1],[1,1.5) with the same problem, increasing in scale
  while increasing in precision but over increasingly small positive magnitude
  offsets of just over 1 as the magnitude went down instead of about 0. Besides
  making less sense to store low magnitudes not asymptoting something jointly,
  it would prove completely bizarre for large numbers, as the units place would
  be on, truncated from storage, and not even the most significant one, which
  would simply mean all given values are odd, but also with increasingly large
  values not containing a 2's place, then 4's, and so forth with less
  specificity of exact units in the same manner as if the high bit shifted, but
  with 1 bit less dedicated to high magnitudes, oddly apart from the positional
  continuity semantic of the others, with no apparent gain in that partial
  specificity while otherwise generalizing groups.  Overall, it's clearly more
  simple in semantic to assume the truncated bit is on and moves: in actuality,
  the domain being symmetrically split was already always true at all scales
  anyhow, which is what itroduced the -0.0 state when the unit bit was listed
  in smaller exponent magnitudes.  It just doesn't become counter-intuitive
  until negative exponents mean the truncation moves medially, as when you move
  the radix (decimal .), point right towards the unit place, it was of little
  concern that the leading value places above the mantissa were being similarly
  truncated.  It just follows that you started when digits became relevant,
  without needing to say there are 0 quintillions, or whatever 2^n place was
  being omitted more accurately.  This does mean by extension, while the
  mantissa may not be able to hold infinitely small values, in a float, many
  more smaller degrees of precision can be described within the increasingly
  small, variable magnitudes by describing upwards of 24 or 53 binary
  significant values, down to smaller & smaller, increasingly precise values
  with the exponent holding the semantic of upwards of -127 or -2047 leading
  0's, meaning far more precise values can be described more infinitely within
  the [0,1] space where there is no larger value to overwrite those highly
  precise meanings in the data.  The range simply becomes that precise
  proportionate to how the exponent non-redundantly implies the number of
  leading 0's which a fixed point value would have to list exhaustively.  As
  the exponent moves down, the leading truncated bit implies smaller & smaller
  positions, and the mantissa the next 23 or 52 on from there.  That means that
  when at 2^{-25}, there is a state to cumulatively describe the 2^{-3} through
  2^{-25} being present, onward up to the exponent's cap of how small a
  magnitude is allowed by exponent range.  Between those states, the tandem
  information held by the fields describes most any number between [0,1] with
  twice as much precision the closer it gets to 0, for each power of 2 it comes
  less than per step of magnitude.  Due to the point of medial discontinuity,
  these aren't quite technically q-values suitable for fixed point bitfields
  operated upon as integer data is normally, as the increasingly wide rifts add
  irregularities in increments and decrements (even in median exponents, the -0
  is an anomaly to this process at the simplest case). Of interest, uq-values
  remove that concern: although the exponent may still be negative (values in 0
  to 1 generally need that).  There is no more gap in domain, or an issue with
  worrying how 1's vs. 2's complement get treated differently (as there are no
  longer any negative values for it to be any different), so integer precision
  can categorically just be bit shifted & set into a float or double with far
  fewer caveats / concerns than noting the magnitude changes (straightforward &
  much more difficult to mess up), & basic operations don't need to consider
  branching based on the signs of the arguments either as 1's complement still
  necessitates, which is of reasonable interest to consider as an alternative
  for all those reasons & more (branching is famously slow afterall...).
  Only 24 or 53 SI may be entailed in each final value, but [-0.5, 0.5] can be
  listed with 25 or 54 bits, [-0.25, 0.25] can be listed with 26 or 55
  bits of precision, [-2^{-3},+2^{-3}] with 27 or 56 bits, and so on.
  This means existing math libraries do have trig functions which get more
  precise answers, but only for those shallow angles of less & less radians,
  meaning upwards of 32 or 64 bits of precision are somewhat possible to
  describe in floats or doubles, but only with increasingly small numbers.
  What fixed point offers is uniformity of data in increments, decrements,
  overflow, underflow, scaling, etc for simpler computational costs per step,
  as well as exact values across the range and reaching only those steps, as
  discrete intervals offer that while float-point automates & convolutes that.
  This is key to utilizing higher bit depths, because trig functions have a
  large amount of symmetry in their definitions, which means that any value a
  larger bit set may describe has a way to be described in primitives in float
  point-values like so many functions expect as input.  This means that if one
  reduces the range of output of what becomes fed as input to the library calls,
  more precise results may still be available without having to resort to
  anything quite as complicated as all that extra work entailed in defining our
  own trig functions needing to be handled so early (however still necessary as
  an eventuality).  So the unit circle is symmetrical across...infinite
  subdivisions really, but how do we use that symmetry to concisely input small
  values to get that full range?
  The first few subdivisions are very easy to speak to: the first one is already
  implicit, as only the [0,pi] or [-pi/2,pi/2] range of any given trig function
  has unique results, and beyond that the definition is periodic, so the move
  back to considering half revolutions returns to relevance.  In fact only the
  first quadrant of the unit circle needs to be considered, as the results are
  comparable to interpolate alternately (0 to 1, back to 0), across quadrants
  until the cycle completes.  Even the third division is simple to mirror, as
  from [0,pi/2] sin values increase as the cos values decrease proportionately
  such that in consistant increments and subdivisions, cos(pi/2 -1 step) &
  sin(1 step) are the same, up to when cos(pi/4) & sin(pi/4) are equal, when
  the pattern reverses symmetrically, so adding signs in by magnitude, only the
  [0, pi/4] value range has unique cos or sin values, which extends to the other
  4 basics as tan is definable as a ratio or sin/cos, and so on for sec csc or
  cot.  Already this is significant as those 3 bits you save from dividing all
  the values into 8ths means that the range up to 1 discussed prior is now more
  than the pi/4 magnitude the problem is reduced to by simple measures.  This
  means there is no reason to experience a loss of precision at all converting
  from full revolutions to radians (the magnitude would actually go down by
  these measures), and the revolutions would be excellent at finding quadrants
  as simply as bitmasking since a full revolution is equal to 0 on overflow,
  but up to that point the half, quarter, etc points being passed are
  demarcated when a higher bit becomes set by virtue of working with powers of
  two to divide the domain alone.  All of these concerns are also able to be
  postponed if floats are desired, as even if an unsigned 32-bit datum were
  used to express a [0,1] ratio, all that information would easily fit in the
  mantissa of a double's 52 bits with no loss, so storing many values in only
  4 bytes could work if precision was desired above considering revolution
  tracking in a bitfield instead of a different value of a struct (arguably a
  more relevant, accessible pattern since so much of this concept deviates out
  of the efficiency posited at the outset in simply bitmasking for an in place
  operation equivalent to a float or double as typically stored).  For a double
  to make full use out of a full 64 bits of precision however, the range would
  need to be reduced considerably further.  This much would get +3 bits in trig
  precision range, so the double would be pushed into an effective 55(+1) bit
  range of sensetivity, but we would need to extort another 9 from there to get
  the full 64 bits of precision (the truncated 1 being more a constant offset
  for both / either).  This would mean working with only a range of [0,pi/2048]
  which is considerably less and certainly doesn't cover a full unit circle
  range nearly as easily, but one can take such small arcs to reach larger
  angles by extrapolating off of those results via rotations with equally
  precisely derived angles.  A 2D rotation may be performed with only a cos &
  sin value input pair, and argument pair using matrix math, which can be
  shortcut to a few multiplication and addition operations, which could be done
  iteratively on the result multiplied by PI*2^{-n} of one less each step, up
  to those 9 to 10 iterations, recovering the [0,PI/2] value range with 64 bit
  precision.  Those values could also get cached in a dynamic programming table
  of cos and sin values to make subsequent calls more economical as well (both
  the more constant PI*2^{-n} pairs and the other queried values: it could be
  the same table if memory allows that much space).  Finding an interpolation
  between several cached sin and cos value intervals with low enough margins
  of error to be equivalent within the given bit depth, it could be possible
  to subvert the need to redundantly recalculate trig value results (not sqrt
  expensive calls, but not computationally cheap ones either), plus this could
  subvert that most common geometric want of a square root call in rendering
  being required in the first place: namely the length function.  If such a
  function were found which did not need to be defined with square roots, and
  applied over a specific arc subtension to interpolate between points, that
  would make it possible to find the intersection along a line with that arc
  with algebraic equation to solve for the scalar, thus giving you your length.
  It's not even that lofty a supposition, as effectively you're just performing
  a scalar on a lerp to make the magnitude stay 1, ie in 2D for unit circle
  intersection values, given lerp values (1-t)i+(t)f for x or y, i and f being
  the (x,y) coords of the given initial and final points of interpolation,
  substituting y all we want is where 1 / sqrt(x^2+(1-x)^2) is equal to some
  function which equates to the given x being along a certain cosine or
  parabola series sum: whatever functions are even and able to normalize to the
  interval where 0=:1, 0.5=:sqrt(2), and 1=:1, with the right interpolations to
  maintain the suggested margin of error of discrepancy as stated above (below
  the bitdepth threshold for the given type's mantissa).  That much could also
  be precalculated to cover only the subtension of 1 unit of arc for the larger
  table of sin / cos values, and that could even be split up in recursive calls
  to smaller tables to create a divide and conquer system to minimize space.
  Ironically, though one would expect the sum of cosines which equates to
  length of unit circle inverse lerp scalars to be relatively graded, and be
  like a sin curve, on inspection it turns out to be much closer to a slight
  bend on an absolute value function that passes through those 3 points,
  mirrored about the x = 0.5 line than expected: between that underestimating
  and a comparable cos function overestimating by comparable measure, while a
  cos^2 function underestimates significantly more than abosulte value, but
  doesn't introduce the point of discontinuity.  Essentially, this mean the aim
  is a cos wave, but one shaped much more like a triangle wave than expected.
  Just a sum of cos & cos^2 lerped to equalize high/low distortion from the
  target values at just less than 0.3 of cos^2, just over 0.7 of cos finds
  margins of error only of about 2.471e-3: over by that much at ~0.3 & ~0.7,
  under by that much at ~0.0678 & ~0.9322.  While far more than the 1.192e-7
  to match a float (let alone a double), it is already strikingly close with
  little effort or computation needed, and cosine sum series are famously adept
  at cancelling out to produce a desired complex wave form.  Just adding cos^3
  terms into the series, the error shrinks to 6.647e-4, or about 1/e the prior
  sequence sum.  At that rate, a series of dimensions in the mid teens ought
  to cut that down adequately, but likely other increasingly higher powered
  terms could mitigate that further.
  RREF matrices of VanDerMonde rows to solve for those extrema with iteratively
  higher powers of cosine functions, it should be possible to more precisely
  asymptote the inverse length scalar coefficients desired to better replicate
  this semi-tri-wave-form with curves of fit, then it would become possible to
  ignore a great deal of architectural limitations imposed in avoiding square
  roots in many places.  Given an easy chance to find the length by lookup
  table, converting rectangular coords to polar form for vectors could even
  prove preferable (as the data separation is generally more useful).  The same
  lookup table logic could be applied to finding arccos, arcsin or arctan
  results and make the conversion as simple as a lookup table for all the
  values needed, even for spherical coordinates.  If polar form were made more
  economical and not subject to the hazard of easy basic vector addition and
  subtraction corrupting the time invested into the length calculations with
  each frame step for any moving object, having the length more easily restored
  could radically change what storage optimization heuristics are viable.  Of
  course, the same lookup table interpolation logic could somewhat be applied
  to the sqrt function itself: although non-periodic or limited in range, still
  as floats get bigger, so too does precision get mired, so greater dividends
  are, as always, likely found in optimizations there as well, and it is not
  wholly unreasonable to suppose an adequately precise approximation could be
  stored for better initial guesses to get fast square roots with fewer guess
  iterations of Newtonian or Euler approximation needed.  The same concession
  could hold for the former tables of values as well.  In fact the two could
  work well together at forming the basis for a custom library for those most
  frequently needed re-optimized trancendental functions.  Binary trees seem
  especially useful for storing either, as starting with sqrt the base values
  for a few root nodes are east to find from bitshifting to bigger integers in
  the whole of the range for the largest datum used: starting with sqrt 0, 1 &
  2 as base cases, move on through to the bit max with doubled increments from
  the last as the solution for the next step (ie 4=:2, 16=:4, 256=:16, etc, up
  to 2^64, or whatever the system supports if higher / lower than 64).  From
  there, calls to find square roots are known to be on or interim to 2 values
  over the possible value range, and can be found as medial to 2 nodes in the
  existing tree, so guessing can begin with a known general magnitude in the
  worst case, and in the best case a previous solution can be used instead. For
  each mantissa bit, we can test it from the top downward.  If a number times
  itself is greater than the queried value, 2 things are true, that number is a
  found square for a larger, found input that we can store elsewhere for future
  reference against other possible queries, and we know that bit must not be on
  in the correct answer, so we can move down to the next and repeat the process
  for each bit up to the maximum bit depth.  If the value is exactly correct,
  we have the solution and can store that, terminate and return it, while if we
  haven't exhausted the possible bit depth, and the solution is less, we know
  the solution is still valid for the squared input found, and that bit is set
  in the final solution for a certainty.  Once all bits available are checked,
  if an exact solution was not found, we at least have the closest possible
  approximation available.  Knowing all this, we know as well that the potential
  size of the tree could be massive, but finite, and the desired compromize
  could also be decided up front to instead perform this same lookup into a
  table of a fixed size to find these results up to a point of best compromise
  for the system, and then reduce the iterations required in the square root
  queries to that remaining bit depth not stored to make square roots much more
  expedient to find for those higher bit depths, while data of the lower bit
  depth would have fully pre-calculated solutions available to look up.
  root for a different input
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
#include "Value.h"      // inline angle unit conversion value constants
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                             Forward References                             */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp //! Contents for use within DigiPen MAT (& CS) class assignments
{

  //! Classification of the way an ang can spin: 1 of 2 ways, cw or ccw
  enum class winding
  {
    counterclockwise,      //! False (0) state for default math; pass-through
    ccw = counterclockwise,//! Counter-clockwise orientation of given ang
    clockwise,             //! True (1) state for exceptional behavior
    cw = clockwise         //! Clockwise orientation of given ang
  };

  //! ang unit measure names / alias types
  enum class arc
  {
    UNDEFINED = -1,   //! No valid setting input; error state
    //inference = -1, //! Ambiguous, range based, inferred results: unreliable
    deg,              //! Familiar for Degrees: 360.0 around a circle
    degs = deg,       //! Familiar plural for Degrees: 360.0 around a circle
    degree = deg,     //! 360.0 units to complete one orbit around a circle
    degrees = deg,    //! 360.0 units to complete one orbit around a circle
    D = deg,          //! Shorthand for degrees: 360.0 around a circle
    rev,              //! Familiar for Revolutions / Rotations / Spins: 1.0 per
    revs = rev,       //! Familiar for Revolutions / Rotations / Spins: 1.0 per
    revolution = rev, //! 1.0 unit to complete one orbit around a circle
    revolutions = rev,//! 1.0 unit to complete one orbit around a circle
    rotation = rev,   //! 1.0 unit to complete one orbit around a circle
    rotations = rev,  //! 1.0 unit to complete one orbit around a circle
    spin = rev,       //! Familiar for Revolutions / Rotations / Spins: 1.0 per
    spins = rev,      //! Familiar for Revolutions / Rotations / Spins: 1.0 per
    S = rev,          //! Short for Spins / Rotations / Revolutions: 1.0 around
    turn = rev,       //! Familiar for Revolutions / Rotations / Turns: 1.0 per
    turns = rev,      //! Familiar for Revolutions / Rotations / Turns: 1.0 per
    T = rev,          //! Short for Revolutions / Rotations / Turns: 1.0 around
    rad,              //! Familiar for radians: 2*&pi; units around a circle
    rads = rad,       //! Familiar for radians: 2*&pi; units around a circle
    radian = rad,     //! 2*&pi; units to go one orbit around a circle
    radians = rad,    //! 2*&pi; units to go one orbit around a circle
    R = rad,          //! Shorthand for Radians: 2*&pi; units around a circle
    grad,             //! Familiar for Gradians: 400.0 units around a circle
    grads = grad,     //! Familiar for Gradians: 400.0 units around a circle
    gradian = grad,   //! 400.0 units to complete one orbit around a circle
    gradians = grad,  //! 400.0 units to complete one orbit around a circle
    G = grad,         //! Shorthand for Gradians: 400.0 units around a circle
    MAX_COUNT         //! Bookkeeping state for count of types possible
  };

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                             Public Classes                               */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  
  class ang //! unit handling wrapper for a 4-byte, float-point rotation
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                             Constructors                               */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Make a 2D rotational angle (units must be specified along with rotation)
    \brief
      - make a 2D rotational angle (units must be specified along with rotation)
    \param rotation
      - desired rotational span (in units specified by measure)
    \param measure
      - units of rotation being expressed: arc::degrees, arc::radians, etc
    \param chirality
      - winding is counter-clockwise by default: negates rotation if clockwise
    */
    ang(f4 rotation, arc measure /* = arc::radians */, /* no default:
      the point is added accountability */ winding chirality = winding::ccw);

    
    /** Create an angle initialized from another's rotation
    \brief
      - create an angle initialized from another's rotation
    \param source
      - other angle to have it's value copied to create this angle
    */
    ang(const ang& source);


    /** Make an angle using another angle value
    \brief
      - make an angle using another angle value
    \param result
      - other angle value to be used as this angle
    */
    ang(ang&& result) noexcept;


    /** Create an angle initialized with 0 rotation (measure independent value)
    \brief
      - create an angle initialized with 0 rotation (measure independent value)
    */
    ang(void);



    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                        Public Static Methods                           */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set the units / direction with which to implicitly output angles
    \brief
      - set the units / direction with which to implicitly output angles
    \param unitMeasure
      - units of rotation being expressed: arc::degrees, arc::radians, etc
    \param chirality
      - winding is counter-clockwise by default: negates rotation if clockwise
    */
    static void ImplicitOutputDefault(arc unitMeasure = arc::radians,
      ang neutralXOffset = ang(0.0f, arc::radians),
      winding chirality = directionDefault);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Public Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set the equivalent in stored angle radians from tangent / slope ratio
    \brief
      - set the equivalent in stored angle radians from tangent / slope ratio
    \details
      - atan being a trigonometric ratio (o/a), custom input units not used
    \param slope
      - tangent / slope cartesian rise / run value equivalent to angle to store
    \param otherQuadrant
      - migrate slope forward ccw by 1/2 a rotation to express the larger angle
    \param rotations
      - store larger angle adding a given number of full rotational cycles
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    ang& atan(f4 slope, bool otherQuadrant = false, int rotations = 0);


    /** Get the stored value of the angle in degrees (counterclockwise default)
    \brief
      - get the stored value of the angle in degrees (counterclockwise default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, get negated angle
    \return
      - value in degrees, equivalent to the stored radians for the set angle
    */
    inline f4 d(winding chirality = directionDefault) const
    { return r(chirality) * DEG_PER_RAD; } // end f4 ang::d(winding) const    */


    /** Set the stored value of the angle in degrees (counterclockwise default)
    \brief
      - set the stored value of the angle in degrees (counterclockwise default)
    \param degrees
      - amount of degrees to set angle to (counterclockwise by default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, negated angle set
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& d(f4 degrees, winding chirality = directionDefault)
    {
      return r(degrees * RAD_PER_DEG, chirality);
    } // end ang& ang::g(f4, winding)                                         */


    /** Offset to change the existing angle by given degrees (ccw default)
    \brief
      - offset to change the existing angle by given degrees (ccw default)
    \param degreeOffset
      - amount of degrees to offset by (counter-clockwise by default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, negated angle set
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& dO(f4 degreeOffset, winding chirality = directionDefault)
    {
      return rO(degreeOffset * RAD_PER_DEG, chirality);
    } // end ang& ang::dO(f4, winding)                                        */


    /** Get the stored value of the angle in gradians (counterclockwise default)
    \brief
      - get the stored value of the angle in gradians (counterclockwise default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, get negated angle
    \return
      - value in gradians, equivalent to the stored radians for the set angle
    */
    inline f4 g(winding chirality = directionDefault) const
    { return r(chirality) * GRAD_PER_RAD; } // end f4 ang::g(winding) const   */


    /** Set the stored value of the angle in gradians (counterclockwise default)
    \brief
      - set the stored value of the angle in gradians (counterclockwise default)
    \param gradians
      - amount of gradians to set angle to (counter-clockwise by default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, negated angle set
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& g(f4 gradians, winding chirality = directionDefault)
    {
      return r(gradians * RAD_PER_GRAD, chirality);
    } // end ang& ang::g(f4, winding)                                         */


    /** Offset to change the existing angle by given gradians (ccw default)
    \brief
      - offset to change the existing angle by given gradians (ccw default)
    \param gradianOffset
      - amount of gradians to offset by (counter-clockwise by default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, negated angle set
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& gO(f4 gradianOffset, winding chirality = directionDefault)
    {
      return rO(gradianOffset * RAD_PER_GRAD, chirality);
    } // end ang& ang::gO(f4, winding)                                        */


    /** Get the stored value of the angle in radians (counterclockwise default)
    \brief
      - get the stored value of the angle in radians (counterclockwise default)
    \param chirality
      - if clockwise specified, get negated value from internal stored angle
    \return
      - value in radians, equivalent to the stored radians for the set angle
    */
    inline f4 r(winding chirality = directionDefault) const
    {
      // math lib radians ( 0 = cos(0) ) -> preferred radians
      f4 rads = angle + offsetRadDefault;
      return ((chirality == winding::clockwise)? -rads : rads);
    } // end f4 ang::r(winding) const                                         */


    /** Set the stored value of the angle in radians (counterclockwise default)
    \brief
      - set the stored value of the angle in radians (counterclockwise default)
    \param radians
      - amount of radians to set angle to (counter-clockwise by default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, negated angle set
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& r(f4 radians, winding chirality = directionDefault)
    {
      // preferred radians-> math lib radians ( 0=cos(0) )
      angle = (chirality == winding::clockwise? -radians : radians)
        -offsetRadDefault;
      return *this;
    } // end ang& ang::r(f4, winding)                                         */


    /** Offset to change the existing angle by given radians (ccw default)
    \brief
      - offset to change the existing angle by given radians (ccw default)
    \param radianOffset
      - amount of radians to offset by (counter-clockwise by default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, negated angle set
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& rO(f4 radianOffset, winding chirality = directionDefault)
    {
      // radians don't vary with 0 point given relative offset: only direction
      angle += chirality == winding::clockwise? -radianOffset : radianOffset;
      return *this;
    } // end ang& ang::rO(f4, winding)                                        */


    /** Get angle's math radians: stored val mapped to [-pi/2,pi/2], w/o revs
    \brief
      - get angle's math radians: stored val mapped to [-pi/2,pi/2], w/o revs
    \details
      - output radians consistent math lib radian cos(0)=0, not custom ones
    \return
      - constant reference to the stored angle in counterclockwise radians
    */
    inline f4 rM(void) const
    { return ModF(angle, PI_2, -PI_2); } // end const f4& ang::rM(void) const */


    /** Get reference of stored radians (addressable to track value externally)
    \brief
      - get reference of stored radians (addressable to track value externally)
    \details
      - value is internal, consistent math lib radian cos(0)=0, not custom
    \return
      - constant reference to the stored angle in counterclockwise radians
    */
    inline const f4& radRef(void) const
    { return angle; } // end const f4& ang::radRef(void) const                */


    /** Get the stored value of the angle in spins / revolutions (ccw default)
    \brief
      - get the stored value of the angle in spins / revolutions (ccw default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, get negated angle
    \return
      - value in spins / revs, equivalent to the radians for the set angle
    */
    inline f4 s(winding chirality = directionDefault) const
    { return r(chirality) * REV_PER_RAD; } // end f4 ang::s(winding) const    */


    /** Set the stored value of the angle in spins / revolutions (ccw default)
    \brief
      - set the stored value of the angle in spins / revolutions (ccw default)
    \param degrees
      - amount of spins / revolutions to set angle to (ccw by default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, negated angle set
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& s(f4 spins, winding chirality = directionDefault)
    {
      return r(spins * RAD_PER_REV, chirality);
    } // end ang& ang::s(f4, winding)                                         */


    /** Offset to change current value by given spins/revolutions (ccw default)
    \brief
      - offset to change current value by given spins/revolutions (ccw default)
    \param spinOffset
      - amount of radians to offset by (counter-clockwise by default)
    \param chirality
      - counter-clockwise by default: if clockwise specified, negated angle set
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& sO(f4 spinOffset, winding chirality = directionDefault)
    {
      return rO(spinOffset * RAD_PER_REV, chirality);
    } // end ang& ang::sO(f4, winding)                                        */


    /** Get the tangent / slope value equivalent to the stored angle radians
    \brief
      - get the tangent / slope value equivalent to the stored angle radians
    \details
      - tan being of mathematical definitions, non conformant to custom output
    \return
      - linear ratio of rise / run value equivalent to current angle setting
    */
    f4 tan(void) const;


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                          Operator Overloads                            */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get the angle on other side of the circle: 'subract' &pi; radians
    \brief
      - get the angle on other side of the circle: 'subract' &pi; radians
    \details
      - offset of -PI defined in Value.h would entail linking issues to inline
    \return
      - angle on the opposite side of the 2D revolution from the original value
    */
    ang operator-(void) const;


    /** Set rotation to another angle's rotation value
    \brief
      - set rotation to another angle's rotation value
    \param source
      - source angle to have it's rotation copied into this angle
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& operator=(const ang& source)
    {
      angle = source.angle;
      return *this;
    } // end ang& ang::operator=(const ang&)                                  */


    /** Set to the next equivalent angle on the opposite side: add PI
    \brief
      - set to the next equivalent angle on the opposite side: add PI
    \return
      - angle on the opposite side of the orbit (going counterclockwise)
    */
    ang& operator!(void);


    /** Add an other angle's rotation, returning the sum
    \brief
      - add an other angle's rotation, returning the sum
    \details
      - not ref param since datum wrapper likely more lite passed by value.
    \param other
      - source angle to have it's rotation summed with this angle
    \return
      - resultant sum angle of the two angles together
    */
    ang operator+(ang other) const
    {
      return ang((angle + other.angle), arc::radians);
    } // end ang ang::operator+(ang) const                                    */


    /** Add an other angle's rotation, setting the sum
    \brief
      - add an other angle's rotation, setting the sum
    \details
      - not ref param since datum wrapper likely more lite passed by value.
    \param other
      - source angle to have it's rotation summed with this angle
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& operator+=(ang other)
    {
      angle += other.angle;
      return *this;
    } // end ang& ang::operator+=(ang)                                        */


    /** Subtract an other angle's rotation, returning the difference
    \brief
      - subtract an other angle's rotation, returning the difference
    \details
      - not ref param since datum wrapper likely more lite passed by value.
    \param other
      - source angle to have it's rotation subtracted from this angle
    \return
      - resultant difference angle of the two angles together
    */
    inline ang operator-(ang other) const
    {
      return ang((angle - other.angle), arc::radians);
    } // end ang ang::operator-(ang) const                                    */


    /** Subtract an other angle's rotation, setting the difference
    \brief
      - subtract an other angle's rotation, setting the difference
    \details
      - not ref param since datum wrapper likely more lite passed by value.
    \param other
      - source angle to have it's rotation subtracted from this angle
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& operator-=(ang other)
    {
      angle -= other.angle;
      return *this;
    } // end ang& ang::operator-=(ang) const                                  */


    /** Scalar unit semantic dictates angle * primitive to preserve dimensions
    \brief
      - scalar unit semantic dictates angle * primitive to preserve dimensions
    \param scalar
      - amount of scalar growth by which to multiply the angle
    \return
      - resultant scaled product angle
    */
    inline ang operator*(f4 scalar) const
    {
      return ang((scalar * angle), arc::radians);
    } // end ang ang::operator*(f4) const                                     */


    /** Scalar unit semantic dictates angle * primitive to preserve dimensions
    \brief
      - scalar unit semantic dictates angle * primitive to preserve dimensions
    \param scalar
      - amount of scalar growth by which to multiply this angle
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    inline ang& operator*=(f4 scalar)
    {
      angle *= scalar;
      return *this;
    } // end ang& ang::operator*=(f4)                                         */


    /** Get signed scalar multiplicity of times one angle fits into another
    \brief
      - get signed scalar multiplicity of times one angle fits into another
    \details
      - quotient semantic dictates (angle / angle) purges arc units, hence f4
      Not reference param since datum wrapper likely more lite passed by value.
    \param arc
      - subsension span by which to divide the existing angle into segments
    \return
      - number of arc spans which fit into the original angle measure
    */
    f4 operator/(ang arc) const;


    /** Get signed scalar inverse of multiplicity (quotient), of an angle
    \brief
      - get signed scalar inverse of multiplicity (quotient), of an angle
    \details
      - arc unit quotient semantic dictates angle/primitive preserves units
    \param divisor
      - quotient by which this angle is to have it's rotation divided
    \return
      - resultant, down-scaled quotient of the original angle
    */
    ang operator/(f4 divisor) const;


    /** Set signed scalar inverse of multiplicity (quotient), of an angle
    \brief
      - set signed scalar inverse of multiplicity (quotient), of an angle
    \details
      - arc unit quotient semantic dictates angle/primitive preserves units
    \param divisor
      - quotient this angle is to have it's rotation divided by this divisor
    \return
      - reference to the modified angle for easier subsequent inline action
    */
    ang& operator/=(f4 divisor);

  private:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Methods                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                           Private Members                              */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    //! Arc measure to use for universal angle output units (radians by default)
    static arc unitDefault;

    //! Default direction output of angle winding (counter-clockwise by default)
    static winding directionDefault;

    // Offset to 0 point if mathematical cos(0) is not the desired default
    static f4 offsetRadDefault; // should affect input + output: in a refactor

    //! Amount of rotation, stored in radians in ccw winding for math libraries
    f4 angle;

  }; // end ang class 



  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                        Helper Operator Overloads                         */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /** Comparison of left angle being strictly greater than than the right side
  \brief
    - comparison of left angle being strictly greater than than the right side
  \details
    - angles being cyclic, can measure displacement, but raw is useful as well.
    Were the displacement wrapping handled internally, this would both inflate
    the processing demand & remove the potential to compare multiple rotations.
    Consequentially, wrapped comparisons may be handled in mother methods.
    Not reference params since datum wrapper likely more lite to pass by value.
  \param lhs
    - left-hand side of comparison lexically relative to comparison operator
  \param rhs
    - right-hand side of comparison lexically relative to comparison operator
  \return
    - true if lhs angle is a higher value in the ccw direction than the rhs
  */
  inline bool operator>(ang lhs, ang rhs)
  { return (lhs.radRef() > rhs.radRef()); } // end bool operator>(ang, ang)   */


  /** Comparison operator of left angle being greater than than the right side
  \brief
    - comparison operator of left angle being greater than than the right side
  \details
    - angles being cyclic, can measure displacement, but raw is useful as well.
    Were the displacement wrapping handled internally, this would both inflate
    the processing demand & remove the potential to compare multiple rotations.
    Consequentially, wrapped comparisons may be handled in mother methods.
    Not reference params since datum wrapper likely more lite to pass by value.
  \param lhs
    - left-hand side of comparison lexically relative to comparison operator
  \param rhs
    - right-hand side of comparison lexically relative to comparison operator
  \return
    - true if lhs angle is a higher or equal value in the ccw direction than rhs
  */
  inline bool operator>= (ang lhs, ang rhs)
  { return (lhs.radRef() >= rhs.radRef()); } // end bool operator>=(ang, ang) */


  /** Comparison of left angle being strictly less than than the right side
  \brief
    - comparison of left angle being strictly less than than the right side
  \details
    - angles being cyclic, can measure displacement, but raw is useful as well.
    Were the displacement wrapping handled internally, this would both inflate
    the processing demand & remove the potential to compare multiple rotations.
    Consequentially, wrapped comparisons may be handled in mother methods.
    Not reference params since datum wrapper likely more lite to pass by value.
  \param lhs
    - left-hand side of comparison lexically relative to comparison operator
  \param rhs
    - right-hand side of comparison lexically relative to comparison operator
  \return
    - true if rhs angle is a lesser value in the ccw direction than the lhs
  */
  inline bool operator<(ang lhs, ang rhs)
  { return (lhs.radRef() < rhs.radRef()); } // end bool operator<(ang, ang)   */


  /** Comparison operator of left angle being less than than the right
  \brief
    - comparison operator of left angle being less than than the right
  \details
    - angles being cyclic, can measure displacement, but raw is useful as well.
    Were the displacement wrapping handled internally, this would both inflate
    the processing demand & remove the potential to compare multiple rotations.
    Consequentially, wrapped comparisons may be handled in mother methods.
    Not reference params since datum wrapper likely more lite to pass by value.
  \param lhs
    - left-hand side of comparison lexically relative to comparison operator
  \param rhs
    - right-hand side of comparison lexically relative to comparison operator
  \return
    - true if lhs angle is a lesser or equal value in the ccw direction than rhs
  */
  inline bool operator<=(ang lhs, ang rhs)
  { return (lhs.radRef() <= rhs.radRef()); } // end bool operator<=(ang, ang) */


  /** Comparison operator of left angle being relatively equal to the right
  \brief
    - comparison operator of left angle being relatively equal to the right

  \details
    - angles are cyclic and can measure displacement, but raw is useful
    as well. Were the displacement wrapping handled internally, this would both
    inflate the processing demand and remove the potential to compare multiple
    rotations. Consequentially, wrapped comparisons may be handled externally.
    Not reference params since datum wrapper likely more lite to pass by value.
  \param lhs
    - left-hand side of comparison lexically relative to comparison operator
  \param rhs
    - right-hand side of comparison lexically relative to comparison operator
  \return
    - true if lhs angle is relatively equal value to the rhs
  */
  bool operator==(ang lhs, ang rhs);


  /** Comparison operator of left angle being inequal to the right
  \brief
    - comparison operator of left angle being inequal to the right
  \details
    - angles are cyclic and can measure displacement, but raw is useful
    as well. Were the displacement wrapping handled internally, this would both
    inflate the processing demand and remove the potential to compare multiple
    rotations. Consequentially, wrapped comparisons may be handled externally.
    Not reference params since datum wrapper likely more lite to pass by value.
  \param lhs
    - left-hand side of comparison lexically relative to comparison operator
  \param rhs
    - right-hand side of comparison lexically relative to comparison operator
  \return
    - true if lhs angle is inequal value to the rhs
  */
  bool operator!=(ang lhs, ang rhs);


  /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^*/
  /*                           Helper Constants                               */
  /*^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^*/

   //! Zero-angle aligned to no arc of displacement
  static const ang nullang = ang(0.0f, arc::radians);



} // end dp namespace
