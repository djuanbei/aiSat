/*
 * =====================================================================================
 *
 *       Filename:  constraint.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/04/2012 10:04:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "mono_const.h"
#include <stdarg.h>
#include "vartable.hpp"
#include "psdtype.h"


namespace aiSat {
namespace psd{
  

bool consEval(indice_t *a, const int varId, MonomialConstraint *C) {
  fn_ptr_t fun = getconsFunById(C->consFunId);
  return fun(a, varId, C->deg, C->varId, C->linCoefs, C->minValue);
}

/**
 * @brief
 *
 * @param a
 * @param varId
 * @param cdeg
 * @param cvarId
 * @param clinCoefs
 * @param cminValue
 *
 * @return
 *  if Sum(a1,...,an)==deg return YES
 *  otherwise return false;
 */
bool homog(indice_t *a, const int varId, const int cdeg, const int cvarId,
           const int *clinCoefs, const int cminValue) {
  int i = 0;
  int sum = 0;
  const int size = Poly_t::VAR_TABLE.getVarNum(varId);

  for (i = 0; i < size; i += 1) {
    sum += a[i];
  }
  if (sum == cdeg){
    return true;
  }
  else{
    return false;
  }
}

bool Khomog(indice_t *a, const int varId, const int cdeg, const int cvarId,
            const int *clinCoefs, const int cminValue) {
  int i, j, sum;

  vector<indice_t>  varMap;
  Poly_t::VAR_TABLE.getVarElem(varId, varMap);
  vector<indice_t> KvarMap;
  Poly_t::VAR_TABLE.getVarElem(cvarId, KvarMap);

  const int size = (int)varMap[0];
  const int KvarSize =Poly_t::VAR_TABLE.getVarNum(cvarId);
  sum = 0;
  i = j = 1;
  while (i <= size && j <= KvarSize) {
    if (varMap[i] < KvarMap[j])
      i++;
    else if (varMap[i] > KvarMap[j])
      j++;
    else {
      sum += a[i];
      i++;
      j++;
    }
  }
  if (cdeg == sum)
    return true;
  else
    return false;
}

/**
 *
 * @brief   every variable contain in symvarMap
 *  has same degree
 * @param a
 * @param varId
 * @param cdeg
 * @param cvarId
 * @param clinCoefs
 * @param cminValue
 *
 * @return
 */
bool symm(indice_t *a, const int varId, const int cdeg, const int cvarId,
          const int *clinCoefs, const int cminValue) {
  int i, j, value;
  vector<indice_t> varMap;
  Poly_t::VAR_TABLE.getVarElem(varId, varMap);
  
  vector<indice_t> symvarMap;
  Poly_t::VAR_TABLE.getVarElem(cvarId, symvarMap);

  const int size = (int)varMap[0];
  const int symSize = (int)symvarMap[0];

  value = -1;
  i = j = 1;
  while (i <= size && j <= symSize) {
    if (varMap[i] < symvarMap[j])
      i++;

    else if (varMap[i] > symvarMap[j]) {
      if (-1 == value)
        value = 0;
      else if (0 != value)
        return false;
      j++;

    } else {
      if (-1 == value)
        value = a[i];
      else if (value != a[i]) {
        return false;
      }
      i++;
      j++;
    }
  }
  if (j != symSize + 1) {
    if (-1 != value || 0 != value) return false;
  }
  return true;
}

bool linCons(indice_t *a, const int varId, const int cdeg, const int cvarId,
             const int *clinCoefs, const int cminValue) {
  int i, j;
  int sum = 0;
  vector<indice_t> varMap;
  Poly_t::VAR_TABLE.getVarElem(varId, varMap);
  
  vector<indice_t> linvarMap;
  Poly_t::VAR_TABLE.getVarElem(cvarId, linvarMap);

  const int size = (int)varMap[0];
  const int linSize = (int)linvarMap[0];

  i = j = 1;
  while (i <= size && j <= linSize) {
    if (varMap[i] < linvarMap[j])
      i++;
    else if (varMap[i] > linvarMap[j]) {
      j++;
    } else {
      sum += clinCoefs[j] * a[i];
      i++;
      j++;
    }
  }
  /*-----------------------------------------------------------------------------
   *  sum>= minValue or sum > minValue need further consider.
   *-----------------------------------------------------------------------------*/
  if (sum >= cminValue)
    return true;
  else
    return false;
}

}
}