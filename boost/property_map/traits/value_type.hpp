// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_PROPERTY_MAP_TRAITS_VALUE_TYPE_DWA200655_HPP
# define BOOST_PROPERTY_MAP_TRAITS_VALUE_TYPE_DWA200655_HPP

# include <boost/iterator/iterator_traits.hpp>
# include <boost/type_traits/remove_reference.hpp>
# include <boost/type_traits/remove_cv.hpp>
# include <boost/utility/result_of.hpp>

namespace boost { namespace property_maps { namespace traits {

template<typename F, typename I>
struct value_type
  : remove_cv<
        typename remove_reference<
            typename result_of<
                F(typename iterator_reference<I>::type)
            >::type
        >::type
    >
{};

}}} // namespace boost::property_maps::traits

#endif // BOOST_PROPERTY_MAP_TRAITS_VALUE_TYPE_DWA200655_HPP
