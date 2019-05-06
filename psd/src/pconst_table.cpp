/*
 * =====================================================================================
 *
 *       Filename:  coeftable.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/06/2012 09:06:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "pconst_table.h"
#include "config.h"

namespace aiSat {
namespace psd {

PolyConstraintTable POLY_CONS_Table;

int PolyConstraintTable::addcoefElem(const PolyConstraintType type,
                                     const int mId) {
  int loc = findcoefElem(type, mId);
  if (loc > -1) {
    return loc;
  }

  int last = values.size();
  values.push_back(new PolyConstraint());
  values[last]->type = type;
  values[last]->supportId = mId;

  return last;
}

int PolyConstraintTable::findcoefElem(const PolyConstraintType type,
                                      const int mId) {
  int i;
  for (i = 0; i < values.size(); i += 1) {
    if (values[i]->type == type && values[i]->supportId == mId) return i;
  }

  return -1;
}
}
}
