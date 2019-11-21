#ifndef PETLIB_ARRAY_HPP
#define PETLIB_ARRAY_HPP
#include <petlib_iterator.hpp>
#include <petlib_ops.hpp>
#include <petlib_mem.hpp>

namespace petlib {

struct range {
    typedef std::size_t size_type;
    size_type off_,len_;

    range(size_type offset_,size_type length_):off_(offset_),len_(length_) {}
    
    constexpr size_type offset() const noexcept { return off_;}
    constexpr size_type size() const noexcept { return len_;}
    constexpr size_type end_size() const noexcept { return off_+len_;};
    constexpr size_type stride() const noexcept { return 1;}
};

struct slice {
    typedef std::size_t size_type;
    size_type off_,len_,str_;

    slice(size_type offset_,size_type length_,size_type stride_):off_(offset_),len_(length_),str_(stride_) {}
    
    constexpr size_type offset() const noexcept { return off_;}
    constexpr size_type size() const noexcept { return len_;}
    constexpr size_type end_size() const noexcept { return (off_+len_*str_);};
    constexpr size_type stride() const noexcept { return str_;}
};


template < typename T > struct varray;
template < typename T > struct slice_array;
template < typename T > struct mask_array;
template < typename T > struct indirect_array;
template < typename T > struct sub_array;

template < typename T > struct varray : public array_base< varray<T>, T> {
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::reverse_iterator<pointer> reverse_iterator;
    typedef std::reverse_iterator<const pointer> const_reverse_iterator;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
      
    pointer data_;
    size_t sz;

    varray():data_(nullptr),sz(0) {}
    varray(const varray& v):data_(get_storage<T>(v.sz)),sz(v.sz) { copy(data_,data_+sz,v.data_);}
    varray(varray&& v):data_(v.data_),sz(v.sz) { v.data_=nullptr; v.sz=0; }
    varray( std::initializer_list<T> ilist):data_(get_storage<T>(ilist.size())),sz(ilist.size()) {
        copy(data_,data_+sz,ilist.begin());
    }
    varray( pointer p, size_type n):data_(get_storage<T>(n)),sz(n) {
        memcpy(p,data_,sizeof(T)*sz);
    }

    varray( T x, size_type n):data_(get_storage<T>(n)),sz(n) {
        for (size_type i =0; i < sz; ++i) data_[i] = x;
    }

    varray( size_type n):data_(get_storage<T>(n)),sz(n) {}
    
    template < class A >
    varray(const array_base<A,T>& a):data_(get_storage<T>(a.size())),sz(a.size()) {
        for (size_type i=0;i<sz;++i) data_[i] = a[i];
    }
    
    template < class A >
    varray(const array_xpr<A,T>& a):data_(get_storage<T>(a.size())),sz(a.size()) {
        for (size_type i=0;i<sz;++i) data_[i] = a[i];
    }
    
    ~varray() { petlib::free_storage(data_); data_=nullptr; sz=0; }
    
    constexpr varray& operator=(const varray& v) {
        copy(data_,data_+sz,v.data_);
        return *this;
    }
    constexpr varray& operator=(varray&& v) {
        data_ = v.data_;
        sz = v.sz;
        v.data_ = nullptr;
        v.sz = 0;
        return *this;
    }
    constexpr varray& operator=( std::initializer_list<T> ilist) {
        copy(this->begin(),this->end(),ilist.begin());
        return *this;       
    }
    template < class A >
    constexpr varray& operator=(const array_base<A,T>& a) {
        for (size_type i = 0 ; i < sz; ++i) data_[i]= a[i];
        return *this;
    }
    template < class A >
    constexpr varray& operator=(const array_xpr<A,T>& a) {
        for (size_type i = 0 ; i < sz; ++i) data_[i]= a[i];
        return *this;
    }

    constexpr size_type size() const noexcept { return sz;}
    constexpr size_type stride() const noexcept { return size_type(1);}
    constexpr value_type operator[](size_type i) const noexcept { return data_[i];}    
    constexpr reference operator[](size_type i) noexcept { return data_[i];}
    
    constexpr iterator begin() noexcept { return iterator(data_);}
    constexpr iterator end() noexcept { return iterator(data_+sz);}
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(this->end());}
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(this->begin());}
    constexpr const_iterator begin() const noexcept { return iterator(data_);}
    constexpr const_iterator end() const noexcept { return iterator(data_+sz);}
    constexpr const_reverse_iterator rbegin() const noexcept { return reverse_iterator(this->end());}
    constexpr const_reverse_iterator rend() const noexcept { return reverse_iterator(this->begin());}
 
    constexpr pointer base() noexcept { return data_;}
    constexpr const pointer base() const noexcept { return data_;}
 
    constexpr varray<T> operator()(const range& r) const noexcept { 
        return varray<T>(data_+r.offset(),r.size());
    }
    constexpr varray<T> operator()(const slice& s) const noexcept { 
        varray<T> v(s.size());  
        slice_iterator<T> iter(data_+s.offset(),s.stride());
        slice_iterator<T> iend(data_+s.end_size(),s.stride());
        iterator riter(v.data_);
        while (iter!=iend) {
            *riter = *iter;
            ++iter;
            ++riter;
        }
        return v;
    }
    
    constexpr sub_array<T> operator()(const range& r) noexcept {
        return sub_array<T>(data_+r.offset(),r.size());
    }
    constexpr slice_array<T> operator()(const slice& s) noexcept {
        return slice_array<T>(data_+s.offset(),s.size(),s.stride());
    }
     
    constexpr value_type max() const noexcept {
        return petlib::max_element(this->begin(),this->end());
    }   
    constexpr value_type min() const noexcept {
        return petlib::min_element(this->begin(),this->end());
    }   
    constexpr size_type max_index() const noexcept {
        return petlib::max_index(this->begin(),this->end());
    }   
    constexpr size_type min_index() const noexcept {
        return petlib::min_index(this->begin(),this->end());
    }   
    constexpr value_type amax() const noexcept {
        return petlib::amax_element(this->begin(),this->end());
    }   
    constexpr value_type amin() const noexcept {
        return petlib::amin_element(this->begin(),this->end());
    }   
    constexpr size_type amax_index() const noexcept {
        return petlib::amax_index(this->begin(),this->end());
    }   
    constexpr size_type amin_index() const noexcept {
        return petlib::amin_index(this->begin(),this->end());
    }   
    constexpr varray<T> shift(size_type n) const noexcept {
        varray<T> v(T(0),sz);
        petlib::copy(v.begin(),v.begin()+sz-n,this->begin()+n);
        return v;
    }
    constexpr varray<T> cshift(size_type n) const noexcept {
        varray<T> v(sz);
        petlib::copy(v.begin(),v.begin()+(sz-n),(this->begin()+n));
        petlib::copy(v.begin()+(sz-n),v.end(),this->begin());
        return v;
    }
    constexpr varray<T> apply( value_type fun(value_type)) const noexcept {
        varray<T> v(sz);
        petlib::func_fill(v.begin(),v.end(),this->begin(),fun);
        return v;
    }
    constexpr varray<T> apply( value_type fun(const value_type&)) const noexcept {
        varray<T> v(sz);
        petlib::func_fill(v.begin(),v.end(),this->begin(),fun);
        return v;
    }

    void swap(varray<T>& v) {
        swap_elements(this->begin(),this->end(),v.begin());    
    }

    void rotate(varray<T>& v, T& x) {
        rotate_elements(this->begin(),this->end(),v.begin(),x);    
    }
    
    void elim(const varray<T>& v) {
        T dotp = dot_product(this->begin(),this->end(),v.begin());
        T nrmp = norm2<T>(v.begin(),v.end()); 
        T fact = dotp/nrmp;
        for (size_type i = 0 ; i < sz ; ++i) data_[i] -= fact*v[i];
    }

    varray<T>& operator+=(const varray<T>& v) {
        addEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator-=(const varray<T>& v) {
        subEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator*=(const varray<T>& v) {
        mulEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator/=(const varray<T>& v) {
        divEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator^=(const varray<T>& v) {
        xorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator|=(const varray<T>& v) {
        iorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator&=(const varray<T>& v) {
        andEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator<<=(const varray<T>& v) {
        lshEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator>>=(const varray<T>& v) {
        rshEq(this->begin(),this->end(),v.begin());
        return *this;
    }

    template < class xpr_t >
    varray<T>& operator+=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]+= v[i];
        return *this;
    }
    template < class xpr_t >
    varray<T>& operator -=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]-= v[i];
        return *this;
    }
    template < class xpr_t >
    varray<T>& operator *=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]*= v[i];
        return *this;
    }
    template < class xpr_t >
    varray<T>& operator /=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]/= v[i];
        return *this;
    }
    template < class xpr_t >
    varray<T>& operator ^=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]^= v[i];
        return *this;
    }
    template < class xpr_t >
    varray<T>& operator |=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]|= v[i];
        return *this;
    }
    template < class xpr_t >
    varray<T>& operator &=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]&= v[i];
        return *this;
    }
    template < class xpr_t >
    varray<T>& operator <<=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]<<= v[i];
        return *this;
    }
    template < class xpr_t >
    varray<T>& operator >>=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]>>= v[i];
        return *this;
    }

    template < class a_t >
    varray<T>& operator+=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]+= v[i];
        return *this;
    }
    template < class a_t >
    varray<T>& operator -=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]-= v[i];
        return *this;
    }
    template < class a_t >
    varray<T>& operator *=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]*= v[i];
        return *this;
    }
    template < class a_t >
    varray<T>& operator /=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]/= v[i];
        return *this;
    }
    template < class a_t >
    varray<T>& operator ^=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]^= v[i];
        return *this;
    }
    template < class a_t >
    varray<T>& operator |=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]|= v[i];
        return *this;
    }
    template < class a_t >
    varray<T>& operator &=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]&= v[i];
        return *this;
    }
    template < class a_t >
    varray<T>& operator <<=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]<<= v[i];
        return *this;
    }
    template < class a_t >
    varray<T>& operator >>=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]>>= v[i];
        return *this;
    }

    varray<T>& operator=(const slice_array<T>& v) {
        copy(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator+=(const slice_array<T>& v) {
        addEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator-=(const slice_array<T>& v) {
        subEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator*=(const slice_array<T>& v) {
        mulEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator/=(const slice_array<T>& v) {
        divEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator^=(const slice_array<T>& v) {
        xorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator|=(const slice_array<T>& v) {
        iorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator&=(const slice_array<T>& v) {
        andEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator<<=(const slice_array<T>& v) {
        lshEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    varray<T>& operator>>=(const slice_array<T>& v) {
        rshEq(this->begin(),this->end(),v.begin());
        return *this;
    }

    varray<T>& operator=(const T v) {
        fill(this->begin(),this->end(),v);
        return *this;
    }
    varray<T>& operator+=(const T v) {
        addFill(this->begin(),this->end(),v);
        return *this;
    }
    varray<T>& operator-=(const T v) {
        subFill(this->begin(),this->end(),v);
        return *this;
    }
    varray<T>& operator*=(const T v) {
        mulFill(this->begin(),this->end(),v);
        return *this;
    }
    varray<T>& operator/=(const T v) {
        divFill(this->begin(),this->end(),v);
        return *this;
    }
    varray<T>& operator^=(const T v) {
        xorFill(this->begin(),this->end(),v);
        return *this;
    }
    varray<T>& operator|=(const T v) {
        iorFill(this->begin(),this->end(),v);
        return *this;
    }
    varray<T>& operator&=(const T v) {
        andFill(this->begin(),this->end(),v);
        return *this;
    }
    varray<T>& operator<<=(const T v) {
        lshFill(this->begin(),this->end(),v);
        return *this;
    }
    varray<T>& operator>>=(const T v) {
        rshFill(this->begin(),this->end(),v);
        return *this;
    }

    void random_fill();

    void resize(size_type n) {
        if (n < sz) {
            sz = n;
            return;
        }
        if (data_) delete [] data_;
        data_ = get_storage<T>(n);
        sz = n;
    }

};


template < typename T > struct slice_array : public array_base< slice_array<T>, T> {
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef slice_iterator<T> iterator;
    typedef const slice_iterator<T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      
    pointer data_;
    size_t sz;
    size_t str;
    
    slice_array()=delete;
    slice_array(const slice_array& v):data_(v.data_),sz(v.sz),str(v.str) {}
    slice_array(slice_array&& v):data_(v.data_),sz(v.sz),str(v.str) { v.data_=nullptr; }

    slice_array( pointer p, size_type n, size_type stride):data_(p),sz(n),str(stride) {
    }
    
    ~slice_array() { data_=nullptr; sz=0; }
    
    constexpr slice_array& operator=(const slice_array& v) {
        copy(this->begin(),this->end(),v.begin());
        return *this;
    }
    constexpr slice_array& operator=(slice_array&& v) {
        data_ = v.data_;
        sz = v.sz;
        str = v.str;
        v.data_ = nullptr;
        v.sz = 0;
        return *this;
    }

    constexpr slice_array& operator=( std::initializer_list<T> ilist) {
        copy(this->begin(),this->end(),ilist.begin());
        return *this;       
    }

    template < class A >
    constexpr slice_array& operator=(const array_base<A,T>& a) {
        pointer p = data_;
        for (size_type i = 0 ; i < sz; ++i, p+=str) *p = a[i];
        return *this;
    }
    template < class A >
    constexpr slice_array& operator=(const array_xpr<A,T>& a) {
        pointer p = data_;
        for (size_type i = 0 ; i < sz; ++i, p+=str) *p = a[i];
        return *this;
    }

    constexpr size_type size() const noexcept { return sz;}
    constexpr size_type stride() const noexcept { return str;}
    constexpr value_type operator[](size_type i) const noexcept { return data_[i*str];}    
    constexpr reference operator[](size_type i) noexcept { return data_[i*str];}
    
    constexpr iterator begin() noexcept { return iterator(data_,str);}
    constexpr iterator end() noexcept { return iterator(data_+sz*str,str);}
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(this->end());}
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(this->begin());}
    constexpr const_iterator begin() const noexcept { return const_iterator(data_,str);}
    constexpr const_iterator end() const noexcept { return const_iterator(data_+sz*str,str);}
    constexpr const_reverse_iterator rbegin() const noexcept { 
        return reverse_iterator(this->end());}
    constexpr const_reverse_iterator rend() const noexcept { return reverse_iterator(this->begin());}
 
    constexpr pointer base() noexcept { return data_;}
    constexpr const pointer base() const noexcept { return data_;}
 
    constexpr slice_array<T> operator()(const range& r) noexcept { 
        return slice_array<T>(data_+r.offset()*str,r.size(),str);
    }
    constexpr slice_array<T> operator()(const slice& sl) noexcept { 
        return slice_array<T>(data_+sl.offset()*str,sl.size(),sl.stride()*str);
    };
    constexpr varray<T> operator()(const range& r) const noexcept { 
        varray<T> v(r.size());
        copy(v.begin(),v.end(),this->begin()+r.offset()*str);
        return v;
    }
    constexpr varray<T> operator()(const slice& sl) const noexcept { 
        varray<T> v(sl.size());
        iterator iter(data_+sl.offset()*str,sl.stride()*str);
        copy(v.begin(),v.end(),iter);
        return v;

    };
    
    constexpr value_type max() const noexcept {
        return petlib::max_element(this->begin(),this->end());
    }   
    constexpr value_type min() const noexcept {
        return petlib::min_element(this->begin(),this->end());
    }   
    constexpr size_type max_index() const noexcept {
        return petlib::max_index(this->begin(),this->end());
    }   
    constexpr size_type min_index() const noexcept {
        return petlib::min_index(this->begin(),this->end());
    }   
    constexpr value_type amax() const noexcept {
        return petlib::amax_element(this->begin(),this->end());
    }   
    constexpr value_type amin() const noexcept {
        return petlib::amin_element(this->begin(),this->end());
    }   
    constexpr size_type amax_index() const noexcept {
        return petlib::amax_index(this->begin(),this->end());
    }   
    constexpr size_type amin_index() const noexcept {
        return petlib::amin_index(this->begin(),this->end());
    }       
    constexpr varray<T> shift(size_type n) const noexcept {
        varray<T> v(T(0),sz-n);
        petlib::copy(v.begin(),v.end(),(this->begin()+n));
        return v;
    }
    constexpr varray<T> cshift(size_type n) const noexcept {
        varray<T> v(sz);
        iterator iter(data_+n*str,str);
        petlib::copy(v.begin(),v.begin()+(sz-n),iter);
        petlib::copy(v.begin()+(sz-n),v.end(),this->begin());
        return v;
    }
    constexpr varray<T> apply( value_type fun(value_type)) const noexcept {
        varray<T> v(sz);
        petlib::func_fill(v.begin(),v.end(),this->begin(),fun);
        return v;
    }
    constexpr varray<T> apply( value_type fun(const value_type&)) const noexcept {
        varray<T> v(sz);
        petlib::func_fill(v.begin(),v.end(),this->begin(),fun);
        return v;
    }

    void swap(varray<T>& v) {
        swap_elements(this->begin(),this->end(),v.begin());    
    }

    void swap(slice_array<T>& v) {
        swap_elements(this->begin(),this->end(),v.begin());    
    }
    
    void rotate(varray<T>& v, T x) {
        rotate_elements(this->begin(),this->end(),v.begin(),x);    
    }
    
    void rotate(slice_array<T>& v, T x) {
        rotate_elements(this->begin(),this->end(),v.begin(),x);        
    }
    
    void elim(const varray<T>& v) {
        T dotp = dot_product(this->begin(),this->end(),v.begin());
        T nrmp = norm2<T>(v.begin(),v.end()); 
        T fact = dotp/nrmp;
        pointer dp = data_;
        for (size_type i = 0 ; i < sz ; ++i, dp+=str) *dp -= fact*v[i];
    }

    slice_array<T>& operator+=(const varray<T>& v) {
        addEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator-=(const varray<T>& v) {
        subEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator*=(const varray<T>& v) {
        mulEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator/=(const varray<T>& v) {
        divEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator^=(const varray<T>& v) {
        xorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator|=(const varray<T>& v) {
        iorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator&=(const varray<T>& v) {
        andEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator<<=(const varray<T>& v) {
        lshEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator>>=(const varray<T>& v) {
        rshEq(this->begin(),this->end(),v.begin());
        return *this;
    }

    template < class xpr_t >
    slice_array<T>& operator+=(const array_xpr<xpr_t,T>& v) {
        pointer dp = data_;
        for (size_type i=0;i<sz;++i,dp+=str) *dp += v[i];
        return *this;
    }
    template < class xpr_t >
    slice_array<T>& operator -=(const array_xpr<xpr_t,T>& v) {
        pointer dp = data_;
        for (size_type i=0;i<sz;++i,dp+=str) *dp -= v[i];
        return *this;
    }
    template < class xpr_t >
    slice_array<T>& operator *=(const array_xpr<xpr_t,T>& v) {
        pointer dp = data_;
        for (size_type i=0;i<sz;++i,dp+=str) *dp *= v[i];
        return *this;
    }
    template < class xpr_t >
    slice_array<T>& operator /=(const array_xpr<xpr_t,T>& v) {
        pointer dp = data_;
        for (size_type i=0;i<sz;++i,dp+=str) *dp /= v[i];
        return *this;
    }
    template < class xpr_t >
    slice_array<T>& operator ^=(const array_xpr<xpr_t,T>& v) {
        pointer dp = data_;
        for (size_type i=0;i<sz;++i,dp+=str) *dp ^= v[i];
        return *this;
    }
    template < class xpr_t >
    slice_array<T>& operator |=(const array_xpr<xpr_t,T>& v) {
        pointer dp = data_;
        for (size_type i=0;i<sz;++i,dp+=str) *dp |= v[i];
        return *this;
    }
    template < class xpr_t >
    slice_array<T>& operator &=(const array_xpr<xpr_t,T>& v) {
        pointer dp = data_;
        for (size_type i=0;i<sz;++i,dp+=str) *dp &= v[i];
        return *this;
    }
    template < class xpr_t >
    slice_array<T>& operator <<=(const array_xpr<xpr_t,T>& v) {
        pointer dp = data_;
        for (size_type i=0;i<sz;++i,dp+=str) *dp <<= v[i];
        return *this;
    }
    template < class xpr_t >
    slice_array<T>& operator >>=(const array_xpr<xpr_t,T>& v) {
        pointer dp = data_;
        for (size_type i=0;i<sz;++i,dp+=str) *dp >>= v[i];
        return *this;
    }

    slice_array<T>& operator+=(const slice_array<T>& v) {
        addEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator-=(const slice_array<T>& v) {
        subEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator*=(const slice_array<T>& v) {
        mulEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator/=(const slice_array<T>& v) {
        divEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator^=(const slice_array<T>& v) {
        xorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator|=(const slice_array<T>& v) {
        iorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator&=(const slice_array<T>& v) {
        andEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator<<=(const slice_array<T>& v) {
        lshEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator>>=(const slice_array<T>& v) {
        rshEq(this->begin(),this->end(),v.begin());
        return *this;
    }

    slice_array<T>& operator=(const sub_array<T>& v) {
        copy(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator+=(const sub_array<T>& v) {
        addEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator-=(const sub_array<T>& v) {
        subEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator*=(const sub_array<T>& v) {
        mulEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator/=(const sub_array<T>& v) {
        divEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator^=(const sub_array<T>& v) {
        xorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator|=(const sub_array<T>& v) {
        iorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator&=(const sub_array<T>& v) {
        andEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator<<=(const sub_array<T>& v) {
        lshEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    slice_array<T>& operator>>=(const sub_array<T>& v) {
        rshEq(this->begin(),this->end(),v.begin());
        return *this;
    }

    slice_array<T>& operator=(const T v) {
        fill(this->begin(),this->end(),v);
        return *this;
    }
    slice_array<T>& operator+=(const T v) {
        addFill(this->begin(),this->end(),v);
        return *this;
    }
    slice_array<T>& operator-=(const T v) {
        subFill(this->begin(),this->end(),v);
        return *this;
    }
    slice_array<T>& operator*=(const T v) {
        mulFill(this->begin(),this->end(),v);
        return *this;
    }
    slice_array<T>& operator/=(const T v) {
        divFill(this->begin(),this->end(),v);
        return *this;
    }
    slice_array<T>& operator^=(const T v) {
        xorFill(this->begin(),this->end(),v);
        return *this;
    }
    slice_array<T>& operator|=(const T v) {
        iorFill(this->begin(),this->end(),v);
        return *this;
    }
    slice_array<T>& operator&=(const T v) {
        andFill(this->begin(),this->end(),v);
        return *this;
    }
    slice_array<T>& operator<<=(const T v) {
        lshFill(this->begin(),this->end(),v);
        return *this;
    }
    slice_array<T>& operator>>=(const T v) {
        rshFill(this->begin(),this->end(),v);
        return *this;
    }
};

template <typename T> void varray<T>::random_fill()
{
    std::mt19937 rgen;
    iterator x = this->begin();
    iterator xlast = this->end();
    while (x!=xlast) { *x = rgen(); ++x; };
}

#include <ctime>
#include <chrono>

template <> void varray<float>::random_fill()
{
    std::mt19937 rgen;
    std::uniform_real_distribution<float> udis(-1.f,1.f);
    iterator x = this->begin();
    iterator xlast = this->end();
    while (x!=xlast) { *x = udis(rgen); ++x; };
}

template <> void varray<double>::random_fill()
{
    static unsigned int iseed = 1;
    std::mt19937 rgen(iseed);
    iseed+=2;
    std::uniform_real_distribution<double> udis(-1.,1.);
    iterator x = this->begin();
    iterator xlast = this->end();
    while (x!=xlast) { *x = udis(rgen); ++x; };
}

template <> void varray<long double>::random_fill()
{
    std::mt19937 rgen;
    std::uniform_real_distribution<long double> udis(-1.L,1.L);
    iterator x = this->begin();
    iterator xlast = this->end();
    while (x!=xlast) { *x = udis(rgen); ++x; };
}

template < typename T > struct sub_array : public array_base< sub_array<T>, T> {
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::reverse_iterator<pointer> reverse_iterator;
    typedef std::reverse_iterator<const pointer> const_reverse_iterator;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
      
    pointer data_;
    size_t sz;

    sub_array()=delete;
    sub_array(const varray<T>& v):data_(v.base()),sz(v.size()) {}
    sub_array(const sub_array<T>& v):data_(v.data_),sz(v.size()) {}
    sub_array(sub_array&& v):data_(v.data_),sz(v.sz) { }

    sub_array( pointer p, size_type n):data_(p),sz(n) {
    }
        
    ~sub_array() { data_=nullptr; sz=0; }
    
    constexpr sub_array& operator=(const sub_array& v) {
        copy(data_,data_+sz,v.data_);
        return *this;
    }
    constexpr sub_array& operator=(sub_array&& v) {
        data_ = v.data_;
        sz = v.sz;
        v.data_ = nullptr;
        v.sz = 0;
        return *this;
    }
    constexpr sub_array& operator=( std::initializer_list<T> ilist) {
        copy(this->begin(),this->end(),ilist.begin());
        return *this;       
    }
    template < class A >
    constexpr sub_array& operator=(const array_base<A,T>& a) {
        for (size_type i = 0 ; i < sz; ++i) data_[i]= a[i];
        return *this;
    }
    template < class A >
    constexpr sub_array& operator=(const array_xpr<A,T>& a) {
        for (size_type i = 0 ; i < sz; ++i) data_[i]= a[i];
        return *this;
    }

    constexpr size_type size() const noexcept { return sz;}
    constexpr size_type stride() const noexcept { return size_type(1);}
    constexpr value_type operator[](size_type i) const noexcept { return data_[i];}    
    constexpr reference operator[](size_type i) noexcept { return data_[i];}
    
    constexpr iterator begin() noexcept { return iterator(data_);}
    constexpr iterator end() noexcept { return iterator(data_+sz);}
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(this->end());}
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(this->begin());}
    constexpr const_iterator begin() const noexcept { return iterator(data_);}
    constexpr const_iterator end() const noexcept { return iterator(data_+sz);}
    constexpr const_reverse_iterator rbegin() const noexcept { return reverse_iterator(this->end());}
    constexpr const_reverse_iterator rend() const noexcept { return reverse_iterator(this->begin());}
 
    constexpr pointer base() noexcept { return data_;}
    constexpr const pointer base() const noexcept { return data_;}
 
    constexpr sub_array<T> operator()(const range& r) noexcept { 
        return sub_array<T>(data_+r.offset(),r.size());
    }
    constexpr varray<T> operator()(const range& r) const noexcept {
        return varray<T>(data_+r.offset(),r.size());
    }
    constexpr varray<T> operator()(const slice& sl) const noexcept { 
        varray<T> v(sl.size());
        slice_iterator<T> iter(data_+sl.offset(),sl.stride());
        slice_iterator<T> iend(data_+sl.end_size(),sl.stride());
        iterator riter(v.data_);
        while (iter!=iend) {
            *riter = *iter;
            ++iter;
            ++riter;
        }
        return v;
    }    
    constexpr slice_array<T> operator()(const slice& sl) noexcept {
        return slice_array<T>(data_+sl.offset(),sl.size(),sl.stride());
    }
     
    constexpr value_type max() const noexcept {
        return petlib::max_element(this->begin(),this->end());
    }   
    constexpr value_type min() const noexcept {
        return petlib::min_element(this->begin(),this->end());
    }   
    constexpr size_type max_index() const noexcept {
        return petlib::max_index(this->begin(),this->end());
    }   
    constexpr size_type min_index() const noexcept {
        return petlib::min_index(this->begin(),this->end());
    }   
    constexpr value_type amax() const noexcept {
        return petlib::amax_element(this->begin(),this->end());
    }   
    constexpr value_type amin() const noexcept {
        return petlib::amin_element(this->begin(),this->end());
    }   
    constexpr size_type amax_index() const noexcept {
        return petlib::amax_index(this->begin(),this->end());
    }   
    constexpr size_type amin_index() const noexcept {
        return petlib::amin_index(this->begin(),this->end());
    }   
    constexpr varray<T> shift(size_type n) const noexcept {
        varray<T> v(T(0),sz-n);
        petlib::copy(v.begin(),v.end(),this->begin());
        return v;
    }
    constexpr varray<T> cshift(size_type n) const noexcept {
        varray<T> v(sz);
        petlib::copy(v.begin(),v.begin()+(sz-n),(this->begin()+sz-n));
        petlib::copy(v.begin()+(sz-n),v.end(),this->begin());
        return v;
    }
    constexpr varray<T> apply( value_type fun(value_type)) const noexcept {
        varray<T> v(sz);
        petlib::func_fill(v.begin(),v.end(),this->begin(),fun);
        return v;
    }
    constexpr varray<T> apply( value_type fun(const value_type&)) const noexcept {
        varray<T> v(sz);
        petlib::func_fill(v.begin(),v.end(),this->begin(),fun);
        return v;
    }

    void swap(sub_array<T>& v) {
        swap_elements(this->begin(),this->end(),v.begin());    
    }

    void rotate(sub_array<T>& v, T& x) {
        rotate_elements(this->begin(),this->end(),v.begin(),x);    
    }
    
    void elim(const sub_array<T>& v) {
        T dotp = dot_product(this->begin(),this->end(),v.begin());
        T nrmp = norm2<T>(v.begin(),v.end()); 
        T fact = dotp/nrmp;
        for (size_type i = 0 ; i < sz ; ++i) data_[i] -= fact*v[i];
    }

    sub_array<T>& operator+=(const sub_array<T>& v) {
        addEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator-=(const sub_array<T>& v) {
        subEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator*=(const sub_array<T>& v) {
        mulEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator/=(const sub_array<T>& v) {
        divEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator^=(const sub_array<T>& v) {
        xorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator|=(const sub_array<T>& v) {
        iorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator&=(const sub_array<T>& v) {
        andEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator<<=(const sub_array<T>& v) {
        lshEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator>>=(const sub_array<T>& v) {
        rshEq(this->begin(),this->end(),v.begin());
        return *this;
    }

    sub_array<T>& operator+=(const varray<T>& v) {
        addEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator-=(const varray<T>& v) {
        subEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator*=(const varray<T>& v) {
        mulEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator/=(const varray<T>& v) {
        divEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator^=(const varray<T>& v) {
        xorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator|=(const varray<T>& v) {
        iorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator&=(const varray<T>& v) {
        andEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator<<=(const varray<T>& v) {
        lshEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator>>=(const varray<T>& v) {
        rshEq(this->begin(),this->end(),v.begin());
        return *this;
    }

    template < class xpr_t >
    sub_array<T>& operator+=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]+= v[i];
        return *this;
    }
    template < class xpr_t >
    sub_array<T>& operator -=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]-= v[i];
        return *this;
    }
    template < class xpr_t >
    sub_array<T>& operator *=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]*= v[i];
        return *this;
    }
    template < class xpr_t >
    sub_array<T>& operator /=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]/= v[i];
        return *this;
    }
    template < class xpr_t >
    sub_array<T>& operator ^=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]^= v[i];
        return *this;
    }
    template < class xpr_t >
    sub_array<T>& operator |=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]|= v[i];
        return *this;
    }
    template < class xpr_t >
    sub_array<T>& operator &=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]&= v[i];
        return *this;
    }
    template < class xpr_t >
    sub_array<T>& operator <<=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]<<= v[i];
        return *this;
    }
    template < class xpr_t >
    sub_array<T>& operator >>=(const array_xpr<xpr_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]>>= v[i];
        return *this;
    }

    template < class a_t >
    sub_array<T>& operator+=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]+= v[i];
        return *this;
    }
    template < class a_t >
    sub_array<T>& operator -=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]-= v[i];
        return *this;
    }
    template < class a_t >
    sub_array<T>& operator *=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]*= v[i];
        return *this;
    }
    template < class a_t >
    sub_array<T>& operator /=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]/= v[i];
        return *this;
    }
    template < class a_t >
    sub_array<T>& operator ^=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]^= v[i];
        return *this;
    }
    template < class a_t >
    sub_array<T>& operator |=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]|= v[i];
        return *this;
    }
    template < class a_t >
    sub_array<T>& operator &=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]&= v[i];
        return *this;
    }
    template < class a_t >
    sub_array<T>& operator <<=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]<<= v[i];
        return *this;
    }
    template < class a_t >
    sub_array<T>& operator >>=(const array_base<a_t,T>& v) {
        for (size_type i=0;i<sz;++i) data_[i]>>= v[i];
        return *this;
    }

    sub_array<T>& operator=(const slice_array<T>& v) {
        copy(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator+=(const slice_array<T>& v) {
        addEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator-=(const slice_array<T>& v) {
        subEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator*=(const slice_array<T>& v) {
        mulEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator/=(const slice_array<T>& v) {
        divEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator^=(const slice_array<T>& v) {
        xorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator|=(const slice_array<T>& v) {
        iorEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator&=(const slice_array<T>& v) {
        andEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator<<=(const slice_array<T>& v) {
        lshEq(this->begin(),this->end(),v.begin());
        return *this;
    }
    sub_array<T>& operator>>=(const slice_array<T>& v) {
        rshEq(this->begin(),this->end(),v.begin());
        return *this;
    }

    sub_array<T>& operator=(const T v) {
        fill(this->begin(),this->end(),v);
        return *this;
    }
    sub_array<T>& operator+=(const T v) {
        addFill(this->begin(),this->end(),v);
        return *this;
    }
    sub_array<T>& operator-=(const T v) {
        subFill(this->begin(),this->end(),v);
        return *this;
    }
    sub_array<T>& operator*=(const T v) {
        mulFill(this->begin(),this->end(),v);
        return *this;
    }
    sub_array<T>& operator/=(const T v) {
        divFill(this->begin(),this->end(),v);
        return *this;
    }
    sub_array<T>& operator^=(const T v) {
        xorFill(this->begin(),this->end(),v);
        return *this;
    }
    sub_array<T>& operator|=(const T v) {
        iorFill(this->begin(),this->end(),v);
        return *this;
    }
    sub_array<T>& operator&=(const T v) {
        andFill(this->begin(),this->end(),v);
        return *this;
    }
    sub_array<T>& operator<<=(const T v) {
        lshFill(this->begin(),this->end(),v);
        return *this;
    }
    sub_array<T>& operator>>=(const T v) {
        rshFill(this->begin(),this->end(),v);
        return *this;
    }
};

}
#endif