
/**
 * @file   monoconst_table.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:42:02 2015
 *
 * @brief
 *
 *
 */

#ifndef MONOCONST_TABLE_INC
#define MONOCONST_TABLE_INC

#include "config.h"
#include "psdtype.h"

namespace aiSat {

namespace psd {

class MonomialConstTable {
 private:
  vector<MonomialConstraint*> values;

 public:
  int addconsElem(const int deg, const int varId, const int* linCoef,
                  int minValue, const int consFunId);

  int findconsElem(const int deg, const int varId, const int* linCoef,
                   int minValue, const int consFunId);

  MonomialConstraint* getconsElem(const int id);

  void clear(void);
};

extern MonomialConstTable MONO_CONS_TABLE;
}
}

#endif
