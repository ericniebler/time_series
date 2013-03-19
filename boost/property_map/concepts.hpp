// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_PROPERTY_MAP_CONCEPTS_DWA2006430_HPP
# define BOOST_PROPERTY_MAP_CONCEPTS_DWA2006430_HPP

# include <boost/concept_check.hpp>
# include <boost/iterator/iterator_concepts.hpp>
# include <boost/iterator/iterator_traits.hpp>
# include <boost/property_map/traits/value_type.hpp>

namespace boost { namespace property_maps { namespace concepts
{

using namespace boost_concepts;

// Just a repository for associated types.  We'll never use this
// concept directly.
template<typename Func, typename Iter>
struct BasicPropertyMap
{
    typedef Iter cursor;
    typedef Func elements;
    typedef typename ReadableIterator<Iter>::reference key_type;
    typedef typename traits::value_type<Func, Iter>::type value_type;

    BOOST_CONCEPT_ASSERT((IncrementableIterator<Iter>));
    BOOST_CONCEPT_ASSERT((ReadableIterator<Iter>));
};

template<typename UnaryFunc, typename Iter>
struct ReadablePropertyMap
  : BasicPropertyMap<UnaryFunc, Iter>
{
    // Associated types
    typedef typename result_of<
        UnaryFunc(typename ReadablePropertyMap::key_type)
    >::type reference;

    BOOST_CONCEPT_ASSERT((
        UnaryFunction<
            UnaryFunc, reference, typename ReadablePropertyMap::key_type>));

    BOOST_CONCEPT_ASSERT((
        Convertible<
            typename ReadablePropertyMap::reference
          , typename ReadablePropertyMap::value_type
        >));
};

template<typename BinaryFunc, typename Iter, typename Value = typename iterator_value<Iter>::type>
struct WritablePropertyMap
  : BasicPropertyMap<BinaryFunc, Iter>
{
    // Associated types
    typedef Value value_type;

    BOOST_CONCEPT_ASSERT((
        BinaryFunction<
            BinaryFunc
          , void
          , typename WritablePropertyMap::key_type, value_type const&>));
};

template<typename UnaryBinaryFunc, typename Iter>
struct ReadWritePropertyMap
  : WritablePropertyMap<
        UnaryBinaryFunc
      , Iter
      , typename ReadablePropertyMap<UnaryBinaryFunc, Iter>::value_type
    >
  , ReadablePropertyMap<UnaryBinaryFunc, Iter>
{};

}}} // namespace boost::property_maps::concepts

#endif // BOOST_PROPERTY_MAP_CONCEPTS_DWA2006430_HPP
