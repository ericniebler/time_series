// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SEQUENCE_END_DWA200655_HPP
# define BOOST_SEQUENCE_END_DWA200655_HPP

# include <boost/detail/function1.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/range/result_iterator.hpp>
# include <boost/range/end.hpp>
# include <boost/sequence/tag.hpp>
# include <boost/mpl/placeholders.hpp>
# include <boost/iterator/counting_iterator.hpp>

namespace boost { namespace sequence
{

/// INTERNAL ONLY
namespace impl
{
    template<typename S, typename T = typename tag<S>::type>
    struct end
    {
        typedef counting_iterator<
            typename range_result_iterator<S>::type
        > result_type;

        result_type operator ()(S &s) const
        {
            return result_type(boost::end(s));
        }
    };
}

namespace op
{
    using mpl::_;

    struct end
      : boost::detail::function1<impl::end<_, impl::tag<_> > >
    {};
}

namespace
{
    op::end const &end = boost::detail::pod_singleton<op::end>::instance;
}

}} // namespace boost::sequence

#endif // BOOST_SEQUENCE_END_DWA200655_HPP
