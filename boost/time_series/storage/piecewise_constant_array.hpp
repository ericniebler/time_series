///////////////////////////////////////////////////////////////////////////////
/// \file piecewise_constant_array.hpp
/// A piecewise_constant array that satisfies the \c InfiniteRangeRunStorage concept
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_PIECEWISE_CONSTANT_ARRAY_EAN_05_22_2006
#define BOOST_TIME_SERIES_STORAGE_PIECEWISE_CONSTANT_ARRAY_EAN_05_22_2006

#include <boost/time_series/time_series_fwd.hpp>
#include <vector>
#include <utility> // for std::pair
#include <boost/detail/construct.hpp>
#include <boost/implicit_cast.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/numeric/functional.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/time_series/detail/ref_counted_object.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/utility/is_zero.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/utility/subrun.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>

// Define the indices property map needed to make
// piecewise_constant_array satisfy the Array concept.
namespace boost { namespace time_series { namespace storage
{
    namespace rrs = range_run_storage;

    template<typename Value, typename Offset = std::ptrdiff_t>
    struct piecewise_constant_array;

    template<typename Value, typename Offset = std::ptrdiff_t>
    struct piecewise_run_value
      : std::pair<Offset, Offset>
    {
        typedef Value value_type;
        typedef Offset offset_type;

        piecewise_run_value()
          : std::pair<Offset, Offset>(0, 0)
          , value()
        {}

        piecewise_run_value(Value const &value, Offset offset, Offset end_offset)
          : std::pair<Offset, Offset>(offset, end_offset)
          , value(value)
        {}

        Value value;
    };

    // piecewise_run_transform
    template<typename Offset = std::ptrdiff_t>
    struct piecewise_run_transform
    {
        typedef std::pair<Offset, Offset> const &result_type;

        template<typename RV>
        result_type operator ()(RV const &rv) const
        {
            return rv;
        }
    };

    // piecewise_runs_map
    template<typename Offset = std::ptrdiff_t>
    struct piecewise_runs_map
    {
        typedef std::pair<Offset, Offset> const &result_type;

        template<typename K>
        result_type operator ()(K const &k) const
        {
            return *k;
        }
    };

    template<typename Value>
    struct msvc_result_piecewise_elements_map
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

    // piecewise_runs_map
    template<typename Value>
    struct piecewise_elements_map
      : msvc_result_piecewise_elements_map<Value>
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

    // piecewise_ordered_inserter
    template<typename Value, typename Offset = std::ptrdiff_t>
    struct piecewise_ordered_inserter
    {
        typedef std::pair<Offset, Offset> run_type;
        typedef Value value_type;

        explicit piecewise_ordered_inserter(piecewise_constant_array<Value, Offset> &array)
          : array_(array)
          , new_runs_(new time_series::detail::ref_counted_object<runs_type>())
        {
            this->new_runs_->get().reserve(this->array_.data_.capacity());
        }

        template<typename R, typename V>
        void set_at(R &run, V &value)
        {
            if(!time_series::detail::is_zero(this->array_, value))
            {
                typedef typename rrs::concepts::Run<R>::offset_type that_offset_type;
                that_offset_type offset = rrs::offset(run);
                that_offset_type endoff = rrs::end_offset(run);
                BOOST_ASSERT(is_float<Offset>::value || offset < endoff);
                if(!this->new_runs_->get().empty())
                {
                    piecewise_run_value<Value, Offset> &back = this->new_runs_->get().back();
                    if(offset == rrs::end_offset(back) && numeric::promote<bool>(value == back.value))
                    {
                        back.second = endoff;
                        return;
                    }
                }
                piecewise_run_value<Value, Offset> rv(value, offset, endoff);
                this->new_runs_->get().push_back(rv);
            }
        }

        void commit()
        {
            this->new_runs_->get().swap(this->array_.data_);
        }

    private:
        typedef std::vector<piecewise_run_value<Value, Offset> > runs_type;
        piecewise_constant_array<Value, Offset> &array_;
        intrusive_ptr<time_series::detail::ref_counted_object<runs_type> > new_runs_;
    };

    // The piecewise_constant_array
    template<typename Value, typename Offset>
    struct piecewise_constant_array
    {
        typedef Value value_type;
        typedef Offset offset_type;
        typedef std::pair<Offset, Offset> piecewise_run;

