///////////////////////////////////////////////////////////////////////////////
/// \file promotion.hpp
/// Defines the result matrices of adding, subtracting, multiplying and
/// dividing the various flavors of time series
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_TRAITS_PROMOTION_EAN_04_27_2006
#define BOOST_TIME_SERIES_TRAITS_PROMOTION_EAN_04_27_2006

#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/time_series/time_series_fwd.hpp>

namespace boost { namespace time_series { namespace traits
{
    // Addition / Subtraction promotion rules
    template<typename Tag>
    struct is_delta
      : is_same<Tag, delta_storage_tag>
    {};

    template<typename Tag>
    struct is_dense
      : is_same<Tag, dense_storage_tag>
    {};

    template<typename Tag>
    struct is_constant
      : is_same<Tag, constant_storage_tag>
    {};

    template<typename Tag>
    struct is_piecewise_constant
      : is_same<Tag, piecewise_constant_storage_tag>
    {};

    template<typename Tag>
    struct is_piecewise
      : mpl::or_<
            is_same<Tag, characteristic_storage_tag>
          , is_same<Tag, heaviside_storage_tag>
          , is_same<Tag, inverse_heaviside_storage_tag>
          , mpl::or_<is_constant<Tag>, is_piecewise_constant<Tag> >
        >
    {};

    template<typename Tag>
    struct is_piecewise<scaled_storage_tag<Tag> >
      : is_piecewise<Tag>
    {};

    template<typename Tag>
    struct is_sparse
      : mpl::or_<is_same<Tag, sparse_storage_tag>, is_same<Tag, delta_storage_tag> >
    {};

    template<typename Tag>
    struct is_sparse<scaled_storage_tag<Tag> >
      : is_sparse<Tag>
    {};

    // 1) Adding anything to a dense array gives a dense array
    // 2) Adding two of (piecewise_constant, heaviside, inverse heaviside,
    //      characteristic or constant) yields a piecewise_constant.
    // 3) Adding two of (sparse or delta) yields a sparse array
    template<typename Left, typename Right>
    struct plus
      : mpl::eval_if<
            mpl::or_<is_dense<Left>, is_dense<Right> >
          , mpl::identity<dense_storage_tag>
          , mpl::eval_if<
                mpl::or_<is_piecewise<Left>, is_piecewise<Right> >
              , mpl::identity<piecewise_constant_storage_tag>
              , mpl::if_<
                    mpl::or_<is_sparse<Left>, is_sparse<Right> >
                  , sparse_storage_tag
                  , void
                >
            >
        >
    {};

    // Special case: adding two constant_series gives a constant series
    template<>
    struct plus<constant_storage_tag, constant_storage_tag>
    {
        typedef constant_storage_tag type;
    };

    // Subtraction follows the same rules as addition
    template<typename Left, typename Right>
    struct minus
      : plus<Left, Right>
    {};


    template<typename Tag>
    struct is_unit
      : mpl::or_<
            is_same<Tag, heaviside_storage_tag>
          , is_same<Tag, inverse_heaviside_storage_tag>
          , is_same<Tag, characteristic_storage_tag>
          , is_same<Tag, delta_storage_tag>
        >
    {};

    template<typename Tag>
    struct is_unit<scaled_storage_tag<Tag> >
      : is_unit<Tag>
    {};

    // Matthias
    // heaviside series, inverse heaviside series, characteristic series and
    // their scaled variants:
    // These act like filters on the range of allowed indices. Thus the return type of multiplying
    // them with any other series is usually of the type of that other series.
    // The only special cases are products of these series with themselves. Multiplying
    // two series of the same type gives a series of that type (e.g. heaviside series multiplied
    // with heaviside series gives a heaviside series). Multiplying two different
    // types always gives a characteristic series (e.g. multiplying heaviside series with
    // inverse heaviside series). The range of allowed indices is the intersection of the runs.
    // If one of the series is scaled (and not a unit series), the result will also be scaled.
    template<
        typename Left
      , typename Right
      , typename IsLeftUnit = typename is_unit<Left>::type
      , typename IsRightUnit = typename is_unit<Right>::type
    >
    struct unit_mult;

    template<typename Unit>
    struct unit_mult<Unit, Unit, mpl::true_, mpl::true_>
    {
        typedef Unit type;
    };

    template<typename Left, typename Right>
    struct unit_mult<Left, Right, mpl::true_, mpl::true_>
    {
        typedef characteristic_storage_tag type;
    };

    template<typename Left, typename Right>
    struct unit_mult<Left, Right, mpl::false_, mpl::true_>
    {
        typedef Left type;
    };

    template<typename Left, typename Right>
    struct unit_mult<Left, Right, mpl::true_, mpl::false_>
    {
        typedef Right type;
    };

    template<typename Left, typename Right>
    struct multiplies
      : mpl::eval_if<
            mpl::or_<is_delta<Left>, is_delta<Right> >
          , scaled_storage_tag<delta_storage_tag>
          , mpl::eval_if<
                mpl::or_<is_sparse<Left>, is_sparse<Right> >
              , mpl::identity<sparse_storage_tag>
              , mpl::eval_if<
                    mpl::or_<is_unit<Left>, is_unit<Right> >
                  , unit_mult<Left, Right>
                  , mpl::if_<
                        mpl::and_<is_piecewise_constant<Left>, is_piecewise_constant<Right> >
                      , piecewise_constant_storage_tag
                      , dense_storage_tag
                    >
                >
            >
        >
    {};

    // Special case: multiplying any unit array by a constant array
    // yields a scaled variant of the unit array. Otherwise, it leaves
    // other types unchanged.
    template<typename Left>
    struct multiplies<Left, constant_storage_tag>
      : mpl::eval_if<
            is_unit<Left>
          , scaled_storage_tag<Left>
          , mpl::identity<Left>
        >
    {};

    template<typename Right>
    struct multiplies<constant_storage_tag, Right>
      : mpl::eval_if<
            is_unit<Right>
          , scaled_storage_tag<Right>
          , mpl::identity<Right>
        >
    {};

    template<>
    struct multiplies<constant_storage_tag, constant_storage_tag>
    {
        typedef constant_storage_tag type;
    };

    template<>
    struct multiplies<delta_storage_tag, delta_storage_tag>
    {
        typedef delta_storage_tag type;
    };

    // Not all series types can be used as the divisor in series division.
    template<typename Divisor>
    struct is_valid_divisor
      : mpl::or_<
            is_same<Divisor, dense_storage_tag>
          , is_same<Divisor, constant_storage_tag>
          , is_same<Divisor, piecewise_constant_storage_tag>
        >
    {};

    // Matthias:
    // Note that dividing by anything but a dense series, constant series or
    // piecewise constant series is generally not well defined, and should
    // probably not be implemented at all for these types
    template<typename Left, typename Right>
    struct divides
      : multiplies<Left, Right>
    {
        BOOST_MPL_ASSERT((is_valid_divisor<Right>));
    };

}}} // boost::time_series::traits

#endif
