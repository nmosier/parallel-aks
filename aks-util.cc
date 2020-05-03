#include <limits>
#include <unordered_map>
#include <gmpxx.h>
#include <gmp.h>
#include "polynomial.hpp"
#include "bit-iterator.hpp"
#include "aks-bit.hpp"

// check whether order is greater than _thresh_
bool mpz_ord_gt(const mpz_class val, const mpz_class mod, const uintmax_t thresh) {
  mpz_class acc(val);
  uintmax_t i = 1;
  while (acc != 1 && i <= thresh) {
    ++i;
    acc = (acc * val) % mod;
  }
  
  return i > thresh;
}

// Euler totient function 
mpz_class mpz_totient(const mpz_class n) {
  mpz_class gcd;
  mpz_class phi = 0;
  for (mpz_class i = 1; i <= n; ++i) {
    mpz_gcd(gcd.get_mpz_t(), i.get_mpz_t(), n.get_mpz_t());
    if (gcd == 1) {
      ++phi;
    }
  }
  return phi;
}



template <typename T>
uintmax_t totient(const T& n) {
  // factorize n
  std::unordered_map<T, int> factors;
  
  
}


// polynomial ring exponentiation
polynomial<mpz_class,mpz_class> pow_polyring(const polynomial<mpz_class,mpz_class>& poly, 
					     const mpz_class& pow,
					     const polynomial<mpz_class,mpz_class>& polymod, 
					     const mpz_class& zmod) {
  return poly.pow_reduce(pow, 
			 [=](const polynomial<mpz_class,mpz_class>& acc) 
			 -> polynomial<mpz_class,mpz_class> {
			   polynomial<mpz_class,mpz_class> newacc = acc % polymod;
			   for (auto& coeff : newacc) {
			     if (coeff.second < 0) {
			       // make positive
			       coeff.second += ((-coeff.second + zmod - 1) / zmod) * zmod;
			     }
			     coeff.second %= zmod;
			   }
			   return newacc;
			 });
}

