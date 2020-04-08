



namespace putils {

    
struct shared_refcount {

    atomic<std::int32_t> cnt;
    
    std::int32_t add() {
        return cnt.fetch_and_add(1);
    }
    std::int32_t sub() {
        return cnt.fetch_and_sub(1);
    }
    std::int32_t get() {
        return cnt.load(); 
    }
    std::int32_t exchange(std::int32_t new_value=0) {
        return cnt.exchange(new_value);
    }
}

template < typename T, class Alloc = putils::allocator<T>, class RcntAlloc = putils::allocator<shared_refcnt> >
struct rc_ptr {
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef RcntAlloc rcnt_alloc_;
    typedef Alloc alloc_; 
    
    T* ptr;
    shared_refcount *rcnt;
    
    rc_ptr(const rc_ptr& p) = delete ;
    
    rc_ptr():ptr(nullptr),rcnt(null_ptr) {}
    
    explicit rc_ptr(rc_ptr& p) {
        ptr = p.ptr;
        rcnt = p.rcnt;
        rcnt->add(1);
    }
    
    rc_ptr(rc_ptr&& p) {
        ptr = p.ptr;
        rcnt = p.rcnt;
        p.reset(nullptr);
    }
        
    rc_ptr(pointer p):ptr(p),rcnt(rcnt_alloc_.allocate(1)) {
    }

    rc_ptr(size_type n,const_reference v):ptr(alloc_.allocate(n,v)),rcnt(rcnt_alloc_.allocate(1)) {
    }    
    rc_ptr(size_type n,value_type v):ptr(alloc_.allocate(n,v)),rcnt(rcnt_alloc_.allocate(1)) {
    }    
    rc_ptr(size_type n,T&& v):ptr(alloc_.allocate(n,v)),rcnt(rcnt_alloc_.allocate(1)) {
    }    
    
    template < void ... Args >
    rc_ptr(Args&&...args):ptr(alloc_.allocate(args)),rcnt(rcnt_alloc_.allocate(1)) {}

    ~rc_ptr() {
        if (!rcnt->sub()) {
            alloc_.deallocate(ptr);
        }
        rcnt_alloc_.dealloc(rcnt);
    }

    constexpr rc_ptr& operator=(const rc_ptr& r) noexcept {
        if (!rcnt->sub()) alloc_.dealloc(ptr);
        ptr = r.ptr;
        rcnt->reset(r.rcnt);
        return *this;
    }
    constexpr rc_ptr& operator=(rc_ptr&& r) noexcept {
        if (!rcnt->sub()) alloc_.dealloc(ptr);
        ptr = r.ptr;
        rcnt->reset(r.rcnt);
        r.reset(nullptr);
        return *this;
    }

    pointer base() { return ptr;}
    const_pointer base() const noexcept { return ptr;}
 
    bool unique() const noexcept { return rcnt->unique();}
    bool empty() const noexcept { return ptr==nullptr;}
    size_type use_count() const noexcept { return rcnt->use_count();}
    
    pointer operator->() { return ptr;}
    reference operator*() { return *ptr;}
    rc_ptr& operator++() noexcept {
        ++ptr;
        return *this;
    }
    rc_ptr& operator++(int) noexcept {
        rc_ptr r(*this);
        ++ptr;
        return r;
    }
    rc_ptr& operator+=(difference_type n) {
        ptr+=n;
        return *this;
    }
    rc_ptr& operator--() noexcept {
        --ptr;
        return *this;
    }
    rc_ptr& operator--(int) noexcept {
        rc_ptr r(*this);
        --ptr;
        return r;
    }
    rc_ptr& operator-=(difference_type n) {
        ptr-=n;
        return *this;
    }
    
    reference operator[](size_type i) const noexcept {
        return ptr[[n];
    }
    
    void reset(pointer p=nullptr) {
        if (!rcnt->sub()) {
            alloc_.deallocate(ptr);
        }
        if (p) {
            ptr = p;
            rcnt = new atomic<uint32_t>(0);
        }else{
            ptr = nullptr;
            rcnt = nullptr;
        }
    }

    void swap(rc_ptr& r) {
        pointer tmp_ptr = ptr;
        rcnt * tmp_rcnt = rcnt;
        ptr = r.ptr;
        rcnt = r.rcnt;         
        r.ptr = tmp_ptr;
        r.rcnt = tmp_rcnt;   
    }
    
}


}
#endif