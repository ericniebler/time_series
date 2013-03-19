//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/time_series/traits/promotion.hpp>

using namespace boost;
using namespace unit_test;

///////////////////////////////////////////////////////////////////////////////
// test_plus_promotion
//
void test_plus_promotion()
{
    using namespace time_series;
    using time_series::traits::plus;

    // anything added to a dense_storage_tag time series is dense_storage_tag
    BOOST_MPL_ASSERT((is_same<plus<dense_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<dense_storage_tag, sparse_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<dense_storage_tag, delta_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<dense_storage_tag, constant_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<dense_storage_tag, piecewise_constant_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<dense_storage_tag, heaviside_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<dense_storage_tag, inverse_heaviside_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<dense_storage_tag, characteristic_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<sparse_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<delta_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<piecewise_constant_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<heaviside_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<inverse_heaviside_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<characteristic_storage_tag, dense_storage_tag>::type, dense_storage_tag>));


    // piecewise constant_storage_tag series, heaviside_storage_tag series, inverse heaviside_storage_tag series,
    // characteristic_storage_tag series: Adding anything but a dense_storage_tag series to these gives
    // a piecewise constant_storage_tag series.
    BOOST_MPL_ASSERT((is_same<plus<piecewise_constant_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<piecewise_constant_storage_tag, sparse_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<piecewise_constant_storage_tag, delta_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<piecewise_constant_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<piecewise_constant_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<piecewise_constant_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<piecewise_constant_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<sparse_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<delta_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<heaviside_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<inverse_heaviside_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<characteristic_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));

    BOOST_MPL_ASSERT((is_same<plus<heaviside_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<heaviside_storage_tag, sparse_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<heaviside_storage_tag, delta_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<heaviside_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<heaviside_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<heaviside_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<sparse_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<delta_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<inverse_heaviside_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<characteristic_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));

    BOOST_MPL_ASSERT((is_same<plus<inverse_heaviside_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<inverse_heaviside_storage_tag, sparse_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<inverse_heaviside_storage_tag, delta_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<inverse_heaviside_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<inverse_heaviside_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<sparse_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<delta_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<characteristic_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));

    BOOST_MPL_ASSERT((is_same<plus<characteristic_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<characteristic_storage_tag, sparse_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<characteristic_storage_tag, delta_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<characteristic_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<sparse_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<delta_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));

    // sparse_storage_tag series and delta_storage_tag series
    // Adding any combination of these gives a sparse_storage_tag series
    BOOST_MPL_ASSERT((is_same<plus<sparse_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<delta_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<sparse_storage_tag, delta_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<delta_storage_tag, delta_storage_tag>::type, sparse_storage_tag>));

    // constant_storage_tag series
    // Adding two of them gives a constant_storage_tag series. Adding anything but a dense_storage_tag
    // series gives a piecewise constant_storage_tag series.
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, constant_storage_tag>::type, constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, sparse_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, delta_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<constant_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<sparse_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<delta_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<piecewise_constant_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<heaviside_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<inverse_heaviside_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<plus<characteristic_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
}

