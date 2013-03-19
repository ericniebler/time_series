///////////////////////////////////////////////////////////////////////////////
/// \file for_each.hpp
/// Invokes a \c TernaryFunction for each run in an \c InfiniteRangeRunStorage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_ALGORITHM_FOR_EACH_EAN_08_04_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_ALGORITHM_FOR_EACH_EAN_08_04_2006_HPP

#include <boost/range_run_storage/algorithm/detail/placeholders.hpp>
#include <boost/range_run_storage/algorithm/detail/transform.hpp>

namespace boost { namespace range_run_storage
{
    namespace rrs = range_run_storage;

    namespace detail
    {
        template<typename Fun>
        struct functor_inserter
        {
            functor_inserter(Fun const &fun)
              : fun(fun)
            {}

            template<typename Run, typename Value>
            void set_at(Run &run, Value &value)
            {
                this->fun(value, rrs::offset(run), rrs::end_offset(run));
            }

            Fun fun;
        };
    }

    /// \brief Invokes the specified \c TernaryFunction for each
    ///     (value, offset, end_offset) tuple in an \c InfiniteRangeRunStorage.
    ///
    /// Invokes the specified \c TernaryFunction for each
    ///     (value, offset, end_offset) tuple in an \c InfiniteRangeRunStorage.
    ///     The functor \c fun is invoked \e N times, where \e N is the number
    ///     of runs in the \c InfiniteRangeRunStorage.
    /// \param[in] in The input \c InfiniteRangeRunStorage.
    /// \param fun An \c TernaryFunction to be invoked with each
    ///     (value, offset, end_offset) tuple in the sequence.
    /// \return A copy of \c fun.
    /// \pre \c In is a model of \c InfiniteRangeRunStorage.
    /// \pre \c Fun is a model of \c TernaryFunction.
    template<typename In, typename Fun>
    Fun for_each(In &in, Fun fun)
    {
        detail::functor_inserter<Fun> out(fun);
        return detail::transform_impl(in, rrs::_1, out).fun;
    }

    /// \overload
    ///
    template<typename In, typename Fun>
    Fun for_each(In const &in, Fun fun)
    {
        detail::functor_inserter<Fun> out(fun);
        return detail::transform_impl(in, rrs::_1, out).fun;
    }

}}

#endif
