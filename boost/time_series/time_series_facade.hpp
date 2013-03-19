///////////////////////////////////////////////////////////////////////////////
/// \file time_series_facade.hpp
/// Includes all of the Time_Series Library
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_TIME_SERIES_FACADE_HPP_EAN_04_18_2006
#define BOOST_TIME_SERIES_TIME_SERIES_FACADE_HPP_EAN_04_18_2006

#include <iosfwd>
#include <boost/integer.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/same_as.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/numeric/functional.hpp>
#include <boost/time_series/time_series_fwd.hpp>
#include <boost/time_series/concepts.hpp>
#include <boost/time_series/traits/is_time_series.hpp>
#include <boost/time_series/traits/conversion.hpp>
#include <boost/time_series/traits/offset_type.hpp>
#include <boost/time_series/traits/storage_category.hpp>
#include <boost/time_series/traits/discretization_type.hpp>
#include <boost/time_series/traits/generate_series.hpp>
#include <boost/time_series/utility/time_series_base.hpp>
#include <boost/range_run_storage/concepts.hpp>
#include <boost/range_run_storage/algorithm/copy.hpp>
#include <boost/range_run_storage/algorithm/for_each.hpp>
#include <boost/range_run_storage/algorithm/transform.hpp>
#include <boost/detail/construct.hpp>

// Forward declaration
namespace boost { namespace serialization
{
    class access;
}}

namespace boost { namespace time_series
{
    namespace detail
    {
        // For accessing the private .data() member function of a
        // time_series_facade.
        struct time_series_facade_access
        {
            template<typename S>
            static typename S::storage_type &data(S &s)
            {
                return s.data();
            }

            template<typename S>
            static typename S::storage_type const &data(S const &s)
            {
                return s.data();
            }
        };
    }

    /// \brief Represents a series of observations at regular intervals.
    ///
    /// \c time_series_facade\<\> is used to make a model of the \c InfiniteRangeRunStorage
    /// concept a model of the \c TimeSeries concept by providing a discretization, and also
    /// to provide a consistent interface constisting of additional helper functions not
    /// part of the \c TimeSeries concept.
    template<
        typename Derived
      , typename Storage
      , typename Discretization
    >
    struct time_series_facade
      : time_series_base<Derived>
    {
        typedef Storage storage_type;
        typedef Discretization discretization_type;
        typedef range_run_storage::concepts::InfiniteRangeRunStorage<Storage> storage_concept;
        typedef typename storage_concept::value_type value_type;
        typedef typename storage_concept::offset_type offset_type;
        typedef typename storage_concept::zero_type zero_type;

        typedef 
            typename result_of<
                range_run_storage::op::get_at(storage_type &, offset_type &)
            >::type
        reference;

        typedef reference const_reference;

        /// Construct a \c time_series_facade\<\> from an \c ArgumentPack.
        ///
        /// Forwards the argument pack to the constructor of the underlying
        ///     \c InfiniteRangeRunStorage data member, after filtering out the
        ///     discretization named parameter.
        ///
        /// \param args A model of \c ArgumentPack
        template<typename Args>
        explicit time_series_facade(Args const &args)
          : data_(constructors::forward_construct_filter<Storage, mpl::not_same_as<tag::discretization> >(args))
          , discretization_(args[time_series::discretization | numeric::one_or_empty<discretization_type>::value])
        {}

        /// Return the value of the series at a particular offset.
        ///
        /// \param n The offset.
        /// \return The result of calling \c range_run_storage::get_at() with the
        ///     underlying \c InfiniteRangeRunStorage and \c n
        reference operator [](offset_type n) const
        {
            return range_run_storage::get_at(this->data(), n);
        }

        /// \return The value of the series' discretization.
        ///
        discretization_type discretization() const
        {
            return this->discretization_;
        }

        /// \brief Sets the value of the series' discretization.
        ///
        void discretization(discretization_type disc)
        {
            this->discretization_ = disc;
        }

        /// Assign from a model of the \c TimeSeries concept.
        ///
        /// \param that The series to assign from.
        /// \pre Series is a model of the \c TimeSeries concept.
        /// \pre <tt>this->discretization() == that.discretization()</tt>
        /// \pre There is a legal series conversion from \c Series to
        ///     \c Derived. (Eg., A delta series cannot represent a dense 
        ///     series in general.)
        /// \pre \c Storage is a model of \c Mutable_InfiniteRangeRunStorage
        template<typename Series>
        BOOST_CONCEPT_REQUIRES(
            ((concepts::TimeSeries<Series>)),
        (typename disable_if<is_same<Series, Derived>, Derived &>::type))
        operator =(Series const &that)
        {
            namespace rrs = range_run_storage;
            typedef rrs::concepts::Mutable_InfiniteRangeRunStorage<Storage> storage_concept;
            BOOST_MPL_ASSERT((traits::is_time_series<Series>));
            BOOST_MPL_ASSERT((traits::is_series_convertible<Series, Derived>));
            BOOST_ASSERT(this->discretization() == that.discretization());

            typename storage_concept::ordered_inserter_type
                out(rrs::ordered_inserter(this->data()));

            rrs::commit(rrs::copy(that, out));
            return this->cast();
        }

        /// Swaps two \c time_series_facade\<\> objects.
        ///
        /// \param that The \c time_series_facade\<\> object to swap with.
        void swap(time_series_facade<Derived, Storage, Discretization> &that)
        {
            using std::swap;
            swap(this->data_, that.data_);
            swap(this->discretization_, that.discretization_);
        }

    private:
        friend class boost::serialization::access;
        friend struct detail::time_series_facade_access;

        storage_type &data()
        {
            return this->data_;
        }

        storage_type const &data() const
        {
            return this->data_;
        }

        template<typename Archive>
        void serialize(Archive & ar, unsigned int const version)
        {
            ar & this->discretization_;
            ar & this->data_;
        }

        storage_type data_;
        discretization_type discretization_;
    };

