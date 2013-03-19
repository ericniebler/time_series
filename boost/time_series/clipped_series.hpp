///////////////////////////////////////////////////////////////////////////////
/// \file clipped_series.hpp
/// A clipped series that satisfies the \c InfiniteRangeRunStorage concept
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_CLIPPED_SERIES_EAN_07_06_2006
#define BOOST_TIME_SERIES_CLIPPED_SERIES_EAN_07_06_2006

#include <utility>
#include <algorithm>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/range_run_storage/algorithm/detail/runs_searching.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/traits/is_time_series.hpp>
#include <boost/time_series/traits/offset_type.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/traits/discretization_type.hpp>
#include <boost/time_series/utility/time_series_base.hpp>
#include <boost/time_series/storage/detail/clipped_runs.hpp>
#include <boost/time_series/utility/series_stl_iterator.hpp>

// A view of an array that has been clipped to the right or left
// by an offset.
namespace boost { namespace time_series
{
    // The clipped_series
    template<typename Series>
    struct clipped_series
      : time_series_base<clipped_series<Series> >
    {
        typedef Series storage_type;
        typedef concepts::TimeSeries<Series> series_concept;

        typedef typename series_concept::value_type value_type;
        typedef typename series_concept::cursor cursor;
        typedef typename series_concept::elements elements;
        typedef typename series_concept::runs runs;
        typedef typename series_concept::run_type run_type;
        typedef typename series_concept::offset_type offset_type;
        typedef typename series_concept::pre_run_type pre_run_type;
        typedef typename series_concept::post_run_type post_run_type;
        typedef typename series_concept::discretization_type discretization_type;

        typedef std::pair<offset_type, offset_type> period_type;

        typedef 
            typename result_of<
                range_run_storage::op::get_at(storage_type &, offset_type &)
            >::type
        get_at_type;

        typedef typename mpl::if_<
            is_reference<get_at_type>
          , value_type const &
          , value_type const
        >::type reference;

        typedef series_stl_iterator<clipped_series<Series> const> iterator;
        typedef series_stl_iterator<clipped_series<Series> const> const_iterator;

        clipped_series(Series &series, offset_type start, offset_type stop)
          : data_(series)
          , period_(start, stop)
          , range_(clipped_series::equal_range(series, start, stop))
        {}

        discretization_type discretization() const
        {
            return this->data().discretization();
        }

        reference operator [](offset_type n) const
        {
            if(n >= this->start() && n < this->stop())
            {
                return range_run_storage::get_at(this->data(), n);
            }
            return range_run_storage::zero(this->data());
        }

        iterator begin() const
        {
            return iterator(*this, this->sequence_begin(), this->start());
        }

        iterator end() const
        {
            return iterator(*this, this->sequence_end(), this->stop());
        }

        // TODO make these private
    //private:
        offset_type start() const
        {
            return range_run_storage::offset(this->period_);
        }

        offset_type stop() const
        {
            return range_run_storage::end_offset(this->period_);
        }

        storage_type &data() const
        {
            return this->data_;
        }

        cursor sequence_begin() const
        {
            return this->range_.first;
        }

        cursor sequence_end() const
        {
            return this->range_.second;
        }

    private:
        static std::pair<cursor, cursor> equal_range(Series &series, offset_type start, offset_type stop)
        {
            return range_run_storage::detail::run_equal_range(
                sequence::begin(series)
              , sequence::end(series)
              , range_run_storage::runs(series)
              , std::make_pair(start, stop)
            );
        }

        Series &data_;
        period_type period_;
        std::pair<cursor, cursor> range_;
    };

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Series>
        struct discretization_type<clipped_series<Series> >
          : discretization_type<Series>
        {};

        /// INTERNAL ONLY
        template<typename Series>
        struct offset_type<clipped_series<Series> >
          : offset_type<Series>
        {};

        /// INTERNAL ONLY
        template<typename Series>
        struct storage_category<clipped_series<Series> >
          : storage_category<Series>
        {};
    }
}}

namespace boost
{
    namespace sequence { namespace impl
    {
        /// INTERNAL ONLY
        struct clipped_series_tag;

        /// INTERNAL ONLY
        template<typename Series>
        struct tag<time_series::clipped_series<Series> >
        {
            typedef clipped_series_tag type;
        };

        /// INTERNAL ONLY
        template<typename S>
        struct elements<S, clipped_series_tag>
        {
            typedef typename S::elements result_type;

            result_type operator ()(S &s) const
            {
                return sequence::elements(s.data());
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct begin<S, clipped_series_tag>
        {
            typedef typename S::cursor result_type;

            result_type operator ()(S &s) const
            {
                return s.sequence_begin();
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct end<S, clipped_series_tag>
        {
            typedef typename S::cursor result_type;

            result_type operator ()(S &s) const
            {
                return s.sequence_end();
            }
        };
    }}

    namespace range_run_storage { namespace impl
    {
        /// INTERNAL ONLY
        template<typename S>
        struct runs<S, sequence::impl::clipped_series_tag>
        {
            typedef typename S::runs runs_type;
            typedef typename S::offset_type offset_type;
            typedef time_series::storage::clipped_runs<runs_type, offset_type> result_type;

            result_type operator ()(S &s) const
            {
                return result_type(range_run_storage::runs(s.data()), s.start(), s.stop());
            }
        };

        /// INTERNAL ONLY
        template<typename S, typename I>
        struct get_at<S, I, sequence::impl::clipped_series_tag>
        {
            typedef typename S::reference result_type;

            result_type operator ()(S &s, I &i) const
            {
                return s[i];
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct zero<S, sequence::impl::clipped_series_tag>
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
        struct pre_run<S, sequence::impl::clipped_series_tag>
        {
            typedef typename S::pre_run_type pre_run_type;
            typedef typename S::offset_type offset_type;
            typedef typename S::period_type result_type;

            result_type operator ()(S &s) const
            {
                pre_run_type run(range_run_storage::pre_run(s.data()));
                result_type result(
                    std::max<offset_type>(s.start(), range_run_storage::offset(run))
                  , std::min<offset_type>(s.stop(), range_run_storage::end_offset(run))
                );
                result.second = std::max<offset_type>(result.first, result.second);
                return result;
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct pre_value<S, sequence::impl::clipped_series_tag>
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
        struct post_run<S, sequence::impl::clipped_series_tag>
        {
            typedef typename S::post_run_type post_run_type;
            typedef typename S::offset_type offset_type;
            typedef typename S::period_type result_type;

            result_type operator ()(S &s) const
            {
                post_run_type run(range_run_storage::post_run(s.data()));
                result_type result(
                    std::max<offset_type>(s.start(), range_run_storage::offset(run))
                  , std::min<offset_type>(s.stop(), range_run_storage::end_offset(run))
                );
                result.first = std::min<offset_type>(result.first, result.second);
                return result;
            }
        };

        /// INTERNAL ONLY
        template<typename S>
        struct post_value<S, sequence::impl::clipped_series_tag>
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

namespace boost { namespace foreach
{
    /// INTERNAL ONLY
    template<typename T>
    struct is_lightweight_proxy;

    /// INTERNAL ONLY
    // This tells BOOST_FOREACH it can avoid the rvalue/lvalue detection stuff.
    template<typename Series>
    struct is_lightweight_proxy<time_series::clipped_series<Series> >
      : boost::mpl::true_
    {
    };
}}

#endif
