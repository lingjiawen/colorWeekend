#ifndef _RANFUC_H_
#define _RANFUC_H_

#include <random>
#include <time.h>
#include "Tag.h"

namespace RanFunc 
{ 
	float GetRandomNumber(std::default_random_engine &e,int i1, int i2);
	#define INIT_RANDOM_ENGINE std::default_random_engine e(time(0))	//随机数引擎类，初始化e()
	#define rn(a,b) GetRandomNumber(e,a,b)
}

#endif