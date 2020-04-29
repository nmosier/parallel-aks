#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include "polynomial.hpp"

int main(int argc, char *argv[]) {
  polynomial<mpz_class> poly1;
  polynomial<mpz_class> poly2;
  
  std::cin >> poly1;
  std::cout << poly1 << std::endl;
  
  return 0;
}
