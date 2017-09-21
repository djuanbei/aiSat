/*
 * =====================================================================================
 *
 *       Filename:  poly.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年05月15日 15时23分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "poly.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "crypt_md5.h"
#include "pointlist.h"
#include "search.h"
#include "selfassert.h"
#include "sort.h"
#include "util.h"

SOSProblem *createSOSProblem(void) {
  SOSProblem *re;
  re = (SOSProblem *)malloc_d(sizeof(SOSProblem));

  re->cap = DEFAULT_SOS_PRO_SIZE;

  re->size = 0;
  re->rhs = NULL;
  re->polys = (Poly **)calloc_d(DEFAULT_SOS_PRO_SIZE, sizeof(Poly *));

  re->polyConstraints = (PolyConstraint **)calloc_d(DEFAULT_SOS_PRO_SIZE,
                                                    sizeof(PolyConstraint *));
  return re;
}

SOSProblem *createSOSProblemN(int size) {
  ASSERT(size >= 0, "");

  SOSProblem *re;
  re = (SOSProblem *)malloc_d(sizeof(SOSProblem));

  re->rhs = NULL;

  re->cap = size;
  re->size = size;
  re->polys = (Poly **)calloc_d(size, sizeof(Poly *));

  re->polyConstraints =
      (PolyConstraint **)calloc_d(size, sizeof(PolyConstraint *));
  return re;
}
void clearSOSProblem(SOSProblem *sys) {
  if (sys == NULL) return;

  if (NULL != sys->rhs) deletePoly(sys->rhs);

  sys->rhs = NULL;
  if (sys->polys != NULL) {
    int i;
    for (i = 0; i < sys->size; i += 1) {
      deletePoly(sys->polys[i]);
    }
  }
  //  sys->polys=NULL;
  if (sys->polyConstraints != NULL) {
    int i;
    for (i = 0; i < sys->size; i += 1) {
      deletePolyConstraint(sys->polyConstraints[i]);
    }
  }
  //  sys->polyConstraints=NULL;
  sys->size = 0;
}
void setConstraint(SOSProblem *sys, const int index, Poly *poly,
                   PolyConstraint *cons) {
  ASSERT(index >= 0, "undeflow");
  ASSERT(index < sys->size, "overflow");
  if (NULL != sys->polys[index]) deletePoly(sys->polys[index]);
  if (NULL != sys->polyConstraints[index])
    deletePolyConstraint(sys->polyConstraints[index]);
  sys->polys[index] = poly;
  sys->polyConstraints[index] = cons;
}

int addConstraint(SOSProblem *sys, Poly *poly, PolyConstraint *cons) {
  while (sys->size >= sys->cap) {
    sys->cap++;
    sys->cap *= ENLARGE_RAT;
    sys->polys = (Poly **)realloc_d(sys->polys, sys->cap * sizeof(Poly *));
    sys->polyConstraints = (PolyConstraint **)realloc_d(
        sys->polyConstraints, sys->cap * sizeof(PolyConstraint));
  }
  sys->polys[sys->size] = poly;
  sys->polyConstraints[sys->size] = cons;
  sys->size++;
  return sys->size - 1;
}

void setSOSRhs(SOSProblem *sys, Poly *rhs) {
  if (NULL != sys->rhs) deletePoly(sys->rhs);
  sys->rhs = rhs;
}

void deleteSOSProblem(SOSProblem *sys) {
  if (sys == NULL) return;

  if (NULL != sys->rhs) deletePoly(sys->rhs);

  if (sys->polys != NULL) {
    int i;
    for (i = 0; i < sys->size; i += 1) {
      deletePoly(sys->polys[i]);
    }
    free(sys->polys);
  }
  if (sys->polyConstraints != NULL) {
    int i;
    for (i = 0; i < sys->size; i += 1) {
      deletePolyConstraint(sys->polyConstraints[i]);
    }
    free(sys->polyConstraints);
  }
  sys->size = 0;
  free(sys);
}

Poly *createPoly(void) {
  Poly *re;
  re = (Poly *)malloc_d(sizeof(Poly));
  re->capacity = DEFAULT_POLY_CAP;
  re->indices = (indice_t *)calloc_d(re->capacity, sizeof(indice_t));
  re->coef = (coef_t *)calloc_d(re->capacity, sizeof(coef_t));
  re->varId = 0; /* default const polynomial */
  re->size = 0;

  return re;
}

