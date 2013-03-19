// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_END_OFFSET_EAN2006717_HPP
# define BOOST_RANGE_RUN_STORAGE_END_OFFSET_EAN2006717_HPP

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
    struct end_offset
    {
        typedef typename R::second_type result_type;
        result_type operator ()(R &r) const
        {
            return r.second;
        }
    };

    template<typename R>
    struct end_offset< R, compressed_pair_tag >
    {
        typedef typename R::second_type result_type;
        result_type operator ()(R &r) const
        {
            return r.second();
        }
    };
}

namespace op
{
    using mpl::_;

    struct end_offset
      : boost::detail::function1<impl::end_offset<_, range_run_storage::impl::tag<_> > >
    {};
}

namespace
{
    op::end_offset const &end_offset = boost::detail::pod_singleton<op::end_offset>::instance;
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_STORAGE_END_OFFSET_EAN2006717_HPP
