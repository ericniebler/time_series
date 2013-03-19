// Copyright Daniel Wallin 2006. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SERIES_060613_HPP
# define BOOST_SERIES_060613_HPP

# include <sstream>

# include <boost/version.hpp>
# include <boost/python/class.hpp>
# include <boost/python/tuple.hpp>
# include <boost/python/list.hpp>
# include <boost/python/detail/api_placeholder.hpp>
# include <boost/python/return_arg.hpp>
# include <boost/python/iterator.hpp>
# include <boost/python/operators.hpp>
# include <boost/python/slice.hpp>
# include <boost/python/make_constructor.hpp>
# include <boost/python/with_custodian_and_ward.hpp>
# include <boost/python/stl_iterator.hpp>

# include <boost/time_series/concepts.hpp>
# include <boost/time_series/clipped_series.hpp>
# include <boost/time_series/ordered_inserter.hpp>
# include <boost/time_series/numeric/numeric.hpp>
# include <boost/time_series/traits/promotion.hpp>
# include <boost/time_series/traits/storage_category.hpp>
# include <boost/range_run_storage/algorithm/for_each.hpp>

# include <boost/mpl/at.hpp>
# include <boost/mpl/map/map10.hpp>
# include <boost/mpl/vector/vector10.hpp>
# include <boost/mpl/back_inserter.hpp>
# include <boost/mpl/placeholders.hpp>
# include <boost/mpl/transform.hpp>
# include <boost/type_traits/remove_pointer.hpp>
# include <boost/utility/enable_if.hpp>
# include <boost/parameter/python.hpp>
# include <boost/detail/construct.hpp>
# include <boost/detail/parameters_mpl.hpp>

namespace boost { namespace time_series { namespace python_
{

    namespace detail 
    {
        template<typename Series>
        struct is_valid_divisor
          : traits::is_valid_divisor<
                typename traits::storage_category<Series>::type
            >
        {};

        template<typename Pred, typename Class, typename Op>
        typename enable_if<Pred>::type
        def_if(Class * what, Op const &op)
        {
            what->def(op);
        }

        template<typename Pred, typename Op>
        typename disable_if<Pred>::type
        def_if(void *, Op const &op)
        {
        }

        template<typename T>
        struct to_python_param
        {
            typedef T type;
        };

        template<typename T, typename U>
        struct to_python_param<parameter::optional<T, U> >
        {
            typedef T* type;
        };

        template<typename T, typename U>
        struct to_python_param<parameter::required<T, U> >
        {
            typedef T type;
        };

        template<typename Value, typename Discretization, typename Offset, typename Param>
        struct to_python_arg
        {
            typedef mpl::map5<
                mpl::pair< time_series::tag::start, Offset >
              , mpl::pair< time_series::tag::stop, Offset >
              , mpl::pair< time_series::tag::value, Value >
              , mpl::pair< time_series::tag::discretization, Discretization >
              , mpl::pair< time_series::tag::zero, Value >
            > args_map;

            typedef typename remove_pointer<Param>::type param_type;

            typedef typename mpl::at<args_map, param_type>::type type;
        };

        template<typename K, typename T>
        struct make_param_spec
        {
            typedef K type(T);
        };

        struct pyton_series_repr_fun
        {
            pyton_series_repr_fun(std::ostream &sout)
              : sout_(sout)
              , first_(true)
            {}

            template<typename Value, typename Offset>
            void operator ()(Value const &value, Offset offset, Offset end_offset) const
            {
                if(!this->first_)
                {
                    this->sout_ << ", ";
                }

                this->sout_ << '(';
                this->sout_ << value;
                this->sout_ << ", ";
                this->put_offset(offset);
                this->sout_ << ", ";
                this->put_offset(end_offset);
                this->sout_ << ')';

                this->first_ = false;
            }

