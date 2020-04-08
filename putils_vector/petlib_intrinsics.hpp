


template < typename T, size_t vec_size >
struct vec_type {
    vec_type operator+=(vec_type& v);
    vec_type operator-=(vec_type& v);
    
}   

struct vec_type<double,256>
{y
   vector< __float4 > x;

   vector< __float4 >& operator+=(const vector< __float4 >& y) {
       iterator iter = x.begin();
       iterator iend = x.end();
       while (iter!=iend) {
           __builtin_ia32_loadps(*titer,i1);
           __builtin_ia32_loadps(*yiter,i2);
           __builtin_ia32_addps(i1,i2,i3);
           
       }
   }

} 