Poly *randPoly(const int varId, const coef_t coefDome, const int termLength,
               const int indiceDome) {
  int i, j;
  coef_t cf;
  const int varSize = getvarNum(varId);

  Poly *re;

  re = (Poly *)malloc_d(sizeof(Poly));

  re->capacity = DEFAULT_POLY_CAP;

  re->indices = (indice_t *)calloc_d(varSize * re->capacity, sizeof(indice_t));

  re->coef = (coef_t *)calloc_d(re->capacity, sizeof(coef_t));

  re->varId = varId; /* default const polynomial */
  re->size = 0;

  indice_t key[getvarNum(varId)];

  int cfD = (int)coefDome;

  for (i = 0; i < termLength; i += 1) {
    cf = rand() % (2 * cfD) - cfD;

    for (j = 0; j < getvarNum(varId); j += 1) {
      key[j] = rand() % indiceDome;
    }
    internal_addTerm(re, key, cf);
  }

  updatePoly(re);

  return re;
}


/** 
 * @brief  poly is constant can not delete
 * 
 * @param poly 
 * 
 * @return 
 */
Poly *copyPoly(const Poly *poly) {
  Poly *re;
  re = (Poly *)malloc_d(sizeof(Poly));

  re->capacity = poly->capacity;
  re->size = poly->size;
  re->varId = poly->varId;
  const int varSize = getvarNum(re->varId);

  re->indices = (indice_t *)calloc_d(re->capacity * varSize, sizeof(indice_t));

  memcpy(re->indices, poly->indices, re->size * varSize * sizeof(indice_t));

  re->coef = (coef_t *)calloc_d(re->capacity, sizeof(coef_t));

  memcpy(re->coef, poly->coef, re->size * sizeof(coef_t));

  return re;
}


/** 
 * @brief enlarge or reduce poly's vars
 *Notice keep capacity and size
 * if reduce I do not deal with the indice who variable is not exist
 * @param poly 
 * @param nvarId 
 */
void polyChangeVarId(Poly *const poly, const int nvarId) {
  int i, j;

  if (poly->varId == nvarId) return;
  /* 	if(0==poly->size){
   * 		poly->varId=nvarId;
   * 		return;
   * 	}
   */

  const int oldSize = getvarNum(poly->varId);
  const int newSize = getvarNum(nvarId);
  int map[oldSize];

  getconvertMap(poly->varId, nvarId, map);

  poly->varId = nvarId;

  if (newSize > oldSize) {
    indice_t temp[newSize];

    poly->indices = (indice_t *)realloc_d(
        poly->indices, poly->capacity * newSize * sizeof(indice_t));

    /* 		memset((poly->indices)+poly->size*oldSize, 0 ,
     * 						sizeof(indice_t)*(poly->capacity)*(newSize-oldSize));
     *
     */
    /*-----------------------------------------------------------------------------
     *  can improve effective
     *-----------------------------------------------------------------------------*/

    for (i = poly->size - 1; i >= 0; i -= 1) {
      for (j = 0; j < newSize; j += 1) {
        temp[j] = 0;
      }

      for (j = 0; j < oldSize; j += 1) {
        if (map[j] > -1) {
          temp[map[j]] = poly->indices[i * oldSize + j];
        }
      }

      for (j = 0; j < newSize; j += 1) {
        poly->indices[i * newSize + j] = temp[j];
      }
    }

  } else if (newSize < oldSize) {
    indice_t temp[newSize];

    for (i = 0; i < poly->size; i += 1) {
      for (j = 0; j < newSize; j += 1) {
        temp[j] = 0;
      }

      for (j = 0; j < oldSize; j += 1) {
        if (map[j] > -1) {
          temp[map[j]] = poly->indices[i * oldSize + j];
        }
      }

      for (j = 0; j < newSize; j += 1) {
        poly->indices[i * newSize + j] = temp[j];
      }
    }

    /* Erase history */
    memset((poly->indices) + poly->size * newSize, 0,
           sizeof(indice_t) * (poly->size) * (oldSize - newSize));

  } else {
    /*-----------------------------------------------------------------------------
     *  I can much better in here
     *-----------------------------------------------------------------------------*/
    indice_t temp[newSize];

    for (i = 0; i < poly->size; i += 1) {
      for (j = 0; j < newSize; j += 1) {
        temp[j] = 0;
      }

      for (j = 0; j < newSize; j += 1) {
        if (map[j] > -1) {
          temp[map[j]] = poly->indices[i * oldSize + j];
        }
      }

      for (j = 0; j < newSize; j += 1) {
        poly->indices[i * oldSize + j] = temp[j];
      }
    }
  }
}

