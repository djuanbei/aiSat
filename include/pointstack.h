/*
 * =====================================================================================
 *
 *       Filename:  pointstack.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/05/2013 01:53:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  POINTSTACK_INC
#define  POINTSTACK_INC


#include	"config.h"

struct pointStack {
	int capacity;
	int size;
	void ** data;
	void (*delFun)(void *);
	
};				/* ----------  end of struct pointStack  ---------- */

typedef struct pointStack PointStack;

PointStack * createStack(void (*)(void *));

void reScale_S(PointStack *);

void delStack(PointStack *);
int isEmpty_S(PointStack *);

void push_S(PointStack *, void *);

void* top_S(PointStack *); 
void pop_S(PointStack *);


#endif   /* ----- #ifndef POINTSTACK_INC  ----- */
