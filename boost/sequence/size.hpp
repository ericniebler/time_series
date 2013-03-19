// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SEQUENCE_SIZE_DWA200658_HPP
# define BOOST_SEQUENCE_SIZE_DWA200658_HPP

# include <boost/version.hpp>
# include <boost/detail/function1.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/mpl/placeholders.hpp>
# if BOOST_VERSION >= 103500
#  include <boost/range/size_type.hpp>
# endif
# include <boost/range/size.hpp>

namespace boost { namespace sequence
{

/// INTERNAL ONLY
namespace impl
{
    template<typename S, typename T = typename tag<S>::type>
    struct size
    {
        // For now, this will compile for some sequences that can't
        // provide size in O(1) (e.g. pairs of list iterators), and for
        // some non-sequences (e.g. char const*).  Enforcing this
        // semantic constraint is up to the user.  When Boost.Range is
        // fixed, it will fall to the library.
        typedef typename range_size<S>::type result_type;

        result_type operator ()(S &s) const
        {
            return boost::size(s);
        }
    };
}

namespace op
{
    using mpl::_;

    struct size
      : boost::detail::function1<impl::size<_, impl::tag<_> > >
    {};
}

namespace
{
    op::size const &size = boost::detail::pod_singleton<op::size>::instance;
}

}} // namespace boost::sequence

#endif // BOOST_SEQUENCE_SIZE_DWA200658_HPP
