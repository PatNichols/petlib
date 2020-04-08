#ifndef PETLIB_ARRAY_H
#define PETLIB_ARRAY_H

#include <algorithm>
#include <cassert>

#include <petlib_array_ops.hpp>
#include <petlib_math.hpp>
#include <petlib_range.hpp>
#include <petlib_slice.hpp>

namespace petlib {

template <typename T>
class Array;
template <typename T>
class SubArray;
template <typename T>
class SliceArray;

template <class T>
class Array : public ArrayBase<Array<T>, T> {
 public:
  typedef T value_t;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef T* pointer_t;
  typedef const T* const_pointer_t;
  typedef T& reference_t;
  typedef const T& const_reference_t;
  typedef RangeIterator<T> iterator_t;

  Array():data_(nullptr),n(0) {}

  Array(size_t sz) : data_(new T[sz]), n(sz) {}

  Array(Array&& a) : data_(a.data_), n(a.n) {
    a.data_ = nullptr;
    a.n = 0;
  }

  Array(const Array& a) : data_(new T[a.n]), n(a.n) {
    std::copy(a.data_, a.data_ + n, data_);
  }

  template <class A_t, typename other_type>
  Array(const ArrayBase<A_t, other_type>& a)
      : data_(new T[a.size()]), n(a.size()) {
    for (auto i = 0; i < n; ++i) data_[i] = a[i];
  }

  template <class Xpr_t>
  Array(const ArrayXpr<Xpr_t>& a) : data_(new T[a.size()]), n(a.size()) {
    std::cerr << "debug \n";
    std::cerr << "xpr.size = " << a.size() << "\n"; 
    for (auto i = 0; i < n; ++i) {
      std::cerr << i << " " << a[i] << "\n";
      data_[i] = a[i];
    }
    std::cerr << "end debug\n";
  }

  ~Array() {
    if (data_) delete[] data_;
    n = 0;
  }

  ///// various operators
   Array& operator=(Array&& a) noexcept {
    delete[] data_;
    data_ = a.data_;
    a.data_ = nullptr;
    n = a.n;
    a.n = 0;
    return *this;
  }

   Array& operator=(const Array& x) noexcept {
//    assert(n == x.n);
    if (n!=x.n) {
       if (data_) delete [] data_;
       data_= new T[x.n];
       n = x.n;
    }
    std::copy(x.data_, x.data_ + n, data_);
    return *this;
  }

  template <class A_t, typename other_type>
   Array& operator=(const ArrayBase<A_t, other_type>& a) noexcept {
    assert(n == a.size());
    for (auto i = 0; i < n; ++i) data_[i] = a[i];
    return *this;
  }

  template <class Xpr_t>
   Array& operator=(const ArrayXpr<Xpr_t>& a) noexcept {
//    assert(n == a.size());
    std::cerr << "debug \n";
    std::cerr << "xpr.size = " << a.size() << "\n"; 
    for (auto i = 0; i < n; ++i) {
      std::cerr << i << " " << a[i] << "\n";
      data_[i] = a[i];
    }
    std::cerr << "end debug\n";
    return *this;
  }

   Array& operator=(const value_t& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] = x;
    return *this;
  }

