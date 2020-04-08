

template < typename T > struct varray;
template < typename T > struct slice_array;
template < typename T > struct gslice_array;
template < typename T > struct mask_array;
template < typename T > struct indirect_array;

template < typename T >
void abs_cmp(const T x,const T y)
{
   return (abs<T>(x)<abs<T>(y));
}

template < class array_type >
struct array_base
{
   typedef typename array_type::value_t value_t;
   constexpr const array_t* leaf() const noexcept { return static_cast<const array_t*>(this);}   
   constexpr value_t operator[](size_type n) const noexcept { return leaf()->operator[](n);}
   constexpr size_type size() const noexcept  { return leaf()->size();}
};

template < class Xpr_t, typename T >
struct array_xpr
{
    typedef T value_t;
    typedef std::size_t size_type;

    const Xpr_t& x;
    
    array_xpr( const Xpr_t& x0):x(x0) {}
    
    constexpr value_t operator[](size_type n) const noexcept { return Xpr_t[n];}
    constexpr size_type size() const noexcept { return Xpr_t.size();}
};

template < typename T> struct slice_iterator {
   typedef T value_t;
   typedef std::size_t size_type;
   typedef std::ptrdiff_t difference_type;
   typedef T* pointer_t;
   typedef const T* const_pointer_t;
   typedef T& reference;
   typedef const  T& const_reference_t;
   typedef slice_iterator<T> self;
   typedef slice_iterator<T>& self_reference;
   typedef const slice_iterator<T>& const_self_reference;
   
   T * ptr;
   size_type str;
   
   slice_iterator( pointer_t p, size_type stride):ptr(p),str(str) {}
   
   reference operator*() noexcept { return *ptr;}
   const_reference operator*() const noexcept { return *ptr}
   size_type stride() const { return str;}
   
   slice_iterator<T>& operator++() noexcept { ptr+=str; return *this;}
   slice_iterator<T>& operator++(int) noexcept {
       slice_iterator<T> tmp(ptr,str);
       ptr+=str;
       return tmp;
   }
   slice_iterator<T>& operator--() noexcept { ptr-=str; return *this;}
   slice_iterator<T>& operator--(int) noexcept {
       slice_iterator<T> tmp(ptr,str);
       ptr-=str;
       return tmp;
   }

   slice_iterator<T>& operator+=(difference_type n) {
       return slice_iterator<T>(ptr+n*str,str);
   }
   
   slice_iterator<T>& operator-=(difference_type n) {
       return slice_iterator<T>(ptr-n*str,str);
   }

   friend bool operator==(slice_iterator<T>& iter1,slice_iterator<T>& iter2) {
       return (iter1.ptr==iter2.ptr);
   }

   friend bool operator!=(slice_iterator<T>& iter) {
       return (iter1.ptr1!=iter2.ptr);
   }

   friend bool operator<=(slice_iterator<T>& iter1,slice_iterator<T>& iter2) {
       return (iter1.ptr<=iter2.ptr);
   }

   friend bool operator>=(slice_iterator<T>& iter) {
       return (iter1.ptr1>=iter2.ptr);
   }

   friend bool operator<(slice_iterator<T>& iter1,slice_iterator<T>& iter2) {
       return (iter1.ptr<iter2.ptr);
   }

   friend bool operator>(slice_iterator<T>& iter) {
       return (iter1.ptr1>iter2.ptr);
   }
};

template < typename t >
slice_iterator<T>& copy(slice_iterator<T>& src_iter,slice_iterator<T>& src_iter_last,
         slice_iterator<T>& dst_iter)
{
    for (src_iter!=src_iter_last;) *dst_iter++ = *src_iter++;
    return dst_iter;
}
 
template < typename t, class unary_op >
slice_iterator<T>& apply(slice_iterator<T>& src_iter,slice_iterator<T>& src_iter_last,
         slice_iterator<T>& dst_iter, unary_op op )
{
    for (src_iter!=src_iter_last;) {
      *dst_iter++ = unary_op( *src_iter );
      ++src_iter;
    }
    return dst_iter;
}
 
template < typename t, class binary_op >
slice_iterator<T>& apply(slice_iterator<T>& lhs_iter,slice_iterator<T>& lhs_iter_last,
         slice_iterator<T>& rhs_iter,slice_iterator<T>& dst_iter, binary_op op )
{
    for (lhs_iter!=lhs_iter_last;) {
      *dst_iter++ = binary_op( *lhs_iter, *rhs_iter );
      ++lhs_iter;
      ++rhs_iter;
    }
    return dst_iter;
}
 
template < typename T >
slice_iterator<T> operator+(slice_iterator<T>& iter,std::ptrdiff_t n)
{
   return (iter+=n);
}

