#include "Bangs.h"
#include "RanFunc.h"
#include "Colors.h"

USING_NS_CC;
NS_RF;

bool Bangs::init()
{
	INIT_RANDOM_ENGINE;
	Colors color;

	this->initWithFile("particle.png");
	this->setPosition(-666666,0);
	for (int i=0;i<50;i++)
	{
		auto sprite = Sprite::create("particle.png");
		sprite->setColor(color[rf::rn(0, 9)]);
		this->addChild(sprite,1,i);
		sprite->setPosition(0,0);
		sprite->retain();
	}

	return true;
}

void Bangs::fireworks(const cocos2d::Rect &r)
{
	auto wOrg =convertToWorldSpace(r.origin);
	auto opos = this->convertToNodeSpace(wOrg);
	INIT_RANDOM_ENGINE;
	for(int i=0;i<50;i++)
	{
		auto mp = Point(opos.x + (rf::rn(0, r.size.width*2/3))*(rf::rn(0,1)?1:-1),
			opos.y + rf::rn(0, r.size.height*3/4));
		//礼花出现
		auto mAction = MoveTo::create(0.6, mp);
		auto mESO = EaseSineOut::create(mAction);

		auto moveBack=MoveBy::create(5,Point(0,-200));			//礼花下落
		auto Seq = Sequence::create(mESO, moveBack, NULL);

		auto rot = RotateBy::create(rf::rn(2, 6), rf::rn(0, 1) ? -360 : 360);	//礼花旋转
		auto rotForever = RepeatForever::create(rot);
		auto sprite = this->getChildByTag(i);			//礼花Tag 0-50
		sprite->runAction(rotForever);
		sprite->runAction(Seq);
	}
}

