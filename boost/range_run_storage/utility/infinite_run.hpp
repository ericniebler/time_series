// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_UTILITY_INFINITE_RUN_EAN2006514_HPP
#define BOOST_RANGE_RUN_STORAGE_UTILITY_INFINITE_RUN_EAN2006514_HPP

#include <cstddef>
#include <utility>
#include <boost/numeric/functional.hpp>
#include <boost/serialization/utility.hpp>

namespace boost { namespace range_run_storage
{

    template<typename Value, typename Offset = std::ptrdiff_t>
    struct infinite_run_value
      : std::pair<Offset, Offset>
    {
        typedef Value value_type;
        typedef std::pair<Offset, Offset> run_type;

        explicit infinite_run_value(Value const &val = numeric::zero<Value>())
          : run_type(0, 0)
          , value(val)
        {}

        template<typename R, typename V>
        void set(R &r, V &v)
        {
            this->first = range_run_storage::offset(r);
            this->second = range_run_storage::end_offset(r);
            this->value = v;
        }

        Value value;
    };

}} // namespace boost::range_run_storage

namespace boost { namespace serialization
{
    template<typename Archive, typename Value, typename Offset>
    void serialize(Archive &ar, range_run_storage::infinite_run_value<Value, Offset> & rv, unsigned int const /*version*/)
    {
        ar & static_cast<std::pair<Offset, Offset> &>(rv);
        if(rv.first != rv.second)
        {
            ar & rv.value;
        }
    }
}}

#endif // BOOST_RANGE_RUN_STORAGE_UTILITY_INFINITE_RUN_EAN2006514_HPP
