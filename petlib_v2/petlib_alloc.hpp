#ifndef PETLIB_ALLOC_HPP
#define PETLIB_ALLOC_HPP
#include <memory>
#include <cstdlib>

namespace petlib {

template < typename Tp , std::size_t align_value = size_t(32) >
struct AlignedAllocator: std::allocator<Tp> {
    typedef Tp value_type;
    typedef Tp& reference;
    typedef const Tp& const_reference;
    typedef Tp* pointer;
    typedef const Tp* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp* iterator;
    typedef const Tp* const_iterator;
    typedef petlib::AlignedAllocator< value_type, 32ULL > allocator_type;

    template < typename U >
    struct rebind {
        typedef petlib::AlignedAllocator<U> other;
    };    

    pointer allocate(size_type n) {
        void * ptr;
        int e = posix_memalign(&ptr,align_value,n*sizeof(Tp));
        if (ptr && !e) return reinterpret_cast< pointer>(ptr);
        throw std::bad_alloc();
    }
    
    void deallocate(pointer p,size_type n) {
        free((void *)p);
    }
    
};

}
#endif