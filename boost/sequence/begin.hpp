// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SEQUENCE_BEGIN_DWA200655_HPP
# define BOOST_SEQUENCE_BEGIN_DWA200655_HPP

# include <boost/detail/function1.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/range/result_iterator.hpp>
# include <boost/range/begin.hpp>
# include <boost/sequence/tag.hpp>
# include <boost/mpl/placeholders.hpp>
# include <boost/iterator/counting_iterator.hpp>

namespace boost { namespace sequence
{

/// INTERNAL ONLY
namespace impl
{
    template<typename S, typename T = typename tag<S>::type>
    struct begin
    {
        typedef counting_iterator<
            typename range_result_iterator<S>::type
        > result_type;

        result_type operator ()(S &s) const
        {
            return result_type(boost::begin(s));
        }
    };
}

namespace op
{
    using mpl::_;

    struct begin
      : boost::detail::function1<impl::begin<_, impl::tag<_> > >
    {};
}

namespace
{
    op::begin const &begin = boost::detail::pod_singleton<op::begin>::instance;
}

}} // namespace boost::sequence

#endif // BOOST_SEQUENCE_BEGIN_DWA200655_HPP
