///////////////////////////////////////////////////////////////////////////////
/// \file time_series_typeof.hpp
/// Type registrations for the Boost.Typeof library
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_TIME_SERIES_TYPEOF_HPP_EAN_10_04_2006
#define BOOST_TIME_SERIES_TIME_SERIES_TYPEOF_HPP_EAN_10_04_2006

#include <boost/typeof/typeof.hpp>
#include <boost/time_series/time_series_fwd.hpp>

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::tag::start)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::tag::stop)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::tag::value)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::tag::discretization)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::tag::zero)

BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::piecewise_constant_storage_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::dense_storage_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::sparse_storage_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::characteristic_storage_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::delta_storage_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::constant_storage_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::heaviside_storage_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::inverse_heaviside_storage_tag)
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::scaled_storage_tag, (typename))

BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::daily)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::weekly)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::monthly)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::quarterly)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::semi_yearly)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::yearly)
BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::unit)

BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::traits::is_time_series, (typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::traits::storage_category, (typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::traits::generate_series, (typename)(typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::traits::discretization_type, (typename))

BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::any)

BOOST_TYPEOF_REGISTER_TYPE(boost::time_series::time_series_root)
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::time_series_base, (typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::time_series_facade, (typename)(typename)(typename))

BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::dense_series, (typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::characteristic_series, (typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::characteristic_unit_series, (typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::constant_series, (typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::delta_series, (typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::delta_unit_series, (typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::heaviside_series, (typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::heaviside_unit_series, (typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::inverse_heaviside_series, (typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::inverse_heaviside_unit_series, (typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::piecewise_constant_series, (typename)(typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::sparse_series, (typename)(typename)(typename))

BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::clipped_series, (typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::shifted_series, (typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::scaled_series, (typename)(typename))

BOOST_TYPEOF_REGISTER_TEMPLATE(boost::time_series::ordered_inserter, (typename))

#endif
