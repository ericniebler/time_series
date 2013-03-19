///////////////////////////////////////////////////////////////////////////////
/// \file transform.hpp
/// single-in transform() for InfiniteRangeRunStorage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_TRANSFORM_EAN_08_04_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_TRANSFORM_EAN_08_04_2006_HPP

#include <boost/range_run_storage/concepts.hpp>

namespace boost { namespace range_run_storage
{
    namespace detail
    {
        namespace rrs = range_run_storage;

        template<typename In, typename UnaryOp, typename Out>
        Out &transform_impl(In &in, UnaryOp &fun, Out &out)
        {
            using concepts::InfiniteRangeRunStorage;

            typedef typename InfiniteRangeRunStorage<In>::cursor cursor_type;
            typedef typename InfiniteRangeRunStorage<In>::runs runs_type;
            typedef typename InfiniteRangeRunStorage<In>::elements elements_type;
            typedef typename InfiniteRangeRunStorage<In>::post_run_type post_run_type;
            typedef typename InfiniteRangeRunStorage<In>::pre_run_type pre_run_type;

            // Transform the pre-run
            pre_run_type pre_run(rrs::pre_run(in));
            if(!rrs::empty(pre_run))
            {
                rrs::set_at(out, pre_run, fun(rrs::pre_value(in)));
            }

            runs_type runs = range_run_storage::runs(in);
            elements_type elements = sequence::elements(in);
            cursor_type begin = sequence::begin(in);
            cursor_type end = sequence::end(in);

            // Transform the finite runs
            for(; begin != end; ++begin)
            {
                rrs::set_at(out, runs(*begin), fun(elements(*begin)));
            }

            // Transform the post-run
            post_run_type post_run(rrs::post_run(in));
            if(!rrs::empty(post_run))
            {
                rrs::set_at(out, post_run, fun(rrs::post_value(in)));
            }

            return out;
        }

        template<typename In, typename UnaryOp>
        In &transform_inplace_impl(In &in, UnaryOp &fun)
        {
            using concepts::Mutable_InfiniteRangeRunStorage;

            typedef typename Mutable_InfiniteRangeRunStorage<In>::cursor cursor_type;
            typedef typename Mutable_InfiniteRangeRunStorage<In>::elements elements_type;

            // mutate the pre-run
            if(!rrs::empty(rrs::pre_run(in)))
            {
                rrs::pre_value(in, fun(rrs::pre_value(in)));
            }

            elements_type elements = sequence::elements(in);
            cursor_type begin = sequence::begin(in);
            cursor_type end = sequence::end(in);

            // Transform the finite runs
            for(; begin != end; ++begin)
            {
                elements(*begin, fun(elements(*begin)));
            }

            // mutate the post-run
            if(!rrs::empty(rrs::post_run(in)))
            {
                rrs::post_value(in, fun(rrs::post_value(in)));
            }

            return in;
        }
    }

}}

#endif
