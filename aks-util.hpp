#include <map>
#include <gmpxx.h>
#include <gmp.h>

#include "polynomial.hpp"
#include "aks-bit.hpp"

#pragma once

bool mpz_ord_gt(const mpz_class val, const mpz_class mod, const uintmax_t thresh);

// ceiling log_2 of integer
inline size_t mpz_log2_ceil(const mpz_class val) {
  return mpz_sizeinbase(val.get_mpz_t(), 2);
}

mpz_class mpz_totient(const mpz_class n);

//template <typename T>
//mpz_class operator<<(T val, const mpz_class& shift);

polynomial<mpz_class,mpz_class> pow_polyring(const polynomial<mpz_class,mpz_class>& poly, 
					     const mpz_class& pow,
					     const polynomial<mpz_class,mpz_class>& polymod, 
					     const mpz_class& zmod);

template <typename T>
std::map<T,int> factorize(T n) {
  std::map<T,int> factors;

  T i = 2;

  while (i <= n) {
    if (n % i == ring::zero<T>) {
      ++factors[i];
      n /= i;
    } else {
      ++i;
    }
  }

  return factors;
}

template <typename T>
T totient(const T& n) {
  const auto factors = factorize(n);
  T acc = ring::unity<T>;
  
  for (const auto factor : factors) {
    for (int i = 0; i < factor.second - 1; ++i) {
      acc *= factor.first;
    }
    acc *= factor.first - 1;
  }
  return acc;
}
