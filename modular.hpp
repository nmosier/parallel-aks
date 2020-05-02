#pragma once

#include <ostream>
#include <cassert>

#if 0

template <typename T>
class modular {
public:

  inline modular<T> operator+(const modular<T>& other) const {
    return modular((val + other.val) % mod, mod);
  }

  inline modular<T> operator-(const modular<T>& other) const {
    return *this + (-other);
  }

  inline modular<T> operator-() const {
    return modular(mod - val, mod);
  }

  inline modular<T> operator*(const modular<T>& other) const {
    return modular((val * other.val) % mod, mod);
  }

  inline modular<T> operator/(const modular<T>& other) const {
    return *this * other.inverse();
  }

  inline modular<T> operator%(const modular<T>& mod) const {
    return *this - *this / mod;
  }

  inline modular<T>& operator=(const T& newval) { val = newval; return *this; }

  inline modular<T>& operator+=(const modular<T>& other) {
    return *this = val + other.val;
  }
  inline modular<T>& operator-=(const modular<T>& other) {
    return *this = val - other.val;
  }

  inline bool operator==(const modular<T>& other) const { 
    return val == other.val;
  }

  inline operator bool() const { return (bool) val; }

  inline modular<T> inverse() const {
    for (T i = 1; i < mod; ++i) {
      if ((val * i) % mod == T(1)) {
	return modular<T>(i, mod);
      }
    }
    std::cerr << "nonexistent inverse of " << *this << std::endl;
    abort();
  }

  modular(const T& val, const T& mod): val(val), mod(mod) {}
  modular() {}
  
  template <typename U>
  friend std::ostream& operator<<(std::ostream& os, const modular<U>& mod);

  template <typename U>
  friend std::istream& operator>>(std::istream& is, modular<U>& mod);

private:
  T val;
  T mod;
};

template <typename T> 
inline std::ostream& operator<<(std::ostream& os, const modular<T>& mod) {
  os << mod.val << " (mod " << mod.mod << ")";
  return os;
}

// NOTE: modulus must be set separately.
template <typename T>
inline std::istream& operator>>(std::istream& is, modular<T>& mod) {
  return is >> mod.val;
}

#else

template <typename T>
class modular_ring {
public:
  modular_ring(const T& mod): mod(mod) {}
  modular_ring(): mod(0) {}

  inline T plus(const T& lhs, const T& rhs) const { return (lhs + rhs) % mod; }
  inline T negate(const T& val) const { return mod - val; }
  inline T minus(const T& lhs, const T& rhs) const { return plus(lhs, negate(rhs)); }
  inline T multiplies(const T& lhs, const T& rhs) const { return (lhs * rhs) % mod; }
  inline T divides(const T& lhs, const T& rhs) const { return multiplies(lhs, inverse(rhs)); }
  inline T modulus(const T& lhs, const T& rhs) const { 
    return minus(lhs, multiplies(divides(lhs, rhs), rhs));
  }

  inline bool equal(const T& lhs, const T& rhs) const { 
    return lhs == rhs || minus(lhs, rhs) == 0;
  }

  template <typename U>
  friend std::istream& operator>>(std::istream& is, modular_ring<U>& mod);

  template <typename U>
  friend std::ostream& operator<<(std::ostream& os, const modular_ring<U>& mod);

private:
  T mod;

  inline T inverse(const T& val) const {
    for (T i = 1; i < mod; ++i) {
      if ((val * i) % mod == 1) {
	return i;
      }
    }
    std::cerr << "nonexistent inverse of " << val << std::endl;
    abort();    
  }
};

template <typename T>
inline std::istream& operator>>(std::istream& is, modular_ring<T>& mod) { return is >> mod.mod; }

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const modular_ring<T>& mod) { 
  return os << mod.mod; 
}

template <typename T>
class standard_ring {
public:
  standard_ring() {}
  
  inline T plus(const T& lhs, const T& rhs) const { return lhs + rhs; }
  inline T minus(const T& lhs, const T& rhs) const { return lhs - rhs; }
  inline T negate(const T& val) const { return -val; }
  inline T multiplies(const T& lhs, const T& rhs) const { return lhs * rhs; }
  inline T divides(const T& lhs, const T& rhs) const { return lhs / rhs; }
  inline T modulus(const T& lhs, const T& rhs) const { return lhs % rhs; }
  inline bool equal(const T& lhs, const T& rhs) const { return lhs == rhs; }

  template <typename U>
  friend std::istream& operator>>(std::istream& is, modular_ring<U>& ring);

  template <typename U>
  friend std::ostream& operator<<(std::ostream& os, const modular_ring<U>& ring);
};

template <typename T>
inline std::istream& operator>>(std::istream& is, standard_ring<T>& ring) { return is; }

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const standard_ring<T>& ring) { return os; }

#endif
