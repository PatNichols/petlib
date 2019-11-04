#ifndef PETLIB_MATRIX_HPP
#define PETLIB_MATRIX_HPP

namespace petlib {
using std::size_t size_t;
using std::ptrdiff_t ptrdiff_t;

template < typename T > class SubMatrix;

template < typename T >
class Matrix: public MatrixBase< Matrix<T>, T>  {
   T * data_;
   size_t n1;
   size_t n2;
   size_t ntot;
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
   
   
   
   Matrix(size_t n1_,size_t n2_):data_(new T[n1_*n2_]),n1(n1_),n2(n2_),ntot(n1_*n2_) {}
   
   Matrix()=default;

   Matrix(const Matrix& m):data_(new T[m.ntot]),n1(m.n1),n2(m.n2),ntot(m.ntot) {}

   Matrix(Matrix&& m):data(m.data_),n1(m.n1),n2(m.n2),ntot(m.ntot) { 
       m.data_ = nullptr;
       m.n1 = m.n2 = m.ntot = 0;
   }

   template < xpr_t >
   Matrix(const MatrixXpr<xpr_t>& m):data(m.nrows()*m.ncols()),n1(m.nrows()),n2(m.ncols()),ntot(m.nrows()*m.ncols()) {
       T * dp = data_;
       for (size_type i = 0; i < n1 ; ++i) {
        for (size_type j = 0; j < n2;  ++j,++dp) {
           *dp = m(i,j);
        }
       }
   }

