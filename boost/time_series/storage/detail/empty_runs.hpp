///////////////////////////////////////////////////////////////////////////////
/// \file empty_runs.hpp
/// stub runs property map that just hands out empty runs
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_DETAIL_EMPTY_RUNS_HPP_EAN_08_15_2006
#define BOOST_TIME_SERIES_DETAIL_EMPTY_RUNS_HPP_EAN_08_15_2006

#include <utility>
#include <boost/assert.hpp>

namespace boost { namespace time_series { namespace storage
{

    // stub runs property map that just hands out empty runs
    template<typename Offset = std::ptrdiff_t>
    struct empty_runs_property_map
    {
        typedef std::pair<Offset, Offset> result_type;

        // Readability : Key to run
        template<typename K>
        result_type operator ()(K const &) const
        {
            BOOST_ASSERT(false);
            return result_type(0, 0);
        }
    };

    template<typename Offset = std::ptrdiff_t>
    struct empty_runs
    {
        typedef empty_runs_property_map<Offset> result_type;

        template<typename S>
        result_type operator ()(S &) const
        {
            return result_type();
        }
    };

}}}

#endif // BOOST_TIME_SERIES_DETAIL_EMPTY_RUNS_HPP_EAN_08_15_2006
