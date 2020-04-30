#pragma once

#include <gmpxx.h>
#include <gmp.h>

#include "bit-iterator.hpp"

template <>
class bit_container<mpz_class> {
public:
  typedef bit_const_iterator<mpz_class> const_iterator;
  
  inline const_iterator begin() const {
    return const_iterator(0, *this);
  }

  inline const_iterator end() const { 
    return const_iterator(mpz_sizeinbase(val.get_mpz_t(), 2), *this);
  }

  template <typename U>
  inline bool operator[](U index) const {
    return mpz_tstbit(val.get_mpz_t(), index);
  }

  bit_container(const mpz_class& val): val(val) {}

private:
  mpz_class val;
};
