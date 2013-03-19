///////////////////////////////////////////////////////////////////////////////
/// \file transform.hpp
/// Apply a unary transformation to a single \c InfiniteRangeRunStorage, or a 
/// binary transform to two \c InfiniteRangeRunStorage.
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_ALGORITHM_TRANSFORM_EAN_07_26_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_ALGORITHM_TRANSFORM_EAN_07_26_2006_HPP

#include <boost/version.hpp>
#include <boost/bind.hpp>
#if BOOST_VERSION <= 103400
# include <boost/spirit/fusion/sequence/tie.hpp>
#else
# include <boost/fusion/tuple/tuple_tie.hpp>
#endif
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/utility/subsequence.hpp>
#include <boost/range_run_storage/algorithm/detail/placeholders.hpp>
#include <boost/range_run_storage/algorithm/detail/transform.hpp>
#include <boost/range_run_storage/algorithm/detail/transform_union.hpp>
#include <boost/range_run_storage/algorithm/detail/transform_intersection.hpp>

namespace boost { namespace range_run_storage
{
    namespace rrs = range_run_storage;

    /// \brief Apply a unary transformation to a single \c InfiniteRangeRunStorage, or a 
    /// binary transform to two \c InfiniteRangeRunStorage.
    ///
    /// The three-parameter form of \c transform, <tt>transform(in, unary_op, out)</tt>,
    /// effectively copies the runs from \c in to \c out after applying \c unary_op
    /// to each element value.
    ///
    /// The four-parameter form of \c transform, <tt>transform(left, right, binary_op, out)</tt>,
    /// steps through the series \c left and \c right in lock-step. Where two runs overlap,
    /// the result of <tt>binary_op(<em>left_value</em>, <em>right_value</em>)</tt> is written to \c out.
    /// Where part of a left run does not overlap with a right, the result of 
    /// <tt>binary_op(<em>left_value</em>, rrs::zero(right))</tt> is written to \c out. 
    /// Where part of a right run does not overlap with a left, the result of 
    /// <tt>binary_op(rrs::zero(left), <em>right_value</em>)</tt> is written to \c out. 
    ///
    /// The six-parameter form of \c transform, <tt>transform(left, right, both_op, left_op, right_op, out)</tt>,
    /// steps through the series \c left and \c right in lock-step. Where two runs overlap,
    /// the result of <tt>both_op(<em>left_value</em>, <em>right_value</em>)</tt> is written to \c out.
    /// Where part of a left run does not overlap with a right, the result of 
    /// <tt>left_op(<em>left_value</em>)</tt> is written to \c out. 
    /// Where part of a right run does not overlap with a left, the result of 
    /// <tt>right_op(<em>right_value</em>)</tt> is written to \c out.
    ///
    /// \param[in] in The input sequence.
    /// \param[in] left The left input sequence.
    /// \param[in] right The right input sequence.
    /// \param unary_op A \c UnaryFunction used to transform the elements
    ///         of the input sequence.
    /// \param binary_op A \c BinaryFunction used to generate the output elements 
    ///         from the left and right elements.
    /// \param both_op A \c BinaryFunction used to generate the output elements 
    ///         wherever left and right runs overlap.
    /// \param left_op A \c UnaryFunction used to generate the output elements 
    ///         wherever a left run does not overlap with a right run.
    /// \param right_op A \c UnaryFunction used to generate the output elements 
    ///         wherever a right run does not overlap with a left run.
    /// \param[out] out An \c OrderedInserter into which the resulting sequence
    ///         is written.
    /// \pre \c In is a model of \c InfiniteRangeRunStorage.
    /// \pre \c Left is a model of \c InfiniteRangeRunStorage.
    /// \pre \c Right is a model of \c InfiniteRangeRunStorage.
    /// \pre \c UnaryOp is a model of \c UnaryFunction.
    /// \pre \c LeftOp is a model of \c UnaryFunction.
    /// \pre \c RightOp is a model of \c UnaryFunction.
    /// \pre \c BothOp is a model of \c BinaryFunction.
    /// \pre \c BinaryOp is a model of \c BinaryFunction.
    /// \pre \c Out is a model of \c OrderedInserter.
    template<typename In, typename UnaryOp, typename Out>
    Out &transform(In &in, UnaryOp const &unary_op, Out &out)
    {
        return detail::transform_impl(in, unary_op, out);
    }

    /// \overload
    ///
    template<typename In, typename UnaryOp, typename Out>
    Out &transform(In const &in, UnaryOp const &unary_op, Out &out)
    {
        return detail::transform_impl(in, unary_op, out);
    }

