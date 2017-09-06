#include "RanFunc.h"

float RanFunc::GetRandomNumber(std::default_random_engine &e,int i1, int i2) {
	std::uniform_int_distribution<int> u(i1, i2); //生成i1到i2（包含）均匀分布的随机数
	return float(u(e));
}