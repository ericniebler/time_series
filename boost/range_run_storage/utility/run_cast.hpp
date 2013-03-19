///////////////////////////////////////////////////////////////////////////////
/// \file run_cast.hpp
/// for casting between run types
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_UTILITY_RUN_CAST_EAN_07_27_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_UTILITY_RUN_CAST_EAN_07_27_2006_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/range_run_storage/offset.hpp>
#include <boost/range_run_storage/end_offset.hpp>

namespace boost { namespace range_run_storage
{
    namespace rrs = range_run_storage;

    template<typename To, typename From>
    typename enable_if<is_same<To, From>, To const &>::type
    run_cast(From const &from)
    {
        return from;
    }

    template<typename To, typename From>
    typename disable_if<is_same<To, From>, To>::type
    run_cast(From const &from)
    {
        return To(rrs::offset(from), rrs::end_offset(from));
    }

}}

#endif
