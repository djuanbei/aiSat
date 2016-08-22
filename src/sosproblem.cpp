#include "sosproblem.h"
#include "findsos.h"
#include "sort.h"
#include "search.h"
#include "monomial.h"
#include "convexhull.h"
#include <map>
#include <iostream>

using std::cout;
using std::endl;
using std::map;


static int delRepeat(indice_t * indices, const int length, const int varNum ){
  ASSERT(length>0,"length must be a positive number ");
  if(1==length) return length;
  int i,k;
  int j=1;
  bool repeat=true;
  qsortM(indices, varNum ,0 , length-1, compare);  
    
  for(i=1; i<length; i++){
    repeat=true;
    for(k=0; k<varNum; k++){
      if(indices[i*varNum+k]!=indices[(i-1)*varNum+k]){
        repeat=false;
        break;
      }
    }
    if(!repeat){
      
      for(k=0;k< varNum; k++){
        indices[j*varNum+ k]=indices[i*varNum+k];
      }
      j++;
    }
   
  }
  return j;
  
}

static int reduceConvexHulll(const int DIM,  indice_t * candidateGsup, int &candidateLength, const indice_t * indices, const int size, indice_t **SOSM  ){
  
  int i, j, k, valueIndex, v, m, sosLength;
  size_t node_size=DIM*sizeof(indice_t);

  if( 1==candidateLength ){
    sosLength=1;
    *SOSM=(indice_t*)malloc_d(sosLength*node_size);
    for( i=0; i< DIM; i++ )
      (  *SOSM)[ i ]=2*candidateGsup[ i ];
    return sosLength;
    
  }



  indice_t * key=new indice_t[DIM];
  indice_t * key1=new indice_t[DIM];

  map<monomial, monomialvalue> monMap;
  
  indice_t *indiceValues=new indice_t[candidateLength*candidateLength*DIM];


  bool run =true;
  bool del=false;

  while (run) {
    
    run=false;
    
    for(map<monomial,monomialvalue>::iterator it=monMap.begin();it!=monMap.end();++it){
      it->second.clear();
      it->second.value=0;

    }

    valueIndex=0;

    for(  i=0; i<candidateLength ;i++ ){
      for( j=i; j<candidateLength; j++ ){
        for( k=0; k< DIM; k++){
          key[k]=candidateGsup[i*DIM+k]+candidateGsup[j*DIM+k];
        }
        if(monMap.find(monomial(key, DIM))!=monMap.end()){
          monMap[monomial(key, DIM)].value++;
          
        }else{
          memcpy(indiceValues+valueIndex*DIM, key, node_size);
          monomialvalue dummy;
          dummy.value=1;
          monMap[monomial(indiceValues+valueIndex*DIM, DIM)]=dummy;
          valueIndex++;
        }
      }
    }

    k=0;
    for( j=0; j< candidateLength;  j++){
      for(i=0; i<DIM;  i++){
        key[i]=2*candidateGsup[j*DIM+i];
      }
      v=monMap[monomial(key,DIM)].value;
      if(v==1){
        for(i=0; i<candidateLength; i++){
          for( m=0; m<DIM; m++){
            key1[m]=key[m]/2+candidateGsup[i*DIM+m];
          }
          monMap.find(monomial(key1,DIM))->second.add(k);
        }

        k++;
        monMap.find(monomial(key,DIM))->second.change=false;
      }
    }
    ASSERT(k<=VERTEX_BOUND,"");

    
    for( j=0; j< candidateLength;  j++){
      for(i=0; i<DIM;  i++){
        key[i]=2*candidateGsup[j*DIM+i];
      }

      if(monMap.find(monomial(key,DIM))->second.size()==1 && findIndex(key,indices, size, DIM )<0){
          
        del=true;
        for(i=0; i<candidateLength; i++){
          for( m=0; m<DIM; m++){
            key1[m]=key[m]/2+candidateGsup[i*DIM+m];
          }
          if(monMap.find(monomial(key1,DIM))->second.size()>1 &&
             monMap.find(monomial(key,DIM))->second.conjunction(monMap.find(monomial(key1,DIM))->second)){
              
            del=false;
            break;
          }
        }
          
        if(del){
          memmove(candidateGsup+j*node_size, candidateGsup+(j+1)*node_size, (candidateLength-j-1)*node_size );
          candidateLength--;
            
          run=true;
          break;
        }
      }

    }
  }
  
  sosLength=0;
  for(map<monomial,monomialvalue>::const_iterator it=monMap.begin();it!=monMap.end();++it){
    if(it->second.size()>0) sosLength++;
  }
  *SOSM=(indice_t*)malloc_d(sosLength*node_size);

  i=0;
  for(map<monomial,monomialvalue>::const_iterator it=monMap.begin(); it!=monMap.end(); ++it){
    if(it->second.size()>0) {
      memcpy((*SOSM)+i*DIM,  it->first.indice, node_size);
      i++;
    }
  }

  ASSERT(i==sosLength,"");

  delete[] indiceValues;
  return sosLength;
   
}


