#include "Colors.h"
#include "Ranfunc.h"
namespace rf = RanFunc;

Colors::Colors()
{
	
	//ʮһ�ַ�����ɫ
	_ColorsKinds.push_back(Color(168, 176, 180));
	_ColorsKinds.push_back(Color(0, 203, 100));
	_ColorsKinds.push_back(Color(254, 158, 150));
	_ColorsKinds.push_back(Color(98, 99, 129));
	_ColorsKinds.push_back(Color(240, 50, 86));
	_ColorsKinds.push_back(Color(145, 55, 155));
	_ColorsKinds.push_back(Color(254, 100, 50));
	_ColorsKinds.push_back(Color(254, 254, 205));
	_ColorsKinds.push_back(Color(50, 205, 205));
	_ColorsKinds.push_back(Color(254, 205, 50));
	_ColorsKinds.push_back(Color(190, 115, 50));

	//ȫ������Ϊ��ɫ��
	//_ColorsKinds.push_back(Color(0, 0, 255));		//��ɫ
	//_ColorsKinds.push_back(Color(0, 255, 0));		//��ɫ
	//_ColorsKinds.push_back(Color(255, 255,0));		//��ɫ
	//_ColorsKinds.push_back(Color(160, 32, 240));	//��ɫ
	//_ColorsKinds.push_back(Color(255, 255, 255));	//��ɫ
	//_ColorsKinds.push_back(Color(8, 46, 84));		//����
	//_ColorsKinds.push_back(Color(54, 94, 15));		//����
	//_ColorsKinds.push_back(Color(0, 0, 0));			//��ɫ
	//_ColorsKinds.push_back(Color(135, 38, 87));		//��ݮɫ
	//_ColorsKinds.push_back(Color(255 ,0, 0));		//��ɫ
	//_ColorsKinds.push_back(Color(192, 192, 192));	//��ɫ

}

void Colors::randomColors()
{
	INIT_RANDOM_ENGINE;

	std::vector<Color> tmp;
	//std::default_random_engine e(time(0));//��������޷�����
	while (_ColorsKinds.size() > 0) {
		//std::uniform_int_distribution<int> u(0, _ColorsKinds.size() - 1); //����0��10�����������ȷֲ��������
		//auto iter = _ColorsKinds.begin() + (int)u(e);
		auto iter = _ColorsKinds.begin() + rf::rn(0,_ColorsKinds.size()-1);
		tmp.push_back(*iter);
		_ColorsKinds.erase(iter);
	}
	_ColorsKinds = tmp;
}

unsigned int Colors::cnt()	
{
	return _ColorsKinds.size();
}

const cocos2d::Color3B Colors::operator[](int index) {
	return _ColorsKinds[index].getCCColor();
}

Color::Color()
{
	_r = 0;
	_b = 0;
	_g = 0;
}

Color::Color(GLubyte r, GLubyte g, GLubyte b)
{
	_r = r;
	_g = g;
	_b = b;
}

Color& Color::operator = (const Color& other) 
{
	_r = other._r;
	_g = other._g;
	_b = other._b;
	return *this;
}

const cocos2d::Color3B Color::getCCColor()
{
	return cocos2d::ccc3(_r, _g, _b);
}