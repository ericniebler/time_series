///////////////////////////////////////////////////////////////////////////////
/// \file shift.hpp
/// Defined the mathematical operator s on time series
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_SHIFT_EAN_06_05_2006
#define BOOST_TIME_SERIES_NUMERIC_SHIFT_EAN_06_05_2006

#include <boost/concept_check.hpp>
#include <boost/concept/requires.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/shifted_series.hpp>

namespace boost { namespace time_series
{

    /// \brief Shifts a \c TimeSeries by the specified offset.
    ///
    /// Shifts a \c TimeSeries by the specified offset. The offset can be positive,
    ///     which shifts the series right, or negative which shifts it left.
    ///
    /// \param series The series to shift.
    /// \param offset The offset by which the series is to be shifted. 
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre \c Offset is convertible to the \c offset_type of the input series.
    /// \return A <tt>shifted_series\<\></tt> view that models \c TimeSeries.
    /// \attention The complexity is O(1).
    template<typename Series, typename Offset>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series>))
        ((Convertible<Offset, typename concepts::TimeSeries<Series>::offset_type>)),
    (shifted_series<Series> const))
    shift(Series &series, Offset offset)
    {
        return shifted_series<Series>(series, offset);
    }

    /// \overload
    ///
    template<typename Series, typename Offset>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((Convertible<Offset, typename concepts::TimeSeries<Series const>::offset_type>)),
    (shifted_series<Series const> const))
    shift(Series const &series, Offset offset)
    {
        return shifted_series<Series const>(series, offset);
    }

}}

#endif // BOOST_TIME_SERIES_NUMERIC_SHIFT_EAN_06_05_2006
