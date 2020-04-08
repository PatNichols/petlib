#ifndef PETLIB_MATRIX_HPP
#define PETLIB_MATRIX_HPP

namespace petlib {

template < typename Tp, class Matrix_t > 
struct MatrixRef {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp value_type;
    typedef Tp& reference;
    typedef const Tp& const_reference;
    const Matrix_t& a;

    MatrixRef(const Matrix_t& a0) noexcept:a(a0) {}
    size_type size() const noexcept { return a.size();}
    size_type size1() const noexcept { return a.size1();}
    size_type size2() const noexcept { return a.size2();}
    const_reference operator()(size_type i, size_type j) const noexcept {
        return a(i,j);
    }
};

template < typename Tp, class Matrix_t > 
struct MatrixBase {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp value_type;
    typedef Tp& reference;
    typedef const Tp& const_reference;

    const Matrix_t* CastDown() const noexcept {
        return static_cast< const Matrix_t* >(this);
    }
    const Tp * row(size_type i) const noexcept { return CastDown()->row(i);}
    size_type size() const noexcept { return CastDown()->size();}
    size_type size1() const noexcept { return a.size1();}
    size_type size2() const noexcept { return a.size2();}
    const_reference operator()(size_type i, size_type j) const noexcept {
        return CastDown()->operator[](i);
    }
};

template < typename Tp, class Expr_t > 
struct MatrixExpr {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp return_type;
    Expr_t a;

    MatrixExpr(const Expr_t& a0) noexcept:a(a0)  {}
    size_type size() const noexcept { return a.size();}
    size_type size1() const noexcept { return a.size1();}
    size_type size2() const noexcept { return a.size2();}
    return_type operator()(size_type i, size_type j) const noexcept {
        return a(i,j);
    }
};

template < class Op_t, class Arg1, class Arg2 > 
struct MatrixBinExpr {
    typedef std::size_t size_type;
    typedef typename Op_t::return_type return_type;
    Arg1 a;
    Arg2 b;
    
    MatrixBinExpr(const Arg1& a0,const Arg2& b0) noexcept:a(a0),b(b0) {}
    size_type size() const noexcept { return a.size();}
    size_type size1() const noexcept { return a.size1();}
    size_type size2() const noexcept { return a.size2();}
    return_type operator()(size_type i, size_type j) const noexcept {
        return Op_t::eval(a(i,j),b(i,j));
    }
};

template < class Op_t, class Arg  > 
struct MatrixUnExpr {
    typedef std::size_t size_type;
    typedef typename Op_t::return_type return_type;
    Arg a;
    
