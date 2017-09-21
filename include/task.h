
/**
 * @file   task.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:54:58 2015
 *
 * @brief
 *
 *
 */

#include "bitset.h"
typedef bool Membership_result;

typedef struct {
  bool is_equal;
  BitSet *counterexample;
} equivalence_result_t;

class Task {
  int mode;

 public:
  virtual Membership_result membership(BitSet *bs) { return false; }

  virtual equivalence_result_t *equivalence(BitSet *bs) { return NULL; }
};
