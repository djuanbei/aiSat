#ifndef MONOMIAL_INC
#define MONOMIAL_INC
#include "polytype.h"

const uint32_t valueMap[]={1u,2u,4u,8u,16u,32u,64u,128u,256u,512u,1024u,2048u,4096u,8192u,
                        16384u,32768u,1u<<16,1u<<17,1u<<18,1u<<19,1u<<20,1u<<21,1u<<22,1u<<23,1u<<24,1u<<25,
                        1u<<26,1u<<27,1u<<28,1u<<29,1u<<30,1u<<31};
struct monomial{
  const indice_t  *indice;
  uint32_t relation[4];
  bool change;
  int dim;

  /*-----------------------------------------------------------------------------
   *  at most 128 vertex
   *-----------------------------------------------------------------------------*/
  monomial(const indice_t *m, const int d):indice(m), dim(d) {
    relation[0]=0u;
    relation[1]=0u;
    relation[2]=0u;
    relation[3]=0u;
    change=false;
  }
  static void add(uint32_t set[], const int v);
  
  static void add(uint32_t set[], const monomial & other);

  static bool contain(const uint32_t set[], const monomial & other);

  static bool conjunction(const uint32_t set[], const monomial & other);
  
  
  void add(const int k);
  
  void clear(void);
  
  void add(const  monomial &other);

  bool contain(const monomial &other) const ;
  
  bool contain(const int k)const ; 
  
  bool conjunction(const monomial &other) const;
  
  bool conjunction(const uint32_t set[]) const;

  void remove(const monomial &other );
  
  int size()const ;
  
  operator size_t() const;

  bool operator==(const monomial &other) const;

};


#endif
