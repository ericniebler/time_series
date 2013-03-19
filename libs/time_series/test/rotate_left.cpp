//  Copyright 2006 Michael Gauckler, Eric Niebler.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/numeric/rotate_left.hpp>
#include <boost/time_series/ordered_inserter.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

void unit_test_func()
{
    typedef boost::counting_iterator<int> int_;

    sparse_series<int> d;
    std::copy(int_(1), int_(6), make_ordered_inserter(d, 0)).commit();

    sparse_series<int> result1;
    make_ordered_inserter(result1)
        (2, 0)(3, 1)(4, 2)(5, 3)
    .commit();

    BOOST_CHECK_EQUAL(result1, rotate_left(d));

    sparse_series<int> result2;
    make_ordered_inserter(result2)
        (2, 0)(3, 1)(4, 2)(5, 3)(42, 4)
    .commit();

    BOOST_CHECK_EQUAL(result2, rotate_left(d, 42));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("rotate_left test");

    test->add(BOOST_TEST_CASE(&unit_test_func));

    return test;
}
