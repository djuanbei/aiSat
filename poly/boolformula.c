/*
 * =====================================================================================
 *
 *       Filename:  boolformula.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  06/30/2013 09:46:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "boolformula.h"
#include <stdio.h>
#include <stdlib.h>
#include "pointvector.h"
#include "selfassert.h"
#include "selfmemutil.h"
//#define DEBUG

/* variable id's start from 1 */

const lit NOT_A_LITERAL = 0;

lit boolformula_lit_from_var(var v) {
  ASSERT(v > 0, "");
  return v;
}

var boolformula_var_from_lit(lit l) {
  ASSERT(l != 0, "");
  return l > 0 ? l : -l;
}

lit boolformula_lit_complement(lit l) {
  ASSERT(l != 0, "");
  return -l;
}

BOOL boolformula_positive_lit(lit l) {
  ASSERT(l != 0, "");
  return l > 0;
}

/*
 * vector of length 0 is the unit of disjunction (F)
 */
boolformula_t *boolformula_disjunction_unit(void) {
  boolformula_t *ret =(boolformula_t*) malloc_d(sizeof(boolformula_t));
  ret->d.v = vector_new(boolformula_free);
  ret->t = DISJUNCT;
  ret->ref = 1;
  return ret;
}

boolformula_t *boolformula_disjunction_new(uscalar_t length) {
  boolformula_t *ret =(boolformula_t*) malloc_d(sizeof(boolformula_t));
  ret->d.v = vector_newByL(boolformula_free, length);
  ret->t = DISJUNCT;
  ret->ref = 1;
  return ret;
}

/*
 * vector of length 0 is the unit of disjunction (T)
 */
boolformula_t *boolformula_conjunction_unit(void) {
  boolformula_t *ret =(boolformula_t*) malloc_d(sizeof(boolformula_t));
  ret->d.v = vector_new(boolformula_free);
  ret->t = CONJUNCT;
  ret->ref = 1;
  return ret;
}

boolformula_t *boolformula_conjunction_new(uscalar_t length) {
  boolformula_t *ret =(boolformula_t*) malloc_d(sizeof(boolformula_t));
  ret->t = CONJUNCT;
  ret->d.v = vector_newByL(boolformula_free, length);
  ret->ref = 1;
  return ret;
}

boolformula_t *boolformula_literal_new(lit l) {
  boolformula_t *ret =(boolformula_t*) malloc_d(sizeof(boolformula_t));
  ret->d.l = l;
  ret->t = LITERAL;
  ret->ref = 1;
  return ret;
}

boolformula_t *boolformula_add(boolformula_t *f, boolformula_t *g) {
  ASSERT(f->t != LITERAL, "");
  push_back_V(f->d.v, g);
  g->ref++;
  return f;
}

boolformula_t *boolformula_set(boolformula_t *f, uscalar_t idx,
                               boolformula_t *g) {
  ASSERT(f->t != LITERAL, "");
  vector_set(f->d.v, idx, g);
  g->ref++;
  return f;
}

void boolformula_free(void *ff) {
  boolformula_t *f = (boolformula_t *)ff;
  if (f->ref > 1) {
    f->ref--;
    return;
  }

  if (f->t != LITERAL) {
    vector_free(f->d.v);

  }
  free(f);
}

void boolformula_neg(boolformula_t *f) {
  int num_of_subformulae;
  int i;
  switch (f->t) {
    case DISJUNCT:
      f->t = CONJUNCT;
      num_of_subformulae = vector_length(f->d.v);
      for (i = 0; i < num_of_subformulae; i++) {
        boolformula_neg((boolformula_t *)vector_get(f->d.v, i));
      }
      break;
    case CONJUNCT:
      f->t = DISJUNCT;
      num_of_subformulae = vector_length(f->d.v);
      for (i = 0; i < num_of_subformulae; i++) {
        boolformula_neg((boolformula_t *)vector_get(f->d.v, i));
      }
      break;
    case LITERAL:
      f->d.l = -f->d.l;
      break;
  }
}

void add_clauses_to_boolformula(boolformula_t *ret, boolformula_t *f,
                                lit *next_fresh) {
  size_t i;
  boolformula_t *cur_neg =
      boolformula_literal_new(boolformula_lit_complement(*next_fresh));
  boolformula_t *dis, *temp;
  switch (f->t) {
    case CONJUNCT:
      for (i = 0; i < vector_length(f->d.v); i++) {
        dis = boolformula_disjunction_new(2);
        boolformula_add(ret, dis);

        boolformula_set(dis, 0, cur_neg);
        if (((boolformula_t *)vector_get(f->d.v, i))->t == LITERAL) {
          temp = boolformula_literal_new(
              ((boolformula_t *)vector_get(f->d.v, i))->d.l);
          boolformula_set(dis, 1, temp);
          boolformula_free(temp);
        } else {
          (*next_fresh)++;
          temp = boolformula_literal_new(*next_fresh);
          boolformula_set(dis, 1, temp);
          boolformula_free(temp);
          add_clauses_to_boolformula(ret, vector_get(f->d.v, i), next_fresh);
        }
        boolformula_free(dis);
      }
      break;
    case DISJUNCT:
      dis = boolformula_disjunction_new(1 + vector_length(f->d.v));
      boolformula_add(ret, dis);
      boolformula_set(dis, 0, cur_neg);
      for (i = 0; i < vector_length(f->d.v); i++) {
        if (((boolformula_t *)vector_get(f->d.v, i))->t == LITERAL) {
          temp = boolformula_literal_new(
              ((boolformula_t *)vector_get(f->d.v, i))->d.l);
          boolformula_set(dis, i + 1, temp);
          boolformula_free(temp);
        } else {
          (*next_fresh)++;
          temp = boolformula_literal_new(*next_fresh);
          boolformula_set(dis, i + 1, temp);
          boolformula_free(temp);
          add_clauses_to_boolformula(ret, vector_get(f->d.v, i), next_fresh);
        }
      }
      boolformula_free(dis);

      break;
    case LITERAL:
      fprintf(stderr, "Something wrong\n");
      exit(13);
      break;
  }
  boolformula_free(cur_neg);
}

