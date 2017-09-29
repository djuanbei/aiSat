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

  vector<int> startLocation;

  int varElemContain(const int sId, const int bId) {
    int re = 0;
    int i, j;
    vector<T> svars, bvars;

    getVarElem(sId, svars);

    getVarElem(bId, bvars);
    int sNum=getVarNum(sId);
    int bNum=getVarNum(bId);

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
      if (i < sNum)
        return re; /* svars has more variables than bvars */
    }

    return 1;
  }

 public:
  VarTable() {
    startLocation.push_back(0);
    startLocation.push_back(0);
  }

  VarTable(const int n) { reset(n); }

  
  bool addVar(const string &name) {
    
    int varNum = variateSymbol.size();
    for (int i = 0; i < varNum; i++) {
      if (name == variateSymbol[i]) {
        return false;
      }
    }

    variateSymbol.push_back( name);
    

    int start = varNum;

    int num = variateCombine.size() - varNum;
    variateCombine.push_back(0);

    for (int i = num; i > 0; i--) {
      variateCombine[start + i+1] = variateCombine[start + i];
    }

    /**
     * right shift one
     *
     */

    for (int i = 1; i < startLocation.size(); i++) {
      startLocation[i]++;
    }
    

    variateCombine[1] = varNum;

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

  int addVarElem(const vector<T> &vars) {
    int n =vars.size();
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

  int findVarElem(const vector<T> &vars) {
    int i, j, k, len;
    int n=vars.size();
    i = 0;
    while (i+1 < startLocation.size()) {
      
      k = startLocation[i];

      len=startLocation[i+1]-startLocation[i];
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

  void getVarElem(const int id, vector<T> &re) {
    re.clear();
    if (id+1 >= startLocation.size()) {
      return;
    }
    int num = startLocation[id+1]-startLocation[id];
    int start=startLocation[id];
    for (int i = 0; i < num; i++) {
      re.push_back(variateCombine[start+i]);
    }
  }

  int getVarNum(const int id) {
    if (id+1 >= startLocation.size()) {
      return -1;
    }
    return startLocation[id+1]-startLocation[id];
  }

  /**
   *
   * @return  the number of current variable in this system
   */
  int getAllVarNum(void) { return variateSymbol.size(); }

  int mergeVar(const int id1, const int id2) {
    int i, j, k;
    i = j = 0;
    k = 0;
    if (id1 == id2) return id1;

    vector<T> var1, var2;

    getVarElem(id1, var1);

    getVarElem(id2, var2);
    int var1Len=getVarNum(id1);
    int var2Len=getVarNum(id2);
    
    
    vector<T> var(var1Len + var2Len);

    while (i < var1Len && j < var2Len) {
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
    while (i <= var1Len) {
      var[k++] = var1[i++];
    }
    while (j <= var2Len) {
      var[k++] = var2[j++];
    }

    return addVarElem(var);
  }

  void getConvertMap(const int oId, const int nId, int mapIndex[]) {
    if (oId != 0) ASSERT(varElemContain(oId, nId), "");

    int i, j;
    vector<T> ovars, nvars;
    getVarElem(oId, ovars);
    getVarElem(nId, nvars);

    const int oSize = getVarNum(oId);
    const int nSize =getVarNum(nId);
    if (oId == nId) {
      for (i = 0; i < oSize; i += 1) {
        mapIndex[i] = i;
      }
      return;
    }

    i = j = 0;

    while (i < oSize && j < nSize) {
      if (ovars[i] < nvars[j]) {
        ASSERT(0,
               "every variable contain in ovars must aslo contain in novars ");

        mapIndex[i] = -1;
        i++;
      }

      else if (ovars[i] > nvars[j])
        j++;

      else {
        mapIndex[i] = j;
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

    startLocation.push_back(0);
    startLocation.push_back(1);
  }

  void clear(void) {
    variateSymbol.clear();
    variateCombine.clear();
    startLocation.clear();
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
