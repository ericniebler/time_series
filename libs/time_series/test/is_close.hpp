//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_TEST_IS_CLOSE_HPP_EAN_10_12_2006
#define BOOST_TIME_SERIES_TEST_IS_CLOSE_HPP_EAN_10_12_2006

#include <boost/version.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/time_series/time_series_facade.hpp>
#include <boost/range_run_storage/algorithm/transform.hpp>

#if BOOST_VERSION <= 103301
namespace boost { namespace test_tools
{
    template<typename FPT>
    inline FPT percent_tolerance( FPT v )
    {
        return v;
    }
}}
#endif

template<typename Left, typename Right, typename Tolerance>
inline bool is_close(Left const &left, Right const &right, Tolerance tolerance)
{
    boost::time_series::equal_to_inserter out;
    
    boost::range_run_storage::transform(
        left
      , right
      , boost::test_tools::close_at_tolerance<Tolerance>(
            boost::test_tools::percent_tolerance(tolerance)
        )
      , out
    );
    
    return out.result;
}

#endif
