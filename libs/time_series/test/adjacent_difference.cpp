//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
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
#include <boost/time_series/numeric/adjacent_difference.hpp>

using namespace boost;
using namespace unit_test;

///////////////////////////////////////////////////////////////////////////////
// test_adjacent_difference
//
void test_adjacent_difference()
{
    using namespace time_series;

    // expected result for tests 1:
    sparse_series<int> result;
    make_ordered_inserter(result)
        (2,  0)
        (-2, 100)
    .commit();

    dense_series<int> d(stop = 100, value = 2);
    sparse_series<int> s = adjacent_difference(d);
    BOOST_CHECK_EQUAL(s, result);

    // expected result for tests 3:
    result = sparse_series<int>();

    constant_series<int> c(2);
    sparse_series<int> s2 = adjacent_difference(c);
    BOOST_CHECK_EQUAL(s2, result);

    // expected result for tests 3:
    make_ordered_inserter(result)
        (2,  0)
        (2,  10)
        (-4, 20)
        (2,  90)
        (-2, 99)
    .commit();

    piecewise_constant_series<int> p;
    make_ordered_inserter(p)
        (2, 0, 10)
        (4, 10, 20)
        (2, 90, 99)
    .commit();
    sparse_series<int> s3 = adjacent_difference(p);
    BOOST_CHECK_EQUAL(s3, result);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("adjacent_difference test");

    test->add(BOOST_TEST_CASE(&test_adjacent_difference));

    return test;
}
