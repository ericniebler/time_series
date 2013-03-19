///////////////////////////////////////////////////////////////////////////////
/// \file time_series/concepts.hpp
/// Defines the \c TimeSeries and \c Mutable_TimeSeries concepts
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_CONCEPTS_HPP_EAN_09_04_2006
#define BOOST_TIME_SERIES_CONCEPTS_HPP_EAN_09_04_2006

#include <boost/range_run_storage/concepts.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/traits/discretization_type.hpp>

namespace boost { namespace time_series { namespace concepts
{

using namespace boost::range_run_storage::concepts;

template<typename S>
struct TimeSeries
  : InfiniteRangeRunStorage<S>
{
    typedef
        typename traits::discretization_type<S>::type
    discretization_type;

    typedef
        typename traits::storage_category<S>::type
    storage_category;

    ~TimeSeries()
    {
        S &s = concepts::lvalue_of<S>();

        // TimeSeries have a discretization
        discretization_type d1 = s.discretization();

        // discretizations can be compared for equality.
        bool b = (d1 == d1);
        b = (d1 != d1);

        // TimeSeries allow indexed access
        typename TimeSeries::value_type v = s[0];

        ignore_unused_variable_warning(v);
        ignore_unused_variable_warning(b);
    }
};

template<typename S>
struct Mutable_TimeSeries
  : TimeSeries<S>
  , Mutable_InfiniteRangeRunStorage<S>
{
    ~Mutable_TimeSeries()
    {
        typedef typename Mutable_TimeSeries::discretization_type discretization_type;

        S &s = concepts::lvalue_of<S>();

        // TimeSeries have a discretization
        discretization_type d = s.discretization();

        // Set the discretization
        s.discretization(d);
    }
};

}}} // namespace boost::time_series::concepts

#endif // BOOST_TIME_SERIES_CONCEPTS_HPP_EAN_09_04_2006
