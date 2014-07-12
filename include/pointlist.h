/*
 * =====================================================================================
 *
 *       Filename:  rootlist.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/04/2013 08:18:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  POINTLIST_INC
#define  POINTLIST_INC
#include <stdlib.h>

struct pointElem {

  void* value;
  struct pointElem *next;
};				/* ----------  end of struct pointElem  ---------- */

typedef struct pointElem PointElem;

#ifdef __cplusplus
extern "C" {
#endif

  PointElem *createElem(void * );

  void setValue( PointElem*, void *, void (* ) (void *));

  void delElem(PointElem * , void (*delfun)(void*));

  void clear_E(PointElem *);

  struct pointList {
    PointElem *head;
    PointElem *tail;
    void (*delfun)(void *);

  };				/* ----------  end of struct pointList  ---------- */

  typedef struct pointList PointList;

  PointList* createList(void (*)(void *) );

  void delList(PointList *list);

  void delListStart(PointList* list, const int start);

  int isEmpty_L(PointList *list);

  void* front_L(PointList *list);

  void pop_front_L(PointList *list);

  void push_front_L(PointList *list, void *);

  void push_back_L(PointList *list, void *);

  void sortList(PointList *list, int ( *com)( void *, void *)); 
  void clear_L(PointList *list);

  size_t size_L(PointList *);


#ifdef __cplusplus 
}
#endif

#endif   /* ----- #ifndef POINTLIST_INC  ----- */