///////////////////////////////////////////////////////////////////////////////
/// \file adjacent_difference.hpp
/// Like \c std::adjacent_difference, but for Series
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_ADJACENT_DIFFERENCE_EAN_06_13_2006
#define BOOST_TIME_SERIES_NUMERIC_ADJACENT_DIFFERENCE_EAN_06_13_2006

#include <boost/implicit_cast.hpp>
#include <boost/concept/requires.hpp>
#include <boost/range_run_storage/algorithm/for_each.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>

namespace boost { namespace time_series
{
    namespace detail
    {
        namespace rrs = range_run_storage;

        // adjacent_difference_functor
        template<typename Out, typename Series>
        struct adjacent_difference_functor
        {
            typedef concepts::TimeSeries<Series> series_concept;
            typedef typename series_concept::value_type value_type;
            typedef typename series_concept::offset_type offset_type;

            adjacent_difference_functor(Out const &out, Series const &series)
              : out_(out)
              , zero_(implicit_cast<value_type const &>(rrs::zero(series)))
              , value_(rrs::get_at(series, -inf))
              , offset_(-inf)
            {}

            void operator ()(value_type const &value, offset_type start, offset_type stop)
            {
                if(this->offset_ == start)
                {
                    this->out_(value - this->value_, start);
                }
                else
                {
                    this->out_(this->zero_ - this->value_, this->offset_);
                    this->out_(value - this->zero_, start);
                }
                this->offset_ = stop;
                this->value_ = value;
            }

            Out const &finalize()
            {
                if(inf != this->offset_)
                {
                    this->out_(this->zero_ - this->value_, this->offset_);
                }
                return this->out_;
            }
        private:
            Out out_;
            value_type zero_;
            value_type value_;
            offset_type offset_;
        };
    }

    /// \brief Calculate the adjacent difference of a series.
    ///
    /// Calculate the adjacent difference of a series, much like \c std::adjacent_difference.
    ///
    /// \param series The input series.
    /// \param out The ordered inserter to receive the adjacent difference result.
    ///
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre \c Out is a model of the \c OrderedInserter concept.
    /// \pre For variaibles \c x and \c y of the series' \c value_type, the expression
    ///     \c (x-y) must be convertible to \c value_type.
    /// \return A \c sparse_series\<\> if no ordered inserter is specified; otherwise,
    ///     the ordered inserter.
    /// \attention If using the version that takes an \c OrderedInserter, you must call
    ///     <tt>.commit()</tt> on the returned \c OrderedInserter when you are done with it.
    template<typename Series, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (ordered_inserter<Out>))
    adjacent_difference(Series const &series, ordered_inserter<Out> out)
    {
        detail::adjacent_difference_functor<ordered_inserter<Out>, Series> fun(out, series);
        return range_run_storage::for_each(series, fun).finalize();
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
    adjacent_difference(Series const &series)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::TimeSeries<Series const>::offset_type offset_type;
        typedef typename concepts::TimeSeries<Series const>::discretization_type discretization_type;

        // The partial differences are held in a sparse array.
        sparse_series<value_type, discretization_type, offset_type> result(
            time_series::discretization = series.discretization()
        );

        time_series::adjacent_difference(
            series
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }

}}

#endif // BOOST_TIME_SERIES_NUMERIC_ADJACENT_DIFFERENCE_EAN_06_12_2006
