#include "psdproblem.h"
#include <iostream>
#include <map>
#include "convexhull.h"
#include "findsos.h"
#include "monomial.h"
#include "psdutil.h"
#include "sdpsolver.h"
#include "search.h"
#include "sort.h"

namespace aiSat {

namespace psd {

using std::cout;
using std::endl;
using std::map;

static int delRepeat(indice_t *indices, const int length, const int varNum) {
  ASSERT(length > 0, "length must be a positive number ");
  if (1 == length) return length;
  int i, k;
  int j = 1;
  bool repeat = true;
  qsortM(indices, varNum, 0, length - 1, compare);

  for (i = 1; i < length; i++) {
    repeat = true;
    for (k = 0; k < varNum; k++) {
      if (indices[i * varNum + k] != indices[(i - 1) * varNum + k]) {
        repeat = false;
        break;
      }
    }
    if (!repeat) {
      for (k = 0; k < varNum; k++) {
        indices[j * varNum + k] = indices[i * varNum + k];
      }
      j++;
    }
  }
  return j;
}

/**
 *
 *
 * @param DIM  the number of variables
 * @param Gsup G support set
 * @param GLength
 * @param polyS poly support set
 * @param polyS_size
 * @param SOSM   the support will occor in polyS convert to G*G
 *
 * @return
 */

static int reduceConvexHulll(const int DIM, indice_t *Gsup, int &GLength,
                             const indice_t *polyS, const int polyS_size,
                             indice_t **SOSM) {
  int i, j, k, valueIndex, m, sosLength;
  size_t node_size = DIM * sizeof(indice_t);

  if (1 == GLength) {
    sosLength = 1;
    *SOSM = (indice_t *)malloc_d(sosLength * node_size);
    for (i = 0; i < DIM; i++) (*SOSM)[i] = 2 * Gsup[i];
    return sosLength;
  }

  indice_t *key = new indice_t[DIM];
  indice_t *key1 = new indice_t[DIM];

  map<Monomial, Monomialvalue> monMap;

  indice_t *indiceValues = new indice_t[GLength * GLength * DIM];

  bool run = true;
  bool del = false;

  while (run) {
    run = false;
    monMap.clear();

    valueIndex = 0;

    for (i = 0; i < GLength; i++) {
      for (j = i; j < GLength; j++) {
        for (k = 0; k < DIM; k++) {
          key[k] = Gsup[i * DIM + k] + Gsup[j * DIM + k];
        }
        if (monMap.find(Monomial(key, DIM)) != monMap.end()) {
          monMap[Monomial(key, DIM)].value++;

        } else {
          memcpy(indiceValues + valueIndex * DIM, key, node_size);
          Monomialvalue dummy;
          dummy.value = 1;
          monMap[Monomial(indiceValues + valueIndex * DIM, DIM)] = dummy;
          valueIndex++;
        }
      }
    }

    k = 0;
    for (j = 0; j < GLength; j++) {
      for (i = 0; i < DIM; i++) {
        key[i] = 2 * Gsup[j * DIM + i];
      }
      /**
       * Monomial(key, DIM) only has one method to create by G^2
       *
       */

      if (1 == monMap[Monomial(key, DIM)].value) {
        for (i = 0; i < GLength; i++) {
          for (m = 0; m < DIM; m++) {
            key1[m] = key[m] / 2 + Gsup[i * DIM + m];
          }
          monMap.find(Monomial(key1, DIM))->second.add(k);
        }

        k++;
        monMap.find(Monomial(key, DIM))->second.change = false;
      }
    }
    ASSERT(k <= VERTEX_BOUND, "Number of vertex is more than limit.");

    for (j = 0; j < GLength; j++) {
      for (i = 0; i < DIM; i++) {
        key[i] = 2 * Gsup[j * DIM + i];
      }

      if (monMap.find(Monomial(key, DIM))->second.size() == 1 &&
          findIndex(key, polyS, polyS_size, DIM) < 0) {
        del = true;
        for (i = 0; i < GLength; i++) {
          for (m = 0; m < DIM; m++) {
            key1[m] = key[m] / 2 + Gsup[i * DIM + m];
          }
          if (monMap.find(Monomial(key1, DIM))->second.size() > 1 &&
              monMap.find(Monomial(key, DIM))
                  ->second.conjunction(
                      monMap.find(Monomial(key1, DIM))->second)) {
            del = false;
            break;
          }
        }

        if (del) {
          memmove(Gsup + j * node_size, Gsup + (j + 1) * node_size,
                  (GLength - j - 1) * node_size);
          GLength--;

          run = true;
          break;
        }
      }
    }
  }

  sosLength = monMap.size();

  *SOSM = (indice_t *)malloc_d(sosLength * node_size);

  i = 0;
  for (map<Monomial, Monomialvalue>::const_iterator it = monMap.begin();
       it != monMap.end(); ++it) {
    memcpy((*SOSM) + i * DIM, it->first.indice, node_size);
    i++;
  }

  delete[] key;
  delete[] key1;
  delete[] indiceValues;
  return sosLength;
}

int SOSP::addSOSsup(vector<SOSMonomial *> *indices_vec) {
  int varNum = getVarTable<indice_t>()
                   .getAllVarNum();  // getvarNum(1);//all num of vaiable

  int *varOccur = new int[varNum];
  int i, j;

  int length = indices_vec->size();

  for (i = 0; i < varNum; i++) {
    varOccur[i] = 0;
  }
  vector<SOSMonomial *>::iterator it = indices_vec->begin();
  for (; it != indices_vec->end(); it++) {
    for (i = 0; i < varNum; i++) {
      if ((*it)->indices[i] > 0) varOccur[i] = 1;
    }
  }

  vector<indice_t> vars(varNum);

  int exactVarNum = 0;

  j = 0;
  for (i = 0; i < varNum; i++) {
    if (varOccur[i] > 0) {
      exactVarNum++;
      vars[j++] = i;
    }
  }

  if (0 == exactVarNum) {
    exactVarNum = getVarTable<indice_t>().getAllVarNum();
    for (i = 0; i < exactVarNum; i++) vars[i] = i;
  }
  int varId = getVarTable<indice_t>().addVarElem(vars, exactVarNum);

  indice_t *indices = new indice_t[exactVarNum * length];

  it = indices_vec->begin();
  j = 0;
  for (; it != indices_vec->end(); it++) {
    for (i = 0; i < exactVarNum; i++) {
      indices[j * exactVarNum + i] = (*it)->indices[vars[i]];
    }
    j++;
  }

  int supportId = SUPPORT_TABLE.addSOSsupByIndice(varId, indices, length);

  indice_t *tempG = new indice_t[exactVarNum * length];

  for (i = 0; i < length; i++) {
    for (j = 0; j < exactVarNum; j++) {
      tempG[i * exactVarNum + j] = indices[i * exactVarNum + j] / 2;
    }
  }

  int origLength = delRepeat(tempG, length, exactVarNum);

  int gLength = 0;

  indice_t *Gsup = overConvexHull(tempG, origLength, exactVarNum, &gLength);

  indice_t *SOSM = NULL;
  indice_t *GSUP = NULL;
  int sosLength =
      reduceConvexHulll(exactVarNum, Gsup, gLength, indices, length, &SOSM);

  GSUP = (indice_t *)realloc_d(Gsup, gLength * exactVarNum * sizeof(indice_t));

  qsortM(GSUP, exactVarNum, 0, gLength - 1, compare);

  qsortM(SOSM, exactVarNum, 0, sosLength - 1, compare);

  SUPPORT_TABLE.setGsup(supportId, gLength, GSUP);
  SUPPORT_TABLE.setSOSsup(supportId, sosLength, SOSM);

  int AMlength = 0;

  createArrangeM(supportId, SOSM, &AMlength, sosLength);

  delete[] indices;
  delete[] tempG;

  delete[] varOccur;
  return supportId;
}

int SOSP::addSOSsup(vector<int> *varVec, vector<int> *degVec) {
  std::sort(varVec->begin(), varVec->end());
  std::sort(degVec->begin(), degVec->end());
  vector<indice_t> vars(varVec->begin(), varVec->end());

  int varNum = varVec->size();

  vector<int>::iterator it;
  int i, j;
  i = 0;

  int varId = getVarTable<indice_t>().addVarElem(vars);
  int length = 0;

  for (it = degVec->begin(); it != degVec->end(); it++) {
    if (0 == *it)
      length++;
    else
      length += nchoosek(varNum + (*it) - 1, varNum - 1);
  }

  indice_t *indices = new indice_t[length * varNum];

  length = 0;
  int tempLen = 0;

  for (it = degVec->begin(); it != degVec->end(); it++) {
    if (0 == *it) {
      for (i = 0; i < varNum; i++) indices[length * varNum + i] = 0;
      length++;

    } else {
      tempLen = nchoosek(varNum + (*it) - 1, varNum - 1);
      getAllMonHomoTd(varNum, *it, tempLen, indices + length * varNum);
      length += tempLen;
    }
  }

  int supportId = SUPPORT_TABLE.addSOSsupByIndice(varId, indices, length);

  indice_t *tempG = new indice_t[length * varNum];

  for (i = 0; i < length; i++) {
    for (j = 0; j < varNum; j++) {
      tempG[i * varNum + j] = (indices[i * varNum + j]) / 2;
    }
  }

  j = delRepeat(tempG, length, varNum);

  int gLength = 0;

  indice_t *Gsup = overConvexHull(tempG, j, varNum, &gLength);
  indice_t *SOSM = NULL;
  indice_t *GSUP = NULL;

  int sosLength =
      reduceConvexHulll(varNum, Gsup, gLength, indices, length, &SOSM);

  if (0 != gLength * varNum) {
    GSUP = (indice_t *)realloc_d(Gsup, gLength * varNum * sizeof(indice_t));
  } else {
    GSUP = Gsup;
  }
  qsortM(GSUP, varNum, 0, gLength - 1, compare);

  qsortM(SOSM, varNum, 0, sosLength - 1, compare);

  SUPPORT_TABLE.setGsup(supportId, gLength, GSUP);
  SUPPORT_TABLE.setSOSsup(supportId, sosLength, SOSM);

  int AMlength = 0;

  createArrangeM(supportId, SOSM, &AMlength, sosLength);

  delete[] indices;
  delete[] tempG;

  return supportId;
}

void SOSP::title() {
  sos_num++;
  std::cout << "====================================" << std::endl;
  std::cout << "             aiSat                  " << std::endl;
  std::cout << "====================================" << std::endl;
  std::cout << "$" << sos_num << ": " << std::endl;
}

void SOSP::solve() {
  title();

  vector<Poly_t > resP;
  int re = problem->sdp_solver(resP, "pro.txt", "result.txt");
  if (re == 0) {
    map<int, string>::iterator it = left_printMap.begin();
    while (it != left_printMap.end()) {
      std::cout << it->second << " := " << (resP[it->first]) << endl;
      it++;
    }


  } else {
    printf("can not find a feasiable solution\n");
  }

  problem->clear();

  left_printMap.clear();
}

void SOSP::findSOS(Poly_t *poly) {
  title();
  SOSChecker checker(*poly);
  if (checker.checksos(false)) {
    cout << "SOS" << endl;
  } else
    cout << "NO" << endl;
  left_printMap.clear();
}

void SOSP::interpolant(SOSProblem *sys, const int sep) {
  title();
  sys->inter_sdp(sep, "pro.txt", "result.txt");
  left_printMap.clear();
}
void SOSP::clear() {
  problem->clear();
  for (map<string, Poly_t *>::iterator it = polyMap.begin();
       it != polyMap.end(); it++) {
    delete it->second;
  }
  for (map<string, PolyConstraint *>::iterator it = polyConsMap.begin();
       it != polyConsMap.end(); it++) {
    delete it->second;
  }

  varMap.clear();
  monoMap.clear();
  polyConsMap.clear();
  left_printMap.clear();
  right_printMap.clear();
}

/*
  return true if str is a new key
*/

bool SOSP::addVarElem(const string &str, vector<int> *value) {
  if (varMap.find(str) == varMap.end()) {
    varMap[str] = value;
    return true;
  } else {
    delete varMap[str];
    varMap[str] = value;
    return false;
  }
}
/*
  return true if str is a new key
*/

bool SOSP::addMonoElem(const string &str, const int value) {
  if (monoMap.find(str) == monoMap.end()) {
    monoMap[str] = value;
    return false;
  }
  monoMap[str] = value;
  return false;
}
/*
  return true if str is a new key
*/

bool SOSP::addPolyElem(const string &str, Poly_t *poly) {
  if (polyMap.find(str) == polyMap.end()) {
    polyMap[str] = poly;
    return true;
  } else {
    delete polyMap[str];
    polyMap[str] = poly;
    return false;
  }
}
/*
  return true if str is a new key
*/

bool SOSP::addPolyConsElem(const string &str, PolyConstraint *polyCons) {
  if (polyConsMap.find(str) == polyConsMap.end()) {
    polyConsMap[str] = polyCons;
    return true;

  } else {
    delete polyConsMap[str];
    polyConsMap[str] = polyCons;
    return false;
  }
}
}
}