int poly_getTotalDegree(const Poly *poly) {
  int re = 0;
  int temp, i, j;
  int varNum = getvarNum(poly->varId);

  for (i = 0; i < poly->size; i += 1) {
    temp = 0;

    for (j = 0; j < varNum; j += 1) {
      temp += poly->indices[i * varNum + j];
    }
    if (temp > re) re = temp;
  }
  return re;
}

void enlargePoly(Poly *const poly) {
  poly->capacity = (poly->capacity) * ENLARGE_RAT + 1;
  poly->indices = (indice_t *)realloc_d(
      poly->indices,
      getvarNum(poly->varId) * (poly->capacity) * sizeof(indice_t));

  poly->coef = (coef_t *)realloc_d(poly->coef, poly->capacity * sizeof(coef_t));
}


/** 
 * @brief  combine the terms whoes indices are same
 * 
 * @param poly 
 */
void updatePoly(Poly *const poly) {
  int i, j, k;
  if (0 == poly->size) return;

  const int n = getvarNum(poly->varId);
  qsortT(poly->coef, poly->indices, n, 0, poly->size - 1, compare);

  j = 0;

  for (i = 1; i < poly->size; i += 1) {
    for (k = 0; k < n; k += 1) {
      if (poly->indices[j * n + k] != poly->indices[i * n + k]) break;
    }
    if (n == k) {
      poly->coef[j] += poly->coef[i];
    } else if (i == j + 1) {
      j++;

    } else {
      j++;
      for (k = 0; k < n; k += 1) {
        poly->indices[j * n + k] = poly->indices[i * n + k];
      }
      poly->coef[j] = poly->coef[i];
    }
  }
  poly->size = j + 1;
}

void printPoly(Poly *const poly) {
  int i, j;
  int isZero = 1;

  updatePoly(poly);

  indice_t *vars = getvarElem(poly->varId);

  const int n = (int)vars[0];

  char *varname[n];
  if (0 == poly->size) {
    printf(" 0");
    return;
  }

  for (i = 0; i < n; i += 1) {
    varname[i] = getvarName((int)vars[i + 1]);
  }

  for (i = 0; i < poly->size; i += 1) {
    if (poly->coef[i] > MIN_COEF || poly->coef[i] < -MIN_COEF) {
      if (i == 0) {
        printf("%.4f", poly->coef[i]);
        isZero = 0;
      } else if (poly->coef[i] > 0.0) {
        printf(" +%.4f", poly->coef[i]);
        isZero = 0;
      } else if (poly->coef[i] < 0.0) {
        isZero = 0;
        printf(" %.4f", poly->coef[i]);
      }

      for (j = 0; j < n; j += 1) {
        if (poly->indices[i * n + j] > 0) {
          if (poly->indices[i * n + j] > 1)
            printf("*%s^%d", varname[j], poly->indices[i * n + j]);
          else
            printf("*%s", varname[j]);
        }
      }
    }
  }
  if (isZero) printf("0 ");
}