        private:
            template<typename Offset>
            std::ostream &put_offset(Offset off) const
            {
                if(-inf == off)
                {
                    return this->sout_ << "-inf";
                }
                else if(inf == off)
                {
                    return this->sout_ << "inf";
                }
                return this->sout_ << off;
            }

            std::ostream &sout_;
            mutable bool first_;
        };
    }

    template<typename Series>
    inline std::string python_series_repr(Series const &s)
    {
        std::stringstream repr;
        repr << '(' << s.discretization();
        repr << ", " << range_run_storage::zero(s);
        repr << ", [";
        detail::pyton_series_repr_fun fun(repr);
        range_run_storage::for_each(s, fun);
        repr << "])";
        return repr.str();
    }

    template<class Series>
    struct python_clipped_series
      : python::class_<clipped_series<Series> >
    {
        typedef clipped_series<Series> clipped;
        typedef python::class_<clipped_series<Series> > base;
        typedef typename clipped::value_type value_type;
        typedef typename clipped::offset_type offset_type;
        typedef typename clipped::const_iterator const_iterator;

        python_clipped_series(std::string const& base_name)
          : base(
                ("clipped_" + base_name).c_str()
              , python::no_init
            )
        {
            value_type (*getitem_1)(clipped const&, offset_type) = &getitem<offset_type>;
            value_type (*getitem_2)(clipped const&, minus_infinity) = &getitem<minus_infinity>;
            value_type (*getitem_3)(clipped const&, plus_infinity) = &getitem<plus_infinity>;

            this->def("__len__", &get_size);
            this->def("__iter__", python::range(&begin, &end));
            this->def("__getitem__", getitem_1);
            this->def("__getitem__", getitem_2);
            this->def("__getitem__", getitem_3);

            this->def(python::self == python::self);
            this->def(python::self + python::self);
            this->def(python::self - python::self);
            this->def(python::self * python::self);
            this->def(str(python::self));
            this->def("__repr__", &python_series_repr<clipped_series<Series> >);

            // wrap division only if Series is dense, constant or piecewise constant
            detail::def_if<detail::is_valid_divisor<Series> >(this, python::self / python::self);

            clipped (*make_1)(Series&, offset_type, offset_type) = &make<offset_type, offset_type>;
            clipped (*make_2)(Series&, minus_infinity, offset_type) = &make<minus_infinity, offset_type>;
            clipped (*make_3)(Series&, offset_type, plus_infinity) = &make<offset_type, plus_infinity>;

            python::def("clip", make_1, python::with_custodian_and_ward_postcall<0,1>());
            python::def("clip", make_2, python::with_custodian_and_ward_postcall<0,1>());
            python::def("clip", make_3, python::with_custodian_and_ward_postcall<0,1>());
        }

        template<typename Offset1, typename Offset2>
        static clipped make(Series& series, Offset1 start, Offset2 stop)
        {
            return clipped(series, start, stop);
        }

        template<typename Offset>
        static value_type getitem(clipped const& s, Offset offset)
        {
            return s[offset];
        }

        static const_iterator begin(clipped const& s)
        {
            return s.begin();
        }

        static const_iterator end(clipped const& s)
        {
            return s.end();
        }

        static std::size_t get_size(clipped const& s)
        {
            return static_cast<std::size_t>(s.stop() - s.start());
        }
    };

