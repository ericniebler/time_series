///////////////////////////////////////////////////////////////////////////////
/// \file transform_intersection.hpp
/// transform() for InfiniteRangeRunStorage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_TRANSFORM_INTERSECTION_EAN_07_27_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_TRANSFORM_INTERSECTION_EAN_07_27_2006_HPP

#include <functional>
#include <boost/assert.hpp>
#include <boost/version.hpp>
#include <boost/detail/function2.hpp>
#if BOOST_VERSION <= 103400
# include <boost/spirit/fusion/sequence/tuple.hpp>
# include <boost/spirit/fusion/sequence/begin.hpp>
# include <boost/spirit/fusion/sequence/end.hpp>
# include <boost/spirit/fusion/iterator/next.hpp>
# include <boost/spirit/fusion/iterator/deref.hpp>
# include <boost/spirit/fusion/iterator/equal_to.hpp>
#else
# include <boost/fusion/tuple.hpp>
# include <boost/fusion/sequence/intrinsic/begin.hpp>
# include <boost/fusion/sequence/intrinsic/end.hpp>
# include <boost/fusion/iterator/next.hpp>
# include <boost/fusion/iterator/deref.hpp>
# include <boost/fusion/iterator/equal_to.hpp>
#endif
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range_run_storage/utility/subrun.hpp>
#include <boost/range_run_storage/utility/subsequence.hpp>
#include <boost/range_run_storage/utility/run_cast.hpp>
#include <boost/range_run_storage/offset.hpp>
#include <boost/range_run_storage/set_at.hpp>
#include <boost/range_run_storage/end_offset.hpp>
#include <boost/range_run_storage/pre_run.hpp>
#include <boost/range_run_storage/post_run.hpp>
#include <boost/range_run_storage/traits/is_unit_run.hpp>
#include <boost/range_run_storage/traits/is_dense_runs.hpp>

namespace boost { namespace fusion
{
    namespace result_of {}
    namespace traits {}
    namespace meta
    {
        using namespace result_of;
        using namespace traits;
    }
}}

namespace boost { namespace range_run_storage
{
    namespace rrs = range_run_storage;

    namespace impl
    {
        // Increment one cursor or the other or both, depending on how their runs overlap
        template<typename L, typename R, 
                 typename LU = typename has_unit_run<L>::type,
                 typename RU = typename has_unit_run<R>::type>
        struct increment
        {
            typedef void result_type;

            result_type operator ()(L &l, R &r) const
            {
                BOOST_ASSERT(!rrs::less(l.subrun(), r.subrun()) && !rrs::less(r.subrun(), l.subrun()));

                bool l_next = (rrs::end_offset(l.subrun()) <= rrs::end_offset(r.subrun()));
                bool r_next = (rrs::end_offset(r.subrun()) <= rrs::end_offset(l.subrun()));
                if(l_next)
                    l.next();
                if(r_next)
                    r.next();
            }
        };

        template<typename L, typename R>
        struct increment<L, R, mpl::true_, mpl::false_>
        {
            typedef void result_type;

            result_type operator ()(L &l, R &r) const
            {
                BOOST_ASSERT(!rrs::less(l.subrun(), r.subrun()) && !rrs::less(r.subrun(), l.subrun()));
                l.next();
            }
        };

        template<typename L, typename R>
        struct increment<L, R, mpl::false_, mpl::true_>
        {
            typedef void result_type;

            result_type operator ()(L &l, R &r) const
            {
                BOOST_ASSERT(!rrs::less(l.subrun(), r.subrun()) && !rrs::less(r.subrun(), l.subrun()));
                r.next();
            }
        };

        template<typename L, typename R>
        struct increment<L, R, mpl::true_, mpl::true_>
        {
            typedef void result_type;

            result_type operator ()(L &l, R &r) const
            {
                BOOST_ASSERT(!rrs::less(l.subrun(), r.subrun()) && !rrs::less(r.subrun(), l.subrun()));
                l.next();
                r.next();
            }
        };

        // work around annoying gcc mpl bug
        template<typename L, typename R>
        struct increment_gcc
          : increment<L, R>
        {};

    } // namespace impl

    namespace op
    {
        using mpl::_;

        struct increment
          : boost::detail::function2<impl::increment_gcc<_, _> >
        {};
    }

    namespace
    {
        op::increment const &increment = boost::detail::pod_singleton<op::increment>::instance;
    }


