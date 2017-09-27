#ifndef __PSD_UTIL_H
#define __PSD_UTIL_H

#include <psdtype.h>

#include "blockmat.h"
#include "sdpsolver.h"
#include "sparse.h"

namespace aiSat {

namespace psd {

bool criteria(const Support *coefM, indice_t *key);

ArrangeMatrix **createArrangeM(const int coefMonomiaId, indice_t const *SOSM,
                               int *const blockSize, int const lengthM);

void deleteW(ArrangeMatrix **s[], const int size, const int len[]);

void deleteSparseA(ArrangeMatrix **s, const int len);

Poly_t *sosConvertPoly(Blockmatrix *const X, const int blockId,
                       const int blockSize, const int sosmId);
}
}

#endif