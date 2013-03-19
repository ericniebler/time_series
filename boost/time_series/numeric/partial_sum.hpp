///////////////////////////////////////////////////////////////////////////////
/// \file partial_sum.hpp
/// Like std::accumulate for the whole series, multiplied by the discretization
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_PARTIAL_SUM_EAN_06_14_2006
#define BOOST_TIME_SERIES_NUMERIC_PARTIAL_SUM_EAN_06_14_2006

#include <boost/implicit_cast.hpp>
#include <boost/concept/requires.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/range_run_storage/traits/is_unit_run.hpp>
#include <boost/range_run_storage/algorithm/copy.hpp>
#include <boost/time_series/time_series_fwd.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>

namespace boost { namespace time_series
{

    namespace detail
    {
        namespace rrs = range_run_storage;

        // partial_sum_inserter
        template<typename Out, typename Series>
        struct partial_sum_inserter
        {
            typedef concepts::TimeSeries<Series> series_concept;
            typedef typename series_concept::value_type value_type;
            typedef typename series_concept::offset_type offset_type;

            partial_sum_inserter(Out const &out, Series const &series)
              : out_(out)
              , sum_(implicit_cast<value_type const &>(rrs::zero(series)))
              , offset_(-inf)
            {}

            template<typename Run>
            void set_at(Run const &run, value_type const &value)
            {
                if(this->offset_ != range_run_storage::offset(run))
                {
                    this->out_(this->sum_, this->offset_, rrs::offset(run));
                }

                this->partial_sum_run(value, run, rrs::traits::is_unit_run<Run>());
                this->offset_ = rrs::end_offset(run);
            }

            Out const &finalize()
            {
                if(inf != this->offset_)
                {
                    this->out_(this->sum_, this->offset_, inf);
                }
                return this->out_;
            }
        private:
            template<typename Run>
            void partial_sum_run(value_type const &value, Run const &run, mpl::false_)
            {
                BOOST_MPL_ASSERT((is_integral<offset_type>)); // TODO
                offset_type offset = rrs::offset(run);
                offset_type endoff = rrs::end_offset(run);

                for(; offset != endoff; ++offset)
                {
                    this->out_(this->sum_ += value, offset);
                }
            }

            template<typename Run>
            void partial_sum_run(value_type const &value, Run const &run, mpl::true_)
            {
                this->out_(this->sum_ += value, rrs::offset(run));
            }

            Out out_;
            value_type sum_;
            offset_type offset_;
        };
    }

    /// \brief Like \c std::partial_sum(), \c time_series::partial_sum() calculates
    /// a series of sums such that the result at offset \em i is the sum of the input
    /// from offsets <em>[-inf,i]</em>.
    ///
    /// Like \c std::partial_sum(), \c time_series::partial_sum() calculates
    /// a series of sums such that the result at offset \em i is the sum of the input
    /// from offsets <em>[0,i]</em>.
    ///
    /// This function assumes that zero value of the series does not contribute to the
    /// sums, and that the series does not have non-zero positive or negative infinite
    /// runs.
    ///
    /// \param series The input series.
    /// \param out The ordered inserter to receive the adjacent difference result.
    ///
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre \c Out is a model of the \c OrderedInserter concept.
    /// \pre The \c offset_type of the \c Series is integral.
    /// \pre For variaibles \c x and \c y of the series' \c value_type, the expression
    ///     \c (x+=y) must be convertible to \c value_type.
    /// \pre For a variable \c x of the series' value_type, 
    ///     <tt>x == (x + range_run_storage::zero(series))</tt>.
    /// \pre <tt>series[-inf] == range_run_storage::zero(series)</tt>
    /// \pre <tt>series[inf] == range_run_storage::zero(series)</tt>
    /// \return A \c piecewise_constant_series\<\> if no ordered inserter is specified;
    ///     otherwise, the ordered inserter.
    /// \attention If using the version that takes an \c OrderedInserter, you must call
    ///     <tt>.commit()</tt> on the returned \c OrderedInserter when you are done with it.
    template<typename Series, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (ordered_inserter<Out>))
    partial_sum(Series const &series, ordered_inserter<Out> out)
    {
        detail::partial_sum_inserter<ordered_inserter<Out>, Series> o(out, series);
        return range_run_storage::copy(series, o).finalize();
    }

    /// \overload
    ///
    template<typename Series>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (piecewise_constant_series<
        typename concepts::TimeSeries<Series const>::value_type
      , typename concepts::TimeSeries<Series const>::discretization_type
    >))
    partial_sum(Series const &series)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::TimeSeries<Series const>::discretization_type discretization_type;

        // The partial differences are held in a piecewise constant array.
        piecewise_constant_series<value_type, discretization_type> result(
            time_series::discretization = series.discretization()
        );

        time_series::partial_sum(
            series
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }

}}

#endif // BOOST_TIME_SERIES_NUMERIC_PARTIAL_SUM_EAN_06_14_2006
