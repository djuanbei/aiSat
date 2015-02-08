
#ifndef POLYISSOS_INC
#define POLYISSOS_INC
#include "poly.h"
#include <stdio.h>
#include <stdlib.h>
#include "poly.h"
#include "polytype.h"
#include "sparse.h"
#include "support_table.h"
#include "vartable.h"
#include "config.h"
#include "declarations.h"
#include "pointlist.h"

#ifdef __cplusplus
extern "C" {
#endif
	//	BOOL polyIsSOS(const Poly * poly);
	void sosrepresent(PointList * sosList, double * X, const int  blockSize , const int sosMid, const float minv );

#ifdef __cplusplus
}
#endif
#endif  /* ifndef POLYISSOS_INC */


