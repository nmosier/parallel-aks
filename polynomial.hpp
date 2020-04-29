#pragma once

#include <ostream>
#include <vector>
#include <cassert>
#include <algorithm>

template <typename T>
class polynomial {
public:
  
  int degree() const { 
    return std::max<size_t>(coeffs.size(), 1) - 1;
  }

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

  polynomial<T> operator*(const polynomial& other) const {
    std::vector<T> result(degree() + other.degree() + 1);
    
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

  polynomial<T>& operator-=(const polynomial<T>& other) {
    const auto degmin = std::min(degree(), other.degree());
    if (degree() < other.degree()) {
      coeffs.resize(other.degree());
      std::copy(other.coeffs.begin() + degmin, other.coeffs.end(), coeffs.begin() + degmin);
    }
    for (int i = 0; i < degmin; ++i) {
      coeffs[i] -= other.coeffs[i];
    }
    
    while (!coeffs.empty() && !coeffs.back()) {
      coeffs.pop_back();
    }

    return *this;
  }

  polynomial<T> operator%(const polynomial<T>& mod) const {
    if (mod.degree() == 0) {
      return polynomial<T>(std::vector<T> {0});
    }

    assert(mod.degree() > 0); // don't divide by zero

    // const auto degdiff = std::max(0, degree() - mod.degree());
    // std::vector<T> result(degdiff + 1);
    
    polynomial<T> acc(coeffs);
    while (acc.degree() >= mod.degree()) {
      // const auto power = acc.degree() - mod.degree();
      const auto coeff = acc.coeffs.back() / mod.coeffs.back();
      // result[power] = coeff;
      acc -= mod * coeff;
    }
    
    // return polynomial<T>(result);
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
