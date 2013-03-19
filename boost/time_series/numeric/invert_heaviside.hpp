///////////////////////////////////////////////////////////////////////////////
/// \file invert_heaviside.hpp
/// From a Heaviside series, construct the inverse Heaviside series with the
/// corresponding jump offset, and vice versa.
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_INVERT_HEAVISIDE_EAN_08_09_2006
#define BOOST_TIME_SERIES_NUMERIC_INVERT_HEAVISIDE_EAN_08_09_2006

#include <boost/range_run_storage/concepts.hpp>
#include <boost/time_series/heaviside_series.hpp>
#include <boost/time_series/inverse_heaviside_series.hpp>

namespace boost { namespace time_series
{

    /// \brief From a Heaviside series, construct the inverse Heaviside series with the
    ///     corresponding jump offset, and vice versa.
    ///
    /// From a Heaviside series, construct the inverse Heaviside series with the
    ///     corresponding jump offset, and vice versa.
    ///
    /// The complexity of this operation is O(1).
    ///
    /// \param series The series to be inverted.
    /// \throw nothrow
    template<typename Value, typename Discretization, typename Offset>
    inline inverse_heaviside_series<Value, Discretization, Offset> const
    invert_heaviside(heaviside_series<Value, Discretization, Offset> const &series)
    {
        return inverse_heaviside_series<Value, Discretization, Offset>(
            time_series::stop = range_run_storage::offset(range_run_storage::post_run(series))
          , time_series::value = range_run_storage::post_value(series)
          , time_series::discretization = series.discretization()
          , time_series::zero = range_run_storage::zero(series)
        );
    }

    /// \overload
    ///
    template<typename Value, typename Discretization, typename Offset>
    inline inverse_heaviside_unit_series<Value, Discretization, Offset> const
    invert_heaviside(heaviside_unit_series<Value, Discretization, Offset> const &series)
    {
        return inverse_heaviside_unit_series<Value, Discretization, Offset>(
            time_series::stop = range_run_storage::offset(range_run_storage::post_run(series))
          , time_series::value = range_run_storage::post_value(series)
          , time_series::discretization = series.discretization()
          , time_series::zero = range_run_storage::zero(series)
        );
    }

    /// \overload
    ///
    template<typename Value, typename Discretization, typename Offset>
    inline heaviside_series<Value, Discretization, Offset> const
    invert_heaviside(inverse_heaviside_series<Value, Discretization, Offset> const &series)
    {
        return heaviside_series<Value, Discretization, Offset>(
            time_series::start = range_run_storage::end_offset(range_run_storage::pre_run(series))
          , time_series::value = range_run_storage::pre_value(series)
          , time_series::discretization = series.discretization()
          , time_series::zero = range_run_storage::zero(series)
        );
    }

    /// \overload
    ///
    template<typename Value, typename Discretization, typename Offset>
    inline heaviside_unit_series<Value, Discretization, Offset> const
    invert_heaviside(inverse_heaviside_unit_series<Value, Discretization, Offset> const &series)
    {
        return heaviside_unit_series<Value, Discretization, Offset>(
            time_series::start = range_run_storage::end_offset(range_run_storage::pre_run(series))
          , time_series::value = range_run_storage::pre_value(series)
          , time_series::discretization = series.discretization()
          , time_series::zero = range_run_storage::zero(series)
        );
    }

}}

#endif // BOOST_TIME_SERIES_NUMERIC_INVERT_HEAVISIDE_EAN_08_09_2006
