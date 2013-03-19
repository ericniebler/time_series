//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/numeric/numeric.hpp>
#include <boost/time_series/ordered_inserter.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

#define BOOST_CHECK_RANGE_RUN_EQUAL(x,y)\
    BOOST_CHECK_EQUAL(x, (boost::time_series::make_ordered_inserter(tmp) y .commit(), tmp))

///////////////////////////////////////////////////////////////////////////////
// test_piecewise_constant_series
//
template<typename Offset>
struct test_piecewise_constant_series
{
    static void call()
    {
        piecewise_constant_series<int,int,Offset> tmp; // needed by BOOST_CHECK_RANGE_RUN_EQUAL
        typedef std::pair<Offset, Offset> run;

        // check addition of a sparse time series and a dense time series yields
        // a dense time series
        piecewise_constant_series<int,int,Offset> pwc;

        rrs::set_at(pwc, run(4, 8), 4);
        BOOST_CHECK_RANGE_RUN_EQUAL(pwc, (4, 4, 8));

        rrs::set_at(pwc, run(0, 1), 42);
        BOOST_CHECK_RANGE_RUN_EQUAL(pwc, (42, 0)(4, 4, 8));

        rrs::set_at(pwc, run(3, 5), 4);
        BOOST_CHECK_RANGE_RUN_EQUAL(pwc, (42, 0)(4, 3, 8));

        rrs::set_at(pwc, run(10, 14), 10);
        BOOST_CHECK_RANGE_RUN_EQUAL(pwc, (42, 0)(4, 3, 8)(10, 10, 14));

        rrs::set_at(pwc, run(8, 12), 8);
        BOOST_CHECK_RANGE_RUN_EQUAL(pwc, (42, 0)(4, 3, 8)(8, 8, 12)(10, 12, 14));

        rrs::set_at(pwc, run(4, 11), 7);
        BOOST_CHECK_RANGE_RUN_EQUAL(pwc, (42, 0)(4, 3, 4)(7, 4, 11)(8, 11, 12)(10, 12, 14));

        rrs::set_at(pwc, run(3, 11), 17);
        BOOST_CHECK_RANGE_RUN_EQUAL(pwc, (42, 0)(17, 3, 11)(8, 11, 12)(10, 12, 14));

        rrs::set_at(pwc, run(13, 14), 10);
        BOOST_CHECK_RANGE_RUN_EQUAL(pwc, (42, 0)(17, 3, 11)(8, 11, 12)(10, 12, 14));

        rrs::set_at(pwc, run(13, 16), 10);
        BOOST_CHECK_RANGE_RUN_EQUAL(pwc, (42, 0)(17, 3, 11)(8, 11, 12)(10, 12, 16));

        rrs::set_at(pwc, run(0, 20), 20);
        BOOST_CHECK_RANGE_RUN_EQUAL(pwc, (20, 0, 20));
    }
};

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("piecewise_constant series test");

    test->add(BOOST_TEST_CASE(&test_piecewise_constant_series<std::ptrdiff_t>::call));
    test->add(BOOST_TEST_CASE(&test_piecewise_constant_series<double>::call));

    return test;
}
