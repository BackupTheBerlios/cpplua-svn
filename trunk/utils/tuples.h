#ifndef UTILS_TUPLES_H
#define UTILS_TUPLES_H

#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace cpplua {
  
using namespace boost;

//   // BEGIN FunctionSignature
//   template <typename RetVal, typename Tuple>
//   struct FunctionSignature {};
//   template <typename RetVal>
//   struct FunctionSignature<RetVal, tuple<> > {
//     typedef RetVal(*)() value;
//   }
//   template <typename RetVal, typename Arg0>
//   struct FunctionSignature<RetVal, tuple<Arg0> > {
//     typedef RetVal(*)(Arg0) value;
//   }  
//   template <typename RetVal, typename Arg0, typename Arg1>
//   struct FunctionSignature<RetVal, tuple<Arg0, Arg1> > {
//     typedef RetVal(*)(Arg0, Arg1) value;
//   }
//   template <typename RetVal, typename Arg0, typename Arg1, typename Arg2>
//   struct FunctionSignature<RetVal, tuple<Arg0, Arg1, Arg2> > {
//     typedef RetVal(*)(Arg0, Arg1, Arg2) value;
//   }
//   // END

  // BEGIN ApplyFunctionToTuple
  template <typename Function, typename Tuple>
  struct ApplyFunctionToTuple {};
  
  template <typename RetVal>
  struct ApplyFunctionToTuple<RetVal(*)(), tuple<> > {
    static RetVal apply(RetVal(*f)(), const tuple<>&) {
      return f();
    }
  };
  
  template <typename RetVal, typename Arg0>
  struct ApplyFunctionToTuple<RetVal(*)(Arg0), tuple<Arg0> > {
    static RetVal apply(RetVal(*f)(Arg0), const tuple<Arg0>& args) {
      return f(args.get<0>());
    }
  };
  
  template <typename RetVal, typename Arg0, typename Arg1>
  struct ApplyFunctionToTuple<RetVal(*)(Arg0, Arg1), tuple<Arg0, Arg1> > {
    static RetVal apply(RetVal(*f)(Arg0, Arg1), const tuple<Arg0, Arg1>& args) {
      return f(args.get<0>(), args.get<1>());
    }
  };
  
  template <typename RetVal, typename Arg0, typename Arg1, typename Arg2>
  struct ApplyFunctionToTuple<RetVal(*)(Arg0, Arg1, Arg2), tuple<Arg0, Arg1, Arg2> > {
    static RetVal apply(RetVal(*f)(Arg0, Arg1, Arg2), const tuple<Arg0, Arg1, Arg2>& args) {
      return f(args.get<0>(), args.get<1>(), args.get<2>());
    }
  };
  // END ApplyFunctionToTuple

  /**
    * @code
    * Map<F>::apply(x, t)
    * is equivalent to
    * F::apply<element<0, Tuple>::type >::value(x, t.get<0>());
    * // ...
    * F::apply<element<n, Tuple>::type >::value(x, t.get<n>());
    * @endcode
    */
  template <typename Function>
  class Map {
    template <typename X>
    class MapDetail {
      typedef typename add_reference<X>::type XRef;
      XRef x;
    public:
      MapDetail(XRef x)
      : x(x) {}
      
      void apply(const tuple<>&) {
        // do nothing
      }
      
      void apply(const tuples::null_type&) {
        // do nothing
      }
      
      template <typename Tuple>
      void apply(const Tuple& t) {
        
        Function::template apply< typename tuples::element<0, Tuple>::type  >::value(x, t.get<0>());
        apply(t.get_tail());
      }
    };
  public:
    template <typename X, typename Tuple>
    static void apply(X x, const Tuple& t) {
      MapDetail<X>(x).apply(t);
    }
  };
}; // namespace cpplua

#endif // UTILS_TUPLES_H
