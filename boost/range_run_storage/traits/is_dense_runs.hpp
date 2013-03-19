///////////////////////////////////////////////////////////////////////////////
/// \file is_dense_runs.hpp
/// Trait for detecting when the runs property map is "dense" -- runs
/// of length one, and offsets monotonically increasing.
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_TRAITS_IS_DENSE_RUNS_EAN_07_27_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_TRAITS_IS_DENSE_RUNS_EAN_07_27_2006_HPP

#include <boost/mpl/bool.hpp>

namespace boost { namespace range_run_storage { namespace traits
{

    template<typename Runs>
    struct is_dense_runs
      : mpl::false_
    {};

    template<typename Runs>
    struct is_dense_runs<Runs const>
      : is_dense_runs<Runs>
    {};

    template<typename Runs>
    struct is_dense_runs<Runs volatile>
      : is_dense_runs<Runs>
    {};

    template<typename Runs>
    struct is_dense_runs<Runs const volatile>
      : is_dense_runs<Runs>
    {};

}}}

#endif
