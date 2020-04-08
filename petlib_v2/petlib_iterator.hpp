#ifndef PETLIB_ITERATOR_HPP
#define PETLIB_ITERATOR_HPP
#include <cstdlib>
#include <iterator>
#include <limits>
#include <cmath>

namespace petlib {

template < class Tp > inline Tp Abs( const Tp& x) noexcept {
  return (x >= Tp(0))? x:-x;
}

template <> inline unsigned char Abs< unsigned char >(const unsigned char& x) noexcept { return x;}
template <> inline unsigned short Abs< unsigned short >(const unsigned short& x) noexcept { return x;}
template <> inline unsigned int Abs< unsigned int >(const unsigned int& x) noexcept { return x;}
template <> inline unsigned long Abs< unsigned long >(const unsigned long& x) noexcept { return x;}
template <> inline unsigned long long Abs< unsigned long long >(const unsigned long long& x) noexcept { return x;}
template <> inline float Abs< float >(const float& x) noexcept { return ::fabs(x);}
template <> inline double Abs< double >(const double& x) noexcept { return ::fabs(x);}
template <> inline long double Abs< long double >(const long double& x) noexcept { return ::fabsl(x);}
template <> inline  int Abs<  int >(const  int& x) noexcept { return abs(x);}
template <> inline  long Abs<  long >(const  long& x) noexcept { return labs(x);}
template <> inline  long long Abs<  long long >(const  long long& x) noexcept { return llabs(x);}


template < class dst_t, class src_t > inline void Copy( dst_t dst, dst_t last, src_t src) {
    for (;dst!=last;++dst,++src) *dst = *src;
}

template < class dst_t, class src_t > inline void Copy( dst_t dst, src_t src, std::size_t cnt) {
    for (std::size_t i=0;i<cnt;++i,++dst,++src) *dst = *src;
}

template < class dst_t, class src_t > inline void CopyRev( dst_t dst, dst_t last, src_t src) {
    for (;dst!=last;--dst,--src) *dst = *src;
}

template < class dst_t, class src_t > inline void CopyRev( dst_t dst, src_t src, std::size_t cnt) {
    for (std::size_t i=0;i<cnt;++i,--dst,--src) *dst = *src;
}

#define PETLIB_EXPR_OP(OP,SYM)\
template < class dst_t, class src_t > inline void OP ## Copy( dst_t dst, dst_t last, src_t src) {\
    for (;dst!=last;++dst,++src) *dst SYM *src;\
} \
  \
template < class dst_t, class src_t > inline void OP ## Copy( dst_t dst, src_t src, std::size_t cnt) {\
    for (std::size_t i=0;i<cnt;++i,++dst,++src) *dst SYM *src;\
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

template < class dst_t, class src_t > inline void Fill( dst_t dst, dst_t last, const src_t& src) {
    for (;dst!=last;++dst) *dst = src;
}

template < class dst_t, class src_t > inline void Fill( dst_t dst, const src_t& src, std::size_t cnt) {
    for (std::size_t i=0;i<cnt;++i,++dst) *dst = src;
}

template < class dst_t, class src_t > inline void FillRev( dst_t dst, dst_t last, const src_t& src) {
    for (;dst!=last;--dst) *dst = src;
}

template < class dst_t, class src_t > inline void FillRev( dst_t dst, const src_t& src, std::size_t cnt) {
    for (std::size_t i=0;i<cnt;++i,--dst) *dst = src;
}

#define PETLIB_EXPR_OP(OP,SYM)\
template < class dst_t, class src_t > inline void OP ## Fill( dst_t dst, dst_t last, const src_t& src) {\
    for (;dst!=last;++dst) *dst SYM src;\
} \
  \
template < class dst_t, class src_t > inline void OP ## Fill( dst_t dst, const src_t& src, std::size_t cnt) {\
    for (std::size_t i=0;i<cnt;++i,++dst) *dst SYM src;\
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

template < class Tp, class iter_t >
inline Tp MaxValue( iter_t iter, iter_t last) {
    Tp vmax( std::numeric_limits< Tp >::min());
    for (;iter!=last;++iter) {
        if (vmax < *iter) vmax = *iter;
    }
    return vmax;
}

