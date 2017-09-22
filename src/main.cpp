#include "sosproblemparser.h"
#include "poly.hpp"


int main(int argc, char *argv[]) {

  
  if (argc > 1) {
    parseProblem(argv[1]);
  } else {
    parseProblem();
  }
  aiSat::poly<double, indice_t> p1,p2;
  aiSat::poly<double, indice_t> p3=p1*p2;
  return 0;
}
