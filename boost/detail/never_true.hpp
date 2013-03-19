// Copyright Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAIL_NEVER_TRUE_EAN_2006_10_15_HPP
#define BOOST_DETAIL_NEVER_TRUE_EAN_2006_10_15_HPP

#include <boost/mpl/bool.hpp>

namespace boost { namespace detail
{
    template<typename T>
    struct never_true
      : mpl::false_
    {};
}}

#endif
