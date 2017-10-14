
/**
 * @file   convexhull.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Oct  9 16:36:43 2017
 *
 * @brief
 *
 *
 */

#include "convexhull.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "f2rat.h"
#include "gcd.h"
#include "pca.h"
#include "selfassert.h"
#include "selfmemutil.h"

#include "util.h"
#include "vartable.hpp"

namespace aiSat {
namespace psd {
extern "C" {
/* DGELSD prototype */
extern void dgelsd_(int* m, int* n, int* nrhs, double* a, int* lda, double* b,
                    int* ldb, double* s, double* rcond, int* rank, double* work,
                    int* lwork, int* iwork, int* info);
}
/* Auxiliary routines prototypes */
extern void print_matrix_(char* desc, int m, int n, double* a, int lda);

static long int BOUND = INT_MAX;

static int node_dim;
static size_t node_b_size;



indice_t* overConvexHull(const indice_t* genSet,
                                          const int genLength, const int dim,
                                          int* reLength) {
  ASSERT(genLength > 0, "at least have one point");
  int i, j, k;
  if (1 == genLength) {
    indice_t* candidateSet = (indice_t*)malloc_d(dim);
    for (i = 0; i < dim; i++) candidateSet[i] = genSet[i];
    *reLength = 1;

    return candidateSet;
  }

  node_dim = dim;
  node_b_size = dim * sizeof(indice_t);
  int max[dim];
  int min[dim];
  int temp[dim];
  int maxSum = 0;
  int minSum = 0;
  int tempsum = 0;
  int index;
  int tempLength = 0;
  int pairNum = (dim * (dim - 1)) / 2;
  int threeNum = (dim * (dim - 1) * (dim - 2)) / 6;
  int pair[pairNum * 2];
  int pairMax[pairNum];
  int pairMin[pairNum];

  int three[threeNum * 3];
  int threeMax[threeNum];
  int threeMin[threeNum];
  //	printf ( "genLength %d\n",genLength );

  k = 0;
  for (i = 0; i < dim; i += 1) {
    for (j = i + 1; j < dim; j += 1) {
      pair[2 * k] = i;
      pairMax[k] = genSet[pair[2 * k]];
      pair[2 * k + 1] = j;
      pairMax[k] += genSet[pair[2 * k + 1]];
      pairMin[k] = pairMax[k];
      k++;
    }
  }
  index = 0;

  for (i = 0; i < dim; i += 1) {
    for (j = i + 1; j < dim; j += 1) {
      for (k = j + 1; k < dim; k += 1) {
        three[3 * index] = i;
        threeMax[index] = genSet[i];
        three[3 * index + 1] = j;
        threeMax[index] += genSet[j];
        three[3 * index + 2] = k;
        threeMax[index] += genSet[k];
        threeMin[index] = threeMax[index];
        index++;
      }
    }
  }

  for (i = 0; i < dim; i += 1) {
    max[i] = genSet[i];
    min[i] = genSet[i];
    maxSum += genSet[i];
    minSum += genSet[i];
  }

  for (i = 1; i < genLength; i += 1) {
    tempsum = 0;
    for (j = 0; j < dim; j += 1) {
      tempsum += genSet[i * dim + j];
      if (genSet[i * dim + j] > max[j]) {
        max[j] = genSet[i * dim + j];
      } else if (genSet[i * dim + j] < min[j]) {
        min[j] = genSet[i * dim + j];
      }
    }
    if (tempsum > maxSum)
      maxSum = tempsum;
    else if (tempsum < minSum)
      minSum = tempsum;

    for (j = 0; j < pairNum; j += 1) {
      tempsum = genSet[i * dim + pair[2 * j]];
      tempsum += genSet[i * dim + pair[2 * j + 1]];
      if (tempsum > pairMax[j])
        pairMax[j] = tempsum;
      else if (tempsum < pairMin[j])
        pairMin[j] = tempsum;
    }

    for (j = 0; j < threeNum; j += 1) {
      tempsum = genSet[i * dim + three[3 * j]];
      tempsum += genSet[i * dim + three[3 * j + 1]];
      tempsum += genSet[i * dim + three[3 * j + 2]];
      if (tempsum > threeMax[j])
        threeMax[j] = tempsum;
      else if (tempsum < threeMin[j])
        threeMin[j] = tempsum;
    }
  }
  memcpy(temp, min, sizeof(int) * dim);

  j = 0;
  index = 0;

  while (1) {
    if (temp[0] > max[0]) {
      j = 1;
      while (j < dim && temp[j] == max[j]) j++;
      if (j == dim) {
        break;
      }
      temp[j]++;
      memcpy(temp, min, j * sizeof(int));
    }

    tempsum = 0;

    for (k = 0; k < dim; k += 1) {
      tempsum += temp[k];
    }
    if (tempsum <= maxSum && tempsum >= minSum) {
      for (k = 0; k < pairNum; k += 1) {
        tempsum = temp[pair[2 * k]];
        tempsum += temp[pair[2 * k + 1]];
        if (tempsum > pairMax[k])
          break;
        else if (tempsum < pairMin[k])
          break;
      }
      if (k == pairNum) {
        for (k = 0; k < threeNum; k += 1) {
          tempsum = temp[three[3 * k]];
          tempsum += temp[three[3 * k + 1]];
          tempsum += temp[three[3 * k + 2]];
          if (tempsum > threeMax[k]) {
            break;
          } else if (tempsum < threeMin[k])
            break;
        }
        if (k == threeNum) index++;
      }
    }

    temp[0]++;
  }

  ASSERT(index >= genLength, "there some bugs occor in your this code");
  tempLength = index;

  indice_t* candidateSet = (indice_t*)malloc_d(tempLength * node_b_size);
  memcpy(temp, min, dim * sizeof(int));

  j = 0;
  index = 0;

  while (1) {
    if (temp[0] > max[0]) {
      j = 1;
      while (j < dim && temp[j] == max[j]) j++;
      if (j == dim) {
        break;
      }
      temp[j]++;
      memcpy(temp, min, j * sizeof(int));
    }

    tempsum = 0;

    for (k = 0; k < dim; k += 1) {
      tempsum += temp[k];
    }

    if (tempsum <= maxSum && tempsum >= minSum) {
      for (k = 0; k < pairNum; k += 1) {
        tempsum = temp[pair[2 * k]];
        tempsum += temp[pair[2 * k + 1]];
        if (tempsum > pairMax[k])
          break;
        else if (tempsum < pairMin[k])
          break;
      }
      if (k == pairNum) {
        for (k = 0; k < threeNum; k += 1) {
          tempsum = temp[three[3 * k]];
          tempsum += temp[three[3 * k + 1]];
          tempsum += temp[three[3 * k + 2]];
          if (tempsum > threeMax[k]) {
            break;
          } else if (tempsum < threeMin[k])
            break;
        }
        if (k == threeNum) {
          for (k = 0; k < dim; k += 1) {
            candidateSet[index * dim + k] = temp[k];
          }
          index++;
        }
      }
    }
    temp[0]++;
  }
  if (tempLength == genLength) {
    *reLength = genLength;

  } else {

    tempLength = reduceByLestEignV(candidateSet, tempLength, dim, genSet,
                                   genLength, max);

    if (tempLength == genLength) {
      *reLength = tempLength;
      return candidateSet;
    }

    *reLength = tempLength;

  }
  return candidateSet;
}


int reduceByLestEignV(indice_t* candidateSet,
                                       const int candLength, const int dim,
                                       const indice_t* genSet,
                                       const int genLength, const int max[]) {
  long int bound;
  int maxSum = 0;
  int minSum;
  double dummy;
  int temp;
  int re = candLength;
  int cutNum = dim;
  int j, i, n, m, r, k = 0;
  int origLength = candLength;

  int DUM_DOMAIN = 1;
  int coefNum[dim];
  int coefDen[dim];
  double a[genLength * dim];

  for (i = 0; i < dim; i += 1) {
    for (j = 0; j < genLength; j += 1) {
      a[ijtok(j + 1, i + 1, genLength)] = genSet[j * dim + i];
    }
  }
  double VT1[dim * dim];
  double eigenvector[dim];
  int info = lpca(a, genLength, dim, VT1, TRUE);

  for (r = 0; r < 1 + RANDTIME; r += 1) {
    if (info != -1) {
      for (k = 0; k < dim; k += 1) {
        for (i = 0; i < dim; i += 1) {
          eigenvector[i] = VT1[ijtok(dim - k, i + 1, dim)];
        }
        dummy = 0;

        for (i = 0; i < dim; i += 1) {
          dummy += eigenvector[i] * eigenvector[i];
        }
        dummy = sqrt(dummy);
        dummy /= 16;
        if (dummy < 1e-6) continue; /* the norm2 of eigenvector is too small */
        DUM_DOMAIN = 1;

        for (m = 0; m < DUN_DOMAIN_BOUND; m += 1) {
          for (i = 0; i < dim; i += 1) {
            f2rat(eigenvector[i] / dummy, DUM_DOMAIN, coefDen + i, coefNum + i);
          }
          bound = 1;

          for (i = 0; i < dim; i += 1) {
            if (coefNum[i] != 0) {
              dummy = bound * coefNum[i];
              if (fabs(dummy) >
                  BOUND) { /* the all norm of the coefs must be in a bound */
                bound = 0;
                break;
              }
              bound *= coefNum[i];
            }
          }

          if (0 == bound)
            break; /* the all norm of the coefs must be in a bound */

          for (i = 0; i < dim; i += 1) {
            if (coefNum[i] != 0) {
              coefDen[i] *= bound / coefNum[i];
            } else
              coefDen[i] = 0;
          }

          for (n = 0; n < 10 * cutNum * sqrt(re); n += 1) {
            dummy = 0;
            bound = 0;
            for (i = 0; i < dim; i += 1) {
              dummy += abs(coefDen[i]) * max[i];
              bound += abs(coefDen[i]) * max[i];
            }
            /* the all norm of the coefs must be in a bound */

            if (dummy > BOUND || bound > BOUND ||
                fabs(dummy - bound) > BOUND / 10)
              break;

            maxSum = 0;

            for (i = 0; i < dim; i += 1) {
              maxSum += coefDen[i] * genSet[i];
            }
            minSum = maxSum;

            for (i = 1; i < genLength; i += 1) {
              temp = 0;
              for (j = 0; j < dim; j += 1) {
                temp += coefDen[j] * genSet[i * dim + j];
              }
              if (temp > maxSum) {
                maxSum = temp;
              } else if (temp < minSum) {
                minSum = temp;
              }
            }

            re = reduceByPlane(candidateSet, re, dim, coefDen, maxSum);

            for (i = 0; i < dim; i += 1) {
              coefDen[i] *= -1;
            }

            re = reduceByPlane(candidateSet, re, dim, coefDen, -minSum);
            if (re == origLength) break;
            origLength = re;

            i = rand() % dim;
            if (rand() % 2 == 1) {
              coefDen[i]++;
            } else
              coefDen[i]--;
          }
          if (re == genLength) return re;

          DUM_DOMAIN++;
        }
      }
    }

    for (i = 0; i < dim; i += 1) {
      for (j = 0; j < genLength; j += 1) {
        a[ijtok(j + 1, i + 1, genLength)] =
            candidateSet[(rand() % re) * dim + i];
      }
    }

    info = lpca(a, genLength, dim, VT1, TRUE);
  }

  return re;
}

/**
 * @brief the three points in indices are Different from each other
 *
 * @param subpoly
 * @param indices
 * @param loc
 * @param dim
 * @param coefs
 * @param ans
 *
 * @return
 */
POLY_SOS_T ConvexGenerator::checkThreeP(const int dim, PointList* ans) {
  int i;
  int VA, VB, VM;
  vector<indice_t> key(dim);
  const vector<coef_t>& coefs = subpoly.getParent().coef;
  const vector<int>& locs = subpoly.locs;
  const vector<indice_t>& indices = subpoly.getParent().indices;

  if (isSameLine(&(subpoly.getParent().indices)[0], &(locs)[0], 3,
                 dim)) { /* on a same line */
    if (coefs[locs[0]] < 0) {
      if (coefs[locs[1]] < 0 || coefs[locs[2]] < 0) {
        return NOSOS;
      }
    } else {
      if (coefs[locs[1]] < 0 && coefs[locs[2]] < 0) {
        return NOSOS;
      }
    }
    i = 0;
    while ((indices[locs[0] * dim + i] == indices[locs[1] * dim + i]) &&
           (indices[locs[1] * dim + i] == indices[locs[2] * dim + i]))
      i++;

    int AB = abs(indices[locs[0] * dim + i] - indices[locs[1] * dim + i]);
    int BC = abs(indices[locs[1] * dim + i] - indices[locs[2] * dim + i]);
    int AC = abs(indices[locs[0] * dim + i] - indices[locs[2] * dim + i]);

    if (AC > AB && AC > BC) { /* A C are vertex  */
      VA = 0;
      VB = 2;
      VM = 1;

    } else if (AB > AC && AB > BC) { /* A B are vertex  */
      VA = 0;
      VB = 1;
      VM = 2;

    } else { /* B C are vertex */
      VA = 1;
      VB = 2;
      VM = 0;
    }

    if (coefs[locs[VA]] < 0 || coefs[locs[VB]] < 0) {
      return NOSOS;
    }
    for (i = 0; i < dim; i += 1) {
      if (indices[locs[VA] * dim + i] & 1) {
        return NOSOS;
      }
    }

    for (i = 0; i < dim; i += 1) {
      if (indices[locs[VB] * dim + i] & 1) {
        return NOSOS;
      }
    }

    if (coefs[locs[VM]] < 0) {
      if (fabs(coefs[locs[VM]]) > coefs[locs[VA]] + coefs[locs[VB]]) {
        return NOSOS;
      }
    } else {
      for (i = 0; i < dim; i += 1) {
        if (indices[locs[VM] * dim + i] & 1) {
          break;
        }
      }
      if (i == dim) {
        if (NULL != ans) {
          Poly_t* p = new Poly_t();
          p->changeVarId(subpoly.getParent().getVarId());

          for (i = 0; i < dim; i += 1) {
            key[i] = indices[locs[0] * dim + i] / 2;
          }
          p->add_term(key, sqrtf(coefs[locs[0]]));

          push_back_L(ans, p);

          Poly_t* p1 = new Poly_t();
          p1->changeVarId(subpoly.getParent().getVarId());

          for (i = 0; i < dim; i += 1) {
            key[i] = indices[locs[1] * dim + i] / 2;
          }
          p1->add_term(key, sqrtf(coefs[locs[1]]));

          push_back_L(ans, p1);

          Poly_t* p2 = new Poly_t();

          p2->changeVarId(subpoly.getParent().getVarId());

          for (i = 0; i < dim; i += 1) {
            key[i] = indices[locs[2] * dim + i] / 2;
          }
          p2->add_term(key, sqrtf(coefs[locs[2]]));

          push_back_L(ans, p2);
        }

        return EXACTLY_SOS;
      } else if (coefs[locs[VM]] > coefs[locs[VA]] + coefs[locs[VB]]) {
        return NOSOS;
      }
    }
    return UNHNOW;
  } else {
    if (coefs[locs[0]] < 0 || coefs[locs[1]] < 0 || coefs[locs[2]] < 0)
      return NOSOS;
    for (i = 0; i < dim; i += 1) {
      if (indices[locs[0] * dim + i] & 1) {
        return NOSOS;
      }
    }
    for (i = 0; i < dim; i += 1) {
      if (indices[locs[1] * dim + i] & 1) {
        return NOSOS;
      }
    }
    for (i = 0; i < dim; i += 1) {
      if (indices[locs[2] * dim + i] & 1) {
        return NOSOS;
      }
    }
    if (NULL != ans) {
      Poly_t* p = new Poly_t();
      p->changeVarId(subpoly.getParent().getVarId());

      for (i = 0; i < dim; i += 1) {
        key[i] = indices[locs[0] * dim + i] / 2;
      }
      p->add_term(key, sqrtf(coefs[locs[i]]));

      push_back_L(ans, p);

      Poly_t* p1 = new Poly_t();
      p1->changeVarId(subpoly.getParent().getVarId());

      for (i = 0; i < dim; i += 1) {
        key[i] = indices[locs[1] * dim + i] / 2;
      }
      p1->add_term(key, sqrtf(coefs[locs[i]]));

      push_back_L(ans, p1);

      Poly_t* p2 = new Poly_t();
      p2->changeVarId(subpoly.getParent().getVarId());

      for (i = 0; i < dim; i += 1) {
        key[i] = indices[locs[2] * dim + i] / 2;
      }
      p2->add_term(key, sqrtf(coefs[locs[i]]));

      push_back_L(ans, p2);
    }

    return EXACTLY_SOS;
  }
}

POLY_SOS_T ConvexGenerator::easyCheck(PointList* ans) {
  const int size = subpoly.size();

  const vector<int>& loc = subpoly.locs;
  const Poly_t& poly = subpoly.getParent();
  const vector<indice_t>& indices = poly.indices;
  const vector<coef_t>& pcoefs = poly.coef;

  int dim = getVarTable<indice_t>().getVarNum(poly.getVarId());

  vector<indice_t> key(dim);
  vector<int> coefs(dim);
  vector<char> convexSurf(size, 0);
  vector<char> maxPoint(size);

  int i, j, k;
  int coefDomain = 1;
  int maxSum = 0;
  long int tempSum;
  int number = 0;
  int first = 0;
  int second = 0;
  double planeSum;

  int loc0 = subpoly[0];
  int loc1 = subpoly[1];

  if (size == 1) {
    if (poly.getCF(loc0) < 0) {
      return NOSOS;
    }
    for (i = 0; i < dim; i += 1) {
      if (poly.getDegreeAt(loc0, i) & 1) {
        return NOSOS;
      }
      key[i] = poly.getDegreeAt(loc0, i) / 2;
    }
    if (NULL != ans) {
      Poly_t* p = new Poly_t();
      p->changeVarId(poly.getVarId());
      p->add_term(key, sqrtf(poly.getCF(loc0)));

      push_back_L(ans, p);
    }

    return EXACTLY_SOS;
  } else if (size == 2) {
    if ((poly.getCF(loc0) < 0) || (poly.getCF(loc1) < 0)) {
      return NOSOS;
    }
    for (i = 0; i < dim; i += 1) {
      if (poly.getDegreeAt(loc0, i) & 1) {
        return NOSOS;
      }
    }
    for (i = 0; i < dim; i += 1) {
      if (poly.getDegreeAt(loc1, i) & 1) {
        return NOSOS;
      }
    }
    if (NULL != ans) {
      Poly_t* p = new Poly_t();
      p->changeVarId(poly.getVarId());
      for (i = 0; i < dim; i += 1) {
        key[i] = poly.getDegreeAt(loc0, i) / 2;
      }
      p->add_term(key, sqrtf(poly.getCF(loc0)));

      push_back_L(ans, p);

      Poly_t* p1 = new Poly_t();
      p1->changeVarId(poly.getVarId());

      for (i = 0; i < dim; i += 1) {
        key[i] = poly.getDegreeAt(loc1, i) / 2;
      }
      p1->add_term(key, sqrtf(poly.getCF(loc1)));

      push_back_L(ans, p1);
    }

    return EXACTLY_SOS;
  } else if (size == 3) {
    return checkThreeP(dim, ans);
  }

  for (i = 0; i < size; i += 1) {
    if (pcoefs[loc[i]] < 0) {
      break;
    }

    for (k = 0; k < dim; k += 1) {
      if (indices[loc[i] * dim + k] & 1) {
        break;
      }
    }
    if (k != dim) {
      break;
    }
  }
  if (i == size) {
    if (NULL != ans)
      for (i = 0; i < size; i += 1) {
        Poly_t* p = new Poly_t();
        p->changeVarId(poly.getVarId());

        for (k = 0; k < dim; k += 1) {
          key[k] = indices[loc[i] * dim + k] / 2;
        }
        p->add_term(key, sqrtf(pcoefs[loc[i]]));

        push_back_L(ans, p);
      }

    return EXACTLY_SOS;
  }

  int checkTime = (size)*dim * dim;

  for (i = 0; i < checkTime; i += 1) {
    fill(maxPoint.begin(), maxPoint.end(), 0);

    number = 0;
    coefDomain = sqrt(i) / (2 * size) + 1;

    maxSum = 0;
    while (0 == maxSum) {
      for (j = 0; j < dim; j += 1) {
        coefs[j] = rand() % (2 * coefDomain + 1) - coefDomain;
        if (coefs[j] != 0) maxSum = 1;
      }
    }
    maxSum = 0;

    for (k = 0; k < dim; k += 1) {
      maxSum += coefs[k] * indices[loc[0] * dim + k];
    }

    maxPoint[0] = 1;
    number = 1;
    first = 0;

    for (k = 1; k < size; k += 1) {
      tempSum = 0;

      for (j = 0; j < dim; j += 1) {
        tempSum += coefs[j] * indices[loc[k] * dim + j];
      }
      if (tempSum > maxSum) {
        maxSum = tempSum;
        fill(maxPoint.begin(), maxPoint.end(), 0);

        maxPoint[k] = 1;
        number = 1;
        first = k;
      } else if (tempSum == maxSum) {
        maxPoint[k] = 1;
        number++;
        second = k;
      }
    }
    if (number == 1) {
      convexSurf[first] = 1;
      if (pcoefs[loc[first]] < -1e-6) {
        return NOSOS;
      }
      for (j = 0; j < dim; j += 1) {
        if (indices[loc[first] * dim + j] & 1) {
          return NOSOS;
        }
      }
    } else if (number == 2) {
      convexSurf[first] = 1;
      if (pcoefs[loc[first]] < -1e-6) {
        return NOSOS;
      }
      for (j = 0; j < dim; j += 1) {
        if (indices[loc[first] * dim + j] & 1) {
          return NOSOS;
        }
      }
      convexSurf[second] = 1;
      if (pcoefs[loc[second]] < -1e-6) {
        return NOSOS;
      }
      for (j = 0; j < dim; j += 1) {
        if (indices[loc[second] * dim + j] & 1) {
          return NOSOS;
        }
      }
    } else {
      planeSum = 0;
      for (k = 0; k < size; k += 1) {
        if (1 == maxPoint[k]) {
          convexSurf[k] = 1;
          planeSum += pcoefs[loc[k]];
        }
      }
      if (planeSum < -1e-6) {
        return NOSOS;
      }
    }
  }
  number = 0;
  for (i = 0; i < size; i += 1) {
    if (0 == convexSurf[i]) {
      number++;
    }
  }
  double base = 0.1;
  if ((number + 0.0) / size < base) {
    return CONVEX_POLY;
  } else {
    return UNHNOW;
  }
}

/**
 * @brief
 *   checkPoints denote this term has not been checked.
 *  limit it the least number of term in one surface
 *
 * @param subpoly
 * @param checkPoints
 * @param size
 * @param rePoints
 * @param limit
 *
 * @return
 */
int ConvexGenerator::onSameSurf(const int* checkPoints, const int size,
                                indice_t* rePoints, const int limit) {
  ASSERT(limit >= 1 && size >= limit,
         "candidate must greater or equal to limit");

  const vector<indice_t>& indices = subpoly.getParent().indices;
  int dim = getVarTable<indice_t>().getVarNum(subpoly.getParent().getVarId());

  const vector<int>& loc = subpoly.locs;
  int i, j, m, k, maxSum, tempSum;
  int checkTime = dim * limit * size;
  int number = 0;
  vector<int> choose(limit);
  // int choose[limit];
  long int numBound;
  vector<int> coefsNum(dim);
  vector<int> coefsDen(dim);
  // int coefsNum[dim];
  // int coefsDen[dim];
  vector<double> A(size * dim);
  // double A[size * dim];
  double dummy;
  int info;
  int DUM_DOMAIN = 1, bound = 1;

  for (i = 0; i < dim; i += 1) {
    for (j = 0; j < size; j += 1) {
      A[ijtok(j + 1, i + 1, size)] = indices[loc[checkPoints[j]] * dim + i];
    }
  }
  vector<double> VT(dim * dim);
  // double VT[dim * dim];
  vector<double> eigenvector(dim);
  // double eigenvector[dim];

  info = lpca(&A[0], size, dim, &VT[0], TRUE);

  if (info != -1) {
    for (k = 0; k < dim; k += 1) {
      for (i = 0; i < dim; i += 1) {
        eigenvector[i] = VT[ijtok(k + 1, i + 1, dim)];
      }

      DUM_DOMAIN = 0;

      for (m = 0; m < EIGN_CEF_BOUND; m += 1) {
        memset(rePoints, 0, size * sizeof(indice_t));

        DUM_DOMAIN++;

        for (i = 0; i < dim; i += 1) {
          f2rat(eigenvector[i], DUM_DOMAIN, &coefsDen[i], &coefsNum[i]);
        }

        bound = 1;

        for (i = 0; i < dim; i += 1) {
          if (coefsNum[i] != 0) {
            dummy = bound * coefsNum[i];
            if (fabs(dummy) > LONG_MAX / 2) {
              bound = 0;
              break;
            }
            bound *= coefsNum[i];
          }
        }

        ASSERT(0 != bound, "");

        if (0 == bound) {
          continue;
        }

        for (i = 0; i < dim; i += 1) {
          if (coefsNum[i] != 0) {
            coefsDen[i] = (bound / coefsNum[i]);
          } else {
            coefsDen[i] = 0;
          }
        }
        number = 0;
        maxSum = 0;

        for (i = 0; i < dim; i += 1) {
          maxSum += coefsDen[i] * indices[loc[checkPoints[0]] * dim + i];
        }
        number = 1;
        rePoints[0] = 1;

        for (i = 1; i < size; i += 1) {
          tempSum = 0;

          for (j = 0; j < dim; j += 1) {
            tempSum += coefsDen[j] * indices[loc[checkPoints[i]] * dim + j];
          }
          if (tempSum > maxSum) {
            maxSum = tempSum;
            memset(rePoints, 0, i * sizeof(indice_t));
            rePoints[i] = 1;
            number = 1;
          } else if (tempSum == maxSum) {
            number++;
            rePoints[i] = 1;
          }
        }
        if (number >= limit) {
          return number;
        }
      }
    }
  }

  int M = limit;
  int N = dim;
  int NRHS = 1;
  int LDA = limit;
  int LDB = dim > limit ? dim : limit;
  m = M;
  int n = N, nrhs = NRHS, lda = LDA, ldb = LDB, lwork, rank;
  /* Negative rcond means using default (machine precision) value */
  double rcond = -1.0;
  double wkopt;
  double* work;
  /* Local arrays */
  /* iwork dimension should be at least 3*min(m,n)*nlvl + 11*min(m,n),
     where nlvl = max( 0, int( log_2( min(m,n)/(smlsiz+1) ) )+1 )
     and smlsiz = 25 */
  int iwork[3 * M * 1 + 11 * M];
  double s[M < N ? M : N];
  double a[LDA * N];
  //	double dummy;

  double b[LDB * NRHS];

  for (i = 0; i < checkTime; i += 1) {
    memset(rePoints, 0, size * sizeof(indice_t));
    number = 0;
    j = 0;
    k = 0;
    while (1) {
      k++;
      if (k == size) k = 0;
      if (rePoints[k]) {
        continue;
      }
      if (rand() % size < limit) {
        rePoints[k] = 1;
        choose[number++] = k;
        if (number >= limit) {
          break;
        }
      }
    }

    numBound = dim * dim;

    for (k = 0; k < dim; k += 1) {
      numBound *= (indices[loc[checkPoints[choose[0]]] * dim + k] + 1);
    }

    for (j = 0; j < limit; j += 1) {
      b[j] = 1;
      for (k = 0; k < dim; k += 1) {
        a[k * limit + j] = indices[loc[checkPoints[choose[j]]] * dim + k];
      }
    }

    /* Query and allocate the optimal workspace */
    lwork = -1;
    dgelsd_(&m, &n, &nrhs, a, &lda, b, &ldb, s, &rcond, &rank, &wkopt, &lwork,
            iwork, &info);
    lwork = (int)wkopt;
    work = (double*)malloc(lwork * sizeof(double));
    /* Solve the equations A*X = B */
    dgelsd_(&m, &n, &nrhs, a, &lda, b, &ldb, s, &rcond, &rank, work, &lwork,
            iwork, &info);
    if (info == 0) { /* success */
      for (k = 0; k < dim; k += 1) {
        f2rat(b[k], numBound, &coefsDen[k], &coefsNum[k]);
      }
      numBound = 1;

      for (k = 0; k < dim; k += 1) {
        if (coefsNum[k] != 0) {
          dummy = numBound * coefsNum[k];
          if (fabs(dummy) > LONG_MAX / 2) {
            numBound = 0;
            break;
          }
          numBound *= coefsNum[k];
        }
      }
      if (0 == numBound) {
        continue;
      }

      for (k = 0; k < dim; k += 1) {
        if (coefsNum[k] != 0) {
          coefsDen[k] *= (numBound / coefsNum[k]);
        } else {
          coefsDen[k] = 0;
        }
      }

      for (k = 0; k < limit; k += 1) {
        tempSum = 0;
        for (j = 0; j < dim; j += 1) {
          tempSum +=
              coefsDen[j] * indices[loc[checkPoints[choose[k]]] * dim + j];
        }
        if (tempSum != numBound) {
          break;
        }
      }

      if (tempSum != numBound) {
        continue;
      }

      k = 0;
      number = limit;
      while (k < size) {
        while (1 == rePoints[k] && k < size) k++;

        if (k == size) {
          return number;
        }
        tempSum = 0;
        for (j = 0; j < dim; j += 1) {
          tempSum += coefsDen[j] * indices[loc[checkPoints[k]] * dim + j];
        }
        k++;
        if (tempSum == numBound) {
          number++;
          rePoints[k - 1] = 1;

        } else {
          break;
        }
      }
      if (k == size) {
        return number;
      }

      if (tempSum > numBound) {
        for (; k < size; k += 1) {
          if (0 == rePoints[k]) {
            tempSum = 0;
            for (j = 0; j < dim; j += 1) {
              tempSum += coefsDen[j] * indices[loc[checkPoints[k]] * dim + j];
            }
            if (tempSum < numBound) {
              break;
            } else if (tempSum == numBound) {
              rePoints[k] = 1;
              number++;
            }
          }
        }
        if (tempSum < numBound) {
          continue;
        }

        return number;

      } else if (tempSum < numBound) {
        for (; k < size; k += 1) {
          if (0 == rePoints[k]) {
            tempSum = 0;
            for (j = 0; j < dim; j += 1) {
              tempSum += coefsDen[j] * indices[loc[checkPoints[k]] * dim + j];
            }
            if (tempSum > numBound) {
              break;
            } else if (tempSum == numBound) {
              rePoints[k] = 1;
              number++;
            }
          }
        }
        if (tempSum > numBound) {
          continue;
        }

        return number;
      }
    }

    free((void*)work);
  }
  return -1;
}

indice_t* ConvexGenerator::randPointSet(const int length, const int dim,
                                        const int domain) {
  indice_t* re = (indice_t*)malloc_d(length * dim * sizeof(indice_t));
  int i, j;

  for (i = 0; i < length; i += 1) {
    for (j = 0; j < dim; j += 1) {
      re[i * dim + j] = rand() % domain;
    }
  }
  return re;
}

/**
 * @brief reduce the point in candidateSet whose sum of index less or equal to
 * maxSum
 *
 * @param candidateSet
 * @param length
 * @param dim
 * @param coefs
 * @param maxSum
 *
 * @return
 */
int reduceByPlane(indice_t* candidateSet, const int length,
                                   const int dim, const int* coefs,
                                   int maxSum) {
  int i, j;
  int tempSum = 0;
  int len = length;

  for (i = 0; i < len; i += 1) {
    tempSum = 0;

    for (j = 0; j < dim; j += 1) {
      tempSum += coefs[j] * candidateSet[i * dim + j];
    }

    if (tempSum > maxSum) {
      memmove(candidateSet + i * dim, candidateSet + (i + 1) * dim,
              (len - i - 1) * node_b_size);
      len--;
      i--;
    }
  }
  return len;
}




}
}
