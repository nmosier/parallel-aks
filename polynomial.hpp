#pragma once

#include <ostream>
#include <istream>
#include <vector>
#include <cassert>
#include <algorithm>

#include "bit-iterator.hpp"

template <typename T>
class polynomial {
public:
  
  int degree() const { 
    return std::max<size_t>(coeffs.size(), 1) - 1;
  }

  template <typename U>
  inline const T& operator[](U index) const { return coeffs[index]; }

  polynomial<T> operator+(const polynomial& other) const {
    const auto maxdeg = std::max(degree(), other.degree());
    const auto mindeg = std::min(degree(), other.degree());
    std::vector<T> result(maxdeg + 1);
    
    for (int i = 0; i <= mindeg; ++i) {
      result[i] = coeffs[i] + other.coeffs[i];
    }

    auto it = (degree() == maxdeg ? coeffs.begin() : other.coeffs.begin());
    for (int i = mindeg + 1; i <= maxdeg; ++i) {
      result[i] = *it++;
    }

    T zero;
    while (!result.empty() && result.back() == zero) {
      result.pop_back();
    }

    return polynomial<T>(result);
  }

  inline operator bool() const {
    return coeffs.size();
  }

  polynomial<T> operator*(const polynomial& other) const {
    std::vector<T> result(degree() + other.degree() + 1);
    
    if (!*this || !other) {
      return polynomial<T>();
    }
    
    for (int i = 0; i <= degree(); ++i) {
      for (int j = 0; j <= other.degree(); ++j) {
	result[i + j] += coeffs[i] * other.coeffs[j];
      }
    }

    /* trim */
    T zero;
    while (!result.empty() && result.back() == zero) {
      result.pop_back();
    }

    return polynomial<T>(result);
  }

  polynomial<T> operator-() const {
    std::vector<T> result(coeffs.size());
    std::transform(coeffs.begin(), coeffs.end(), result.begin(),
		   [](const T& val) { return -val; });
    return polynomial<T>(result);
  }

  polynomial<T> operator-(const polynomial<T>& other) const { 
    return *this + (-other);
  }

  polynomial<T>& operator+=(const polynomial<T>& other) {
    const auto degmin = std::min(degree(), other.degree());
    if (degree() < other.degree()) {
      coeffs.resize(other.degree() + 1);
      std::copy(other.coeffs.begin() + degmin + 1, other.coeffs.end(), 
		coeffs.begin() + degmin + 1);
    }
    for (int i = 0; i <= degmin; ++i) {
      coeffs[i] += other.coeffs[i];
    }
    
    while (!coeffs.empty() && !coeffs.back()) {
      coeffs.pop_back();
    }
    
    return *this;
  }

  polynomial<T>& operator-=(const polynomial<T>& other) {
    const auto degmin = std::min(degree(), other.degree());
    if (degree() < other.degree()) {
      coeffs.resize(other.degree() + 1);
      std::transform(other.coeffs.begin() + degmin + 1, other.coeffs.end(), 
		     coeffs.begin() + degmin + 1,
		     [](const T& coeff) {
		       return -coeff;
		     });
    }
    for (int i = 0; i <= degmin; ++i) {
      coeffs[i] -= other.coeffs[i];
    }
    
    while (!coeffs.empty() && !coeffs.back()) {
      coeffs.pop_back();
    }

    return *this;
  }

  inline polynomial<T>& operator*=(const polynomial<T>& other) {
    return *this = *this * other;
  }

  template <typename U>
  static polynomial<T> power(U deg) {
    // polynomial<T> poly;
    // poly.coeffs.resize(p);
    std::vector<T> vec(deg + 1);
    vec[deg] = 1;
    return polynomial<T>(vec);
  }

  polynomial<T> operator%(const polynomial<T>& mod) const {

    if (mod.degree() == 0) {
      return polynomial<T>(std::vector<T> {0});
    }

    polynomial<T> acc(coeffs);
    while (acc.degree() >= mod.degree()) {
      const auto power = polynomial<T>::power(acc.degree() - mod.degree());
      const auto coeff = acc.coeffs.back() / mod.coeffs.back();
      acc -= mod * power * coeff;
    }
    
    return acc;
  }

  template <typename U>
  polynomial<T> operator*(U n) const {
    std::vector<T> result(coeffs.size());
    std::transform(coeffs.begin(), coeffs.end(), result.begin(), [=](const auto coeff) {
	return coeff * n;
      });
    return polynomial<T>(result);
  }

  polynomial() {}
  
  template <typename... Args>
  polynomial(Args... args): coeffs(args...) {
    while (!coeffs.empty() && !coeffs.back()) { coeffs.pop_back(); }
  }

  
  template <typename U>
  friend std::ostream& operator<<(std::ostream& os, const polynomial<U>& poly);

  template <typename U>
  friend std::istream& operator>>(std::istream& is, polynomial<U>& poly);

  template <typename Power, typename Func>
  polynomial<T> pow_reduce(Power power, Func reduce) const {
    polynomial<T> acc(std::vector<T>(1, 1));
    bit_container<Power> power_bits(power);
    auto power_bits_it = power_bits.end();
    while (power_bits_it-- != power_bits.begin()) {
      acc = acc * acc;
      if (*power_bits_it) {
	acc *= *this;
      }
      acc = reduce(acc);
    }

    return acc;
  }

  template <typename Power>
  inline polynomial<T> pow(Power power) const {
    return pow_reduce(power, [](auto& v){ return v; });
  }

  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  iterator begin() { return coeffs.begin(); }
  const_iterator begin() const { return coeffs.begin(); }
  iterator end() { return coeffs.end(); }
  const_iterator end() const { return coeffs.end(); }
  
private:
  std::vector<T> coeffs; // coefficients
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const polynomial<T>& poly) {
  if (poly.coeffs.empty()) {
    T zero;
    os << zero;
  } else {
    for (size_t i = 0; i < poly.coeffs.size(); ++i) {
      if (poly.coeffs[i]) {
	os << poly.coeffs[i] << "x^" << i;
	if (i != poly.coeffs.size() - 1) {
	  os << " + ";
	}
      }
    }
  }
  
  return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, polynomial<T>& poly) {
  std::vector<T> vec;
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
    size_t pow = 0;

    if (c == 'x') {
      is.get(); // discard 'x'
      if (is.peek() == '^') {
	is.get(); // discard '^'
	is >> pow;
      } else {
	pow = 1;
      }
      c = is.peek();
    }

    if (vec.size() < pow + 1) {
      vec.resize(pow + 1);
    }

    vec[pow] += coeff;

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
  poly = polynomial<T>(vec);
  return is;
}
