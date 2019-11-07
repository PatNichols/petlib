#ifndef PETLIB_MATRIX_HPP
#define PETLIB_MATRIX_HPP

#include <cstdint>
#include <cstdlib>
#include <petlib_array_ops.hpp>
#include <petlib_io.hpp>
#include <petlib_array.hpp>
#include <petlib_math.hpp>

namespace petlib {

template <class Matrix_t, typename A_t>
struct MatrixBase {
  typedef A_t value_t;
  typedef std::size_t size_type;
  const Matrix_t* leaf() const {
    return static_cast<const Matrix_t*>(this);
  }
  value_t operator()(size_t i,size_t j) const {
    return leaf()->operator()(i,j);
  }
  size_type size() const { return leaf()->size(); }
  size_type nrows() const { return leaf()->nrows();}
  size_type ncols() const { return leaf()->ncols();}
};

template <class Matrix_t, typename A_t>
struct MatrixRef {
  typedef A_t value_t;
  typedef std::size_t size_type;
  const Matrix_t& a;

  MatrixRef(const MatrixBase<Matrix_t,A_t>& a0) : a(*a0.leaf()){}
  value_t operator()(size_t i,size_t j) const { return a(i,j); }
  size_type size() const { return a.size(); }
  size_type nrows() const { return a.nrows();}
  size_type ncols() const { return a.ncols();}
};

template <class A, class Op>
struct MatrixUnaryXpr {
  typedef typename Op::value_t result_t;
  typedef typename Op::value_t value_t;
  typedef std::size_t size_type;
  const A a;

  MatrixUnaryXpr(const A& a0) : a(a0){}
  result_t operator()(size_t i,size_t j) const { return Op::eval(a(i,j)); }
  size_type size() const { return a.size(); }
  size_type nrows() const { return a.nrows();}
  size_type ncols() const { return a.ncols();}
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

  MatrixBinaryXpr(const A& a0, const B& b0) : a(a0), b(b0){}
  result_t operator()(size_t i,size_t j) const { return Op::eval(a(i,j), b(i,j)); }
  size_type size() const { return a.size(); }
  size_type nrows() const { return a.nrows();}
  size_type ncols() const { return a.ncols();}
};

template <class Xpr_t>
struct MatrixXpr {
  typedef typename Xpr_t::value_t result_t;
  typedef typename Xpr_t::value_t value_t;
  typedef std::size_t size_type;
  const Xpr_t a;

