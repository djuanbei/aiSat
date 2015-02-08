
/**
 * @file   polytype.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 11:45:01 2015
 * 
 * @brief  the basic type 
 * 
 * 
 */

#ifndef  POLYTYPE_INC
#define  POLYTYPE_INC
#include    <stdint.h>
#include    <stdlib.h>
#include    "pointlist.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif
  /**
   * An array of spare two dimension coordinate
   * 
   */

  struct sparseRowCol {
    uint32_t row: 16;
    uint32_t col: 16;
  };    

  typedef struct sparseRowCol SparseRowCol;

  typedef unsigned char  indice_t;

  typedef uint8_t var_t;

  typedef float coef_t;

  enum BOOL {
    FALSE=0, TRUE=1
  };    

  typedef enum BOOL BOOL;

  enum  monomialOrder {
    LEX, GRLEX,GREVLEX
  };    

  typedef enum monomialOrder MonomialOrder;


  enum monomialConstraintType {
    HOMOG, KHOMOG, SYMM
  };    

  typedef enum monomialConstraintType monomialConstraintType;

  struct monomialConstraint {
    int id;
    int deg;
    int varId;
    int *linCoefs;
    int minValue;                               /* only use in linconstraints */
    int consFunId;
  };    

  typedef struct monomialConstraint MonomialConstraint;

  enum polyConstraintType {
    EQ, GE, GT, NE
  };    

  typedef enum polyConstraintType PolyConstraintType;


  struct poly {
    int varId; /// the variables id of this polynomial
    int id;
    int capacity;
    int size;                                   /* Current terms length */
    indice_t *indices;/// exponents of this polynomial
    coef_t *coef;/// the coefficient of this polynomial

  };    

  typedef struct poly Poly;


  struct subPoly {
    Poly* poly;/// the reference polynomial
    uint8_t md5sum[DIGEST_SIZE]; ///the md5sum of this subpolynomial, when test whether the two polynomials equal first test 
    /// the two mds5sum equal
    int size; /// the size of monomials
    int *locs;
  };	

  typedef struct subPoly SubPoly;


  struct polyConstraint {
    PolyConstraintType type;    int supportId;
  };
  typedef struct polyConstraint PolyConstraint;


  enum supportType{
    NORMAL,
    SUB_POLY,
    INDICE
  };
  typedef enum supportType SupportType;

  struct support {
    
    SupportType type;
    //    BOOL isPoly;
    
    SubPoly* subp;

    uint8_t md5sum[DIGEST_SIZE];
 
    int deg;
    int varId;
    int *consId;
    int constNum;
    int consCap;
  };    
  typedef struct support Support;



  /*
    sum_{i=0}^{size-1} polys[i]*polyConstraints[i]=rhs
  */
  struct sosProblem {
    int cap;
    int size;
    Poly** polys;
    Poly* rhs;
    PolyConstraint ** polyConstraints;
  };    

  typedef struct sosProblem SOSProblem;

  typedef long  scalar_t;
  typedef unsigned long uscalar_t;
  typedef void *pointer_t;

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef POLYTYPE_INC  ----- */
