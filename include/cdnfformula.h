/*
 * =====================================================================================
 *
 *       Filename:  cdnfformula.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/30/2013 01:57:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  CDNFFORMULA_INC
#define  CDNFFORMULA_INC

#include	"bitset.h"
typedef PointVector  conjunction, disjunction, monomial;

inline monomial *cdnfformula_monomial_M_DNF (BitSet *bv);
inline disjunction *cdnfformula_disjunction_new (uscalar_t length);
inline disjunction *cdnfformula_disjunction_unit (void);
inline disjunction *cdnfformula_disjunction_add (disjunction *, monomial *);
inline void cdnfformula_disjunction_free (disjunction *);
inline disjunction *cdnfformula_conjunction_new (uscalar_t length);
inline conjunction *cdnfformula_conjunction_unit (void);
inline conjunction *cdnfformula_conjunction_add (conjunction *, disjunction *);
inline void cdnfformula_free (conjunction *);
void cdnfformula_print (conjunction *f);
inline boolformula_t *cdnfformula_to_boolformula (conjunction *);
bool cdnfformula_eval_M_DNF (disjunction *m_dnf, BitSet *bv);

#endif   /* ----- #ifndef CDNFFORMULA_INC  ----- */
