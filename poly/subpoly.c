/*
 * =====================================================================================
 *
 *       Filename:  subpoly.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/25/2013 07:03:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "subpoly.h"
#include "crypt_md5.h"
#include "poly.h"
#include "selfassert.h"
#include "selfmemutil.h"
#include "vartable.h"

static void getMd5sum(const SubPoly *subp, md5_ctx_t *ctx) {
  int i;
  md5_init(ctx);
  ASSERT(sizeof(uint8_t) == sizeof(indice_t), "");
  uint8_t varId = subp->poly->varId;

  md5_update(ctx, &varId, 1);

  int dim = getvarNum(subp->poly->varId);

  uint8_t ch = (uint8_t)dim;

  md5_update(ctx, &ch, 1);
  for (i = 0; i < subp->size; i += 1) {
    md5_update(ctx, subp->poly->indices + subp->locs[i] * dim, dim);
  }
  md5_finalize(ctx);
}

SubPoly *createSubPolyByPoly(Poly *poly) {
  SubPoly *re = malloc_d(sizeof(SubPoly));
  re->poly = poly;
  re->size = poly->size;
  re->locs = (int *)malloc_d(poly->size * sizeof(int));
  int i;
  for (i = 0; i < poly->size; i += 1) {
    re->locs[i] = i;
  }

  md5_ctx_t ctx;
  getMd5sum(re, &ctx);
  memcpy(re->md5sum, MD5_DIGEST(&ctx), 16 * sizeof(uint8_t));
  return re;
}

SubPoly *createSubPoly(Poly *poly, const int size, int *loc) {
  SubPoly *re = malloc_d(sizeof(SubPoly));
  re->poly = poly;
  re->size = size;
  re->locs = (int *)malloc_d(size * sizeof(int));
  memcpy(re->locs, loc, size * sizeof(int));
  md5_ctx_t ctx;
  getMd5sum(re, &ctx);
  memcpy(re->md5sum, MD5_DIGEST(&ctx), 16 * sizeof(uint8_t));
  return re;
}

void printSubPoly(const SubPoly *subpoly) {
  int i, j;
  Poly *poly = subpoly->poly;

  indice_t *vars = getvarElem(poly->varId);

  int *loc = subpoly->locs;

  const int n = (int)vars[0];

  char *varname[n];
  if (0 == subpoly->size) printf(" 0 \n");

  for (i = 0; i < n; i += 1) {
    varname[i] = getvarName((int)vars[i + 1]);
  }

  for (i = 0; i < subpoly->size; i += 1) {
    if (poly->coef[loc[i]] > MIN_COEF || poly->coef[loc[i]] < -MIN_COEF) {
      if (i == 0) {
        printf("%.4f", poly->coef[loc[i]]);
      } else if (poly->coef[loc[i]] > 0.0) {
        printf(" +%.4f", poly->coef[loc[i]]);
      } else if (poly->coef[loc[i]] < 0.0) {
        printf(" %.4f", poly->coef[loc[i]]);
      }

      for (j = 0; j < n; j += 1) {
        if (poly->indices[loc[i] * n + j] > 0) {
          if (poly->indices[loc[i] * n + j] > 1)
            printf("*%s^%d", varname[j], poly->indices[loc[i] * n + j]);
          else
            printf("*%s", varname[j]);
        }
      }
    }
  }
  //	printf ( "\n" );
}

void deleteSubPoly(SubPoly *poly) {
  if (poly != NULL) {
    free(poly->locs);
    free(poly);
    poly = NULL;
  }
}
