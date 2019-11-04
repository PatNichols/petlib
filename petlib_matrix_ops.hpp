#ifndef PETLIB_ARRAY_OPS_H
#define PETLIB_ARRAY_OPS_H

#include <cstdint>
#include <cstdlib>
#include "petlib_array_ops.hpp"

namespace petlib {


template <class Array_t, typename A_t>
struct MatrixBase {
  typedef A_t value_t;
  typedef std::size_t size_type;
  const Matrix_t* leaf() const {
    return static_cast<const Matrix_t*>(this);
  };
  value_t operator()(size_t i,size_t j) const {
    return leaf()->operator()(i,j);
  };
  size_type size() const { return leaf()->size(); };
  size_type nrows() const { return leaf()->nrows();};
  size_type ncols() const { return leaf()->ncols();};
};

template <class Matrix_t, typename A_t>
struct MatrixRef {
  typedef A_t value_t;
  typedef std::size_t size_type;
  const Matrix_t& a;

  MatrixRef(const MatrixBase<Matrix_t,A_t>& a0) : a(*a0.leaf()){};
  value_t operator()(size_t i,size_t j) const { return a(i,j); }
  size_type size() const { return a.size(); }
  size_type nrows() const { return a.nrows();};
  size_type ncols() const { return a.ncols();};
};

template <class A, class Op>
struct MatrixUnaryXpr {
  typedef typename Op::value_t result_t;
  typedef typename Op::value_t value_t;
  typedef std::size_t size_type;
  const A a;

  MatrixUnaryXpr(const A& a0) : a(a0){};
  result_t operator()(size_t i,size_t j) const { return Op::eval(a(i,j)); }
  size_type size() const { return a.size(); }
  size_type nrows() const { return a.nrows();};
  size_type ncols() const { return a.ncols();};
};

template <class A, class B, class Op>
struct MatrixBinaryXpr {
  typedef typename Op::value_t result_t;
  typedef typename A::value_t a_value_t;
  typedef typename B::value_t b_value_t;
  typedef result_t value_t;
  typedef std::size_t size_type;

  const A a;
  const B b;

  MatrixBinaryXpr(const A& a0, const B& b0) : a(a0), b(b0){};
  result_t operator()(size_t i,size_t j) const { return Op::eval(a(i,j), b(i,j)); }
  size_type size() const { return a.size(); }
  size_type nrows() const { return a.nrows();};
  size_type ncols() const { return a.ncols();};
};

template <class Xpr_t>
struct MatrixXpr {
  typedef typename Xpr_t::value_t result_t;
  typedef typename Xpr_t::value_t value_t;
  typedef std::size_t size_type;
  const Xpr_t a;

