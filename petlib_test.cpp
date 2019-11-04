

#include <petlib.hpp>




int main() {
   petlib::Array<double> a(4);
   a=10.0;
   std::cout << "a=10 \n" << a << "\n";
   std::cout << "a.size " << a.size() << "\n";
   std::cout << "a[2] = " << a[2] << "\n";
   petlib::Array<double> b(a);
   std::cout << "b(a) \n" << b << "\n";
   petlib::randomFill<double>(a.data(),a.size());
   std::cout << "rand \n" << a << "\n";
   std::cout << "a.size " << a.size() << "\n";
   std::cout << "a[2] = " << a[2] << "\n";
   b=a;
   std::cout << "a=b \n" << b << "\n";
   petlib::Array<double> c(a.size());
   c = a;
   std::cout << "c(a)      \n" << c << "\n";
   c += 2;
   std::cout << "c(a)+=2.0 \n" << c << "\n";
   c = a + 2;
   std::cout << "c = (a+2.)\n" << c << "\n";
   std::cout << "a+2.0 \n" << (a+2.0) << "\n";
   c = 2 + a;
   std::cout << "c = (2.+a)\n" << c << "\n";
   std::cout << "2.0+a \n" << (2.0+a) << "\n";
   c = a;
   c += b;
   std::cout << "c(a)+=b \n" << c << "\n";
   c = a + b;
   std::cout << "c = (a+b)\n" << c << "\n";
   std::cout << "a+b \n" << (a+b) << "\n";
   petlib::Array<double> d(a*2.0);
   std::cout << d << "\n";
   return EXIT_SUCCESS;
}