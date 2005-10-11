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
  
  template <typename Function, typename X, typename Tuple>
  struct MapHelper;
  
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
  struct Map {
    template <typename X>
    class MapDetail {
      typedef typename add_reference<X>::type XRef;
      XRef x;
    public:      
      template <typename T>
      void applyHead(const T& head) {
        Function::template apply<T>::value(x, head);
      }
      
      template <typename T>
      T transformHead() {
        return Function::template apply<T>::value(x);
      }
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
        applyHead(t.get_head());
        apply(t.get_tail());
      }
      
      template <typename Tuple>
      Tuple transform() {
        return MapHelper<Function, X, Tuple>::apply(this);
      }
      
      template <typename Tuple>
      void revApply(const Tuple& t) {
        apply(t.get_tail());
        applyHead(t.get_head());
      }
      
      template <typename Tuple>
      Tuple revTransform() {
        return MapHelper<Function, X, Tuple>::revApply(this);
      }
    };
    template <typename X, typename Tuple>
    static void apply(X x, const Tuple& t) {
      MapDetail<X>(x).apply(t);
    }
    
    template <typename X, typename Tuple> 
    static Tuple transform(X x) {
      return MapDetail<X>(x).transform<Tuple>();
    }
    
    template <typename X, typename Tuple>
    static void revApply(X x, const Tuple& t) {
      MapDetail<X>(x).revApply(t);
    }
    
    template <typename X, typename Tuple>
    static Tuple revTransform(X x) {
      return MapDetail<X>(x).revTransform<Tuple>();
    }
  };
  
  template <typename Function, typename X, typename Tuple>
  struct MapHelper {
    typedef typename Tuple::head_type head_type;
    typedef typename Tuple::tail_type tail_type;
    static Tuple apply(typename Map<Function>::template MapDetail<X>* d) {
      head_type head = d->template transformHead<head_type>();
      tail_type tail = d->template transform<tail_type>();
      return tuples::cons<head_type, tail_type>(head, tail);
    }
    static Tuple revApply(typename Map<Function>::template MapDetail<X>* d) {
      tail_type tail = d->template transform<tail_type>();
      head_type head = d->template transformHead<head_type>();
      return tuples::cons<head_type, tail_type>(head, tail);
    }
  };
  
  template <typename Function, typename X>
  struct MapHelper<Function, X, tuple<> > {
    static tuple<> apply(typename Map<Function>::template MapDetail<X>*) { return make_tuple(); }
    static tuple<> revApply(typename Map<Function>::template MapDetail<X>*) { return make_tuple(); }
  };
  
  template <typename Function, typename X>
  struct MapHelper<Function, X, tuples::null_type > {
    static tuples::null_type apply(typename Map<Function>::template MapDetail<X>*) { return tuples::null_type(); }
    static tuples::null_type revApply(typename Map<Function>::template MapDetail<X>*) { return tuples::null_type(); }
  };  
  
}; // namespace cpplua

#endif // UTILS_TUPLES_H
