//  Copyright 2006 Michael Gauckler, Eric Niebler.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/characteristic_series.hpp>
#include <boost/time_series/numeric/numeric.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/time_series_typeof.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

template<typename T, typename U>
void static_assert_type_is( U const & )
{
    BOOST_MPL_ASSERT((boost::is_same<T, U>));
}

void unit_test_func()
{
    sparse_series<int> s, s1;
    make_ordered_inserter(s,0)(1)(2)(3)(4).commit();
    make_ordered_inserter(s1,0)(1)(4)(9)(16).commit();

    dense_series<int> d, d1;
    make_ordered_inserter(d,0)(1)(2)(3)(4).commit();
    make_ordered_inserter(d1,0)(2)(4)(6)(8).commit();

    sparse_series< sparse_series<int> > ss;
    make_ordered_inserter(ss,0)(s)(s)(s)(s).commit();

    sparse_series< dense_series<int> > sd;
    make_ordered_inserter(sd,0)(d)(d)(d)(d).commit();

    static_assert_type_is< sparse_series< dense_series<int> > >( ss + sd );
    sparse_series< dense_series<int> > ss_plus_sd( ss + sd );
    BOOST_CHECK(ss_plus_sd == characteristic_series< dense_series<int> >(0, 4, d1));

    static_assert_type_is< sparse_series< sparse_series<int> > >( ss * sd );
    sparse_series< sparse_series<int> > ss_times_sd( ss * sd );
    BOOST_CHECK(ss_times_sd == characteristic_series< sparse_series<int> >(0, 4, s1));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("nested_series test");

    test->add(BOOST_TEST_CASE(&unit_test_func));

    return test;
}
