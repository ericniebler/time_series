//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/characteristic_series.hpp>
#include <boost/time_series/constant_series.hpp>
#include <boost/time_series/delta_series.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/heaviside_series.hpp>
#include <boost/time_series/inverse_heaviside_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/clipped_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/clip.hpp>

using namespace boost;
using namespace unit_test;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

///////////////////////////////////////////////////////////////////////////////
// test_shift
//
template<typename Series>
void test_clip()
{
    Series base;

    time_series::make_ordered_inserter(base)
        (1, -1, 2)
        (2, 3, 5)
        (3, 6, 9)
    .commit();

    // Sanity check
    time_series::piecewise_constant_series<int> base_result;
    time_series::make_ordered_inserter(base_result)
        (1, -1, 2)
        (2, 3, 5)
        (3, 6, 9)
    .commit();
    BOOST_CHECK_EQUAL(base, base_result);

    // Test1: clip right 1
    Series test1 = time_series::clip(base, -1, 8);
    time_series::piecewise_constant_series<int> test1_result;
    time_series::make_ordered_inserter(test1_result)
        (1, -1, 2)
        (2, 3, 5)
        (3, 6, 8)
    .commit();
    BOOST_CHECK_EQUAL(test1.discretization(), 1);
    BOOST_CHECK_EQUAL(test1, test1_result);

    // Test2: clip left 1
    Series test2 = time_series::clip(base, 0, 9);
    time_series::piecewise_constant_series<int> test2_result;
    time_series::make_ordered_inserter(test2_result)
        (1, 0, 2)
        (2, 3, 5)
        (3, 6, 9)
    .commit();
    BOOST_CHECK_EQUAL(test2.discretization(), 1);
    BOOST_CHECK_EQUAL(test2, test2_result);

    // Test3: clip right 5
    Series test3 = time_series::clip(base, -1, 4);
    time_series::piecewise_constant_series<int> test3_result;
    time_series::make_ordered_inserter(test3_result)
        (1, -1, 2)
        (2, 3, 4)
    .commit();
    BOOST_CHECK_EQUAL(test3.discretization(), 1);
    BOOST_CHECK_EQUAL(test3, test3_result);

    // Test4: clip left 5
    Series test4 = time_series::clip(base, 4, 9);
    time_series::piecewise_constant_series<int> test4_result;
    time_series::make_ordered_inserter(test4_result)
        (2, 4, 5)
        (3, 6, 9)
    .commit();
    BOOST_CHECK_EQUAL(test4.discretization(), 1);
    BOOST_CHECK_EQUAL(test4, test4_result);

    // Test5: clip right 4, self-assign
    test3 = time_series::clip(test3, -1, 0);
    time_series::piecewise_constant_series<int> test5_result;
    time_series::make_ordered_inserter(test5_result)
        (1, -1, 0)
    .commit();
    BOOST_CHECK_EQUAL(test3.discretization(), 1);
    BOOST_CHECK_EQUAL(test3, test5_result);

    // Test6: clip left 4, self-assign
    test4 = time_series::clip(test4, 8, 9);
    time_series::piecewise_constant_series<int> test6_result;
    time_series::make_ordered_inserter(test6_result)
        (3, 8, 9)
    .commit();
    BOOST_CHECK_EQUAL(test4.discretization(), 1);
    BOOST_CHECK_EQUAL(test4, test6_result);
}

void test_clip2()
{
    using namespace boost;
    using namespace time_series;

    dense_series<int> d(start = -2, stop = 8, value = 5);
    clipped_series<dense_series<int> > cd = clip(d, 3, 8);

    piecewise_constant_series<int> test1_result;
    make_ordered_inserter(test1_result)
        (5, 3, 8)
    .commit();
    BOOST_CHECK_EQUAL(cd, test1_result);

    for(int i = -10; i < 20; ++i)
    {
        BOOST_CHECK_EQUAL(cd[i], test1_result[i]);
    }

    for(int i = -10; i < 20; ++i)
    {
        BOOST_CHECK_EQUAL(range_run_storage::get_at(cd, i), range_run_storage::get_at(test1_result, i));
    }

    constant_series<int> c(5);
    clipped_series<constant_series<int> > cc = clip(c, 3, 8);

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(cc), seq::end(cc)));
    BOOST_CHECK_EQUAL(3, rrs::offset(rrs::pre_run(cc)));
    BOOST_CHECK_EQUAL(5u, rrs::length(rrs::pre_run(cc)));
    BOOST_CHECK_EQUAL(5, rrs::pre_value(cc));

    clipped_series<constant_series<int> > cc2 = clip(c, 3, inf);

    BOOST_CHECK_EQUAL(0, std::distance(seq::begin(cc2), seq::end(cc2)));
    BOOST_CHECK_EQUAL(5, rrs::pre_value(cc2));
    BOOST_CHECK_EQUAL(3, rrs::offset(rrs::pre_run(cc2)));
    BOOST_CHECK(inf == rrs::end_offset(rrs::pre_run(cc2)));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("clip test");

    void (*pfn_dense)() = &test_clip<time_series::dense_series<int> >;
    test->add(BOOST_TEST_CASE(pfn_dense));

    void (*pfn_sparse)() = &test_clip<time_series::sparse_series<int> >;
    test->add(BOOST_TEST_CASE(pfn_sparse));

    void (*pfn_piecewise_constant)() = &test_clip<time_series::piecewise_constant_series<int> >;
    test->add(BOOST_TEST_CASE(pfn_piecewise_constant));

    test->add(BOOST_TEST_CASE(&test_clip2));

    return test;
}