///////////////////////////////////////////////////////////////////////////////
// test_minus_promotion
//
void test_minus_promotion()
{
    using namespace time_series;
    using time_series::traits::minus;

    // anything subtracted from a dense_storage_tag time series is dense_storage_tag
    BOOST_MPL_ASSERT((is_same<minus<dense_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<dense_storage_tag, sparse_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<dense_storage_tag, delta_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<dense_storage_tag, constant_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<dense_storage_tag, piecewise_constant_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<dense_storage_tag, heaviside_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<dense_storage_tag, inverse_heaviside_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<dense_storage_tag, characteristic_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<sparse_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<delta_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<piecewise_constant_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<heaviside_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<inverse_heaviside_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<characteristic_storage_tag, dense_storage_tag>::type, dense_storage_tag>));


    // piecewise constant_storage_tag series, heaviside_storage_tag series, inverse heaviside_storage_tag series,
    // characteristic_storage_tag series: Subtracting anything but a dense_storage_tag series form these gives
    // a piecewise constant_storage_tag series.
    BOOST_MPL_ASSERT((is_same<minus<piecewise_constant_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<piecewise_constant_storage_tag, sparse_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<piecewise_constant_storage_tag, delta_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<piecewise_constant_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<piecewise_constant_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<piecewise_constant_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<piecewise_constant_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<sparse_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<delta_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<heaviside_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<inverse_heaviside_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<characteristic_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));

    BOOST_MPL_ASSERT((is_same<minus<heaviside_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<heaviside_storage_tag, sparse_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<heaviside_storage_tag, delta_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<heaviside_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<heaviside_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<heaviside_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<sparse_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<delta_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<inverse_heaviside_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<characteristic_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));

    BOOST_MPL_ASSERT((is_same<minus<inverse_heaviside_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<inverse_heaviside_storage_tag, sparse_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<inverse_heaviside_storage_tag, delta_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<inverse_heaviside_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<inverse_heaviside_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<sparse_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<delta_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<characteristic_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));

    BOOST_MPL_ASSERT((is_same<minus<characteristic_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<characteristic_storage_tag, sparse_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<characteristic_storage_tag, delta_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<characteristic_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<sparse_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<delta_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));

    // sparse_storage_tag series and delta_storage_tag series
    // Subtracting any combination of these gives a sparse_storage_tag series
    BOOST_MPL_ASSERT((is_same<minus<sparse_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<delta_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<sparse_storage_tag, delta_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<delta_storage_tag, delta_storage_tag>::type, sparse_storage_tag>));

    // constant_storage_tag series
    // Subtracting two of them gives a constant_storage_tag series. Subtracting anything but a dense_storage_tag
    // series gives a piecewise constant_storage_tag series.
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, constant_storage_tag>::type, constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, sparse_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, delta_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<constant_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<sparse_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<delta_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<piecewise_constant_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<heaviside_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<inverse_heaviside_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<minus<characteristic_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
}

