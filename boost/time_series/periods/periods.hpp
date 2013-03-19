///////////////////////////////////////////////////////////////////////////////
/// \file periods.hpp
/// A periods<> class that models RandomAccessRange
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_PERIODS_PERIODS_HPP_EAN_10_06_2006
#define BOOST_TIME_SERIES_PERIODS_PERIODS_HPP_EAN_10_06_2006

#include <cstddef>
#include <boost/assert.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

namespace boost { namespace time_series
{

    /// \brief A \c RandomAccessRange that generates regularly sized
    /// periods from a starting offset.
    ///
    /// A \c RandomAccessRange that generates regularly sized
    /// periods from a starting offset.
    ///
    template<typename Offset = std::ptrdiff_t>
    struct periods
    {
        typedef Offset value_type;
        typedef Offset const reference;
        typedef Offset const const_reference;
        typedef Offset size_type;

        struct iterator
          : iterator_adaptor<iterator, counting_iterator<Offset> >
        {
            typedef iterator_adaptor<iterator, counting_iterator<Offset> > super_t;

            iterator()
              : super_t(), skip_(1)
            {}

            iterator(Offset start, Offset skip)
              : super_t(make_counting_iterator(start)), skip_(skip)
            {}

        private:
            friend class boost::iterator_core_access;

            void increment()
            {
                this->base_reference() += this->skip_;
            }

            void decrement()
            {
                this->base_reference() -= this->skip_;
            }

            void advance(typename super_t::difference_type n)
            {
                this->base_reference() += n * this->skip_;
            }

            typename super_t::difference_type distance_to(iterator const &that) const
            {
                BOOST_ASSERT(0 == (that.base() - this->base()) % this->skip_);
                return (that.base() - this->base()) / this->skip_;
            }

            Offset skip_;
        };

        typedef iterator const_iterator;

        explicit periods(Offset stop = Offset(0))
          : start_(0), stop_(stop + 1), skip_(1)
        {
            BOOST_ASSERT(0 <= stop);
        }

        periods(Offset start, Offset stop)
          : start_(start), stop_(stop + 1), skip_(1)
        {
            BOOST_ASSERT(start <= stop);
        }

        periods(Offset start, Offset stop, Offset skip)
          : start_(start), stop_(stop + skip), skip_(skip)
        {
            BOOST_ASSERT(start <= stop);
            if(0 != (stop - start) % skip)
            {
                this->stop_ = start + ((stop - start) / skip + 1) * skip + skip;
            }
        }

        iterator begin() const
        {
            return iterator(this->start_, this->skip_);
        }

        iterator end() const
        {
            return iterator(this->stop_, this->skip_);
        }

        size_type size() const
        {
            return this->end() - this->begin();
        }

        reference operator[](Offset n) const
        {
            return *(this->begin() + n);
        }

    private:
        Offset start_, stop_, skip_;
    };

}}

#endif
