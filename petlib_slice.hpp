#ifndef SLICE_HPP
#define SLICE_HPP
#include <cstdlib>
#include <iterator>

namespace petlib {

struct Slice {
  constexpr Slice() = delete;
  constexpr Slice(const Slice &sl) : off(sl.off), len(sl.len), str(sl.str) {}
  constexpr Slice(Slice &&sl) : off(sl.off), len(sl.len), str(sl.str) {}
  constexpr Slice(std::size_t offset_,std::size_t length_,std::ptrdiff_t stride_): off(offset_),
   len(length_), str(stride_) {}
  ~Slice() = default;
  constexpr Slice& operator=(Slice &&sl) noexcept {
    off = sl.off;
    len = sl.len;
    str = sl.str;
    return *this;
  }
  constexpr Slice& operator=(const Slice &sl) noexcept {
    off = sl.off;
    len = sl.len;
    str = sl.str;
    return *this;
  }

  constexpr Slice operator()(Slice &sl) noexcept {
    return Slice(off + sl.offset() * str, sl.size(), sl.stride());
  }
  constexpr std::size_t offset() const noexcept { return off; }
  constexpr std::size_t size() const noexcept { return len; }
  constexpr std::ptrdiff_t stride() const noexcept { return str; }
  constexpr std::ptrdiff_t end_index() const noexcept {
    return str * static_cast<std::ptrdiff_t>(len);
  }
  std::size_t off, len;
  std::ptrdiff_t str;
};

template <typename T>
struct SliceIterator {
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef T value_t;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef std::random_access_iterator_tag iterator_category;

  constexpr SliceIterator() = default;
  constexpr SliceIterator(const SliceIterator &&s) : p(s.p), str(s.str){};
  constexpr SliceIterator(pointer ptr, difference_type Stride) : p(ptr), str(Stride) {}
  constexpr SliceIterator(SliceIterator &&s) : p(s.p), str(s.str) {}

  constexpr SliceIterator &operator=(const SliceIterator &s) noexcept {
    p = s.p;
    str = s.str;
    return *this;
  };
  constexpr SliceIterator &operator=(SliceIterator &&s) noexcept {
    p = s.p;
    str = s.str;
    return *this;
  };

  constexpr SliceIterator& operator++() noexcept {
    p += str;
    return *this;
  }
  constexpr SliceIterator& operator++(int) noexcept {
    SliceIterator<T> clone(p, str);
    p += str;
    return clone;
  }
  constexpr SliceIterator& operator--() noexcept {
    p -= str;
    return *this;
  }
  constexpr SliceIterator& operator--(int) noexcept {
    SliceIterator<T> clone(p, str);
    p -= str;
    return clone;
  }
  constexpr bool operator==(const SliceIterator<T> &s) const noexcept {
    return p == s.p;
  }
  constexpr bool operator!=(const SliceIterator<T> &s) const noexcept {
    return p != s.p;
  }
  constexpr bool operator>=(const SliceIterator<T> &s) const noexcept {
    return p >= s.p;
  }
  constexpr bool operator<=(const SliceIterator<T> &s) const noexcept {
    return p <= s.p;
  }

  constexpr reference operator*() noexcept { return *p; }
  constexpr const_reference operator*() const noexcept { return *p; }
  constexpr pointer operator->() noexcept { return p; }
  constexpr const pointer operator->() const noexcept { return &*p; }
  constexpr SliceIterator operator+=(difference_type n) noexcept {
    return SliceIterator(p + n * str, str);
  };
  constexpr SliceIterator operator-=(difference_type n) noexcept {
    return SliceIterator(p - n * str, str);
  }
  constexpr reference operator[](difference_type i) noexcept {
    return p[i * str];
  }
  constexpr const_reference operator[](difference_type i) const noexcept {
    return p[i * str];
  }

  constexpr pointer data() noexcept { return p; }
  constexpr difference_type stride() noexcept { return str; }

  pointer p;
  difference_type str;
};

template <typename T>
constexpr SliceIterator<T> operator+(SliceIterator<T> &x,
                                     std::ptrdiff_t n) noexcept {
  return SliceIterator<T>(x.data() + n * x.stride(), x.stride());
}

template <typename T>
constexpr SliceIterator<T> operator-(SliceIterator<T> &x,
                                     std::ptrdiff_t n) noexcept {
  return SliceIterator<T>(x.data() - n * x.stride(), x.stride());
}

template <typename T>
constexpr SliceIterator<T> operator+(std::ptrdiff_t n,
                                     SliceIterator<T> &x) noexcept {
  return SliceIterator<T>(x.data() + n * x.stride(), x.stride());
}

}  // namespace petlib
#endif