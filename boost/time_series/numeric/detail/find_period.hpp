///////////////////////////////////////////////////////////////////////////////
/// \file find_period.hpp
/// Calculate the first period with which a given run overlaps
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_DETAIL_FIND_PERIOD_EAN_10_02_2006
#define BOOST_TIME_SERIES_NUMERIC_DETAIL_FIND_PERIOD_EAN_10_02_2006

#include <utility>
#include <boost/assert.hpp>
#include <boost/range_run_storage/offset.hpp>

namespace boost { namespace time_series
{

    namespace detail
    {
        namespace rrs = range_run_storage;

        // Find the lowest period of which this run is a part
        template<typename Run, typename Offset, typename Length>
        inline std::pair<Offset, Offset>
        find_period(Run const &run, Offset offset, Length length)
        {
            Offset o = static_cast<Offset>(rrs::offset(run));
            Offset start = o - (o - offset) % length;
            Offset stop = static_cast<Offset>(start + length);

            BOOST_ASSERT(start <= rrs::offset(run));
            BOOST_ASSERT(stop > rrs::offset(run));
            return std::make_pair(start, stop);
        }

    }

}}

#endif // BOOST_TIME_SERIES_NUMERIC_DETAIL_FIND_PERIOD_EAN_10_02_2006
