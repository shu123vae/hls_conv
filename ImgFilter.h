#pragma once
#include "ap_int.h"
#include "ap_fixed.h"
#include <math.h>

#include <stdio.h>
#include <string.h>

#include "hls_stream.h"

#define SIZE_INT 7
#define SIZE_GUARD 11
#define SIZE_EXT 15
#define TH_ALPHAA 5.0
#define TH_ALPHA 9.3619e+15
#define SIZE_IMG 512
#define SIZE_IMG_EXP SIZE_IMG+SIZE_EXT+SIZE_INT-2
#define DIGIT_IMG_SIZE 10

//≤‚ ‘
#define MYTEST_SIZE 7
#define MYTEST_SIZE_EDGRFILL (MYTEST_SIZE-1)/2

#define INPUT
#define OUTPUT

typedef ap_uint<14> uint14;

template <unsigned int Digit>
struct TypeMN
{
	ap_uint<Digit> M;
	ap_uint<Digit> N;
};

typedef ap_uint<14> uint14;
typedef hls::stream<uint14> TypeUint14Stream;
typedef hls::stream<float> TypeFloatStream;
typedef ap_uint<20> uint20;
typedef ap_uint<21> uint21;

typedef TypeMN<DIGIT_IMG_SIZE> TypeD09MN;
typedef hls::stream<TypeD09MN> TypeD09MNStream;

void Filter512(TypeUint14Stream &InImg, /*TypeD09MNStream &StreamD10MN,*/ TypeFloatStream &StreamCluSup);
/**/
