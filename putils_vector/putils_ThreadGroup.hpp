
namespace putils {

struct ThreadGroup {
    std::ptrdiff_t nthrds;
    pthread_t * pth;
    pthread_attr_t *attr;

    ThreadGroup(std::ptrdiff_t num_threads):nthrds(num_threads),
       pth(0x0),attr(0x0) {}
    
    ~ThreadGroup() {
        
    }   

    void setAttribute()
    {
      attr = new pthread_attr_t[nthrds];  
      for (size_type i=0;i<nthrds;++i) 
          pthread_attribute_init(&attr[i]);
      for (size_type i=0;i<nthrds;++i)
          pthread_attr_    
    };

    
    void run( void * fun( void * ), void *args)
    {
        pth = malloc(sizeof(pthread_t)*nth);
        for ( ptrdiff_t n = 0; n < nthreads; ++n) {
            int e=pthread_create(&pth[n],attr,fun,args);
            if (e) {
                throw system_thread_exception(e,pth,n);
            }
        }
        for (ptrdiff_t n=0;n<nthreads;++n) {
            
            int e = pthread_join(pth[n],&status);
            if (e) {
                throw_system_exception(e,pth,n,status);
            }
        }
        free(pth);
    }
    
};


struct Barrier {
    std::atomic<int64_t> counter;
    std::int64_t nwait;
    pthread_cond_t *cond;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond_c;
    pthread_mutex_t *mutex_c;
    
    Barrier()=delete;
    explicit Barrier(const Barrier& b,std::int64_t id)
    
    explicit Barrier(int64_t num_of_threads):
    {
        counter.store(num_of_threads);
        nwait=num_of_threads);
        cond = new pthread_cond_t[2];
        mutex = new pthread_mutex_t[2];
        cond_c = cond;
        mutex_c = mutex;
        pthread_cond_init(cond,0x0);
        pthread_cond_init(cond+1,0x0);
        pthread_mutex_init(mutex,0x0);
        pthread_mutex_init(mutex+1,0x0);
    }
    
    ~Barrier() {
        for (size_type i=0;i<2;++i) pthread_cond_destroy(cond+i);
        for (size_type i=0;i<2;++i) pthread_mutex_destroy(mutex+i);
        delete [] cond;
        delete [] mutex;
    }
    
    void reduce(void *f(void *),void *args) {
        pthread_mutex_lock(mutex_c);
        if (counter.fetch_and_sub(1)!=1) {
            pthread_cond_wait(cond_c,mutex_c);
            pthread_mutex_unlock(mutex_c);
        }else{
            counter.store(nwait);
            f(args);
            pthread_mutex_unlock(mutex_c);
            pthread_cond_broadcast(cond_c);
        }    
    }
    void wait() {
        pthread_mutex_lock(mutex_c);
        if (counter.fetch_and_sub(1)!=1) {
            pthread_cond_wait(cond_c,mutex_c);
            pthread_mutex_unlock(mutex_c);
        }else{
            counter.store(nwait);
            pthread_mutex_unlock(mutex_c);
            pthread_cond_broadcast(cond_c);
        }
    }
    void wait_and_drop() {
        pthread_mutex_lock(mutex_c);
        if (counter.fetch_and_sub(1)!=1) {
            pthread_mutex_unlock(mutex_c);
            pthread_exit();
        }else{
            counter.store(nwait);
            pthread_mutex_unlock(mutex_c);
        }
    };
}; 


struct CTBarrier {
    

};



}
