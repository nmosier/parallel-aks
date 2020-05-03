#pragma once

#include <ostream>
#include <istream>
#include <map>
#include <cassert>
#include <algorithm>

#include "ring.hpp"
#include "bit-iterator.hpp"

/*
 * @tparam T type of coefficients
 * @tparam U type of powers
 */
template <typename T, typename U>
class polynomial {
public:
  typedef std::map<U,T> Coeffs;
  
  // mutable coeffs 
  class Coeff {
  public:
    inline Coeff& operator=(const T& newval) {
      if (newval == ring::zero<T>) {
	container.erase(power);
      } else {
	container[power] = newval;
      }
      return *this;
    }

    inline operator T() const {
      const auto it = container.find(power);
      return it == container.end() ? ring::zero<T> : it->second;
    }
    
    friend polynomial<T,U>;

  private:
    U power;
    Coeffs& container;

    Coeff(const U& power, Coeffs& container): power(power), container(container) {}
  };
  
  U degree() const {
    return coeffs.empty() ? ring::zero<U> : coeffs.rbegin()->first; 
  }
  
  inline Coeff operator[](const U& index) {
    return Coeff(index, coeffs);
  }

  inline const T& at(const U& index) const { return coeffs.at(index); }

  inline bool operator==(const polynomial<T,U>& other) const {
    return coeffs == other.coeffs;
  }

  inline bool operator!=(const polynomial<T,U>& other) const {
    return !(coeffs == other.coeffs);
  }

  inline polynomial<T,U> operator+(const polynomial<T,U>& other) const {
    return polynomial<T,U>(coeffs) += other;
  }

  polynomial<T,U>& operator+=(const polynomial<T,U>& other) {
    for (const std::pair<U,T> pair : other) {
      coeffs[pair.first] += pair.second;
    }
    return trim();
  }

  inline operator bool() const {
    return coeffs.empty();
  }

  polynomial<T,U> operator*(const polynomial<T,U>& other) const {
    polynomial<T,U> result;
    
    for (const std::pair<U,T> i : coeffs) {
      for (const std::pair<U,T> j : other.coeffs) {
	result.coeffs[i.first + j.first] += i.second * j.second;
      }
    }
    
    return result.trim();
  }

  inline polynomial<T,U> operator-() const {
    Coeffs result;
    std::transform(coeffs.begin(), coeffs.end(), std::inserter(result, result.end()),
		   [](const auto pair) { return std::make_pair(pair.first, -pair.second); });
    return polynomial<T,U>(result);
  }

  polynomial<T,U> operator-(const polynomial<T,U>& other) const { 
    return *this + (-other);
  }

  polynomial<T,U>& operator-=(const polynomial<T,U>& other) {
    for (const std::pair<U,T> pair : other) {
      coeffs[pair.first] -= pair.second;
    }
    return trim();
  }
  
  inline polynomial<T,U>& operator*=(const polynomial<T,U>& other) {
    return *this = *this * other;
  }
  
  // x^n
  static inline polynomial<T,U> power(const U& n) {
    return polynomial<T,U>(Coeffs {{n, ring::unity<T>}});
  }

  polynomial<T,U> operator%(const polynomial<T,U>& mod) const {
    if (can_mod_fast(mod)) { return mod_fast(mod); }

    polynomial<T,U> acc(coeffs);
    while (acc.degree() >= mod.degree()) {
      const auto power = polynomial<T,U>::power(acc.degree() - mod.degree());
      const auto coeff = acc.coeffs.rbegin()->second / mod.coeffs.rbegin()->second;
      acc -= mod * power * coeff;
    }
    return acc;
  }

  inline polynomial<T,U>& operator%=(const polynomial<T,U>& mod) {
    if (can_mod_fast(mod)) { return mod_fast_inplace(mod); }
    return *this = *this % mod;
  }

  template <typename V>
  inline polynomial<T,U> operator*(const V& n) const {
    Coeffs result;
    std::transform(coeffs.begin(), coeffs.end(), std::inserter(result, result.end()),
		   [&](const auto pair) { 
		     return std::make_pair(pair.first, pair.second * n); 
		   });
    return polynomial<T,U>(result);
  }

  polynomial() {}
  
