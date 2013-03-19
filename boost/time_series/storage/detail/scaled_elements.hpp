///////////////////////////////////////////////////////////////////////////////
/// \file scaled_elements.hpp
/// Modifies each element by a constant factor
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TIME_SERIES_STORAGE_DETAIL_SCALED_ELEMENTS_EAN_05_16_2006
#define BOOST_TIME_SERIES_STORAGE_DETAIL_SCALED_ELEMENTS_EAN_05_16_2006

#include <boost/utility/result_of.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace time_series { namespace storage
{

    template<typename Elements>
    struct msvc_result_scaled_elements
    {
        template<typename K>
        struct result;

        template<typename This, typename K>
        struct result<This(K)>
          : remove_reference<
                typename result_of<Elements(K)>::type
            >
        {};

        template<typename This, typename K, typename V>
        struct result<This(K, V)>
        {
            typedef void type;
        };
    };

    template<typename Elements, typename Factor>
    struct scaled_elements
      : msvc_result_scaled_elements<Elements>
    {
        scaled_elements(Elements const &elements, Factor const &factor)
          : elements_(elements)
          , factor_(factor)
        {}

        template<typename K>
        typename msvc_result_scaled_elements<Elements>
            ::template result<scaled_elements(K &)>::type
        operator ()(K &k) const
        {
            return this->factor_ * this->elements_(k);
        }

        template<typename K>
        typename msvc_result_scaled_elements<Elements>
            ::template result<scaled_elements(K const &)>::type
        operator ()(K const &k) const
        {
            return this->factor_ * this->elements_(k);
        }

        template<typename K, typename V>
        void operator ()(K &k, V &v) const
        {
            this->elements_(k, v / this->factor_);
        }

        template<typename K, typename V>
        void operator ()(K const &k, V &v) const
        {
            this->elements_(k, v / this->factor_);
        }

        template<typename K, typename V>
        void operator ()(K &k, V const &v) const
        {
            this->elements_(k, v / this->factor_);
        }

        template<typename K, typename V>
        void operator ()(K const &k, V const &v) const
        {
            this->elements_(k, v / this->factor_);
        }

    private:
        Elements elements_;
        Factor factor_;
    };

}}}

#endif
