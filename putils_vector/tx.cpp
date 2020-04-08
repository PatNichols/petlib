

#include <memory>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
int main()
{
    std::allocator<double> dalloc;
    std::size_t max_sz = dalloc.max_size();
    std::size_t mb_sz = 1<<30;
    std::size_t max_mb = max_sz/mb_sz;
    std::cerr <<     
        dalloc.max_size() << "\n";
    std::cerr << mb_sz << "\n";
    std::cerr << max_mb << "\n";
    long max_pages = sysconf(_SC_PHYS_PAGES);
    long pg_size = sysconf(_SC_PAGESIZE);
    max_sz = pg_size * max_pages;
    std::cerr << max_sz << "\n";
    std::cerr << (max_sz/mb_sz) << "\n";    
}