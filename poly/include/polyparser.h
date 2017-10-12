#ifndef __POLY_PARSER_H
#define __POLY_PARSER_H

#include "extenttype.h"

#include "poly.hpp"
#include "vartable.hpp"

namespace aiSat {

namespace poly {

Poly<coef_t, indice_t> parse(const string &str);
}
}

#endif