    MatrixUnExpr(const Arg& a0) noexcept:a(a0) {}
    size_type size() const noexcept { return a.size();}
    size_type size1() const noexcept { return a.size1();}
    size_type size2() const noexcept { return a.size2();}
    return_type operator()(size_type i, size_type j) const noexcept {
        return Op_t::eval(a(i,j));
    }
};

#define PETLIB_EXPR_OP(OP,SYM)\
template < typename T, class A, class B > \
inline  MatrixExpr< T, \
  MatrixBinExpr< OP<T>, MatrixRef< T, A >, MatrixRef< T, B > > > \
  operator SYM (const MatrixBase< T, A >& a, const MatrixBase< T, B >& b) noexcept {\
   typedef MatrixBinExpr< OP<T>, MatrixRef< T, A >, MatrixRef< T, B > >  xpr_t;\
   return MatrixExpr< T, xpr_t >(xpr_t( MatrixRef< T, A >(*a.CastDown()), MatrixRef< T, B >(*b.CastDown()) ) );\
} \
  \
template < typename T, class A, class B > \
inline  MatrixExpr< T, \
  MatrixBinExpr< OP<T>, MatrixRef< T, A >, MatrixExpr< T, B > > > \
  operator SYM (const MatrixBase< T, A >& a, const MatrixExpr< T, B >& b) noexcept {\
   typedef MatrixBinExpr< OP<T>, MatrixRef< T, A >, MatrixExpr< T, B > >  xpr_t;\
   return MatrixExpr< T, xpr_t >(xpr_t( MatrixRef< T, A >(*a.CastDown()), b ) );\
} \
  \
template < typename T, class A, class B > \
inline  MatrixExpr< T, \
  MatrixBinExpr< OP<T>, MatrixExpr< T, A >, MatrixRef< T, B > > > \
  operator SYM (const MatrixExpr< T, A >& a, const MatrixBase< T, B >& b) noexcept {\
   typedef MatrixBinExpr< OP<T>, MatrixExpr< T, A >, MatrixRef< T, B > >  xpr_t;\
   return MatrixExpr< T, xpr_t >(xpr_t( a, MatrixRef< T, B >(*b.CastDown()) ) );\
} \
  \
template < typename T, class A, class B > \
inline  MatrixExpr< T, \
  MatrixBinExpr< OP<T>, MatrixExpr< T, A >, MatrixExpr< T, B > > > \
  operator SYM (const MatrixExpr< T, A >& a, const MatrixExpr< T, B >& b) noexcept {\
   typedef MatrixBinExpr< OP<T>, MatrixExpr< T, A >, MatrixExpr< T, B > >  xpr_t;\
   return MatrixExpr< T, xpr_t >(xpr_t( a, b ) );\
} \
  \
template < typename T, class A > \
inline  MatrixExpr< T, \
  MatrixBinExpr< OP<T>, MatrixRef< T, A >, ScalarRef< T > > > \
  operator SYM (const MatrixBase< T, A >& a, const T& b) noexcept {\
   typedef MatrixBinExpr< OP<T>, MatrixRef< T, A >, ScalarRef< T > >  xpr_t;\
   return MatrixExpr< T, xpr_t >(xpr_t( MatrixRef< T, A >(*a.CastDown()), ScalarRef< T > (b) ) );\
} \
  \
template < typename T, class A > \
inline  MatrixExpr< T, \
  MatrixBinExpr< Rev ## OP<T>, MatrixRef< T, A >, ScalarRef< T > > > \
  operator SYM (const T& b,const MatrixBase< T, A >& a) noexcept {\
   typedef MatrixBinExpr< Rev ## OP<T>, MatrixRef< T, A >, ScalarRef< T > >  xpr_t;\
   return MatrixExpr< T, xpr_t >(xpr_t( MatrixRef< T, A >(*a.CastDown()), ScalarRef< T >(b) ) );\
} \
  \
template < typename T, class A > \
inline  MatrixExpr< T, \
  MatrixBinExpr< OP<T>, MatrixExpr< T, A >, ScalarRef< T > > > \
  operator SYM (const MatrixExpr< T, A >& a, const T& b) noexcept {\
   typedef MatrixBinExpr< OP<T>, MatrixExpr< T, A >, ScalarRef< T > >  xpr_t;\
   return MatrixExpr< T, xpr_t >(xpr_t( a, ScalarRef< T > (b) ) );\
} \
  \
template < typename T, class A > \
inline  MatrixExpr< T, \
  MatrixBinExpr< Rev ## OP<T>, MatrixExpr< T, A >, ScalarRef< T > > > \
  operator SYM (const T& b,const MatrixExpr< T, A >& a) noexcept {\
   typedef MatrixBinExpr< Rev ## OP<T>, MatrixExpr< T, A >, ScalarRef< T > >  xpr_t;\
   return MatrixExpr< T, xpr_t >(xpr_t( a, ScalarRef< T >(b) ) );\
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
inline  MatrixExpr< bool,\
 MatrixBinExpr< OP<T> , MatrixRef< T, A >, MatrixRef< T, B > > > \
operator SYM (const MatrixBase< T, A >& a,const MatrixBase<T,B>& b) noexcept {\
 typedef  MatrixBinExpr< OP<T> , MatrixRef< T, A >, MatrixRef< T, B > >   xpr_t;\
 return MatrixExpr< bool, xpr_t >(xpr_t(MatrixRef< T, A >(*a.CastDown()),MatrixRef< T, B >(*b.CastDown())));\
} \
  \
template < class A, class B, class T > \
inline  MatrixExpr< bool,\
 MatrixBinExpr< OP<T> , MatrixRef< T, A >, MatrixExpr< T, B > > > \
operator SYM (const MatrixBase< T, A >& a,const MatrixExpr<T,B>& b) noexcept {\
 typedef  MatrixBinExpr< OP<T> , MatrixRef< T, A >, MatrixExpr< T, B > >   xpr_t;\
 return MatrixExpr< bool, xpr_t >(xpr_t(MatrixRef< T, A >(*a.CastDown()),b));\
} \
  \
template < class A, class B, class T > \
inline  MatrixExpr< bool,\
 MatrixBinExpr< OP<T> , MatrixExpr< T, A >, MatrixRef< T, B > > > \
operator SYM (const MatrixExpr< T, A >& a,const MatrixBase<T,B>& b) noexcept {\
 typedef  MatrixBinExpr< OP<T> , MatrixExpr< T, A >, MatrixRef< T, B > >   xpr_t;\
 return MatrixExpr< bool, xpr_t >(xpr_t(a,MatrixRef< T, B >(*b.CastDown())));\
} \
  \
template < class A, class B, class T > \
inline  MatrixExpr< bool,\
 MatrixBinExpr< OP<T> , MatrixExpr< T, A >, MatrixExpr< T, B > > > \
operator SYM (const MatrixExpr< T, A >& a,const MatrixExpr<T,B>& b) noexcept {\
 typedef  MatrixBinExpr< OP<T> , MatrixExpr< T, A >, MatrixExpr< T, B > >   xpr_t;\
 return MatrixExpr< bool, xpr_t >(xpr_t(a,b));\
} \
  \
template < class A, class T  > \
inline  MatrixExpr< bool,\
 MatrixBinExpr< OP<T> , MatrixRef< T, A >, ScalarRef< T > > > \
operator SYM (const MatrixBase< T, A >& a,const T& b) noexcept {\
 typedef  MatrixBinExpr< OP<T> , MatrixRef< T, A >, ScalarRef< T > >   xpr_t;\
 return MatrixExpr< bool, xpr_t >(xpr_t(MatrixRef< T, A >(*a.CastDown()),ScalarRef<T>(b)));\
} \
  \
template < class A, class T > \
inline  MatrixExpr< typename Rev ## OP<T>::return_type,\
 MatrixBinExpr< Rev ## OP<T> , MatrixRef< T, A >, ScalarRef< T > > > \
operator SYM (const T& b, const MatrixBase< T, A >& a) noexcept {\
 typedef  MatrixBinExpr< Rev ## OP<T> , MatrixRef< T, A >, ScalarRef< T > >   xpr_t;\
 return MatrixExpr< bool, xpr_t >(xpr_t(MatrixRef< T, A >(*a.CastDown()),ScalarRef<T>(b)));\
} \
  \
template < class A, class T > \
inline  MatrixExpr< bool,\
 MatrixBinExpr< OP<T> , MatrixExpr< T, A >, ScalarRef< T > > > \
operator SYM (const MatrixExpr< T, A >& a,const T& b) noexcept {\
 typedef  MatrixBinExpr< OP<T> , MatrixExpr< T, A >, ScalarRef< T > >   xpr_t;\
 return MatrixExpr< bool, xpr_t >(xpr_t(a,ScalarRef<T>(b)));\
} \
  \
template < class A, class T > \
inline  MatrixExpr< bool,\
 MatrixBinExpr< Rev ## OP<T> , MatrixExpr< T, A >, ScalarRef< T > > > \
operator SYM (const T& b, const MatrixExpr< T, A >& a) noexcept {\
 typedef  MatrixBinExpr< Rev ## OP<T> , MatrixExpr< T, A >, ScalarRef< T > >   xpr_t;\
 return MatrixExpr< bool, xpr_t >(xpr_t(a,ScalarRef<T>(b)));\
} 

PETLIB_EXPR_OP(Eq,==)
PETLIB_EXPR_OP(NotEq,!=)
#undef PETLIB_EXPR_OP



#define PETLIB_EXPR_OP(OP,SYM)\
template < class A, class T > \
inline  MatrixExpr< T,\
 MatrixUnExpr< OP<T> , MatrixRef< T, A > > > \
operator SYM (const MatrixBase< T, A >& a) noexcept {\
 typedef  MatrixUnExpr< OP<T> , MatrixRef< T, A > > xpr_t;\
 return MatrixExpr< T , xpr_t >(xpr_t(MatrixRef< T, A >(*a.CastDown())));\
} \
  \
template < class A, class T > \
inline  MatrixExpr< T,\
 MatrixUnExpr< OP<T>, MatrixExpr< T, A > > > \
operator SYM (const MatrixExpr< T, A >& a) noexcept {\
 typedef  MatrixUnExpr< OP<T> , MatrixExpr< T, A > > xpr_t;\
 return MatrixExpr< T , xpr_t >(xpr_t(a));\
}

PETLIB_EXPR_OP(Not,!)
PETLIB_EXPR_OP(Com,~)
PETLIB_EXPR_OP(Neg,-)
PETLIB_EXPR_OP(Pos,+)
#undef PETLIB_EXPR_OP

template < class T,  class A, class X >  
inline void MatrixExprCopy( A av, const MatrixExpr< T, X >& x, std::size_t n1, std::size_t n2) noexcept { 
    for (std::size_t i=0;i<n1;++i) {
        for (std:::size_t j=0;j<n2;++j,++a) *a = x(i,j); 
    }
}

#define PETLIB_EXPR_OP(OP,SYM)\
template < class T,  class A, class X > \
inline void MatrixExpr ## OP ## Copy( A av, const MatrixExpr< T, X >& x, std::size_t n1, std::size_t n2) noexcept {\
    for (std::size_t i=0;i<n1;++i) { \
        for (std:::size_t j=0;j<n2;++j,++a) *a = x(i,j);\
    } \
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

template < class T, class A > inline std::ostream& operator<<(std::ostream& os,const MatrixBase<T,A>& a) {
  std::size_t asize1 = a.size1();
  std::size_t asize2 = a.size2();
  os << "---------------------------------------------\n";
  os << " index   |    index    |    value \n";   
  for (std::size_t i=0;i<asize1;++i) {
  for (std::size_t j=0;j<asize2;++j) {
      os << std::setw(12) << i << " - " << j << " - " << a(i,j) << "\n";
  }
  }
  os << "---------------------------------------------\n";
  return os;
}

template < class T, class A > inline std::ostream& operator<<(std::ostream& os,const MatrixExpr<T,A>& a) {
  std::size_t asize1 = a.size1();
  std::size_t asize2 = a.size2();
  os << "---------------------------------------------\n";
  os << " index   |    index    |    value \n";   
  for (std::size_t i=0;i<asize1;++i) {
  for (std::size_t j=0;j<asize2;++j) {
      os << std::setw(12) << i << " - " << j << " - " << a(i,j) << "\n";
  }
  }
  os << "---------------------------------------------\n";
  return os;

}

template < typename Tp > class Matrix;
template < typename Tp > class SubMatrix;

enum class MatrixOrder : std::uint8_t { RowMajor=0, ColMajor=1, FortranOrder=2, IDEOrder=3 };
 
template < enum MatrixOrder > struct Indexer {
    typedef std::size_t size_type;
    
    constexpr explicit Indexer(size_type dim1, size_type dim2)  noexcept:n1(dim1),n2(dim2) {}
    
    /* convert the i and j into the zero offset flat_index (index into array representing matrix) */
    constexpr size_type IndicesToFlatIndex( size_type i, size_type j) const noexcept;
    /* convert the zero offset flat_index (index into array representing matrix) into i and j */
    constexpr void FlatIndexToIndices( size_type flat_index, size_type& i, size_type& j) const noexcept;
    constexpr size_type Extent1() const noexcept { return n1;}
    constexpr size_type Extent2() const noexcept { return n2;}
    constexpr size_type TotalSize() const noexcept { return n1*n2;}
    size_type n1,n2;    
};
    
template <> constexpr size_type Indexer< MatrixOrder::RowMajor >::IndicesToFlatIndex(std::size_t i,std::size_t j) const noexcept {
    return (i*n2+j);
}

template <> constexpr size_type Indexer< MatrixOrder::ColMajor >::IndicesToFlatIndex(std::size_t i,std::size_t j) const noexcept {
    return (i+j*n1);
}

template <> constexpr size_type Indexer< MatrixOrder::FortranOrder >::IndicesToFlatIndex(std::size_t i,std::size_t j) const noexcept {
    --i;
    --j;
    return (i+j*n1);
}

template <> constexpr size_type Indexer< MatrixOrder::IDEOrder >::IndicesToFlatIndex(std::size_t i,std::size_t j) const noexcept {
    --i;
    --j;
    return (i*n2+j);
}

template <> constexpr void Indexer< MatrixOrder::RowMajor >::FlatIndexToIndices(std::size_t flat_index,std::size_t& i,std::size_t& j) const noexcept {
    i = flat_index / n2;
    j = flat_index - i*n2;
    return;
}

template <> constexpr size_type Indexer< MatrixOrder::ColMajor >::IndicesToFlatIndex(std::size_t i,std::size_t j) const noexcept {
    j = flat_index / n1;
    i = flat_index - j*n1;
    return;
}

template <> constexpr size_type Indexer< MatrixOrder::FortranOrder >::IndicesToFlatIndex(std::size_t i,std::size_t j) const noexcept {
    j = flat_index / n1;
    i = flat_index - j*n1;
    ++j;
    ++i;
    return;
}

template <> constexpr size_type Indexer< MatrixOrder::IDEOrder >::IndicesToFlatIndex(std::size_t i,std::size_t j) const noexcept {
    i = flat_index / n2;
    j = flat_index - i * n2;
    ++i;
    ++j;
    return;
}

template < typename Tp  >
struct Matrix: public MatrixBase< Tp , Matrix<Tp> > {
public:
    typedef Tp value_type;
    typedef Tp& reference;
    typedef const Tp& const_reference;
    typedef Tp* pointer;
    typedef const Tp* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp* iterator;
    typedef const Tp* const_iterator;
    typedef std::reverse_iterator< iterator > reverse_iterator; 
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;
    typedef petlib::AlignedAllocator< value_type > allocator_type;
    typedef MatrixOrder matrix_order_type;
    pointer m_ptr;
    size_type m_size,m_size1,m_size2;
    allocator_type m_alloc;

    Matrix():m_ptr(nullptr),m_size(0),m_alloc() {}

    Matrix(size_type n1,size_type n2):m_ptr(nullptr),m_size(n1*n2),m_size1(n1),m_size2(n2),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
    }

    Matrix(const_pointer p,size_type n1,size_t n2):m_ptr(nullptr),m_size(n1*n2),m_size1(n1),m_size2(n2),nm_alloc() {
        m_ptr = m_alloc.allocate(m_size);
        petlib::Copy< iterator, const_pointer >(m_ptr,p,m_size);
    }
    
    Matrix(const Matrix& a):m_ptr(nullptr),m_size(a.size()),m_size1(a.size1()),m_size2(a.size2()),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
        for (size_type i=0;i<n1;++i) {
            petlib::Copy< iterator, const_pointer >(m_ptr+i*m_size2,a.row(i).begin(),m_size2);
        }
    }

