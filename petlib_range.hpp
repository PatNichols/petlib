#ifndef PETLIB_RANGE_HPP
#define PETLIB_RANGE_HPP
#include <cstdlib>
#include <iterator>

namespace petlib {

struct Range {
  constexpr Range() = delete;
  constexpr Range(std::size_t offset_, std::size_t length_)
      : off(offset_), len(length_) {}
  constexpr Range(Range& r) : off(r.off),len(r.len) {}
  constexpr Range(const Range &r) : off(r.off), len(r.len) {}
  constexpr Range(Range &&r) : off(r.off), len(r.len) {}
  ~Range() = default;
  constexpr Range& operator=(Range &&r) noexcept {
    off = r.off;
    len = r.len;
    return *this;
  }
  constexpr Range& operator=(const Range &r) noexcept {
    off = r.off;
    len = r.len;
    return *this;
  }
  constexpr Range operator()(Range &r) { return Range(off + r.offset(),r.size()); }

  constexpr std::size_t offset() const noexcept { return off; }
  constexpr std::size_t size() const noexcept { return len; }
  std::size_t off, len; 
};

template <typename T>
struct RangeIterator {
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef T value_t;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef std::random_access_iterator_tag iterator_category;

  constexpr RangeIterator() : p(nullptr) {}
  constexpr RangeIterator(RangeIterator &&r) : p(r.p) {}
  constexpr RangeIterator(pointer ptr) : p(ptr) {}
  constexpr RangeIterator(const RangeIterator &s) : p(s.p) {}
  constexpr RangeIterator& operator=(const RangeIterator &r) noexcept {
    p = r.p;
    return *this;
  }
  constexpr RangeIterator& operator=(RangeIterator &&r) noexcept {
    p = r.p;
    return *this;
  }

  constexpr RangeIterator& operator++() noexcept {
    ++p;
    return *this;
  }

  constexpr RangeIterator& operator++(int) noexcept {
    RangeIterator<T> clone(p);
    ++p;
    return clone;
  }

  constexpr RangeIterator& operator--() noexcept {
    --p;
    return *this;
  }

  constexpr RangeIterator& operator--(int) noexcept {
    RangeIterator<T> clone(p);
    --p;
    return clone;
  }
  constexpr bool operator==(const RangeIterator<T> &s) const noexcept {
    return p == s.p;
  }
  constexpr bool operator!=(const RangeIterator<T> &s) const noexcept {
    return p != s.p;
  }
  constexpr bool operator>=(const RangeIterator<T> &s) const noexcept {
    return p >= s.p;
  }
  constexpr bool operator<=(const RangeIterator<T> &s) const noexcept {
    return p <= s.p;
  }

  constexpr reference operator*() noexcept { return *p; }
  constexpr const_reference operator*() const noexcept { return *p; }
  constexpr pointer operator->() noexcept { return p; }
  constexpr const pointer operator->() const noexcept { return &*p; }
  constexpr RangeIterator operator+=(difference_type n) noexcept {
    return RangeIterator(p + n);
  };
  constexpr RangeIterator operator-=(difference_type n) noexcept {
    return RangeIterator(p - n);
  }
  constexpr reference operator[](difference_type i) noexcept { return p[i]; }
  constexpr const_reference operator[](difference_type i) const noexcept {
    return p[i];
  }

  constexpr pointer data() noexcept { return p; }
  pointer p;
};

template <typename T>
constexpr RangeIterator<T> operator+(RangeIterator<T> &x,
                                     std::ptrdiff_t n) noexcept {
  return RangeIterator<T>(x.data() + n);
}

template <typename T>
constexpr RangeIterator<T> operator-(RangeIterator<T> &x,
                                     std::ptrdiff_t n) noexcept {
  return RangeIterator<T>(x.data() - n);
}

template <typename T>
constexpr RangeIterator<T> operator+(std::ptrdiff_t n,
                                     RangeIterator<T> &x) noexcept {
  return RangeIterator<T>(x.data() + n);
}

}  // namespace petlib
#endif
