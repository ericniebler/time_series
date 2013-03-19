// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_ORDERED_INSERTER_EAN20060523_HPP
# define BOOST_RANGE_RUN_STORAGE_ORDERED_INSERTER_EAN20060523_HPP

# include <boost/detail/function1.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/mpl/placeholders.hpp>

namespace boost { namespace range_run_storage
{

/// INTERNAL ONLY
namespace impl
{
    template<typename S, typename T = typename sequence::impl::tag<S>::type>
    struct ordered_inserter
    {
        typedef S &result_type;

        result_type operator ()(S &s) const
        {
            return s;
        }
    };
}

namespace op
{
    using mpl::_;

    struct ordered_inserter
      : boost::detail::function1<impl::ordered_inserter<_, sequence::impl::tag<_> > >
    {};
}

namespace
{
    op::ordered_inserter const &ordered_inserter = boost::detail::pod_singleton<op::ordered_inserter>::instance;
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_STORAGE_ORDERED_INSERTER_EAN20060523_HPP
