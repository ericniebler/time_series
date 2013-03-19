///////////////////////////////////////////////////////////////////////////////
/// \file invert_elements.hpp
/// Construct a new series from the old where each element is inverted.
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_INVERT_ELEMENTS_EAN_10_11_2006
#define BOOST_TIME_SERIES_NUMERIC_INVERT_ELEMENTS_EAN_10_11_2006

#include <boost/concept/requires.hpp>
#include <boost/detail/construct.hpp>
#include <boost/numeric/functional.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/range_run_storage/algorithm/transform.hpp>

namespace boost { namespace time_series
{
    namespace detail
    {
        template<typename Value>
        struct inv_op : std::unary_function<Value, Value>
        {
            Value operator ()(Value const &value) const
            {
                // Roughly equivalent to Value(1) / value:
                return numeric::divides(numeric::as_one(value), value);
            }
        };
    }

    /// \brief Construct a new series from the old where each element is inverted.
    ///
    /// Construct a new series from the old where each element is inverted.
    ///
    /// \param series The series to invert.
    /// \param out The inserter into which the new series is written.
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre \c Out is a model of the \c OrderedInserter concept.
    template<typename Series, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series>)),
    (ordered_inserter<Out>))
    invert_elements(Series const &series, ordered_inserter<Out> out)
    {
        typedef typename concepts::TimeSeries<Series>::value_type value_type;
        return range_run_storage::transform(series, detail::inv_op<value_type>(), out);
    }

    /// \overload
    ///
    template<typename Series>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series>)),
    (Series const))
    invert_elements(Series const &series)
    {
        Series result(constructors::construct<Series>(
            time_series::discretization = series.discretization()
          , time_series::zero = range_run_storage::zero(series)
        ));
        
        time_series::invert_elements(
            series
          , time_series::make_ordered_inserter(result)
        ).commit();
        
        return result;
    }

    /// \brief Invert each element in a series in-place.
    ///
    /// Invert each element in a series in-place.
    ///
    /// \param series The series to invert.
    /// \return series
    /// \pre \c Series is a model of the \c Mutable_TimeSeries concept.
    template<typename Series>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::Mutable_TimeSeries<Series>)),
    (Series &))
    invert_elements_inplace(Series &series)
    {
        typedef typename concepts::Mutable_TimeSeries<Series>::value_type value_type;
        return range_run_storage::transform_inplace(series, detail::inv_op<value_type>());
    }

}}

#endif // BOOST_TIME_SERIES_NUMERIC_INVERT_ELEMENTS_EAN_10_11_2006
