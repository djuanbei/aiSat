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

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

#include "util.h"
#include "config.h"

#include "selfassert.h"

#include "polytype.h"

/**
 * macro to string from the name x to string("x")
 *
 * 
 */

#define VAR(x) string(QUdi(x))

namespace aiSat {

namespace poly {
using namespace std;

template <typename T>
class VarTable {
 private:
  vector<string> variateSymbol;

  vector<T> variateCombine;

  vector<int> startLocation;

  int varElemContain(const int sId, const int bId) {
    int re = 0;
    int i, j;
    vector<T> svars, bvars;

    getVarElem(sId, svars);

    getVarElem(bId, bvars);
    int sNum = getVarNum(sId);
    int bNum = getVarNum(bId);

    if (sNum <= bNum) {
      i = j = 0;
      while (i < sNum && j < bNum) {
        if (bvars[j] < svars[i])
          j++;
        else if (bvars[j] == svars[i]) {
          j++;
          i++;
        } else
          return re;
      }
      if (i < sNum) return re; /* svars has more variables than bvars */
    }

    return 1;
  }

 public:
  VarTable() {
    startLocation.push_back(0);
    startLocation.push_back(0);
  }

  VarTable(const int n) { reset(n); }

  /**
   *
   * @param vname  new variable name
   *
   * @return the id of vname, if there are no vname in the table then
   * vname will append in the table
   */
  
  int addVar(const string &vname) {
    int varNum = variateSymbol.size();

    for (int i = 0; i < varNum; i++) {
      if (vname == variateSymbol[i]) {
        return i;
      }
    }

    variateSymbol.push_back(vname);

    return variateSymbol.size()-1;
  }

  /**
   *
   *
   * @param name the variable name
   *
   * @return nonnegative integer if --name-- in the table, otherwise, -1
   */

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

  /**
   * @brief the variables will be sorted  in descending order
   *
   * @param evars
   *
   * @return id of variable set
   */

  int addVarElem(const vector<T> &evars) {
    vector<T> vars(evars);
    sort(vars.begin(), vars.end());

    int n = vars.size();
    if (0 == n) {
      return 0;
    }
    ASSERT(n <= variateSymbol.size(), "");

    int re = findVarElem(vars);
    if (re > -1) {
      return re;
    }

    for (int i = 0; i < n; i += 1) {
      ASSERT(vars[i] < variateSymbol.size(), "");
      variateCombine.push_back(vars[i]);
    }
    startLocation.push_back(variateCombine.size());
    return startLocation.size() - 2;
  }

  /**
   *
   *
   * @param evars  the variables will be sorted  in descending order
   *
   * @return  nonnegative integer if evars in the table
   *, otherwise, -1
   */

  int findVarElem(const vector<T> &evars) {
    if (evars.empty()) {
      return 0;
    }

    vector<T> vars(evars);
    sort(vars.begin(), vars.end());

    int i, j, k, len;
    int n = vars.size();
    i = 1;
    while (i + 1 < startLocation.size()) {
      k = startLocation[i];

      len = startLocation[i + 1] - startLocation[i];
      if (n == len) {
        for (j = 0; j < n; j += 1) {
          if (variateCombine[k + j] != vars[j]) break;
        }
        if (j == n) {
          return i;
        }
      }
      i++;
    }

    return -1;
  }

  /**
   * @brief obtain varset id corresponding vars
   *
   * @param id
   * @param re
   */

  void getVarElem(const int id, vector<T> &re) {
    re.clear();
    if (id + 1 >= startLocation.size()) {
      return;
    }
    int num = startLocation[id + 1] - startLocation[id];
    int start = startLocation[id];
    for (int i = 0; i < num; i++) {
      re.push_back(variateCombine[start + i]);
    }
  }

  int getVarNum(const int id) {
    if (id + 1 >= startLocation.size()) {
      return -1;
    }
    return startLocation[id + 1] - startLocation[id];
  }

  /**
   *
   * @return  the number of current variable in this system
   */
  int getAllVarNum(void) { return variateSymbol.size(); }

  /**
   * @brief merge the two variable sets into one variable set
   *
   * @param id1
   * @param id2
   *
   * @return new variable set id
   */
  int mergeVar(const int id1, const int id2) {
    int i, j, k;
    i = j = k = 0;
    if (id1 == id2) {
      return id1;
    }

    vector<T> var1, var2;

    getVarElem(id1, var1);

    getVarElem(id2, var2);

    int var1Len = getVarNum(id1);
    int var2Len = getVarNum(id2);
    std::vector<T> var(var1Len + var2Len);
    typename vector<T>::iterator it = std::set_union(
        var1.begin(), var1.end(), var2.begin(), var2.end(), var.begin());
    var.resize(it - var.begin());
    return addVarElem(var);
  }

  /**
   *
   *
   * @param oldId  a variable set id
   * @param newId  a variable set id which contain oldId
   * @param after return  mapIndex  oldVar[i]==newVar[mapIndex[i]] invariant
   * hold
   */
  void getConvertMap(const int oldId, const int newId, vector<int> &mapIndex) {
    if (oldId != 0) ASSERT(varElemContain(oldId, newId), "");

    int i, j;
    vector<T> ovars, nvars;
    getVarElem(oldId, ovars);
    getVarElem(newId, nvars);

    const int oSize = getVarNum(oldId);
    const int nSize = getVarNum(newId);
    mapIndex.resize(oSize);
    if (oldId == newId) {
      for (i = 0; i < oSize; i += 1) {
        mapIndex[i] = i;
      }
      return;
    }

    i = j = 0;

    while (i < oSize && j < nSize) {
      if (ovars[i] < nvars[j]) {
        ASSERT(false,
               "every variable contain in ovars must aslo contain in oldvars ");

        mapIndex[i] = -1;
        i++;
      }

      else if (ovars[i] > nvars[j]) {
        j++;
      }

      else {
        mapIndex[i] = j;
        i++;
        j++;
      }
    }
    ASSERT(i >= oSize, "");
  }

  /**
   * @brief reset n variables table and clear original table
   *
   * @param n  number of variable
   */

  void reset(const int n) {
    clear();

    variateSymbol.resize(n);

    for (int i = 0; i < n; i += 1) {
      variateSymbol[i] = string("aa@\0");
      variateSymbol[i][0] = 'a' + i % 26;
      variateSymbol[i][1] = 'a' + (i / 26) % 26;
      variateSymbol[i][2] = '@' + (i / 26) % 26;
    }
  }

  void clear(void) {
    variateSymbol.clear();
    variateCombine.clear();
    startLocation.clear();
    startLocation.push_back(0);
    startLocation.push_back(0);
  }
};

template <typename T>
VarTable<T> &getVarTable() {
  static VarTable<T> re;
  return re;
}
}
}
#endif
