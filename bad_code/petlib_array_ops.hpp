#ifndef PETLIB_ARRAY_OPS_H
#define PETLIB_ARRAY_OPS_H

#include <cstdint>
#include <cstdlib>

namespace petlib {

template <typename A>
struct auto_promote_traits {
  typedef A promote_t;
};

template <>
struct auto_promote_traits<bool> {
  typedef int promote_t;
};
template <>
struct auto_promote_traits<char> {
  typedef int promote_t;
};
template <>
struct auto_promote_traits<unsigned char> {
  typedef int promote_t;
};
template <>
struct auto_promote_traits<short> {
  typedef int promote_t;
};
template <>
struct auto_promote_traits<unsigned short> {
  typedef unsigned int promote_t;
};

template <typename A>
struct type_promote_traits {
  static const bool is_known=false;
  static const int rank=0;
};

template <>
struct type_promote_traits<int> {
  static const bool is_known=true;
  static const int rank=10;
};

template <>
struct type_promote_traits<unsigned int> {
  static const bool is_known=true;
  static const int rank=15;
};

template <>
struct type_promote_traits<int64_t> {
  static const bool is_known=true;
  static const int rank=20;
};

template <>
struct type_promote_traits<uint64_t> {
  static const bool is_known=true;
  static const int rank=25;
};

template <>
struct type_promote_traits<float> {
  static const bool is_known=(true);
  static const int rank=(30);
};

template <>
struct type_promote_traits<double> {
  static const bool is_known=(true);
  static const int rank=(40);
};

template <>
struct type_promote_traits<long double> {
  static const bool is_known=(true);
  static const int rank=(50);
};

template <typename A, typename B, bool promote_to_A>
struct promoter {
  typedef A promote_t;
};

template <typename A, typename B>
struct promoter<A, B, false> {
  typedef B promote_t;
};

template <typename A, typename B>
struct promote_traits {
  typedef typename auto_promote_traits<A>::promote_t a_type;
  typedef typename auto_promote_traits<B>::promote_t b_type;

  static const bool t1_is_better =
      type_promote_traits<a_type>::rank > type_promote_traits<b_type>::rank;
  static const bool know_ranks = type_promote_traits<a_type>::is_known >
                                 type_promote_traits<b_type>::is_known;
  static const bool promote_to_a = t1_is_better && know_ranks;

  typedef typename promoter<a_type, b_type, promote_to_a>::promote_t promote_t;
};


template <class Array_t, typename A_t>
struct ArrayBase {
  typedef A_t value_t;
  typedef std::size_t size_type;
  constexpr const Array_t* leaf() const {
    return static_cast<const Array_t*>(this);
  };
  constexpr value_t operator[](size_t i) const {
    return leaf()->operator[](i);
  };
  constexpr size_type size() const { return leaf()->size(); };
};

template <class Array_t, typename A_t>
struct ArrayRef {
  typedef A_t value_t;
  typedef std::size_t size_type;
  const Array_t& a;

  ArrayRef(const ArrayBase<Array_t,A_t>& a0) : a(*a0.leaf()){};
  constexpr value_t operator[](size_t i) const { return a[i]; }
  constexpr size_type size() const { return a.size(); }
};


template <typename T>
struct ScalarRef {
  typedef T value_t;
  typedef std::size_t size_type;

  const T a;

  ScalarRef(const T& a0) : a(a0){};
  constexpr value_t operator[](size_t i) const { return a; }
};


template <class A, class Op>
struct UnaryXpr {
  typedef typename Op::value_t result_t;
  typedef typename Op::value_t value_t;
  typedef std::size_t size_type;
  const A a;

  UnaryXpr(const A& a0) : a(a0){};
  constexpr result_t operator[](size_t i) const { return Op::eval(a[i]); }
  constexpr size_type size() const { return a.size(); }
};

template <class A, class B, class Op>
struct BinaryXpr {
  typedef typename Op::value_t result_t;
  typedef typename A::value_t a_value_t;
  typedef typename B::value_t b_value_t;
  typedef result_t value_t;
  typedef std::size_t size_type;

  const A a;
  const B b;

  BinaryXpr(const A& a0, const B& b0) : a(a0), b(b0){};
   constexpr result_t operator[](size_t i) const { return Op::eval(a[i], b[i]); }
  constexpr size_type size() const { return a.size(); }
};

template <class Xpr_t>
struct ArrayXpr {
  typedef typename Xpr_t::value_t result_t;
  typedef typename Xpr_t::value_t value_t;
  typedef std::size_t size_type;
  const Xpr_t a;

