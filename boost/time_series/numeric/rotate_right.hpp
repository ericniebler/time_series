///////////////////////////////////////////////////////////////////////////////
/// \file rotate_right.hpp
/// 
//
//  Copyright 2006 Michael Gauckler, Eric Niebler.
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_ROTATE_RIGHT_MCG_08_23_2006
#define BOOST_TIME_SERIES_NUMERIC_ROTATE_RIGHT_MCG_08_23_2006

#include <boost/concept/requires.hpp>
#include <boost/range_run_storage/algorithm/copy.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>

namespace boost { namespace time_series
{
    namespace detail
    {
        namespace rrs = range_run_storage;

        // rotate_right_inserter
        template<typename Out, typename Value>
        struct rotate_right_inserter
        {
            rotate_right_inserter(Out const &out, Value const &first)
              : out_(out)
              , last_(first)
            {}

            template<typename Run>
            void set_at(Run const &run, Value const &value)
            {
                rrs::set_at(this->out_, run, this->last_);
                this->last_ = value;
            }            

            Out const &commit()
            {
                return this->out_;
            }
        private:
            Out out_;
            Value last_;
        };
    }

    /// \brief Rotates a series right, in the sense that the i-th run assumes the
    ///     value of the (i-1)-th run. The first run in the series assumes the value
    ///     of the \c value parameter if specified; otherwise, the first run is 
    ///     dropped.
    ///
    /// Rotates a series right, in the sense that the i-th run assumes the
    ///     value of the (i-1)-th run. The first run in the series assumes the value
    ///     of the \c value parameter if specified; otherwise, the first run is 
    ///     dropped.
    ///
    /// \param series The input series.
    /// \param value The value the first run in the series should assume.
    /// \param out The ordered inserter to receive the adjacent difference result.
    ///
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre \c Out is a model of the \c OrderedInserter concept.
    /// \return A \c sparse_series\<\> if no ordered inserter is specified; otherwise,
    ///     the ordered inserter.
    /// \attention If using the version that takes an \c OrderedInserter, you must call
    ///     <tt>.commit()</tt> on the returned \c OrderedInserter when you are done with it.
    template<typename Series, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (ordered_inserter<Out>))
    rotate_right(
        Series const &series
      , typename concepts::TimeSeries<Series const>::value_type const &value
      , ordered_inserter<Out> out
    )
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        detail::rotate_right_inserter<ordered_inserter<Out>, value_type>
            inserter(out, value);
        return range_run_storage::copy(series, inserter).commit();
    }

    /// \overload
    ///
    template<typename Series, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (ordered_inserter<Out>))
    rotate_right(Series const &series, ordered_inserter<Out> out)
    {
        namespace rrs = range_run_storage;
        typename concepts::TimeSeries<Series const>::value_type const &zero = rrs::zero(series);
        return time_series::rotate_right(series, zero, out);
    }

    /// \overload
    ///
    template<typename Series>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (sparse_series<
        typename concepts::TimeSeries<Series const>::value_type
      , typename concepts::TimeSeries<Series const>::discretization_type
      , typename concepts::TimeSeries<Series const>::offset_type
    >))
    rotate_right(Series const &series, typename concepts::TimeSeries<Series const>::value_type const &value)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::TimeSeries<Series const>::discretization_type discretization_type;
        typedef typename concepts::TimeSeries<Series const>::offset_type offset_type;

        // The partial differences are held in a sparse array.
        sparse_series<value_type, discretization_type, offset_type> result(
            time_series::discretization = series.discretization()
        );

        time_series::rotate_right(
            series
          , value
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }

    /// \overload
    ///
    template<typename Series>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (sparse_series<
        typename concepts::TimeSeries<Series const>::value_type
      , typename concepts::TimeSeries<Series const>::discretization_type
      , typename concepts::TimeSeries<Series const>::offset_type
    >))
    rotate_right(Series const &series)
    {
        namespace rrs = range_run_storage;
        typename concepts::TimeSeries<Series const>::value_type const &zero = rrs::zero(series);
        return time_series::rotate_right(series, zero);
    }
}}

#endif // BOOST_TIME_SERIES_NUMERIC_ROTATE_RIGHT_MCG_08_23_2006
