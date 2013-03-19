// Copyright Daniel Wallin, Eric Niebler 2006. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_COARSE_GRAIN_060615_HPP
#define BOOST_TIME_SERIES_NUMERIC_COARSE_GRAIN_060615_HPP

#include <boost/concept/requires.hpp>
#include <boost/integer_traits.hpp>
#include <boost/iterator/counting_iterator.hpp>
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

        template<typename Out, typename Offset>
        struct coarse_grain_inserter
        {
            coarse_grain_inserter(Out out, Offset factor)
              : out_(out)
              , offset_(inf)
              , factor_(factor)
            {}

            template<typename Run, typename Value>
            void set_at(Run &run, Value &value)
            {
                this->offset_ = std::min<Offset>(this->offset_, rrs::offset(run));
                typedef typename result_of<rrs::op::subrun(Run &)>::type subrun_type;
                typedef std::pair<Offset, Offset> period_type;
                typedef rrs::op::overlap overlap_fun(period_type, subrun_type);
                typedef typename result_of<overlap_fun>::type overlap_type;

                subrun_type subrun(rrs::subrun(run));

                do
                {
                    period_type period(detail::find_period(subrun, this->offset_, this->factor_));
                    overlap_type overlap(rrs::overlap(period, subrun));

                    if(0 == rrs::offset(overlap) % this->factor_)
                    {
                        this->out_(value, period.first / this->factor_);
                    }

                    rrs::advance_to(subrun, rrs::end_offset(overlap));
                }
                while(rrs::has_leftover(subrun));
            }

            Out const &out()
            {
                return this->out_;
            }
        private:
            Out out_;
            Offset offset_;
            Offset factor_;
        };

    } // namespace detail

    /// \brief Generate a series of a coarser discretization by picking the values
    /// at the coarse-grained points.
    ///
    /// Generate a series of a coarser discretization by picking the values
    /// at the coarse-grained points.
    ///
    /// \param series The input series.
    /// \param discretization The coarser discretization.
    /// \param out An \c OrderedInserter into which to write the coarser series.
    /// \return Either a <tt>sparse_series\<\></tt> containing the coarser series, or a
    ///         copy of the \c OrderedInserter passed in.
    /// \attention If using the version that takes an \c OrderedInserter, you must call
    ///         <tt>.commit()</tt> on the returned \c OrderedInserter when you are done with it.
    /// \pre The coarser discretization is a multiple of the finer discretization.
    template <class Series, class Discretization, class Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (ordered_inserter<Out>))
    coarse_grain(Series const &series, Discretization discretization, ordered_inserter<Out> out)
    {
        typedef typename concepts::TimeSeries<Series const>::offset_type offset_type;

        BOOST_ASSERT(discretization > series.discretization());
        BOOST_ASSERT(discretization % series.discretization() == 0);

        offset_type factor = discretization / series.discretization();

        detail::coarse_grain_inserter<ordered_inserter<Out>, offset_type> o(out, factor);
        return range_run_storage::copy(series, o).out();
    }

    /// \overload
    ///
    template <class Series, class Discretization>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (sparse_series<
        typename concepts::TimeSeries<Series const>::value_type
      , Discretization
    >))
    coarse_grain(Series const &series, Discretization discretization)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;

        sparse_series<value_type, Discretization> result(
            time_series::discretization = discretization
        );

        time_series::coarse_grain(
            series
          , discretization
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }

}} // namespace boost::time_series

#endif // BOOST_TIME_SERIES_NUMERIC_COARSE_GRAIN_060615_HPP
