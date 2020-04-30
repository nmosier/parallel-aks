#include <gmpxx.h>
#include <gmp.h>

#include "polynomial.hpp"

#pragma once

bool mpz_ord_gt(const mpz_class val, const mpz_class mod, const uintmax_t thresh);

// ceiling log_2 of integer
inline size_t mpz_log2_ceil(const mpz_class val) {
  return mpz_sizeinbase(val.get_mpz_t(), 2);
}

mpz_class mpz_totient(const mpz_class n);

polynomial<mpz_class> pow_polyring(const polynomial<mpz_class>& poly, mpz_class pow, const polynomial<mpz_class>& polymod, mpz_class zmod);
