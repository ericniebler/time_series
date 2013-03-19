// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_ZERO_EAN_06_29_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_ZERO_EAN_06_29_2006_HPP

# include <boost/config.hpp>
# include <boost/detail/workaround.hpp>
# include <boost/detail/function1.hpp>
# include <boost/detail/function2.hpp>
# include <boost/detail/overload.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/sequence/tag.hpp>
# include <boost/sequence/concepts.hpp>
# include <boost/mpl/placeholders.hpp>
# include <boost/mpl/vector/vector10.hpp>
# include <boost/numeric/functional.hpp>

namespace boost { namespace range_run_storage
{

/// INTERNAL ONLY
namespace impl
{
    // Fetch the "zero" value of the storage -- that is, the value
    // the sequence assumes where it's not covered by a run.
    template<typename S, typename T = typename sequence::impl::tag<S>::type>
    struct zero
    {
        typedef typename sequence::concepts::Sequence<S>::value_type value_type;
        typedef value_type const &result_type;

        result_type operator ()(S &) const
        {
            return numeric::zero<value_type>::value;
        }
    };

    template<typename S, typename V, typename T = typename sequence::impl::tag<S>::type>
    struct set_zero
    {
        typedef void result_type;

        void operator ()(S &s, V &v) const
        {
            typedef typename sequence::concepts::Sequence<S>::value_type value_type;
            value_type const &z = zero<S>()(s);
            BOOST_ASSERT(numeric::promote<bool>(z == v));
        }
    };
}

namespace op
{
    using mpl::_;

    namespace detail
    {
        struct zero
          : boost::detail::function1<impl::zero<_, sequence::impl::tag<_> > >
        {};

        struct set_zero
          : boost::detail::function2<impl::set_zero<_, _, sequence::impl::tag<_> > >
        {};
    }

    struct zero
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1310)
      : boost::detail::overload1_2<detail::zero, detail::set_zero>
#else
      : boost::detail::overload<mpl::vector2<detail::zero, detail::set_zero> >
#endif
    {};
}

namespace
{
    op::zero const &zero = boost::detail::pod_singleton<op::zero>::instance;
}

}}

#endif
