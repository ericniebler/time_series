///////////////////////////////////////////////////////////////////////////////
/// \file constants.hpp
///
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_UTILITY_CONSTANTS_EAN_06_23_2006
#define BOOST_TIME_SERIES_UTILITY_CONSTANTS_EAN_06_23_2006

#include <complex>

namespace boost { namespace time_series { namespace detail
{

    // constant
    template<typename Value>
    struct constant
    {
        explicit constant(Value const &value = Value())
          : value_(value)
        {}

        typedef Value const &const_reference;

        operator const_reference() const
        {
            return this->value_;
        }
    private:
        Value value_;
    };

    template<typename Value>
    struct complex_zero
    {
        operator std::complex<Value>() const
        {
            return std::complex<Value>();
        }
    };

    template<typename Value>
    struct complex_one
    {
        operator std::complex<Value>() const
        {
            return std::complex<Value>(1, 1);
        }
    };

    // zero_constant
    template<typename Value>
    struct zero_constant
      : constant<Value>
    {
        explicit zero_constant(Value const &value = Value())
          : constant<Value>(value)
        {}
    };

    // one_constant
    template<typename Value>
    struct one_constant
      : constant<Value>
    {
        explicit one_constant(Value const &value = Value(1))
          : constant<Value>(value)
        {}
    };

    // partial specialization for std::complex
    template<typename Value>
    struct zero_constant<std::complex<Value> >
      : constant<std::complex<Value> >
    {
        explicit zero_constant(std::complex<Value> const &value = std::complex<Value>())
          : constant<std::complex<Value> >(value)
        {}
    };

    template<typename Value>
    struct one_constant<std::complex<Value> >
      : constant<std::complex<Value> >
    {
        explicit one_constant(std::complex<Value> const &value = std::complex<Value>(Value(1), Value(1)))
          : constant<std::complex<Value> >(value)
        {}
    };

}}}

#endif
