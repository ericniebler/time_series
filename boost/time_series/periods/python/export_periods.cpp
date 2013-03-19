///////////////////////////////////////////////////////////////////////////////
/// @brief Exports the periods class to python
///
/// @file       
/// @author     olivier

#include <boost/python.hpp>

#include "plato/misc/periods.hpp"

using namespace boost::python;

///
/// @brief Exports the period class to python
///
template<typename OffsetType>
struct export_period
  : boost::python::class_<plato::misc::period<OffsetType> >
{
    typedef typename plato::misc::period<OffsetType>   period_type;
    typedef boost::python::class_<period_type>         base_type;
      
    export_period(const char * name)
      : base_type(name, boost::python::no_init)
    {
        /// Constructors
        this->def(init<>());
        this->def(init<OffsetType, OffsetType>(args("start", "stop")));
        
        /// Member functions
        this->def("start",      &period_type::start,       return_value_policy<copy_const_reference>());
        this->def("stop",       &period_type::stop,        return_value_policy<copy_const_reference>());
        this->def("span",       &period_type::span,        return_value_policy<copy_const_reference>());
        
        /// String representation
        this->def("__repr__", &period_type::repr);
    }
};

void do_export_period()
{
    export_period<int>("Period");
}

///
/// @brief Exports the periods class to python
///
template<typename OffsetType>
struct export_periods
  : boost::python::class_<plato::misc::periods<OffsetType> >
{
    typedef typename plato::misc::periods<OffsetType>   periods_type;
    typedef typename periods_type::offset_vector_type   offset_vector_type;
    typedef boost::python::class_<periods_type>         base_type;
      
    export_periods(const char * name)
      : base_type(name, boost::python::no_init)
    {
        /// Constructors
        this->def(init<>());
        this->def(init<OffsetType>                          (args("size")));
        this->def(init<OffsetType, OffsetType>              (args("start", "stop")));
        this->def(init<OffsetType, OffsetType, OffsetType>  (args("start", "stop", "span")));
        this->def(init<offset_vector_type>                  (args("offsets")));
        
        /// Member functions
        this->def("start",      &periods_type::start,       return_value_policy<copy_const_reference>());
        this->def("stop",       &periods_type::stop,        return_value_policy<copy_const_reference>());
        this->def("size",       &periods_type::size,        return_value_policy<copy_const_reference>());
        this->def("real_stop",  &periods_type::real_stop,   return_value_policy<copy_const_reference>());
        
        /// Iterator
        this->def("__iter__", iterator<periods_type>());
        
        /// String representation
        this->def("__repr__", &periods_type::repr);
    }
};

void do_export_periods()
{
    export_periods<int>("Periods");
}
