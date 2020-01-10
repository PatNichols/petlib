// -*- C++ -*-
//===-------------------------- valarray ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_VALARRAY
#define _LIBCPP_VALARRAY

/*
    valarray synopsis

namespace std
{

template<class T>
class valarray
{
public:
    typedef T value_type;

    // construct/destroy:
    valarray();
    explicit valarray(size_t n);
    valarray(const value_type& x, size_t n);
    valarray(const value_type* px, size_t n);
    valarray(const valarray& v);
    valarray(valarray&& v) noexcept;
    valarray(const slice_array<value_type>& sa);
    valarray(const gslice_array<value_type>& ga);
    valarray(const mask_array<value_type>& ma);
    valarray(const indirect_array<value_type>& ia);
    valarray(initializer_list<value_type> il);
    ~valarray();

    // assignment:
    valarray& operator=(const valarray& v);
    valarray& operator=(valarray&& v) noexcept;
    valarray& operator=(initializer_list<value_type> il);
    valarray& operator=(const value_type& x);
    valarray& operator=(const slice_array<value_type>& sa);
    valarray& operator=(const gslice_array<value_type>& ga);
    valarray& operator=(const mask_array<value_type>& ma);
    valarray& operator=(const indirect_array<value_type>& ia);

    // element access:
    const value_type& operator[](size_t i) const;
    value_type&       operator[](size_t i);

    // subset operations:
    valarray                   operator[](slice s) const;
    slice_array<value_type>    operator[](slice s);
    valarray                   operator[](const gslice& gs) const;
    gslice_array<value_type>   operator[](const gslice& gs);
    valarray                   operator[](const valarray<bool>& vb) const;
    mask_array<value_type>     operator[](const valarray<bool>& vb);
    valarray                   operator[](const valarray<size_t>& vs) const;
    indirect_array<value_type> operator[](const valarray<size_t>& vs);

    // unary operators:
    valarray       operator+() const;
    valarray       operator-() const;
    valarray       operator~() const;
    valarray<bool> operator!() const;

    // computed assignment:
    valarray& operator*= (const value_type& x);
    valarray& operator/= (const value_type& x);
    valarray& operator%= (const value_type& x);
    valarray& operator+= (const value_type& x);
    valarray& operator-= (const value_type& x);
    valarray& operator^= (const value_type& x);
    valarray& operator&= (const value_type& x);
    valarray& operator|= (const value_type& x);
    valarray& operator<<=(const value_type& x);
    valarray& operator>>=(const value_type& x);

    valarray& operator*= (const valarray& v);
    valarray& operator/= (const valarray& v);
    valarray& operator%= (const valarray& v);
    valarray& operator+= (const valarray& v);
    valarray& operator-= (const valarray& v);
    valarray& operator^= (const valarray& v);
    valarray& operator|= (const valarray& v);
    valarray& operator&= (const valarray& v);
    valarray& operator<<=(const valarray& v);
    valarray& operator>>=(const valarray& v);

    // member functions:
    void swap(valarray& v) noexcept;

    size_t size() const;

    value_type sum() const;
    value_type min() const;
    value_type max() const;

    valarray shift (int i) const;
    valarray cshift(int i) const;
    valarray apply(value_type f(value_type)) const;
    valarray apply(value_type f(const value_type&)) const;
    void resize(size_t n, value_type x = value_type());
};

class slice
{
public:
    slice();
    slice(size_t start, size_t size, size_t stride);

    size_t start()  const;
    size_t size()   const;
    size_t stride() const;
};

template <class T>
class slice_array
{
public:
    typedef T value_type;

    const slice_array& operator=(const slice_array& sa) const;
    void operator=  (const valarray<value_type>& v) const;
    void operator*= (const valarray<value_type>& v) const;
    void operator/= (const valarray<value_type>& v) const;
    void operator%= (const valarray<value_type>& v) const;
    void operator+= (const valarray<value_type>& v) const;
    void operator-= (const valarray<value_type>& v) const;
    void operator^= (const valarray<value_type>& v) const;
    void operator&= (const valarray<value_type>& v) const;
    void operator|= (const valarray<value_type>& v) const;
    void operator<<=(const valarray<value_type>& v) const;
    void operator>>=(const valarray<value_type>& v) const;

    void operator=(const value_type& x) const;

    slice_array() = delete;
};

class gslice
{
public:
    gslice();
    gslice(size_t start, const valarray<size_t>& size,
                         const valarray<size_t>& stride);

    size_t           start()  const;
    valarray<size_t> size()   const;
    valarray<size_t> stride() const;
};

template <class T>
class gslice_array
{
public:
    typedef T value_type;

    void operator=  (const valarray<value_type>& v) const;
    void operator*= (const valarray<value_type>& v) const;
    void operator/= (const valarray<value_type>& v) const;
    void operator%= (const valarray<value_type>& v) const;
    void operator+= (const valarray<value_type>& v) const;
    void operator-= (const valarray<value_type>& v) const;
    void operator^= (const valarray<value_type>& v) const;
    void operator&= (const valarray<value_type>& v) const;
    void operator|= (const valarray<value_type>& v) const;
    void operator<<=(const valarray<value_type>& v) const;
    void operator>>=(const valarray<value_type>& v) const;

    gslice_array(const gslice_array& ga);
    ~gslice_array();
    const gslice_array& operator=(const gslice_array& ga) const;
    void operator=(const value_type& x) const;

    gslice_array() = delete;
};

template <class T>
class mask_array
{
public:
    typedef T value_type;

    void operator=  (const valarray<value_type>& v) const;
    void operator*= (const valarray<value_type>& v) const;
    void operator/= (const valarray<value_type>& v) const;
    void operator%= (const valarray<value_type>& v) const;
    void operator+= (const valarray<value_type>& v) const;
    void operator-= (const valarray<value_type>& v) const;
    void operator^= (const valarray<value_type>& v) const;
    void operator&= (const valarray<value_type>& v) const;
    void operator|= (const valarray<value_type>& v) const;
    void operator<<=(const valarray<value_type>& v) const;
    void operator>>=(const valarray<value_type>& v) const;

    mask_array(const mask_array& ma);
    ~mask_array();
    const mask_array& operator=(const mask_array& ma) const;
    void operator=(const value_type& x) const;

    mask_array() = delete;
};

template <class T>
class indirect_array
{
public:
    typedef T value_type;

    void operator=  (const valarray<value_type>& v) const;
    void operator*= (const valarray<value_type>& v) const;
    void operator/= (const valarray<value_type>& v) const;
    void operator%= (const valarray<value_type>& v) const;
    void operator+= (const valarray<value_type>& v) const;
    void operator-= (const valarray<value_type>& v) const;
    void operator^= (const valarray<value_type>& v) const;
    void operator&= (const valarray<value_type>& v) const;
    void operator|= (const valarray<value_type>& v) const;
    void operator<<=(const valarray<value_type>& v) const;
    void operator>>=(const valarray<value_type>& v) const;

    indirect_array(const indirect_array& ia);
    ~indirect_array();
    const indirect_array& operator=(const indirect_array& ia) const;
    void operator=(const value_type& x) const;

    indirect_array() = delete;
};

template<class T> void swap(valarray<T>& x, valarray<T>& y) noexcept;

template<class T> valarray<T> operator* (const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> operator* (const valarray<T>& x, const T& y);
template<class T> valarray<T> operator* (const T& x, const valarray<T>& y);

template<class T> valarray<T> operator/ (const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> operator/ (const valarray<T>& x, const T& y);
template<class T> valarray<T> operator/ (const T& x, const valarray<T>& y);

template<class T> valarray<T> operator% (const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> operator% (const valarray<T>& x, const T& y);
template<class T> valarray<T> operator% (const T& x, const valarray<T>& y);

template<class T> valarray<T> operator+ (const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> operator+ (const valarray<T>& x, const T& y);
template<class T> valarray<T> operator+ (const T& x, const valarray<T>& y);

template<class T> valarray<T> operator- (const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> operator- (const valarray<T>& x, const T& y);
template<class T> valarray<T> operator- (const T& x, const valarray<T>& y);

template<class T> valarray<T> operator^ (const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> operator^ (const valarray<T>& x, const T& y);
template<class T> valarray<T> operator^ (const T& x, const valarray<T>& y);

template<class T> valarray<T> operator& (const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> operator& (const valarray<T>& x, const T& y);
template<class T> valarray<T> operator& (const T& x, const valarray<T>& y);

template<class T> valarray<T> operator| (const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> operator| (const valarray<T>& x, const T& y);
template<class T> valarray<T> operator| (const T& x, const valarray<T>& y);

template<class T> valarray<T> operator<<(const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> operator<<(const valarray<T>& x, const T& y);
template<class T> valarray<T> operator<<(const T& x, const valarray<T>& y);

template<class T> valarray<T> operator>>(const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> operator>>(const valarray<T>& x, const T& y);
template<class T> valarray<T> operator>>(const T& x, const valarray<T>& y);

template<class T> valarray<bool> operator&&(const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<bool> operator&&(const valarray<T>& x, const T& y);
template<class T> valarray<bool> operator&&(const T& x, const valarray<T>& y);

template<class T> valarray<bool> operator||(const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<bool> operator||(const valarray<T>& x, const T& y);
template<class T> valarray<bool> operator||(const T& x, const valarray<T>& y);

template<class T> valarray<bool> operator==(const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<bool> operator==(const valarray<T>& x, const T& y);
template<class T> valarray<bool> operator==(const T& x, const valarray<T>& y);

template<class T> valarray<bool> operator!=(const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<bool> operator!=(const valarray<T>& x, const T& y);
template<class T> valarray<bool> operator!=(const T& x, const valarray<T>& y);

template<class T> valarray<bool> operator< (const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<bool> operator< (const valarray<T>& x, const T& y);
template<class T> valarray<bool> operator< (const T& x, const valarray<T>& y);

template<class T> valarray<bool> operator> (const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<bool> operator> (const valarray<T>& x, const T& y);
template<class T> valarray<bool> operator> (const T& x, const valarray<T>& y);

template<class T> valarray<bool> operator<=(const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<bool> operator<=(const valarray<T>& x, const T& y);
template<class T> valarray<bool> operator<=(const T& x, const valarray<T>& y);

template<class T> valarray<bool> operator>=(const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<bool> operator>=(const valarray<T>& x, const T& y);
template<class T> valarray<bool> operator>=(const T& x, const valarray<T>& y);

template<class T> valarray<T> abs (const valarray<T>& x);
template<class T> valarray<T> acos (const valarray<T>& x);
template<class T> valarray<T> asin (const valarray<T>& x);
template<class T> valarray<T> atan (const valarray<T>& x);

template<class T> valarray<T> atan2(const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> atan2(const valarray<T>& x, const T& y);
template<class T> valarray<T> atan2(const T& x, const valarray<T>& y);

template<class T> valarray<T> cos (const valarray<T>& x);
template<class T> valarray<T> cosh (const valarray<T>& x);
template<class T> valarray<T> exp (const valarray<T>& x);
template<class T> valarray<T> log (const valarray<T>& x);
template<class T> valarray<T> log10(const valarray<T>& x);

template<class T> valarray<T> pow(const valarray<T>& x, const valarray<T>& y);
template<class T> valarray<T> pow(const valarray<T>& x, const T& y);
template<class T> valarray<T> pow(const T& x, const valarray<T>& y);

template<class T> valarray<T> sin (const valarray<T>& x);
template<class T> valarray<T> sinh (const valarray<T>& x);
template<class T> valarray<T> sqrt (const valarray<T>& x);
template<class T> valarray<T> tan (const valarray<T>& x);
template<class T> valarray<T> tanh (const valarray<T>& x);

template <class T> unspecified1 begin(valarray<T>& v);
template <class T> unspecified2 begin(const valarray<T>& v);
template <class T> unspecified1 end(valarray<T>& v);
template <class T> unspecified2 end(const valarray<T>& v);

}  // std

*/