   Array& operator+=(const Array& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] += x.data_[i];
    return *this;
  }
   Array& operator-=(const Array& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] -= x.data_[i];
    return *this;
  }
   Array& operator*=(const Array& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] *= x.data_[i];
    return *this;
  }
   Array& operator/=(const Array& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] /= x.data_[i];
    return *this;
  }

  template <class Xpr_t>
   Array& operator+=(const ArrayXpr<Xpr_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] += x[i];
    return *this;
  }
  template <class Xpr_t>
   Array& operator-=(const ArrayXpr<Xpr_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] -= x[i];
    return *this;
  }
  template <class Xpr_t>
   Array& operator*=(const ArrayXpr<Xpr_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] *= x[i];
    return *this;
  }
  template <class Xpr_t>
   Array& operator/=(const ArrayXpr<Xpr_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] /= x[i];
    return *this;
  }

  template <class A_t>
   Array& operator+=(const ArrayBase<A_t, value_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] += x[i];
    return *this;
  }
  template <class A_t>
   Array& operator-=(const ArrayBase<A_t, value_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] -= x[i];
    return *this;
  }
  template <class A_t>
   Array& operator*=(const ArrayBase<A_t, value_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] *= x[i];
    return *this;
  }
  template <class A_t>
   Array& operator/=(const ArrayBase<A_t, value_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] /= x[i];
    return *this;
  }

   Array& operator+=(const value_t x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] += x;
    return *this;
  }
   Array& operator-=(const value_t x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] -= x;
    return *this;
  }
   Array& operator*=(const value_t x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] *= x;
    return *this;
  }
   Array& operator/=(const value_t x) noexcept {
    value_t xi = value_t(1) / x;
    for (auto i = 0; i < n; ++i) data_[i] *= xi;
    return *this;
  }

   pointer_t begin() noexcept { return data_; }
   const_pointer_t begin() const noexcept { return data_; }
   const_pointer_t end() const noexcept { return (data_ + n); }
   pointer_t rbegin() noexcept { return (data_ + n); }
   const_pointer_t rbegin() const noexcept { return (data_ + n); }
   const_pointer_t rend() const noexcept { return data_; }
   const_pointer_t data() const noexcept { return data_; }
   pointer_t data() noexcept { return data_; }
   bool empty() noexcept { return n == 0; }
   size_type size() const noexcept { return n; }
   difference_type stride() const noexcept {
    return difference_type{1};
  }

   value_t& operator[](size_type i) noexcept { return data_[i]; }
   value_t operator[](size_type i) const noexcept { return data_[i]; }

   value_t dotProduct(const Array& a) noexcept {
    value_t sum(0);
    for (auto i = 0; i < n; ++i) sum += data_[i] * a[i];
    return sum;
  }

   value_t max() const noexcept { return *std::max_element(data_, data_ + n); }

   value_t amax() const noexcept {
    return *std::max_element(data_, data_ + n, petlib::abs_cmp<value_t>);
  }

   value_t min() const noexcept { return *std::min_element(data_, data_ + n); }

   value_t amin() const noexcept {
    return *std::min_element(data_, data_ + n, petlib::abs_cmp<value_t>);
  }

   size_t index_max() const noexcept {
    return size_t(std::max_element(data_, data_ + n) - data_);
  }

   size_t index_amax() const noexcept {
    pointer_t p = std::max_element(data_, data_ + n, petlib::abs_cmp<value_t>);  
    return size_t(p - data_);
  }

   size_t index_min() const noexcept {
    return size_t(std::min_element(data_, data_ + n) - data_);
  }

   size_t index_amin() const noexcept {
    pointer_t p = std::min_element(data_, data_ + n, petlib::abs_cmp<value_t>);  
    return size_t(p- data_);
  }

  //
  // scale vector so that the dot product with itself = 1 or 0 if array is null
  //

   void normalize() noexcept {
    value_t sum = 0;
    for (auto i = 0; i < n; ++i) sum += data_[i] * data_[i];
    if (sum < petlib::numeric_traits<T>::eps) {
      return;
    }
    sum = std::sqrt(sum);
    sum = value_t(1) / sum;
    for (auto i = 0; i < n; ++i) data_[i] *= sum;
  }

   void swap_elements(size_t i, size_t j) noexcept {
    value_t tmp = data_[i];
    data_[i] = data_[j];
    data_[j] = tmp;
  }

   void rotate_elements(size_t i, size_t j,
                                 const value_t& angle) noexcept {
    value_t tmp = data_[i];
    value_t cs = std::cos(angle);
    value_t sn = std::sin(angle);
    data_[i] = data_[i] * cs + data_[j] * sn;
    data_[j] = data_[j] * cs - tmp * sn;
  }

  template <class A_t>
   void reflect(const ArrayBase<A_t, value_t>& x) noexcept {
    value_t prd = dotProduct(*this, x);
    value_t den = dotProduct(x, x);
    value_t fact = prd / den;
    for (auto i = 0; i < n; ++i) data_[i] = data_[i] - fact * x[i];
  }

   SliceArray<T> operator()(Slice& s) noexcept {
    return SliceArray<T>(data_ + s.offset(), s.size(), s.stride());
  }

   SubArray<T> operator()(Range& r) noexcept {
    return SubArray<T>(data_ + r.offset(), r.size());
  }

   void resize(size_t new_size) noexcept {
    if (data_) delete[] data_;
    data_ = new T[new_size];
    n = new_size;
  }

   void grow(size_t new_size) noexcept {
    if (new_size < n) {
      n = new_size;
    } else {
      if (n != new_size) {
        T* tmp = new T[new_size];
        std::copy(tmp, tmp + n, data_);
        delete[] data_;
        data_ = tmp;
      }
    }
  }

 private:
  T* data_;
  size_type n;
};