    Matrix(const SubMatrix< value_type >& a):m_ptr(nullptr),m_size(a.size()),m_size1(a.size1()),m_size2(a.size2()),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
        for (size_type i=0;i<n1;++i) {
            petlib::Copy< iterator, const_pointer >(m_ptr+i*m_size2,a.row(i).begin(),m_size2);
        }
    }

    template < class X >
    Matrix(const MatrixExpr<Tp,X>& a):m_ptr(nullptr),m_size(a.size()),m_size1(a.size1()),m_size2(a.size2()),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
        MatrixExprCopy< Tp, iterator, X  >(m_ptr,a,m_size1,m_size2);
    }

    ~Matrix() {
        m_alloc.deallocate(m_ptr,m_size);
    }
    
    /// Range r1 is the range of rows and Range r2 is the range for columns
    SubMatrix< value_type > operator()(const Range& r1,const Range& r2) noexcept {
        return SubMatrix<value_type>( m_ptr + r1.offset()*m_size2 + r2.offset(), r1.size(), r2.size(), msize_2);
    }

    Matrix operator()(const Range& r1, const Range& r2) const noexcept { 
        Matrix m( r1.size(), r2.size() );
        for (size_type i=0;i<r1.size();++i) {
           petlib::Copy(m_ptr+(r1.offset()+i)*m_size2+r2.offset(),m.row(i).begin(),r2.size());
        }  
        return m;
    } 


    constexpr bool empty() const noexcept { return m_size==0;}
    reference operator()(size_type i,size_type j) noexcept { return m_ptr[i*m_size2+j];}
    const_reference operator()(size_type i,size_type j) const noexcept { return m_ptr[i*m_size2+j];}
    constexpr pointer data() noexcept { return m_ptr;}
    constexpr const_pointer data() const noexcept { return m_ptr;}
    constexpr size_type size() const noexcept { return m_size;}
    constexpr size_type size1() const noexcept { return m_size1;}
    constexpr size_type size2() const noexcept { return m_size2;}
    constexpr iterator begin() noexcept { return m_ptr;}
    constexpr iterator end() noexcept { return (m_ptr+m_size);}
    constexpr const_iterator cbegin() const noexcept { return m_ptr;}
    constexpr const_iterator cend() const noexcept { return (m_ptr+m_size);}
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end());}
    constexpr reverse_iterator rend() noexcept { return  reverse_iterator(begin());}
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end());}
    constexpr const_reverse_iterator crend() const noexcept { return  const_reverse_iterator(begin());}
    
    Matrix& operator=(const Matrix& a) noexcept {
        petlib::Copy< iterator, const_iterator >(begin(),a.cbegin());
        return *this;
    }
    Matrix& operator=(const SubMatrix< value_type >& a) noexcept {
        petlib::MatrixCopy< iterator, const_iterator >(begin(),end(),a.cbegin());
        return *this;
    }

    template < class X >
    Matrix& operator=(const MatrixExpr< value_type, X >& x) noexcept {
        petlib::MatExprCopy< value_type, iterator, X >(begin(),x,m_size);
        return *this;
    }

    Matrix& operator=(const_reference v) noexcept {
        petlib::Fill< iterator, const_reference >(begin(),v,m_size);
        return *this;
    }

