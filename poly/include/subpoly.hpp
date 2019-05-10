/**
 * @file   Subpoly.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Sep 22 22:31:51 2017
 *
 * @brief  Subpoly
 *
 *
 */
#ifndef __SUB_OLY_H
#define __SUB_OLY_H
#include "crypt_md5.h"
#include "poly.hpp"
#include <cstring>
namespace aiSat {
namespace psd {
class ConvexGenerator;
class SOSChecker;
} // namespace psd
namespace poly {

template <typename C = double, typename T = unsigned char>

class Subpoly {
  friend class aiSat::psd::ConvexGenerator;
  friend class aiSat::psd::SOSChecker;

private:
  const Poly<C, T> &parent;
  uint8_t md5sum[ DIGEST_SIZE ]; /// the md5sum of this subpolynomial, when test
  /// whether the two polynomials equal first test
  /// the two mds5sum equal
  vector<int> locs;

  void getMd5sum( md5_ctx_t *ctx ) {
    md5_init( ctx );
    uint8_t id = parent.getId();
    md5_update( ctx, &id, 1 );

    for ( size_t i = 0; i < locs.size(); i++ ) {
      uint8_t loc = locs[ i ];
      md5_update( ctx, &loc, 1 );
    }

    md5_finalize( ctx );
  }

public:
  Subpoly( const Poly<C, T> &p )
      : parent( p ) {
    int size = parent.getSize();
    for ( int i = 0; i < size; i++ ) {
      locs.push_back( i );
    }
    md5_ctx_t ctx;
    getMd5sum( &ctx );
    memcpy( md5sum, MD5_DIGEST( &ctx ), 16 * sizeof( uint8_t ) );
  }
  Subpoly( const Poly<C, T> &p, const vector<int> &loc )
      : parent( p )
      , locs( loc ) {
    md5_ctx_t ctx;
    getMd5sum( &ctx );
    memcpy( md5sum, MD5_DIGEST( &ctx ), 16 * sizeof( uint8_t ) );
  }

  Subpoly( const Poly<C, T> &p, const int num, const vector<int> &loc )
      : parent( p )
      , locs( loc ) {
    locs.resize( num );
    md5_ctx_t ctx;
    getMd5sum( &ctx );
    memcpy( md5sum, MD5_DIGEST( &ctx ), 16 * sizeof( uint8_t ) );
  }

  int size() const { return locs.size(); }

  const uint8_t *getmd5() const { return md5sum; }

  int operator[]( const int i ) const { return locs[ i ]; }
  int getTotalDegree() {
    int re = 0;
    int temp, j;

    int varNum = parent.getVarNum();

    for ( size_t i = 0; i < locs.size(); i += 1 ) {
      temp = 0;

      for ( j = 0; j < varNum; j += 1 ) {
        temp += parent.getDegreeAt(
            locs[ i ], j ); // poly->indices[sp->locs[i] * varNum + j];
      }
      if ( temp > re ) {
        re = temp;
      }
    }
    return re;
  }

  string toString( void ) const { return parent.toString( locs ); }

  Poly<C, T> getPoly() const {
    Poly<C, T> rep;
    parent.getSubpoly( locs, rep );
    return rep;
  }
  const Poly<C, T> &getParent() const { return parent; }

  template <typename CC, typename TT>
  friend bool operator==( Poly<CC, TT> &lhs, Poly<CC, TT> &rhs );

  template <typename CC, typename TT>
  friend ostream &operator<<( ostream &os, Poly<CC, TT> &p );
};

template <typename CC, typename TT>
bool operator==( Subpoly<CC, TT> &lhs, Subpoly<CC, TT> &rhs ) {
  if ( 0 != memcmp( lhs.getmd5(), rhs.getmd5(), DIGEST_SIZE ) ) {
    return false;
  }
  return true;
}
template <typename CC, typename TT>
ostream &operator<<( ostream &os, Subpoly<CC, TT> &p ) {
  os << p.toString();
  return os;
}
} // namespace poly
} // namespace aiSat

#endif
