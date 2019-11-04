

namespace petlib {

#define PETLIB_MAKE_LOOP(name,sym_)\
template < typename T> \
constexpr void name##Assign(T * arr, const T * src, std::size_t n , std::ptrdiff_t str) \
{ \
    std::size_t i=0;\
    std::ptrdiff_t ix=0;\
    for (;i<n;++i,ix+=str) arr[ix] sym_ src[i]; \
}

PETLIB_MAKE_LOOP(Add,+=)
PETLIB_MAKE_LOOP(Sub,-=)
PETLIB_MAKE_LOOP(Mul,*=)
PETLIB_MAKE_LOOP(Div,/=)
#undef PETLIB_MAKE_LOOP


}