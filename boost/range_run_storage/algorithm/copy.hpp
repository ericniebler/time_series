///////////////////////////////////////////////////////////////////////////////
/// \file copy.hpp
/// Copies one \c InfiniteRangeRunStorage to an \c OrderedInserter.
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_ALGORITHM_COPY_EAN_08_04_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_ALGORITHM_COPY_EAN_08_04_2006_HPP

#include <boost/range_run_storage/algorithm/detail/placeholders.hpp>
#include <boost/range_run_storage/algorithm/detail/transform.hpp>

namespace boost { namespace range_run_storage
{
    namespace rrs = range_run_storage;

    /// \brief Copies the runs and values from an \c InfiniteRangeRunStorage
    /// to an \c OrderedInserter.
    ///
    /// Copies the runs and values from an \c InfiniteRangeRunStorage
    /// to an \c OrderedInserter. Complexity is O(N).
    ///
    /// \param[in] in An \c InfiniteRangeRunStorage from which to copy elements
    ///     and values.
    /// \param[out] out An \c OrderedInserter to receive the elements and values
    /// \return A reference to \c out.
    /// \pre \c In is a model of \c InfiniteRangeRunStorage.
    /// \pre \c Out is a model of \c OrderedInserter.
    /// \attention You must call <tt>range_run_storage::commit()</tt> on the
    ///     return value to commit the changes.
    template<typename In, typename Out>
    Out &copy(In const &in, Out &out)
    {
        return detail::transform_impl(in, rrs::_1, out);
    }

}}

#endif
