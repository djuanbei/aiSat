/**
 * @file   vartable.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:55:37 2015
 *
 * @brief
 *
 *
 */

#ifndef VARTABLE_INC
#define VARTABLE_INC

#include <assert.h>
#include "config.h"
#include "polytype.h"

#ifdef __cplusplus
extern "C" {
#endif

  struct varTable {
    int varNum;
    char *varName;
    int varCap;

    int capacity;
    int last;
    indice_t *values; /* variable start from 0 */

    int *locMap;
    int mapLast;
    int mapCap;
  };

  typedef struct varTable VarTable;

  void defaultVarTable(void);

  void initvarTable(const int n);

  BOOL addVar(const char *name);

  int findVarIndex(const char *name);

  char *getvarName(const int index);

  void setvarName(const int index, const char *name);

  void resetvarTable(const int n);

  int addvarElem(const indice_t *vars, const int n);

  int findvarElem(const indice_t *vars, const int n);

  indice_t *getvarElem(const int id);

  int getvarNum(const int id);

  int getAllVarNum(void);

  int mergeVar(const int id1, const int id2);

  void getconvertMap(const int oId, const int nId, int map[]);

  void clearvarTable(void);

  void deletevarTable(void);

#ifdef __cplusplus
}
#endif

#endif
