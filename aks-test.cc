#include <iostream>
#include <gmpxx.h>
#include <gmp.h>

#include "aks-util.hpp"

int main(int argc, char *argv[]) {
  mpz_class num, mod; 
  uintmax_t thresh;

  if (argc == 1) { 
    std::cin >> num >> mod >> thresh;
  } else if (argc == 2) {
    num = mpz_class(argv[1]);
    mod = mpz_class(argv[2]);
    thresh = atoi(argv[3]);
  }

  std::cout << (mpz_ord_gt(num, mod, thresh) ? "true" : "false") << std::endl;

  return 0;
}