    namespace detail
    {
        template<
            typename LeftSubSequence
          , typename RightSubSequence
          , typename BinOp
          , typename Out
        >
        void transform_intersection_partial(
            LeftSubSequence &left
          , RightSubSequence &right
          , BinOp const &binop
          , Out &out
        )
        {
            while(!left.done() && !right.done())
            {
                if(rrs::less(left.subrun(), right.subrun()))
                {
                    left.skip_ahead_to(right.subrun());
                }
                else if(rrs::less(right.subrun(), left.subrun()))
                {
                    right.skip_ahead_to(left.subrun());
                }
                else
                {
                    // there is some range overlap
                    rrs::set_at(
                        out
                      , rrs::overlap(left.subrun(), right.subrun())
                      , binop(left.value(), right.value())
                    );

                    // increment either left or right or both
                    rrs::increment(left, right);
                }
            }
        }

        template<
            typename LeftBegin
          , typename LeftEnd
          , typename RightBegin
          , typename RightEnd
          , typename BinOp
          , typename Out
        >
        void transform_intersection_recurse(
            LeftBegin const &
          , LeftEnd const &
          , RightBegin const &
          , RightEnd const &
          , BinOp const &
          , Out &out
          , mpl::true_
          , mpl::true_
        )
        {}

        template<
            typename LeftBegin
          , typename LeftEnd
          , typename RightBegin
          , typename RightEnd
          , typename BinOp
          , typename Out
        >
        void transform_intersection_recurse(
            LeftBegin const &
          , LeftEnd const &
          , RightBegin const &
          , RightEnd const &
          , BinOp const &
          , Out &out
          , mpl::true_
          , mpl::false_
        )
        {}

        template<
            typename LeftBegin
          , typename LeftEnd
          , typename RightBegin
          , typename RightEnd
          , typename BinOp
          , typename Out
        >
        void transform_intersection_recurse(
            LeftBegin const &
          , LeftEnd const &
          , RightBegin const &
          , RightEnd const &
          , BinOp const &
          , Out &out
          , mpl::false_
          , mpl::true_
        )
        {}

        template<
            typename LeftBegin
          , typename LeftEnd
          , typename RightBegin
          , typename RightEnd
          , typename BinOp
          , typename Out
        >
        void transform_intersection_recurse(
            LeftBegin const &left_begin
          , LeftEnd const &left_end
          , RightBegin const &right_begin
          , RightEnd const &right_end
          , BinOp const &binop
          , Out &out
          , mpl::false_
          , mpl::false_
        )
        {
            typedef typename fusion::meta::next<LeftBegin>::type left_next_type;
            typedef typename fusion::meta::next<RightBegin>::type right_next_type;

            detail::transform_intersection_partial(
                *left_begin
              , *right_begin
              , binop
              , out
            );
            
            BOOST_ASSERT(rrs::is_done_and_empty(*left_begin) || rrs::is_done_and_empty(*right_begin));
            
            if(rrs::is_done_and_empty(*left_begin) && rrs::is_done_and_empty(*right_begin))
            {
                detail::transform_intersection_recurse(
                    fusion::next(left_begin)
                  , left_end
                  , fusion::next(right_begin)
                  , right_end
                  , binop
                  , out
                  , fusion::meta::equal_to<left_next_type, LeftEnd>()
                  , fusion::meta::equal_to<right_next_type, RightEnd>()
                );
            }
            else if(rrs::is_done_and_empty(*left_begin))
            {
                detail::transform_intersection_recurse(
                    fusion::next(left_begin)
                  , left_end
                  , right_begin
                  , right_end
                  , binop
                  , out
                  , fusion::meta::equal_to<left_next_type, LeftEnd>()
                  , mpl::false_()
                );
            }
            else
            {
                detail::transform_intersection_recurse(
                    left_begin
                  , left_end
                  , fusion::next(right_begin)
                  , right_end
                  , binop
                  , out
                  , mpl::false_()
                  , fusion::meta::equal_to<right_next_type, RightEnd>()
                );
            }
        }

        template<
            typename LeftSubSequences
          , typename RightSubSequences
          , typename BinOp
          , typename Out
        >
        void transform_intersection(
            LeftSubSequences const &left
          , RightSubSequences const &right
          , BinOp const &binop
          , Out &out
        )
        {
            typedef typename fusion::meta::begin<LeftSubSequences>::type left_begin_type;
            typedef typename fusion::meta::end<LeftSubSequences>::type left_end_type;
            typedef typename fusion::meta::begin<RightSubSequences>::type right_begin_type;
            typedef typename fusion::meta::end<RightSubSequences>::type right_end_type;

            detail::transform_intersection_recurse(
                fusion::begin(left)
              , fusion::end(left)
              , fusion::begin(right)
              , fusion::end(right)
              , binop
              , out
              , fusion::meta::equal_to<left_begin_type, left_end_type>()
              , fusion::meta::equal_to<right_begin_type, right_end_type>()
            );
        }
    } // namespace detail
}}

#endif // BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_TRANSFORM_INTERSECTION_EAN_07_27_2006_HPP
