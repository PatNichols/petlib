#ifndef PETLIB_ARRAY_EXPR_HPP
#define PETLIB_ARRAY_EXPR_HPP
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>

namespace petlib {

#define PETLIB_EXPR_OP(OP,SYM)\
template < class T > struct OP { \
    typedef T return_type;\
    static inline return_type eval(const T& x,const T& y) noexcept { return x SYM y;} \
}; \
  \
template < class T > struct Rev ## OP { \
    typedef T return_type;\
    static inline return_type eval(const T& x,const T& y) noexcept { return y SYM x;} \
}; 

PETLIB_EXPR_OP(Add,+)
PETLIB_EXPR_OP(Sub,-)
PETLIB_EXPR_OP(Mul,*)
PETLIB_EXPR_OP(Div,/)
PETLIB_EXPR_OP(Mod,%)
PETLIB_EXPR_OP(Xor,^)
PETLIB_EXPR_OP(Ior,|)
PETLIB_EXPR_OP(And,&)
PETLIB_EXPR_OP(Lsh,<<)
PETLIB_EXPR_OP(Rsh,>>)
#undef PETLIB_EXPR_OP


#define PETLIB_EXPR_OP(OP,SYM)\
template < class T > struct OP { \
    typedef bool return_type;\
    static inline return_type eval(const T& x,const T& y) noexcept { return x SYM y;} \
};  \
   \
template < class T > struct Rev ## OP { \
    typedef bool return_type;\
    static inline return_type eval(const T& x,const T& y) noexcept { return y SYM x;} \
};  \
   \
   
PETLIB_EXPR_OP(Eq,==)
PETLIB_EXPR_OP(NotEq,!=)
#undef PETLIB_EXPR_OP

#define PETLIB_EXPR_OP(OP,SYM)\
template < class T > struct OP { \
    typedef T return_type;\
    static inline return_type eval(const T& x) noexcept { return SYM x;} \
};

PETLIB_EXPR_OP(Not,!)
PETLIB_EXPR_OP(Com,~)
PETLIB_EXPR_OP(Neg,-)
PETLIB_EXPR_OP(Pos,+)
#undef PETLIB_EXPR_OP


template < typename Tp, class Array_t > 
struct ArrayRef {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp value_type;
    typedef Tp& reference;
    typedef const Tp& const_reference;
    const Array_t& a;

    ArrayRef(const Array_t& a0) noexcept:a(a0) {}
    size_type size() const noexcept { return a.size();}
    const_reference operator[](size_type i) const noexcept {
        return a[i];
    }
};

template < typename Tp > 
struct ScalarRef {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp value_type;
    typedef Tp& reference;
    typedef const Tp& const_reference;
    const Tp& a;

    ScalarRef(const Tp& a0) noexcept:a(a0) {}
    const_reference operator[](size_type i) const noexcept {
        return a;
    }
    const_reference operator()(size_type i,size_type j) const noexcept {
        return a;
    }
};

template < typename Tp, class Array_t > 
struct ArrayBase {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp value_type;
    typedef Tp& reference;
    typedef const Tp& const_reference;

    const Array_t* CastDown() const noexcept {
        return static_cast< const Array_t* >(this);
    }
    size_type size() const noexcept { return CastDown()->size();}
    const_reference operator[](size_type i) const noexcept {
        return CastDown()->operator[](i);
    }
};

template < typename Tp, class Expr_t > 
struct ArrayExpr {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp return_type;
    Expr_t a;

    ArrayExpr(const Expr_t& a0) noexcept:a(a0)  {}
    size_type size() const noexcept { return a.size();}
    return_type operator[](size_type i) const noexcept {
        return a[i];
    }
};

template < class Op_t, class Arg1, class Arg2 > 
struct ArrayBinExpr {
    typedef std::size_t size_type;
    typedef typename Op_t::return_type return_type;
    Arg1 a;
    Arg2 b;
    
    ArrayBinExpr(const Arg1& a0,const Arg2& b0) noexcept:a(a0),b(b0) {}
    size_type size() const noexcept { return a.size();}
    return_type operator[](size_type i) const noexcept {
        return Op_t::eval(a[i],b[i]);
    }
};

template < class Op_t, class Arg  > 
struct ArrayUnExpr {
    typedef std::size_t size_type;
    typedef typename Op_t::return_type return_type;
    Arg a;
    
