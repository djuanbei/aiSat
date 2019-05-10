#ifndef __PSD_UTIL_H
#define __PSD_UTIL_H

#include <psdtype.h>

#include "blockmat.h"
#include "sosproblem.h"
#include "sparse.h"

namespace aiSat {

namespace psd {

bool criteria( const Support *coefM, indice_t *key );

/**
 * @brief  n (n>=1) is the number of variables of polynomial and d is the
 * degree of polynomial
 * enumerate all monomials whos total degree less or equal than d.
 * Useing Lexicographic Order x1>x2>..>xn
 *
 * @param monomialsId the id of this indices  corresponding varMap
 * @param length
 *
 * @return
 */

indice_t *getAllMonByTd( const int supportId, int *const length );

void getAllMonHomoTd( const int varNum, const int deg, const int length,
                      indice_t *indices );

ArrangeMatrix **createArrangeM( const int coefMonomiaId, indice_t const *SOSM,
                                int *const blockSize, int const lengthM );

void deleteW( ArrangeMatrix **s[], const int size, const int len[] );

void deleteSparseA( ArrangeMatrix **s, const int len );

Poly_t sosConvertPoly( Blockmatrix *const X, const int blockId,
                       const int blockSize, const int sosmId );

} // namespace psd
} // namespace aiSat

#endif
