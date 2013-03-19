///////////////////////////////////////////////////////////////////////////////
/// \file delta_series.hpp
/// A time series that uses delta storage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_DELTA_SERIES_HPP_EAN_05_29_2006
#define BOOST_TIME_SERIES_DELTA_SERIES_HPP_EAN_05_29_2006

#include <boost/time_series/time_series_facade.hpp>
#include <boost/time_series/storage/delta_array.hpp>

namespace boost { namespace time_series
{

    /// \brief A \c Mutable_TimeSeries that has the unit value at some unique
    ///     offset, and zero elsewhere.
    ///
    /// A \c Mutable_TimeSeries that has the unit value at some unique
    ///     offset, and zero elsewhere.
    /// 
    /// The named parameters for the constructor are, in order:
    ///   -# \c start, with a default of \c Offset(0)
    ///   -# \c value, with a default of \c Value(1)
    ///   -# \c discretization, with a default of \c Discretization(1)
    ///   -# \c zero, with a default of \c Value(0)
    template<typename Value, typename Discretization, typename Offset>
    struct delta_unit_series
      : time_series_facade<
            delta_unit_series<Value, Discretization, Offset>
          , storage::delta_array<Value, Offset>
          , Discretization
        >
    {
        typedef time_series_facade<
            delta_unit_series<Value, Discretization, Offset>
          , storage::delta_array<Value, Offset>
          , Discretization
        > base_type;

        using base_type::operator=;

        BOOST_TIME_SERIES_DEFINE_CTORS(delta_unit_series)
    };

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct storage_category<delta_unit_series<Value, Discretization, Offset> >
          : storage_category<storage::delta_array<Value, Offset> >
        {};

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct discretization_type<delta_unit_series<Value, Discretization, Offset> >
        {
            typedef Discretization type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct offset_type<delta_unit_series<Value, Discretization, Offset> >
        {
            typedef Offset type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct generate_series<delta_storage_tag, Value, Discretization, Offset>
        {
            typedef delta_unit_series<Value, Discretization, Offset> type;
        };
    }

}}

namespace boost { namespace sequence { namespace impl
{
    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::delta_unit_series<Value, Discretization, Offset> >
    {
        typedef time_series_tag type;
    };
}}}

namespace boost { namespace time_series
{

    /// \brief A \c Mutable_TimeSeries that has a distinct value at some unique
    ///     offset, and zero elsewhere.
    ///
    /// A \c Mutable_TimeSeries that has a distinct value at some unique
    ///     offset, and zero elsewhere.
    /// 
    /// The named parameters for the constructor are, in order:
    ///   -# \c start, with a default of \c Offset(0)
    ///   -# \c value, with a default of \c Value(1)
    ///   -# \c discretization, with a default of \c Discretization(1)
    ///   -# \c zero, with a default of \c Value(0)
    template<typename Value, typename Discretization, typename Offset>
    struct delta_series
      : time_series_facade<
            delta_series<Value, Discretization, Offset>
          , storage::delta_array<Value, Offset, storage::constant_elements<Value> >
          , Discretization
        >
    {
        typedef time_series_facade<
            delta_series<Value, Discretization, Offset>
          , storage::delta_array<Value, Offset, storage::constant_elements<Value> >
          , Discretization
        > base_type;

        using base_type::operator=;

        BOOST_TIME_SERIES_DEFINE_CTORS(delta_series)
    };

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct storage_category<delta_series<Value, Discretization, Offset> >
          : storage_category<storage::delta_array<Value, Offset, storage::constant_elements<Value> > >
        {};

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct discretization_type<delta_series<Value, Discretization, Offset> >
        {
            typedef Discretization type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct offset_type<delta_series<Value, Discretization, Offset> >
        {
            typedef Offset type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct generate_series<scaled_storage_tag<delta_storage_tag>, Value, Discretization, Offset>
        {
            typedef delta_series<Value, Discretization, Offset> type;
        };
    }

}}

namespace boost { namespace sequence { namespace impl
{
    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::delta_series<Value, Discretization, Offset> >
    {
        typedef time_series_tag type;
    };
}}}

namespace boost { namespace constructors { namespace impl
{
    /// INTERNAL ONLY
    struct delta_series_tag;

    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::delta_series<Value, Discretization, Offset> >
    {
        typedef delta_series_tag type;
    };

    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::delta_unit_series<Value, Discretization, Offset> >
    {
        typedef delta_series_tag type;
    };

    /// INTERNAL ONLY
    template<typename T>
    struct construct<T, delta_series_tag>
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
