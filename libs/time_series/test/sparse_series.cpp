//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/numeric/numeric.hpp>
#include <boost/time_series/ordered_inserter.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

///////////////////////////////////////////////////////////////////////////////
// test_sparse_series
//
void test_sparse_series()
{
    // sanity checks, shifted series.
    sparse_series<int> ts;

    make_ordered_inserter(ts)(42, -1).commit();

    BOOST_CHECK_EQUAL(-1, rrs::offset(rrs::runs(ts)(*seq::begin(ts))));
    BOOST_CHECK_EQUAL(42, seq::elements(ts)(*seq::begin(ts)));

    // check addition of two sparse time series yields a sparse time series
    sparse_series<int> ss1;
    sparse_series<int> ss2;

    make_ordered_inserter(ss1)(3, 1)(4, 2).commit();
    make_ordered_inserter(ss2)(4, 2)(5, 3).commit();

    BOOST_CHECK_EQUAL(2, std::distance(seq::begin(ss1), seq::end(ss1)));
    BOOST_CHECK_EQUAL(2, std::distance(seq::begin(ss2), seq::end(ss2)));

    sparse_series<int> ss3 = ss1 + ss2;
    BOOST_CHECK_EQUAL(3, std::distance(seq::begin(ss3), seq::end(ss3)));
    BOOST_CHECK_EQUAL(3, seq::elements(ss3)(*seq::begin(ss3)));
    BOOST_CHECK_EQUAL(1, rrs::offset(rrs::runs(ss3)(*seq::begin(ss3))));
    BOOST_CHECK_EQUAL(8, seq::elements(ss3)(*++seq::begin(ss3)));
    BOOST_CHECK_EQUAL(2, rrs::offset(rrs::runs(ss3)(*++seq::begin(ss3))));
    BOOST_CHECK_EQUAL(5, seq::elements(ss3)(*++++seq::begin(ss3)));
    BOOST_CHECK_EQUAL(3, rrs::offset(rrs::runs(ss3)(*++++seq::begin(ss3))));
}

///////////////////////////////////////////////////////////////////////////////
// test_random_access
//
void test_random_access()
{
    sparse_series<int> s;
    sparse_series<int> const &cs = s;

    make_ordered_inserter(s)(1, -1).commit();

    BOOST_CHECK_EQUAL(1, std::distance(seq::begin(s), seq::end(s)));
    BOOST_CHECK_EQUAL(1, cs[-1]);
    BOOST_CHECK_EQUAL(0, cs[0]);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("sparse series test");

    test->add(BOOST_TEST_CASE(&test_sparse_series));
    test->add(BOOST_TEST_CASE(&test_random_access));

    return test;
}
