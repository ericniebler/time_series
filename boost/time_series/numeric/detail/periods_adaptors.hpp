///////////////////////////////////////////////////////////////////////////////
/// \file periods_adaptors.hpp
/// Given periods in a RandomAccessRange, make it appear as a RangeRunStorage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_NUMERIC_DETAIL_PERIOD_ADAPTORS_EAN_10_07_2006
#define BOOST_TIME_SERIES_NUMERIC_DETAIL_PERIOD_ADAPTORS_EAN_10_07_2006

#include <boost/assert.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/size_type.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/result_iterator.hpp>
#include <boost/range_run_storage/utility/unit_run.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/property_map/dereference.hpp>

namespace boost { namespace time_series { namespace detail
{
    namespace rrs = range_run_storage;

    //template<typename RandomAccessRange>
    //struct periods_storage_adaptor
    //{
    //    periods_storage_adaptor(RandomAccessRange & rng)
    //      : rng_(rng)
    //    {
    //        BOOST_ASSERT(0 < boost::size(rng));
    //    }

    //    typedef typename range_size<RandomAccessRange>::type size_type;
    //    typedef typename range_value<RandomAccessRange>::type value_type;
    //    typedef typename range_result_iterator<RandomAccessRange>::type iterator;
    //    typedef iterator const_iterator;

    //    iterator begin() const
    //    { 
    //        return boost::begin(this->rng_);
    //    }

    //    iterator end() const
    //    { 
    //        return boost::end(this->rng_) - 1;
    //    }

    //    struct runs_type
    //    {
    //        typedef std::pair<value_type, value_type> result_type;

    //        result_type operator()(iterator i) const
    //        {
    //            return std::make_pair(*i, *(i+1));
    //        }
    //    };

    //private:
    //    RandomAccessRange & rng_;
    //};

    template<typename Offsets>
    struct periods_storage_adaptor
    {
        periods_storage_adaptor(Offsets & offsets)
          : offsets_(offsets)
        {}

        typedef typename range_size<Offsets>::type size_type;
        typedef typename range_value<Offsets>::type value_type;
        typedef typename range_result_iterator<Offsets>::type iterator;
        typedef iterator const_iterator;

        iterator begin() const
        {
            return boost::begin(this->offsets_);
        }

        iterator end() const
        {
            return boost::end(this->offsets_);
        }

        struct runs
        {
            typedef rrs::unit_run<value_type> result_type;

            result_type operator()(iterator i) const
            {
                return result_type(*i);
            }
        };

        typedef property_maps::dereference elements;

    private:
        Offsets & offsets_;
    };

    template<typename Offsets, typename Elements>
    struct periods_and_elements_storage_adaptor
    {
        periods_and_elements_storage_adaptor(Offsets & offsets, Elements & elements)
          : offsets_(offsets)
          , elements_(elements)
        {
            BOOST_ASSERT(boost::size(offsets) == boost::size(elements));
        }

        typedef typename range_size<Offsets>::type size_type;
        typedef typename range_value<Elements>::type value_type;
        typedef typename range_result_iterator<Offsets>::type offsets_iterator;
        typedef typename range_result_iterator<Elements>::type elements_iterator;
        typedef zip_iterator<tuple<offsets_iterator, elements_iterator> > iterator;
        typedef iterator const_iterator;

        iterator begin() const
        {
            return boost::make_zip_iterator(
                boost::make_tuple(
                    boost::begin(this->offsets_)
                  , boost::begin(this->elements_)
                )
            );
        }

        iterator end() const
        {
            return boost::make_zip_iterator(
                boost::make_tuple(
                    boost::end(this->offsets_)
                  , boost::end(this->elements_)
                )
            );
        }

        struct runs
        {
            typedef rrs::unit_run<value_type> result_type;

            result_type operator()(iterator i) const
            {
                return result_type(boost::get<0>(*i));
            }
        };

        struct elements
        {
            typedef value_type result_type;

            result_type operator()(iterator i) const
            {
                return boost::get<1>(*i);
            }
        };

    private:
        Offsets & offsets_;
        Elements & elements_;
    };

    struct periods_storage_adaptor_tag;

}}}

namespace boost { namespace sequence { namespace impl
{
    template<typename Offsets>
    struct tag<time_series::detail::periods_storage_adaptor<Offsets> >
    {
        typedef time_series::detail::periods_storage_adaptor_tag type;
    };

    template<typename Offsets, typename Elements>
    struct tag<time_series::detail::periods_and_elements_storage_adaptor<Offsets, Elements> >
    {
        typedef time_series::detail::periods_storage_adaptor_tag type;
    };

    template<typename S>
    struct elements<S, time_series::detail::periods_storage_adaptor_tag>
    {
        typedef typename S::elements result_type;

        result_type operator()(S &) const
        {
            return result_type();
        }
    };
}}}

namespace boost { namespace range_run_storage { namespace impl
{
    template<typename S>
    struct runs<S, time_series::detail::periods_storage_adaptor_tag>
    {
        typedef typename S::runs result_type;

        result_type operator()(S &) const
        {
            return result_type();
        }
    };
}}}

#endif // BOOST_TIME_SERIES_NUMERIC_DETAIL_PERIOD_ADAPTORS_EAN_10_07_2006
