///////////////////////////////////////////////////////////////////////////////
/// \file dense_series.hpp
/// A time series that uses dense storage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_DENSE_SERIES_HPP_EAN_05_29_2006
#define BOOST_TIME_SERIES_DENSE_SERIES_HPP_EAN_05_29_2006

#include <cstddef>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/time_series/time_series_facade.hpp>
#include <boost/time_series/storage/dense_array.hpp>

namespace boost { namespace time_series
{

    /// \brief A \c Mutable_TimeSeries that has dense storage.
    ///
    /// A \c Mutable_TimeSeries that has dense storage within some <tt>[start,stop)</tt> range,
    /// an optional pre-run with a distinct value stretching from <tt>[-inf,pre_stop)</tt>,
    /// an optional post-run with a distinct value stretching from <tt>[post_start,inf)</tt>,
    /// where <tt>pre_stop \<= start</tt> and <tt>post_start \>= stop</tt>, and zero elsewhere.
    ///
    /// The named parameters for the constructor are, in order:
    ///   -# \c start, with a default of \c std::ptrdiff_t(0)
    ///   -# \c stop, with a default of the \c start named parameter
    ///   -# \c value, with a default of the \c zero named parameter
    ///   -# \c discretization, with a default of \c Discretization(1)
    ///   -# \c zero, with a default of \c Value(0)
    template<typename Value, typename Discretization>
    struct dense_series
      : time_series_facade<
            dense_series<Value, Discretization>
          , storage::dense_array<Value>
          , Discretization
        >
    {
        typedef time_series_facade<
            dense_series<Value, Discretization>
          , storage::dense_array<Value>
          , Discretization
        > base_type;

        using base_type::operator=;

        BOOST_TIME_SERIES_DEFINE_CTORS(dense_series)
    };

    /// \brief Fetch a raw pointer to the internal dense storage of a \c dense_series\<\>
    ///
    /// Fetch a raw pointer to the internal dense storage of a \c dense_series\<\>
    ///
    /// \return A \c std::pair\<\> containing the raw pointer to the buffer and a
    ///     \c std::size_t with the number of elements in the buffer. If the \c dense_series\<\>
    ///     is empty, the returned pointer is 0 and the returned size is 0.
    /// \throw nothrow
    template<typename Value, typename Discretization>
    inline std::pair<Value *, std::size_t>
    get_raw_buffer(dense_series<Value, Discretization> &series)
    {
        typedef detail::time_series_facade_access access;
        typedef std::pair<Value *, std::size_t> buffer_type;
        storage::dense_array<Value> &data = access::data(series);
        return data.empty() ? buffer_type(0, 0) : buffer_type(&*data.begin(), data.size());
    }

    /// \overload
    ///
    template<typename Value, typename Discretization>
    inline std::pair<Value const *, std::size_t>
    get_raw_buffer(dense_series<Value, Discretization> const &series)
    {
        typedef detail::time_series_facade_access access;
        typedef std::pair<Value const *, std::size_t> buffer_type;
        storage::dense_array<Value> const &data = access::data(series);
        return data.empty() ? buffer_type(0, 0) : buffer_type(&*data.begin(), data.size());
    }

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Value, typename Discretization>
        struct storage_category<dense_series<Value, Discretization> >
          : storage_category<storage::dense_array<Value> >
        {};

        /// INTERNAL ONLY
        template<typename Value, typename Discretization>
        struct discretization_type<dense_series<Value, Discretization> >
        {
            typedef Discretization type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization>
        struct offset_type<dense_series<Value, Discretization> >
        {
            typedef std::ptrdiff_t type;
        };

        /// INTERNAL ONLY
        template<typename Value, typename Discretization, typename Offset>
        struct generate_series<dense_storage_tag, Value, Discretization, Offset>
        {
            BOOST_MPL_ASSERT((is_same<Offset, std::ptrdiff_t>));
            typedef dense_series<Value, Discretization> type;
        };
    }

}}

namespace boost { namespace sequence { namespace impl
{
    /// INTERNAL ONLY
    template<typename Value, typename Discretization>
    struct tag<time_series::dense_series<Value, Discretization> >
    {
        typedef time_series_tag type;
    };
}}}

namespace boost { namespace constructors { namespace impl
{
    /// INTERNAL ONLY
    struct dense_series_tag;

    /// INTERNAL ONLY
    template<typename Value, typename Discretization>
    struct tag<time_series::dense_series<Value, Discretization> >
    {
        typedef dense_series_tag type;
    };

    /// INTERNAL ONLY
    template<typename T>
    struct construct<T, dense_series_tag>
      : arg_pack_construct
    {
        typedef parameter::parameters<
            parameter::optional<time_series::tag::start>
          , parameter::optional<time_series::tag::stop>
          , parameter::optional<time_series::tag::value>
          , parameter::optional<time_series::tag::discretization>
          , parameter::optional<time_series::tag::zero>
        > args_type;
    };

}}}

#endif
