///////////////////////////////////////////////////////////////////////////////
/// \file tagged_op.hpp
/// functors for tagging args as "left", "right", or "both"
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_DETAIL_TAGGED_OPS_EAN_08_31_2006
#define BOOST_TIME_SERIES_NUMERIC_DETAIL_TAGGED_OPS_EAN_08_31_2006

#include <boost/mpl/placeholders.hpp>
#include <boost/detail/function1.hpp>
#include <boost/detail/function2.hpp>
#include <boost/detail/pod_singleton.hpp>

namespace boost { namespace time_series { namespace detail
{
    template<typename Arg>
    struct left 
    {
        explicit left(Arg &arg)
          : arg(arg) 
        {}
        
        Arg &arg;
    };

    template<typename Arg>
    struct right
    {
        explicit right(Arg &arg)
          : arg(arg) 
        {}
        
        Arg &arg;
    };

    template<typename Left, typename Right>
    struct both
    {
        both(Left &left, Right &right)
          : left(left)
          , right(right)
        {}
        
        Left &left;
        Right &right;
    };

    namespace impl
    {
        template<typename Arg>
        struct left_op
        {
            typedef left<Arg> result_type;
            result_type operator ()(Arg &arg) const
            {
                return result_type(arg);
            }
        };

        template<typename Arg>
        struct right_op
        {
            typedef right<Arg> result_type;
            result_type operator ()(Arg &arg) const
            {
                return result_type(arg);
            }
        };

        template<typename Left, typename Right>
        struct both_op
        {
            typedef both<Left, Right> result_type;
            result_type operator ()(Left &left, Right &right) const
            {
                return result_type(left, right);
            }
        };
    }

    namespace op
    {
        using boost::mpl::_;

        struct left_op
          : boost::detail::function1<impl::left_op<_> >
        {};

        struct right_op
          : boost::detail::function1<impl::right_op<_> >
        {};

        struct both_op
          : boost::detail::function2<impl::both_op<_, _> >
        {};
    }

    namespace
    {
        op::left_op const &left_op = boost::detail::pod_singleton<op::left_op>::instance;

        op::right_op const &right_op = boost::detail::pod_singleton<op::right_op>::instance;

        op::both_op const &both_op = boost::detail::pod_singleton<op::both_op>::instance;
    }

}}}

#endif // BOOST_TIME_SERIES_NUMERIC_VARIABLE_PERIOD_SUMS_EAN_08_10_2006
