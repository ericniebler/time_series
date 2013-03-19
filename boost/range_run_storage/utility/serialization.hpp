///////////////////////////////////////////////////////////////////////////////
/// \file serialization.hpp
/// Genenric serialization routines for models of \c Mutable_InfiniteRangeRunStorage
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_UTILITY_SERIALIZATION_EAN_10_23_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_UTILITY_SERIALIZATION_EAN_10_23_2006_HPP

#include <algorithm>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/implicit_cast.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/range_run_storage/utility/run_cast.hpp>

namespace boost { namespace range_run_storage
{

    template<typename Archive, typename Series>
    void save_range_run_storage(Archive & ar, Series const &series, unsigned int const version)
    {
        namespace seq = sequence;
        namespace rrs = range_run_storage;
        typedef concepts::InfiniteRangeRunStorage<Series const> storage_concept;
        typedef typename storage_concept::value_type value_type;
        typedef typename storage_concept::offset_type offset_type;
        typedef typename storage_concept::cursor cursor_type;
        typedef typename storage_concept::elements elements_type;
        typedef typename storage_concept::runs runs_type;
        typedef std::pair<offset_type, offset_type> run_type;

        // serialize the zero
        ar << implicit_cast<value_type const &>(rrs::zero(series));

        // serialize the pre-run
        run_type const pre_run_ = rrs::run_cast<run_type>(rrs::pre_run(series));
        ar << pre_run_;
        if(!rrs::empty(pre_run_))
        {
            ar << implicit_cast<value_type const &>(rrs::pre_value(series));
        }

        // serialize the runs
        runs_type runs = rrs::runs(series);
        elements_type elements = seq::elements(series);
        cursor_type begin = seq::begin(series);
        cursor_type end = seq::end(series);

        // write out the number of runs
        typedef typename iterator_difference<cursor_type>::type difference_type;
        difference_type len = std::distance(begin, end);
        ar << len;

        for(; begin != end; ++begin)
        {
            run_type const run = rrs::run_cast<run_type>(runs(*begin));
            value_type const &element = elements(*begin);

            ar << run;
            ar << element;
        }

        // serialize the post-run
        run_type const post_run_ = rrs::run_cast<run_type>(rrs::post_run(series));
        ar << post_run_;
        if(!rrs::empty(post_run_))
        {
            ar << implicit_cast<value_type const &>(rrs::post_value(series));
        }
    }

    template<typename Archive, typename Series>
    void load_range_run_storage(Archive & ar, Series &series, unsigned int const version)
    {
        namespace rrs = range_run_storage;
        typedef concepts::Mutable_InfiniteRangeRunStorage<Series> storage_concept;
        typedef typename storage_concept::value_type value_type;
        typedef typename storage_concept::offset_type offset_type;
        typedef typename storage_concept::cursor cursor_type;
        typedef typename storage_concept::ordered_inserter_type ordered_inserter_type;

        // deserialize the zero
        value_type value;
        ar >> value;
        rrs::zero(series, value);

        // make an ordered inserter:
        ordered_inserter_type oi = rrs::ordered_inserter(series);

        std::pair<offset_type, offset_type> run;

        // deserialize the pre-run
        ar >> run;
        if(run.first != run.second)
        {
            ar >> value;
            rrs::set_at(oi, run, value);
        }

        // read in the number of runs
        typedef typename iterator_difference<cursor_type>::type difference_type;
        difference_type len;
        ar >> len;

        for(difference_type i = 0; i < len; ++i)
        {
            ar >> run;
            ar >> value;
            rrs::set_at(oi, run, value);
        }

        // deserialize the post-run
        ar >> run;
        if(run.first != run.second)
        {
            ar >> value;
            rrs::set_at(oi, run, value);
        }

        // commit the new runs
        rrs::commit(oi);
    }

}}

#define BOOST_RANGE_RUN_STORAGE_DEFINE_SERIALIZE_MEMBER()                       \
    friend class boost::serialization::access;                                  \
    template<typename Archive>                                                  \
    void save(Archive & ar, const unsigned int version) const                   \
    {                                                                           \
        boost::range_run_storage::save_range_run_storage(ar, *this, version);   \
    }                                                                           \
    template<typename Archive>                                                  \
    void load(Archive & ar, const unsigned int version)                         \
    {                                                                           \
        boost::range_run_storage::load_range_run_storage(ar, *this, version);   \
    }                                                                           \
    BOOST_SERIALIZATION_SPLIT_MEMBER()                                          \
    /**/

#endif
