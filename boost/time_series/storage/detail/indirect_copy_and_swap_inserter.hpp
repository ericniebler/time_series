///////////////////////////////////////////////////////////////////////////////
/// \file indirect_copy_and_swap_inserter.hpp
/// An index property map with an integer offset
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_DETAIL_INDIRECT_COPY_AND_SWAP_INSERTER_HPP_EAN_06_17_2006
#define BOOST_TIME_SERIES_DETAIL_INDIRECT_COPY_AND_SWAP_INSERTER_HPP_EAN_06_17_2006

#include <boost/intrusive_ptr.hpp>
#include <boost/sequence/tag.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/set_at.hpp>
#include <boost/range_run_storage/ordered_inserter.hpp>
#include <boost/range_run_storage/commit.hpp>
#include <boost/time_series/detail/ref_counted_object.hpp>
#include <boost/detail/construct.hpp>

namespace boost { namespace time_series { namespace storage
{
    // An inserter that simply calls set_at on an internal
    // temporary series, and then calls swap() in commit().
    template<typename Series>
    struct indirect_copy_and_swap_inserter
    {
        // For result_of support
        typedef void result_type;

        indirect_copy_and_swap_inserter(Series &series)
          : array_(series)
          , temp_(new time_series::detail::ref_counted_object<Series>(
                constructors::construct<Series>(
                    time_series::zero = range_run_storage::zero(series)
                )
            ))
        {}

        template<typename R, typename V>
        void set_at(R &r, V &v)
        {
            range_run_storage::set_at(this->temp_->get(), r, v);
        }

        void commit()
        {
            using std::swap;
            swap(this->array_, this->temp_->get());
        }

    private:
        Series &array_;
        intrusive_ptr<time_series::detail::ref_counted_object<Series> > temp_;
    };
}}}

namespace boost { namespace sequence { namespace impl
{
    struct indirect_copy_and_swap_inserter_tag;

    template<typename Series>
    struct tag<time_series::storage::indirect_copy_and_swap_inserter<Series> >
    {
        typedef indirect_copy_and_swap_inserter_tag type;
    };
}}}

namespace boost { namespace range_run_storage { namespace impl
{
    template<typename S, typename R, typename V>
    struct set_at<S, R, V, sequence::impl::indirect_copy_and_swap_inserter_tag>
    {
        typedef void result_type;

        void operator ()(S &s, R &r, V &v) const
        {
            s.set_at(r, v);
        }
    };

    template<typename S>
    struct commit<S, sequence::impl::indirect_copy_and_swap_inserter_tag>
    {
        typedef void result_type;

        void operator ()(S &s) const
        {
            s.commit();
        }
    };
}}}

#endif // BOOST_TIME_SERIES_DETAIL_INDIRECT_COPY_AND_SWAP_INSERTER_HPP_EAN_06_17_2006
