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

enum polyConstraintType { EQ, GE, GT, NE };

typedef enum polyConstraintType PolyConstraintType;



struct polyConstraint {
  PolyConstraintType type;
  int supportId;
};
typedef struct polyConstraint PolyConstraint;

enum supportType { NORMAL, SUB_POLY, INDICE };
typedef enum supportType SupportType;

struct support {
  SupportType type;
  //    BOOL isPoly;

  Subpoly_t *subp;

  uint8_t md5sum[DIGEST_SIZE];

  int deg;
  int varId;
  int *consId;
  int constNum;
  int consCap;
};
typedef struct support Support;

/*
  sum_{i=0}^{size-1} polys[i]*polyConstraints[i]=rhs
*/
struct sosProblem {
  int cap;
  int size;
  Poly_t **polys;
  Poly_t *rhs;
  PolyConstraint **polyConstraints;
};

typedef struct sosProblem SOSProblem;
}
}
#endif
