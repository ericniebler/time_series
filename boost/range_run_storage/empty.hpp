// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_EMPTY_EAN_2006_08_15_HPP
# define BOOST_RANGE_RUN_STORAGE_EMPTY_EAN_2006_08_15_HPP

# include <boost/detail/function1.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/range_run_storage/tag.hpp>
# include <boost/range_run_storage/offset.hpp>
# include <boost/range_run_storage/end_offset.hpp>
# include <boost/mpl/placeholders.hpp>

namespace boost { namespace range_run_storage
{

/// INTERNAL ONLY
namespace impl
{
    template<typename R, typename T = typename range_run_storage::impl::tag<R>::type>
    struct empty
    {
        typedef bool result_type;

        result_type operator ()(R &r) const
        {
            return range_run_storage::offset(r) == range_run_storage::end_offset(r);
        }
    };
}

namespace op
{
    using mpl::_;

    struct empty
      : boost::detail::function1<impl::empty<_, range_run_storage::impl::tag<_> > >
    {};
}

namespace
{
    op::empty const &empty = boost::detail::pod_singleton<op::empty>::instance;
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_STORAGE_EMPTY_EAN_2006_08_15_HPP
