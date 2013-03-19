///////////////////////////////////////////////////////////////////////////////
/// \file single_iterator.hpp
/// An STL iterator over a 1-element sequence.
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_DETAIL_SINGLE_ITERATOR_EAN_05_16_2006
#define BOOST_TIME_SERIES_STORAGE_DETAIL_SINGLE_ITERATOR_EAN_05_16_2006

#include <iterator>
#include <boost/iterator/iterator_facade.hpp>

// Define the indices property map needed to make
// heaviside_array satisfy the Array concept.
namespace boost { namespace time_series { namespace storage { namespace detail
{
    // Iterator for use with heaviside, inverse_heaviside and characteristic
    template<typename Value>
    struct single_iterator
      : iterator_facade<
            single_iterator<Value>
          , Value const
          , std::forward_iterator_tag
        >
    {
        single_iterator()
          : done_(true)
          , value_()
        {}

        explicit single_iterator(Value const &value)
          : done_(false)
          , value_(value)
        {}

    private:
        friend class boost::iterator_core_access;

        Value const &dereference() const
        {
            return this->value_;
        }

        void increment()
        {
            this->done_ = true;
        }

        bool equal(single_iterator const &that) const
        {
            return this->done_ == that.done_;
        }

        bool done_;
        Value value_;
    };

}}}}

#endif
