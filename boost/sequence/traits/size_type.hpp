// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SEQUENCE_TRAITS_SIZE_TYPE_DWA200658_HPP
# define BOOST_SEQUENCE_TRAITS_SIZE_TYPE_DWA200658_HPP

# include <boost/utility/result_of.hpp>
# include <boost/sequence/size.hpp>
# include <boost/type_traits/add_reference.hpp>

namespace boost { namespace sequence { namespace traits {

// Don't specialize this.
template<typename S>
struct size_type
  : result_of<
        op::size(typename add_reference<S>::type)
    >
{};

}}} // namespace boost::sequence::traits

#endif // BOOST_SEQUENCE_TRAITS_SIZE_TYPE_DWA200658_HPP