  MatrixXpr(const Xpr_t& a0) : a(a0){}
  result_t operator()(size_t i,size_t j) const { return a(i,j); }
  size_type size() const { return a.size(); }
  size_type nrows() const { return a.nrows();}
  size_type ncols() const { return a.ncols();}
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

template < class mat_t, typename T >
std::ostream& operator<<(std::ostream& os, const MatrixBase<mat_t,T>& m)
{
    std::size_t prec = petlib::print_traits<T>::prec;
    std::size_t idig = petlib::print_traits<std::size_t>::ndigits;
    std::size_t ndig = petlib::print_traits<T>::ndigits;
    std::size_t n1=m.nrows();
    std::size_t n2=m.ncols();
    if (n1<5 && n2 <5) {
       if (n1==n2 && n1==0) {
          os << "empty matrix\n";
          return os;
       }
       for (std::size_t i = 0 ; i < n1 ; ++i) {
          os << "| " << std::setw(ndig) << std::setprecision(prec) << m(i,0) << " ";
          for (std::size_t j = 1; j < n2;++j) {
             os << std::setw(ndig) << std::setprecision(prec) << m(i,j) << " "; 
          }
          os << "|\n";
       }
    }else{
       os << " row_index    "  << " col_index   " << " value \n";
       for(std::size_t i=0;i<n1;++i) {
         for (std::size_t j=0;j<n2;++j) {
            os << std::setw(idig) << i << "  " << std::setw(idig) << j << "  ";
            os << std::setw(ndig) << std::setprecision(prec) << m(i,j) << "\n"; 
         }
       }
    }
    return os;
}

template < class xpr_t >
std::ostream& operator<<(std::ostream& os, const MatrixXpr<xpr_t>& m)
{
    typedef typename xpr_t::value_t value_t;
    std::size_t prec = petlib::print_traits<value_t>::prec;
    std::size_t idig = petlib::print_traits<std::size_t>::ndigits;
    std::size_t ndig = petlib::print_traits<value_t>::ndigits;
    std::size_t n1=m.nrows();
    std::size_t n2=m.ncols();
    if (n1<5 && n2 <5) {
       if (n1==n2 && n1==0) {
          os << "empty matrix\n";
          return os;
       }
       for (std::size_t i = 0 ; i < n1 ; ++i) {
          os << "| " << std::setw(ndig) << std::setprecision(prec) << m(i,0) << " ";
          for (std::size_t j = 1; j < n2;++j) {
             os << std::setw(ndig) << std::setprecision(prec) << m(i,j) << " "; 
          }
          os << "|\n";
       }
    }else{
       os << " row_index    "  << " col_index   " << " value \n";
       for(std::size_t i=0;i<n1;++i) {
         for (std::size_t j=0;j<n2;++j) {
            os << std::setw(idig) << i << "  " << std::setw(idig) << j << "  ";
            os << std::setw(ndig) << std::setprecision(prec) << m(i,j) << "\n"; 
         }
       }
    }
    return os;
}

template < typename T > class Matrix;
template < typename T > class SubMatrix;

template < typename T >
class Matrix: public MatrixBase< Matrix<T>, T>  {
   T * data_;
   std::size_t n1;
   std::size_t n2;
   std::size_t ntot;
public:   
   typedef T value_t;
   typedef T* pointer_t;
   typedef const T* const_pointer_t;
   typedef T& reference_t;
   typedef const T& const_reference_t;
   typedef SliceIterator<T> col_iterator;
   typedef RangeIterator<T> row_iterator;
   typedef std::size_t size_type;
   typedef std::ptrdiff_t difference_type;
   
    
   Matrix(std::size_t n1_,std::size_t n2_):data_(new value_t[n1_*n2_]),n1(n1_),n2(n2_),ntot(n1_*n2_) {}
   
   Matrix()=default;

   Matrix(const Matrix& m):data_(new value_t[m.ntot]),n1(m.n1),n2(m.n2),ntot(m.ntot) {
       for (std::size_t i=0;i<n1;++i) {
          std::copy(m.data_+i*n2,m.data_+i*n2+n2,data_+i*n2);
       }
   }

   Matrix(Matrix&& m):data_(m.data_),n1(m.n1),n2(m.n2),ntot(m.ntot) { 
       m.data_ = nullptr;
       m.n1 = m.n2 = m.ntot = 0;
   }

   template < class xpr_t >
   Matrix(const MatrixXpr<xpr_t>& m):data_(new value_t[m.nrows()*m.ncols()]),n1(m.nrows()),n2(m.ncols()),ntot(m.nrows()*m.ncols()) {
       pointer_t dp = data_;
       for (size_type i = 0; i < n1 ; ++i) {
        for (size_type j = 0; j < n2;  ++j,++dp) {
           *dp = m(i,j);
        }
       }
   }

   Matrix(const SubMatrix<value_t>& m):data_(new value_t[m.nrows()*m.ncols()]),n1(m.nrows()),n2(m.ncols()),ntot(m.nrows()*m.ncols()) {
       pointer_t dp = data_;
       for (size_type i = 0; i < n1 ; ++i) {
        for (size_type j = 0; j < n2;  ++j,++dp) {
           *dp = m(i,j);
        }
       }
   }
   
   ~Matrix() {
       if (data_) delete [] data_;
       n1=n2=ntot=0;
   }
   
   Matrix& operator=(const Matrix& m) {
       for (size_type i = 0; i < n1; ++i) {
           std::copy(m.data_+i*m.n2,m.data_+ntot+i*m.n2,data_+i*n2); 
       }
       return *this;
   }

   Matrix& operator=(const SubMatrix<value_t>& m) {
       pointer_t p = data_;
       for (size_type i = 0; i < n1; ++i) {
         for (size_type j = 0; j < n2; ++j, ++p) *p= m(i,j); 
       }
       return *this;
   }