   Matrix(const SubMatrix<xT>& m):data(m.nrows()*m.ncols()),n1(m.nrows()),n2(m.ncols()),ntot(m.nrows()*m.ncols()) {
       T * dp = data_;
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

   Matrix& operator=(const SubMatrix<T>& m) {
       T * p = data_;
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
   Matrix& operator+=(const Matrix& m) {
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
   Matrix& operator = (const MatrixBase< mat_type, T>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] = m(i,j);
       return *this;
   }
   template < typename mat_type >   
   Matrix& operator += (const MatrixBase< mat_type, T>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] += m(i,j);
       return *this;
   }
   template < typename mat_type >   
   Matrix& operator -= (const MatrixBase< mat_type, T>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] -= m(i,j);
       return *this;
   }
   template < typename mat_type >   
   Matrix& operator *= (const MatrixBase< mat_type, T>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] *= m(i,j);
       return *this;
   }
   template < typename mat_type >   
   Matrix& operator /= (const MatrixBase< mat_type, T>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] /= m(i,j);
       return *this;
   }

   template < typename xpr_type >   
   Matrix& operator = (const MaxtrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] = m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   Matrix& operator += (const MaxtrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] += m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   Matrix& operator -= (const MaxtrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] -= m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   Matrix& operator *= (const MaxtrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] *= m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   Matrix& operator /= (const MaxtrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] /= m(i,j);
       return *this;
   }
      
   Matrix& operator=( T x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] = x;
       return *this   
   }
    
   Matrix& operator+=( T x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] += x;
       return *this   
   }
    
   Matrix& operator-=( T x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] -= x;
       return *this   
   }
   
   Matrix& operator*=( T x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] *= x;
       return *this   
   }
   
   Matrix& operator/=( T x) {
       T xi  = T(1)/x;
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*n2+j] *= xi;
       return *this   
   }
    
   SubMatrix<T> operator()(Range& row_rng,Range& col_rng) {
       return SubMatrix(data_+row_rng.offset()*n2+col_rng.offset(),row_rng.size(),col_rng.size(),n2);
   }
   
   value_t operator()(size_t i,size_t j) const { return data_[i*n2+j];}
   
   reference_t operator()(size_t i,size_t j)  { return data_[i*n2+j];}

   T * data() { return data_;}
   const T * data() const { return data_;} 
   
   SubArray<T> row_array(size_t i) { return SubArray<T>(data+i*n2,n2);}
   SliceArray<T> col_array(size_t i) { return SliceArray<T>(data+i,n1,n2);}


   ArrayIterator<T> row_begin(size_t i) { return ArrayIterator<T>(data+i*n2,n2);}
   SliceIterator<T> col_begin(size_t i) { return SliceIterator<T>(data+i,n1,n2);}
   const ArrayIterator<T> row_end(size_t i) const { return SubArray<T>(data+i*n2+n2,n2);}
   const SliceIterator<T> col_end(size_t i) const { return SliceIterator<T>(data+i+n2*n1,n1,n2);}
   const ArrayIterator<T> row_cend(size_t i) const { return ArrayIterator<T>(data+i*n2,n2);}
   const SliceIterator<T> col_cend(size_t i) const { return SliceIterator<T>(data+i,n1,n2);}
   ArrayIterator<T> row_cbegin(size_t i) { return SubArray<T>(data+i*n2+n2,n2);}
   SliceIterator<T> col_cbegin(size_t i) { return SliceIterator<T>(data+i+n2*n1,n1,n2);}
   
   size_t ncols() const { return n2;}
   size_t nrows() const { return n1;}
   bool empty() const { return data_==0;}
   bool is_square() const { return n1==n2;}
   bool is_symmetric() const {
     if (n1!=n2) return false;
     for (size_t i=0;i<n1;++i) {
        SliceIterator<T> ci(data_+i,n1,n2);
        ArrayIterator<T> ri(daat_+i*n2,n2);  
        for (size_t j=0;j<=i;++j,++ci,++ri) {
           if ( *ci != *ri) return false;
        }
     }
     return true;   
   }
   bool is_skey_symmetric() const {
     if (n1!=n2) return false;
     for (size_t i=0;i<n1;++i) {
        SliceIterator<T> ci(data_+i,n1,n2);
        ArrayIterator<T> ri(daat_+i*n2,n2);  
        for (size_t j=0;j<i;++j,++ci,++ri) {
           if ( *ci != -*ri) return false;
        }
     }
     return true;   
   }



   
   ////
   //   Return a view of the Matrix starting in row r1.offset and ending in r1.size()+r1.offset() and
   //     columns r2.offset() to r2.offset()+r2.size()
   ////
   
   SubMatrix<T> operator(Range& r1,Range& r2) { 
       return SubMatrix(data_+r1.offset()*n2+r2.offset(),r1.size(),r2.size(),n2);
   }
   
   void swap_rows(size_t i,size_t j) {
       std::swap(data_+i*n2,data_+i*n2+n2,data_+j*n2);
   }
   void elim_row(size_t i,size_t j,const T c) {
       for (size_type k=0;k<n2;++k) data_[i*n2+k] -= data[j*n2+k]*c;
   }
   
   void rotate_rows(size_t i,size_t j,T angle) {
       pointer p = data_ + i * n2;
       pointer q = data_ + j * n2;
       T cs = std::cos(angle);
       T sn = std::sin(angle);
       for (size_t i=0;i<n2;++i) {
          T tmp = p[i];
          p[i] = p[i]*cs - q[i]*sn;
          q[i] = q[i]*cs + tmp *sn;
       }
   }
   
   difference_type find_row_pivot(size_t j) {
       T * p =std::max(data_+j*n2,data_+j*n2+n2,bool_abs_cmp<T>); 
       return (p-data_+j*n2);
   }
   
   void fill_row(size_t j,const T* ptr) {
      std::copy(ptr,ptr+n2,data_+j*n2);
   }
   
   void scal_row(size_t i,const T factor) {
      dp = data_ + i * n2;
      for (size_t j=0;j<n2;++j) dp[j] *= factor;
   }
   
   void swap_cols(size_t i,size_t j) {
       std::swap(SliceIterator<T>(data+i,n2),SliceIterator(data+i+n2*n1,n2),SliceIterator(data_+j,n2));
   }
   
   void elim_col(size_t i,size_t j,const T c) {
       dpi = data_ + i;
       dpj = data_ + j;
       for (size_t k=0;k<n1;++k) {
          *dpi -= *dpj *c;
          dpi += n2;
          dpj += n2;
       }
   }
   void  rotate_cols(size_t i,size_t j,T angle) {
       pointer p = data_ + i;
       pointer q = data_ + j;
       T cs = std::cos(angle);
       T sn = std::sin(angle);
       for (size_t i=0;i<n1;++i) {
          T tmp = *p;
          *p = *p*cs - *q*sn;
          *q = *q*cs + tmp *sn;
          p += n2;
          q += n2;
       }
   }
   difference_type find_col_pivot(size_t j) {
       SliceIterator<T> iter(data_+j,n2);
       const SliceIterator<T> iend(data_+j+n2,n2);
       SliceIterator max_elem =std::max(iter,iend,bool_abs_cmp<T>); 
       return (max_elem.data()-data_+j);
   }
   
   void fill_col(size_t j,const T* ptr) {
       SliceIterator<T> iter(data_+j,n2);
       std::copy(ptr,ptr+n,iter);
   }
   
   void scal_col(size_t j,const T factor) {
       SliceIterator<T> iter(data_+j,n2);
       const SliceIterator<T> iend(data_+j+n2,n2);
       for ( ;iter!=iend;++iter) {
         *iter *= factor;
       }   
   }
   
   T col_norm(size_t i) const {
       pointer q = data + i;
       T sum(0);
       for (size_t i=0;i<n1;++i,q+=n2) sum += *q * *q;
       return sum;
   }
   T row_norm(size_t i) const {
       pointer p = data + i * n2;
       T sum(0);
       for (size_t i=0;i<n2;++i) sum+=p[i]*p[i];
       return sum;
   }
   T row_norm(size_t i,Range& r) {
       T sum=0;
       T * p = data_ + i * n2 + r.offset()();
       for (size_t k=0;k<r.size();++k) sum += p[k]*p[k];
       return sum;
   }

   T col_norm(size_t i,Range& r) {
       size_t low = r.offset();
       T sum=0;
       SliceIterator p(data_+i+r.offset()*n2,n2);
       for (size_t k=0;k<r.size();++k,++p) sum += (*p) * (*p);
       return sum;
   }
       
   void scal_diag(const T& c) {
       SliceIterator<T> p(data_,n2+1);  
       const SliceIterator<T> pend(data_+n1*n2+n1,n2+1);
       for (;p!=pend;++p) { p *= c; }
   }
   
   void assign_diag(const T* ptr) {
       SliceIterator<T> p(data_,n2+1);  
       std::copy(ptr,ptr+n1,p);    
   }
   
   void assign_diag(const T& c)
   {
       SliceIterator<T> p(data_,n2+1);  
       const SliceIterator<T> pend(data_+n1*n2+n1,n2+1);
       for (;p!=pend;++p) { p = c; }   
   }
   
   Matrix<T> transpose() const {
       Matrix<T> t(n2,n1);
       for (size_t i=0;i<n1;++i) {
          SliceIterator<T> tp(t.data_ + i,n1);
          for (size_t jj=0;j<n2;++j,++tp) {
              *tp = data_[i*n2+j];
          }  
       }
       return t;
   }
   
   void resize(size_t new_n1,size_t new_n2) {
       if (data_) delete [] data_;
       data=new T[new_n1*new_n2];
       n1 = new_n1;
       n2 = new_n2;
   }
   
   T norm() const {
       T sum(0);
       for (size_t i=0;i<n1;++i) sum += row_norm(i);
       return sum;
   }
    
   T trace() const {
       T sum(0);
       SliceIterator p(data_,n2+1);
       for (size_t k=0;k<n1;++k) sum+=p[k];
       return sum;
   }   
   
};

