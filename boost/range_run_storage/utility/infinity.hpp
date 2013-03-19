///////////////////////////////////////////////////////////////////////////////
/// \file infinity.hpp
/// A time series that uses characteristic storage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_UTILITY_INFINITY_HPP_EAN_07_21_2006
#define BOOST_RANGE_RUN_STORAGE_UTILITY_INFINITY_HPP_EAN_07_21_2006

#include <limits>
#include <boost/utility/enable_if.hpp>
#include <boost/integer_traits.hpp>
#include <boost/detail/pod_singleton.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace boost { namespace range_run_storage
{
    namespace detail
    {
        template<typename T>
        typename enable_if<is_integral<T>, T>::type
        infinity(T * = 0)
        {
            return integer_traits<T>::const_max;
        }

        template<typename T>
        typename disable_if<is_integral<T>, T>::type
        infinity(T * = 0)
        {
            return std::numeric_limits<T>::infinity();
        }

        template<typename T>
        typename enable_if<is_integral<T>, T>::type
        neg_infinity(T * = 0)
        {
            return integer_traits<T>::const_min;
        }

        template<typename T>
        typename disable_if<is_integral<T>, T>::type
        neg_infinity(T * = 0)
        {
            return -std::numeric_limits<T>::infinity();
        }
    }

    template<bool Positive>
    struct infinity_base
    {
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1310)
        operator std::ptrdiff_t () const    { return integer_traits<std::ptrdiff_t>::const_max; }
        operator std::size_t () const       { return integer_traits<std::size_t>::const_max; }
        operator float () const             { return std::numeric_limits<float>::infinity(); }
        operator double () const            { return std::numeric_limits<double>::infinity(); }
#else
        template<typename T>
        operator T () const
        {
            return detail::infinity<T>();
        }
#endif
    };

    template<>
    struct infinity_base<false>
    {
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1310)
        operator std::ptrdiff_t () const    { return integer_traits<std::ptrdiff_t>::const_min; }
        operator std::size_t () const       { return integer_traits<std::size_t>::const_min; }
        operator float () const             { return -std::numeric_limits<float>::infinity(); }
        operator double () const            { return -std::numeric_limits<double>::infinity(); }
#else
        template<typename T>
        operator T () const
        {
            return detail::neg_infinity<T>();
        }
#endif
    };

    template<bool Positive = true>
    struct infinity
      : infinity_base<Positive>
    {
        template<typename T> friend bool operator ==(infinity inf, T t)
        {
            return static_cast<T>(inf) == t;
        }

        template<typename T> friend bool operator !=(infinity inf, T t)
        {
            return static_cast<T>(inf) != t;
        }

        template<typename T> friend bool operator ==(T t, infinity inf)
        {
            return t == static_cast<T>(inf);
        }

        template<typename T> friend bool operator !=(T t, infinity inf)
        {
            return t != static_cast<T>(inf);
        }

        infinity<!Positive> operator-() const
        {
            return infinity<!Positive>();
        }

        template<typename T> friend T &operator +=(T &t, infinity inf)
        {
            return t = static_cast<T>(inf);
        }

        template<typename T> friend T &operator -=(T &t, infinity inf)
        {
            return t = static_cast<T>(-inf);
        }

        template<typename T> friend infinity operator +(T t, infinity inf)
        {
            return inf;
        }

        template<typename T> friend infinity<!Positive> operator -(T t, infinity inf)
        {
            return -inf;
        }

        template<typename T> friend infinity operator +(infinity inf, T t)
        {
            return inf;
        }

        template<typename T> friend infinity operator -(infinity inf, T t)
        {
            return inf;
        }
    };

    typedef infinity<true> plus_infinity;
    typedef infinity<false> minus_infinity;

    namespace
    {
        plus_infinity const &inf = boost::detail::pod_singleton<plus_infinity>::instance;
    }

}}

#endif
