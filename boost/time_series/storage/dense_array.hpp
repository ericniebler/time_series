///////////////////////////////////////////////////////////////////////////////
/// \file dense_array.hpp
/// A dense array that satisfies the \c InfiniteRangeRunStorage concept
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_DENSE_ARRAY_EAN_04_26_2006
#define BOOST_TIME_SERIES_STORAGE_DENSE_ARRAY_EAN_04_26_2006

#include <boost/time_series/time_series_fwd.hpp>
#include <vector>
#include <boost/implicit_cast.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/detail/construct.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/difference1.hpp>
#include <boost/range_run_storage/utility/infinite_run.hpp>
#include <boost/range_run_storage/algorithm/transform.hpp>
#include <boost/time_series/detail/ref_counted_object.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/storage/detail/offset_runs.hpp>
#include <boost/time_series/storage/detail/indirect_copy_and_swap_inserter.hpp>
#include <boost/time_series/storage/characteristic_array.hpp>
#include <boost/serialization/vector.hpp>

namespace boost { namespace time_series { namespace storage
{
    namespace rrs = range_run_storage;
    using rrs::concepts::Run;

    template<typename Value>
    struct dense_array;

    ///////////////////////////////////////////////////////////////////////////////
    // dense_ordered_inserter
    template<typename Value>
    struct dense_ordered_inserter
    {
        typedef Value value_type;

        explicit dense_ordered_inserter(dense_array<Value> &that)
          : old_(that)
          , new_(new time_series::detail::ref_counted_object<dense_array<Value> >(
                constructors::construct<dense_array<Value> >(
                    time_series::start = static_cast<std::ptrdiff_t>(inf)
                  , time_series::zero = rrs::zero(that)
                )
            ))
        {}

        template<typename R, typename V>
        void set_at(R &run, V &value)
        {
            dense_array<Value> &that = this->new_->get();
            typename Run<R>::offset_type off = rrs::offset(run);
            typename Run<R>::offset_type endoff = rrs::end_offset(run);
            BOOST_ASSERT(off < endoff);
            if(off == -inf)
            {
                that.pre_.set(run, value);
            }
            else if(endoff == inf)
            {
                that.post_.set(run, value);
            }
            else
            {
                that.offset_ = std::min<std::ptrdiff_t>(that.offset_, off);
                that.reserve(endoff - that.offset_);
                that.resize(off - that.offset_, implicit_cast<Value const &>(rrs::zero(that)));
                that.resize(endoff - that.offset_, value);
            }
        }

        void commit()
        {
            dense_array<Value> &that = this->new_->get();
            that.swap(this->old_);
        }

    private:
        dense_array<Value> &old_;
        intrusive_ptr<time_series::detail::ref_counted_object<dense_array<Value> > > new_;
    };

    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////////////
        // get_size
        inline std::size_t get_size(numeric::zero<std::ptrdiff_t>, std::ptrdiff_t)
        {
            return 0;
        }

        inline std::size_t get_size(std::ptrdiff_t stop, std::ptrdiff_t start)
        {
            return stop - start;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // dense_array
    template<typename Value>
    struct dense_array
      : private std::vector<Value>
    {
        // TODO return by reference
        typedef rrs::infinite_run_value<Value> pre_type;
        typedef rrs::infinite_run_value<Value> post_type;

        typedef typename pre_type::run_type pre_run_type;
        typedef typename post_type::run_type post_run_type;

        typedef typename std::vector<Value>::size_type size_type;
        typedef typename std::vector<Value>::value_type value_type;
        typedef typename std::vector<Value>::iterator iterator;
        typedef typename std::vector<Value>::const_iterator const_iterator;

        typedef typename std::vector<Value>::const_reference const_reference;
        typedef typename std::vector<Value>::const_reference reference;

        friend struct dense_ordered_inserter<Value>;

        template<typename Args>
        explicit dense_array(Args const &args)
          : std::vector<Value>(
                detail::get_size(args[time_series::stop | numeric::zero<std::ptrdiff_t>()], args[time_series::start | 0])
              , args[time_series::value | args[time_series::zero | numeric::zero<Value>()]]
            )
          , offset_(args[time_series::start | 0])
          , zero_(implicit_cast<Value const &>(args[time_series::zero | numeric::zero<Value>()]))
          , pre_(args[time_series::zero | numeric::zero<Value>()])
          , post_(args[time_series::zero | numeric::zero<Value>()])
        {
            this->pre_.first = this->pre_.second = -inf;
            this->post_.first = this->post_.second = inf;
        }

        using std::vector<Value>::begin;
        using std::vector<Value>::end;
        using std::vector<Value>::size;
        using std::vector<Value>::empty;
        using std::vector<Value>::max_size;

        reference operator [](std::ptrdiff_t i) const
        {
            if(i < this->pre_.second)
            {
                return this->pre_.value;
            }
            else if(i >= this->post_.first)
            {
                return this->post_.value;
            }
            else
            {
                i -= this->offset_;

                if(i >= 0 && static_cast<size_type>(i) < this->base().size())
                {
                    return this->base()[i];
                }
            }

            return this->zero();
        }

        void swap(dense_array<Value> &that)
        {
            using std::swap;
            this->base().swap(that);
            swap(this->offset_, that.offset_);
            swap(this->zero_, that.zero_);
            swap(this->pre_, that.pre_);
            swap(this->post_, that.post_);
        }

        template<typename Run, typename Val>
        void set_at(Run &run, Val &val)
        {
            Value const &v(val), &z(this->zero());
            if(rrs::offset(run) >= this->offset() && rrs::end_offset(run) <= this->end_offset())
            {
                std::ptrdiff_t off = rrs::offset(run) - this->offset_;
                std::ptrdiff_t endoff = rrs::end_offset(run) - this->offset_;
                std::fill(this->begin() + off, this->begin() + endoff, v);
            }
            else
            {
                dense_ordered_inserter<Value> o(*this);
                rrs::transform(as_characteristic(run, v, z), *this, rrs::_1, rrs::_1, rrs::_1, o);
                rrs::commit(o);
            }
        }

        std::ptrdiff_t offset() const
        {
            return this->offset_;
        }

        std::ptrdiff_t end_offset() const
        {
            return this->offset_ + this->base().size();
        }

        reference zero() const
        {
            return this->zero_;
        }

        void set_zero(reference z)
        {
            this->zero_ = z;
        }

        pre_type &pre() { return this->pre_; }
        post_type &post() { return this->post_; }
        pre_type const &pre() const { return this->pre_; }
        post_type const &post() const { return this->post_; }

    private:
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive &ar, unsigned int const version)
        {
            ar & this->base();
            ar & this->offset_;
            ar & this->zero_;
            ar & this->pre_;
            ar & this->post_;
        }

        std::vector<Value> &base() { return *this; }
        std::vector<Value> const &base() const { return *this; }

        std::ptrdiff_t offset_;
        Value zero_;
        pre_type pre_;
        post_type post_;
    };

