// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_LENGTH_DWA2006514_HPP
# define BOOST_RANGE_RUN_STORAGE_LENGTH_DWA2006514_HPP

# include <limits>  // for std::numeric_limits
# include <climits> // for CHAR_BIT
# include <cstddef> // for std::size_t and std::ptrdiff_t
# include <boost/integer.hpp>
# include <boost/utility/result_of.hpp>
# include <boost/type_traits/add_reference.hpp>
# include <boost/detail/function1.hpp>
# include <boost/detail/pod_singleton.hpp>
# include <boost/range_run_storage/tag.hpp>
# include <boost/range_run_storage/offset.hpp>
# include <boost/range_run_storage/end_offset.hpp>
# include <boost/mpl/placeholders.hpp>

namespace boost { namespace range_run_storage
{

/// INTERNAL ONLY
namespace impl
{
    template<typename Offset, bool IsIntegral = std::numeric_limits<Offset>::is_integer>
    struct length_type
    {
        typedef typename boost::uint_t<sizeof(Offset) * CHAR_BIT>::fast type;
    };

    template<typename Offset>
    struct length_type<Offset, false>
    {
        typedef Offset type; // float offset -> float length
    };

    template<>
    struct length_type<std::ptrdiff_t, true>
    {
        typedef std::size_t type;
    };

    template<typename R, typename T = typename range_run_storage::impl::tag<R>::type>
    struct length
    {
        typedef typename result_of<
            op::offset(typename add_reference<R>::type)
        >::type offset_type;

        typedef typename length_type<offset_type>::type result_type;

        result_type operator ()(R &r) const
        {
            return static_cast<result_type>(
                range_run_storage::end_offset(r) - range_run_storage::offset(r)
            );
        }
    };
}

namespace op
{
    using mpl::_;

    struct length
      : boost::detail::function1<impl::length<_, range_run_storage::impl::tag<_> > >
    {};
}

namespace
{
    op::length const &length = boost::detail::pod_singleton<op::length>::instance;
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_STORAGE_LENGTH_DWA2006514_HPP
