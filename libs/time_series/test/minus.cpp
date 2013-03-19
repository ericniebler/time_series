//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/characteristic_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/numeric/numeric.hpp>
#include <boost/time_series/ordered_inserter.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

///////////////////////////////////////////////////////////////////////////////
// test_series_minus
//
void test_series_minus()
{
    // check addition of a sparse time series and a dense time series yields
    // a dense time series
    dense_series<int> ss1;
    sparse_series<int> ss2;

    make_ordered_inserter(ss1)(3, 1)(4, 2).commit();
    make_ordered_inserter(ss2)(4, 2)(5, 3).commit();

    BOOST_CHECK_EQUAL(2, std::distance(seq::begin(ss1), seq::end(ss1)));
    BOOST_CHECK_EQUAL(2, std::distance(seq::begin(ss2), seq::end(ss2)));

    dense_series<int> ss3 = ss1 - ss2;
    BOOST_CHECK_EQUAL(3, std::distance(seq::begin(ss3), seq::end(ss3)));
    BOOST_CHECK_EQUAL(3, seq::elements(ss3)(*seq::begin(ss3)));
    BOOST_CHECK_EQUAL(1, rrs::offset(rrs::runs(ss3)(*seq::begin(ss3))));
    BOOST_CHECK_EQUAL(0, seq::elements(ss3)(*++seq::begin(ss3)));
    BOOST_CHECK_EQUAL(2, rrs::offset(rrs::runs(ss3)(*++seq::begin(ss3))));
    BOOST_CHECK_EQUAL(-5, seq::elements(ss3)(*++++seq::begin(ss3)));
    BOOST_CHECK_EQUAL(3, rrs::offset(rrs::runs(ss3)(*++++seq::begin(ss3))));

    ss2 -= ss1;
    BOOST_CHECK_EQUAL(2, std::distance(seq::begin(ss2), seq::end(ss2)));
    BOOST_CHECK_EQUAL(-3, seq::elements(ss2)(*seq::begin(ss2)));
    BOOST_CHECK_EQUAL(1, rrs::offset(rrs::runs(ss2)(*seq::begin(ss2))));
    BOOST_CHECK_EQUAL(5, seq::elements(ss2)(*++seq::begin(ss2)));
    BOOST_CHECK_EQUAL(3, rrs::offset(rrs::runs(ss2)(*++seq::begin(ss2))));
}

///////////////////////////////////////////////////////////////////////////////
// test_series_minus_2
//
void test_series_minus_2()
{
    // check subtraction of two characteristic series yields
    // a piecewise_constant time series
    characteristic_unit_series<int> ss1;
    characteristic_unit_series<int> ss2;

    make_ordered_inserter(ss1)(1, 2, 6).commit();
    make_ordered_inserter(ss2)(1, 4, 8).commit();

    piecewise_constant_series<int> ss3 = ss1 - ss2;
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss3, 0));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss3, 1));
    BOOST_CHECK_EQUAL(1, rrs::get_at(ss3, 2));
    BOOST_CHECK_EQUAL(1, rrs::get_at(ss3, 3));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss3, 4));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss3, 5));
    BOOST_CHECK_EQUAL(-1, rrs::get_at(ss3, 6));
    BOOST_CHECK_EQUAL(-1, rrs::get_at(ss3, 7));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss3, 8));
    BOOST_CHECK_EQUAL(0, rrs::get_at(ss3, 9));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("series minus test");

    test->add(BOOST_TEST_CASE(&test_series_minus));
    test->add(BOOST_TEST_CASE(&test_series_minus_2));

    return test;
}
