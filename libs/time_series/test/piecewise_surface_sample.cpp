//  (C) Copyright Daniel Egloff 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/numeric.hpp>
#include <boost/time_series/numeric/piecewise_surface_sample.hpp>

using namespace boost;
using namespace unit_test;

///////////////////////////////////////////////////////////////////////////////
// test_piecewise_surface_sample
//
void test_piecewise_surface_sample()
{
    namespace bt = boost::time_series;
    typedef bt::piecewise_constant_series<double>      series_type;
    typedef bt::piecewise_constant_series<series_type> surface_type;
    
    series_type base;
    bt::make_ordered_inserter(base)
        (1, -bt::inf, 10)
        (2, 10, 20)   
        (3, 20, 30)   
        (4, 30, 40)   
        (5, 40, 50)   
    .commit();    
    
    surface_type surface;
    bt::make_ordered_inserter(surface)
        (base * -1, -bt::inf, 0)
        (base * 1, 0, 10)
        (base * 2, 10, 20)
        (base * 3, 20, 30)
        (base * 4, 30, 40)
    .commit();
    
    std::ptrdiff_t x[] = { -10, 15, 35 };
    std::ptrdiff_t y[] = { 0, 25, 45 };
    
    surface_type result = bt::piecewise_surface_sample(surface, x, y);
    
    series_type expected_base;
    bt::make_ordered_inserter(expected_base)
        (1, -bt::inf, 25)
        (3, 25, 45)   
        (5, 45, bt::inf)   
    .commit();    
    
    surface_type expected_result;
    bt::make_ordered_inserter(expected_result)
        (expected_base * -1, -bt::inf, 15)
        (expected_base * 2, 15, 35)
        (expected_base * 4, 35, bt::inf)
    .commit();
       
    BOOST_CHECK_EQUAL(result, expected_result);  
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("piecewise_surface_sample test");

    test->add(BOOST_TEST_CASE(&test_piecewise_surface_sample));

    return test;
}
