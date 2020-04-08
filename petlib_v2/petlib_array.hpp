#ifndef PETLIB_ARRAY_HPP
#define PETLIB_ARRAY_HPP
#include <petlib_iterator.hpp>
#include <petlib_array_expr.hpp>
#include <petlib_alloc.hpp>
#include <iterator>
#include <memory>

namespace petlib {

template < typename Tp > class Array;
template < typename Tp > class SubArray;
template < typename Tp > class SliceArray;

template < typename Tp >
class Array: public ArrayBase< Tp , Array<Tp> > {
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


    Array():m_ptr(nullptr),m_size(0),m_alloc() {}

    Array(size_type n):m_ptr(nullptr),m_size(n),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
    }

    Array(const_pointer p,size_type n):m_ptr(nullptr),m_size(n),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
        Copy< iterator, const_pointer >(m_ptr,p,m_size);
    }
    
    Array(const Array& a):m_ptr(nullptr),m_size(a.size()),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
        Copy< iterator, const_pointer >(m_ptr,a.cbegin(),m_size);
    }

    Array(const SubArray<Tp>& a):m_ptr(nullptr),m_size(a.size()),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
        Copy< iterator, const_pointer >(m_ptr,a.cbegin(),m_size);
    }

    Array(const SliceArray<Tp>& a):m_ptr(nullptr),m_size(a.size()),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
        Copy< iterator, const SliceIterator<Tp> >(m_ptr,a.cbegin(),m_size);
    }
    
    template < class X >
    Array(const ArrayExpr<Tp,X>& a):m_ptr(nullptr),m_size(a.size()),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
        ExprCopy< Tp, iterator, X  >(m_ptr,a,m_size);
    }

    template < class iter_t >
    Array( iter_t iter, iter_t last):m_ptr(nullptr),m_size((last-iter)),m_alloc() {
        m_ptr = m_alloc.allocate(m_size);
        Copy< pointer, iter_t  >(m_ptr,iter,m_size);        
    } 

    ~Array() {
        m_alloc.deallocate(m_ptr,m_size);
    }
    
    SubArray< value_type > operator()(Range& r) noexcept {
        return SubArray< value_type >(m_ptr + r.offset(),r.size());
    }
    SliceArray< value_type > operator()(Slice& s) noexcept {
        return SliceArray< value_type >(m_ptr + s.offset(),s.size(),s.stride());
    } 
    Array operator()(const Range& r) const noexcept { return Array(m_ptr+r.offset(),r.size());}    

    Array operator()(const Slice& s) const noexcept { 
        petlib::SliceIterator< value_type > s1(m_ptr+s.offset(),s.stride());
        petlib::SliceIterator< value_type > s2(m_ptr+s.offset+s.stride()*s.size(),s.stride());
        return Array::Array< SliceIterator< value_type > >(s1,s2);
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
    
    Array& operator=(const Array& a) noexcept {
        petlib::Copy< iterator, const_iterator >(begin(),end(),a.cbegin());
        return *this;
    }
    Array& operator=(const SubArray< value_type >& a) noexcept {
        petlib::Copy< iterator, const_iterator >(begin(),end(),a.cbegin());
        return *this;
    }
    Array& operator=(const SliceArray< value_type >& a) noexcept {
        petlib::Copy< iterator, const SliceIterator< value_type> >(begin(),end(),a.cbegin());
        return *this;
    }

    template < class X >
    Array& operator=(const ArrayExpr< value_type, X >& x) noexcept {
        petlib::ExprCopy< value_type, iterator, X >(begin(),x,m_size);
        return *this;
    }

    Array& operator=(const_reference v) noexcept {
        petlib::Fill< iterator, const_reference >(begin(),v,m_size);
        return *this;
    }

#define PETLIB_MAKE_OP(NAME,SYM)\
    Array& operator SYM (const Array& a) noexcept {\
        NAME ## Copy< iterator, const_iterator >(begin(),end(),a.cbegin());\
        return *this;\
    }\
     \
    Array& operator SYM (const SubArray< value_type >& a) noexcept {\
        NAME ## Copy< iterator, const_iterator >(begin(),end(),a.cbegin()); \
        return *this; \
    } \
      \
    Array& operator SYM (const SliceArray< value_type >& a) noexcept {\
        NAME ## Copy< iterator, const SliceIterator< value_type> >(begin(),end(),a.cbegin());\
        return *this;\
    }\
     \
    template < class X > \
    Array& operator SYM (const ArrayExpr< value_type, X >& x) noexcept {\
        Expr ## NAME ## Copy< value_type, iterator, X >(begin(),x,m_size);\
        return *this;\
    }\
     \
    Array& operator SYM (const_reference v) noexcept {\
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
  
    constexpr value_type Dot(const Array& a) const noexcept {
        return petlib::Dot< value_type, const_iterator, const_iterator >(begin(),a.cbegin(),m_size);
    }
    constexpr value_type Dot(const SubArray<value_type>& a) const noexcept {
        return petlib::Dot< value_type, const_iterator, const_iterator >(begin(),a.cbegin(),m_size);
    }
    constexpr value_type Dot(const SliceArray<value_type>& a) const noexcept {
        return petlib::Dot< value_type, const_iterator, const SliceIterator<value_type> >(begin(),a.cbegin(),m_size);
    }
    constexpr void Swap(Array& a) noexcept {
        return petlib::Swap< iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(SubArray<value_type>& a) noexcept {
        return petlib::Swap< iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(SliceArray<value_type>& a) noexcept {
        return petlib::Swap< iterator, SliceIterator<value_type> >(begin(),a.begin(),m_size);
    }
    constexpr void Rot(Array& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, iterator >(begin(),a.begin(),cs,sn,m_size);
    }
    constexpr void Rot(SubArray<value_type>& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, iterator >(begin(),a.begin(),cs,sn,m_size);
    }
    constexpr void Rot(SliceArray<value_type>& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, SliceIterator<value_type> >(begin(),a.begin(),cs,sn,m_size);
    }    
private:
    pointer m_ptr;
    size_type m_size;
    allocator_type m_alloc;
};

template < typename Tp >
class SubArray:  public ArrayBase< Tp , SubArray<Tp> > {
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

    
    SubArray(const_pointer p,size_type n):m_ptr(p),m_size(n) {}
    
    explicit SubArray(Array<value_type>& a):m_ptr(a.data()),m_size(a.size()) {}

    ~SubArray() {
        m_ptr = nullptr;
    }
    
    SubArray operator()(Range& r) noexcept {
        return SubArray(m_ptr + r.offset(),r.size());
    }
    SliceArray< value_type > operator()(Slice& s) noexcept {
        return SubArray(m_ptr + s.offset(),s.size(),s.stride());
    } 
    Array< value_type > operator()(const Range& r) const noexcept { return Array< value_type >(m_ptr+r.offset(),r.size());}    
    Array< value_type > operator()(const Slice& s) const noexcept { 
      return Array< value_type >::Array< SliceIterator<value_type>(SliceIterator<value_type>(m_ptr+s.offset(),s.stride()),
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
    
    SubArray& operator=(const Array< value_type >& a) noexcept {
        Copy< iterator, const_iterator >(begin(),end(),a.cbegin());
        return *this;
    }
    SubArray& operator=(const SubArray& a) noexcept {
        Copy< iterator, const_iterator >(begin(),end(),a.cbegin());
        return *this;
    }
    SubArray& operator=(const SliceArray< value_type >& a) noexcept {
        Copy< iterator, const SliceIterator< value_type> >(begin(),end(),a.cbegin());
        return *this;
    }
    template < class X >
    SubArray& operator=(const ArrayExpr< value_type, X >& x) noexcept {
        ExprCopy< value_type, iterator, X >(begin(),end(),x);
        return *this;
    }

    SubArray& operator=(const_reference v) noexcept {
        Fill< iterator, value_type >(begin(),v,m_size);
        return *this;
    }

#define PETLIB_MAKE_OP(NAME,SYM)\
    SubArray& operator SYM (const SubArray& a) noexcept {\
        NAME ## Copy< iterator, const_iterator >(begin(),end(),a.cbegin());\
        return *this;\
    }\
     \
    SubArray& operator SYM (const Array< value_type >& a) noexcept {\
        NAME ## Copy< iterator, const_iterator >(begin(),end(),a.cbegin()); \
        return *this; \
    } \
     \
    SubArray& operator SYM (const SliceArray< value_type >& a) noexcept {\
        NAME ## Copy< iterator, const SliceIterator< value_type> >(begin(),end(),a.cbegin());\
        return *this;\
    }\
     \
    template < class X > \
    SubArray& operator SYM (const ArrayExpr< value_type, X >& x) noexcept {\
        Expr ## NAME ## Copy< value_type, iterator, X >(begin(),x,m_size);\
        return *this;\
    }\
     \
    SubArray& operator SYM (const_reference v) noexcept {\
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
  
    constexpr value_type Dot(const SubArray& a) const noexcept {
        return petlib::Dot< value_type, const_iterator, const_iterator >(cbegin(),a.cbegin(),m_size);
    }
    constexpr value_type Dot(const Array<value_type>& a) const noexcept {
        return petlib::Dot< value_type, const_iterator, const_iterator >(cbegin(),a.cbegin(),m_size);
    }
    constexpr value_type Dot(const SliceArray<value_type>& a) const noexcept {
        return petlib::Dot< value_type, const_iterator, const SliceIterator<value_type> >(cbegin(),a.cbegin(),m_size);
    }
    constexpr void Swap(SubArray& a) noexcept {
        return petlib::Swap< iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(Array<value_type>& a) noexcept {
        return petlib::Swap< iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(SliceArray<value_type>& a) noexcept {
        return petlib::Swap< iterator, SliceIterator<value_type> >(begin(),a.begin(),m_size);
    }
    constexpr void Rot(SubArray& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, iterator >(begin(),a.begin(),cs,sn,m_size);
    }
    constexpr void Rot(Array<value_type>& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, iterator >(begin(),a.begin(),cs,sn,m_size);
    }
    constexpr void Rot(SliceArray<value_type>& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, SliceIterator<value_type> >(begin(),a.begin(),cs,sn,m_size);
    }    
private:
    pointer m_ptr;
    size_type m_size;
};

template < typename Tp >
class SliceArray:  public ArrayBase< Tp , SliceArray<Tp> > {
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

    SliceArray(const_pointer p,size_type n,size_type s):m_ptr(p),m_size(n),m_str(s) {}
    
    explicit SliceArray(Array<value_type>& a):m_ptr(a.data()),m_size(a.size()),m_str(1) {}

    ~SliceArray() {
        m_ptr = nullptr;
    }
    
    SliceArray operator()(Range& r) noexcept {
        return SliceArray(m_ptr + r.offset()*m_str,r.size(),m_str);
    }
    SliceArray< value_type > operator()(Slice& s) noexcept {
        return SliceArray(m_ptr + s.offset()*m_str,s.size(),m_str*s.stride());
    } 
    Array< value_type > operator()(const Range& r) const noexcept { 
        return Array< value_type >::Array(
               SliceIterator< value_type >(m_ptr + r.offset()*m_str, m_str),
               SliceIterator< value_type >(m_ptr + (r.offset()+r.size())*m_str,m_str)
               );
    }    
    Array< value_type > operator()(const Slice& s) const noexcept { 
        return Array< value_type >::Array(
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
    
    SliceArray& operator=(const Array< value_type >& a) noexcept {
        petlib::Copy< iterator, const_pointer >(begin(),end(),a.cbegin());
        return *this;
    }

    SliceArray& operator=(const SubArray< value_type >& a) noexcept {
        petlib::Copy< iterator, pointer >(begin(),end(),a.cbegin());
        return *this;
    }

    SliceArray& operator=(const SliceArray< value_type >& a) noexcept {
        petlib::Copy< iterator,  iterator >(begin(),end(),a.cbegin());
        return *this;
    }

    template < class X >
    SliceArray& operator=(const ArrayExpr< value_type, X >& x) noexcept {
        ExprCopy< value_type, iterator, X >(begin(),end(),x);
        return *this;
    }

    SliceArray& operator=(const_reference v) noexcept {
        Fill< value_type, iterator >(begin(),v,m_size);
        return *this;
    }

#define PETLIB_MAKE_OP(NAME,SYM)\
    SliceArray& operator SYM (const SubArray< value_type >& a) noexcept {\
        NAME ## Copy< iterator, pointer >(begin(),end(),a.cbegin());\
        return *this;\
    }\
     \
    SliceArray& operator SYM (const Array< value_type >& a) noexcept {\
        NAME ## Copy< iterator, pointer >(begin(),end(),a.cbegin()); \
        return *this; \
    } \
      \
    SliceArray& operator SYM (const SliceArray< value_type >& a) noexcept {\
        NAME ## Copy< iterator, iterator >(begin(),end(),a.cbegin());\
        return *this;\
    }\
     \
    template < class X > \
    SliceArray& operator SYM (const ArrayExpr< value_type, X >& x) noexcept {\
        Expr ## NAME ## Copy< value_type, iterator, X >(begin(),x,m_size);\
        return *this;\
    }\
     \
    SliceArray& operator SYM (const_reference v) noexcept {\
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
  
    constexpr value_type Dot(const SliceArray& a) const noexcept {
        return petlib::Dot< value_type, iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr value_type Dot(const Array<value_type>& a) const noexcept {
        return petlib::Dot< value_type, iterator, pointer >(begin(),a.begin(),m_size);
    }
    constexpr value_type Dot(const SubArray<value_type>& a) const noexcept {
        return petlib::Dot< value_type, iterator, pointer >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(SliceArray& a) noexcept {
        return petlib::Swap< iterator, iterator >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(Array<value_type>& a) noexcept {
        return petlib::Swap< iterator, pointer >(begin(),a.begin(),m_size);
    }
    constexpr void Swap(SubArray<value_type>& a) noexcept {
        return petlib::Swap< iterator, pointer >(begin(),a.begin(),m_size);
    }
    constexpr void Rot(SliceArray& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, iterator >(begin(),a.begin(),cs,sn,m_size);
    }
    constexpr void Rot(Array<value_type>& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, pointer >(begin(),a.begin(),cs,sn,m_size);
    }
    constexpr void Rot(SubArray<value_type>& a, const_reference cs, const_reference sn) noexcept {
        return petlib::Rot< value_type, iterator, pointer >(begin(),a.begin(),cs,sn,m_size);
    }    
private:
    pointer m_ptr;
    size_type m_size;
    size_type m_str;
};


}
#endif