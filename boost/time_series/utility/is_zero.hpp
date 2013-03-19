///////////////////////////////////////////////////////////////////////////////
/// \file array.hpp
/// Utilities for Arrays, cursors and property maps
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_DETAIL_ARRAY_HPP_EAN_04_18_2006
#define BOOST_TIME_SERIES_DETAIL_ARRAY_HPP_EAN_04_18_2006

#include <boost/numeric/functional.hpp>
#include <boost/range_run_storage/concepts.hpp>

namespace boost { namespace time_series { namespace detail
{
    namespace rrs = boost::range_run_storage;
    using rrs::concepts::RangeRunStorage;

    template<typename Series, typename Value>
    inline bool is_zero(Series const &series, Value const &value)
    {
        typedef typename RangeRunStorage<Series const>::value_type value_type;
        value_type const &v(value), &z(rrs::zero(series));
        return numeric::promote<bool>(z == v);
    }

}}} // namespace boost::time_series::detail

#endif
