/*
 * =====================================================================================
 *
 *       Filename:  pointvector.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  01/05/2013 03:58:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "pointvector.h"
#include "selfmemutil.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PointVector *vector_new( void ( *del )( void * ) ) {
  PointVector *re;
  re           = (PointVector *) malloc_d( sizeof( PointVector ) );
  re->capacity = DEFAULT_CAP;
  re->data     = (pointer_t *) calloc_d( re->capacity, sizeof( void * ) );
  re->size     = -1;
  re->delfun   = del;
  return re;
}

PointVector *vector_newByL( void ( *del )( void * ), unsigned length ) {
  PointVector *re;
  re           = (PointVector *) malloc_d( sizeof( PointVector ) );
  re->capacity = length;
  re->data     = (pointer_t *) calloc_d( re->capacity, sizeof( void * ) );
  re->size     = -1;
  re->delfun   = del;
  return re;
}

void reScale_V( PointVector *p ) {
  size_t origCap = p->capacity;

  p->capacity *= ENLARGE_RAT;
  p->capacity += 2;
  p->data = (void **) realloc_d( p->data, p->capacity * sizeof( void * ) );
  memset( p->data + origCap, 0, ( p->capacity - origCap ) * sizeof( void * ) );
}

void vector_free( PointVector *p ) {
  size_t i;
  for ( i = 0; i < p->size; i += 1 ) {
    p->delfun( p->data[ i ] );
  }
  free( p->data );
  free( p );
}

void push_back_V( PointVector *p, void *elem ) {
  p->size++;
  if ( p->size >= p->capacity ) {
    p->capacity = p->size + 1;
    reScale_V( p );
  }

  p->data[ p->size ] = elem;
}

void insertBack_V( PointVector *lhs, const PointVector *rhs ) {
  size_t i;

  for ( i = 0; i < rhs->size + 1; i += 1 ) {
    push_back_V( lhs, rhs->data[ i ] );
  }
}

size_t vector_length( const PointVector *p ) { return p->size + 1; }

void vector_set( PointVector *vector, unsigned idx, void *v ) {
  ASSERT( idx < vector->size, "overflow " );
  if ( NULL != vector->data[ idx ] ) vector->delfun( vector->data[ idx ] );
  vector->data[ idx ] = v;
}

void *vector_get( const PointVector *p, const int n ) { return p->data[ n ]; }

void clear_V( PointVector *vec ) {
  vec->size = -1;
  /* 	int i;
   * 	for ( i = 0; i < vec->size; i += 1 ) {
   *
   * 		vec->delfun(vec->data[i]);
   *
   * 	}
   * 	vec->size=-1;
   */
}