template < typename T >
slice_iterator<T> operator-(slice_iterator<T>& iter,std::ptrdiff_t n)
{
   return (iter-=n);
}

template < typename T >
slice_iterator<T> operator+(std::ptrdiff_t n,slice_iterator<T>& iter)
{
   return (iter+=n);
}

template < typename T >
slice_iterator<T> operator-(std::ptrdiff_t n,slice_iterator<T>& iter)
{
   return (iter-=n);
}

template < typename T> struct range_iterator {
   typedef T value_t;
   typedef std::size_t size_type;
   typedef std::ptrdiff_t difference_type;
   typedef T* pointer_t;
   typedef const T* const_pointer_t;
   typedef T& reference;
   typedef const  T& const_reference_t;
   typedef range_iterator<T> self;
   typedef range_iterator<T>& self_reference;
   typedef const range_iterator<T>& const_self_reference;
   
   T * ptr;
   
   range_iterator( pointer_t p ):ptr(p) {}
   
   reference operator*() noexcept { return *ptr;}
   const_reference operator*() const noexcept { return *ptr}
   size_type stride() const { return str;}
   
   range_iterator<T>& operator++() noexcept { ++ptr; return *this;}
   range_iterator<T>& operator++(int) noexcept {
       range_iterator<T> tmp(ptr,str);
       ++ptr;
       return tmp;
   }
   range_iterator<T>& operator--() noexcept { --ptr; return *this;}
   range_iterator<T>& operator--(int) noexcept {
       range_iterator<T> tmp(ptr,str);
       --ptr;
       return tmp;
   }

   range_iterator<T>& operator+=(difference_type n) {
       return range_iterator<T>(ptr+n);
   }
   
   range_iterator<T>& operator-=(difference_type n) {
       return range_iterator<T>(ptr-n);
   }

   friend bool operator==(range_iterator<T>& iter1,range_iterator<T>& iter2) {
       return (iter1.ptr==iter2.ptr);
   }

   friend bool operator!=(range_iterator<T>& iter) {
       return (iter1.ptr1!=iter2.ptr);
   }

   friend bool operator<=(range_iterator<T>& iter1,range_iterator<T>& iter2) {
       return (iter1.ptr<=iter2.ptr);
   }

   friend bool operator>=(range_iterator<T>& iter1,range_iterator<T>& iter2) {
       return (iter1.ptr1>=iter2.ptr);
   }

   friend bool operator<(range_iterator<T>& iter1,range_iterator<T>& iter2) {
       return (iter1.ptr<iter2.ptr);
   }

   friend bool operator>(range_iterator<T>& iter1,range_iterator<T>& iter2) {
       return (iter1.ptr1>iter2.ptr);
   }
};

template < typename t >
range_iterator<T>& copy(range_iterator<T>& src_iter,range_iterator<T>& src_iter_last,
         range_iterator<T>& dst_iter)
{
    for (src_iter!=src_iter_last;) *dst_iter++ = *src_iter++;
    return dst_iter;
}
 
template < typename t, class unary_op >
range_iterator<T>& apply(range_iterator<T>& src_iter,range_iterator<T>& src_iter_last,
         range_iterator<T>& dst_iter, unary_op op )
{
    for (src_iter!=src_iter_last;) {
      *dst_iter++ = unary_op( *src_iter );
      ++src_iter;
    }
    return dst_iter;
}
 
template < typename t, class binary_op >
range_iterator<T>& apply(range_iterator<T>& lhs_iter,range_iterator<T>& lhs_iter_last,
         range_iterator<T>& rhs_iter,range_iterator<T>& dst_iter, binary_op op )
{
    for (lhs_iter!=lhs_iter_last;) {
      *dst_iter++ = binary_op( *lhs_iter, *rhs_iter );
      ++lhs_iter;
      ++rhs_iter;
    }
    return dst_iter;
}
 
 
 
template < typename T >
range_iterator<T> operator+(range_iterator<T>& iter,std::ptrdiff_t n)
{
   return (iter+=n);
}

template < typename T >
range_iterator<T> operator-(range_iterator<T>& iter,std::ptrdiff_t n)
{
   return (iter-=n);
}

template < typename T >
range_iterator<T> operator+(std::ptrdiff_t n,range_iterator<T>& iter)
{
   return (iter+=n);
}

template < typename T >
range_iterator<T> operator-(std::ptrdiff_t n,range_iterator<T>& iter)
{
   return (iter-=n);
}


template < typename t >
range_iterator<T>& copy(slice_iterator<T>& src_iter,slice_iterator<T>& src_iter_last,
         range_iterator<T>& dst_iter)
{
    for (src_iter!=src_iter_last;) *dst_iter++ = *src_iter++;
    return dst_iter;
}
 
