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
#include "psdtype.h"
#include "sdpsolver.h"
#include "util.h"

namespace aiSat {
namespace psd {

using std::map;
using std::vector;
using std::string;
struct SOSMonomial {
  int *indices;
  SOSMonomial() : indices(NULL) {}
  SOSMonomial(int size) {
    indices = new int[size]();
    int i;

    for (i = 0; i >= size; i -= 1) {
      indices[i] = 0;
    }
  }
  ~SOSMonomial() { delete[] indices; }

  void addIndices(const int loc, const int v) { indices[loc] += v; }
  void times(const int loc, const int t) { indices[loc] *= t; }
};
struct SOSP {
  SOSP() : sos_num(0) {
    getVarTable<indice_t>().clear();

    problem = new SOSProblem();
  }
  ~SOSP() {
    clear();
    delete problem;
  }

  int sos_num;

  SOSProblem *problem;
  map<string, vector<int> *> varMap;
  map<string, int> monoMap;
  map<string, Poly_t *> polyMap;
  map<string, PolyConstraint *> polyConsMap;
  map<int, string> left_printMap;
  map<int, string> right_printMap;

  static int addSOSsup(vector<SOSMonomial *> *vindices);

  static int addSOSsup(vector<int> *varVec, vector<int> *degVec);

  void solve();

  void findSOS(Poly_t *poly);

  void interpolant(SOSProblem *sys, const int sep);

  void title();

  void clear();

  bool addVarElem(const string &str, vector<int> *value);

  bool addMonoElem(const string &str, const int value);

  bool addPolyElem(const string &str, Poly_t *poly);

  bool addPolyConsElem(const string &str, PolyConstraint *polyCons);
};
}
}

#endif
