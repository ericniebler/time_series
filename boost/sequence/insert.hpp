// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SEQUENCE_INSERT_DWA200655_HPP
# define BOOST_SEQUENCE_INSERT_DWA200655_HPP

# include <utility>
# include <boost/detail/function4.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/sequence/tag.hpp>
# include <boost/mpl/placeholders.hpp>

namespace boost { namespace sequence
{

/// INTERNAL ONLY
namespace impl
{
    template<typename S, typename C, typename I, typename V, typename T = typename tag<S>::type>
    struct insert
    {
        typedef void result_type;

        void operator ()(S &s, C &c, I &i, V &v) const
        {
            s.insert(*c, std::make_pair(i, v));
        }
    };
}

namespace op
{
    using mpl::_;

    struct insert
      : boost::detail::function4<impl::insert<_, _, _, _, impl::tag<_> > >
    {};
}

namespace
{
    op::insert const &insert = boost::detail::pod_singleton<op::insert>::instance;
}

}} // namespace boost::sequence

#endif // BOOST_SEQUENCE_BEGIN_DWA200655_HPP