template <typename T>
class SubArray : public ArrayBase<SubArray<T>, T> {
 public:
  typedef T value_t;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef T* pointer_t;
  typedef const T* const_pointer_t;
  typedef T& reference_t;
  typedef const T& const_reference_t;
  typedef RangeIterator<T> iterator_t;

  SubArray(pointer_t p, size_type len) : data_(p), n(len) {}

  SubArray() = delete;

  SubArray(SubArray&& a) : data_(a.data_), n(a.n) {
    a.data_ = nullptr;
    a.n = 0;
  }

  SubArray(const SubArray& a) : data_(a.data_), n(a.n) {}

  SubArray(const Array<T>& a) : data_(a.data()), n(a.size()) {}

  ~SubArray() {
    data_ = nullptr;
    n = 0;
  }

  ///// various operators
   SubArray& operator=(SubArray&& a) noexcept {
    data_ = a.data_;
    a.data_ = nullptr;
    n = a.n;
    a.n = 0;
    return *this;
  }

   SubArray& operator=(const SubArray& x) noexcept {
    assert(n == x.n);
    data_ = x.data_;
    n = x.n;
    return *this;
  }

   SubArray& operator=(const Array<T>& x) noexcept {
    assert(n == x.n);
    data_ = x.data();
    n = x.size();
    return *this;
  }

  template <class A_t, typename other_type>
   SubArray& operator=(const ArrayBase<A_t, other_type>& a) noexcept {
    assert(n == a.size());
    for (auto i = 0; i < n; ++i) data_[i] = a[i];
    return *this;
  }

  template <class Xpr_t>
   SubArray& operator=(const ArrayXpr<Xpr_t>& a) noexcept {
    assert(n == a.size());
    for (auto i = 0; i < n; ++i) data_[i] = a[i];
    return *this;
  }

