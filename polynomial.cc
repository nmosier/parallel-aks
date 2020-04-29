#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include "polynomial.hpp"

int main(int argc, char *argv[]) {
  std::vector<int> coeffs {1, 2, 3};
  polynomial<mpz_class> test(coeffs.begin(), coeffs.end());
  std::vector<int> coeffs_ {-1,-2,-3};
  polynomial<mpz_class> test2(coeffs_.begin(), coeffs_.end());
  
  std::cout << "+: " << test + test2 << std::endl;
  std::cout << "*: " << test * test2 << std::endl;
  std::cout << "%: " << test % test2 << std::endl;
  
  return 0;
}
