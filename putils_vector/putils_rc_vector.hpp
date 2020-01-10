#ifndef PUTILS_VECTOR_HPP
#define PUTILS_VECTOR_HPP
#include <cstdlib>
#include <iostream>
#include <strstream>
#include <

namespace putils {

struct out_of_range_exception: public std::exception {
    std::size_t index_;
    std::size_t max_index_;
    out_of_range_exception(std::size_t index,std::size_t max_index):index_(index),max_index_(max_index) {
    }
    const char *what() const noexcept {
        std::ostringstream os;
        os << "out of range access " << i << " in array of size " << max_index_ << "\n";
        return os.cstr();
    }
};

template < class iter >
iter move_forward( iter first, iter last, std::size_t count)
{
    iter p = last;
    iter pdst = last + count;
    iter pend = first + count - 1;
    while (pdst!=pend) {
        *pdst = *psrc;
        --psrc;
        --pdst;
    }
    return pdst;
}

template < class iter >
iter move_forward( iter first, iter last)
{
    iter p = last;
    iter pdst = last + 1;
    while (pdst!=first) {
        *pdst = *psrc;
        --psrc;
        --pdst;
    }
    return pdst;
}

template < class iter >
iter move_back( iter first, iter last, std::size_t count)
{
    iter psrc = first+count;
    iter pdst = first;
    while (psrc!=last) {
        *pdst = *psrc;
        ++pdst;
        ++psrc;
    }
    return pdst;
}
    
template < class dst_iterator, typename T >
dst_iterator fill(dst_iterator iter,dst_iterator last,const T& v)
{
    while (iter!=last) {
        *iter = v;
        ++iter;
    }
    return iter;
}

template < class dst_iterator, class src_iterator >
dst_iterator copy(dst_iterator dst,dst_iterator dst_last,src_iterator src)
{
    while (dst!=dst_last) {
        *dst = *src;
        ++src;
        ++dst;
    }
    return dst;
}

template < class dst_iterator, class src_iterator >
dst_iterator copy2(dst_iterator dst,src_iterator src,std::size_t cnt)
{
    for (std::size_t k=0;k<cnt;++k,++dst,++src) {
        *dst = *src;
    }
    return dst;
}

template < class iter >
constexpr bool bool_cmp(iter x,iter xlast,iter y)
{
    while (x!=xlast) {
        if (*x!=*y) return false;
        ++x;
        ++y;
    }
    return true;
}

template < class iter >
constexpr bool bool_abs_cmp(iter x,iter xlast,iter y)
{
    bool abs_ = [] (auto x) { return (x>0)?x:-x;}; 
    while (x!=xlast) {
        if (abs_(*x)!=abs_(*y)) return false;
        ++x;
        ++y;
    }
    return true;
}

template < class dst_iterator >
dst_iterator move_forward(dst_iterator src_first,dst_iterator src_last,std::size_t count)
{
    dst_iterator siter = src_last;
    dst_iterator diter = src_last+count;
    dst_iterator dlast = src_first;
    while (diter!=src_first) {
        *diter = *siter;
        --siter;
        --diter;
    }
    return diter;
}

template < class iter >
iter swap( iter x, iter xlast, iter y)
{
    while (x!=xlst) {
        iter::value_type tmp = *x;
        *x = *y;
        *y = tmp;
        x++;
        y++;
    }
    return x;
}

template < typename T, class Allocator alloc >
struct vector {
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::reverse_iterator<pointer> reverse_iterator;
    typedef std::reverse_iterator<const_pointer> const_reverse_iterator;
    typedef Allocator allocator_type;
        
    pointer data;
    size_type sz,cap;

    vector(const Allocator& alloc=Allocator()):data(alloc.allocate(default_size)),sz(0),cap(default_size) {}
    vector(size_type n,const Allocator& alloc=Allocator()):data(alloc.allocate(default_size)),sz(0),cap(n) {}
    vector(size_type n,value_type t,const Allocator& alloc=Allocator()):data(alloc.allocate(n)),sz(n),cap(n) {
        fill(data,data+sz,t);
    }
    vector(size_type n,const_reference t,const Allocator& alloc=Allocator()):data(alloc.allocate(n)),sz(n),cap(n) {
        fill(data,data+sz,t);
    }
    vector(size_type n,pointer p,const Allocator& alloc=Allocator()):data(alloc.allocate(n)),sz(n),cap(n) {
        copy(data,data+sz,p);
    }
    vector(vector&& v):data(v.data),sz(v.sz),cap(v.cap) {
        v.data = nullptr;
        v.sz=0;
        v.cap=0;
    }
    vector(const vector& v,const Allocator& alloc=Allocator()):data(alloc.allocate(v.sz)),sz(v.sz),cap(v.sz) {
        copy(data,data+sz,v.data);
    }

