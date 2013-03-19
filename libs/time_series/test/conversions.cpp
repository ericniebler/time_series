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
#include <boost/time_series/shifted_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/numeric/shift.hpp>
#include <boost/time_series/numeric/numeric.hpp>

using namespace boost;
using namespace unit_test;
using namespace time_series;

characteristic_series<float> characteristic_float;
constant_series<float> constant_float;
delta_series<float> delta_float;
dense_series<float> dense_float;
heaviside_series<float> heaviside_float;
inverse_heaviside_series<float> inverse_heaviside_float;
piecewise_constant_series<float> piecewise_constant_float;
sparse_series<float> sparse_float;
characteristic_unit_series<float> characteristic_unit_float;
delta_unit_series<float> delta_unit_float;
heaviside_unit_series<float> heaviside_unit_float;
inverse_heaviside_unit_series<float> inverse_heaviside_unit_float;

characteristic_series<double> characteristic_double;
constant_series<double> constant_double;
delta_series<double> delta_double;
dense_series<double> dense_double;
heaviside_series<double> heaviside_double;
inverse_heaviside_series<double> inverse_heaviside_double;
piecewise_constant_series<double> piecewise_constant_double;
sparse_series<double> sparse_double;
characteristic_unit_series<double> characteristic_unit_double;
delta_unit_series<double> delta_unit_double;
heaviside_unit_series<double> heaviside_unit_double;
inverse_heaviside_unit_series<double> inverse_heaviside_unit_double;

///////////////////////////////////////////////////////////////////////////////
// test_conversions
//
void test_conversions()
{
    dense_double = characteristic_float;
    dense_double = constant_float;
    dense_double = delta_float;
    dense_double = dense_float;
    dense_double = heaviside_float;
    dense_double = inverse_heaviside_float;
    dense_double = piecewise_constant_float;
    dense_double = sparse_float;
    dense_double = characteristic_unit_float;
    dense_double = delta_unit_float;
    dense_double = heaviside_unit_float;
    dense_double = inverse_heaviside_unit_float;

    sparse_double = characteristic_float;
    sparse_double = constant_float;
    sparse_double = delta_float;
    sparse_double = dense_float;
    sparse_double = heaviside_float;
    sparse_double = inverse_heaviside_float;
    sparse_double = piecewise_constant_float;
    sparse_double = sparse_float;
    sparse_double = characteristic_unit_float;
    sparse_double = delta_unit_float;
    sparse_double = heaviside_unit_float;
    sparse_double = inverse_heaviside_unit_float;

    piecewise_constant_double = characteristic_float;
    piecewise_constant_double = constant_float;
    piecewise_constant_double = delta_float;
    piecewise_constant_double = dense_float;
    piecewise_constant_double = heaviside_float;
    piecewise_constant_double = inverse_heaviside_float;
    piecewise_constant_double = piecewise_constant_float;
    piecewise_constant_double = sparse_float;
    piecewise_constant_double = characteristic_unit_float;
    piecewise_constant_double = delta_unit_float;
    piecewise_constant_double = heaviside_unit_float;
    piecewise_constant_double = inverse_heaviside_unit_float;

    characteristic_unit_double = delta_unit_float;
    characteristic_unit_double = heaviside_unit_float;
    characteristic_unit_double = inverse_heaviside_unit_float;
    characteristic_unit_double = characteristic_unit_float;

    characteristic_double = delta_unit_float;
    characteristic_double = heaviside_unit_float;
    characteristic_double = inverse_heaviside_unit_float;
    characteristic_double = characteristic_unit_float;

    characteristic_double = delta_float;
    characteristic_double = heaviside_float;
    characteristic_double = inverse_heaviside_float;
    characteristic_double = characteristic_float;
}

