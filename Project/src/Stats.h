/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#
File Name:  Stats.h
Purpose:  Structs / functions to extract generalizations from larger data sets
Project:  a.Surprise_CS350_2
Author:   Ari Surprise, a.surprise, 40000218
Language: C++ / Eng, MSVC (used, but should work with most compilers)
Platform: MSVC / Windows 10 (used, but should have no requirements)
Creation date:  Summer 2022
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
#^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
#ifndef STATISTICAL_DATA_ALANYSIS_H
#define STATISTICAL_DATA_ANALYSIS_H

/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

// <stl>
// "./src/..."
#include "TypeErrata.h" // Platform primitive type aliases for easier porting


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                                  Includes                                  */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

namespace dp
{
  union v2f; // probable for min/max definition
}


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                              Public Constants                              */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

//constexpr f8 exmpl = 1.0; //! =: 

/*enum class Types //! =:
{
  Listed, //! Example description
  Max     //! Bookkeeping state of all type listings
}; // end Endian enum                                                         */


/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
/*                         Public Classes / Structs                           */
/*^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/


namespace dp
{

  //! Container of settings by which one may iterate regularly over data
  class uRng
  {
  public:

    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                            Constructors                                */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Sampling context of data iteration created by sample count, start & step
    \brief
      - sampling context of data iteration created by sample count, start & step
    \param samples
      - maximum sample count to be read: iterations to be taken by a scan
    \param size
      - total dataset size (samples <= size), for maximum defined data points
    \param first
      - offset [0,total) for first sample to be an offset this far in data list
    \param steps
      - intersample step [-size/2,size/2): next data point [steps] from prior
    */
    uRng(u4 samples = u4_MAX, u4 size = u4_MAX, u2 first = 0u, s2 steps = 1);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                               Getters                                  */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Get initial data point subscript from which a dataset scan begins
    \brief
      - get initial data point subscript from which a dataset scan begins
    \return
      - initial data point index from which a dataset scan begins iteration
    */
    inline u2 First(void) const
    { return initS; } // end u2 First(void) const                             */


    /** Get completed iterations of point samples over the dataset scan
    \brief
      - get completed iterations of point samples over the dataset scan
    \return
      - current number of completed iterations within the current scan
    */
    inline u4 Sampled(void) const
    { return sampled; } // end u4 Iter(void) const                            */


    /** Get the subscript for the current data point within the dataset scan
    \brief
      - get the subscript for the current data point within the dataset scan
    \return
      - current subscript for the active data point within the current scan
    */
    inline u4 Index(void) const
    { return current; } // end u4 Index(void) const                           */


    /** Get the total dataset points defined as in range which may be sampled
    \brief
      - get the total dataset points defined as in range which may be sampled
    \return
      - total dataset points defined in range from which points may be sampled
    */
    inline u4 Dataset(void) const
    { return total; } // end u4 Max(void) const                               */


    /** Get total data points to be sampled over the course of a dataset scan
    \brief
      - get total data points to be sampled over the course of a dataset scan
    \return
      - number of data points requisite for a full dataset scan
    */
    inline u4 Samples(void) const
    { return samples; } // end u4 Samples(void) const                         */


    /** Get total data points to be sampled over the course of a dataset scan
    \brief
      - get total data points to be sampled over the course of a dataset scan
    \return
      - true if there are sample points yet to be scanned (Iter() < Samples())
    */
    inline bool Scanning(void)
    { return sampled < samples; } // end bool Scanning(void) const            */


    /** Get inter-sample interval of subscripts from one data point to the next
    \brief
      - get inter-sample interval of subscripts from one data point to the next
    \return
      - offset / increment to iterate over dataset from one sample to the next
    */
    inline s2 Step(void) const
    { return step; } // end s2 Step(void) const                               */


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                              Setters                                   */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Reset data iteration context by dataset / sample counts
    \brief
      - reset data iteration context by dataset / sample counts
    \param datasetSize
      - data sample total count defined to be sampled: datalist.size()
    \param samples
      - maximum sample count: iterations to be taken by a scan
    \param reset
      - if true set initial sample point iteration
    \return
      - reference to the modified Sampling for easier subsequent inline action
    */
    inline uRng& Reset(void)
    {
      sampled = 0;
      current = initS;
      return *this;

    } // end Smpl& Reset(void)                                                */


    /** Reset data iteration to default settings (iteration =: 0; index = init)
    \brief
      - reset data iteration context by dataset / sample counts
    \param datasetSize
      - data sample total count defined to be sampled: datalist.size()
    \param samples
      - maximum sample count: iterations to be taken by a scan
    \param reset
      - if true set initial sample point iteration
    \return
      - reference to the modified Sampling for easier subsequent inline action
    */
    uRng& SetMax(u4 datasetSize, u4 sampleSize = u4_INVALID, bool reset = true);


    /** Set iterative sampling distance, i.e. increments between samples
    \brief
      - set iterative sampling distance, i.e. increments between samples
    \details
      - ignored if sampling all data; input offsets when making cyclic samples
      still possible to take composite samples that sample cyclicly by primes
      i.e. with 12 data, sampling by 6 would be barred, but 3 still valid
      (is an undesirable result: needs fix after a prime seive is implemented)
    \param increment
      - amount to set current sample to increment by at each sample point read
    \return
      - reference to the modified Sampling for easier subsequent inline action
    */
    uRng& SetStep(s2 increment);


    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
    /*                         Operator Overloads                             */
    /*^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

    /** Set data iteration context from another set of context
    \brief
      - set data iteration context from another set of context
    \param other
      - settings of how / where to iterate over a data set to be copied
    \return
      - reference to the modified Sampling for easier subsequent inline action
    */
    uRng& operator++(void);


    /** Set data iteration context from another set of context
    \brief
      - set data iteration context from another set of context
    \param other
      - settings of how / where to iterate over a data set to be copied
    \return
      - reference to the modified Sampling for easier subsequent inline action
    */
    uRng& operator=(const uRng& other);


  private:

    u4 samples; //! Total samples to be read in dataset before sampling ends
    u2 initS;   //! First sample to be read from data set at sampling start
    s2 step;    //! Step from a sample to the next data point to be read
    u4 sampled; //! Current sample iteration from [0,count) (current could loop)
    u4 current; //! Current sample subscript in data set being scanned
    u4 total;   //! Samples defined in data set currently being scanned

  };

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                        Public Static Variables                           */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                      Public Function Declarations                        */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/
  /*                 Inline / Template Function Definitions                   */
  /*^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^^#^^^^^^^^*/

/** Get the output of input value
\brief
  - get the output of input value
\param value
  - input value to be output
\return
  - output value
*/
/*inline s4 Out(s4 value)
{ return value; } // end s4 Out(s4)                                         */

}

#endif
