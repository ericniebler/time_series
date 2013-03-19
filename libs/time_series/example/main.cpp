///////////////////////////////////////////////////////////////////////////////
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/random.hpp>
#include <boost/time_series/delta_series.hpp>
#include <boost/time_series/dense_series.hpp>
#include <boost/time_series/sparse_series.hpp>
#include <boost/time_series/heaviside_series.hpp>
#include <boost/time_series/inverse_heaviside_series.hpp>
#include <boost/time_series/characteristic_series.hpp>
#include <boost/time_series/piecewise_constant_series.hpp>
#include <boost/time_series/constant_series.hpp>
#include <boost/time_series/ordered_inserter.hpp>
#include <boost/time_series/numeric/numeric.hpp>
#include <boost/time_series/numeric/shift.hpp>
#include <boost/time_series/numeric/period_sums.hpp>
#include <boost/time_series/numeric/adjacent_difference.hpp>
#include <boost/time_series/numeric/coarse_grain.hpp>

namespace rrs = boost::range_run_storage;
namespace seq = boost::sequence;

int main()
{
    using namespace boost::time_series;

    boost::mt19937 rng;

    // the discretization is a template parameter
    // it is passed to the constructor mainly to check
    // whether two series are compatible. If not specified,
    // the discretization defaults to int(1), so that the 
    // discretization is a runtime quantity.
    typedef monthly discretization;

    int number_of_years = 5; // the simulation horizon
    int start_time = -2; // start two months in the past

    int end_time = number_of_years * days_per_year() / discretization();

    // create the interest rates series
    dense_series<double, discretization> interest_rate(start = start_time, stop = end_time);

    // sample interest rates by a uniform distribution
    // this is a stupid model for interest rates but simple

    boost::variate_generator<boost::mt19937&, boost::uniform_real<> >
        rates_generator(rng, boost::uniform_real<>(0.02, 0.1));

    // syntax here works with Boost.random. Note: Only dense_series can be
    // filled this way.
    std::pair<double *, std::size_t> buffer = get_raw_buffer(interest_rate);
    std::generate(
         buffer.first
       , buffer.first + buffer.second
       , rates_generator
    );

    // now let's set up a financial instrument
    // the client gets the loan on January 10 of the first year;
    // we assume that index 0 stands for amount at the end of the first month

    int loan_date = 10;
    int loan_quarter = loan_date / days_per_quarter();
    int loan_duration = 3 * days_per_year(); // 3 years

    // the outstanding is now 1000000 for the duration of the loan and 0 otherwise
    constant_series<double, discretization> amount(1000000.);
    characteristic_unit_series<double, discretization> outstanding_range(
        loan_date / discretization()
      , loan_duration / discretization()
    );
    
    // outstanding amout at sampling time
    characteristic_series<double, discretization> outstanding = amount * outstanding_range;

    // quarterly interest payments at end of quarter
    // let's prepare a series consiting of 1s in the months where he pays
    // interest

    sparse_series<double, discretization> interest_payment_dates;
    ordered_inserter<sparse_series<double, discretization> > in1(interest_payment_dates);
    for(int i = days_per_quarter(); i <= loan_duration; i += days_per_quarter())
    {
        in1(1., (i + days_per_quarter() * loan_quarter) / discretization());
    }
    in1.commit();

    // let's extract the interest rates: they might be the ones from the end of
    // the last month of the previous quarter, thus we need to shift
    // afterwards rate contains the rates applicable to that interest payment

    sparse_series<double, discretization> rate = 
        interest_payment_dates * shift(interest_rate, days_per_quarter()/discretization());

    // let us next calculate the amortization, assuming an annual schedule
    // and amortization of 2% per year

    sparse_series<double, discretization> amortization;
    ordered_inserter<sparse_series<double, discretization> > in2(amortization);
    for(int i = days_per_year(); i < loan_duration; i += days_per_year())
    {
        in2(0.02 * amount[0], i / discretization());
    }
    in2.commit();

    // now we can adjust the outstanding: subtract the amortization

    outstanding -= amortization;

    // finally we want to get the payback at the last date
    // as the adjacent difference at the end of the loan

    int loan_ends = (loan_date + loan_duration) / discretization();

    delta_series<double, discretization> payback(
        start = loan_ends - 1
      , value = outstanding[loan_ends] - outstanding[loan_ends - 1]
    );

    // next we need to calculate the interest payments

    // first integrate the outstanding over each quarter
    // starting at the end of the previous month (-1), not at 0 or -2
    sparse_series<double, discretization> integrated_amount =
        period_sums(outstanding, -1, days_per_quarter() / discretization());

    // we need to define where we store that sum: in the 1st or last entry
    // of the period. Le us assume it is the first, then we now need to shift
    // it by 3 months

    integrated_amount = shift(integrated_amount, days_per_quarter() / discretization());

    // next we might need to adjust the first and last partial month, but
    // we skip this here for now

    sparse_series<double, discretization> interest_payment = integrated_amount * rate;

    // Now we know how much the bank gets:

    sparse_series<double, discretization> cash_flow = 
        adjacent_difference(outstanding) + interest_payment;

    // we can also calculate the total payments per year:

    sparse_series<double, discretization> annual_payments =
        period_sums(amortization + interest_payment + payback, 0, days_per_year() / discretization());

    // we can also go to coarser grids to save storage if needed
    // we might have different coarse graining functions:
    // coarse graining by sampling or integrating over periods
    // here we did the period sums before, so sampling is fine

    sparse_series<double, yearly> coarse_grained_paymets =
        coarse_grain(annual_payments, yearly());

    // now, what happens in case of bankruptcy

    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > bankruptcy(
        rng, boost::uniform_int<>(0, 3600));

    // the client goes bankrupt some time in the next 10 years
    int tau = bankruptcy();
    int tau_coarse = tau / discretization();

    sparse_series<double, discretization> real_cashflow =
        inverse_heaviside_unit_series<double, discretization>(tau_coarse - 1) * cash_flow;

    if(tau < loan_date + loan_duration)
    {
        // he goes bankrupt during the loan period
        // we can still recover e.g. 80% of the outstanding
        real_cashflow += delta_series<double, discretization>(tau_coarse, 0.8 * outstanding[tau_coarse]);
    }

    return 0;
}
