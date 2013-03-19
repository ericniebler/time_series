///////////////////////////////////////////////////////////////////////////////
/// \file variable_period_sums.hpp
/// Calculate the sums within variable-width ranges in a series
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_VARIABLE_PERIOD_SUMS_EAN_08_10_2006
#define BOOST_TIME_SERIES_NUMERIC_VARIABLE_PERIOD_SUMS_EAN_08_10_2006

#include <boost/implicit_cast.hpp>
#include <boost/concept/requires.hpp>
#include <boost/detail/range_concepts.hpp>
#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/range_run_storage/algorithm/transform.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/detail/tagged_ops.hpp>
#include <boost/time_series/numeric/detail/periods_adaptors.hpp>

namespace boost { namespace time_series
{

    namespace detail
    {
        namespace rrs = range_run_storage;

        // variable_period_sums_inserter
        template<typename Out, typename Series, typename Shift = mpl::false_>
        struct variable_period_sums_inserter
        {
            typedef typename concepts::TimeSeries<Series>::value_type value_type;
            typedef typename concepts::TimeSeries<Series>::offset_type offset_type;

            variable_period_sums_inserter(Out const &out, Series const &series)
              : out_(out)
              , zero_(implicit_cast<value_type const &>(rrs::zero(series)))
              , value_(implicit_cast<value_type const &>(rrs::zero(series)))
              , offset_(-inf)
            {}

            template<typename Run, typename Arg>
            void set_at(Run const &run, left<Arg> const &value)
            {
                this->value_ += value.arg * rrs::length(run);
            }

            template<typename Run, typename Arg>
            void set_at(Run const &run, right<Arg> const &value)
            {
                this->flush();
                this->offset_ = rrs::offset(run);
                this->value_ = this->zero_;
                this->shift_(value.arg, Shift());
            }

            template<typename Run, typename Left, typename Right>
            void set_at(Run const &run, both<Left, Right> const &value)
            {
                this->flush();
                this->offset_ = rrs::offset(run);
                this->value_ = value.left * rrs::length(run);
                this->shift_(value.right, Shift());
            }

            Out const &flush()
            {
                if(-inf != this->offset_)
                {
                    this->out_(this->value_, this->offset_);
                }
                return this->out_;
            }
        private:
            template<typename Offset>
            void shift_(Offset, mpl::false_)
            {}

            template<typename Offset>
            void shift_(Offset shift, mpl::true_)
            {
                this->offset_ += shift;
            }

            Out out_;
            value_type zero_;
            value_type value_;
            offset_type offset_;
        };
    }

    /// \brief Calculate the sums within variable-width periods in a series.
    ///
    /// Calculate the sums within variable-width periods in a series, where
    ///     the periods are defined in a \c RandomAccessRange. The periods
    ///     are specified as follows:
    /// \li <tt> \verbatim [-inf,*boost::begin(periods)) \endverbatim </tt>
    /// \li <tt> \verbatim [*boost::begin(periods),*(boost::begin(periods)+1)) \endverbatim </tt>
    /// \li <tt> \verbatim [*(boost::begin(periods)+1),*(boost::begin(periods)+2)) \endverbatim </tt>
    /// \li <tt> \verbatim ... \endverbatim </tt>
    /// \li <tt> \verbatim [*(boost::end(periods)-1),inf) \endverbatim </tt>
    ///
    /// The sum for each period is stored at the start of the period.
    ///
    /// \param series The input series.
    /// \param periods The periods over which to calculate the sums.
    /// \param out The inserter into which to write the new series.
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre \c Periods is a model of the \c RandomAccessRange concept.
    /// \pre \c Out is a model of the \c OrderedInserter concept.
    template<typename Series, typename Periods, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((concepts::RandomAccessRange<Periods /*const*/>))
        ,
    (ordered_inserter<Out>))
    variable_period_sums(Series const &series, Periods const &periods, ordered_inserter<Out> out)
    {
        detail::periods_storage_adaptor<Periods const> periods_adaptr(periods);
        detail::variable_period_sums_inserter<ordered_inserter<Out>, Series>
            o(out, series);

        return range_run_storage::transform(
            series
          , periods_adaptr
          , detail::both_op
          , detail::left_op
          , detail::right_op
          , o
        ).flush();
    }

