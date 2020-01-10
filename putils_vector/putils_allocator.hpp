#ifndef PUTILS_ALLOCATOR_HPP
#define PUTILS_ALLOCATOR_HPP

namespace putils {

template < typename T >
struct allocator::std::allocator<T> {
    
    typedef std::size_type size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef std::true_type is_always_equal;
    typedef std::true_type propagate_on_container_move_assignment; 
    constexpr static size_type putils_alignment=32;

    pointer allocate(size_type n) {
        pointer p=reinterpret_cast<pointer>(std::aligned_alloc(putils_alignment,sizeof(T)*n));
        if (p) return p;
        throw bad_alloc();
    }

    pointer allocate(size_type n,size_type align_) {
        pointer p=reinterpret_cast<pointer>(std::aligned_alloc(align_,sizeof(T)*n));
        if (p) return p;
        throw bad_alloc();
    }
  
    pointer callocate(size_type n) {
        pointer p = this->allocate(n);
        for (size_type i=0;i<n;++i) p[i]=T{0};
        return p;
    }
    
    void deallocate(pointer p)
    {
        free(p);
    }

    template < class U, class ...Args >
    void construct(U* ptr, void&& ... Args)
    {
        U* ptr_place = reinterpret_cast<U*>(allocate(sizeof(U)));
        ptr = new (ptr_place) U(Args);
    }
    
};





}
#endif

}
