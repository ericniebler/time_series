//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/delta_series.hpp>
#include <boost/time_series/numeric/numeric.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

///////////////////////////////////////////////////////////////////////////////
// test_delta_series
//
void test_delta_series()
{
    // sanity checks, delta series.
    delta_unit_series<int> ts(-1);
    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(ts), seq::end(ts)));
    BOOST_CHECK_EQUAL(-1, rrs::offset(rrs::pre_run(ts)));
    BOOST_CHECK_EQUAL(1, rrs::pre_value(ts));
}

///////////////////////////////////////////////////////////////////////////////
// test_random_access
//
void test_random_access()
{
    delta_unit_series<int> s(1);
    delta_unit_series<int> const &cs = s;

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(s), seq::end(s)));
    BOOST_CHECK_EQUAL(0, cs[-1]);
    BOOST_CHECK_EQUAL(0, cs[0]);
    BOOST_CHECK_EQUAL(1, cs[1]);
    BOOST_CHECK_EQUAL(0, cs[2]);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("delta series test");

    test->add(BOOST_TEST_CASE(&test_delta_series));
    test->add(BOOST_TEST_CASE(&test_random_access));

    return test;
}
