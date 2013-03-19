///////////////////////////////////////////////////////////////////////////////
/// \file is_time_series.hpp
/// An index property map with an integer offset and a clip range
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_TRAITS_IS_TIME_SERIES_HPP_EAN_06_06_2006
#define BOOST_TIME_SERIES_TRAITS_IS_TIME_SERIES_HPP_EAN_06_06_2006

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/time_series/utility/time_series_base.hpp>

namespace boost { namespace time_series { namespace traits
{

    template<typename S>
    struct is_time_series
      : is_base_and_derived<time_series_root, S>
    {};

    template<typename S>
    struct is_time_series<S const>
      : is_time_series<S>
    {};

    template<typename S>
    struct is_time_series<S volatile>
      : is_time_series<S>
    {};

    template<typename S>
    struct is_time_series<S volatile const>
      : is_time_series<S>
    {};

}}}

#endif

