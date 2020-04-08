



namespace putils {

template < typename T , std::size_t sz >
struct array {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef T value_type;
    typedef pointer iterator;
    typedef const pointer const_iterator;
    typedef std::reverse_iterator<pointer> reverse_iterator;
    typedef std::reverse_iterator<const pointer> const_reverse_iterator;
private:    
    T a[sz];
public:    
    
    constexpr void fill(value_type x) noexcept {
        fill(begin(),end(),x);
    }    
    
    
    constexpr void swap(putils::array<T,n>& v) {
         value_type tmp;
         for (auto i =0; i < n ;++i) {
             tmp =v.a[i];
             v.a[i]=a[i];
             a[i]=tmp;
         }
    }
    
    constexpr value_type front() const noexcept { return a[0];}
    constexpr value_type back() const noexcept { return a[sz-1];}
    constexpr reference front() noexcept { return a[0];}
    constexpr reference back() noexcept { return a[sz-1];}
    constexpr size_type size() const noexcept { return sz;}
    constexpr bool empty() const noexcept { return sz==0;}
    constexpr pointer data() const noexcept { return a;}
    constexpr reference operator[](size_type n) noexcept { return a[n];}
    constexpr value_type operator[](size_type n) const noexcept { return a[n];}
    reference at(size_type i) noexcept { 
        check_access(i);
        return a[i];
    }
    value_type at(size_type i) const noexcept {
        check_access(i);
        return a[i];
    }


    constexpr iterator begin() noexcept { return a;}
    constexpr iterator end() noexcept { return (a+sz);}
    constexpr const_iterator begin() noexcept { return a;}
    constexpr const_iterator end() noexcept { return (a+sz);}
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end());}
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin());}
    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end());}
    constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin());} 
};

}