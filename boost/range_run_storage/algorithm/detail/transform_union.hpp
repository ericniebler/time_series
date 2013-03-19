///////////////////////////////////////////////////////////////////////////////
/// \file transform_union.hpp
/// transform() for InfiniteRangeRunStorage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_TRANSFORM_UNION_EAN_07_27_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_TRANSFORM_UNION_EAN_07_27_2006_HPP

#include <boost/assert.hpp>
#include <boost/version.hpp>
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
#include <boost/range_run_storage/utility/subrun.hpp>
#include <boost/range_run_storage/utility/subsequence.hpp>
#include <boost/range_run_storage/offset.hpp>
#include <boost/range_run_storage/set_at.hpp>
#include <boost/range_run_storage/end_offset.hpp>
#include <boost/range_run_storage/pre_run.hpp>
#include <boost/range_run_storage/post_run.hpp>

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

    namespace detail
    {
        template<
            typename LeftSubSequence
          , typename RightSubSequence
          , typename BothOp
          , typename LeftOp
          , typename RightOp
          , typename Out
        >
        void transform_union_partial(
            LeftSubSequence &left
          , RightSubSequence &right
          , BothOp const &both_op
          , LeftOp const &left_op
          , RightOp const &right_op
          , Out &out
        )
        {
            typedef typename LeftSubSequence::subrun_type left_subrun_type;
            typedef typename RightSubSequence::subrun_type right_subrun_type;

            while(!left.done() && !right.done())
            {
                if(rrs::less(left.subrun(), right.subrun()))
                {
                    rrs::set_at(
                        out
                      , left.subrun()
                      , left_op(left.value())
                    );
                    left.next();
                }
                else if(rrs::less(right.subrun(), left.subrun()))
                {
                    rrs::set_at(
                        out
                      , right.subrun()
                      , right_op(right.value())
                    );
                    right.next();
                }
                else
                {
                    if(rrs::before(left.subrun(), right.subrun()))
                    {
                        rrs::set_at(
                            out
                          , rrs::front(left.subrun(), rrs::offset(right.subrun()))
                          , left_op(left.value())
                        );
                        left.advance_to(rrs::offset(right.subrun()));
                    }
                    else if(rrs::before(right.subrun(), left.subrun()))
                    {
                        rrs::set_at(
                            out
                          , rrs::front(right.subrun(), rrs::offset(left.subrun()))
                          , right_op(right.value())
                        );
                        right.advance_to(rrs::offset(left.subrun()));
                    }

                    typedef rrs::op::overlap overlap_fun(left_subrun_type, right_subrun_type);

                    typename boost::result_of<overlap_fun>::type overlap =
                        rrs::overlap(left.subrun(), right.subrun());

                    rrs::set_at(
                        out
                      , overlap
                      , both_op(left.value(), right.value())
                    );

                    left.advance_to(rrs::end_offset(overlap));
                    right.advance_to(rrs::end_offset(overlap));

                    if(!rrs::has_leftover(left.subrun())) // left sub-range exhausted?
                    {
                        left.next();
                    }

                    if(!rrs::has_leftover(right.subrun())) // right sub-range exhausted?
                    {
                        right.next();
                    }
                }
            }
        }

        template<
            typename LeftBegin
          , typename LeftEnd
          , typename RightBegin
          , typename RightEnd
          , typename BothOp
          , typename LeftOp
          , typename RightOp
          , typename Out
        >
        void transform_union_recurse(
            LeftBegin const &
          , LeftEnd const &
          , RightBegin const &
          , RightEnd const &
          , BothOp const &
          , LeftOp const &
          , RightOp const &
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
          , typename BothOp
          , typename LeftOp
          , typename RightOp
          , typename Out
        >
        void transform_union_recurse(
            LeftBegin const &left_begin
          , LeftEnd const &left_end
          , RightBegin const &right_begin
          , RightEnd const &right_end
          , BothOp const &both_op
          , LeftOp const &left_op
          , RightOp const &right_op
          , Out &out
          , mpl::true_
          , mpl::false_
        )
        {
            typedef typename fusion::meta::next<RightBegin>::type right_next_type;

            if(rrs::has_leftover((*right_begin).subrun()))
            {
                rrs::set_at(
                    out
                  , (*right_begin).subrun()
                  , right_op((*right_begin).value())
                );
                (*right_begin).next();
            }

            for(; !(*right_begin).done(); (*right_begin).next())
            {
                rrs::set_at(
                    out
                  , (*right_begin).subrun()
                  , right_op((*right_begin).value())
                );
            }

            detail::transform_union_recurse(
                left_begin
              , left_end
              , fusion::next(right_begin)
              , right_end
              , both_op
              , left_op
              , right_op
              , out
              , mpl::true_()
              , fusion::meta::equal_to<right_next_type, RightEnd>()
            );
        }

        template<
            typename LeftBegin
          , typename LeftEnd
          , typename RightBegin
          , typename RightEnd
          , typename BothOp
          , typename LeftOp
          , typename RightOp
          , typename Out
        >
        void transform_union_recurse(
            LeftBegin const &left_begin
          , LeftEnd const &left_end
          , RightBegin const &right_begin
          , RightEnd const &right_end
          , BothOp const &both_op
          , LeftOp const &left_op
          , RightOp const &right_op
          , Out &out
          , mpl::false_
          , mpl::true_
        )
        {
            typedef typename fusion::meta::next<LeftBegin>::type left_next_type;

            if(rrs::has_leftover((*left_begin).subrun()))
            {
                rrs::set_at(
                    out
                  , (*left_begin).subrun()
                  , left_op((*left_begin).value())
                );
                (*left_begin).next();
            }

            for(; !(*left_begin).done(); (*left_begin).next())
            {
                rrs::set_at(
                    out
                  , (*left_begin).subrun()
                  , left_op((*left_begin).value())
                );
            }

            detail::transform_union_recurse(
                fusion::next(left_begin)
              , left_end
              , right_begin
              , right_end
              , both_op
              , left_op
              , right_op
              , out
              , fusion::meta::equal_to<left_next_type, LeftEnd>()
              , mpl::true_()
            );
        }

        template<
            typename LeftBegin
          , typename LeftEnd
          , typename RightBegin
          , typename RightEnd
          , typename BothOp
          , typename LeftOp
          , typename RightOp
          , typename Out
        >
        void transform_union_recurse(
            LeftBegin const &left_begin
          , LeftEnd const &left_end
          , RightBegin const &right_begin
          , RightEnd const &right_end
          , BothOp const &both_op
          , LeftOp const &left_op
          , RightOp const &right_op
          , Out &out
          , mpl::false_
          , mpl::false_
        )
        {
            typedef typename fusion::meta::next<LeftBegin>::type left_next_type;
            typedef typename fusion::meta::next<RightBegin>::type right_next_type;

            detail::transform_union_partial(
                *left_begin
              , *right_begin
              , both_op
              , left_op
              , right_op
              , out
            );
            
            BOOST_ASSERT(rrs::is_done_and_empty(*left_begin) || rrs::is_done_and_empty(*right_begin));
            
            if(rrs::is_done_and_empty(*left_begin) && rrs::is_done_and_empty(*right_begin))
            {
                detail::transform_union_recurse(
                    fusion::next(left_begin)
                  , left_end
                  , fusion::next(right_begin)
                  , right_end
                  , both_op
                  , left_op
                  , right_op
                  , out
                  , fusion::meta::equal_to<left_next_type, LeftEnd>()
                  , fusion::meta::equal_to<right_next_type, RightEnd>()
                );
            }
            else if(rrs::is_done_and_empty(*left_begin))
            {
                detail::transform_union_recurse(
                    fusion::next(left_begin)
                  , left_end
                  , right_begin
                  , right_end
                  , both_op
                  , left_op
                  , right_op
                  , out
                  , fusion::meta::equal_to<left_next_type, LeftEnd>()
                  , mpl::false_()
                );
            }
            else
            {
                detail::transform_union_recurse(
                    left_begin
                  , left_end
                  , fusion::next(right_begin)
                  , right_end
                  , both_op
                  , left_op
                  , right_op
                  , out
                  , mpl::false_()
                  , fusion::meta::equal_to<right_next_type, RightEnd>()
                );
            }
        }

        template<
            typename LeftSubSequences
          , typename RightSubSequences
          , typename BothOp
          , typename LeftOp
          , typename RightOp
          , typename Out
        >
        void transform_union(
            LeftSubSequences const &left
          , RightSubSequences const &right
          , BothOp const &both_op
          , LeftOp const &left_op
          , RightOp const &right_op
          , Out &out
        )
        {
            typedef typename fusion::meta::begin<LeftSubSequences>::type left_begin_type;
            typedef typename fusion::meta::end<LeftSubSequences>::type left_end_type;
            typedef typename fusion::meta::begin<RightSubSequences>::type right_begin_type;
            typedef typename fusion::meta::end<RightSubSequences>::type right_end_type;

            detail::transform_union_recurse(
                fusion::begin(left)
              , fusion::end(left)
              , fusion::begin(right)
              , fusion::end(right)
              , both_op
              , left_op
              , right_op
              , out
              , fusion::meta::equal_to<left_begin_type, left_end_type>()
              , fusion::meta::equal_to<right_begin_type, right_end_type>()
            );
        }
    } // namespace detail
}}

#endif // BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_TRANSFORM_UNION_EAN_07_27_2006_HPP
