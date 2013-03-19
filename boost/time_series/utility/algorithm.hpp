///////////////////////////////////////////////////////////////////////////////
/// \file algorithm.hpp
/// Utilities for Sequences, cursors and property maps
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_DETAIL_ALGORITHM_HPP_EAN_05_01_2006
#define BOOST_TIME_SERIES_DETAIL_ALGORITHM_HPP_EAN_05_01_2006

namespace boost { namespace time_series { namespace detail
{

    /////////////////////////////////////////////////////////////////////////////////
    //// copy
    //template<
    //    typename InIndexMap
    //  , typename InElementsMap
    //  , typename InCursor
    //  , typename Array
    //>
    //void copy(
    //    InIndexMap const &in_index
    //  , InElementsMap const &in_elements
    //  , InCursor in_begin
    //  , InCursor in_end
    //  , Array &out_array
    //)
    //{
    //    typename result_of<sequence::op::begin(Array &)>::type cursor = sequence::begin(out_array);
    //    for(; in_begin != in_end; ++in_begin)
    //    {
    //        cursor = set_at(
    //            out_array
    //          , cursor
    //          , in_index(*in_begin)
    //          , in_elements(*in_begin)
    //        );
    //    }
    //}

}}}

#endif
