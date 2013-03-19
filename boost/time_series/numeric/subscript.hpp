///////////////////////////////////////////////////////////////////////////////
/// \file subscript.hpp
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_INVERT_EAN_08_09_2006
#define BOOST_TIME_SERIES_NUMERIC_INVERT_EAN_08_09_2006

#include <boost/concept/requires.hpp>
#include <boost/range_run_storage/algorithm/for_each.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>

namespace boost { namespace time_series
{
    namespace detail
    {
        namespace rrs = range_run_storage;

        template<typename Series, typename Out>
        struct subscript_functor
        {
            subscript_functor(Series &series, Out &out)
              : out_(out)
              , series_(series)
            {}

            template<typename Value, typename Offset>
            void operator ()(Value &value, Offset offset, Offset) const
            {
                this->out_(this->series_[offset + value], offset);
            }

        private:
            Out &out_;
            Series &series_;
        };
    }

    /// \brief Builds a new series from an existing series and an "index" series.
    ///
    /// Builds a new series from an existing series and an "index" series, where the
    ///     new series has the following values and offsets:
    ///     \li <tt> \verbatim (series[ index_offset_0 + index_value_0 ], index_offset_0) \endverbatim </tt>
    ///     \li <tt> \verbatim (series[ index_offset_1 + index_value_1 ], index_offset_1) \endverbatim </tt>
    ///     \li <tt> \verbatim (series[ index_offset_2 + index_value_2 ], index_offset_2) \endverbatim </tt>
    ///     \li <tt> \verbatim ... \endverbatim </tt>
    ///
    ///     where index_offset_N is the offset of the N-th run in the index series,
    ///     and index_value_N is the value of the N-th run in the index series.
    ///
    /// \param series The input series.
    /// \param indices The index series.
    /// \param out The ordered inserter to receive the adjacent difference result.
    ///
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre \c Indices is a model of the \c TimeSeries concept.
    /// \pre \c Out is a model of the \c OrderedInserter concept.
    /// \pre For variaibles \c x of the \c Indices \c value_type and \c y of 
    ///     the \c Indices \c offset_type, the expression \c (x+y) must be 
    ///     convertible to \c Series \c offset_type.
    /// \return A \c sparse_series\<\> if no ordered inserter is specified; otherwise,
    ///     the ordered inserter.
    /// \attention If using the version that takes an \c OrderedInserter, you must call
    ///     <tt>.commit()</tt> on the returned \c OrderedInserter when you are done with it.
    template<typename Series, typename Indices, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((concepts::TimeSeries<Indices const>)),
    (ordered_inserter<Out>))
    subscript(Series const &series, Indices const &indices, ordered_inserter<Out> out)
    {
        detail::subscript_functor<Series const, ordered_inserter<Out> >
            fun(series, out);

        range_run_storage::for_each(indices, fun);
        return out;
    }

    /// \overload
    ///
    template<typename Series, typename Indices>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((concepts::TimeSeries<Indices const>)),
    (sparse_series<
        typename concepts::TimeSeries<Series const>::value_type
      , typename concepts::TimeSeries<Series const>::discretization_type
      , typename concepts::TimeSeries<Indices const>::offset_type
    >))
    subscript(Series const &series, Indices const &indices)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::TimeSeries<Series const>::discretization_type discretization_type;
        typedef typename concepts::TimeSeries<Indices const>::offset_type offset_type;

        // The subscripts are held in a sparse array.
        sparse_series<value_type, discretization_type, offset_type> result(
            time_series::discretization = series.discretization()
        );

        time_series::subscript(
            series
          , indices
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }

}}

#endif
