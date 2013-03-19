///////////////////////////////////////////////////////////////////////////////
/// \file numeric.hpp
/// Defined the mathematical operator s on time series
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_NUMERIC_EAN_04_27_2006
#define BOOST_TIME_SERIES_NUMERIC_NUMERIC_EAN_04_27_2006

#include <utility>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/numeric/functional.hpp>
#include <boost/range_run_storage/algorithm/transform.hpp>
#include <boost/time_series/time_series_fwd.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/traits/generate_series.hpp>
#include <boost/time_series/traits/promotion.hpp>
#include <boost/time_series/scaled_series.hpp>
#include <boost/time_series/numeric/detail/zero.hpp>
#include <boost/detail/construct.hpp>

namespace boost { namespace time_series
{
    namespace detail
    {
        namespace rrs = range_run_storage;

        using concepts::Run;
        using concepts::TimeSeries;
        using concepts::Mutable_TimeSeries;

        template<typename Out, typename Left, typename Right, typename BinOp>
        Out &run_bin_op_union(Out &out, Left &left, Right &right, BinOp const &binop)
        {
            typename Mutable_TimeSeries<Out>::ordered_inserter_type
                o(rrs::ordered_inserter(out));

            rrs::commit(
                rrs::transform(left, right, binop, o)
            );

            return out;
        }

        // For operations that are only defined
        template<typename Out, typename Left, typename Right, typename BinOp>
        Out &run_bin_op_intersection(Out &out, Left &left, Right &right, BinOp const &binop)
        {
            typename Mutable_TimeSeries<Out>::ordered_inserter_type
                o(rrs::ordered_inserter(out));

            rrs::commit(
                rrs::transform(left, right, binop, rrs::skip, rrs::skip, o)
            );

            return out;
        }

        // operation traits
        template<typename S1, typename S2, typename Gen, typename Op, typename Disc>
        struct operation_traits_base
        {
            typedef typename TimeSeries<S1>::storage_category storage1_tag;
            typedef typename TimeSeries<S2>::storage_category storage2_tag;
            typedef typename mpl::apply<Gen, storage1_tag, storage2_tag>::type storage_category;

            typedef typename TimeSeries<S1>::value_type value1_type;
            typedef typename TimeSeries<S2>::value_type value2_type;
            typedef typename result_of<Op(value1_type, value2_type)>::type value_type;

            typedef typename TimeSeries<S1>::offset_type offset1_type;
            typedef typename TimeSeries<S2>::offset_type offset2_type;
            BOOST_MPL_ASSERT((is_same<offset1_type, offset2_type>)); // is there a better option?
            typedef offset1_type offset_type;
            
            typedef typename traits::generate_series<
                storage_category, value_type, Disc, offset_type
            >::type result_type;
        };

        template<typename S1, typename S2, typename Gen, typename Op
          , typename Disc1 = typename TimeSeries<S1>::discretization_type
          , typename Disc2 = typename TimeSeries<S2>::discretization_type
        >
        struct operation_traits
        {};

        template<typename S1, typename S2, typename Gen, typename Op, typename Disc>
        struct operation_traits<S1, S2, Gen, Op, time_series::any, Disc>
          : operation_traits_base<S1, S2, Gen, Op, Disc>
        {};

        template<typename S1, typename S2, typename Gen, typename Op, typename Disc>
        struct operation_traits<S1, S2, Gen, Op, Disc, time_series::any>
          : operation_traits_base<S1, S2, Gen, Op, Disc>
        {};

        template<typename S1, typename S2, typename Gen, typename Op>
        struct operation_traits<S1, S2, Gen, Op, time_series::any, time_series::any>
          : operation_traits_base<S1, S2, Gen, Op, time_series::any>
        {};

        template<typename S1, typename S2, typename Gen, typename Op, typename Disc>
        struct operation_traits<S1, S2, Gen, Op, Disc, Disc>
          : operation_traits_base<S1, S2, Gen, Op, Disc>
        {};

        template<typename Left, typename Right, typename Op>
        typename result_of<Op(
            typename TimeSeries<Left>::value_type const &
          , typename TimeSeries<Right>::value_type const &
        )>::type
        make_zero(Left &left, Right &right, Op op)
        {
            typename TimeSeries<Left>::value_type const &left_zero = rrs::zero(left);
            typename TimeSeries<Right>::value_type const &right_zero = rrs::zero(right);
            return op(left_zero, right_zero);
        }

