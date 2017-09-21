
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

#include "monconstfun_table.h"
#include "polytype.h"
#ifdef __cplusplus
extern "C" {
#endif



  BOOL consEval(indice_t *, const int varId, MonomialConstraint *C);


  /** 
   * @brief
   Input:
   *   (a1,...,an)
   *  if Sum(a1,...,an)==deg return YES
   *  otherwise return false;
   * @param a 
   * @param varId 
   * @param cdeg 
   * @param cvarId 
   * @param clinCoefs 
   * @param cminValue 
   * 
   * @return 
   */
  BOOL homog(indice_t *a, const int varId, const int cdeg, const int cvarId,
             const int *clinCoefs, const int cminValue);


  BOOL Khomog(indice_t *a, const int varId, const int cdeg, const int cvarId,
              const int *clinCoefs, const int cminValue);


  BOOL symm(indice_t *a, const int varId, const int cdeg, const int cvarId,
            const int *clinCoefs, const int cminValue);


  BOOL linCons(indice_t *a, const int varId, const int cdeg, const int cvarId,
               const int *clinCoefs, const int cminValue);
#ifdef __cplusplus
}
#endif

#endif
