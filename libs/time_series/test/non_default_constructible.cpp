//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/time_series/characteristic_series.hpp>
#include <boost/time_series/constant_series.hpp>
#include <boost/time_series/delta_series.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/heaviside_series.hpp>
#include <boost/time_series/inverse_heaviside_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/numeric/numeric.hpp>
#include <boost/time_series/ordered_inserter.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

// doesn't have a default constructor
struct Int
{
    explicit Int(int i) : value(i) {}

    friend bool operator==(Int l, Int r) { return l.value == r.value; }
    friend bool operator!=(Int l, Int r) { return l.value != r.value; }
    friend Int operator+(Int l, Int r) { return Int(l.value + r.value); }
    friend Int operator*(Int l, Int r) { return Int(l.value * r.value); }

    int value;
};

inline std::ostream &operator <<(std::ostream &sout, Int const &i)
{
    return sout << i.value;
}

//namespace boost { namespace numeric
//{
//    template<>
//    Int const zero<Int>::value = Int(0);
//}}

///////////////////////////////////////////////////////////////////////////////
// test_characteristic_series
//
void test_characteristic_series()
{
    characteristic_series<Int> s(start = 4, stop = 6, value = Int(5), zero = Int(0));
    BOOST_CHECK_EQUAL(s[0], Int(0));
    BOOST_CHECK_EQUAL(s[4], Int(5));

    make_ordered_inserter(s)(Int(1), 0).commit();
    BOOST_CHECK_EQUAL(s[0], Int(1));
    BOOST_CHECK_EQUAL(s[4], Int(0));
}

/*
///////////////////////////////////////////////////////////////////////////////
// test_constant_series
//
void test_constant_series()
{
    constant_series<Int> s(value = Int(5));
    BOOST_CHECK_EQUAL(s[4], Int(5));

    make_ordered_inserter(s)(Int(1), -inf, inf).commit();
    BOOST_CHECK_EQUAL(s[0], Int(1));
}
//*/

///////////////////////////////////////////////////////////////////////////////
// test_dense_series
//
void test_dense_series()
{
    dense_series<Int> s(stop = 10, zero = Int(0));
    BOOST_CHECK_EQUAL(s[4], Int(0));

    make_ordered_inserter(s)(Int(1), 0).commit();
    BOOST_CHECK_EQUAL(s[0], Int(1));
}

///////////////////////////////////////////////////////////////////////////////
// test_delta_series
//
void test_delta_series()
{
    delta_series<Int> s(value = Int(5), start = 1, zero = Int(0));
    BOOST_CHECK_EQUAL(s[0], Int(0));
    BOOST_CHECK_EQUAL(s[1], Int(5));

    make_ordered_inserter(s)(Int(1), 0).commit();
    BOOST_CHECK_EQUAL(s[0], Int(1));
    BOOST_CHECK_EQUAL(s[1], Int(0));
}

///////////////////////////////////////////////////////////////////////////////
// test_heaviside_series
//
void test_heaviside_series()
{
    heaviside_series<Int> s(value = Int(5), start = 1, zero = Int(0));
    BOOST_CHECK_EQUAL(s[-1], Int(0));
    BOOST_CHECK_EQUAL(s[0], Int(0));
    BOOST_CHECK_EQUAL(s[1], Int(5));
    BOOST_CHECK_EQUAL(s[2], Int(5));

    make_ordered_inserter(s)(Int(1), 0, inf).commit();
    BOOST_CHECK_EQUAL(s[-1], Int(0));
    BOOST_CHECK_EQUAL(s[0], Int(1));
    BOOST_CHECK_EQUAL(s[1], Int(1));
    BOOST_CHECK_EQUAL(s[2], Int(1));
}

///////////////////////////////////////////////////////////////////////////////
// test_inverse_heaviside_series
//
void test_inverse_heaviside_series()
{
    inverse_heaviside_series<Int> s(value = Int(5), stop = 1, zero = Int(0));
    BOOST_CHECK_EQUAL(s[-1], Int(5));
    BOOST_CHECK_EQUAL(s[0], Int(5));
    BOOST_CHECK_EQUAL(s[1], Int(0));
    BOOST_CHECK_EQUAL(s[2], Int(0));

    make_ordered_inserter(s)(Int(1), -inf, 0).commit();
    BOOST_CHECK_EQUAL(s[-1], Int(1));
    BOOST_CHECK_EQUAL(s[0], Int(0));
    BOOST_CHECK_EQUAL(s[1], Int(0));
    BOOST_CHECK_EQUAL(s[2], Int(0));
}

///////////////////////////////////////////////////////////////////////////////
// test_piecewise_constant_series
//
void test_piecewise_constant_series()
{
    piecewise_constant_series<Int> s(zero = Int(0));
    BOOST_CHECK_EQUAL(s[0], Int(0));

    make_ordered_inserter(s)(Int(1), 0).commit();
    BOOST_CHECK_EQUAL(s[0], Int(1));
    BOOST_CHECK_EQUAL(s[1], Int(0));
}

///////////////////////////////////////////////////////////////////////////////
// test_sparse_series
//
void test_sparse_series()
{
    sparse_series<Int> s(zero = Int(0));
    BOOST_CHECK_EQUAL(s[0], Int(0));
    BOOST_CHECK_EQUAL(s[1], Int(0));

    make_ordered_inserter(s)(Int(1), 0).commit();
    BOOST_CHECK_EQUAL(s[0], Int(1));
    BOOST_CHECK_EQUAL(s[1], Int(0));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("non_default_constructible test");

    test->add(BOOST_TEST_CASE(&test_characteristic_series));
    //test->add(BOOST_TEST_CASE(&test_constant_series));
    test->add(BOOST_TEST_CASE(&test_dense_series));
    test->add(BOOST_TEST_CASE(&test_delta_series));
    test->add(BOOST_TEST_CASE(&test_heaviside_series));
    test->add(BOOST_TEST_CASE(&test_inverse_heaviside_series));
    test->add(BOOST_TEST_CASE(&test_piecewise_constant_series));
    test->add(BOOST_TEST_CASE(&test_sparse_series));

    return test;
}
