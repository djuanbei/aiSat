
#include "declarations.h"
#include <stdio.h>
#include <stdlib.h>

struct Entry {
  int    indexi;
  int    indexj;
  int    indexk;
  int    indexl;
  double entry;
};

int mycompare( const void *p1, const void *p2 ) {
  if ( ( (struct Entry *) p1 )->indexi < ( (struct Entry *) p2 )->indexi ) {
    return ( -1 );
  };
  if ( ( (struct Entry *) p1 )->indexi == ( (struct Entry *) p2 )->indexi ) {
    if ( ( (struct Entry *) p1 )->indexj < ( (struct Entry *) p2 )->indexj ) {
      return ( -1 );
    };
    if ( ( (struct Entry *) p1 )->indexj == ( (struct Entry *) p2 )->indexj ) {
      return ( 0 );
    };
    if ( ( (struct Entry *) p1 )->indexj > ( (struct Entry *) p2 )->indexj ) {
      return ( 1 );
    };
  };

  /*
   * If we get here, then p1->indexi > p2->indexi
   */

  return ( 1 );
}

void                     sort_entries( k, C, constraints ) int k;
struct blockmatrix       C;
struct constraintmatrix *constraints;
{
  int                 i, j;
  struct sparseblock *ptr;
  int                 maxentries;
  struct Entry *      entries;

  /*
   * First, find out who has the most entries.
   */

  maxentries = 0;
  for ( i = 1; i <= k; i++ ) {
    ptr = constraints[ i ].blocks;
    while ( ptr != NULL ) {
      if ( ptr->numentries > maxentries ) maxentries = ptr->numentries;
      ptr = ptr->next;
    };
  };

  /*
   * Allocate space for entries.
   */

  entries = (struct Entry *) malloc_d( maxentries * sizeof( struct Entry ) );

  for ( i = 1; i <= k; i++ ) {
    ptr = constraints[ i ].blocks;
    while ( ptr != NULL ) {
      /*
       * Copy in
       */

      for ( j = 1; j <= ptr->numentries; j++ ) {
        entries[ j - 1 ].indexi = ptr->iindices[ j ];
        entries[ j - 1 ].indexj = ptr->jindices[ j ];
        entries[ j - 1 ].entry  = ptr->entries[ j ];
      };

      /*
       * Sort
       */

      qsort( entries, (size_t) ptr->numentries, sizeof( struct Entry ),
             mycompare );

      /*
       * Copy out.
       */

      for ( j = 1; j <= ptr->numentries; j++ ) {
        ptr->iindices[ j ] = entries[ j - 1 ].indexi;
        ptr->jindices[ j ] = entries[ j - 1 ].indexj;
        ptr->entries[ j ]  = entries[ j - 1 ].entry;
      };

      ptr = ptr->next;
    };
  }; /* end i */

  free( entries );
}
