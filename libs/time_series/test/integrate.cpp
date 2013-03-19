//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/numeric/integrate.hpp>
#include <boost/time_series/ordered_inserter.hpp>

using namespace boost;
using namespace unit_test;

///////////////////////////////////////////////////////////////////////////////
// test_integrate
//
void test_integrate()
{
    using namespace time_series;

    piecewise_constant_series<int> p1(discretization = 30);
    make_ordered_inserter(p1)
        (2, 0, 10)
        (4, 10, 20)
        (2, 90, 99)
    .commit();
    BOOST_CHECK_EQUAL(2340, integrate(p1));

    piecewise_constant_series<int, monthly> p2;
    make_ordered_inserter(p2)
        (2, 0, 10)
        (4, 10, 20)
        (2, 90, 99)
    .commit();
    BOOST_CHECK_EQUAL(2340, integrate(p2));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("integrate test");

    test->add(BOOST_TEST_CASE(&test_integrate));

    return test;
}
