#include "poly.hpp"
#include "sosproblemparser.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    parseProblem(argv[1]);
  } else {
    parseProblem();
  }
  aiSat::poly::Poly<double, indice_t> p1, p2;
  aiSat::poly::Poly<double, indice_t> p3 = p1 * p2;
  return 0;
}
