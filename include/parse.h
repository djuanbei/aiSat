
/**
 * @file   parse.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:49:53 2015
 *
 * @brief  lex syntax  file
 *
 *
 */

#ifndef PARSE_INC
#define PARSE_INC
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
#define LEAF 0 /*  */

typedef enum _Node_Type {
  VAR_Node,
  AND_Node = '&',
  OR_Node  = '|',
  EQ_Node  = '=',
  IMP_Node,
  NOT_Node = '!'
} Node_Type;

typedef int Node;

void clear_NodeTable( void );

void reset( void );

Node addVar( void );

Node make_node( Node_Type type, const Node left, const Node right );

int bool2cnf( const Node root, FILE *outfile );

int parsefile( FILE *filein, FILE *fileout );

int variable_index( const char * );

#ifdef __cplusplus
}
#endif

#endif
