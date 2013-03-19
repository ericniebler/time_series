// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SEQUENCE_TRAITS_INDEX_TYPE_DWA200658_HPP
# define BOOST_SEQUENCE_TRAITS_INDEX_TYPE_DWA200658_HPP

# include <boost/sequence/traits/size_type.hpp>
# include <boost/sequence/detail/mutable_index_type.hpp>

namespace boost { namespace sequence { namespace traits {

template<typename S>
struct index_type
  : detail::mutable_index_type<
        typename size_type<S>::type
    >
{};

}}} // namespace boost::sequence::traits

#endif // BOOST_SEQUENCE_TRAITS_INDEX_TYPE_DWA200658_HPP