    ArrayUnExpr(const Arg& a0) noexcept:a(a0) {}
    size_type size() const noexcept { return a.size();}
    return_type operator[](size_type i) const noexcept {
        return Op_t::eval(a[i]);
    }
};

#define PETLIB_EXPR_OP(OP,SYM)\
template < typename T, class A, class B > \
inline  ArrayExpr< T, \
  ArrayBinExpr< OP<T>, ArrayRef< T, A >, ArrayRef< T, B > > > \
  operator SYM (const ArrayBase< T, A >& a, const ArrayBase< T, B >& b) noexcept {\
   typedef ArrayBinExpr< OP<T>, ArrayRef< T, A >, ArrayRef< T, B > >  xpr_t;\
   return ArrayExpr< T, xpr_t >(xpr_t( ArrayRef< T, A >(*a.CastDown()), ArrayRef< T, B >(*b.CastDown()) ) );\
} \
  \
template < typename T, class A, class B > \
inline  ArrayExpr< T, \
  ArrayBinExpr< OP<T>, ArrayRef< T, A >, ArrayExpr< T, B > > > \
  operator SYM (const ArrayBase< T, A >& a, const ArrayExpr< T, B >& b) noexcept {\
   typedef ArrayBinExpr< OP<T>, ArrayRef< T, A >, ArrayExpr< T, B > >  xpr_t;\
   return ArrayExpr< T, xpr_t >(xpr_t( ArrayRef< T, A >(*a.CastDown()), b ) );\
} \
  \
template < typename T, class A, class B > \
inline  ArrayExpr< T, \
  ArrayBinExpr< OP<T>, ArrayExpr< T, A >, ArrayRef< T, B > > > \
  operator SYM (const ArrayExpr< T, A >& a, const ArrayBase< T, B >& b) noexcept {\
   typedef ArrayBinExpr< OP<T>, ArrayExpr< T, A >, ArrayRef< T, B > >  xpr_t;\
   return ArrayExpr< T, xpr_t >(xpr_t( a, ArrayRef< T, B >(*b.CastDown()) ) );\
} \
  \
template < typename T, class A, class B > \
inline  ArrayExpr< T, \
  ArrayBinExpr< OP<T>, ArrayExpr< T, A >, ArrayExpr< T, B > > > \
  operator SYM (const ArrayExpr< T, A >& a, const ArrayExpr< T, B >& b) noexcept {\
   typedef ArrayBinExpr< OP<T>, ArrayExpr< T, A >, ArrayExpr< T, B > >  xpr_t;\
   return ArrayExpr< T, xpr_t >(xpr_t( a, b ) );\
} \
  \
template < typename T, class A > \
inline  ArrayExpr< T, \
  ArrayBinExpr< OP<T>, ArrayRef< T, A >, ScalarRef< T > > > \
  operator SYM (const ArrayBase< T, A >& a, const T& b) noexcept {\
   typedef ArrayBinExpr< OP<T>, ArrayRef< T, A >, ScalarRef< T > >  xpr_t;\
   return ArrayExpr< T, xpr_t >(xpr_t( ArrayRef< T, A >(*a.CastDown()), ScalarRef< T > (b) ) );\
} \
  \
template < typename T, class A > \
inline  ArrayExpr< T, \
  ArrayBinExpr< Rev ## OP<T>, ArrayRef< T, A >, ScalarRef< T > > > \
  operator SYM (const T& b,const ArrayBase< T, A >& a) noexcept {\
   typedef ArrayBinExpr< Rev ## OP<T>, ArrayRef< T, A >, ScalarRef< T > >  xpr_t;\
   return ArrayExpr< T, xpr_t >(xpr_t( ArrayRef< T, A >(*a.CastDown()), ScalarRef< T >(b) ) );\
} \
  \
template < typename T, class A > \
inline  ArrayExpr< T, \
  ArrayBinExpr< OP<T>, ArrayExpr< T, A >, ScalarRef< T > > > \
  operator SYM (const ArrayExpr< T, A >& a, const T& b) noexcept {\
   typedef ArrayBinExpr< OP<T>, ArrayExpr< T, A >, ScalarRef< T > >  xpr_t;\
   return ArrayExpr< T, xpr_t >(xpr_t( a, ScalarRef< T > (b) ) );\
} \
  \
