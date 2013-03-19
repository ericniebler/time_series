///////////////////////////////////////////////////////////////////////////////
/// \file ordered_inserter.hpp
/// Definition of the <tt>ordered_inserter\<\></tt> class template, and the 
/// <tt>make_ordered_inserter()</tt> helper function.
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_ORDERED_INSERTER_HPP_EAN_05_31_2006
#define BOOST_TIME_SERIES_ORDERED_INSERTER_HPP_EAN_05_31_2006

#include <utility>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/compressed_pair.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/sequence/tag.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/time_series_fwd.hpp>

namespace boost { namespace time_series
{

    /// \brief A wrapper for an \c OrderedInserter which itself satisfies the
    ///     \c OrderedInserter concept and that provides some conveniences
    ///     for efficiently populating a \c Mutable_TimeSeries.
    ///
    /// A wrapper for an \c OrderedInserter which itself satisfies the
    ///     \c OrderedInserter concept and that provides some conveniences
    ///     for efficiently populating a \c Mutable_TimeSeries.
    ///
    /// Since \c ordered_inserter\<\> satisfies the \c OrderedInserter concept,
    ///     it can be used anywhere an \c OrderedInserter is required; for example,
    ///     when calling any of the RangeRunStorage algorithms.
    /// 
    /// An \c ordered_inserter\<\> also overloads the function call operator
    ///     so that runs can be inserted using the following convenient syntax:
    ///     <tt>inserter(value, offset, end_offset)</tt>. The return value of the
    ///     predeeding call is a reference to the \c inserter object, so successive
    ///     insertion operations can be chained.
    /// 
    /// An \c ordered_inserter\<\> is also a valid STL output iterator.
    template<typename Series>
    struct ordered_inserter
      : std::iterator<std::output_iterator_tag, void, void, void, void>
    {
        typedef concepts::Mutable_TimeSeries<Series> series_concept;

        typedef typename series_concept::value_type value_type;
        typedef typename series_concept::run_type run_type;
        typedef typename series_concept::offset_type offset_type;
        typedef typename series_concept::ordered_inserter_type inserter_type;

        /// Initializes \c this->inserter_ to \c range_run_storage::ordered_inserter(series)
        ///
        /// \param series The series into which this inserter writes.
        /// \post <tt>this->offset_ == -inf</tt>
        /// \attention implicit
        ordered_inserter(Series &series)
          : offset_(-inf)
          , inserter_(range_run_storage::ordered_inserter(series))
        {}

        /// Initializes \c this->inserter_ to \c range_run_storage::ordered_inserter(series)
        ///
        /// \param series The series into which this inserter writes.
        /// \param offset The offset of the first run written into this inserter,
        ///     if the offset of that run is not otherwise specified.
        /// \post <tt>this->offset_ == offset</tt>
        ordered_inserter(Series &series, offset_type offset)
          : offset_(offset)
          , inserter_(range_run_storage::ordered_inserter(series))
        {}

        /// \return \c *this
        ordered_inserter &operator *()
        {
            return *this;
        }

        /// \return \c *this
        ordered_inserter &operator ++()
        {
            return *this;
        }

        /// \return \c *this
        ordered_inserter &operator ++(int)
        {
            return *this;
        }

        /// \return \c *this
        ordered_inserter &operator =(ordered_inserter const &)
        {
            return *this;
        }

        /// Same as <tt>(*this)(value)</tt>
        ///
        /// \return \c *this
        ordered_inserter &operator =(value_type const &value)
        {
            return (*this)(value);
        }

        /// Same as <tt>(*this)(tup.first, tup.second)</tt>
        ///
        /// \pre \c Value is implicitly convertible to \c value_type
        /// \pre \c Offset is implicitly convertible to \c offset_type
        /// \return \c *this
        template<typename Value, typename Offset>
        ordered_inserter &operator =(std::pair<Value, Offset> const &tup)
        {
            return (*this)(tup.first, tup.second);
        }

        /// Same as <tt>(*this)(tup.first(), tup.second())</tt>
        ///
        /// \pre \c Value is implicitly convertible to \c value_type
        /// \pre \c Offset is implicitly convertible to \c offset_type
        /// \return \c *this
        template<typename Value, typename Offset>
        ordered_inserter &operator =(compressed_pair<Value, Offset> const &tup)
        {
            return (*this)(tup.first(), tup.second());
        }

        /// Same as <tt>(*this)(get\<0\>(tup), get\<1\>(tup))</tt>
        ///
        /// \pre \c Value is implicitly convertible to \c value_type
        /// \pre \c Offset is implicitly convertible to \c offset_type
        /// \return \c *this
        template<typename Value, typename Offset>
        ordered_inserter &operator =(tuple<Value, Offset> const &tup)
        {
            return (*this)(get<0>(tup), get<1>(tup));
        }

        /// Same as <tt>(*this)(get\<0\>(tup), get\<1\>(tup), get\<2\>(tup))</tt>
        ///
        /// \pre \c Value is implicitly convertible to \c value_type
        /// \pre \c Offset is implicitly convertible to \c offset_type
        /// \return \c *this
        template<typename Value, typename Offset>
        ordered_inserter &operator =(tuple<Value, Offset, Offset> const &tup)
        {
            return (*this)(get<0>(tup), get<1>(tup), get<2>(tup));
        }

        /// \attention This is only necessary because zip_iterator doesn't give us a real tuple.
        /// INTERNAL ONLY
        template<typename Value, typename Offset>
        ordered_inserter &operator =(tuples::cons<Value, tuples::cons<Offset, tuples::null_type> > const &tup)
        {
            return (*this)(get<0>(tup), get<1>(tup));
        }

        /// \attention This is only necessary because zip_iterator doesn't give us a real tuple.
        /// INTERNAL ONLY
        template<typename Value, typename Offset>
        ordered_inserter &operator =(tuples::cons<Value, tuples::cons<Offset, tuples::cons<Offset, tuples::null_type> > > const &tup)
        {
            return (*this)(get<0>(tup), get<1>(tup), get<2>(tup));
        }

        /// Calls <tt>range_run_storage::set_at(this->inserter_, run_type(this->offset_, this->offset_ + 1), value)</tt>
        ///
        /// \pre <tt>-inf != this->offset_</tt>
        /// \post \c this->offset_ is greater by 1.
        /// \return \c *this
        ordered_inserter &operator ()(value_type const &value)
        {
            BOOST_ASSERT(-inf != this->offset_);
            run_type run(this->offset_, this->offset_ + 1);
            range_run_storage::set_at(this->inserter_, run, value);
            ++this->offset_;
            return *this;
        }

        /// Calls <tt>range_run_storage::set_at(this->inserter_, run_type(offset, offset + 1), value)</tt>
        ///
        /// \pre <tt>offset >= this->offset_</tt>
        /// \post <tt>this->offset_ == offset + 1</tt>
        /// \return \c *this
        ordered_inserter &operator ()(value_type const &value, offset_type offset)
        {
            BOOST_ASSERT(offset >= this->offset_);
            run_type run(offset, offset + 1);
            range_run_storage::set_at(this->inserter_, run, value);
            ++(this->offset_ = offset);
            return *this;
        }

        /// Calls <tt>range_run_storage::set_at(this->inserter_, std::make_pair(offset, endoff), value)</tt>
        ///
        /// \pre <tt>offset >= this->offset_</tt>
        /// \post <tt>this->offset_ == endoff</tt>
        /// \return \c *this
        ordered_inserter &operator ()(value_type const &value, offset_type offset, offset_type endoff)
        {
            BOOST_ASSERT(offset >= this->offset_);
            range_run_storage::set_at(this->inserter_, std::make_pair(offset, endoff), value);
            this->offset_ = endoff;
            return *this;
        }

        /// Calls <tt>range_run_storage::commit(this->inserter_)</tt>
        ///
        void commit()
        {
            range_run_storage::commit(this->inserter_);
        }

    #ifndef BOOST_TIME_SERIES_DOXYGEN_INVOKED
    private:
    #endif

        offset_type offset_;        ///< For exposition only
        inserter_type inserter_;    ///< For exposition only
    };


