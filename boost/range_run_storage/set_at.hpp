// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_SET_AT_DWA2006514_HPP
# define BOOST_RANGE_RUN_STORAGE_SET_AT_DWA2006514_HPP

# include <boost/mpl/assert.hpp>
# include <boost/detail/function3.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/mpl/placeholders.hpp>
# include <boost/sequence/tag.hpp>
# include <boost/sequence/concepts.hpp>
# include <boost/sequence/composed.hpp>
# include <boost/range_run_storage/offset.hpp>


// There is a problem with the interface for
// set_at. It could cause cursor invalidation,
// but set_at doesn't return a cursor to the
// set element. This makes it impossible to
// implement a generic algorithm that performs
// an in-place modification of a series.


namespace boost { namespace range_run_storage
{

namespace concepts
{
    template<typename R>
    struct Run;
}

/// INTERNAL ONLY
namespace impl
{
    template<typename S, typename R, typename V, typename T = typename sequence::impl::tag<S>::type>
    struct set_at
    {
        typedef void result_type;

        result_type operator ()(S &s, R &r, V &v) const
        {
            s.set_at(r, v);
        }
    };

    template<typename S, typename R, typename V>
    struct set_at<S, R, V, sequence::impl::composed_tag>
    {
        typedef void result_type;

        result_type operator ()(S &s, R &r, V &v) const
        {
            BOOST_MPL_ASSERT((typename concepts::Run<R>::is_unit_length));
            sequence::elements(s)(*(sequence::begin(s) + range_run_storage::offset(r)), v);
        }
    };
}

namespace op
{
    using mpl::_;

    struct set_at
      : boost::detail::function3<impl::set_at<_, _, _, sequence::impl::tag<_> > >
    {};
}

namespace
{
    op::set_at const &set_at = boost::detail::pod_singleton<op::set_at>::instance;
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_STORAGE_SET_AT_DWA2006514_HPP
