///////////////////////////////////////////////////////////////////////////////
/// \file shifted_series.hpp
/// A shifted series that satisfies the \c InfiniteRangeRunStorage concept
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_SHIFTED_SERIES_EAN_06_05_2006
#define BOOST_TIME_SERIES_SHIFTED_SERIES_EAN_06_05_2006

#include <utility>
#include <algorithm>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/traits/is_time_series.hpp>
#include <boost/time_series/traits/offset_type.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/traits/discretization_type.hpp>
#include <boost/time_series/utility/time_series_base.hpp>
#include <boost/time_series/storage/detail/offset_runs.hpp>

// A view of an array that has been shifted to the right or left
// by an offset.
namespace boost { namespace time_series
{
    // The shifted_series
    template<typename Series>
    struct shifted_series
      : time_series_base<shifted_series<Series> >
    {
        typedef Series storage_type;
        typedef concepts::TimeSeries<Series> series_concept;

        typedef typename series_concept::value_type value_type;
        typedef typename series_concept::cursor cursor;
        typedef typename series_concept::elements elements;
        typedef typename series_concept::runs runs;
        typedef typename series_concept::run_type run_type;
        typedef typename series_concept::offset_type offset_type;
        typedef typename series_concept::discretization_type discretization_type;

        typedef 
            typename result_of<
                range_run_storage::op::get_at(Series &, offset_type &)
            >::type
        reference;

        shifted_series(Series &series, offset_type shift)
          : data_(series)
          , shift_(shift)
        {}

        discretization_type discretization() const
        {
            return this->data().discretization();
        }

        reference operator [](offset_type n) const
        {
            return range_run_storage::get_at(this->data(), n - this->shift());
        }

        // TODO make these private
    //private:
        offset_type shift() const
        {
            return this->shift_;
        }

        storage_type &data() const
        {
            return this->data_;
        }

    private:
        Series &data_;
        offset_type shift_;
    };

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Series>
        struct discretization_type<time_series::shifted_series<Series> >
          : discretization_type<Series>
        {};

        /// INTERNAL ONLY
        template<typename Series>
        struct offset_type<time_series::shifted_series<Series> >
          : offset_type<Series>
        {};

        /// INTERNAL ONLY
        template<typename Series>
        struct storage_category<time_series::shifted_series<Series> >
          : storage_category<Series>
        {};
    }
}}


namespace boost
{
    namespace sequence { namespace impl
    {
        /// INTERNAL ONLY
        struct shifted_series_tag;

        /// INTERNAL ONLY
        template<typename Series>
        struct tag<time_series::shifted_series<Series> >
        {
            typedef shifted_series_tag type;
        };

        /// INTERNAL ONLY
        template<typename S>
        struct elements<S, shifted_series_tag>
        {
            typedef typename S::elements result_type;

            result_type operator ()(S &s) const
            {
                return sequence::elements(s.data());
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct begin<S, shifted_series_tag>
        {
            typedef typename S::cursor result_type;

            result_type operator ()(S &s) const
            {
                return sequence::begin(s.data());
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct end<S, shifted_series_tag>
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
        struct runs<S, sequence::impl::shifted_series_tag>
        {
            typedef typename S::runs runs_type;
            typedef typename S::offset_type offset_type;
            typedef time_series::storage::offset_runs<runs_type, offset_type> result_type;

            result_type operator ()(S &s) const
            {
                return result_type(range_run_storage::runs(s.data()), s.shift());
            }
        };

        /// INTERNAL ONLY
        template<typename S, typename I>
        struct get_at<S, I, sequence::impl::shifted_series_tag>
        {
            typedef typename S::reference result_type;

            result_type operator ()(S &s, I &i) const
            {
                return s[i];
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct zero<S, sequence::impl::shifted_series_tag>
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
        struct pre_run<S, sequence::impl::shifted_series_tag>
        {
            typedef typename S::storage_type storage_type;
            typedef typename result_of<op::pre_run(storage_type &)>::type result_type;
            typedef typename concepts::Run<result_type>::offset_type offset_type;

            result_type operator ()(S &s) const
            {
                result_type pre(range_run_storage::pre_run(s.data()));
                if(rrs::empty(pre))
                    return pre;

                offset_type start(range_run_storage::offset(pre));
                offset_type stop(range_run_storage::end_offset(pre));

                if(-inf != start)
                    start += s.shift();
                if(inf != stop)
                    stop += s.shift();

                return result_type(start, stop);
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct pre_value<S, sequence::impl::shifted_series_tag>
        {
            typedef typename S::storage_type storage_type;
            typedef typename result_of<op::pre_value(storage_type &)>::type result_type;

            result_type operator ()(S &s) const
            {
                return range_run_storage::pre_value(s.data());
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct post_run<S, sequence::impl::shifted_series_tag>
        {
            typedef typename S::storage_type storage_type;
            typedef typename result_of<op::post_run(storage_type &)>::type result_type;
            typedef typename concepts::Run<result_type>::offset_type offset_type;

            result_type operator ()(S &s) const
            {
                result_type post(range_run_storage::post_run(s.data()));
                if(rrs::empty(post))
                    return post;

                offset_type start(range_run_storage::offset(post));
                offset_type stop(range_run_storage::end_offset(post));

                if(-inf != start)
                    start += s.shift();
                if(inf != stop)
                    stop += s.shift();

                return result_type(start, stop);
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct post_value<S, sequence::impl::shifted_series_tag>
        {
            typedef typename S::storage_type storage_type;
            typedef typename result_of<op::post_value(storage_type &)>::type result_type;

            result_type operator ()(S &s) const
            {
                return range_run_storage::post_value(s.data());
            }
        };

    }}
}

#endif
