// Copyright Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAIL_OVERLOAD_EAN_2006_10_15_HPP
#define BOOST_DETAIL_OVERLOAD_EAN_2006_10_15_HPP

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/inherit_linearly.hpp>

namespace boost { namespace detail
{
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1310)
    // Hack for MSVC, which can't handle the more general overload<>
    template<typename Unary, typename Binary>
    struct overload1_2
    {
        template<typename Sig>
        struct result;

        template<typename This, typename A>
        struct result<This(A)>
          : Unary::template result<This(A)>
        {};

        template<typename This, typename A, typename B>
        struct result<This(A, B)>
          : Binary::template result<This(A, B)>
        {};

        template<typename A>
        typename Unary::template result<Unary(A &)>::type
        operator()(A &a) const
        {
            return Unary()(a);
        }

        template<typename A>
        typename Unary::template result<Unary(A const &)>::type
        operator()(A const &a) const
        {
            return Unary()(a);
        }

        template<typename A, typename B>
        typename Binary::template result<Binary(A &, B &)>::type
        operator()(A &a, B &b) const
        {
            return Binary()(a, b);
        }

        template<typename A, typename B>
        typename Binary::template result<Binary(A const &, B &)>::type
        operator()(A const &a, B &b) const
        {
            return Binary()(a, b);
        }

        template<typename A, typename B>
        typename Binary::template result<Binary(A &, B const &)>::type
        operator()(A &a, B const &b) const
        {
            return Binary()(a, b);
        }

        template<typename A, typename B>
        typename Binary::template result<Binary(A const &, B const &)>::type
        operator()(A const &a, B const &b) const
        {
            return Binary()(a, b);
        }
    };

#else
    using mpl::_1;
    using mpl::_2;

    template<typename State, typename Function>
    struct function_inherit
      : State, Function
    {
        using State::operator();
        using Function::operator();
    };

    template<typename Function>
    struct function_inherit<mpl::empty_base, Function>
      : Function
    {
        using Function::operator();
    };

    template<typename A, typename B>
    struct myinherit
      : A, B
    {};

    template<typename State, typename Function, typename Signature>
    struct result_inherit
    {
        typedef myinherit<State, typename Function::template result<Signature> > type;
    };

    template<typename Functions>
    struct overload
      : mpl::inherit_linearly<
            Functions
          , function_inherit<_1, _2>
        >::type
    {
        template<typename Signature>
        struct result
          : mpl::inherit_linearly<
                Functions
              , result_inherit<_1, _2, Signature>
            >::type
        {};
    };
#endif

}}

#endif
