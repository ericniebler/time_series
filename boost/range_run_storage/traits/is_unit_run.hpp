///////////////////////////////////////////////////////////////////////////////
/// \file is_unit_run.hpp
/// Trait for detecting runs with length 1
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_TRAITS_IS_UNIT_RUN_EAN_07_17_2006_HPP
# define BOOST_RANGE_RUN_STORAGE_TRAITS_IS_UNIT_RUN_EAN_07_17_2006_HPP

# include <boost/mpl/bool.hpp>

namespace boost { namespace range_run_storage { namespace traits
{

    template<typename R>
    struct is_unit_run
      : mpl::false_
    {};

    template<typename R>
    struct is_unit_run<R const>
      : is_unit_run<R>
    {};

    template<typename R>
    struct is_unit_run<R volatile>
      : is_unit_run<R>
    {};

    template<typename R>
    struct is_unit_run<R const volatile>
      : is_unit_run<R>
    {};

}}}

#endif
