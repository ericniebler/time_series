///////////////////////////////////////////////////////////////////////////////
/// \file heaviside_series.hpp
/// A time series that uses heaviside storage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_HEAVISIDE_SERIES_HPP_EAN_05_29_2006
#define BOOST_TIME_SERIES_HEAVISIDE_SERIES_HPP_EAN_05_29_2006

#include <boost/time_series/time_series_facade.hpp>
#include <boost/time_series/storage/heaviside_array.hpp>

namespace boost { namespace time_series
{

    /// \brief A \c Mutable_TimeSeries that has the unit value in the range <tt>[start,inf)</tt> and
    ///     zero elsewhere.
    ///
    /// A \c Mutable_TimeSeries that has the unit value in the range <tt>[start,inf)</tt> and
    ///     zero elsewhere.
    ///
    /// The named parameters for the constructor are, in order:
    ///   -# \c start, with a default of \c Offset(0)
    ///   -# \c value, with a default of \c Value(1)
    ///   -# \c discretization, with a default of \c Discretization(1)
    ///   -# \c zero, with a default of \c Value(0)
    template<typename Value, typename Discretization, typename Offset>
    struct heaviside_unit_series
      : time_series_facade<
            heaviside_unit_series<Value, Discretization, Offset>
          , storage::heaviside_array<Value, Offset>
          , Discretization
        >
    {
        typedef time_series_facade<
            heaviside_unit_series<Value, Discretization, Offset>
          , storage::heaviside_array<Value, Offset>
          , Discretization
        > base_type;

        using base_type::operator=;

        BOOST_TIME_SERIES_DEFINE_CTORS(heaviside_unit_series)
    };

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct storage_category<heaviside_unit_series<Value, Discretization, Offset> >
          : storage_category<storage::heaviside_array<Value, Offset> >
        {};

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct discretization_type<heaviside_unit_series<Value, Discretization, Offset> >
        {
            typedef Discretization type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct offset_type<heaviside_unit_series<Value, Discretization, Offset> >
        {
            typedef Offset type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct generate_series<heaviside_storage_tag, Value, Discretization, Offset>
        {
            typedef heaviside_unit_series<Value, Discretization, Offset> type;
        };
    }

}}

namespace boost { namespace sequence { namespace impl
{
    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::heaviside_unit_series<Value, Discretization, Offset> >
    {
        typedef time_series_tag type;
    };
}}}

namespace boost { namespace time_series
{

    /// \brief A \c Mutable_TimeSeries that has a discrete value in the range <tt>[start,inf)</tt> and
    ///     zero elsewhere.
    ///
    /// A \c Mutable_TimeSeries that has a discrete value in the range <tt>[start,inf)</tt> and
    ///     zero elsewhere.
    ///
    /// The named parameters for the constructor are, in order:
    ///   -# \c start, with a default of \c Offset(0)
    ///   -# \c value, with a default of \c Value(1)
    ///   -# \c discretization, with a default of \c Discretization(1)
    ///   -# \c zero, with a default of \c Value(0)
    template<typename Value, typename Discretization, typename Offset>
    struct heaviside_series
      : time_series_facade<
            heaviside_series<Value, Discretization, Offset>
          , storage::heaviside_array<Value, Offset, storage::constant_elements<Value> >
         , Discretization
        >
    {
        typedef time_series_facade<
            heaviside_series<Value, Discretization, Offset>
          , storage::heaviside_array<Value, Offset, storage::constant_elements<Value> >
          , Discretization
        > base_type;

        using base_type::operator=;

        BOOST_TIME_SERIES_DEFINE_CTORS(heaviside_series)
    };

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct storage_category<heaviside_series<Value, Discretization, Offset> >
          : storage_category<storage::heaviside_array<Value, Offset, storage::constant_elements<Value> > >
        {};

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct discretization_type<heaviside_series<Value, Discretization, Offset> >
        {
            typedef Discretization type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct offset_type<heaviside_series<Value, Discretization, Offset> >
        {
            typedef Offset type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct generate_series<scaled_storage_tag<heaviside_storage_tag>, Value, Discretization, Offset>
        {
            typedef heaviside_series<Value, Discretization, Offset> type;
        };
    }

}}

namespace boost { namespace sequence { namespace impl
{
    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::heaviside_series<Value, Discretization, Offset> >
    {
        typedef time_series_tag type;
    };
}}}

namespace boost { namespace time_series
{
    /// \brief Fetches the offset at which the heaviside series jumps from zero to non-zero.
    ///
    /// Fetches the offset at which the heaviside series jumps from zero to non-zero.
    ///
    /// \throw nothrow
    template<typename Value, typename Discretization, typename Offset>
    Offset jump_offset(heaviside_series<Value, Discretization, Offset> const &series)
    {
        return range_run_storage::offset(range_run_storage::post_run(series));
    }

    /// \overload
    ///
    template<typename Value, typename Discretization, typename Offset>
    Offset jump_offset(heaviside_unit_series<Value, Discretization, Offset> const &series)
    {
        return range_run_storage::offset(range_run_storage::post_run(series));
    }
}}

namespace boost { namespace constructors { namespace impl
{
    /// INTERNAL ONLY
    struct heaviside_series_tag;

    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::heaviside_series<Value, Discretization, Offset> >
    {
        typedef heaviside_series_tag type;
    };

    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::heaviside_unit_series<Value, Discretization, Offset> >
    {
        typedef heaviside_series_tag type;
    };

    /// INTERNAL ONLY
    template<typename T>
    struct construct<T, heaviside_series_tag>
      : arg_pack_construct
    {
        typedef parameter::parameters<
            parameter::optional<time_series::tag::start>
          , parameter::optional<time_series::tag::value>
          , parameter::optional<time_series::tag::discretization>
          , parameter::optional<time_series::tag::zero>
        > args_type;
    };

}}}

#endif