    /// \overload
    ///
    template<typename Series, typename Periods>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((concepts::RandomAccessRange<Periods /*const*/>))
        ,
    (sparse_series<
        typename concepts::TimeSeries<Series const>::value_type
      , typename concepts::TimeSeries<Series const>::discretization_type
      , typename concepts::TimeSeries<Series const>::offset_type
    >))
    variable_period_sums(Series const &series, Periods const &periods)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::TimeSeries<Series const>::discretization_type discretization_type;
        typedef typename concepts::TimeSeries<Series const>::offset_type offset_type;
        typedef typename concepts::RandomAccessRange<Periods /*const*/>::value_type periods_offset_type;

        BOOST_MPL_ASSERT((is_same<offset_type, periods_offset_type>));

        // The periodic sums are held in a sparse array. The sums are
        // stored at the start of their associated periods.
        sparse_series<value_type, discretization_type, offset_type> result(
            time_series::discretization = series.discretization()
        );

        time_series::variable_period_sums(
            series
          , periods
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }


    /// \brief Calculate the sums within variable-width periods in a series
    ///     with a variable shift, where the periods and the shifts are defined
    ///     in a \c RandomAccessRange.
    ///
    /// Calculate the sums within variable-width periods in a series
    ///     with a variable shift, where the periods and the shifts are defined
    ///     in a \c RandomAccessRange. The periods are specified as follows:
    /// \li <tt> \verbatim [-inf,*boost::begin(periods)) \endverbatim </tt>
    /// \li <tt> \verbatim [*boost::begin(periods),*(boost::begin(periods)+1)) \endverbatim </tt>
    /// \li <tt> \verbatim [*(boost::begin(periods)+1),*(boost::begin(periods)+2)) \endverbatim </tt>
    /// \li <tt> \verbatim ... \endverbatim </tt>
    /// \li <tt> \verbatim [*(boost::end(periods)-1),inf) \endverbatim </tt>
    ///
    /// The sum for each period is stored at the start of the period, plus
    ///     the shift for that period.
    ///
    /// \param series The input series.
    /// \param periods The periods over which to calculate the sums.
    /// \param shifts The shift offsets for each period.
    /// \param out The inserter into which to write the new series.
    /// \pre \c Series is a model of the \c TimeSeries concept.
    /// \pre \c Periods is a model of the \c RandomAccessRange concept.
    /// \pre \c Shifts is a model of the \c RandomAccessRange concept.
    /// \pre \c Out is a model of the \c OrderedInserter concept.
    /// \pre <tt> \verbatim boost::size(periods) == boost::size(shifts) \endverbatim </tt>
    template<typename Series, typename Periods, typename Shifts, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((concepts::RandomAccessRange<Periods /*const*/>))
        ((concepts::RandomAccessRange<Shifts /*const*/>))
        ,
    (ordered_inserter<Out>))
    shifted_variable_period_sums(
        Series const &series
      , Periods const &periods
      , Shifts const &shifts
      , ordered_inserter<Out> out
    )
    {
        detail::periods_and_elements_storage_adaptor<Periods const, Shifts const> periods_adaptr(periods, shifts);
        detail::variable_period_sums_inserter<ordered_inserter<Out>, Series, mpl::true_>
            o(out, series);

        return range_run_storage::transform(
            series
          , periods_adaptr
          , detail::both_op
          , detail::left_op
          , detail::right_op
          , o
        ).flush();
    }

    /// \overload
    ///
    template<typename Series, typename Periods, typename Shifts>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>))
        ((concepts::RandomAccessRange<Periods /*const*/>))
        ((concepts::RandomAccessRange<Shifts /*const*/>))
        ,
    (sparse_series<
        typename concepts::TimeSeries<Series const>::value_type
      , typename concepts::TimeSeries<Series const>::discretization_type
      , typename concepts::TimeSeries<Series const>::offset_type
    >))
    shifted_variable_period_sums(Series const &series, Periods const &periods, Shifts const &shifts)
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;
        typedef typename concepts::TimeSeries<Series const>::discretization_type discretization_type;
        typedef typename concepts::TimeSeries<Series const>::offset_type offset_type;
        typedef typename concepts::RandomAccessRange<Periods /*const*/>::value_type periods_offset_type;

        BOOST_MPL_ASSERT((is_same<offset_type, periods_offset_type>));

        // The periodic sums are held in a sparse array. The sums are
        // stored at the start of their associated periods.
        sparse_series<value_type, discretization_type, offset_type> result(
            time_series::discretization = series.discretization()
        );

        time_series::shifted_variable_period_sums(
            series
          , periods
          , shifts
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }

}}

#endif // BOOST_TIME_SERIES_NUMERIC_VARIABLE_PERIOD_SUMS_EAN_08_10_2006
