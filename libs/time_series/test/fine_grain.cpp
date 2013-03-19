// Copyright Daniel Wallin 2006. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/numeric/fine_grain.hpp>
#include <boost/foreach.hpp>

using namespace boost;
using namespace unit_test;

void test_fine_grain()
{
    using namespace boost::time_series;

    sparse_series<int> s1(discretization = 4);

    make_ordered_inserter(s1)
        (1, 0)(2)(3)(4)(5)(6)(7)
    .commit();

    piecewise_constant_series<int> r11(discretization = 2);

    make_ordered_inserter(r11)
        (1, 0, 2)
        (2, 2, 4)
        (3, 4, 6)
        (4, 6, 8)
        (5, 8, 10)
        (6, 10, 12)
        (7, 12, 14)
    .commit();

    BOOST_CHECK_EQUAL(r11, fine_grain(s1, 2));

    sparse_series<int> r12(discretization = 2);

    make_ordered_inserter(r12)
        (1, 0)
        (2, 2)
        (3, 4)
        (4, 6)
        (5, 8)
        (6, 10)
        (7, 12)
    .commit();

    BOOST_CHECK_EQUAL(r12, fine_grain(s1, 2, sparse_upsample));

    sparse_series<int> s2(discretization = 2);

    make_ordered_inserter(s2)
        (1, -2)(2)(3)(4)
    .commit();

    sparse_series<int> r2;

    make_ordered_inserter(r2)
        (1, -4)(0)(2)(0)(3)(0)(4)(0)
    .commit();

    BOOST_CHECK_EQUAL(r2, fine_grain(s2, 1, sparse_upsample));

    sparse_series<int, mpl::int_<10> > s3;

    make_ordered_inserter(s3)
        (1, 0)(2)(3)(4)
    .commit();

    piecewise_constant_series<int, mpl::int_<5> > r3;

    make_ordered_inserter(r3)
        (1, 0, 2)
        (2, 2, 4)
        (3, 4, 6)
        (4, 6, 8)
    .commit();

    BOOST_CHECK_EQUAL(r3, fine_grain(s3, mpl::int_<5>()));

    piecewise_constant_series<int> o1(discretization = 2);

    ordered_inserter<piecewise_constant_series<int> > out1(o1);
    fine_grain(s1, 2, out1).commit();

    BOOST_CHECK_EQUAL(r11, o1);

    sparse_series<int> o2(discretization = 2);

    ordered_inserter<sparse_series<int> > out2(o2);
    fine_grain(s1, 2, sparse_upsample, out2).commit();

    BOOST_CHECK_EQUAL(r12, o2);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("fine_grain test");

    test->add(BOOST_TEST_CASE(&test_fine_grain));

    return test;
}