#define PETLIB_MAKE_OP(NAME,SYM)\
    Matrix& operator SYM (const Matrix& a) noexcept {\
        Matrix ## NAME ## Copy< iterator, const_iterator >(begin(),a.cbegin(),m_size);\
        return *this;\
    }\
     \
    Matrix& operator SYM (const SubMatrix< value_type >& a) noexcept {\
        Matrix ## NAME ## Copy< iterator, const_iterator >(begin(),a.cbegin(),m_size1,m_size2); \
        return *this; \
    } \
     \
    template < class X > \
    Matrix& operator SYM (const MatrixExpr< value_type, X >& x) noexcept {\
        MatrixExpr ## NAME ## Copy< value_type, iterator, X >(begin(),x,m_size1,m_size2);\
        return *this;\
    }\
     \
    Matrix& operator SYM (const_reference v) noexcept {\
        NAME ## Fill< iterator, const_reference >(begin(),v,m_size);\
    }

PETLIB_MAKE_OP(Add,+=)
PETLIB_MAKE_OP(Sub,-=)
PETLIB_MAKE_OP(Mul,*=)
PETLIB_MAKE_OP(Div,/=)
PETLIB_MAKE_OP(Mod,%=)
PETLIB_MAKE_OP(Xor,^=)
PETLIB_MAKE_OP(Ior,|=)
PETLIB_MAKE_OP(And,&=)
PETLIB_MAKE_OP(Lsh,<<=)
PETLIB_MAKE_OP(Rsh,>>=)
#undef PETLIB_MAKE_OP

    value_type Norm() const noexcept { return petlib::Norm2<value_type,const_pointer>(m_ptr,m_size);}
    value_type Asum() const noexcept { return petlib::Asum<value_type,const_pointer>(m_ptr,m_size);}
    size_type IndexAmax() const noexcept { return petlib::AmaxIndex<value_type,const_pointer>(m_ptr,m_size);}
    size_type IndexAmin() const noexcept { return petlib::AminIndex<value_type,const_pointer>(m_ptr,m_size);}
    size_type IndexMax() const noexcept { return petlib::MaxIndex<value_type,const_pointer>(m_ptr,m_size);}
    size_type IndexMin() const noexcept { return petlib::MinIndex<value_type,const_pointer>(m_ptr,m_size);}
    value_type Max() const noexcept { return petlib::MaxValue<value_type,const_pointer>(m_ptr,m_size);}
    value_type Min() const noexcept { return petlib::MinValue<value_type,const_pointer>(m_ptr,m_size);}
    value_type Amax() const noexcept { return petlib::AmaxValue<value_type,const_pointer>(m_ptr,m_size);}
    value_type Amin() const noexcept { return petlib::AminValue<value_type,const_pointer>(m_ptr,m_size);}
  
    constexpr void Swap(Matrix& a) noexcept {
        return petlib::Swap< iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(SubMatrix<value_type>& a) noexcept {
        return petlib::Swap< iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr SubArray<value_type> row(size_type i) noexcept {
        return SubArray<value_type>::SubArray(m_ptr + i * m_size2,m_size2);
    }
    constexpr SliceArray<value_type> col(size_type i) noexcept {
        return SliceArray<value_type>::SliceArray(m_ptr + i,m_size1,m_size2);
    }
    constexpr SliceArray<value_type> diag() noexcept {
        return SliceArray<value_type>::SliceArraY(m_ptr,m_size1,m_size2+1);
    }

private:
    pointer m_ptr;
    size_type m_size;
    allocator_type m_alloc;
};

template < typename Tp >
class SubMatrix:  public MatrixBase< Tp , SubMatrix<Tp> > {
public:
    typedef Tp value_type;
    typedef Tp& reference;
    typedef const Tp& const_reference;
    typedef Tp* pointer;
    typedef const Tp* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp* iterator;
    typedef const Tp* const_iterator;
    typedef std::reverse_iterator< iterator > reverse_iterator; 
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;

    
    SubMatrix(const_pointer p,size_type n):m_ptr(p),m_size(n) {}
    
    explicit SubMatrix(Matrix<value_type>& a):m_ptr(a.data()),m_size(a.size()) {}

    ~SubMatrix() {
        m_ptr = nullptr;
    }
    
    SubMatrix operator()(Range& r) noexcept {
        return SubMatrix(m_ptr + r.offset(),r.size());
    }
    SliceMatrix< value_type > operator()(Slice& s) noexcept {
        return SubMatrix(m_ptr + s.offset(),s.size(),s.stride());
    } 
    Matrix< value_type > operator()(const Range& r) const noexcept { return Matrix< value_type >(m_ptr+r.offset(),r.size());}    
    Matrix< value_type > operator()(const Slice& s) const noexcept { 
      return Matrix< value_type >::Matrix< SliceIterator<value_type>(SliceIterator<value_type>(m_ptr+s.offset(),s.stride()),
        SliceIterator<value_type>(m_ptr+s.offset+s.stride()*s.size(),s.stride()));
    }
    constexpr bool empty() const noexcept { return m_size==0;}
    reference operator[](size_type i) noexcept { return m_ptr[i];}
    const_reference operator[](size_type i) const noexcept { return m_ptr[i];}
    constexpr pointer data() noexcept { return m_ptr;}
    constexpr const_pointer data() const noexcept { return m_ptr;}
    constexpr size_type size() const noexcept { return m_size;}
    constexpr size_type stride() const noexcept { return difference_type(1);}
    constexpr iterator begin() noexcept { return m_ptr;}
    constexpr iterator end() noexcept { return (m_ptr+m_size);}
    constexpr const_iterator cbegin() const noexcept { return m_ptr;}
    constexpr const_iterator cend() const noexcept { return (m_ptr+m_size);}
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end());}
    constexpr reverse_iterator rend() noexcept { return  reverse_iterator(begin());}
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end());}
    constexpr const_reverse_iterator crend() const noexcept { return  const_reverse_iterator(begin());}
    
    SubMatrix& operator=(const Matrix< value_type >& a) noexcept {
        Copy< iterator, const_iterator >(begin(),end(),a.cbegin());
        return *this;
    }
    SubMatrix& operator=(const SubMatrix& a) noexcept {
        Copy< iterator, const_iterator >(begin(),end(),a.cbegin());
        return *this;
    }
    SubMatrix& operator=(const SliceMatrix< value_type >& a) noexcept {
        Copy< iterator, const SliceIterator< value_type> >(begin(),end(),a.cbegin());
        return *this;
    }
    template < class X >
    SubMatrix& operator=(const MatrixExpr< value_type, X >& x) noexcept {
        ExprCopy< value_type, iterator, X >(begin(),end(),x);
        return *this;
    }

    SubMatrix& operator=(const_reference v) noexcept {
        Fill< iterator, value_type >(begin(),v,m_size);
        return *this;
    }

