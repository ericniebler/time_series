//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/variable_period_sums.hpp>
#include "./is_close.hpp"

using namespace boost;
using namespace unit_test;

///////////////////////////////////////////////////////////////////////////////
// test_variable_period_sums
//
void test_variable_period_sums()
{
    using namespace time_series;
    typedef counting_iterator<int> int_;

    dense_series<int> d;
    std::copy(int_(0), int_(10), make_ordered_inserter(d, 0)).commit();

    std::ptrdiff_t periods[] = { 0, 4, 6, 8, 12 };

    sparse_series<int> result;
    make_ordered_inserter(result)
        (6, 0)(9, 4)(13, 6)(17, 8)
    .commit();

    BOOST_CHECK_EQUAL(result, variable_period_sums(d, periods));
}

///////////////////////////////////////////////////////////////////////////////
// test_shifted_variable_period_sums
//
void test_shifted_variable_period_sums()
{
    using namespace time_series;
    typedef counting_iterator<int> int_;

    dense_series<int> d;
    std::copy(int_(0), int_(10), make_ordered_inserter(d, 0)).commit();

    std::ptrdiff_t periods[] = { 0, 4, 6, 8, 12 };
    int elements[] = { 0, 1, 0, 2, 0 };

    sparse_series<int> result;
    make_ordered_inserter(result)
        (6, 0)(9, 5)(13, 6)(17, 10)
    .commit();

    BOOST_CHECK_EQUAL(result, shifted_variable_period_sums(d, periods, elements));
}

///////////////////////////////////////////////////////////////////////////////
// test_floating_variable_period_sums
//
void test_floating_variable_period_sums()
{
    using namespace time_series;
    piecewise_constant_series<double, int, double> pwc;
    make_ordered_inserter(pwc)
        (3., -2.2, -1.1)
        (4., -.1, .1)
        (5., 1.1, 2.2)
        (6., 10.1, 20.2)
    .commit();

    double periods[] = { -10.0, -2.1, -0.9, 0.9, 2.1, 3.3, 6.6, 12.3, 15.7 };

    sparse_series<double, int, double> expected;
    make_ordered_inserter(expected)
        (0.3, -10)
        (3, -2.1)
        (0.8, -0.9)
        (5, 0.9)
        (0.5, 2.1)
        (13.2, 6.6)
        (20.4, 12.3)
        (27, 15.7)
    .commit();

    sparse_series<double, int, double> actual = variable_period_sums(pwc, periods);
    BOOST_CHECK(is_close(expected, actual, 0.1));
}

///////////////////////////////////////////////////////////////////////////////
// test_floating_variable_period_sums2
//
void test_floating_variable_period_sums2()
{
    using namespace time_series;
    piecewise_constant_series<double> pwc;
    make_ordered_inserter(pwc)
        (3.1, -2, -1)
        (4.2, -1, 0)
        (5.3, 1, 2)
        (6.4, 10, 20)
    .commit();

    std::ptrdiff_t periods[] = { -10, -2, 0, 1, 2, 3, 6, 12, 15 };

    sparse_series<double> expected;
    make_ordered_inserter(expected)
        (7.3, -2)
        (5.3, 1)
        (12.8, 6)
        (19.2, 12)
        (32, 15)
    .commit();

    sparse_series<double> actual = variable_period_sums(pwc, periods);
    BOOST_CHECK(is_close(expected, actual, 0.1));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("variable_period_sums test");

    test->add(BOOST_TEST_CASE(&test_variable_period_sums));
    test->add(BOOST_TEST_CASE(&test_shifted_variable_period_sums));
    test->add(BOOST_TEST_CASE(&test_floating_variable_period_sums));
    test->add(BOOST_TEST_CASE(&test_floating_variable_period_sums2));

    return test;
}
