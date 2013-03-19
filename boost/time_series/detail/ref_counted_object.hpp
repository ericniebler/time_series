//////////////////////////////////////////////////////////////////////////////
// (c) Copyright Eric Niebler 2006
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_TIME_SERIES_DETAIL_REF_COUNTED_OBJECT_HPP_EAN_06_17_2006
#define BOOST_TIME_SERIES_DETAIL_REF_COUNTED_OBJECT_HPP_EAN_06_17_2006

#include <boost/time_series/detail/counted_base.hpp>

namespace boost { namespace time_series { namespace detail
{
    template<typename T>
    struct ref_counted_object
      : counted_base<ref_counted_object<T> >
    {
        ref_counted_object()
          : obj_()
        {}

        ref_counted_object(ref_counted_object const &that)
          : obj_(that.obj_)
        {}

        explicit ref_counted_object(T const &obj)
          : obj_(obj)
        {}

        template<typename A0>
        explicit ref_counted_object(A0 const &a0)
          : obj_(a0)
        {}

        template<typename A0, typename A1>
        ref_counted_object(A0 const &a0, A1 const &a1)
          : obj_(a0, a1)
        {}

        template<typename A0, typename A1, typename A2>
        ref_counted_object(A0 const &a0, A1 const &a1, A2 const &a2)
          : obj_(a0, a1, a2)
        {}

        T &get()
        {
            return this->obj_;
        }

        T const &get() const
        {
            return this->obj_;
        }
    private:
        T obj_;
    };

}}} // namespace boost::time_series::detail

#endif
