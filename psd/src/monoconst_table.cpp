/*
 * =====================================================================================
 *
 *       Filename:  table.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/07/2012 04:58:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "monoconst_table.h"
#include "mono_const.h"
#include "psdtype.h"
#include "selfmemutil.h"

namespace aiSat{
namespace psd{

MonomialConstTable MONO_CONS_TABLE;



int MonomialConstTable::addconsElem(const int deg, const int varId, const int* linCoef,
                int minValue, const int consFunId) {
  int re = findconsElem(deg, varId, linCoef, minValue, consFunId);
  if (re > -1){
    return re;
  }

  int last=values.size();
  
  values.push_back(new MonomialConstraint());
  values[last]->deg = deg;
  values[last]->varId = varId;
  values[last]->minValue = minValue;
  values[last]->consFunId = consFunId;
  values[last]->id = re;

  return re;
}

int  MonomialConstTable::findconsElem(const int deg, const int varId, const int* linCoef,
                 int minValue, const int consFunId) {
  int i, j, num;
  int last=values.size();
  for (i = 0; i < last; i += 1) {
    if (deg == values[i]->deg && varId == values[i]->varId &&
        minValue == values[i]->minValue &&
        consFunId == values[i]->consFunId) {
      if (linCoef == NULL){
        return i;
      }
      num = Poly_t::VAR_TABLE.getVarNum(varId);

      for (j = 0; j < num; j += 1) {
        if (linCoef[j] != values[i]->linCoefs[j]){
          break;
        }
      }
      if (j == num){
        return i;
      }
    }
  }
  return -1;
}

MonomialConstraint* MonomialConstTable::getconsElem(const int id) {
  if (id >= values.size() ){
    return NULL;
  }
  return values[id];
}


void MonomialConstTable::clear(void) {
  int i;
  for (i = 0; i < values.size(); i += 1) {
    delete values[i];

  }
  values.clear();

}


}
}
