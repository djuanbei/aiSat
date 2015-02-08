

/**
 * @file   poly.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 11:42:38 2015
 * 
 * @brief  
 * 
 * 
 */

#ifndef  POLY_INC
#define  POLY_INC

#include	"polytype.h"
#include	<string.h>

#ifdef __cplusplus
extern "C" {
#endif

  SOSProblem * createSOSProblem(void);
  
  /*-----------------------------------------------------------------------------
   *  12/02/2012 12:25:10 PM
   *-----------------------------------------------------------------------------*/


  SOSProblem * createSOSProblemN(int size);

  void setConstraint(SOSProblem * sys, const int index, Poly *poly, PolyConstraint * cons   );
  

  int addConstraint(SOSProblem * sys, Poly *poly, PolyConstraint * cons  );

  void setSOSRhs(SOSProblem * sys, Poly *rhs);
  

  void clearSOSProblem(SOSProblem * sys);

  /*-----------------------------------------------------------------------------
   *  12/02/2012 12:35:29 PM
   *-----------------------------------------------------------------------------*/
  void deleteSOSProblem( SOSProblem * sys);

  Poly* createPoly(void);

  Poly * createPoly1(indice_t * vars, const int n);

  Poly * randPoly(const int varId, const coef_t coeffDome, const int termLength,
                  const int indiceDome);

  Poly* copyPoly( const Poly  *poly);

  /* void enlargePolycoef(Poly *const poly );
   */

  void polyChangeVarId( Poly *const  poly, const int varId);


  int poly_getTotalDegree(const Poly *poly);


  void enlargePoly(Poly *const poly );

  void printPoly(Poly *const poly);


  void   printPolyPrec ( Poly *const poly, const float minv  );

  void printPoly1(const Poly * poly);

  void updatePoly(Poly * const poly);

  void assign_del(Poly *lhs, Poly *rhs );

  void deletePoly(Poly *poly);

  BOOL isZero(Poly *poly);

  BOOL isOne(Poly *poly);

  BOOL isConstant(Poly *);

  BOOL isPositive(Poly *);

  BOOL isNegative(Poly *);
  
  void resetZero(Poly *poly);

  void resetOne(Poly *poly);


  void internal_addTerm(Poly *const poly, indice_t  key[], const coef_t cf);

  void p_add_Term_assign(Poly *const poly,indice_t const key[],  const coef_t cf);

  void p_mult_Term_assign(Poly *const poly,indice_t const key[],const coef_t cf);

  Poly* p_mult_Term(Poly const *poly,const indice_t  key[],const coef_t cf);

  void  p_mult_cons_assign(Poly * const poly , const coef_t cf);

  void  p_add_cons_assign(Poly * const poly, const coef_t cf);

  void p_add_Poly_assign(Poly *const poly1, Poly *const poly2);

  void p_add_Poly_assign_del(Poly *const poly1, Poly* const poly2);

  /* Poly * p_add_Poly1(Poly *const poly1, Poly *const poly2);
   */

  void p_mult_Poly_assign(Poly * poly1, Poly *const poly2);

  void p_mult_Poly_assign_del(Poly * poly1, Poly *const poly2);


  Poly* p_mult_Poly(const Poly   *poly1,const  Poly * poly2);

  void p_pow_assign(Poly *poly, const int p);
 
  indice_t ** mergeSort(indice_t  **const list1, indice_t **const list2, const int size1, const int size2, int *const size3,const int n); 

  indice_t ** mergeAll(SOSProblem *const  sys , int  *const size );

  PolyConstraint* createPolyConstraint(void);
  
  PolyConstraint *copyPolyConstraint(const PolyConstraint *polycons);
  

  void deletePolyConstraint(PolyConstraint *c);


  void printPolyConstraint(PolyConstraint * const cpoly);

  void enlargePolyConstraint(PolyConstraint * const coefpoly) ;

  void addCons(PolyConstraint * const cpoly, MonomialConstraint * const cons);

  MonomialConstraint* createConstraint(void);

  void deleteConstraint (MonomialConstraint * const cons);

  Support* createSupport(const int deg, const int varId, 
                         const int consNum,  int *consId);
  Support* createSupByPoly(SubPoly* poly );

  Support* createSupBySup(const int varId, const indice_t * indices, const int size);
  
  void printSupport(const int  id);

  void deleteSupport(Support *  S);

  void enlargeSupport(Support * const S);

  int  subpoly_getTotalDegree(SubPoly *sp);

  void md5sumbyIndice(uint8_t * re,  const int varId, const indice_t * indices, const int size);
			
#ifdef __cplusplus
}
#endif

	
#endif   /* ----- #ifndef POLY_INC  ----- */
