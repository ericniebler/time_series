///////////////////////////////////////////////////////////////////////////////
/// @brief      Tests periods.hpp
///
///
/// @file
/// @author     olivier

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include "plato/math/matrix.hpp"
#include "plato/misc/periods.hpp"

using namespace boost;
using namespace unit_test;

using namespace plato::misc;
using namespace plato::math;

void period_unit_test_func()
{
    typedef plato::misc::period<int> period_type;
    
    /// An empty period object
    period_type period_1;
    
    /// A period object
    period_type period_2(2,5);
    
    BOOST_CHECK_EQUAL(period_1.start(), 0);
    BOOST_CHECK_EQUAL(period_1.stop() , 0);
    BOOST_CHECK_EQUAL(period_1.span() , 0);
    
    BOOST_CHECK_EQUAL(period_2.start(), 2);
    BOOST_CHECK_EQUAL(period_2.stop() , 5);
    BOOST_CHECK_EQUAL(period_2.span() , 3);
    
    /// Copy constructing a new period object and checking operator==
    period_type period_3(period_2);
    BOOST_CHECK(period_2 == period_3);
    
    /// Check operator!=
    BOOST_CHECK(period_type(2,5) != period_type(3,5));
    BOOST_CHECK(period_type(2,5) != period_type(2,6));
    
    /// Check operator=
    period_type period_4 = period_3;
    BOOST_CHECK(period_3 == period_3);
    
    /// Check repr()
    BOOST_CHECK_NO_THROW(period_1.repr());
    
    /// Checks a non-valid periods
    BOOST_CHECK_THROW(period_type p(6,2), std::runtime_error);
}

