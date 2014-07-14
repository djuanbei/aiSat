/*
 * =====================================================================================
 *
 *       Filename:  subpoly.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/25/2013 06:59:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  SUBPOLY_INC
#define  SUBPOLY_INC
#include	"polytype.h"

#ifdef __cplusplus
extern "C" {
#endif

  SubPoly *createSubPoly(Poly * poly);
  SubPoly *createSubPoly1(Poly * poly, const int size, int *loc);

  void printSubPoly ( const SubPoly * subpoly  );
  void deleteSubPoly(SubPoly * poly);


#ifdef __cplusplus 
}
#endif

#endif   /* ----- #ifndef SUBPOLY_INC  ----- */
