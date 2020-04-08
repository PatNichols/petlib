#ifndef PUTILS_ITER_HPP
#define PUTILS_ITER_HPP
#include <cstdlib>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>

namespace putils {

constexpr auto default_size = 32;

/////
// Memmove like iterator functions
/////
template <class iter>
iter move_forward(iter first, iter last, std::size_t count) {
  iter p = last;
  iter pdst = last + count;
  iter pend = first + count - 1;
  while (pdst != pend) {
    *pdst = *p;
    --p;
    --pdst;
  }
  return pdst;
}

template <class iter>
iter move_forward(iter first, iter last) {
  iter p = last;
  iter pdst = last + 1;
  while (pdst != first) {
    *pdst = *p;
    --p;
    --pdst;
  }
  return pdst;
}

template <class iter>
iter move_back(iter first, iter last, std::size_t count) {
  iter psrc = first + count;
  iter pdst = first;
  while (psrc != last) {
    *pdst = *psrc;
    ++pdst;
    ++psrc;
  }
  return pdst;
}

template <class dst_iterator, typename T>
dst_iterator fill(dst_iterator iter, dst_iterator last, const T& v) {
  while (iter != last) {
    *iter = v;
    ++iter;
  }
  return iter;
}

template <class dst_iterator, typename T>
dst_iterator fill(dst_iterator iter, dst_iterator last, const T&& v) {
  T x{v};
  while (iter != last) {
    *iter = T{x};
    ++iter;
  }
  return iter;
}

/////
// memcpy like functions
////
template <class dst_iterator, class src_iterator>
dst_iterator copy(dst_iterator dst, dst_iterator dst_last, src_iterator src) {
  while (dst != dst_last) {
    *dst = *src;
    ++src;
    ++dst;
  }
  return dst;
}

template <class dst_iterator, class src_iterator>
dst_iterator copy2(dst_iterator dst, src_iterator src, std::size_t cnt) {
  for (std::size_t k = 0; k < cnt; ++k, ++dst, ++src) {
    *dst = *src;
  }
  return dst;
}

template <class iter>
constexpr bool bool_cmp(iter x, iter xlast, iter y) {
  while (x != xlast) {
    if (*x != *y) return false;
    ++x;
    ++y;
  }
  return true;
}

template <class iter>
constexpr bool bool_abs_cmp(iter x, iter xlast, iter y) {
  bool abs_ = [](auto x) { return (x > 0) ? x : -x; };
  while (x != xlast) {
    if (abs_(*x) != abs_(*y)) return false;
    ++x;
    ++y;
  }
  return true;
}

#define PUTILS_MAKE_OP(name_,sym_) \
template < class iter_t1, class iter_t2 > \
void name_##EqIters(iter_t1 l_iter,iter_t1 l_last,iter_t2 r_iter) {\
    while (l_iter!=l_last) { \
        *l_iter sym_ *r_iter; \
        ++l_iter; \
        ++r_iter; \
    } \
}  

PUTILS_MAKE_OP(Add,+=)
PUTILS_MAKE_OP(Sub,-=)
PUTILS_MAKE_OP(Mul,*=)
PUTILS_MAKE_OP(Div,/=)
PUTILS_MAKE_OP(Xor,^=)
PUTILS_MAKE_OP(Ior,|=)
PUTILS_MAKE_OP(And,&=)
PUTILS_MAKE_OP(Lsh,<<=)
PUTILS_MAKE_OP(Rsh,>>=)
PUTILS_MAKE_OP(Cmp,<=>)
#undef PUTILS_MAKE_OP

#define PUTILS_MAKE_OP(name_,sym_) \
template < class iter_t1, class iter_t2, class iter_t3 > \
void name_##EqIters(iter_t1 l_iter,iter_t1 l_last,iter_t2 r_iter,iter_t3 result) {\
    while (l_iter!=l_last) { \
        *result = *l_iter sym_ *r_iter; \
        ++l_iter; \
        ++r_iter; \
        ++result; \
    } \
}  

PUTILS_MAKE_OP(Add,+)
PUTILS_MAKE_OP(Sub,-)
PUTILS_MAKE_OP(Mul,*)
PUTILS_MAKE_OP(Div,/)
PUTILS_MAKE_OP(Xor,^)
PUTILS_MAKE_OP(Ior,|)
PUTILS_MAKE_OP(And,&)
PUTILS_MAKE_OP(Lsh,<<)
PUTILS_MAKE_OP(Rsh,>>)
PUTILS_MAKE_OP(Cmp,<=>)
PUTILS_MAKE_OP(LogAnd,&&)
PUTILS_MAKE_OP(LogOr,||)
PUTILS_MAKE_OP(Lt,<)
PUTILS_MAKE_OP(Gt,>)
PUTILS_MAKE_OP(LtEq,<=)
PUTILS_MAKE_OP(GtEq,>=)
PUTILS_MAKE_OP(LogEq,=)
PUTILS_MAKE_OP(LogNEq,!=)
#undef PUTILS_MAKE_OP

