/*
  Solve a system of equations using the Cholesky factorization of A.
  Note that we assume that A is positive definite and that A has already
  been factored.
*/

#include <stdio.h>
#include <stdlib.h>
#include "declarations.h"

int solvesys(m, ldam, A, rhs) int m;
int ldam;
double *A;
double *rhs;
{
  int incx;
  int info;

  incx = 1;
  MY_DPOTRS("U", &m, &incx, A, &ldam, rhs + 1, &ldam, &info);

  if (info != 0) {
    return (6);
  };

  return (0);
}
