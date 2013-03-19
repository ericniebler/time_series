// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_OFFSET_DWA2006514_HPP
# define BOOST_RANGE_RUN_STORAGE_OFFSET_DWA2006514_HPP

# include <boost/detail/function1.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/range_run_storage/tag.hpp>
# include <boost/mpl/placeholders.hpp>

namespace boost { namespace range_run_storage
{

/// INTERNAL ONLY
namespace impl
{
    template<typename R, typename T = typename range_run_storage::impl::tag<R>::type>
    struct offset
    {
        typedef typename R::first_type result_type;
        result_type operator ()(R &r) const
        {
            return r.first;
        }
    };

    template<typename R>
    struct offset< R, compressed_pair_tag >
    {
        typedef typename R::first_type result_type;
        result_type operator ()(R &r) const
        {
            return r.first();
        }
    };
}

namespace op
{
    using mpl::_;

    struct offset
      : boost::detail::function1<impl::offset<_, range_run_storage::impl::tag<_> > >
    {};
}

namespace
{
    op::offset const &offset = boost::detail::pod_singleton<op::offset>::instance;
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_STORAGE_OFFSET_DWA2006514_HPP
