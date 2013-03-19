///////////////////////////////////////////////////////////////////////////////
/// \file series_stl_iterator.hpp
/// An STL-compatible iterator over a range/run storage that
/// makes it look like a continuous sequence
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_DETAIL_ARRAY_ITERATOR_HPP_EAN_06_01_2006
#define BOOST_TIME_SERIES_DETAIL_ARRAY_ITERATOR_HPP_EAN_06_01_2006

#include <boost/ref.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/time_series/concepts.hpp>

namespace boost { namespace time_series
{

    namespace detail
    {
        namespace rrs = range_run_storage;

        template<typename Offset, typename Run>
        inline bool in_run(Offset offset, Run const &run)
        {
            return offset >= rrs::offset(run) && offset < rrs::end_offset(run);
        }
    }

    /// An STL-compatible iterator over a range/run storage that
    /// makes it look like a continuous sequence
    template<typename Series>
    struct series_stl_iterator
      : iterator_facade<
            series_stl_iterator<Series>
          , typename concepts::TimeSeries<Series>::value_type const
          , std::forward_iterator_tag
        >
    {
        typedef concepts::TimeSeries<Series> series_concept;

        typedef typename series_concept::value_type value_type;
        typedef typename series_concept::cursor cursor_type;
        typedef typename series_concept::offset_type offset_type;
        typedef typename series_concept::run_type run_type;
        typedef value_type const &reference;

        series_stl_iterator(Series &series, cursor_type cursor, offset_type offset)
          : series_(series)
          , cursor_(cursor)
          , offset_(offset)
        {}

    private:
        friend class boost::iterator_core_access;

        void increment()
        {
            ++this->offset_;

            if(this->cursor_ != sequence::end(this->series_.get()))
            {
                run_type run(range_run_storage::runs(this->series_.get())(*this->cursor_));

                if(this->offset_ >= range_run_storage::end_offset(run))
                {
                    ++this->cursor_;
                }
            }
        }

        bool equal(series_stl_iterator const &that) const
        {
            return this->offset_ == that.offset_;
        }

        reference dereference() const
        {
            if(detail::in_run(this->offset_, range_run_storage::pre_run(this->series_.get())))
            {
                return range_run_storage::pre_value(this->series_.get());
            }
            else if(this->cursor_ != sequence::end(this->series_.get()))
            {
                run_type run(range_run_storage::runs(this->series_.get())(*this->cursor_));

                if(detail::in_run(this->offset_, run))
                {
                    return sequence::elements(this->series_.get())(*this->cursor_);
                }
            }
            else if(detail::in_run(this->offset_, range_run_storage::post_run(this->series_.get())))
            {
                return range_run_storage::post_value(this->series_.get());
            }

            return range_run_storage::zero(this->series_.get());
        }

        reference_wrapper<Series> series_;
        cursor_type cursor_;
        offset_type offset_;
    };

}} // namespace boost::time_series

#endif