template < class Tp, class iter_t >
inline Tp MinValue( iter_t iter, iter_t last) {
    Tp vmin( std::numeric_limits< Tp >::max());
    for (;iter!=last;++iter) {
        if (vmin > *iter) vmin = *iter;
    }
    return vmin;
}

template < class Tp, class iter_t >
inline Tp AmaxValue( iter_t iter, iter_t last) {
    Tp vmax(0);
    for (;iter!=last;++iter) {
        Tp t = Abs<Tp>(*iter);
        if (vmax < t) vmax = t;
    }
    return vmax;
}

template < class Tp, class iter_t >
inline Tp AminValue( iter_t iter, iter_t last) {
    Tp vmin( std::numeric_limits< Tp >::max());
    for (;iter!=last;++iter) {
        Tp t = Abs<Tp>(*iter);
        if (vmin > t) vmin = t;
    }
    return vmin;
}

template < class Tp, class iter_t >
inline std::size_t MaxIndex( iter_t iter, iter_t last) {
    Tp vmax( std::numeric_limits< Tp >::min());
    std::size_t imax = 0;
    for (std::size_t ix=0;iter!=last;++iter,++ix) {
        if (vmax < *iter) {
            vmax = *iter;
            imax = ix;
        }
    }
    return imax;
}

template < class Tp, class iter_t >
inline std::size_t MinIndex( iter_t iter, iter_t last) {
    Tp vmin( std::numeric_limits< Tp >::max());
    std::size_t imin = 0;
    for (std::size_t ix=0;iter!=last;++iter,++ix) {
        if (vmin < *iter) {
            vmin = *iter;
            imin = ix;
        }
    }
    return imin;
}

template < class Tp, class iter_t >
inline std::size_t AmaxIndex( iter_t iter, iter_t last) {
    Tp vmax(0);
    std::size_t imax = 0;
    for (std::size_t ix=0;iter!=last;++ix,++iter) {
        Tp t = Abs<Tp>(*iter);
        if (vmax < t) {
            vmax = t;
            imax = ix;
        }
    }
    return imax;
}

template < class Tp, class iter_t >
inline std::size_t AminIndex( iter_t iter, iter_t last) {
    Tp vmin( std::numeric_limits< Tp >::max());
    std::size_t imin = 0;
    for (std::size_t ix=0;iter!=last;++ix,++iter) {
        Tp t = Abs<Tp>(*iter);
        if (vmin > t) {
            vmin = t;
            imin = ix;
        }
    }
    return imin;
}

template < class Tp, class iter_t >
inline Tp MaxValue( iter_t iter, std::size_t cnt) {
    Tp vmax( std::numeric_limits< Tp >::min());
    for (std::size_t ix=0;ix<cnt;++ix,++iter) {
        if (vmax < *iter) vmax = *iter;
    }
    return vmax;
}

template < class Tp, class iter_t >
inline Tp MinValue( iter_t iter, std::size_t cnt) {
    Tp vmin( std::numeric_limits< Tp >::max());
    for (std::size_t ix=0;ix<cnt;++ix,++iter) {
        if (vmin > *iter) vmin = *iter;
    }
    return vmin;
}

template < class Tp, class iter_t >
inline Tp AmaxValue( iter_t iter, std::size_t cnt) {
    Tp vmax(0);
    for (std::size_t ix=0;ix<cnt;++ix,++iter) {
        Tp t = Abs<Tp>(*iter);
        if (vmax < t) vmax = t;
    }
    return vmax;
}

template < class Tp, class iter_t >
inline Tp AminValue( iter_t iter, std::size_t cnt) {
    Tp vmin( std::numeric_limits< Tp >::max());
    for (std::size_t ix=0;ix<cnt;++ix,++iter) {
        Tp t = Abs<Tp>(*iter);
        if (vmin > t) vmin = t;
    }
    return vmin;
}

template < class Tp, class iter_t >
inline std::size_t MaxIndex( iter_t iter, std::size_t cnt) {
    Tp vmax( std::numeric_limits< Tp >::min());
    std::size_t imax = 0;
    for (std::size_t ix=0;ix<cnt;++ix,++iter) {
        if (vmax < *iter) {
            vmax = *iter;
            imax = ix;
        }
    }
    return imax;
}

