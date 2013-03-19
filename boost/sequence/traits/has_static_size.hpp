// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SEQUENCE_TRAITS_HAS_STATIC_SIZE_DWA200658_HPP
# define BOOST_SEQUENCE_TRAITS_HAS_STATIC_SIZE_DWA200658_HPP

# include <boost/mpl/not.hpp>
# include <boost/detail/is_incrementable.hpp>
# include <boost/type_traits/size_type.hpp>

namespace boost { namespace sequence { namespace traits {

template<typename S>
struct has_static_size
  : mpl::not_<
        typename boost::detail::is_incrementable<
            typename size_type<S>::type
        >::type
    >
{};

}}} // namespace boost::sequence::traits

#endif // BOOST_SEQUENCE_TRAITS_HAS_STATIC_SIZE_DWA200658_HPP
