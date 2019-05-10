
/**
 * @file   support_table.c
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 15:35:45 2015
 *
 * @brief   Since every polynomial constrain is parametric polynomial
 * when the coefficients of the template constraint is concrete the
 * constraint is definited. And there are different polynomial contraints may
 * share monomial base such as polynomial contraints
 * c00+c01x+c10y+c11xy+c02x^2+c20y^2 >=0  and b00+b01x+b10y+b11xy+b02x^2+b20y^2
 * >=0
 * where c**, b** are unknown parameters and set {1, x, y, xy, x^2, y^2 } is the
 * monomial base.
 *
 *
 * f= G^T M G=<G G^T,M > = \sum_{i=1}^s g_i^2
 * G called half monomial set
 * all the elements occouring in GG^T called all monomial set
 * we define an ArrangeMatrix by the location   x^a  occurring in  for every
 * element x^a
 * in  GG^T. For example G=[ 1 x y z ]
 * G G^T=[ 1  x    y    z ]
 *       [ x  x^2  xy   xz]
 *       [ y  xy   y^2  yz]
 *       [ z  xz   yz  z^2], then the ArrangeMatrix corresponding monomial xy is
 *  [0  0  0  0]
 *  [0  0  1  0]
 *  [0  1  0  0]
 *  [0  0  0  0]
 *
 */

#include "support_table.h"

#include "crypt_md5.h"
#include "psdutil.h"
#include "sort.h"
#include "sparse.h"
#include "util.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace aiSat {
namespace psd {

Support *createSupport( const int deg, const int varId, const int consNum,
                        int *consId ) {
  Support *re;
  re       = (Support *) malloc_d( sizeof( Support ) );
  re->type = NORMAL;

  re->varId = -1;
  memset( re->md5sum, 0, DIGEST_SIZE );

  /*-----------------------------------------------------------------------------
   *  have some problems
   *-----------------------------------------------------------------------------*/
  re->md5sum[ 0 ] = (uint8_t) deg;
  re->md5sum[ 1 ] = (uint8_t) varId;
  re->md5sum[ 2 ] = (uint8_t) consNum;

  re->deg      = deg;
  re->varId    = varId;
  re->constNum = consNum;
  re->consId   = consId;
  re->consCap  = consNum;
  return re;
}

int Supporttable::findSupElem( const int deg, const int varId,
                               const int consNum, const int *consIds ) {
  int i, j;

  /* 	if(0==deg) return 0;
   */
  /* when deg==0 the sosM is constant 1 */

  for ( i = 0; i < (int) values.size(); i += 1 ) {
    if ( values[ i ]->type == NORMAL ) {
      if ( deg == values[ i ]->deg && varId == values[ i ]->varId &&
           consNum == values[ i ]->constNum ) {
        j = 0;
        while ( j < consNum ) {
          if ( consIds[ j ] != values[ i ]->consId[ j ] ) {
            break;
          }
          j++;
        }
        if ( j == consNum ) {
          return i;

        }
      }
    }
  }

  return -1;
}

int Supporttable::findSupByPoly( const Poly_t &subpoly ) {
  int i = 0;
  // for ( i = 0; i < (int) values.size(); i += 1 ) {
  //   if ( values[ i ]->type == POLY ) {
      
  //     if ( 0 == memcmp( values[ i ]->md5sum, subpoly.getmd5(), DIGEST_SIZE ) ) {
  //       return i;
  //       //        return findBitmapByValue( loc, i );
  //     }
  //   }
  // }
  return -1;
}

int Supporttable::findSupElemByIndice( const int varId, const indice_t *indices,
                                       const int value_size ) {
  int     i;
  uint8_t md5sum[ DIGEST_SIZE ];
  md5sumbyIndice( md5sum, varId, indices, value_size );
  for ( i = 0; i < (int) values.size(); i++ ) {
    if ( values[ i ]->type == INDICE &&
         0 == memcmp( values[ i ]->md5sum, md5sum, DIGEST_SIZE ) ) {
      return i;
    }
  }
  return -1;
}

int Supporttable::addSOSup( const int deg, const int varId, const int consNum,
                            int *consIds ) {
  int re = findSupElem( deg, varId, consNum, consIds );

  if ( re > -1 ) {
    return re;
  }

  re = (int) values.size();
  values.push_back( createSupport( deg, varId, consNum, consIds ) );

  return re;

}

int Supporttable::addSOSsupByIndice( const int varId, indice_t *indices,
                                     const int esize ) {
  qsortKElem( indices, getVarTable<indice_t>().getVarNum( varId ), 0, esize - 1,
              compare );

  int re = findSupElemByIndice( varId, indices, esize );

  if ( re > -1 ) {
    return re;
  }
  re             = values.size();
  int value_size = esize;
  values.push_back( new Support( varId, indices, value_size ) );

  return re;

}

Support *Supporttable::getSupElem( const int id ) {
  if( id>=values.size( )){
    return NULL;
  }
  return values[ id ];
}

indice_t *Supporttable::getSOSsup( const int id, int *length ) {
  if( id>=values.size( )){
    return NULL;
  }

  *length  = values[ id]->sosLength;
  return values[ id]->SOSsup;
}

indice_t *Supporttable::getGsup( const int id, int *length ) {

  if( id>=values.size( )){
    return NULL;
  }

  *length = values[id]->gLength;
  return values[id]->Gsup;
}

void Supporttable::setSOSsup( const int id, const int len, indice_t *value ) {

  if( id>=values.size( )){
    return ;
  }
  values[id ]->sosLength = len;
  values[id ]->SOSsup    = value;
}

void Supporttable::setGsup( const int id, const int len, indice_t *value ) {
  if( id>=values.size( )){
    return ;
  }
  values[id]->gLength = len;
  values[id]->Gsup = value;
}

int Supporttable::addconvexsosSup( Poly_t &poly ) {
  int re = findSupByPoly( poly );
  if ( re > -1 ) {
    return re;
  }

  int value_size = values.size();

  values.push_back( new Support( &poly ) );

  return value_size;

}

int Supporttable::getsosSLength( const int id ) {
  if( id>= values.size( )){
    return -1;
  }
  return values[id]->sosLength;
}

void Supporttable::setArrangeM( const int id, ArrangeMatrix **value,
                                const int egLength ) {
  if( id>=values.size( )){
    return;
  }
  values[id]->gLength  = egLength;
  values[id]->arrangeM = value;
}

ArrangeMatrix **Supporttable::getAMIndex( const int id, int *egLength ) {
  if( id>=values.size( )){
    return NULL;
  }
  *egLength =values[ id]-> gLength;
  return values[ id]->arrangeM;
}

void Supporttable::clearSupportTable( void ) {

  int value_size = (int) values.size();
  for ( int i = 0; i < value_size; i += 1 ) {
    delete values[ i ];
  }
  values.clear();
}
Supporttable::Supporttable() {  }
Supporttable::~Supporttable() {
  int value_size = (int) values.size();

  for ( int i = 0; i < value_size; i += 1 ) {
    delete values[ i ];
  }
  values.clear( );

}

Supporttable SUPPORT_TABLE;
} // namespace psd
} // namespace aiSat
