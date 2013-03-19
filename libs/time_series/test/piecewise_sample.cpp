//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/piecewise_sample.hpp>

using namespace boost;
using namespace unit_test;

///////////////////////////////////////////////////////////////////////////////
// test_piecewise_sample
//
void test_piecewise_sample()
{
    using namespace time_series;

    piecewise_constant_series<double> discounts;
    make_ordered_inserter(discounts)
        (1, -inf, 0)
        (.98, 1, 7)
        (.96, 8, 30)
        (.95, 30, 60)
        (.94, 60, 90)
        (.93, 90, 180)
        (.92, 180, 365)
        (.91, 365, 2*365)
        (.90, 2*365, 3*365)
        (.89, 3*365, inf)
    .commit();

    // the periods
    int constancy_periods[] = { 0, 30, 180, 365, 2*365 };

    // the expected results
    piecewise_constant_series<double> expected;
    make_ordered_inserter(expected)
        (1, -inf, 0)
        (.95, 30, 180)
        (.92, 180, 365)
        (.91, 365, 2*365)
        (.90, 2*365, inf)
    .commit();

    piecewise_constant_series<double> actual = piecewise_sample(discounts, constancy_periods);

    BOOST_CHECK_EQUAL(actual, expected);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("piecewise_sample test");

    test->add(BOOST_TEST_CASE(&test_piecewise_sample));

    return test;
}
