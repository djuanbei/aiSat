
/**
 * @file   sparse.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:54:11 2015
 *
 * @brief
 *
 *
 */

#ifndef SPARSE_INC
#define SPARSE_INC

#include "config.h"
#include "psdtype.h"

namespace aiSat {
namespace psd {

/*-----------------------------------------------------------------------------
 *  struct sparse is a special matrix have few same nonzero element
 *-----------------------------------------------------------------------------*/
struct arrangeMatrix {
  coef_t mult; /* coefficient of this matrix */
  int rowLength;
  int size;
  SparseRowCol* data; /* start from 0  all elements of matrix is one or zero
                         and data record the coordinate of element if one*/
  int capacity;
};

typedef struct arrangeMatrix ArrangeMatrix;

ArrangeMatrix* createSparse(const int rowLength);

void printS(ArrangeMatrix* s);

void deleteSparse(ArrangeMatrix* s);

void addSparseElem(ArrangeMatrix* const s, int i, int j);

coef_t getValue(ArrangeMatrix const* s, int i, int j);

void reScale(ArrangeMatrix* const S);

void mult(ArrangeMatrix* const S, const double a);

void print(const ArrangeMatrix* S);
}
}

#endif