void printPolyPrec(Poly *const poly, const float minv) {
  int i, j;
  int first = 1;

  indice_t *vars = getvarElem(poly->varId);

  const int n = (int)vars[0];

  char format[10];
  if (minv >= 1) {
    format[0] = '%';
    format[1] = 'f';
    format[2] = '\0';
  } else {
    format[0] = '%';
    format[1] = '.';
    format[2] = '1' - log10(minv) - 1;
    format[3] = 'f';
    format[4] = '\0';
  }

  char *varname[n];

  if (0 == poly->size) printf(" 0 \n");

  for (i = 0; i < n; i += 1) {
    varname[i] = getvarName((int)vars[i + 1]);
  }

  for (i = 0; i < poly->size; i += 1) {
    if (poly->coef[i] > minv || poly->coef[i] < -minv) {
      if (i == 0) {
        printf(format, poly->coef[i]);
      } else if (poly->coef[i] > 0.0) {
        if (!first) printf("+");
        printf(format, poly->coef[i]);
      } else if (poly->coef[i] < 0.0) {
        printf(format, poly->coef[i]);
      }

      for (j = 0; j < n; j += 1) {
        if (poly->indices[i * n + j] > 0) {
          if (poly->indices[i * n + j] > 1)
            printf("*%s^%d", varname[j], poly->indices[i * n + j]);
          else if (1 == poly->indices[i * n + j])
            printf("*%s", varname[j]);
        }
      }
      first = 0;
    }
  }

}

void printPoly1(const Poly *poly) {
  int i, j;

  indice_t *vars = getvarElem(poly->varId);

  const int n = (int)vars[0];

  char *varname[n];
  if (0 == poly->size) printf(" 0 \n");

  for (i = 0; i < n; i += 1) {
    varname[i] = getvarName((int)vars[i + 1]);
  }

  for (i = 0; i < poly->size; i += 1) {
    if (abs(poly->coef[i]) > OUTPUT_MINVALUE) {
      if (i == 0) {
        printf("%d", (int)(OUTPUT_TIME * poly->coef[i]));
      } else if (poly->coef[i] > 0.0) {
        printf(" +%d", (int)(OUTPUT_TIME * poly->coef[i]));

      } else if (poly->coef[i] < 0.0) {
        printf(" %d", (int)(OUTPUT_TIME * poly->coef[i]));
      }

      for (j = 0; j < n; j += 1) {
        if (poly->indices[i * n + j] > 0) {
          if (poly->indices[i * n + j] > 1)
            printf("*%s^%d", varname[j], poly->indices[i * n + j]);
          else if (poly->indices[i * n + j] == 1)
            printf("*%s", varname[j]);
        }
      }
    }
  }
  printf("\n");
}

void deletePoly(Poly *poly) {
  free(poly->coef);
  free(poly->indices);
  poly->coef = NULL;
  poly->indices = NULL;
  poly->size = poly->capacity = 0;

  free(poly);
}

void assign_del(Poly *lhs, Poly *rhs) {
  updatePoly(rhs);
  free(lhs->coef);

  free(lhs->indices);
  lhs->size = rhs->size;
  lhs->capacity = rhs->capacity;
  lhs->coef = rhs->coef;
  lhs->indices = rhs->indices;
  rhs->coef = NULL;
  rhs->indices = NULL;
  rhs->size = rhs->capacity = 0;
  free(rhs);
}

void resetZero(Poly *poly) { poly->size = 0; }

void resetOne(Poly *poly) {
  poly->size = 1;
  poly->coef[0] = 1;
  int varNum = getvarNum(poly->varId);
  int i;
  for (i = 0; i >= varNum; i -= 1) {
    poly->indices[i] = 0;
  }
}

BOOL isZero(Poly *poly) {
  updatePoly(poly);

  if (poly->size == 0) return TRUE;
  if (poly->size == 1 && poly->coef[0] == 0) return TRUE;

  return FALSE;
}
BOOL isOne(Poly *poly) {
  updatePoly(poly);
  if (poly->size != 1) return FALSE;
  if (poly->coef[0] == 1) {
    int varNum = getvarNum(poly->varId);
    int i;
    for (i = 0; i < varNum; ++i) {
      if (poly->indices[i] != 0) return FALSE;
    }
    return TRUE;
  }

  return FALSE;
}

