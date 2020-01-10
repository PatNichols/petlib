
#include <putils_vector.hpp>

template < typename T , class Alloc>
void print_vec(putils::vector<T,Alloc>& v)
{
    std::cerr << v.size() << " " << v.capacity() << "\n";
    if (v.size()) {
        std::cerr << "( " << v[0];
        typename putils::vector<T,Alloc>::iterator iter = v.begin();
        typename putils::vector<T,Alloc>::iterator last = v.end();     
        ++iter;
        while (iter!=last) {
            std::cerr << ", " << *iter;
            ++iter;
        }
        std::cerr << ")\n";
    }  else {
        std::cerr << "empty vector\n";
    } 
}


int main()
{
    putils::vector<double,std::allocator<double>> v;
    v.push_back(1.);
    v.push_back(2.);
    v.push_back(3.);
    v.push_back(4.);
    v.push_back(5.);
    v.push_back(6.);
    v.push_back(7.);
        
    print_vec(v);
    
    v.reserve(10);
    
    print_vec(v);
    
    v.shrink_to_fit();
    
    print_vec(v);
    
    typename putils::vector<double>::iterator viter = v.begin();
    viter += 2;
    double x=15.0;
    v.insert(viter,x);
    
    print_vec(v);

    std::cerr << "push front\n";
    v.push_front(10.0);
    
    print_vec(v);

    std::cerr << "push back\n";
    v.push_back(20.0);
    
    print_vec(v);

    viter=v.begin();
    viter+=2;
    
    std::cerr << "insert (2, 9)\n";
    v.insert(viter,9.0);
    
    print_vec(v);

    std::cerr << "here\n";   
    
    viter = v.begin()+2;
    v.erase(viter,std::size_t(2));
       
    print_vec(v);
    
    putils::vector<double> e({1.2,2.4,3.6,4.8,5,});
    
    print_vec(e); 

    e = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.5 };
    
    print_vec(e);
    
}