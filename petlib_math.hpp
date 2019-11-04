#ifndef PETLIB_MATH_HPP
#define PETLIB_MATH_HPP
#include <cmath>
#include <random>
#include <cfloat>

namespace petlib {
	
template < typename T > 
constexpr T abs(const T& x) noexcept {
   T zero(0);
   if (x>=zero) return x;
   return -x;
}

template < typename T > constexpr bool abs_cmp( T x , T y) noexcept {
	return abs<T>(x)<abs<T>(y);
}

template < typename T > constexpr T max( T x, T y) noexcept {
        return (x>=y)?x:y;
}
template < typename T > constexpr T min( T x, T y) noexcept {
        return (x>=y)?x:y;
}

template < typename T > constexpr T zero() noexcept { return T(0.f);}
template < typename T > constexpr T one() noexcept { return T(1.f);}
template < typename T > constexpr T two() noexcept { return T(2.f);}
template < typename T > constexpr T half() noexcept { return T(0.5f);}
template < typename T > constexpr T neg_one() noexcept { return T(-1.f);}

template < typename T >
struct RandomEngine {
   std::minstd_rand rgen;
   typedef std::minstd_rand::result_type result_type;

   result_type next() { return rgen();}
   
   T urand() noexcept {
        return T(next());
   }
   T irand() noexcept {
        T x= T(next());
        T y= x - rgen.max();
        return (x + y);;
   }
   T frand() noexcept {
       T fact = T(1)/rgen.max();
       T x = T(next())*fact;
       return (x+x-T(1));
   }  
};

template < typename T >
constexpr void randomFill(T * arr, std::size_t n)
{
   RandomEngine<T> r;
   for (auto i = std::size_t(0);i<n;++i) arr[i] = r.frand(); 
}


template <typename T> class numeric_traits {
public:
  static inline size_t radix() { return FLT_RADIX; };
  static inline T eps() { return FLT_EPSILON; };
  static inline T max() { return FLT_MAX; };
  static inline T min() { return FLT_MIN; };
  static inline size_t dig() { return FLT_DIG; };
  static inline int max_exp() { return FLT_MAX_EXP; };
  static inline int min_exp() { return FLT_MIN_EXP; };
  static inline int max_10_exp() { return FLT_MAX_10_EXP; };
  static inline int min_10_exp() { return FLT_MIN_10_EXP; };
  static inline int mant_digs() { return FLT_MANT_DIG; };
  static inline size_t block_size() { return 64; };
  static inline size_t slab_size() { return 256; };
};

template <> class numeric_traits<double> {
public:
  static inline size_t radix() { return FLT_RADIX; };
  static inline double eps() { return DBL_EPSILON; };
  static inline double max() { return DBL_MAX; };
  static inline double min() { return DBL_MIN; };
  static inline size_t dig() { return DBL_DIG; };
  static inline int max_exp() { return DBL_MAX_EXP; };
  static inline int min_exp() { return DBL_MIN_EXP; };
  static inline int max_10_exp() { return DBL_MAX_10_EXP; };
  static inline int min_10_exp() { return DBL_MIN_10_EXP; };
  static inline int mant_digs() { return DBL_MANT_DIG; };
  static inline size_t block_size() { return 32; };
  static inline size_t slab_size() { return 32; };
};

template <> class numeric_traits<long double> {
public:
  static inline size_t radix() { return FLT_RADIX; };
  static inline long double eps() { return LDBL_EPSILON; };
  static inline long double max() { return LDBL_MAX; };
  static inline long double min() { return LDBL_MIN; };
  static inline size_t dig() { return LDBL_DIG; };
  static inline int max_exp() { return LDBL_MAX_EXP; };
  static inline int min_exp() { return LDBL_MIN_EXP; };
  static inline int max_10_exp() { return LDBL_MAX_10_EXP; };
  static inline int min_10_exp() { return LDBL_MIN_10_EXP; };
  static inline int mant_digs() { return LDBL_MANT_DIG; };
  static inline size_t block_size() { return 32; };
  static inline size_t slab_size() { return 32; };
};

}
#endif