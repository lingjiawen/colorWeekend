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

	void AddCDot(const Color3B &color);										//СԲ��
	void AddHelpDot(const Color3B &color);
	bool soundIsOn() { return _soundBtn; }									//��������
	virtual void setPosition(const Point &pos);
protected:
	void update(float delta);
	void doItBtnFunc(Ref* pSender, TouchEventType type);			//DOIT��ť
	void helpBtnFunc(Ref* pSender, TouchEventType type);			//������ť
	void soundBtnFunc(Ref* pSender, TouchEventType type);			//������ť
	void scoreBtnFunc(Ref* pSender, TouchEventType type);			//������ť
	void doPause(Ref* pSender, TouchEventType type);
	void uiOutActionDoneCallBack1(float dt);
	void uiOutActionDoneCallBack2(float dt);
	void HelpUIEndedCB(float dt);

	virtual bool onTouchBegan( Touch *touch, Event *unused_event );
	virtual void onTouchEnded( Touch *touch, Event *unused_event );
	virtual bool onContactBegin(PhysicsContact& contact);
	virtual void onContactSeperate(PhysicsContact& contact);
	static Scene* scene(RenderTexture* sqr);


	void gameOverCB(float dt);			//��Ϸ����
	void playerInit(float dt);			//���ǳ�ʼ��
private:
	StepsLayer *_StepLayer;			//̨��Layer
	Player *_player;				//����
	int _cDotCnt;				//�ɲ���ɫ��Ŀ
	bool _soundBtn;					//��������
	int _treadStepCnt;				//����̨������
	int _contactStepCnt;			//�Ӵ�̨������

	GAME_STATE _gameCurrentState;//���ڵ���Ϸ״̬
	ActionObject *_uiIn;	//����UI����
	ActionObject *_uiOut;	//����UI����
	ActionObject *_Helpui;
};


#endif