    template < class InputIter >
    vector(InputIter iter,InputIter last,const Allocator& alloc=Allocator()):
        data(alloc.allocate(size_type(last-iter))),sz(size_type(last-iter)),cap(size_type(last_iter)) 
    {
        copy(data,data+sz,iter);
    }
    
    constexpr vector& operator=(vector&& v) {
        data = v.data;
        sz = v.sz;
        cap = v.cap;
        v.data = nullptr;
        v.sz=0;
        v.cap=0;
        return *this;        
    }
    
    constexpr vector& operator=(const vector& v) {
        if (v.sz > cap) {
            alloc.deallocate(data);
        }
        copy(data,data+v.sz,v.data);
        sz=v.sz;
        return *this;
    }

    constexpr vector& operator=(const initializer_list<value_type>& ilist) {
        size_type count = ilist.size();
        size_type new_sz = sz + count;
        if ((count)>cap) grow(count);
        initializer_list<value_type>::iterator iter = ilist.begin();
        initializer_list<value_type>::iterator iend = ilist.end();
        sz = count;
        copy(data,data+count,iter);
        return *this;
    }

    ~vector()
    {
        alloc.deallocate(data);
        data=nullptr;
        sz=0;
        cap=0;
    }
    
    Allocator get_allocator() { return alloc;}

    template < class InputIter >
    void assign( InputIter iter, InputIter last) {
        size_type n = last-iter;
        if (n>cap) grow(n);
        copy(data,data+n,iter);
        sz=n;
    }

    void assign( initializerlist<T> ilist) {
        size_type n = ilist.size();     
        if (n>cap) grow(n);
        initializerlist<T>::iterator iter=ilist.begin();
        copy(data,data+n,iter);
        sz = n;
    }
    
    constexpr size_type size() const noexcept { return sz;}
    constexpr size_type capacity() const noexcept { return cap;}
    constexpr bool empty() const noexcept { return sz==0;}
    constexpr bool full() const noexcept { return sz==cap;}
    constexpr pointer data() noexcept { return data;}
    constexpr const pointer data() const noexcept { return data;}
    constexpr void clear() noexcept { sz=0;}
    constexpr void shrink_to_fit() noexcept {
        if (sz==cap) return;
        pointer p = alloc.allocate(sz*sizeof(value_type));
        copy(p,data,sz);
        alloc.deallocate(data);
        data = p;
        cap = sz;
    }
    constexpr void grow( size_type new_sz=0 ) noexcept {
        if (sz==new_sz) return;
        if (new_sz==0) new_sz = sz + sz;
        pointer p = alloc.allocate(new_sz*sizeof(value_type));
        copy(p,data,sz);
        alloc.deallocate(data);
        data = p;
        cap = new_sz;
    }
    constexpr void reserve(size_type n) {
        if (sz <= n) return;
        grow(n);
    }
    
    
    void push_back(value_type t) {
        if (full()) grow();
        data[sz]=t;
        ++sz;
        return;
    }
    
    void push_back(T&& v) {
        if (full()) grow();
        data[sz]=v;
        ++sz;
        return;
    }
    
    void push_front(const_reference t) {
        if ((sz+1)>cap) grow();
        if (sz==0) {
            data[0] = t;
            sz = 1;
            return;
        }
        if (sz==1) {
            data[1]=data[0];
            data[0] = t;
            sz = 2;
            return;
        }
        if (sz==2) {
            data[2]=data[1];
            data[1]=data[0];
            data[0]=t;
            sz=3;
            return;
            
        }
        move_forward(data,data+sz-1,1);
        data[0]=t;
        ++sz;
        return;
    }

