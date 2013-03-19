//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/numeric.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

///////////////////////////////////////////////////////////////////////////////
// test_dense_series
//
void test_dense_series()
{
    // sanity checks, shifted series.
    dense_series<int> ts(start = -1, stop = 3);

    make_ordered_inserter(ts)(42, -1).commit();

    BOOST_CHECK_EQUAL(-1, rrs::offset(rrs::runs(ts)(*seq::begin(ts))));
    BOOST_CHECK_EQUAL(42, seq::elements(ts)(*seq::begin(ts)));

    // check addition of two dense time series yields a dense time series
    dense_series<int> ss1;
    dense_series<int> ss2;

    make_ordered_inserter(ss1)(3, 1)(4, 2).commit();
    make_ordered_inserter(ss2)(4, 2)(5, 3).commit();

    dense_series<int> ss3 = ss1 + ss2;
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
    dense_series<int> s(start = -1, stop = 3);
    dense_series<int> const &cs = s;

    typedef rrs::unit_run<std::ptrdiff_t> run_type;
    rrs::set_at(s, run_type(-1), 1);

    BOOST_CHECK_EQUAL(4, std::distance(seq::begin(s), seq::end(s)));
    BOOST_CHECK_EQUAL(1, cs[-1]);
    BOOST_CHECK_EQUAL(0, cs[0]);
}

///////////////////////////////////////////////////////////////////////////////
// test_get_raw_buffer
//
void test_get_raw_buffer()
{
    dense_series<int> d(0, 20, 5);
    std::pair<int *, std::size_t> buff = get_raw_buffer(d);
    BOOST_CHECK_EQUAL(20u, buff.second);

    dense_series<int> const &cd = d;
    std::pair<int const *, std::size_t> cbuff = get_raw_buffer(cd);
    BOOST_CHECK_EQUAL(20u, cbuff.second);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("dense series test");

    test->add(BOOST_TEST_CASE(&test_dense_series));
    test->add(BOOST_TEST_CASE(&test_random_access));
    test->add(BOOST_TEST_CASE(&test_get_raw_buffer));

    return test;
}
