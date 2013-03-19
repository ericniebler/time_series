// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_CATEGORY_DWA2006513_HPP
# define BOOST_RANGE_RUN_CATEGORY_DWA2006513_HPP

# include <utility>
# include <boost/compressed_pair.hpp>

namespace boost { namespace range_run_storage {

/// INTERNAL ONLY
namespace impl
{
    template<typename S>
    struct tag
    {
        typedef void type;
    };

    template<typename S>
    struct tag<S const>
      : tag<S>
    {};

    template<typename S>
    struct tag<S volatile>
      : tag<S>
    {};

    template<typename S>
    struct tag<S const volatile>
      : tag<S>
    {};

    // BUGBUG find a better place for this
    struct std_pair_tag;
    struct compressed_pair_tag;

    template<typename F, typename S>
    struct tag<compressed_pair<F, S> >
    {
        typedef compressed_pair_tag type;
    };

    template<typename F, typename S>
    struct tag<std::pair<F, S> >
    {
        typedef std_pair_tag type;
    };
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_CATEGORY_DWA2006513_HPP