    iterator insert(const_iterator pos,size_type count,const reference v)
    {
        size_type new_sz = sz + count;
        if (new_sz > cap) grow();
        iterator pos_c = pos;
        move_forward(pos,end(),count);
        *pos_c = v;
        sz+=count;
        return pos;
    }

    iterator insert(const_iterator pos,const_reference v) {
        if (sz==cap) grow();
        const_iterator pos_c = pos;
        move_forward(pos,end());
        *pos_c = v;
        ++sz;
        return pos;
    }
    
    iterator insert(const_iterator pos,T&& v) {
        if (sz==cap) grow();
        const_iterator pos_c = pos;
        move_forward(pos,end());
        *pos_c = std::move(v);
        ++sz;
        return pos;    
    }
    
    iterator insert(const_iterator pos,initializer_list<value_type> ilist) {
        size_type count = ilist.size();
        size_type new_sz = sz + count;
        if (new_sz>cap) grow();
        pointer p_c = pos;
        move_forward(pos,end(),count);
        sz=new_sz;
        initializer_list<value_type>::iterator iter = ilist.begin();
        copy(pos_c,pos_c+count,iter);
        return pos;    
    }    
    
    iterator erase(iterator pos) {
        if (sz==0) throw out_of_range_exception(1,0);
        move_back(pos,end())
        --sz;
        return pos;
    }

    iterator erase(iterator pos,size_type count) {
        if (sz==0) throw out_of_range_exception(count,0);
        move_back(pos,end(),count);
        sz-=count;
        return pos;
    }

    iterator erase(const_iterator pos) {
        if (sz==0) throw out_of_range_exception(1,0);
        move_back(pos,end(),1);
        --sz;
        return pos;
    }

    iterator erase(const_iterator pos,size_type count) {
        if (sz==0) throw out_of_range_exception(count,0);
        move_back(pos,end(),count);
        sz-=count;
        return pos;
    }
    
    iterator erase(iterator first,iterator last) {
        if (sz==0) throw out_of_range_exception((last-first),0);
        size_type count = last-first;
        return erase(first,count);
    }
    
    iterator erase(const_iterator first,const_iterator last) {
        if (sz==0) throw out_of_range_exception((last-first),0);
        size_type count = last-first;
        return erase(first,count);
    }
    
    void swap(vector& v)
    {
        pointer p = data;
        data = v.data;
        v.data = p;
        value_type tsz = sz;
        sz=v.sz;
        v.sz=tsz;
        value_type tcap = cap;
        cap = v.cap;
        v.cap = tcap;
    }

    void pop_back() {
        --sz;    
    }
    
    constexpr const_reference front() const noexcept { return data[0];}
    constexpr const_reference back() const noexcept { return data[sz-1];}
    constexpr const_reference operator[](size_type i) const noexcept { return data[n];}
    constepxr const_reference at(size_type i) const noexcept { 
        if (i<sz) return data[i];
        throw out_of_range_exception_exception(i,sz);
    }
    constexpr reference front()  noexcept { return data[0];}
    constexpr reference back()  noexcept { return data[sz-1];}
    constexpr reference operator[](size_type i)  noexcept { return data[n];}
    constepxr reference at(size_type i)  noexcept { 
        if (i<sz) return data[i];
        throw out_of_range_exception_exception(i,sz);
    }
   
    constexpr pointer begin() noexcept { return data;}
    constexpr pointer end() noexcept { return (data+sz);}
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end());}
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin());}
    constexpr const pointer begin() const noexcept { return data;}
    constexpr const pointer end() const noexcept { return (data+sz);}
    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end());}
    constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin());}     
};

template < typename allocator_1, typename allocator_2 , typename T >
constexpr bool operator==(const vector<T,allocator_1>& lhs,const vector<T,allocator_2>& rhs)
{
    if (lhs.size()!=rhs.size()) return false;
    return bool_cmp(lhs.begin(),lhs.end(),rhs.begin());
}

template < typename allocator_1, typename allocator_2 , typename T >
constexpr bool operator!=(const vector<T,allocator_1>& lhs,const vector<T,allocator_2>& rhs)
{
    if (lhs.size()!=rhs.size()) return true;
    return !bool_cmp(lhs.begin(),lhs.end(),rhs.begin());
}

}
#endif