
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
#include "polytype.h"
#ifdef __cplusplus
extern "C" {
#endif

  struct polyConstraintTable {
    int capacity;
    int last;
    PolyConstraint** values;

  };

  typedef struct polyConstraintTable PolyConstraintTable;

  void initcoefTable(void);

  int addcoefElem(const PolyConstraintType type, const int mId);

  int findcoefElem(const PolyConstraintType type, const int mId);

#ifdef __cplusplus
}
#endif

#endif
