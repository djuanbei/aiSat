
/**
 * @file   support_table.c
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 15:35:45 2015
 *
 * @brief   Since every polynomial constrain is parametric polynomial
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
 *
 */

#include "support_table.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "crypt_md5.h"
#include "psdutil.h"
#include "sort.h"
#include "sparse.h"
#include "util.h"

namespace aiSat {
namespace psd {

Support *createSupport(const int deg, const int varId, const int consNum,
                       int *consId) {
  Support *re;
  re = (Support *)malloc_d(sizeof(Support));
  re->type = NORMAL;

  re->varId = -1;
  memset(re->md5sum, 0, DIGEST_SIZE);

  /*-----------------------------------------------------------------------------
   *  have some problems
   *-----------------------------------------------------------------------------*/
  re->md5sum[0] = (uint8_t)deg;
  re->md5sum[1] = (uint8_t)varId;
  re->md5sum[2] = (uint8_t)consNum;

  re->deg = deg;
  re->varId = varId;
  re->constNum = consNum;
  re->consId = consId;
  re->consCap = consNum;
  return re;
}




int Supporttable::findSupElem(const int deg, const int varId, const int consNum,
                              const int *consIds) {
  int i, j;

  /* 	if(0==deg) return 0;
   */
  /* when deg==0 the sosM is constant 1 */

  for (i = 0; i < values.size(); i += 1) {
    if (values[i]->type == NORMAL) {
      if (deg == values[i]->deg && varId == values[i]->varId &&
          consNum == values[i]->constNum) {
        j = 0;
        while (j < consNum) {
          if (consIds[j] != values[i]->consId[j]) {
            break;
          }
          j++;
        }
        if (j == consNum) {
          return findBimapByValue(loc, i);
        }
      }
    }
  }

  return -1;
}

int Supporttable::findSupByPoly(const Subpoly_t *subpoly) {
  int i = 0;
  for (i = 0; i < values.size(); i += 1) {
    if (values[i]->type == SUB_POLY) {
      if (0 == memcmp(values[i]->md5sum, subpoly->getmd5(), DIGEST_SIZE)){
        return findBimapByValue(loc, i);
      }
    }
  }
  return -1;
}

int Supporttable::findSupElemByIndice(const int varId, const indice_t *indices,
                                      const int value_size) {
  int i;
  uint8_t md5sum[DIGEST_SIZE];
  md5sumbyIndice(md5sum, varId, indices, value_size);
  for (i = 0; i < value_size; i++) {
    if (values[i]->type == INDICE &&
        0 == memcmp(values[i]->md5sum, md5sum, DIGEST_SIZE)) {
      return findBimapByValue(loc, i);
    }
  }
  return -1;
}

int Supporttable::addSOSup(const int deg, const int varId, const int consNum,
                           int *consIds) {
  int re = findSupElem(deg, varId, consNum, consIds);
  int value_size = values.size();
  if (re > -1) {
    return re;
  }

  re = values.size();
  values.push_back(createSupport(deg, varId, consNum, consIds));

  SOSsup.push_back(NULL);
  sosLength.push_back(0);
  
  Gsup.push_back(NULL);
  gLength.push_back(0);
  
  arrangeM.push_back(NULL);
  return addBimapElem(loc, re);
}

int Supporttable::addSOSsupByIndice(const int varId, indice_t *indices,
                                    const int esize) {
  qsortM(indices, getVarTable<indice_t>().getVarNum(varId), 0, esize - 1,
         compare);

  int re = findSupElemByIndice(varId, indices, esize);

  if (re > -1) {
    return re;
  }
  re = values.size();
  int value_size = re;
  values.push_back(new Support(varId, indices, value_size));

  SOSsup.push_back(NULL);
  sosLength.push_back(0);
    
  Gsup.push_back(NULL);
  gLength.push_back(0);
  arrangeM.push_back(NULL);

  return addBimapElem(loc, re);
}

Support *Supporttable::getSupElem(const int id) {
  int index = findBimapByKey(loc, id);

  if (index < 0) {
    return NULL;
  }
  return values[index];
}

indice_t *Supporttable::getSOSsup(const int id, int *length) {
  int index = findBimapByKey(loc, id);
  if (index < 0) {
    return NULL;
  }
  *length = sosLength[index];

  return SOSsup[index];
}

indice_t *Supporttable::getGsup(const int id, int *length) {
  int index = findBimapByKey(loc, id);
  if (index < 0) {
    return NULL;
  }

  *length = gLength[index];
  return Gsup[index];
}

void Supporttable::setSOSsup(const int id, const int len, indice_t *value) {
  int index = findBimapByKey(loc, id);

  ASSERT(index > -1, "There are some bugs");
  if (index < 0) return;
  sosLength[index] = len;
  SOSsup[index] = value;
}

void Supporttable::setGsup(const int id, const int len, indice_t *value) {
  int index = findBimapByKey(loc, id);
  ASSERT(index > -1, "There are some bugs");
  if (index < 0) return;
  gLength[index] = len;
  Gsup[index] = value;
}

int Supporttable::addconvexsosSup(Subpoly_t *poly) {
  int re = findSupByPoly(poly);
  if (re > -1) {
    return re;
  }

  int value_size = values.size();

  values.push_back(new Support(poly));

  SOSsup.push_back(NULL);
  sosLength.push_back(0);
  
  Gsup.push_back(NULL);
  gLength.push_back(0);
  
  arrangeM.push_back(NULL);
  re = value_size;
  value_size++;
  return addBimapElem(loc, re);
}

int Supporttable::getsosSLength(const int id) {
  int index = findBimapByKey(loc, id);
  if (index < 0) return -1;
  return sosLength[index];
}

void Supporttable::setArrangeM(const int id, ArrangeMatrix **value,
                               const int egLength) {
  int index = findBimapByKey(loc, id);
  ASSERT(index > -1, "There are some bugs");
  if (index < 0) return;
  gLength[index] = egLength;
  arrangeM[index] = value;
}

ArrangeMatrix **Supporttable::getAMIndex(const int id, int *gLength) {
  int index = findBimapByKey(loc, id);
  if (index < 0) return NULL;

  *gLength = gLength[index];
  return arrangeM[index];
}

void Supporttable::clearSupportTable(void) {
  if (!values.empty()) {
    int value_size = values.size();
    int i;
    for (i = 0; i < value_size; i += 1) {
      delete values[i];

      if (SOSsup[i] != NULL) free(SOSsup[i]);
      if (Gsup[i] != NULL) free(Gsup[i]);
      if (arrangeM[i] != NULL) {
        deleteSparseA(arrangeM[i], sosLength[i]);
      }
    }
    clearBimap(loc);
  }
  sosLength.clear();
  gLength.clear();
  arrangeM.clear();
  SOSsup.clear();
  Gsup.clear();

  values.clear();
}
Supporttable::Supporttable() {
  loc = createBimap();
}
Supporttable::~Supporttable() {
  int value_size = values.size();

  for (int i = 0; i < value_size; i += 1) {
    delete values[i];

    if (SOSsup[i] != NULL) {
      free(SOSsup[i]);
    }
    if (Gsup[i] != NULL) {
      free(Gsup[i]);
    }
    if (arrangeM[i] != NULL) {
      deleteSparseA(arrangeM[i], sosLength[i]);
    }
  }

  deleteBimap(loc);
}

Supporttable SUPPORT_TABLE;
}
}
