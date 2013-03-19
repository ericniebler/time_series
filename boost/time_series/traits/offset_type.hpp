///////////////////////////////////////////////////////////////////////////////
/// \file offset_type.hpp
/// Given a series, return the type of the offset
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_TRAITS_OFFSET_TYPE_HPP_EAN_08_07_2007
#define BOOST_TIME_SERIES_TRAITS_OFFSET_TYPE_HPP_EAN_08_07_2007

#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>

namespace boost { namespace time_series { namespace traits
{

    template<typename S>
    struct offset_type
    {
        BOOST_MPL_ASSERT_MSG(false, SERIES_NOT_REGISTERED_YOU_FORGOT_TO_INCLUDE_THE_SERIES_HEADER, (S));
    };

    template<typename S>
    struct offset_type<S const>
      : offset_type<S>
    {};

    template<typename S>
    struct offset_type<S volatile>
      : offset_type<S>
    {};

    template<typename S>
    struct offset_type<S volatile const>
      : offset_type<S>
    {};

}}}

#endif
