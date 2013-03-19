// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SEQUENCE_DETAIL_MUTABLE_INDEX_TYPE_DWA200658_HPP
# define BOOST_SEQUENCE_DETAIL_MUTABLE_INDEX_TYPE_DWA200658_HPP

# include <boost/detail/is_incrementable.hpp>
# include <boost/mpl/identity.hpp>

namespace boost { namespace sequence { namespace detail {

template<typename T>
struct nested_value_type
{
    typedef typename T::value_type type;
};

template<typename T>
struct mutable_index_type
  : mpl::eval_if<
        boost::detail::is_incrementable<T>
      , mpl::identity<T>
      , nested_value_type<T>
    >
{};

}}} // namespace boost::sequence::detail

#endif // BOOST_SEQUENCE_DETAIL_MUTABLE_INDEX_TYPE_DWA200658_HPP
