#include "monomial.h"
#include <ulib/hash_func.h>
#include <ulib/hash_open.h>
#include <ulib/math_rand_prot.h>
using namespace ulib;



void monomial::add(uint32_t set[], const int v){
  set[v/32]|=valueMap[v%32];
}
void monomial::add(uint32_t set[], const monomial & other){

  set[0]|=other.relation[0];
  set[1]|=other.relation[1];
  set[2]|=other.relation[2];
  set[3]|=other.relation[3];

}

bool monomial::contain(const uint32_t set[], const monomial & other){
  if((other.relation[0]&set[0])!=other.relation[0]) return false;
  if((other.relation[1]&set[1])!=other.relation[1]) return false;
  if((other.relation[2]&set[2])!=other.relation[2]) return false;
  if((other.relation[3]&set[3])!=other.relation[3]) return false;
  return true;

}

bool monomial::conjunction(const uint32_t set[], const monomial & other){
  if(set[0]&other.relation[0]) return true;
  if(set[1]&other.relation[1]) return true;
  if(set[2]&other.relation[2]) return true;
  if(set[3]&other.relation[3]) return true;
  return false;
}


void monomial::add(const int k){
  relation[k/32]|=valueMap[k%32];
  change=true;
}


void monomial::clear(void){
  relation[0]=0;
  relation[1]=0;
  relation[2]=0;
  relation[3]=0;
  change=false;
}
void monomial::add(const  monomial &other){
  relation[0]|=other.relation[0];
  relation[1]|=other.relation[1];
  relation[2]|=other.relation[2];
  relation[3]|=other.relation[3];
  change=true;

}

bool monomial::contain(const monomial &other) const {
  if((other.relation[0]&relation[0])!=other.relation[0]) return false;
  if((other.relation[1]&relation[1])!=other.relation[1]) return false;
  if((other.relation[2]&relation[2])!=other.relation[2]) return false;
  if((other.relation[3]&relation[3])!=other.relation[3]) return false;
  return true;
    
}

bool monomial::contain(const int k)const {
  
  return relation[k/32] & valueMap[k%32];
}

bool monomial::conjunction(const monomial &other) const{
  if(other.relation[0]&relation[0]) return true;
  if(other.relation[1]&relation[1]) return true;
  if(other.relation[2]&relation[2]) return true;
  if(other.relation[3]&relation[3]) return true;
  return false;

}

bool monomial::conjunction(const uint32_t set[]) const{
  if(set[0]&relation[0]) return true;
  if(set[1]&relation[1]) return true;
  if(set[2]&relation[2]) return true;
  if(set[3]&relation[3]) return true;
  return false;

}

void monomial::remove(const monomial &other ){
  relation[0]=relation[0]^(relation[0]&other.relation[0]);
  relation[1]=relation[1]^(relation[1]&other.relation[1]);
  relation[2]=relation[2]^(relation[2]&other.relation[2]);
  relation[3]=relation[3]^(relation[3]&other.relation[3]);

  
  change=true;
  
}

int monomial::size()const {
  int re=0;
  re=0;
  int i=0;
  for ( i = 0; i < 32; i += 1 ) {
    if(relation[0]&valueMap[i]) re++;
  }

  for ( i = 0; i < 32; i += 1 ) {
    if(relation[1]&valueMap[i]) re++;
  }

  for ( i = 0; i < 32; i += 1 ) {
    if(relation[2]&valueMap[i]) re++;
  }

  for ( i = 0; i < 32; i += 1 ) {
    if(relation[3]&valueMap[i]) re++;
  }
  return re;

}


monomial::operator size_t  () const
{ return hash_fast64((const unsigned char *)indice, dim*sizeof(indice_t), 0); }

bool monomial::operator==(const monomial &other) const
{ return memcmp(indice, other.indice, dim*sizeof(indice_t)) == 0; }

