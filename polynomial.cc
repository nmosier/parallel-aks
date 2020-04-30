#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include "polynomial.hpp"

int main(int argc, char *argv[]) {
  polynomial<mpz_class> poly1;
  polynomial<mpz_class> poly2;
  
  std::cin >> poly1 >> poly2;

  while (std::cin) {
    std::string op;
    std::cin >> op;

    if (op == "plus") {
      std::cout << poly1 + poly2 << std::endl;
    } else if (op == "minus") {
      std::cout << poly1 - poly2 << std::endl;
    } else if (op == "times") {
      std::cout << poly1 * poly2 << std::endl;
    } else if (op == "mod") {
      std::cout << poly1 % poly2 << std::endl;
    }
  }
  
  return 0;
}
