#ifndef UTILS_TRAITS
#define UTILS_TRAITS

#include <boost/type_traits/function_traits.hpp>
#include <boost/tuple/tuple.hpp>

namespace cpplua {

template <int n, typename traits>
struct FunctionTraitsHelper {};

template <typename traits>
struct FunctionTraitsHelper<0, traits> {
  typedef tuple<> type;
};

template <typename traits>
struct FunctionTraitsHelper<1, traits> {
  typedef tuple<typename traits::arg1_type> type;
};

template <typename traits>
struct FunctionTraitsHelper<2, traits> {
  typedef tuple<typename traits::arg1_type,
                typename traits::arg2_type> type;
};

template <typename traits>
struct FunctionTraitsHelper<3, traits> {
  typedef tuple<typename traits::arg1_type,
                typename traits::arg2_type,
                typename traits::arg3_type> type;
};

template <typename traits>
struct FunctionTraitsHelper<4, traits> {
  typedef tuple<typename traits::arg1_type,
                typename traits::arg2_type,
                typename traits::arg3_type,
                typename traits::arg4_type> type;
};


template <typename Function>
struct FunctionTraits {
  typedef boost::function_traits<Function> traits;
  typedef typename traits::result_type result_type;
  typedef typename FunctionTraitsHelper<traits::arity, traits>::type args_type;
};

};

#endif // UTILS_TRAITS
