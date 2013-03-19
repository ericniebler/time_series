///////////////////////////////////////////////////////////////////////////////
/// \file time_series_base.hpp
/// CRTP base class for all series
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_UTILITY_TIME_SERIES_BASE_HPP_EAN_06_07_2006
#define BOOST_TIME_SERIES_UTILITY_TIME_SERIES_BASE_HPP_EAN_06_07_2006

#include <boost/time_series/time_series_fwd.hpp>

namespace boost { namespace time_series
{
    struct time_series_root
    {};

    template<typename Derived>
    struct time_series_base
      : time_series_root
    {
        typedef Derived derived_type;

        Derived &cast()
        {
            return *static_cast<Derived *>(this);
        }

        Derived const &cast() const
        {
            return *static_cast<Derived const *>(this);
        }
    };
}}

#endif