BOOL isConstant(Poly *poly) {
  updatePoly(poly);
  if (poly->size != 1) return FALSE;

  int varNum = getvarNum(poly->varId);
  int i;
  for (i = 0; i < varNum; ++i) {
    if (poly->indices[i] != 0) return FALSE;
  }
  return TRUE;
}
BOOL isPositive(Poly *poly) {
  if (isZero(poly)) return TRUE;

  if (poly->coef[0] >= 0 && isConstant(poly)) return TRUE;
  return FALSE;
}

BOOL isNegative(Poly *poly) {
  if (isZero(poly)) return TRUE;

  if (poly->coef[0] <= 0 && isConstant(poly)) return TRUE;
  return FALSE;
}


/** 
 * @brief  n is number of variable
 *  poly doesn't contain key
 * 
 * @param poly 
 * @param key 
 * @param cf 
 */
void internal_addTerm(Poly *const poly, indice_t key[], const coef_t cf) {
  int i;
  const int varSize = getvarNum(poly->varId);

  if (poly->size == poly->capacity) {
    enlargePoly(poly); /* coef contain */
  }

  for (i = 0; i < varSize; i += 1) {
    poly->indices[(poly->size) * varSize + i] = key[i];
  }
  poly->coef[poly->size] = cf;
  poly->size++;
}


/** 
 * @brief add and keep sort indices
 * 
 * @param poly 
 * @param key 
 * @param cf 
 */
void p_add_Term_assign(Poly *const poly, indice_t const key[],
                       const coef_t cf) {
  int i, j;

  const int varSize = getvarNum(poly->varId);

  /* 	assert(varSize==n);
   */

  if (0 == poly->size) {
    poly->coef[0] = cf;

    for (i = 0; i < varSize; i += 1) {
      poly->indices[i] = key[i];
    }
    poly->size = 1;
    return;
  }

  int location = findIndex(key, poly->indices, poly->size, varSize);

  ASSERT(location < poly->size, "");

  if (location > -1) {
    poly->coef[location] += cf;
    return;
  } else {
    location = findLocationBettwen(key, poly->indices, poly->size, varSize);

    if (poly->size == poly->capacity) enlargePoly(poly);

    for (i = poly->size; i > location; i -= 1) {
      for (j = 0; j < varSize; j += 1) {
        poly->indices[i * varSize + j] =
            poly->indices[i * varSize - varSize + j];
      }
      poly->coef[i] = poly->coef[i - 1];
    }

    for (j = 0; j < varSize; j += 1) {
      poly->indices[location * varSize + j] = key[j];
    }
    poly->coef[location] = cf;
    poly->size++;
  }
}

void p_add_cons_assign(Poly *const poly, const coef_t cf) {
  int i;
  const int n = getvarNum(poly->varId);
  indice_t key[n];

  for (i = 0; i < n; i += 1) {
    key[i] = 0;
  }
  p_add_Term_assign(poly, key, cf);
}

Poly *p_mult_Term(Poly const *poly, const indice_t key[], const coef_t cf) {
  Poly *re = copyPoly(poly);

  p_mult_Term_assign(re, key, cf);

  return re;
}

void p_mult_Term_assign(Poly *const poly, indice_t const key[],
                        const coef_t cf) {
  int i, j;

  const int n = getvarNum(poly->varId);

  for (i = 0; i < poly->size; i += 1) {
    for (j = 0; j < n; j += 1) {
      poly->indices[i * n + j] += key[j];
    }
    poly->coef[i] *= cf;
  }
}

void p_mult_cons_assign(Poly *const poly, const coef_t cf) {
  int i;

  for (i = 0; i < poly->size; i += 1) {
    poly->coef[i] *= cf;
  }
}


/** 
 * @brief poly1=poly1+poly2
 *  does not delete poly2
 * 
 * @param poly1 
 * @param poly2 
 */