template < class Tp, class iter_t >
inline std::size_t MinIndex( iter_t iter, std::size_t cnt) {
    Tp vmin( std::numeric_limits< Tp >::max());
    std::size_t imin = 0;
    for (std::size_t ix=0;ix<cnt;++ix,++iter) {
        if (vmin < *iter) {
            vmin = *iter;
            imin = ix;
        }
    }
    return imin;
}

template < class Tp, class iter_t >
inline std::size_t AmaxIndex( iter_t iter, std::size_t cnt) {
    Tp vmax(0);
    std::size_t imax = 0;
    for (std::size_t ix=0;ix<cnt;++ix,++iter) {
        Tp t = Abs<Tp>(*iter);
        if (vmax < t) {
            vmax = t;
            imax = ix;
        }
    }
    return imax;
}

template < class Tp, class iter_t >
inline std::size_t AminIndex( iter_t iter, std::size_t cnt) {
    Tp vmin( std::numeric_limits< Tp >::max());
    std::size_t imin = 0;
    for (std::size_t ix=0;ix<cnt;++ix,++iter) {
        Tp t = Abs<Tp>(*iter);
        if (vmin > t) {
            vmin = t;
            imin = ix;
        }
    }
    return imin;
}

template < class Tp , class iter_t > inline Tp Norm2(iter_t iter, iter_t last) {
    Tp s(0);
    for (;iter!=last;++iter) s += *iter * *iter;
    return s;
} 

template < class Tp , class iter_t > inline Tp Norm2(iter_t iter, std::size_t cnt) {
    Tp s(0);
    for (std::size_t ix=0;ix<cnt;++ix,++iter) s += *iter * *iter;
    return s;
} 

template < class Tp , class iter_t > inline Tp Asum(iter_t iter, iter_t last) {
    Tp s(0);
    for (;iter!=last;++iter) s += Abs<Tp>(*iter);
    return s;
} 

template < class Tp , class iter_t > inline Tp Asum(iter_t iter, std::size_t cnt) {
    Tp s(0);
    for (std::size_t ix=0;ix<cnt;++ix,++iter) s += Abs<Tp>(*iter);
    return s;
} 

template < class Tp, class iter1_t, class iter2_t > inline void Dot(iter1_t iter1, iter1_t last, iter2_t iter2) {
    Tp s(0);
    for (;iter1!=last;++iter1,++iter2) {
        s += *iter1 * *iter2;
    }
    return s;
} 

template < class Tp, class iter1_t, class iter2_t > inline void Dot(iter1_t iter1, iter2_t iter2, std::size_t cnt) {
    Tp s(0);
    for (std::size_t ix=0;ix<cnt;++ix,++iter1,++iter2) {
        s += *iter1 * iter2;
    }
    return s;
} 

template < class iter1_t, class iter2_t > inline void Swap(iter1_t iter1, iter1_t last, iter2_t iter2) {
    for (;iter1!=last;++iter1,++iter2) {
        auto t = *iter1;
        *iter1 = *iter2;
        *iter2 = t;
    }
} 

template < class iter1_t, class iter2_t > inline void Swap(iter1_t iter1, iter2_t iter2, std::size_t cnt) {
    for (std::size_t ix=0;ix<cnt;++ix,++iter1,++iter2) {
        auto t = *iter1;
        *iter1 = *iter2;
        *iter2 = t;
    }
} 


template < typename Tp, class iter1_t, class iter2_t > 
inline void Rot(iter1_t iter1, iter1_t last, iter2_t iter2, const Tp& cs, const Tp& sn) {
    for (;iter1!=last;++iter1,++iter2) {
        auto t = *iter1;
        *iter1 = cs* t + sn* *iter2;
        *iter2 = cs* iter2 - sn*t;
    }
} 

template < typename Tp, class iter1_t, class iter2_t > 
inline void Rot(iter1_t iter1, iter2_t iter2, const Tp& cs, const Tp& sn,std::size_t cnt) {
    for (std::size_t ix=0;ix<cnt;++ix,++iter1,++iter2) {
        auto t = *iter1;
        *iter1 = cs* t + sn* *iter2;
        *iter2 = cs* iter2 - sn*t;
    }
} 

