/*
 * =====================================================================================
 *
 *       Filename:  bitset.c
 *
 *    Description:
 *
 *
 *        Version:  1.0
 *        Created:  05/05/2013 05:05:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "bitset.h"

#include <string.h>

static const int PER = 32;

static unsigned int map[] = {
    1u,       2u,       4u,       8u,       16u,      32u,      64u,
    128u,     256u,     512u,     1024u,    2048u,    4096u,    8192u,
    16384u,   32768u,   1u << 16, 1u << 17, 1u << 18, 1u << 19, 1u << 20,
    1u << 21, 1u << 22, 1u << 23, 1u << 24, 1u << 25, 1u << 26, 1u << 27,
    1u << 28, 1u << 29, 1u << 30, 1u << 31};

BitSet *bitSet_new( void ) {
  BitSet *re = (BitSet *) malloc_d( sizeof( BitSet ) );
  re->cap    = BITSET_DEAFULT_CAP;
  re->value  = (uint32_t *) malloc_d( re->cap * sizeof( uint32_t ) );
  memset( re->value, 0, re->cap * sizeof( uint32_t ) );
  re->length = 0;
  return re;
}
BitSet *bitSet_newByL( const unsigned length ) {
  BitSet *re = (BitSet *) malloc_d( sizeof( BitSet ) );
  re->cap    = BITSET_DEAFULT_CAP + length / PER;
  re->value  = (uint32_t *) malloc_d( re->cap * sizeof( uint32_t ) );
  memset( re->value, 0, re->cap * sizeof( uint32_t ) );
  re->length = length;
  return re;
}

BitSet *bitSet_copy( const BitSet *orig ) {
  BitSet *re;

  re         = (BitSet *) malloc_d( sizeof( BitSet ) );
  re->cap    = orig->cap;
  re->length = orig->length;

  re->value = (uint32_t *) malloc_d( re->cap * sizeof( uint32_t ) );

  memcpy( re->value, orig->value, re->cap * sizeof( uint32_t ) );

  return re;
}

uscalar_t bitSet_length( const BitSet *bs ) { return bs->length; }

void bitSet_free( BitSet *b ) {
  free( b->value );
  free( b );
}

void bitSet_clear( BitSet *b ) {
  memset( b->value, 0, b->cap * sizeof( uint32_t ) );
}

void bitSet_add( BitSet *b, const unsigned int loc ) {
  unsigned int row;
  unsigned int col;

  col = loc & 31;
  row = loc >> 5;
  if ( row >= b->cap ) {
    int origCap = b->cap;
    b->cap      = row;
    b->cap++;
    b->cap *= ENLARGE_RAT;
    b->value = (uint32_t *) realloc_d( b->value, b->cap * sizeof( uint32_t ) );
    memset( b->value + origCap, 0u, ( b->cap - origCap ) * sizeof( uint32_t ) );
  }

  b->value[ row ] |= map[ col ];
  if ( loc >= b->length ) b->length = loc + 1;
}

void bitSet_set( BitSet *b, unsigned int loc, BOOL v ) {
  unsigned int row;
  unsigned int col;

  col = loc & 31;
  row = loc >> 5;
  if ( v )
    b->value[ row ] |= map[ col ];
  else
    b->value[ row ] &= ~( map[ col ] );
}

BOOL bitSet_get( const BitSet *b, const unsigned int loc ) {
  ASSERT( loc < ( b->length ), "overflow" );
  unsigned int row;
  unsigned int col;

  col = loc & 31;
  row = loc >> 5;
  return b->value[ row ] & map[ col ];
}
