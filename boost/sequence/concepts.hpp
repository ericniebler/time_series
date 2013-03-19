// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SEQUENCE_CONCEPTS_DWA200652_HPP
# define BOOST_SEQUENCE_CONCEPTS_DWA200652_HPP

# include <boost/property_map/concepts.hpp>
# include <boost/sequence/begin.hpp>
# include <boost/sequence/end.hpp>
# include <boost/sequence/elements.hpp>
# include <boost/type_traits/add_reference.hpp>
# include <boost/sequence/traits/size_type.hpp>
# include <boost/sequence/traits/index_type.hpp>

namespace boost { namespace sequence { namespace concepts
{

using namespace boost_concepts;
using namespace boost::property_maps::concepts;

template<typename S>
struct Sequence
  : ReadablePropertyMap<
        typename result_of<
            // Note that we *must* add_reference to S because it might
            // be an array type that would otherwise decay into a
            // pointer.
            op::elements(typename add_reference<S>::type)
        >::type
      , typename result_of<
            op::begin(typename add_reference<S>::type)
        >::type
    >
{
    // Associated types cursor, elements, key_type, value_type,
    // and reference, all come from ReadablePropertyMap

    // The end cursor doesn't have to have the same type as the begin
    // cursor, just as long as you can compare them.
    typedef typename result_of<
        op::end(typename add_reference<S>::type)
    >::type end_cursor;

    // This isn't quite the right requirement because it imposes
    // convertibility, but it's good enough for a first approximation.
    BOOST_CONCEPT_ASSERT((
        InteroperableIterator<typename Sequence::cursor, end_cursor>));

    ~Sequence()
    {
        typename Sequence::elements elts = boost::sequence::elements(s);
        typename Sequence::cursor c = boost::sequence::begin(s);
        end_cursor end = boost::sequence::end(s);

        ignore_unused_variable_warning(elts);
        ignore_unused_variable_warning(c);
        ignore_unused_variable_warning(end);
    }
 private:
    Sequence(); // satisfying some older GCCs
    S s;
};

template<typename S>
struct O1SizeSequence
  : Sequence<S>
{
    // A type representing the size of the sequence.
    typedef typename sequence::traits::size_type<S>::type size_type;

    // A type that can act as an index into the sequence.  Because
    // size_type may be a wrapper for a compile-time constant,
    // e.g. mpl::integral_c<std::ptrdiff_t, N>, we need a distinct type in order to be
    // able to count at runtime.
    typedef typename sequence::traits::index_type<S>::type index_type;

    ~O1SizeSequence()
    {
        size_type size = sequence::size(s);

        BOOST_CONCEPT_ASSERT((Convertible<size_type, index_type>));

        index_type i = sequence::size(s);

        // Need EqualityComparable2
        i == size;
        i != size;
        size == i;
        size != i;
    }
 private:
    S s;
};

template<typename S>
struct LvalueSequence
  : Sequence<S>
{
    BOOST_CONCEPT_ASSERT((LvalueIterator<typename LvalueSequence::cursor>));
};

template<typename S>
struct SinglePassSequence
  : Sequence<S>
{
    BOOST_CONCEPT_ASSERT((SinglePassIterator<typename SinglePassSequence::cursor>));
};

template<typename S>
struct ForwardSequence
  : SinglePassSequence<S>
{
    BOOST_CONCEPT_ASSERT((ForwardTraversal<typename ForwardSequence::cursor>));
};

template<typename S>
struct BidirectionalSequence
  : ForwardSequence<S>
{
    BOOST_CONCEPT_ASSERT((BidirectionalTraversal<typename BidirectionalSequence::cursor>));
};

template<typename S>
struct RandomAccessSequence
  : BidirectionalSequence<S>
{
    BOOST_CONCEPT_ASSERT((RandomAccessTraversal<typename RandomAccessSequence::cursor>));
};

template<typename S>
struct Mutable_Sequence
  : Sequence<S>
{
    BOOST_CONCEPT_ASSERT((
        ReadWritePropertyMap<
            typename Mutable_Sequence::elements
          , typename Mutable_Sequence::cursor>));
};


template<typename S>
struct Mutable_SinglePassSequence
  : SinglePassSequence<S>
  , Mutable_Sequence<S>
{};

template<typename S>
struct Mutable_ForwardSequence
  : Mutable_Sequence<S>
  , ForwardSequence<S>
{};

template<typename S>
struct Mutable_BidirectionalSequence
  : Mutable_Sequence<S>
  , BidirectionalSequence<S>
{};

template<typename S>
struct Mutable_RandomAccessSequence
  : Mutable_Sequence<S>
  , RandomAccessSequence<S>
{};

}}} // namespace boost::sequence::concepts

#endif // BOOST_SEQUENCE_CONCEPTS_DWA200652_HPP
