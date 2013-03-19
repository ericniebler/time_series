// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_COMMIT_EAN20060523_HPP
# define BOOST_RANGE_RUN_STORAGE_COMMIT_EAN20060523_HPP

# include <boost/detail/function1.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/mpl/placeholders.hpp>

namespace boost { namespace range_run_storage
{

/// INTERNAL ONLY
namespace impl
{
    template<typename S, typename T = typename sequence::impl::tag<S>::type>
    struct commit
    {
        typedef void result_type;

        result_type operator ()(S &s) const
        {}
    };
}

namespace op
{
    using mpl::_;

    struct commit
      : boost::detail::function1<impl::commit<_, sequence::impl::tag<_> > >
    {};
}

namespace
{
    op::commit const &commit = boost::detail::pod_singleton<op::commit>::instance;
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_STORAGE_COMMIT_EAN20060523_HPP
