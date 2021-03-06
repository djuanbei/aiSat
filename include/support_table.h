/**
 * @file   support_table.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 11:13:28 2015
 *
 * @brief  Since every polynomial constrain is parametric polynomial
 * when the coefficients of the template constraint is concrete the
 * constraint is definited. And there are different polynomial contraints may
 * share monomial base such as polynomial contraints
 * c00+c01x+c10y+c11xy+c02x^2+c20y^2 >=0  and b00+b01x+b10y+b11xy+b02x^2+b20y^2
 * >=0
 * where c**, b** are unknown parameters and set {1, x, y, xy, x^2, y^2 } is the
 * monomial base.
 *
 *
 * f= G^T M G=<G G^T,M > = \sum_{i=1}^s g_i^2
 * G called half monomial set
 * all the elements occouring in GG^T called all monomial set
 * we define an ArrangeMatrix by the location   x^a  occurring in  for every
 * element x^a
 * in  GG^T. For example G=[ 1 x y z ]
 * G G^T=[ 1  x    y    z ]
 *       [ x  x^2  xy   xz]
 *       [ y  xy   y^2  yz]
 *       [ z  xz   yz  z^2], then the ArrangeMatrix corresponding monomial xy is
 *  [0  0  0  0]
 *  [0  0  1  0]
 *  [0  1  0  0]
 *  [0  0  0  0]
 */

#ifndef SUPPORT_TABLE_INC
#define SUPPORT_TABLE_INC

#include "bimap.h"
#include "config.h"
#include "polytype.h"
#include "sparse.h"

#ifdef __cplusplus
extern "C" {
#endif

  struct supporttable {
    /**
       For example G=[ 1 x y z ]
       * G G^T=[ 1  x    y    z ]
       *       [ x  x^2  xy   xz]
       *       [ y  xy   y^2  yz]
       *       [ z  xz   yz  z^2], then the ArrangeMatrix corresponding monomial
       xy is
       *  [0  0  0  0]
       *  [0  0  1  0]
       *  [0  1  0  0]
       *  [0  0  0  0].
       *Gsup=[ 1 x y z ], SOSsup=[ 1 x y z x^2 xy xz y^2 yz z^2]
       */

    int capacity;
    int size;
    Bimap *loc;
    int *sosLength;
    int *gLength;
    ArrangeMatrix ***arrangeM;
    /*-----------------------------------------------------------------------------
     *  monomial of p
     *-----------------------------------------------------------------------------*/
    indice_t **SOSsup;
    /*-----------------------------------------------------------------------------
     * p=sum_i g_i^2
     * monomial of g
     *-----------------------------------------------------------------------------*/
    indice_t **Gsup;

    Support **values;
  }; /* ----------  end of struct coefMtable  ---------- */

  typedef struct supporttable Supporttable;
  /*
   * return >=0 if find
   * otherwise <0
   */

  int findSupByPoly(const SubPoly *poly);

  indice_t *getSOSsup(const int id, int *len);

  int getsosSLength(const int id);

  void setSOSsup(const int id, const int len, indice_t *value);

  void setGsup(const int id, const int len, indice_t *value);

  void setArrangeM(const int id, ArrangeMatrix **value, const int gLength);

  indice_t *getGsup(const int id, int *len);

  ArrangeMatrix **getAMIndex(const int id, int *gLength);

  int addSOSsup(const int deg, const int varId, const int consNum, int *consId);

  int addSOSsupByIndice(const int varId, indice_t *indices, const int size);

  int addconvexsosSup(SubPoly *poly);

  /* 	int findSupElem( const int deg , const int varId,
   * 				const int consNum, const int * consId);
   */

  Support *getSupElem(const int id);

  void clearSupportTable(void);

  void deleteSupportTable(void);

#ifdef __cplusplus
}
#endif

#endif 
