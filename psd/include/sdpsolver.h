
/**
 * @file   sdpsolver.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 11:41:55 2015
 *
 * @brief
 *
 *
 */

#ifndef CSDPINTER_INC
#define CSDPINTER_INC
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "declarations.h"
#include "poly.hpp"
#include "polytype.h"
#include "sparse.h"
#include "support_table.h"
#include "vartable.hpp"
namespace aiSat {
namespace psd {

struct sparseSOSblock {
  int size;
  int capacity;
  coef_t *value;
  int *index; /* location of sosM */
};

typedef struct sparseSOSblock SparseSOSblock;

typedef struct blockrec Blockrec;

typedef struct blockmatrix Blockmatrix;

typedef struct sparseblock Sparseblock;

SparseSOSblock *createSparseSOSblock(void);

void enlargeSparseSOSblock(SparseSOSblock *const bolck);

void deleteSparseSOSblock(SparseSOSblock *block);

void addSparse(SparseSOSblock *const block, const int index,
               const coef_t value);

Sparseblock *createblock(const int bnum, const int consnum,
                         const ArrangeMatrix *s);
/*
 * ===  FUNCTION
 * ======================================================================
 *         Name:  createSparseblock
 *  Description:
 * =====================================================================================
 */
Sparseblock *createSparseblock(const int bnum, const int bsize,
                               const int consnum, const int nument);

void wellform(SOSProblem *const sys, const int sep, const int sosMId[],
              const int sosMap[], const int blockSize[], const int blockMap[],
              Blockmatrix *X);

typedef struct constraintmatrix Constraintmatrix;

Constraintmatrix *createNconstraintmatrix(const int n);

void frontInsertBlock(Constraintmatrix *matrix, Sparseblock *block);

void createBlockMatrixC(int blockSize[], const int bnum, Blockmatrix *);

void addMonomial(indice_t **const array, const indice_t *element,
                 int *const capcity, int *const size, const int n);

indice_t **createAllIndices(SOSProblem *const sys, int *const sosmMap,
                            indice_t **const SOSM, int const *lengthM,
                            int size[], vector<vector<indice_t> > &varMap,
                            vector<vector<indice_t> > &pvarMap,
                            vector<vector<indice_t> > &svarMap);

void deleteAllIndices(indice_t **all, const int length);

Constraintmatrix *createConstraintmatrx(SOSProblem *const sys, int *consSize,
                                        int sosMId[], int sosMap[],
                                        int blockSize[], int blockMap[],
                                        int *bnum, double **);

int inter_sdp(SOSProblem *const sys, const int sep, const char *fproname,
              const char *fsolname);

int sdp_solver(SOSProblem *const sys, Poly_t **resP, const char *fproname,
               const char *fsolname);

int getSOSMsize(SOSProblem *const sys, int sosmMap[], int sosMId[]);
}
}
#endif
