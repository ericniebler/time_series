// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_GET_AT_DWA2006514_HPP
# define BOOST_RANGE_RUN_STORAGE_GET_AT_DWA2006514_HPP

# include <boost/detail/function2.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/sequence/tag.hpp>
# include <boost/sequence/concepts.hpp>
# include <boost/sequence/composed.hpp>
# include <boost/mpl/placeholders.hpp>

namespace boost { namespace range_run_storage
{

/// INTERNAL ONLY
namespace impl
{
    template<typename S, typename I, typename T = typename sequence::impl::tag<S>::type>
    struct get_at
    {
        typedef typename sequence::concepts::Sequence<S>::reference result_type;
        result_type operator ()(S &s, I &i) const
        {
            return s[i];
        }
    };

    template<typename S, typename I>
    struct get_at< S, I, sequence::impl::composed_tag >
    {
        typedef typename sequence::concepts::Sequence<S>::reference result_type;
        result_type operator ()(S &s, I &i) const
        {
            return sequence::elements(s)(*(sequence::begin(s) + i));
        }
    };
}

namespace op
{
    using mpl::_;

    struct get_at
      : boost::detail::function2<impl::get_at<_, _, sequence::impl::tag<_> > >
    {};
}

namespace
{
    op::get_at const &get_at = boost::detail::pod_singleton<op::get_at>::instance;
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_STORAGE_GET_AT_DWA2006514_HPP
