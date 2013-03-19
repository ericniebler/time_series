//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/heaviside_series.hpp>
#include <boost/time_series/inverse_heaviside_series.hpp>
#include <boost/time_series/numeric/invert_heaviside.hpp>

using namespace boost;
using namespace unit_test;

///////////////////////////////////////////////////////////////////////////////
// test_invert_heaviside
//
void test_invert_heaviside()
{
    using namespace time_series;

    heaviside_series<int> h(start = 10, value = 42, discretization = 3);
    inverse_heaviside_series<int> ih(stop = 10, value = 42, discretization = 3);

    BOOST_CHECK_EQUAL(h, invert_heaviside(ih));
    BOOST_CHECK_EQUAL(ih, invert_heaviside(h));

    heaviside_unit_series<int> hu(start = 10, discretization = 3);
    inverse_heaviside_unit_series<int> ihu(stop = 10, discretization = 3);

    BOOST_CHECK_EQUAL(hu, invert_heaviside(ihu));
    BOOST_CHECK_EQUAL(ihu, invert_heaviside(hu));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("invert_heaviside test");

    test->add(BOOST_TEST_CASE(&test_invert_heaviside));

    return test;
}