void p_add_Poly_assign(Poly *const poly1, Poly *const poly2) {
  int i, j;
  int varId;
  if (poly1->varId == poly2->varId)
    varId = poly1->varId;
  else {
    varId = mergeVar(poly1->varId, poly2->varId);
    polyChangeVarId(poly1, varId);
  }

  indice_t *vars = getvarElem(varId);

  const int varSize = (int)vars[0];

  indice_t key[varSize];

  const int p2Size = getvarNum(poly2->varId);

  int map[p2Size];

  getconvertMap(poly2->varId, varId, map);

  for (i = 0; i < poly2->size; i += 1) {
    if (poly2->coef[i] == 0) continue;

    for (j = 0; j < varSize; j += 1) {
      key[j] = 0;
    }

    for (j = 0; j < p2Size; j += 1) {
      if (map[j] > -1) key[map[j]] = poly2->indices[i * p2Size + j];
    }

    p_add_Term_assign(poly1, key, poly2->coef[i]);
  }
}


/** 
 * @brief    poly1=poly1+poly2
 *   delete poly2
 * 
 * @param poly1 
 * @param poly2 
 */
void p_add_Poly_assign_del(Poly *const poly1, Poly *const poly2) {
  p_add_Poly_assign(poly1, poly2);

  deletePoly(poly2);
}


/** 
 * @brief poly1=poly1*poly2
 * 
 * @param poly1 
 * @param poly2 
 */
void p_mult_Poly_assign(Poly *poly1, Poly *const poly2) {
  int i, j;

  Poly *re = createPoly();

  int varId = mergeVar(poly1->varId, poly2->varId);

  polyChangeVarId(re, varId);

  indice_t *vars = getvarElem(varId);

  const int varSize = (int)vars[0];

  indice_t key[varSize];

  if (poly1->size >= poly2->size) {
    Poly *poly11 = copyPoly(poly1);

    polyChangeVarId(poly11, varId);

    const int p2Size = getvarNum(poly2->varId);

    int map[p2Size];

    getconvertMap(poly2->varId, varId, map);


    for (i = 0; i < poly2->size; i += 1) {
      if (poly2->coef[i] == 0) continue;
      for (j = 0; j < varSize; j += 1) {
        key[j] = 0;
      }
      for (j = 0; j < p2Size; j += 1) {
        if (map[j] > -1) key[map[j]] = poly2->indices[i * p2Size + j];
      }

      p_add_Poly_assign_del(re, p_mult_Term(poly11, key, poly2->coef[i]));
    }

    deletePoly(poly11);

  } else {
    Poly *poly22 = copyPoly(poly2);

    polyChangeVarId(poly22, varId);

    const int p1Size = getvarNum(poly1->varId);

    int map[p1Size];

    getconvertMap(poly1->varId, varId, map);


    for (i = 0; i < poly1->size; i += 1) {
      if (poly1->coef[i] == 0) continue;
      for (j = 0; j < varSize; j += 1) {
        key[j] = 0;
      }
      for (j = 0; j < p1Size; j += 1) {
        if (map[j] > -1) key[map[j]] = poly1->indices[i * p1Size + j];
      }

      p_add_Poly_assign_del(re, p_mult_Term(poly22, key, poly1->coef[i]));
    }

    deletePoly(poly22);
  }
  assign_del(poly1, re);
}
/*
 * delete poly2
 */

void p_mult_Poly_assign_del(Poly *poly1, Poly *const poly2) {
  p_mult_Poly_assign(poly1, poly2);
  free(poly2);
}


/** 
 * 
 * 
 * @param poly1 
 * @param poly2 
 * 
 * @return poly1*poly2
 */