   Matrix& operator=(Matrix&& m) {
       data_ = m.data_;
       n1 = m.n1;
       n2 = m.n2;
       ntot= m.ntot;
       m.data_=nullptr;
       m.n1 =0;
       m.n2 =0;
       m.ntot =0;
       return *this;
   }
   
   Matrix& operator+=(const Matrix& m) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] += m.data_[i*m.n2+j];
       return *this;
   }
   Matrix& operator-=(const Matrix& m) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] -= m.data_[i*m.n2+j];
       return *this;
   }
   Matrix& operator*=(const Matrix& m) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] *= m.data_[i*m.n2+j];
       return *this;
   }
   Matrix& operator/=(const Matrix& m) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] /= m.data_[i*m.n2+j];
       return *this;
   }
   
   template < typename mat_type >   
   Matrix& operator = (const MatrixBase< mat_type, value_t>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] = m(i,j);
       return *this;
   }
   template < typename mat_type >   
   Matrix& operator += (const MatrixBase< mat_type, value_t>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] += m(i,j);
       return *this;
   }
   template < typename mat_type >   
   Matrix& operator -= (const MatrixBase< mat_type, value_t>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] -= m(i,j);
       return *this;
   }
   template < typename mat_type >   
   Matrix& operator *= (const MatrixBase< mat_type, value_t>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] *= m(i,j);
       return *this;
   }
   template < typename mat_type >   
   Matrix& operator /= (const MatrixBase< mat_type, value_t>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] /= m(i,j);
       return *this;
   }

   template < typename xpr_type >   
   Matrix& operator = (const MatrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] = m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   Matrix& operator += (const MatrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] += m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   Matrix& operator -= (const MatrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] -= m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   Matrix& operator *= (const MatrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] *= m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   Matrix& operator /= (const MatrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] /= m(i,j);
       return *this;
   }
      
   Matrix& operator=( value_t x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] = x;
       return *this;   
   }
    
   Matrix& operator+=( value_t x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] += x;
       return *this;   
   }
    
   Matrix& operator-=( value_t x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] -= x;
       return *this;   
   }
   
   Matrix& operator*=( value_t x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] *= x;
       return *this;   
   }
   
   Matrix& operator/=( value_t x) {
       value_t xi  = value_t(1)/x;
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] *= xi;
       return *this;   
   }
    
   SubMatrix<value_t> operator()(const Range& row_rng,const Range& col_rng) {
       return SubMatrix(data_+row_rng.offset()*n2+col_rng.offset(),row_rng.size(),col_rng.size(),n2);
   }
   
   value_t operator()(std::size_t i,std::size_t j) const { return data_[i*n2+j];}
   
   reference_t operator()(std::size_t i,std::size_t j)  { return data_[i*n2+j];}

   pointer_t data() { return data_;}
   const_pointer_t data() const { return data_;} 
   
   SubArray<value_t> row_array(std::size_t i) { return SubArray<value_t>(data_+i*n2,n2);}
   SliceArray<value_t> col_array(std::size_t i) { return SliceArray<value_t>(data_+i,n1,n2);}
   SliceArray<value_t> diag_array() { return SliceArray<value_t>(data_,n1,n2+1);}

   RangeIterator<value_t> row_begin(std::size_t i) { return RangeIterator<value_t>(data_+i*n2,n2);}
   SliceIterator<value_t> col_begin(std::size_t i) { return SliceIterator<value_t>(data_+i,n1,n2);}
   const RangeIterator<value_t> row_end(std::size_t i) const { return SubArray<value_t>(data_+i*n2+n2,n2);}
   const SliceIterator<value_t> col_end(std::size_t i) const { return SliceIterator<value_t>(data_+i+n2*n1,n1,n2);}
   const RangeIterator<value_t> row_cend(std::size_t i) const { return RangeIterator<value_t>(data_+i*n2,n2);}
   const SliceIterator<value_t> col_cend(std::size_t i) const { return SliceIterator<value_t>(data_+i,n1,n2);}
   RangeIterator<value_t> row_cbegin(std::size_t i) { return SubArray<value_t>(data_+i*n2+n2,n2);}
   SliceIterator<value_t> col_cbegin(std::size_t i) { return SliceIterator<value_t>(data_+i+n2*n1,n1,n2);}
   
   std::size_t ncols() const { return n2;}
   std::size_t nrows() const { return n1;}
   std::size_t size() const { return ntot;}
   std::size_t row_stride() const { return std::ptrdiff_t(1);}
   std::size_t col_stride() const { return std::ptrdiff_t(n2);}
   std::size_t diag_stride() const { return std::ptrdiff_t(n2+1);}
   bool empty() const { return data_==0;}
   bool is_square() const { return n1==n2;}
   bool is_symmetric() const {
     if (n1!=n2) return false;
     for (std::size_t i=0;i<n1;++i) {
        SliceIterator<value_t> ci(data_+i,n1,n2);
        RangeIterator<value_t> ri(data_+i*n2,n2);  
        for (std::size_t j=0;j<=i;++j,++ci,++ri) {
           if ( *ci != *ri) return false;
        }
     }
     return true;   
   }
   bool is_skew_symmetric() const {
     if (n1!=n2) return false;
     for (std::size_t i=0;i<n1;++i) {
        SliceIterator<value_t> ci(data_+i,n1,n2);
        RangeIterator<value_t> ri(data_+i*n2,n2);  
        for (std::size_t j=0;j<i;++j,++ci,++ri) {
           if ( *ci != -*ri) return false;
        }
     }
     return true;   
   }



   
   ////
   //   Return a view of the Matrix starting in row r1.offset and ending in r1.size()+r1.offset() and
   //     columns r2.offset() to r2.offset()+r2.size()
   ////
   
   void swap_rows(std::size_t i,std::size_t j) {
       std::swap(data_+i*n2,data_+i*n2+n2,data_+j*n2);
   }
   void elim_row(std::size_t i,std::size_t j,const value_t c) {
       for (size_type k=0;k<n2;++k) data_[i*n2+k] -= data_[j*n2+k]*c;
   }
   
   void rotate_rows(std::size_t i,std::size_t j,value_t angle) {
       pointer_t p = data_ + i * n2;
       pointer_t q = data_ + j * n2;
       value_t cs = std::cos(angle);
       value_t sn = std::sin(angle);
       for (std::size_t i=0;i<n2;++i) {
          value_t tmp = p[i];
          p[i] = p[i]*cs - q[i]*sn;
          q[i] = q[i]*cs + tmp *sn;
       }
   }
   
   difference_type find_row_pivot(std::size_t j) {
       pointer_t p =std::max(data_+j*n2,data_+j*n2+n2,petlib::abs_cmp<value_t>); 
       return (p-data_+j*n2);
   }
   
   void fill_row(std::size_t j,const_pointer_t ptr) {
      std::copy(ptr,ptr+n2,data_+j*n2);
   }
   
   void scal_row(std::size_t i,const value_t factor) {
      pointer_t dp = data_ + i * n2;
      for (std::size_t j=0;j<n2;++j) dp[j] *= factor;
   }
   
   void swap_cols(std::size_t i,std::size_t j) {
       std::swap(SliceIterator<value_t>(data_+i,n2),SliceIterator(data_+i+n2*n1,n2),SliceIterator(data_+j,n2));
   }
   
   void elim_col(std::size_t i,std::size_t j,const value_t c) {
       pointer_t dpi = data_ + i;
       pointer_t dpj = data_ + j;
       for (std::size_t k=0;k<n1;++k) {
          *dpi -= *dpj *c;
          dpi += n2;
          dpj += n2;
       }
   }
   void  rotate_cols(std::size_t i,std::size_t j,value_t angle) {
       pointer_t p = data_ + i;
       pointer_t q = data_ + j;
       value_t cs = std::cos(angle);
       value_t sn = std::sin(angle);
       for (std::size_t i=0;i<n1;++i) {
          value_t tmp = *p;
          *p = *p*cs - *q*sn;
          *q = *q*cs + tmp *sn;
          p += n2;
          q += n2;
       }
   }
   difference_type find_col_pivot(std::size_t j) {
       SliceIterator<value_t> iter(data_+j,n2);
       const SliceIterator<value_t> iend(data_+j+n2,n2);
       SliceIterator<value_t> max_elem =std::max(iter,iend,abs_cmp<value_t>); 
       return (max_elem.data()-data_+j);
   }
   
   void fill_col(std::size_t j,const_pointer_t ptr) {
       SliceIterator<value_t> iter(data_+j,n2);
       std::copy(ptr,ptr+n1,iter);
   }
   
   void scal_col(std::size_t j,const value_t factor) {
       SliceIterator<value_t> iter(data_+j,n2);
       const SliceIterator<value_t> iend(data_+j+n2,n2);
       for ( ;iter!=iend;++iter) {
         *iter *= factor;
       }   
   }
   
   value_t col_norm(std::size_t i) const {
       pointer_t q = data_ + i;
       value_t sum(0);
       for (std::size_t i=0;i<n1;++i,q+=n2) sum += *q * *q;
       return sum;
   }
   value_t row_norm(std::size_t i) const {
       pointer_t p = data_ + i * n2;
       value_t sum(0);
       for (std::size_t i=0;i<n2;++i) sum+=p[i]*p[i];
       return sum;
   }
   value_t row_norm(std::size_t i,Range& r) {
       value_t sum=0;
       pointer_t p = data_ + i * n2 + r.offset();
       for (std::size_t k=0;k<r.size();++k) sum += p[k]*p[k];
       return sum;
   }

   value_t col_norm(std::size_t i,Range& r) {
       std::size_t low = r.offset();
       value_t sum=0;
       SliceIterator p(data_+i+r.offset()*n2,n2);
       for (std::size_t k=0;k<r.size();++k,++p) sum += (*p) * (*p);
       return sum;
   }
       
   void scal_diag(const value_t& c) {
       SliceIterator<value_t> p(data_,n2+1);  
       const SliceIterator<value_t> pend(data_+n1*n2+n1,n2+1);
       for (;p!=pend;++p) { p *= c; }
   }
   
   void assign_diag(const_pointer_t ptr) {
       SliceIterator<value_t> p(data_,n2+1);  
       std::copy(ptr,ptr+n1,p);    
   }
   
   void assign_diag(const value_t& c)
   {
       SliceIterator<value_t> p(data_,n2+1);  
       const SliceIterator<value_t> pend(data_+n1*n2+n1,n2+1);
       for (;p!=pend;++p) { p = c; }   
   }
   
   Matrix<value_t> transpose() const {
       Matrix<value_t> t(n2,n1);
       for (std::size_t i=0;i<n1;++i) {
          SliceIterator<value_t> tp(t.data_ + i,n1);
          for (std::size_t j=0;j<n2;++j,++tp) {
              *tp = data_[i*n2+j];
          }  
       }
       return t;
   }
   
   void resize(std::size_t new_n1,std::size_t new_n2) {
       if (data_) delete [] data_;
       data_=new value_t[new_n1*new_n2];
       n1 = new_n1;
       n2 = new_n2;
   }
   
   value_t norm() const {
       value_t sum(0);
       for (std::size_t i=0;i<n1;++i) sum += row_norm(i);
       return sum;
   }
    
   value_t trace() const {
       value_t sum(0);
       SliceIterator p(data_,n2+1);
       for (std::size_t k=0;k<n1;++k) sum+=p[k];
       return sum;
   }   
   
};