template < typename Tp, class iter1_t, class iter2_t > 
inline void Axpy(iter1_t iter1, iter1_t last, iter2_t iter2, const Tp& cs) {
    for (;iter1!=last;++iter1,++iter2) {
        *iter1 += cs* *iter2;
    }
} 

template < typename Tp, class iter1_t, class iter2_t > 
inline void Axpy(iter1_t iter1, iter2_t iter2, const Tp& cs, std::size_t cnt) {
    for (std::size_t ix=0;ix<cnt;++ix,++iter1,++iter2) {
        *iter1 += cs* *iter2;
    }
} 


template < class Tp, class iter1_t , class iter2_t > inline void MakeOrtho(
iter1_t *iter1,iter2_t *iter2,std::size_t cnt) {
    Tp s = Dot<Tp,iter1_t,iter2_t>(iter1,iter2,cnt);
    Tp t = Norm2<Tp,iter2_t>(iter2,cnt);
    Tp f = -s/t;
    AddCopy<Tp,iter1,iter2>(iter1,iter2,f,cnt);
}

template < class Tp, class iter1_t , class iter2_t > inline void Normalize(
iter1_t *iter1,std::size_t cnt) {
    Tp t = Norm2<Tp,iter1_t>(iter1,cnt);
    Tp f = -Tp(1)/sqrt(t);
    MulFill< iter1_t, Tp >(iter1,f,cnt);
}

struct Slice {
    typedef std::size_t size_type;

    size_type m_off;
    size_type m_size;
    size_type m_str;
    
    constexpr Slice():m_off(0),m_size(0),m_str(0) {}
    Slice(const Slice&) = default;
    Slice(Slice&&) = default;  
    explicit Slice( size_type offset_, size_type size_, size_type stride_):m_off(offset_),
      m_size(size_),m_str(stride_) {}
    
    size_type offset() const noexcept { return m_off;}
    size_type size() const noexcept { return m_size;}
    size_type stride() const noexcept { return m_str;}
    
    constexpr bool IsValid(size_type sz) const noexcept {
       const size_type new_end = m_off + m_str*m_size;     
      if (new_end <= sz) return true;
       return false;
    }
    
    Slice CreateSlice( const Slice& s) const noexcept {
        return Slice(m_off+m_str*s.offset(),s.size(),s.stride()*m_str);
    }

};

struct Range {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    size_type m_off;
    size_type m_size;
    
    Range()=default;
    
    explicit Range( size_type offset_, size_type size_):m_off(offset_),m_size(size_) {}
    
    constexpr size_type offset() const noexcept { return m_off;}
    constexpr size_type size() const noexcept { return m_size;}
    
    constexpr bool IsValid(const size_type sz) const noexcept {
        size_type new_end = m_off + m_size;     
        if (new_end <= sz) return true;
        return false;
    }

    Range CreateRange( const Range& r ) const noexcept {
        return Range(m_off+r.m_off,r.size());
    }

};


template <typename T>
struct SliceIterator {
  typedef std::ptrdiff_t difference_type;
  typedef std::size_t size_type;
  typedef T value_type;
  typedef T value_t;
  typedef T *pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef std::random_access_iterator_tag iterator_category;

  pointer m_ptr;
  size_type m_str;
  
  constexpr SliceIterator() = default;
  constexpr SliceIterator(const SliceIterator &&s) : m_ptr(s.m_ptr), m_str(s.m_str){};
  constexpr explicit SliceIterator(pointer ptr, size_type Stride) : m_ptr(ptr), m_str(Stride) {}
  constexpr SliceIterator(SliceIterator &&s) : m_ptr(s.m_ptr), m_str(s.m_str) {}
  
  ~SliceIterator() { m_ptr = nullptr; m_str=0; }
  
  constexpr SliceIterator& operator=(const SliceIterator &s) noexcept {
    m_ptr = s.m_ptr;
    m_str = s.m_str;
    return *this;
  };
  constexpr SliceIterator& operator=(SliceIterator &&s) noexcept {
    m_ptr = s.m_ptr;
    m_str = s.m_str;
    return *this;
  };

  constexpr SliceIterator& operator++() noexcept {
    m_ptr += m_str;
    return *this;
  }
  constexpr SliceIterator& operator++(int) noexcept {
    SliceIterator<T> clone(m_ptr, m_str);
    m_ptr += m_str;
    return clone;
  }
  constexpr SliceIterator& operator--() noexcept {
    m_ptr -= m_str;
    return *this;
  }
  constexpr SliceIterator& operator--(int) noexcept {
    SliceIterator<T> clone(m_ptr, m_str);
    m_ptr -= m_str;
    return clone;
  }

