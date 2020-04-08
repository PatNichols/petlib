#ifndef PUTILS_ALLOCATOR_HPP
#define PUTILS_ALLOCATOR_HPP



namespace putils {

#define PUTILS_DEFAULT_ALIGNMENT std::size_t(32)

template < typename T, std::size_t DEFAULT_ALIGNMENT=PUTILS_DEFAULT_ALIGNMENT > struct allocator {
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type is_always_equal;    

    template < class U > struct rebind {
        typedef allocator<U,ALIGN> other;
     };

    constexpr allocator() {}
    constexpr allocator(const allocator alloc&) {}
    template < class U >
    constexpr allocator(const allocator<U>& other) noexcept {}
    
    constexpr pointer allocate(size_type nmem, pointer hint=nullptr) const {
        pointer ptr = reinterpret_cast<pointer>(std::align_alloc(DEFAULT_ALIGNMENT,sizeof(T)*nmem);
        if (ptr) return ptr;
        throw bad_alloc();
    }
    constexpr pointer allocate(size_type bytes,size_type new_alignment) {
        pointer ptr = reinterpret_cast<pointer>(std::align_alloc(new_alignment,sizeof(T)*nmem);
        if (ptr) return ptr;
        throw bad_alloc();            
    }
    constexpr void construct( pointer p , Args&&...args) {
    }
    constexpr void deallocate( pointer ptr ) { std::free(ptr);}
    template < class V >
    constexpr void construct( pointer p, const V& v) {
       return new( reinterpret_cast<void*>(p) ) value_type(v);             
    }
    template < class V >
    constexpr void construct( pointer p, V&& v) {
       return new( reinterpret_cast<void*>(p) ) value_type(v);             
    }
    constexpr void construct( pointer p) {
       return new( reinterpret_cast<void*>(p) ) value_type();             
    }
    constexpr void destroy( pointer p) {
        ptr->~value_type();
    }
    constexpr size_type max_size() const noexcept { 
        size_type nmaxx=numeric_limits<std::size_t>::max()/sizeof(value_type);
        return maxx;
    }       
};

template < typename T, typename U , std::size_t ALIGN>
constexpr bool operator==(const allocator<T,ALIGN>& x,const allocator<U,ALIGN>& y) { return true;}

template < typename T, typename U , std::size_t ALIGN>
constexpr bool operator!=(const allocator<T,ALIGN>& x,const allocator<U,ALIGN>& y) { return false;}






}
#endif