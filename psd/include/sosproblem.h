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
#include "config.h"
#include "declarations.h"
#include "poly.hpp"
#include "polytype.h"
#include "sparse.h"
#include "support_table.h"
#include "vartable.hpp"
#include <stdio.h>
#include <stdlib.h>

#include <vector>

namespace aiSat {
namespace psd {
using namespace ::std;

typedef sparseblock Sparseblock;

typedef blockmatrix Blockmatrix;

typedef blockrec Blockrec;

Sparseblock *createblock( const int bnum, const int consnum,
                          const ArrangeMatrix *s );

Sparseblock *createSparseblock( const int bnum, const int bsize,
                                const int consnum, const int nument );

typedef struct constraintmatrix Constraintmatrix;

Constraintmatrix *createNconstraintmatrix( const int n );

void frontInsertBlock( Constraintmatrix *matrix, Sparseblock *block );

void createBlockMatrixC( int blockSize[], const int bnum, Blockmatrix * );

void addMonomial( indice_t **const array, const indice_t *element,
                  int *const capcity, int *const size, const int n );

void deleteAllIndices( indice_t **all, const int length );

/*
  sum_{i=0}^{size-1} polys[i]*polyConstraints[i]=rhs
*/
class SOSProblem {

public:
  SOSProblem() {}
  ~SOSProblem() { clear(); }

  void clear() {
    polys.clear();
    polyConstraints.clear();
  }
  void setRhs( const Poly_t &r ) {
    rhs = r;
    rhs.update();
  }

  Poly_t &getRhs( void ) { return rhs; }

  void deleteRhs( void ) {

    rhs = Poly_t();
    rhs.setInit( false );
  }
  int size() const { return (int) polys.size(); }

  Poly_t &getPoly( const int id ) { return polys[ id ]; }

  PolyConstraint &getPolyConstraint( const int id ) {
    return polyConstraints[ id ];
  }

  int addElem( const Poly_t &poly, const PolyConstraint &polycons ) {
    int re = (int) polys.size();
    polys.push_back( poly );
    polys[ re ].update();
    polyConstraints.push_back( polycons );
    return re;
  }

  int inter_sdp( const int sep, const char *fproname,
                 const char *fsolname ) const;

  int sdp_solver( vector<Poly_t> &resP, const char *fproname,
                  const char *fsolname ) const;

private:
  Poly_t rhs;

  vector<Poly_t> polys;

  vector<PolyConstraint> polyConstraints;

  void wellform( const int sep, const int sosMId[], const int sosMap[],
                 const int blockSize[], const int blockMap[],
                 Blockmatrix *X ) const;

  indice_t **createAllIndices( int *const sosmMap, indice_t **const SOSM,
                               int const *lengthM, int size[],
                               vector<vector<indice_t>> &varMap,
                               vector<vector<indice_t>> &pvarMap,
                               vector<vector<indice_t>> &svarMap ) const;

  Constraintmatrix *createConstraintmatrx( int *consSize, int sosMId[],
                                           int sosMap[], int blockSize[],
                                           int blockMap[], int *bnum,
                                           double ** ) const;
  int               getSOSMsize( int sosmMap[], int sosMId[] ) const;

  Poly_t getConstraintPoly( const int index, const int sosMId[],
                            const int sosMap[], const int blockSize[],
                            const int blockMap[], Blockmatrix *const X ) const;
};
} // namespace psd
} // namespace aiSat
#endif