        template<typename Discretization>
        Discretization make_discretization(Discretization left, Discretization right)
        {
            BOOST_ASSERT(left == right);
            return left;
        }

        template<typename Discretization>
        Discretization make_discretization(Discretization left, time_series::any)
        {
            return left;
        }

        template<typename Discretization>
        Discretization make_discretization(time_series::any, Discretization right)
        {
            return right;
        }

        inline time_series::any make_discretization(time_series::any, time_series::any)
        {
            return time_series::any();
        }

    } // namespace detail

    using mpl::_;

    ///////////////////////////////////////////////////////////////////////////
    // operator +
    template<typename Left, typename Right>
    typename detail::operation_traits<Left, Right, traits::plus<_, _>, numeric::op::plus>::result_type
    operator +(time_series_base<Left> const &left, time_series_base<Right> const &right)
    {
        typedef detail::operation_traits<Left, Right, traits::plus<_, _>, numeric::op::plus> op_traits;
        typedef typename op_traits::result_type result_type;

        result_type result(constructors::construct<result_type>(
            time_series::discretization
              = detail::make_discretization(left.cast().discretization(), right.cast().discretization())
          , time_series::zero
              = detail::make_zero(left.cast(), right.cast(), numeric::plus)
        ));

        return detail::run_bin_op_union(result, left.cast(), right.cast(), numeric::plus);
    }

    ///////////////////////////////////////////////////////////////////////////
    // operator -
    template<typename Left, typename Right>
    typename detail::operation_traits<Left, Right, traits::minus<_, _>, numeric::op::minus>::result_type
    operator -(time_series_base<Left> const &left, time_series_base<Right> const &right)
    {
        typedef detail::operation_traits<Left, Right, traits::minus<_, _>, numeric::op::minus> op_traits;
        typedef typename op_traits::result_type result_type;

        result_type result(constructors::construct<result_type>(
            time_series::discretization
              = detail::make_discretization(left.cast().discretization(), right.cast().discretization())
          , time_series::zero
              = detail::make_zero(left.cast(), right.cast(), numeric::minus)
        ));

        return detail::run_bin_op_union(result, left.cast(), right.cast(), numeric::minus);
    }

    ///////////////////////////////////////////////////////////////////////////
    // operator *
    template<typename Left, typename Right>
    typename detail::operation_traits<Left, Right, traits::multiplies<_, _>, numeric::op::multiplies>::result_type
    operator *(time_series_base<Left> const &left, time_series_base<Right> const &right)
    {
        typedef detail::operation_traits<Left, Right, traits::multiplies<_, _>, numeric::op::multiplies> op_traits;
        typedef typename op_traits::result_type result_type;

        result_type result(constructors::construct<result_type>(
            time_series::discretization
              = detail::make_discretization(left.cast().discretization(), right.cast().discretization())
          , time_series::zero
              = detail::make_zero(left.cast(), right.cast(), numeric::multiplies)
        ));

        return detail::run_bin_op_intersection(result, left.cast(), right.cast(), numeric::multiplies);
    }

    ///////////////////////////////////////////////////////////////////////////
    // operator /
    template<typename Left, typename Right>
    typename detail::operation_traits<Left, Right, traits::divides<_, _>, numeric::op::divides>::result_type
    operator /(time_series_base<Left> const &left, time_series_base<Right> const &right)
    {
        typedef detail::operation_traits<Left, Right, traits::divides<_, _>, numeric::op::divides> op_traits;
        typedef typename op_traits::result_type result_type;

        // BUGBUG possible divide by zero, so use multiplies instead of divides here ...
        result_type result(constructors::construct<result_type>(
            time_series::discretization
              = detail::make_discretization(left.cast().discretization(), right.cast().discretization())
          , time_series::zero
              = detail::make_zero(left.cast(), right.cast(), numeric::multiplies)
        ));

        return detail::run_bin_op_intersection(result, left.cast(), right.cast(), numeric::divides);
    }