#include <__config>
#include <cstddef>
#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <functional>
#include <new>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>


_LIBCPP_BEGIN_NAMESPACE_STD

template<class _Tp> class _LIBCPP_TEMPLATE_VIS valarray;

class _LIBCPP_TEMPLATE_VIS slice
{
    size_t __start_;
    size_t __size_;
    size_t __stride_;
public:
    _LIBCPP_INLINE_VISIBILITY
    slice()
        : __start_(0),
          __size_(0),
          __stride_(0)
          {}

    _LIBCPP_INLINE_VISIBILITY
    slice(size_t __start, size_t __size, size_t __stride)
        : __start_(__start),
          __size_(__size),
          __stride_(__stride)
          {}

    _LIBCPP_INLINE_VISIBILITY size_t start()  const {return __start_;}
    _LIBCPP_INLINE_VISIBILITY size_t size()   const {return __size_;}
    _LIBCPP_INLINE_VISIBILITY size_t stride() const {return __stride_;}
};

template <class _Tp> class _LIBCPP_TEMPLATE_VIS slice_array;
class _LIBCPP_TYPE_VIS gslice;
template <class _Tp> class _LIBCPP_TEMPLATE_VIS gslice_array;
template <class _Tp> class _LIBCPP_TEMPLATE_VIS mask_array;
template <class _Tp> class _LIBCPP_TEMPLATE_VIS indirect_array;

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY
_Tp*
begin(valarray<_Tp>& __v);

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY
const _Tp*
begin(const valarray<_Tp>& __v);

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY
_Tp*
end(valarray<_Tp>& __v);

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY
const _Tp*
end(const valarray<_Tp>& __v);

template <class _Op, class _A0>
struct _UnaryOp
{
    typedef typename _Op::result_type result_type;
    typedef typename _A0::value_type value_type;

    _Op __op_;
    _A0 __a0_;

    _LIBCPP_INLINE_VISIBILITY
    _UnaryOp(const _Op& __op, const _A0& __a0) : __op_(__op), __a0_(__a0) {}

    _LIBCPP_INLINE_VISIBILITY
    result_type operator[](size_t __i) const {return __op_(__a0_[__i]);}

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __a0_.size();}
};

template <class _Op, class _A0, class _A1>
struct _BinaryOp
{
    typedef typename _Op::result_type result_type;
    typedef typename _A0::value_type value_type;

    _Op __op_;
    _A0 __a0_;
    _A1 __a1_;

    _LIBCPP_INLINE_VISIBILITY
    _BinaryOp(const _Op& __op, const _A0& __a0, const _A1& __a1)
        : __op_(__op), __a0_(__a0), __a1_(__a1) {}

    _LIBCPP_INLINE_VISIBILITY
    value_type operator[](size_t __i) const {return __op_(__a0_[__i], __a1_[__i]);}

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __a0_.size();}
};

template <class _Tp>
class __scalar_expr
{
public:
    typedef _Tp        value_type;
    typedef const _Tp& result_type;
private:
    const value_type& __t_;
    size_t __s_;
public:
    _LIBCPP_INLINE_VISIBILITY
    explicit __scalar_expr(const value_type& __t, size_t __s) : __t_(__t), __s_(__s) {}

    _LIBCPP_INLINE_VISIBILITY
    result_type operator[](size_t) const {return __t_;}

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __s_;}
};

template <class _Tp>
struct __unary_plus : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return +__x;}
};

template <class _Tp>
struct __bit_not  : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return ~__x;}
};

template <class _Tp>
struct __bit_shift_left : binary_function<_Tp, _Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x, const _Tp& __y) const
        {return __x << __y;}
};

template <class _Tp>
struct __bit_shift_right : binary_function<_Tp, _Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x, const _Tp& __y) const
        {return __x >> __y;}
};

template <class _Tp, class _Fp>
struct __apply_expr   : unary_function<_Tp, _Tp>
{
private:
    _Fp __f_;
public:
    _LIBCPP_INLINE_VISIBILITY
    explicit __apply_expr(_Fp __f) : __f_(__f) {}

    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return __f_(__x);}
};

template <class _Tp>
struct __abs_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return abs(__x);}
};

template <class _Tp>
struct __acos_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return acos(__x);}
};

template <class _Tp>
struct __asin_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return asin(__x);}
};

template <class _Tp>
struct __atan_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return atan(__x);}
};

template <class _Tp>
struct __atan2_expr : binary_function<_Tp, _Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x, const _Tp& __y) const
        {return atan2(__x, __y);}
};

template <class _Tp>
struct __cos_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return cos(__x);}
};

template <class _Tp>
struct __cosh_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return cosh(__x);}
};

template <class _Tp>
struct __exp_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return exp(__x);}
};

template <class _Tp>
struct __log_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return log(__x);}
};

template <class _Tp>
struct __log10_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return log10(__x);}
};

template <class _Tp>
struct __pow_expr : binary_function<_Tp, _Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x, const _Tp& __y) const
        {return pow(__x, __y);}
};

template <class _Tp>
struct __sin_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return sin(__x);}
};

template <class _Tp>
struct __sinh_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return sinh(__x);}
};

template <class _Tp>
struct __sqrt_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return sqrt(__x);}
};

template <class _Tp>
struct __tan_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return tan(__x);}
};

template <class _Tp>
struct __tanh_expr : unary_function<_Tp, _Tp>
{
    _LIBCPP_INLINE_VISIBILITY
    _Tp operator()(const _Tp& __x) const
        {return tanh(__x);}
};

template <class _ValExpr>
class __slice_expr
{
    typedef typename remove_reference<_ValExpr>::type  _RmExpr;
public:
    typedef typename _RmExpr::value_type value_type;
    typedef value_type result_type;

private:
    _ValExpr __expr_;
    size_t __start_;
    size_t __size_;
    size_t __stride_;

    _LIBCPP_INLINE_VISIBILITY
    __slice_expr(const slice& __sl, const _RmExpr& __e)
        : __expr_(__e),
          __start_(__sl.start()),
          __size_(__sl.size()),
          __stride_(__sl.stride())
        {}
public:

    _LIBCPP_INLINE_VISIBILITY
    result_type operator[](size_t __i) const
        {return __expr_[__start_ + __i * __stride_];}

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __size_;}

    template <class> friend class _LIBCPP_TEMPLATE_VIS valarray;
};

template <class _ValExpr>
class __mask_expr;

template <class _ValExpr>
class __indirect_expr;

template <class _ValExpr>
class __shift_expr
{
    typedef typename remove_reference<_ValExpr>::type  _RmExpr;
public:
    typedef typename _RmExpr::value_type value_type;
    typedef value_type result_type;

private:
    _ValExpr __expr_;
    size_t __size_;
    ptrdiff_t __ul_;
    ptrdiff_t __sn_;
    ptrdiff_t __n_;
    static const ptrdiff_t _Np = static_cast<ptrdiff_t>(
                                    sizeof(ptrdiff_t) * __CHAR_BIT__ - 1);

    _LIBCPP_INLINE_VISIBILITY
    __shift_expr(int __n, const _RmExpr& __e)
        : __expr_(__e),
          __size_(__e.size()),
          __n_(__n)
        {
            ptrdiff_t __neg_n = static_cast<ptrdiff_t>(__n_ >> _Np);
            __sn_ = __neg_n | static_cast<ptrdiff_t>(static_cast<size_t>(-__n_) >> _Np);
            __ul_ = ((__size_ - __n_) & ~__neg_n) | ((__n_ + 1) & __neg_n);
        }
public:

    _LIBCPP_INLINE_VISIBILITY
    result_type operator[](size_t __j) const
        {
            ptrdiff_t __i = static_cast<ptrdiff_t>(__j);
            ptrdiff_t __m = (__sn_ * __i - __ul_) >> _Np;
            return (__expr_[(__i + __n_) & __m] & __m) | (value_type() & ~__m);
        }

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __size_;}

    template <class> friend class __val_expr;
};

