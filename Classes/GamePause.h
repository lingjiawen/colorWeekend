#ifndef _GAMEPAUSE_H_
#define _GAMEPAUSE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;
class GamePause :public Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static Scene* createScene(RenderTexture* sqr);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	void PlayBtn(Ref* pSender,TouchEventType type);
	void HomeBtn(Ref* pSender, TouchEventType type);
	void RestartBtn(Ref* pSender, TouchEventType type);
	void PlayTouchEndedCB(float dt);
	void HomeTouchEndedCB(float dt);
	void RestartTouchEndedCB(float dt);

	CREATE_FUNC(GamePause);
private:
	ActionObject *_uiIn;
	ActionObject *_uiOut;
};




#endif