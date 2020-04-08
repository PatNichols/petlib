





template < class T >
class refcnt_ptr {
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef const T& const_reference;
    typedef const T* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    
    template < class... Args >
    refcnt_ptr(Args&&... args):ptr(make_shared<value_type>(args)),cnt(1) {};
    
    refcnt_ptr(const refcnt_ptr& rcp)
    
    refcnt_ptr(refcnt_ptr&& rcp);
    
    constexpr refcnt_ptr& operator++() noexcept
    {
        ++ptr;
        return *this;
    }
    constexpr refcnt_ptr& operator++(int) noexcept
    {
        refcnt_ptr rcp=*this;
        ++ptr;
        return rcp;
    }
    

    T * ptr;
    atomic<int32_t> cnt;
}