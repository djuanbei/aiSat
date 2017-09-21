
/**
* @file   util.c
* @author Liyun Dai <dlyun2009@gmail.com>
* @date   Sun Feb  8 16:24:30 2015
*
* @brief
*
*
*/

#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mono_const.h"
#include "monoconst_table.h"
#include "pconst_table.h"
#include "polytype.h"
#include "search.h"
#include "selfassert.h"
#include "support_table.h"
#include "vartable.h"

extern void qsortM(indice_t *, const int n, const int left, const int right,
                   int (*comp)(const indice_t *, const indice_t *, const int));
BOOL isSameLine(const indice_t points[], const int *loc, const int n,
                const int dim) {
  if (dim == 1 || n < 3) return TRUE;
  int i, j, k;
  int base[dim];
  int current, next;
  for (i = 0; i < dim; i += 1) {
    base[i] = points[loc[1] * dim + i] - points[loc[0] * dim + i];
    if (base[i] != 0) k = i;
  }
  for (i = 2; i < n; i += 1) {
    current = points[loc[i] * dim + k] - points[loc[0] * dim + k];
    for (j = 1; j < dim - 1; j += 1) {
      next = points[loc[i] * dim + j] - points[loc[0] * dim + j];
      if (current * base[j] != next * base[k]) return FALSE;
    }
  }
  return TRUE;
}

/**
 *@brief the number of different group of choice k elements in n elements
 *
 * @param n
 * @param k
 *
 * @return
 */
int nchoosek(const int n, const int k) {
  if (n < k) return 0;

  if (n < k || n < 0 || k < 0) {
    printf("input error\n");
    exit(1);
  }

  int i = 0;
  long double re;
  re = 1;
  for (i = 0; i < k; i++) {
    re *= (n - i);
    re /= (k - i);
  }

  return (int)(re + 0.5);
}

/**
 *@brief enlarge the capacity of Z to cap and keep the orignal data
 *
 * @param Z
 * @param n
 * @param cap
 */
void enLargeZ(indice_t *Z, const int n, int *const cap) {
  *cap = (*cap) + (*cap) * ENLARGE_RAT + 1;
  Z = realloc_d(Z, n * (*cap) * sizeof(indice_t));
}

/**
 *@brief enumerate all monomials who belong to support whose id is supportId
 * Useing Lexicographic Order x1>x2>..>xn
 * restraintNum is the number of restraints
 * output all the monomials which satsfy these restraints
 *
 * @param supportId the support id
 * @param length the length of all monomials
 *
 * @return all monomials who belong to support whose id is supportId if success
 * false otherwise.
 */

indice_t *getAllMonByTd(const int supportId, int *const length) {
  int i, j, k;
  int sum = 1;
  int size = 0;
  int cap;
  int nch;
  int n, d;
  indice_t *Z;

  Support *sup = getSupElem(supportId);

  if (sup == NULL) return NULL;

  n = getvarNum(sup->varId);
  if (0 == n) {
    Z = malloc_d(sizeof(indice_t));
    Z[0] = 0;
    *length = 1;
    return Z;
  }

  d = sup->deg;

  if (sup->constNum == 0) {
    *length = nchoosek(n + d, d);
    Z = (indice_t *)malloc_d((*length) * n * sizeof(indice_t));

    /*-----------------------------------------------------------------------------
     *  The first row denote const 1, and second ros denote monomial x1
     *-----------------------------------------------------------------------------*/
    for (i = 0; i < n;
         i += 1) { /* initial the first two rows (monomial) to zero  */
      Z[i] = 0;
      Z[n + i] = 0;
    }

    Z[2 * n - 1] = 1;
    i = 2;

    while (i < (*length)) {
      /*memcpy(Z[i],Z[i-1],n*sizeof(int));*/

      for (j = 0; j < n; j++) {
        Z[i * n + j] = Z[(i - 1) * n + j];
      }

      if (sum < d) {
        Z[i * n + n - 1]++;
        sum++;
      } else {
        k = n - 1;
        while (Z[i * n + k] == 0) k--;
        sum -= (Z[i * n + k] - 1);
        Z[i * n + k] = 0;
        Z[i * n + k - 1]++;
      }
      i++;
    }
  } else {
    nch = nchoosek(n + d, d);
    cap = nch / ((sup->constNum + 1) * n);

    Z = (indice_t *)malloc_d(n * cap * sizeof(indice_t));

    indice_t temp[n];

    for (i = 0; i < n; i += 1) {
      temp[i] = 0; /* constant term */
    }
    /*-----------------------------------------------------------------------------
     *  satsfies restraints
     *-----------------------------------------------------------------------------*/
    if (criteria(sup, temp)) {
      addZ(Z, &cap, &size, n, temp);
    }

    temp[n - 1] = 1;

    if (criteria(sup, temp)) {
      addZ(Z, &cap, &size, n, temp);
    }
    i = 2;
    while (i < nch) {
      if (sum < d) {
        temp[n - 1]++;
        sum++;
      } else {
        k = n - 1;
        while (temp[k] == 0) k--;
        sum -= (temp[k] - 1);
        temp[k] = 0;
        temp[k - 1]++;
      }
      if (criteria(sup, temp)) {
        addZ(Z, &cap, &size, n, temp);
      }

      i++;
    }
    *length = size;
  }
  return Z;
}

