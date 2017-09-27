/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  11/29/2013 12:53:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

// #include "convexhull.h"

#include <stdio.h>
#include <stdlib.h>
#include "poly.hpp"

using namespace aiSat::poly;

int main(int argc, char *argv[]) {
  
  // VarTable<indice_t> Poly<double,indice_t>::VAR_TABLE;
  
  // int length = 10;
  // const int dim = 7;
  // const int domain = 5;
  // indice_t *points = randPointSet(length, dim, domain);

  // int relength;
  // indice_t *cand = overConvexHull(points, length, dim, &relength);
  // printf("%d\n %s", relength, cand);
  // printf(" argc %d file %s\n", argc, argv[0]);
  Poly<double, indice_t> p1, p2;
  Poly<double, indice_t> p3 = p1 * p2;
  return EXIT_SUCCESS;
}
