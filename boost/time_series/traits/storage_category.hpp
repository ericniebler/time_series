///////////////////////////////////////////////////////////////////////////////
/// \file storage_category.hpp
/// Given a concrete storage type, fetch its associated storage tag
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_TRAITS_STORAGE_CATEGORY_HPP_EAN_06_06_2006
#define BOOST_TIME_SERIES_TRAITS_STORAGE_CATEGORY_HPP_EAN_06_06_2006

namespace boost { namespace time_series { namespace traits
{

    template<typename S>
    struct storage_category
    {};

    template<typename S>
    struct storage_category<S const>
      : storage_category<S>
    {};

    template<typename S>
    struct storage_category<S volatile>
      : storage_category<S>
    {};

    template<typename S>
    struct storage_category<S volatile const>
      : storage_category<S>
    {};

}}}

#endif

