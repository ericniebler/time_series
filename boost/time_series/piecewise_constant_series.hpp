///////////////////////////////////////////////////////////////////////////////
/// \file piecewise_constant_series.hpp
/// A time series that uses piecewise_constant storage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_PIECEWISE_CONSTANT_SERIES_HPP_EAN_05_29_2006
#define BOOST_TIME_SERIES_PIECEWISE_CONSTANT_SERIES_HPP_EAN_05_29_2006

#include <boost/time_series/time_series_facade.hpp>
#include <boost/time_series/storage/piecewise_constant_array.hpp>

namespace boost { namespace time_series
{

    /// \brief A \c Mutable_TimeSeries that has an arbitrary number of runs of arbitrary length and value.
    ///
    /// A \c Mutable_TimeSeries that has an arbitrary number of runs of arbitrary length and value.
    /// 
    /// The named parameters for the constructor are, in order:
    ///   -# \c discretization, with a default of \c Discretization(1)
    ///   -# \c zero, with a default of \c Value(0)
    template<typename Value, typename Discretization, typename Offset>
    struct piecewise_constant_series
      : time_series_facade<
            piecewise_constant_series<Value, Discretization, Offset>
          , storage::piecewise_constant_array<Value, Offset>
          , Discretization
        >
    {
        typedef time_series_facade<
            piecewise_constant_series<Value, Discretization, Offset>
          , storage::piecewise_constant_array<Value, Offset>
          , Discretization
        > base_type;

        using base_type::operator=;

        BOOST_TIME_SERIES_DEFINE_CTORS(piecewise_constant_series)
    };

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct storage_category<piecewise_constant_series<Value, Discretization, Offset> >
          : storage_category<storage::piecewise_constant_array<Value, Offset> >
        {};

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct discretization_type<piecewise_constant_series<Value, Discretization, Offset> >
        {
            typedef Discretization type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct offset_type<piecewise_constant_series<Value, Discretization, Offset> >
        {
            typedef Offset type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct generate_series<piecewise_constant_storage_tag, Value, Discretization, Offset>
        {
            typedef piecewise_constant_series<Value, Discretization, Offset> type;
        };
    }

}}

namespace boost { namespace sequence { namespace impl
{
    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::piecewise_constant_series<Value, Discretization, Offset> >
    {
        typedef time_series_tag type;
    };
}}}

namespace boost { namespace constructors { namespace impl
{
    /// INTERNAL ONLY
    struct piecewise_constant_series_tag;

    /// INTERNAL ONLY
    template<typename Value, typename Discretization, typename Offset>
    struct tag<time_series::piecewise_constant_series<Value, Discretization, Offset> >
    {
        typedef piecewise_constant_series_tag type;
    };

    /// INTERNAL ONLY
    template<typename T>
    struct construct<T, piecewise_constant_series_tag>
      : arg_pack_construct
    {
        typedef parameter::parameters<
            parameter::optional<time_series::tag::discretization>
          , parameter::optional<time_series::tag::zero>
        > args_type;
    };

}}}

#endif
