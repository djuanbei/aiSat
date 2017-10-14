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

#include <cstdint>
#include "poly.hpp"
#include "subpoly.hpp"
#include "util.h"
namespace aiSat {
namespace psd {
using namespace poly;

typedef Poly<coef_t, indice_t> Poly_t;
typedef Subpoly<coef_t, indice_t> Subpoly_t;

/**
*An array of spare two dimension coordinate
*
*/

struct sparseRowCol {
  uint32_t row : 16;
  uint32_t col : 16;
};

typedef struct sparseRowCol SparseRowCol;

enum monomialOrder { LEX, GRLEX, GREVLEX };

typedef enum monomialOrder MonomialOrder;

enum monomialConstraintType { HOMOG, KHOMOG, SYMM };

typedef enum monomialConstraintType monomialConstraintType;

struct MonomialConstraint {
  int id;
  int deg;
  int varId;
  int *linCoefs;
  int minValue; /* only use in linconstraints */
  int consFunId;
  MonomialConstraint() { linCoefs = NULL; }
};

enum PolyConstraintType { EQ, GE, GT, NE };



struct PolyConstraint {
  PolyConstraintType type;
  int supportId;
  PolyConstraint(){
  }
  PolyConstraint(PolyConstraint *other){
    type=other->type;
    supportId=other->supportId;
  }
};


enum SupportType { NORMAL, SUB_POLY, INDICE };


struct Support {
  SupportType type;

  Subpoly_t *subp;

  uint8_t md5sum[DIGEST_SIZE];

  int deg;
  int varId;
  int *consId;
  int constNum;
  int consCap;
  Support(const int evarId, const indice_t *indices, const int size) {
    subp=NULL;
    type = INDICE;
    md5sumbyIndice(md5sum, evarId, indices, size);
    varId = evarId;
    consId = NULL;
  }
  
  Support(Subpoly_t *subpoly) {
    type = SUB_POLY;
    subp = subpoly;

    memcpy(md5sum, subpoly->getmd5(), DIGEST_SIZE);

    deg = subpoly->getTotalDegree();

    varId = subpoly->getParent().getVarId();
    constNum = 0;
    consId = NULL;
    consCap = 0;
  }
  ~ Support(){
    if (consId != NULL) {
      free(consId);
    }
  }
};
}
}
#endif