template < typename iter_t >
void ComIters( iter_t iter, iter_t last, iter_t res_iter) {
    while (iter!=last) {
        *res_iter = ~*iter;
        ++iter;
        ++res_iter;
    }
}

template < typename T > struct stride_iterator {
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;
    typedef std::random_access_iterator_tag iterator_category;
    
    pointer p;
    difference_type str;
    
    constexpr stride_iterator& operator++() noexcept {
        p += str;
        return *this;
    }
    constexpr stride_iterator& operator++(int) noexcept {
        stride_iterator tmp = *this;
        p += str;
        return tmp;
    }
    constexpr stride_iterator& operator+=(difference_type n) noexcept {
        p += str*n;
        return *this;
    }
    constexpr stride_iterator& operator--() noexcept {
        p -= str;
        return *this;
    }
    constexpr stride_iterator& operator--(int) noexcept {
        stride_iterator tmp = *this;
        p -= str;
        return tmp;
    }
    constexpr stride_iterator& operator-=(difference_type n) noexcept {
        p -= str*n;
        return *this;
    }
    constexpr pointer operator->() noexcept { return p;}
    constexpr reference operator*() noexcept { return *p;}
    constexpr rerefence operator[](size_type n) noexcept { return p[n*str];}
    constexpr const_reference operator[](size_type n) const noexcept { return p[n*str];}
    constexpr void swap(stride_iterator& s) {
        pointer tmp_p(this->p);
        difference_type tmp_str(this->str);
        this->p = s.p;
        this->str = s.str;
        s.p = tmp_p;
        s.str = tmp_str;
    }
    constexpr difference_type stride() const noexcept { return str;}
    constexpr const_pointer data() const noexcept { return p;}
    constexpr pointer data() noexcept { return p;}
};

template < typename T >
constexpr bool operator==(stride_iterator<T>& x,stride_iterator<T>& y) {
    if (x.stride()!=y.stride() || x.data()!=y.data()) return false; 
    return true;
}

template < typename T >
constexpr bool operator!=(stride_iterator<T>& x,stride_iterator<T>& y) {
    if (x.stride()==y.stride() && x.data()==y.data()) return false; 
    return true;
}

template < typename T >
constexpr stride_iterator<T> operator+(stride_iterator<T>& x,difference_type n)
{
    return stride_iterator<T>(x+=n);
}

template < typename T >
constexpr stride_iterator<T> operator-(stride_iterator<T>& x,difference_type n)
{
    return stride_iterator<T>(x-=n);
}

template < typename T >
constexpr stride_iterator<T> operator+(difference_type n,stride_iterator<T>& x)
{
    return stride_iterator<T>(x+=n);
}

template < typename T >
constexpr stride_iterator<T> operator-(difference_type n,stride_iterator<T>& x)
{
    return stride_iterator<T>(x-=n);
}

template < typename T >
constexpr difference_type operator-(const stride_iterator<T>& x,const stride_iterator<T>& y)
{
    difference_type n=0;
    if (x.stride() == y.stride()) {
        stride_iterator<T> xp=x;
        stride_iterator<T> yp=y;
        if ( *xp < *yp) {
            while ( xp < yp) { ++xp; ++n };
            n = -n;
        }else{
            while ( yp < xp) { ++yp, ++n };
        }
        return n;
    }
    return std::numeric_limits<difference_type>::max();
}

template < typename T >
constexpr difference_type raw_distance(const stride_iterator<T>& x,const stride_iterator<T>& y)
{
    if (x.stride()!=y.stride()) return std::numeric_limits<difference_type>::max();
    return (x.data()-y.data());
}