int SOSP::addSOSsup( vector<Monomial*> * indices_vec )
{
    
  int varNum=getAllVarNum(  );// getvarNum(1);//all num of vaiable
  int *varOccur=new int[varNum];
  int i,j;

  int length =indices_vec->size();
  
  for(i=0;i<varNum;i++){
    varOccur[i]=0;
  }
  vector<Monomial*>::iterator it=indices_vec->begin();
  for(; it!=indices_vec->end(); it++){
    for(i=0;i<varNum;i++){
      if((*it)->indices[i]>0)
        varOccur[i]=1;
    }
  }
  indice_t *vars=new indice_t[varNum];
    
  int exactVarNum=0;
  
  j=0;
  for(i=0;i<varNum;i++){

    if(varOccur[i]>0) {
      exactVarNum++;
      vars[j++]=i;
    }
  }

  if(0==exactVarNum ){
    exactVarNum=getAllVarNum( );
    for( i=0;i< exactVarNum; i++  )
      vars[i]=i;
  }
  int varId=addvarElem(vars, exactVarNum);

  
  indice_t *indices= new indice_t[exactVarNum*length];
  
    
  it=indices_vec->begin();
  j=0;
  for(;it!=indices_vec->end();it++){

    for(i=0; i<exactVarNum; i++){

      indices[j*exactVarNum+i]=(*it)->indices[vars[i]];
    }
    j++;
  }
    
  int supportId=addSOSsupByIndice(varId, indices, length);
  
  
  indice_t *tempG=new indice_t[exactVarNum*length ];

  for (i = 0; i < length; i++) {
    for (j=0; j<exactVarNum; j++) {
      tempG[i*exactVarNum+j]=indices[i*exactVarNum+j]/2;
    }
  }
  
  int origLength= delRepeat(tempG, length, exactVarNum );


  int gLength=0;
  
  indice_t *candidateGsup =overConvexHull(tempG, origLength, exactVarNum,  &gLength);

  indice_t *SOSM=NULL;
  indice_t* GSUP=NULL;
  int sosLength= reduceConvexHulll(exactVarNum, candidateGsup, gLength, indices, length, &SOSM );


  GSUP=(indice_t*)realloc_d(candidateGsup, gLength*exactVarNum*sizeof(indice_t));

  qsortM(GSUP,exactVarNum , 0, gLength-1, compare);
  
  qsortM(SOSM, exactVarNum, 0,sosLength-1, compare);
  
  setGsup(supportId, gLength, GSUP);
  setSOSsup(supportId,sosLength, SOSM  );
    
  
  int AMlength=0;
  //  ArrangeMatrix **AM=  
  createArrangeM(supportId,  SOSM ,&AMlength ,sosLength);
  //  setArrangeM(supportId, AM, AMlength );

  delete[] indices;
  delete[] tempG;
  delete[] vars;
  delete[] varOccur;
  return supportId;
}


