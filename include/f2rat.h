
/**
 * @file   f2rat.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:38:34 2015
 * 
 * @brief  convert a float number to the closest rational number whos den is bounded by bound
 * 
 * 
 */


#ifndef  F2RAT_INC
#define  F2RAT_INC

void f2rat(const double  orig, const int bound, int * den, int * num );


#endif   /* ----- #ifndef F2RAT_INC  ----- */