    /// \brief swaps two \c time_series_facade\<\> objects.
    /// \param left The \c time_series_facade\<\> object to swap with \c right.
    /// \param right The \c time_series_facade\<\> object to swap with \c left.
    template<typename Derived, typename Storage, typename Discretization>
    void swap(
        time_series_facade<Derived, Storage, Discretization> &left
      , time_series_facade<Derived, Storage, Discretization> &right
    )
    {
        left.swap(right);
    }

    /// INTERNAL ONLY
    struct equal_to_inserter
    {
        equal_to_inserter()
          : result(true)
        {}

        template<typename R, typename V>
        void set_at(R &, V &v)
        {
            this->result = (this->result && numeric::promote<bool>(v));
        }

        bool result;
    };

    /// \brief Compare two series for equality.
    ///
    /// \return True if the two series have the same discretization and
    ///     the same values at the same offsets; false, otherwise.
    /// \pre \c Left is a model of the \c TimeSeries concept.
    /// \pre \c Right is a model of the \c TimeSeries concept.
    template<typename Left, typename Right>
    bool operator ==(time_series_base<Left> const &left, time_series_base<Right> const &right)
    {
        namespace rrs = range_run_storage;

        if(left.cast().discretization() != right.cast().discretization())
        {
            return false;
        }

        if(numeric::promote<bool>(rrs::zero(left.cast()) != rrs::zero(right.cast())))
        {
            return false;
        }

        equal_to_inserter out;
        rrs::transform(left.cast(), right.cast(), numeric::equal_to, out);
        return out.result;
    }

    /// \brief Compare two series for inequality.
    ///
    /// \return False if the two series have the same discretization and
    ///     the same values at the same offsets; true, otherwise.
    /// \pre \c Left is a model of the \c TimeSeries concept.
    /// \pre \c Right is a model of the \c TimeSeries concept.
    template<typename Left, typename Right>
    bool operator !=(time_series_base<Left> const &left, time_series_base<Right> const &right)
    {
        return ! operator ==(left.cast(), right.cast());
    }

    #ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4244) // std::cout << std::ptrdiff_t(0) gives warning
    #endif
    // run_ostream_inserter
    // writes a run/value to an ostream
    /// INTERNAL ONLY
    template<typename Char = char, typename Traits = std::char_traits<Char> >
    struct run_output_functor
    {
        explicit run_output_functor(std::basic_ostream<Char, Traits> &sout)
          : sout_(sout)
        {}

        template<typename Value, typename Offset>
        void operator ()(Value const &value, Offset start, Offset stop) const
        {
            this->sout_
                << " value = " << value << ", "
                << " offset = " << start << ", "
                << " end offset = " << stop << '\n';
        }
    private:
        std::basic_ostream<Char, Traits> &sout_;
    };
    #ifdef _MSC_VER
    #pragma warning(pop)
    #endif

    /// \brief Writes a \c TimeSeries to an output stream.
    /// \param sout the \c std::basic_ostream\<\> to receive the output.
    /// \param series the \c TimeSeries to output.
    /// \return \c sout
    /// \pre \c Series is a model of the \c TimeSeries concept.
    template<typename Char, typename Traits, typename Series>
    inline std::basic_ostream<Char, Traits> &
    operator <<(std::basic_ostream<Char, Traits> &sout, time_series_base<Series> const &series)
    {
        run_output_functor<Char, Traits> out(sout);
        range_run_storage::for_each(series.cast(), out);
        return sout;
    }

