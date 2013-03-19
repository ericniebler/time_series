///////////////////////////////////////////////////////////////////////////////
/// \file scaled_series.hpp
/// A scaled series that satisfies the \c InfiniteRangeRunStorage concept
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_SCALED_SERIES_EAN_06_16_2006
#define BOOST_TIME_SERIES_SCALED_SERIES_EAN_06_16_2006

#include <utility>
#include <algorithm>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/traits/is_time_series.hpp>
#include <boost/time_series/traits/offset_type.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/traits/discretization_type.hpp>
#include <boost/time_series/utility/time_series_base.hpp>
#include <boost/time_series/storage/detail/scaled_elements.hpp>

// A view of an array that has been scaled by a constant factor.
namespace boost { namespace time_series
{
    // The scaled_series
    template<typename Series, typename Factor>
    struct scaled_series
      : time_series_base<scaled_series<Series, Factor> >
    {
    public:
        typedef Series storage_type;
        typedef Factor factor_type;
        typedef concepts::TimeSeries<Series> series_concept;

        typedef typename series_concept::value_type value_type;
        typedef typename series_concept::cursor cursor;
        typedef typename series_concept::elements elements;
        typedef typename series_concept::runs runs;
        typedef typename series_concept::run_type run_type;
        typedef typename series_concept::offset_type offset_type;
        typedef typename series_concept::discretization_type discretization_type;

        typedef value_type const reference;

        scaled_series(Series &series, factor_type factor)
          : data_(series)
          , factor_(factor)
        {}

        discretization_type discretization() const
        {
            return this->data().discretization();
        }

        reference operator [](offset_type n) const
        {
            return range_run_storage::get_at(this->data(), n) * this->factor();
        }

        // TODO make these private
    //private:
        factor_type factor() const
        {
            return this->factor_;
        }

        storage_type &data() const
        {
            return this->data_;
        }

    private:
        Series &data_;
        factor_type factor_;
    };

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Series, typename Factor>
        struct discretization_type<scaled_series<Series, Factor> >
          : discretization_type<Series>
        {};

        /// INTERNAL ONLY
        template<typename Series, typename Factor>
        struct offset_type<scaled_series<Series, Factor> >
          : offset_type<Series>
        {};

        /// INTERNAL ONLY
        template<typename Series, typename Factor>
        struct storage_category<scaled_series<Series, Factor> >
          : storage_category<Series>
        {};
    }
}} // namespace boost::time_series

namespace boost
{
    namespace sequence { namespace impl
    {
        /// INTERNAL ONLY
        struct scaled_series_tag;

        /// INTERNAL ONLY
        template<typename Series, typename Factor>
        struct tag<time_series::scaled_series<Series, Factor> >
        {
            typedef scaled_series_tag type;
        };

        /// INTERNAL ONLY
        template<typename S>
        struct elements<S, scaled_series_tag>
        {
            typedef typename S::factor_type factor_type;
            typedef typename S::elements elements_type;
            typedef time_series::storage::scaled_elements<elements_type, factor_type> result_type;

            result_type operator ()(S &s) const
            {
                return result_type(sequence::elements(s.data()), s.factor());
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct begin<S, scaled_series_tag>
        {
            typedef typename S::cursor result_type;

            result_type operator ()(S &s) const
            {
                return sequence::begin(s.data());
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct end<S, scaled_series_tag>
        {
            typedef typename S::cursor result_type;

            result_type operator ()(S &s) const
            {
                return sequence::end(s.data());
            }
        };
    }}

    namespace range_run_storage { namespace impl
    {
        /// INTERNAL ONLY
        template<typename S>
        struct runs<S, sequence::impl::scaled_series_tag>
        {
            typedef typename S::runs result_type;

            result_type operator ()(S &s) const
            {
                return range_run_storage::runs(s.data());
            }
        };

        /// INTERNAL ONLY
        template<typename S, typename I>
        struct get_at<S, I, sequence::impl::scaled_series_tag>
        {
            typedef typename S::reference result_type;

            result_type operator ()(S &s, I &i) const
            {
                return s[i];
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct zero<S, sequence::impl::scaled_series_tag>
        {
            typedef typename S::storage_type storage_type;
            typedef typename result_of<op::zero(storage_type &)>::type result_type;

            result_type operator ()(S &s) const
            {
                return range_run_storage::zero(s.data());
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct pre_run<S, sequence::impl::scaled_series_tag>
        {
            typedef typename S::storage_type storage_type;
            typedef typename result_of<op::pre_run(storage_type &)>::type result_type;

            result_type operator ()(S &s) const
            {
                return range_run_storage::pre_run(s.data());
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct pre_value<S, sequence::impl::scaled_series_tag>
        {
            typedef typename S::reference result_type;

            result_type operator ()(S &s) const
            {
                return range_run_storage::pre_value(s.data()) * s.factor();
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct post_run<S, sequence::impl::scaled_series_tag>
        {
            typedef typename S::storage_type storage_type;
            typedef typename result_of<op::post_run(storage_type &)>::type result_type;

            result_type operator ()(S &s) const
            {
                return range_run_storage::post_run(s.data());
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct post_value<S, sequence::impl::scaled_series_tag>
        {
            typedef typename S::reference result_type;

            result_type operator ()(S &s) const
            {
                return range_run_storage::post_value(s.data()) * s.factor();
            }
        };

    }}
}

#endif