  constexpr bool empty() noexcept { return m_ptr==nullptr;}
  constexpr pointer data() noexcept { return m_ptr;}
  constexpr const_pointer data() const noexcept { return m_ptr;}
  constexpr reference operator*() noexcept { return *m_ptr; }
  constexpr const_reference operator*() const noexcept { return *m_ptr; }
  constexpr pointer operator->() noexcept { return m_ptr; }
  constexpr const pointer operator->() const noexcept { return &*m_ptr; }
  constexpr SliceIterator& operator+=(difference_type n) noexcept {
    m_ptr += n * difference_type(m_str);
    return *this;
  };
  constexpr SliceIterator& operator-=(difference_type n) noexcept {
    m_ptr -= n * difference_type(m_str);
    return  *this;
  }
  constexpr reference operator[](size_type i) noexcept {
    return m_ptr[i * m_str];
  }
  constexpr const_reference operator[](size_type i) const noexcept {
    return m_ptr[i * m_str];
  }

  constexpr size_type stride() const noexcept { return m_str; }

  constexpr SliceIterator Plus(difference_type n) const noexcept {
    return SliceIterator(m_ptr + n * difference_type(m_str), m_str);
  };
  constexpr SliceIterator Minus(difference_type n) const noexcept {
    return SliceIterator(m_ptr - n * difference_type(m_str), m_str);
  }
  constexpr difference_type Distance(const SliceIterator& s) const noexcept { 
      if (m_str && m_str==s.m_str) {
          return ((m_ptr-s.m_ptr)/m_str);
      }
      return 1<<sizeof(difference_type)*8;
  }
  constexpr bool Equals(const SliceIterator<T> &s) const noexcept {
    return m_ptr == s.m_ptr;
  }
  constexpr bool NotEquals(const SliceIterator<T> &s) const noexcept {
    return m_ptr != s.m_ptr;
  }
  constexpr bool LessThanEquals(const SliceIterator<T> &s) const noexcept {
    return m_ptr <= s.m_ptr;
  }
  constexpr bool GreaterThanEquals(const SliceIterator<T> &s) const noexcept {
    return m_ptr >= s.m_ptr;
  }
  constexpr bool LessThan(const SliceIterator<T> &s) const noexcept {
    return m_ptr < s.m_ptr;
  }
  constexpr bool GreaterThan(const SliceIterator<T> &s) const noexcept {
    return m_ptr > s.m_ptr;
  }
};

template <typename T>
constexpr SliceIterator<T> operator+(const SliceIterator<T> &x,
                                     std::ptrdiff_t n) noexcept {
  return x.Plus(n);
}

template <typename T>
constexpr SliceIterator<T> operator-(const SliceIterator<T> &x,
                                     std::ptrdiff_t n) noexcept {
  return x.Minus(n);
}

template <typename T>
constexpr SliceIterator<T> operator+(std::ptrdiff_t n,
                                     const SliceIterator<T> &x) noexcept {
  return x.Plus(n);
}
    
template < typename T >
constexpr bool operator==(const SliceIterator<T>& x,const SliceIterator<T>& y) noexcept {
    return x.Equals(y);
}

template < typename T >
constexpr bool operator!=(const SliceIterator<T>& x,const SliceIterator<T>& y) noexcept {
    return x.NotEquals(y);
}

template < typename T >
constexpr bool operator<=(const SliceIterator<T>& x,const SliceIterator<T>& y) noexcept {
    return x.LessThanEquals(y);
}

template < typename T >
constexpr bool operator<(const SliceIterator<T>& x,const SliceIterator<T>& y) noexcept {
    return x.LessThan(y);
}

template < typename T >
constexpr bool operator>=(const SliceIterator<T>& x,const SliceIterator<T>& y) noexcept {
    return x.GreaterThanEquals(y);
}

template < typename T >
constexpr bool operator>(const SliceIterator<T>& x,const SliceIterator<T>& y) noexcept {
    return x.GreaterThan(y);
}

}
#endif