template < typename T > struct mask_iterator {
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;
    typedef std::random_access_iterator_tag iterator_category;
    
    pointer p;
    const vector<bool>::iterator * m_start;
    const vector<bool>::iterator * m_curr;
    const vector<bool>::iterator * m_last;
    vector<bool> mask_vec;
    
    void assign_mask(const vector<bool>& vin) {
        m_start = vin.begin();
        m_curr = vin.begin();
        m_last = vin.end();
    }
    
    pointer advance() {
        while (m_curr!=m_last && !*m_curr) {
            ++p;
            ++mcurr;
        }
        return p;
    }
    pointer stepback() {
        while (m_curr>=m_start && !*m_curr) {
            --p;
            --mcurr;
        }
        return p;
    }
    pointer advance_n(difference_type n) {
        for (auto i=0;i<n;++i) p = advance();
        return p;
    }
    pointer stepback_n(difference_type n) {
        for (auto i=0;i<n;++i) p = stepback();
        return p;
    }
    
    mask_iterator():p(nullptr),m_first(nullptr),m_curr(nullptr),m_last(nullptr),mask_vec() {}
    mask_iterator(pointer ptr,const vector<bool>& v):p(ptr),m_first(v.begin()),
        m_curr(v.begin()),m_last(v.end()),mask_vec(v) {
    }
    mask_iterator(const mask_iterator<T>& r):p(r.p),m_first(r.first),
        m_curr(r.curr),m_last(r.m_last),mask_vec(r.mask_vec)
    {
    }     

    template < iter_t >
    mask_iterator( pointer ptr, pointer p_last, bool fun(const typename iter_t::value_type)) {
        difference_type n=p_last-ptr;
        p = ptr;
        mask_vec.reserve(n);
        while (ptr!=p_last) {
            bool b = fun(*ptr); 
            mask_vec.push_back(b); 
            ++ptr;
        }
        m_first = mask_vec.begin();
        m_curr = mask_vec.begin();
        m_last = mask_vec.end();
    }

    constexpr mask_iterator& operator++() noexcept {
        p = advance();
        return *this;
    }
    constexpr mask_iterator& operator++(int) noexcept {
        mask_iterator tmp = *this;
        p = advance();
        return tmp;
    }
    constexpr mask_iterator& operator+=(difference_type n) noexcept {
        for (auto i=0;i<n;++i) {
            p = advance();
        }
        return *this;
    }
    constexpr mask_iterator& operator--() noexcept {
        p = stepback();
        return *this;
    }
    constexpr mask_iterator& operator--(int) noexcept {
        mask_iterator tmp = *this;
        p = stepback();
        return tmp;
    }
    constexpr mask_iterator& operator-=(difference_type n) noexcept {
        for (auto i=0;i<n;++i) {
            p = stepback();
        }
        return *this;
    }
    constexpr pointer operator->() noexcept { return p;}
    constexpr reference operator*() noexcept { return *p;}
    constexpr rerefence operator[](size_type n) noexcept {
        for (auto i=0;i<n;++i) p = advance(n); 
        return *p;
    }
    constexpr const_reference operator[](size_type n) const noexcept { 
        for (auto i=0;i<n;++i) p = advance(n); 
        return *p;
    }
    constexpr void swap(mask_iterator& s) {
        pointer tmp_p(this->p);
        bool * tmp_m_first(this->m_first);
        bool * tmp_m_curr(this->m_curr);
        bool * tmp_m_last(this->m_last);
        vector<bool>::swap(this->mask_vec,s.mask_vec);
        this->p = s.p;
        this->m_first = s.m_first;
        this->m_curr = s.m_curr;
        this->m_last = s.m_last;
        s.p = tmp_p;
        s.m_first = tmp_m_first;
        s.m_curr = tmp_m_curr;
        s.m_last = tmp_m_last;
    }
    constexpr vector<bool> mask() const noexcept { return mask_vec;}
    constexpr const_pointer data() const noexcept { return p;}
    constexpr pointer data() noexcept { return p;}
    
    bool IsEqual(const mask_iterator& m) const noexcept {
        if (m.p == p ) return true;
        if (m.m_first == m_first) return true; 
        return false; 
    }
};

template < typename T >
constexpr bool operator==(mask_iterator<T>& x,mask_iterator<T>& y) {
    return x.IsEqual(y);
}

template < typename T >
constexpr bool operator!=(mask_iterator<T>& x,mask_iterator<T>& y) {
    !return x.IsEqual(y)
}

template < typename T >
constexpr mask_iterator<T> operator+(mask_iterator<T>& x,difference_type n)
{
    return mask_iterator<T>(x+=n);
}

template < typename T >
constexpr mask_iterator<T> operator-(mask_iterator<T>& x,difference_type n)
{
    return mask_iterator<T>(x-=n);
}

template < typename T >
constexpr mask_iterator<T> operator+(difference_type n,mask_iterator<T>& x)
{
    return mask_iterator<T>(x+=n);
}

template < typename T >
constexpr mask_iterator<T> operator-(difference_type n,mask_iterator<T>& x)
{
    return mask_iterator<T>(x-=n);
}

}  // namespace putils
#endif