template < typename t, class unary_op >
range_iterator<T>& apply(range_iterator<T>& src_iter,slice_iterator<T>& src_iter_last,
         range_iterator<T>& dst_iter, unary_op op )
{
    for (src_iter!=src_iter_last;) {
      *dst_iter++ = unary_op( *src_iter );
      ++src_iter;
    }
    return dst_iter;
}
 
template < typename t, class binary_op >
range_iterator<T>& apply(range_iterator<T>& lhs_iter,range_iterator<T>& lhs_iter_last,
         range_iterator<T>& rhs_iter,range_iterator<T>& dst_iter, binary_op op )
{
    for (lhs_iter!=lhs_iter_last;) {
      *dst_iter++ = binary_op( *lhs_iter, *rhs_iter );
      ++lhs_iter;
      ++rhs_iter;
    }
    return dst_iter;
}
 
 
 
template < typename T >
range_iterator<T> operator+(range_iterator<T>& iter,std::ptrdiff_t n)
{
   return (iter+=n);
}

template < typename T >
range_iterator<T> operator-(range_iterator<T>& iter,std::ptrdiff_t n)
{
   return (iter-=n);
}

template < typename T >
range_iterator<T> operator+(std::ptrdiff_t n,range_iterator<T>& iter)
{
   return (iter+=n);
}

template < typename T >
range_iterator<T> operator-(std::ptrdiff_t n,range_iterator<T>& iter)
{
   return (iter-=n);
}




template < typename T >
struct varray: public array_base< T, varray<T> > {
   typedef T value_t;
   typedef std::size_t size_type;
   typedef std::ptrdiff_t difference_type;
   typedef T* pointer_t;
   typedef const T* const_pointer_t;
   typedef T& reference;
   typedef const  T& const_reference_t;
   
   value_t * alloc(size_type n) {
       value_t * p = std::
   }
   
