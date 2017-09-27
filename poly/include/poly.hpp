/**
 * @file   Poly.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Sep 21 22:01:34 2017
 *
 * @brief  Poly template class
 *
 *
 */
#ifndef __POLY_H
#define __POLY_H

#include <iostream>
#include <string>
#include <vector>

#include <sstream>

#include <cmath>
#include <iomanip>

#include "vartable.hpp"

namespace aiSat {

namespace poly {
static size_t POLY_ID = 0;

using namespace std;
template <typename C = double, typename T = int>
class Poly {
 public:
  struct term {
    vector<T> key;
    C cf;
    term() {}
    term(const vector<T> &k, const C &c) : key(k), cf(c) {}
  };

  typedef Poly<C, T> poly_t;

  typedef Poly<C, T>::term term_t;

  static VarTable<T> VAR_TABLE;

 private:
  size_t id;
  int varId;  /// the variables id of this polynomial
  int varNum;
  vector<T> indices;
  vector<C> coef;

  void add_term_with_sort(const vector<T> &key, const C cf) {
    int i, j;
    int length = coef.size();
    if (coef.empty()) {
      return add_term(key, cf);
    }
    int location = findIndex(key);
    if (location > -1) {
      coef[location] += cf;
    } else {
      location = findLocationBettwen(key);
      coef.push_back(0);
      for (j = 0; j < varNum; j++) {
        indices.push_back(0);
      }
      for (i = length; i > location; i--) {
        for (j = 0; j < varNum; j++) {
          indices[i * varNum + j] = indices[(i - 1) * varNum + j];
        }
        coef[i] = coef[i - 1];
      }
      for (j = 0; j < varNum; j++) {
        indices[location * varNum + j] = key[j];
      }
      coef[location] = cf;
    }
  }

  int compare(const int k1, const int k2) const {
    int i = 0;
    for (i = 0; i < varNum; i++) {
      if (indices[k1 + i] > indices[k2 + i]) {
        return 1;
      } else if (indices[k1 + i] < indices[k2 + i]) {
        return -1;
      }
    }
    return 0;
  }

  int compare(const vector<T> &key, int loc) const {
    for (int i = 0; i < varNum; i++) {
      if (key[i] > indices[loc + i]) {
        return 1;
      } else if (key[i] < indices[loc + i]) {
        return -1;
      }
    }
    return 0;
  }
  /**
   * @param key an element which been found in map
   * @param map an ascending array
   * @param length the length of map
   * @param n the size of key
   *
   * @return  loc if loc in [ 0, length ) if map[ loc ]==key
   * otherwise -1
   */
  int findIndex(const vector<T> &key) const {
    if (coef.empty()) {
      return -1;
    }
    int length = coef.size();

    int low = 0;
    int high = length - 1;
    int mid, comp;
    while (high >= low) {
      mid = (high + low) / 2;
      if ((comp = compare(key, mid * varNum)) < 0) {
        high = mid - 1;
      } else if (comp > 0) {
        low = mid + 1;
      } else {
        return mid;
      }
    }
    return -1;
  }


  /**
   * bi-search method
   *
   * @param key the element need find
   *
   * @return i if   indices[i-1]<=key< indices[i] or  indices[i]=key
   */
  int findLocationBettwen(const vector<T> &key) const {
    int length = coef.size();
    int low = 0;
    int high = length - 1;
    int mid;

    while (high > low) {
      mid = (high + low) / 2;

      if (compare(key, mid * varNum) > 0) {
        low = mid + 1;
      } else {
        high = mid - 1;
      }
    }

    if (compare(key, low * varNum) < 0) {
      return low;
    } else {
      return low + 1;
    }
  }

  void swapT(const int i, const int j) {
    int k;
    int iindex = i * varNum;
    int jindex = j * varNum;
    T temp;

    for (k = 0; k < varNum; k += 1) {
      temp = indices[iindex + k];
      indices[iindex + k] = indices[jindex + k];
      indices[jindex + k] = temp;
    }
    C ctemp = coef[i];
    coef[i] = coef[j];
    coef[j] = ctemp;
  }
  /**
   * @brief  sort arrary vars by ascending order
   *
   * @param left
   * @param right
   */
  void qsort(const int left, const int right) {
    int i, last;
    if (left >= right) {
      return;
    }
    swapT(left, (left + right) / 2);

    last = left;
    for (i = left + 1; i <= right; i += 1) {
      if (compare(i * varNum, left * varNum) < 0) {
        swapT(++last, i);
      }
    }

    swapT(left, last);
    qsort(left, last - 1);
    qsort(last + 1, right);
  }

 public:
  Poly() {
    varId = 0;
    varNum = 0;
    id = POLY_ID++;
  }
  Poly(const int vid) {
    varId = vid;
    varNum = VAR_TABLE.getVarNum(varId);
    id = POLY_ID++;
  }

