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
#include "psdtype.h"
#include "vartable.hpp"
#include <stdarg.h>

namespace aiSat {
namespace psd {

bool consEval( indice_t *a, const int varId, MonomialConstraint *C ) {
  fn_ptr_t fun = getconsFunById( C->consFunId );
  return fun( a, varId, C->deg, C->varId, C->linCoefs, C->minValue );
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
bool homog( indice_t *a, const int varId, const int cdeg, const int cvarId,
            const int *clinCoefs, const int cminValue ) {
  int       i    = 0;
  int       sum  = 0;
  const int size = getVarTable<indice_t>().getVarNum( varId );

  for ( i = 0; i < size; i += 1 ) {
    sum += a[ i ];
  }
  if ( sum == cdeg ) {
    return true;
  } else {
    return false;
  }
}

bool Khomog( indice_t *a, const int varId, const int cdeg, const int cvarId,
             const int *clinCoefs, const int cminValue ) {
  int i, j, sum;

  vector<indice_t>    varMap;
  VarTable<indice_t> &vartable = getVarTable<indice_t>();

  vartable.getVarVec( varId, varMap );
  vector<indice_t> KvarMap;
  vartable.getVarVec( cvarId, KvarMap );

  const int size = vartable.getVarNum( varId );

  const int KvarSize = vartable.getVarNum( cvarId );
  sum                = 0;
  i = j = 0;
  while ( i < size && j < KvarSize ) {
    if ( varMap[ i ] < KvarMap[ j ] )
      i++;
    else if ( varMap[ i ] > KvarMap[ j ] )
      j++;
    else {
      sum += a[ i ];
      i++;
      j++;
    }
  }
  if ( cdeg == sum )
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
bool symm( indice_t *a, const int varId, const int cdeg, const int cvarId,
           const int *clinCoefs, const int cminValue ) {
  int                 i, j, value;
  vector<indice_t>    varMap;
  VarTable<indice_t> &vartable = getVarTable<indice_t>();

  vartable.getVarVec( varId, varMap );

  vector<indice_t> symvarMap;
  vartable.getVarVec( cvarId, symvarMap );

  const int size    = vartable.getVarNum( varId );
  const int symSize = vartable.getVarNum( cvarId );

  value = -1;
  i = j = 0;
  while ( i < size && j < symSize ) {
    if ( varMap[ i ] < symvarMap[ j ] ) {
      i++;
    } else if ( varMap[ i ] > symvarMap[ j ] ) {
      if ( -1 == value ) {
        value = 0;
      } else if ( 0 != value ) {
        return false;
      }
      j++;

    } else {
      if ( -1 == value ) {
        value = a[ i ];
      } else if ( value != a[ i ] ) {
        return false;
      }
      i++;
      j++;
    }
  }
  if ( j != symSize + 1 ) {
    if ( -1 != value || 0 != value ) {
      return false;
    }
  }
  return true;
}

bool linCons( indice_t *a, const int varId, const int cdeg, const int cvarId,
              const int *clinCoefs, const int cminValue ) {
  int                 i, j;
  int                 sum = 0;
  vector<indice_t>    varMap;
  VarTable<indice_t> &vartable = getVarTable<indice_t>();

  vartable.getVarVec( varId, varMap );

  vector<indice_t> linvarMap;
  vartable.getVarVec( cvarId, linvarMap );

  const int size    = vartable.getVarNum( varId );
  const int linSize = vartable.getVarNum( cvarId );

  i = j = 0;
  while ( i < size && j < linSize ) {
    if ( varMap[ i ] < linvarMap[ j ] )
      i++;
    else if ( varMap[ i ] > linvarMap[ j ] ) {
      j++;
    } else {
      sum += clinCoefs[ j ] * a[ i ];
      i++;
      j++;
    }
  }
  /*-----------------------------------------------------------------------------
   *  sum>= minValue or sum > minValue need further consider.
   *-----------------------------------------------------------------------------*/
  if ( sum >= cminValue ) {
    return true;
  } else {
    return false;
  }
}
} // namespace psd
} // namespace aiSat