#define PETLIB_MAKE_OP(NAME,SYM)\
    SubMatrix& operator SYM (const SubMatrix& a) noexcept {\
        NAME ## Copy< iterator, const_iterator >(begin(),end(),a.cbegin());\
        return *this;\
    }\
     \
    SubMatrix& operator SYM (const Matrix< value_type >& a) noexcept {\
        NAME ## Copy< iterator, const_iterator >(begin(),end(),a.cbegin()); \
        return *this; \
    } \
     \
    SubMatrix& operator SYM (const SliceMatrix< value_type >& a) noexcept {\
        NAME ## Copy< iterator, const SliceIterator< value_type> >(begin(),end(),a.cbegin());\
        return *this;\
    }\
     \
    template < class X > \
    SubMatrix& operator SYM (const MatrixExpr< value_type, X >& x) noexcept {\
        Expr ## NAME ## Copy< value_type, iterator, X >(begin(),x,m_size);\
        return *this;\
    }\
     \
    SubMatrix& operator SYM (const_reference v) noexcept {\
        NAME ## Fill< iterator, const_reference >(begin(),v,m_size);\
    }

PETLIB_MAKE_OP(Add,+=)
PETLIB_MAKE_OP(Sub,-=)
PETLIB_MAKE_OP(Mul,*=)
PETLIB_MAKE_OP(Div,/=)
PETLIB_MAKE_OP(Mod,%=)
PETLIB_MAKE_OP(Xor,^=)
PETLIB_MAKE_OP(Ior,|=)
PETLIB_MAKE_OP(And,&=)
PETLIB_MAKE_OP(Lsh,<<=)
PETLIB_MAKE_OP(Rsh,>>=)
#undef PETLIB_MAKE_OP

    value_type Norm() const noexcept { return petlib::Norm2< value_type, const_iterator >(m_ptr,m_size);}
    value_type Asum() const noexcept { return petlib::Asum< value_type, const_iterator >(m_ptr,m_size);}
    size_type IndexAmax() const noexcept { return petlib::AmaxIndex< value_type, const_iterator >(m_ptr,m_size);}
    size_type IndexAmin() const noexcept { return petlib::AminIndex< value_type, const_iterator >(m_ptr,m_size);}
    size_type IndexMax() const noexcept { return petlib::MaxIndex< value_type, const_iterator >(m_ptr,m_size);}
    size_type IndexMin() const noexcept { return petlib::MinIndex< value_type, const_iterator >(m_ptr,m_size);}
    value_type Max() const noexcept { return petlib::MaxValue< value_type, const_iterator >(m_ptr,m_size);}
    value_type Min() const noexcept { return petlib::MinValue< value_type, const_iterator >(m_ptr,m_size);}
    value_type Amax() const noexcept { return petlib::AmaxValue< value_type, const_iterator >(m_ptr,m_size);}
    value_type Amin() const noexcept { return petlib::AminValue< value_type, const_iterator >(m_ptr,m_size);}
  
    constexpr value_type Dot(const SubMatrix& a) const noexcept {
        return petlib::Dot< value_type, const_iterator, const_iterator >(cbegin(),a.cbegin(),m_size);
    }
    constexpr value_type Dot(const Matrix<value_type>& a) const noexcept {
        return petlib::Dot< value_type, const_iterator, const_iterator >(cbegin(),a.cbegin(),m_size);
    }
    constexpr value_type Dot(const SliceMatrix<value_type>& a) const noexcept {
        return petlib::Dot< value_type, const_iterator, const SliceIterator<value_type> >(cbegin(),a.cbegin(),m_size);
    }
    constexpr void Swap(SubMatrix& a) noexcept {
        return petlib::Swap< iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(Matrix<value_type>& a) noexcept {
        return petlib::Swap< iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(SliceMatrix<value_type>& a) noexcept {
        return petlib::Swap< iterator, SliceIterator<value_type> >(begin(),a.begin(),m_size);
    }
    constexpr void Rot(SubMatrix& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, iterator >(begin(),a.begin(),cs,sn,m_size);
    }
    constexpr void Rot(Matrix<value_type>& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, iterator >(begin(),a.begin(),cs,sn,m_size);
    }
    constexpr void Rot(SliceMatrix<value_type>& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, SliceIterator<value_type> >(begin(),a.begin(),cs,sn,m_size);
    }    
private:
    pointer m_ptr;
    size_type m_size;
};

template < typename Tp >
class SliceMatrix:  public MatrixBase< Tp , SliceMatrix<Tp> > {
    typedef Tp value_type;
    typedef Tp& reference;
    typedef const Tp& const_reference;
    typedef Tp* pointer;
    typedef const Tp* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef SliceIterator< Tp > iterator;
    typedef const SliceIterator< Tp > const_iterator;
    typedef std::reverse_iterator< iterator > reverse_iterator; 
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;

    SliceMatrix(const_pointer p,size_type n,size_type s):m_ptr(p),m_size(n),m_str(s) {}
    
    explicit SliceMatrix(Matrix<value_type>& a):m_ptr(a.data()),m_size(a.size()),m_str(1) {}

    ~SliceMatrix() {
        m_ptr = nullptr;
    }
    
    SliceMatrix operator()(Range& r) noexcept {
        return SliceMatrix(m_ptr + r.offset()*m_str,r.size(),m_str);
    }
    SliceMatrix< value_type > operator()(Slice& s) noexcept {
        return SliceMatrix(m_ptr + s.offset()*m_str,s.size(),m_str*s.stride());
    } 
    Matrix< value_type > operator()(const Range& r) const noexcept { 
        return Matrix< value_type >::Matrix(
               SliceIterator< value_type >(m_ptr + r.offset()*m_str, m_str),
               SliceIterator< value_type >(m_ptr + (r.offset()+r.size())*m_str,m_str)
               );
    }    
    Matrix< value_type > operator()(const Slice& s) const noexcept { 
        return Matrix< value_type >::Matrix(
               SliceIterator< value_type >(m_ptr + s.offset()*m_str, s.stride()*m_str),
               SliceIterator< value_type >(m_ptr + (s.offset()+s.size()*s.stride())*m_str,s.stride()*m_str)
               );
    }
    constexpr bool empty() const noexcept { return m_size==0;}
    reference operator[](size_type i) noexcept { return m_ptr[i*m_str];}
    const_reference operator[](size_type i) const noexcept { return m_ptr[i*m_str];}
    constexpr pointer data() noexcept { return m_ptr;}
    constexpr const_pointer data() const noexcept { return m_ptr;}
    constexpr size_type size() const noexcept { return m_size;}
    constexpr size_type stride() const noexcept { return m_str;}
    constexpr iterator begin() noexcept { return m_ptr;}
    constexpr iterator end() noexcept { return (m_ptr+m_size*m_str);}
    constexpr const_iterator cbegin() const noexcept { return m_ptr;}
    constexpr const_iterator cend() const noexcept { return (m_ptr+m_size*m_str);}
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end());}
    constexpr reverse_iterator rend() noexcept { return  reverse_iterator(begin());}
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end());}
    constexpr const_reverse_iterator crend() const noexcept { return  const_reverse_iterator(begin());}
    
    SliceMatrix& operator=(const Matrix< value_type >& a) noexcept {
        petlib::Copy< iterator, const_pointer >(begin(),end(),a.cbegin());
        return *this;
    }

    SliceMatrix& operator=(const SubMatrix< value_type >& a) noexcept {
        petlib::Copy< iterator, pointer >(begin(),end(),a.cbegin());
        return *this;
    }

    SliceMatrix& operator=(const SliceMatrix< value_type >& a) noexcept {
        petlib::Copy< iterator,  iterator >(begin(),end(),a.cbegin());
        return *this;
    }

    template < class X >
    SliceMatrix& operator=(const MatrixExpr< value_type, X >& x) noexcept {
        ExprCopy< value_type, iterator, X >(begin(),end(),x);
        return *this;
    }

    SliceMatrix& operator=(const_reference v) noexcept {
        Fill< value_type, iterator >(begin(),v,m_size);
        return *this;
    }