   SubArray& operator=(const value_t& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] = x;
    return *this;
  }

   SubArray& operator+=(const SubArray& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] += x.data_[i];
    return *this;
  }
   SubArray& operator-=(const SubArray& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] -= x.data_[i];
    return *this;
  }
   SubArray& operator*=(const SubArray& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] *= x.data_[i];
    return *this;
  }
   SubArray& operator/=(const SubArray& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] /= x.data_[i];
    return *this;
  }

   SubArray& operator+=(const Array<T>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] += x.data_[i];
    return *this;
  }
   SubArray& operator-=(const Array<T>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] -= x.data_[i];
    return *this;
  }
   SubArray& operator*=(const Array<T>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] *= x.data_[i];
    return *this;
  }
   SubArray& operator/=(const Array<T>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] /= x.data_[i];
    return *this;
  }

  template <class Xpr_t>
   SubArray& operator+=(const ArrayXpr<Xpr_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] += x[i];
    return *this;
  }
  template <class Xpr_t>
   SubArray& operator-=(const ArrayXpr<Xpr_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] -= x[i];
    return *this;
  }
  template <class Xpr_t>
   SubArray& operator*=(const ArrayXpr<Xpr_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] *= x[i];
    return *this;
  }
  template <class Xpr_t>
   SubArray& operator/=(const ArrayXpr<Xpr_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] /= x[i];
    return *this;
  }

  template <class A_t>
   SubArray& operator+=(const ArrayBase<A_t, value_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] += x[i];
    return *this;
  }
  template <class A_t>
   SubArray& operator-=(const ArrayBase<A_t, value_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] -= x[i];
    return *this;
  }
  template <class A_t>
   SubArray& operator*=(const ArrayBase<A_t, value_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] *= x[i];
    return *this;
  }
  template <class A_t>
   SubArray& operator/=(const ArrayBase<A_t, value_t>& x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] /= x[i];
    return *this;
  }

   SubArray& operator+=(const value_t x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] += x;
    return *this;
  }
   SubArray& operator-=(const value_t x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] -= x;
    return *this;
  }
   SubArray& operator*=(const value_t x) noexcept {
    for (auto i = 0; i < n; ++i) data_[i] *= x;
    return *this;
  }
   SubArray& operator/=(const value_t x) noexcept {
    value_t xi = value_t(1) / x;
    for (auto i = 0; i < n; ++i) data_[i] *= xi;
    return *this;
  }

   pointer_t begin() noexcept { return data_; }
   const_pointer_t begin() const noexcept { return data_; }
   const_pointer_t end() const noexcept { return (data_ + n); }
   pointer_t rbegin() noexcept { return (data_ + n); }
   const_pointer_t rbegin() const noexcept { return (data_ + n); }
   const_pointer_t rend() const noexcept { return data_; }
   const_pointer_t data() const noexcept { return data_; }
   pointer_t data() noexcept { return data_; }
   bool empty() noexcept { return n == 0; }
   size_type size() const noexcept { return n; }
   difference_type stride() const noexcept {
    return difference_type{1};
  }

   value_t& operator[](size_type i) noexcept { return data_[i]; }
   value_t operator[](size_type i) const noexcept { return data_[i]; }

   value_t dotProduct(const SubArray& a) noexcept {
    value_t sum(0);
    for (auto i = 0; i < n; ++i) sum += data_[i] * a[i];
    return sum;
  }

   value_t max() const noexcept { return *std::max_element(data_, data_ + n); }

   value_t amax() const noexcept {
    return *std::max_element(data_, data_ + n, petlib::abs_cmp<value_t>);
  }

   value_t min() const noexcept { return *std::min_element(data_, data_ + n); }

   value_t amin() const noexcept {
    return *std::min_element(data_, data_ + n, petlib::abs_cmp<value_t>);
  }

   size_t index_max() const noexcept {
    return size_t(std::max_element(data_, data_ + n) - data_);
  }

   size_t index_amax() const noexcept {
    return size_t(std::max_element(data_, data_ + n, petlib::abs_cmp<value_t>) - data_);
  }

   size_t index_min() const noexcept {
    return size_t(std::min_element(data_, data_ + n) - data_);
  }

   size_t index_amin() const noexcept {
    return size_t(std::min_element(data_, data_ + n, petlib::abs_cmp<value_t>) - data_);
  }

  //
  // scale vector so that the dot product with itself = 1 or 0 if array is null
  //

   void normalize() noexcept {
    value_t sum = 0;
    for (auto i = 0; i < n; ++i) sum += data_[i] * data_[i];
    if (sum < petlib::numeric_traits<T>::eps) {
      return;
    }
    sum = std::sqrt(sum);
    sum = value_t(1) / sum;
    for (auto i = 0; i < n; ++i) data_[i] *= sum;
  }

   void swap_elements(size_t i, size_t j) noexcept {
    value_t tmp = data_[i];
    data_[i] = data_[j];
    data_[j] = tmp;
  }

   void rotate_elements(size_t i, size_t j,
                                 const value_t& angle) noexcept {
    value_t tmp = data_[i];
    value_t cs = std::cos(angle);
    value_t sn = std::sin(angle);
    data_[i] = data_[i] * cs + data_[j] * sn;
    data_[j] = data_[j] * cs - tmp * sn;
  }

  template <class A_t>
   void reflect(const ArrayBase<A_t, value_t>& x) noexcept {
    value_t prd = dotProduct(*this, x);
    value_t den = dotProduct(x, x);
    value_t fact = prd / den;
    for (auto i = 0; i < n; ++i) data_[i] = data_[i] - fact * x[i];
  }

   SliceArray<T> operator()(Slice& s) noexcept {
    return SliceArray<T>(data_ + s.offset(), s.size(), s.stride());
  }

   SubArray<T> operator()(Range& r) noexcept {
    return SubArray<T>(data_ + r.offset(), r.size());
  }

 private:
  T* data_;
  size_type n;
};

template <typename T>
class SliceArray : public ArrayBase<SliceArray<T>, T> {
 public:
  typedef T value_t;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef T* pointer_t;
  typedef const T* const_pointer_t;
  typedef T& reference_t;
  typedef const T& const_reference_t;
  typedef SliceIterator<T> iterator_t;
  typedef const SliceIterator<T> const_iterator_t;

  SliceArray(pointer_t p, size_type len, difference_type stride)
      : data_(p), n(len), str(stride) {}

  SliceArray() = delete;

  SliceArray(SliceArray&& a) : data_(a.data_), n(a.n), str(a.str) {
    a.data_ = nullptr;
    a.n = 0;
  }

  SliceArray(const SliceArray& a) : data_(a.data_), n(a.n), str(a.str) {}

