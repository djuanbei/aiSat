
/**
 * @file   mono_const.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:41:44 2015
 *
 * @brief
 *
 *
 */

#ifndef MONO_CONST_INC
#define MONO_CONST_INC

#include "polytype.h"
#include "monconstfun_table.h"
#ifdef __cplusplus
extern "C" {
#endif

/*
 * ===  FUNCTION
 * ======================================================================
 *         Name:  cons
 *  Description:
 * =====================================================================================
 */

BOOL consEval(indice_t *, const int varId, MonomialConstraint *C);

/*
 * ===  FUNCTION
 * ======================================================================
 *         Name:  homog
 *  Description:
 *  Input:
 *   (a1,...,an)
 *  if Sum(a1,...,an)==deg return YES
 *  otherwise return false;
 * =====================================================================================
 */
BOOL homog(indice_t *a, const int varId, const int cdeg, const int cvarId,
           const int *clinCoefs, const int cminValue);

/*
 * ===  FUNCTION
 * ======================================================================
 *         Name:  Khomog
 *  Description:
 * =====================================================================================
 */
BOOL Khomog(indice_t *a, const int varId, const int cdeg, const int cvarId,
            const int *clinCoefs, const int cminValue);

/*
 * ===  FUNCTION
 * ======================================================================
 *         Name:  symm
 *  Description:
 * =====================================================================================
 */
BOOL symm(indice_t *a, const int varId, const int cdeg, const int cvarId,
          const int *clinCoefs, const int cminValue);

/*
 * ===  FUNCTION
 * ======================================================================
 *         Name:  linCons
 *  Description:
 * =====================================================================================
 */
BOOL linCons(indice_t *a, const int varId, const int cdeg, const int cvarId,
             const int *clinCoefs, const int cminValue);
#ifdef __cplusplus
}
#endif

#endif /* ----- #ifndef MONO_CONST_INC  ----- */
