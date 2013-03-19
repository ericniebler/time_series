///////////////////////////////////////////////////////////////////////////////
/// \file constant_elements.hpp
/// An elements property map that always returns the same value
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_DETAIL_CONSTANT_ELEMENTS_EAN_05_16_2006
#define BOOST_TIME_SERIES_STORAGE_DETAIL_CONSTANT_ELEMENTS_EAN_05_16_2006

#include <algorithm>

// Define the indices property map needed to make
// heaviside_array satisfy the Array concept.
namespace boost { namespace time_series { namespace storage
{

    template<typename Value>
    struct msvc_result_constant_elements
    {
        template<typename K>
        struct result;

        template<typename This, typename K>
        struct result<This(K)>
        {
            typedef Value const &type;
        };

        template<typename This, typename K, typename V>
        struct result<This(K, V)>
        {
            typedef void type;
        };
    };    

    template<typename Value>
    struct constant_elements
      : msvc_result_constant_elements<Value>
    {
        constant_elements()
          : value_()
        {}

        explicit constant_elements(Value const &value)
          : value_(value)
        {}

        void set_value(Value const &value)
        {
            this->value_ = value;
        }

        template<typename K>
        Value const &operator ()(K const &) const
        {
            return this->value_;
        }

        template<typename K, typename V>
        void operator ()(K const &, V const &v) const
        {
            this->set_value(v);
        }

        void swap(constant_elements<Value> &that)
        {
            using std::swap;
            swap(this->value_, that.value_);
        }

    private:
        Value value_;
    };

    template<typename Value>
    void swap(constant_elements<Value> &left, constant_elements<Value> &right)
    {
        left.swap(right);
    }

}}}

#endif
