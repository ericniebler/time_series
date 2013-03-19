///////////////////////////////////////////////////////////////////////////////
/// \file sparse_array.hpp
/// A sparse array that satisfies the \c InfiniteRangeRunStorage concept
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_SPARSE_ARRAY_EAN_05_24_2006
#define BOOST_TIME_SERIES_STORAGE_SPARSE_ARRAY_EAN_05_24_2006

#include <boost/time_series/time_series_fwd.hpp>

#include <vector>
#include <utility> // for std::pair
#include <boost/detail/construct.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/implicit_cast.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/numeric/functional.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/algorithm/transform.hpp>
#include <boost/range_run_storage/utility/infinite_run.hpp>
#include <boost/time_series/detail/ref_counted_object.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/utility/is_zero.hpp>
#include <boost/range_run_storage/utility/unit_run.hpp>
#include <boost/range_run_storage/traits/is_unit_run.hpp>
#include <boost/time_series/storage/characteristic_array.hpp>
#include <boost/serialization/vector.hpp>

// Define the indices property map needed to make
// sparse_array satisfy the Array concept.
namespace boost { namespace time_series { namespace storage
{
    namespace rrs = range_run_storage;
    using rrs::concepts::Run;

    template<typename Value, typename Offset = std::ptrdiff_t>
    struct sparse_array;

    template<typename Value, typename Offset>
    struct sparse_run_value
      : rrs::unit_run<Offset>
    {
        typedef Value value_type;
        typedef Offset offset_type;

        sparse_run_value()
          : rrs::unit_run<Offset>(0)
          , value()
        {}

        sparse_run_value(Value const &value_, Offset offset)
          : rrs::unit_run<Offset>(offset)
          , value(value_)
        {}

        Value value;
    };

    // sparse_run_transform
    template<typename Offset>
    struct sparse_run_transform
    {
        typedef rrs::unit_run<Offset> const &result_type;

        template<typename RV>
        result_type operator ()(RV const &rv) const
        {
            return rv;
        }
    };

    // sparse_runs_map
    template<typename Offset>
    struct sparse_runs_map
    {
        typedef rrs::unit_run<Offset> const &result_type;

        template<typename K>
        result_type operator ()(K const &k) const
        {
            return *k;
        }
    };

    template<typename Value>
    struct msvc_result_sparse_elements_map
    {
        template<typename K>
        struct result;

        template<typename This, typename K>
        struct result<This(K)>
        {
            typedef Value const &type;
        };

        template<typename This, typename K, typename V>
        struct result<This(K, V)>
        {
            typedef void type;
        };
    };

    // sparse_elements_map
    template<typename Value>
    struct sparse_elements_map
      : msvc_result_sparse_elements_map<Value>
    {
        template<typename K>
        Value const &operator ()(K const &k) const
        {
            return (*k).value;
        }

        template<typename K, typename V>
        void operator ()(K const &k, V const &v) const
        {
            (*k).value = v;
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // sparse_ordered_inserter
    template<typename Value, typename Offset>
    struct sparse_ordered_inserter
    {
        typedef Value value_type;
        typedef Offset offset_type;

        explicit sparse_ordered_inserter(sparse_array<Value, Offset> &that)
          : old_(that)
          , new_(new time_series::detail::ref_counted_object<sparse_array<Value, Offset> >(
                constructors::construct<sparse_array<Value, Offset> >(
                    time_series::zero = rrs::zero(that)
                )
            ))
        {}

        template<typename R, typename V>
        void set_at(R &run, V &value)
        {
            if(!time_series::detail::is_zero(this->new_->get(), value))
            {
                this->set_at_(run, value, rrs::traits::is_unit_run<R>());
            }
        }

        void commit()
        {
            sparse_array<Value, Offset> &that = this->new_->get();
            that.swap(this->old_);
        }

    private:
        template<typename R, typename V>
        void set_at_(R &run, V &value, mpl::true_) // unit_runs
        {
            sparse_array<Value, Offset> &that = this->new_->get();
            typename Run<R>::offset_type off = rrs::offset(run);
            typename Run<R>::offset_type endoff = rrs::end_offset(run);
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
                that.data_.push_back(sparse_run_value<Value, Offset>(value, off));
            }
        }

        template<typename R, typename V>
        void set_at_(R &run, V &value, mpl::false_) // non-unit_runs
        {
            sparse_array<Value, Offset> &that = this->new_->get();
            typename Run<R>::offset_type off = rrs::offset(run);
            typename Run<R>::offset_type endoff = rrs::end_offset(run);
            BOOST_ASSERT(is_float<Offset>::value || off < endoff);
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
                BOOST_ASSERT(is_integral<typename Run<R>::offset_type>::value);
                that.data_.reserve(that.data_.size() + rrs::length(run));
                do
                {
                    that.data_.push_back(sparse_run_value<Value, Offset>(value, off));
                }
                while(++off < endoff);
            }
        }

        sparse_array<Value, Offset> &old_;
        intrusive_ptr<time_series::detail::ref_counted_object<sparse_array<Value, Offset> > > new_;
    };