Poly *p_mult_Poly(const Poly *poly1, const Poly *poly2) {
  int i, j;

  Poly *re = createPoly();

  int varId = mergeVar(poly1->varId, poly2->varId);

  polyChangeVarId(re, varId);

  indice_t *vars = getvarElem(varId);

  const int varSize = (int)vars[0];

  indice_t key[varSize];

  if (poly1->size >= poly2->size) {
    Poly *poly11 = copyPoly(poly1);

    polyChangeVarId(poly11, varId);

    const int p2Size = getvarNum(poly2->varId);

    int map[p2Size];

    getconvertMap(poly2->varId, varId, map);


    for (i = 0; i < poly2->size; i += 1) {
      if (poly2->coef[i] == 0) continue;
      for (j = 0; j < varSize; j += 1) {
        key[j] = 0;
      }
      for (j = 0; j < p2Size; j += 1) {
        if (map[j] > -1) key[map[j]] = poly2->indices[i * p2Size + j];
      }

      p_add_Poly_assign_del(re, p_mult_Term(poly11, key, poly2->coef[i]));
    }

    deletePoly(poly11);

  } else {
    Poly *poly22 = copyPoly(poly2);

    polyChangeVarId(poly22, varId);

    const int p1Size = getvarNum(poly1->varId);

    int map[p1Size];

    getconvertMap(poly1->varId, varId, map);


    for (i = 0; i < poly1->size; i += 1) {
      if (poly1->coef[i] == 0) continue;
      for (j = 0; j < varSize; j += 1) {
        key[j] = 0;
      }
      for (j = 0; j < p1Size; j += 1) {
        if (map[j] > -1) key[map[j]] = poly1->indices[i * p1Size + j];
      }

      p_add_Poly_assign_del(re, p_mult_Term(poly22, key, poly1->coef[i]));
    }

    deletePoly(poly22);
  }

  return re;

}

void p_pow_assign(Poly *poly, const int p) {
  ASSERT(p >= 0, "");

  if (1 == p) return;

  if (isOne(poly) || isZero(poly)) return;

  if (0 == p) {
    resetOne(poly);
    return;
  }

  int i;
  if (poly->size == 1) {
    int varNum = getvarNum(poly->varId);

    for (i = 0; i < varNum; i++) {
      poly->indices[i] *= p;
    }
    poly->coef[0] = pow(poly->coef[0], p);

    return;
  }

  Poly *dummy = copyPoly(poly);

  for (i = 0; i < p - 1; i++) {
    p_mult_Poly_assign(poly, dummy);
  }
  deletePoly(dummy);
}

/** 
 * @brief  list1 and list2 are order list.  re is the list merge list1
 * and list2. list1 and list2 doesn't delete after this function.
 *  re is always new allow memmory
 * 
 * @param list1 
 * @param list2 
 * @param size1 
 * @param size2 
 * @param size3 
 * @param n 
 * 
 * @return 
 */
indice_t **mergeSort(indice_t **const list1, indice_t **const list2,
                     const int size1, const int size2, int *const size3,
                     int const n) {
  int i, j;
  int com;
  indice_t **re;
  *size3 = 0;


  i = 0;
  j = 0;

  re = (indice_t **)calloc_d((size1 + size2), sizeof(indice_t *));

  while (i < size1 && j < size2) {
    com = compare(list1[i], list2[j], n);

    if (com == -1) {
      re[(*size3)++] = list1[i];

      i++;

    } else if (com == 1) {
      re[(*size3)++] = list2[j];
      j++;
    } else {
      re[(*size3)++] = list1[i];
      i++;
      j++;
    }
  }

  while (i < size1) {
    re[(*size3)++] = list1[i];
    i++;
  }

  while (j < size2) {
    re[(*size3)++] = list2[j];
    j++;
  }

  return re;
}

PolyConstraint *createPolyConstraint(void) {
  PolyConstraint *re;

  re = (PolyConstraint *)malloc_d(sizeof(PolyConstraint));
  return re;
}

PolyConstraint *copyPolyConstraint(const PolyConstraint *polycons) {
  PolyConstraint *re = createPolyConstraint();
  re->type = polycons->type;
  re->supportId = polycons->supportId;
  return re;
}

void printPolyConstraint(PolyConstraint *const cpoly) {
  printSupport(cpoly->supportId);
  if (cpoly->type == EQ)
    printf("==0\n");
  else if (cpoly->type == GE)
    printf(">=0 \n");

  else if (cpoly->type == GT)
    printf(">\n");

  else if (cpoly->type == NE)
    printf("!=0\n");
}

void deletePolyConstraint(PolyConstraint *cpoly) {
  free(cpoly);
  cpoly = NULL;
}

