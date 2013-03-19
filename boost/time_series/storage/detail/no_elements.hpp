///////////////////////////////////////////////////////////////////////////////
/// \file no_elements.hpp
/// A base class which defines an empty elements property map
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_DETAIL_NO_ELEMENTS_EAN_05_16_2006
#define BOOST_TIME_SERIES_STORAGE_DETAIL_NO_ELEMENTS_EAN_05_16_2006

#include <boost/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/detail/pod_singleton.hpp>

namespace boost { namespace time_series { namespace storage
{

    template<typename Value>
    struct invalid_iterator
      : iterator_facade<
            invalid_iterator<Value>
          , Value
          , std::random_access_iterator_tag
        >
    {
        invalid_iterator()
        {}
    private:
        friend class boost::iterator_core_access;

        void increment()
        {
            BOOST_ASSERT(false);
        }

        void decrement()
        {
            BOOST_ASSERT(false);
        }

        void advance(std::ptrdiff_t)
        {
            BOOST_ASSERT(false);
        }

        bool equal(invalid_iterator const &) const
        {
            return true;
        }

        std::ptrdiff_t distance_to(invalid_iterator const &) const
        {
            return 0;
        }

        Value &dereference() const
        {
            BOOST_ASSERT(false);
            Value *p = 0;
            return *p;
        }
    };

    /// A base class which defines an empty elements property map
    ///
    template<typename Value>
    struct no_elements_array
    {
        typedef invalid_iterator<Value const> iterator;
        typedef invalid_iterator<Value const> const_iterator;

        iterator begin() { return iterator(); }
        iterator end() { return iterator(); }

        const_iterator begin() const { return const_iterator(); }
        const_iterator end() const { return const_iterator(); }
    };

    template<typename Value>
    struct msvc_result_no_elements_property_map
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
    struct no_elements_property_map
      : msvc_result_no_elements_property_map<Value>
    {
        template<typename K>
        Value const &operator ()(K const &) const
        {
            BOOST_ASSERT(false);
            Value const *p = 0;
            return *p;
        }

        template<typename K, typename V>
        void operator ()(K const &, V const &v) const
        {
            BOOST_ASSERT(false);
        }
    };

    template<typename Value>
    struct no_elements
    {
        typedef no_elements_property_map<Value> result_type;

        template<typename S>
        result_type operator ()(S &) const
        {
            return result_type();
        }
    };

}}}

#endif
