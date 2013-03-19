///////////////////////////////////////////////////////////////////////////////
/// \file constant_array.hpp
/// A constant array that satisfies the \c InfiniteRangeRunStorage concept
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_CONSTANT_ARRAY_EAN_04_26_2006
#define BOOST_TIME_SERIES_STORAGE_CONSTANT_ARRAY_EAN_04_26_2006

#include <algorithm>
#include <boost/time_series/time_series_fwd.hpp>
#include <boost/detail/construct.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/storage/detail/empty_runs.hpp>
#include <boost/time_series/storage/detail/no_elements.hpp>
#include <boost/time_series/storage/detail/copy_and_swap_inserter.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/utility/serialization.hpp>

namespace boost { namespace time_series { namespace storage
{
    template<typename Value, typename Offset = std::ptrdiff_t>
    struct constant_array
      : no_elements_array<Value>
    {
        typedef Value value_type;
        typedef Offset offset_type;

        typedef Value const &const_reference;
        typedef const_reference reference;

        explicit constant_array(Value const &value = numeric::zero<Value>())
          : value_(value)
        {}

        void swap(constant_array &that)
        {
            using std::swap;
            swap(this->value_, that.value_);
        }

        reference operator [](offset_type) const
        {
            return this->value_;
        }

        template<typename R, typename V>
        void set_at(R &, V &v)
        {
            this->value_ = v;
        }

    private:
        BOOST_RANGE_RUN_STORAGE_DEFINE_SERIALIZE_MEMBER()

        Value value_;
    };

    template<typename Value, typename Offset>
    void swap(constant_array<Value, Offset> &left, constant_array<Value, Offset> &right)
    {
        left.swap(right);
    }
}}}

namespace boost
{
    namespace sequence { namespace impl
    {
        struct constant_array_tag;

        template<typename Value, typename Offset>
        struct tag<time_series::storage::constant_array<Value, Offset> >
        {
            typedef constant_array_tag type;
        };

        template<typename S>
        struct elements<S, constant_array_tag>
          : time_series::storage::no_elements<typename S::value_type>
        {};
    }}

    namespace range_run_storage { namespace impl
    {
        template<typename S>
        struct runs<S, sequence::impl::constant_array_tag>
          : time_series::storage::empty_runs<typename S::offset_type>
        {};

        template<typename S>
        struct ordered_inserter<S, sequence::impl::constant_array_tag>
        {
            typedef time_series::storage::copy_and_swap_inserter<S> result_type;

            result_type operator ()(S &s) const
            {
                return result_type(s);
            }
        };

        template<typename S>
        struct pre_run<S, sequence::impl::constant_array_tag>
        {
            typedef typename S::offset_type offset_type;
            typedef std::pair<offset_type, offset_type> result_type;

            result_type operator ()(S &) const
            {
                return result_type(-inf, inf);
            }
        };

        template<typename S>
        struct pre_value<S, sequence::impl::constant_array_tag>
        {
            typedef typename S::const_reference result_type;

            result_type operator ()(S &s) const
            {
                return s[0];
            }
        };

        template<typename S, typename V>
        struct set_pre_value<S, V, sequence::impl::constant_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                return s.set_at(s, v);
            }
        };

    }}
}

namespace boost { namespace time_series { namespace traits
{
    template<typename Value, typename Offset>
    struct storage_category<storage::constant_array<Value, Offset> >
    {
        typedef constant_storage_tag type;
    };
}}}

namespace boost { namespace constructors { namespace impl
{
    template<typename Value, typename Offset>
    struct tag<time_series::storage::constant_array<Value, Offset> >
    {
        typedef sequence::impl::constant_array_tag type;
    };

    template<typename T>
    struct construct<T, sequence::impl::constant_array_tag>
    {
        typedef parameter::parameters<
            parameter::optional<time_series::tag::value>
          , parameter::optional<time_series::tag::zero>
        > args_type;

        typedef T result_type;

        template<typename Args>
        T operator()(Args const &args) const
        {
            typedef typename T::value_type value_type;
            return T(args[time_series::value | args[time_series::zero | numeric::zero<value_type>()]]);
        }
    };

}}}

#endif
