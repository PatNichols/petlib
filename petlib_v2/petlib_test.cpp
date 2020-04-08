

#include <petlib.hpp>



int main() {
   petlib::Array<double> a(4);
   a=10.0;
   std::cout << "a=10 \n" << a << "\n";
   std::cout << "a.size " << a.size() << "\n";
   std::cout << "a[2] = " << a[2] << "\n";
   petlib::Array<double> b(a);
   std::cout << "b(a) \n" << b << "\n";
   petlib::RandomFill< double*, double >(a.data(),a.size());
   std::cout << "rand \n" << a << "\n";
   std::cout << "a.size " << a.size() << "\n";
   std::cout << "a[2] = " << a[2] << "\n";
   b=a;
   std::cout << "a=b \n" << b << "\n";
   petlib::Array<double> c(a.size());
   c = a;
   std::cout << "c(a)      \n" << c << "\n";
   c += double(2);
   std::cout << "c(a)+=2.0 \n" << c << "\n";
   c = a + double(2);
   std::cout << "c = (a+2.)\n" << c << "\n";
   std::cout << "a+2.0 \n" << (a+2.0) << "\n";
   c = double(2) + a;
   std::cout << "c = (2.+a)\n" << c << "\n";
   std::cout << "2.0+a \n" << (2.0+a) << "\n";
   c = a;
   c += b;
   std::cout << "c(a)+=b \n" << c << "\n";
   c = a + b;
   std::cout << "c = (a+b)\n" << c << "\n";
   std::cout << "a+b \n" << (a+b) << "\n";
   petlib::Array<double> d(10);
   petlib::RandomFill< double*, double >(d.data(),d.size());
   
   std::cout << d << "\n";

   std::cout << "min " << d.Min() << "\n";
   std::cout << "imin " << d.IndexMin() << "\n";
   std::cout << "max " << d.Max() << "\n";
   std::cout << "imax " << d.IndexMax() << "\n";
   std::cout << "amax " << d.Amax() << "\n";
   std::cout << "iamax " << d.IndexAmax() << "\n";
   std::cout << "amin " << d.Amin() << "\n";
   std::cout << "iamin " << d.IndexAmin() << "\n";

   return EXIT_SUCCESS;
}