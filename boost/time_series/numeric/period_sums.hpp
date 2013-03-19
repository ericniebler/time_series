///////////////////////////////////////////////////////////////////////////////
/// \file period_sums.hpp
/// Calculate the sums within fixed-width ranges in a series
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_PERIOD_SUMS_EAN_06_12_2006
#define BOOST_TIME_SERIES_NUMERIC_PERIOD_SUMS_EAN_06_12_2006

#include <boost/implicit_cast.hpp>
#include <boost/concept/requires.hpp>
#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/range_run_storage/traits/is_unit_run.hpp>
#include <boost/range_run_storage/algorithm/copy.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/detail/find_period.hpp>

namespace boost { namespace time_series
{

    namespace detail
    {
        namespace rrs = range_run_storage;

        // period_sums_inserter
        template<typename Out, typename Series, typename Offset, typename Length>
        struct period_sums_inserter
        {
            typedef typename concepts::TimeSeries<Series>::value_type value_type;

            typedef Offset offset_type;
            typedef Length length_type;
            typedef std::pair<offset_type, offset_type> period_type;

            period_sums_inserter(Out const &out, Series const &series, offset_type start, length_type length)
              : out_(out)
              , zero_(implicit_cast<value_type const &>(rrs::zero(series)))
              , value_(implicit_cast<value_type const &>(rrs::zero(series)))
              , period_(start, static_cast<offset_type>(start + length))
              , start_(start)
              , length_(length)
            {}

            template<typename Run>
            void set_at(Run const &run, value_type const &value)
            {
                if(this->start_ < rrs::end_offset(run))
                {
                    if(rrs::less(this->period_, run))
                    {
                        this->out_(this->value_, this->period_.first);
                        this->value_ = this->zero_;
                        this->period_ = detail::find_period(run, this->start_, this->length_);
                    }
                    this->set_at_(run, value, rrs::traits::is_unit_run<Run>());
                }
            }

            Out const &finalize()
            {
                this->out_(this->value_, this->period_.first);
                return this->out_;
            }
        private:
            template<typename Run>
            void set_at_(Run const &, value_type const &value, mpl::true_)
            {
                this->value_ += value;
            }

            template<typename Run>
            void set_at_(Run const &run, value_type const &value, mpl::false_)
            {
                typedef typename result_of<rrs::op::subrun(Run)>::type subrun_type;
                typedef typename result_of<rrs::op::overlap(subrun_type, period_type)>::type overlap_type;
                subrun_type subrun(rrs::subrun(run));

                for(;;)
                {
                    overlap_type overlap(rrs::overlap(subrun, this->period_));
                    this->value_ += value * rrs::length(overlap);
                    rrs::advance_to(subrun, rrs::end_offset(overlap));

                    if(!rrs::has_leftover(subrun))
                        break;

                    this->out_(this->value_, this->period_.first);
                    this->value_ = this->zero_;
                    this->period_.first = this->period_.second;
                    this->period_.second += this->length_;
                }
            }

            Out out_;
            value_type zero_;
            value_type value_;
            period_type period_;
            offset_type start_;
            length_type length_;
        };
    }

    /// \brief Calculate the sums within fixed-width ranges in a series and store
    ///     the results at the start of each range.
    ///
    /// Calculate the sums within fixed-width ranges in a series and store
    ///     the results at the start of each range.
    ///
    /// This function assumes that zero value of the series does not contribute to the
    /// sums, that \c start is not \c -inf and that the series does not have a non-zero
    /// positive infinite run.
    ///
    /// The input series is allowed to use floating point offsets, but the periods as
    /// specified by the \c start and \c length parameters must be integral. As a result,
    /// the output series will necessarily have integral offsets.
    ///
    /// \param series The input series.
    /// \param start The offset of the first period. Must be integral.
    /// \param length The length of the periods. Must be integral.
    /// \param out The ordered inserter to receive the adjacent difference result.
    ///
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre \c Out is a model of the \c OrderedInserter concept.
    /// \pre \c Offset is integral.
    /// \pre \c Length is integral.
    /// \pre For variaibles \c x and \c y of the series' \c value_type, and \c l of
    ///     type \c Length, the expression \c (x+=y*l) must be convertible to 
    ///     \c value_type.
    /// \pre For a variable \c x of the series' value_type, 
    ///     <tt>x == (x + range_run_storage::zero(series))</tt>.
    /// \pre <tt>start != -inf</tt>
    /// \pre <tt>series[inf] == range_run_storage::zero(series)</tt>
    /// \return A \c sparse_series\<\> if no ordered inserter is specified;
    ///     otherwise, the ordered inserter.
    /// \attention If using the version that takes an \c OrderedInserter, you must call
    ///     <tt>.commit()</tt> on the returned \c OrderedInserter when you are done with it.
    template<typename Series, typename Offset, typename Length, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (ordered_inserter<Out>))
    period_sums(Series const &series, Offset start, Length length, ordered_inserter<Out> out)
    {
        BOOST_MPL_ASSERT((is_integral<Offset>));
        BOOST_MPL_ASSERT((is_integral<Length>));

        detail::period_sums_inserter<ordered_inserter<Out>, Series, Offset, Length>
            o(out, series, start, length);

        return range_run_storage::copy(series, o).finalize();
    }


    /// \overload
    ///
    template<typename Series, typename Offset, typename Length>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (sparse_series<
        typename concepts::TimeSeries<Series const>::value_type
      , typename concepts::TimeSeries<Series const>::discretization_type
      , Offset
    >))
    period_sums(Series const &series, Offset start, Length length)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::TimeSeries<Series const>::discretization_type discretization_type;

        // The periodic sums are held in a sparse array. The sums are
        // stored at the start of their associated periods.
        sparse_series<value_type, discretization_type, Offset> result(
            time_series::discretization = series.discretization()
        );

        time_series::period_sums(
            series
          , start
          , length
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }

}}

#endif // BOOST_TIME_SERIES_NUMERIC_PERIOD_SUMS_EAN_06_12_2006
