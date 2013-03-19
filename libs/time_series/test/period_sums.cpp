//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/array.hpp>
#include <boost/time_series/characteristic_series.hpp>
#include <boost/time_series/constant_series.hpp>
#include <boost/time_series/delta_series.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/heaviside_series.hpp>
#include <boost/time_series/inverse_heaviside_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/shifted_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/period_sums.hpp>
#include "./is_close.hpp"

using namespace boost;
using namespace unit_test;

///////////////////////////////////////////////////////////////////////////////
// test_period_sums
//
void test_period_sums()
{
    using namespace time_series;

    // expected result for test 1:
    sparse_series<int> result;
    make_ordered_inserter(result)
        (20, 1)
        (20, 11)
        (20, 21)
        (20, 31)
        (20, 41)
        (20, 51)
        (20, 61)
        (20, 71)
        (20, 81)
        (18, 91)
    .commit();

    dense_series<int> d(stop = 100, value = 2);
    sparse_series<int> s = period_sums(d, 1, 10);
    BOOST_CHECK_EQUAL(s, result);

    // expected result for tests 2:
    sparse_series<int> result2;
    make_ordered_inserter(result2)
        (18, 1)
        (2,  81)
        (18, 91)
    .commit();

    piecewise_constant_series<int> p;
    make_ordered_inserter(p)
        (2, 0, 10)
        (2, 90, 100)
    .commit();
    sparse_series<int> s2 = period_sums(p, 1, 10);
    BOOST_CHECK_EQUAL(s2, result2);
}

///////////////////////////////////////////////////////////////////////////////
// test_period_sums_float
//
void test_period_sums_float()
{
    using namespace time_series;

    piecewise_constant_series<double,int,double> pwc;
    make_ordered_inserter(pwc)
        (3., 0.5, 11.5)
        (2., 90., 100.)
    .commit();

    sparse_series<double> expected;
    make_ordered_inserter(expected)
        (30., 1)
        (1.5, 11)
        (2.,  81)
        (18., 91)
    .commit();

    sparse_series<double> actual = period_sums(pwc, 1, 10);
    BOOST_CHECK(is_close(expected, actual, 0.1));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("period_sums test");

    test->add(BOOST_TEST_CASE(&test_period_sums));
    test->add(BOOST_TEST_CASE(&test_period_sums_float));

    return test;
}
