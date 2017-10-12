

#ifndef UTIL_INC
#define UTIL_INC

#include <signal.h>
#include "declarations.h"
#include "extenttype.h"

#include "selfassert.h"
#include "selfmemutil.h"

#define QUDI(x) #x
#define QUdi(x) QUDI(x)

#ifdef __cplusplus
extern "C" {
#endif
BOOL isSameLine(const indice_t points[], const int *loc, const int n,
                const int dim);

/**
 * @brief  notice that n d cannot too large.
 *
 * @param int
 * @param int
 *
 * @return  n(n-1)...(n-d+1)/((n-d)(n-d-1)...1)
 */
int nchoosek(const int, const int);

void addZ(indice_t *const Z, int *const cap, int *const size, const int n,
          indice_t *key);

void removeZ(indice_t *const Z, int cap, int size, const int n);

void enLargeZ(indice_t *Z, const int n, int *const cap);

void deleteZ(indice_t **Z, int length);

/**
 * @brief  compare key1 and key use the default order. by Lexicographical
 *
 * @param key1
 * @param key2
 * @param size
 *
 * @return
 */
int compare(const indice_t *key1, const indice_t *key2, const int size);

void catch_memory_allocate_error(int);

void config(void);

uint64_t hash_fast64(const void *buf, size_t len, uint64_t seed);

void md5sumbyIndice(uint8_t *re, const int varId, const indice_t *indices,
                    const int size);

#ifdef __cplusplus
}
#endif
#endif