void getAllMonHomoTd(const int varNum, const int deg, const int length,
                     indice_t *indices) {
  ASSERT(varNum > 0, "variable number must be positive number");

  if (varNum == 1) {
    indices[0] = deg;
    return;
  }

  int n = varNum - 1;

  indice_t *Z = malloc_d((length + 1) * n * sizeof(indice_t));

  int sum = 1;
  int i, j, k;

  for (i = 0; i < n; i++) {
    Z[i] = Z[n + i] = 0;
  }

  Z[2 * n - 1] = 1;
  i = 2;

  while (i < length) {
    for (j = 0; j < n; j++) {
      Z[i * n + j] = Z[(i - 1) * n + j];
    }
    if (sum < deg) {
      Z[i * n + n - 1]++;
      sum++;
    } else {
      k = n - 1;
      while (Z[i * n + k] == 0) k--;

      sum -= (Z[i * n + k] - 1);
      Z[i * n + k] = 0;
      Z[i * n + k - 1]++;
    }
    i++;
  }

  for (i = 0; i < length; i++) {
    sum = 0;
    for (j = 0; j < n; j++) {
      indices[i * varNum + j] = Z[i * n + j];
      sum += Z[i * n + j];
    }
    indices[i * varNum + j] = deg - sum;  // homogenous
  }

  free(Z);
}

void addZ(indice_t *const Z, int *const cap, int *const size, const int n,
          indice_t *key) {
  int i;
  if (*size == *cap) {
    enLargeZ(Z, n, cap);
  }

  for (i = 0; i < n; i += 1) {
    Z[(*size) * n + i] = key[i];
  }
  (*size)++;
}

BOOL criteria(const Support *S, indice_t *key) {
  int i;
  int resNum = S->constNum;

  for (i = 0; i < resNum; i += 1) {
    if (!consEval(key, S->varId, getconsElem(S->consId[i]))) return FALSE;
  }

  return TRUE;
}


void deleteZ(indice_t **Z, const int length) {
  int i;

  for (i = 0; i < length; i += 1) {
    free(Z[i]);
  }
  free(Z);

}

/** 
 * @brief compare ">"
 *  x>y is and only if exists k x(k)>y(k) and when i<k x(i)=y(i)
 * @param key1 
 * @param key2 
 * @param size 
 * 
 * @return  if key1==key2 return 0
 *  otherwise -1
 */
int compare(const indice_t *key1, const indice_t *key2, const int size) {
  int i = 0;

  for (i = 0; i < size; i += 1) {
    if (key1[i] > key2[i])
      return 1;
    else if (key1[i] < key2[i])
      return -1;
  }

  return 0;
}

/**
 *@brief  For every monomial c of SOSM  W_c is  a  nchoosek(n+d/2,d/2) *
 * nchoosek(n+d/2,d/2) size symmetric matrix
 *  all element of W_c is one identity or zero. It means the location of c
 * occurs in Z^T * Z
 *  where Z=[1,x_1,...,x_n,x_1x_2,...,x_n^(d/2)]
 *  n is the number of variable and d is the max total degree of SOSM.
 *  d is a even integer
 *
 * @param supportId
 * @param SOSM
 * @param blockSize
 * @param lengthSOS the number of different element in SOSM
 *
 * @return A array of ArrangeMatrix with nchoosek(n+d/2,d/2) *
 * nchoosek(n+d/2,d/2)  for every element in SOSM
 */