  SliceArray(SliceArray& a) : data_(a.data_), n(a.n), str(a.str) {}

  SliceArray(Array<T>& a) : data_(a.data()), n(a.size()), str(1) {}

  SliceArray(SubArray<T>& a) : data_(a.data()), n(a.size()), str(1) {}

  ~SliceArray() {
    data_ = nullptr;
    n = 0;
  }

  ///// various operators
   SliceArray& operator=(SliceArray&& a) noexcept {
    data_ = a.data_;
    a.data_ = nullptr;
    n = a.n;
    a.n = 0;
    str = a.str;
    return *this;
  }

   SliceArray& operator=(const SliceArray& x) noexcept {
    assert(n == x.n);
    data_ = x.data_;
    n = x.n;
    str = x.str;
    return *this;
  }

   SliceArray& operator=(const Array<T>& x) noexcept {
    assert(n == x.n);
    data_ = x.data();
    n = x.size();
    str = difference_type(1);
    return *this;
  }

  template <class A_t, typename other_type>
   SliceArray& operator=(
      const ArrayBase<A_t, other_type>& a) noexcept {
    assert(n == a.size());
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] = a[i];
    }
    return *this;
  }

  template <class Xpr_t>
   SliceArray& operator=(const ArrayXpr<Xpr_t>& a) noexcept {
    assert(n == a.size());
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] = a[i];
    }
    return *this;
  }

   SliceArray& operator=(const value_t& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] = x;
    }
    return *this;  
  }

   SliceArray& operator+=(const SliceArray& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    std::ptrdiff_t iy(0);
    for (;i<n;++i,ix+=str,iy+=x.str) {
      data_[ix] += x.data_[iy];
    }
    return *this;
  }


   SliceArray& operator-=(const SliceArray& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    std::ptrdiff_t iy(0);
    for (;i<n;++i,ix+=str,iy+=x.str) {
      data_[ix] -= x.data_[iy];
    }
    return *this;
  }


   SliceArray& operator*=(const SliceArray& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    std::ptrdiff_t iy(0);
    for (;i<n;++i,ix+=str,iy+=x.str) {
      data_[ix] *= x.data_[iy];
    }
    return *this;
  }


   SliceArray& operator/=(const SliceArray& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    std::ptrdiff_t iy(0);
    for (;i<n;++i,ix+=str,iy+=x.str) {
      data_[ix] /= x.data_[iy];
    }
    return *this;
  }

   SliceArray& operator+=(const Array<T>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] += x[i];
    }
    return *this;
  }
   SliceArray& operator-=(const Array<T>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] -= x[i];
    }
    return *this;
  }
   SliceArray& operator*=(const Array<T>& x) noexcept {
    std::size_t i(0);;
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] *= x[i];
    }
    return *this;
  }
   SliceArray& operator/=(const Array<T>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] /= x[i];
    }
    return *this;
  }

  template <class Xpr_t>
   SliceArray& operator+=(const ArrayXpr<Xpr_t>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) data_[ix] += x[i];
    return *this;
  }
  template <class Xpr_t>
   SliceArray& operator-=(const ArrayXpr<Xpr_t>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) data_[ix] -= x[i];
  }
  template <class Xpr_t>
   SliceArray& operator*=(const ArrayXpr<Xpr_t>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] *= x[i];
    } 
    return *this;
  }
  template <class Xpr_t>
   SliceArray& operator/=(const ArrayXpr<Xpr_t>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] /= x[i];
    }
    return *this;
  }

  template <class A_t>
   SliceArray& operator+=(const ArrayBase<A_t, value_t>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] += x[i];
    }
    return *this;
  }
  template <class A_t>
   SliceArray& operator-=(const ArrayBase<A_t, value_t>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] -= x[i];
    }
    return *this;
  }
  template <class A_t>
   SliceArray& operator*=(const ArrayBase<A_t, value_t>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] *= x[i];
    }
    return *this;
  }
  template <class A_t>
   SliceArray& operator/=(const ArrayBase<A_t, value_t>& x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] /= x[i];
    }
    return *this;
  }

   SliceArray& operator+=(const value_t x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] += x;
    }
    return *this;
  }
   SliceArray& operator-=(const value_t x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] -= x;
    }
    return *this;
  }
   SliceArray& operator*=(const value_t x) noexcept {
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] *= x;
    }
    return *this;
  }
   SliceArray& operator/=(const value_t x) noexcept {
    value_t xi = value_t(1) / x;
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] *= xi;
    }
    return *this;
  }

   iterator_t begin() noexcept { return iterator_t(data_, str); }
   const_iterator_t begin() const noexcept {
    return const_iterator_t(data_, str);
  }
   const_iterator_t end() const noexcept {
    return const_iterator_t(data_ + n * str, str);
  }
   iterator_t rbegin() noexcept { return iterator_t(data_+n*str, str); }
   const_iterator_t rbegin() const noexcept {
    return const_iterator_t(data_+n*str, str);
  }
   const_iterator_t rend() const noexcept {
    return const_iterator_t(data_, str);
  }
    
  const_pointer_t data() const noexcept { return data_; }
  pointer_t data() noexcept { return data_; }

   bool empty() const noexcept { return n == 0; }

   size_type size() const noexcept { return n; }
   difference_type stride() const noexcept { return str; }

   value_t& operator[](size_type i) noexcept { return data_[i * str]; }
   value_t operator[](size_type i) const noexcept {
    return data_[i * str];
  }

   value_t max() const noexcept {
    return *std::max_element(this->begin(), this->end());
  }

   value_t amax() const noexcept {
    return *std::max_element(this->begin(), this->end(), petlib::abs_cmp<value_t>);
  }

   value_t min() const noexcept {
    return *std::min_element(this->begin(), this->end());
  }

   value_t amin() const noexcept {
    return *std::min_element(this->begin(), this->end(), petlib::abs_cmp<value_t>);
  }

   size_t index_max() const noexcept {
    return size_t(std::max_element(this->begin(), this->end()) - data_);
  }

   size_t index_amax() const noexcept {
    return size_t(std::max_element(data_, this->end(), petlib::abs_cmp<value_t>) - data_);
  }

   size_t index_min() const noexcept {
         return size_t(std::min_element(data_,this->end())-data_);
  }

   size_t index_amin() const noexcept {
    return size_t(std::min_element(data_, this->end(), petlib::abs_cmp<value_t>) - data_);
  }

  //
  // scale vector so that the dot product with itself = 1 or 0 if array is null
  //

   void normalize() noexcept {
    value_t sum = dotProduct(*this);
    if (sum < petlib::numeric_traits<T>::eps) {
      return;
    }
    sum = std::sqrt(sum);
    sum = value_t(1) / sum;
    std::size_t i(0);;
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) data_[ix] *= sum;
  }

   void swap_elements(size_t i, size_t j) noexcept {
    std::size_t ix = i * str;
    std::size_t jx = j * str;
    value_t tmp = data_[ix];
    data_[ix] = data_[jx];
    data_[jx] = tmp;
  }

   void rotate_elements(size_t i, size_t j,
                                 const value_t& angle) noexcept {
    std::size_t ix = i * str;
    std::size_t jx = j * str;
    value_t tmp = data_[ix];
    value_t cs = std::cos(angle);
    value_t sn = std::sin(angle);
    data_[ix] = data_[ix] * cs + data_[jx] * sn;
    data_[jx] = data_[jx] * cs - tmp * sn;
  }

  template <class A_t>
   void reflect(const ArrayBase<A_t, value_t>& x) noexcept {
    value_t prd = dotProduct(*this, x);
    value_t den = dotProduct(x, x);
    value_t fact = prd / den;
    std::size_t i(0);
    std::ptrdiff_t ix(0);
    for (;i<n;++i,ix+=str) {
      data_[ix] = data_[ix] - fact *x[i];
    }
  }

   SliceArray<T> operator()(Slice& s) noexcept {
    return SliceArray<T>(data_ + s.offset() * str, s.size(), s.stride() * str);
  }

   SliceArray<T> operator()(Range& r) noexcept {
    return SliceArray<T>(data_ + str * r.offset(), r.size(), str);
  }

 private:
  T* data_;
  size_type n;
  difference_type str;
};

template <class A_t, class B_t, typename x_t, typename y_t>
 typename promote_traits<x_t, y_t>::promote_t dotProduct(
    const ArrayBase<A_t, x_t>& a, const ArrayBase<B_t, y_t>& b) {
  typedef typename promote_traits<x_t, y_t>::promote_t value_t;
  value_t sum(0);
  for (std::size_t i = std::size_t(0); i < a.size(); ++i) sum += a[i] * b[i];
  return sum;
}

}  // namespace petlib

#endif