template <class _ValExpr>
class __cshift_expr
{
    typedef typename remove_reference<_ValExpr>::type  _RmExpr;
public:
    typedef typename _RmExpr::value_type value_type;
    typedef value_type result_type;

private:
    _ValExpr __expr_;
    size_t __size_;
    size_t __m_;
    size_t __o1_;
    size_t __o2_;

    _LIBCPP_INLINE_VISIBILITY
    __cshift_expr(int __n, const _RmExpr& __e)
        : __expr_(__e),
          __size_(__e.size())
        {
            __n %= static_cast<int>(__size_);
            if (__n >= 0)
            {
                __m_ = __size_ - __n;
                __o1_ = __n;
                __o2_ = __n - __size_;
            }
            else
            {
                __m_ = -__n;
                __o1_ = __n + __size_;
                __o2_ = __n;
            }
        }
public:

    _LIBCPP_INLINE_VISIBILITY
    result_type operator[](size_t __i) const
        {
            if (__i < __m_)
                return __expr_[__i + __o1_];
            return __expr_[__i + __o2_];
        }

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __size_;}

    template <class> friend class __val_expr;
};

template<class _ValExpr>
class __val_expr;

template<class _ValExpr>
struct __is_val_expr : false_type {};

template<class _ValExpr>
struct __is_val_expr<__val_expr<_ValExpr> > : true_type {};

template<class _Tp>
struct __is_val_expr<valarray<_Tp> > : true_type {};

template<class _Tp>
class _LIBCPP_TEMPLATE_VIS valarray
{
public:
    typedef _Tp value_type;
    typedef _Tp result_type;

private:
    value_type* __begin_;
    value_type* __end_;

public:
    // construct/destroy:
    _LIBCPP_INLINE_VISIBILITY
    valarray() : __begin_(0), __end_(0) {}
    inline _LIBCPP_EXTERN_TEMPLATE_INLINE_VISIBILITY
    explicit valarray(size_t __n);
    _LIBCPP_INLINE_VISIBILITY
    valarray(const value_type& __x, size_t __n);
    valarray(const value_type* __p, size_t __n);
    valarray(const valarray& __v);
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    valarray(valarray&& __v) _NOEXCEPT;
    valarray(initializer_list<value_type> __il);
#endif  // _LIBCPP_CXX03_LANG
    valarray(const slice_array<value_type>& __sa);
    valarray(const gslice_array<value_type>& __ga);
    valarray(const mask_array<value_type>& __ma);
    valarray(const indirect_array<value_type>& __ia);
    inline _LIBCPP_EXTERN_TEMPLATE_INLINE_VISIBILITY
    ~valarray();

    // assignment:
    valarray& operator=(const valarray& __v);
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator=(valarray&& __v) _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator=(initializer_list<value_type>);
#endif  // _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator=(const value_type& __x);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator=(const slice_array<value_type>& __sa);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator=(const gslice_array<value_type>& __ga);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator=(const mask_array<value_type>& __ma);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator=(const indirect_array<value_type>& __ia);
    template <class _ValExpr>
        _LIBCPP_INLINE_VISIBILITY
        valarray& operator=(const __val_expr<_ValExpr>& __v);

    // element access:
    _LIBCPP_INLINE_VISIBILITY
    const value_type& operator[](size_t __i) const {return __begin_[__i];}

    _LIBCPP_INLINE_VISIBILITY
    value_type&       operator[](size_t __i)       {return __begin_[__i];}

    // subset operations:
    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__slice_expr<const valarray&> >    operator[](slice __s) const;
    _LIBCPP_INLINE_VISIBILITY
    slice_array<value_type>                       operator[](slice __s);
    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__indirect_expr<const valarray&> > operator[](const gslice& __gs) const;
    _LIBCPP_INLINE_VISIBILITY
    gslice_array<value_type>   operator[](const gslice& __gs);
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__indirect_expr<const valarray&> > operator[](gslice&& __gs) const;
    _LIBCPP_INLINE_VISIBILITY
    gslice_array<value_type>                      operator[](gslice&& __gs);
#endif  // _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__mask_expr<const valarray&> >     operator[](const valarray<bool>& __vb) const;
    _LIBCPP_INLINE_VISIBILITY
    mask_array<value_type>                        operator[](const valarray<bool>& __vb);
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__mask_expr<const valarray&> >     operator[](valarray<bool>&& __vb) const;
    _LIBCPP_INLINE_VISIBILITY
    mask_array<value_type>                        operator[](valarray<bool>&& __vb);
#endif  // _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__indirect_expr<const valarray&> > operator[](const valarray<size_t>& __vs) const;
    _LIBCPP_INLINE_VISIBILITY
    indirect_array<value_type>                    operator[](const valarray<size_t>& __vs);
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__indirect_expr<const valarray&> > operator[](valarray<size_t>&& __vs) const;
    _LIBCPP_INLINE_VISIBILITY
    indirect_array<value_type>                    operator[](valarray<size_t>&& __vs);
#endif  // _LIBCPP_CXX03_LANG

    // unary operators:
    valarray       operator+() const;
    valarray       operator-() const;
    valarray       operator~() const;
    valarray<bool> operator!() const;

    // computed assignment:
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator*= (const value_type& __x);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator/= (const value_type& __x);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator%= (const value_type& __x);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator+= (const value_type& __x);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator-= (const value_type& __x);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator^= (const value_type& __x);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator&= (const value_type& __x);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator|= (const value_type& __x);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator<<=(const value_type& __x);
    _LIBCPP_INLINE_VISIBILITY
    valarray& operator>>=(const value_type& __x);

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        valarray&
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator*= (const _Expr& __v);

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        valarray&
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator/= (const _Expr& __v);

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        valarray&
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator%= (const _Expr& __v);

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        valarray&
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator+= (const _Expr& __v);

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        valarray&
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator-= (const _Expr& __v);

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        valarray&
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator^= (const _Expr& __v);

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        valarray&
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator|= (const _Expr& __v);

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        valarray&
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator&= (const _Expr& __v);

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        valarray&
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator<<= (const _Expr& __v);

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        valarray&
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator>>= (const _Expr& __v);

    // member functions:
    _LIBCPP_INLINE_VISIBILITY
    void swap(valarray& __v) _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return static_cast<size_t>(__end_ - __begin_);}

    _LIBCPP_INLINE_VISIBILITY
    value_type sum() const;
    _LIBCPP_INLINE_VISIBILITY
    value_type min() const;
    _LIBCPP_INLINE_VISIBILITY
    value_type max() const;

    valarray shift (int __i) const;
    valarray cshift(int __i) const;
    valarray apply(value_type __f(value_type)) const;
    valarray apply(value_type __f(const value_type&)) const;
    void     resize(size_t __n, value_type __x = value_type());

private:
    template <class> friend class _LIBCPP_TEMPLATE_VIS valarray;
    template <class> friend class _LIBCPP_TEMPLATE_VIS slice_array;
    template <class> friend class _LIBCPP_TEMPLATE_VIS gslice_array;
    template <class> friend class _LIBCPP_TEMPLATE_VIS mask_array;
    template <class> friend class __mask_expr;
    template <class> friend class _LIBCPP_TEMPLATE_VIS indirect_array;
    template <class> friend class __indirect_expr;
    template <class> friend class __val_expr;

    template <class _Up>
    friend
    _Up*
    begin(valarray<_Up>& __v);

    template <class _Up>
    friend
    const _Up*
    begin(const valarray<_Up>& __v);

    template <class _Up>
    friend
    _Up*
    end(valarray<_Up>& __v);

    template <class _Up>
    friend
    const _Up*
    end(const valarray<_Up>& __v);

    void __clear();
    valarray& __assign_range(const value_type* __f, const value_type* __l);
};

_LIBCPP_EXTERN_TEMPLATE(_LIBCPP_FUNC_VIS valarray<size_t>::valarray(size_t))
_LIBCPP_EXTERN_TEMPLATE(_LIBCPP_FUNC_VIS valarray<size_t>::~valarray())
_LIBCPP_EXTERN_TEMPLATE(_LIBCPP_FUNC_VIS void valarray<size_t>::resize(size_t, size_t))

template <class _Op, class _Tp>
struct _UnaryOp<_Op, valarray<_Tp> >
{
    typedef typename _Op::result_type result_type;
    typedef _Tp value_type;

    _Op __op_;
    const valarray<_Tp>& __a0_;

    _LIBCPP_INLINE_VISIBILITY
    _UnaryOp(const _Op& __op, const valarray<_Tp>& __a0) : __op_(__op), __a0_(__a0) {}

    _LIBCPP_INLINE_VISIBILITY
    result_type operator[](size_t __i) const {return __op_(__a0_[__i]);}

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __a0_.size();}
};

template <class _Op, class _Tp, class _A1>
struct _BinaryOp<_Op, valarray<_Tp>, _A1>
{
    typedef typename _Op::result_type result_type;
    typedef _Tp value_type;

    _Op __op_;
    const valarray<_Tp>& __a0_;
    _A1 __a1_;

    _LIBCPP_INLINE_VISIBILITY
    _BinaryOp(const _Op& __op, const valarray<_Tp>& __a0, const _A1& __a1)
        : __op_(__op), __a0_(__a0), __a1_(__a1) {}

    _LIBCPP_INLINE_VISIBILITY
    value_type operator[](size_t __i) const {return __op_(__a0_[__i], __a1_[__i]);}

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __a0_.size();}
};

template <class _Op, class _A0, class _Tp>
struct _BinaryOp<_Op, _A0, valarray<_Tp> >
{
    typedef typename _Op::result_type result_type;
    typedef _Tp value_type;

