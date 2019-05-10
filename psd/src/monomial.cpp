#include "monomial.h"
#include "util.h"
#include <cstring>

namespace aiSat {

namespace psd {

void Monomialvalue::add( uint32_t set[], const int v ) {
  set[ v / 32 ] |= valueMap[ v % 32 ];
}
void Monomialvalue::add( uint32_t set[], const Monomialvalue &other ) {
  set[ 0 ] |= other.relation[ 0 ];
  set[ 1 ] |= other.relation[ 1 ];
  set[ 2 ] |= other.relation[ 2 ];
  set[ 3 ] |= other.relation[ 3 ];
}

bool Monomialvalue::contain( const uint32_t       set[],
                             const Monomialvalue &other ) {
  if ( ( other.relation[ 0 ] & set[ 0 ] ) != other.relation[ 0 ] ) return false;
  if ( ( other.relation[ 1 ] & set[ 1 ] ) != other.relation[ 1 ] ) return false;
  if ( ( other.relation[ 2 ] & set[ 2 ] ) != other.relation[ 2 ] ) return false;
  if ( ( other.relation[ 3 ] & set[ 3 ] ) != other.relation[ 3 ] ) return false;
  return true;
}

bool Monomialvalue::conjunction( const uint32_t       set[],
                                 const Monomialvalue &other ) {
  if ( set[ 0 ] & other.relation[ 0 ] ) return true;
  if ( set[ 1 ] & other.relation[ 1 ] ) return true;
  if ( set[ 2 ] & other.relation[ 2 ] ) return true;
  if ( set[ 3 ] & other.relation[ 3 ] ) return true;
  return false;
}

void Monomialvalue::add( const int k ) {
  relation[ k / 32 ] |= valueMap[ k % 32 ];
  change = true;
}

void Monomialvalue::clear( void ) {
  relation[ 0 ] = 0;
  relation[ 1 ] = 0;
  relation[ 2 ] = 0;
  relation[ 3 ] = 0;
  change        = false;
}
void Monomialvalue::add( const Monomialvalue &other ) {
  relation[ 0 ] |= other.relation[ 0 ];
  relation[ 1 ] |= other.relation[ 1 ];
  relation[ 2 ] |= other.relation[ 2 ];
  relation[ 3 ] |= other.relation[ 3 ];
  change = true;
}

bool Monomialvalue::contain( const Monomialvalue &other ) const {
  if ( ( other.relation[ 0 ] & relation[ 0 ] ) != other.relation[ 0 ] )
    return false;
  if ( ( other.relation[ 1 ] & relation[ 1 ] ) != other.relation[ 1 ] )
    return false;
  if ( ( other.relation[ 2 ] & relation[ 2 ] ) != other.relation[ 2 ] )
    return false;
  if ( ( other.relation[ 3 ] & relation[ 3 ] ) != other.relation[ 3 ] )
    return false;
  return true;
}

bool Monomialvalue::contain( const int k ) const {
  return relation[ k / 32 ] & valueMap[ k % 32 ];
}

bool Monomialvalue::conjunction( const Monomialvalue &other ) const {
  if ( other.relation[ 0 ] & relation[ 0 ] ) return true;
  if ( other.relation[ 1 ] & relation[ 1 ] ) return true;
  if ( other.relation[ 2 ] & relation[ 2 ] ) return true;
  if ( other.relation[ 3 ] & relation[ 3 ] ) return true;
  return false;
}

bool Monomialvalue::conjunction( const uint32_t set[] ) const {
  if ( set[ 0 ] & relation[ 0 ] ) return true;
  if ( set[ 1 ] & relation[ 1 ] ) return true;
  if ( set[ 2 ] & relation[ 2 ] ) return true;
  if ( set[ 3 ] & relation[ 3 ] ) return true;
  return false;
}

void Monomialvalue::remove( const Monomialvalue &other ) {
  relation[ 0 ] = relation[ 0 ] ^ ( relation[ 0 ] & other.relation[ 0 ] );
  relation[ 1 ] = relation[ 1 ] ^ ( relation[ 1 ] & other.relation[ 1 ] );
  relation[ 2 ] = relation[ 2 ] ^ ( relation[ 2 ] & other.relation[ 2 ] );
  relation[ 3 ] = relation[ 3 ] ^ ( relation[ 3 ] & other.relation[ 3 ] );

  change = true;
}

int Monomialvalue::size() const {
  int re = 0;
  re     = 0;
  int i  = 0;
  for ( i = 0; i < 32; i += 1 ) {
    if ( relation[ 0 ] & valueMap[ i ] ) re++;
  }

  for ( i = 0; i < 32; i += 1 ) {
    if ( relation[ 1 ] & valueMap[ i ] ) re++;
  }

  for ( i = 0; i < 32; i += 1 ) {
    if ( relation[ 2 ] & valueMap[ i ] ) re++;
  }

  for ( i = 0; i < 32; i += 1 ) {
    if ( relation[ 3 ] & valueMap[ i ] ) re++;
  }
  return re;
}

Monomial::operator size_t() const {
  return hash_fast64( (const unsigned char *) indice, dim * sizeof( indice_t ),
                      0 );
}

bool Monomial::operator==( const Monomial &other ) const {
  return memcmp( indice, other.indice, dim * sizeof( indice_t ) ) == 0;
}
} // namespace psd
} // namespace aiSat