    // The sparse_array
    template<typename Value, typename Offset>
    struct sparse_array
    {
        typedef Value value_type;
        typedef Offset offset_type;

        // TODO return by reference
        typedef rrs::infinite_run_value<Value, Offset> pre_type;
        typedef rrs::infinite_run_value<Value, Offset> post_type;

        typedef typename pre_type::run_type pre_run_type;
        typedef typename post_type::run_type post_run_type;

        typedef std::vector<sparse_run_value<Value, Offset> > data_type;
        typedef typename data_type::iterator iterator;
        typedef typename data_type::const_iterator const_iterator;

        typedef typename time_series::detail::zero_constant<Value>::const_reference const_reference;
        typedef const_reference reference;

        friend struct sparse_ordered_inserter<Value, Offset>;

        template<typename Args>
        explicit sparse_array(Args const &args)
          : data_()
          , zero_(implicit_cast<Value const &>(args[time_series::zero | numeric::zero<Value>()]))
          , pre_(args[time_series::zero | numeric::zero<Value>()])
          , post_(args[time_series::zero | numeric::zero<Value>()])
        {
            this->pre_.first = this->pre_.second = -inf;
            this->post_.first = this->post_.second = inf;
        }

        iterator begin() { return this->data_.begin(); }
        iterator end() { return this->data_.end(); }
        const_iterator begin() const { return this->data_.begin(); }
        const_iterator end() const { return this->data_.end(); }

        void swap(sparse_array &that)
        {
            using std::swap;
            swap(this->data_, that.data_);
            swap(this->zero_, that.zero_);
            swap(this->pre_, that.pre_);
            swap(this->post_, that.post_);
        }

        reference operator [](offset_type i) const
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
                sparse_run_transform<offset_type> tfx;
                rrs::unit_run<offset_type> tmp(i);

                const_iterator where = sparse_array::find_element(
                    this->data_.begin()
                  , this->data_.end()
                  , i
                );

                if(where != this->data_.end() && !rrs::less(tmp, tfx(*where)))
                {
                    return where->value;
                }
            }

            return this->zero();
        }

        template<typename Run, typename Val>
        void set_at(Run &run, Val &val)
        {
            // TODO: find more efficient implementation.
            Value const &v(val), &z(this->zero());
            sparse_ordered_inserter<Value, Offset> o(*this);
            rrs::transform(as_characteristic(run, v, z), *this, rrs::_1, rrs::_1, rrs::_1, o);
            rrs::commit(o);
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
            ar & this->data_;
            ar & this->zero_;
            ar & this->pre_;
            ar & this->post_;
        }

        template<typename Iter>
        static Iter find_element(Iter begin, Iter end, Offset offset)
        {
            sparse_run_transform<Offset> tfx;
            return std::lower_bound(
                make_transform_iterator(begin, tfx)
              , make_transform_iterator(end, tfx)
              , rrs::unit_run<Offset>(offset)
              , rrs::less
            ).base();
        }

        data_type data_;
        Value zero_;
        pre_type pre_;
        post_type post_;
    };

    template<typename Value, typename Offset>
    void swap(sparse_array<Value, Offset> &left, sparse_array<Value, Offset> &right)
    {
        left.swap(right);
    }
}}}

namespace boost
{
    namespace sequence { namespace impl
    {
        struct sparse_array_tag;
        struct sparse_ordered_inserter_tag;

