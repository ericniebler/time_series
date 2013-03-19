//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/time_series/characteristic_series.hpp>
#include <boost/time_series/constant_series.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/heaviside_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/shifted_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/shift.hpp>

using namespace boost;
using namespace unit_test;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

using time_series::inf;

std::ptrdiff_t min_ = -inf;
std::ptrdiff_t max_ = inf;

time_series::piecewise_constant_series<int> tmp;
#define BOOST_CHECK_RANGE_RUN_EQUAL(x,y)\
    BOOST_CHECK_EQUAL(x, (boost::time_series::make_ordered_inserter(::tmp) y .commit(), ::tmp))

///////////////////////////////////////////////////////////////////////////////
// test_shift
//
template<typename Series>
void test_shift()
{
    Series base;

    time_series::make_ordered_inserter(base)
        (1, -1, 2)
        (2, 3, 5)
        (3, 6, 9)
    .commit();

    // Sanity check
    BOOST_CHECK_RANGE_RUN_EQUAL(base, (1, -1, 2)(2, 3, 5)(3, 6, 9));

    // Test1: shift right 1
    Series test1 = time_series::shift(base, 1);
    BOOST_CHECK_EQUAL(test1.discretization(), 1);
    BOOST_CHECK_RANGE_RUN_EQUAL(test1, (1, 0, 3)(2, 4, 6)(3, 7, 10));

    // Test2: shift left 1
    Series test2 = time_series::shift(base, -1);
    BOOST_CHECK_EQUAL(test2.discretization(), 1);
    BOOST_CHECK_RANGE_RUN_EQUAL(test2, (1, -2, 1)(2, 2, 4)(3, 5, 8));

    // Test3: shift right 5
    Series test3 = time_series::shift(base, 5);
    BOOST_CHECK_EQUAL(test3.discretization(), 1);
    BOOST_CHECK_RANGE_RUN_EQUAL(test3, (1, 4, 7)(2, 8, 10)(3, 11, 14));

    // Test4: shift left 5
    Series test4 = time_series::shift(base, -5);
    BOOST_CHECK_EQUAL(test4.discretization(), 1);
    BOOST_CHECK_RANGE_RUN_EQUAL(test4, (1, -6, -3)(2, -2, 0)(3, 1, 4));

    // Test5: shift right 4, self-assign
    test3 = time_series::shift(test3, 4);
    BOOST_CHECK_EQUAL(test3.discretization(), 1);
    BOOST_CHECK_RANGE_RUN_EQUAL(test3, (1, 8, 11)(2, 12, 14)(3, 15, 18));

    // Test6: shift left 4, self-assign
    test4 = time_series::shift(test4, -4);
    BOOST_CHECK_EQUAL(test4.discretization(), 1);
    BOOST_CHECK_RANGE_RUN_EQUAL(test4, (1, -10, -7)(2, -6, -4)(3, -3, 0));
}

void test_shift2()
{
    using namespace boost;
    using namespace time_series;

    dense_series<int> d(-2, 8, 5);
    shifted_series<dense_series<int> > sd = shift(d, 5);

    characteristic_series<int> ch(3, 13, 5);
    BOOST_CHECK_EQUAL(ch, sd);

    constant_series<int> c(5);
    shifted_series<constant_series<int> > sc = shift(c, -5);

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(sc), seq::end(sc)));
    BOOST_CHECK_EQUAL(min_, rrs::offset(rrs::pre_run(sc)));
    BOOST_CHECK_EQUAL(max_, rrs::end_offset(rrs::pre_run(sc)));
    BOOST_CHECK_EQUAL(5, rrs::pre_value(sc));
}

void test_heaviside()
{
    using namespace time_series;

    heaviside_unit_series<int> h(start = 5);
    heaviside_unit_series<int> r(start = 4);
    h = shift(h, -1);

    BOOST_CHECK_EQUAL(h, r);

    make_ordered_inserter(r)(1, 6, inf).commit();
    h = shift(h, 2);

    BOOST_CHECK_EQUAL(h, r);
}

void test_floating_point_shift()
{
    using namespace boost;
    using namespace time_series;

    characteristic_series<int,int,double> ch(3.5, 13.5, 5);
    shifted_series<characteristic_series<int,int,double> > sch = shift(ch, -0.5);

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(sch), seq::end(sch)));
    BOOST_CHECK_CLOSE(3.0, rrs::offset(rrs::pre_run(sch)),0.1);
    BOOST_CHECK_CLOSE(13.0, rrs::end_offset(rrs::pre_run(sch)),0.1);
    BOOST_CHECK_EQUAL(5, rrs::pre_value(sch));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("shift test");

    void (*pfn_dense)() = &test_shift<time_series::dense_series<int> >;
    test->add(BOOST_TEST_CASE(pfn_dense));

    void (*pfn_sparse)() = &test_shift<time_series::sparse_series<int> >;
    test->add(BOOST_TEST_CASE(pfn_sparse));

    void (*pfn_piecewise_constant)() = &test_shift<time_series::piecewise_constant_series<int> >;
    test->add(BOOST_TEST_CASE(pfn_piecewise_constant));

    test->add(BOOST_TEST_CASE(&test_shift2));
    test->add(BOOST_TEST_CASE(&test_heaviside));
    test->add(BOOST_TEST_CASE(&test_floating_point_shift));

    return test;
}
