///////////////////////////////////////////////////////////////////////////////
/// \file subsequence.hpp
/// An abstraction over either an run/value or a RangeRunStorage. 
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_RUN_STORAGE_UTILITY_SUBSEQUENCE_EAN_07_26_2006_HPP
#define BOOST_RANGE_RUN_STORAGE_UTILITY_SUBSEQUENCE_EAN_07_26_2006_HPP

#include <boost/sequence/elements.hpp>
#include <boost/sequence/begin.hpp>
#include <boost/sequence/end.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/runs.hpp>
#include <boost/range_run_storage/utility/subrun.hpp>
#include <boost/range_run_storage/traits/is_unit_run.hpp>
#include <boost/range_run_storage/offset.hpp>
#include <boost/range_run_storage/end_offset.hpp>
#include <boost/range_run_storage/algorithm/detail/runs_searching.hpp>

namespace boost { namespace range_run_storage
{
    namespace rrs = range_run_storage;
    namespace seq = sequence;

    template<typename RRS>
    struct range_run_subsequence
    {
        typedef typename concepts::RangeRunStorage<RRS const>::value_type   value_type;
        typedef typename concepts::RangeRunStorage<RRS const>::reference    reference;
        typedef typename concepts::RangeRunStorage<RRS const>::runs         runs_type;
        typedef typename concepts::RangeRunStorage<RRS const>::elements     elements_type;
        typedef typename concepts::RangeRunStorage<RRS const>::cursor       cursor_type;
        typedef typename concepts::RangeRunStorage<RRS const>::offset_type  offset_type;
        typedef typename concepts::RangeRunStorage<RRS const>::run_type     run_type;

        typedef typename boost::result_of<rrs::op::subrun(run_type const &)>::type subrun_type;

        explicit range_run_subsequence(RRS const &storage)
          : subrun_()
          , elements_(seq::elements(storage))
          , runs_(rrs::runs(storage))
          , begin_(seq::begin(storage))
          , end_(seq::end(storage))
        {
            if(this->begin_ != this->end_)
            {
                this->subrun_ = rrs::subrun(this->runs_(*this->begin_));
            }
        }

        void next()
        {
            BOOST_ASSERT(this->begin_ != this->end_);
            ++this->begin_;
            this->set_subrun_();
        }

        bool done() const
        {
            return this->begin_ == this->end_;
        }

        reference value() const
        {
            BOOST_ASSERT(this->begin_ != this->end_);
            return this->elements_(*this->begin_);
        }

        subrun_type subrun() const
        {
            return this->subrun_;
        }

        void advance_to(offset_type off)
        {
            rrs::advance_to(this->subrun_, off);
        }

        template<typename R>
        void skip_ahead_to(R const &r)
        {
            this->begin_ = detail::run_skip_ahead_to(this->begin_, this->end_, this->runs_, r);
            this->set_subrun_();
        }

    private:
        void set_subrun_()
        {
            if(this->begin_ != this->end_)
            {
                this->subrun_ = rrs::subrun(this->runs_(*this->begin_));
            }
            else
            {
                this->advance_to(rrs::end_offset(this->subrun_)); // done
            }
        }

        subrun_type subrun_;
        elements_type elements_;
        runs_type runs_;
        cursor_type begin_;
        cursor_type end_;
    };

    template<typename Run, typename Value>
    struct run_value_subsequence
    {
        typedef Value value_type;
        typedef Value const &reference;
        typedef typename concepts::Run<Run>::offset_type offset_type;
        typedef std::pair<offset_type, offset_type> subrun_type;

        explicit run_value_subsequence(Run const &run, Value const &value)
          : subrun_(rrs::run_cast<subrun_type>(run))
          , value_(value)
        {}

        void next()
        {
            this->advance_to(rrs::end_offset(this->subrun_));
        }

        bool done() const
        {
            return !rrs::has_leftover(this->subrun_);
        }

        reference value() const
        {
            return this->value_;
        }

        subrun_type subrun() const
        {
            return this->subrun_;
        }

        void advance_to(offset_type off)
        {
            rrs::advance_to(this->subrun_, off);
        }

        template<typename R>
        void skip_ahead_to(R const &)
        {
            this->next();
        }

    private:
        subrun_type subrun_;
        value_type value_;
    };

    template<typename SubSequence>
    inline bool is_done_and_empty(SubSequence const &sub)
    {
        return sub.done() && !rrs::has_leftover(sub.subrun());
    }

    template<typename SubSequence>
    struct has_unit_run
      : traits::is_unit_run<typename SubSequence::subrun_type>
    {};

}}

#endif
