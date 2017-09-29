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
#include "polyparser.h"

using namespace aiSat::poly;

typedef Poly<coef_t, indice_t> Poly_t;

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

  Poly_t p=parse("x+2");
  cout<<p.toString()<<endl;
  Poly_t p1=parse("y+1");

  VarTable<indice_t>& table= getVarTable<indice_t>();
  // table.clear();
  cout<<table.getVarNum(p.getVarId())<<endl;
  cout<<table.getVarNum(p1.getVarId())<<endl;
  // Poly<coef_t, indice_t> p=parse("x+1");
  // string str=p.toString();
  // cout<<str<<endl;

  // Poly<double, indice_t> p1, p2;
  // Poly<double, indice_t> p3 = p1 * p2;
  return EXIT_SUCCESS;
}