    /// \overload
    ///
    template<typename Left, typename Right, typename BothOp, typename LeftOp, typename RightOp, typename Out>
    Out &transform(
        Left const &left
      , Right const &right
      , BothOp const &both_op
      , LeftOp const &left_op
      , RightOp const &right_op
      , Out &out
    )
    {
        using concepts::InfiniteRangeRunStorage;

        typedef typename InfiniteRangeRunStorage<Left const>::value_type left_value_type;
        typedef typename InfiniteRangeRunStorage<Left const>::pre_run_type left_pre_run_type;
        typedef typename InfiniteRangeRunStorage<Left const>::post_run_type left_post_run_type;
        typedef typename InfiniteRangeRunStorage<Right const>::value_type right_value_type;
        typedef typename InfiniteRangeRunStorage<Right const>::pre_run_type right_pre_run_type;
        typedef typename InfiniteRangeRunStorage<Right const>::post_run_type right_post_run_type;

        run_value_subsequence<left_pre_run_type, left_value_type> left_pre_(rrs::pre_run(left), rrs::pre_value(left));
        run_value_subsequence<left_post_run_type, left_value_type> left_post_(rrs::post_run(left), rrs::post_value(left));
        run_value_subsequence<right_pre_run_type, right_value_type> right_pre_(rrs::pre_run(right), rrs::pre_value(right));
        run_value_subsequence<right_post_run_type, right_value_type> right_post_(rrs::post_run(right), rrs::post_value(right));

        range_run_subsequence<Left> left_(left);
        range_run_subsequence<Right> right_(right);

        detail::transform_union(
            fusion::tie(left_pre_, left_, left_post_)
          , fusion::tie(right_pre_, right_, right_post_)
          , both_op     // binary op
          , left_op     // left unary op
          , right_op    // right unary op
          , out
        );

        return out;
    }

    /// \overload
    ///
    template<typename Left, typename Right, typename BinOp, typename Out>
    Out &transform(Left const &left, Right const &right, BinOp const &binary_op, Out &out)
    {
        using concepts::InfiniteRangeRunStorage;

        typedef typename InfiniteRangeRunStorage<Left const>::value_type left_value_type;
        typedef typename InfiniteRangeRunStorage<Right const>::value_type right_value_type;

        left_value_type const &left_zero(rrs::zero(left));
        right_value_type const &right_zero(rrs::zero(right));

        typedef typename result_of<BinOp(left_value_type, right_value_type)>::type result_type;

        rrs::transform(
            left
          , right
          , binary_op                                               // binary op
          , boost::bind<result_type>(binary_op, ::_1, right_zero)   // left unary op
          , boost::bind<result_type>(binary_op, left_zero, ::_1)    // right unary op
          , out
        );

        return out;
    }

    /// INTERNAL ONLY
    // transform intersection
    template<typename Left, typename Right, typename BinOp, typename Out>
    Out &transform(Left const &left, Right const &right, BinOp const &binary_op, placeholders_::skip_ const &, placeholders_::skip_ const &, Out &out)
    {
        using concepts::InfiniteRangeRunStorage;

        typedef typename InfiniteRangeRunStorage<Left const>::value_type left_value_type;
        typedef typename InfiniteRangeRunStorage<Left const>::pre_run_type left_pre_run_type;
        typedef typename InfiniteRangeRunStorage<Left const>::post_run_type left_post_run_type;
        typedef typename InfiniteRangeRunStorage<Right const>::value_type right_value_type;
        typedef typename InfiniteRangeRunStorage<Right const>::pre_run_type right_pre_run_type;
        typedef typename InfiniteRangeRunStorage<Right const>::post_run_type right_post_run_type;

        run_value_subsequence<left_pre_run_type, left_value_type> left_pre_(rrs::pre_run(left), rrs::pre_value(left));
        run_value_subsequence<left_post_run_type, left_value_type> left_post_(rrs::post_run(left), rrs::post_value(left));
        run_value_subsequence<right_pre_run_type, right_value_type> right_pre_(rrs::pre_run(right), rrs::pre_value(right));
        run_value_subsequence<right_post_run_type, right_value_type> right_post_(rrs::post_run(right), rrs::post_value(right));

        range_run_subsequence<Left> left_(left);
        range_run_subsequence<Right> right_(right);

        detail::transform_intersection(
            fusion::tie(left_pre_, left_, left_post_)
          , fusion::tie(right_pre_, right_, right_post_)
          , binary_op
          , out
        );

        return out;
    }

    template<typename In, typename UnaryOp>
    In &transform_inplace(In &in, UnaryOp const &unary_op)
    {
        return detail::transform_inplace_impl(in, unary_op);
    }
}}

#endif
