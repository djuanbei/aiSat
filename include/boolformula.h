

/**
 * @file   boolformula.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:26:30 2015
 * 
 * @brief  reference wang bo yao's library 
 * 
 * 
 */



#ifndef  boolFORMULA_INC
#define  boolFORMULA_INC

#include	"pointvector.h"
#ifdef __cplusplus
extern "C" {
#endif
  typedef uscalar_t var;
  typedef scalar_t lit;
  extern const lit NOT_A_LITERAL;

  enum type{DISJUNCT, CONJUNCT, LITERAL};

  union data {
    lit l;
    PointVector* v;
  };

  typedef struct {
    uscalar_t ref;
    enum type t;
    union data d;
  } boolformula_t;

  lit boolformula_lit_from_var (var);
  var boolformula_var_from_lit (lit);
  lit boolformula_lit_complement (lit);
  BOOL boolformula_positive_lit (lit);

  void boolformula_neg (boolformula_t *);
  boolformula_t *boolformula_disjunction_new (uscalar_t length);
  boolformula_t *boolformula_disjunction_unit (void);
  boolformula_t *boolformula_conjunction_new (uscalar_t length);
  boolformula_t *boolformula_conjunction_unit (void);
  boolformula_t *boolformula_literal_new (lit);

  boolformula_t *boolformula_add (boolformula_t *, boolformula_t *);
  boolformula_t *boolformula_set (boolformula_t *, uscalar_t idx, boolformula_t *);

  enum type boolformula_get_type (boolformula_t *);
  uscalar_t boolformula_get_length (boolformula_t *);
  boolformula_t *boolformula_get_subformula (boolformula_t *, uscalar_t idx);//for disjunct/disjunct
  lit boolformula_get_value (boolformula_t *);//for literal

  void boolformula_free (void *);
  void boolformula_print (boolformula_t *f);
  scalar_t boolformula_num_of_var(boolformula_t* f);

  boolformula_t *boolformula_to_cnf (boolformula_t *, scalar_t);
  boolformula_t *boolformula_copy(boolformula_t *);


#ifdef __cplusplus 
}
#endif

#endif   /* ----- #ifndef boolFORMULA_INC  ----- */
