#include <iostream>
#include <gmpxx.h>
#include <gmp.h>

int main(int argc, char *argv[]) {
  mpz_class num;

  if (argc == 1) { 
    std::cin >> num;
  } else if (argc == 2) {
    num = mpz_class(argv[1]);

    if (gmp_sscanf(argv[1], "%Zi", num) != 1) {
      fprintf(stderr, "gmp_sscanf: invalid integer '%s'\n", argv[1]);
      return 1;
    }
  }

  gmp_printf("%Zd\n", num);

  std::cerr << "main: stub" << std::endl;
  return 1;
}
