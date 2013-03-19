///////////////////////////////////////////////////////////////////////////////
/// \file generate_series.hpp
/// Given a storage tag and a value type, return the concrete type of
/// the associated \c TimeSeries
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_TRAITS_GENERATE_SERIES_HPP_EAN_06_07_2006
#define BOOST_TIME_SERIES_TRAITS_GENERATE_SERIES_HPP_EAN_06_07_2006

#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>

namespace boost { namespace time_series { namespace traits
{

    template<typename Tag, typename Value, typename Discretization, typename Offset>
    struct generate_series
    {
        BOOST_MPL_ASSERT_MSG(false, SERIES_NOT_REGISTERED_YOU_FORGOT_TO_INCLUDE_THE_SERIES_HEADER, (Tag));
    };

}}}

#endif
