///////////////////////////////////////////////////////////////////////////////
/// \file subrun.hpp
/// utilities for manipulating runs
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_UTILITY_SUBRUN_EAN_07_17_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_UTILITY_SUBRUN_EAN_07_17_2006_HPP

#include <utility>
#include <algorithm>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/detail/function1.hpp>
#include <boost/detail/function2.hpp>
#include <boost/detail/pod_singleton.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/traits/is_unit_run.hpp>

namespace boost { namespace range_run_storage
{

    namespace impl
    {
        ///////////////////////////////////////////////////////////////////////
        // less
        template<typename L, typename R>
        struct less
        {
            typedef bool result_type;

            result_type operator ()(L &l, R &r) const
            {
                typedef typename range_run_storage::concepts::Run<L>::offset_type loff_type;
                typedef typename range_run_storage::concepts::Run<R>::offset_type roff_type;
                return less::call(
                    range_run_storage::end_offset(l)
                  , range_run_storage::offset(r)
                  , mpl::or_<is_float<loff_type>, is_float<roff_type> >()
                );
            }
        private:
            template<typename LOff, typename ROff>
            static bool call(LOff l, ROff r, mpl::false_)
            {
                return l <= r; // ints use half-open range
            }

            template<typename LOff, typename ROff>
            static bool call(LOff l, ROff r, mpl::true_)
            {
                return l < r; // floats have closed ranges. (Yuk).
            }
        };

        ///////////////////////////////////////////////////////////////////////
        // before
        template<typename L, typename R>
        struct before
        {
            typedef bool result_type;

            result_type operator ()(L &l, R &r) const
            {
                return range_run_storage::offset(l) < range_run_storage::offset(r);
            }
        };

        ///////////////////////////////////////////////////////////////////////
        // subrun
        template<typename R, typename T = typename traits::is_unit_run<R>::type>
        struct subrun
        {
            typedef typename concepts::Run<R>::offset_type offset_type;
            typedef std::pair<offset_type, offset_type> result_type;

            result_type operator ()(R &r) const
            {
                return std::make_pair(range_run_storage::offset(r), range_run_storage::end_offset(r));
            }
        };

        template<typename R>
        struct subrun<R, mpl::true_>
        {
            typedef typename remove_cv<R>::type result_type;

            R &operator ()(R &r) const
            {
                return r;
            }
        };

        ///////////////////////////////////////////////////////////////////////
        // has_leftover
        template<typename R, typename T = typename traits::is_unit_run<R>::type>
        struct has_leftover
        {
            typedef bool result_type;

            result_type operator ()(R &r) const
            {
                return 0 < range_run_storage::length(r);
            }
        };

        template<typename R>
        struct has_leftover<R, mpl::true_>
        {
            typedef bool result_type;

            result_type operator ()(R &r) const
            {
                return false; // unit runs never have any left-overs
            }
        };

        ///////////////////////////////////////////////////////////////////////
        // advance_to
        template<typename R, typename O, typename T = typename traits::is_unit_run<R>::type>
        struct advance_to
        {
            typedef void result_type;

            result_type operator ()(R &r, O &o) const
            {
                r.first = o;
            }
        };

        template<typename R, typename O>
        struct advance_to<R, O, mpl::true_>
        {
            typedef void result_type;

            result_type operator ()(R &r, O &o) const
            {}
        };

        ///////////////////////////////////////////////////////////////////////
        // front
        template<typename R, typename O, typename T = typename traits::is_unit_run<R>::type>
        struct front
        {
            typedef typename concepts::Run<R>::offset_type offset_type;
            typedef std::pair<offset_type, offset_type> result_type;

            result_type operator ()(R &r, O &o) const
            {
                return std::make_pair(range_run_storage::offset(r), o);
            }
        };

        template<typename R, typename O>
        struct front<R, O, mpl::true_>
        {
            typedef R result_type;

            result_type operator ()(R &r, O &o) const
            {
                BOOST_ASSERT(false);
                return r;
            }
        };
    } // namespace impl

    namespace op
    {
        using mpl::_;
        using mpl::_1;
        using mpl::_2;

        struct less
          : boost::detail::function2<impl::less<_, _> >
        {};

        struct before
          : boost::detail::function2<impl::before<_, _> >
        {};

        struct subrun
          : boost::detail::function1<impl::subrun<_, traits::is_unit_run<_> > >
        {};

        struct has_leftover
          : boost::detail::function1<impl::has_leftover<_, traits::is_unit_run<_> > >
        {};

        struct advance_to
          : boost::detail::function2<impl::advance_to<_1, _2, traits::is_unit_run<_1> > >
        {};

        struct front
          : boost::detail::function2<impl::front<_1, _2, traits::is_unit_run<_1> > >
        {};
    }

    namespace
    {
        op::less const &less = boost::detail::pod_singleton<op::less>::instance;
        op::before const &before = boost::detail::pod_singleton<op::before>::instance;
        op::subrun const &subrun = boost::detail::pod_singleton<op::subrun>::instance;
        op::has_leftover const &has_leftover = boost::detail::pod_singleton<op::has_leftover>::instance;
        op::advance_to const &advance_to = boost::detail::pod_singleton<op::advance_to>::instance;
        op::front const &front = boost::detail::pod_singleton<op::front>::instance;
    }

    namespace impl
    {
        ///////////////////////////////////////////////////////////////////////
        // overlap
        template<
            typename L
          , typename R
          , typename LU = typename traits::is_unit_run<L>::type
          , typename RU = typename traits::is_unit_run<R>::type
        >
        struct overlap
        {
            // BUGBUG promotion?
            typedef typename concepts::Run<L>::offset_type offset_type;
            typedef std::pair<offset_type, offset_type> result_type; 

            result_type operator ()(L &l, R &r) const
            {
                BOOST_ASSERT(!op::less()(l, r) && !op::less()(r, l));
                return std::make_pair(
                    std::max<offset_type>(range_run_storage::offset(l), range_run_storage::offset(r))
                  , std::min<offset_type>(range_run_storage::end_offset(l), range_run_storage::end_offset(r))
                );
            }
        };

        template<typename L, typename R>
        struct overlap<L, R, mpl::true_, mpl::false_>
        {
            typedef typename remove_cv<L>::type result_type;

            L &operator ()(L &l, R &r) const
            {
                BOOST_ASSERT(!op::less()(l, r) && !op::less()(r, l));
                return l;
            }
        };

        template<typename L, typename R>
        struct overlap<L, R, mpl::false_, mpl::true_>
        {
            typedef typename remove_cv<R>::type result_type;

            R &operator ()(L &l, R &r) const
            {
                BOOST_ASSERT(!op::less()(l, r) && !op::less()(r, l));
                return r;
            }
        };

        template<typename L, typename R>
        struct overlap<L, R, mpl::true_, mpl::true_>
        {
            typedef typename remove_cv<L>::type result_type;

            L &operator ()(L &l, R &r) const
            {
                BOOST_ASSERT(!op::less()(l, r) && !op::less()(r, l));
                return l;
            }
        };
    } // namespace impl

    namespace op
    {
        using mpl::_1;
        using mpl::_2;

        struct overlap
          : boost::detail::function2<impl::overlap<_1, _2, traits::is_unit_run<_1>, traits::is_unit_run<_2> > >
        {};
    }

    namespace
    {
        op::overlap const &overlap = boost::detail::pod_singleton<op::overlap>::instance;
    }

}} // namespace boost::range_run_storage

#endif
