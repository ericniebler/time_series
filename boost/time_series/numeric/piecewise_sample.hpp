///////////////////////////////////////////////////////////////////////////////
/// \file piecewise_sample.hpp
/// Calculate a piecewise constant series by sampling one series at
/// steps specified by a \c Range.
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_PIECEWISE_SAMPLE_060615_HPP
#define BOOST_TIME_SERIES_NUMERIC_PIECEWISE_SAMPLE_060615_HPP

#include <boost/implicit_cast.hpp>
#include <boost/concept/requires.hpp>
#include <boost/detail/range_concepts.hpp>
#include <boost/range_run_storage/algorithm/transform.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/numeric/detail/tagged_ops.hpp>
#include <boost/time_series/numeric/detail/periods_adaptors.hpp>

namespace boost { namespace time_series
{

    namespace detail
    {
        namespace rrs = range_run_storage;

        template<typename Value, typename Offset, typename Out>
        struct piecewise_sample_inserter
        {
            template<typename Series>
            piecewise_sample_inserter(Series const &series, Out const &out)
              : out_(out)
              , value_(rrs::get_at(series, -inf))
              , zero_(implicit_cast<Value const &>(rrs::zero(series)))
              , offset_(-inf)
            {}

            template<typename Run, typename Arg>
            void set_at(Run const &run, left<Arg> const &value)
            {}

            template<typename Run, typename Arg>
            void set_at(Run const &run, right<Arg> const &value)
            {
                this->out_(this->value_, this->offset_, rrs::offset(run));
                this->offset_ = rrs::offset(run);
                this->value_ = this->zero_;
            }

            template<typename Run, typename Left, typename Right>
            void set_at(Run const &run, both<Left, Right> const &value)
            {
                this->out_(this->value_, this->offset_, rrs::offset(run));
                this->offset_ = rrs::offset(run);
                this->value_ = value.left;
            }

            Out const &finalize()
            {
                this->out_(this->value_, this->offset_, inf);
                return this->out_;
            }
        private:
            Out out_;
            Value value_;
            Value zero_;
            Offset offset_;
        };

    } // namespace detail

    /// \brief Calculate a piecewise constant series by sampling one series at
    /// steps specified by a \c Range.
    ///
    /// Calculate a piecewise constant series by sampling one series at
    /// steps specified by a \c Range. The steps are specified with \c periods as follows:
    /// \li <tt> \verbatim [-inf,*boost::begin(periods)) \endverbatim </tt>
    /// \li <tt> \verbatim [*boost::begin(periods),*(boost::begin(periods)+1)) \endverbatim </tt>
    /// \li <tt> \verbatim [*(boost::begin(periods)+1),*(boost::begin(periods)+2)) \endverbatim </tt>
    /// \li <tt> \verbatim ... \endverbatim </tt>
    /// \li <tt> \verbatim [*(boost::end(periods)-1),inf) \endverbatim </tt>
    ///
    /// \param series The series to sample.
    /// \param periods The steps at which to sample the series.
    /// \param out The inserter into which to write the resulting series.
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre \c Periods is a model of the \c RandomAccessRange concept.
    /// \pre \c Out is a model of the \c OrderedInserter concept.
    template<typename Series, typename Periods, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((concepts::RandomAccessRange<Periods /*const*/>))
        ,
    (ordered_inserter<Out>))
    piecewise_sample(Series const &series, Periods const &periods, ordered_inserter<Out> out)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::RandomAccessRange<Periods /*const*/>::value_type offset_type;

        detail::periods_storage_adaptor<Periods const> periods_adaptr(periods);
        detail::piecewise_sample_inserter<value_type, offset_type, ordered_inserter<Out> >
            o(series, out);

        return range_run_storage::transform(
            series
          , periods_adaptr
          , detail::both_op
          , detail::left_op
          , detail::right_op
          , o
        ).finalize();
    }

    /// \overload
    ///
    template<typename Series, typename Periods>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((concepts::RandomAccessRange<Periods /*const*/>))
        ,
    (piecewise_constant_series<
        typename concepts::TimeSeries<Series const>::value_type
      , typename concepts::TimeSeries<Series const>::discretization_type
      , typename concepts::RandomAccessRange<Periods /*const*/>::value_type
    >))
    piecewise_sample(Series const &series, Periods const &periods)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::TimeSeries<Series const>::discretization_type discretization_type;
        typedef typename concepts::RandomAccessRange<Periods /*const*/>::value_type offset_type;

        piecewise_constant_series<value_type, discretization_type, offset_type> result(
            time_series::discretization = series.discretization()
          , time_series::zero = range_run_storage::zero(series)
        );

        time_series::piecewise_sample(
            series
          , periods
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }

}} // namespace boost::time_series

#endif // BOOST_TIME_SERIES_NUMERIC_PIECEWISE_SAMPLE_060615_HPP
