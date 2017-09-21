

/**
 * @file   cdnfformula.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:30:42 2015
 *
 * @brief
 *
 *
 */

#ifndef CDNFFORMULA_INC
#define CDNFFORMULA_INC

#include "bitset.h"
typedef PointVector conjunction, disjunction, monomial;

inline monomial *cdnfformula_monomial_M_DNF(BitSet *bv);
inline disjunction *cdnfformula_disjunction_new(uscalar_t length);
inline disjunction *cdnfformula_disjunction_unit(void);
inline disjunction *cdnfformula_disjunction_add(disjunction *, monomial *);
inline void cdnfformula_disjunction_free(disjunction *);
inline disjunction *cdnfformula_conjunction_new(uscalar_t length);
inline conjunction *cdnfformula_conjunction_unit(void);
inline conjunction *cdnfformula_conjunction_add(conjunction *, disjunction *);
inline void cdnfformula_free(conjunction *);
void cdnfformula_print(conjunction *f);
inline boolformula_t *cdnfformula_to_boolformula(conjunction *);
bool cdnfformula_eval_M_DNF(disjunction *m_dnf, BitSet *bv);

#endif 
