//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/characteristic_series.hpp>
#include <boost/time_series/constant_series.hpp>
#include <boost/time_series/delta_series.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/heaviside_series.hpp>
#include <boost/time_series/inverse_heaviside_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/numeric/invert_elements.hpp>
#include "./is_close.hpp"

using namespace boost;
using namespace unit_test;

///////////////////////////////////////////////////////////////////////////////
// test_invert_elements
//
void test_invert_elements()
{
    using namespace time_series;

    piecewise_constant_series<double> pwc;
    make_ordered_inserter(pwc)
        (1., 2, 4)
        (2., 6, 8)
        (4., 10, 12)
    .commit();

    piecewise_constant_series<double> expected;
    make_ordered_inserter(expected)
        (1., 2, 4)
        (.5, 6, 8)
        (.25, 10, 12)
    .commit();

    piecewise_constant_series<double> actual = invert_elements(pwc);
    BOOST_CHECK(is_close(expected, actual, 0.1));
}

///////////////////////////////////////////////////////////////////////////////
// test_invert_elements_inplace
//
void test_invert_elements_inplace()
{
    using namespace time_series;
    piecewise_constant_series<double> result;
    make_ordered_inserter(result)(.5, -inf, -10)(.25, -5, 5)(.2, 10, inf).commit();

    {
        characteristic_series<double> c(-10, 10, 2.);
        characteristic_series<double> result(-10, 10, 0.5);

        invert_elements_inplace(c);
        BOOST_CHECK(is_close(c, result, 0.1));
    }

    {
        constant_series<double> c(2.);
        constant_series<double> result(0.5);

        invert_elements_inplace(c);
        BOOST_CHECK(is_close(c, result, 0.1));
    }

    {
        delta_series<double> d(0, 2.);
        delta_series<double> result(0, 0.5);

        invert_elements_inplace(d);
        BOOST_CHECK(is_close(d, result, 0.1));
    }

    {
        dense_series<double> d;
        make_ordered_inserter(d)(2., -inf, -10)(4., -5, 5)(5., 10, inf).commit();

        invert_elements_inplace(d);
        BOOST_CHECK(is_close(d, result, 0.1));
    }

    {
        heaviside_series<double> h(10, 2.);
        heaviside_series<double> result(10, 0.5);

        invert_elements_inplace(h);
        BOOST_CHECK(is_close(h, result, 0.1));
    }

    {
        inverse_heaviside_series<double> ih(10, 2.);
        inverse_heaviside_series<double> result(10, 0.5);

        invert_elements_inplace(ih);
        BOOST_CHECK(is_close(ih, result, 0.1));
    }

    {
        piecewise_constant_series<double> pwc;
        make_ordered_inserter(pwc)(2., -inf, -10)(4., -5, 5)(5., 10, inf).commit();

        invert_elements_inplace(pwc);
        BOOST_CHECK(is_close(pwc, result, 0.1));
    }

    {
        sparse_series<double> s;
        make_ordered_inserter(s)(2., -inf, -10)(4., -5, 5)(5., 10, inf).commit();

        invert_elements_inplace(s);
        BOOST_CHECK(is_close(s, result, 0.1));
    }

}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("invert_elements test");

    test->add(BOOST_TEST_CASE(&test_invert_elements));
    test->add(BOOST_TEST_CASE(&test_invert_elements_inplace));

    return test;
}
