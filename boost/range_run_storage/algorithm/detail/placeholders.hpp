///////////////////////////////////////////////////////////////////////////////
/// \file placeholders.hpp
/// dummy args for range_run_storage algorithms
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_PLACEHOLDERS_EAN_08_04_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_ALGORITHM_DETAIL_PLACEHOLDERS_EAN_08_04_2006_HPP

namespace boost { namespace range_run_storage
{
    namespace rrs = range_run_storage;

    namespace placeholders_
    {
        // skip placeholder
        struct skip_ {};
        skip_ const skip = {};

        // poor-man's lambda::_1 that correctly handles non-const rvalues
        struct first_
        {
            template<typename R>
            struct result;

            template<typename This, typename Arg1>
            struct result<This(Arg1)>
            {
                typedef Arg1 type;
            };

            template<typename This, typename Arg1, typename Arg2>
            struct result<This(Arg1, Arg2)>
            {
                typedef Arg1 type;
            };

            template<typename Arg1>
            Arg1 &operator ()(Arg1 &arg1) const
            {
                return arg1;
            }

            template<typename Arg1>
            Arg1 const &operator ()(Arg1 const &arg1) const
            {
                return arg1;
            }

            template<typename Arg1, typename Arg2>
            Arg1 &operator ()(Arg1 &arg1, Arg2 const &) const
            {
                return arg1;
            }

            template<typename Arg1, typename Arg2>
            Arg1 const &operator ()(Arg1 const &arg1, Arg2 const &) const
            {
                return arg1;
            }
        };

        first_ const _1 = {};

    } // namespace placeholders_

    using placeholders_::skip;
    using placeholders_::_1;

}}

#endif
