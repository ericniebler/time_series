///////////////////////////////////////////////////////////////////////////////
/// \file conversion.hpp
/// Defines which series can be converted to which other series
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_TRAITS_CONVERSION_EAN_06_08_2006
#define BOOST_TIME_SERIES_TRAITS_CONVERSION_EAN_06_08_2006

#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/time_series/time_series_fwd.hpp>
#include <boost/time_series/traits/offset_type.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/traits/discretization_type.hpp>

namespace boost { namespace time_series { namespace traits
{

    ///////////////////////////////////////////////////////////////////////////
    // is_storage_convertible
    template<typename From, typename To>
    struct is_storage_convertible : is_same<From, To> {};

    template<typename From, typename To>
    struct is_storage_convertible<From, scaled_storage_tag<To> > : is_storage_convertible<From, To> {};

    template<typename From, typename To>
    struct is_storage_convertible<scaled_storage_tag<From>, scaled_storage_tag<To> > : is_storage_convertible<From, To> {};

    template<typename From>
    struct is_storage_convertible<From, sparse_storage_tag> : mpl::true_ {};

    template<typename From>
    struct is_storage_convertible<From, dense_storage_tag> : mpl::true_ {};

    template<typename From>
    struct is_storage_convertible<From, piecewise_constant_storage_tag> : mpl::true_ {};

    template<>
    struct is_storage_convertible<delta_storage_tag, characteristic_storage_tag> : mpl::true_ {};

    template<>
    struct is_storage_convertible<heaviside_storage_tag, characteristic_storage_tag> : mpl::true_ {};

    template<>
    struct is_storage_convertible<inverse_heaviside_storage_tag, characteristic_storage_tag> : mpl::true_ {};


    ///////////////////////////////////////////////////////////////////////////
    // is_series_convertible
    template<typename From, typename To>
    struct is_series_convertible
      : mpl::and_<
            is_same<
                typename offset_type<From>::type
              , typename offset_type<To>::type
            >
          , is_same<
                typename discretization_type<From>::type
              , typename discretization_type<To>::type
            >
          , is_storage_convertible<
                typename storage_category<From>::type
              , typename storage_category<To>::type
            >
        >
    {};

    template<typename From, typename To>
    struct is_series_convertible<clipped_series<From>, To>
      : is_series_convertible<From, To>
    {};

    template<typename From, typename To>
    struct is_series_convertible<shifted_series<From>, To>
      : is_series_convertible<From, To>
    {};

    template<typename From, typename Factor, typename To>
    struct is_series_convertible<scaled_series<From, Factor>, To>
      : is_series_convertible<From, To>
    {};

}}} // boost::time_series::traits

#endif
