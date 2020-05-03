#include <iostream>
#include <gmpxx.h>
#include <gmp.h>
#include <mpf2mpfr.h>
#include <mpfr.h>
#include <unistd.h>

#include "aks-util.hpp"
#include "aks-bit.hpp"
#include "polynomial.hpp"
#include "util.hpp"

inline void prime() { 
  std::cout << "prime" << std::endl; 
}

inline void composite() { 
  std::cout << "composite" << std::endl; 
}

inline void indeterminate() {
  std::cout << "indeterminate" << std::endl;
}



int main(int argc, char *argv[]) {
  bool verbose = false;

  timer::global_enable = false;

  const char *usage = "usage: %s [-tvh] [n]\n";
  const char *optstring = "tvh";
  int optchar;
  while ((optchar = getopt(argc, argv, optstring)) >= 0) {
    switch (optchar) {
    case 't':
      timer::global_enable = true;
      break;
    case 'v':
      verbose = true;
      break;
    case 'h':
      fprintf(stderr, usage, argv[0]);
      return 0;
    default:
      fprintf(stderr, usage, argv[0]);
      return 1;
    }
  }

  mpz_class num;

  if (argc - optind == 0) { 
    std::cin >> num;
  } else if (argc - optind == 1) {
    num = mpz_class(argv[optind]);
#if 0
    if (gmp_sscanf(argv[optind], "%Zi", num) != 1) {
      fprintf(stderr, "gmp_sscanf: invalid integer '%s'\n", argv[optind]);
      return 1;
    }
#endif
  } else {
    fprintf(stderr, usage, argv[0]);
    return 1;
  }

  if (num <= 1) {
    fprintf(stderr, "%s: requires positive integer greater than 1\n", argv[0]);
    return 2;
  }

  /* BEGIN MAIN ALGORITHM */
  if (verbose) std::clog << "stage 1" << std::endl;
  {
    timer t("stage 1");
    if (mpz_perfect_power_p(num.get_mpz_t())) {
      if (verbose) std::clog << "perfect power" << std::endl;
      composite();
      return 0;
    }
  }

  /* Find the smallest positive integer r such that o_r(n) > log^2(n) */
  uintmax_t log2_n = mpz_log2_ceil(num);
  uintmax_t log2_n_sq = log2_n * log2_n;
  mpz_class r = 2;
  while (!mpz_ord_gt(r, num, log2_n_sq)) {
    ++r;
  }
  if (verbose) std::clog << "r = " << r << std::endl;

  /* If 1 < gcd(a,n) < n for some a <= r, then COMPOSITE. */
  if (verbose) std::clog << "stage 2" << std::endl;
  {
    timer t("stage 2");
  mpz_class gcd;
  for (mpz_class a = 2; a < r; ++a) {
    mpz_gcd(gcd.get_mpz_t(), a.get_mpz_t(), num.get_mpz_t());
    if (1 < gcd && gcd < num) {
      if (verbose) std::clog << "a = " << a << std::endl;
      composite();
      return 0;
    }
  }
  }

  /* If n <= r, return prime. */
  if (verbose) std::clog << "stage 3" << std::endl;
  {
    timer t("stage 3");
  if (num <= r) {
    if (verbose) std::clog << "n <= r" << std::endl;
    prime();
    return 0;
  }
  }


  mpfr_t log2_n_f;
  mpfr_t num_f;
  mpz_class phi_n;
  mpfr_t phi_n_f, sqrt_phi_n, l_f;  
  mpz_class l;
  {
    timer t("compute l");
    {
      timer t("compute l (prelim)");
      mpfr_init_set_z(num_f, num.get_mpz_t(), MPFR_RNDN);
      mpfr_init2(log2_n_f, mpfr_get_prec(num_f));
      mpfr_log2(log2_n_f, num_f, MPFR_RNDN);
    }

  /* Let l = floor(sqrt(phi(r)) log(n)) */
    {
      timer t("compute l (totient)");
      phi_n = mpz_totient(r);
    }
    
  mpfr_init_set_z(phi_n_f, phi_n.get_mpz_t(), MPFR_RNDN);
  mpfr_init2(sqrt_phi_n, mpfr_get_prec(phi_n_f));
  
  {
    timer t("compute l (sqrt)");
    mpfr_sqrt(sqrt_phi_n, phi_n_f, MPFR_RNDN);
  }
  mpfr_init2(l_f, mpfr_get_prec(sqrt_phi_n));
  {
    timer t("compute l (mul)");
  mpfr_mul(l_f, sqrt_phi_n, log2_n_f, MPFR_RNDN);
  }
  mpfr_get_z(l.get_mpz_t(), l_f, MPFR_RNDD);
  }

  if (verbose) std::clog << "l = " << l << std::endl;

  if (verbose) std::clog << "stage 4" << std::endl;
  {
    timer t("stage 4");
  for (mpz_class a = 0; a <= l; ++a) {
    // compute (X+a)^n and X^n+a (mod X^r - 1, n)
    polynomial<mpz_class,mpz_class> lhs;
    lhs[ring::unity<mpz_class>] = ring::unity<mpz_class>;
    lhs[ring::zero<mpz_class>] = a;

    polynomial<mpz_class,mpz_class> rhs;
    rhs[num] = ring::unity<mpz_class>;
    rhs[ring::zero<mpz_class>] = a;
    
    polynomial<mpz_class,mpz_class> mod;
    mod[r] = ring::unity<mpz_class>;
    mod[ring::zero<mpz_class>] = -ring::unity<mpz_class>;
    
    const polynomial<mpz_class,mpz_class> lhs_reduced = pow_polyring(lhs, num, mod, num);
    polynomial<mpz_class,mpz_class> rhs_reduced = rhs % mod;
    for (auto& coeff : rhs_reduced) { 
      coeff.second %= num;
    }
    
    // compare LHS and RHS
    if (lhs_reduced != rhs_reduced) {
      if (verbose) std::clog << "lhs = " << lhs_reduced << ", rhs = " << rhs_reduced << std::endl;
      composite();
      return 0;
    }
  }
  }

  prime();
  return 0;
}