  Poly(const int evarId, const C coefDomain, const int termLength,
       const int indiceDomain) {
    id = POLY_ID++;
    varId = evarId;
    varNum = VAR_TABLE.getVarNum(varId);
    C cf;
    vector<T> key(varNum, 0);
    int cfD = (int)coefDomain;

    for (int i = 0; i < termLength; i++) {
      cf = rand() % (2 * cfD) - cfD;
      for (int j = 0; j < varNum; j++) {
        key[j] = rand() % indiceDomain;
      }
      add_term(key, cf);
    }
    update();
  }

  Poly(const poly_t &other) {
    id = POLY_ID++;
    varId = other.varId;
    varNum = other.varNum;
    indices = other.indices;
    coef = other.coef;
    update();
  }

  int getSize() const { return coef.size(); }

  int getVarId() const { return varId; }
  int getVarNum() const { return varNum; }


  /** 
   * @brief get the corresponding degree of k-th term i variate degree
   * 
   * @param k term index
   * @param i  variate index
   * 
   * @return 
   */
  int getDegreeAt(int k, int i) const{

    return indices[k*varNum+i];
  }
  
  void getDegreeAt(int k, vector<T> & re) const{
    re.resize(varNum);
    for(int i=0; i< varNum; i++){
      re[i]=indices[k*varNum+i];
    }
  }

  C getCF(const int i) const{
    return coef[i];
  }
  
  /**
   * @brief  combine the terms whoes indices are same
   *
   */
  void update(void) {
    int i, j, k;
    if (coef.empty()) {
      return;
    }
    qsort(0, coef.size() - 1);

    j = 0;
    int size = coef.size();
    for (i = 0; i < size; i++) {
      for (k = 0; k < varNum; k++) {
        if (indices[j * varNum + k] != indices[i * varNum + k]) {
          break;
        }
      }
      if (varNum == k) {
        coef[j] += coef[i];
      } else if (i == j + 1) {
        j++;
      } else {
        j++;
        for (k = 0; k < varNum; k++) {
          indices[j * varNum + k] = indices[i * varNum + k];
        }
        coef[j] = coef[i];
      }
    }

    if (j + 1 < coef.size()) {
      indices.resize((j + 1) * varNum);
      coef.resize(j + 1);
    }
  }

  void getSubpoly(const vector<int> &locs, poly_t &re) const {
    re.id = POLY_ID++;
    re.varId = varId;
    re.varNum = varNum;
    re.indices.clear();
    re.coef.clear();
    int size = locs.size();
    for (int i = 0; i < size; i++) {
      int loc = locs[i];
      re.coef.push_back(coef[loc]);
      int k = loc * varNum;
      for (int j = 0; j < varNum; j++) {
        re.indices.push_back(indices[k + j]);
      }
    }
    re.update();
  }

  string toString() {
    update();
    return toString(MIN_COEF, 4);
  }

  string toString(const float minValue, const int prec) const {
    int i, j;
    bool isFirst = true;
    stringstream str;
    vector<T> vars;

    VAR_TABLE.getVarElem(varId, vars);

    vector<string> varname(varNum);

    if (0 == coef.size()) {
      return " 0 \n";
    }

    for (i = 0; i < varNum; i++) {
      varname[i] = VAR_TABLE.getVarName(vars[i + 1]);
    }

    size_t size = coef.size();

    for (i = 0; i < size; i += 1) {
      if (coef[i] > minValue || coef[i] < -minValue) {
        if (i == 0) {
          str << std::setprecision(prec) << coef[i];

        } else if (coef[i] > 0.0) {
          if (!isFirst) {
            str << "+" << std::setprecision(prec) << coef[i];
          }

        } else if (coef[i] < 0.0) {
          str << std::setprecision(prec) << coef[i];
        }

        for (j = 0; j < varNum; j += 1) {
          if (indices[i * varNum + j] > 0) {
            if (indices[i * varNum + j] > 1) {
              str << "*" << varname[j] << "^" << indices[i * varNum + j];
            }

            else if (1 == indices[i * varNum + j]) {
              str << "*" << varname[j];
            }
          }
        }
        isFirst = false;
      }
    }

    if (isFirst) {
      return " 0 \n";
    }

    return str.str();
  }

