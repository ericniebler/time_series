///////////////////////////////////////////////////////////////////////////////
/// \file integrate.hpp
/// Like \c std::accumulate for the whole series, multiplied by the discretization
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_INTEGRATE_EAN_06_14_2006
#define BOOST_TIME_SERIES_NUMERIC_INTEGRATE_EAN_06_14_2006

#include <boost/implicit_cast.hpp>
#include <boost/concept/requires.hpp>
#include <boost/range_run_storage/algorithm/copy.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/utility/time_series_base.hpp>

namespace boost { namespace time_series
{
    namespace detail
    {
        template<typename Value>
        struct integrate_inserter
        {
            explicit integrate_inserter(Value const &value)
              : value_(value)
            {}

            template<typename Run>
            void set_at(Run const &run, Value const &value)
            {
                this->value_ += value * range_run_storage::length(run);
            }

            Value const &value() const
            {
                return this->value_;
            }
        private:
            Value value_;
        };
    }

    /// \brief Calculates the integral of the series. 
    ///
    /// The integral is calculated by multiplying each run's value by the run's length,
    ///     summing all the results, and multiplying the sum by the series' discretization.
    ///
    /// \param series The input series.
    /// \pre Given a variable \c x of the series' \c value_type and \c y of the series'
    ///     \c length_type, the expression \c (x*y) must be convertible to \c value_type.
    /// \pre Given variables \c x and \c y of the series' \c value_type, the expression
    ///     \c (x+y) must be convertible to \c value_type.
    /// \pre Given a variable \c x of the series' \c value_type and \c y of the series'
    ///     \c discretization_type, the expression \c (x*y) must be convertible to \c value_type.
    /// \return A \c value_type holding the result of \c std::accumulate of the series,
    ///     multiplied by the series discretization.
    /// \attention Currently assumes the zeros of the series do not participate
    ///     in the integral.
    template<typename Series>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (typename concepts::TimeSeries<Series const>::value_type))
    integrate(Series const &series)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;

        detail::integrate_inserter<value_type> o(
            implicit_cast<value_type const &>(range_run_storage::zero(series)));
        range_run_storage::copy(series, o);
        return o.value() * series.discretization();
    }

}}

#endif // BOOST_TIME_SERIES_NUMERIC_INTEGRATE_EAN_06_14_2006
