#include <iostream>
#include "bit-iterator.hpp"
#include "aks-bit.hpp"

int main(void) {
  bit_container<mpz_class> container(0xa);

  for (auto it = container.end(); it-- != container.begin(); ) {
    std::cout << *it << std::endl;
  }

  return 0;
}