   varray():data_(),n(){}
   varray(const varray<T>& v):data_(new value_t[v.ne]),ne(v.ne) {
       std::copy(v.begin(),v.end(),data_);
   }
   varray(varray&& v):data_(move<T>(v.data_)),ne(v.ne) {
       v.data_ = nullptr;
       v.ne = 0; 
   }
   explicit varray(size_t n):data_(new value_t[v.ne])
   varray(const range_array<T>& v);
   varray(const grange_array<T>& v);
   varray(const mask_array<T>& v);
   varray(const indirect_array<T>& v);
   varray(value_t x,size_type n);   
   varray(const initializer_list<T>& ilist);
   varray(const_pointer_t p,size_type n);
   varray(
   ~varray() {
       alloc.deallocate(data_);
       ne = 0;
   }
   varray& operator=(const varray& v);
   varray& operator=(varray&& v);
   
   varray& operator=(initializer_list<T>& ilist);
   
   constexpr value_t operator[](size_type n) const noexcept { return data_[n];};
   value_t& operator[](size_type n) noexcept { return data_[n];};
   
   varray<T>& operator[](const range& s) const noexcept;
   range_array<T> operator[](const range& s) noexcept;
   varray<T>& operator[](const grange& s) const noexcept;
   grange_array<T> operator[](const grange& s) noexcept;
   varray<T>& operator[]( const varray<bool>& b) const noexcept;
   mask_array<T> operator[](const varray<bool>& b) noexcept;
   varray<T>& operator[](const varray<size_type>& ix) const noexcept;
   indirect_array<T> operator[](const varray<size_type>& iv) noexcept;
   
   // unary operator
   varray<T> operator+() const;
   varray<T> operator-() const;
   varray<T> operator~() const;
   varray<bool> operator!() const;
   
   // boolean operators
   bool operator==(const varray<T>& v);
   bool operator!=(const varray<T>& v);
      
   varray& operator+=(const varray& v);
   varray& operator-=(const varray& v);
   varray& operator*=(const varray& v);
   varray& operator/=(const varray& v);
   varray& operator^=(const varray& v);
   varray& operator|=(const varray& v);
   varray& operator&=(const varray& v);
   varray& operator>>=(const varray& v);
   varray& operator<<=(const varray& v);

   varray& operator=(const slice_array<T>& v);
   varray& operator+=(const slice_array<T>& v);
   varray& operator-=(const slice_array<T>& v);
   varray& operator*=(const slice_array<T>& v);
   varray& operator/=(const slice_array<T>& v);
   varray& operator^=(const slice_array<T>& v);
   varray& operator|=(const slice_array<T>& v);
   varray& operator&=(const slice_array<T>& v);
   varray& operator>>=(const slice_array<T>& v);
   varray& operator<<=(const slice_array<T>& v);

   varray& operator=(const indirect_array<T>& v);
   varray& operator+=(const indirect_array<T>& v);
   varray& operator-=(const indirect_array<T>& v);
   varray& operator*=(const indirect_array<T>& v);
   varray& operator/=(const indirect_array<T>& v);
   varray& operator^=(const indirect_array<T>& v);
   varray& operator|=(const indirect_array<T>& v);
   varray& operator&=(const indirect_array<T>& v);
   varray& operator>>=(const indirect_array<T>& v);
   varray& operator<<=(const indirect_array<T>& v);


   varray& operator=(const gslice_array<T>& v);
   varray& operator+=(const gslice_array<T>& v);
   varray& operator-=(const gslice_array<T>& v);
   varray& operator*=(const gslice_array<T>& v);
   varray& operator/=(const gslice_array<T>& v);
   varray& operator^=(const gslice_array<T>& v);
   varray& operator|=(const gslice_array<T>& v);
   varray& operator&=(const gslice_array<T>& v);
   varray& operator>>=(const gslice_array<T>& v);
   varray& operator<<=(const gslice_array<T>& v);


   varray& operator=(const mask_array<T>& v);
   varray& operator+=(const mask_array<T>& v);
   varray& operator-=(const mask_array<T>& v);
   varray& operator*=(const mask_array<T>& v);
   varray& operator/=(const mask_array<T>& v);
   varray& operator^=(const mask_array<T>& v);
   varray& operator|=(const mask_array<T>& v);
   varray& operator&=(const mask_array<T>& v);
   varray& operator>>=(const mask_array<T>& v);
   varray& operator<<=(const mask_array<T>& v);

   varray& operator=(const T& v);
   varray& operator+=(const T& v);
   varray& operator-=(const T& v);
   varray& operator*=(const T& v);
   varray& operator/=(const T& v);
   varray& operator^=(const T& v);
   varray& operator|=(const T& v);
   varray& operator&=(const T& v);
   varray& operator>>=(const T& v);
   varray& operator<<=(const T& v);

   template < class Xpr_t >
   template < class Xpr_t > 
   varray& operator=(const Expr<Xpr_t,value_type> v);
   template < class Xpr_t > 
   varray& operator+=(const Expr<Xpr_t,value_type> v);
   template < class Xpr_t > 
   varray& operator-=(const Expr<Xpr_t,value_type> v);
   template < class Xpr_t > 
   varray& operator*=(const Expr<Xpr_t,value_type> v);
   template < class Xpr_t > 
   varray& operator/=(const Expr<Xpr_t,value_type> v);
   template < class Xpr_t > 
   varray& operator^=(const Expr<Xpr_t,value_type> v);
   template < class Xpr_t > 
   varray& operator|=(const Expr<Xpr_t,value_type> v);
   template < class Xpr_t > 
   varray& operator&=(const Expr<Xpr_t,value_type> v);
   template < class Xpr_t > 
   varray& operator>>=(const Expr<Xpr_t,value_type> v);
   template < class Xpr_t > 
   varray& operator<<=(const Expr<Xpr_t,value_type> v);

   pointer_t begin() { return data_;} 
   const_pointer_t begin() const { return data_;}
   pointer_t end() { return data_+ne;}
   pointer_t end() const { return data_+ne;}
   reverse_iterator<T> rbegin() { return end();}
   const_reverse_iterator<T> rbegin() const { return end();}
   reverse_iterator<T> rend() { return begin();}
   const_reverse_iterator<T> rend() const { return begin();}
   bool empty() const noexcept { return data_==nullptr;}
   
   constexpr size_type size() const { return ne;}
   constexpr pointer_t * data() const { return data_;}
   constexpr const_pointer_t * data() const noexcept { return data_;}
   
   slice_iterator<T> get_slice(const slice& s) {
       return slice_iterator<T>(data_+s.offset(),s.stride());
   }
   
   varray<T> apply( value_t fun( value_t ) ) const noexcept;
   varray<T> apply( value_t fun( const value_t&) ) const noexcept;
   void swap(varray<T>& v) noexcept {
       for (size_type n=0;n<ne;++n) {
           T tmp = data_[n];
           data_[n]= v.data_[n];
           v.data_[n]= tmp; 
       }
   }
   void shift( size_type n);
   void cshift( size_type n);
   void resize( size_type n, value_type x=value_type());
   
   value_type sum() const;
   value_type min() const;
   value_type max() const;
   value_type amax() const;
   value_type amin() const;
   size_type index_min() const;
   size_type index_max() const;
   size_type index_amin() const;
   size_type index_amax() const;
private:
   pointer_t * p;
   size_type ne;
};

