///////////////////////////////////////////////////////////////////////////////
/// \file first_last.hpp
/// Returns the first and last runs and values from an \c InfiniteRangeRunStorage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_ALGORITHM_FIRST_LAST_EAN_10_26_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_ALGORITHM_FIRST_LAST_EAN_10_26_2006_HPP

#include <utility>
#include <boost/assert.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/utility/run_cast.hpp>

namespace boost { namespace range_run_storage
{
    namespace seq = sequence;
    namespace rrs = range_run_storage;

    namespace detail
    {
        template<typename Offset>
        struct std_pair_run
        {
            typedef std::pair<Offset, Offset> type;
        };
    }

    /// \brief Returns the first run of an \c InfiniteRangeRunStorage.
    ///
    /// Returns the first \c Run of an \c InfiniteRangeRunStorage. If there are
    /// no runs in the \c InfinteRangeRunStorage, an empty run is returned.
    /// Complexity is O(1).
    ///
    /// \param[in] s The \c InfiniteRangeRunStorage.
    /// \return A \c Run which has the same offset and end offset as the 
    ///     first run in the \c InfiniteRangeRunStorage if one exists, or
    ///     and empty run if one doesn't.
    /// \pre \c In is a model of \c InfiniteRangeRunStorage.
    template<typename In>
    typename detail::std_pair_run<
        typename concepts::InfiniteRangeRunStorage<In const>::offset_type
    >::type
    first_run(In const &s)
    {
        typedef typename concepts::InfiniteRangeRunStorage<In const>::offset_type offset_type;
        typedef typename concepts::InfiniteRangeRunStorage<In const>::cursor cursor_type;
        typedef std::pair<offset_type, offset_type> run_type;

        run_type run(rrs::run_cast<run_type>(rrs::pre_run(s)));
        if(!rrs::empty(run))
        {
            return run;
        }

        cursor_type begin(seq::begin(s)), end(seq::end(s));
        if(begin != end)
        {
            return rrs::run_cast<run_type>(rrs::runs(s)(*begin));
        }

        // could be empty
        return rrs::run_cast<run_type>(rrs::post_run(s));
    }

    /// \brief Returns the value of the first run of an \c InfiniteRangeRunStorage.
    ///
    /// Returns the value of the first \c Run of an \c InfiniteRangeRunStorage.
    /// Complexity is O(1).
    ///
    /// \param[in] s The \c InfiniteRangeRunStorage.
    /// \return The value of the first run in the \c InfiniteRangeRunStorage.
    /// \pre \c In is a model of \c InfiniteRangeRunStorage.
    /// \pre \c s contains at least one run.
    template<typename In>
    typename concepts::InfiniteRangeRunStorage<In const>::reference
    first_value(In const &s)
    {
        typedef typename concepts::InfiniteRangeRunStorage<In const>::cursor cursor_type;

        if(!rrs::empty(rrs::pre_run(s)))
        {
            return rrs::pre_value(s);
        }

        cursor_type begin(seq::begin(s)), end(seq::end(s));
        if(begin != end)
        {
            return seq::elements(s)(*begin);
        }

        BOOST_ASSERT(!rrs::empty(rrs::post_run(s)));
        return rrs::post_value(s);
    }

    /// \brief Returns the last run of an \c InfiniteRangeRunStorage.
    ///
    /// Returns the last \c Run of an \c InfiniteRangeRunStorage. If there are
    /// no runs in the \c InfinteRangeRunStorage, an empty run is returned.
    /// Complexity is O(1).
    ///
    /// \param[in] s The \c InfiniteRangeRunStorage.
    /// \return A \c Run which has the same offset and end offset as the 
    ///     last run in the \c InfiniteRangeRunStorage if one exists, or
    ///     and empty run if one doesn't.
    /// \pre \c In is a model of \c InfiniteRangeRunStorage.
    /// \pre <tt>InfiniteRangeRunStorage<In const>::cursor</tt> is a model of
    ///     \c BidirectionalIterator.
    template<typename In>
    typename detail::std_pair_run<
        typename concepts::InfiniteRangeRunStorage<In const>::offset_type
    >::type
    last_run(In const &s)
    {
        typedef typename concepts::InfiniteRangeRunStorage<In const>::offset_type offset_type;
        typedef typename concepts::InfiniteRangeRunStorage<In const>::cursor cursor_type;
        typedef std::pair<offset_type, offset_type> run_type;

        run_type run(rrs::run_cast<run_type>(rrs::post_run(s)));
        if(!rrs::empty(run))
        {
            return run;
        }

        cursor_type begin(seq::begin(s)), end(seq::end(s));
        if(begin != end)
        {
            --end;
            return rrs::run_cast<run_type>(rrs::runs(s)(*end));
        }

        // could be empty
        return rrs::run_cast<run_type>(rrs::pre_run(s));
    }

    /// \brief Returns the value of the last \c Run of an \c InfiniteRangeRunStorage.
    ///
    /// Returns the value of the last \c Run of an \c InfiniteRangeRunStorage.
    /// Complexity is O(1).
    ///
    /// \param[in] s The \c InfiniteRangeRunStorage.
    /// \return The value of the last run in the \c InfiniteRangeRunStorage.
    /// \pre \c In is a model of \c InfiniteRangeRunStorage.
    /// \pre \c s contains at least one run.
    /// \pre <tt>InfiniteRangeRunStorage<In const>::cursor</tt> is a model of
    ///     \c BidirectionalIterator.
    template<typename In>
    typename concepts::InfiniteRangeRunStorage<In const>::reference
    last_value(In const &s)
    {
        typedef typename concepts::InfiniteRangeRunStorage<In const>::cursor cursor_type;

        if(!rrs::empty(rrs::post_run(s)))
        {
            return rrs::post_value(s);
        }

        cursor_type begin(seq::begin(s)), end(seq::end(s));
        if(begin != end)
        {
            --end;
            return seq::elements(s)(*end);
        }

        BOOST_ASSERT(!rrs::empty(rrs::pre_run(s)));
        return rrs::pre_value(s);
    }

}}

#endif
