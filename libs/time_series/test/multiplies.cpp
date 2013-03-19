//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/delta_series.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/characteristic_series.hpp>
#include <boost/time_series/numeric/numeric.hpp>
#include <boost/time_series/ordered_inserter.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;
typedef rrs::unit_run<std::ptrdiff_t> rng;

///////////////////////////////////////////////////////////////////////////////
// test_series_multiplies
//
void test_series_multiplies()
{
    // check multiplication of a sparse time series and a dense time series yields
    // a sparse time series
    dense_series<int> ss1;
    sparse_series<int> ss2;

    make_ordered_inserter(ss1)(3, 1)(4, 2).commit();
    make_ordered_inserter(ss2)(4, 2)(5, 3).commit();

    BOOST_CHECK_EQUAL(2, std::distance(seq::begin(ss1), seq::end(ss1)));
    BOOST_CHECK_EQUAL(2, std::distance(seq::begin(ss2), seq::end(ss2)));

    sparse_series<int> ss3 = ss1 * ss2;
    BOOST_CHECK_EQUAL(1, std::distance(seq::begin(ss3), seq::end(ss3)));
    BOOST_CHECK_EQUAL(16, seq::elements(ss3)(*seq::begin(ss3)));
    BOOST_CHECK_EQUAL(2, rrs::offset(rrs::runs(ss3)(*seq::begin(ss3))));

    ss2 *= ss1;
    BOOST_CHECK_EQUAL(1, std::distance(seq::begin(ss2), seq::end(ss2)));
    BOOST_CHECK_EQUAL(16, seq::elements(ss2)(*seq::begin(ss2)));
    BOOST_CHECK_EQUAL(2, rrs::offset(rrs::runs(ss2)(*seq::begin(ss2))));

    //// check multiplication of a dense time series and a delta time series yields
    //// a scaled delta time series
    //descriptor<> disc2(size = 4, offset = -1);
    //dense_series<int> ss4(disc2);
    //delta_unit_series<int> ss5(disc2, 2);
    //rrs::set_at(ss4, rng(1), 3);
    //rrs::set_at(ss4, rng(2), 4);
    //BOOST_CHECK_EQUAL(4, std::distance(seq::begin(ss4), seq::end(ss4)));
    //BOOST_CHECK_EQUAL(1, std::distance(seq::begin(ss5), seq::end(ss5)));

    //time_series_facade<int, scaled_storage_tag<delta_storage_tag>, daily> ss6 = ss4 * ss5;
    //BOOST_CHECK_EQUAL(1, std::distance(seq::begin(ss6), seq::end(ss6)));
    //BOOST_CHECK_EQUAL(16, seq::elements(ss6)(*seq::begin(ss6)));
    //BOOST_CHECK_EQUAL(2, rrs::offset(rrs::runs(ss6)(*seq::begin(ss6))));
}

///////////////////////////////////////////////////////////////////////////////
// test_series_multiplies2
//
void test_series_multiplies2()
{
    piecewise_constant_series<int> discounts;
    make_ordered_inserter(discounts)
        (1, -inf, 1)
        (2, 1, 7)
        (3, 7, 30)
        (4, 30, 60)
        (5, 60, 90)
    .commit();

    piecewise_constant_series<int> expected;
    make_ordered_inserter(expected)
        (1, 0, 1)
        (2, 1, 7)
        (3, 7, 30)
    .commit();

    characteristic_unit_series<int> clip_range(0, 30);
    piecewise_constant_series<double> result = discounts * clip_range;

    BOOST_CHECK_EQUAL(expected, result);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("series multiplies test");

    test->add(BOOST_TEST_CASE(&test_series_multiplies));
    test->add(BOOST_TEST_CASE(&test_series_multiplies2));

    return test;
}
