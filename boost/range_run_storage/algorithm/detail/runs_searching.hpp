///////////////////////////////////////////////////////////////////////////////
/// \file runs_searching.hpp
/// mini-algorithms for searching in a runs property map for an offset
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_RUNS_SEARCHING_EAN_07_27_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_RUNS_SEARCHING_EAN_07_27_2006_HPP

#include <utility>
#include <algorithm>
#include <functional>
#include <boost/assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range_run_storage/utility/subrun.hpp>
#include <boost/range_run_storage/utility/run_cast.hpp>
#include <boost/range_run_storage/traits/is_dense_runs.hpp>

namespace boost { namespace range_run_storage
{
    namespace rrs = range_run_storage;

    namespace detail
    {
        // A transform that converts a cursor to a run using a runs property map
        template<typename Runs, typename Run, typename Cursor>
        struct runs_transform
          : std::unary_function<Cursor, Run>
        {
            explicit runs_transform(Runs const *runs = 0)
              : runs_(runs)
            {}

            Run operator ()(Cursor const &cursor) const
            {
                return rrs::run_cast<Run>((*this->runs_)(*cursor));
            }

        private:
            Runs const *runs_;
        };

        template<typename Cursor>
        std::pair<typename Cursor::base_type, typename Cursor::base_type> const
        range_cast(std::pair<Cursor, Cursor> const &rng)
        {
            return std::make_pair(rng.first.base(), rng.second.base());
        }

        template<typename Cursor, typename Runs, typename Run>
        std::pair<Cursor, Cursor> const
        run_equal_range(Cursor begin, Cursor end, Runs const &runs, Run const &run)
        {
            typedef typename rrs::concepts::Run<Run>::offset_type offset_type;
            typedef std::pair<offset_type, offset_type> run_type;

            runs_transform<Runs, run_type, Cursor> rng_fx(&runs);
            return detail::range_cast(
                std::equal_range(
                    make_transform_iterator(begin, rng_fx)
                  , make_transform_iterator(end, rng_fx)
                  , rrs::run_cast<run_type>(run)
                  , rrs::less
                )
            );
        }

        template<typename Iter, typename Runs, typename Run>
        typename enable_if<
            traits::is_dense_runs<Runs>
          , std::pair<counting_iterator<Iter>, counting_iterator<Iter> > const
        >::type
        run_equal_range(
            counting_iterator<Iter> begin
          , counting_iterator<Iter> end
          , Runs const &runs
          , Run const &run
        )
        {
            typedef std::pair<std::ptrdiff_t, std::ptrdiff_t> period_type;
            std::ptrdiff_t offset = rrs::offset(runs(*begin));
            period_type period(offset, offset + static_cast<std::ptrdiff_t>(end - begin));

            if(rrs::less(period, run))
                return std::make_pair(end, end);

            if(rrs::less(run, period))
                return std::make_pair(begin, begin);

            typedef typename result_of<
                rrs::op::overlap(period_type, Run)
            >::type overlap_type;
            
            overlap_type overlap(rrs::overlap(period, run));

            counting_iterator<Iter> first = begin + (rrs::offset(overlap) - offset);
            return std::make_pair(first, first + rrs::length(overlap));
        }

        // Binary search in the runs property map for the requested run
        template<typename Cursor, typename Runs, typename Run>
        Cursor
        run_lower_bound(Cursor begin, Cursor end, Runs const &runs, Run const &run)
        {
            typedef typename rrs::concepts::Run<Run>::offset_type offset_type;
            typedef std::pair<offset_type, offset_type> run_type;

            detail::runs_transform<Runs, run_type, Cursor> rng_fx(&runs);
            return std::lower_bound(
                make_transform_iterator(begin, rng_fx)
              , make_transform_iterator(end, rng_fx)
              , rrs::run_cast<run_type>(run)
              , rrs::less
            ).base();
        }

        // Optimize for random-access
        template<typename Iter, typename Runs, typename Run>
        typename enable_if<rrs::traits::is_dense_runs<Runs>, counting_iterator<Iter> >::type
        run_lower_bound(
            counting_iterator<Iter> begin
          , counting_iterator<Iter> end
          , Runs const &runs
          , Run const &run
        )
        {
            typedef std::pair<std::ptrdiff_t, std::ptrdiff_t> period_type;
            std::ptrdiff_t offset = rrs::offset(runs(*begin));
            period_type period(offset, offset + static_cast<std::ptrdiff_t>(end - begin));

            if(rrs::end_offset(period) <= rrs::offset(run))
                return end;

            if(rrs::offset(period) >= rrs::offset(run))
                return begin;

            return begin + (rrs::offset(run) - rrs::offset(period));
        }

        // Binary search in the index property map for the requested offset
        template<typename Cursor, typename Runs, typename Run>
        Cursor
        run_skip_ahead_to(Cursor begin, Cursor end, Runs const &runs, Run const &run)
        {
            return run_lower_bound(begin, end, runs, run);
        }

        // Optimize for random-access
        template<typename Iter, typename Runs, typename Run>
        typename enable_if<rrs::traits::is_dense_runs<Runs>, counting_iterator<Iter> >::type
        run_skip_ahead_to(
            counting_iterator<Iter> begin
          , counting_iterator<Iter> end
          , Runs const &runs
          , Run const &run
        )
        {
            BOOST_ASSERT(rrs::offset(runs(*begin)) <= rrs::offset(run));
            BOOST_ASSERT(rrs::offset(runs(*begin)) + (end-begin) >= rrs::offset(run));

            return begin + (rrs::offset(run) - rrs::offset(runs(*begin)));
        }

    } // namespace detail
}}

#endif // BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_RUNS_SEARCHING_EAN_07_27_2006_HPP
