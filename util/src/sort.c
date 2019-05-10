
/**
 * @file   sort.c
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 15:12:57 2015
 *
 * @brief
 *
 *
 */

#include "sort.h"
/**
 * swap the content of loc i and j in array Z
 *
 * @param Z an array
 * @param i
 * @param j
 * @param n size of one element in Z
 */
void swap( indice_t *Z, const int i, const int j, const int n ) {
  int      k;
  indice_t temp;
  int      iindex = i * n;
  int      jindex = j * n;

  for ( k = 0; k < n; k += 1 ) {
    temp            = Z[ iindex + k ];
    Z[ iindex + k ] = Z[ jindex + k ];
    Z[ jindex + k ] = temp;
  }
}

/**
 * sort tht elements in [ left, right ] array Z by ascending order
 *
 * @param Z an array
 * @param n the size of one element in Z
 * @param left
 * @param right
 * @param comp comparat funtion
 */
void qsortKElem( indice_t *Z, const int n, const int left, const int right,
                 int ( *comp )( const indice_t *, const indice_t *,
                                const int ) ) {
  int i, last;

  /*-----------------------------------------------------------------------------
   * do nothing fewer than two elements
   *-----------------------------------------------------------------------------*/
  if ( left >= right ) {
    return;
  }

  swap( Z, left, ( left + right ) / 2, n );

  last = left;
  for ( i = left + 1; i <= right; i += 1 ) {
    if ( comp( Z + i * n, Z + left * n, n ) < 0 ) {
      swap( Z, ++last, i, n );
    }
  }

  swap( Z, left, last, n );
  qsortKElem( Z, n, left, last - 1, comp );
  qsortKElem( Z, n, last + 1, right, comp );
}

/**
 *
 * sort an array Z[ left,right ] sort by ascending order. Be care that the
 * element between C and Z is one by one. Thus, after sort the location relation
 * between   C and Z shold keep.
 * @param C
 * @param Z an array sort by ascending order.
 * @param n the size of one element in Z
 * @param left
 * @param right
 * @param comp the comparable function
 */
void qsortT( coef_t *C, indice_t *Z, const int n, const int left,
             const int right,
             int ( *comp )( const indice_t *, const indice_t *, const int ) ) {
  int    i, last;
  coef_t tempC;

  /*-----------------------------------------------------------------------------
   * do nothing fewer than two elements
   *-----------------------------------------------------------------------------*/
  if ( left >= right ) {
    return;
  }

  swap( Z, left, ( left + right ) / 2, n );
  tempC                     = C[ ( left + right ) / 2 ];
  C[ ( left + right ) / 2 ] = C[ left ];
  C[ left ]                 = tempC;

  last = left;

  for ( i = left + 1; i <= right; i += 1 ) {
    if ( ( comp )( Z + i * n, Z + left * n, n ) < 0 ) {
      swap( Z, ++last, i, n );
      tempC     = C[ last ];
      C[ last ] = C[ i ];
      C[ i ]    = tempC;
    }
  }

  swap( Z, left, last, n );
  tempC     = C[ left ];
  C[ left ] = C[ last ];
  C[ last ] = tempC;

  qsortT( C, Z, n, left, last - 1, comp );
  qsortT( C, Z, n, last + 1, right, comp );
}

/**
 * sort arrary vars by ascending order
 *
 * @param vars
 * @param n
 */
void sortVar( indice_t *const vars, const int n ) {
  int      i, j;
  indice_t temp;

  for ( i = 0; i < n - 1; i += 1 ) {
    for ( j = i + 1; j < n; j += 1 ) {
      if ( vars[ j ] < vars[ i ] ) {
        temp      = vars[ j ];
        vars[ j ] = vars[ i ];
        vars[ i ] = temp;
      }
    }
  }
}

/**
 *
 *
 * @param key1 an exponent of one monomial
 * @param varMap1 the variables corresponding key1
 * @param key2 an exponent of one monomial
 * @param varMap2 the variables corresponding key1
 *
 * @return 1 if key1> key2 by alphabeta order
           -1 if key1< key2 by alphabeta order
           0 otherwise
 */

int compareM( const int s1, const indice_t *const key1, const indice_t *varMap1,
              const int s2, const indice_t *const key2,
              const indice_t *varMap2 ) {
  int i, j;

  i = j = 0;
  while ( i < s1 && j < s2 ) {
    if ( varMap1[ i ] < varMap2[ j ] ) {
      if ( key1[ i ] > 0 ) return 1;
      i++;
    } else if ( varMap1[ i ] > varMap2[ j ] ) {
      if ( key2[ j ] > 0 ) return -1;
      j++;

    } else {
      if ( key1[ i ] > key2[ j ] )
        return 1;
      else if ( key1[ i ] < key2[ j ] )
        return -1;
      else {
        i++;
        j++;
      }
    }
  }

  while ( i < s1 )
    if ( key1[ i++ ] > 0 ) return 1;
  while ( j < s2 )
    if ( key2[ j++ ] > 0 ) return -1;
  return 0;
}
