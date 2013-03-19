//  (C) Copyright Eric Niebler 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
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
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace boost;
using namespace unit_test;

template<typename T> 
T const &const_(T const &t)
{
    return t;
}

///////////////////////////////////////////////////////////////////////////////
// test_serialization
//
void test_serialization()
{
    namespace rrs = range_run_storage;
    using namespace time_series;

    {
        characteristic_series<int> expected(-10, 10, 2, 30, 3);

        {
            std::ofstream ofs("serialization_test.txt");
            boost::archive::text_oarchive oa(ofs);
            oa << ::const_(expected);
        }

        {
            characteristic_series<int> result;
            std::ifstream ifs("serialization_test.txt", std::ios::binary);
            boost::archive::text_iarchive ia(ifs);
            ia >> result;

            BOOST_CHECK_EQUAL(expected, result);
            BOOST_CHECK_EQUAL(result.discretization(), 30);
            BOOST_CHECK_EQUAL(3, rrs::zero(result));
        }
    }

    {
        constant_series<int> expected(2, 30);

        {
            std::ofstream ofs("serialization_test.txt");
            boost::archive::text_oarchive oa(ofs);
            oa << ::const_(expected);
        }

        {
            constant_series<int> result;
            std::ifstream ifs("serialization_test.txt", std::ios::binary);
            boost::archive::text_iarchive ia(ifs);
            ia >> result;

            BOOST_CHECK_EQUAL(expected, result);
            BOOST_CHECK_EQUAL(result.discretization(), 30);
        }
    }

    {
        delta_series<int> expected(0, 2, 30, 3);

        {
            std::ofstream ofs("serialization_test.txt");
            boost::archive::text_oarchive oa(ofs);
            oa << ::const_(expected);
        }

        {
            delta_series<int> result;
            std::ifstream ifs("serialization_test.txt", std::ios::binary);
            boost::archive::text_iarchive ia(ifs);
            ia >> result;

            BOOST_CHECK_EQUAL(expected, result);
            BOOST_CHECK_EQUAL(result.discretization(), 30);
            BOOST_CHECK_EQUAL(3, rrs::zero(result));
        }
    }

    {
        dense_series<int> expected(discretization = 30, zero = 3);
        make_ordered_inserter(expected)(2, -inf, -10)(4, -5, 5)(5, 10, inf).commit();

        {
            std::ofstream ofs("serialization_test.txt");
            boost::archive::text_oarchive oa(ofs);
            oa << ::const_(expected);
        }

        {
            dense_series<int> result;
            std::ifstream ifs("serialization_test.txt", std::ios::binary);
            boost::archive::text_iarchive ia(ifs);
            ia >> result;

            BOOST_CHECK_EQUAL(expected, result);
            BOOST_CHECK_EQUAL(result.discretization(), 30);
            BOOST_CHECK_EQUAL(3, rrs::zero(result));
        }
    }

    {
        heaviside_series<int> expected(10, 2, 30, 3);

        {
            std::ofstream ofs("serialization_test.txt");
            boost::archive::text_oarchive oa(ofs);
            oa << ::const_(expected);
        }

        {
            heaviside_series<int> result;
            std::ifstream ifs("serialization_test.txt", std::ios::binary);
            boost::archive::text_iarchive ia(ifs);
            ia >> result;

            BOOST_CHECK_EQUAL(expected, result);
            BOOST_CHECK_EQUAL(result.discretization(), 30);
            BOOST_CHECK_EQUAL(3, rrs::zero(result));
        }
    }

    {
        inverse_heaviside_series<int> expected(10, 2, 30, 3);

        {
            std::ofstream ofs("serialization_test.txt");
            boost::archive::text_oarchive oa(ofs);
            oa << ::const_(expected);
        }

        {
            inverse_heaviside_series<int> result;
            std::ifstream ifs("serialization_test.txt", std::ios::binary);
            boost::archive::text_iarchive ia(ifs);
            ia >> result;

            BOOST_CHECK_EQUAL(expected, result);
            BOOST_CHECK_EQUAL(result.discretization(), 30);
            BOOST_CHECK_EQUAL(3, rrs::zero(result));
        }
    }

    {
        piecewise_constant_series<int> expected(discretization = 30, zero = 3);
        make_ordered_inserter(expected)(2, -inf, -10)(4, -5, 5)(5, 10, inf).commit();

        {
            std::ofstream ofs("serialization_test.txt");
            boost::archive::text_oarchive oa(ofs);
            oa << ::const_(expected);
        }

        {
            piecewise_constant_series<int> result;
            std::ifstream ifs("serialization_test.txt", std::ios::binary);
            boost::archive::text_iarchive ia(ifs);
            ia >> result;

            BOOST_CHECK_EQUAL(expected, result);
            BOOST_CHECK_EQUAL(result.discretization(), 30);
            BOOST_CHECK_EQUAL(3, rrs::zero(result));
        }
    }

    {
        sparse_series<int> expected(discretization = 30, zero = 3);
        make_ordered_inserter(expected)(2, -inf, -10)(4, -5, 5)(5, 10, inf).commit();

        {
            std::ofstream ofs("serialization_test.txt");
            boost::archive::text_oarchive oa(ofs);
            oa << ::const_(expected);
        }

        {
            sparse_series<int> result;
            std::ifstream ifs("serialization_test.txt", std::ios::binary);
            boost::archive::text_iarchive ia(ifs);
            ia >> result;

            BOOST_CHECK_EQUAL(expected, result);
            BOOST_CHECK_EQUAL(result.discretization(), 30);
            BOOST_CHECK_EQUAL(3, rrs::zero(result));
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("series serialization test");

    test->add(BOOST_TEST_CASE(&test_serialization));

    return test;
}