MonomialConstraint *createConstraint(void) {
  MonomialConstraint *re;

  re = (MonomialConstraint *)malloc_d(sizeof(MonomialConstraint));

  re->linCoefs = NULL;

  return re;
}

void deleteConstraint(MonomialConstraint *const cons) {
  free(cons->linCoefs);
  free(cons);
}

Support *createSupport(const int deg, const int varId, const int consNum,
                       int *consId) {
  Support *re;
  re = (Support *)malloc_d(sizeof(Support));
  re->type = NORMAL;

  re->varId = -1;
  memset(re->md5sum, 0, DIGEST_SIZE);

  /*-----------------------------------------------------------------------------
   *  have some problems
   *-----------------------------------------------------------------------------*/
  re->md5sum[0] = (uint8_t)deg;
  re->md5sum[1] = (uint8_t)varId;
  re->md5sum[2] = (uint8_t)consNum;

  re->deg = deg;
  re->varId = varId;
  re->constNum = consNum;
  re->consId = consId;
  re->consCap = consNum;
  return re;
}

Support *createSupBySup(const int varId, const indice_t *indices,
                        const int size) {
  Support *re;

  re = (Support *)malloc_d(sizeof(Support));
  re->type = INDICE;
  md5sumbyIndice(re->md5sum, varId, indices, size);
  re->varId = varId;
  return re;
}

Support *createSupByPoly(SubPoly *subpoly) {
  Support *re;
  re = (Support *)malloc_d(sizeof(Support));

  re->type = SUB_POLY;
  re->subp = subpoly;

  memcpy(re->md5sum, subpoly->md5sum, DIGEST_SIZE);

  re->deg = subpoly_getTotalDegree(subpoly);
  re->varId = subpoly->poly->varId;
  re->constNum = 0;
  re->consId = NULL;
  re->consCap = 0;
  return re;
}

void printSupport(const int id) {
  int length;

  indice_t *indices = getSOSsup(id, &length);

  if (indices == NULL) {
    printf("unknow\n");
    return;
  } else {
    int i, k;
    indice_t *vars = getvarElem(id);
    const int n = (int)vars[0];
    char *varname[n];

    for (i = 0; i < n; i += 1) {
      varname[i] = getvarName(vars[i + 1]);
    }
    const int len = getsosSLength(id);

    for (i = 0; i < len; i += 1) {
      if (i != 0 && i % 5 == 0) printf("\n");
      if (i != 0) printf(" + a");
      for (k = 0; k < n; k += 1) {
        printf("%d", indices[i * n + k]);
      }
      for (k = 0; k < n; k += 1) {
        printf("%s^%d", varname[k], indices[i * n + k]);
      }
    }
  }
}

void deleteSupport(Support *S) {
  if (S == NULL) return;
  if (S->consId != NULL) free(S->consId);
  free(S);
  S = NULL;
}

void enlargeSupport(Support *const S) {
  S->consCap = S->consCap * ENLARGE_RAT + 1;

  S->consId = (int *)realloc_d(S->consId, S->consCap * sizeof(int));
}

int subpoly_getTotalDegree(SubPoly *sp) {
  int re = 0;
  int temp, i, j;
  Poly *poly = sp->poly;
  int varNum = getvarNum(poly->varId);

  for (i = 0; i < sp->size; i += 1) {
    temp = 0;

    for (j = 0; j < varNum; j += 1) {
      temp += poly->indices[sp->locs[i] * varNum + j];
    }
    if (temp > re) re = temp;
  }
  return re;
}
void md5sumbyIndice(uint8_t *re, const int varId, const indice_t *indices,
                    const int size) {
  md5_ctx_t ctx;
  md5_init(&ctx);
  uint8_t dummy = (uint8_t)varId;

  md5_update(&ctx, &dummy, 1);
  dummy = getvarNum(varId);
  md5_update(&ctx, &dummy, 1);

  md5_update(&ctx, indices, size * dummy * sizeof(indice_t));

  md5_finalize(&ctx);

  memcpy(re, MD5_DIGEST(&ctx), DIGEST_SIZE * sizeof(uint8_t));
}
