#include "RanFunc.h"

float RanFunc::GetRandomNumber(std::default_random_engine &e,int i1, int i2) {
	std::uniform_int_distribution<int> u(i1, i2); //����i1��i2�����������ȷֲ��������
	return float(u(e));
}