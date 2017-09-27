/**
 * @file   vartable.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:55:37 2015
 *
 * @brief variate table
 *
 *
 */

#ifndef VARTABLE_INC
#define VARTABLE_INC

#include <cassert>
#include <string>
#include <vector>

#include "config.h"

#include "selfassert.h"

#include "polytype.h"

namespace aiSat {

namespace poly {
using namespace std;



template <typename T>
class VarTable {
 private:
  vector<string> variateSymbol;

  vector<T> variateCombine;

  vector<int> locationIndex;

  int varElemContain(const int sId, const int bId) {
    int re = 0;
    int i, j;
    vector<T> svars, bvars;

    getVarElem(sId, svars);

    getVarElem(bId, bvars);

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
      if (i < (int)svars[0])
        return re; /* svars has more variables than bvars */
    }

    return 1;
  }

 public:
  VarTable() {
    variateCombine.push_back(0); /* const indices */
    variateCombine.push_back(0);

    locationIndex.push_back(0);
    locationIndex.push_back(1);
  }

  VarTable(const int n) { reset(n); }

  bool addVar(const string &name) {
    int varNum = variateSymbol.size();
    for (int i = 0; i < varNum; i++) {
      if (name == variateSymbol[i]) {
        return false;
      }
    }
    int varCap = variateSymbol.size();
    if (varNum >= variateSymbol.size()) {
      varCap *= ENLARGE_RAT;
      varCap++;
      variateSymbol.resize(varCap);
    }
    variateSymbol[varNum] = name;
    varNum++;

    int start = varNum + 1;

    int num = variateCombine.size() - varNum - 1;
    variateCombine.push_back(0);

    for (int i = num; i > 0; i--) {
      variateCombine[start + i] = variateCombine[start + i - 1];
    }

    /**
     * right shift one
     *
     */

    for (int i = 2; i < locationIndex.size(); i++) {
      locationIndex[i]++;
    }

    variateCombine[1] = varNum;
    variateCombine[varNum + 1] = varNum - 1;

    return true;
  }

  int findVarIndex(const string &name) const {
    int varNum = variateSymbol.size();
    for (int i = 0; i < varNum; i++) {
      if (name == variateSymbol[i]) {
        return i;
      }
    }
    return -1;
  }

  void setVarName(const int index, const string &name) {
    ASSERT(index < variateSymbol.size(), "overflow");

    variateSymbol[index] = name;
  }

  string getVarName(const int index) {
    ASSERT(index < variateSymbol.size(), "");

    return variateSymbol[index];
  }

  int addVarElem(const vector<T> &vars, const int n) {
    if (0 == n) {
      return 0;
    }
    ASSERT(n <= variateSymbol.size(), "");

    int re, i;

    re = findVarElem(vars, n);
    if (re > -1) {
      return re;
    }

    locationIndex.push_back(variateCombine.size());
    variateCombine.push_back(n);

    for (i = 0; i < n; i += 1) {
      ASSERT(vars[i] < variateSymbol.size(), "");
      variateCombine.push_back(vars[i]);
    }

    return locationIndex.size() - 1;
  }

  int findVarElem(const vector<T> &vars, const int n) {
    int i, j, k, step;

    i = 0;
    while (i < locationIndex.size()) {
      k = locationIndex[i];
      step = (int)variateCombine[k];
      if (n == step) {
        for (j = 0; j < n; j += 1) {
          if (variateCombine[i + j + 1] != vars[j]) break;
        }
        if (j == n) {
          return i;
        }
      }
      i++;
    }

    return -1;
  }

  void getVarElem(const int id, vector<T> &re) {
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

  int getVarNum(const int id) {
    if (id >= locationIndex.size()) {
      return -1;
    }

    return (int)variateCombine[locationIndex[id]];
  }

  /**
   *
   * @return  the number of current variable in this system
   */
  int getAllVarNum(void) { return variateSymbol.size(); }

  int mergeVar(const int id1, const int id2) {
    int i, j, k;
    i = j = 1;
    k = 0;
    if (id1 == id2) return id1;

    vector<T> var1, var2;

    getVarElem(id1, var1);

    getVarElem(id2, var2);
    vector<T> var(var1[0] + var2[0]);

    while (i <= var1[0] && j <= var2[0]) {
      if (var1[i] < var2[j]) {
        var[k] = var1[i++];
      } else if (var1[i] > var2[j]) {
        var[k] = var2[j++];
      } else {
        var[k] = var1[i];
        i++;
        j++;
      }
      k++;
    }
    while (i <= var1[0]) {
      var[k++] = var1[i++];
    }
    while (j <= var2[0]) {
      var[k++] = var2[j++];
    }

    return addVarElem(var, k);
  }

  void getConvertMap(const int oId, const int nId, int map[]) {
    if (oId != 0) ASSERT(varElemContain(oId, nId), "");

    int i, j;
    vector<T> ovars, nvars;
    getVarElem(oId, ovars);
    getVarElem(nId, nvars);

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
        ASSERT(0,
               "every variable contain in ovars must aslo contain in novars ");

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

  void reset(const int n) {
    clear();

    variateSymbol.resize(n);

    variateCombine.push_back(0); /* const indices */
    variateCombine.push_back(n);

    for (int i = 0; i < n; i += 1) {
      variateSymbol[i] = string("aa@\0");
      variateSymbol[i][0] = 'a' + i % 26;
      variateSymbol[i][1] = 'a' + (i / 26) % 26;
      variateSymbol[i][2] = '@' + (i / 26) % 26;

      variateCombine.push_back(i);
    }

    locationIndex.push_back(0);
    locationIndex.push_back(1);
  }

  void clear(void) {
    variateSymbol.clear();
    variateCombine.clear();
    locationIndex.clear();
  }
};

template <typename T>
VarTable<T>& getVarTable(){
  static VarTable<T> re;
  return re;
}
}
}
#endif
