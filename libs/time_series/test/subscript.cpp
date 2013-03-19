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
#include <boost/time_series/numeric/subscript.hpp>
#include "./is_close.hpp"

using namespace boost;
using namespace unit_test;

///////////////////////////////////////////////////////////////////////////////
// test_subscript
//
void test_subscript()
{
    using namespace time_series;
    typedef counting_iterator<int> int_;

    dense_series<int> d;
    std::copy(int_(0), int_(100), make_ordered_inserter(d, 0)).commit();

    sparse_series<int> indices;
    make_ordered_inserter(indices)
        (1, 0)(10, 10)(20, 20)(30, 30)(40, 40)(50, 49)
    .commit();

    sparse_series<int> result;
    make_ordered_inserter(result)
        (1, 0)(20, 10)(40, 20)(60, 30)(80, 40)(99, 49)
    .commit();

    BOOST_CHECK_EQUAL(result, subscript(d, indices));
}

///////////////////////////////////////////////////////////////////////////////
// test_floating_point_subscript
//
void test_floating_point_subscript()
{
    using namespace time_series;

    piecewise_constant_series<double, int, double> pwc;
    make_ordered_inserter(pwc)
        (1., 00., 10.)(2., 10., 20.)(3., 20., 30.)(4., 30., 40.)(5., 40., 50.)
        (6., 50., 60.)(7., 60., 70.)(8., 70., 80.)(9., 80., 90.)(10., 90., 100.)
    .commit();

    sparse_series<double, int, double> indices;
    make_ordered_inserter(indices)
        (1.5, 0.)(10.5, 10.)(20.5, 20.)(30.5, 30.)(40.5, 40.)(50.5, 49.)
    .commit();

    sparse_series<double, int, double> expected;
    make_ordered_inserter(expected)
        (1., 0.)(3., 10.)(5., 20.)(7., 30.)(9., 40.)(10., 49)
    .commit();

    sparse_series<double, int, double> actual = subscript(pwc, indices);
    BOOST_CHECK(is_close(expected, actual, 0.1));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("subscript test");

    test->add(BOOST_TEST_CASE(&test_subscript));
    test->add(BOOST_TEST_CASE(&test_floating_point_subscript));

    return test;
}
