
/**
 * @file   pconst_table.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:52:04 2015
 *
 * @brief
 *
 *
 */

#ifndef PCONST_TABLE_INC
#define PCONST_TABLE_INC

#include "config.h"
#include "psdtype.h"
namespace aiSat {
namespace psd {

class PolyConstraintTable {
 private:
  vector<PolyConstraint*> values;

 public:
  void initcoefTable(void);

  int addcoefElem(const PolyConstraintType type, const int mId);

  int findcoefElem(const PolyConstraintType type, const int mId);
};

extern PolyConstraintTable POLY_CONS_Table;
}
}

#endif
