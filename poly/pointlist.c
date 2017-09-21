/*
 * =====================================================================================
 *
 *       Filename:  pointlist.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  01/04/2013 08:48:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "pointlist.h"
#include <stdio.h>
#include <stdlib.h>
#include "selfmemutil.h"

PointElem *createElem(void *elem) {
  PointElem *re;

  re = (PointElem *)malloc_d(sizeof(PointElem));
  re->value = elem;
  re->next = NULL;
  return re;
}

void setValue(PointElem *elem, void *value, void (*delRoot)(void *)) {
  if (elem->value != NULL) delRoot(elem->value);
  elem->value = value;
}

void delElem(PointElem *elem, void (*delfun)(void *)) {
  if (elem != NULL) {
    if (elem->value != NULL) delfun(elem->value);
    free(elem);
  }
}

void clear_E(PointElem *elem) {
  if (elem != NULL) {
    free(elem);
  }
}

PointList *createList(void (*del)(void *)) {
  PointList *re;

  re = (PointList *)malloc_d(sizeof(PointList));

  re->head = NULL;
  re->tail = NULL;
  re->delfun = del;
  return re;
}

void delList(PointList *list) {
  if (list != NULL) {
    PointElem *temp = list->head;
    PointElem *temp1;
    while (NULL != temp) {
      temp1 = temp->next;
      delElem(temp, list->delfun);
      temp = temp1;
    }
    if (temp != NULL) delElem(temp, list->delfun);
    free(list);
  }
}

void delListStart(PointList *list, const int start) {
  int i;
  if (NULL != list) {
    PointElem *temp = list->head;
    i = 0;
    while (NULL != temp && i < start) {
      temp = temp->next;
      i++;
    }
    if (NULL != temp) {
      list->tail = temp;
      PointElem *temp1;
      while (NULL != temp) {
        temp1 = temp->next;
        delElem(temp, list->delfun);
        temp = temp1;
      }
      list->tail->next = NULL;
    }
  }
}

int isEmpty_L(PointList *list) { return (NULL == list->head); }

void *front_L(PointList *list) {
  if (list->head != NULL) return list->head->value;
  return NULL;
}

void pop_front_L(PointList *list) {
  if (list->head != NULL) {
    if (list->head == list->tail) {
      free(list->head);
      list->head = list->tail = NULL;
      return;
    } else {
      PointElem *temp = list->head;
      list->head = list->head->next;
      free(temp);
    }
  }
}

void push_back_L(PointList *list, void *value) {
  PointElem *elem = createElem(value);
  if (NULL == list->head) { /* empty */
    list->head = elem;
    list->tail = elem;
  } else {
    list->tail->next = elem;
    list->tail = elem;
  }
}

void push_front_L(PointList *list, void *v) {
  PointElem *elem = createElem(v);
  if (NULL == list->head) { /* empty */
    list->head = elem;
    list->tail = elem;
  } else {
    elem->next = list->head;
    list->head = elem;
  }
}


/** 
 * @brief  O(n^2)
 * 
 * @param list 
 * @param com 
 */
void sortList(PointList *list, int (*com)(void *, void *)) {
  if (list->head == list->tail) return;

  PointElem *temp;
  PointElem *temp1;
  PointElem *temp2;

  /*-----------------------------------------------------------------------------
   *  insert sort
   *-----------------------------------------------------------------------------*/
  temp = list->head;

  while (NULL != temp->next) {
    /*-----------------------------------------------------------------------------
     *  temp1 isolate
     *-----------------------------------------------------------------------------*/
    temp1 = temp->next;
    //		temp->next=temp1->next;             /* temp-> ( jump(temp1)
    // temp->next) ->temp1->next */
    //		temp=temp->next;
    //
    if ((*com)(temp1->value, list->head->value) <= 0) {
      temp->next = temp1->next;
      temp1->next = list->head;
      list->head = temp1;

    } else {
      temp2 = list->head;
      while (temp2->next != temp1 &&
             (*com)(temp1->value, temp2->next->value) > 0)
        temp2 = temp2->next;

      if (temp2->next != temp1) {
        temp->next = temp1->next;
        temp1->next = temp2->next;
        temp2->next = temp1;

      } else {
        temp = temp->next;
      }
    }
  }
  list->tail = temp;
  list->tail->next = NULL;
}
void clear_L(PointList *list) {
  if (list != NULL) {
    PointElem *temp = list->head;
    PointElem *temp1;
    while (NULL != temp) {
      temp1 = temp->next;
      clear_E(temp);
      temp = temp1;
    }
    list->head = list->tail = NULL;
  }
}

size_t size_L(PointList *list) {
  size_t re = 0;

  if (list->head == NULL) return re;
  PointElem *elem = list->head;
  while (NULL != elem) {
    re++;
    elem = elem->next;
  }
  return re;
}
