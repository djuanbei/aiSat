
/**
 * @file   monoconst_table.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:42:02 2015
 * 
 * @brief  
 * 
 * 
 */

#ifndef  MONOCONST_TABLE_INC
#define  MONOCONST_TABLE_INC

#ifdef __cplusplus
extern "C" {
#endif

#include	"config.h"
#include	"polytype.h"

  struct monomialConstTable {
    int capacity;
    int last;
    MonomialConstraint ** values;
  };				/* ----------  end of struct consTable  ---------- */

  typedef struct monomialConstTable MonomialConstTable;


  void initconsTable(void);

  int addconsElem( const int deg, const int varId,const int* linCoef, int minValue, const int consFunId   );

  int findconsElem(  const int deg, const int varId,const int* linCoef, int minValue,const int consFunId  );

  MonomialConstraint* getconsElem(const int id);

  void deleteconsTable(void);
  

#ifdef __cplusplus 
}
#endif
#endif   /* ----- #ifndef MONOCONST_TABLE_INC  ----- */
