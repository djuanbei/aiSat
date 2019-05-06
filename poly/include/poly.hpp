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
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <cmath>
#include <iomanip>

#include <iterator>

#include "vartable.hpp"

namespace aiSat {

std::string trim(const std::string &str);

template <typename Out>
void split(const std::string &s, char delim, Out result) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    item = trim(item);
    if (!item.empty()) {
      *(result++) = item;
    }
  }
}

std::vector<std::string> split(const std::string &s, char delim);

namespace psd {
class ConvexGenerator;

class SOSChecker;
}

namespace poly {

template <typename C, typename T>
class Subpoly;

static size_t POLY_ID = 0;

using namespace std;

template <typename C = double, typename T = int>
class Poly {
  friend class aiSat::psd::ConvexGenerator;

  friend class aiSat::psd::SOSChecker;
  friend class Subpoly<C, T>;

 private:
  struct term {
    vector<T> key;
    C cf;
    term() {}
    term(const vector<T> &k, const C &c) : key(k), cf(c) {}
  };
  typedef Poly<C, T>::term term_t;

 public:
  struct Term {
    vector<pair<T, T> > key;  // var, power
    C cf;
    Term() {}
    Term(const vector<pair<T, T> > &k, C c = 1) : key(k), cf(c) {}
  };

  struct assignElem {
    int var;
    C value;
    assignElem() : var(0), value(0) {}

    assignElem(int v, C c) : var(v), value(c) {}
  };

  typedef Poly<C, T> poly_t;

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

  void add_term(const term_t &t) {
    indices.insert(indices.end(), t.key.begin(), t.key.begin() + varNum);
    coef.push_back(t.cf);
  }

  void add_term(const vector<T> &key, const C cf) {
    indices.insert(indices.end(), key.begin(), key.begin() + varNum);
    coef.push_back(cf);
  }

  void add_term(const T *key, const C cf) {
    for (int i = 0; i < varNum; i++) {
      indices.push_back(key[i]);
    }
    coef.push_back(cf);
  }

  Poly<C, T> operator+(const term_t &t) const {
    Poly<C, T> temp = *this;
    temp.add_term(t);
    return temp;
  }