        template<typename Value, typename Offset>
        struct tag<time_series::storage::sparse_array<Value, Offset> >
        {
            typedef sparse_array_tag type;
        };

        template<typename Value, typename Offset>
        struct tag<time_series::storage::sparse_ordered_inserter<Value, Offset> >
        {
            typedef sparse_ordered_inserter_tag type;
        };

        template<typename S>
        struct elements<S, sparse_array_tag>
        {
            typedef time_series::storage::sparse_elements_map<
                typename S::value_type
            > result_type;

            result_type operator ()(S &) const
            {
                return result_type();
            }
        };
    }}

    namespace range_run_storage { namespace impl
    {
        template<typename S>
        struct runs<S, sequence::impl::sparse_array_tag>
        {
            typedef typename S::offset_type offset_type;
            typedef time_series::storage::sparse_runs_map<offset_type> result_type;

            result_type operator ()(S &) const
            {
                return result_type();
            }
        };

        template<typename S, typename I>
        struct get_at<S, I, sequence::impl::sparse_array_tag>
        {
            typedef typename S::const_reference result_type;

            result_type operator ()(S &s, I &i) const
            {
                return s[i];
            }
        };

        template<typename S>
        struct zero<S, sequence::impl::sparse_array_tag>
        {
            typedef typename S::reference result_type;

            result_type operator ()(S &s) const
            {
                return s.zero();
            }
        };

        template<typename S, typename V>
        struct set_zero<S, V, sequence::impl::sparse_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                return s.set_zero(v);
            }
        };

        template<typename S>
        struct pre_run<S, sequence::impl::sparse_array_tag>
        {
            typedef typename S::pre_run_type result_type;

            result_type operator ()(S &s) const
            {
                return s.pre();
            }
        };

        template<typename S>
        struct pre_value<S, sequence::impl::sparse_array_tag>
        {
            typedef typename S::value_type const &result_type;

            result_type operator ()(S &s) const
            {
                return s.pre().value;
            }
        };

        template<typename S, typename V>
        struct set_pre_value<S, V, sequence::impl::sparse_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                s.pre().value = v;
            }
        };

        template<typename S>
        struct post_run<S, sequence::impl::sparse_array_tag>
        {
            typedef typename S::post_run_type result_type;

            result_type operator ()(S &s) const
            {
                return s.post();
            }
        };

        template<typename S>
        struct post_value<S, sequence::impl::sparse_array_tag>
        {
            typedef typename S::value_type const &result_type;

            result_type operator ()(S &s) const
            {
                return s.post().value;
            }
        };

        template<typename S, typename V>
        struct set_post_value<S, V, sequence::impl::sparse_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                s.post().value = v;
            }
        };

        template<typename S>
        struct ordered_inserter<S, sequence::impl::sparse_array_tag>
        {
            typedef time_series::storage::sparse_ordered_inserter<
                typename S::value_type
              , typename S::offset_type
            > result_type;

            result_type operator ()(S &s) const
            {
                return result_type(s);
            }
        };

        template<typename S>
        struct commit<S, sequence::impl::sparse_ordered_inserter_tag>
        {
            typedef void result_type;

            void operator ()(S &s) const
            {
                return s.commit();
            }
        };
    }}
}

namespace boost { namespace time_series { namespace traits
{
    template<typename Value, typename Offset>
    struct storage_category<storage::sparse_array<Value, Offset> >
    {
        typedef sparse_storage_tag type;
    };
}}}

namespace boost { namespace constructors { namespace impl
{
    template<typename Value, typename Offset>
    struct tag<time_series::storage::sparse_array<Value, Offset> >
    {
        typedef sequence::impl::sparse_array_tag type;
    };

    template<typename T>
    struct construct<T, sequence::impl::sparse_array_tag>
      : arg_pack_construct
    {
        typedef parameter::parameters<
            parameter::optional<time_series::tag::zero>
        > args_type;
    };

}}}

namespace boost { namespace serialization
{
    template<typename Archive, typename Value, typename Offset>
    void serialize(Archive &ar, time_series::storage::sparse_run_value<Value, Offset> & rv, unsigned int const version)
    {
        ar & static_cast<range_run_storage::unit_run<Offset> &>(rv);
        ar & rv.value;
    }
}}

#endif
