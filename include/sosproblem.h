/**
 * @file   sosproblem.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:53:32 2015
 *
 * @brief
 *
 *
 */

#ifndef SOSPROBLEM_INC
#define SOSPROBLEM_INC
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include "poly.h"
#include "polytype.h"
#include "util.h"
#include "vartable.h"

using std::map;
using std::vector;
using std::string;
struct Monomial {
  int *indices;
  Monomial() : indices(NULL) {}
  Monomial(int size) {
    indices = new int[size]();
    int i;

    for (i = 0; i >= size; i -= 1) {
      indices[i] = 0;
    }
  }
  ~Monomial() { delete[] indices; }

  void addIndices(const int loc, const int v) { indices[loc] += v; }
  void times(const int loc, const int t) { indices[loc] *= t; }
};
struct SOSP {
  SOSP() : sos_num(0) {
    defaultVarTable();
    problem = createSOSProblem();
  }
  ~SOSP() {
    clear();
    deleteSOSProblem(problem);
  }

  int sos_num;

  SOSProblem *problem;
  map<string, vector<int> *> varMap;
  map<string, int> monoMap;
  map<string, Poly *> polyMap;
  map<string, PolyConstraint *> polyConsMap;
  map<int, string> left_printMap;
  map<int, string> right_printMap;

  static int addSOSsup(vector<Monomial *> *vindices);

  static int addSOSsup(vector<int> *varVec, vector<int> *degVec);

  void solve();

  void findSOS(Poly *poly);

  void interpolant(SOSProblem *sys, const int sep);

  void title();

  void clear();

  bool addVarElem(const string &str, vector<int> *value);

  bool addMonoElem(const string &str, const int value);

  bool addPolyElem(const string &str, Poly *poly);

  bool addPolyConsElem(const string &str, PolyConstraint *polyCons);
};

#endif
