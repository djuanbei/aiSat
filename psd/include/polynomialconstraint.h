/**
 * @file   polynomialconstraint.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Tue May  7 14:51:36 2019
 *
 * @brief   polynomial like poly >0, polynomial <0
 *
 *
 */
#ifndef _POLYNOMIAL_CONSTRAINT_H
#define _POLYNOMIAL_CONSTRAINT_H

enum PolyConstraintType { EQ, GE, GT, NE };

struct PolyConstraint {
  PolyConstraintType type;
  int                supportId;
  PolyConstraint() {}
  PolyConstraint( PolyConstraint *other ) {
    type      = other->type;
    supportId = other->supportId;
  }
};

#endif