  string toString(const vector<int> &locs) const {
    int i, j;
    bool isFirst = true;
    stringstream str;
    C minValue = MIN_COEF;
    int prec = 6;
    vector<int> vars;
    VAR_TABLE.getVarElem(varId, vars);

    vector<string> varname(varNum);

    if (0 == coef.size() || locs.empty()) {
      return " 0 \n";
    }

    for (i = 0; i < varNum; i++) {
      varname[i] = VAR_TABLE.getVarName(vars[i + 1]);
    }

    size_t size = locs.size();

    for (i = 0; i < size; i += 1) {
      int loc = locs[i];
      if (coef[loc] > minValue || coef[loc] < -minValue) {
        if (i == 0) {
          str << std::setprecision(prec) << coef[loc];

        } else if (coef[loc] > 0.0) {
          if (!isFirst) {
            str << "+" << std::setprecision(prec) << coef[loc];
          }

        } else if (coef[loc] < 0.0) {
          str << std::setprecision(prec) << coef[loc];
        }

        for (j = 0; j < varNum; j += 1) {
          if (indices[loc * varNum + j] > 0) {
            if (indices[loc * varNum + j] > 1) {
              str << "*" << varname[j] << "^" << indices[loc * varNum + j];
            }

            else if (1 == indices[loc * varNum + j]) {
              str << "*" << varname[j];
            }
          }
        }
        isFirst = false;
      }
    }

    if (isFirst) {
      return " 0 \n";
    }

    return str.str();
  }

  string toString1() const { return toString(MIN_COEF, 4); }
  void resetZero() {
    indices.clear();
    coef.clear();
  }

  void resetOne() {
    coef.clear();
    indices.clear();
    coef.push_back(1);
    for (int i = 0; i < varNum; i++) {
      indices.push_back(0);
    }
  }

  bool isZero() {
    update();
    if (0 == coef.size()) {
      return true;
    }

    if (1 == coef.size() && 0 == coef[0]) {
      return true;
    }

    return false;
  }

  bool isOne() {
    update();
    if (1 != coef.size()) {
      return false;
    }
    if (1 == coef[0]) {
      for (int i = 0; i < varNum; i++) {
        if (0 != indices[i]) {
          return false;
        }
      }
      return true;
    }
    return false;
  }

