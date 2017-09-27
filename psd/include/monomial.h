/**
 * @file   monomial.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:42:42 2015
 *
 * @brief  a struct monomial
 *
 *
 */

#ifndef MONOMIAL_INC
#define MONOMIAL_INC
#include "polytype.h"

const uint32_t valueMap[] = {
    1u,       1u << 1,  1u << 2,  1u << 3,  1u << 4,  1u << 5,  1u << 6,
    1u << 7,  1u << 8,  1u << 9,  1u << 10, 1u << 11, 1u << 12, 1u << 13,
    1u << 14, 1u << 15, 1u << 16, 1u << 17, 1u << 18, 1u << 19, 1u << 20,
    1u << 21, 1u << 22, 1u << 23, 1u << 24, 1u << 25, 1u << 26, 1u << 27,
    1u << 28, 1u << 29, 1u << 30, 1u << 31};
struct monomial {
  const indice_t *indice;
  int dim;

  /*-----------------------------------------------------------------------------
   *  at most 128 vertex
   *-----------------------------------------------------------------------------*/
  monomial(const indice_t *m, const int d) : indice(m), dim(d) {}
  operator size_t() const;

  bool operator==(const monomial &other) const;
};

struct monomialvalue {
  uint32_t relation[4];
  bool change;

  int value;

  monomialvalue() : change(false), value(0) {
    relation[0] = 0u;
    relation[1] = 0u;
    relation[2] = 0u;
    relation[3] = 0u;
  }
  static void add(uint32_t set[], const int v);

  static void add(uint32_t set[], const monomialvalue &other);

  static bool contain(const uint32_t set[], const monomialvalue &other);

  static bool conjunction(const uint32_t set[], const monomialvalue &other);

  void add(const int k);

  void clear(void);

  void add(const monomialvalue &other);

  bool contain(const monomialvalue &other) const;

  bool contain(const int k) const;

  bool conjunction(const monomialvalue &other) const;

  bool conjunction(const uint32_t set[]) const;

  void remove(const monomialvalue &other);

  int size() const;
};

#endif
