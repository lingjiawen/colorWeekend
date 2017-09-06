#ifndef _BANGS_H_
#define _BANGS_H_

#include "cocos2d.h"
USING_NS_CC;

class Bangs:public Sprite
{
public:
	void fireworks(const Rect &rect);
	CREATE_FUNC(Bangs);
protected:
	virtual bool init();

};

#endif