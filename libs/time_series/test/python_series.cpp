// Copyright Daniel Wallin 2006. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/python/series.hpp>

#include <boost/python/module.hpp>
#include <boost/python/scope.hpp>

using namespace boost::time_series;

BOOST_PYTHON_MODULE(python_series_ext)
{
    python_::python_series<
        dense_series<double>
    >("dense_series");

    python_::python_series<
        sparse_series<double>
    >("sparse_series");

    python_::python_infinity<true>();
    python_::python_infinity<false>();

    boost::python::scope().attr("inf") = inf;
}

