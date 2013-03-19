///////////////////////////////////////////////////////////////////////////////
/// \file construct.hpp
/// Generic object constuction and constructor delegation using Boost.Parameters
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONSTRUCTORS_CONSTRUCT_EAN_2006_09_22
#define BOOST_CONSTRUCTORS_CONSTRUCT_EAN_2006_09_22

#include <boost/version.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/binding.hpp>

#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>

#if BOOST_VERSION > 103301
# include <boost/parameter/preprocessor.hpp>
#else
namespace boost { namespace parameter { namespace aux
{
    #if BOOST_VERSION == 103301
    // Metafunction that returns an ArgumentPack.
    template <
        class Parameters
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
            BOOST_PARAMETER_MAX_ARITY, class A, = void_ BOOST_PP_INTERCEPT
        )
    >
    struct argument_pack
      : Parameters::template argument_pack<BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, A)>
    {};
    #else
    template <
        class Parameters
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
            BOOST_PARAMETER_MAX_ARITY, class A, = void_ BOOST_PP_INTERCEPT
        )
    >
    struct argument_pack
    {
        typedef typename aux::make_arg_list<
            typename BOOST_PARAMETER_build_arg_list(
                BOOST_PARAMETER_MAX_ARITY, aux::make_items, typename Parameters::parameter_spec, A
            )::type
          , typename Parameters::deduced_list
          , aux::tag_keyword_arg
        >::type result;

        typedef typename mpl::first<result>::type type;
    };
    #endif
}}}
#endif

namespace boost { namespace constructors
{

#define BOOST_CONSTRUCT_positional_keyword(z, n, d)\
    BOOST_PARAMETER_KEYWORD(tag, BOOST_PP_CAT(_, n))

    BOOST_PP_REPEAT(
        BOOST_PARAMETER_MAX_ARITY
      , BOOST_CONSTRUCT_positional_keyword
      , _
    )

    template<typename T>
    struct is_arg_pack
      : mpl::false_
    {};

    template<typename T>
    struct is_arg_pack<T const>
      : is_arg_pack<T>
    {};

    template<typename T>
    struct is_arg_pack<T volatile>
      : is_arg_pack<T>
    {};

    template<typename T>
    struct is_arg_pack<T const volatile>
      : is_arg_pack<T>
    {};

    template<typename Head, typename Tail>
    struct is_arg_pack<parameter::aux::arg_list<Head, Tail> >
      : mpl::true_
    {};

    template<>
    struct is_arg_pack<parameter::aux::empty_arg_list>
      : mpl::true_
    {};

    namespace impl
    {
        template< typename T >
        struct tag
        {
            typedef void type;
        };

        template< typename T >
        struct tag< T const >
          : tag< T >
        {};

        template< typename T >
        struct tag< T volatile >
          : tag< T >
        {};

        template< typename T >
        struct tag< T const volatile >
          : tag< T >
        {};

        // unpacks an argument pack into a constructor call
        template<typename T, long Size>
        struct unpack_args;

#define BOOST_CONSTRUCT_unpack_args(z, n, _)\
        template<typename T>\
        struct unpack_args<T, n>\
        {\
            typedef T result_type;\
            template<typename Args>\
            T operator()(Args const &args) const\
            {\
                return T(\
                    BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, args[ constructors::_, ] BOOST_PP_INTERCEPT)\
                );\
            }\
        };\
        /**/

        BOOST_PP_REPEAT(
            BOOST_PARAMETER_MAX_ARITY
          , BOOST_CONSTRUCT_unpack_args
          , _
        )

        template< typename T, typename Tag = typename tag< T >::type >
        struct construct;

        template< typename T>
        struct construct<T, void>
        {
            typedef parameter::parameters< 
                BOOST_PP_ENUM_BINARY_PARAMS(
                    BOOST_PARAMETER_MAX_ARITY
                  , parameter::optional< constructors::tag::_
                  , > BOOST_PP_INTERCEPT
                )
            > args_type;

            typedef T result_type;

            template<typename Args>
            T operator()(Args const & args) const
            {
                return unpack_args<T, mpl::size<Args>::type::value>()(args);
            }
        };

        template<typename T>
        inline parameter::keyword<T> const &make_keyword()
        {
            static parameter::keyword<T> const key = {};
            return key;
        }

        struct msvc_result_repack_args
        {
            template< typename F >
            struct result;
            
            template< typename This, typename Args >
            struct result< This( Args ) >
            {
                typedef Args type;
            };
        };

