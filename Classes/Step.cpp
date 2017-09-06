#include "Step.h"

StepAttr::StepAttr(float y, cocos2d::Color3B color, bool allow)
{
	_bh = y;
	_color = color;
	_allow = allow;
}

StepAttr& StepAttr::operator=(const StepAttr& tmp){
	_bh = tmp._bh;
	_color = tmp._color;
	_allow = tmp._allow;
	return *this;
}

StepAttr::StepAttr()
{
	_bh = 0;
	_color = cocos2d::ccc3(0, 0, 0);
	_allow = false;
}

bool StepAttr::isAllow()
{
	return _allow;
}

Step::Step()
{
	this->setTag(BLOCK_TAG);
}

void Step::setAttributes(const StepAttr &tmp)
{
	_bh = tmp._bh;
	_color = tmp._color;
	_allow = tmp._allow;
	this->setColor(_color);
}

bool Step::init()
{
	this->initWithFile("blockTop1@2x.png", cocos2d::Rect(26,30, 200, 482)); //(x,y,width,height)
	return true;
}