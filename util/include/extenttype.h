/**
 * @file   extenttype.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Sep 23 10:35:57 2017
 *
 * @brief  add some user define type
 *
 *
 */

#ifndef __EXTENR_TYPE_H
#define __EXTENR_TYPE_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef unsigned char indice_t;

typedef uint8_t var_t;

typedef float coef_t;

enum BOOL { FALSE = 0, TRUE = 1 };

typedef enum BOOL BOOL;

typedef long scalar_t;
typedef unsigned long uscalar_t;
typedef void *pointer_t;
#ifdef __cplusplus
}
#endif
#endif
