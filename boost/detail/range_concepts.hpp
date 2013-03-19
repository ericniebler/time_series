// Copyright Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_CONCEPTS_EAN_2006_10_12_HPP
# define BOOST_RANGE_CONCEPTS_EAN_2006_10_12_HPP

# include <boost/version.hpp>
# include <boost/assert.hpp>
# include <boost/iterator/iterator_concepts.hpp>
# include <boost/iterator/iterator_traits.hpp>
# include <boost/concept_check.hpp>
# include <boost/range/functions.hpp>
# include <boost/range/metafunctions.hpp>

namespace boost_concepts
{

/// INTERNAL ONLY
template<typename S>
inline S &lvalue_of()
{
    BOOST_ASSERT(false);
    throw "should never get here";
}

template<typename Range>
struct SinglePassRange
{
    typedef typename boost::range_value<Range>::type value_type;
    typedef typename boost::range_result_iterator<Range>::type iterator;

    BOOST_CONCEPT_ASSERT((SinglePassIterator<iterator>));

    ~SinglePassRange()
    {
        Range & rng = lvalue_of<Range>();
        iterator i = boost::begin(rng);
        iterator j = boost::end(rng);
        bool b = boost::empty(rng);
        value_type const &v = *i;

        boost::ignore_unused_variable_warning(i);
        boost::ignore_unused_variable_warning(j);
        boost::ignore_unused_variable_warning(b);
        boost::ignore_unused_variable_warning(v);
    }
};

template<typename Range>
struct ForwardRange
  : SinglePassRange<Range>
{
    typedef typename boost::range_size<Range>::type size_type;
    typedef typename boost::range_difference<Range>::type difference_type;

    BOOST_CONCEPT_ASSERT((boost::UnsignedInteger<size_type>));
    BOOST_CONCEPT_ASSERT((boost::SignedInteger<difference_type>));
    BOOST_CONCEPT_ASSERT((ForwardTraversal<typename ForwardRange::iterator>));

    ~ForwardRange()
    {
        Range & rng = lvalue_of<Range>();
        size_type s = boost::size(rng);

        boost::ignore_unused_variable_warning(s);
    }
};

template<typename Range>
struct BidirectionalRange
  : ForwardRange<Range>
{
    typedef typename boost::range_reverse_result_iterator<Range>::type reverse_iterator;

    BOOST_CONCEPT_ASSERT((BidirectionalTraversal<typename BidirectionalRange::iterator>));

    ~BidirectionalRange()
    {
        Range & rng = lvalue_of<Range>();
        reverse_iterator i = boost::rbegin(rng);
        reverse_iterator j = boost::rend(rng);

        boost::ignore_unused_variable_warning(i);
        boost::ignore_unused_variable_warning(j);
    }
};

template<typename Range>
struct RandomAccessRange
  : BidirectionalRange<Range>
{
    BOOST_CONCEPT_ASSERT((RandomAccessTraversal<typename RandomAccessRange::iterator>));
};

} // namespace boost_concepts

#endif // BOOST_RANGE_CONCEPTS_EAN_2006_10_12_HPP
