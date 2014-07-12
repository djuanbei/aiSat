/*
 * =====================================================================================
 *
 *       Filename:  task.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/30/2013 09:03:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"bitset.h"
typedef bool  Membership_result;

typedef struct {
  bool is_equal;
  BitSet *counterexample;
} equivalence_result_t;


class Task{

	int mode;

	public:
		virtual Membership_result  membership(BitSet *bs){
			return false;

		}

		virtual equivalence_result_t* equivalence(BitSet *bs){
			return NULL;

		}

};


