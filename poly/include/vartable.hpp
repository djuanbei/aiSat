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

#include "config.h"
#include "util.h"

#include "selfassert.h"

#include "polytype.h"

/**
 * macro to string from the name x to string("x")
 *
 *
 */

#define VAR_STR(x) string(QUdi(x))

namespace aiSat {

namespace poly {
using namespace std;

template <typename T>
class VarTable {
 private:
  vector<string> variateSymbol;

  vector<T> variateCombine;

  vector<int> startLocation;


 public:


  int varVecContain(const int sId, const int bId)const {
    int re = 0;
    int i, j;
    vector<T> svars, bvars;

    getVarVec(sId, svars);

    getVarVec(bId, bvars);
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
    int re= variateSymbol.size() ;
    variateSymbol.push_back(vname);

    return  re;
  }

  int addVar(const string *vname) { return addVar(*vname); }

  void setVarName(const int index, const string &name) {
    ASSERT(index < variateSymbol.size(), "overflow");

    variateSymbol[index] = name;
  }


  /**
   * @brief the variables will be sorted  in descending order
   *
   * @param evars
   *
   * @return id of variable set
   */

  int addVarVec(const vector<T> &evars) {
    vector<T> vars(evars);
    sort(vars.begin(), vars.end());

    int n = (int)vars.size();
    if (0 == n) {
      return 0;
    }
    ASSERT(n <= (int)variateSymbol.size(), "");

    int re = findVarVec(vars);
    if (re > -1) {
      return re;
    }

    for (int i = 0; i < n; i += 1) {
      ASSERT(vars[i] < variateSymbol.size(), "");
      variateCombine.push_back(vars[i]);
    }
      
    startLocation.push_back((int)variateCombine.size());
    return ((int)startLocation.size() )- 2;
  }

  int addVarVec(const vector<T> &evars, const int num) {
    vector<T> vars(evars.begin(), evars.begin() + num);
    sort(vars.begin(), vars.end());

    int n = (int)vars.size();
    if (0 == n) {
      return 0;
    }
    ASSERT(n <= (int)variateSymbol.size(), "");

    int re = findVarVec(vars);
    if (re > -1) {
      return re;
    }

    for (int i = 0; i < n; i += 1) {
      ASSERT(vars[i] < (int)variateSymbol.size(), "");
      variateCombine.push_back(vars[i]);
    }
      
    startLocation.push_back((int)variateCombine.size());
    return ((int)startLocation.size()) - 2;
  }

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

    getVarVec(id1, var1);

    getVarVec(id2, var2);

    int var1Len = getVarNum(id1);
    int var2Len = getVarNum(id2);
    std::vector<T> var(var1Len + var2Len);
    typename vector<T>::iterator it = std::set_union(
        var1.begin(), var1.end(), var2.begin(), var2.end(), var.begin());
    var.resize(it - var.begin());
    return addVarVec(var);
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
  
  
  VarTable() {
    startLocation.push_back(0);
    startLocation.push_back(0);
  }
  VarTable(const int n) { reset(n); }
  
  /**
   *
   *
   * @param name the variable name
   *
   * @return nonnegative integer if --name-- in the table, otherwise, -1
   */

  int findVarIndex(const string &name) const {
    int varNum = (int)variateSymbol.size();
    for (int i = 0; i < varNum; i++) {
      if (name == variateSymbol[i]) {
        return i;
      }
    }
    return -1;
  }

  int findVarIndex(const string *name) const { return findVarIndex(*name); }



  string getVarName(const int index) const {
    ASSERT(index < variateSymbol.size(), "");

    return variateSymbol[index];
  }



  /**
   *
   *
   * @param evars  the variables will be sorted  in descending order
   *
   * @return  nonnegative integer if evars in the table
   *, otherwise, -1
   */

  int findVarVec(const vector<T> &evars) const{
    if (evars.empty()) {
      return 0;
    }

    vector<T> vars(evars);
    sort(vars.begin(), vars.end());

    int i, j, k, len;
    int n = (int)vars.size();
    i = 1;
    while (i + 1 < (int)startLocation.size()) {
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

  void getVarVec(const int id, vector<T> &re) const{
    re.clear();
    if (id + 1 >= (int)startLocation.size()) {
      return;
    }
    int num = startLocation[id + 1] - startLocation[id];
    int start = startLocation[id];
    for (int i = 0; i < num; i++) {
      re.push_back(variateCombine[start + i]);
    }
  }

  int getVarNum(const int id) const{
    if (id + 1 >= (int)startLocation.size()) {
      return -1;
    }
    return startLocation[id + 1] - startLocation[id];
  }

  /**
   *
   * @return  the number of current variable in this system
   */
  int getAllVarNum(void)const { return (int) variateSymbol.size(); }



  /**
   *
   *
   * @param oldId  a variable set id
   * @param newId  a variable set id which contain oldId
   * @param after return  mapIndex  oldVar[i]==newVar[mapIndex[i]] invariant
   * hold
   */
  void getConvertMap(const int oldId, const int newId, vector<int> &mapIndex) const{
    if (oldId != 0) {
      ASSERT(varVecContain(oldId, newId), "");
    }

    int i, j;
    vector<T> ovars, nvars;
    getVarVec(oldId, ovars);
    getVarVec(newId, nvars);

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

  
  template<typename T1>
  friend VarTable<T1> &getVarTable();

};

template <typename T>
VarTable<T> &getVarTable() {
  static VarTable<T> re;
  return re;
}

}
}
#endif
