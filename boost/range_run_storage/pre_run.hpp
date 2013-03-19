// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_PRE_RUN_EAN_08_03_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_PRE_RUN_EAN_08_03_2006_HPP

# include <boost/config.hpp>
# include <boost/detail/workaround.hpp>
# include <boost/assert.hpp>
# include <boost/detail/function1.hpp>
# include <boost/detail/function2.hpp>
# include <boost/detail/overload.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/sequence/tag.hpp>
# include <boost/mpl/assert.hpp>
# include <boost/mpl/placeholders.hpp>
# include <boost/mpl/vector/vector10.hpp>
# include <boost/range_run_storage/tag.hpp>
# include <boost/range_run_storage/offset.hpp>
# include <boost/range_run_storage/end_offset.hpp>
# include <boost/range_run_storage/length.hpp>
# include <boost/range_run_storage/empty.hpp>
# include <boost/range_run_storage/zero.hpp>
# include <boost/range_run_storage/utility/infinity.hpp>

namespace boost { namespace range_run_storage
{

namespace concepts
{
    template<typename S>
    struct RangeRunStorage;
}

/// INTERNAL ONLY
namespace impl
{
    ///////////////////////////////////////////////////////////////////////////////
    // empty_pre_run
    template<typename Offset = std::ptrdiff_t>
    struct empty_pre_run
    {
        typedef Offset offset_type;
        explicit empty_pre_run(Offset = Offset(), Offset = Offset())
        {}
    };

    struct empty_pre_run_tag;

    template<typename Offset>
    struct tag<empty_pre_run<Offset> >
    {
        typedef empty_pre_run_tag type;
    };

    template<typename R>
    struct offset<R, empty_pre_run_tag>
    {
        typedef typename R::offset_type result_type;
        result_type operator ()(R &) const
        {
            return -inf;
        }
    };

    template<typename R>
    struct end_offset<R, empty_pre_run_tag>
    {
        typedef typename R::offset_type result_type;
        result_type operator ()(R &) const
        {
            return -inf;
        }
    };

    template<typename R>
    struct length<R, empty_pre_run_tag>
    {
        typedef typename R::offset_type offset_type;
        typedef typename length_type<offset_type>::type result_type;
        result_type operator ()(R &) const
        {
            return static_cast<result_type>(0);
        }
    };

    template<typename R>
    struct empty<R, empty_pre_run_tag>
    {
        typedef bool result_type;
        result_type operator ()(R &) const
        {
            return true;
        }
    };

    // Fetch the "pre_run" of the storage -- that is, the run
    // that comes before all other runs.
    template<typename S, typename T = typename sequence::impl::tag<S>::type>
    struct pre_run
    {
        typedef typename concepts::RangeRunStorage<S>::offset_type offset_type;
        typedef empty_pre_run<offset_type> result_type;

        result_type operator ()(S &) const
        {
            return result_type();
        }
    };

    // Get the value of the "pre_run".
    template<typename S, typename T = typename sequence::impl::tag<S>::type>
    struct pre_value
    {
        typedef typename concepts::RangeRunStorage<S>::zero_type result_type;

        result_type operator ()(S &s) const
        {
            return range_run_storage::zero(s);
        }
    };

    // Set the value of the "pre_run".
    template<typename S, typename V, typename T = typename sequence::impl::tag<S>::type>
    struct set_pre_value
    {
        typedef void result_type;

        void operator ()(S &, V &) const
        {
            BOOST_ASSERT(false);
        }
    };
}

namespace op
{
    using mpl::_;

    namespace detail
    {
        struct pre_value
          : boost::detail::function1<impl::pre_value<_, sequence::impl::tag<_> > >
        {};

        struct set_pre_value
          : boost::detail::function2<impl::set_pre_value<_, _, sequence::impl::tag<_> > >
        {};
    }

    struct pre_run
      : boost::detail::function1<impl::pre_run<_, sequence::impl::tag<_> > >
    {};

    struct pre_value
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1310)
      : boost::detail::overload1_2<detail::pre_value, detail::set_pre_value>
#else
      : boost::detail::overload<mpl::vector2<detail::pre_value, detail::set_pre_value> >
#endif
    {};
}

namespace
{
    op::pre_run const &pre_run = boost::detail::pod_singleton<op::pre_run>::instance;
    op::pre_value const &pre_value = boost::detail::pod_singleton<op::pre_value>::instance;
}

}}

#endif
