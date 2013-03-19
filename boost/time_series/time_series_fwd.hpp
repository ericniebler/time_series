///////////////////////////////////////////////////////////////////////////////
/// \file time_series_fwd.hpp
/// Includes all of the Time_Series Library
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_TIME_SERIES_FWD_HPP_EAN_04_26_2006
#define BOOST_TIME_SERIES_TIME_SERIES_FWD_HPP_EAN_04_26_2006

#ifdef BOOST_PARAMETER_MAX_ARITY
# if BOOST_PARAMETER_MAX_ARITY < 7
#  error You must define BOOST_PARAMETER_MAX_ARITY to be 7 or greater.
# endif
#else
# define BOOST_PARAMETER_MAX_ARITY 7
#endif

#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/integer_traits.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/range_run_storage/utility/infinity.hpp>

// HACKHACK make keyword objects const
/// INTERNAL ONLY
#define BOOST_TIME_SERIES_KEYWORD(tag, name)\
    namespace non_const_keyword_detail_\
    {\
        BOOST_PARAMETER_KEYWORD(tag, name)\
    }\
    namespace tag\
    {\
        using non_const_keyword_detail_::tag::name;\
    }\
    namespace\
    {\
        ::boost::parameter::keyword< tag::name > const &name = non_const_keyword_detail_::name;\
    }\
    /**/

namespace boost { namespace time_series
{
    // initialization tags
    BOOST_TIME_SERIES_KEYWORD(tag, discretization)
    BOOST_TIME_SERIES_KEYWORD(tag, zero)
    BOOST_TIME_SERIES_KEYWORD(tag, value)
    BOOST_TIME_SERIES_KEYWORD(tag, start)
    BOOST_TIME_SERIES_KEYWORD(tag, stop)

    // storage tags
    struct piecewise_constant_storage_tag {};
    struct dense_storage_tag {};
    struct sparse_storage_tag {};
    struct characteristic_storage_tag {};
    struct delta_storage_tag {};
    struct constant_storage_tag {};
    struct heaviside_storage_tag {};
    struct inverse_heaviside_storage_tag {};

    template<typename Tag>
    struct scaled_storage_tag
    {
        typedef scaled_storage_tag<Tag> type;
    };

    template<typename Tag>
    struct scaled_storage_tag<scaled_storage_tag<Tag> >
    {
        typedef scaled_storage_tag<Tag> type;
    };

    // discretizations
    typedef mpl::int_<7>    days_per_week;
    typedef mpl::int_<30>   days_per_month;
    typedef mpl::int_<90>   days_per_quarter;
    typedef mpl::int_<180>  days_per_semi_year;
    typedef mpl::int_<360>  days_per_year;

    struct daily        : mpl::int_<1> {};
    struct weekly       : days_per_week {};
    struct monthly      : days_per_month {};
    struct quarterly    : days_per_quarter {};
    struct semi_yearly  : days_per_semi_year {};
    struct yearly       : days_per_year {};

    struct unit         : mpl::int_<1> {};

    namespace traits
    {
        template<typename S>
        struct is_time_series;

        template<typename T>
        struct storage_category;

        template<typename Tag, typename Value, typename Discretization, typename Offset>
        struct generate_series;

        template<typename S>
        struct discretization_type;
    }

    struct any {}; // for use as the discretization template parameter
    template<typename T> inline bool operator ==(any, T const &) { return true; }
    template<typename T> inline bool operator !=(any, T const &) { return false; }
    template<typename T> inline bool operator ==(T const &, any) { return true; }
    template<typename T> inline bool operator !=(T const &, any) { return false; }

    struct time_series_root;

    template<typename Derived>
    struct time_series_base;

    /// Represents a series of observations at regular intervals.
    template<
        typename Derived
      , typename Storage
      , typename Discretization
    >
    struct time_series_facade;

    // End-user
    template<typename Value, typename Discretization = int>
    struct dense_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct characteristic_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct characteristic_unit_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct constant_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct delta_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct delta_unit_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct heaviside_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct heaviside_unit_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct inverse_heaviside_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct inverse_heaviside_unit_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct piecewise_constant_series;

    template<typename Value, typename Discretization = int, typename Offset = std::ptrdiff_t>
    struct sparse_series;

    //
    // series adaptors:
    //
    template<typename Series>
    struct clipped_series;

    template<typename Series>
    struct shifted_series;

    template<typename Series, typename Factor>
    struct scaled_series;

    //
    // ordered inserter:
    //
    template<typename Series>
    struct ordered_inserter;

    using range_run_storage::plus_infinity;
    using range_run_storage::minus_infinity;
    using range_run_storage::inf;

}} // namespace boost::time_series

#endif
