// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_PROPERTY_MAP_DEREFERENCE_DWA200655_HPP
# define BOOST_PROPERTY_MAP_DEREFERENCE_DWA200655_HPP

# include <boost/iterator/iterator_concepts.hpp>
# include <boost/concept/requires.hpp>

namespace boost { namespace property_maps
{

using namespace boost_concepts;

struct dereference
{
    // For result_of support
    template<typename K>
    struct result;

    template<typename This, typename I>
    struct result<This(I)>
    {
        typedef typename remove_cv<
           typename remove_reference<I>::type
        >::type iter;

        typedef typename ReadableIterator<iter>::reference type;
    };

    template<typename This, typename K, typename V>
    struct result<This(K, V)>
    {
        typedef void type;
    };

    template<typename I>
    BOOST_CONCEPT_REQUIRES(
        ((ReadableIterator<I>)),

    (typename ReadableIterator<I>::reference))
    operator ()(I const &i) const
    {
        return *i;
    }

    template<typename I, typename V>
    BOOST_CONCEPT_REQUIRES(
        ((WritableIterator<I>)),

    (void))
    operator ()(I const &i, V const &v) const
    {
        *i = v;
    }


    template<typename I, typename V>
    BOOST_CONCEPT_REQUIRES(
        ((WritableIterator<I>)),

    (void))
    operator ()(I const &i, V &v) const
    {
        *i = v;
    }
};

}} // namespace boost::property_maps

#endif // BOOST_PROPERTY_MAP_DEREFERENCE_DWA200655_HPP
