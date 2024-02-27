#include <cstdlib>
#include <memory>
#include <span>
#include <vector>
#include <iostream>

template < size_t N >
void use_span(std::span<double,N>& v)
{
    for ( double& x : v)
    {
        std::cout << x << "\n";
    }
}

template < size_t N >
void use_span2(std::span<double,N>& v)
{
    for (double& x : v) {
        x = x + x;
    }
    use_span(v);
}

int main()
{
    std::vector<double> vs(6);
    int i=1;
    for (double& x:vs) {
        x=double(i);
        ++i;
    }
    std::span<double,4> vspan(vs.data()+1,4);
    use_span(vspan );
    std::span<double,3> vspan2(vs.data()+1,3);
    use_span( vspan2 );
    for (double& x:vspan2) {
        std::cout << x << "\n";
    }
}