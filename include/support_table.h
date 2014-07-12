/*
 * =====================================================================================
 *
 *       Filename:  coefMtable.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/07/2012 10:03:58 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  SUPPORT_TABLE_INC
#define  SUPPORT_TABLE_INC

#ifdef __cplusplus
extern "C" {
#endif

#include	"sparse.h"
#include	"config.h"
#include	"polytype.h"
#include	"bimap.h"
  struct supporttable {

    int capacity;
    int size;
    Bimap* loc;
    int *sosLength;
    int *gLength;
    ArrangeMatrix ***arrangeM;
    /*-----------------------------------------------------------------------------
     *  monomial of p
     *-----------------------------------------------------------------------------*/
    indice_t ** sosSup;
    /*-----------------------------------------------------------------------------
     * p=sum_i g_i^2
     * monomial of g
     *-----------------------------------------------------------------------------*/
    indice_t **Gsup;

    Support ** values;
  };				/* ----------  end of struct coefMtable  ---------- */

  typedef struct supporttable Supporttable;
  /*
   * return >=0 if find
   * otherwise <0
   */

  int findSupByPoly(const SubPoly * poly);

  indice_t * getsosSup(const int id, int *len );

  int getsosSLength(const int id);

  void setsosSup(const int id, const int len, indice_t * value);

  void setGsup(const int id, const int len, indice_t * value);

  void setArrangeM(const int id, ArrangeMatrix **value, const int gLength); 

  indice_t * getGsup(const int id, int *len);

  ArrangeMatrix **getAMIndex(const int id , int * gLength);

  int addsosSup (const int deg , const int varId, const int consNum,  int * consId);

  int addsosSupByIndice(const int varId,  indice_t * indices, const int size);

  int addconvexsosSup(SubPoly* poly);

  /* 	int findSupElem( const int deg , const int varId,
   * 				const int consNum, const int * consId);
   */

  Support * getSupElem(const int id);

  void clearSupportTable(void);

  void deleteSupportTable(void);


#ifdef __cplusplus 
}
#endif

#endif   /* ----- #ifndef SUPPORT_TABLE_INC  ----- */
