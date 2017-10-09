#include "monomial.h"
#include <cstring>
#include "util.h"

namespace aiSat {

namespace psd {

void monomialvalue::add(uint32_t set[], const int v) {
  set[v / 32] |= valueMap[v % 32];
}
void monomialvalue::add(uint32_t set[], const monomialvalue &other) {
  set[0] |= other.relation[0];
  set[1] |= other.relation[1];
  set[2] |= other.relation[2];
  set[3] |= other.relation[3];
}

bool monomialvalue::contain(const uint32_t set[], const monomialvalue &other) {
  if ((other.relation[0] & set[0]) != other.relation[0]) return false;
  if ((other.relation[1] & set[1]) != other.relation[1]) return false;
  if ((other.relation[2] & set[2]) != other.relation[2]) return false;
  if ((other.relation[3] & set[3]) != other.relation[3]) return false;
  return true;
}

bool monomialvalue::conjunction(const uint32_t set[],
                                const monomialvalue &other) {
  if (set[0] & other.relation[0]) return true;
  if (set[1] & other.relation[1]) return true;
  if (set[2] & other.relation[2]) return true;
  if (set[3] & other.relation[3]) return true;
  return false;
}

void monomialvalue::add(const int k) {
  relation[k / 32] |= valueMap[k % 32];
  change = true;
}

void monomialvalue::clear(void) {
  relation[0] = 0;
  relation[1] = 0;
  relation[2] = 0;
  relation[3] = 0;
  change = false;
}
void monomialvalue::add(const monomialvalue &other) {
  relation[0] |= other.relation[0];
  relation[1] |= other.relation[1];
  relation[2] |= other.relation[2];
  relation[3] |= other.relation[3];
  change = true;
}

bool monomialvalue::contain(const monomialvalue &other) const {
  if ((other.relation[0] & relation[0]) != other.relation[0]) return false;
  if ((other.relation[1] & relation[1]) != other.relation[1]) return false;
  if ((other.relation[2] & relation[2]) != other.relation[2]) return false;
  if ((other.relation[3] & relation[3]) != other.relation[3]) return false;
  return true;
}

bool monomialvalue::contain(const int k) const {
  return relation[k / 32] & valueMap[k % 32];
}

bool monomialvalue::conjunction(const monomialvalue &other) const {
  if (other.relation[0] & relation[0]) return true;
  if (other.relation[1] & relation[1]) return true;
  if (other.relation[2] & relation[2]) return true;
  if (other.relation[3] & relation[3]) return true;
  return false;
}

bool monomialvalue::conjunction(const uint32_t set[]) const {
  if (set[0] & relation[0]) return true;
  if (set[1] & relation[1]) return true;
  if (set[2] & relation[2]) return true;
  if (set[3] & relation[3]) return true;
  return false;
}

void monomialvalue::remove(const monomialvalue &other) {
  relation[0] = relation[0] ^ (relation[0] & other.relation[0]);
  relation[1] = relation[1] ^ (relation[1] & other.relation[1]);
  relation[2] = relation[2] ^ (relation[2] & other.relation[2]);
  relation[3] = relation[3] ^ (relation[3] & other.relation[3]);

  change = true;
}

int monomialvalue::size() const {
  int re = 0;
  re = 0;
  int i = 0;
  for (i = 0; i < 32; i += 1) {
    if (relation[0] & valueMap[i]) re++;
  }

  for (i = 0; i < 32; i += 1) {
    if (relation[1] & valueMap[i]) re++;
  }

  for (i = 0; i < 32; i += 1) {
    if (relation[2] & valueMap[i]) re++;
  }

  for (i = 0; i < 32; i += 1) {
    if (relation[3] & valueMap[i]) re++;
  }
  return re;
}

monomial::operator size_t() const {
  return hash_fast64((const unsigned char *)indice, dim * sizeof(indice_t), 0);
}

bool monomial::operator==(const monomial &other) const {
  return memcmp(indice, other.indice, dim * sizeof(indice_t)) == 0;
}
}
}