  void mult_term(const term_t &t) {
    int j;
    for (size_t i = 0; i < coef.size(); i++) {
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
    varNum = getVarTable<T>().getVarNum(varId);
    id = POLY_ID++;
  }
  Poly(bool totalVars) {


    varNum = getVarTable<T>().getAllVarNum();

    vector<T> vars;
    for (int i = 0; i < varNum; i++) {
      vars.push_back(i);
    }

    varId = getVarTable<T>().addVarElem(vars);

    id = POLY_ID++;

  }

  Poly(const int evarId, const C coefDomain, const int termLength,
       const int indiceDomain) {
    id = POLY_ID++;
    varId = evarId;
    varNum = getVarTable<T>().getVarNum(varId);
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

  int getId() const { return id; }
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
  int getDegreeAt(int k, int i) const { return indices[k * varNum + i]; }

  void getDegreeAt(int k, vector<T> &re) const {
    re.resize(varNum);
    for (int i = 0; i < varNum; i++) {
      re[i] = indices[k * varNum + i];
    }
  }

  C getCF(const int i) const { return coef[i]; }

  C getCF(const Term &term) const {
    map<int, int> mapIndex;
    vector<T> vars;
    getVarTable<T>().getVarElem(varId, vars);
    for (int i = 0; i < varNum; i++) {
      mapIndex[vars[i]] = i;
    }
    vector<indice_t> pow(varNum, 0);

    for (size_t i = 0; i < term.key.size(); i++) {
      pow[mapIndex[term.key[i].first]] += term.key[i].second;
    }
    int loc = findIndex(pow);
    ASSERT(loc > -1, "There are no that term");
    if (loc > -1) {
      return getCF(loc);
    } else {
      return 0;
    }
  }

  C getCF(const string &term) const {
    Term temp_term;
    vector<string> elems = split(term, '*');
    for (vector<string>::iterator it = elems.begin(); it != elems.end(); it++) {
      string str = *it;
      std::size_t found = str.find("^");
      string var;
      int pow = 1;
      if (found != std::string::npos) {
        var = str.substr(0, found);
        pow = atoi(str.substr(found + 1).c_str());
      } else {
        var = str;
      }
      int index = getVarTable<T>().findVarIndex(var);
      if (index < 0) {
        return 0;
      }
      temp_term.key.push_back(make_pair(index, pow));
    }

    return getCF(temp_term);
  }

  /**
   * @brief obtain polynomal after assiginment value
   *
   * @param value  there are no twos elements in value have same var
   *
   * @return the polyomial after substitute variable as corresponding value
   */

  Poly<C, T> eval(const vector<assignElem> &value) const {
    if (value.empty()) {
      return *this;
    }

    vector<C> ncoef(coef);
    map<int, int> mapIndex;
    vector<T> vars;
    getVarTable<T>().getVarElem(varId, vars);
    vector<C> values(varNum);
    vector<bool> haveAssign(varNum, false);
    for (int i = 0; i < varNum; i++) {
      values[i] = 1;
      mapIndex[vars[i]] = i;
    }

    for (typename vector<assignElem>::const_iterator it = value.begin();
         it != value.end(); it++) {
      haveAssign[mapIndex[it->var]] = true;
      values[mapIndex[it->var]] = it->value;
    }

    vector<T> nvars;
    for (int i = 0; i < varNum; i++) {
      if (!haveAssign[i]) {
        nvars.push_back(vars[i]);
      }
    }

    int nvarNum = nvars.size();
    int nvid = getVarTable<T>().addVarElem(nvars);
    Poly<C, T> re(nvid);
    vector<indice_t> nindices(coef.size() * nvarNum);

    for (size_t i = 0; i < coef.size(); i++) {
      int h = 0;
      for (int j = 0; j < varNum; j++) {
        if (haveAssign[j]) {
          if ((values[j] == 0) && (indices[i * varNum + j] > 0)) {
            ncoef[i] = 0;
            break;
          }

          for (int k = 0; k < indices[i * varNum + j]; k++) {
            ncoef[i] *= values[j];
          }
        } else {
          nindices[nvarNum * i + h] = indices[i * varNum + j];
          h++;
        }
      }
    }

    re.coef = ncoef;
    re.indices = nindices;
    re.update();
    return re;
  }

  /**
   * @brief  combine the terms whoes indices are same
   *
   */
  void update(void) {
    size_t i, j;
    int k;
    if (coef.empty()) {
      return;
    }
    qsort(0, coef.size() - 1);

    j = 0;
    size_t size = coef.size();
    for (i = 1; i < size; i++) {
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

  /**
   * @brief delete the variable which never occur in polynomial
   *
   */

  void simplify() {
    update();
    vector<T> usedVars;
    vector<T> vars;
    vector<bool> exists(varNum, false);
    getVarTable<T>().getVarElem(varId, vars);
    int size = coef.size();
    for (int i = 0; i < varNum; i++) {
      int j = 0;
      for (; j < size; j++) {
        if (indices[i * varNum + j] > 0) {
          break;
        }
      }
      if (j < size) {
        usedVars.push_back(vars[i]);
        exists[i] = true;
      }
    }

    if (usedVars.size() < varNum) {
      varId = getVarTable<T>().addVarElem(usedVars);
      varNum = usedVars.size();
      int i, j;
      i = j = 0;
      for (; i < size * varNum; i++) {
        if (exists[i % varNum]) {
          indices[j++] = indices[i];
        }
      }
      indices.resize(j);
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

  /**
   * @brief compare two polynomial after simplify them.
   *
   * @param other
   *
   * @return
   */
  template <typename CC, typename TT>
  bool friend operator==(Poly<CC, TT> &lhs, Poly<CC, TT> &rhs);

  string toString() {
    update();
    return toString(MIN_COEF, 4);
  }

  string toString(const float minValue, const int prec) const {
    vector<int> locs;
    for (size_t i = 0; i < coef.size(); i++) {
      locs.push_back(i);
    }
    return toString(locs, minValue, prec);
  }

  string toString(const vector<int> &locs, float minValue = MIN_COEF,
                  int prec = 6) const {
    int i, j;
    bool isFirst = true;
    stringstream str;

    vector<T> vars;
    getVarTable<T>().getVarElem(varId, vars);

    vector<string> varname(varNum);

    if (0 == coef.size() || locs.empty()) {
      return " 0 \n";
    }

    for (i = 0; i < varNum; i++) {
      varname[i] = getVarTable<T>().getVarName(vars[i]);
    }

    int size = (int)locs.size();

    for (i = 0; i < size; i += 1) {
      int loc = locs[i];
      if (coef[loc] > minValue || coef[loc] < -minValue) {
        bool isConstant = true;
        for (j = 0; j < varNum; j++) {
          if ((int)indices[loc * varNum + j] > 0) {
            isConstant = false;
            break;
          }
        }

        if (isConstant) {
          if (coef[loc] > 0.0) {
            if (!isFirst) {
              str << "+";
            }
            str << std::setprecision(prec) << coef[loc];
          } else if (coef[loc] < 0.0) {
            str << std::setprecision(prec) << coef[loc];
          }

        } else {
          bool isCeng = false;
          if (coef[loc] > 0.0) {
            if (!isFirst) {
              str << "+";
            }
            if (coef[loc] != (C)1.0) {
              isCeng = true;
              str << std::setprecision(prec) << coef[loc];
            }
          }

          else if (coef[loc] < 0.0) {
            str << std::setprecision(prec) << coef[loc];
          }

          for (j = 0; j < varNum; j += 1) {
            if (indices[loc * varNum + j] != 0) {
              if (isCeng) {
                str << "*";
              }
              isCeng = true;
            }
            if (indices[loc * varNum + j] > 0) {
              if (indices[loc * varNum + j] > 1) {
                str << varname[j] << "^" << (int)indices[loc * varNum + j];
              }

              else if (1 == indices[loc * varNum + j]) {
                str << varname[j];
              }
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
  bool isAllCoefNegative() const {
    for (size_t i = 0; i < coef.size(); i++) {
      if (coef[i] > 0) {
        return false;
      }
    }

    return true;
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

  C getConstant() {
    update();
    ASSERT(isConstant(), "polynomail must be constant ");
    return coef[0];
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
    const int newSize = getVarTable<T>().getVarNum(vid);

    vector<int> mapIndex(oldSize);
    getVarTable<T>().getConvertMap(varId, vid, mapIndex);
    varId = vid;
    varNum = newSize;
    if (coef.empty()) {
      return;
    }

    if (newSize > oldSize) {
      vector<T> temp(newSize);

      indices.resize(coef.size() * newSize);

      /*-----------------------------------------------------------------------------
       *  can improve effective
       *-----------------------------------------------------------------------------*/

      for (i = length - 1; i >= 0; i -= 1) {
        for (j = 0; j < newSize; j += 1) {
          temp[j] = 0;
        }

        for (j = 0; j < oldSize; j += 1) {
          if (mapIndex[j] > -1) {
            temp[mapIndex[j]] = indices[i * oldSize + j];
          }
        }

        for (j = 0; j < newSize; j += 1) {
          indices[i * newSize + j] = temp[j];
        }
      }

    } else if (newSize < oldSize) {
      vector<T> temp(newSize);

      for (i = 0; i < coef.size(); i += 1) {
        for (j = 0; j < newSize; j += 1) {
          temp[j] = 0;
        }

        for (j = 0; j < oldSize; j += 1) {
          if (mapIndex[j] > -1) {
            temp[mapIndex[j]] = indices[i * oldSize + j];
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
      vector<T> temp(newSize);

      for (i = 0; i < length; i += 1) {
        for (j = 0; j < newSize; j += 1) {
          temp[j] = 0;
        }

        for (j = 0; j < newSize; j += 1) {
          if (mapIndex[j] > -1) {
            temp[mapIndex[j]] = indices[i * oldSize + j];
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

  void add_term(const Term &term) {
    map<int, int> mapIndex;
    vector<T> vars;
    getVarTable<T>().getVarElem(varId, vars);
    for (int i = 0; i < varNum; i++) {
      mapIndex[vars[i]] = i;
    }
    coef.push_back(term.cf);
    size_t last = indices.size();
    for (int i = 0; i < varNum; i++) {
      indices.push_back(0);
    }
    for (size_t i = 0; i < term.key.size(); i++) {
      indices[last + mapIndex[term.key[i].first]] = term.key[i].second;
    }
  }

  Poly<C, T> operator+(const Term &term) const {
    Poly<C, T> temp = *this;
    temp.add_term(term);
    return temp;
  }
  void mult(const C &cf) {
    for (size_t i = 0; i < coef.size(); i += 1) {
      coef[i] *= cf;
    }
  }

  Poly<C, T> operator*(const C &cf) const {
    Poly<C, T> temp = *this;
    temp.mult(cf);
    return temp;
  }

  void mult_term(const Term &term) {
    if (0 == term.cf) {
      resetZero();
      return;
    }

    map<int, int> mapIndex;
    vector<T> vars;
    getVarTable<T>().getVarElem(varId, vars);
    for (int i = 0; i < varNum; i++) {
      mapIndex[vars[i]] = i;
    }

    for (size_t i = 0; i < coef.size(); i++) {
      coef[i] *= term.cf;
      for (size_t j = 0; j < term.key.size(); j++) {
        indices[i * varNum + mapIndex[term.key[i].first]] += term.key[i].second;
      }
    }
  }

  Poly<C, T> operator*(const Term &term) const {
    Poly<C, T> temp = *this;
    temp.mult_term(term);
    return temp;
  }

  void add_poly(const poly_t &poly2) {
    int vid = varId;
    if (varId != poly2.varId) {
      vid = getVarTable<T>().mergeVar(varId, poly2.varId);
      changeVarId(vid);
    }

    vector<T> key(varNum);

    const int p2VarNum = poly2.varNum;

    vector<int> mapIndex(p2VarNum);
    getVarTable<T>().getConvertMap(poly2.varId, varId, mapIndex);

    int size = poly2.coef.size();

    for (int i = 0; i < size; i += 1) {
      if (0 == poly2.coef[i]) {
        continue;
      }
      fill(key.begin(), key.end(), 0);

      for (int j = 0; j < p2VarNum; j += 1) {
        if (mapIndex[j] > -1)
          key[mapIndex[j]] = poly2.indices[i * p2VarNum + j];
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

  void add_poly(const poly_t *poly2) { add_poly(*poly2); }

  void mult_poly(const poly_t &poly2) {
    int j;

    poly_t re;

    int vid = getVarTable<T>().mergeVar(varId, poly2.varId);

    re.changeVarId(vid);

    const int varSize = re.varNum;
    vector<T> key(varSize);
    vector<int> mapKey;

    if (coef.size() >= poly2.coef.size()) {
      poly_t poly11 = *this;

      poly11.changeVarId(vid);

      const int p2VarNum = poly2.varNum;

      getVarTable<T>().getConvertMap(poly2.varId, vid, mapKey);

      for (size_t i = 0; i < poly2.coef.size(); i += 1) {
        if (poly2.coef[i] == 0) continue;
        fill(key.begin(), key.begin() + varSize, 0);

        for (j = 0; j < p2VarNum; j += 1) {
          if (mapKey[j] > -1) key[mapKey[j]] = poly2.indices[i * p2VarNum + j];
        }

        term_t tempt(key, poly2.coef[i]);

        re.add_poly(poly11 * tempt);
      }

    } else {
      poly_t poly22 = poly2;

      poly22.changeVarId(vid);

      const int p1VarNum = getVarTable<T>().getVarNum(varId);

      getVarTable<T>().getConvertMap(varId, vid, mapKey);

      for (size_t i = 0; i < coef.size(); i += 1) {
        if (coef[i] == 0) continue;
        fill(key.begin(), key.end(), 0);

        for (j = 0; j < p1VarNum; j += 1) {
          if (mapKey[j] > -1) key[mapKey[j]] = indices[i * p1VarNum + j];
        }

        term_t tempt(key, coef[i]);
        re.add_poly(poly22 * tempt);
      }
    }
    *this = re;
  }

  poly_t operator*(const poly_t &poly2) const {
    poly_t temp = *this;
    temp.mult_poly(poly2);
    return temp;
  }

  void mult_poly(const poly_t *poly2) { mult_poly(*poly2); }

  void pow(const int p) {
    poly_t dummy = *this;
    for (int i = 1; i < p; i++) {
      this->mult_poly(dummy);
    }
  }

  poly_t operator^(const int p) const {
    poly_t temp = *this;
    temp.pow(p);
    return temp;
  }

  template <typename CC, typename TT>
  friend ostream &operator<<(ostream &os, Poly<CC, TT> &p);
};

template <typename CC, typename TT>
ostream &operator<<(ostream &os, Poly<CC, TT> &p) {
  os << p.toString();
  return os;
}

template <typename CC, typename TT>
bool operator==(Poly<CC, TT> &lhs, Poly<CC, TT> &rhs) {
  lhs.simplify();
  rhs.simplify();
  if (lhs.varId != rhs.varId) {
    return false;
  }

  if (lhs.coef != rhs.coef) {
    return false;
  }
  if (lhs.indices != rhs.indices) {
    return false;
  }

  return true;
}
}
}
#endif