#define PETLIB_MAKE_OP(NAME,SYM)\
    SliceMatrix& operator SYM (const SubMatrix< value_type >& a) noexcept {\
        NAME ## Copy< iterator, pointer >(begin(),end(),a.cbegin());\
        return *this;\
    }\
     \
    SliceMatrix& operator SYM (const Matrix< value_type >& a) noexcept {\
        NAME ## Copy< iterator, pointer >(begin(),end(),a.cbegin()); \
        return *this; \
    } \
      \
    SliceMatrix& operator SYM (const SliceMatrix< value_type >& a) noexcept {\
        NAME ## Copy< iterator, iterator >(begin(),end(),a.cbegin());\
        return *this;\
    }\
     \
    template < class X > \
    SliceMatrix& operator SYM (const MatrixExpr< value_type, X >& x) noexcept {\
        Expr ## NAME ## Copy< value_type, iterator, X >(begin(),x,m_size);\
        return *this;\
    }\
     \
    SliceMatrix& operator SYM (const_reference v) noexcept {\
        NAME ## Fill< iterator, value_type >(begin(),v,m_size);\
    }

PETLIB_MAKE_OP(Add,+=)
PETLIB_MAKE_OP(Sub,-=)
PETLIB_MAKE_OP(Mul,*=)
PETLIB_MAKE_OP(Div,/=)
PETLIB_MAKE_OP(Mod,%=)
PETLIB_MAKE_OP(Xor,^=)
PETLIB_MAKE_OP(Ior,|=)
PETLIB_MAKE_OP(And,&=)
PETLIB_MAKE_OP(Lsh,<<=)
PETLIB_MAKE_OP(Rsh,>>=)
#undef PETLIB_MAKE_OP

    value_type Norm() const noexcept { return petlib::Norm2< value_type, iterator >(m_ptr,m_size);}
    value_type Asum() const noexcept { return petlib::Asum< value_type, iterator >(m_ptr,m_size);}
    size_type IndexAmax() const noexcept { return petlib::AmaxIndex< value_type, iterator >(m_ptr,m_size);}
    size_type IndexAmin() const noexcept { return petlib::AminIndex< value_type, iterator >(m_ptr,m_size);}
    size_type IndexMax() const noexcept { return petlib::MaxIndex< value_type, iterator >(m_ptr,m_size);}
    size_type IndexMin() const noexcept { return petlib::MinIndex< value_type, iterator >(m_ptr,m_size);}
    value_type Max() const noexcept { return petlib::MaxValue< value_type, iterator >(m_ptr,m_size);}
    value_type Min() const noexcept { return petlib::MinValue< value_type, iterator >(m_ptr,m_size);}
    value_type Amax() const noexcept { return petlib::AmaxValue< value_type, iterator >(m_ptr,m_size);}
    value_type Amin() const noexcept { return petlib::AminValue< value_type, iterator >(m_ptr,m_size);}
  
    constexpr value_type Dot(const SliceMatrix& a) const noexcept {
        return petlib::Dot< value_type, iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr value_type Dot(const Matrix<value_type>& a) const noexcept {
        return petlib::Dot< value_type, iterator, pointer >(begin(),a.begin(),m_size);
    }
    constexpr value_type Dot(const SubMatrix<value_type>& a) const noexcept {
        return petlib::Dot< value_type, iterator, pointer >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(SliceMatrix& a) noexcept {
        return petlib::Swap< iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(Matrix<value_type>& a) noexcept {
        return petlib::Swap< iterator, pointer >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(SubMatrix<value_type>& a) noexcept {
        return petlib::Swap< iterator, pointer >(begin(),a.begin(),m_size);
    }
    constexpr void Rot(SliceMatrix& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, iterator >(begin(),a.begin(),cs,sn,m_size);
    }
    constexpr void Rot(Matrix<value_type>& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, pointer >(begin(),a.begin(),cs,sn,m_size);
    }
    constexpr void Rot(SubMatrix<value_type>& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, pointer >(begin(),a.begin(),cs,sn,m_size);
    }    
private:
    pointer m_ptr;
    size_type m_size;
    size_type m_str;
};


}
#endif

}
#endif
