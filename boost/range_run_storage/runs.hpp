// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_RANGES_DWA200658_HPP
# define BOOST_RANGE_RUN_STORAGE_RANGES_DWA200658_HPP

# include <boost/range_run_storage/difference1.hpp>
# include <boost/range_run_storage/address_difference1.hpp>
# include <boost/detail/is_xxx.hpp>
# include <boost/iterator/counting_iterator.hpp>
# include <boost/iterator/iterator_traits.hpp>

# include <boost/sequence/concepts.hpp>
# include <boost/detail/function1.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/utility/enable_if.hpp>
# include <boost/mpl/and.hpp>
# include <boost/type_traits/is_convertible.hpp>
# include <boost/type_traits/is_pointer.hpp>

# include <boost/mpl/placeholders.hpp>

namespace boost { namespace range_run_storage
{

namespace rrs_detail_
{
    BOOST_DETAIL_IS_XXX_DEF(counting_iterator, boost::counting_iterator, 3)
}

/// INTERNAL ONLY
namespace impl
{
    template<typename S, typename enable = void>
    struct runs_base;

    template<typename S, typename T = typename sequence::impl::tag<S>::type>
    struct runs
      : runs_base<S>
    {};

    // By default, every Boost.Range is a sequence whose cursors are
    // counting_iterator<RandomAccessIterator>, so we'll provide a 
    // default implementation of runs that works for that case.
    template<typename S>
    struct runs_base<
        S
      , typename enable_if<
            mpl::and_<
                rrs_detail_::is_counting_iterator<
                    typename sequence::concepts::Sequence<S>::cursor
                >
              , is_convertible<
                    typename iterator_traversal<
                        typename sequence::concepts::Sequence<S>::cursor
                    >::type
                  , random_access_traversal_tag
                >
            >
        >::type
    >
    {
        typedef difference1<
            typename sequence::concepts::Sequence<S>::cursor
        > result_type;

        result_type operator ()(S &s) const
        {
            return result_type(sequence::begin(s));
        }
    };

    // Sequences whose cursors are pointers can also have a default
    // runs implementation
    template<typename S>
    struct runs_base<
        S
      , typename enable_if<
            is_pointer<
                typename sequence::concepts::Sequence<S>::cursor
            >
        >::type
    >
    {
        typedef address_difference1<
            typename sequence::concepts::Sequence<S>::cursor
        > result_type;

        result_type operator ()(S &s) const
        {
            return result_type(sequence::begin(s));
        }
    };
}

namespace op
{
    using mpl::_;

    struct runs
      : boost::detail::function1<impl::runs<_, sequence::impl::tag<_> > >
    {};
}

namespace
{
    op::runs const &runs = boost::detail::pod_singleton<op::runs>::instance;
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_STORAGE_RANGES_DWA200658_HPP
