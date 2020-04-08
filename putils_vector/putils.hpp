


#include <cstdlib>
#include <cstdio>


namespace putils {

inline void * Xmalloc(size_t nsize) {
    void *ptr = malloc(nsize);
    if (ptr) return ptr;
    std::cerr << "cannot malloc " << nsize << " bytes\n";
    exit(EXIT_FAILURE);
}

struct ThreadGroup {
    pthread_t * pth;
    pthread_attr_t * attr;
    size_type nth;
    
    ThreadGroup(size_type nthreads):pth(0),nth(nthreads)
    {
        pth = new pthread_t[nth];
    }  
    
    ~ThreadGroup() { delete [] pth; }
    
    void run( void * fun ( void * ) , void *args) {
        for (size_type i=0;i<nth;++i) {
            int err = pthread_create(pth+i,fun,args);
            if (err) {
                std::cerr << "could not launch thread "<<i<<" from ThreadGroup\n";
                throw std::exception();
            }
        }
        for (size_type i=0;i<nth;++i) {
            long status;
            int err = pthread_join(pth[i],&status);
            if (err) {
                std::cerr << " could not join thread " << i << " from ThreadGrp\n";
                throw std::exception(); 
            }
        }    
        std::cerr << "threadGroup::run completed\n";
    } 
    
};


struct cyclic_barrier {
    pthread_mutex_t *mutex;
    pthread_mutex_t *mutex_c;
    pthread_cond_t *cond;
    pthread_cond_t *cond_c;
    std::atomic<std::int64_t> *cnter;
    std::atomic<std::int64_t> *cnter_c;
    std::int64_t nwait;
    std::int64_t tid;
    
    cyclic_barrier(std:int64_t nthreads):mutex(new pthread_mutex[2]),
        mutex_c(0x0),
        cond(new pthread_cond_t[2]),cond_c(0x0),
        cnter(new std::atomic<std::int64_t>[2]),cnter_c(0x0),nwait(nthreads),tid(0) {
        mutex_c = mutex;
        cond_c = cond;
        cnter_c = cnter;
    }
    cyclic_barrier(cyclic_barrier& barr,std:int64_t thread_id):mutex(barr.mutex),
        mutex_c(barr.mutex_c),
        cond(barr.cond),cond_c(barr.cond_c),
        cnter(barr.cnter),cnter_c(barr.cnter_c),nwait(barr.nwait),tid(thread_id) {
    }
    
    ~cyclic_barrier() {
        if (!tid) {
            delete [] cnter;
            delete [] cond;
            delete [] mutex;
        }        
    }
    
    void reset() {
        if (mutex_c==mutex) {
            mutex_c = mutex+1;
            cond_c = cond+1;
            cnter_c = cnter+1;
        }else{
            mutex_c = mutex;
            cond_c = cond;
            cnter_c = cnter;
        }
        
    }
    
    void wait() {
        pthread_mutex_lock(mutex_c);
        if (cnter_c.fetch_and_sub(1)!=1) {
            pthread_cond_wait(mutex_c,cond_c);
            pthread_mutex_unlock(mutex_c);
        }else{
            pthread_mutex_unlock(mutex_c);
            cnter_c.store(nwait);
            pthread_cond_broadcast(cond_c);
        }
    }
    void wait_and_reduce( void *f(void *), void *args) {
        pthread_mutex_lock(mutex_c);
        if (cnter_c.fetch_and_sub(1)!=1) {
            pthread_cond_wait(mutex_c,cond_c);
            pthread_mutex_unlock(mutex_c);
        }else{
            pthread_mutex_unlock(mutex_c);
            f(args);
            cnter_c.store(nwait);
            pthread_cond_broadcast(cond_c);
        }
        reset();
    }
};

struct tbarrier {

    void init() {
        int64_t level  =  0;
        int64_t nwait_ = nthreads;
        for (int64_t ilevel = 0 ; ilevel < nlevels; ++ilevel) {
            bsz <<= 1;
            nnodes = nthreads/bsz;
            extra = ((nnodes*bsz)==nthreads);
            for (size_t k=0;k<nnodes;++k) {
                nwait[ilevel][nnodes] = nnodes;
            } 
            if (extra) nwait[ilevel][nnodes-1]+=1;
        
        } 
        
    
    }


    void wait() {
        int64_t bsz = 2;
        int64_t lsz = nthread/bsz;
        int64_t off = 0;
        int64_t level  = 0;    
        while (winner) {
            int64_t level_sz = nthread/bsz;
            if ( bsz*level_sz != nthreads) 
            int64_t level_id = tid/bsz;
            pthread_mutex_t * my_mutex = mutex_c + level_id;
            pthread_cond_t * my_cond = cond_c + level_id;
            std::atomic<std::int64_t>& cnt = cnter_c[level_id];
            std::int64_t nwait_ = nwait[level_id];
            pthread_mutex_lock( my_mutex );
            if ( cnt.fetch_and_sub(1) == 1) {
                cnt.load(nwait_);
                pthread_mutex_unlock( my_mutex );
                bsz <<=1;
                ++level;
            }else{
                winner = 0
                pthread_cond_wait(my_cond,my_mutex);
                pthread_mutex_unlock(my_mutex);
                break;
            }
        }
        while(level) {
            bsz >>=1;
            int64_t level_sz = nthreads/bsz; 
            int64_t level_id = tid/bsz;
            pthread_mutex_t * my_mutex = mutex_c + level_id;
            pthread_cond_t * my_cond = cond_c + level_id;
            std::atomic<std::int64_t>& cnt = cnter_c[level_id];
            std::int64_t nwait_ = nwait[level_id];
            cnt.load(nwait);
            pthread_cond_broadcast( my_cond );
            --level;
        }        
        reset();
    }
    
};