        typedef std::vector<piecewise_run_value<Value, Offset> > data_type;
        typedef typename data_type::iterator iterator;
        typedef typename data_type::const_iterator const_iterator;

        typedef Value const &const_reference;
        typedef const_reference reference;

        friend struct piecewise_ordered_inserter<Value, Offset>;

        template<typename Args>
        explicit piecewise_constant_array(Args const &args)
          : data_()
          , zero_(implicit_cast<Value const &>(args[time_series::zero | numeric::zero<Value>()]))
        {}

        iterator begin()
        {
            return this->data_.begin() + (this->data_.size() > 0u);
        }

        iterator end()
        {
            return this->data_.end() - (this->data_.size() > 1u);
        }

        const_iterator begin() const
        {
            return this->data_.begin() + (this->data_.size() > 0u);
        }

        const_iterator end() const
        {
            return this->data_.end() - (this->data_.size() > 1u);
        }

        void swap(piecewise_constant_array &that)
        {
            using std::swap;
            swap(this->data_, that.data_);
            swap(this->zero_, that.zero_);
        }

        reference operator [](Offset offset) const
        {
            piecewise_run_transform<Offset> tfx;
            rrs::unit_run<Offset> tmp(offset);

            const_iterator where = piecewise_constant_array::find_element(
                this->data_.begin()
              , this->data_.end()
              , offset
            );

            if(where == this->data_.end() || rrs::less(tmp, tfx(*where)))
            {
                return this->zero();
            }

            return where->value;
        }

        template<typename R>
        void set_at(R const &run, Value const &value)
        {
            // BUGBUG this doesn't handle the case when value == 0
            piecewise_run_transform<Offset> tfx;

            typedef typename rrs::concepts::Run<R>::offset_type that_offset_type;
            that_offset_type offset = rrs::offset(run);
            that_offset_type endoff = rrs::end_offset(run);

            rrs::unit_run<Offset> tmp(offset);
            piecewise_run_value<Value, Offset> rv(value, offset, endoff);

            typedef typename std::vector<piecewise_run_value<Value, Offset> >::iterator iterator;
            iterator where = piecewise_constant_array::find_element(
                this->data_.begin()
              , this->data_.end()
              , offset
            );

            if(where == this->data_.end() || rrs::less(tmp, tfx(*where)))
            {
                where = this->data_.insert(where, rv);
            }
            else if(numeric::promote<bool>(value == where->value))
            {
                where->second = (std::max)(where->second, endoff);
            }
            else if(where->first == offset)
            {
                where = this->data_.insert(where, rv);
            }
            else
            {
                where->second = offset;
                where = this->data_.insert(where + 1, rv);
            }

            endoff = where->second;
            iterator wend = where + 1;
            for(; wend != this->data_.end() && endoff >= rrs::end_offset(*wend); ++wend)
                {}
            wend = this->data_.erase(where + 1, wend);

            if(wend != this->data_.end() && endoff > wend->first)
            {
                if(numeric::promote<bool>(where->value == wend->value))
                {
                    where->second = wend->second;
                    wend = this->data_.erase(wend);
                }
                else
                {
                    wend->first = endoff;
                }
            }
        }

        reference zero() const
        {
            return this->zero_;
        }

        void set_zero(reference z)
        {
            this->zero_ = z;
        }

        data_type &data()
        {
            return this->data_;
        }

        data_type const &data() const
        {
            return this->data_;
        }

    private:
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive &ar, unsigned int const version)
        {
            ar & this->data_;
            ar & this->zero_;
        }

        template<typename Iter>
        static Iter find_element(Iter begin, Iter end, Offset offset)
        {
            piecewise_run_transform<Offset> tfx;
            return std::lower_bound(
                make_transform_iterator(begin, tfx)
              , make_transform_iterator(end, tfx)
              , rrs::unit_run<Offset>(offset)
              , rrs::less
            ).base();
        }

        data_type data_;
        Value zero_;
    };

    template<typename Value, typename Offset>
    void swap(
        piecewise_constant_array<Value, Offset> &left
      , piecewise_constant_array<Value, Offset> &right
    )
    {
        left.swap(right);
    }
}}}

namespace boost
{
    namespace sequence { namespace impl
    {
        struct piecewise_constant_array_tag;
        struct piecewise_ordered_inserter_tag;

        template<typename Value, typename Offset>
        struct tag<time_series::storage::piecewise_constant_array<Value, Offset> >
        {
            typedef piecewise_constant_array_tag type;
        };

