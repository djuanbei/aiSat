
/**
 * @file   config.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:22:14 2015
 *
 * @brief
 *
 *
 */

#ifndef CONFIG_INC
#define CONFIG_INC

#include <stdio.h>
/**
 * the declare of dynamic interface in windows is a little complex
 *
 */

#ifdef _WIN32
#ifdef CLOGLIB_EXPORT
#define CLOGLIB __declspec(dllexport)
#elif defined(CLOGLIB_IMPORT)
#define CLOGLIB __declspec(dllimport)
#else
#define CLOGLIB
#endif
#else
#define CLOGLIB
#endif

enum {
  DEFAULT_SOS_PRO_SIZE = 5,
  MAX_VAR_NAME_LENGTH = 15, /*  */
  VERTEX_BOUND = 128,
  EIGN_CEF_BOUND = 3,
  MAX_VAR_NUM = 256,
  RANDTIME = 40,
  COEFDOMAIN_INCA = 3,
  COEFDOMAIN_INCB = 5,
  DUN_DOMAIN_BOUND = 8,
  DIGEST_SIZE = 16,
  DEFAULT_CAP = 10,
  BITSET_DEAFULT_CAP = 64,
  DEFAULT_VARTABLE_SIZE = 64,
  DEFAULT_VARTABLE_MAP_CAP = 32,
  DEFAULT_VARTABLE_NUM = 256,
  DEFAULT_COEFMTABLE_SIZE = 10,
  DEFAULT_CONS_SIZE = 4,
  DEFAULT_LIST_CAP = 8,
  RECANLE_LIST = 2,
  DEFAULT_POLY_CAP = 8,
  OUTPUT_TIME = 1000,
  DEFAULT_POLY_PER_VAR = 8, /* per variables in polynomail   */
  SIGMEM = 101,             /*  */
  DEFAULT_COEFP_CAP = 1,
  DEFAULT_COEFTABLE_SIZE = 6,
  DEFAULT_NUM = 8

}; /* ----------  end of enum typ  ---------- */

#define MIN_COEF 0.0001 /*  */

#define MAX_DEGREE (DEFAULT_COEFMTABLE_SIZE - 2) /*  */

#define OUTPUT_MINVALUE (10 / OUTPUT_TIME) /*  */

#define ENLARGE_RAT 1.2 /*  */

#endif /* ----- #ifndef CONFIG_INC  ----- */
