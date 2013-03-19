// Copyright David Abrahams, Eric Niebler 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANGE_RUN_STORAGE_CONCEPTS_DWA200658_HPP
# define BOOST_RANGE_RUN_STORAGE_CONCEPTS_DWA200658_HPP

# include <boost/mpl/assert.hpp>
# include <boost/utility/result_of.hpp>
# include <boost/sequence/concepts.hpp>
# include <boost/type_traits/add_reference.hpp>
# include <boost/type_traits/remove_reference.hpp>
# include <boost/type_traits/remove_cv.hpp>
# include <boost/type_traits/is_convertible.hpp>
# include <boost/range_run_storage/runs.hpp>
# include <boost/range_run_storage/get_at.hpp>
# include <boost/range_run_storage/set_at.hpp>
# include <boost/range_run_storage/offset.hpp>
# include <boost/range_run_storage/length.hpp>
# include <boost/range_run_storage/empty.hpp>
# include <boost/range_run_storage/commit.hpp>
# include <boost/range_run_storage/end_offset.hpp>
# include <boost/range_run_storage/zero.hpp>
# include <boost/range_run_storage/pre_run.hpp>
# include <boost/range_run_storage/post_run.hpp>
# include <boost/range_run_storage/ordered_inserter.hpp>
# include <boost/range_run_storage/traits/is_unit_run.hpp>

namespace boost { namespace range_run_storage { namespace concepts
{

using namespace boost::sequence::concepts;
using boost::sequence::concepts::Sequence;

/// INTERNAL ONLY
template<typename S>
inline S &lvalue_of()
{
    BOOST_ASSERT(false);
    throw "should never get here";
}

template<typename S>
struct Run
{
    typedef
        typename result_of<
            op::offset(typename add_reference<S>::type)
        >::type
    offset_type;

    typedef
        typename result_of<
            op::length(typename add_reference<S>::type)
        >::type
    length_type;

    typedef typename traits::is_unit_run<S>::type is_unit_length;

    ~Run()
    {
        S &s = concepts::lvalue_of<S>();
        offset_type off = range_run_storage::offset(s);
        offset_type end = range_run_storage::end_offset(s);
        length_type len = range_run_storage::length(s);
        bool emp = range_run_storage::empty(s);

        ignore_unused_variable_warning(s);
        ignore_unused_variable_warning(off);
        ignore_unused_variable_warning(end);
        ignore_unused_variable_warning(len);
        ignore_unused_variable_warning(emp);
    }
};

template<typename S>
struct RangeRunStorage
  : Sequence<S>
{
    typedef
        typename result_of<
            op::runs(typename add_reference<S>::type)
        >::type
    runs;

    BOOST_CONCEPT_ASSERT((ReadablePropertyMap<runs, typename RangeRunStorage::cursor>));

    typedef
        typename ReadablePropertyMap<
            runs
          , typename RangeRunStorage::cursor
        >::value_type
    run_type;

    BOOST_CONCEPT_ASSERT((Run<run_type>));

    typedef typename Run<run_type>::offset_type offset_type;
    typedef typename Run<run_type>::length_type length_type;

    typedef
        typename result_of<
            op::zero(typename add_reference<S>::type)
        >::type
    zero_type;

    BOOST_CONCEPT_ASSERT((Convertible<zero_type, typename RangeRunStorage::value_type>));

    ~RangeRunStorage()
    {
        typedef typename RangeRunStorage::value_type value_type;
        S &s = concepts::lvalue_of<S>();
        runs r1 = range_run_storage::runs(s);
        run_type r2 = r1(*sequence::begin(s));
        zero_type z = range_run_storage::zero(s);
        value_type vz = z;
        offset_type o = offset_type();
        value_type vg = range_run_storage::get_at(s, o);

        ignore_unused_variable_warning(r1);
        ignore_unused_variable_warning(r2);
        ignore_unused_variable_warning(vz);
        ignore_unused_variable_warning(vg);
    }
};

template<typename S>
struct InfiniteRangeRunStorage
  : RangeRunStorage<S>
{
    typedef
        typename result_of<
            op::pre_run(typename add_reference<S>::type)
        >::type
    pre_run_type;

    BOOST_CONCEPT_ASSERT((Run<pre_run_type>));

    typedef
        typename result_of<
            op::post_run(typename add_reference<S>::type)
        >::type
    post_run_type;

    BOOST_CONCEPT_ASSERT((Run<post_run_type>));

    ~InfiniteRangeRunStorage()
    {
        S &s = concepts::lvalue_of<S>();
        pre_run_type p = range_run_storage::pre_run(s);
        post_run_type m = range_run_storage::post_run(s);
        typename InfiniteRangeRunStorage::value_type pv = range_run_storage::pre_value(s);
        typename InfiniteRangeRunStorage::value_type mv = range_run_storage::post_value(s);
        ignore_unused_variable_warning(p);
        ignore_unused_variable_warning(m);
        ignore_unused_variable_warning(pv);
        ignore_unused_variable_warning(mv);
    }
};

template<typename OI, typename S>
struct OrderedInserter
{
    typedef typename RangeRunStorage<S>::value_type value_type;
    typedef typename RangeRunStorage<S>::run_type run_type;

    ~OrderedInserter()
    {
        OI &oi = concepts::lvalue_of<OI>();
        run_type &r = concepts::lvalue_of<run_type>();
        value_type &v = concepts::lvalue_of<value_type>();

        range_run_storage::set_at(oi, r, v);
        range_run_storage::commit(oi);
    }
};

template<typename S>
struct Mutable_RangeRunStorage
  : RangeRunStorage<S>
  , Mutable_Sequence<S>
{
    typedef
        typename result_of<
            op::ordered_inserter(typename add_reference<S>::type)
        >::type
    ordered_inserter_type;

    BOOST_CONCEPT_ASSERT((OrderedInserter<ordered_inserter_type, S>));

    ~Mutable_RangeRunStorage()
    {
        typedef typename Mutable_RangeRunStorage::value_type value_type;
        typedef typename Mutable_RangeRunStorage::run_type run_type;

        S &s = concepts::lvalue_of<S>();
        run_type &r = concepts::lvalue_of<run_type>();
        value_type &v = concepts::lvalue_of<value_type>();

        range_run_storage::set_at(s, r, v);
        ordered_inserter_type oi = range_run_storage::ordered_inserter(s);
        ignore_unused_variable_warning(oi);

        range_run_storage::zero(s, v);
    }
};

template<typename S>
struct Mutable_InfiniteRangeRunStorage
  : InfiniteRangeRunStorage<S>
  , Mutable_RangeRunStorage<S>
{
    ~Mutable_InfiniteRangeRunStorage()
    {
        typedef typename Mutable_InfiniteRangeRunStorage::value_type value_type;

        S &s = concepts::lvalue_of<S>();
        value_type &v = concepts::lvalue_of<value_type>();

        range_run_storage::pre_value(s, v);
        range_run_storage::post_value(s, v);
    }
};

}}} // namespace boost::range_run_storage::concepts

#endif // BOOST_RANGE_RUN_STORAGE_CONCEPTS_DWA200658_HPP
