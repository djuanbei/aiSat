/*
  Calculate the inverse of an n by n matrix A.  A is assumed to be symmetric
  and positive definite.  The original matrix A is destroyed in the process.
  On return, A is certified to be symmetric and positive definite.
*/

#include "declarations.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int     chol_blk( n, lda, A ) int n;
int     lda;
double *A;
{
  int info;
  int i;
  int j;

  info = 0;

  MY_DPOTRF( "U", &n, A, &lda, &info );

  if ( info != 0 ) {
    return ( 1 );
  };

  /*
   * Now, make sure that the lower triangle of A is 0.0
   */

  for ( j = 1; j < n; j++ )
    for ( i = j + 1; i <= n; i++ )
      A[ ijtok( i, j, lda ) ] = 0.0;

  /*
   * We're safely through the computation.  Return 0 to indicate success.
   */

  return ( 0 );
}

int     chol_diag( n, A ) int n;
double *A;
{
  double ent;
  int    i;

  for ( i = 1; i <= n; i++ ) {
    ent = A[ i ];
    if ( ent <= 0.0 ) {
      return ( 1 );
    };
    A[ i ] = sqrt( ent );
  };

  /*
   * We're safely through the computation.  Return 0 to indicate success.
   */

  return ( 0 );
}

/*
 * Find the Cholesky factor of a block structured matrix.
 */

int chol( A ) struct blockmatrix A;
{
  int blk;
  int ret;

  for ( blk = 1; blk <= A.nblocks; blk++ ) {
    switch ( A.blocks[ blk ].blockcategory ) {
    case DIAG:
      ret = chol_diag( A.blocks[ blk ].blocksize, A.blocks[ blk ].data.vec );
      break;
    case MATRIX:
      ret = chol_blk( A.blocks[ blk ].blocksize, A.blocks[ blk ].blocksize,
                      A.blocks[ blk ].data.mat );
      break;
    default:
      printf( "Unknown block type! \n" );
      exit( 12 );
    };
    if ( ret != 0 ) return ( 1 );
  };

  return ( 0 );
}

void trans( A ) struct blockmatrix A;
{
  int i, j;
  int blk;

  for ( blk = 1; blk <= A.nblocks; blk++ ) {
    switch ( A.blocks[ blk ].blockcategory ) {
    case DIAG:
      break;
    case MATRIX:
      for ( i = 2; i <= A.blocks[ blk ].blocksize; i++ )
        for ( j = 1; j < i; j++ ) {
          A.blocks[ blk ].data.mat[ ijtok( i, j, A.blocks[ blk ].blocksize ) ] =
              A.blocks[ blk ]
                  .data.mat[ ijtok( j, i, A.blocks[ blk ].blocksize ) ];
          A.blocks[ blk ].data.mat[ ijtok( j, i, A.blocks[ blk ].blocksize ) ] =
              0.0;
        };

      break;
    default:
      printf( "Unknown block type! \n" );
      exit( 12 );
    };
  };

  return;
}

/*
  Computes the inverse of a Cholesky factor in A.  The result is stored
  in work.  Note that the original Cholesky factor in A is not destroyed.
*/

void chol_inv( A, work ) struct blockmatrix A, work;
{
  int     i;
  int     info;
  int     blk;
  int     n;
  double *ap;
  double  scale = 1.0;

  copy_mat( A, work );

  for ( blk = 1; blk <= A.nblocks; blk++ ) {
    switch ( work.blocks[ blk ].blockcategory ) {
    case DIAG:
      for ( i = 1; i <= work.blocks[ blk ].blocksize; i++ )
        work.blocks[ blk ].data.vec[ i ] =
            1.0 / work.blocks[ blk ].data.vec[ i ];
      break;
    case MATRIX:
      n     = work.blocks[ blk ].blocksize;
      ap    = work.blocks[ blk ].data.mat;
      scale = 1.0;
      MY_DTRTRI( "U", "N", &n, ap, &n, &info );
      if ( info != 0 ) {
        printf( "DTTRI failed!\n" );
        exit( 11 );
      };

      break;
    default:
      printf( "Unknown block type! \n" );
      exit( 12 );
    };
  };

  return;
}