template < typename T >
class SubMatrix: public MatrixBase< SubMatrix<T>, T>  {
   T * data_;
   size_t n1;
   size_t n2;
   size_t nstr;
   size_t ntot;
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
   
  
   SubMatrix(const T * p, size_t n1_, size_t n2_, size_t str):data_(p),n1(n1_),n2(n2_),nstr(str),ntot(n1_*n2_) {}  
   
   SubMatrix()=default;

   SubMatrix(const SubMatrix& m):data_(m.data_),n1(m.n1),n2(m.n2),nstr(m.nstr),ntot(m.ntot) {}

   SubMatrix(SubMatrix&& m):data(m.data_),n1(m.n1),n2(m.n2),nstr(m.nstr),ntot(m.ntot) { 
       m.data_ = nullptr;
       m.n1 = m.n2 = m.ntot = 0;
   }

   SubMatrix(const Matrix<xT>& m):data(m.nrows()*m.ncols()),n1(m.nrows()),n2(m.ncols()),nstr(m.ncols()),ntot(m.nrows()*m.ncols()) {
       T * dp = data_;
       for (size_type i = 0; i < n1 ; ++i) {
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
          T * p = data_ + i * nstr;
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
   SubMatrix& operator+=(const SubMatrix& m) {
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
   SubMatrix& operator = (const MatrixBase< mat_type, T>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] = m(i,j);
       return *this;
   }
   template < typename mat_type >   
   SubMatrix& operator += (const MatrixBase< mat_type, T>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] += m(i,j);
       return *this;
   }
   template < typename mat_type >   
   SubMatrix& operator -= (const MatrixBase< mat_type, T>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] -= m(i,j);
       return *this;
   }
   template < typename mat_type >   
   SubMatrix& operator *= (const MatrixBase< mat_type, T>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] *= m(i,j);
       return *this;
   }
   template < typename mat_type >   
   SubMatrix& operator /= (const MatrixBase< mat_type, T>& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] /= m(i,j);
       return *this;
   }

   template < typename xpr_type >   
   SubMatrix& operator = (const MaxtrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] = m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   SubMatrix& operator += (const MaxtrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] += m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   SubMatrix& operator -= (const MaxtrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] -= m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   SubMatrix& operator *= (const MaxtrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] *= m(i,j);
       return *this;
   }
   template < typename xpr_type >   
   SubMatrix& operator /= (const MaxtrixXpr< xpr_type >& m)
   {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] /= m(i,j);
       return *this;
   }
      
   SubMatrix& operator=( T x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] = x;
       return *this   
   }
    
   SubMatrix& operator+=( T x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] += x;
       return *this   
   }
    
   SubMatrix& operator-=( T x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] -= x;
       return *this   
   }
   
   SubMatrix& operator*=( T x) {
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] *= x;
       return *this   
   }
   
   SubMatrix& operator/=( T x) {
       T xi  = T(1)/x;
       for (size_type i=0;i<n1;++i) 
        for (size_type j=0;j<n2;++j)
          data_[i*nstr+j] *= xi;
       return *this   
   }
    
   SubMatrix<T> operator()(Range& row_rng,Range& col_rng) {
       return SubMatrix(data_+row_rng.offset()*n2+col_rng.offset(),row_rng.size(),col_rng.size(),n2);
   }
   
   value_t operator()(size_t i,size_t j) const { return data_[i*nstr+j];}
   
   reference_t operator()(size_t i,size_t j)  { return data_[i*nstr+j];}

   T * data() { return data_;}
   const T * data() const { return data_;} 
   
   SubArray<T> row_array(size_t i) { return SubArray<T>(data+i*n2,n2);}
   SliceArray<T> col_array(size_t i) { return SliceArray<T>(data+i,n1,n2);}


   ArrayIterator<T> row_begin(size_t i) { return ArrayIterator<T>(data+i*n2,n2);}
   SliceIterator<T> col_begin(size_t i) { return SliceIterator<T>(data+i,n1,n2);}
   const ArrayIterator<T> row_end(size_t i) const { return SubArray<T>(data+i*n2+n2,n2);}
   const SliceIterator<T> col_end(size_t i) const { return SliceIterator<T>(data+i+n2*n1,n1,n2);}
   const ArrayIterator<T> row_cend(size_t i) const { return ArrayIterator<T>(data+i*n2,n2);}
   const SliceIterator<T> col_cend(size_t i) const { return SliceIterator<T>(data+i,n1,n2);}
   ArrayIterator<T> row_cbegin(size_t i) { return SubArray<T>(data+i*n2+n2,n2);}
   SliceIterator<T> col_cbegin(size_t i) { return SliceIterator<T>(data+i+n2*n1,n1,n2);}
   
   size_t ncols() const { return n2;}
   size_t nrows() const { return n1;}
   bool empty() const { return data_==0;}
   bool is_square() const { return n1==n2;}
   bool is_symmetric() const {
     if (n1!=n2) return false;
     for (size_t i=0;i<n1;++i) {
        SliceIterator<T> ci(data_+i,n1,n2);
        ArrayIterator<T> ri(daat_+i*n2,n2);  
        for (size_t j=0;j<=i;++j,++ci,++ri) {
           if ( *ci != *ri) return false;
        }
     }
     return true;   
   }
   bool is_skey_symmetric() const {
     if (n1!=n2) return false;
     for (size_t i=0;i<n1;++i) {
        SliceIterator<T> ci(data_+i,n1,n2);
        ArrayIterator<T> ri(daat_+i*n2,n2);  
        for (size_t j=0;j<i;++j,++ci,++ri) {
           if ( *ci != -*ri) return false;
        }
     }
     return true;   
   }



   
   ////
   //   Return a view of the Matrix starting in row r1.offset and ending in r1.size()+r1.offset() and
   //     columns r2.offset() to r2.offset()+r2.size()
   ////
   
   SubMatrix<T> operator(Range& r1,Range& r2) { 
       return SubMatrix(data_+r1.offset()*n2+r2.offset(),r1.size(),r2.size(),n2);
   }
   
   void swap_rows(size_t i,size_t j) {
       std::swap(data_+i*n2,data_+i*n2+n2,data_+j*n2);
   }
   void elim_row(size_t i,size_t j,const T c) {
       for (size_type k=0;k<n2;++k) data_[i*n2+k] -= data[j*n2+k]*c;
   }
   
   void rotate_rows(size_t i,size_t j,T angle) {
       pointer p = data_ + i * n2;
       pointer q = data_ + j * n2;
       T cs = std::cos(angle);
       T sn = std::sin(angle);
       for (size_t i=0;i<n2;++i) {
          T tmp = p[i];
          p[i] = p[i]*cs - q[i]*sn;
          q[i] = q[i]*cs + tmp *sn;
       }
   }
   
   difference_type find_row_pivot(size_t j) {
       T * p =std::max(data_+j*n2,data_+j*n2+n2,bool_abs_cmp<T>); 
       return (p-data_+j*n2);
   }
   
   void fill_row(size_t j,const T* ptr) {
      std::copy(ptr,ptr+n2,data_+j*n2);
   }
   
   void scal_row(size_t i,const T factor) {
      dp = data_ + i * n2;
      for (size_t j=0;j<n2;++j) dp[j] *= factor;
   }
   
   void swap_cols(size_t i,size_t j) {
       std::swap(SliceIterator<T>(data+i,n2),SliceIterator(data+i+n2*n1,n2),SliceIterator(data_+j,n2));
   }
   
   void elim_col(size_t i,size_t j,const T c) {
       dpi = data_ + i;
       dpj = data_ + j;
       for (size_t k=0;k<n1;++k) {
          *dpi -= *dpj *c;
          dpi += n2;
          dpj += n2;
       }
   }
   void  rotate_cols(size_t i,size_t j,T angle) {
       pointer p = data_ + i;
       pointer q = data_ + j;
       T cs = std::cos(angle);
       T sn = std::sin(angle);
       for (size_t i=0;i<n1;++i) {
          T tmp = *p;
          *p = *p*cs - *q*sn;
          *q = *q*cs + tmp *sn;
          p += n2;
          q += n2;
       }
   }
   difference_type find_col_pivot(size_t j) {
       SliceIterator<T> iter(data_+j,n2);
       const SliceIterator<T> iend(data_+j+n2,n2);
       SliceIterator max_elem =std::max(iter,iend,bool_abs_cmp<T>); 
       return (max_elem.data()-data_+j);
   }
   
   void fill_col(size_t j,const T* ptr) {
       SliceIterator<T> iter(data_+j,n2);
       std::copy(ptr,ptr+n,iter);
   }
   
   void scal_col(size_t j,const T factor) {
       SliceIterator<T> iter(data_+j,n2);
       const SliceIterator<T> iend(data_+j+n2,n2);
       for ( ;iter!=iend;++iter) {
         *iter *= factor;
       }   
   }
   
   T col_norm(size_t i) const {
       pointer q = data + i;
       T sum(0);
       for (size_t i=0;i<n1;++i,q+=n2) sum += *q * *q;
       return sum;
   }
   T row_norm(size_t i) const {
       pointer p = data + i * n2;
       T sum(0);
       for (size_t i=0;i<n2;++i) sum+=p[i]*p[i];
       return sum;
   }
   T row_norm(size_t i,Range& r) {
       T sum=0;
       T * p = data_ + i * n2 + r.offset()();
       for (size_t k=0;k<r.size();++k) sum += p[k]*p[k];
       return sum;
   }

   T col_norm(size_t i,Range& r) {
       size_t low = r.offset();
       T sum=0;
       SliceIterator p(data_+i+r.offset()*n2,n2);
       for (size_t k=0;k<r.size();++k,++p) sum += (*p) * (*p);
       return sum;
   }
       
   void scal_diag(const T& c) {
       SliceIterator<T> p(data_,n2+1);  
       const SliceIterator<T> pend(data_+n1*n2+n1,n2+1);
       for (;p!=pend;++p) { p *= c; }
   }
   
   void assign_diag(const T* ptr) {
       SliceIterator<T> p(data_,n2+1);  
       std::copy(ptr,ptr+n1,p);    
   }
   
   void assign_diag(const T& c)
   {
       SliceIterator<T> p(data_,n2+1);  
       const SliceIterator<T> pend(data_+n1*n2+n1,n2+1);
       for (;p!=pend;++p) { p = c; }   
   }
   
   Matrix<T> transpose() const {
       Matrix<T> t(n2,n1);
       for (size_t i=0;i<n1;++i) {
          SliceIterator<T> tp(t.data_ + i,n2);
          for (size_t jj=0;j<n2;++j,++tp) {
              *tp = data_[i*nstr+j];
          }  
       }
       return t;
   }
      
   T norm() const {
       T sum(0);
       for (size_type i=0;i<n1;++i) {
         T * p = data_+i*nstr;
         for (size_type j = 0; j < n2; ++j,++p) {
           sum += (*p)*(*p);
         }
       }
       return sum;
   }
    
   T trace() const {
       T sum(0);
       SliceIterator p(data_,nstr+1);
       for (size_t k=0;k<n1;++k) sum+=p[k];
       return sum;
   }   
   
};


}
#endif  