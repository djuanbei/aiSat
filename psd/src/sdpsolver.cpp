

/**
 * @file   sdpsolver.c
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 15:28:21 2015
 *
 * @brief
 *
 *
 */

#include <iostream>

#include <assert.h>
#include "psdutil.h"
#include "sdpsolver.h"
#include "search.h"

namespace aiSat {
namespace psd {

SparseSOSblock *createSparseSOSblock(void) {
  SparseSOSblock *re;
  re = (SparseSOSblock *)malloc_d(sizeof(SparseSOSblock));
  re->capacity = 8; /* initialize capacity */
  re->value = (coef_t *)malloc_d(re->capacity * sizeof(coef_t));
  re->index = (int *)malloc_d(re->capacity * sizeof(int));
  re->size = 0;

  return re;
}

static Poly_t *getConstraintPoly(SOSProblem *const sys, const int index,
                                 const int sosMId[], const int sosMap[],
                                 const int blockSize[], const int blockMap[],
                                 Blockmatrix *const X) {
  ASSERT(index < sys->size && index >= 0, "overflow ");
  int i, k;
  k = 0;
  for (i = 0; i < sys->size; ++i) {
    if (index == i) {
      if (sys->polyConstraints[i]->type == EQ) {
        Poly_t *posPoly = sosConvertPoly(X, k, blockSize[k],
                                         sosMId[sosMap[abs(blockMap[k]) - 1]]);

        k++;
        Poly_t *negPoly = sosConvertPoly(X, k, blockSize[k],
                                         sosMId[sosMap[abs(blockMap[k]) - 1]]);

        negPoly->mult(-1);

        // p_mult_cons_assign(negPoly, -1);
        posPoly->mult_poly(*negPoly);
        delete negPoly;
        // p_add_Poly_assign_del(posPoly, negPoly);
        return posPoly;
      } else {
        Poly_t *poly = sosConvertPoly(X, k, blockSize[k],
                                      sosMId[sosMap[abs(blockMap[k]) - 1]]);
        return poly;
      }
    }

    if (sys->polyConstraints[i]->type == EQ) {
      k += 2;
    } else {
      k++;
    }
  }
  return NULL;
}

/**
 * @brief  interpolant:= -( 1/2+1/2f0+\sum_{f\in T_2}) >0
 *
 * @param sys
 * @param sep
 * @param sosMId
 * @param sosMap
 * @param blockSize
 * @param blockMap
 * @param X
 */
void wellform(SOSProblem *const sys, const int sep, const int sosMId[],
              const int sosMap[], const int blockSize[], const int blockMap[],
              Blockmatrix *const X) {
  int i, k;

  Poly_t check;

  Poly_t interpolant = (*(sys->rhs)) * (0.5);

  cout << std::endl;
  k = 0;
  for (i = 0; i < sys->size; i += 1) {
    cout << endl
         << i + 1 << "............................... " << i + 1 << endl;
    cout << "....................................." << endl;

    if (sys->polyConstraints[i]->type == EQ) {
      Poly_t *posPoly = sosConvertPoly(X, k, blockSize[k],
                                       sosMId[sosMap[abs(blockMap[k]) - 1]]);

      k++;
      Poly_t *negPoly = sosConvertPoly(X, k, blockSize[k],
                                       sosMId[sosMap[abs(blockMap[k]) - 1]]);
      negPoly->mult(-1.0);

      posPoly->add_poly(*negPoly);
      delete negPoly;

      cout << i + 1 << " normal polynomial :" << endl;
      string str = posPoly->toString();
      cout << str;
      Poly_t mult = (*(sys->polys[i])) * (*posPoly);

      if (i + 1 >= sep) {
        interpolant.add_poly(mult);
      }
      check.add_poly(mult);
      delete posPoly;

    } else {
      Poly_t *poly = sosConvertPoly(X, k, blockSize[k],
                                    sosMId[sosMap[abs(blockMap[k]) - 1]]);

      Poly_t mult = (*sys->polys[i]) * (*poly);

      if (i + 1 >= sep) {
        interpolant.add_poly(mult);
      }
      check.add_poly(mult);

      cout << i + 1 << " sos polynomial :" << endl;

      string str = poly->toString();
      cout << str;

      if (0 == i) {
        poly->mult(0.5);
        interpolant.add_poly(*poly);
      }
      delete poly;
    }
    k++;
  }
  cout << endl << "check: 1+sum_i(fi*si)=0:::::::::::::::::;" << endl;

  Poly_t dummy(*sys->rhs);

  dummy.mult(-1);
  check.add_poly(dummy);

  string str = check.toString();
  cout << str;

  cout << endl << " Intepolant polynomial is :" << endl;

  interpolant.mult(-1);

  str = interpolant.toString();
  cout << str;
  cout << " > 0" << endl;
}

void enlargeSparseSOSblock(SparseSOSblock *const block) {
  block->capacity = ENLARGE_RAT * (block->capacity) + 1;
  block->value = (coef_t *)realloc_d(block->value,
                                     (size_t)block->capacity * sizeof(coef_t));
  block->index =
      (int *)realloc_d(block->index, (size_t)block->capacity * sizeof(int));
}

void deleteSparseSOSblock(SparseSOSblock *block) {
  free(block->index);
  free(block->value);
  free(block);

  return;
}

void addSparse(SparseSOSblock *const block, const int index,
               const coef_t value) {
  if (block->size == block->capacity) {
    enlargeSparseSOSblock(block);
  }
  block->index[block->size] = index;
  block->value[block->size] = value;
  block->size++;
}

Sparseblock *createblock(const int blockNum, const int consnum,
                         ArrangeMatrix const *s) {
  int i;
  Sparseblock *re;

  re = (Sparseblock *)malloc_d(sizeof(Sparseblock));

  re->blocknum = blockNum;
  re->blocksize = s->getRowLength();
  re->constraintnum = consnum;
  re->next = NULL;
  re->nextbyblock = NULL;
  re->entries = (double *)malloc_d((s->size() + 1) * sizeof(double));
  re->iindices = (int *)malloc_d((s->size() + 1) * sizeof(int));
  re->jindices = (int *)malloc_d((s->size() + 1) * sizeof(int));
  re->numentries = s->size();
  for (i = 0; i < s->size(); i += 1) {
    re->iindices[i] = (*s)[i].row;
    re->jindices[i] = (*s)[i].col;
    re->entries[i] = s->getCF();
  }

  return re;
}

Sparseblock *createSparseblock(const int blockNum, const int bsize,
                               const int consnum, const int nument) {
  Sparseblock *re = (Sparseblock *)malloc_d(sizeof(Sparseblock));
  re->blocknum = blockNum;
  re->blocksize = bsize;
  re->constraintnum = consnum;
  re->next = NULL;
  re->nextbyblock = NULL;
  re->entries = (double *)malloc_d((nument + 1) * sizeof(double));
  re->iindices = (int *)malloc_d((nument + 1) * sizeof(int));
  re->jindices = (int *)malloc_d((nument + 1) * sizeof(int));
  re->numentries = nument;
  return re;
}

Constraintmatrix *createNconstraintmatrix(const int n) {
  int i;
  Constraintmatrix *re =
      (Constraintmatrix *)malloc_d((n + 1) * sizeof(Constraintmatrix));
  for (i = 1; i <= n; i++) {
    re[i].blocks = NULL;
  }
  return re;
}

void frontInsertBlock(Constraintmatrix *matrix, Sparseblock *block) {
  block->next = matrix->blocks;
  matrix->blocks = block;
}

/**
 * @brief  construct sdp's matrix  C. blockNum is the number of block of
 * matrix C. n
 *  is the respect to polynomial's number of variables and d is the total degree
 * of
 *  respect with polynomial . We need d is a even number. blockSize every block
 * matrix
 *  size
 *
 * @param blockSize
 * @param blockNum
 * @param C
 */
void createBlockMatrixC(int blockSize[], int const blockNum, Blockmatrix *C) {
  int i;

  C->nblocks = blockNum;

  C->blocks = (Blockrec *)malloc_d(
      (blockNum + 1) * sizeof(Blockrec)); /* index all start from 1 */

  for (i = 0; i < blockNum; i += 1) {
    C->blocks[i + 1].blockcategory = MATRIX;
    C->blocks[i + 1].blocksize = blockSize[i];
    C->blocks[i + 1].data.mat =
        (double *)calloc((size_t)(blockSize[i] * blockSize[i]), sizeof(double));

    if (C->blocks[i + 1].data.mat == NULL) {
      fprintf(stderr, "\ndynamic memory allocation failed\n");
      exit(EXIT_FAILURE);
    }

    /*      for ( j = 1; j < blockSize[i]+1; j += 1 ) {
     *
     *          for ( k = 1; k <=j; k += 1 ) {
     *              C->blocks[i+1].data.mat[ijtok(j,k,blockSize[i])]=rand()%10-5;
     *              if(k!=j)
     *C->blocks[i+1].data.mat[ijtok(k,j,blockSize[i])]=C->blocks[i+1].data.mat[ijtok(j,k,blockSize[i])];
     *
     *          }
     *      }
     */
  }
}

/**
 * @brief sys is the set of polynomials, n is the number of variables d
 *  is the  total degree of multiplication  sos polynomial.
 *  Find all monomials occurs in this multiplicity system.
 *
 * @param sys
 * @param sosmMap
 * @param SOSM
 * @param lengthM
 * @param reSize
 * @param varMap
 * @param polyVarMap
 * @param sosVarMap
 *
 * @return
 */
indice_t **createAllIndices(SOSProblem *const sys, int *const sosmMap,
                            indice_t **const SOSM, int const *lengthM,
                            int reSize[], vector<vector<indice_t> > &varMap,
                            vector<vector<indice_t> > &polyVarMap,
                            vector<vector<indice_t> > &sosVarMap) {
  int i, k, p, pi, si;
  int choose;
  int psize;
  int polyVarSize;
  int sosVarSize;

  const int n = getVarTable<indice_t>().getAllVarNum(); /* all the variables */

  indice_t temp1[n];
  indice_t temp2[n];
  /*    indice_t index[n];
   */

  int varSize[sys->size];
  int multVarId[sys->size];
  int capacity[sys->size];

  indice_t **re;
  varMap.resize(sys->size);
  polyVarMap.resize(sys->size);
  sosVarMap.resize(sys->size);

  for (i = 0; i < sys->size; i += 1) {
    reSize[i] = 0;

    capacity[i] =
        lengthM[sosmMap[i]] * ENLARGE_RAT /
        (i * i * i + 1); /* here can have more smart method to deal with */

    getVarTable<indice_t>().getVarElem(sys->polys[i]->getVarId(), polyVarMap[i]);

    getVarTable<indice_t>().getVarElem(
        SUPPORT_TABLE.getSupElem((sys->polyConstraints[i])->supportId)->varId,
        sosVarMap[i]);

    multVarId[i] = getVarTable<indice_t>().mergeVar(
        sys->polys[i]->getVarId(),
        SUPPORT_TABLE.getSupElem((sys->polyConstraints[i])->supportId)->varId);

    getVarTable<indice_t>().getVarElem(multVarId[i], varMap[i]);

    varSize[i] = getVarTable<indice_t>().getVarNum(multVarId[i]);
  }

  re = (indice_t **)malloc_d((sys->size) * sizeof(indice_t *));

  for (i = 0; i < sys->size; i += 1) {
    re[i] = (indice_t *)malloc_d(varSize[i] * capacity[i] * sizeof(indice_t));
  }

  for (p = 0; p < sys->size; p += 1) { /* polynomial index */

    psize = sys->polys[p]->getSize(); /* psize polynomial term size */

    int index[psize];
    int jump[psize];

    for (i = 0; i < psize; i += 1) {
      jump[i] = index[i] = 0;
    }

    choose = 1;

    while (choose) {
      choose = 0;

      for (k = 0; k < psize; k += 1) { /* polynomial term size */

        if (index[k] < lengthM[sosmMap[p]]) {
          polyVarSize = getVarTable<indice_t>().getVarNum(sys->polys[p]->getVarId()); 
          sosVarSize = getVarTable<indice_t>().getVarNum(SUPPORT_TABLE.getSupElem((sys->polyConstraints[i])->supportId)->varId ); 

          pi = 0;
          si = 0;

          if (0 == choose) {
            choose = 1;
            jump[k] = 1;

            for (i = 0; i < varSize[p]; i += 1) {
              if (pi < polyVarSize && si < sosVarSize &&
                  polyVarMap[p][pi] == varMap[p][i] &&
                  sosVarMap[p][si] == varMap[p][i]) {
                temp1[i] = sys->polys[p]->getDegreeAt(k, pi) +
                           SOSM[sosmMap[p]][sosVarSize * index[k] + si];

                si++;
                pi++;

              } else if (pi < polyVarSize &&
                         polyVarMap[p][pi] == varMap[p][i ]) {
                temp1[i] = sys->polys[p]->getDegreeAt(k, pi);
                pi++;
              } else if (si < sosVarSize &&
                         sosVarMap[p][si] == varMap[p][i]) {
                temp1[i] = SOSM[sosmMap[p]][sosVarSize * index[k] + si];
                si++;
              } else {
                ASSERT(0, "this case never been consider");
              }
            }

          } else {
            for (i = 0; i < varSize[p]; i += 1) {
              if (pi < polyVarSize && si < sosVarSize &&
                  polyVarMap[p][pi] == varMap[p][i ] &&
                  sosVarMap[p][si] == varMap[p][i]) {
                temp2[i] = sys->polys[p]->getDegreeAt(k, pi) +
                           SOSM[sosmMap[p]][sosVarSize * index[k] + si];

                si++;
                pi++;

              } else if (pi < polyVarSize &&
                         polyVarMap[p][pi ] == varMap[p][i ]) {
                temp2[i] = sys->polys[p]->getDegreeAt(k, pi);
                pi++;
              } else {
                temp2[i] = SOSM[sosmMap[p]][sosVarSize * index[k] + si];
                si++;
              }
            }

            int com = compare(temp2, temp1, varSize[p]);

            if (com == -1) { /* temp2 < temp1  instead temp1 by temp2*/

              for (i = 0; i < varSize[p]; i += 1) {
                temp1[i] = temp2[i];
              }

              for (i = 0; i < k; i += 1) {
                jump[i] = 0;
              }

              jump[k] = 1;

            } else if (com == 0) { /* temp1==temp2 */
              jump[k] = 1;
            }
          }
        }
      }

      if (choose) {
        for (i = 0; i < p; i += 1) {
          if (findlocation(re[i], reSize[i], temp1, &(varMap[i])[0],
                           &(varMap[p])[0]) > -1)
            break;  // check whether have added
        }
        if (i == p) {
          addMonomial(re + p, temp1, capacity + p, reSize + p, varSize[p]);
          /*                    reSize[p]++;
           */
        }

        for (i = 0; i < psize; i += 1) {
          if (jump[i]) {
            index[i]++;
            jump[i] = 0;
          }
        }
      }
    }
  }

  return re;
}

void deleteAllIndices(indice_t **all, const int length) {
  int i;

  for (i = 0; i < length; i += 1) {
    free(all[i]);
  }
  free(all);
}

void addMonomial(indice_t **const array, const indice_t *element,
                 int *const capacity, int *const size, const int n) {
  int i;

  if (*size == *capacity) {
    *capacity = (int)(*capacity) * ENLARGE_RAT + 1;
    *array = (indice_t *)realloc_d(*array,
                                   (size_t)(*capacity) * n * sizeof(indice_t));
  }

  for (i = 0; i < n; i += 1) {
    (*array)[(*size) * n + i] = element[i];
  }

  (*size)++;
}

/**
 *@brief  sys  allM is the set of all the monomials in
 *  the sys. numofCons  is the number of variables in sys. deg is the total
 *degree   of sos
 *  polynomial consSize is the length of allM . blocksize is the number of
 *coefficients occor
 *  in the polynomial system
 *
 * @param sys is a polynomials constraints system.
 * @param numofCons in semideinite prigramming the number of constraint
 *<A_i,X>=b_i
 * @param sosMId
 * @param sosmMap
 * @param blockSize
 * @param blockMap
 * @param blockNum
 * @param b
 *
 * @return
 */
Constraintmatrix *createConstraintmatrx(SOSProblem *const sys,
                                        int *const numofCons, int sosMId[],
                                        int sosmMap[], int blockSize[],
                                        int blockMap[], int *blockNum,
                                        double **b) {
  int i, j, k, h, p, pi, si, sum, tempI;
  int pSize, sSize, mSize;
  int index, tempIndex;
  indice_t key[getVarTable<indice_t>().getAllVarNum()];
  Sparseblock *blockptr;
  /*    int sosMId[sys->size];
   */
  int diffSOSMIdSize;
  int everyConstraintSize[sys->size];
  vector<vector<indice_t> > varMap(sys->size);
  vector<vector<indice_t> > polyVarMap(sys->size);
  vector<vector<indice_t> > sosVarMap(sys->size);

  /*-----------------------------------------------------------------------------
   *  At now  multiplied sos polynomials to difference polynomials are same.
   *  I will remove this restraintion make program more flexible.
   *12/03/2012 04:34:54 PM
   *-----------------------------------------------------------------------------*/

  diffSOSMIdSize = getSOSMsize(sys, sosmMap, sosMId);

  indice_t *SOSM[diffSOSMIdSize];

  int tempblockSize[diffSOSMIdSize];

  int lengthM[diffSOSMIdSize];

  for (i = 0; i < diffSOSMIdSize; i += 1) {
    SOSM[i] = SUPPORT_TABLE.getSOSsup(sosMId[i], lengthM + i);
    if (NULL == SOSM[i]) {
      SOSM[i] = getAllMonByTd(
          sosMId[i],
          lengthM + i); /* all monomial has  occured in SOS polynomial */
      SUPPORT_TABLE.setSOSsup(sosMId[i], lengthM[i], SOSM[i]);
    }
  }

  SparseSOSblock ***block;

  /*-----------------------------------------------------------------------------
   *  Every Monomial in SOSM has some combined methods
   *-----------------------------------------------------------------------------*/

  ArrangeMatrix **AM[diffSOSMIdSize];

  for (i = 0; i < diffSOSMIdSize; i += 1) {
    AM[i] = SUPPORT_TABLE.getAMIndex(sosMId[i], tempblockSize + i);
    if (NULL == AM[i]) {
      AM[i] = createArrangeM(sosMId[i], SOSM[i], tempblockSize + i,
                             lengthM[i]); /* (Z^T * Z)_c =AM */
    }
  }

  *blockNum = 0;
  for (i = 0; i < sys->size; i += 1) {
    if (sys->polyConstraints[i]->type == EQ) {
      blockSize[*blockNum] = tempblockSize[sosmMap[i]];
      blockMap[*blockNum] = i + 1; /* more attention on +1 */
      blockSize[*blockNum + 1] = tempblockSize[sosmMap[i]];
      blockMap[*blockNum + 1] = -(i + 1);
      (*blockNum) += 2; /* EQ coef equivalent GT or GE */

    } else {
      blockSize[*blockNum] = tempblockSize[sosmMap[i]];
      blockMap[*blockNum] = i + 1;
      (*blockNum)++;
    }
  }

  /*
   * find all the monomials occur in this polynomial system when  w.r.t
   * multiplied sos polynomials. delete repeat
   */
  indice_t **allM =
      createAllIndices(sys, sosmMap, SOSM, lengthM, everyConstraintSize, varMap,
                       polyVarMap, sosVarMap);

  (*numofCons) = 0;

  for (i = 0; i < sys->size; i += 1) {
    (*numofCons) += everyConstraintSize[i];
  }

  *b = (double *)calloc_d(((*numofCons) + 1), sizeof(double));

  if (NULL != sys->rhs) {
    Poly_t *rhs = sys->rhs;
    rhs->update();

    vector<indice_t> rhsVars;
    getVarTable<indice_t>().getVarElem(rhs->getVarId(), rhsVars);


    int size = rhs->getSize();
    for (i = 0; i < size; i++) {
      vector<indice_t> key;
      rhs->getDegreeAt(i, key);
      index = 0;
      k = 0;
      while (k < sys->size) {
        tempIndex = findlocation(allM[k], everyConstraintSize[k], &key[0],  varMap[k].size(),
                                 &(varMap[k])[0], rhsVars.size() ,  &rhsVars[0]);
        if (tempIndex > -1) {
          index += tempIndex;
          break;
        } else
          index += everyConstraintSize[k];

        k++;
      }
      if (k < sys->size) {
        (*b)[index + 1] = rhs->getCF(i);  // b start from 1
      }
    }
  }

  Constraintmatrix *re = createNconstraintmatrix(
      *numofCons); /* every monomial w.r.t. to a constraint */

  block =
      (SparseSOSblock ***)malloc_d((*numofCons) * sizeof(SparseSOSblock **));

  for (i = 0; i < *numofCons; i += 1) {
    block[i] =
        (SparseSOSblock **)malloc_d((*blockNum) * sizeof(SparseSOSblock *));

    for (j = 0; j < *blockNum; j += 1) {
      /*-----------------------------------------------------------------------------
       *  i is the number of constraint j is the number of block, and every
       *block has a
       *  sosM polynomial respective.
       *-----------------------------------------------------------------------------*/
      block[i][j] = createSparseSOSblock(); /* every polynomial with regard to a
                                               constraint block */
    }
  }

  for (i = 0; i < *blockNum; i += 1) {
    p = blockMap[i];
    if (p < 0) p = -p;
    p--; /* p  sys polynomial index */

    int size = sys->polys[p]->getSize();
    for (h = 0; h < size; h += 1) {
      for (j = 0; j < lengthM[sosmMap[p]];
           j += 1) { /*lengthM is the length of  SOS's Monomial */

        pi = si = 0;

        pSize = polyVarMap[p].size();
        sSize = sosVarMap[p].size();
        mSize = varMap[p].size();

        for (k = 0; k < mSize; k += 1) {
          if (pi < pSize && si < sSize &&
              polyVarMap[p][pi ] == varMap[p][k ] &&
              sosVarMap[p][si ] == varMap[p][k ]) {
            key[k] = sys->polys[p]->getDegreeAt(h, pi) +
                     SOSM[sosmMap[p]][sSize * j + si];

            si++;
            pi++;
          } else if (pi < pSize && polyVarMap[p][pi ] == varMap[p][k ]) {
            key[k] = sys->polys[p]->getDegreeAt(h, pi);

            pi++;
          } else {
            key[k] = SOSM[sosmMap[p]][sSize * j + si];
            si++;
          }
        }

        index = 0;
        k = 0;
        while (k < sys->size) {
          tempIndex = findlocation(allM[k], everyConstraintSize[k], key, varMap[k].size(), 
                                   &(varMap[k])[0], varMap[p].size(),  &(varMap[p])[0]);
          if (tempIndex > -1) {
            index += tempIndex;
            break;
          } else
            index += everyConstraintSize[k];

          k++;
        }

        if (blockMap[i] > 0) {
          addSparse(block[index][i], j, sys->polys[p]->getCF(h));
        }
        /* >= index is the constraints
           location and i is the number of
           block, j is the index of SOSM. */
        else {
          addSparse(block[index][i], j, (-1) * (sys->polys[p]->getCF(h)));
        }
        /* <= index is the constraints location
           and j is the number of polynomial it
           respect to. */
      }
    }
  }

  /*-----------------------------------------------------------------------------
   *  use block to  construct  constraints
   *-----------------------------------------------------------------------------*/
  for (i = 0; i < *numofCons; i += 1) {
    for (j = *blockNum - 1; j >= 0;
         j -= 1) { /* notice the order of polynomial is reverse  */

      sum = 0; /* sum  the respect to block size */

      /*-----------------------------------------------------------------------------
       *  blockMap: block-> poly; sosMMap: poly ->sosM
       *-----------------------------------------------------------------------------*/
      p = sosmMap[abs(blockMap[j]) - 1]; /* which sosM */

      for (k = 0; k < block[i][j]->size; k += 1) {
        sum += AM[p][block[i][j]->index[k]]->size();
      }

      blockptr = createSparseblock(
          j + 1, blockSize[j], i + 1,
          sum); /* j+1  in the first location  system  and index start form 1 */

      tempI = 1;

      for (k = 0; k < block[i][j]->size; k += 1) {
        for (h = 0; h < AM[p][block[i][j]->index[k]]->size(); h += 1) {
          blockptr->iindices[tempI] = (*(AM[p][block[i][j]->index[k]]))[h].row +
                                      1; /* row start form 1 */
          blockptr->jindices[tempI] = (*(AM[p][block[i][j]->index[k]]))[h].col +
                                      1; /* colum start form 1 */

          /*-----------------------------------------------------------------------------
           *   There need must attention bolckptr->entries is
           *   double type bute block[i][j]->value[k] is coef_t type.
           *   If we need do some convert, please sure they can exchange
           *-----------------------------------------------------------------------------*/

          blockptr->entries[tempI] = block[i][j]->value[k];
          tempI++;
        }
      }

      frontInsertBlock(re + i + 1, blockptr);
    }
  }

  for (i = 0; i < *numofCons; i += 1) {
    for (j = 0; j < *blockNum; j += 1) {
      deleteSparseSOSblock(block[i][j]);
    }
    free(block[i]);
  }
  free(block);

  /*    deleteW(W, diffSOSMIdSize, lengthM );
   */

  deleteAllIndices(allM, sys->size);

  return re;
} /* -----  end of function createConstraintmatrix  ----- */

/**
 *@brief   From Putiner's positivtellensatz Theorem
 *  The main function we use.
 *  num is a label of whether output the problem solution in a file. sys is the
 *  polynomials . The sys is a constrain system where all the constraints are
 *  inequalities(>=). sys contains two subsystem. >= sep is the first system the
 *other
 *  are the second system. The constraints satsify Archimedean condition.  Under
 *the
 *  sys has no solution in another word the system
 *  is unsatisfied. By real algebra  theorem  (Putiner's positivtellensatz
 *Theorem)
 *  and semidefinite programming mostly we find
 *  a set of sos polynomials which satisfy
 *sys->rhs=sos_0+sos_1f_1+...+sos_{sep}f_{sep}+sos_{sep+1}f_{sep+1}...
 *  . We can easily find 1/2sos_0+sos_1f_1+...+sos_{sep}f_{sep} is a Craig
 *interpolation
 *
 * @param sys contain all the consraint equality and inequality and the rhs
 *polynomial
 * @param sep  the location of interoplant
 * @param fprobname the file name of write matix problem which convert from the
 *original  polynomials.
 * @param fsolname the file name of output
 *
 * @return 0 if success
 *   otherwise failure
 */
int inter_sdp(SOSProblem *const sys, const int sep, char const *fprobname,
              char const *fsolname) {
  int ret = 0;
  int numofCons;
  int blockSize[2 * (sys->size)]; /* the blockmatrix size */
  int blockMap[2 * (sys->size)];  /* block -> poly map */
  int sosMId[sys->size];          /* sosMId: coefPoly-> sosMid*/
  int sosMap[sys->size];          /* every coefpoly has a sosM */
  int blockNum = 0;
  int i;
  /**
   *
   * The semidefinite programming need data.
   *
   * max : trace (CX) i.e. <C,X>
   *      s.t. (*constraints)*X = b
   *      X>=0 is a semidefinite matrix.
   *
   *      And there C is a zero block matrix.
   *      The matrix X with respect to the monomial will used in Putiner's
   *positivtellensatz Theorem
   *      b is a vector with respect to sys->rhs.
   *      Main work we will do is working out the number of blocks C need and
   *the
   *      block size. and the length of b.
   *      Construct constraints.
   */

  Blockmatrix C;

  Constraintmatrix *constraints;

  Blockmatrix X, Z;
  double *y;
  double pobj, dobj;
  // int constantIndex;

  double *b = NULL;

  //(double*) malloc_d(sizeof(double) );
  //  free(b);
  /**
   * the key step for me
   *
   */
  constraints = createConstraintmatrx(sys, &numofCons, sosMId, sosMap,
                                      blockSize, blockMap, &blockNum,
                                      &b); /* this place has some problem */

  createBlockMatrixC(blockSize, blockNum,
                     &C); /* 1 is a default polynomial we want to notice  */

  //  double *b=malloc_d((numofCons+1)*sizeof(double));;

  // for ( i = 0; i < numofCons+1; i += 1 ) {
  //   b[i]=0;
  // }

  // printf ( "const in index %d\n",constantIndex+1 ); /* index start from 1 */
  // b[constantIndex+1]=-1; /*  the sum of the mult polynomials with sos
  // polynomial equal to -1*/

  /* index start from 1 not 0 */

  int numofblock = 0;

  for (i = 0; i < blockNum; i += 1) {
    numofblock += blockSize[i];
  }

  write_prob(fprobname, numofblock, numofCons, C, b,
             constraints); /* write of the problem in fprobname file */

  initsoln(numofblock, numofCons, C, b, constraints, &X, &y, &Z);

  ret = easy_sdp(numofblock, numofCons, C, b, constraints, 0.0, &X, &y, &Z,
                 &pobj, &dobj);

  if (ret < 2) {
    wellform(sys, sep, sosMId, sosMap, blockSize, blockMap, &X);

  } else {
    printf("SDP failed.\n");
  }

  /*
   * Write out the problem solution.
   *
   */
  /*
    int bo,row,col;

    for ( bo = 1; bo < sys->size+1; bo += 1 ) {

    for ( row = 1; row < blockSize+1; row += 1 ) {

    for ( col = 1; col < blockSize+1; col += 1 ) {
    printf ( "%.3e  ",X.blocks[bo].data.mat[ijtok(row,col,blockSize)] );
    }
    printf ( "\n" );
    }
    printf ( "=======================\n" );
    }

  */
  write_sol(fsolname, numofblock, numofCons, X, y, Z);

  /*
   * Free storage allocated for the problem and return.
   */
  free_prob(numofblock, numofCons, C, b, constraints, X, y, Z);

  return ret;
}

/**
 * @brief
 * From Putiner's positivtellensatz Theorem
 *  Description:  The main function we use.
 *  num is a label of whether output the problem solution in a file. sys is the
 *  polynomials . The sys is a constrain system where all the constraints are
 *  inequalities(>=). sys contains two subsystem. >= sep is the first system the
 *other
 *  are the second system. The constraints satsify Archimedean condition.  Under
 *the
 *  sys has no solution in another word the system
 *  is unsatisfied. By real algebra  theorem  (Putiner's positivtellensatz
 *Theorem)
 *  and semidefinite programming mostly we find
 *  a set of sos polynomials which satisfy
 *  find sys->cons satisfy that
 *  \sum_{i=0}^{sys->size-1}sys->polys[i]*sys->cons[i]=rhs->rhs
 * @param sys
 * @param resP
 * @param fprobname
 * @param fsolname
 *
 * @return
 */
int sdp_solver(SOSProblem *const sys, Poly_t **resP, char const *fprobname,
               char const *fsolname) {
  int ret = 0;
  int numofCons;

  int blockSize[2 * (sys->size)]; /* the blockmatrix size */
  int blockMap[2 * (sys->size)];  /* block -> poly map */
  int sosMId[sys->size];          /* sosMId: coefPoly-> sosMid*/
  int sosMap[sys->size];          /* every coefpoly has a sosM */
  int blockNum = 0;
  int i;
  /*
   * The semidefinite programming need data.
   *
   *
   *
   *max : trace (CX)
   *     s.t. (*constraints)*X = b
   *      X>=0 is a semidefinite matrix.
   *
   *      And there C is a zero block matrix.
   *      b with respect to sys->rhs.
   *      Main work we will do is working out the number of blocks C need and
   *the
   *      block size. and the length of b.
   *      Construct constraints.
   */

  Blockmatrix C;

  Constraintmatrix *constraints;

  Blockmatrix X, Z;
  double *y;
  double pobj, dobj;
  //  int constantIndex;

  double *b = NULL;

  constraints = createConstraintmatrx(sys, &numofCons, sosMId, sosMap,
                                      blockSize, blockMap, &blockNum,
                                      &b); /* this place has some problem */

  createBlockMatrixC(blockSize, blockNum,
                     &C); /* 1 is a default polynomial we want to notice  */

  /* index start from 1 not 0 */

  int numofblock = 0;

  for (i = 0; i < blockNum; i += 1) {
    numofblock += blockSize[i];
  }

  write_prob(fprobname, numofblock, numofCons, C, b,
             constraints); /* write of the problem in fprobname file */

  initsoln(numofblock, numofCons, C, b, constraints, &X, &y, &Z);

  ret = easy_sdp(numofblock, numofCons, C, b, constraints, 0.0, &X, &y, &Z,
                 &pobj, &dobj);

  //  wellform(sys , sep , sosMId, sosMap, blockSize, blockNum, blockMap ,&X);
  if (ret == 0) {
    for (i = 0; i < sys->size; i++) {
      resP[i] =
          getConstraintPoly(sys, i, sosMId, sosMap, blockSize, blockMap, &X);
    }

    /*      wellform(sys , sep , sosMId, sosMap, blockSize, blockNum, blockMap
     * ,&X);
     */

  } else {
    printf("SDP failed.\n");
  }

  /*
   * Write out the problem solution.
   *
   */
  /*

   */
  write_sol(fsolname, numofblock, numofCons, X, y, Z);

  /*
   * Free storage allocated for the problem and return.
   */
  free_prob(numofblock, numofCons, C, b, constraints, X, y, Z);

  return ret;
}

/**
 *
 *
 * @param sys all the constraits polynomials
 * @param sosmMap the location of polynomial->support in array sosMId
 * @param sosMId the array of  support id
 * @return the different support if accour in sys
 */

int getSOSMsize(SOSProblem *const sys, int sosmMap[], int sosMId[]) {
  int re = 0;
  int i = 0;
  int j = 0;

  for (i = 0; i < sys->size; i += 1) {
    j = 0;

    while (j < re) {
      /// different  sys->polyConstraints[i] may use
      /// sosMId[j]==sys->polyConstraints[i]->supportId.
      /// Since the same monomial bases will definite different constrains such
      /// as c_00+c_10x+c_01y==0 and
      /// b_00+b_10x+b_01y >=0
      // This stretegy avoid repeat computing
      if (sosMId[j] == sys->polyConstraints[i]->supportId) break;
      j++;
    }

    sosmMap[i] = j;

    if (re == j) {
      sosMId[j] = sys->polyConstraints[i]->supportId;
      re++;
    }
  }

  return re;
}
}
}
