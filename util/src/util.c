
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

#include "config.h"
#include "crypt_md5.h"
#include "search.h"
#include "selfassert.h"

extern void qsortM(indice_t *, const int n, const int left, const int right,
                   int (*comp)(const indice_t *, const indice_t *, const int));

BOOL isSameLine(const indice_t points[], const int *loc, const int n,
                const int dim) {
  if (dim == 1 || n < 3) {
    return TRUE;
  }

  int i, j, k = 0;
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
      if (current * base[j] != next * base[k]) {
        return FALSE;
      }
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

void catch_memory_alllocate_error(int a) {
  printf("cannot allow memory %d\n", a);
  fprintf(stderr, "\ndynamic memory allocation failed\n");
  exit(EXIT_FAILURE);
}

// void config(void) { initcoefTable(); }

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

void md5sumbyIndice(uint8_t *re, const int varId, const indice_t *indices,
                    const int size) {
  md5_ctx_t ctx;
  md5_init(&ctx);
  uint8_t dummy = (uint8_t)varId;

  md5_update(&ctx, &dummy, 1);

  md5_update(&ctx, indices, size * dummy * sizeof(indice_t));

  md5_finalize(&ctx);

  memcpy(re, MD5_DIGEST(&ctx), DIGEST_SIZE * sizeof(uint8_t));
}