        template<typename Value, typename Offset>
        struct tag<time_series::storage::piecewise_ordered_inserter<Value, Offset> >
        {
            typedef piecewise_ordered_inserter_tag type;
        };

        template<typename S>
        struct elements<S, piecewise_constant_array_tag>
        {
            typedef time_series::storage::piecewise_elements_map<
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
        struct runs<S, sequence::impl::piecewise_constant_array_tag>
        {
            typedef time_series::storage::piecewise_runs_map<
                typename S::offset_type
            > result_type;

            result_type operator ()(S &) const
            {
                return result_type();
            }
        };

        template<typename S, typename I>
        struct get_at<S, I, sequence::impl::piecewise_constant_array_tag>
        {
            typedef typename S::const_reference result_type;

            result_type operator ()(S &s, I &i) const
            {
                return s[i];
            }
        };

        template<typename S>
        struct zero<S, sequence::impl::piecewise_constant_array_tag>
        {
            typedef typename S::reference result_type;

            result_type operator ()(S &s) const
            {
                return s.zero();
            }
        };

        template<typename S, typename V>
        struct set_zero<S, V, sequence::impl::piecewise_constant_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                return s.set_zero(v);
            }
        };

        template<typename S>
        struct pre_run<S, sequence::impl::piecewise_constant_array_tag>
        {
            typedef typename S::offset_type offset_type;
            typedef std::pair<offset_type, offset_type> result_type;

            result_type operator ()(S &s) const
            {
                return s.data().empty()
                  ? result_type(-inf, -inf)
                  : static_cast<result_type>(s.data().front());
            }
        };

        template<typename S>
        struct pre_value<S, sequence::impl::piecewise_constant_array_tag>
        {
            typedef typename S::const_reference result_type;

            result_type operator ()(S &s) const
            {
                if(s.data().empty())
                    return s.zero();
                else
                    return s.data().front().value;
            }
        };

        template<typename S, typename V>
        struct set_pre_value<S, V, sequence::impl::piecewise_constant_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                if(!s.data().empty())
                {
                    s.data().front().value = v;
                }
            }
        };

        template<typename S>
        struct post_run<S, sequence::impl::piecewise_constant_array_tag>
        {
            typedef typename S::offset_type offset_type;
            typedef std::pair<offset_type, offset_type> result_type;

            result_type operator ()(S &s) const
            {
                return s.data().size() <= 1u
                  ? result_type(inf, inf)
                  : static_cast<result_type>(s.data().back());
            }
        };

        template<typename S>
        struct post_value<S, sequence::impl::piecewise_constant_array_tag>
        {
            typedef typename S::const_reference result_type;

            result_type operator ()(S &s) const
            {
                if(s.data().size() <= 1u)
                    return s.zero();
                else
                    return s.data().back().value;
            }
        };

        template<typename S, typename V>
        struct set_post_value<S, V, sequence::impl::piecewise_constant_array_tag>
        {
            typedef void result_type;

            void operator ()(S &s, V &v) const
            {
                if(s.data().size() > 1u)
                {
                    s.data().back().value = v;
                }
            }
        };

        template<typename S>
        struct ordered_inserter<S, sequence::impl::piecewise_constant_array_tag>
        {
            typedef time_series::storage::piecewise_ordered_inserter<
                typename S::value_type
              , typename S::offset_type
            > result_type;

            result_type operator ()(S &s) const
            {
                return result_type(s);
            }
        };

        template<typename S>
        struct commit<S, sequence::impl::piecewise_ordered_inserter_tag>
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
    struct storage_category<storage::piecewise_constant_array<Value, Offset> >
    {
        typedef piecewise_constant_storage_tag type;
    };
}}}

namespace boost { namespace constructors { namespace impl
{
    template<typename Value, typename Offset>
    struct tag<time_series::storage::piecewise_constant_array<Value, Offset> >
    {
        typedef sequence::impl::piecewise_constant_array_tag type;
    };

    template<typename T>
    struct construct<T, sequence::impl::piecewise_constant_array_tag>
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
    void serialize(Archive &ar, time_series::storage::piecewise_run_value<Value, Offset> & rv, unsigned int const version)
    {
        ar & static_cast<std::pair<Offset, Offset> &>(rv);
        ar & rv.value;
    }
}}

#endif
