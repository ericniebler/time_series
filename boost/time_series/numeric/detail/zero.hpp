///////////////////////////////////////////////////////////////////////////////
/// \file zero.hpp
/// Some operator overloads for zero_constant
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_DETAIL_ZERO_EAN_06_22_2006
#define BOOST_TIME_SERIES_NUMERIC_DETAIL_ZERO_EAN_06_22_2006

#include <boost/utility/result_of.hpp>
#include <boost/numeric/functional.hpp>
#include <boost/time_series/utility/constants.hpp>

namespace boost { namespace time_series { namespace detail
{
    struct zero_constant_tag;

    template<typename Left, typename Right>
    typename result_of<
        numeric::op::promote<
            typename result_of<numeric::op::plus(Left const &, Right const &)>::type const
        >(Left const &)
    >::type
    operator +(Left const &left, zero_constant<Right> const &)
    {
        typedef typename result_of<numeric::op::plus(Left const &, Right const &)>::type result_type;
        return numeric::promote<result_type const>(left);
    }

    template<typename Left, typename Right>
    typename result_of<
        numeric::op::promote<
            typename result_of<numeric::op::plus(Left const &, Right const &)>::type const
        >(Right const &)
    >::type
    operator +(zero_constant<Left> const &, Right const &right)
    {
        typedef typename result_of<numeric::op::plus(Left const &, Right const &)>::type result_type;
        return numeric::promote<result_type const>(right);
    }

    template<typename Left, typename Right>
    typename result_of<
        numeric::op::promote<
            typename result_of<numeric::op::minus(Left const &, Right const &)>::type const
        >(Left const &)
    >::type
    operator -(Left const &left, zero_constant<Right> const &)
    {
        typedef typename result_of<numeric::op::minus(Left const &, Right const &)>::type result_type;
        return numeric::promote<result_type const>(left);
    }

    template<typename Left, typename Right>
    typename result_of<
        numeric::op::promote<
            typename result_of<numeric::op::plus(Left const &, Right const &)>::type const
        >(typename result_of<numeric::op::unary_minus(Right const &)>::type)
    >::type
    operator -(zero_constant<Left> const &, Right const &right)
    {
        typedef typename result_of<numeric::op::minus(Left const &, Right const &)>::type result_type;
        return numeric::promote<result_type const>(numeric::unary_minus(right));
    }

    template<typename Left, typename Right>
    typename result_of<
        numeric::op::promote<
            typename result_of<numeric::op::multiplies(Left const &, Right const &)>::type const
        >(typename zero_constant<Right>::const_reference)
    >::type
    operator *(Left const &left, zero_constant<Right> const &right)
    {
        typedef typename result_of<numeric::op::multiplies(Left const &, Right const &)>::type result_type;
        typename zero_constant<Right>::const_reference zero = right;
        return numeric::promote<result_type const>(zero);
    }

    template<typename Left, typename Right>
    typename result_of<
        numeric::op::promote<
            typename result_of<numeric::op::multiplies(Left const &, Right const &)>::type const
        >(typename zero_constant<Left>::const_reference)
    >::type
    operator *(zero_constant<Left> const &left, Right const &right)
    {
        typedef typename result_of<numeric::op::multiplies(Left const &, Right const &)>::type result_type;
        typename zero_constant<Left>::const_reference zero = left;
        return numeric::promote<result_type const>(zero);
    }

    template<typename Left, typename Right>
    typename result_of<
        numeric::op::promote<
            typename result_of<numeric::op::divides(Left const &, Right const &)>::type const
        >(typename zero_constant<Left>::const_reference)
    >::type
    operator /(zero_constant<Left> const &left, Right const &right)
    {
        typedef typename result_of<numeric::op::divides(Left const &, Right const &)>::type result_type;
        typename zero_constant<Left>::const_reference zero = left;
        return numeric::promote<result_type const>(zero);
    }
}}}

namespace boost { namespace numeric { namespace functional
{
    template<typename T>
    struct tag<time_series::detail::zero_constant<T> >
    {
        typedef time_series::detail::zero_constant_tag type;
    };
}}}

#endif