    ///////////////////////////////////////////////////////////////////////////
    // operator +=
    template<typename Left, typename Right>
    Left &operator +=(time_series_base<Left> &left, time_series_base<Right> const &right)
    {
        BOOST_MPL_ASSERT((is_same<
            typename concepts::TimeSeries<Left>::discretization_type
          , typename concepts::TimeSeries<Right>::discretization_type
        >));

        // TODO also add the series zero
        BOOST_ASSERT(left.cast().discretization() == right.cast().discretization());
        return detail::run_bin_op_union(left.cast(), left.cast(), right.cast(), numeric::plus);
    }

    ///////////////////////////////////////////////////////////////////////////
    // operator -=
    template<typename Left, typename Right>
    Left &operator -=(time_series_base<Left> &left, time_series_base<Right> const &right)
    {
        BOOST_MPL_ASSERT((is_same<
            typename concepts::TimeSeries<Left>::discretization_type
          , typename concepts::TimeSeries<Right>::discretization_type
        >));

        // TODO also subtract the series zero
        BOOST_ASSERT(left.cast().discretization() == right.cast().discretization());
        return detail::run_bin_op_union(left.cast(), left.cast(), right.cast(), numeric::minus);
    }

    ///////////////////////////////////////////////////////////////////////////
    // operator *=
    template<typename Left, typename Right>
    Left &operator *=(time_series_base<Left> &left, time_series_base<Right> const &right)
    {
        BOOST_MPL_ASSERT((is_same<
            typename concepts::TimeSeries<Left>::discretization_type
          , typename concepts::TimeSeries<Right>::discretization_type
        >));

        BOOST_ASSERT(left.cast().discretization() == right.cast().discretization());

        // BUGBUG not optimal
        Left result(constructors::construct<Left>(
            time_series::discretization
              = detail::make_discretization(left.cast().discretization(), right.cast().discretization())
          , time_series::zero
              = detail::make_zero(left.cast(), right.cast(), numeric::multiplies)
        ));

        left.cast().swap(detail::run_bin_op_intersection(result, left.cast(), right.cast(), numeric::multiplies));
        return left.cast();
    }

    ///////////////////////////////////////////////////////////////////////////
    // operator /=
    template<typename Left, typename Right>
    Left &operator /=(time_series_base<Left> &left, time_series_base<Right> const &right)
    {
        BOOST_MPL_ASSERT((is_same<
            typename concepts::TimeSeries<Left>::discretization_type
          , typename concepts::TimeSeries<Right>::discretization_type
        >));

        BOOST_MPL_ASSERT((traits::is_valid_divisor<
            typename concepts::TimeSeries<Right>::storage_category
        >));

        BOOST_ASSERT(left.cast().discretization() == right.cast().discretization());

        // BUGBUG not optimal
        // BUGBUG possible divide by zero, so use multiplies instead of divides here ...
        Left result(constructors::construct<Left>(
            time_series::discretization
              = detail::make_discretization(left.cast().discretization(), right.cast().discretization())
          , time_series::zero
              = detail::make_zero(left.cast(), right.cast(), numeric::multiplies)
        ));

        left.cast().swap(detail::run_bin_op_intersection(result, left.cast(), right.cast(), numeric::divides));
        return left.cast();
    }

    ///////////////////////////////////////////////////////////////////////////
    // operator * (with scalar)
    template<typename Series, typename Factor>
    typename disable_if<
        traits::is_time_series<Factor>
      , scaled_series<Series const, Factor> const
    >::type
    operator *(time_series_base<Series> const &left, Factor const &right)
    {
        return scaled_series<Series const, Factor>(left.cast(), right);
    }

    ///////////////////////////////////////////////////////////////////////////
    // operator * (with scalar)
    template<typename Factor, typename Series>
    typename disable_if<
        traits::is_time_series<Factor>
      , scaled_series<Series const, Factor> const
    >::type
    operator *(Factor const &left, time_series_base<Series> const &right)
    {
        return scaled_series<Series const, Factor>(right.cast(), left);
    }

    ///////////////////////////////////////////////////////////////////////////
    // operator *= (with scalar)
    template<typename Series, typename Factor>
    typename disable_if<
        traits::is_time_series<Factor>
      , Series &
    >::type
    operator *=(time_series_base<Series> &left, Factor const &right)
    {
        left.cast() = left.cast() * right;
        return left.cast();
    }

}} // namespace boost::time_series

#endif
