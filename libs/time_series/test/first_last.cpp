//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <boost/test/unit_test.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/characteristic_series.hpp>
#include <boost/time_series/constant_series.hpp>
#include <boost/time_series/delta_series.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/heaviside_series.hpp>
#include <boost/time_series/inverse_heaviside_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/range_run_storage/algorithm/first_last.hpp>

using namespace boost;
using namespace unit_test;

std::ptrdiff_t const neg_inf = -time_series::inf;
std::ptrdiff_t const pos_inf = time_series::inf;

///////////////////////////////////////////////////////////////////////////////
// test_first_last
//
void test_first_last()
{
    namespace rrs = range_run_storage;
    using namespace time_series;

    {
        characteristic_series<int> expected(-10, 10, 2, 30, 3);

        BOOST_CHECK_EQUAL(-10, rrs::offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(10, rrs::end_offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::first_value(expected));

        BOOST_CHECK_EQUAL(-10, rrs::offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(10, rrs::end_offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::last_value(expected));
    }

    {
        constant_series<int> expected(2, 30);

        BOOST_CHECK_EQUAL(neg_inf, rrs::offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(pos_inf, rrs::end_offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::first_value(expected));

        BOOST_CHECK_EQUAL(neg_inf, rrs::offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(pos_inf, rrs::end_offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::last_value(expected));
    }

    {
        delta_series<int> expected(0, 2, 30, 3);

        BOOST_CHECK_EQUAL(0, rrs::offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(1, rrs::end_offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::first_value(expected));

        BOOST_CHECK_EQUAL(0, rrs::offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(1, rrs::end_offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::last_value(expected));
    }

    {
        dense_series<int> expected(discretization = 30, zero = 3);
        make_ordered_inserter(expected)(2, -inf, -10)(4, -5, 5)(5, 10, inf).commit();

        BOOST_CHECK_EQUAL(neg_inf, rrs::offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(-10, rrs::end_offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::first_value(expected));

        BOOST_CHECK_EQUAL(10, rrs::offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(pos_inf, rrs::end_offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(5, rrs::last_value(expected));
    }

    {
        heaviside_series<int> expected(10, 2, 30, 3);

        BOOST_CHECK_EQUAL(10, rrs::offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(pos_inf, rrs::end_offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::first_value(expected));

        BOOST_CHECK_EQUAL(10, rrs::offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(pos_inf, rrs::end_offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::last_value(expected));
    }

    {
        inverse_heaviside_series<int> expected(10, 2, 30, 3);

        BOOST_CHECK_EQUAL(neg_inf, rrs::offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(10, rrs::end_offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::first_value(expected));

        BOOST_CHECK_EQUAL(neg_inf, rrs::offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(10, rrs::end_offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::last_value(expected));
    }

    {
        piecewise_constant_series<int> expected(discretization = 30, zero = 3);
        make_ordered_inserter(expected)(2, -inf, -10)(4, -5, 5)(5, 10, inf).commit();

        BOOST_CHECK_EQUAL(neg_inf, rrs::offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(-10, rrs::end_offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::first_value(expected));

        BOOST_CHECK_EQUAL(10, rrs::offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(pos_inf, rrs::end_offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(5, rrs::last_value(expected));
    }

    {
        sparse_series<int> expected(discretization = 30, zero = 3);
        make_ordered_inserter(expected)(2, -inf, -10)(4, -5, 5)(5, 10, inf).commit();

        BOOST_CHECK_EQUAL(neg_inf, rrs::offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(-10, rrs::end_offset(rrs::first_run(expected)));
        BOOST_CHECK_EQUAL(2, rrs::first_value(expected));

        BOOST_CHECK_EQUAL(10, rrs::offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(pos_inf, rrs::end_offset(rrs::last_run(expected)));
        BOOST_CHECK_EQUAL(5, rrs::last_value(expected));
    }
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("series serialization test");

    test->add(BOOST_TEST_CASE(&test_first_last));

    return test;
}