  ArrayXpr(const Xpr_t& a0) : a(a0){};
   constexpr result_t operator[](size_t i) const { return a[i]; }
  constexpr size_type size() const { return a.size(); }
};

template <class Xpr_t>
struct ScalarXpr {
  typedef typename Xpr_t::value_t result_t;
  typedef typename Xpr_t::value_t value_t;
  typedef std::size_t size_type;
  const Xpr_t a;

  ScalarXpr(const Xpr_t& a0) : a(a0){};
   constexpr result_t operator[](size_t i) const { return a[i]; }
  constexpr size_type size() const { return 1; }
};

template <typename A, typename B>
struct AddOp {
  typedef typename promote_traits<A, B>::promote_t value_t;
  static value_t eval(const A& x, const B& y) noexcept { return x + y; }
};

template <typename A, typename B>
struct SubOp {
  typedef typename promote_traits<A, B>::promote_t value_t;
  static value_t eval(const A& x, const B& y) noexcept { return x - y; }
};

template <typename A, typename B>
struct MulOp {
  typedef typename promote_traits<A, B>::promote_t value_t;
  static value_t eval(const A& x, const B& y) noexcept { return x * y; }
};

template <typename A, typename B>
struct DivOp {
  typedef typename promote_traits<A, B>::promote_t value_t;
  static value_t eval(const A& x, const B& y) noexcept { return x / y; }
};

template <typename A, typename B>
struct RevAddOp {
  typedef typename promote_traits<A, B>::promote_t value_t;
  static value_t eval(const A& x, const B& y) noexcept { return y + x; }
};

template <typename A, typename B>
struct RevSubOp {
  typedef typename promote_traits<A, B>::promote_t value_t;
  static value_t eval(const A& x, const B& y) noexcept { return y - x; }
};

template <typename A, typename B>
struct RevMulOp {
  typedef typename promote_traits<A, B>::promote_t value_t;
  static value_t eval(const A& x, const B& y) noexcept { return y * x; }
};

template <typename A, typename B>
struct RevDivOp {
  typedef typename promote_traits<A, B>::promote_t value_t;
  static value_t eval(const A& x, const B& y) noexcept { return y / x; }
};

template <typename A>
struct NegOp {
  typedef typename auto_promote_traits<A>::promote_t value_t;
  static value_t eval(const A& x) noexcept { return -x; };
};

template <typename A>
struct NotOp {
  typedef typename auto_promote_traits<A>::promote_t value_t;
  static value_t eval(const A& x) noexcept { return ~x; };
};

template <typename A>
struct PlusOp {
  typedef typename auto_promote_traits<A>::promote_t value_t;
  static value_t eval(const A& x) noexcept { return +x; };
};

#define PETLIB_MAKE_OP_(name_,sym_) \
template <class ArrayA_t, class ArrayB_t, typename A_t, typename B_t> \
constexpr ArrayXpr<BinaryXpr<ArrayRef<ArrayA_t, A_t>, ArrayRef<ArrayB_t, B_t>,\
                             name_##Op<A_t, B_t> > > \
operator sym_ (const ArrayBase<ArrayA_t, A_t>& a,\
          const ArrayBase<ArrayB_t, B_t>& b) {\
  typedef BinaryXpr<ArrayRef<ArrayA_t, A_t>, ArrayRef<ArrayB_t, B_t>,\
                             name_##Op<A_t, B_t> > \
      xpr_t; \
  return ArrayXpr<xpr_t>(xpr_t(ArrayRef<ArrayA_t,A_t>(a),ArrayRef<ArrayB_t,B_t>(b)));\
} \
  \
template <class ArrayA_t, class XprB_t, typename A_t> \
 constexpr ArrayXpr<BinaryXpr<ArrayRef<ArrayA_t, A_t>, ArrayXpr<XprB_t>,\
                             name_##Op<A_t, typename XprB_t::value_t> > > \
operator sym_ (const ArrayBase<ArrayA_t, A_t>& a, const ArrayXpr<XprB_t>& b) { \
  typedef BinaryXpr<ArrayRef<ArrayA_t, A_t>, ArrayXpr<XprB_t>, \
                             name_##Op<A_t, typename XprB_t::value_t> > \
      xpr_t; \
  return ArrayXpr<xpr_t>(xpr_t(ArrayRef<ArrayA_t,A_t>(a), b)); \
} \
  \
template <class XprA_t, class ArrayB_t, typename B_t> \
 constexpr ArrayXpr<BinaryXpr<ArrayXpr<XprA_t>, ArrayRef<ArrayB_t, B_t>, \
                             name_##Op<typename XprA_t::value_t, B_t> > > \
operator sym_ (const ArrayXpr<XprA_t>& a, const ArrayBase<ArrayB_t, B_t>& b) { \
  typedef BinaryXpr<ArrayXpr<XprA_t>, ArrayRef<ArrayB_t, B_t>,\
                             name_##Op<typename XprA_t::value_t, B_t> > \
      xpr_t; \
  return ArrayXpr<xpr_t>(xpr_t(a, ArrayRef<ArrayB_t,B_t>(b))); \
} \
  \
template <class XprA_t, class XprB_t> \
 constexpr ArrayXpr< \
    BinaryXpr<ArrayXpr<XprA_t>, ArrayXpr<XprB_t>,\
              name_##Op<typename XprA_t::value_t, typename XprB_t::value_t> > > \
operator sym_ (const ArrayXpr<XprA_t>& a, const ArrayXpr<XprB_t>& b) { \
  typedef BinaryXpr<\
      ArrayXpr<XprA_t>, ArrayXpr<XprB_t>,\
      name_##Op<typename XprA_t::value_t, typename XprB_t::value_t> > \
      xpr_t; \
  return ArrayXpr<xpr_t>(xpr_t(a, b));\
}

PETLIB_MAKE_OP_(Add,+)
PETLIB_MAKE_OP_(Sub,-)
PETLIB_MAKE_OP_(Mul,*)
PETLIB_MAKE_OP_(Div,/)
#undef PETLIB_MAKE_OP_

#define PETLIB_MAKE_OP_(name_, sym_)                                         \
  template <class Array_t, class A_t>                                        \
   constexpr ArrayXpr<UnaryXpr<ArrayRef<Array_t, A_t>, name_##Op<A_t> > >      \
  operator sym_(const ArrayBase<Array_t, A_t>& a) {                          \
    typedef UnaryXpr<ArrayRef<Array_t, A_t>, name_##Op<A_t> > xpr_t; \
    return ArrayXpr<xpr_t>(xpr_t(ArrayRef<Array_t,A_t>(a)));                               \
  }                                                                          \
  \
\
template<class XprA_t>  constexpr ArrayXpr<                                   \
      UnaryXpr<ArrayXpr<XprA_t>, name_##Op<typename XprA_t::value_t> > >      \
  operator sym_(const ArrayXpr<XprA_t>& a) {                                 \
    typedef UnaryXpr<ArrayXpr<XprA_t>,                               \
                             name_##Op<typename XprA_t::value_t> >           \
        xpr_t;                                                               \
    return ArrayXpr<xpr_t>(xpr_t(a));                                        \
  }

PETLIB_MAKE_OP_(Neg,-)
PETLIB_MAKE_OP_(Not,~)
PETLIB_MAKE_OP_(Plus,+)
#undef PETLIB_MAKE_OP_

#define PETLIB_MAKE_OP_(name_,sym_,type_)\
template < class Array_t, typename A_t > \
 constexpr ArrayXpr< BinaryXpr<ArrayRef<Array_t,A_t>,ScalarRef< type_ >, name_##Op<A_t, type_ > > > \
operator sym_ (const ArrayBase<Array_t,A_t>& a,const  type_ & s) {\
 typedef BinaryXpr<ArrayRef<Array_t,A_t>,ScalarRef< type_ >, name_##Op<A_t, type_ > >  xpr_t;\
 return ArrayXpr<xpr_t>(xpr_t(ArrayRef<Array_t,A_t>(a),ScalarRef< type_ >(s)));\
} \
 \
template < class Array_t, typename A_t> \
 constexpr ArrayXpr< BinaryXpr<ArrayRef<Array_t,A_t>,ScalarRef< type_ >, Rev##name_##Op<A_t, type_ > > > \
operator sym_ (const  type_ & s,const ArrayBase<Array_t,A_t>& a) {\
 typedef BinaryXpr<ArrayRef<Array_t,A_t>,ScalarRef< type_ >, Rev##name_##Op<A_t, type_ > >  xpr_t;\
 return ArrayXpr<xpr_t>(xpr_t(ArrayRef<Array_t,A_t>(a),ScalarRef< type_ >(s)));\
} \
 \
template < class Xpr_t> \
 constexpr ArrayXpr< BinaryXpr< ArrayXpr<Xpr_t>,ScalarRef< type_ >, name_##Op<typename Xpr_t::value_t, type_ > > > \
operator sym_ (const ArrayXpr<Xpr_t>& a,const  type_ & s) {\
 typedef BinaryXpr<ArrayXpr<Xpr_t>,ScalarRef< type_ >, name_##Op<typename Xpr_t::value_t, type_ > >  xpr_t;\
 return ArrayXpr<xpr_t>(xpr_t(a,ScalarRef< type_ >(s)));\
} \
 \
template < class Xpr_t > \
 constexpr ArrayXpr< BinaryXpr< ArrayXpr<Xpr_t>,ScalarRef< type_ >, Rev##name_##Op<typename Xpr_t::value_t, type_ > > > \
operator sym_ (const  type_ & s,const ArrayXpr<Xpr_t>& a) {\
 typedef BinaryXpr<ArrayXpr<Xpr_t>,ScalarRef< type_ >, Rev##name_##Op<typename Xpr_t::value_t, type_ > >  xpr_t;\
 return ArrayXpr<xpr_t>(xpr_t(a,ScalarRef< type_ >(s)));\
} 

PETLIB_MAKE_OP_(Add,+,bool)
PETLIB_MAKE_OP_(Sub,-,bool)
PETLIB_MAKE_OP_(Mul,*,bool)
PETLIB_MAKE_OP_(Div,/,bool)
PETLIB_MAKE_OP_(Add,+,char)
PETLIB_MAKE_OP_(Sub,-,char)
PETLIB_MAKE_OP_(Mul,*,char)
PETLIB_MAKE_OP_(Div,/,char)
PETLIB_MAKE_OP_(Add,+,unsigned char)
PETLIB_MAKE_OP_(Sub,-,unsigned char)
PETLIB_MAKE_OP_(Mul,*,unsigned char)
PETLIB_MAKE_OP_(Div,/,unsigned char)
PETLIB_MAKE_OP_(Add,+,short)
PETLIB_MAKE_OP_(Sub,-,short)
PETLIB_MAKE_OP_(Mul,*,short)
PETLIB_MAKE_OP_(Div,/,short)
PETLIB_MAKE_OP_(Add,+,unsigned short)
PETLIB_MAKE_OP_(Sub,-,unsigned short)
PETLIB_MAKE_OP_(Mul,*,unsigned short)
PETLIB_MAKE_OP_(Div,/,unsigned short)
PETLIB_MAKE_OP_(Add,+,int)
PETLIB_MAKE_OP_(Sub,-,int)
PETLIB_MAKE_OP_(Mul,*,int)
PETLIB_MAKE_OP_(Div,/,int)
PETLIB_MAKE_OP_(Add,+,unsigned int)
PETLIB_MAKE_OP_(Sub,-,unsigned int)
PETLIB_MAKE_OP_(Mul,*,unsigned int)
PETLIB_MAKE_OP_(Div,/,unsigned int)
PETLIB_MAKE_OP_(Add,+,long)
PETLIB_MAKE_OP_(Sub,-,long)
PETLIB_MAKE_OP_(Mul,*,long)
PETLIB_MAKE_OP_(Div,/,long)
PETLIB_MAKE_OP_(Add,+,unsigned long)
PETLIB_MAKE_OP_(Sub,-,unsigned long)
PETLIB_MAKE_OP_(Mul,*,unsigned long)
PETLIB_MAKE_OP_(Div,/,unsigned long)
PETLIB_MAKE_OP_(Add,+,long long)
PETLIB_MAKE_OP_(Sub,-,long long)
PETLIB_MAKE_OP_(Mul,*,long long)
PETLIB_MAKE_OP_(Div,/,long long)
PETLIB_MAKE_OP_(Add,+,unsigned long long)
PETLIB_MAKE_OP_(Sub,-,unsigned long long)
PETLIB_MAKE_OP_(Mul,*,unsigned long long)
PETLIB_MAKE_OP_(Div,/,unsigned long long)
PETLIB_MAKE_OP_(Add,+,float)
PETLIB_MAKE_OP_(Sub,-,float)
PETLIB_MAKE_OP_(Mul,*,float)
PETLIB_MAKE_OP_(Div,/,float)
PETLIB_MAKE_OP_(Add,+,double)
PETLIB_MAKE_OP_(Sub,-,double)
PETLIB_MAKE_OP_(Mul,*,double)
PETLIB_MAKE_OP_(Div,/,double)
PETLIB_MAKE_OP_(Add,+,long double)
PETLIB_MAKE_OP_(Sub,-,long double)
PETLIB_MAKE_OP_(Mul,*,long double)
PETLIB_MAKE_OP_(Div,/,long double)

#undef PETLIB_MAKE_OP_
  


}  // namespace petlib

#endif