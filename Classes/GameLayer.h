#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "StepsLayer.h"
#include "player.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

class GameL:public cocos2d::Layer
{
public:
	CREATE_FUNC(GameL);
	virtual bool init();
	GAME_STATE getGameState();
	void setGameState(GAME_STATE gs);

	void AddCDot(const Color3B &color);										//小圆球
	void AddHelpDot(const Color3B &color);
	bool soundIsOn() { return _soundBtn; }									//声音开关
	virtual void setPosition(const Point &pos);
protected:
	void update(float delta);
	void doItBtnFunc(Ref* pSender, TouchEventType type);			//DOIT按钮
	void helpBtnFunc(Ref* pSender, TouchEventType type);			//帮助按钮
	void soundBtnFunc(Ref* pSender, TouchEventType type);			//声音按钮
	void scoreBtnFunc(Ref* pSender, TouchEventType type);			//分数按钮
	void doPause(Ref* pSender, TouchEventType type);
	void uiOutActionDoneCallBack1(float dt);
	void uiOutActionDoneCallBack2(float dt);
	void HelpUIEndedCB(float dt);

	virtual bool onTouchBegan( Touch *touch, Event *unused_event );
	virtual void onTouchEnded( Touch *touch, Event *unused_event );
	virtual bool onContactBegin(PhysicsContact& contact);
	virtual void onContactSeperate(PhysicsContact& contact);
	static Scene* scene(RenderTexture* sqr);


	void gameOverCB(float dt);			//游戏结束
	void playerInit(float dt);			//主角初始化
private:
	StepsLayer *_StepLayer;			//台阶Layer
	Player *_player;				//主角
	int _cDotCnt;				//可踩颜色数目
	bool _soundBtn;					//声音开关
	int _treadStepCnt;				//被踩台阶数量
	int _contactStepCnt;			//接触台阶数量

	GAME_STATE _gameCurrentState;//现在的游戏状态
	ActionObject *_uiIn;	//进场UI动画
	ActionObject *_uiOut;	//返场UI动画
	ActionObject *_Helpui;
};


#endif