template < typename T, class A > \
inline  ArrayExpr< T, \
  ArrayBinExpr< Rev ## OP<T>, ArrayExpr< T, A >, ScalarRef< T > > > \
  operator SYM (const T& b,const ArrayExpr< T, A >& a) noexcept {\
   typedef ArrayBinExpr< Rev ## OP<T>, ArrayExpr< T, A >, ScalarRef< T > >  xpr_t;\
   return ArrayExpr< T, xpr_t >(xpr_t( a, ScalarRef< T >(b) ) );\
}


PETLIB_EXPR_OP(Add,+)
PETLIB_EXPR_OP(Sub,-)
PETLIB_EXPR_OP(Mul,*)
PETLIB_EXPR_OP(Div,/)
PETLIB_EXPR_OP(Mod,%)
PETLIB_EXPR_OP(Xor,^)
PETLIB_EXPR_OP(Ior,|)
PETLIB_EXPR_OP(And,&)
PETLIB_EXPR_OP(Lsh,<<)
PETLIB_EXPR_OP(Rsh,>>)
#undef PETLIB_EXPR_OP

#define PETLIB_EXPR_OP(OP,SYM)\
template < class A, class B, class T > \
inline  ArrayExpr< bool,\
 ArrayBinExpr< OP<T> , ArrayRef< T, A >, ArrayRef< T, B > > > \
operator SYM (const ArrayBase< T, A >& a,const ArrayBase<T,B>& b) noexcept {\
 typedef  ArrayBinExpr< OP<T> , ArrayRef< T, A >, ArrayRef< T, B > >   xpr_t;\
 return ArrayExpr< bool, xpr_t >(xpr_t(ArrayRef< T, A >(*a.CastDown()),ArrayRef< T, B >(*b.CastDown())));\
} \
  \
template < class A, class B, class T > \
inline  ArrayExpr< bool,\
 ArrayBinExpr< OP<T> , ArrayRef< T, A >, ArrayExpr< T, B > > > \
operator SYM (const ArrayBase< T, A >& a,const ArrayExpr<T,B>& b) noexcept {\
 typedef  ArrayBinExpr< OP<T> , ArrayRef< T, A >, ArrayExpr< T, B > >   xpr_t;\
 return ArrayExpr< bool, xpr_t >(xpr_t(ArrayRef< T, A >(*a.CastDown()),b));\
} \
  \
template < class A, class B, class T > \
inline  ArrayExpr< bool,\
 ArrayBinExpr< OP<T> , ArrayExpr< T, A >, ArrayRef< T, B > > > \
operator SYM (const ArrayExpr< T, A >& a,const ArrayBase<T,B>& b) noexcept {\
 typedef  ArrayBinExpr< OP<T> , ArrayExpr< T, A >, ArrayRef< T, B > >   xpr_t;\
 return ArrayExpr< bool, xpr_t >(xpr_t(a,ArrayRef< T, B >(*b.CastDown())));\
} \
  \
template < class A, class B, class T > \
inline  ArrayExpr< bool,\
 ArrayBinExpr< OP<T> , ArrayExpr< T, A >, ArrayExpr< T, B > > > \
operator SYM (const ArrayExpr< T, A >& a,const ArrayExpr<T,B>& b) noexcept {\
 typedef  ArrayBinExpr< OP<T> , ArrayExpr< T, A >, ArrayExpr< T, B > >   xpr_t;\
 return ArrayExpr< bool, xpr_t >(xpr_t(a,b));\
} \
  \
template < class A, class T  > \
inline  ArrayExpr< bool,\
 ArrayBinExpr< OP<T> , ArrayRef< T, A >, ScalarRef< T > > > \
operator SYM (const ArrayBase< T, A >& a,const T& b) noexcept {\
 typedef  ArrayBinExpr< OP<T> , ArrayRef< T, A >, ScalarRef< T > >   xpr_t;\
 return ArrayExpr< bool, xpr_t >(xpr_t(ArrayRef< T, A >(*a.CastDown()),ScalarRef<T>(b)));\
} \
  \
template < class A, class T > \
inline  ArrayExpr< typename Rev ## OP<T>::return_type,\
 ArrayBinExpr< Rev ## OP<T> , ArrayRef< T, A >, ScalarRef< T > > > \
operator SYM (const T& b, const ArrayBase< T, A >& a) noexcept {\
 typedef  ArrayBinExpr< Rev ## OP<T> , ArrayRef< T, A >, ScalarRef< T > >   xpr_t;\
 return ArrayExpr< bool, xpr_t >(xpr_t(ArrayRef< T, A >(*a.CastDown()),ScalarRef<T>(b)));\
} \
  \
