/**
 * @file   pointlist.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:20:48 2015
 *
 * @brief  point list containt
 *
 *
 */

#ifndef POINTLIST_INC
#define POINTLIST_INC
#include <stdlib.h>

struct pointElem {
  void *            value;
  struct pointElem *next;
};

typedef struct pointElem PointElem;

#ifdef __cplusplus
extern "C" {
#endif

PointElem *createElem( void * );

void setValue( PointElem *, void *, void ( * )( void * ) );

void delElem( PointElem *, void ( *delfun )( void * ) );

void clear_E( PointElem * );

struct pointList {
  PointElem *head;
  PointElem *tail;
  void ( *delfun )( void * );
};

typedef struct pointList PointList;

PointList *createList( void ( * )( void * ) );

void delList( PointList *list );

void delListStart( PointList *list, const int start );

int isEmpty_L( PointList *list );

void *front_L( PointList *list );

void pop_front_L( PointList *list );

void push_front_L( PointList *list, void * );

void push_back_L( PointList *list, void * );

void sortList( PointList *list, int ( *com )( void *, void * ) );
void clear_L( PointList *list );

size_t size_L( PointList * );

#ifdef __cplusplus
}
#endif

#endif