  bool isConstant() {
    update();
    if (1 != coef.size()) {
      return false;
    }
    for (int i = 0; i < varNum; i++) {
      if (0 != indices[i]) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief p>=0
   *
   *
   * @return
   */
  bool isPositive() {
    if (isZero()) {
      return true;
    }
    if (coef[0] >= 0 && isConstant()) {
      return true;
    }
    return false;
  }
  /**
   * @brief check whether p <=0
   *
   *
   * @return
   */
  bool isNegative() {
    if (isZero()) {
      return true;
    }
    if (coef[0] <= 0 && isConstant()) {
      return true;
    }
    return false;
  }

  int getTotalDegree() const {
    int re = 0;
    int temp, i, j;

    int size = coef.size();
    for (i = 0; i < size; i += 1) {
      temp = 0;

      for (j = 0; j < varNum; j += 1) {
        temp += indices[i * varNum + j];
      }
      if (temp > re) re = temp;
    }
    return re;
  }

  /**
   * @brief enlarge or reduce Poly's vars
   *Notice keep capacity and size
   * if reduce I do not deal with the indice who variable is not exist
   *
   * @param vid
   */
  void changeVarId(const int vid) {
    int i, j;
    if (varId == vid) {
      return;
    }
    int length = coef.size();
    const int oldSize = varNum;
    const int newSize = VAR_TABLE.getVarNum(vid);

    int map[oldSize];
    VAR_TABLE.getConvertMap(varId, vid, map);
    varId = vid;
    varNum = newSize;
    if (coef.empty()) {
      return;
    }

    if (newSize > oldSize) {
      T temp[newSize];

      indices.resize(coef.size() * newSize);

      /*-----------------------------------------------------------------------------
       *  can improve effective
       *-----------------------------------------------------------------------------*/

      for (i = length - 1; i >= 0; i -= 1) {
        for (j = 0; j < newSize; j += 1) {
          temp[j] = 0;
        }

        for (j = 0; j < oldSize; j += 1) {
          if (map[j] > -1) {
            temp[map[j]] = indices[i * oldSize + j];
          }
        }

        for (j = 0; j < newSize; j += 1) {
          indices[i * newSize + j] = temp[j];
        }
      }

    } else if (newSize < oldSize) {
      T temp[newSize];

      for (i = 0; i < coef.size(); i += 1) {
        for (j = 0; j < newSize; j += 1) {
          temp[j] = 0;
        }

        for (j = 0; j < oldSize; j += 1) {
          if (map[j] > -1) {
            temp[map[j]] = indices[i * oldSize + j];
          }
        }

        for (j = 0; j < newSize; j += 1) {
          indices[i * newSize + j] = temp[j];
        }
      }

      /* Erase history */
      indices.resize(coef.size() * newSize);

    } else {
      /*-----------------------------------------------------------------------------
       *  I can much better in here
       *-----------------------------------------------------------------------------*/
      T temp[newSize];

      for (i = 0; i < length; i += 1) {
        for (j = 0; j < newSize; j += 1) {
          temp[j] = 0;
        }

        for (j = 0; j < newSize; j += 1) {
          if (map[j] > -1) {
            temp[map[j]] = indices[i * oldSize + j];
          }
        }

        for (j = 0; j < newSize; j += 1) {
          indices[i * oldSize + j] = temp[j];
        }
      }
    }
  }

  void add(const C &cf) {
    vector<T> key(varNum, 0);
    add_term(key, cf);
  }

  Poly<C, T> operator+(const C &cf) const {
    Poly<C, T> temp = *this;
    temp.add(cf);
    return temp;
  }

  void add_term(const term_t &t) {
    indices.insert(indices.end(), t.key.begin(), t.key.begin() + varNum);
    coef.push_back(t.cf);
  }

  Poly<C, T> operator+(const term_t &t) const {
    Poly<C, T> temp = *this;
    temp.add_term(t);
    return temp;
  }

  void mult(const C &cf) {
    for (int i = 0; i < coef.size(); i += 1) {
      coef[i] *= cf;
    }
  }

  Poly<C, T> operator*(const C &cf) const {
    Poly<C, T> temp = *this;
    temp.mult(cf);
    return temp;
  }

  void mult_term(const term_t &t) {
    int i, j;
    for (i = 0; i < coef.size(); i++) {
      for (j = 0; j < varNum; j++) {
        indices[i * varNum + j] += t.key[j];
      }
      coef[i] *= t.cf;
    }
  }

  Poly<C, T> operator*(const term_t &t) const {
    Poly<C, T> temp = *this;
    temp.mult_term(t);
    return temp;
  }

  void add_poly(const poly_t &poly2) {
    int i, j;
    int vid = varId;
    if (varId != poly2.varId) {
      vid = VAR_TABLE.mergeVar(varId, poly2.varId);
      changeVarId(vid);
    }

    vector<T> key(varNum);

    const int p2Size = poly2.varNum;

    int map[p2Size];

    VAR_TABLE.getConvertMap(poly2.varId, varId, map);

    int size = coef.size();

    for (i = 0; i < size; i += 1) {
      if (0 == coef[i]) continue;

      for (j = 0; j < varNum; j += 1) {
        key[j] = 0;
      }

      for (j = 0; j < p2Size; j += 1) {
        if (map[j] > -1) key[map[j]] = indices[i * p2Size + j];
      }
      term_t tempt(key, poly2.coef[i]);
      add_term(tempt);  //  key, poly2.coef[i]);
    }
  }

  Poly<C, T> operator+(const poly_t &poly2) const {
    Poly<C, T> temp = *this;
    temp, add_poly(poly2);
    return temp;
  }

  void mult_poly(const poly_t &poly2) {
    int i, j;

    poly_t re;

    int vid = VAR_TABLE.mergeVar(varId, poly2.varId);

    re.changeVarId(vid);

    const int varSize = re.varNum;

    T key[varSize];

    if (coef.size() >= poly2.coef.size()) {
      poly_t poly11 = *this;

      poly11.changeVarId(vid);

      const int p2Size = poly2.varNum;

      int map[p2Size];

      VAR_TABLE.getConvertMap(poly2.varId, vid, map);

      for (i = 0; i < poly2.coef.size(); i += 1) {
        if (poly2.coef[i] == 0) continue;
        fill(key, key + varSize, 0);

        for (j = 0; j < p2Size; j += 1) {
          if (map[j] > -1) key[map[j]] = poly2.indices[i * p2Size + j];
        }
        vector<T> tempkey(key, key + varSize);
        term_t tempt(tempkey, poly2.coef[i]);

        re.add_poly(poly11 + tempt);
      }

    } else {
      poly_t poly22 = poly2;

      poly22.changeVarId(vid);

      const int p1Size = VAR_TABLE.getVarNum(varId);
      int map[p1Size];
      VAR_TABLE.getConvertMap(varId, vid, map);

      for (i = 0; i < coef.size(); i += 1) {
        if (coef[i] == 0) continue;
        for (j = 0; j < varSize; j += 1) {
          key[j] = 0;
        }
        for (j = 0; j < p1Size; j += 1) {
          if (map[j] > -1) key[map[j]] = indices[i * p1Size + j];
        }
        vector<T> tempkey(key, key + varSize);
        term_t tempt(tempkey, coef[i]);
        re.add_poly(poly22 + tempt);
      }
    }
    *this = re;
  }

  poly_t operator*(const poly_t &poly2) const {
    poly_t temp = *this;
    temp.mult_poly(poly2);
    return temp;
  }
};
}
}
#endif
