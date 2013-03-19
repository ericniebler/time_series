///////////////////////////////////////////////////////////////////////////////
/// \file clipped_runs.hpp
/// A runs property map with a clip range
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_DETAIL_CLIPPED_RUNS_HPP_EAN_06_05_2006
#define BOOST_TIME_SERIES_DETAIL_CLIPPED_RUNS_HPP_EAN_06_05_2006

#include <utility>
#include <boost/utility/result_of.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/traits/is_dense_runs.hpp>
#include <boost/range_run_storage/utility/run_cast.hpp>

namespace boost { namespace time_series { namespace storage
{

    template<typename Runs>
    struct msvc_result_clipped_runs
    {
        // For result_of support
        template<typename K>
        struct result;

        template<typename This, typename K>
        struct result<This(K)>
          : remove_reference<typename result_of<Runs(K)>::type>
        {};
    };

    // wraps a runs property map and
    // an offset.
    template<typename Runs, typename Offset>
    struct clipped_runs
      : msvc_result_clipped_runs<Runs>
    {
        clipped_runs(Runs const &runs, Offset start, Offset stop)
          : runs_(runs)
          , period_(start, stop)
        {}

        // Readability : Key to index
        template<typename K>
        typename msvc_result_clipped_runs<Runs>
            ::template result<clipped_runs(K)>::type
        operator ()(K const &k) const
        {
            typedef typename msvc_result_clipped_runs<Runs>
                ::template result<clipped_runs(K)>::type run_type;
            return range_run_storage::run_cast<run_type>(
                range_run_storage::overlap(this->runs_(k), this->period_)
            );
        }

    private:
        Runs runs_;
        std::pair<Offset, Offset> period_;
    };

    template<typename Runs, typename Offset>
    inline clipped_runs<Runs, Offset>
    make_clipped_runs(Runs const &runs, Offset start, Offset stop)
    {
        return clipped_runs<Runs, Offset>(runs, start, stop);
    }

}}}

namespace boost { namespace range_run_storage { namespace traits
{
    template<typename Runs, typename Offset>
    struct is_dense_runs<time_series::storage::clipped_runs<Runs, Offset> >
      : is_dense_runs<Runs>
    {};
}}}

#endif // BOOST_TIME_SERIES_DETAIL_CLIPPED_RUNS_HPP_EAN_06_05_2006
