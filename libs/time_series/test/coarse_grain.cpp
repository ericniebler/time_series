// Copyright Daniel Wallin 2006. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/numeric/coarse_grain.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/dense_series.hpp>

using namespace boost;
using namespace unit_test;

void test_coarse_grain()
{
    using namespace boost::time_series;

    sparse_series<int> s1;

    make_ordered_inserter(s1)
        (1, 0)(2)(3)(4)(5)(6)(7)
    .commit();

    sparse_series<int> r1(discretization = 3);

    make_ordered_inserter(r1)
        (1, 0)(4)(7)
    .commit();

    BOOST_CHECK_EQUAL(r1, coarse_grain(s1, 3));

    sparse_series<int> r2(discretization = 2);

    make_ordered_inserter(r2)
        (1, 0)(3)(5)(7)
    .commit();

    BOOST_CHECK_EQUAL(r2, coarse_grain(s1, 2));

    sparse_series<int> s2(discretization = 10);

    make_ordered_inserter(s2)
        (1, 2, 4)
        (2, 6, 8)
    .commit();

    sparse_series<int> r3(discretization = 50);

    make_ordered_inserter(r3)
        (0, 0)(0)
    .commit();

    BOOST_CHECK_EQUAL(r3, coarse_grain(s2, 50));

    piecewise_constant_series<int> s3(discretization = 30);

    make_ordered_inserter(s3)
        (1, 2, 4)
        (2, 6, 8)
    .commit();

    sparse_series<int> r4(discretization = 60);

    make_ordered_inserter(r4)
        (0, 0)(1)(0)(2)(0)
    .commit();

    BOOST_CHECK_EQUAL(r4, coarse_grain(s3, 60));

    dense_series<int> d(stop = 100, value = 2);

    sparse_series<int> r5(discretization = 10);

    make_ordered_inserter(r5)
        (2, 0)(2)(2)(2)(2)(2)(2)(2)(2)(2)
    .commit();

    BOOST_CHECK_EQUAL(r5, coarse_grain(d, 10));

    piecewise_constant_series<int> s4;

    make_ordered_inserter(s4)
        (1, 0, 3)
        (3, 8, 11)
    .commit();

    sparse_series<int> r6(discretization = 2);

    make_ordered_inserter(r6)
        (1, 0)(1)(0)(0)(3)(3)
    .commit();

    BOOST_CHECK_EQUAL(r6, coarse_grain(s4, 2));

    dense_series<int, mpl::int_<10> > s5;
    make_ordered_inserter(s5)
        (1, 0)(2)(3)(4)
    .commit();

    sparse_series<int, mpl::int_<20> > r7;

    make_ordered_inserter(r7)
        (1, 0)(3)
    .commit();

    BOOST_CHECK_EQUAL(r7, coarse_grain(s5, mpl::int_<20>()));

    sparse_series<int> o4(discretization = 60);
    ordered_inserter<sparse_series<int> > out(o4);

    coarse_grain(s3, 60, out).commit();
    BOOST_CHECK_EQUAL(r4, o4);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("coarse_grain test");

    test->add(BOOST_TEST_CASE(&test_coarse_grain));

    return test;
}

