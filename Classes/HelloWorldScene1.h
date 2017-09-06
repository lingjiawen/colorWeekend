#ifndef __HELLOWORLD1_SCENE_H__
#define __HELLOWORLD1_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameLayer.h"
#include "GameLayer1.h"

class HelloWorld1 : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void CloudRun(float dt);							//云彩动画
	void CloudRunCB(cocos2d::Node *sprite);			//云彩动画结束回调

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld1);

protected:


private:
	void InitGameLayer1();

	GameL1 *_gameLayer1;

};

#endif
