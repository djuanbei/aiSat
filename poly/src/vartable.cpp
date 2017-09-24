/*
 * =====================================================================================
 *
 *       Filename:  varmap.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/05/2012 08:13:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "vartable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "selfassert.h"
#include "selfmemutil.h"

namespace aiSat {

namespace poly {

using namespace std;

static bool INIT = false;

void varTable::initvarTable(int k){
  int n = 1.2 * k + 10;

  if (!INIT) {
    INIT = true;
  } else {
    if (variateSymbol.size() < n){
      resetvarTable(n);
    }
  }

  int i;

  variateSymbol.resize(n);


  variateCombine.push_back(0); /* const indices */
  variateCombine.push_back(k);
  


  for (i = 0; i < k; i += 1) {
    variateSymbol[i] = string("aa@\0");
    variateSymbol[i][0] = 'a' + i % 26;
    variateSymbol[i][1] = 'a' + (i / 26) % 26;
    variateSymbol[i][2] = '@' + (i / 26) % 26;

    variateCombine.push_back(i);
  }


  locationIndex.clear();
  locationIndex.push_back(0);
  locationIndex.push_back(1);
  locationIndex.push_back(2);
\
}
  


varTable::varTable() {
  
  int n = DEFAULT_VARTABLE_NUM;

  if (!INIT) {
    INIT = true;
  } else {
    if (variateSymbol.size() < n){
      resetvarTable(n);
    }
    return;
  }


  variateSymbol.resize(n);



  int mapCap = DEFAULT_VARTABLE_MAP_CAP + 2;


  variateCombine.push_back(0); /* const indices */
  variateCombine.push_back(0);
  
  locationIndex.clear();
  locationIndex.push_back( 0);
  locationIndex.push_back( 1);

}

varTable::varTable(const int k) {
  initvarTable(k);
}

bool varTable::addVar(const string &name) {
  int i;
  int varNum= variateSymbol.size();
  for (i = 0; i < varNum; i++) {
    if (name == variateSymbol[i]) {
      return false;
    }
  }
  int varCap=variateSymbol.size();
  if (varNum >=variateSymbol.size()) {
    varCap *= ENLARGE_RAT;
    varCap++;
    variateSymbol.resize(varCap);
  }
  variateSymbol[varNum] = name;
  varNum++;



  int start = varNum + 1;

  int  num=variateCombine.size()-varNum-1;
  variateCombine.push_back(0);

  for (int i = num; i > 0; i--) {
    variateCombine[start + i] = variateCombine[start + i - 1];
  }
  /**
   * right shift one
   * 
   */

  for (i = 2; i < locationIndex.size(); i++) {
    locationIndex[i]++;
  }
  
  variateCombine[1] = varNum;
  variateCombine[varNum + 1] = varNum - 1;


  return TRUE;
}

int varTable::findVarIndex(const string &name) const {
  int i;
  int varNum=variateSymbol.size();
  for (i = 0; i < varNum; i++) {
    if (name == variateSymbol[i]) {
      return i;
    }
  }
  return -1;
}

void varTable::setvarName(const int index, const string &name) {
  ASSERT(index < variateSymbol.size(), "overflow");

  variateSymbol[index] = name;
}

string varTable::getvarName(const int index) {
  ASSERT(index < variateSymbol.size(), "");

  return variateSymbol[index];
}

void varTable::resetvarTable(const int n) {
  
  if (INIT) {
    int i;

    clearvarTable();
    int capacity=variateCombine.size();
    
    if (n > variateSymbol.size()) {
      variateSymbol.resize(n);

    }
    variateCombine.clear();
    variateCombine.push_back(0);/* const indices */
    variateCombine.push_back(n);


    for (i = 0; i < n; i += 1) {
      variateSymbol[i] = string("aa@\0");
      variateSymbol[i][0] = 'a' + i % 26;
      variateSymbol[i][1] = 'a' + (i / 26) % 26;
      variateSymbol[i][2] = '@' + (i / 26) % 26;

      variateCombine.push_back(i);

    }

    locationIndex.clear();
    locationIndex.push_back(0);
    locationIndex.push_back(1);


  } else {
    initvarTable(n);
  }
}