        template< typename To, typename Tags, long Size = mpl::size< Tags >/*::type*/::value >
        struct repack_args
          : msvc_result_repack_args
        {
            template< typename Args >
            Args const &operator()( Args const & args ) const
            {
                return args;
            }
        };

#define BOOST_CONSTRUCT_binding_fun(z, n, _)\
      , typename parameter::aux::tag<\
            typename mpl::at_c< Tags, n >::type\
          , typename remove_reference<\
                typename parameter::binding< Args, typename mpl::at_c< Tags, n >::type >::type\
            >::type\
        >::type const\
        /**/

#define BOOST_CONSTRUCT_keyword_fun(z, n, _)\
        BOOST_PP_COMMA_IF(n)\
        impl::make_keyword< typename mpl::at_c< Tags, n >::type >()\
            = args[ impl::make_keyword< typename mpl::at_c< Tags, n >::type >() ]\
        /**/

#define BOOST_CONSTRUCT_repack_args(z, n, _)\
        template< typename To, typename Tags >\
        struct repack_args< To, Tags, n >\
        {\
            template< typename F >\
            struct result;\
            template< typename This, typename Args >\
            struct result< This( Args ) >\
              : parameter::aux::argument_pack< To \
                    BOOST_PP_REPEAT_ ## z(n, BOOST_CONSTRUCT_binding_fun, _)\
                >\
            {};\
            template< typename Args >\
            typename result< repack_args( Args ) >::type\
            operator()( Args const & args ) const\
            {\
                return To()(\
                    BOOST_PP_REPEAT_ ## z(n, BOOST_CONSTRUCT_keyword_fun, _)\
                );\
            }\
        };\
        /**/

        //BOOST_PP_REPEAT(
        //    BOOST_PARAMETER_MAX_ARITY
        //  , BOOST_CONSTRUCT_repack_args
        //  , _
        //)

        template<typename T, typename Tags>
        struct msvc_result_forward_construct
        {
            typedef repack_args<typename impl::construct< T >::args_type, Tags> repacker;

            template< typename F >
            struct result;
            
            template< typename This, typename Args >
            struct result< This(Args) >
              : result_of<
                    impl::construct< T >(typename result_of< repacker(Args) >::type)
                >
            {};
        };

        template<typename T, typename Tags>
        struct forward_construct
          : msvc_result_forward_construct<T, Tags>
        {
            typedef repack_args<typename impl::construct< T >::args_type, Tags> repacker;

            template< typename Args >
            typename msvc_result_forward_construct<T, Tags>
                ::template result< forward_construct(Args) >::type
            operator()( Args const & args ) const
            {
                return impl::construct< T >()( repacker()( args ) );
            }
        };

    }

#define BOOST_CONSTRUCTORS_construct_fun(z, n, _)\
    template< typename T BOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, typename A) >\
    typename result_of< \
        impl::construct< T >(\
            typename parameter::aux::argument_pack<typename impl::construct< T >::args_type\
                BOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, A)\
            >::type\
        )\
    >::type\
    construct( BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, A, &a) )\
    {\
        typedef typename impl::construct< T >::args_type args_type;\
        return impl::construct< T >()( args_type()( BOOST_PP_ENUM_PARAMS_Z(z, n, a) ) );\
    }\
    /**/

    BOOST_PP_REPEAT(
        BOOST_PARAMETER_MAX_ARITY
      , BOOST_CONSTRUCTORS_construct_fun
      , _
    )

    template< typename T, typename Args >
    typename result_of< impl::forward_construct< T, Args >( Args ) >::type
    forward_construct( Args const & args )
    {
        return impl::forward_construct< T, Args >()( args );
    }

    template< typename T, typename Filter, typename Args >
    typename result_of<
        impl::forward_construct<T, mpl::filter_view< Args, Filter > >( Args )
    >::type
    forward_construct_filter( Args const & args )
    {
        return impl::forward_construct< T, mpl::filter_view< Args, Filter > >()( args );
    }

    struct arg_pack_construct
    {
        template<typename F>
        struct result;

        template<typename This, typename Args>
        struct result< This( Args ) >
        {
            typedef Args type;
        };

        template<typename Args>
        Args const &operator ()(Args const &args) const
        {
            return args;
        }
    };

    template<typename T>
    struct default_construct
    {
        typedef parameter::parameters<void> args_type;

        typedef T result_type;

        template<typename Args>
        T operator ()(Args const &) const
        {
            return T();
        }
    };

}}

#endif