template < typename T >
class SubMatrix: public MatrixBase< SubMatrix<T>, T>  {
   T * data_;
   std::size_t n1;
   std::size_t n2;
   std::size_t nstr;
   std::size_t ntot;
public:   
   typedef T value_t;
   typedef T* pointer_t;
   typedef const T* const_pointer_t;
   typedef T& reference_t;
   typedef const T& const_reference_t;
   typedef SliceIterator<T> col_iterator;
   typedef RangeIterator<T> row_iterator;
   typedef std::size_t size_type;
   typedef std::ptrdiff_t difference_type;
   
  
   SubMatrix(const_pointer_t * p, std::size_t n1_, std::size_t n2_, std::size_t str):data_(p),n1(n1_),n2(n2_),nstr(str),ntot(n1_*n2_) {}  
   
   SubMatrix()=default;

   SubMatrix(const SubMatrix& m):data_(m.data_),n1(m.n1),n2(m.n2),nstr(m.nstr),ntot(m.ntot) {}

   SubMatrix(SubMatrix&& m):data_(m.data_),n1(m.n1),n2(m.n2),nstr(m.nstr),ntot(m.ntot) { 
       m.data_ = nullptr;
       m.n1 = m.n2 = m.ntot = 0;
   }

   SubMatrix(const Matrix<value_t>& m):data_(m.nrows()*m.ncols()),n1(m.nrows()),n2(m.ncols()),nstr(m.ncols()),ntot(m.nrows()*m.ncols()) {
       for (size_type i = 0; i < n1 ; ++i) {
        pointer_t dp = data_ + i * nstr;
        for (size_type j = 0; j < n2;  ++j,++dp) {
           *dp = m(i,j);
        }
       }
   }
   
