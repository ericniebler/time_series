///////////////////////////////////////////////////////////////////////////////
/// \file unit_run.hpp
/// A type for holding a run of length 1
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_UTILITY_UNIT_RUN_EAN_07_17_2006_HPP
# define BOOST_RANGE_RUN_STORAGE_UTILITY_UNIT_RUN_EAN_07_17_2006_HPP

# include <limits>
# include <cstddef>
# include <boost/assert.hpp>
# include <boost/mpl/if.hpp>
# include <boost/type_traits/is_floating_point.hpp>
# include <boost/range_run_storage/offset.hpp>
# include <boost/range_run_storage/end_offset.hpp>
# include <boost/range_run_storage/length.hpp>
# include <boost/range_run_storage/empty.hpp>
# include <boost/range_run_storage/traits/is_unit_run.hpp>

namespace boost { namespace range_run_storage
{
    template<typename Offset = std::ptrdiff_t>
    struct unit_run
    {
        typedef is_floating_point<Offset> is_float;

        explicit unit_run(Offset offset = 0)
          : offset_(offset)
        {}

        unit_run(Offset offset, Offset endoff)
          : offset_(offset)
        {
            BOOST_ASSERT(is_float::value || endoff == offset + 1);
        }

        typedef Offset offset_type;
        Offset offset_;
    };

    struct unit_run_tag;

    namespace impl
    {
        template<typename Offset>
        struct tag<unit_run<Offset> >
        {
            typedef unit_run_tag type;
        };

        template<typename R>
        struct offset<R, unit_run_tag>
        {
            typedef typename R::offset_type result_type;

            result_type operator ()(R &r) const
            {
                return r.offset_;
            }
        };

        template<typename R>
        struct end_offset_int_
        {
            typedef typename R::offset_type result_type;

            result_type operator ()(R &r) const
            {
                return r.offset_ + 1;
            }
        };

        // If the offset_type is a float, the end_offset is the same
        // as the offset.
        template<typename R>
        struct end_offset<R, unit_run_tag>
          : mpl::if_<
                typename R::is_float
              , offset<R, unit_run_tag>
              , end_offset_int_<R>
            >::type
        {};

        template<typename R>
        struct length_int_
        {
            typedef typename R::offset_type offset_type;
            typedef typename length_type<offset_type>::type result_type;

            result_type operator ()(R &) const
            {
                return static_cast<result_type>(1u);
            }
        };

        template<typename R>
        struct length_float_
        {
            typedef typename R::offset_type result_type;

            result_type operator ()(R &) const
            {
                return static_cast<result_type>(0);
            }
        };

        // If the offset_type is a float, the run length is zero.
        // Otherwise it is 1.
        template<typename R>
        struct length<R, unit_run_tag>
          : mpl::if_<
                typename R::is_float
              , length_float_<R>
              , length_int_<R>
            >::type
        {};

        template<typename R>
        struct empty<R, unit_run_tag>
        {
            typedef bool result_type;

            result_type operator ()(R &) const
            {
                return false;
            }
        };
    }

    namespace traits
    {
        template<typename Offset>
        struct is_unit_run<unit_run<Offset> >
          : mpl::true_
        {};
    }
}}

namespace boost { namespace serialization
{
    template<typename Archive, typename Offset>
    void serialize(Archive &ar, range_run_storage::unit_run<Offset> &r, unsigned int const version)
    {
        ar & r.offset_;
    }
}}

#endif
