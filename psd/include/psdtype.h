/**
 * @file   psdtype.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Sep 23 10:38:08 2017
 *
 * @brief  the type will use in polynomial constraint system
 *
 *
 */

#ifndef __PSD_TYPE_H
#define __PSD_TYPE_H
#include "sparse.h"
#include "poly.hpp"
#include "polynomialconstraint.h"

#include "util.h"


#include <cstdint>


namespace aiSat {
namespace psd {
using namespace poly;

extern void deleteSparseA( ArrangeMatrix **s, const int len );

typedef Poly<coef_t, indice_t>    Poly_t;
//typedef Subpoly<coef_t, indice_t> Subpoly_t;

/**
 *An array of spare two dimension coordinate
 *
 */



enum monomialOrder { LEX, GRLEX, GREVLEX };

typedef enum monomialOrder MonomialOrder;

enum monomialConstraintType { HOMOG, KHOMOG, SYMM };

typedef enum monomialConstraintType monomialConstraintType;

struct MonomialConstraint {
  int  id;
  int  deg;
  int  varId;
  int *linCoefs;
  int  minValue; /* only use in linconstraints */
  int  consFunId;
  MonomialConstraint() { linCoefs = NULL; }
};

enum SupportType { NORMAL, POLY, INDICE };


struct Support {
  SupportType type;
  Poly_t *subp;

  uint8_t md5sum[ DIGEST_SIZE ];

  int  deg;
  int  varId;
  int *consId;
  int  constNum;
  int  consCap;

  int  sosLength;
  int gLength;
  ArrangeMatrix ** arrangeM;
  indice_t * SOSsup;
  indice_t * Gsup;

  


  Support( const int evarId, const indice_t *indices, const int size ) {
    subp = NULL;
    type = INDICE;
    md5sumbyIndice( md5sum, evarId, indices, size );
    varId  = evarId;
    consId = NULL;

    sosLength=gLength=0;
    arrangeM=NULL;
    SOSsup=NULL;
    Gsup=NULL;
  }

  Support( Poly_t *poly ) {
    type = POLY;
    subp = poly;
    varId    = poly->getVarId();
    int size=poly->getSize( );
    md5sumbyIndice( md5sum, varId, &(poly->getIndices( )[ 0]), size );

    deg = poly->getTotalDegree();

    constNum = 0;
    consId   = NULL;
    consCap  = 0;

    sosLength=gLength=0;
    arrangeM=NULL;
    SOSsup=NULL;
    Gsup=NULL;
        
  }
  ~Support() {
    if ( consId != NULL ) {
      free( consId );
    }
    if ( SOSsup != NULL ) free( SOSsup );
    if ( Gsup != NULL ) free( Gsup );
    if ( arrangeM != NULL ) {
      deleteSparseA( arrangeM, sosLength );
    }
    
  }
};
} // namespace psd
} // namespace aiSat
#endif