template < class A, class T > \
inline  ArrayExpr< bool,\
 ArrayBinExpr< OP<T> , ArrayExpr< T, A >, ScalarRef< T > > > \
operator SYM (const ArrayExpr< T, A >& a,const T& b) noexcept {\
 typedef  ArrayBinExpr< OP<T> , ArrayExpr< T, A >, ScalarRef< T > >   xpr_t;\
 return ArrayExpr< bool, xpr_t >(xpr_t(a,ScalarRef<T>(b)));\
} \
  \
template < class A, class T > \
inline  ArrayExpr< bool,\
 ArrayBinExpr< Rev ## OP<T> , ArrayExpr< T, A >, ScalarRef< T > > > \
operator SYM (const T& b, const ArrayExpr< T, A >& a) noexcept {\
 typedef  ArrayBinExpr< Rev ## OP<T> , ArrayExpr< T, A >, ScalarRef< T > >   xpr_t;\
 return ArrayExpr< bool, xpr_t >(xpr_t(a,ScalarRef<T>(b)));\
} 

PETLIB_EXPR_OP(Eq,==)
PETLIB_EXPR_OP(NotEq,!=)
#undef PETLIB_EXPR_OP



#define PETLIB_EXPR_OP(OP,SYM)\
template < class A, class T > \
inline  ArrayExpr< T,\
 ArrayUnExpr< OP<T> , ArrayRef< T, A > > > \
operator SYM (const ArrayBase< T, A >& a) noexcept {\
 typedef  ArrayUnExpr< OP<T> , ArrayRef< T, A > > xpr_t;\
 return ArrayExpr< T , xpr_t >(xpr_t(ArrayRef< T, A >(*a.CastDown())));\
} \
  \
template < class A, class T > \
inline  ArrayExpr< T,\
 ArrayUnExpr< OP<T>, ArrayExpr< T, A > > > \
operator SYM (const ArrayExpr< T, A >& a) noexcept {\
 typedef  ArrayUnExpr< OP<T> , ArrayExpr< T, A > > xpr_t;\
 return ArrayExpr< T , xpr_t >(xpr_t(a));\
}

PETLIB_EXPR_OP(Not,!)
PETLIB_EXPR_OP(Com,~)
PETLIB_EXPR_OP(Neg,-)
PETLIB_EXPR_OP(Pos,+)
#undef PETLIB_EXPR_OP

template < class T,  class A, class X >  
inline void ExprCopy( A a, const ArrayExpr< T, X >& x, std::size_t n) noexcept { 
    for (std::size_t i=0;i<n;++i,++a) *a = x[i]; 
}

#define PETLIB_EXPR_OP(OP,SYM)\
template < class T,  class A, class X > \
inline void Expr ## OP ## Copy( A a, const ArrayExpr< T, X >& x, std::size_t n) noexcept {\
    for (std::size_t i=0;i<n;++i,++a) *a SYM x[i];\
} 

PETLIB_EXPR_OP(Add,+=)
PETLIB_EXPR_OP(Sub,-=)
PETLIB_EXPR_OP(Mul,*=)
PETLIB_EXPR_OP(Div,/=)
PETLIB_EXPR_OP(Mod,%=)
PETLIB_EXPR_OP(Xor,^=)
PETLIB_EXPR_OP(Ior,|=)
PETLIB_EXPR_OP(And,&=)
PETLIB_EXPR_OP(Lsh,<<=)
PETLIB_EXPR_OP(Rsh,>>=)
#undef PETLIB_EXPR_OP

template < class T, class A > inline std::ostream& operator<<(std::ostream& os,const ArrayBase<T,A>& a) {
  std::size_t asize = a.size();
  os << "---------------------------------------------\n";
  os << " index       |    value \n";   
  for (std::size_t i=0;i<asize;++i) {
      os << std::setw(12) << i << " - " << a[i] << "\n";
  }
  os << "---------------------------------------------\n";
  return os;
}

template < class T, class A > inline std::ostream& operator<<(std::ostream& os,const ArrayExpr<T,A>& a) {
  std::size_t asize = a.size();
  os << "---------------------------------------------\n";
  os << " index       |    value \n";   
  for (std::size_t i=0;i<asize;++i) {
      os << std::setw(12) << i << " - " << a[i] << "\n";
  }
  os << "---------------------------------------------\n";
  return os;
}

}
#endif

  