    namespace traits
    {
        /// INTERNAL ONLY
        template<typename Derived>
        struct storage_category<time_series_base<Derived> >
          : storage_category<Derived>
        {};

        /// INTERNAL ONLY
        template<typename Derived, typename Storage, typename Discretization>
        struct storage_category<time_series_facade<Derived, Storage, Discretization> >
          : storage_category<Storage>
        {};

        /// INTERNAL ONLY
        template<typename Derived, typename Storage, typename Discretization>
        struct discretization_type<time_series_facade<Derived, Storage, Discretization> >
        {
            typedef Discretization type;
        };

        /// INTERNAL ONLY
        template<typename Derived, typename Storage, typename Discretization>
        struct offset_type<time_series_facade<Derived, Storage, Discretization> >
          : offset_type<Derived>
        {};
    }

    /// INTERNAL ONLY
    template<typename Op, typename S>
    struct time_series_forward1
    {
        typedef typename mpl::if_<
            is_const<S>
          , typename S::storage_type const
          , typename S::storage_type
        >::type storage_type;

        typedef typename result_of<Op(storage_type &)>::type result_type;

        result_type operator ()(S &s) const
        {
            typedef detail::time_series_facade_access access;
            return Op()(access::data(s));
        }
    };

    /// INTERNAL ONLY
    template<typename Op, typename S, typename A1>
    struct time_series_forward2
    {
        typedef typename mpl::if_<
            is_const<S>
          , typename S::storage_type const
          , typename S::storage_type
        >::type storage_type;

        typedef typename result_of<Op(storage_type &, A1 &)>::type result_type;

        result_type operator ()(S &s, A1 &a1) const
        {
            typedef detail::time_series_facade_access access;
            return Op()(access::data(s), a1);
        }
    };

    /// INTERNAL ONLY
    template<typename Op, typename S, typename A1, typename A2>
    struct time_series_forward3
    {
        typedef typename mpl::if_<
            is_const<S>
          , typename S::storage_type const
          , typename S::storage_type
        >::type storage_type;

        typedef typename result_of<Op(storage_type &, A1 &, A2 &)>::type result_type;

        result_type operator ()(S &s, A1 &a1, A2 &a2) const
        {
            typedef detail::time_series_facade_access access;
            return Op()(access::data(s), a1, a2);
        }
    };

}} // namespace boost::time_series

namespace boost { namespace sequence { namespace impl
{
    /// INTERNAL ONLY
    struct time_series_tag;

    /// INTERNAL ONLY
    template<typename Derived, typename Storage, typename Discretization>
    struct tag<time_series::time_series_facade<Derived, Storage, Discretization> >
    {
        typedef time_series_tag type;
    };

    /// INTERNAL ONLY
    template<typename S>
    struct elements<S, time_series_tag>
      : time_series::time_series_forward1<sequence::op::elements, S>
    {};

    /// INTERNAL ONLY
    template<typename S>
    struct begin<S, time_series_tag>
      : time_series::time_series_forward1<sequence::op::begin, S>
    {};

    /// INTERNAL ONLY
    template<typename S>
    struct end<S, time_series_tag>
      : time_series::time_series_forward1<sequence::op::end, S>
    {};
}}}

namespace boost { namespace range_run_storage { namespace impl
{
    /// INTERNAL ONLY
    template<typename S>
    struct runs<S, sequence::impl::time_series_tag>
      : time_series::time_series_forward1<range_run_storage::op::runs, S>
    {};

    /// INTERNAL ONLY
    template<typename S, typename R, typename V>
    struct set_at<S, R, V, sequence::impl::time_series_tag>
      : time_series::time_series_forward3<range_run_storage::op::set_at, S, R, V>
    {};

    /// INTERNAL ONLY
    template<typename S, typename I>
    struct get_at<S, I, sequence::impl::time_series_tag>
      : time_series::time_series_forward2<range_run_storage::op::get_at, S, I>
    {};

    /// INTERNAL ONLY
    template<typename S>
    struct zero<S, sequence::impl::time_series_tag>
      : time_series::time_series_forward1<range_run_storage::op::zero, S>
    {};

    /// INTERNAL ONLY
    template<typename S, typename V>
    struct set_zero<S, V, sequence::impl::time_series_tag>
      : time_series::time_series_forward2<range_run_storage::op::zero, S, V>
    {};

