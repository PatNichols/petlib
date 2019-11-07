#include "petlib.hpp"
#include "petlib_matrix.hpp"

int main()
{
   const size_t n = 4;
   petlib::Matrix<double> a(n,n);
   std::cerr << "init a\n";
   std::cerr << "a = \n" << a << "\n";
   std::cerr << a.nrows() << " " << a.ncols() << " " << a.size() << "\n";
   petlib::randomFill<double>(a.data(),a.size());
   std::cerr << "filled a\n";
   std::cerr << "a = \n" << a << "\n";
   petlib::Matrix<double> b(a);
   std::cerr << "init b\n";
   std::cerr << "b = \n" << b << "\n";
   petlib::Matrix<double> c((a+b));
   std::cerr << "init c\n";
   std::cout << "a = \n" << a << "\n";
   std::cout << "b = \n" << b << "\n";
   std::cout << "c = \n" << c << "\n"; 

   petlib::randomFill<double>(b.data(),b.size());
   std::cerr << "fiiled b\n";
   std::cerr << "b = \n" << b << "\n";
   petlib::Matrix<double> d(n,n);
   c=a+b;
   d=a;
   d+=b;
   std::cout << "c = a+b \n" << c << "\n";
   std::cout << "c(a) += b \n" << d << "\n"; 
   c=a-b;
   d=a;
   d-=b;
   std::cout << "c = a-b \n" << c << "\n";
   std::cout << "c(a) -= b \n" << d << "\n"; 
   c=a*b;
   d=a;
   d*=b;
   std::cout << "c = a*b \n" << c << "\n";
   std::cout << "c(a) *= b \n" << d << "\n"; 
   c=a/b;
   d=a;
   d/=b;
   std::cout << "c = a/b \n" << c << "\n";
   std::cout << "c(a) /= b \n" << d << "\n"; 
   
   c=a+2.;
   d=a;
   d+=2.;
   std::cout << "c = a+2. \n" << c << "\n";
   std::cout << "c(a) += 2. \n" << d << "\n"; 
   c=a-2.;
   d=a;
   d-=2.;
   std::cout << "c = a-2. \n" << c << "\n";
   std::cout << "c(a) -= 2. \n" << d << "\n"; 
   c=a*2.;
   d=a;
   d*=2.;
   std::cout << "c = a*2. \n" << c << "\n";
   std::cout << "c(a) *= 2. \n" << d << "\n"; 
   c=a/2.;
   d=a;
   d/=2.;
   std::cout << "c = a/2. \n" << c << "\n";
   std::cout << "c(a) /= 2. \n" << d << "\n"; 
   
   
   
      

}