int varTable::findvarElem(const vector<int> &vars, const int n) {
  int i, j, k, step;

  i = 0;
  while (i < locationIndex.size()) {
    k = locationIndex[i];
    step = (int)variateCombine[k];
    if (n == step) {
      for (j = 0; j < n; j += 1) {
        if (variateCombine[i + j + 1] != vars[j]) break;
      }
      if (j == n) return i;
    }
    i++;
  }

  return -1;
}

int varTable::varContain(const int sId, const int bId) {
  int re = 0;
  int i, j;
  vector<int> svars, bvars;

  getvarElem(sId, svars);

  getvarElem(bId, bvars);

  if (svars[0] <= bvars[0]) {
    i = j = 0;
    while (i < (int)svars[0] && j < (int)bvars[0]) {
      if (bvars[j + 1] < svars[i + 1])
        j++;
      else if (bvars[j + 1] == svars[i + 1]) {
        j++;
        i++;
      } else
        return re;
    }
    if (i < (int)svars[0]) return re; /* svars has more variables than bvars */
  }

  return 1;
}

int varTable::addvarElem(const vector<int> &vars, const int n) {
  
  if (0 == n) return 0;
  ASSERT(n <= variateSymbol.size(), "");

  int re;
  int i;

  re = findvarElem(vars, n);
  if (re > -1) return re;
  

  locationIndex.push_back(variateCombine.size());
  variateCombine.push_back(n);
  
  for (i = 0; i < n; i += 1) {
    ASSERT(vars[i] < variateSymbol.size(), "");
    variateCombine.push_back( vars[i]);
  }

  return locationIndex.size() - 1;
}

void varTable::getvarElem(const int id, vector<int> &re) {
  re.clear();
  if (id >= locationIndex.size()) {
    return;
  }
  int num = variateCombine[locationIndex[id]];
  re.push_back(num);
  for (int i = 0; i < num; i++) {
    re.push_back(variateCombine[locationIndex[id]] + i);
  }
}

int varTable::mergeVar(const int id1, const int id2) {
  int i, j, k;
  i = j = 1;
  k = 0;
  if (id1 == id2) return id1;

  vector<int> var1, var2;

  getvarElem(id1, var1);

  getvarElem(id2, var2);
  vector<int> var(var1[0] + var2[0]);

  while (i <= var1[0] && j <= var2[0]) {
    if (var1[i] < var2[j])
      var[k] = var1[i++];
    else if (var1[i] > var2[j])
      var[k] = var2[j++];
    else {
      var[k] = var1[i];
      i++;
      j++;
    }
    k++;
  }
  while (i <= var1[0]) var[k++] = var1[i++];
  while (j <= var2[0]) var[k++] = var2[j++];

  return addvarElem(var, k);
}

void varTable::getconvertMap(const int oId, const int nId, int map[]) {
  if (oId != 0) ASSERT(varContain(oId, nId), "");

  int i, j;
  vector<int> ovars, nvars;
  getvarElem(oId, ovars);
  getvarElem(nId, nvars);

  const int oSize = (int)ovars[0];
  const int nSize = (int)nvars[0];
  if (oId == nId) {
    for (i = 0; i < oSize; i += 1) {
      map[i] = i;
    }
    return;
  }

  i = j = 1;

  while (i <= oSize && j <= nSize) {
    if (ovars[i] < nvars[j]) {
      ASSERT(0, "every variable contain in ovars must aslo contain in novars ");

      map[i - 1] = -1;
      i++;
    }

    else if (ovars[i] > nvars[j])
      j++;

    else {
      map[i - 1] = j - 1;
      i++;
      j++;
    }
  }
  ASSERT(i > oSize, "");
}

int varTable::getvarNum(const int id) {
  if (id >= locationIndex.size()){
    return -1;
  }

  return (int)variateCombine[locationIndex[id]];
}
/**
 *
 * @return  the number of current variable in this system
 */

int varTable::getAllVarNum(void) { return variateSymbol.size(); }

void varTable::deletevarTable(void) {
  variateCombine.clear();
  variateSymbol.clear();
  locationIndex.clear();
}

void varTable::clearvarTable(void) {

  variateSymbol.clear();
  variateCombine.clear();
  locationIndex.clear();
}
}
}