boolformula_t *boolformula_to_cnf(boolformula_t *f, scalar_t num_var) {
  scalar_t next_fresh = num_var + 1;
  //	int i;
  boolformula_t *ret = NULL;
  boolformula_t *cur;
  //    boolformula_t* cur_neg;
  //    boolformula_t* temp;

  switch (f->t) {
    case LITERAL:
      ret = boolformula_copy(f);
      break;
    case CONJUNCT:

    case DISJUNCT:
      ret = boolformula_conjunction_unit();
      cur = boolformula_literal_new(next_fresh);
      boolformula_add(ret, cur);
      add_clauses_to_boolformula(ret, f, &next_fresh);
      boolformula_free(cur);
      break;
  }
  return ret;
}

enum type boolformula_get_type(boolformula_t *f) {
  ASSERT(f->t == LITERAL || f->t == CONJUNCT || f->t == DISJUNCT, "");
  return f->t;
}

uscalar_t boolformula_get_length(boolformula_t *f) {
  ASSERT(f->t == CONJUNCT || f->t == DISJUNCT, "");
  return vector_length(f->d.v);
}

boolformula_t *boolformula_get_subformula(boolformula_t *f, uscalar_t idx) {
  ASSERT(f->t == CONJUNCT || f->t == DISJUNCT, "");
  return vector_get(f->d.v, idx);
}

lit boolformula_get_value(boolformula_t *f) {
  ASSERT(f->t == LITERAL, "");
  return f->d.l;
}

void boolformula_print(boolformula_t *f) {
  switch (f->t) {
    case DISJUNCT:
      if (vector_length(f->d.v) == 0) {
        fprintf(stderr, "{ F }");
      } else {
        size_t i;
        fprintf(stderr, "{ ");
        for (i = 0; i < vector_length(f->d.v) - 1; i++) {
          boolformula_print(vector_get(f->d.v, i));
          fprintf(stderr, " | ");
        }
        ASSERT((i == vector_length(f->d.v) - 1), "");
        boolformula_print(vector_get(f->d.v, vector_length(f->d.v) - 1));
        fprintf(stderr, " }");
      }
      break;
    case CONJUNCT:
      if (vector_length(f->d.v) == 0) {
        fprintf(stderr, "{ T }");
      } else {
        size_t i;
        fprintf(stderr, "{ ");
        for (i = 0; i < vector_length(f->d.v) - 1; i++) {
          boolformula_print(vector_get(f->d.v, i));
          fprintf(stderr, " & ");
        }
        ASSERT((i == vector_length(f->d.v) - 1), "");
        boolformula_print(vector_get(f->d.v, vector_length(f->d.v) - 1));
        fprintf(stderr, " }");
      }
      break;
    case LITERAL:
      fprintf(stderr, "%ld", f->d.l);
      break;
  }
}

scalar_t boolformula_num_of_var(boolformula_t *f) {
  scalar_t cur = 0, temp;
  size_t i;
  switch (f->t) {
    case LITERAL:
      return abs(f->d.l);
      break;
    default:
      for (i = 0; i < f->d.v->size; i++) {
        temp = boolformula_num_of_var((boolformula_t *)vector_get(f->d.v, i));
        cur = cur > temp ? cur : temp;
      }
      break;
  }
  return cur;
}

boolformula_t *boolformula_copy(boolformula_t *f) {
  boolformula_t *ret = NULL, *temp;
  size_t i;
  switch (f->t) {
    case CONJUNCT:
      ret = boolformula_conjunction_new(f->d.v->size);
      for (i = 0; i < f->d.v->size; i++) {
        temp = boolformula_copy(vector_get(f->d.v, i));
        boolformula_set(ret, i, temp);
        boolformula_free(temp);
      }
      break;
    case DISJUNCT:
      ret = boolformula_disjunction_new(f->d.v->size);
      for (i = 0; i < f->d.v->size; i++) {
        temp = boolformula_copy(vector_get(f->d.v, i));
        boolformula_set(ret, i, temp);
        boolformula_free(temp);
      }
      break;
    case LITERAL:
      ret = boolformula_literal_new(f->d.l);
      break;
  }
  ASSERT(f->t == ret->t, "");

  return ret;
}
