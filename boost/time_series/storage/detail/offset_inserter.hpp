///////////////////////////////////////////////////////////////////////////////
/// \file offset_inserter.hpp
/// An index property map with an integer offset
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_DETAIL_OFFSET_INSERTER_HPP_EAN_04_26_2006
#define BOOST_TIME_SERIES_DETAIL_OFFSET_INSERTER_HPP_EAN_04_26_2006

#include <boost/call_traits.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/sequence/tag.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/ordered_inserter.hpp>
#include <boost/range_run_storage/commit.hpp>

namespace boost { namespace time_series { namespace storage
{
    // wraps a inserter property map and
    // an offset.
    template<typename Inserter, typename Offset>
    struct offset_inserter
    {
        // For result_of support
        typedef void result_type;

        offset_inserter(typename call_traits<Inserter>::param_type inserter, Offset offset)
          : inserter_(inserter)
          , offset_(offset)
        {}

        template<typename R, typename V>
        void set_at(R &r, V &v)
        {
            R tmp(
                range_run_storage::offset(r) - this->offset_
              , range_run_storage::end_offset(r) - this->offset_
            );
            range_run_storage::set_at(this->inserter_, tmp, v);
        }

        void commit()
        {
            range_run_storage::commit(this->inserter_);
        }

    private:
        Inserter inserter_;
        Offset offset_;
    };
}}}

namespace boost { namespace sequence { namespace impl
{
    struct offset_inserter_tag;

    template<typename Inserter, typename Offset>
    struct tag<time_series::storage::offset_inserter<Inserter, Offset> >
    {
        typedef offset_inserter_tag type;
    };
}}}

namespace boost { namespace range_run_storage { namespace impl
{
    template<typename S, typename R, typename V>
    struct set_at<S, R, V, sequence::impl::offset_inserter_tag>
    {
        typedef void result_type;

        void operator ()(S &s, R &r, V &v) const
        {
            s.set_at(r, v);
        }
    };

    template<typename S>
    struct commit<S, sequence::impl::offset_inserter_tag>
    {
        typedef void result_type;

        void operator ()(S &s) const
        {
            s.commit();
        }
    };
}}}

#endif
