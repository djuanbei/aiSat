#include "sosproblemparser.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    parseProblem(argv[1]);
  } else {
    parseProblem();
  }

  return 0;
}
