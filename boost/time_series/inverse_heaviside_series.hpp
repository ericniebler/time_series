///////////////////////////////////////////////////////////////////////////////
/// \file inverse_heaviside_series.hpp
/// A time series that uses inverse_heaviside storage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_INVERSE_HEAVISIDE_SERIES_HPP_EAN_05_29_2006
#define BOOST_TIME_SERIES_INVERSE_HEAVISIDE_SERIES_HPP_EAN_05_29_2006

#include <boost/time_series/time_series_facade.hpp>
#include <boost/time_series/storage/inverse_heaviside_array.hpp>

namespace boost { namespace time_series
{

    /// \brief A \c Mutable_TimeSeries that has the unit value in the range <tt>[-inf,stop)</tt> and
    ///     zero elsewhere.
    ///
    /// A \c Mutable_TimeSeries that has the unit value in the range <tt>[-inf,stop)</tt> and
    ///     zero elsewhere.
    ///
    /// The named parameters for the constructor are, in order:
    ///   -# \c stop, with a default of \c Offset(0)
    ///   -# \c value, with a default of \c Value(1)
    ///   -# \c discretization, with a default of \c Discretization(1)
    ///   -# \c zero, with a default of \c Value(0)
    template<typename Value, typename Discretization, typename Offset>
    struct inverse_heaviside_unit_series
      : time_series_facade<
            inverse_heaviside_unit_series<Value, Discretization, Offset>
          , storage::inverse_heaviside_array<Value, Offset>
          , Discretization
        >
    {
        typedef time_series_facade<
            inverse_heaviside_unit_series<Value, Discretization, Offset>
          , storage::inverse_heaviside_array<Value, Offset>
          , Discretization
        > base_type;

        using base_type::operator=;

        BOOST_TIME_SERIES_DEFINE_CTORS(inverse_heaviside_unit_series)
    };

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct storage_category<inverse_heaviside_unit_series<Value, Discretization, Offset> >
          : storage_category<storage::inverse_heaviside_array<Value, Offset> >
        {};

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct discretization_type<inverse_heaviside_unit_series<Value, Discretization, Offset> >
        {
            typedef Discretization type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct offset_type<inverse_heaviside_unit_series<Value, Discretization, Offset> >
        {
            typedef Offset type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct generate_series<inverse_heaviside_storage_tag, Value, Discretization, Offset>
        {
            typedef inverse_heaviside_unit_series<Value, Discretization, Offset> type;
        };
    }

}}

namespace boost { namespace sequence { namespace impl
{
    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::inverse_heaviside_unit_series<Value, Discretization, Offset> >
    {
        typedef time_series_tag type;
    };
}}}

namespace boost { namespace time_series
{

    /// \brief A \c Mutable_TimeSeries that has a discrete value in the range <tt>[-inf,stop)</tt> and
    ///     zero elsewhere.
    ///
    /// A \c Mutable_TimeSeries that has a discrete value in the range <tt>[-inf,stop)</tt> and
    ///     zero elsewhere.
    ///
    /// The named parameters for the constructor are, in order:
    ///   -# \c stop, with a default of \c Offset(0)
    ///   -# \c value, with a default of \c Value(1)
    ///   -# \c discretization, with a default of \c Discretization(1)
    ///   -# \c zero, with a default of \c Value(0)
    template<typename Value, typename Discretization, typename Offset>
    struct inverse_heaviside_series
      : time_series_facade<
            inverse_heaviside_series<Value, Discretization, Offset>
          , storage::inverse_heaviside_array<Value, Offset, storage::constant_elements<Value> >
          , Discretization
        >
    {
        typedef time_series_facade<
            inverse_heaviside_series<Value, Discretization, Offset>
          , storage::inverse_heaviside_array<Value, Offset, storage::constant_elements<Value> >
          , Discretization
        > base_type;

        using base_type::operator=;

        BOOST_TIME_SERIES_DEFINE_CTORS(inverse_heaviside_series)
    };

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct storage_category<inverse_heaviside_series<Value, Discretization, Offset> >
          : storage_category<storage::inverse_heaviside_array<Value, Offset, storage::constant_elements<Value> > >
        {};

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct discretization_type<inverse_heaviside_series<Value, Discretization, Offset> >
        {
            typedef Discretization type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct offset_type<inverse_heaviside_series<Value, Discretization, Offset> >
        {
            typedef Offset type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct generate_series<scaled_storage_tag<inverse_heaviside_storage_tag>, Value, Discretization, Offset>
        {
            typedef inverse_heaviside_series<Value, Discretization, Offset> type;
        };
    }

}}

namespace boost { namespace sequence { namespace impl
{
    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::inverse_heaviside_series<Value, Discretization, Offset> >
    {
        typedef time_series_tag type;
    };
}}}

namespace boost { namespace time_series
{
    /// \brief Fetches the offset at which the inverse heaviside series jumps from non-zero to zero.
    ///
    /// Fetches the offset at which the inverse heaviside series jumps from non-zero to zero.
    ///
    /// \throw nothrow
    template<typename Value, typename Discretization, typename Offset>
    Offset jump_offset(inverse_heaviside_series<Value, Discretization, Offset> const &series)
    {
        return range_run_storage::end_offset(range_run_storage::pre_run(series));
    }

    /// \overload
    ///
    template<typename Value, typename Discretization, typename Offset>
    Offset jump_offset(inverse_heaviside_unit_series<Value, Discretization, Offset> const &series)
    {
        return range_run_storage::end_offset(range_run_storage::pre_run(series));
    }
}}

namespace boost { namespace constructors { namespace impl
{
    /// INTERNAL ONLY
    struct inverse_heaviside_series_tag;

    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::inverse_heaviside_series<Value, Discretization, Offset> >
    {
        typedef inverse_heaviside_series_tag type;
    };

    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::inverse_heaviside_unit_series<Value, Discretization, Offset> >
    {
        typedef inverse_heaviside_series_tag type;
    };

    /// INTERNAL ONLY
    template<typename T>
    struct construct<T, inverse_heaviside_series_tag>
      : arg_pack_construct
    {
        typedef parameter::parameters<
            parameter::optional<time_series::tag::stop>
          , parameter::optional<time_series::tag::value>
          , parameter::optional<time_series::tag::discretization>
          , parameter::optional<time_series::tag::zero>
        > args_type;
    };

}}}

#endif
