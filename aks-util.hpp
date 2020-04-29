#include <gmpxx.h>
#include <gmp.h>

#pragma once

bool mpz_ord_gt(const mpz_class val, const mpz_class mod, const uintmax_t thresh);

// ceiling log_2 of integer
inline size_t mpz_log2_ceil(const mpz_class val) {
  return mpz_sizeinbase(val.get_mpz_t(), 2);
}

mpz_class mpz_totient(const mpz_class n);