  template <typename... Args>
  polynomial(Args... args): coeffs(args...) {
    trim();
  }
  
  template <typename A, typename B>
  friend std::ostream& operator<<(std::ostream& os, const polynomial<A,B>& poly);

  template <typename A, typename B>
  friend std::istream& operator>>(std::istream& is, polynomial<A,B>& poly);

  template <typename Func>
  polynomial<T,U> pow_reduce(U power, Func reduce) const {
    polynomial<T,U> acc = ring::unity<polynomial<T,U>>;
    bit_container<U> power_bits(power);
    auto power_bits_it = power_bits.end();
    while (power_bits_it-- != power_bits.begin()) {
      acc = acc * acc;
      if (*power_bits_it) {
	acc *= *this;
      }
      acc = reduce(acc);
    }

    return acc.trim();
  }

  template <typename Power>
  inline polynomial<T,U> pow(Power power) const {
    return pow_reduce(power, [](auto& v){ return v; });
  }

  typedef typename Coeffs::iterator iterator;
  typedef typename Coeffs::const_iterator const_iterator;
  iterator begin() { return coeffs.begin(); }
  const_iterator begin() const { return coeffs.begin(); }
  iterator end() { return coeffs.end(); }
  const_iterator end() const { return coeffs.end(); }
  
private:
  Coeffs coeffs; // coefficients

  polynomial<T,U>& trim() {
    for (auto it = coeffs.begin(); it != coeffs.end(); ) {
      if (it->second) {
	++it; // skip
      } else {
	it = coeffs.erase(it);
      }
    }
    return *this;
  }
  
  inline bool can_mod_fast(const polynomial<T,U>& mod) const { 
    return mod == ring::unity<polynomial<T,U>> || 
      (mod.coeffs.size() == 2 && 
       mod.coeffs.rbegin()->second == ring::unity<T> && 
       mod.coeffs.begin()->second == -ring::unity<T>);
  }

  // works if modulus is X^n - 1 
  polynomial<T,U>& mod_fast_inplace(const polynomial<T,U>& mod) { 
    U n = mod.coeffs.rbegin()->first;
    auto it = coeffs.rbegin();
    while (!coeffs.empty() && it->first >= n) {
      coeffs[it->first % n] += it->second;
      coeffs.erase(it->first);
      it = coeffs.rbegin();
    }
    return *this;
  }

  inline polynomial<T,U> mod_fast(const polynomial<T,U>& mod) const {
    polynomial<T,U> result = *this;
    return result.mod_fast_inplace(mod);
  }
};

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const polynomial<T,U>& poly) {
  if (poly.coeffs.empty()) {
    os << ring::zero<polynomial<T,U>>;
  } else {
    for (auto it = poly.coeffs.begin(); it != poly.coeffs.end(); ++it) {
      if (it != poly.coeffs.begin()) {
	os << " + ";
      }
      os << it->second << "x^" << it->first;
    }
  }
  return os;
}

template <typename T, typename U>
std::istream& operator>>(std::istream& is, polynomial<T,U>& poly) {
  typename polynomial<T,U>::Coeffs coeffs;
  T coeff;
  
  while (is) {
    // skip whitespace
    while (isspace(is.peek())) { is.get(); }
    
    if (is.peek() != 'x') {
      is >> coeff;
    } else {
      coeff = 1;
    }

    int c = is.peek();
    U pow = ring::zero<U>;

    if (c == 'x') {
      is.get(); // discard 'x'
      if (is.peek() == '^') {
	is.get(); // discard '^'
	is >> pow;
      } else {
	pow = ring::unity<U>;
      }
      c = is.peek();
    }

    coeffs[pow] += coeff;

    switch (c) {
    case '+':
      is.get();
      break;
    case '-':
      break;
    default:
      goto done;
    }
  }

 done:
  poly = polynomial<T,U>(coeffs);
  return is;
}


namespace ring {
  template <typename T, typename U>
  const polynomial<T,U> unity<polynomial<T,U>>(typename polynomial<T,U>::Coeffs {{ring::zero<U>, ring::unity<T>}});
  
  template <typename T, typename U>
  const polynomial<T,U> zero<polynomial<T,U>>;
}