int SOSP::addSOSsup(vector<int> *varVec, vector<int> *degVec  ){
      
  std::sort(varVec->begin(),varVec->end() );
  std::sort(degVec->begin(),degVec->end() );
    

  int varNum=varVec->size();
  indice_t *vars=new indice_t[varNum];
  vector<int>::iterator it;
  int i,j;
  i=0;

  for(it=varVec->begin(); it!=varVec->end(); it++){
    vars[i++]=*it;
  }
  int varId=addvarElem(vars, varNum);
  int length=0;

    
  for(it=degVec->begin(); it!=degVec->end(); it++){
    if(0==*it)
      length++;
    else
      length+=nchoosek(varNum+(*it)-1, varNum-1);
  }
  
  indice_t *indices=new indice_t[length*varNum ];
    
  length=0;
  int tempLen=0;
  
  for(it=degVec->begin(); it!=degVec->end(); it++){
    if(0==*it){
      for(i =0 ; i <varNum; i++)
        indices[length*varNum+ i] =0;
      length++;
      
    }else{
      
      tempLen=nchoosek(varNum+(*it)-1, varNum-1);
      getAllMonHomoTd(varNum, *it, tempLen, indices+length*varNum );
      length+=tempLen;
    }
  }
  
  int supportId=addSOSsupByIndice(varId, indices, length);
  

  indice_t *tempG=new indice_t[length*varNum ];

  for(i=0; i<length; i++){
    for(j=0; j<varNum; j++){
      tempG[i*varNum+j]=(indices[i*varNum+j]+1)/2;
    }
  }

  j=delRepeat(tempG, length, varNum );


  int gLength=0;
  
  indice_t *candidateGsup =overConvexHull(tempG, j , varNum,  &gLength);
  indice_t *SOSM=NULL;
  indice_t* GSUP=NULL;

  int sosLength= reduceConvexHulll(varNum, candidateGsup, gLength, indices,length, &SOSM );

  if( 0!=gLength*varNum )
    GSUP=(indice_t*)realloc_d(candidateGsup, gLength*varNum*sizeof(indice_t));
  else
    GSUP=candidateGsup;
  qsortM(GSUP,varNum , 0, gLength-1, compare);
  
  qsortM(SOSM, varNum, 0,sosLength-1, compare);

  
  setGsup(supportId,gLength, GSUP );
  setSOSsup(supportId,sosLength, SOSM  );
  
  int AMlength=0;
  //  ArrangeMatrix **AM=x
  createArrangeM( supportId,  SOSM,  &AMlength, sosLength);
  //  setArrangeM(supportId, AM, AMlength );
    
  delete[] indices;
  delete [] tempG;
  delete []vars;
  return supportId;
    
}

void SOSP::title(){
  sos_num++;
  std::cout << "====================================" << std::endl;
  std::cout << "             aiSat                  " << std::endl;
  std::cout << "====================================" << std::endl;
  std::cout <<"$" <<sos_num <<": "<< std::endl;
}

void SOSP::solve(){
  title();
  Poly** resP=(Poly**)calloc_d(problem->size, sizeof(Poly*));
  int re= sdp_solver(problem,resP, "pro.txt","result.txt" );
  if(re==0){
    map<int, string>::iterator it=left_printMap.begin();
    while (it!=left_printMap.end()) {
      std::cout<<it->second<<" := ";
      printPoly(resP[it->first]);
      printf("\n");
      it++;
    }
    int i;
    for(i=0;i<problem->size;i++){
      deletePoly(resP[i]);
    }
  }else{
    printf("can not find a feasiable solution\n");
  }
  free(resP);
  clearSOSProblem(problem);
  left_printMap.clear();
  
}

void SOSP::findSOS( Poly *poly ){
  title();

  if(easychecksos( poly )){
    cout	<< "SOS" << endl;
  }else
    cout	<< "NO" << endl;
  left_printMap.clear();
}

void SOSP::interpolant(SOSProblem *sys, const int sep){
  title();
  inter_sdp(sys, sep, "pro.txt","result.txt" );
  left_printMap.clear();
}
void SOSP::clear(){
  clearSOSProblem(problem);
  map<string,Poly*>::iterator it=polyMap.begin();
  for(;it!=polyMap.end();it++){
    deletePoly(it->second);
  }
  map<string, PolyConstraint*>::iterator iit=polyConsMap.begin();
  for(;iit!=polyConsMap.end();iit++){
    deletePolyConstraint(iit->second);
  }
  varMap.clear();
  monoMap.clear();
  polyConsMap.clear();
  left_printMap.clear();
  right_printMap.clear();
  
}

/*
  return true if str is a new key
*/

bool SOSP::addVarElem(const string & str, vector<int> * value ){
  if(varMap.find(str)==varMap.end()){
    varMap[str]=value;
    return true;
  }else{
    delete varMap[str];
    varMap[str]=value;
    return false;
  }
}
/*
  return true if str is a new key
*/

bool SOSP::addMonoElem(const string& str, const int value){
  if(monoMap.find(str)==monoMap.end()){
    monoMap[str]=value;
    return false;
  }
  monoMap[str]=value;
  return false;
}
/*
  return true if str is a new key
*/

bool SOSP::addPolyElem(const string & str, Poly* poly){
  if(polyMap.find(str)==polyMap.end()){
    polyMap[str]=poly;
    return true;
  }else{
    deletePoly(polyMap[str]);
    polyMap[str]=poly;
    return false;
  }
}
/*
  return true if str is a new key
*/

bool SOSP::addPolyConsElem(const string &str, PolyConstraint *polyCons){
  if(polyConsMap.find(str)==polyConsMap.end()){
    polyConsMap[str]=polyCons;
    return true;
    
  }else{
    deletePolyConstraint(polyConsMap[str]);
    polyConsMap[str]=polyCons;
    return false;
  }
}
