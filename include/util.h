

#ifndef UTIL_INC
#define UTIL_INC

#include <signal.h>
#include "declarations.h"
#include "polytype.h"
#include "sdpsolver.h"
#include "selfassert.h"
#include "selfmemutil.h"
#include "sparse.h"

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


  /** 
   * @brief  n (n>=1) is the number of variables of polynomial and d is the
   * degree of polynomial
   * enumerate all monomials whos total degree less or equal than d.
   * Useing Lexicographic Order x1>x2>..>xn
   * 
   * @param monomialsId the id of this indices  corresponding varMap
   * @param length 
   * 
   * @return 
   */
  indice_t *getAllMonByTd(const int monomialsId, int *const length);

  void getAllMonHomoTd(const int varNum, const int deg, const int length,
                       indice_t *indices);



  BOOL criteria(const Support *coefM, indice_t *key);

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

  ArrangeMatrix **createArrangeM(const int coefMonomiaId, indice_t const *SOSM,
                                 int *const blockSize, int const lengthM);

  void deleteW(ArrangeMatrix **s[], const int size, const int len[]);

  void deleteSparseA(ArrangeMatrix **s, const int len);

  void catch_memory_allocate_error(int);

  void config(void);

  Poly *sosConvertPoly(Blockmatrix *const X, const int blockId,
                       const int blockSize, const int sosmId);

  uint64_t hash_fast64(const void *buf, size_t len, uint64_t seed);

#ifdef __cplusplus
}
#endif
#endif 
