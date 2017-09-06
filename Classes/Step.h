#ifndef _STEP_H_
#define _STEP_H_

#include "cocos2d.h"
#include "Tag.h"



class StepAttr
{
public:
	StepAttr();
	StepAttr(float y, cocos2d::Color3B color, bool allow);
	StepAttr& operator= (const StepAttr& tmp);
	bool isAllow();

	float _bh;							//̨�׸߶�
	cocos2d::Color3B _color;			//��ɫ
	bool _allow;						//�Ƿ�������
};


class Step :public cocos2d::Sprite, public StepAttr
{
public:
	Step();
	void setAttributes(const StepAttr &tmp);
	virtual bool init();
	CREATE_FUNC(Step);
};


#endif