    /// \brief Creates a new <tt>ordered_inserter\<Series\></tt> object.
    ///
    /// Creates a new <tt>ordered_inserter\<Series\></tt> object.
    ///
    /// \param series The series into which this inserter writes.
    /// \param offset The offset of the first run written into the resulting inserter,
    ///     if the offset of that run is not otherwise specified.
    /// \return If \c offset is specified, returns <tt>ordered_inserter<Series>(series, offset)</tt>.
    ///     Otherwise, returns <tt>ordered_inserter<Series>(series)</tt>
    template<typename Series, typename Offset>
    inline ordered_inserter<Series> make_ordered_inserter(Series &series, Offset offset)
    {
        return ordered_inserter<Series>(series, offset);
    }

    /// \overload
    ///
    template<typename Series>
    inline ordered_inserter<Series> make_ordered_inserter(Series &series)
    {
        return ordered_inserter<Series>(series);
    }

}}

// A time_series::ordered_inserter is also a model of
// the OrderedInserter concept.
namespace boost { namespace sequence { namespace impl
{
    /// INTERNAL ONLY
    struct time_series_ordered_inserter_tag;

    /// INTERNAL ONLY
    template<typename Series>
    struct tag<time_series::ordered_inserter<Series> >
    {
        typedef time_series_ordered_inserter_tag type;
    };
}}}

namespace boost { namespace range_run_storage { namespace impl
{
    /// INTERNAL ONLY
    template<typename S, typename R, typename V>
    struct set_at<S, R, V, sequence::impl::time_series_ordered_inserter_tag>
    {
        typedef void result_type;

        void operator ()(S &s, R &r, V &v) const
        {
            s(v, range_run_storage::offset(r), range_run_storage::end_offset(r));
        }
    };

    /// INTERNAL ONLY
    template<typename S>
    struct commit<S, sequence::impl::time_series_ordered_inserter_tag>
    {
        typedef void result_type;

        void operator ()(S &s) const
        {
            s.commit();
        }
    };
}}}

#endif
