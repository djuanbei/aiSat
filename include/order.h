
/**
 * @file   order.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 11:44:30 2015
 *
 * @brief
 *
 *
 */

#ifndef ORDER_INC
#define ORDER_INC

#include "polytype.h"
#ifdef __cplusplus
extern "C" {
#endif

BOOL indiceEqual(coef_t *a, int aSize, int *avarMap, coef_t *b, int bSize,
                 int *bvarMap);

/**
 *
 *
 * @param a is the indices (a1,...,an)
 * @param aSize is length of  array a
 * @param avarMap is the map of a's index to variables
 * @param b indices (b1,...,bn)
 * @param bSize is the length of array b
 * @param bvarMap is the map of b's index to variables
 *
 * @return   * (a1,..an) > (b1,...,bn) if the leftmost  nozero of (a1,...,an) -
 * (b1,...,bn)
 * is positive return YES.
 * if (a1,...,an) == (b1,...,bn) return YES
 * otherwise return false;
 */
BOOL lex(coef_t *a, int aSize, int *avarMap, coef_t *b, int bSize,
         int *bvarMap);

/**
 *
 *
 * @param a is the indices (a1,...,an)
 * @param aSize is the length of array a
 * @param avarMap is the map of a's index to variables
 * @param b  is other indices (b1,...,bn)
 * @param bSize is the length of array b
 * @param bvarMap is the map of b's index to variables
 *
 * @return  if Sum(a1,...,an)> Sum(b1,...,bn) return YES
 *  if Sum(a1,...,an) < Sum(b1,...,bn) return NO
 *  otherwise return lex(a,b)
 */
BOOL grlex(coef_t *a, int aSize, int *avarMap, coef_t *b, int bSize,
           int *bvarMap);

/**
 *
 *
 * @param a  is the indices (a1,...,an)
 * @param aSize is length of array  a
 * @param avarMap is the map of a's index to variables
 * @param b is other indices (b1,...,bn)
 * @param bSize is the length of array b
 * @param bvarMap  is the map of b's index to variables
 *
 * @return  if Sum(a1,...,an)> Sum(b1,...,bn) return YES
 *  if Sum(a1,...,an) < Sum(b1,...,bn) return NO
 *
 *  if the rightmose nonzero enry of a-b is negative return YES
 *   otherwise return NO
 */
BOOL grevlex(coef_t *a, int aSize, int *avarMap, coef_t *b, int bSize,
             int *bvarMap);

#ifdef __cplusplus
}
#endif

#endif 
