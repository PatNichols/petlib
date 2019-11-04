

namespace petlib {

#define PETLIB_ALIGN 32

void* operator new(size_t sz, size_t align) {
  void* ptr = std::align_malloc(PETLIB_ALIGN, sz);
  if (ptr) return ptr;
  std::cerr << "bad alloc\n";
  throw std::bad_alloc();
}

void* operator new[](size_t sz, size_t align) {
  void* ptr = std::align_malloc(PETLIB_ALIGN, sz);
  if (ptr) return ptr;
  std::cerr << "bad alloc\n";
  throw std::bad_alloc();
}

void operator delete(void* ptr) { free(ptr); };
void operator delete[](void* ptr) { free(ptr); };

template <typename T,std::size_t N>>
struct AlignedAllocator {
  typedef T value_type;
  typedef T& reference_type;
  typedef T* pointer_type;
  typedef const T* const_pointer_type;
  typedef const T& const_reference_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  template <typename U>
  struct rebind {
    typedef aligned_allocator<U, N> other;
  };

  inline aligned_allocator() throw() {}
  inline aligned_allocator(const aligned_allocator&) throw() {}

  template <class U>
  inline aligned_allocator(const aligned_allocator<U, N>&) throw() {}

  inline ~aligned_allocator() throw() {}

  inline pointer address(reference r) { return &r; }
  inline const_pointer address(const_reference r) const { return &r; }

  pointer allocate(size_type n,
                   typename std::allocator<void>::const_pointer hint = 0) {
      pointer res = reinterpret_cast<pointer>(std::aligned_malloc(sizeof(T)*n,PETLIB_ALIGN));
      if (res) return res;
      throw bad_alloc();                
  };
  
  inline void deallocate(pointer p, size_type) {
      if (p) free( reinterpret_cast<void*>(p) );
  };

  inline void construct(pointer p, const_reference value) {
    new (p) value_type(value);
  }
  inline void destroy(pointer p) { p->~value_type(); }

  inline size_type max_size() const throw() {
    return size_type(-1) / sizeof(T);
  }

  inline bool operator==(const aligned_allocator&) { return true; }
  inline bool operator!=(const aligned_allocator& rhs) {
    return !operator==(rhs);
  }
};



template <typename T>
struct memblock

}  // namespace petlib
