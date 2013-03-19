///////////////////////////////////////////////////////////////////////////////
/// \file characteristic_array.hpp
/// A characteristic array that satisfies the \c InfiniteRangeRunStorage concept
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_CHARACTERISTIC_ARRAY_EAN_05_16_2006
#define BOOST_TIME_SERIES_STORAGE_CHARACTERISTIC_ARRAY_EAN_05_16_2006

#include <boost/time_series/time_series_fwd.hpp>

#include <utility> // for std::pair
#include <algorithm> // for std::min and std::max
#include <boost/detail/construct.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/numeric/functional.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/storage/detail/unit_elements.hpp>
#include <boost/time_series/storage/detail/empty_runs.hpp>
#include <boost/time_series/storage/detail/no_elements.hpp>
#include <boost/time_series/storage/detail/constant_elements.hpp>
#include <boost/time_series/storage/detail/copy_and_swap_inserter.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/utility/serialization.hpp>

namespace boost { namespace time_series { namespace storage
{
    namespace rrs = range_run_storage;

    // The characteristic_array
    template<typename Value, typename Offset = std::ptrdiff_t, typename Elements = unit_elements<Value> >
    struct characteristic_array
      : Elements, no_elements_array<Value>
    {
        typedef Value value_type;
        typedef Offset offset_type;

        typedef Value const &const_reference;
        typedef const_reference reference;

        explicit characteristic_array(
            offset_type start = 0
          , offset_type stop = 0
          , Value const &value = numeric::one<Value>()
          , Value const &zero = numeric::zero<Value>()
        )
          : Elements(value)
          , start_(start)
          , stop_(stop)
          , zero_(zero)
        {
            BOOST_ASSERT(this->start_ <= this->stop_);
        }

        reference operator [](offset_type i) const
        {
            if(i >= this->start() && i < this->stop())
            {
                return (*this)(0);
            }
            return this->zero();
        }

        void swap(characteristic_array &that)
        {
            using std::swap;
            this->Elements::swap(that);
            swap(this->start_, that.start_);
            swap(this->stop_, that.stop_);
            swap(this->zero_, that.zero_);
        }

        template<typename R, typename V>
        void set_at(R &r, V &v)
        {
            BOOST_ASSERT(is_float<Offset>::value || range_run_storage::offset(r) < range_run_storage::end_offset(r));
            this->set_value(v);
            this->start_ = range_run_storage::offset(r);
            this->stop_ = range_run_storage::end_offset(r);
        }

        offset_type start() const
        {
            return this->start_;
        }

        offset_type stop() const
        {
            return this->stop_;
        }

        reference zero() const
        {
            return this->zero_;
        }

        void set_zero(reference z)
        {
            this->zero_ = z;
        }

    private:
        BOOST_RANGE_RUN_STORAGE_DEFINE_SERIALIZE_MEMBER()

        offset_type start_;
        offset_type stop_;
        Value zero_;
    };

    template<typename Value, typename Offset, typename Elements>
    void swap(
        characteristic_array<Value, Offset, Elements> &left
      , characteristic_array<Value, Offset, Elements> &right
    )
    {
        left.swap(right);
    }

    template<typename Run, typename Value>
    inline characteristic_array<Value, typename rrs::concepts::Run<Run>::offset_type> const
    as_characteristic(Run const &run, Value const &value, Value const &zero)
    {
        typedef typename rrs::concepts::Run<Run>::offset_type offset_type;
        return characteristic_array<Value, offset_type>(
            rrs::offset(run)
          , rrs::end_offset(run)
          , value
          , zero
        );
    }
}}}

namespace boost
{
    namespace sequence { namespace impl
    {
        struct characteristic_array_tag;

        template<typename Value, typename Offset, typename Elements>
        struct tag<time_series::storage::characteristic_array<Value, Offset, Elements> >
        {
            typedef characteristic_array_tag type;
        };

        template<typename S>
        struct elements<S, characteristic_array_tag>
          : time_series::storage::no_elements<typename S::value_type>
        {};
    }}

    namespace range_run_storage { namespace impl
    {
        template<typename S>
        struct runs<S, sequence::impl::characteristic_array_tag>
          : time_series::storage::empty_runs<typename S::offset_type>
        {};

        template<typename S>
        struct zero<S, sequence::impl::characteristic_array_tag>
        {
            typedef typename S::reference result_type;

            result_type operator ()(S &s) const
            {
                return s.zero();
            }
        };

        template<typename S, typename V>
        struct set_zero<S, V, sequence::impl::characteristic_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                return s.set_zero(v);
            }
        };

        template<typename S>
        struct pre_run<S, sequence::impl::characteristic_array_tag>
        {
            typedef typename S::offset_type offset_type;
            typedef std::pair<offset_type, offset_type> result_type;

            result_type operator ()(S &s) const
            {
                return result_type(s.start(), s.stop());
            }
        };

        template<typename S>
        struct pre_value<S, sequence::impl::characteristic_array_tag>
        {
            typedef typename S::const_reference result_type;

            result_type operator ()(S &s) const
            {
                return s(0);
            }
        };

        template<typename S, typename V>
        struct set_pre_value<S, V, sequence::impl::characteristic_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                return s.set_value(v);
            }
        };

        template<typename S>
        struct ordered_inserter<S, sequence::impl::characteristic_array_tag>
        {
            typedef time_series::storage::copy_and_swap_inserter<S> result_type;

            result_type operator ()(S &s) const
            {
                return result_type(s);
            }
        };
    }}
}

namespace boost { namespace time_series { namespace traits
{
    template<typename Value, typename Offset>
    struct storage_category<storage::characteristic_array<Value, Offset> >
    {
        typedef characteristic_storage_tag type;
    };

    template<typename Value, typename Offset>
    struct storage_category<storage::characteristic_array<Value, Offset, storage::constant_elements<Value> > >
    {
        typedef scaled_storage_tag<characteristic_storage_tag> type;
    };
}}}

namespace boost { namespace constructors { namespace impl
{
    template<typename Value, typename Offset, typename Elements>
    struct tag<time_series::storage::characteristic_array<Value, Offset, Elements> >
    {
        typedef sequence::impl::characteristic_array_tag type;
    };

    template<typename T>
    struct construct<T, sequence::impl::characteristic_array_tag>
    {
        typedef parameter::parameters<
            parameter::optional<time_series::tag::start>
          , parameter::optional<time_series::tag::stop>
          , parameter::optional<time_series::tag::value>
          , parameter::optional<time_series::tag::zero>
        > args_type;

        typedef T result_type;

        template<typename Args>
        T operator()(Args const &args) const
        {
            typedef typename T::value_type value_type;
            return T(
                args[time_series::start | 0]
              , args[time_series::stop | 0]
              , args[time_series::value | numeric::one<value_type>()]
              , args[time_series::zero | numeric::zero<value_type>()]
            );
        }
    };

}}}

#endif