ArrangeMatrix **createArrangeM(const int supportId, indice_t const *SOSM,
                               int *const blockSize, int const lengthSOS) {
  int i, j, k, index;
  Support *sup = getSupElem(supportId);
  const int n = getvarNum(sup->varId);

  indice_t temp[n + 1];
  indice_t *Z = getGsup(supportId, blockSize);
  if (NULL == Z) {
    sup->deg /= 2;
    Z = getAllMonByTd(supportId, blockSize); /* degree squared */
    setGsup(supportId, *blockSize, Z);
    sup->deg *= 2;
  }

  ArrangeMatrix **re =
      (ArrangeMatrix **)malloc_d(lengthSOS * sizeof(ArrangeMatrix *));

  for (i = 0; i < lengthSOS; i++) {
    re[i] = createSparse(*blockSize);
  }

  /*-----------------------------------------------------------------------------
   *  |  a00 a10 a20  |
   *  |  a10 a11 a12  |
   *  |  a20 a21 a22  |
   *  As (Z^T) Z is a systematic matrix, we only need to compute the lower
   *triangle
   *  of it.
   *-----------------------------------------------------------------------------*/
  for (i = 0; i < (*blockSize); i++) {
    for (j = 0; j <= i; j += 1) {
      for (k = 0; k < n; k += 1) {
        temp[k] = Z[i * n + k] + Z[j * n + k]; /* monomial add */
      }
      index = findIndex(temp, SOSM, lengthSOS, n);
      ASSERT(index >= 0, "some thing wrong");
      addSparseElem(re[index], i, j);
    }
  }

  //	free(Z);
  setArrangeM(supportId, re, *blockSize);
  return re;
}

void deleteW(ArrangeMatrix **W[], const int size, const int len[]) {
  int i, j;

  for (i = 0; i < size; i += 1) {
    for (j = 0; j < len[i]; j += 1) {
      deleteSparse(W[i][j]);
    }
    free(W[i]);
  }
}

void deleteSparseA(ArrangeMatrix **s, const int len) {
  int i;
  for (i = 0; i < len; i += 1) {
    deleteSparse(s[i]);
  }
  free(s);
}

void catch_memory_alllocate_error(int a) {
  printf("cannot allow memory %d\n", a);
  fprintf(stderr, "\ndynamic memory allocation failed\n");
  exit(EXIT_FAILURE);
}

void config(void) { initcoefTable(); }

Poly *sosConvertPoly(Blockmatrix *const X, const int k, const int blockSize,
                     const int supportId) {
  Poly *re;
  coef_t cf;
  int i, j, row, col;
  re = (Poly *)malloc_d(sizeof(Poly));

  re->varId = getSupElem(supportId)->varId;

  const int varSize = getvarNum(re->varId);

  const int length = getsosSLength(supportId);

  re->capacity = length;

  re->size = re->capacity;

  re->coef = (coef_t *)malloc_d(re->size * sizeof(coef_t));

  //  memset(re->coef,0, re->size*sizeof(coef_t));

  re->indices = (indice_t *)malloc_d(re->size * varSize * sizeof(indice_t));

  int dummy;
  memcpy(re->indices, getSOSsup(supportId, &dummy),
         re->size * varSize * sizeof(indice_t));

  ArrangeMatrix **S = getAMIndex(supportId, &dummy);

  for (i = 0; i < length; i += 1) {
    cf = 0;
    for (j = 0; j < S[i]->size; j += 1) {
      row = S[i]->data[j].row + 1;
      col = S[i]->data[j].col + 1;
      if (row == col) {
        cf += X->blocks[k + 1].data.mat[ijtok(row, col, blockSize)];

      } else {
        cf += 2 * (X->blocks[k + 1].data.mat[ijtok(row, col, blockSize)]);
      }
    }
    re->coef[i] = cf;
  }

  return re;
}

uint64_t hash_fast64(const void *buf, size_t len, uint64_t seed) {
  const uint64_t m = 0x880355f21e6d1965ULL;
  const uint64_t *pos = (const uint64_t *)buf;
  const uint64_t *end = pos + (len >> 3);
  const unsigned char *pc;
  uint64_t h = len * m ^ seed;
  uint64_t v;

  while (pos != end) {
    v = *pos++;
    v ^= v >> 23;
    v *= 0x2127599bf4325c37ULL;
    h ^= v ^ (v >> 47);
    h *= m;
  }

  pc = (const unsigned char *)pos;
  v = 0;

  switch (len & 7) {
    case 7:
      v ^= (uint64_t)pc[6] << 48;
    case 6:
      v ^= (uint64_t)pc[5] << 40;
    case 5:
      v ^= (uint64_t)pc[4] << 32;
    case 4:
      v ^= (uint64_t)pc[3] << 24;
    case 3:
      v ^= (uint64_t)pc[2] << 16;
    case 2:
      v ^= (uint64_t)pc[1] << 8;
    case 1:
      v ^= (uint64_t)pc[0];
      v ^= v >> 23;
      v *= 0x2127599bf4325c37ULL;
      h ^= v ^ (v >> 47);
      h *= m;
  }

  h ^= h >> 23;
  h *= 0x2127599bf4325c37ULL;
  h ^= h >> 47;

  return h;
}