  MatrixXpr(const Xpr_t& a0) : a(a0){};
  result_t operator()(size_t i,size_t j) const { return a(i,j); }
  size_type size() const { return a.size(); }
  size_type nrows() const { return a.nrows();};
  size_type ncols() const { return a.ncols();};
};


#define PETLIB_MAKE_OP_(name_,sym_) \
template <class MatrixA_t, class MatrixB_t, typename A_t, typename B_t> \
 MatrixXpr<MatrixBinaryXpr<MatrixRef<MatrixA_t, A_t>, MatrixRef<MatrixB_t, B_t>,\
                             name_##Op<A_t, B_t> > > \
operator sym_ (const MatrixBase<MatrixA_t, A_t>& a,\
          const MatrixBase<MatrixB_t, B_t>& b) {\
  typedef MatrixBinaryXpr<MatrixRef<MatrixA_t, A_t>, MatrixRef<MatrixB_t, B_t>,\
                             name_##Op<A_t, B_t> > \
      xpr_t; \
  return MatrixXpr<xpr_t>(xpr_t(MatrixRef<MatrixA_t,A_t>(a),MatrixRef<MatrixB_t,B_t>(b)));\
} \
  \
template <class MatrixA_t, class XprB_t, typename A_t> \
 MatrixXpr<MatrixBinaryXpr<MatrixRef<MatrixA_t, A_t>, MatrixXpr<XprB_t>,\
                             name_##Op<A_t, typename XprB_t::value_t> > > \
operator sym_ (const MatrixBase<MatrixA_t, A_t>& a, const MatrixXpr<XprB_t>& b) { \
  typedef MatrixBinaryXpr<MatrixRef<MatrixA_t, A_t>, MatrixXpr<XprB_t>, \
                             name_##Op<A_t, typename XprB_t::value_t> > \
      xpr_t; \
  return MatrixXpr<xpr_t>(xpr_t(MatrixRef<MatrixA_t,A_t>(a), b)); \
} \
  \
template <class XprA_t, class MatrixB_t, typename B_t> \
 MatrixXpr<MatrixBinaryXpr<MatrixXpr<XprA_t>, MatrixRef<MatrixB_t, B_t>, \
                             name_##Op<typename XprA_t::value_t, B_t> > > \
operator sym_ (const MatrixXpr<XprA_t>& a, const MatrixBase<MatrixB_t, B_t>& b) { \
  typedef MatrixBinaryXpr<MatrixXpr<XprA_t>, MatrixRef<MatrixB_t, B_t>,\
                             name_##Op<typename XprA_t::value_t, B_t> > \
      xpr_t; \
  return MatrixXpr<xpr_t>(xpr_t(a, MatrixRef<MatrixB_t,B_t>(b))); \
} \
  \
template <class XprA_t, class XprB_t> \
 MatrixXpr< \
    MatrixBinaryXpr<MatrixXpr<XprA_t>, MatrixXpr<XprB_t>,\
              name_##Op<typename XprA_t::value_t, typename XprB_t::value_t> > > \
operator sym_ (const MatrixXpr<XprA_t>& a, const MatrixXpr<XprB_t>& b) { \
  typedef MatrixBinaryXpr<\
      MatrixXpr<XprA_t>, MatrixXpr<XprB_t>,\
      name_##Op<typename XprA_t::value_t, typename XprB_t::value_t> > \
      xpr_t; \
  return MatrixXpr<xpr_t>(xpr_t(a, b));\
}

PETLIB_MAKE_OP_(Add,+)
PETLIB_MAKE_OP_(Sub,-)
PETLIB_MAKE_OP_(Mul,*)
PETLIB_MAKE_OP_(Div,/)
#undef PETLIB_MAKE_OP_

#define PETLIB_MAKE_OP_(name_, sym_)                                         \
  template <class Matrix_t, class A_t>                                        \
   MatrixXpr<MatrixUnaryXpr<MatrixRef<Matrix_t, A_t>, name_##Op<A_t> > >      \
  operator sym_(const MatrixBase<Matrix_t, A_t>& a) {                          \
    typedef MatrixUnaryXpr<MatrixRef<Matrix_t, A_t>, name_##Op<A_t> > xpr_t; \
    return MatrixXpr<xpr_t>(xpr_t(MatrixRef<Matrix_t,A_t>(a)));                               \
  }                                                                          \
  \
\
template<class XprA_t>  MatrixXpr<                                   \
      MatrixUnaryXpr<MatrixXpr<XprA_t>, name_##Op<typename XprA_t::value_t> > >      \
  operator sym_(const MatrixXpr<XprA_t>& a) {                                 \
    typedef MatrixUnaryXpr<MatrixXpr<XprA_t>,                               \
                             name_##Op<typename XprA_t::value_t> >           \
        xpr_t;                                                               \
    return MatrixXpr<xpr_t>(xpr_t(a));                                        \
  }

PETLIB_MAKE_OP_(Neg,-)
PETLIB_MAKE_OP_(Not,~)
PETLIB_MAKE_OP_(Plus,+)
#undef PETLIB_MAKE_OP_

#define PETLIB_MAKE_OP_(name_,sym_,type_)\
template < class Matrix_t, typename A_t > \
 MatrixXpr< MatrixBinaryXpr<MatrixRef<Matrix_t,A_t>,ScalarRef< type_ >, name_##Op<A_t, type_ > > > \
operator sym_ (const MatrixBase<Matrix_t,A_t>& a,const  type_ & s) {\
 typedef MatrixBinaryXpr<MatrixRef<Matrix_t,A_t>,ScalarRef< type_ >, name_##Op<A_t, type_ > >  xpr_t;\
 return MatrixXpr<xpr_t>(xpr_t(MatrixRef<Matrix_t,A_t>(a),ScalarRef< type_ >(s)));\
} \
 \
template < class Matrix_t, typename A_t> \
 MatrixXpr< MatrixBinaryXpr<MatrixRef<Matrix_t,A_t>,ScalarRef< type_ >, Rev##name_##Op<A_t, type_ > > > \
operator sym_ (const  type_ & s,const MatrixBase<Matrix_t,A_t>& a) {\
 typedef MatrixBinaryXpr<MatrixRef<Matrix_t,A_t>,ScalarRef< type_ >, Rev##name_##Op<A_t, type_ > >  xpr_t;\
 return MatrixXpr<xpr_t>(xpr_t(MatrixRef<Matrix_t,A_t>(a),ScalarRef< type_ >(s)));\
} \
 \
template < class Xpr_t> \
 MatrixXpr< MatrixBinaryXpr< MatrixXpr<Xpr_t>,ScalarRef< type_ >, name_##Op<typename Xpr_t::value_t, type_ > > > \
operator sym_ (const MatrixXpr<Xpr_t>& a,const  type_ & s) {\
 typedef MatrixBinaryXpr<MatrixXpr<Xpr_t>,ScalarRef< type_ >, name_##Op<typename Xpr_t::value_t, type_ > >  xpr_t;\
 return MatrixXpr<xpr_t>(xpr_t(a,ScalarRef< type_ >(s)));\
} \
 \
template < class Xpr_t > \
 MatrixXpr< MatrixBinaryXpr< MatrixXpr<Xpr_t>,ScalarRef< type_ >, Rev##name_##Op<typename Xpr_t::value_t, type_ > > > \
operator sym_ (const  type_ & s,const MatrixXpr<Xpr_t>& a) {\
 typedef MatrixBinaryXpr<MatrixXpr<Xpr_t>,ScalarRef< type_ >, Rev##name_##Op<typename Xpr_t::value_t, type_ > >  xpr_t;\
 return MatrixXpr<xpr_t>(xpr_t(a,ScalarRef< type_ >(s)));\
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