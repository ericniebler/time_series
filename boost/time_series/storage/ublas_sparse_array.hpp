///////////////////////////////////////////////////////////////////////////////
/// \file ublas_sparse_array.hpp
/// A generator for a sparse Array that satisfies the Array concept
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_UBLAS_SPARSE_ARRAY_EAN_04_26_2006
#define BOOST_TIME_SERIES_STORAGE_UBLAS_SPARSE_ARRAY_EAN_04_26_2006

#include <cstddef>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/time_series/utility/array.hpp>
#include <boost/time_series/storage/detail/mem_fun_runs.hpp>

namespace boost
{
    namespace time_series_ { namespace detail
    {
        // A property map whose keys are C++98 iterators and
        // whose values are the integer indices corresponding
        // to the iterator.
        struct ublas_runs_map
        {
            // For result_of support
            typedef compressed_pair<
                std::ptrdiff_t
              , mpl::size_t<1>
            > result_type;

            // Readability : Iterator to index
            template<typename K>
            result_type
            operator ()(K const &k) const
            {
                return result_type(static_cast<std::ptrdiff_t>(k.index()));
            }
        };
    }}

    namespace sequence { namespace impl
    {
        struct ublas_mapped_vector_tag;

        template<typename T, typename A>
        struct tag<numeric::ublas::mapped_vector<T, A> >
        {
            typedef ublas_mapped_vector_tag type;
        };

        template<class S, class C, class I, class V>
        struct insert<S, C, I, V, ublas_mapped_vector_tag>
        {
            typedef void result_type;

            void operator ()(S &s, C &c, I &i, V &v) const
            {
                if(0 != v)
                {
                    s.insert_element(i, v);
                }
            }
        };
    }}

    namespace range_run_storage { namespace impl
    {
        template<typename S>
        struct runs<S, sequence::impl::ublas_mapped_vector_tag>
        {
            typedef time_series_::detail::ublas_runs_map result_type;

            result_type operator ()(S &) const
            {
                return result_type();
            }
        };

        template<typename S, typename I>
        struct get_at<S, I, sequence::impl::ublas_mapped_vector_tag>
        {
            typedef typename S::value_type result_type;

            result_type operator ()(S &s, I &i) const
            {
                return s[i];
            }
        };

        template<typename S, typename R, typename V>
        struct set_at<S, R, V, sequence::impl::ublas_mapped_vector_tag>
        {
            typedef void result_type;

            result_type operator ()(S &s, R &r, V &v) const
            {
                BOOST_MPL_ASSERT((typename concepts::Run<R>::is_unit_length));
                if(0 != v)
                {
                    s[range_run_storage::offset(r)] = v;
                }
                else if(typename S::value_type *where = s.find_element(range_run_storage::offset(r)))
                {
                    *where = 0;
                }
            }
        };
    }}
}

namespace boost { namespace time_series_ { namespace detail
{
    template<typename Value>
    struct storage<Value, tag::sparse>
    {
        typedef numeric::ublas::mapped_vector<Value> type;
    };
}}}

#endif
