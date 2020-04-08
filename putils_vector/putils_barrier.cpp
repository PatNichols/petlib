#ifndef PUTILS_BARRIER_HPP
#define PUTILS_BARRIER_HPP
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <cstdlib>
#include <cstdint>
namespace putils {

class barrier {
    std::mutex m_mutex;
    std::condition_variable m_cond;
    atomic<int32_t> m_count;
    atomic<int32_t> m_generation;
    
     
    explicit barrier(uint32_t count):
      m_mutex(),m_cond(),
      m_count(count),m_generation(0),


    void wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        int32_t gen = m_generation;
        
        if (--m_count == 0) {
            ++m_generation;
            m_count = nthreads;
            lock.unlock();
            m_cond.notify_all();
            return true;
        }
        while (gen==m_generation) {
            m_cond.wait(lock);
        }
        return false;
    }
    
    template < class Fun >
    void reduce(Fun,void *args) {
        std::unique_lock<std::mutex> lock(m_mutex);
        int32_t gen = m_generation;
        
        if (--m_count == 0) {
            ++m_generation;
            m_count = nthreads;
            lock.unlock();
            Fun(args);
            m_cond.notify_all();
            return true;
        }
        while (gen==m_generation) {
            m_cond.wait(lock);
        }
        return false;

    }
};

}
#endif

}