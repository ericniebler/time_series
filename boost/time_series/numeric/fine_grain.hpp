// Copyright Daniel Wallin 2006. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_FINE_GRAIN_060614_HPP
# define BOOST_TIME_SERIES_NUMERIC_FINE_GRAIN_060614_HPP

#include <boost/concept/requires.hpp>
#include <boost/detail/pod_singleton.hpp>
#include <boost/range_run_storage/algorithm/for_each.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>

namespace boost { namespace time_series
{

    namespace samplers
    {

        template <typename Derived>
        struct sampler_base
        {
            Derived const &cast() const
            {
                return static_cast<Derived const&>(*this);
            }
        };

        /// A fine_grain upsampler that makes a piecewise constant function
        /// with jumps at the coarse-grained points.
        ///
        struct piecewise_upsample
          : sampler_base<piecewise_upsample>
        {
            template <typename ValueType, typename Discretization>
            struct apply
            {
                typedef piecewise_constant_series<ValueType, Discretization> type;
            };

            template <typename Value, typename Offset, typename Out>
            void operator()(Out &out, Value value, Offset offset, Offset endoff, Offset factor) const
            {
                out(value, offset * factor, endoff * factor);
            }
        };

        /// A fine_grain upsampler that sets non-zero values at the coarse-grained
        /// points.
        ///
        struct sparse_upsample
          : sampler_base<sparse_upsample>
        {
            template <typename ValueType, typename Discretization>
            struct apply
            {
                typedef sparse_series<ValueType, Discretization> type;
            };

            template <typename Value, typename Offset, typename Out>
            void operator()(Out &out, Value value, Offset offset, Offset endoff, Offset factor) const
            {
                std::size_t length = endoff - offset;
                offset *= factor;

                while(length-- > 0)
                {
                    out(value, offset);
                    offset += factor;
                }
            }
        };

        /// INTERNAL ONLY
        template<typename UpSampler, typename Out, typename Offset>
        struct upsampler_functor
        {
            upsampler_functor(UpSampler upsampler, Out &out, Offset factor)
              : upsampler_(upsampler)
              , out_(out)
              , factor_(factor)
            {}

            template<typename Value>
            void operator ()(Value const &value, Offset start, Offset stop) const
            {
                this->upsampler_(this->out_, value, start, stop, this->factor_);
            }
        private:
            UpSampler upsampler_;
            Out &out_;
            Offset factor_;
        };

    } // namespace samplers

    namespace 
    {
        samplers::piecewise_upsample const &piecewise_upsample
            = boost::detail::pod_singleton<samplers::piecewise_upsample>::instance;

        samplers::sparse_upsample const &sparse_upsample
            = boost::detail::pod_singleton<samplers::sparse_upsample>::instance;
    }

    /// \brief Generate a series of finer discretization from a series of coarser
    /// discretization according to a sampling policy.
    ///
    /// Generate a series of finer discretization from a series of coarser
    /// discretization according to a sampling policy.
    ///
    /// \param series The input series.
    /// \param discretization The finer discretization.
    /// \param upsampler The sampling policy. Could be \c piecewise_upsample or \c sparse_upsample.
    /// \param out An \c OrderedInserter into which to write the finer series.
    /// \return If you specify an \c OrderedInserter, the return value is a copy of the
    ///     \c OrderedInserter. Otherwise, if you specify the \c sparse_upsample, the return
    ///     value is a <tt>sparse_series\<\></tt> containing the finer-grained series. Otherwise,
    ///     if you specify the \c piecewise_upsample, the return value is a
    ///     <tt>piecewise_constant_series\<\></tt> containing the finer-grained series.
    /// \attention If using the version that takes an \c OrderedInserter, you must call
    ///     <tt>.commit()</tt> on the returned \c OrderedInserter when you are done with it.
    /// \pre The coarser discretization is a multiple of the finer discretization.
    template<typename Series, typename Discretization, typename UpSampler, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (ordered_inserter<Out>))
    fine_grain(
        Series const &series
      , Discretization discretization
      , samplers::sampler_base<UpSampler> const &upsampler
      , ordered_inserter<Out> out
    )
    {
        typedef typename concepts::TimeSeries<Series const>::offset_type offset_type;

        BOOST_ASSERT(series.discretization() > discretization);
        BOOST_ASSERT(series.discretization() % discretization == 0);

        offset_type factor = series.discretization() / discretization;
        samplers::upsampler_functor<UpSampler, ordered_inserter<Out>, offset_type>
            fun(upsampler.cast(), out, factor);

        range_run_storage::for_each(series, fun);
        return out;
    }

    /// \overload
    ///
    template <typename Series, typename Discretization, typename Out>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (ordered_inserter<Out>))
    fine_grain(
        Series const &series
      , Discretization discretization
      , ordered_inserter<Out> out
    )
    {
        return time_series::fine_grain(
            series
          , discretization
          , piecewise_upsample
          , out
        );
    }

    /// \overload
    ///
    template<typename Series, typename Discretization, typename UpSampler>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (typename mpl::apply_wrap2<
        UpSampler
      , typename concepts::TimeSeries<Series const>::value_type
      , Discretization
    >::type))
    fine_grain(
        Series const &series
      , Discretization discretization
      , samplers::sampler_base<UpSampler> const &upsampler
    )
    {
        typedef typename concepts::TimeSeries<Series const>::value_type value_type;

        BOOST_ASSERT(series.discretization() > discretization);
        BOOST_ASSERT(series.discretization() % discretization == 0);

        typedef typename mpl::apply_wrap2<
            UpSampler
          , value_type
          , Discretization
        >::type result_type;

        result_type result(time_series::discretization = discretization);

        time_series::fine_grain(
            series
          , discretization
          , upsampler.cast()
          , time_series::make_ordered_inserter(result)
        ).commit();

        return result;
    }

    /// \overload
    ///
    template <typename Series, typename Discretization>
    BOOST_CONCEPT_REQUIRES(
        ((concepts::TimeSeries<Series const>)),
    (typename mpl::apply_wrap2<
        samplers::piecewise_upsample
      , typename concepts::TimeSeries<Series const>::value_type
      , Discretization
    >::type))
    fine_grain(Series const &series, Discretization discretization)
    {
        return time_series::fine_grain(
            series
          , discretization
          , piecewise_upsample
        );
    }

}} // namespace boost::time_series

#endif // BOOST_TIME_SERIES_NUMERIC_FINE_GRAIN_060614_HPP
