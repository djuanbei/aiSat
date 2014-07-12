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

#include	"convexhull.h"

#include	<stdio.h>
#include	<stdlib.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( int argc, char *argv[] ){
  
  int length=10;
  const int dim=7;
  const int domain=5;
  indice_t *points=randPointSet(length,dim, domain);

  int relength;
  indice_t *cand=overConvexHull(points, length, dim, &relength);
  printf ( "%d\n %s",relength,cand );
  printf(" argc %d file %s\n",argc,argv[0]);

  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
