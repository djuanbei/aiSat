/*
 * =====================================================================================
 *
 *       Filename:  sparse.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年05月11日 22时24分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "sparse.h"

#include <cstdlib>
#include <sstream>

#include "polytype.h"
#include "selfmemutil.h"

#include <assert.h>

namespace aiSat {
namespace psd {

ArrangeMatrix::ArrangeMatrix(const int rLength) {
  rowLength = rLength;
  multCF = 1;
}

void ArrangeMatrix::printS() {

  for (size_t i = 0; i < data.size(); i += 1) {
    printf("%d %d %f\n", data[i].row, data[i].col, multCF);
  }
  printf("\n");

  return;
}

void ArrangeMatrix::addSparseElem(int row, int col) {
  SparseRowCol dummy;
  dummy.row = row; /* start from 0 */
  dummy.col = col;
  data.push_back(dummy);
}

/**
 * @brief  s is the sparse matirx row is row and col is col
 *  s's contain sort by row first
 *
 * @param s
 * @param row
 * @param col
 *
 * @return
 */
coef_t ArrangeMatrix::getValue(int row, int col) {
  int start = 0;
  int end = data.size();
  int mid;

  if (row < col) {  /* unit form */
    int temp = row; /* row>=col */
    row = col;
    col = temp;
  }

  while (start < end) {
    mid = (start + end) / 2;

    if (data[mid].row > row) {
      end = mid;
    } else if (data[mid].row < row) {
      start = mid + 1;
    } else if (data[mid].col > col) {
      end = mid;
    } else if (data[mid].col < col) {
      start = mid + 1;
    } else {
      return multCF;
    }
  }

  if (data[start].row == row && data[start].col == col) {
    return multCF;
  }

  return 0;
}

void ArrangeMatrix::mult(const double a) { multCF = a; }

string ArrangeMatrix::toString() const {

  stringstream str;
  for (size_t i = 0; i < data.size(); i += 1) {
    str << "(" << data[i].row << " , " << data[i].col << ")" << endl;
  }
  return str.str();
}
}
}
