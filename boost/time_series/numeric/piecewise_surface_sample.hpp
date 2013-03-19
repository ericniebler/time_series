///////////////////////////////////////////////////////////////////////////////
/// \file piecewise_surface_sample.hpp
/// Calculate a piecewise constant surface by sampling series at
/// steps defined by two range representing the sample points
/// for x and y coordinate of the surface.
//
//  Copyright 2006 Daniel Egloff. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_PIECEWISE_SURFACE_SAMPLE_060615_HPP
#define BOOST_TIME_SERIES_NUMERIC_PIECEWISE_SURFACE_SAMPLE_060615_HPP

#include <boost/implicit_cast.hpp>
#include <boost/concept/requires.hpp>
#include <boost/detail/range_concepts.hpp>
#include <boost/range_run_storage/algorithm/transform.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/numeric/detail/tagged_ops.hpp>
#include <boost/time_series/numeric/piecewise_sample.hpp>

namespace boost { namespace time_series
{

    namespace detail
    {
        namespace rrs = range_run_storage;

        template<typename Value, typename Offset, typename Out, typename Periods>
        struct piecewise_surface_sample_inserter
        {
            template<typename Series>
            piecewise_surface_sample_inserter(Series const &series, Periods const &periods, Out const &out)
              : out_(out)
              , value_(rrs::get_at(series, -inf))
              , zero_(implicit_cast<Value const &>(rrs::zero(series)))
              , offset_(-inf)
              , periods_(periods)
            {}

            template<typename Run, typename Arg>
            void set_at(Run const &run, left<Arg> const &value)
            {}

            template<typename Run, typename Arg>
            void set_at(Run const &run, right<Arg> const &value)
            {
                this->out_(
                    time_series::piecewise_sample(this->value_, this->periods_)
                  , this->offset_
                  , rrs::offset(run)
                );
                this->offset_ = rrs::offset(run);
                this->value_ = this->zero_;
            }

            template<typename Run, typename Left, typename Right>
            void set_at(Run const &run, both<Left, Right> const &value)
            {
                this->out_(
                    time_series::piecewise_sample(this->value_, this->periods_)
                  , this->offset_
                  , rrs::offset(run)
                );
                this->offset_ = rrs::offset(run);
                this->value_ = value.left;
            }

            Out const &finalize()
            {
                this->out_(
                    time_series::piecewise_sample(this->value_, this->periods_)
                  , this->offset_
                  , inf
                );
                return this->out_;
            }
        private:
            Out out_;
            Value value_;
            Value zero_;
            Offset offset_;
            Periods const &periods_;
        };

    } // namespace detail

    /// \brief Calculate a piecewise constant surface by sampling series at
    ///     steps defined by two ranges representing the sample points
    ///     for x and y coordinate of the surface.
    ///
    /// Calculate a piecewise constant surface by sampling series at
    ///     steps defined by two ranges representing the sample points
    ///     for x and y coordinate of the surface. The \c x periods are specified 
    ///     as follows:
    /// \li <tt> \verbatim [-inf,*boost::begin(x_periods)) \endverbatim </tt>
    /// \li <tt> \verbatim [*boost::begin(x_periods),*(boost::begin(x_periods)+1)) \endverbatim </tt>
    /// \li <tt> \verbatim [*(boost::begin(x_periods)+1),*(boost::begin(x_periods)+2)) \endverbatim </tt>
    /// \li <tt> \verbatim ... \endverbatim </tt>
    /// \li <tt> \verbatim [*(boost::end(x_periods)-1),inf) \endverbatim </tt>
    ///
    /// ... and likewise for \c y periods.
    ///
    /// \param series The series of series to sample
    /// \param x_periods The x coordinates of the surface
    /// \param y_periods The y coordinates of the surface
    /// \param out The inserter into which the new series is written
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre The \c value_type of \c Series is a model of the \c TimeSeries concept.
    /// \pre \c XPeriods is a model of the \c RandomAccessRange concept.
    /// \pre \c YPeriods is a model of the \c RandomAccessRange concept.
    /// \pre \c Out is a model of the \c OrderedInserter concept.
    template<typename Series, typename XPeriods, typename YPeriods, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((concepts::TimeSeries<typename concepts::TimeSeries<Series const>::value_type>))
        ((concepts::RandomAccessRange<XPeriods /*const*/>))
        ((concepts::RandomAccessRange<YPeriods /*const*/>))
        ,
    (ordered_inserter<Out>))
    piecewise_surface_sample(Series const &series, XPeriods const &x_periods, YPeriods const &y_periods, ordered_inserter<Out> out)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::RandomAccessRange<XPeriods /*const*/>::value_type offset_type;

        detail::periods_storage_adaptor<XPeriods const> x_periods_adaptr(x_periods);
        detail::piecewise_surface_sample_inserter<value_type, offset_type, ordered_inserter<Out>, YPeriods>
            o(series, y_periods, out);

        return range_run_storage::transform(
            series
          , x_periods_adaptr
          , detail::both_op
          , detail::left_op
          , detail::right_op
          , o
        ).finalize();
    }

    /// \overload
    ///
    template<typename Series, typename XPeriods, typename YPeriods>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((concepts::TimeSeries<typename concepts::TimeSeries<Series const>::value_type>))
        ((concepts::RandomAccessRange<XPeriods /*const*/>))
        ((concepts::RandomAccessRange<YPeriods /*const*/>))
        ,
    (piecewise_constant_series<
        typename concepts::TimeSeries<Series const>::value_type
      , typename concepts::TimeSeries<Series const>::discretization_type
      , typename concepts::RandomAccessRange<XPeriods /*const*/>::value_type
    >))
    piecewise_surface_sample(Series const &series, XPeriods const &x_periods, YPeriods const &y_periods)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::TimeSeries<Series const>::discretization_type discretization_type;
        typedef typename concepts::RandomAccessRange<XPeriods /*const*/>::value_type offset_type;

        piecewise_constant_series<value_type, discretization_type, offset_type> result(
            time_series::discretization = series.discretization()
          , time_series::zero = range_run_storage::zero(series)
        );

        time_series::piecewise_surface_sample(
            series
          , x_periods
          , y_periods
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }

}} // namespace boost::time_series

#endif // BOOST_TIME_SERIES_NUMERIC_PIECEWISE_SURFACE_SAMPLE_060615_HPP

