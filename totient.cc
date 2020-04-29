#include <iostream>
#include "aks-util.hpp"

int main(int argc, char *argv[]) {
  mpz_class n;

  if (argc == 1) {
    std::cin >> n;
  } else {
    n = mpz_class(argv[1]);
  }

  std::cout << mpz_totient(n) << std::endl;

  return 0;
}
