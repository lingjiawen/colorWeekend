#include "Colors.h"
#include "Ranfunc.h"
namespace rf = RanFunc;

Colors::Colors()
{
	
	//十一种方块颜色
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

	//全部调整为重色。
	//_ColorsKinds.push_back(Color(0, 0, 255));		//蓝色
	//_ColorsKinds.push_back(Color(0, 255, 0));		//绿色
	//_ColorsKinds.push_back(Color(255, 255,0));		//黄色
	//_ColorsKinds.push_back(Color(160, 32, 240));	//紫色
	//_ColorsKinds.push_back(Color(255, 255, 255));	//白色
	//_ColorsKinds.push_back(Color(8, 46, 84));		//靛青
	//_ColorsKinds.push_back(Color(54, 94, 15));		//绿土
	//_ColorsKinds.push_back(Color(0, 0, 0));			//黑色
	//_ColorsKinds.push_back(Color(135, 38, 87));		//草莓色
	//_ColorsKinds.push_back(Color(255 ,0, 0));		//红色
	//_ColorsKinds.push_back(Color(192, 192, 192));	//灰色

}

void Colors::randomColors()
{
	INIT_RANDOM_ENGINE;

	std::vector<Color> tmp;
	//std::default_random_engine e(time(0));//生成随机无符号数
	while (_ColorsKinds.size() > 0) {
		//std::uniform_int_distribution<int> u(0, _ColorsKinds.size() - 1); //生成0到10（包含）均匀分布的随机数
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