void periods_unit_test_func()
{
    typedef periods<int> periods_type;
    typedef periods_type::period_type period_type;
    
    /// An empty periods object
    periods_type periods_1;
    
    /// A periods object with 10 periods with span 1, starting at 0
    periods_type periods_2(10);
    
    /// A periods object with periods with span 1, starting at 5 and stopping at 15
    periods_type periods_3(5,15);
    
    /// A periods object with periods of span 2, starting at 7 and stopping at 17
    periods_type periods_4(7,17,2);
    
    /// A periods object with periods of span 2, starting at 7 and stopping at 16
    periods_type periods_5(7,16,2);
    
    /// A periods object with periods of different spans
    plato::math::vector<int> offsets(8);
    offsets << 0, 7, 14, 30, 60, 90, 180, 365;
    periods_type periods_6(offsets);
    
    /// Checking start, stop and size
    BOOST_CHECK_EQUAL(periods_1.start(),      0);
    BOOST_CHECK_EQUAL(periods_1.stop() ,      0);
    BOOST_CHECK_EQUAL(periods_1.real_stop(),  0);
    BOOST_CHECK_EQUAL(periods_1.size() ,      0u);
    
    BOOST_CHECK_EQUAL(periods_2.start(),      0);
    BOOST_CHECK_EQUAL(periods_2.stop() ,     10);
    BOOST_CHECK_EQUAL(periods_2.real_stop(), 10);
    BOOST_CHECK_EQUAL(periods_2.size() ,     10u);
    
    BOOST_CHECK_EQUAL(periods_3.start(),      5);
    BOOST_CHECK_EQUAL(periods_3.stop() ,     15);
    BOOST_CHECK_EQUAL(periods_3.real_stop(), 15);
    BOOST_CHECK_EQUAL(periods_3.size() ,     10u);
    
    BOOST_CHECK_EQUAL(periods_4.start(),      7);
    BOOST_CHECK_EQUAL(periods_4.stop() ,     17);
    BOOST_CHECK_EQUAL(periods_4.real_stop(), 17);
    BOOST_CHECK_EQUAL(periods_4.size() ,      5u);
    
    BOOST_CHECK_EQUAL(periods_5.start(),      7);
    BOOST_CHECK_EQUAL(periods_5.stop() ,     16);
    BOOST_CHECK_EQUAL(periods_5.real_stop(), 17);
    BOOST_CHECK_EQUAL(periods_5.size() ,     5u);
    
    BOOST_CHECK_EQUAL(periods_6.start(),      0);
    BOOST_CHECK_EQUAL(periods_6.stop() ,    365);
    BOOST_CHECK_EQUAL(periods_6.real_stop(),365);
    BOOST_CHECK_EQUAL(periods_6.size() ,     7u);
    
    /// Checking operator==
    BOOST_CHECK(periods_type(10) == periods_type(0,10));
    BOOST_CHECK(periods_type(10) == periods_type(0,10,1));
    
    /// periods_4 and periods_5 have a different stop offsets, 
    /// but same real_stop offsets and since they produce the 
    /// same periods, they should be considered equal. Checking that...
    BOOST_CHECK(periods_4 == periods_5);
    
    /// Check operator!=
    BOOST_CHECK(periods_type(0,10,1) != periods_type(1,10,1));
    BOOST_CHECK(periods_type(0,10,1) != periods_type(0,11,1));
    BOOST_CHECK(periods_type(0,10,1) != periods_type(0,10,2));
    
    /// Check operator=
    periods_type periods_7 = periods_6;
    BOOST_CHECK(periods_6 == periods_7);
    
    /// Check repr()
    BOOST_CHECK_NO_THROW(periods_7.repr());
    
    /// Check iteration
    periods_type p(periods_7);
    BOOST_CHECK(*(p.begin())              == period_type(  0,   7));
    BOOST_CHECK(*(++p.begin())            == period_type(  7,  14));
    BOOST_CHECK(*(++++p.begin())          == period_type( 14,  30));
    BOOST_CHECK(*(++++++p.begin())        == period_type( 30,  60));
    BOOST_CHECK(*(++++++++p.begin())      == period_type( 60,  90));
    BOOST_CHECK(*(++++++++++p.begin())    == period_type( 90, 180));
    BOOST_CHECK(*(++++++++++++p.begin())  == period_type(180, 365));
    BOOST_CHECK(  ++++++++++++++p.begin() == p.end());
    BOOST_CHECK(*(--p.end())              == period_type(180, 365));
    BOOST_CHECK(*(----p.end())            == period_type( 90, 180));
    BOOST_CHECK(*(------p.end())          == period_type( 60,  90));
    BOOST_CHECK(*(--------p.end())        == period_type( 30,  60));
    BOOST_CHECK(*(----------p.end())      == period_type( 14,  30));
    BOOST_CHECK(*(------------p.end())    == period_type(  7,  14));
    BOOST_CHECK(*(--------------p.end())  == period_type(  0,   7));
        
    /// Check random access
    BOOST_CHECK(p.begin()                 == p.begin() + 0);
    BOOST_CHECK(++p.begin()               == p.begin() + 1);
    BOOST_CHECK(++++p.begin()             == p.begin() + 2);
    BOOST_CHECK(++++++p.begin()           == p.begin() + 3);
    BOOST_CHECK(++++++++p.begin()         == p.begin() + 4);
    BOOST_CHECK(++++++++++p.begin()       == p.begin() + 5);
    BOOST_CHECK(++++++++++++p.begin()     == p.begin() + 6);
    BOOST_CHECK(++++++++++++++p.begin()   == p.begin() + 7);
    BOOST_CHECK(p.begin()                 == p.end()   - 7);
    BOOST_CHECK(++p.begin()               == p.end()   - 6);
    BOOST_CHECK(++++p.begin()             == p.end()   - 5);
    BOOST_CHECK(++++++p.begin()           == p.end()   - 4);
    BOOST_CHECK(++++++++p.begin()         == p.end()   - 3);
    BOOST_CHECK(++++++++++p.begin()       == p.end()   - 2);
    BOOST_CHECK(++++++++++++p.begin()     == p.end()   - 1);
    BOOST_CHECK(++++++++++++++p.begin()   == p.end()   - 0);
    
    /// Check BOOST_FOREACH
    std::size_t count(0);
    BOOST_FOREACH(period_type period, p)
    {
        BOOST_CHECK_EQUAL(period, period_type(
                                      (*(p.begin() + count)).start()
                                    , (*(p.begin() + count)).stop()
                                  )
                         );
        ++count;
    }
    BOOST_CHECK_EQUAL(count, static_cast<std::size_t>(p.size()));
    
    /// Check some non-valid periods
    BOOST_CHECK_THROW(periods_type p(0,10,-1), std::runtime_error);
    BOOST_CHECK_THROW(periods_type p(10,0, 1), std::runtime_error);
    BOOST_CHECK_THROW(periods_type p(10,0,-1), std::runtime_error);
    plato::math::vector<int> o(8);
    o << 0, 7, 14, 60, 30, 90, 180, 365;
    BOOST_CHECK_THROW(periods_type p(o), std::runtime_error);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("plato::misc::periods test");

    test->add(BOOST_TEST_CASE(&period_unit_test_func));
    test->add(BOOST_TEST_CASE(&periods_unit_test_func));


    return test;
}
