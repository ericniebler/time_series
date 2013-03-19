///////////////////////////////////////////////////////////////////////////////
/// \file unit_elements.hpp
/// An elements property map that always returns 1
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_DETAIL_UNIT_ELEMENTS_EAN_05_16_2006
#define BOOST_TIME_SERIES_STORAGE_DETAIL_UNIT_ELEMENTS_EAN_05_16_2006

#include <boost/assert.hpp>
#include <boost/numeric/functional.hpp>
#include <boost/time_series/utility/constants.hpp>

namespace boost { namespace time_series { namespace storage
{

    template<typename Value>
    struct msvc_result_unit_elements
    {
        template<typename K>
        struct result;

        template<typename This, typename K>
        struct result<This(K)>
        {
            typedef Value const &result_type;
        };

        template<typename This, typename K, typename V>
        struct result<This(K, V)>
        {
            typedef void type;
        };
    };

    /// An elements property map that always returns 1
    ///
    template<typename Value>
    struct unit_elements
      : time_series::detail::one_constant<Value>
      , msvc_result_unit_elements<Value>
    {
        explicit unit_elements(Value const &value)
          : time_series::detail::one_constant<Value>(value)
        {}

        void set_value(Value const &value)
        {
            BOOST_ASSERT(*this == value);
        }

        template<typename K>
        Value const &operator ()(K const &) const
        {
            return *this;
        }

        template<typename K, typename V>
        void operator ()(K const &, V const &v) const
        {
            this->set_value(v);
        }

        void swap(unit_elements<Value> &)
        {}
    };

    template<typename Value>
    void swap(unit_elements<Value> &, unit_elements<Value> &)
    {}

}}}

#endif