    template<typename Value>
    void swap(dense_array<Value> &left, dense_array<Value> &right)
    {
        left.swap(right);
    }

}}}

namespace boost
{
    namespace sequence { namespace impl
    {
        struct dense_array_tag;
        struct dense_ordered_inserter_tag;

        template<typename T>
        struct tag<time_series::storage::dense_array<T> >
        {
            typedef dense_array_tag type;
        };

        template<typename T>
        struct tag<time_series::storage::dense_ordered_inserter<T> >
        {
            typedef dense_ordered_inserter_tag type;
        };
    }}

    namespace range_run_storage { namespace impl
    {
        template<typename S>
        struct runs<S, sequence::impl::dense_array_tag>
        {
            typedef typename sequence::concepts::Sequence<S>::cursor cursor_type;
            typedef difference1<cursor_type> difference_runs_type;
            typedef time_series::storage::offset_runs<difference_runs_type, std::ptrdiff_t> result_type;

            result_type operator ()(S &s) const
            {
                return result_type(difference_runs_type(sequence::begin(s)), s.offset());
            }
        };

        template<typename S, typename I>
        struct get_at<S, I, sequence::impl::dense_array_tag>
        {
            typedef typename S::const_reference result_type;

            result_type operator ()(S &s, I &i) const
            {
                return s[i];
            }
        };

        template<typename S>
        struct zero<S, sequence::impl::dense_array_tag>
        {
            typedef typename S::reference result_type;

            result_type operator ()(S &s) const
            {
                return s.zero();
            }
        };

        template<typename S, typename V>
        struct set_zero<S, V, sequence::impl::dense_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                return s.set_zero(v);
            }
        };

        template<typename S>
        struct pre_run<S, sequence::impl::dense_array_tag>
        {
            typedef typename S::pre_run_type result_type;

            result_type operator ()(S &s) const
            {
                return s.pre();
            }
        };

        template<typename S>
        struct pre_value<S, sequence::impl::dense_array_tag>
        {
            typedef typename mpl::if_<
                is_const<S>
              , typename S::value_type const &
              , typename S::value_type &
            >::type result_type;

            result_type operator ()(S &s) const
            {
                return s.pre().value;
            }
        };

        template<typename S, typename V>
        struct set_pre_value<S, V, sequence::impl::dense_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                s.pre().value = v;
            }
        };

        template<typename S>
        struct post_run<S, sequence::impl::dense_array_tag>
        {
            typedef typename S::post_run_type result_type;

            result_type operator ()(S &s) const
            {
                return s.post();
            }
        };

        template<typename S>
        struct post_value<S, sequence::impl::dense_array_tag>
        {
            typedef typename mpl::if_<
                is_const<S>
              , typename S::value_type const &
              , typename S::value_type &
            >::type result_type;

            result_type operator ()(S &s) const
            {
                return s.post().value;
            }
        };

        template<typename S, typename V>
        struct set_post_value<S, V, sequence::impl::dense_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                s.post().value = v;
            }
        };

        template<typename S>
        struct ordered_inserter<S, sequence::impl::dense_array_tag>
        {
            typedef time_series::storage::dense_ordered_inserter<typename S::value_type> result_type;

            result_type operator ()(S &s) const
            {
                return result_type(s);
            }
        };

        template<typename S>
        struct commit<S, sequence::impl::dense_ordered_inserter_tag>
        {
            typedef void result_type;

            void operator ()(S &s) const
            {
                return s.commit();
            }
        };
    }}

    namespace time_series { namespace traits
    {
        template<typename Value>
        struct storage_category<storage::dense_array<Value> >
        {
            typedef dense_storage_tag type;
        };
    }}
}

namespace boost { namespace constructors { namespace impl
{
    template<typename Value>
    struct tag<time_series::storage::dense_array<Value> >
    {
        typedef sequence::impl::dense_array_tag type;
    };

    template<typename T>
    struct construct<T, sequence::impl::dense_array_tag>
      : arg_pack_construct
    {
        typedef parameter::parameters<
            parameter::optional<time_series::tag::start>
          , parameter::optional<time_series::tag::stop>
          , parameter::optional<time_series::tag::value>
          , parameter::optional<time_series::tag::zero>
        > args_type;
    };

}}}

#endif
