/*
 * =====================================================================================
 *
 *       Filename:  parse.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/25/2013 09:15:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  PARSE_INC
#define  PARSE_INC
#include	<stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
#define	LEAF 0			/*  */

typedef enum _Node_Type {
  VAR_Node,
  AND_Node = '&',
  OR_Node = '|',
  EQ_Node = '=',
  IMP_Node,
  NOT_Node = '!'
} Node_Type;

typedef int Node;


void clear_NodeTable(void);

void reset(void);

Node addVar(void);

Node make_node(Node_Type type, const Node left, const Node right);

int bool2cnf( const Node root, FILE *outfile);

int parsefile(FILE * filein, FILE* fileout);

int variable_index(const char *);


#ifdef __cplusplus 
}
#endif

#endif   /* ----- #ifndef PARSE_INC  ----- */