    _Op __op_;
    _A0 __a0_;
    const valarray<_Tp>& __a1_;

    _LIBCPP_INLINE_VISIBILITY
    _BinaryOp(const _Op& __op, const _A0& __a0, const valarray<_Tp>& __a1)
        : __op_(__op), __a0_(__a0), __a1_(__a1) {}

    _LIBCPP_INLINE_VISIBILITY
    value_type operator[](size_t __i) const {return __op_(__a0_[__i], __a1_[__i]);}

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __a0_.size();}
};

template <class _Op, class _Tp>
struct _BinaryOp<_Op, valarray<_Tp>, valarray<_Tp> >
{
    typedef typename _Op::result_type result_type;
    typedef _Tp value_type;

    _Op __op_;
    const valarray<_Tp>& __a0_;
    const valarray<_Tp>& __a1_;

    _LIBCPP_INLINE_VISIBILITY
    _BinaryOp(const _Op& __op, const valarray<_Tp>& __a0, const valarray<_Tp>& __a1)
        : __op_(__op), __a0_(__a0), __a1_(__a1) {}

    _LIBCPP_INLINE_VISIBILITY
    value_type operator[](size_t __i) const {return __op_(__a0_[__i], __a1_[__i]);}

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __a0_.size();}
};

// slice_array

template <class _Tp>
class _LIBCPP_TEMPLATE_VIS slice_array
{
public:
    typedef _Tp value_type;

private:
    value_type* __vp_;
    size_t __size_;
    size_t __stride_;

public:
    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator*=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator/=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator%=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator+=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator-=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator^=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator&=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator|=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator<<=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator>>=(const _Expr& __v) const;

    _LIBCPP_INLINE_VISIBILITY
    const slice_array& operator=(const slice_array& __sa) const;

    _LIBCPP_INLINE_VISIBILITY
    void operator=(const value_type& __x) const;

private:
    _LIBCPP_INLINE_VISIBILITY
    slice_array(const slice& __sl, const valarray<value_type>& __v)
        : __vp_(const_cast<value_type*>(__v.__begin_ + __sl.start())),
          __size_(__sl.size()),
          __stride_(__sl.stride())
        {}

    template <class> friend class valarray;
    template <class> friend class sliceExpr;
};

template <class _Tp>
inline
const slice_array<_Tp>&
slice_array<_Tp>::operator=(const slice_array& __sa) const
{
    value_type* __t = __vp_;
    const value_type* __s = __sa.__vp_;
    for (size_t __n = __size_; __n; --__n, __t += __stride_, __s += __sa.__stride_)
        *__t = *__s;
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t = __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator*=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t *= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator/=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t /= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator%=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t %= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator+=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t += __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator-=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t -= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator^=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t ^= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator&=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t &= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator|=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t |= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator<<=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t <<= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
slice_array<_Tp>::operator>>=(const _Expr& __v) const
{
    value_type* __t = __vp_;
    for (size_t __i = 0; __i < __size_; ++__i, __t += __stride_)
        *__t >>= __v[__i];
}

template <class _Tp>
inline
void
slice_array<_Tp>::operator=(const value_type& __x) const
{
    value_type* __t = __vp_;
    for (size_t __n = __size_; __n; --__n, __t += __stride_)
        *__t = __x;
}

// gslice

class _LIBCPP_TYPE_VIS gslice
{
    valarray<size_t> __size_;
    valarray<size_t> __stride_;
    valarray<size_t> __1d_;

public:
    _LIBCPP_INLINE_VISIBILITY
    gslice() {}

    _LIBCPP_INLINE_VISIBILITY
    gslice(size_t __start, const valarray<size_t>& __size,
                           const valarray<size_t>& __stride)
        : __size_(__size),
          __stride_(__stride)
        {__init(__start);}

#ifndef _LIBCPP_CXX03_LANG

    _LIBCPP_INLINE_VISIBILITY
    gslice(size_t __start, const valarray<size_t>&  __size,
                                 valarray<size_t>&& __stride)
        : __size_(__size),
          __stride_(move(__stride))
        {__init(__start);}

    _LIBCPP_INLINE_VISIBILITY
    gslice(size_t __start,       valarray<size_t>&& __size,
                           const valarray<size_t>&  __stride)
        : __size_(move(__size)),
          __stride_(__stride)
        {__init(__start);}

    _LIBCPP_INLINE_VISIBILITY
    gslice(size_t __start,       valarray<size_t>&& __size,
                                 valarray<size_t>&& __stride)
        : __size_(move(__size)),
          __stride_(move(__stride))
        {__init(__start);}

#endif  // _LIBCPP_CXX03_LANG

//  gslice(const gslice&)            = default;
//  gslice(gslice&&)                 = default;
//  gslice& operator=(const gslice&) = default;
//  gslice& operator=(gslice&&)      = default;

    _LIBCPP_INLINE_VISIBILITY
    size_t           start()  const {return __1d_.size() ? __1d_[0] : 0;}

    _LIBCPP_INLINE_VISIBILITY
    valarray<size_t> size()   const {return __size_;}

    _LIBCPP_INLINE_VISIBILITY
    valarray<size_t> stride() const {return __stride_;}

private:
    void __init(size_t __start);

    template <class> friend class gslice_array;
    template <class> friend class valarray;
    template <class> friend class __val_expr;
};

// gslice_array

template <class _Tp>
class _LIBCPP_TEMPLATE_VIS gslice_array
{
public:
    typedef _Tp value_type;

private:
    value_type*      __vp_;
    valarray<size_t> __1d_;

public:
    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator*=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator/=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator%=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator+=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator-=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator^=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator&=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator|=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator<<=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator>>=(const _Expr& __v) const;

    _LIBCPP_INLINE_VISIBILITY
    const gslice_array& operator=(const gslice_array& __ga) const;

    _LIBCPP_INLINE_VISIBILITY
    void operator=(const value_type& __x) const;

//  gslice_array(const gslice_array&)            = default;
//  gslice_array(gslice_array&&)                 = default;
//  gslice_array& operator=(const gslice_array&) = default;
//  gslice_array& operator=(gslice_array&&)      = default;

private:
    gslice_array(const gslice& __gs, const valarray<value_type>& __v)
        : __vp_(const_cast<value_type*>(__v.__begin_)),
          __1d_(__gs.__1d_)
        {}

#ifndef _LIBCPP_CXX03_LANG
    gslice_array(gslice&& __gs, const valarray<value_type>& __v)
        : __vp_(const_cast<value_type*>(__v.__begin_)),
          __1d_(move(__gs.__1d_))
        {}
#endif  // _LIBCPP_CXX03_LANG

    template <class> friend class valarray;
};

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] = __v[__j];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator*=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] *= __v[__j];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator/=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] /= __v[__j];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator%=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] %= __v[__j];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator+=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] += __v[__j];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator-=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] -= __v[__j];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator^=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] ^= __v[__j];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator&=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] &= __v[__j];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator|=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] |= __v[__j];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator<<=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] <<= __v[__j];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
gslice_array<_Tp>::operator>>=(const _Expr& __v) const
{
    typedef const size_t* _Ip;
    size_t __j = 0;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i, ++__j)
        __vp_[*__i] >>= __v[__j];
}

template <class _Tp>
inline
const gslice_array<_Tp>&
gslice_array<_Tp>::operator=(const gslice_array& __ga) const
{
    typedef const size_t* _Ip;
    const value_type* __s = __ga.__vp_;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_, __j = __ga.__1d_.__begin_;
            __i != __e; ++__i, ++__j)
        __vp_[*__i] = __s[*__j];
    return *this;
}

template <class _Tp>
inline
void
gslice_array<_Tp>::operator=(const value_type& __x) const
{
    typedef const size_t* _Ip;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i)
        __vp_[*__i] = __x;
}

// mask_array

template <class _Tp>
class _LIBCPP_TEMPLATE_VIS mask_array
{
public:
    typedef _Tp value_type;

private:
    value_type*      __vp_;
    valarray<size_t> __1d_;

public:
    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator*=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator/=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator%=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator+=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator-=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator^=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator&=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator|=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator<<=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator>>=(const _Expr& __v) const;

    _LIBCPP_INLINE_VISIBILITY
    const mask_array& operator=(const mask_array& __ma) const;

    _LIBCPP_INLINE_VISIBILITY
    void operator=(const value_type& __x) const;

//  mask_array(const mask_array&)            = default;
//  mask_array(mask_array&&)                 = default;
//  mask_array& operator=(const mask_array&) = default;
//  mask_array& operator=(mask_array&&)      = default;

private:
    _LIBCPP_INLINE_VISIBILITY
    mask_array(const valarray<bool>& __vb, const valarray<value_type>& __v)
        : __vp_(const_cast<value_type*>(__v.__begin_)),
          __1d_(static_cast<size_t>(count(__vb.__begin_, __vb.__end_, true)))
          {
              size_t __j = 0;
              for (size_t __i = 0; __i < __vb.size(); ++__i)
                  if (__vb[__i])
                      __1d_[__j++] = __i;
          }

    template <class> friend class valarray;
};

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] = __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator*=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] *= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator/=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] /= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator%=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] %= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator+=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] += __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator-=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] -= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator^=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] ^= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator&=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] &= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator|=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] |= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator<<=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] <<= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
mask_array<_Tp>::operator>>=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] >>= __v[__i];
}

template <class _Tp>
inline
const mask_array<_Tp>&
mask_array<_Tp>::operator=(const mask_array& __ma) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] = __ma.__vp_[__1d_[__i]];
    return *this;
}

template <class _Tp>
inline
void
mask_array<_Tp>::operator=(const value_type& __x) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] = __x;
}

