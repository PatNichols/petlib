#ifndef PETLIB_ARRAY_RANDOM_HPP
#define PETLIB_ARRAY_RANDOM_HPP
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstddef>

namespace petlib {

template < typename value_type > inline value_type random_num()
{
    value_type x(rand());
    x /= RAND_MAX;
    x = x + x - value_type(1);
    return x; 
} 

template <> inline std::uint8_t random_num< std::uint8_t >() {
    return std::uint8_t(rand());
}

template <> inline std::uint16_t random_num< std::uint16_t >() {
    return std::uint16_t(rand());
}

template <> inline std::uint32_t random_num< std::uint32_t >() {
    return  std::uint32_t(rand());
}

template <> inline std::uint64_t random_num< std::uint64_t >() {
    return std::uint64_t(rand());
}

template < class iter_t, typename value_type > inline void RandomFill( iter_t a, std::size_t n)
{
    srand(time(0));
    for (std::size_t i=0;i<n;++i,++a) {
        *a = petlib::random_num< value_type >(); 
    }
}

}
#endif