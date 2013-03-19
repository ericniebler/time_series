//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/time_series/constant_series.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/partial_sum.hpp>

using namespace boost;
using namespace unit_test;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

///////////////////////////////////////////////////////////////////////////////
// test_partial_sum
//
void test_partial_sum()
{
    using namespace time_series;

    // expected result for test 1:
    piecewise_constant_series<int> result;
    make_ordered_inserter(result)
        (2, 0)(4)(6)(8)(10)(12)(14)(16)(18)(20)
        (24)(28)(32)(36)(40)(44)(48)(52)(56)(60)
        (60, 20, 90)
        (62)(64)(66)(68)(70)(72)(74)(76)
        (78, 98, inf)
    .commit();

    piecewise_constant_series<int> p;
    make_ordered_inserter(p)
        (2, 0, 10)
        (4, 10, 20)
        (2, 90, 99)
    .commit();
    piecewise_constant_series<int> s3 = partial_sum(p);
    BOOST_CHECK_EQUAL(s3, result);

    // expected result for test 2:
    piecewise_constant_series<int, monthly> result2;
    make_ordered_inserter(result2)
        (1, 1)(3)(6)(10)(15)(21)(28)(36)(45, 9, inf)
    .commit();

    typedef rrs::unit_run<std::ptrdiff_t> dense_run;
    dense_series<int, monthly> d(stop = 10);
    for(int i=0; i<10; ++i)
        range_run_storage::set_at(d, dense_run(i), i);

    piecewise_constant_series<int, monthly> s4 = partial_sum(d);
    BOOST_CHECK_EQUAL(s4, result2);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("partial_sum test");

    test->add(BOOST_TEST_CASE(&test_partial_sum));

    return test;
}
