/**
 * @file   Subpoly.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Sep 22 22:31:51 2017
 *
 * @brief  Subpoly
 *
 *
 */
#ifndef __SUB_OLY_H
#define __SUB_OLY_H
#include "crypt_md5.h"
#include "poly.hpp"

namespace aiSat {
namespace poly {

template <typename C = double, typename T = unsigned char>

class Subpoly {
 private:
  const poly<C, T> &parent;
  uint8_t md5sum[DIGEST_SIZE];  /// the md5sum of this subpolynomial, when test
  /// whether the two polynomials equal first test
  /// the two mds5sum equal
  vector<int> locs;

  void getMd5sum(md5_ctx_t *ctx) {
    md5_init(ctx);
    uint8_t varId = parent.getVarId();
    md5_update(ctx, &varId, 1);
    int dim = parent.getVarNum();
    for (int i; i < locs.size(); i++) {
      uint8_t loc = locs[i];
      md5_update(ctx, &loc, 1);
    }

    md5sum_finalize(ctx);
  }

 public:
  Subpoly(poly<C, T> &p) : parent(p) {
    int size = parent.getSize();
    for (int i = 0; i < size; i++) {
      locs.push_back(i);
    }
    md5_ctx_t ctx;
    getMd5sum(&ctx);
    memcpy(md5sum, MD5_DIGEST(&ctx), 16 * sizeof(uint8_t));
  }
  Subpoly(poly<C, T> &p, const vector<int> &loc) : parent(p), locs(loc) {
    md5_ctx_t ctx;
    getMd5sum(&ctx);
    memcpy(md5sum, MD5_DIGEST(&ctx), 16 * sizeof(uint8_t));
  }
  string toString(void) const { return parent.toString(locs); }

  poly<C, T> getPoly() const {
    poly<C, T> rep;
    parent.getSubpoly(locs, rep);
    return rep;
  }
};
}
}

#endif