///////////////////////////////////////////////////////////////////////////////
// test_multiplies_promotion
//
void test_multiplies_promotion()
{
    using namespace time_series;
    using time_series::traits::multiplies;

    // constant_storage_tag series
    // Multiplying anything with a constant_storage_tag series turns any unit series into
    // its scaled_storage_tag variant, and leaves all other types unchanged. It just
    // changes the overall scale.
    BOOST_MPL_ASSERT((is_same<multiplies<constant_storage_tag, constant_storage_tag>::type, constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<constant_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<constant_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<constant_storage_tag, delta_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<constant_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<constant_storage_tag, heaviside_storage_tag>::type, scaled_storage_tag<heaviside_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<constant_storage_tag, inverse_heaviside_storage_tag>::type, scaled_storage_tag<inverse_heaviside_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<constant_storage_tag, characteristic_storage_tag>::type, scaled_storage_tag<characteristic_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<dense_storage_tag, constant_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<sparse_storage_tag, constant_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<delta_storage_tag, constant_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<piecewise_constant_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<heaviside_storage_tag, constant_storage_tag>::type, scaled_storage_tag<heaviside_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<inverse_heaviside_storage_tag, constant_storage_tag>::type, scaled_storage_tag<inverse_heaviside_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<characteristic_storage_tag, constant_storage_tag>::type, scaled_storage_tag<characteristic_storage_tag> >));

    // delta_storage_tag series and its scaled_storage_tag variant
    // Multiplying anything with a delta_storage_tag series gives a scaled_storage_tag delta_storage_tag series,
    // picking out the value at the location of the delta_storage_tag peak
    BOOST_MPL_ASSERT((is_same<multiplies<delta_storage_tag, delta_storage_tag>::type, delta_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<delta_storage_tag, dense_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<delta_storage_tag, sparse_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<delta_storage_tag, piecewise_constant_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<delta_storage_tag, heaviside_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<delta_storage_tag, inverse_heaviside_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<delta_storage_tag, characteristic_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<dense_storage_tag, delta_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<sparse_storage_tag, delta_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<piecewise_constant_storage_tag, delta_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<heaviside_storage_tag, delta_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<inverse_heaviside_storage_tag, delta_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<multiplies<characteristic_storage_tag, delta_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));

    // sparse_storage_tag series
    // Multiplying anything but a delta_storage_tag series with a sparse_storage_tag series gives a
    // sparse_storage_tag series.
    BOOST_MPL_ASSERT((is_same<multiplies<sparse_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<sparse_storage_tag, dense_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<sparse_storage_tag, piecewise_constant_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<sparse_storage_tag, heaviside_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<sparse_storage_tag, inverse_heaviside_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<sparse_storage_tag, characteristic_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<dense_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<piecewise_constant_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<heaviside_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<inverse_heaviside_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<characteristic_storage_tag, sparse_storage_tag>::type, sparse_storage_tag>));

    // heaviside_storage_tag series, inverse heaviside_storage_tag series, characteristic_storage_tag series and
    // their scaled_storage_tag variants
    //    These act like filters on the range of allowed indices. Thus the return
    // type of multiplying them with any other series is usually of the type of
    // that other series.
    //    The only special cases are products of these series with themselves.
    // Multiplying two series of the same type gives a series of that type
    // (e.g. heaviside_storage_tag series multiplied with heaviside_storage_tag series gives a
    // heaviside_storage_tag series). Multiplying two different types always gives a
    // characteristic_storage_tag series (e.g. multiplying heaviside_storage_tag series with inverse
    // heaviside_storage_tag series). The range of allowed indices is the intersection of
    // the runs. If one of the series is scaled_storage_tag (and not a unit series), the
    // result will also be scaled_storage_tag.
    BOOST_MPL_ASSERT((is_same<multiplies<heaviside_storage_tag, heaviside_storage_tag>::type, heaviside_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<heaviside_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<heaviside_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<heaviside_storage_tag, inverse_heaviside_storage_tag>::type, characteristic_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<heaviside_storage_tag, characteristic_storage_tag>::type, characteristic_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<dense_storage_tag, heaviside_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<piecewise_constant_storage_tag, heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<inverse_heaviside_storage_tag, heaviside_storage_tag>::type, characteristic_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<characteristic_storage_tag, heaviside_storage_tag>::type, characteristic_storage_tag>));

    BOOST_MPL_ASSERT((is_same<multiplies<inverse_heaviside_storage_tag, inverse_heaviside_storage_tag>::type, inverse_heaviside_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<inverse_heaviside_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<inverse_heaviside_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<inverse_heaviside_storage_tag, characteristic_storage_tag>::type, characteristic_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<dense_storage_tag, inverse_heaviside_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<piecewise_constant_storage_tag, inverse_heaviside_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<characteristic_storage_tag, inverse_heaviside_storage_tag>::type, characteristic_storage_tag>));

    BOOST_MPL_ASSERT((is_same<multiplies<characteristic_storage_tag, characteristic_storage_tag>::type, characteristic_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<characteristic_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<characteristic_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<dense_storage_tag, characteristic_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<piecewise_constant_storage_tag, characteristic_storage_tag>::type, piecewise_constant_storage_tag>));

    // piecewise constant_storage_tag series and dense_storage_tag series
    // Multiplying two piecewise constant_storage_tag series gives a piecewise constant_storage_tag
    // series. Multiplying a dense_storage_tag series with another dense_storage_tag series or a
    // piecewise constant_storage_tag series gives a dense_storage_tag series.
    BOOST_MPL_ASSERT((is_same<multiplies<piecewise_constant_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<dense_storage_tag, piecewise_constant_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<piecewise_constant_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<multiplies<dense_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
}

///////////////////////////////////////////////////////////////////////////////
// test_divides_promotion
//
void test_divides_promotion()
{
    using namespace time_series;
    using time_series::traits::divides;

    // Note that dividing by anything but a dense_storage_tag series, constant_storage_tag series or
    // piecewise constant_storage_tag series is generally not well defined, and should
    // probably not be implemented at all for these types. In the following
    // I will just write multiplication. Division will follow the same rules
    // wherever appropriate.

    // constant_storage_tag series
    // Multiplying anything with a constant_storage_tag series turns any unit series into
    // its scaled_storage_tag variant, and leaves all other types unchanged. It just
    // changes the overall scale.
    BOOST_MPL_ASSERT((is_same<divides<constant_storage_tag, constant_storage_tag>::type, constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<constant_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<constant_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<dense_storage_tag, constant_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<sparse_storage_tag, constant_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<delta_storage_tag, constant_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<divides<piecewise_constant_storage_tag, constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<heaviside_storage_tag, constant_storage_tag>::type, scaled_storage_tag<heaviside_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<divides<inverse_heaviside_storage_tag, constant_storage_tag>::type, scaled_storage_tag<inverse_heaviside_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<divides<characteristic_storage_tag, constant_storage_tag>::type, scaled_storage_tag<characteristic_storage_tag> >));

    // delta_storage_tag series and its scaled_storage_tag variant
    // Multiplying anything with a delta_storage_tag series gives a scaled_storage_tag delta_storage_tag series,
    // picking out the value at the location of the delta_storage_tag peak
    // ERICNE: we don't currently have a scaled_storage_tag delta_storage_tag
    BOOST_MPL_ASSERT((is_same<divides<delta_storage_tag, dense_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));
    BOOST_MPL_ASSERT((is_same<divides<delta_storage_tag, piecewise_constant_storage_tag>::type, scaled_storage_tag<delta_storage_tag> >));

    // sparse_storage_tag series
    // Multiplying anything but a delta_storage_tag series with a sparse_storage_tag series gives a
    // sparse_storage_tag series.
    BOOST_MPL_ASSERT((is_same<divides<sparse_storage_tag, dense_storage_tag>::type, sparse_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<sparse_storage_tag, piecewise_constant_storage_tag>::type, sparse_storage_tag>));

    // heaviside_storage_tag series, inverse heaviside_storage_tag series, characteristic_storage_tag series and
    // their scaled_storage_tag variants
    //    These act like filters on the range of allowed indices. Thus the return
    // type of multiplying them with any other series is usually of the type of
    // that other series.
    //    The only special cases are products of these series with themselves.
    // Multiplying two series of the same type gives a series of that type
    // (e.g. heaviside_storage_tag series multiplied with heaviside_storage_tag series gives a
    // heaviside_storage_tag series). Multiplying two different types always gives a
    // characteristic_storage_tag series (e.g. multiplying heaviside_storage_tag series with inverse
    // heaviside_storage_tag series). The range of allowed indices is the intersection of
    // the runs. If one of the series is scaled_storage_tag (and not a unit series), the
    // result will also be scaled_storage_tag.
    BOOST_MPL_ASSERT((is_same<divides<heaviside_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<heaviside_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));

    BOOST_MPL_ASSERT((is_same<divides<inverse_heaviside_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<inverse_heaviside_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));

    BOOST_MPL_ASSERT((is_same<divides<characteristic_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<characteristic_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));

    // piecewise constant_storage_tag series and dense_storage_tag series
    // Multiplying two piecewise constant_storage_tag series gives a piecewise constant_storage_tag
    // series. Multiplying a dense_storage_tag series with another dense_storage_tag series or a
    // piecewise constant_storage_tag series gives a dense_storage_tag series.
    BOOST_MPL_ASSERT((is_same<divides<piecewise_constant_storage_tag, piecewise_constant_storage_tag>::type, piecewise_constant_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<dense_storage_tag, piecewise_constant_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<piecewise_constant_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
    BOOST_MPL_ASSERT((is_same<divides<dense_storage_tag, dense_storage_tag>::type, dense_storage_tag>));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("series promotions test");

    test->add(BOOST_TEST_CASE(&test_plus_promotion));
    test->add(BOOST_TEST_CASE(&test_minus_promotion));
    test->add(BOOST_TEST_CASE(&test_multiplies_promotion));
    test->add(BOOST_TEST_CASE(&test_divides_promotion));

    return test;
}