///////////////////////////////////////////////////////////////////////////////
// test_shifted_conversions
//
void test_shifted_conversions()
{
    dense_double = shift(characteristic_float, 1);
    dense_double = shift(constant_float, 1);
    dense_double = shift(delta_float, 1);
    dense_double = shift(dense_float, 1);
    dense_double = shift(heaviside_float, 1);
    dense_double = shift(inverse_heaviside_float, 1);
    dense_double = shift(piecewise_constant_float, 1);
    dense_double = shift(sparse_float, 1);
    dense_double = shift(characteristic_unit_float, 1);
    dense_double = shift(delta_unit_float, 1);
    dense_double = shift(heaviside_unit_float, 1);
    dense_double = shift(inverse_heaviside_unit_float, 1);

    sparse_double = shift(characteristic_float, 1);
    sparse_double = shift(constant_float, 1);
    sparse_double = shift(delta_float, 1);
    sparse_double = shift(dense_float, 1);
    sparse_double = shift(heaviside_float, 1);
    sparse_double = shift(inverse_heaviside_float, 1);
    sparse_double = shift(piecewise_constant_float, 1);
    sparse_double = shift(sparse_float, 1);
    sparse_double = shift(characteristic_unit_float, 1);
    sparse_double = shift(delta_unit_float, 1);
    sparse_double = shift(heaviside_unit_float, 1);
    sparse_double = shift(inverse_heaviside_unit_float, 1);

    piecewise_constant_double = shift(characteristic_float, 1);
    piecewise_constant_double = shift(constant_float, 1);
    piecewise_constant_double = shift(delta_float, 1);
    piecewise_constant_double = shift(dense_float, 1);
    piecewise_constant_double = shift(heaviside_float, 1);
    piecewise_constant_double = shift(inverse_heaviside_float, 1);
    piecewise_constant_double = shift(piecewise_constant_float, 1);
    piecewise_constant_double = shift(sparse_float, 1);
    piecewise_constant_double = shift(characteristic_unit_float, 1);
    piecewise_constant_double = shift(delta_unit_float, 1);
    piecewise_constant_double = shift(heaviside_unit_float, 1);
    piecewise_constant_double = shift(inverse_heaviside_unit_float, 1);

    characteristic_unit_double = shift(delta_unit_float, 1);
    characteristic_unit_double = shift(heaviside_unit_float, 1);
    characteristic_unit_double = shift(inverse_heaviside_unit_float, 1);
    characteristic_unit_double = shift(characteristic_unit_float, 1);

    characteristic_double = shift(delta_unit_float, 1);
    characteristic_double = shift(heaviside_unit_float, 1);
    characteristic_double = shift(inverse_heaviside_unit_float, 1);
    characteristic_double = shift(characteristic_unit_float, 1);

    characteristic_double = shift(delta_float, 1);
    characteristic_double = shift(heaviside_float, 1);
    characteristic_double = shift(inverse_heaviside_float, 1);
    characteristic_double = shift(characteristic_float, 1);
}

///////////////////////////////////////////////////////////////////////////////
// test_scaled_conversions
//
void test_scaled_conversions()
{
    dense_double = characteristic_float * 1.0f;
    dense_double = constant_float * 1.0f;
    dense_double = delta_float * 1.0f;
    dense_double = dense_float * 1.0f;
    dense_double = heaviside_float * 1.0f;
    dense_double = inverse_heaviside_float * 1.0f;
    dense_double = piecewise_constant_float * 1.0f;
    dense_double = sparse_float * 1.0f;
    dense_double = characteristic_unit_float * 1.0f;
    dense_double = delta_unit_float * 1.0f;
    dense_double = heaviside_unit_float * 1.0f;
    dense_double = inverse_heaviside_unit_float * 1.0f;

    sparse_double = characteristic_float * 1.0f;
    sparse_double = constant_float * 1.0f;
    sparse_double = delta_float * 1.0f;
    sparse_double = dense_float * 1.0f;
    sparse_double = heaviside_float * 1.0f;
    sparse_double = inverse_heaviside_float * 1.0f;
    sparse_double = piecewise_constant_float * 1.0f;
    sparse_double = sparse_float * 1.0f;
    sparse_double = characteristic_unit_float * 1.0f;
    sparse_double = delta_unit_float * 1.0f;
    sparse_double = heaviside_unit_float * 1.0f;
    sparse_double = inverse_heaviside_unit_float * 1.0f;

    piecewise_constant_double = characteristic_float * 1.0f;
    piecewise_constant_double = constant_float * 1.0f;
    piecewise_constant_double = delta_float * 1.0f;
    piecewise_constant_double = dense_float * 1.0f;
    piecewise_constant_double = heaviside_float * 1.0f;
    piecewise_constant_double = inverse_heaviside_float * 1.0f;
    piecewise_constant_double = piecewise_constant_float * 1.0f;
    piecewise_constant_double = sparse_float * 1.0f;
    piecewise_constant_double = characteristic_unit_float * 1.0f;
    piecewise_constant_double = delta_unit_float * 1.0f;
    piecewise_constant_double = heaviside_unit_float * 1.0f;
    piecewise_constant_double = inverse_heaviside_unit_float * 1.0f;

    characteristic_unit_double = delta_unit_float * 1.0f;
    characteristic_unit_double = heaviside_unit_float * 1.0f;
    characteristic_unit_double = inverse_heaviside_unit_float * 1.0f;
    characteristic_unit_double = characteristic_unit_float * 1.0f;

    characteristic_double = delta_unit_float * 1.0f;
    characteristic_double = heaviside_unit_float * 1.0f;
    characteristic_double = inverse_heaviside_unit_float * 1.0f;
    characteristic_double = characteristic_unit_float * 1.0f;

    characteristic_double = delta_float * 1.0f;
    characteristic_double = heaviside_float * 1.0f;
    characteristic_double = inverse_heaviside_float * 1.0f;
    characteristic_double = characteristic_float * 1.0f;
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("series conversions test");

    test->add(BOOST_TEST_CASE(&test_conversions));
    test->add(BOOST_TEST_CASE(&test_shifted_conversions));
    test->add(BOOST_TEST_CASE(&test_scaled_conversions));

    return test;
}
