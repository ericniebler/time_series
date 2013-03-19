///////////////////////////////////////////////////////////////////////////////
/// \file offset_runs.hpp
/// An index property map with an integer offset
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_DETAIL_OFFSET_RUNS_HPP_EAN_04_26_2006
#define BOOST_TIME_SERIES_DETAIL_OFFSET_RUNS_HPP_EAN_04_26_2006

#include <boost/time_series/time_series_fwd.hpp>

#include <boost/assert.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/traits/is_dense_runs.hpp>

namespace boost { namespace time_series { namespace storage
{
    template<typename Runs>
    struct msvc_result_offset_runs
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
    struct offset_runs
      : msvc_result_offset_runs<Runs>
    {
        offset_runs(Runs const &runs, Offset offset)
          : runs_(runs)
          , offset_(offset)
        {}

        // Readability : Key to index
        template<typename K>
        typename msvc_result_offset_runs<Runs>
            ::template result<offset_runs(K)>::type
        operator ()(K const &k) const
        {
            using range_run_storage::concepts::Run;
            typedef typename msvc_result_offset_runs<Runs>
                ::template result<offset_runs(K)>::type run_type;
            typedef typename Run<run_type>::is_unit_length is_unit_length;
            return this->call_(this->runs_(k), is_unit_length());
        }

    private:
        template<typename R>
        R call_(R const &r, mpl::true_) const
        {
            typedef typename range_run_storage::concepts::Run<R>::offset_type offset_type;
            offset_type offset = range_run_storage::offset(r);

            // With the InfiniteRangeRunStorage concept, infinite runs
            // can only be a pre_run or a post_run, never from the runs
            // property map
            BOOST_ASSERT(-inf != offset && inf != offset);
            return R(offset + this->offset_);
        }

        template<typename R>
        R call_(R const &r, mpl::false_) const
        {
            typedef typename range_run_storage::concepts::Run<R>::offset_type offset_type;
            offset_type offset = range_run_storage::offset(r);
            offset_type endoff = range_run_storage::end_offset(r);

            // With the InfiniteRangeRunStorage concept, infinite runs
            // can only be a pre_run or a post_run, never from the runs
            // property map
            BOOST_ASSERT(-inf != offset && inf != offset);
            BOOST_ASSERT(-inf != endoff && inf != endoff);
            return R(offset + this->offset_, endoff + this->offset_);
        }

        Runs runs_;
        Offset offset_;
    };

    template<typename Runs, typename Offset>
    inline offset_runs<Runs, Offset>
    make_offset_runs(Runs const &runs, Offset offset)
    {
        return offset_runs<Runs, Offset>(runs, offset);
    }

}}}

namespace boost { namespace range_run_storage { namespace traits
{
    template<typename Runs, typename Offset>
    struct is_dense_runs<time_series::storage::offset_runs<Runs, Offset> >
      : is_dense_runs<Runs>
    {};
}}}

#endif
