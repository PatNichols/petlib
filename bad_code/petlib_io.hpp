#ifndef PETLIB_IO_HPP_
#define PETLIB_IO_HPP_

#include <iomanip>
#include <iostream>

#include <petlib_array_ops.hpp>

namespace petlib {

template <typename A>
struct print_traits {
  static const int ndigits = 0;
  static const int prec = 0;
  static const bool is_int = false;
};

template <>
struct print_traits<bool> {
  static const int ndigits = 1;
  static const int prec = 0;
  static const bool is_int = true;
};

template <>
struct print_traits<char> {
  static const int ndigits = 4;
  static const int prec = 0;
  static const bool is_int = true;
};

template <>
struct print_traits<unsigned char> {
  static const int ndigits = 4;
  static const int prec = 0;
  static const bool is_int = true;
};

template <>
struct print_traits<short> {
  static const int ndigits = 7;
  static const int prec = 0;
  static const bool is_int = true;
};

template <>
struct print_traits<unsigned short> {
  static const int ndigits = 7;
  static const int prec = 0;
  static const bool is_int = true;
};

template <>
struct print_traits<int> {
  static const int ndigits = 11;
  static const int prec = 0;
  static const bool is_int = true;
};

template <>
struct print_traits<unsigned int> {
  static const int ndigits = 11;
  static const int prec = 0;
  static const bool is_int = true;
};

template <>
struct print_traits<int64_t> {
  static const int ndigits = 22;
  static const int prec = 0;
  static const bool is_int = true;
};

template <>
struct print_traits<uint64_t> {
  static const int ndigits = 22;
  static const int prec = 0;
  static const bool is_int = true;
};

template <>
struct print_traits<float> {
  static const int ndigits = 16;
  static const int prec = 7;
  static const bool is_int = false;
};

template <>
struct print_traits<double> {
  static const int ndigits = 25;
  static const int prec = 14;
  static const bool is_int = false;
};

template <>
struct print_traits<long double> {
  static const int ndigits = 50;
  static const int prec = 28;
  static const bool is_int = false;
};

template <class Array_t, typename A_t>
std::ostream& operator<<(std::ostream& os, const ArrayBase<Array_t, A_t>& a) {
  std::size_t sz = a.size();
  std::size_t ndig = print_traits<A_t>::ndigits;
  std::size_t prec = print_traits<A_t>::prec;
  std::size_t idig = print_traits<std::size_t>::ndigits;
  if (!sz) { 
    os<<"(empty)\n";
    return os;
  }
  if (sz <= 5) {
    os << "( ";
    os << std::setw(ndig) << std::setprecision(prec) << a[0];
    for (auto i=std::size_t(1); i < sz; ++i)
      os << ", " << std::setw(ndig) << std::setprecision(prec) << a[i];
    os << ")\n";
  } else {
    os << "index-value\n";
    for (auto i = std::size_t(0); i < sz; ++i)
      os << std::setw(idig) << i << " - " << std::setw(ndig)
         << std::setprecision(prec) << a[i] << "\n";
      os << "---------------\n";
  }
  return os;
}

template <class Xpr_t>
std::ostream& operator<<(std::ostream& os, const ArrayXpr<Xpr_t>& a) {
  typedef typename ArrayXpr<Xpr_t>::value_t A_t;
  std::size_t sz = a.size();
  std::size_t ndig = print_traits<A_t>::ndigits;
  std::size_t prec = print_traits<A_t>::prec;
  std::size_t idig = print_traits<std::size_t>::ndigits;
  if (!sz) {
    os << "(empty)\n";
    return os;
  }
  if (sz < 5) {
    os << "( ";
    os << std::setw(ndig) << std::setprecision(prec) << a[0];
    for (auto i=std::size_t(1); i < sz; ++i)
      os << ", " << std::setw(ndig) << std::setprecision(prec) << a[i];
    os << ")\n";
  } else {
    os << "index-value\n";
    for (auto i = std::size_t(0); i < sz; ++i)
      os << std::setw(idig) << i << " - " << std::setw(ndig)
         << std::setprecision(prec) << a[i] << "\n";
      os << "---------------\n";
  }
  return os;
}

}  // namespace petlib

#endif
