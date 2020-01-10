//#ifndef _BARRIER_HPP_
//#define _BARRIER_HPP_
#include <pthread.h>
#include <sys/time.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace putils {

struct barrier {
    std::mutex m_mutex;
    std::condition_variable m_cond;
    atomic<int32_t> m_count;
    atomic<int32_t> m_generation;    
    int32_t nth;
         
    explicit barrier(uint32_t count):
      m_mutex(),m_cond(),
      m_count(count),m_generation(0),nth(count) {}
      
    bool wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        int32_t gen = m_generation;
        
        if (--m_count == 0) {
            ++m_generation;
            m_count = nth;
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
    bool reduce(Fun f,void *args) {
        std::unique_lock<std::mutex> lock(m_mutex);
        int32_t gen = m_generation;
        
        if (--m_count == 0) {
            f(args);
            ++m_generation;
            m_count = nth;
            lock.unlock();
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


struct Stopwatch {
  chrono::duration<double> acc;
  chrono::high_resolution_clock::time_point ts;

 public:
  Stopwatch() : acc(0), ts(){};

  void start() throw() { ts = chrono::high_resolution_clock::now(); };
  void stop() throw() {
    chrono::high_resolution_clock::time_point tf =
        chrono::high_resolution_clock::now();
    acc += chrono::duration_cast<chrono::duration<double> >(tf - ts);
  };
  double time() const throw() { return acc.count(); };
  void clear() throw() { acc = chrono::duration<double>(0); };
};

struct red_sum_args {
  double *asum;
  int32_t nth;
};

auto red_summer = [] ( void * args_in ) {
  red_sum_args *args_ = reinterpret_cast< red_sum_args *>(args_in);
  const int32_t n = args_->nth;
  double * const asum = args_->asum;
  double sum = 0.;
  for (int32_t k=0;k<n;++k) sum += asum[k];
  asum[0]=sum;
};

struct Summer {
  putils::barrier *b;
  int lo, hi, n, tid, nthreads, npass;
  double *a;
  double *asum;
  double asum_total;

  Summer(int nth, int nsize)
      : b(new putils::barrier(nth)),
        lo(0),
        hi(0),
        n(nsize),
        tid(0),
        nthreads(nth),
        npass(2000),
        a(new double[nsize]),
        asum(new double[nth]),
        asum_total(0.) {
    int bsz = nsize / nth;
    int xsz = nsize % nth;
    if (xsz)
      hi = (bsz + 1);
    else
      hi = bsz;
    for (int j = 0; j < nsize; ++j) {
      a[j] = (2. * (((double)rand()) / RAND_MAX) - 1.);
    }
    npass = (90000000) / nsize;
    if (npass < 1) npass = 1;
    //        npass=1;
    //        fprintf(stderr,"a %d %d %d\n",tid,lo,hi);
  };

  Summer(Summer &summ, int thread_id)
      : b(summ.b),
        lo(0),
        hi(0),
        n(summ.n),
        tid(thread_id),
        nthreads(summ.nthreads),
        a(summ.a),
        asum(summ.asum),
        asum_total(summ.asum_total) {
    int bsz = n / nthreads;
    int xsz = n % nthreads;
    if (thread_id < xsz) {
      lo = thread_id * (bsz + 1);
      hi = lo + bsz + 1;
    } else {
      lo = xsz + thread_id * bsz;
      hi = lo + bsz;
    }
    npass = summ.npass;
    //        fprintf(stderr,"a %d %d %d\n",tid,lo,hi);
  };

  ~Summer() {
    //        fprintf(stderr,"summer %d is done\n",tid);
    if (tid == 0) {
      delete b;
      delete[] asum;
      delete[] a;
    }
  };

  void reduce_sum() {
    double s = 0;
    for (int j = 0; j < nthreads; j++) s += asum[j];
    asum_total = s;
  };

  void sum() {
    red_sum_args sum_args;
    sum_args.asum = asum;
    sum_args.nth = this->nthreads;
    for (int k = 0; k < npass; ++k) {
      double s = 0;
      for (int j = lo; j < hi; ++j) s += a[j] * a[j];
      asum[tid] = s;
      //        fprintf(stderr,"asum [ %d ] = %lg\n",tid,s);
      b->reduce(red_summer,reinterpret_cast<void*>(&sum_args));
      asum_total = asum[0];
    }
    if (tid == 0) {
      fprintf(stderr, "sum = %lg\n", asum_total);
    }
  }
};

void run_summer(void *arg) {
  Summer *s = reinterpret_cast<Summer *>(arg);
  s->sum();
}

void test_(int n, int nthr) {
  std::thread **thd = new std::thread *[nthr];
  Summer **summer = new Summer *[nthr];
  summer[0] = new Summer(nthr, n);
  for (int k = 1; k < nthr; ++k) summer[k] = new Summer(*summer[0], k);
  Stopwatch timer;
  cerr << "Starting timer " << n << " " << nthr << "\n";
  timer.start();
  for (int k = 0; k < nthr; ++k) {
    thd[k] = new std::thread(run_summer, reinterpret_cast<void *>(summer[k]));
  }
  for (int k = 0; k < nthr; ++k) {
    thd[k]->join();
  }
  timer.stop();
  fprintf(stderr, "%12d %3d %15.3lg\n", n, nthr, timer.time());
  for (int k = nthr; k--;) delete summer[k];
  for (int k = nthr; k--;) delete thd[k];
  delete[] summer;
  delete[] thd;
}

int main() {
  //    int sz[]= {100,300,400, 500, 700, 800, 1000, 1200, 10000,-1};
  int sz[] = {100000, 200000, 500000, 1000000, 2000000, 50000000, -1};
  int nt[] = {1, 2, 3, 4, 5, 6, 7, 8, -1};
  int ks = 0;
  int kt;

  while (sz[ks] != -1) {
    kt = 0;
    while (nt[kt] != -1) {
      test_(sz[ks], nt[kt]);
      ++kt;
      fprintf(stderr,"XXXXXXX\n");
    }
    fprintf(stderr,"YYYYYYYYYY\n");
    ++ks; 
  }
}