    template<class Series>
    struct python_inserter
      : python::class_<ordered_inserter<Series> >
    {
        typedef ordered_inserter<Series> inserter;
        typedef python::class_<ordered_inserter<Series> > base;

        typedef typename concepts::TimeSeries<Series>::value_type value_type;
        typedef typename concepts::TimeSeries<Series>::offset_type offset_type;

        python_inserter(std::string const& base_name)
          : base(
                (base_name + "_ordered_inserter").c_str()
              , python::no_init
            )
        {
            inserter& (*call3_1)(inserter&, value_type, offset_type, offset_type)      = &call3<offset_type, offset_type>;
            inserter& (*call3_2)(inserter&, value_type, minus_infinity, offset_type)   = &call3<minus_infinity, offset_type>;
            inserter& (*call3_3)(inserter&, value_type, offset_type, plus_infinity)    = &call3<offset_type, plus_infinity>;
            inserter& (*call3_4)(inserter&, value_type, minus_infinity, plus_infinity) = &call3<minus_infinity, plus_infinity>;

            this->def("__call__", &call1, python::return_self<>());
            this->def("__call__", &call2, python::return_self<>());
            this->def("__call__", &call2, python::return_self<>());
            this->def("__call__", call3_1, python::return_self<>());
            this->def("__call__", call3_2, python::return_self<>());
            this->def("__call__", call3_3, python::return_self<>());
            this->def("__call__", call3_4, python::return_self<>());
            this->def("commit", &inserter::commit);

            python::def("ordered_inserter", &make);
            python::def("ordered_inserter", &make2);
        }

        static inserter make(Series& series)
        {
            return inserter(series);
        }

        static inserter make2(Series& series, offset_type o)
        {
            return inserter(series, o);
        }

        static inserter& call1(inserter& self, value_type v)
        {
            return self(v);
        }

        static inserter& call2(inserter& self, value_type v, offset_type o)
        {
            return self(v, o);
        }

        template<typename Offset1, typename Offset2>
        static inserter& call3(inserter& self, value_type v, Offset1 o, Offset2 e)
        {
            return self(v, o, e);
        }
    };

    template<typename Offset>
    Offset extract_offset(python::object const &o)
    {
        python::extract<minus_infinity> neg_inf(o);
        python::extract<plus_infinity> pos_inf(o);
        python::extract<Offset> off(o);

        if(off.check())
            return off();
        if(neg_inf.check())
            return neg_inf();
        if(pos_inf.check())
            return pos_inf();

        PyErr_SetObject(PyExc_ValueError, ("expected offset; got %s" % o).ptr());
        python::throw_error_already_set();
        return Offset();
    }

    template<typename Series, typename Args>
    std::auto_ptr<Series> python_make_series_constructor(python::tuple const &o)
    {
        typedef typename concepts::TimeSeries<Series>::value_type value_type;
        typedef typename concepts::TimeSeries<Series>::offset_type offset_type;
        typedef typename concepts::TimeSeries<Series>::discretization_type discretization_type;

        if(python::len(o) != 3)
        {
            PyErr_SetObject(PyExc_ValueError,
                          ("expected 3-item tuple in call to time series constructor; got %s"
                           % o).ptr()
            );
            python::throw_error_already_set();
        }

        discretization_type disc = python::extract<discretization_type>(o[0]);
        value_type z = python::extract<value_type>(o[1]);
        python::list data = python::extract<python::list>(o[2]);

        std::auto_ptr<Series> series(new Series(
            constructors::construct<Series>(time_series::discretization = disc, time_series::zero = z)
        ));

        python::stl_input_iterator<python::tuple> begin(data), end;
        time_series::ordered_inserter<Series> oi(*series);
        
        for(; begin != end; ++begin)
        {
            python::tuple t = *begin;
            switch(python::len(t))
            {
            case 1:
                oi(python::extract<value_type>(t[0])());
                break;
            case 2:
                oi(python::extract<value_type>(t[0])(),
                   extract_offset<offset_type>(t[1]));
                break;
            case 3:
                oi(python::extract<value_type>(t[0])(),
                   extract_offset<offset_type>(t[1]),
                   extract_offset<offset_type>(t[2]));
                break;
            default:
                PyErr_SetObject(PyExc_ValueError,
                              ("expected 1-, 2-, or 3-item tuple in call to time series constructor; got %s"
                               % t).ptr()
                );
                python::throw_error_already_set();
                break;
            }
        }

        oi.commit();
        return series;
    }

