// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SEQUENCE_ELEMENTS_DWA200655_HPP
# define BOOST_SEQUENCE_ELEMENTS_DWA200655_HPP

# include <boost/detail/function1.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/mpl/placeholders.hpp>
# include <boost/property_map/dereference.hpp>

namespace boost { namespace sequence
{

/// INTERNAL ONLY
namespace impl
{
    template<typename S, typename T = typename tag<S>::type>
    struct elements
    {
        typedef property_maps::dereference result_type;

        result_type operator ()(S &s) const
        {
            return result_type();
        }
    };
}

namespace op
{
    using mpl::_;

    struct elements
      : boost::detail::function1<impl::elements<_, impl::tag<_> > >
    {};
}

namespace
{
    op::elements const &elements = boost::detail::pod_singleton<op::elements>::instance;
}

}} // namespace boost::sequence

#endif // BOOST_SEQUENCE_ELEMENTS_DWA200655_HPP
