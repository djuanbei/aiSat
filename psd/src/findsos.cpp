

/**
 * @file   findsos.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Oct  9 20:02:27 2017
 * 
 * @brief  polynomial semindefinite programming
 * 
 * 
 */


#include "findsos.h"

#include <cmath>
#include <map>
#include "convexhull.h"

#include "iostream"
#include "psdutil.h"
#include "sdpsolver.h"
#include "search.h"
#include "selfassert.h"
#include "sort.h"
#include "subpoly.hpp"
#include "util.h"

#include "pointlist.h"

extern "C" {
void dsyev_(char *jobz, char *uplo, int *n, double *a, int *lda, double *w,
            double *work, int *lwork, int *info);
}

extern int csdp_verbose;

namespace aiSat {
namespace psd {

using std::map;
using std::cout;
using std::endl;

struct params {
  double mineignvalue;
  float printMinValue;
  params() : mineignvalue(1e-2), printMinValue(1e-2) {}
};

static params par;
static clock_t start;

static int DIM;

enum { SUCCESS = 1, FAILURE = 2, OTHER = 3 };

static void delpoly(void *p) { delete ((Poly_t *)p); }

void SOSChecker::sosrepresent(PointList *sosList, double *X,
                              const int blockSize, const int sosMid,
                              const float minV) {
  int i, j, l, lda = blockSize;
  double *w = (double *)malloc_d(blockSize * sizeof(double));
  int lwork = blockSize * blockSize * 2;
  double *work;
  int info;
  int n = blockSize;
  lwork = -1;
  double workopt;

  dsyev_("V", "U", &n, X, &lda, w, &workopt, &lwork, &info);
  lwork = (int)workopt;
  work = (double *)malloc_d(lwork * sizeof(double));

  dsyev_("V", "U", &n, X, &lda, w, work, &lwork, &info);

  const int varNum = getVarTable<indice_t>().getVarNum(
      SUPPORT_TABLE.getSupElem(sosMid)->varId);

  indice_t *Z = SUPPORT_TABLE.getGsup(sosMid, &l);

  for (i = 0; i < blockSize; i += 1) {
    if (w[i] > minV) {
      Poly_t *p1 = new Poly_t();
      p1->changeVarId(SUPPORT_TABLE.getSupElem(sosMid)->varId);
      vector<indice_t> vars;
      getVarTable<indice_t>().getVarElem(
          SUPPORT_TABLE.getSupElem(sosMid)->varId, vars);

      Poly_t::Term temp_term;
      for (j = 0; j < blockSize; j += 1) {
        for (size_t k = 0; k < vars.size(); k++) {
          temp_term.key.push_back(make_pair(vars[k], Z[j * varNum + k]));
        }
        temp_term.cf = X[ijtok(j + 1, i + 1, blockSize)];

        p1->add_term(temp_term);
      }

      p1->mult(sqrtf(w[i]));

      push_back_L(sosList, p1);
    }
  }

  free(w);
  free(work);
}

void SOSChecker::sosPresent(Subpoly_t *subpoly, const PointList *sosList,
                            const double printMin) {
  cout << *subpoly<<"="<< endl;

  PointElem *temp = sosList->head;
                          
                          
  while (temp->next != NULL) {
    Poly_t *tp = (Poly_t *)temp->value;
    if (tp->getSize() > 0) {
      cout << "(";
      string str = tp->toString(0.0001, printMin);
      cout << str;

      temp = temp->next;
      cout << ")^2+";
    }
  }

  Poly_t *tp = (Poly_t *)temp->value;
  if (tp->getSize() > 0) {
    cout << "(";
    string str = tp->toString(0.0001, printMin);
    cout << str;

    cout << ")^2" << endl;
  }
}

int SOSChecker::exactConvHull(const Subpoly_t *subpoly, indice_t *candidateSet,
                              int &candidateLength, const indice_t *genSet,
                              const int genLength, indice_t **SOSM,
                              int &sosLength, PointList *ans) {
  int i, j, valueIndex, v, m;
  int k;
  int currCandLen = candidateLength;
  int lastre = -1;
  int size = subpoly->size();
  const vector<int> &loc = subpoly->locs;

  const indice_t *indices = &(subpoly->parent.indices[0]);

  size_t node_size = DIM * sizeof(indice_t);

  indice_t *key = new indice_t[DIM];
  indice_t *key1 = new indice_t[DIM];

  map<monomial, monomialvalue> monMap;
  indice_t *indiceValues =
      new indice_t[candidateLength * candidateLength * DIM];

  valueIndex = 0;
  for (i = 0; i < genLength; i += 1) {
    for (j = 0; j < DIM; j += 1) {
      key[j] = 2 * genSet[i * DIM + j];
    }
    memcpy(indiceValues + valueIndex * DIM, key, node_size);
    monomialvalue dummy;
    dummy.value = 1;
    monMap[monomial(indiceValues + valueIndex * DIM, DIM)] = dummy;
    valueIndex++;
  }

  for (i = 0; i < currCandLen; i += 1) {
    for (j = i; j < currCandLen; j += 1) {
      for (k = 0; k < DIM; k += 1) {
        key[k] = candidateSet[i * DIM + k] + candidateSet[j * DIM + k];
      }
      if (monMap.find(monomial(key, DIM)) != monMap.end()) {
        monMap[monomial(key, DIM)].value++;
        //=monMap[key,DIM)]+1;
      } else {
        memcpy(indiceValues + valueIndex * DIM, key, node_size);
        monomialvalue dummy;
        dummy.value = 1;
        monMap[monomial(indiceValues + valueIndex * DIM, DIM)] = dummy;
        valueIndex++;
      }
    }
  }

  //	for(open_hash_map<nominal,int>::const_iterator
  // it=monMap.begin();it!=monMap.end();++it){
  //		cout << it.key() << " => " << it.value() << '\n';
  //	}

  /*-----------------------------------------------------------------------------
   *  delete some monomial in monMap which value equal to 1 and the value with
   *relate with this monomial,
   *  itertation delete monomial in monMap
   *-----------------------------------------------------------------------------*/

  while (lastre != currCandLen) {
    lastre = currCandLen;

    for (i = 0; i < currCandLen; i += 1) {
      for (j = 0; j < DIM; j += 1) {
        key[j] = 2 * candidateSet[i * DIM + j];
      }
      if (monMap[monomial(key, DIM)].value ==
          1) { /* expect 2*candidateSet[i*DIM+j], no other combines
of monomial has value key*/
        for (j = 0; j < currCandLen; j += 1) {
          for (k = 0; k < DIM; k += 1) {
            key1[k] = candidateSet[i * DIM + k] + candidateSet[j * DIM + k];
          }
          monMap[monomial(key1, DIM)].value--;
        }
        monMap.erase(monomial(key, DIM));
        if (i + 1 < currCandLen)
          memmove(candidateSet + i * DIM, candidateSet + (i + 1) * DIM,
                  (currCandLen - i - 1) * node_size);

        i--;
        currCandLen--;
      }
    }
  }

  candidateLength = currCandLen;
  if (NULL != ans)
    cout << "Constructing G use time: "
         << (clock() - start - 0.0) / CLOCKS_PER_SEC << endl;

  sosLength = 0;

  for (map<monomial, monomialvalue>::const_iterator it = monMap.begin();
       it != monMap.end(); ++it) {
    if (it->second.value > 0) sosLength++;
  }

  //  open_hash_map<monomial,int>::const_iterator it=monMap.begin();
  // which(it!=monMap.end()){
  //   if(it.value()==0)it= monMap.erase(it.key());
  //   else it++;
  // }
  for (map<monomial, monomialvalue>::iterator it = monMap.begin();
       it != monMap.end(); ++it) {
    if (it->second.value == 0) monMap.erase(it);
    if (it == monMap.end()) break;
  }

  for (i = 0; i < genLength; i += 1) {
    for (j = 0; j < DIM; j += 1) {
      key[j] = 2 * genSet[i * DIM + j];
    }
    monMap[monomial(key, DIM)].value--;
  }

  k = 0;

  for (j = 0; j < genLength; j += 1) {
    for (i = 0; i < DIM; i += 1) {
      key[i] = 2 * genSet[j * DIM + i];
    }
    v = monMap[monomial(key, DIM)].value;

    if (v == 1) {
      for (i = 0; i < currCandLen; i += 1) {
        for (m = 0; m < DIM; m += 1) {
          key1[m] = key[m] / 2 + candidateSet[i * DIM + m];
        }
        map<monomial, monomialvalue>::iterator it =
            monMap.find(monomial(key1, DIM));

        it->second.add(k);
      }

      k++;
      monMap.find(monomial(key, DIM))->second.change = false;
    }
  }
  ASSERT(k <= VERTEX_BOUND, "");
  //  int vertex_num=k;/* the number of vertex */

  bool run = true;

  while (run) {
    run = false;
    for (i = 0; i < currCandLen; i += 1) {
      for (j = 0; j < DIM; j += 1) {
        key[j] = 2 * candidateSet[i * DIM + j];
      }
      if (monMap.find(monomial(key, DIM))->second.change) {
        monMap.find(monomial(key, DIM))->second.change = false;

        for (j = 0; j < currCandLen; j += 1) {
          for (m = 0; m < DIM; m += 1) {
            key1[m] = key[m] / 2 + candidateSet[j * DIM + m];
          }
          if (!(monMap.find(monomial(key1, DIM))
                    ->second.contain(
                        monMap.find(monomial(key, DIM))->second))) {
            monMap.find(monomial(key1, DIM))
                ->second.add(monMap.find(monomial(key, DIM))->second);
            run = true;
          }
        }
      }
    }
  }
  delete[] key;
  delete[] key1;

  /*-----------------------------------------------------------------------------
   *  necessary check
   *-----------------------------------------------------------------------------*/
  if (monMap.find(monomial(indices + loc[0] * DIM, DIM)) == monMap.end()) {
    if (NULL != ans) cout << "Corollary 3" << endl;
    delete[] indiceValues;
    return FAILURE;
  }

  for (i = 1; i < size; i += 1) {
    if (monMap.find(monomial(indices + loc[i] * DIM, DIM)) == monMap.end()) {
      if (NULL != ans) cout << "Corollary 3" << endl;
      delete[] indiceValues;
      return FAILURE;
    }
  }
  int dummy = overConvexChecking(subpoly, indices, loc, size, monMap, ans);
  if (SUCCESS == dummy) {
    delete[] indiceValues;
    return SUCCESS;

  } else if (FAILURE == dummy) {
    delete[] indiceValues;
    return FAILURE;
  }

  *SOSM = (indice_t *)malloc_d(sosLength * node_size);

  i = 0;

  for (map<monomial, monomialvalue>::const_iterator it = monMap.begin();
       it != monMap.end(); ++it) {
    if (it->second.value > 0) {
      memcpy((*SOSM) + i * DIM, it->first.indice, node_size);
      i++;
    }
  }

  delete[] indiceValues;
  return OTHER;
}

/*-----------------------------------------------------------------------------
 *  varId is the var id of polynomial we want to write as
 *  sums of  sos polynomials.
 *  td is the total degree of poly we want check
 *
 *  <numofCons>: is the number of monomials whose total degree less or equal to
 *poly's
 *  total degree
 *  <blockSize>: the nchoosek(td(poly)/2+varnums(poly),varnums(poly))
 *  <b>: is the array of coefs of poly.
 *-----------------------------------------------------------------------------*/

Constraintmatrix *SOSChecker::createConstraintMatrix(
    const Subpoly_t *subpoly, const int sosId, double **b, const indice_t *SOSM,
    const int sosLength, const int gLength) {
  int i, k, tempI;
  const int varNum = getVarTable<indice_t>().getVarNum(subpoly->parent.varId);
  //	const int totalDegree=poly_getTotalDegree(poly);
  int size = subpoly->size();
  const vector<int> &loc = subpoly->locs;
  const indice_t *indices = &(subpoly->parent.indices[0]);
  const vector<coef_t> &coef = subpoly->parent.coef;

  Sparseblock *blockptr;

  printf("Matrix's size %d,  Constraints size: %d,  G size:  %d\n",
         gLength * gLength, sosLength, gLength);

  int temploc;
  *b = (double *)calloc_d((sosLength + 1), sizeof(double));


  for (i = 0; i < size; i += 1) {

    temploc = findIndex(indices + loc[i] * varNum, SOSM, sosLength, varNum);

    if (temploc < 0) {
      return NULL;
    }
    /* b index start form 1 */
    (*b)[temploc + 1] = coef[loc[i]];
    /* printf(" %d %f",loc, poly->coef[i]);  */
  }

  ArrangeMatrix **AM = SUPPORT_TABLE.getAMIndex(sosId, &temploc);
  if (NULL == AM) {
    AM = createArrangeM(sosId, SOSM, &temploc, sosLength);

  }

  Constraintmatrix *re = createNconstraintmatrix(
      sosLength); /* every monomial w.r.t. to a constraint */


  /*-----------------------------------------------------------------------------
   *  use block to  construct  constraints
   *-----------------------------------------------------------------------------*/

  for (i = 0; i < sosLength; i += 1) {
    /* 		sum=0;
     *
     * 		for ( k = 0; k < block[i]->size; k += 1 ) {
     * 			sum+=W[block[i]->index[k]]->size;
     * 		}
     */
    /* assert(sum==1); */

    /* printf("b size %d",*blockSize);; */

    blockptr = createSparseblock(1, gLength, i + 1, AM[i]->size());
    tempI = 1;
    for (k = 0; k < AM[i]->size(); k += 1) {
      blockptr->iindices[tempI] = (*(AM[i]))[k].row + 1;
      blockptr->jindices[tempI] = (*(AM[i]))[k].col + 1;
      blockptr->entries[tempI] = 1;
      tempI++;
    }
    frontInsertBlock(re + i + 1, blockptr);
  }


  return re;
}

int SOSChecker::overConvexChecking(const Subpoly_t *subpoly,
                                   const indice_t *indices,
                                   const vector<int> &loc, const int size,
                                   const map<monomial, monomialvalue> &monMap,
                                   PointList *ans) {

  int i, j, num, k;

  int *check = new int[size];
  int checkSize = size;
  int *choose = new int[size];
  int *checked = new int[size];
  vector<int> tempLoc(size);
  //  const int randTime=vertex_num+DIM;
  uint32_t set[4];

  for (i = 0; i < checkSize; i += 1) {
    check[i] = i;
  }
  i = 0;
  memset(checked, 0, size * sizeof(int));
  while (i < checkSize) {
    if (!checked[check[i]]) {
      set[0] = 0u;
      set[1] = 0u;
      set[2] = 0u;
      set[3] = 0u;
      memset(choose, 0, checkSize * sizeof(int));
      monomialvalue curr =
          monMap.find(monomial(indices + loc[check[i]] * DIM, DIM))->second;
      num = 0;
      monomialvalue::add(set, curr);
      checked[check[i]] = 1;
      tempLoc[num++] = loc[check[i]];
      choose[i] = 1;
      int tempNum = 0;
      while (tempNum != num) {
        tempNum = num;
        for (j = 0; j < checkSize; j += 1) {
          monomialvalue temp =
              monMap.find(monomial(indices + loc[check[j]] * DIM, DIM))->second;
          if (monomialvalue::conjunction(set, temp) && !choose[j]) {
            monomialvalue::add(set, temp);
            checked[check[j]] = 1;
            tempLoc[num++] = loc[check[j]];
            choose[j] = 1;
          }
        }
      }

      if (num < size) {
        if (NULL != ans){
          cout << "found a split poly" << endl;
        }
        Subpoly_t *tempSub = new Subpoly_t(subpoly->parent, num, tempLoc);
        int preAnsLength = 0;
        if (NULL != ans) preAnsLength = size_L(ans);
        if (!polyIsSOS(tempSub, ans)) { /* necessary condition */
          if (NULL != ans) delListStart(ans, preAnsLength);
          delete[] check;
          delete[] checked;
          delete[] choose;
          delete tempSub;

          return FAILURE;
        }
        delete  tempSub;

        k = 0;

        for (j = 0; j < checkSize; j += 1) {
          if (0 == choose[j]) {
            check[k++] = check[j];
          }
        }
        checkSize = k;
        i = -1;
      }
    }
    i++;
  }

  if (0 == checkSize) {
    delete[] check;
    delete[] checked;
    delete[] choose;

    return SUCCESS;
  }

  if (checkSize < size) {
    for (i = 0; i < checkSize; i += 1) {
      tempLoc[i] = loc[check[i]];
    }
    int preAnsLength = 0;
    if (NULL != ans) preAnsLength = size_L(ans);

    Subpoly_t *tempSub = new Subpoly_t(subpoly->parent, checkSize, tempLoc);
    if (!polyIsSOS(tempSub, ans)) {
      if (NULL != ans) delListStart(ans, preAnsLength);
      delete[] check;
      delete[] checked;
      delete[] choose;

      delete  tempSub;

      return FAILURE;

    } else {
      delete[] check;
      delete[] choose;
      delete[] checked;

      delete tempSub;

      return SUCCESS;
    }
  }

  delete[] check;
  delete[] choose;

  return OTHER;
}

/**
 *
 * test whether a given subpoly is a SOS polynomial

 * The semidefinite programming need data.
 *
 *     s.t. <(*constraints),X > (in dot) = b
 *      X>=0 is a semidefinite matrix.
 *      b is the array of the coefs of poly
 *
 *      X is the matrix [x^ax^b_{a,b}]
 *

 * @param subpoly a mult-variable polynomial
 * @param ans if subpoly is a SOS polynomial the subpoly=\sum_{
 i=0}^s ans[ i ]^2
 }
 * @param verbose print information
 *
 * @return true if find a SOS representation
 * false otherwise
 */

bool SOSChecker::polyIsSOS(Subpoly_t *subpoly, PointList *ans,
                           const int verbose) {
  int i, k, j, sosId, sosLength, gLength, genLength, tempLength = 0;
  bool even;
  indice_t *SOSM = NULL;
  indice_t *GSUP = NULL;
  ConvexGenerator generator(subpoly);
  int check = generator.easyCheck(ans);

  if (CONVEX_POLY == check) {
    
  } else if (NOSOS == check) {
    return false;
  } else if (EXACTLY_SOS == check){
    return true;
  }

  sosId = SUPPORT_TABLE.findSupByPoly(subpoly);
  const indice_t *indices = &(subpoly->parent.indices[0]);
  const vector<int> &loc = subpoly->locs;
  int size = subpoly->size();

  if (sosId > -1) {
    SOSM = SUPPORT_TABLE.getSOSsup(sosId, &sosLength);
    GSUP = SUPPORT_TABLE.getGsup(sosId, &gLength);
  }
  if (NULL == SOSM || NULL == GSUP) {
    start = clock();
    sosId = SUPPORT_TABLE.addconvexsosSup(subpoly);
    indice_t *temp = new indice_t[size * DIM];
    j = 0;

    for (i = 0; i < size; i += 1) {
      even = true;
      for (k = 0; k < DIM; k += 1) {
        if ((indices[loc[i] * DIM + k] & 1) == 1) {
          even = false;
          break;
        }
      }
      if (even) {
        for (k = 0; k < DIM; k += 1) {
          temp[j * DIM + k] = indices[loc[i] * DIM + k] / 2;
        }
        j++;
      }
    }
    genLength = j;

    indice_t *candidate =
        generator.overConvexHull(temp, genLength, DIM, &tempLength);

    int dummy = exactConvHull(subpoly, candidate, tempLength, temp, genLength,
                              &SOSM, sosLength, ans);

    if (SUCCESS == dummy) {
      delete[] temp;
      free(SOSM);
      free(candidate);
      return true;
    } else if (FAILURE == dummy) {
      delete[] temp;
      free(SOSM);
      free(candidate);
      return false;
    }

    GSUP =
        (indice_t *)realloc_d(candidate, tempLength * DIM * sizeof(indice_t));
    qsortM(GSUP, DIM, 0, tempLength - 1, compare);
    gLength = tempLength;

    qsortM(SOSM, DIM, 0, sosLength - 1, compare);
    SUPPORT_TABLE.setGsup(sosId, gLength, GSUP);
    SUPPORT_TABLE.setSOSsup(sosId, sosLength, SOSM);
    delete[] temp;
  }

  Blockmatrix *C = (Blockmatrix *)malloc_d(sizeof(Blockmatrix));

  double **b = (double **)malloc_d(sizeof(double *));

  Constraintmatrix *constraints;
  Blockmatrix X, Z;
  double *y;
  double pobj, dobj;

  /*-----------------------------------------------------------------------------
   *  For every term of poly whose total degree less or equal to total degree of
   *poly
   *  which contain the term whose coefficient eqaul to zero. There is a
   *constraint
   *  related it.
   *-----------------------------------------------------------------------------*/

  constraints =
      createConstraintMatrix(subpoly, sosId, b, SOSM, sosLength, gLength);

  if (constraints != NULL) {
    int bsize[1];

    bsize[0] = gLength;
    /* printf(" bb size %d", blockSize); */

    createBlockMatrixC(bsize, 1, C);

    //		int numofblock=blockSize;

    initsoln(gLength, sosLength, *C, *b, constraints, &X, &y, &Z);
    csdp_verbose = verbose;

    if (0 == easy_sdp(gLength, sosLength, *C, *b, constraints, 0.0, &X, &y, &Z,
                      &pobj, &dobj)) {
      if (NULL != ans)
        sosrepresent(ans, X.blocks[1].data.mat, gLength, sosId, 1e-2);
      free_prob(gLength, sosLength, *C, *b, constraints, X, y, Z);
      free(C);
      free(b);
      return true;
    } else {
      free_prob(gLength, sosLength, *C, *b, constraints, X, y, Z);
      free(C);
      free(b);
      return false;
    }
  } else {
    free(*b);
    free(b);
    return false;
  }
}

bool SOSChecker::easychecksos() {
  if (p->getTotalDegree() <= 0) {
    if (p->isZero() || p->isPositive()){
      return true;
    }
    else{
      return false;
    }
  }

  DIM = getVarTable<indice_t>().getVarNum(p->varId);

  Poly_t *poly = new Poly_t(*p);

  Subpoly_t *subp = new Subpoly_t(*poly);

  PointList *ans = createList(delpoly);

  bool re = polyIsSOS(subp, ans, 1);

  if (re) {
    sosPresent(subp, ans, par.printMinValue);
  }
  delList(ans);
  delete subp;
  delete poly;


  return re;
}
}
}
