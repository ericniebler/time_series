//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/numeric/functional/vector.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/numeric/numeric.hpp>
#include <boost/time_series/ordered_inserter.hpp>

using namespace boost::unit_test;
using namespace boost::time_series;

namespace seq = boost::sequence;
namespace rrs = boost::range_run_storage;

// for debug printfs
namespace std
{
    template<typename Char, typename Traits, typename Value, typename Allocator>
    inline basic_ostream<Char, Traits> &
    operator <<(basic_ostream<Char, Traits> &sout, vector<Value, Allocator> const &v)
    {
        sout << '[';
        for(size_t i = 0;;)
        {
            sout << v[i];
            if(v.size() == ++i)
                break;
            sout << ',';
        }
        return sout << ']';
    }
}

///////////////////////////////////////////////////////////////////////////////
// test_vector_plus
//
void test_vector_plus()
{
    typedef std::vector<int> value_type;
    value_type const nil(4,0);

    // check addition of a sparse time series and a dense time series yields
    // a dense time series
    dense_series<value_type> ss1(zero = nil);
    sparse_series<value_type> ss2(zero = nil);

    make_ordered_inserter(ss1)(value_type(4,3), 1)(value_type(4,4), 2).commit();
    make_ordered_inserter(ss2)(value_type(4,4), 2)(value_type(4,5), 3).commit();

    dense_series<value_type> result1(zero = nil);
    make_ordered_inserter(result1)
        (value_type(4,3), 1)
        (value_type(4,8), 2)
        (value_type(4,5), 3)
    .commit();

    dense_series<value_type> ss3 = ss1 + ss2;
    BOOST_CHECK_EQUAL(result1, ss3);

    sparse_series<value_type> result2(zero = nil);
    make_ordered_inserter(result2)
        (value_type(4,3), 1)
        (value_type(4,8), 2)
        (value_type(4,5), 3)
    .commit();

    ss2 += ss1;
    BOOST_CHECK_EQUAL(result2, ss2);
}

///////////////////////////////////////////////////////////////////////////////
// test_vector_multiplies
//
void test_vector_multiplies()
{
    typedef std::vector<int> value_type;
    value_type const nil(4,0);

    // check multiplication of a sparse time series and a dense time series yields
    // a sparse time series
    dense_series<value_type> ss1(zero = nil);
    sparse_series<value_type> ss2(zero = nil);

    make_ordered_inserter(ss1)(value_type(4,3), 1)(value_type(4,4), 2).commit();
    make_ordered_inserter(ss2)(value_type(4,4), 2)(value_type(4,5), 3).commit();

    sparse_series<value_type> ss3 = ss1 * ss2;
    BOOST_CHECK_EQUAL(1, std::distance(seq::begin(ss3), seq::end(ss3)));
    BOOST_CHECK_EQUAL(2, rrs::offset(rrs::runs(ss3)(*seq::begin(ss3))));
    BOOST_CHECK_EQUAL(value_type(4,16), seq::elements(ss3)(*seq::begin(ss3)));

    ss2 *= ss1;
    BOOST_CHECK_EQUAL(1, std::distance(seq::begin(ss2), seq::end(ss2)));
    BOOST_CHECK_EQUAL(2, rrs::offset(rrs::runs(ss2)(*seq::begin(ss2))));
    BOOST_CHECK_EQUAL(value_type(4,16), seq::elements(ss2)(*seq::begin(ss2)));
}


///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("series of std::vector test");

    test->add(BOOST_TEST_CASE(&test_vector_plus));
    test->add(BOOST_TEST_CASE(&test_vector_multiplies));

    return test;
}
