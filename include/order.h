/*
 * =====================================================================================
 *
 *       Filename:  order.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/03/2012 08:25:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#ifndef  ORDER_INC
#define  ORDER_INC

#include	"polytype.h"
#ifdef __cplusplus
extern "C" {
#endif

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  indiceEqual
 *  Description:  
 * =====================================================================================
 */
BOOL
indiceEqual (coef_t *a, int aSize, int *avarMap, coef_t *b, int bSize, int* bvarMap  );

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  lex
 *  Description:  Lexicographic Order
 *
 *  Input:
 *  a is the indices (a1,...,an) b is other indices (b1,...,bn)
 *  aSize if length of a and bSize is the length of b
 *  avarMap is the map of a's index to variables
 *  bvarMap is the map of b's index to variables
 *
 *  Output:
 * (a1,..an) > (b1,...,bn) if the leftmost  nozero of (a1,...,an) - (b1,...,bn) 
 * is positive return YES.
 * if (a1,...,an) == (b1,...,bn) return YES
 * otherwise return false;
 * =====================================================================================
 */
BOOL
lex (coef_t *a, int aSize, int * avarMap, coef_t *b, int bSize, int *bvarMap     );


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  grlex
 *  Description:  Graded Lex Order
 *   Input:
 *  a is the indices (a1,...,an) b is other indices (b1,...,bn)
 *  aSize if length of a and bSize is the length of b
 *  avarMap is the map of a's index to variables
 *  bvarMap is the map of b's index to variables
 *  
 *  Output:
 *  if Sum(a1,...,an)> Sum(b1,...,bn) return YES
 *  if Sum(a1,...,an) < Sum(b1,...,bn) return NO
 *  otherwise return lex(a,b)
 * =====================================================================================
 */
BOOL
grlex ( coef_t *a, int aSize, int * avarMap, coef_t *b, int bSize, int *bvarMap   );


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  grevlex
 *  Description:  Graded Reverse Lex Order
 *
 *  Input:
 *  a is the indices (a1,...,an) b is other indices (b1,...,bn)
 *  aSize if length of a and bSize is the length of b
 *  avarMap is the map of a's index to variables
 *  bvarMap is the map of b's index to variables
 *
 * Output:
 *
 *  if Sum(a1,...,an)> Sum(b1,...,bn) return YES
 *  if Sum(a1,...,an) < Sum(b1,...,bn) return NO
 *  
 *  if the rightmose nonzero enry of a-b is negative return YES
 *   otherwise return NO 
 * =====================================================================================
 */
BOOL
grevlex ( coef_t *a, int aSize, int * avarMap, coef_t *b, int bSize, int *bvarMap  ) ;


#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef ORDER_INC  ----- */
