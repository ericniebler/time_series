//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/inverse_heaviside_series.hpp>
#include <boost/time_series/numeric/numeric.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

using boost::time_series::inf;
std::ptrdiff_t const min_ = -inf;

///////////////////////////////////////////////////////////////////////////////
// test_inverse_heaviside_series
//
void test_inverse_heaviside_series()
{
    // sanity checks, shifted series.
    inverse_heaviside_unit_series<int> ts(1);
    BOOST_CHECK_EQUAL(min_, rrs::offset(rrs::pre_run(ts)));
    BOOST_CHECK_EQUAL(1, rrs::pre_value(ts));
    BOOST_CHECK_EQUAL(1, rrs::get_at(ts, -1));
    BOOST_CHECK_EQUAL(1, rrs::get_at(ts, 0));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ts, 1));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ts, 2));

    // check multiplication of two inverse_heaviside time series yields a inverse_heaviside time series
    inverse_heaviside_unit_series<int> ss1(1);
    inverse_heaviside_unit_series<int> ss2(2);
    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(ss1), seq::end(ss1)));
    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(ss2), seq::end(ss2)));

    BOOST_CHECK_EQUAL(1, rrs::get_at(ss1, 0));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss1, 1));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss1, 2));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss1, 3));

    BOOST_CHECK_EQUAL(1, rrs::get_at(ss2, 0));
    BOOST_CHECK_EQUAL(1, rrs::get_at(ss2, 1));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss2, 2));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss2, 3));

    inverse_heaviside_unit_series<int> ss3 = ss1 * ss2;
    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(ss3), seq::end(ss3)));
    BOOST_CHECK_EQUAL(1, rrs::pre_value(ss3));
    BOOST_CHECK_EQUAL(min_, rrs::offset(rrs::pre_run(ss3)));
    BOOST_CHECK_EQUAL(1, rrs::get_at(ss3, 0));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss3, 1));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss3, 2));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss3, 3));
}

///////////////////////////////////////////////////////////////////////////////
// test_random_access
//
void test_random_access()
{
    inverse_heaviside_unit_series<int> s(2);
    inverse_heaviside_unit_series<int> const &cs = s;

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(s), seq::end(s)));
    BOOST_CHECK_EQUAL(1, cs[-1]);
    BOOST_CHECK_EQUAL(1, cs[0]);
    BOOST_CHECK_EQUAL(1, cs[1]);
    BOOST_CHECK_EQUAL(0, cs[2]);
}

///////////////////////////////////////////////////////////////////////////////
// test_infinite
//
void test_infinite()
{
    inverse_heaviside_unit_series<int> ihs0(inf);
    inverse_heaviside_unit_series<int> ihs1(stop = inf);

    BOOST_CHECK_EQUAL(ihs0, ihs1);

    inverse_heaviside_unit_series<int> ihs2(-inf);
    inverse_heaviside_unit_series<int> ihs3(stop = -inf);

    BOOST_CHECK_EQUAL(ihs2, ihs3);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("inverse_heaviside series test");

    test->add(BOOST_TEST_CASE(&test_inverse_heaviside_series));
    test->add(BOOST_TEST_CASE(&test_random_access));
    test->add(BOOST_TEST_CASE(&test_infinite));

    return test;
}
