///////////////////////////////////////////////////////////////////////////////
/// \file rotate_left.hpp
/// 
//
//  Copyleft 2006 Michael Gauckler, Eric Niebler.
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_ROTATE_LEFT_MCG_08_23_2006
#define BOOST_TIME_SERIES_NUMERIC_ROTATE_LEFT_MCG_08_23_2006

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

        // rotate_left_inserter
        template<typename Out, typename Offset>
        struct rotate_left_inserter
        {
            typedef std::pair<Offset, Offset> run_type;

            rotate_left_inserter(Out const &out)
              : out_(out)
              , last_(-inf, -inf)
            {}

            template<typename Run2, typename Value>
            void set_at(Run2 const &run, Value const &value)
            {
                if(-inf != last_.second)
                {
                    rrs::set_at(this->out_, this->last_, value);
                }
                this->last_ = rrs::run_cast<run_type>(run);
            }            

            template<typename Value>
            Out const &finalize(Value const &value)
            {
                rrs::set_at(this->out_, this->last_, value);
                return this->out_;
            }
        private:
            Out out_;
            run_type last_;
        };
    }

    /// \brief Rotates a series left, in the sense that the i-th run assumes the
    ///     value of the (i+1)-th run. The final run in the series assumes the value
    ///     of the \c value parameter if specified; otherwise, the last run is 
    ///     dropped.
    ///
    /// Rotates a series left, in the sense that the i-th run assumes the
    ///     value of the (i+1)-th run. The final run in the series assumes the value
    ///     of the \c value parameter if specified; otherwise, the last run is 
    ///     dropped.
    ///
    /// \param series The input series.
    /// \param value The value the final run in the series should assume.
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
    rotate_left(
        Series const &series
      , typename concepts::TimeSeries<Series const>::value_type const &value
      , ordered_inserter<Out> out
    )
    {
        typedef typename concepts::TimeSeries<Series const>::offset_type offset_type;
        detail::rotate_left_inserter<ordered_inserter<Out>, offset_type>
            inserter(out);
        return range_run_storage::copy(series, inserter).finalize(value);
    }

    /// \overload
    ///
    template<typename Series, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (ordered_inserter<Out>))
    rotate_left(Series const &series, ordered_inserter<Out> out)
    {
        namespace rrs = range_run_storage;
        typename concepts::TimeSeries<Series const>::value_type const &zero = rrs::zero(series);
        return time_series::rotate_left(series, zero, out);
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
    rotate_left(Series const &series, typename concepts::TimeSeries<Series const>::value_type const &value)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::TimeSeries<Series const>::discretization_type discretization_type;
        typedef typename concepts::TimeSeries<Series const>::offset_type offset_type;

        // The partial differences are held in a sparse array.
        sparse_series<value_type, discretization_type, offset_type> result(
            time_series::discretization = series.discretization()
        );

        time_series::rotate_left(
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
    rotate_left(Series const &series)
    {
        namespace rrs = range_run_storage;
        typename concepts::TimeSeries<Series const>::value_type const &zero = rrs::zero(series);
        return time_series::rotate_left(series, zero);
    }
}}

#endif // BOOST_TIME_SERIES_NUMERIC_ROTATE_RIGHT_MCG_08_23_2006