template <class _ValExpr>
class __mask_expr
{
    typedef typename remove_reference<_ValExpr>::type  _RmExpr;
public:
    typedef typename _RmExpr::value_type value_type;
    typedef value_type result_type;

private:
    _ValExpr __expr_;
    valarray<size_t> __1d_;

    _LIBCPP_INLINE_VISIBILITY
    __mask_expr(const valarray<bool>& __vb, const _RmExpr& __e)
        : __expr_(__e),
          __1d_(static_cast<size_t>(count(__vb.__begin_, __vb.__end_, true)))
          {
              size_t __j = 0;
              for (size_t __i = 0; __i < __vb.size(); ++__i)
                  if (__vb[__i])
                      __1d_[__j++] = __i;
          }

public:
    _LIBCPP_INLINE_VISIBILITY
    result_type operator[](size_t __i) const
        {return __expr_[__1d_[__i]];}

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __1d_.size();}

    template <class> friend class valarray;
};

// indirect_array

template <class _Tp>
class _LIBCPP_TEMPLATE_VIS indirect_array
{
public:
    typedef _Tp value_type;

private:
    value_type*      __vp_;
    valarray<size_t> __1d_;

public:
    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator*=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator/=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator%=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator+=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator-=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator^=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator&=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator|=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator<<=(const _Expr& __v) const;

    template <class _Expr>
    typename enable_if
    <
        __is_val_expr<_Expr>::value,
        void
    >::type
    _LIBCPP_INLINE_VISIBILITY
    operator>>=(const _Expr& __v) const;

    _LIBCPP_INLINE_VISIBILITY
    const indirect_array& operator=(const indirect_array& __ia) const;

    _LIBCPP_INLINE_VISIBILITY
    void operator=(const value_type& __x) const;

//  indirect_array(const indirect_array&)            = default;
//  indirect_array(indirect_array&&)                 = default;
//  indirect_array& operator=(const indirect_array&) = default;
//  indirect_array& operator=(indirect_array&&)      = default;

private:
     _LIBCPP_INLINE_VISIBILITY
   indirect_array(const valarray<size_t>& __ia, const valarray<value_type>& __v)
        : __vp_(const_cast<value_type*>(__v.__begin_)),
          __1d_(__ia)
        {}

#ifndef _LIBCPP_CXX03_LANG

    _LIBCPP_INLINE_VISIBILITY
    indirect_array(valarray<size_t>&& __ia, const valarray<value_type>& __v)
        : __vp_(const_cast<value_type*>(__v.__begin_)),
          __1d_(move(__ia))
        {}

#endif  // _LIBCPP_CXX03_LANG

    template <class> friend class valarray;
};

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] = __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator*=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] *= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator/=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] /= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator%=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] %= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator+=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] += __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator-=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] -= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator^=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] ^= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator&=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] &= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator|=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] |= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator<<=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] <<= __v[__i];
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    void
>::type
indirect_array<_Tp>::operator>>=(const _Expr& __v) const
{
    size_t __n = __1d_.size();
    for (size_t __i = 0; __i < __n; ++__i)
        __vp_[__1d_[__i]] >>= __v[__i];
}

template <class _Tp>
inline
const indirect_array<_Tp>&
indirect_array<_Tp>::operator=(const indirect_array& __ia) const
{
    typedef const size_t* _Ip;
    const value_type* __s = __ia.__vp_;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_, __j = __ia.__1d_.__begin_;
            __i != __e; ++__i, ++__j)
        __vp_[*__i] = __s[*__j];
    return *this;
}

template <class _Tp>
inline
void
indirect_array<_Tp>::operator=(const value_type& __x) const
{
    typedef const size_t* _Ip;
    for (_Ip __i = __1d_.__begin_, __e = __1d_.__end_; __i != __e; ++__i)
        __vp_[*__i] = __x;
}

template <class _ValExpr>
class __indirect_expr
{
    typedef typename remove_reference<_ValExpr>::type  _RmExpr;
public:
    typedef typename _RmExpr::value_type value_type;
    typedef value_type result_type;

private:
    _ValExpr __expr_;
    valarray<size_t> __1d_;

    _LIBCPP_INLINE_VISIBILITY
    __indirect_expr(const valarray<size_t>& __ia, const _RmExpr& __e)
        : __expr_(__e),
          __1d_(__ia)
          {}

#ifndef _LIBCPP_CXX03_LANG

    _LIBCPP_INLINE_VISIBILITY
    __indirect_expr(valarray<size_t>&& __ia, const _RmExpr& __e)
        : __expr_(__e),
          __1d_(move(__ia))
          {}

#endif  // _LIBCPP_CXX03_LANG

public:
    _LIBCPP_INLINE_VISIBILITY
    result_type operator[](size_t __i) const
        {return __expr_[__1d_[__i]];}

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __1d_.size();}

    template <class> friend class _LIBCPP_TEMPLATE_VIS valarray;
};

template<class _ValExpr>
class __val_expr
{
    typedef typename remove_reference<_ValExpr>::type  _RmExpr;

    _ValExpr __expr_;
public:
    typedef typename _RmExpr::value_type value_type;
    typedef typename _RmExpr::result_type result_type;

    _LIBCPP_INLINE_VISIBILITY
    explicit __val_expr(const _RmExpr& __e) : __expr_(__e) {}

    _LIBCPP_INLINE_VISIBILITY
    result_type operator[](size_t __i) const
        {return __expr_[__i];}

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__slice_expr<_ValExpr> > operator[](slice __s) const
        {return __val_expr<__slice_expr<_ValExpr> >(__expr_, __s);}

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__indirect_expr<_ValExpr> > operator[](const gslice& __gs) const
        {return __val_expr<__indirect_expr<_ValExpr> >(__expr_, __gs.__1d_);}

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__mask_expr<_ValExpr> > operator[](const valarray<bool>& __vb) const
        {return __val_expr<__mask_expr<_ValExpr> >(__expr_, __vb);}

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__indirect_expr<_ValExpr> > operator[](const valarray<size_t>& __vs) const
        {return __val_expr<__indirect_expr<_ValExpr> >(__expr_, __vs);}

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<_UnaryOp<__unary_plus<value_type>, _ValExpr> >
    operator+() const
    {
        typedef _UnaryOp<__unary_plus<value_type>, _ValExpr> _NewExpr;
        return __val_expr<_NewExpr>(_NewExpr(__unary_plus<value_type>(), __expr_));
    }

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<_UnaryOp<negate<value_type>, _ValExpr> >
    operator-() const
    {
        typedef _UnaryOp<negate<value_type>, _ValExpr> _NewExpr;
        return __val_expr<_NewExpr>(_NewExpr(negate<value_type>(), __expr_));
    }

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<_UnaryOp<__bit_not<value_type>, _ValExpr> >
    operator~() const
    {
        typedef _UnaryOp<__bit_not<value_type>, _ValExpr> _NewExpr;
        return __val_expr<_NewExpr>(_NewExpr(__bit_not<value_type>(), __expr_));
    }

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<_UnaryOp<logical_not<value_type>, _ValExpr> >
    operator!() const
    {
        typedef _UnaryOp<logical_not<value_type>, _ValExpr> _NewExpr;
        return __val_expr<_NewExpr>(_NewExpr(logical_not<value_type>(), __expr_));
    }

    operator valarray<result_type>() const;

    _LIBCPP_INLINE_VISIBILITY
    size_t size() const {return __expr_.size();}

    _LIBCPP_INLINE_VISIBILITY
    result_type sum() const
    {
        size_t __n = __expr_.size();
        result_type __r = __n ? __expr_[0] : result_type();
        for (size_t __i = 1; __i < __n; ++__i)
            __r += __expr_[__i];
        return __r;
    }

    _LIBCPP_INLINE_VISIBILITY
    result_type min() const
    {
        size_t __n = size();
        result_type __r = __n ? (*this)[0] : result_type();
        for (size_t __i = 1; __i < __n; ++__i)
        {
            result_type __x = __expr_[__i];
            if (__x < __r)
                __r = __x;
        }
        return __r;
    }

    _LIBCPP_INLINE_VISIBILITY
    result_type max() const
    {
        size_t __n = size();
        result_type __r = __n ? (*this)[0] : result_type();
        for (size_t __i = 1; __i < __n; ++__i)
        {
            result_type __x = __expr_[__i];
            if (__r < __x)
                __r = __x;
        }
        return __r;
    }

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__shift_expr<_ValExpr> > shift (int __i) const
        {return __val_expr<__shift_expr<_ValExpr> >(__shift_expr<_ValExpr>(__i, __expr_));}

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<__cshift_expr<_ValExpr> > cshift(int __i) const
        {return __val_expr<__cshift_expr<_ValExpr> >(__cshift_expr<_ValExpr>(__i, __expr_));}

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<_UnaryOp<__apply_expr<value_type, value_type(*)(value_type)>, _ValExpr> >
    apply(value_type __f(value_type)) const
    {
        typedef __apply_expr<value_type, value_type(*)(value_type)> _Op;
        typedef _UnaryOp<_Op, _ValExpr> _NewExpr;
        return __val_expr<_NewExpr>(_NewExpr(_Op(__f), __expr_));
    }

    _LIBCPP_INLINE_VISIBILITY
    __val_expr<_UnaryOp<__apply_expr<value_type, value_type(*)(const value_type&)>, _ValExpr> >
    apply(value_type __f(const value_type&)) const
    {
        typedef __apply_expr<value_type, value_type(*)(const value_type&)> _Op;
        typedef _UnaryOp<_Op, _ValExpr> _NewExpr;
        return __val_expr<_NewExpr>(_NewExpr(_Op(__f), __expr_));
    }
};

