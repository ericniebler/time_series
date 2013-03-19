///////////////////////////////////////////////////////////////////////////////
/// \file clip.hpp
/// Defined the mathematical operator s on time series
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_CLIP_EAN_07_07_2006
#define BOOST_TIME_SERIES_NUMERIC_CLIP_EAN_07_07_2006

#include <boost/concept_check.hpp>
#include <boost/concept/requires.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/clipped_series.hpp>

namespace boost { namespace time_series
{

    /// \brief Clips a \c TimeSeries to a [start,stop) range.
    ///
    /// Clips a \c TimeSeries to a [start,stop) range, where the resulting series view
    /// has zeros outside of the specified range.
    ///
    /// \param series The series to clip
    /// \param start The start offset of the clipped range, inclusive.
    /// \param stop The stop offset of the clipped range, exclusive.
    /// \pre The input series is a model of the \c TimeSeries concept.
    /// \pre \c Offset is convertible to the \c offset_type of the input series.
    /// \return A <tt>clipped_series\<\></tt> view that models \c TimeSeries as
    ///         well as \c Range.
    /// \attention The complexity is O(log N), where N is the number
    ///         of runs in the input series.
    template<typename Series, typename Start, typename Stop>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series>))
        ((Convertible<Start, typename concepts::TimeSeries<Series>::offset_type>))
        ((Convertible<Stop, typename concepts::TimeSeries<Series>::offset_type>)),
    (clipped_series<Series> const))
    clip(Series &series, Start start, Stop stop)
    {
        return clipped_series<Series>(series, start, stop);
    }

    /// \overload
    ///
    template<typename Series, typename Start, typename Stop>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((Convertible<Start, typename concepts::TimeSeries<Series const>::offset_type>))
        ((Convertible<Stop, typename concepts::TimeSeries<Series const>::offset_type>)),
    (clipped_series<Series const> const))
    clip(Series const &series, Start start, Stop stop)
    {
        return clipped_series<Series const>(series, start, stop);
    }

}}

#endif // BOOST_TIME_SERIES_NUMERIC_CLIP_EAN_07_07_2006
