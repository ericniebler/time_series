// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_DIFFERENCE1_DWA200659_HPP
# define BOOST_RANGE_RUN_STORAGE_DIFFERENCE1_DWA200659_HPP

# include <boost/iterator/iterator_traits.hpp>
# include <boost/range_run_storage/utility/unit_run.hpp>
# include <boost/range_run_storage/traits/is_dense_runs.hpp>

namespace boost { namespace range_run_storage
{

template<typename Base>
struct difference1
{
    typedef typename iterator_difference<Base>::type difference_type;
    typedef unit_run<difference_type> result_type;

    explicit difference1(Base x)
      : base(x)
    {}

    result_type operator ()(Base const &y) const
    {
        return result_type(y - base);
    }

 private:
    Base base;
};

namespace traits
{
    template<typename Base>
    struct is_dense_runs<difference1<Base> >
      : mpl::true_
    {};
}

}} // namespace boost::range_run_storage

#endif // BOOST_RANGE_RUN_STORAGE_DIFFERENCE1_DWA200659_HPP