    /// INTERNAL ONLY
    template<typename S>
    struct ordered_inserter<S, sequence::impl::time_series_tag>
      : time_series::time_series_forward1<range_run_storage::op::ordered_inserter, S>
    {};

    /// INTERNAL ONLY
    template<typename S>
    struct pre_run<S, sequence::impl::time_series_tag>
      : time_series::time_series_forward1<range_run_storage::op::pre_run, S>
    {};

    /// INTERNAL ONLY
    template<typename S>
    struct pre_value<S, sequence::impl::time_series_tag>
      : time_series::time_series_forward1<range_run_storage::op::pre_value, S>
    {};

    /// INTERNAL ONLY
    template<typename S, typename V>
    struct set_pre_value<S, V, sequence::impl::time_series_tag>
      : time_series::time_series_forward2<range_run_storage::op::pre_value, S, V>
    {};

    /// INTERNAL ONLY
    template<typename S>
    struct post_run<S, sequence::impl::time_series_tag>
      : time_series::time_series_forward1<range_run_storage::op::post_run, S>
    {};

    /// INTERNAL ONLY
    template<typename S>
    struct post_value<S, sequence::impl::time_series_tag>
      : time_series::time_series_forward1<range_run_storage::op::post_value, S>
    {};

    /// INTERNAL ONLY
    template<typename S, typename V>
    struct set_post_value<S, V, sequence::impl::time_series_tag>
      : time_series::time_series_forward2<range_run_storage::op::post_value, S, V>
    {};

}}}

namespace boost { namespace constructors { namespace impl
{
    /// INTERNAL ONLY
    template<typename Derived, typename Storage, typename Discretization>
    struct tag<time_series::time_series_facade<Derived, Storage, Discretization> >
    {
        typedef sequence::impl::time_series_tag type;
    };

    /// INTERNAL ONLY
    template<typename T>
    struct construct<T, sequence::impl::time_series_tag>
      : arg_pack_construct
    {
        typedef typename T::derived_type derived_type;
        typedef typename construct<derived_type>::args_type args_type;
    };
}}}


/// INTERNAL ONLY
#define BOOST_TIME_SERIES_constructor_fun(z, n, data)\
    template<BOOST_PP_ENUM_PARAMS_Z(z, n, typename A)>\
    data(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, A, const &a))\
      : base_type(boost::constructors::construct<base_type>(BOOST_PP_ENUM_PARAMS_Z(z, n, a)))\
    {}

/// \brief For use by types derived from \c time_series_facade\<\> to define their
/// forwarding constructors
///
/// For use by types derived from \c time_series_facade\<\> to define their
/// forwarding constructors. The \c series_name argument is the name of the
/// derived time series type. The macro defines the following constructors:
///  \li Default constructor
///  \li Copy constructor
///  \li Converting constructor from any \c TimeSeries model
///  \li Converting constructor from any \c ArgumentPack model
///  \li Constructor overloads that take up to \c BOOST_PARAMETER_MAX_ARITY
///     named or positional arguments.
///
/// The \c series_name type must have an associated specialization
/// of \c boost::constructors::construct\<\> that specifies the named
/// parameters of the \c series_name constructors.
#define BOOST_TIME_SERIES_DEFINE_CTORS(series_name)\
    series_name()\
      : base_type(boost::constructors::construct<base_type>())\
    {}\
    series_name(series_name const &that)\
      : base_type(*static_cast<base_type const *>(&that))\
    {}\
    template<typename ArgPack>\
    series_name(ArgPack const &args, typename boost::enable_if<boost::constructors::is_arg_pack<ArgPack> >::type * = 0)\
      : base_type(boost::constructors::forward_construct<base_type>(args))\
    {}\
    template<typename Series>\
    series_name(Series const &that, typename boost::enable_if<boost::time_series::traits::is_time_series<Series> >::type * = 0)\
      : base_type(boost::constructors::construct<base_type>(\
            boost::time_series::discretization = that.discretization()\
          , boost::time_series::zero = boost::range_run_storage::zero(that)\
        ))\
    {\
        this->base_type::operator =(that);\
    }\
    template<typename A0>\
    explicit series_name(A0 const &a0, typename boost::disable_if<boost::mpl::or_<boost::time_series::traits::is_time_series<A0>, boost::constructors::is_arg_pack<A0> > >::type * = 0)\
      : base_type(boost::constructors::construct<base_type>(a0))\
    {}\
    BOOST_PP_REPEAT_FROM_TO(\
        2\
      , BOOST_PP_INC(BOOST_PARAMETER_MAX_ARITY)\
      , BOOST_TIME_SERIES_constructor_fun\
      , series_name\
    )

#endif
