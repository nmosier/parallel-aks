#include <iostream>
#include <gmpxx.h>
#include <gmp.h>
#include <mpf2mpfr.h>
#include <mpfr.h>

#include "aks-util.hpp"

inline void prime() { 
  std::cout << "prime" << std::endl; 
  exit(0);
}

inline void composite() { 
  std::cout << "composite" << std::endl; 
  exit(0);
}

inline void indeterminate() {
  std::cout << "indeterminate" << std::endl;
  exit(0);
}



int main(int argc, char *argv[]) {
  mpz_class num;

  if (argc == 1) { 
    std::cin >> num;
  } else if (argc == 2) {
    num = mpz_class(argv[1]);

    if (gmp_sscanf(argv[1], "%Zi", num) != 1) {
      fprintf(stderr, "gmp_sscanf: invalid integer '%s'\n", argv[1]);
      return 1;
    }
  }

  if (num <= 1) {
    fprintf(stderr, "%s: requires positive integer greater than 1\n", argv[0]);
    return 2;
  }

  /* BEGIN MAIN ALGORITHM */
  if (mpz_perfect_power_p(num.get_mpz_t())) {
    std::clog << "perfect power" << std::endl;
    composite();
  }

  /* Find the smallest positive integer r such that o_r(n) > log^2(n) */
  uintmax_t log2_n = mpz_log2_ceil(num);
  uintmax_t log2_n_sq = log2_n * log2_n;
  mpz_class r = 2;
  while (!mpz_ord_gt(r, num, log2_n_sq)) {
    ++r;
  }
  std::clog << "r = " << r << std::endl;

  /* If 1 < gcd(a,n) < n for some a <= r, then COMPOSITE. */
  mpz_class gcd;
  for (mpz_class a = 2; a < r; ++a) {
    mpz_gcd(gcd.get_mpz_t(), a.get_mpz_t(), num.get_mpz_t());
    if (1 < gcd && gcd < num) {
      std::clog << "a = " << a << std::endl;
      composite();
    }
  }

  /* If n <= r, return prime. */
  if (num <= r) {
    std::clog << "n <= r" << std::endl;
    prime();
  }

  mpfr_t log2_n_f;
  mpfr_t num_f;
  mpfr_init_set_z(num_f, num.get_mpz_t(), MPFR_RNDN);
  mpfr_init2(log2_n_f, mpfr_get_prec(num_f));
  mpfr_log2(log2_n_f, num_f, MPFR_RNDN);

  /* Let l = floor(sqrt(phi(r)) log(n)) */
  mpz_class phi_n = mpz_totient(num);
  mpfr_t phi_n_f, sqrt_phi_n, l_f;
  mpfr_init_set_z(phi_n_f, phi_n.get_mpz_t(), MPFR_RNDN);
  mpfr_init2(sqrt_phi_n, mpfr_get_prec(phi_n_f));
  mpfr_sqrt(sqrt_phi_n, phi_n_f, MPFR_RNDN);
  mpfr_init2(l_f, mpfr_get_prec(sqrt_phi_n));
  mpfr_mul(l_f, sqrt_phi_n, log2_n_f, MPFR_RNDN);
  mpz_class l;
  mpfr_get_z(l.get_mpz_t(), l_f, MPFR_RNDD);

  std::clog << "l = " << l << std::endl;

  indeterminate();

  return 0;
}

