// Copyright Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAIL_NEVER_TRUE_EAN_2006_10_15_HPP
#define BOOST_DETAIL_NEVER_TRUE_EAN_2006_10_15_HPP

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/long.hpp>

namespace boost { namespace parameter
{
    struct parameters_tag;
    struct parameters_iterator_tag;

    template<typename Parameters, long I>
    struct at_impl;

#define BOOST_PARAMETERS_DEFINE_AT_IMPL(z, n, _)\
    template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PARAMETER_MAX_ARITY, typename A)>\
    struct at_impl<parameters<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PARAMETER_MAX_ARITY, A)>, n>\
    {\
        typedef BOOST_PP_CAT(A, n) type;\
    };\
    /**/

    BOOST_PP_REPEAT(
        BOOST_PARAMETER_MAX_ARITY
      , BOOST_PARAMETERS_DEFINE_AT_IMPL
      , ~
    )

    template<typename Parameters>
    struct size_impl;

#define BOOST_PARAMETERS_DEFINE_SIZE_IMPL(z, n, _)\
    template<BOOST_PP_ENUM_PARAMS_Z(z, n, typename A)>\
    struct size_impl<parameters<BOOST_PP_ENUM_PARAMS_Z(z, n, A)> >\
    {\
        typedef mpl::long_<n> type;\
    };\
    /**/

    BOOST_PP_REPEAT_FROM_TO(
        1
      , BOOST_PP_INC(BOOST_PARAMETER_MAX_ARITY)
      , BOOST_PARAMETERS_DEFINE_SIZE_IMPL
      , ~
    )

    template<typename Parameters, long I>
    struct parameters_iterator
    {
        typedef mpl::random_access_iterator_tag category;
        typedef parameters_iterator_tag tag;
        typedef Parameters parameters_type;
        BOOST_STATIC_CONSTANT(long, index = I);

        typedef typename at_impl<Parameters, I>::type type;
        typedef parameters_iterator<Parameters, I+1> next;
        typedef parameters_iterator<Parameters, I-1> prior;
    };

    template<typename Iterator, long D>
    struct advance_impl
    {
        typedef parameters_iterator<
            typename Iterator::parameters_type
          , Iterator::index + D
        > type;
    };

    template<typename From, typename To>
    struct distance_impl
    {
        typedef mpl::long_<To::index - From::index> type;
    };
}}

namespace boost { namespace mpl
{
    template<BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, typename A)>
    struct sequence_tag<
        parameter::parameters<BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, A)>
    >
    {
        typedef parameter::parameters_tag type;
    };

    template<>
    struct begin_impl<parameter::parameters_tag>
    {
        template<typename Parameters>
        struct apply
        {
            typedef parameter::parameters_iterator<Parameters, 0> type;
        };
    };

    template<>
    struct end_impl<parameter::parameters_tag>
    {
        template<typename Parameters>
        struct apply
        {
            typedef parameter::parameters_iterator<
                Parameters
              , parameter::size_impl<Parameters>::type::value
            > type;
        };
    };

    template<>
    struct at_impl<parameter::parameters_tag>
    {
        template<typename Parameters, typename N>
        struct apply
        {
            typedef typename parameter::at_impl<Parameters, N::value>::type type;
        };
    };

    template<>
    struct advance_impl<parameter::parameters_iterator_tag>
    {
        template<typename Iterator, typename N>
        struct apply
        {
            typedef typename parameter::advance_impl<Iterator, N::value>::type type;
        };
    };

    template<>
    struct distance_impl<parameter::parameters_iterator_tag>
    {
        template<typename From, typename To>
        struct apply
        {
            typedef typename parameter::distance_impl<From, To>::type type;
        };
    };

}}

#endif