   ~SubMatrix() {
       data_ = nullptr;
       n1=n2=ntot=0;
   }
   
   SubMatrix& operator=(const SubMatrix& m) {
       for (size_type i = 0; i< n1; ++i) {
          pointer_t p = data_ + i * nstr;
          for (size_type j = 0; j< n2; ++j,++p) *p = m(i,j);
       }
       return *this;
   }
   SubMatrix& operator=(SubMatrix&& m) {
       data_ = m.data_;
       n1 = m.n1;
       n2 = m.n2;
       ntot= m.ntot;
       m.data_=nullptr;
       m.n1 =0;
       m.n2 =0;
       m.ntot =0;
       return *this;
   }
   
   SubMatrix& operator+=(const SubMatrix& m) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] += m.data_[i*m.nstr+j];
       return *this;
   }
   SubMatrix& operator-=(const SubMatrix& m) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] -= m.data_[i*m.nstr+j];
       return *this;
   }
   SubMatrix& operator*=(const SubMatrix& m) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] *= m.data_[i*m.nstr+j];
       return *this;
   }
   SubMatrix& operator/=(const SubMatrix& m) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] /= m.data_[i*m.nstr+j];
       return *this;
   }
   
   template < typename mat_type >   
   SubMatrix& operator = (const MatrixBase< mat_type, value_t>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] = m(i,j);
       return *this;
   }
   template < typename mat_type >   
   SubMatrix& operator += (const MatrixBase< mat_type, value_t>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] += m(i,j);
       return *this;
   }
   template < typename mat_type >   
   SubMatrix& operator -= (const MatrixBase< mat_type, value_t>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] -= m(i,j);
       return *this;
   }
   template < typename mat_type >   
   SubMatrix& operator *= (const MatrixBase< mat_type, value_t>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] *= m(i,j);
       return *this;
   }
   template < typename mat_type >   
   SubMatrix& operator /= (const MatrixBase< mat_type, value_t>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] /= m(i,j);
       return *this;
   }

   template < typename xpr_type >   
   SubMatrix& operator = (const MatrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] = m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   SubMatrix& operator += (const MatrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] += m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   SubMatrix& operator -= (const MatrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] -= m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   SubMatrix& operator *= (const MatrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] *= m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   SubMatrix& operator /= (const MatrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] /= m(i,j);
       return *this;
   }
      
   SubMatrix& operator=( value_t x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] = x;
       return *this;  
   }
    
   SubMatrix& operator+=( value_t x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] += x;
       return *this;   
   }
    
   SubMatrix& operator-=( value_t x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] -= x;
       return *this;   
   }
   
   SubMatrix& operator*=( value_t x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] *= x;
       return *this;   
   }
   
   SubMatrix& operator/=( value_t x) {
       value_t xi  = value_t(1)/x;
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] *= xi;
       return *this;   
   }
    
   SubMatrix<value_t> operator()(Range& row_rng,Range& col_rng) {
       return SubMatrix(data_+row_rng.offset()*n2+col_rng.offset(),row_rng.size(),col_rng.size(),n2);
   }
   
   value_t operator()(std::size_t i,std::size_t j) const { return data_[i*nstr+j];}
   
   reference_t operator()(std::size_t i,std::size_t j)  { return data_[i*nstr+j];}

   pointer_t data() { return data_;}
   const_pointer_t * data() const { return data_;} 
   
   SubArray<value_t> row_array(std::size_t i) { return SubArray<value_t>(data_+i*n2,n2);}
   SliceArray<value_t> col_array(std::size_t i) { return SliceArray<value_t>(data_+i,n1,n2);}
   SliceArray<value_t> diag_array() { return SliceArray<value_t>(data_,n1,n2+1); }

   RangeIterator<value_t> row_begin(std::size_t i) { return RangeIterator<value_t>(data_+i*n2,n2);}
   SliceIterator<value_t> col_begin(std::size_t i) { return SliceIterator<value_t>(data_+i,n1,n2);}
   const RangeIterator<value_t> row_end(std::size_t i) const { return SubArray<value_t>(data_+i*n2+n2,n2);}
   const SliceIterator<value_t> col_end(std::size_t i) const { return SliceIterator<value_t>(data_+i+n2*n1,n1,n2);}
   const RangeIterator<value_t> row_cend(std::size_t i) const { return RangeIterator<value_t>(data_+i*n2,n2);}
   const SliceIterator<value_t> col_cend(std::size_t i) const { return SliceIterator<value_t>(data_+i,n1,n2);}
   RangeIterator<value_t> row_cbegin(std::size_t i) { return SubArray<value_t>(data_+i*n2+n2,n2);}
   SliceIterator<value_t> col_cbegin(std::size_t i) { return SliceIterator<value_t>(data_+i+n2*n1,n1,n2);}
   
   std::size_t ncols() const { return n2;}
   std::size_t nrows() const { return n1;}
   std::size_t size() const { return ntot;};
   std::size_t row_stride() const { return std::ptrdiff_t(1);}
   std::size_t col_stride() const { return std::ptrdiff_t(nstr);}
   std::size_t diag_stride() const { return std::ptrdiff_t(nstr+1);}

   bool empty() const { return data_==0;}
   bool is_square() const { return n1==n2;}
   bool is_symmetric() const {
     if (n1!=n2) return false;
     for (std::size_t i=0;i<n1;++i) {
        SliceIterator<value_t> ci(data_+i,n1,n2);
        RangeIterator<value_t> ri(data_+i*n2,n2);  
        for (std::size_t j=0;j<=i;++j,++ci,++ri) {
           if ( *ci != *ri) return false;
        }
     }
     return true;   
   }
   bool is_skew_symmetric() const {
     if (n1!=n2) return false;
     for (std::size_t i=0;i<n1;++i) {
        SliceIterator<value_t> ci(data_+i,n1,n2);
        RangeIterator<value_t> ri(data_+i*n2,n2);  
        for (std::size_t j=0;j<i;++j,++ci,++ri) {
           if ( *ci != -*ri) return false;
        }
     }
     return true;   
   }



   ////
   //   Return a view of the Matrix starting in row r1.offset and ending in r1.size()+r1.offset() and
   //     columns r2.offset() to r2.offset()+r2.size()
   ////
   
   void swap_rows(std::size_t i,std::size_t j) {
       std::swap(data_+i*n2,data_+i*n2+n2,data_+j*n2);
   }
   void elim_row(std::size_t i,std::size_t j,const value_t c) {
       for (size_type k=0;k<n2;++k) data_[i*n2+k] -= data_[j*n2+k]*c;
   }
   
   void rotate_rows(std::size_t i,std::size_t j,value_t angle) {
       pointer_t p = data_ + i * n2;
       pointer_t q = data_ + j * n2;
       value_t cs = std::cos(angle);
       value_t sn = std::sin(angle);
       for (std::size_t i=0;i<n2;++i) {
          value_t tmp = p[i];
          p[i] = p[i]*cs - q[i]*sn;
          q[i] = q[i]*cs + tmp *sn;
       }
   }
   
   difference_type find_row_pivot(std::size_t j) {
       pointer_t p =std::max(data_+j*n2,data_+j*n2+n2,abs_cmp<value_t>); 
       return (p-data_+j*n2);
   }
   
   void fill_row(std::size_t j,const_pointer_t ptr) {
      std::copy(ptr,ptr+n2,data_+j*n2);
   }
   
   void scal_row(std::size_t i,const T factor) {
      pointer_t dp = data_ + i * n2;
      for (std::size_t j=0;j<n2;++j) dp[j] *= factor;
   }
   
   void swap_cols(std::size_t i,std::size_t j) {
       std::swap(SliceIterator<value_t>(data_+i,n2),SliceIterator(data_+i+n2*n1,n2),SliceIterator(data_+j,n2));
   }
   
   void elim_col(std::size_t i,std::size_t j,const value_t c) {
       pointer_t dpi = data_ + i;
       pointer_t dpj = data_ + j;
       for (std::size_t k=0;k<n1;++k) {
          *dpi -= *dpj *c;
          dpi += n2;
          dpj += n2;
       }
   }
   void  rotate_cols(std::size_t i,std::size_t j,value_t angle) {
       pointer_t p = data_ + i;
       pointer_t q = data_ + j;
       value_t cs = std::cos(angle);
       value_t sn = std::sin(angle);
       for (std::size_t i=0;i<n1;++i) {
          value_t tmp = *p;
          *p = *p*cs - *q*sn;
          *q = *q*cs + tmp *sn;
          p += n2;
          q += n2;
       }
   }
   difference_type find_col_pivot(std::size_t j) {
       SliceIterator<value_t> iter(data_+j,n2);
       const SliceIterator<value_t> iend(data_+j+n2,n2);
       SliceIterator max_elem =std::max(iter,iend,abs_cmp<value_t>); 
       return (max_elem.data()-data_+j);
   }
   
   void fill_col(std::size_t j,const_pointer_t ptr) {
       SliceIterator<value_t> iter(data_+j,n2);
       std::copy(ptr,ptr+n1,iter);
   }
   
   void scal_col(std::size_t j,const value_t factor) {
       SliceIterator<value_t> iter(data_+j,n2);
       const SliceIterator<value_t> iend(data_+j+n2,n2);
       for ( ;iter!=iend;++iter) {
         *iter *= factor;
       }   
   }
   
   value_t col_norm(std::size_t i) const {
       pointer_t q = data_ + i;
       value_t sum(0);
       for (std::size_t i=0;i<n1;++i,q+=n2) sum += *q * *q;
       return sum;
   }
   value_t row_norm(std::size_t i) const {
       pointer_t p = data_ + i * n2;
       value_t sum(0);
       for (std::size_t i=0;i<n2;++i) sum+=p[i]*p[i];
       return sum;
   }
   value_t row_norm(std::size_t i,Range& r) {
       value_t sum=0;
       pointer_t p = data_ + i * n2 + r.offset();
       for (std::size_t k=0;k<r.size();++k) sum += p[k]*p[k];
       return sum;
   }

   value_t col_norm(std::size_t i,Range& r) {
       std::size_t low = r.offset();
       value_t sum=0;
       SliceIterator p(data_+i+r.offset()*n2,n2);
       for (std::size_t k=0;k<r.size();++k,++p) sum += (*p) * (*p);
       return sum;
   }
       
   void scal_diag(const value_t& c) {
       SliceIterator<value_t> p(data_,n2+1);  
       const SliceIterator<value_t> pend(data_+n1*n2+n1,n2+1);
       for (;p!=pend;++p) { p *= c; }
   }
   
   void assign_diag(const_pointer_t ptr) {
       SliceIterator<value_t> p(data_,n2+1);  
       std::copy(ptr,ptr+n1,p);    
   }
   
   void assign_diag(const value_t& c)
   {
       SliceIterator<value_t> p(data_,n2+1);  
       const SliceIterator<value_t> pend(data_+n1*n2+n1,n2+1);
       for (;p!=pend;++p) { p = c; }   
   }
   
   Matrix<value_t> transpose() const {
       Matrix<value_t> t(n2,n1);
       for (std::size_t i=0;i<n1;++i) {
          SliceIterator<value_t> tp(t.data_ + i,n2);
          for (std::size_t j=0;j<n2;++j,++tp) {
              *tp = data_[i*nstr+j];
          }  
       }
       return t;
   }
      
   value_t norm() const {
       value_t sum(0);
       for (size_type i=0;i<n1;++i) {
         pointer_t p = data_+i*nstr;
         for (size_type j = 0; j < n2; ++j,++p) {
           sum += (*p)*(*p);
         }
       }
       return sum;
   }
    
   value_t trace() const {
       value_t sum(0);
       SliceIterator p(data_,nstr+1);
       for (std::size_t k=0;k<n1;++k) sum+=p[k];
       return sum;
   }   
   
};


}
#endif  