    template <class Series>
    struct python_series
      : python::class_<Series>
    {
        typedef python::class_<Series> base;

        typedef typename concepts::TimeSeries<Series>::value_type value_type;
        typedef typename concepts::TimeSeries<Series>::offset_type offset_type;
        typedef typename concepts::TimeSeries<Series>::discretization_type discretization_type;

        typedef typename constructors::impl::construct<Series>::args_type parameters_type;

        typedef typename mpl::transform<
            parameters_type
          , detail::to_python_param<mpl::_>
          , mpl::back_inserter<mpl::vector0<> >
        >::type keywords_type;

        typedef typename mpl::transform<
            keywords_type
          , detail::to_python_arg<value_type, discretization_type, offset_type, mpl::_>
          , mpl::back_inserter<mpl::vector0<> >
        >::type arguments_type;

    #if BOOST_VERSION == 103400
        typedef typename mpl::transform<
            keywords_type
          , arguments_type
          , detail::make_param_spec<mpl::_, mpl::_>
          , mpl::back_inserter<mpl::vector0<> >
        >::type param_specs_type;

        typedef parameter::python::init<param_specs_type> parameter_python_init;
    #else
        typedef parameter::python::init<keywords_type, arguments_type> parameter_python_init;
    #endif

        python_series(char const* name = "time_series")
          : base(name, python::no_init)
        {
            this->def(parameter_python_init());
            this->def("__init__", python::make_constructor(&python_make_series_constructor<Series, arguments_type>));

            value_type (*getitem_1)(Series const&, offset_type) = &getitem<offset_type>;
            value_type (*getitem_2)(Series const&, minus_infinity) = &getitem<minus_infinity>;
            value_type (*getitem_3)(Series const&, plus_infinity) = &getitem<plus_infinity>;

            this->def("__getitem__", getitem_1);
            this->def("__getitem__", getitem_2);
            this->def("__getitem__", getitem_3);

            this->def(python::self == python::self);
            this->def(python::self + python::self);
            this->def(python::self - python::self);
            this->def(python::self * python::self);
            this->def(python::self += python::self);
            this->def(python::self -= python::self);
            this->def(python::self *= python::self);
            this->def(str(python::self));

            this->def("__repr__", &python_series_repr<Series>);
            
            // wrap division only if Series is dense, constant or piece wise constant
            detail::def_if<detail::is_valid_divisor<Series> >(this, python::self / python::self);
            detail::def_if<detail::is_valid_divisor<Series> >(this, python::self /= python::self);

            python_inserter<Series> inserter(name);
            python_clipped_series<Series> clipped(name);

            this->def("__getitem__", &getslice, python::with_custodian_and_ward_postcall<0,1>());
        }

        static discretization_type get_discretization(Series const& s)
        {
            return s.discretization();
        }

        template<typename Offset>
        static value_type getitem(Series const& s, Offset offset)
        {
            return s[offset];
        }

        static clipped_series<Series> getslice(Series & s, python::slice sl)
        {
            python::extract<minus_infinity> neg_inf(sl.start());
            python::extract<plus_infinity> pos_inf(sl.stop());
            python::extract<offset_type> start(sl.start());
            python::extract<offset_type> stop(sl.stop());

            if(neg_inf.check())
            {
                return clipped_series<Series>(s, neg_inf(), stop());
            }
            
            if(pos_inf.check())
            {
                return clipped_series<Series>(s, start(), pos_inf());
            }

            return clipped_series<Series>(s, start(), stop());
        }
    };

    template<bool Positive>
    struct python_infinity
      : python::class_<range_run_storage::infinity<Positive> >
    {
        typedef python::class_<range_run_storage::infinity<Positive> > base;

        python_infinity(char const* name = (Positive? "plus_infinity" : "minus_infinity"))
          : base(name, python::no_init)
        {
            this->def(-python::self);
        }
    };

}}} // namespace boost::time_series::python_

#endif // BOOST_SERIES_060613_HPP

