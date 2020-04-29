#include <limits>
#include <gmpxx.h>
#include <gmp.h>

#if 0
inline mp_bitcnt_t mpz_bits(const mpz_class op) { return op.get_mpz_t(); }
mp_bitcnt_t mpz_bits(const mpz_t op) {
  /* binary expansion */
  //  mp_bitcnt_t last = 0;
  mp_bitcnt_t upper = 0;
  while (mpz_scan1(op, upper) < std::numeric_limits<mp_bitcnt_t>::max()) {
    current = current * 2 + 1;
  }
  mp_bitcnt_t lower = (upper - 1) / 2;
  
  /* binary search */
  while (upper > lower + 1) {
    mp_bitcnt_t middle = (upper + lower) / 2;
    if (mpz_scan1(op, middle) < std::numeric_limits<mp_bitcnt_t>::max()) {
      lower = middle;
    } else {
      upper = middle;
    }
  }
  


  return upper;
}
#endif

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