template<class _ValExpr>
__val_expr<_ValExpr>::operator valarray<__val_expr::result_type>() const
{
    valarray<result_type> __r;
    size_t __n = __expr_.size();
    if (__n)
    {
        __r.__begin_ =
            __r.__end_ =
                static_cast<result_type*>(
                    _VSTD::__libcpp_allocate(__n * sizeof(result_type), __alignof(result_type)));
        for (size_t __i = 0; __i != __n; ++__r.__end_, ++__i)
            ::new (__r.__end_) result_type(__expr_[__i]);
    }
    return __r;
}

// valarray

template <class _Tp>
inline
valarray<_Tp>::valarray(size_t __n)
    : __begin_(0),
      __end_(0)
{
    if (__n)
    {
        __begin_ = __end_ = static_cast<value_type*>(
            _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            for (; __n; --__n, ++__end_)
                ::new (__end_) value_type();
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __clear();
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
}

template <class _Tp>
inline
valarray<_Tp>::valarray(const value_type& __x, size_t __n)
    : __begin_(0),
      __end_(0)
{
    resize(__n, __x);
}

template <class _Tp>
valarray<_Tp>::valarray(const value_type* __p, size_t __n)
    : __begin_(0),
      __end_(0)
{
    if (__n)
    {
        __begin_ = __end_ = static_cast<value_type*>(
            _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            for (; __n; ++__end_, ++__p, --__n)
                ::new (__end_) value_type(*__p);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __clear();
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
}

template <class _Tp>
valarray<_Tp>::valarray(const valarray& __v)
    : __begin_(0),
      __end_(0)
{
    if (__v.size())
    {
        __begin_ = __end_ = static_cast<value_type*>(
            _VSTD::__libcpp_allocate(__v.size() * sizeof(value_type), __alignof(value_type)));
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            for (value_type* __p = __v.__begin_; __p != __v.__end_; ++__end_, ++__p)
                ::new (__end_) value_type(*__p);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __clear();
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
}

#ifndef _LIBCPP_CXX03_LANG

template <class _Tp>
inline
valarray<_Tp>::valarray(valarray&& __v) _NOEXCEPT
    : __begin_(__v.__begin_),
      __end_(__v.__end_)
{
    __v.__begin_ = __v.__end_ = nullptr;
}

template <class _Tp>
valarray<_Tp>::valarray(initializer_list<value_type> __il)
    : __begin_(0),
      __end_(0)
{
    size_t __n = __il.size();
    if (__n)
    {
        __begin_ = __end_ = static_cast<value_type*>(
_VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            for (const value_type* __p = __il.begin(); __n; ++__end_, ++__p, --__n)
                ::new (__end_) value_type(*__p);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __clear();
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
}

#endif  // _LIBCPP_CXX03_LANG

template <class _Tp>
valarray<_Tp>::valarray(const slice_array<value_type>& __sa)
    : __begin_(0),
      __end_(0)
{
    size_t __n = __sa.__size_;
    if (__n)
    {
        __begin_ = __end_ = static_cast<value_type*>(
          _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            for (const value_type* __p = __sa.__vp_; __n; ++__end_, __p += __sa.__stride_, --__n)
                ::new (__end_) value_type(*__p);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __clear();
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
}

template <class _Tp>
valarray<_Tp>::valarray(const gslice_array<value_type>& __ga)
    : __begin_(0),
      __end_(0)
{
    size_t __n = __ga.__1d_.size();
    if (__n)
    {
        __begin_ = __end_ = static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            typedef const size_t* _Ip;
            const value_type* __s = __ga.__vp_;
            for (_Ip __i = __ga.__1d_.__begin_, __e = __ga.__1d_.__end_;
                    __i != __e; ++__i, ++__end_)
                ::new (__end_) value_type(__s[*__i]);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __clear();
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
}

template <class _Tp>
valarray<_Tp>::valarray(const mask_array<value_type>& __ma)
    : __begin_(0),
      __end_(0)
{
    size_t __n = __ma.__1d_.size();
    if (__n)
    {
        __begin_ = __end_ = static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            typedef const size_t* _Ip;
            const value_type* __s = __ma.__vp_;
            for (_Ip __i = __ma.__1d_.__begin_, __e = __ma.__1d_.__end_;
                    __i != __e; ++__i, ++__end_)
                ::new (__end_) value_type(__s[*__i]);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __clear();
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
}

template <class _Tp>
valarray<_Tp>::valarray(const indirect_array<value_type>& __ia)
    : __begin_(0),
      __end_(0)
{
    size_t __n = __ia.__1d_.size();
    if (__n)
    {
        __begin_ = __end_ = static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            typedef const size_t* _Ip;
            const value_type* __s = __ia.__vp_;
            for (_Ip __i = __ia.__1d_.__begin_, __e = __ia.__1d_.__end_;
                    __i != __e; ++__i, ++__end_)
                ::new (__end_) value_type(__s[*__i]);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __clear();
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
}

template <class _Tp>
inline
valarray<_Tp>::~valarray()
{
    __clear();
}

template <class _Tp>
valarray<_Tp>&
valarray<_Tp>::__assign_range(const value_type* __f, const value_type* __l)
{
    size_t __n = __l - __f;
    if (size() != __n)
    {
        __clear();
        __begin_ = static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
        __end_ = __begin_ + __n;
        _VSTD::uninitialized_copy(__f, __l, __begin_);
    } else {
        _VSTD::copy(__f, __l, __begin_);
    }
    return *this;
}

template <class _Tp>
valarray<_Tp>&
valarray<_Tp>::operator=(const valarray& __v)
{
    if (this != &__v)
        return __assign_range(__v.__begin_, __v.__end_);
    return *this;
}

#ifndef _LIBCPP_CXX03_LANG

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator=(valarray&& __v) _NOEXCEPT
{
    __clear();
    __begin_ = __v.__begin_;
    __end_ = __v.__end_;
    __v.__begin_ = nullptr;
    __v.__end_ = nullptr;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator=(initializer_list<value_type> __il)
{
    return __assign_range(__il.begin(), __il.end());
}

#endif  // _LIBCPP_CXX03_LANG

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator=(const value_type& __x)
{
    _VSTD::fill(__begin_, __end_, __x);
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator=(const slice_array<value_type>& __sa)
{
    value_type* __t = __begin_;
    const value_type* __s = __sa.__vp_;
    for (size_t __n = __sa.__size_; __n; --__n, __s += __sa.__stride_, ++__t)
        *__t = *__s;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator=(const gslice_array<value_type>& __ga)
{
    typedef const size_t* _Ip;
    value_type* __t = __begin_;
    const value_type* __s = __ga.__vp_;
    for (_Ip __i = __ga.__1d_.__begin_, __e = __ga.__1d_.__end_;
                    __i != __e; ++__i, ++__t)
        *__t = __s[*__i];
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator=(const mask_array<value_type>& __ma)
{
    typedef const size_t* _Ip;
    value_type* __t = __begin_;
    const value_type* __s = __ma.__vp_;
    for (_Ip __i = __ma.__1d_.__begin_, __e = __ma.__1d_.__end_;
                    __i != __e; ++__i, ++__t)
        *__t = __s[*__i];
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator=(const indirect_array<value_type>& __ia)
{
    typedef const size_t* _Ip;
    value_type* __t = __begin_;
    const value_type* __s = __ia.__vp_;
    for (_Ip __i = __ia.__1d_.__begin_, __e = __ia.__1d_.__end_;
                    __i != __e; ++__i, ++__t)
        *__t = __s[*__i];
    return *this;
}

template <class _Tp>
template <class _ValExpr>
inline
valarray<_Tp>&
valarray<_Tp>::operator=(const __val_expr<_ValExpr>& __v)
{
    size_t __n = __v.size();
    if (size() != __n)
        resize(__n);
    value_type* __t = __begin_;
    for (size_t __i = 0; __i != __n; ++__t, ++__i)
        *__t = result_type(__v[__i]);
    return *this;
}

template <class _Tp>
inline
__val_expr<__slice_expr<const valarray<_Tp>&> >
valarray<_Tp>::operator[](slice __s) const
{
    return __val_expr<__slice_expr<const valarray&> >(__slice_expr<const valarray&>(__s, *this));
}

template <class _Tp>
inline
slice_array<_Tp>
valarray<_Tp>::operator[](slice __s)
{
    return slice_array<value_type>(__s, *this);
}

template <class _Tp>
inline
__val_expr<__indirect_expr<const valarray<_Tp>&> >
valarray<_Tp>::operator[](const gslice& __gs) const
{
    return __val_expr<__indirect_expr<const valarray&> >(__indirect_expr<const valarray&>(__gs.__1d_, *this));
}

template <class _Tp>
inline
gslice_array<_Tp>
valarray<_Tp>::operator[](const gslice& __gs)
{
    return gslice_array<value_type>(__gs, *this);
}

#ifndef _LIBCPP_CXX03_LANG

template <class _Tp>
inline
__val_expr<__indirect_expr<const valarray<_Tp>&> >
valarray<_Tp>::operator[](gslice&& __gs) const
{
    return __val_expr<__indirect_expr<const valarray&> >(__indirect_expr<const valarray&>(move(__gs.__1d_), *this));
}

template <class _Tp>
inline
gslice_array<_Tp>
valarray<_Tp>::operator[](gslice&& __gs)
{
    return gslice_array<value_type>(move(__gs), *this);
}

#endif  // _LIBCPP_CXX03_LANG

template <class _Tp>
inline
__val_expr<__mask_expr<const valarray<_Tp>&> >
valarray<_Tp>::operator[](const valarray<bool>& __vb) const
{
    return __val_expr<__mask_expr<const valarray&> >(__mask_expr<const valarray&>(__vb, *this));
}

template <class _Tp>
inline
mask_array<_Tp>
valarray<_Tp>::operator[](const valarray<bool>& __vb)
{
    return mask_array<value_type>(__vb, *this);
}

#ifndef _LIBCPP_CXX03_LANG

template <class _Tp>
inline
__val_expr<__mask_expr<const valarray<_Tp>&> >
valarray<_Tp>::operator[](valarray<bool>&& __vb) const
{
    return __val_expr<__mask_expr<const valarray&> >(__mask_expr<const valarray&>(move(__vb), *this));
}

template <class _Tp>
inline
mask_array<_Tp>
valarray<_Tp>::operator[](valarray<bool>&& __vb)
{
    return mask_array<value_type>(move(__vb), *this);
}

#endif  // _LIBCPP_CXX03_LANG

template <class _Tp>
inline
__val_expr<__indirect_expr<const valarray<_Tp>&> >
valarray<_Tp>::operator[](const valarray<size_t>& __vs) const
{
    return __val_expr<__indirect_expr<const valarray&> >(__indirect_expr<const valarray&>(__vs, *this));
}

template <class _Tp>
inline
indirect_array<_Tp>
valarray<_Tp>::operator[](const valarray<size_t>& __vs)
{
    return indirect_array<value_type>(__vs, *this);
}

#ifndef _LIBCPP_CXX03_LANG

template <class _Tp>
inline
__val_expr<__indirect_expr<const valarray<_Tp>&> >
valarray<_Tp>::operator[](valarray<size_t>&& __vs) const
{
    return __val_expr<__indirect_expr<const valarray&> >(__indirect_expr<const valarray&>(move(__vs), *this));
}

template <class _Tp>
inline
indirect_array<_Tp>
valarray<_Tp>::operator[](valarray<size_t>&& __vs)
{
    return indirect_array<value_type>(move(__vs), *this);
}

#endif  // _LIBCPP_CXX03_LANG

template <class _Tp>
valarray<_Tp>
valarray<_Tp>::operator+() const
{
    valarray<value_type> __r;
    size_t __n = size();
    if (__n)
    {
        __r.__begin_ =
            __r.__end_ =
                static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
        for (const value_type* __p = __begin_; __n; ++__r.__end_, ++__p, --__n)
            ::new (__r.__end_) value_type(+*__p);
    }
    return __r;
}

template <class _Tp>
valarray<_Tp>
valarray<_Tp>::operator-() const
{
    valarray<value_type> __r;
    size_t __n = size();
    if (__n)
    {
        __r.__begin_ =
            __r.__end_ =
                static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
        for (const value_type* __p = __begin_; __n; ++__r.__end_, ++__p, --__n)
            ::new (__r.__end_) value_type(-*__p);
    }
    return __r;
}

template <class _Tp>
valarray<_Tp>
valarray<_Tp>::operator~() const
{
    valarray<value_type> __r;
    size_t __n = size();
    if (__n)
    {
        __r.__begin_ =
            __r.__end_ =
                static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
        for (const value_type* __p = __begin_; __n; ++__r.__end_, ++__p, --__n)
            ::new (__r.__end_) value_type(~*__p);
    }
    return __r;
}

template <class _Tp>
valarray<bool>
valarray<_Tp>::operator!() const
{
    valarray<bool> __r;
    size_t __n = size();
    if (__n)
    {
        __r.__begin_ =
            __r.__end_ =
                static_cast<bool*>(_VSTD::__libcpp_allocate(__n * sizeof(bool), __alignof(bool)));
        for (const value_type* __p = __begin_; __n; ++__r.__end_, ++__p, --__n)
            ::new (__r.__end_) bool(!*__p);
    }
    return __r;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator*=(const value_type& __x)
{
    for (value_type* __p = __begin_; __p != __end_; ++__p)
        *__p *= __x;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator/=(const value_type& __x)
{
    for (value_type* __p = __begin_; __p != __end_; ++__p)
        *__p /= __x;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator%=(const value_type& __x)
{
    for (value_type* __p = __begin_; __p != __end_; ++__p)
        *__p %= __x;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator+=(const value_type& __x)
{
    for (value_type* __p = __begin_; __p != __end_; ++__p)
        *__p += __x;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator-=(const value_type& __x)
{
    for (value_type* __p = __begin_; __p != __end_; ++__p)
        *__p -= __x;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator^=(const value_type& __x)
{
    for (value_type* __p = __begin_; __p != __end_; ++__p)
        *__p ^= __x;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator&=(const value_type& __x)
{
    for (value_type* __p = __begin_; __p != __end_; ++__p)
        *__p &= __x;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator|=(const value_type& __x)
{
    for (value_type* __p = __begin_; __p != __end_; ++__p)
        *__p |= __x;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator<<=(const value_type& __x)
{
    for (value_type* __p = __begin_; __p != __end_; ++__p)
        *__p <<= __x;
    return *this;
}

template <class _Tp>
inline
valarray<_Tp>&
valarray<_Tp>::operator>>=(const value_type& __x)
{
    for (value_type* __p = __begin_; __p != __end_; ++__p)
        *__p >>= __x;
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    valarray<_Tp>&
>::type
valarray<_Tp>::operator*=(const _Expr& __v)
{
    size_t __i = 0;
    for (value_type* __t = __begin_; __t != __end_ ; ++__t, ++__i)
        *__t *= __v[__i];
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    valarray<_Tp>&
>::type
valarray<_Tp>::operator/=(const _Expr& __v)
{
    size_t __i = 0;
    for (value_type* __t = __begin_; __t != __end_ ; ++__t, ++__i)
        *__t /= __v[__i];
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    valarray<_Tp>&
>::type
valarray<_Tp>::operator%=(const _Expr& __v)
{
    size_t __i = 0;
    for (value_type* __t = __begin_; __t != __end_ ; ++__t, ++__i)
        *__t %= __v[__i];
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    valarray<_Tp>&
>::type
valarray<_Tp>::operator+=(const _Expr& __v)
{
    size_t __i = 0;
    for (value_type* __t = __begin_; __t != __end_ ; ++__t, ++__i)
        *__t += __v[__i];
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    valarray<_Tp>&
>::type
valarray<_Tp>::operator-=(const _Expr& __v)
{
    size_t __i = 0;
    for (value_type* __t = __begin_; __t != __end_ ; ++__t, ++__i)
        *__t -= __v[__i];
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    valarray<_Tp>&
>::type
valarray<_Tp>::operator^=(const _Expr& __v)
{
    size_t __i = 0;
    for (value_type* __t = __begin_; __t != __end_ ; ++__t, ++__i)
        *__t ^= __v[__i];
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    valarray<_Tp>&
>::type
valarray<_Tp>::operator|=(const _Expr& __v)
{
    size_t __i = 0;
    for (value_type* __t = __begin_; __t != __end_ ; ++__t, ++__i)
        *__t |= __v[__i];
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    valarray<_Tp>&
>::type
valarray<_Tp>::operator&=(const _Expr& __v)
{
    size_t __i = 0;
    for (value_type* __t = __begin_; __t != __end_ ; ++__t, ++__i)
        *__t &= __v[__i];
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    valarray<_Tp>&
>::type
valarray<_Tp>::operator<<=(const _Expr& __v)
{
    size_t __i = 0;
    for (value_type* __t = __begin_; __t != __end_ ; ++__t, ++__i)
        *__t <<= __v[__i];
    return *this;
}

template <class _Tp>
template <class _Expr>
inline
typename enable_if
<
    __is_val_expr<_Expr>::value,
    valarray<_Tp>&
>::type
valarray<_Tp>::operator>>=(const _Expr& __v)
{
    size_t __i = 0;
    for (value_type* __t = __begin_; __t != __end_ ; ++__t, ++__i)
        *__t >>= __v[__i];
    return *this;
}

template <class _Tp>
inline
void
valarray<_Tp>::swap(valarray& __v) _NOEXCEPT
{
    _VSTD::swap(__begin_, __v.__begin_);
    _VSTD::swap(__end_, __v.__end_);
}

template <class _Tp>
inline
_Tp
valarray<_Tp>::sum() const
{
    if (__begin_ == __end_)
        return value_type();
    const value_type* __p = __begin_;
    _Tp __r = *__p;
    for (++__p; __p != __end_; ++__p)
        __r += *__p;
    return __r;
}

template <class _Tp>
inline
_Tp
valarray<_Tp>::min() const
{
    if (__begin_ == __end_)
        return value_type();
    return *_VSTD::min_element(__begin_, __end_);
}

template <class _Tp>
inline
_Tp
valarray<_Tp>::max() const
{
    if (__begin_ == __end_)
        return value_type();
    return *_VSTD::max_element(__begin_, __end_);
}

template <class _Tp>
valarray<_Tp>
valarray<_Tp>::shift(int __i) const
{
    valarray<value_type> __r;
    size_t __n = size();
    if (__n)
    {
        __r.__begin_ =
            __r.__end_ =
                static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
        const value_type* __sb;
        value_type* __tb;
        value_type* __te;
        if (__i >= 0)
        {
            __i = _VSTD::min(__i, static_cast<int>(__n));
            __sb = __begin_ + __i;
            __tb = __r.__begin_;
            __te = __r.__begin_ + (__n - __i);
        }
        else
        {
            __i = _VSTD::min(-__i, static_cast<int>(__n));
            __sb = __begin_;
            __tb = __r.__begin_ + __i;
            __te = __r.__begin_ + __n;
        }
        for (; __r.__end_ != __tb; ++__r.__end_)
            ::new (__r.__end_) value_type();
        for (; __r.__end_ != __te; ++__r.__end_, ++__sb)
            ::new (__r.__end_) value_type(*__sb);
        for (__te = __r.__begin_ + __n; __r.__end_ != __te; ++__r.__end_)
            ::new (__r.__end_) value_type();
    }
    return __r;
}

template <class _Tp>
valarray<_Tp>
valarray<_Tp>::cshift(int __i) const
{
    valarray<value_type> __r;
    size_t __n = size();
    if (__n)
    {
        __r.__begin_ =
            __r.__end_ =
                static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
        __i %= static_cast<int>(__n);
        const value_type* __m = __i >= 0 ? __begin_ + __i : __end_ + __i;
        for (const value_type* __s = __m; __s != __end_; ++__r.__end_, ++__s)
            ::new (__r.__end_) value_type(*__s);
        for (const value_type* __s = __begin_; __s != __m; ++__r.__end_, ++__s)
            ::new (__r.__end_) value_type(*__s);
    }
    return __r;
}

template <class _Tp>
valarray<_Tp>
valarray<_Tp>::apply(value_type __f(value_type)) const
{
    valarray<value_type> __r;
    size_t __n = size();
    if (__n)
    {
        __r.__begin_ =
            __r.__end_ =
                static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
        for (const value_type* __p = __begin_; __n; ++__r.__end_, ++__p, --__n)
            ::new (__r.__end_) value_type(__f(*__p));
    }
    return __r;
}

template <class _Tp>
valarray<_Tp>
valarray<_Tp>::apply(value_type __f(const value_type&)) const
{
    valarray<value_type> __r;
    size_t __n = size();
    if (__n)
    {
        __r.__begin_ =
            __r.__end_ =
                static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
        for (const value_type* __p = __begin_; __n; ++__r.__end_, ++__p, --__n)
            ::new (__r.__end_) value_type(__f(*__p));
    }
    return __r;
}

template <class _Tp>
void
valarray<_Tp>::__clear()
{
    if (__begin_ != nullptr)
    {
        while (__end_ != __begin_)
            (--__end_)->~value_type();
        _VSTD::__libcpp_deallocate(__begin_, __alignof(value_type));
        __begin_ = __end_ = nullptr;
    }
}

template <class _Tp>
void
valarray<_Tp>::resize(size_t __n, value_type __x)
{
    __clear();
    if (__n)
    {
        __begin_ = __end_ = static_cast<value_type*>(
           _VSTD::__libcpp_allocate(__n * sizeof(value_type), __alignof(value_type)));
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            for (; __n; --__n, ++__end_)
                ::new (__end_) value_type(__x);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __clear();
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
void
swap(valarray<_Tp>& __x, valarray<_Tp>& __y) _NOEXCEPT
{
    __x.swap(__y);
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<multiplies<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator*(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<multiplies<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(multiplies<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<multiplies<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator*(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<multiplies<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(multiplies<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<multiplies<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator*(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<multiplies<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(multiplies<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<divides<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator/(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<divides<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(divides<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<divides<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator/(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<divides<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(divides<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<divides<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator/(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<divides<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(divides<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<modulus<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator%(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<modulus<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(modulus<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<modulus<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator%(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<modulus<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(modulus<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<modulus<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator%(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<modulus<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(modulus<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<plus<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator+(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<plus<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(plus<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<plus<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator+(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<plus<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(plus<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<plus<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator+(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<plus<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(plus<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<minus<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator-(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<minus<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(minus<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<minus<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator-(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<minus<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(minus<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<minus<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator-(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<minus<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(minus<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<bit_xor<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator^(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<bit_xor<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(bit_xor<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<bit_xor<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator^(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<bit_xor<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(bit_xor<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<bit_xor<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator^(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<bit_xor<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(bit_xor<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<bit_and<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator&(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<bit_and<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(bit_and<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<bit_and<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator&(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<bit_and<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(bit_and<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<bit_and<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator&(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<bit_and<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(bit_and<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<bit_or<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator|(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<bit_or<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(bit_or<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<bit_or<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator|(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<bit_or<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(bit_or<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<bit_or<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator|(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<bit_or<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(bit_or<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<__bit_shift_left<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator<<(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<__bit_shift_left<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(__bit_shift_left<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<__bit_shift_left<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator<<(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<__bit_shift_left<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(__bit_shift_left<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<__bit_shift_left<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator<<(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<__bit_shift_left<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__bit_shift_left<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<__bit_shift_right<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator>>(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<__bit_shift_right<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(__bit_shift_right<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<__bit_shift_right<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator>>(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<__bit_shift_right<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(__bit_shift_right<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<__bit_shift_right<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator>>(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<__bit_shift_right<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__bit_shift_right<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<logical_and<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator&&(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<logical_and<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(logical_and<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<logical_and<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator&&(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<logical_and<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(logical_and<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<logical_and<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator&&(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<logical_and<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(logical_and<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<logical_or<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator||(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<logical_or<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(logical_or<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<logical_or<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator||(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<logical_or<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(logical_or<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<logical_or<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator||(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<logical_or<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(logical_or<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<equal_to<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator==(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<equal_to<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(equal_to<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<equal_to<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator==(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<equal_to<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(equal_to<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<equal_to<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator==(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<equal_to<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(equal_to<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<not_equal_to<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator!=(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<not_equal_to<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(not_equal_to<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<not_equal_to<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator!=(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<not_equal_to<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(not_equal_to<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<not_equal_to<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator!=(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<not_equal_to<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(not_equal_to<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<less<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator<(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<less<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(less<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<less<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator<(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<less<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(less<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<less<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator<(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<less<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(less<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<greater<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator>(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<greater<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(greater<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<greater<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator>(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<greater<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(greater<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<greater<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator>(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<greater<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(greater<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<less_equal<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator<=(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<less_equal<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(less_equal<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<less_equal<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator<=(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<less_equal<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(less_equal<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<less_equal<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator<=(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<less_equal<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(less_equal<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<greater_equal<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
operator>=(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<greater_equal<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(greater_equal<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<greater_equal<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
operator>=(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<greater_equal<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(greater_equal<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<greater_equal<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
operator>=(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<greater_equal<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(greater_equal<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__abs_expr<typename _Expr::value_type>, _Expr> >
>::type
abs(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__abs_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__abs_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__acos_expr<typename _Expr::value_type>, _Expr> >
>::type
acos(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__acos_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__acos_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__asin_expr<typename _Expr::value_type>, _Expr> >
>::type
asin(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__asin_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__asin_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__atan_expr<typename _Expr::value_type>, _Expr> >
>::type
atan(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__atan_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__atan_expr<value_type>(), __x));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<__atan2_expr<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
atan2(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<__atan2_expr<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(__atan2_expr<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<__atan2_expr<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
atan2(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<__atan2_expr<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(__atan2_expr<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<__atan2_expr<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
atan2(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<__atan2_expr<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__atan2_expr<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__cos_expr<typename _Expr::value_type>, _Expr> >
>::type
cos(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__cos_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__cos_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__cosh_expr<typename _Expr::value_type>, _Expr> >
>::type
cosh(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__cosh_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__cosh_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__exp_expr<typename _Expr::value_type>, _Expr> >
>::type
exp(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__exp_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__exp_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__log_expr<typename _Expr::value_type>, _Expr> >
>::type
log(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__log_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__log_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__log10_expr<typename _Expr::value_type>, _Expr> >
>::type
log10(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__log10_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__log10_expr<value_type>(), __x));
}

template<class _Expr1, class _Expr2>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr1>::value && __is_val_expr<_Expr2>::value,
    __val_expr<_BinaryOp<__pow_expr<typename _Expr1::value_type>, _Expr1, _Expr2> >
>::type
pow(const _Expr1& __x, const _Expr2& __y)
{
    typedef typename _Expr1::value_type value_type;
    typedef _BinaryOp<__pow_expr<value_type>, _Expr1, _Expr2> _Op;
    return __val_expr<_Op>(_Op(__pow_expr<value_type>(), __x, __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<__pow_expr<typename _Expr::value_type>,
               _Expr, __scalar_expr<typename _Expr::value_type> > >
>::type
pow(const _Expr& __x, const typename _Expr::value_type& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<__pow_expr<value_type>, _Expr, __scalar_expr<value_type> > _Op;
    return __val_expr<_Op>(_Op(__pow_expr<value_type>(),
                           __x, __scalar_expr<value_type>(__y, __x.size())));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_BinaryOp<__pow_expr<typename _Expr::value_type>,
               __scalar_expr<typename _Expr::value_type>, _Expr> >
>::type
pow(const typename _Expr::value_type& __x, const _Expr& __y)
{
    typedef typename _Expr::value_type value_type;
    typedef _BinaryOp<__pow_expr<value_type>, __scalar_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__pow_expr<value_type>(),
                           __scalar_expr<value_type>(__x, __y.size()), __y));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__sin_expr<typename _Expr::value_type>, _Expr> >
>::type
sin(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__sin_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__sin_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__sinh_expr<typename _Expr::value_type>, _Expr> >
>::type
sinh(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__sinh_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__sinh_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__sqrt_expr<typename _Expr::value_type>, _Expr> >
>::type
sqrt(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__sqrt_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__sqrt_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__tan_expr<typename _Expr::value_type>, _Expr> >
>::type
tan(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__tan_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__tan_expr<value_type>(), __x));
}

template<class _Expr>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if
<
    __is_val_expr<_Expr>::value,
    __val_expr<_UnaryOp<__tanh_expr<typename _Expr::value_type>, _Expr> >
>::type
tanh(const _Expr& __x)
{
    typedef typename _Expr::value_type value_type;
    typedef _UnaryOp<__tanh_expr<value_type>, _Expr> _Op;
    return __val_expr<_Op>(_Op(__tanh_expr<value_type>(), __x));
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
begin(valarray<_Tp>& __v)
{
    return __v.__begin_;
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
const _Tp*
begin(const valarray<_Tp>& __v)
{
    return __v.__begin_;
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp*
end(valarray<_Tp>& __v)
{
    return __v.__end_;
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
const _Tp*
end(const valarray<_Tp>& __v)
{
    return __v.__end_;
}

_LIBCPP_END_NAMESPACE_STD

_LIBCPP_POP_MACROS

#endif  // _LIBCPP_VALARRAY
