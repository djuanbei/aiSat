/*
 * =====================================================================================
 *
 *       Filename:  order.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/04/2012 10:06:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "order.h"

namespace aiSat {

namespace psd {

bool indiceEqual(coef_t *a, int aSize, int *avarMap, coef_t *b, int bSize,
                 int *bvarMap) {
  int i, j;
  i = j = 1;
  /* The first index of a or b is the id location */
  while (i <= aSize && j <= bSize) {
    if (avarMap[i] < bvarMap[j]) {
      if (a[i] > 0) {
        return false;
      }
      i++;

    } else if (avarMap[i] > bvarMap[j]) {
      if (b[j] > 0) {
        return false;
      }
      j++;

    } else {
      if (a[i] > b[j]) {
        return false;
      } else if (a[i] < b[j]) {
        return false;
      }
      i++;
      j++;
    }
  }

  while (i <= aSize) {
    if (a[i++] > 0) {
      return false;
    }
  }
  while (j <= bSize) {
    if (b[j++] > 0) {
      return false;
    }
  }

  return true;
}

/**
 * @brief  Lexicographic Order
 *
 * @param a is the indices (a1,...,an)
 * @param aSize  is length of a
 * @param avarMap  is the map of a's index to variables
 * @param b   b is other indices (b1,...,bn)
 * @param bSize  bSize is the length of b
 * @param bvarMap is the map of b's index to variables
 *
 * @return  * (a1,..an) > (b1,...,bn) if the leftmost  nozero of (a1,...,an) -
 * (b1,...,bn)
 * is positive return YES.
 * if (a1,...,an) == (b1,...,bn) return YES,
 * otherwise, return false;
 */
bool lex(coef_t *a, int aSize, int *avarMap, coef_t *b, int bSize,
         int *bvarMap) {
  int i, j;
  i = j = 1;

  while (i <= aSize && j <= bSize) {
    if (avarMap[i] < bvarMap[j]) {
      if (a[i] > 0) {
        return true;
      }
      i++;

    } else if (avarMap[i] > bvarMap[j]) {
      if (b[j] > 0) {
        return false;
      }
      j++;

    } else {
      if (a[i] > b[j]) {
        return true;
      } else if (a[i] < b[j]) {
        return false;
      }
      i++;
      j++;
    }
  }

  while (i <= aSize)
    if (a[i++] > 0) {
      return true;
    }
  while (j <= bSize)
    if (b[j++] > 0) {
      return false;
    }

  return true;
}

/**
 * @brief Graded Lex Order
 *
 * @param a is the indices (a1,...,an)
 * @param aSize aSize if length of a
 * @param avarMap is the map of a's index to variables
 * @param b b is other indices (b1,...,bn)
 * @param bSize bSize is the length of b
 * @param bvarMap  is the map of b's index to variables
 *
 * @return
 *  if Sum(a1,...,an)> Sum(b1,...,bn) return YES
 *  if Sum(a1,...,an) < Sum(b1,...,bn) return NO,
 *  otherwise, return lex(a,b)
 */
bool grlex(coef_t *a, int aSize, int *avarMap, coef_t *b, int bSize,
           int *bvarMap) {
  int i, j, sumA, sumB;
  sumA = sumB = 0;
  i = j = 1;
  for (i = 1; i <= aSize; i += 1) {
    sumA += a[i];
  }

  for (j = 1; j <= bSize; j += 1) {
    sumB += b[i];
  }
  if (sumA > sumB) {
    return true;
  } else if (sumA < sumB) {
    return false;
  }

  return lex(a, aSize, avarMap, b, bSize, bvarMap);
}

/**
 * @brief   Graded Reverse Lex Order
 *
 * @param a a is the indices (a1,...,an)
 * @param aSize b is other indices (b1,...,bn)
 * @param avarMap is the map of a's index to variables
 * @param b b is other indices (b1,...,bn)
 * @param bSize is the length of b
 * @param bvarMap is the map of b's index to variables
 *
 * @return
 *  if Sum(a1,...,an)> Sum(b1,...,bn) return YES
 *  if Sum(a1,...,an) < Sum(b1,...,bn) return NO
 *  if the rightmose nonzero enry of a-b is negative return YES
 *   otherwise return NO
 */
bool grevlex(coef_t *a, int aSize, int *avarMap, coef_t *b, int bSize,
             int *bvarMap) {
  int i, j, sumA, sumB;
  sumA = sumB = 0;
  i = j = 1;
  for (i = 1; i <= aSize; i += 1) {
    sumA += a[i];
  }

  for (j = 1; j <= bSize; j += 1) {
    sumB += b[j];
  }

  if (sumA > sumB) {
    return true;
  } else if (sumA < sumB) {
    return false;
  }

  while (i > 0 && j > 0) {
    if (avarMap[i] > bvarMap[j]) {
      if (a[i] > 0) {
        return true;
      }
      i--;
    } else if (avarMap[i] < bvarMap[j]) {
      if (b[j] > 0) {
        return false;
      }
      j--;
    } else {
      if (a[i] > b[j]) {
        return true;
      } else if (a[i] < b[j]) {
        return false;
      }
      i--;
      j--;
    }
  }

  while (i > 0) {
    if (a[i--] > 0) {
      return true;
    }
  }
  while (j > 0) {
    if (b[j--] > 0) {
      return false;
    }
  }
  return true;
}
}
}
