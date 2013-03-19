//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/constant_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/numeric/numeric.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

std::ptrdiff_t min_ = -inf;
std::ptrdiff_t max_ = inf;

boost::time_series::piecewise_constant_series<int> tmp;
#define BOOST_CHECK_RANGE_RUN_EQUAL(x,y)\
    BOOST_CHECK_EQUAL(x, (boost::time_series::make_ordered_inserter(::tmp) y .commit(), ::tmp))

///////////////////////////////////////////////////////////////////////////////
// test_constant_series
//
void test_constant_series()
{
    // check addition of two constant series yields a constant series
    constant_series<int> c1(4);
    constant_series<int> c2(5);

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(c1), seq::end(c1)));
    BOOST_CHECK_EQUAL(min_, rrs::offset(rrs::pre_run(c1)));
    BOOST_CHECK_EQUAL(max_, rrs::end_offset(rrs::pre_run(c1)));
    BOOST_CHECK_EQUAL(4, rrs::pre_value(c1));

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(c2), seq::end(c2)));
    BOOST_CHECK_EQUAL(min_, rrs::offset(rrs::pre_run(c2)));
    BOOST_CHECK_EQUAL(max_, rrs::end_offset(rrs::pre_run(c2)));
    BOOST_CHECK_EQUAL(5, rrs::pre_value(c2));

    constant_series<int> c3 = c1 + c2;

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(c3), seq::end(c3)));
    BOOST_CHECK_EQUAL(min_, rrs::offset(rrs::pre_run(c3)));
    BOOST_CHECK_EQUAL(max_, rrs::end_offset(rrs::pre_run(c3)));
    BOOST_CHECK_EQUAL(9, rrs::pre_value(c3));

    c3 += c1;

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(c3), seq::end(c3)));
    BOOST_CHECK_EQUAL(min_, rrs::offset(rrs::pre_run(c3)));
    BOOST_CHECK_EQUAL(max_, rrs::end_offset(rrs::pre_run(c3)));
    BOOST_CHECK_EQUAL(13, rrs::pre_value(c3));
}

///////////////////////////////////////////////////////////////////////////////
// test_constant_series_2
//
void test_constant_series_2()
{
    typedef rrs::unit_run<std::ptrdiff_t> run; // sparse run

    // check addition of two constant series yields a constant series
    constant_series<int> c(42);
    sparse_series<int> s;

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(c), seq::end(c)));

    make_ordered_inserter(s)
        (8, 2)
        (6, 4)
        (4, 6)
        (2, 8)
    .commit();

    BOOST_CHECK_EQUAL(4, std::distance(seq::begin(s), seq::end(s)));

    piecewise_constant_series<int> pwc = c + s;
    boost::result_of<seq::op::begin(piecewise_constant_series<int> &)>::type where;

    BOOST_CHECK_RANGE_RUN_EQUAL(
        pwc
      , (42, -inf, 2)(50, 2, 3)(42, 3, 4)(48, 4, 5)(42, 5, 6)(46, 6, 7)(42, 7, 8)(44, 8, 9)(42, 9, inf)
    );
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("constant series test");

    test->add(BOOST_TEST_CASE(&test_constant_series));
    test->add(BOOST_TEST_CASE(&test_constant_series_2));

    return test;
}
