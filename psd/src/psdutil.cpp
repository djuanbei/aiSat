/**
 * @file   psdutil.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Sep 23 11:00:00 2017
 *
 * @brief  the util function in polynomial constraints system
 *
 *
 */
#include "psdutil.h"
#include "mono_const.h"
#include "monoconst_table.h"
#include "pconst_table.h"
#include "search.h"
#include "support_table.h"
#include "util.h"
#include <algorithm>
#define TYPE_H(n ) \
  if ( K== ( n)){                 \
    typedef   H<indice_t, n> H_t; \
   H_t* temp=(H_t * ) indices; \
   std::sort( temp, temp+size); return ;        \
}

namespace aiSat {

namespace psd {

void qsortKElem( indice_t *indices,  const size_t K,   size_t size){
  if( K==1){
    std::sort( indices, indices+size);
    return;
  }
  TYPE_H( 2);
  TYPE_H( 3);
  TYPE_H( 4);
  TYPE_H( 5);
  TYPE_H( 6);
  TYPE_H( 7);
  TYPE_H( 8);
  TYPE_H( 9);
  TYPE_H( 10);
  TYPE_H( 11);
  TYPE_H( 12);
  TYPE_H( 13);
  TYPE_H( 14);
  TYPE_H( 15);
  ASSERT( false,"two many variables");
}



/**
 *@brief enumerate all monomials who belong to support whose id is supportId
 * Useing Lexicographic Order x1>x2>..>xn
 * restraintNum is the number of restraints
 * output all the monomials which satsfy these restraints
 *
 * @param supportId the support id
 * @param length the length of all monomials
 *
 * @return all monomials who belong to support whose id is supportId if success
 * false otherwise.
 */

indice_t *getAllMonByTd( const int supportId, int *const length ) {
  int       i, j, k;
  int       sum  = 1;
  int       size = 0;
  int       cap;
  int       nch;
  int       n, d;
  indice_t *Z;

  Support *sup = SUPPORT_TABLE.getSupElem( supportId );

  if ( sup == NULL ) {
    return NULL;
  }

  n = getVarTable<indice_t>().getVarNum( sup->varId );
  if ( 0 == n ) {
    Z       = (indice_t *) malloc_d( sizeof( indice_t ) );
    Z[ 0 ]  = 0;
    *length = 1;
    return Z;
  }

  d = sup->deg;

  if ( sup->constNum == 0 ) {
    *length = nchoosek( n + d, d );
    Z       = (indice_t *) malloc_d( ( *length ) * n * sizeof( indice_t ) );

    /*-----------------------------------------------------------------------------
     *  The first row denote const 1, and second ros denote monomial x1
     *-----------------------------------------------------------------------------*/
    for ( i = 0; i < n;
          i += 1 ) { /* initial the first two rows (monomial) to zero  */
      Z[ i ]     = 0;
      Z[ n + i ] = 0;
    }

    Z[ 2 * n - 1 ] = 1;
    i              = 2;

    while ( i < ( *length ) ) {
      /*memcpy(Z[i],Z[i-1],n*sizeof(int));*/

      for ( j = 0; j < n; j++ ) {
        Z[ i * n + j ] = Z[ ( i - 1 ) * n + j ];
      }

      if ( sum < d ) {
        Z[ i * n + n - 1 ]++;
        sum++;
      } else {
        k = n - 1;
        while ( Z[ i * n + k ] == 0 )
          k--;
        sum -= ( Z[ i * n + k ] - 1 );
        Z[ i * n + k ] = 0;
        Z[ i * n + k - 1 ]++;
      }
      i++;
    }
  } else {
    nch = nchoosek( n + d, d );
    cap = nch / ( ( sup->constNum + 1 ) * n );

    Z = (indice_t *) malloc_d( n * cap * sizeof( indice_t ) );

    indice_t temp[ n ];

    for ( i = 0; i < n; i += 1 ) {
      temp[ i ] = 0; /* constant term */
    }
    /*-----------------------------------------------------------------------------
     *  satsfies restraints
     *-----------------------------------------------------------------------------*/
    if ( criteria( sup, temp ) ) {
      addZ( Z, &cap, &size, n, temp );
    }

    temp[ n - 1 ] = 1;

    if ( criteria( sup, temp ) ) {
      addZ( Z, &cap, &size, n, temp );
    }
    i = 2;
    while ( i < nch ) {
      if ( sum < d ) {
        temp[ n - 1 ]++;
        sum++;
      } else {
        k = n - 1;
        while ( temp[ k ] == 0 )
          k--;
        sum -= ( temp[ k ] - 1 );
        temp[ k ] = 0;
        temp[ k - 1 ]++;
      }
      if ( criteria( sup, temp ) ) {
        addZ( Z, &cap, &size, n, temp );
      }

      i++;
    }
    *length = size;
  }
  return Z;
}

void getAllMonHomoTd( const int varNum, const int deg, const int length,
                      indice_t *indices ) {
  ASSERT( varNum > 0, "variable number must be positive number" );

  if ( varNum == 1 ) {
    indices[ 0 ] = deg;
    return;
  }

  int n = varNum - 1;

  indice_t *Z =
      (indice_t *) malloc_d( ( length + 1 ) * n * sizeof( indice_t ) );

  int sum = 1;
  int i, j, k;

  for ( i = 0; i < n; i++ ) {
    Z[ i ] = Z[ n + i ] = 0;
  }

  Z[ 2 * n - 1 ] = 1;
  i              = 2;

  while ( i < length ) {
    for ( j = 0; j < n; j++ ) {
      Z[ i * n + j ] = Z[ ( i - 1 ) * n + j ];
    }
    if ( sum < deg ) {
      Z[ i * n + n - 1 ]++;
      sum++;
    } else {
      k = n - 1;
      while ( Z[ i * n + k ] == 0 )
        k--;

      sum -= ( Z[ i * n + k ] - 1 );
      Z[ i * n + k ] = 0;
      Z[ i * n + k - 1 ]++;
    }
    i++;
  }

  for ( i = 0; i < length; i++ ) {
    sum = 0;
    for ( j = 0; j < n; j++ ) {
      indices[ i * varNum + j ] = Z[ i * n + j ];
      sum += Z[ i * n + j ];
    }
    indices[ i * varNum + j ] = deg - sum; // homogenous
  }

  free( Z );
}

bool criteria( const Support *S, indice_t *key ) {
  int i;
  int resNum = S->constNum;

  for ( i = 0; i < resNum; i += 1 ) {
    if ( !consEval( key, S->varId,
                    MONO_CONS_TABLE.getconsElem( S->consId[ i ] ) ) ) {
      return false;
    }
  }

  return true;
}

/**
 *@brief  For every monomial c of SOSM  W_c is  a  nchoosek(n+d/2,d/2) *
 * nchoosek(n+d/2,d/2) size symmetric matrix
 *  all element of W_c is one identity or zero. It means the location of c
 * occurs in G^T * G
 *  where G=[1,x_1,...,x_n,x_1x_2,...,x_n^(d/2)]
 *  n is the number of variable and d is the max total degree of SOSM.
 *  d is a even integer
 *
 * @param supportId
 * @param SOSM
 * @param blockSize
 * @param lengthSOS the number of different element in SOSM
 *
 * @return A array of ArrangeMatrix with nchoosek(n+d/2,d/2) *
 * nchoosek(n+d/2,d/2)  for every element in SOSM
 */
ArrangeMatrix **createArrangeM( const int supportId, indice_t const *SOSM,
                                int *const blockSize, int const lengthSOS ) {
  int       i, j, k, index;
  Support * sup = SUPPORT_TABLE.getSupElem( supportId );
  const int n   = getVarTable<indice_t>().getVarNum( sup->varId );

  indice_t  temp[ n + 1 ];
  indice_t *G = SUPPORT_TABLE.getGsup( supportId, blockSize );
  if ( NULL == G ) {
    sup->deg /= 2;
    G = getAllMonByTd( supportId, blockSize ); /* degree squared */
    SUPPORT_TABLE.setGsup( supportId, *blockSize, G );
    sup->deg *= 2;
  }

  ArrangeMatrix **re =
      (ArrangeMatrix **) malloc_d( lengthSOS * sizeof( ArrangeMatrix * ) );

  for ( i = 0; i < lengthSOS; i++ ) {
    re[ i ] = new ArrangeMatrix( *blockSize );
  }

  /*-----------------------------------------------------------------------------
   *  |  a00 a10 a20  |
   *  |  a10 a11 a12  |
   *  |  a20 a21 a22  |
   *  As (Z^T)Z  is a symmetric matrix, we only need to compute the lower
   *triangle
   *  of it.
   *-----------------------------------------------------------------------------*/
  for ( i = 0; i < ( *blockSize ); i++ ) {
    for ( j = 0; j <= i; j += 1 ) {
      for ( k = 0; k < n; k += 1 ) {
        temp[ k ] = G[ i * n + k ] + G[ j * n + k ]; /* monomial add */
      }
      index = findIndex( temp, SOSM, lengthSOS, n );
      ASSERT( index >= 0, "some thing wrong" );
      re[ index ]->addSparseElem( i, j );
    }
  }


  SUPPORT_TABLE.setArrangeM( supportId, re, *blockSize );
  return re;
}

void deleteW( ArrangeMatrix **W[], const int size, const int len[] ) {
  int i, j;

  for ( i = 0; i < size; i += 1 ) {
    for ( j = 0; j < len[ i ]; j += 1 ) {
      delete W[ i ][ j ];
      // deleteSparse(W[i][j]);
    }
    free( W[ i ] );
  }
}

void deleteSparseA( ArrangeMatrix **s, const int len ) {
  int i;
  for ( i = 0; i < len; i += 1 ) {
    delete s[ i ];
  }
  free( s );
}

Poly_t sosConvertPoly( Blockmatrix *const X, const int k, const int blockSize,
                       const int supportId ) {
  int    vid = SUPPORT_TABLE.getSupElem( supportId )->varId;
  Poly_t re( vid );
  coef_t cf;
  int    i, j, row, col;

  vector<indice_t> vars;
  getVarTable<indice_t>().getVarVec( vid, vars );
  const int varSize = vars.size();

  const int length = SUPPORT_TABLE.getsosSLength( supportId );

  int       dummy;
  indice_t *support = SUPPORT_TABLE.getSOSsup( supportId, &dummy );

  ArrangeMatrix **S = SUPPORT_TABLE.getAMIndex( supportId, &dummy );

  for ( i = 0; i < length; i += 1 ) {
    Poly_t::Term temp_term;
    for ( int k = 0; k < varSize; k++ ) {
      temp_term.key.push_back(
          make_pair( vars[ k ], support[ i * varSize + k ] ) );
    }

    cf = 0;
    for ( j = 0; j < S[ i ]->size(); j += 1 ) {
      row = ( *( S[ i ] ) )[ j ].row + 1;
      col = ( *( S[ i ] ) )[ j ].col + 1;
      if ( row == col ) {
        cf += X->blocks[ k + 1 ].data.mat[ ijtok( row, col, blockSize ) ];

      } else {
        cf +=
            2 * ( X->blocks[ k + 1 ].data.mat[ ijtok( row, col, blockSize ) ] );
      }
    }

    temp_term.cf = cf;
    re.add_term( temp_term );
  }

  return re;
}

} // namespace psd
} // namespace aiSat
