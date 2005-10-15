#ifndef UTILS_TRAITS
#define UTILS_TRAITS

#include <boost/type_traits/function_traits.hpp>
#include <boost/tuple/tuple.hpp>

namespace cpplua {

//BEGIN FunctionTraits

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

template <typename traits>
struct FunctionTraitsHelper<5, traits> {
  typedef tuple<typename traits::arg1_type,
                typename traits::arg2_type,
                typename traits::arg3_type,
                typename traits::arg4_type,
                typename traits::arg5_type> type;
};

template <typename Function>
struct FunctionTraits {
  typedef boost::function_traits<Function> traits;
  typedef typename traits::result_type result_type;
  typedef typename FunctionTraitsHelper<traits::arity, traits>::type args_type;
};

//END FunctionTraits

#if 0
// BEGIN FunctionSignature
template <typename RetVal, typename Tuple>
struct FunctionSignature {};
template <typename RetVal>
struct FunctionSignature<RetVal, tuple<> > {
  typedef RetVal(*)() value;
}
template <typename RetVal, typename Arg0>
struct FunctionSignature<RetVal, tuple<Arg0> > {
  typedef RetVal(*)(Arg0) value;
}  
template <typename RetVal, typename Arg0, typename Arg1>
struct FunctionSignature<RetVal, tuple<Arg0, Arg1> > {
  typedef RetVal(*)(Arg0, Arg1) value;
}
template <typename RetVal, typename Arg0, typename Arg1, typename Arg2>
struct FunctionSignature<RetVal, tuple<Arg0, Arg1, Arg2> > {
  typedef RetVal(*)(Arg0, Arg1, Arg2) value;
}
template <typename RetVal, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
struct FunctionSignature<RetVal, tuple<Arg0, Arg1, Arg2, Arg3> > {
  typedef RetVal(*)(Arg0, Arg1, Arg2, Arg3) value;
}
template <typename RetVal, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
struct FunctionSignature<RetVal, tuple<Arg0, Arg1, Arg2, Arg3, Arg4> > {
  typedef RetVal(*)(Arg0, Arg1, Arg2, Arg3, Arg4) value;
}
// END
#endif

};

#endif // UTILS_TRAITS
