#include "GameLayer1.h"
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "Bangs.h"
#include "SimpleAudioEngine.h"
#include "GamePause.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;

bool GameL1::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//���ó�ʼ״̬
	_gameCurrentState = GS_READY;
	_cDotCnt = 0;
	_treadStepCnt = 0;
	_contactStepCnt = 0;

	//������ͣUI
	auto pauseUI = GUIReader::getInstance()->widgetFromJsonFile("PauseBtn/PauseBtn_1.ExportJson");
	pauseUI->setPosition(Point::ZERO);
	pauseUI->setTouchEnabled(false);
	this->addChild(pauseUI, 3, 9);

	//��ȡ�ؼ�����
	auto _pauseBtn = static_cast<Button*>(Helper::seekWidgetByName(pauseUI, "pauseBtn"));
	_pauseBtn->addTouchEventListener(this, toucheventselector(GameL1::doPause));

	//Size visibleSize = Director::getInstance()->getVisibleSize();
	//MenuItemImage* pauseBtn = MenuItemImage::create(
	//	"pause@2x.png",
	//	"pause.png",
	//	this,
	//	menu_selector(GameL::doPause));
	//Menu* pause = Menu::create(pauseBtn, NULL);
	//pause->setPosition(visibleSize.width - 20, visibleSize.height - 20);
	//this->addChild(pause);


	//��ʼ��Steps
	_StepLayer = StepsLayer::create();
	_StepLayer->setPosition(Point::ZERO);
	this->addChild(_StepLayer);
	this->scheduleOnce(schedule_selector(GameL1::uiOutActionDoneCallBack1), 0.1);
	auto layerUp = MoveBy::create(0.5, Point(0, 100));	//̨������100
	auto esoUp = EaseSineOut::create(layerUp);
	this->runAction(esoUp);

	//��ʼ������
	_player = Player::create();
	auto playerBody = PhysicsBody::createBox(_player->getContentSize());
	playerBody->setCategoryBitmask(1);    // 0001
	playerBody->setContactTestBitmask(1); // 0001
	playerBody->setCollisionBitmask(1);   // 0001
	playerBody->setGravityEnable(false);	// ȡ������
	_player->setPhysicsBody(playerBody);
	_player->setPosition(300, 500);
	this->addChild(_player, 2);
	this->scheduleOnce(schedule_selector(GameL1::playerInit), 0.1);

	//����
	_soundBtn = true;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/bg.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/bg.wav", true);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/color.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/jump1.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/jump2.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/pop.wav");

	//���㴥���¼�����
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);				// ��û�¼�
	touchListener->onTouchBegan = CC_CALLBACK_2(GameL1::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameL1::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//������ײ����
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameL1::onContactBegin, this);
	contactListener->onContactSeperate = CC_CALLBACK_1(GameL1::onContactSeperate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();		//�ж���Ϸ�Ƿ����

	return true;
}

void GameL1::gameOverCB(float dt)
{

	ActionManagerEx::destroyInstance();
	auto reScene = TransitionProgressRadialCW::create(0.6, HelloWorld::createScene()); //������ʱ����ʧ����ʼ����
	Director::getInstance()->replaceScene(reScene);

}

//ÿ֡�ж���Ϸ�Ƿ����
void GameL1::update(float delta)
{

	auto pPt = convertToWorldSpace(_player->getPosition());
	auto pSize = _player->getContentSize();

	if (pPt.x + pSize.width / 2<0 || pPt.y + pSize.height / 2<0)
	{
		this->stopActionByTag(VIBRATION_TAG);
		auto mD = MoveBy::create(0.05, Point(0, -80));
		auto eso1 = EaseSineOut::create(mD);
		auto mU = MoveBy::create(0.05, Point(0, 80));
		auto esi1 = EaseSineIn::create(mU);
		auto actionSeq = Sequence::create(eso1, esi1, NULL);
		actionSeq->setTag(VIBRATION_TAG);
		this->runAction(actionSeq);


		_gameCurrentState = GS_OVER;
		_player->stopAllActions();
		_StepLayer->stopAllActions();
		auto moveDown = MoveBy::create(3, ccp(0, -120));	//��׹
		_StepLayer->runAction(moveDown);
		this->unscheduleUpdate();
		//����
		auto fire = Bangs::create();
		this->addChild(fire, 4);
		auto firePos = _player->getPosition();
		fire->setPosition(firePos);
		fire->fireworks(Rect(0, 500, 300, 500));

		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();//ȡ�����м���

		if (_soundBtn)		//�����������
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sounds/pop.wav");

		//�񻨽����ص���ʼ����
		this->scheduleOnce(schedule_selector(GameL1::gameOverCB), 3);
		return;
	}
}

void GameL1::uiOutActionDoneCallBack1(float dt){
	_gameCurrentState = GS_BEGIN;
	_StepLayer->StepsMove(0.50);			//�ӿ�̨���ƶ��ٶ�
	AddCDot(_StepLayer->_colors[0]);			//���ص�һ��ColDot
}

bool GameL1::onTouchBegan(Touch *touch, Event *unused_event) {
	if (_gameCurrentState == GS_BEGIN || _gameCurrentState == GS_HELP)
	{
		auto vibration = _player->runJumpAction(true);
		if (vibration&&_gameCurrentState == GS_BEGIN)
		{
			//��Ч��
			_StepLayer->stopActionByTag(VIBRATION_TAG);
			auto moveDown = MoveBy::create(0.2, ccp(0, -8));
			auto eso = EaseSineOut::create(moveDown);
			auto moveUp = MoveBy::create(0.2, ccp(0, 8));
			auto esi = EaseSineIn::create(moveUp);
			auto actionSeq = Sequence::create(eso, esi, NULL);
			actionSeq->setTag(VIBRATION_TAG);
			_StepLayer->runAction(actionSeq);
		}
	}
	return true;
}

void GameL1::onTouchEnded(Touch *touch, Event *unused_event) {}

void GameL1::AddCDot(const Color3B &color)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto cDot = Sprite::create("colorDot@2x.png");
	auto cDotsize = cDot->getContentSize();
	cDot->setColor(color);
	Point postion, DetPt;
	//���һ��
	if (_cDotCnt < 5)
	{
		postion = Point(visibleSize.width + cDotsize.width, visibleSize.height - 150);
		if (_cDotCnt == 0)
		{
			DetPt = Point(visibleSize.width / 2, postion.y);
		}
		else
		{
			for (int i = 0; i < _cDotCnt; i++)
			{
				auto pDot = this->getChildByTag(DOT_FIRST_TAG + i);
				auto pMove = MoveBy::create(0.6, Point(-(cDotsize.width / 2), 0));
				auto eEO = EaseElasticOut::create(pMove);
				pDot->runAction(eEO);
				if (i == _cDotCnt - 1)
				{
					DetPt = Point((pDot->getPosition().x + cDotsize.width / 2), postion.y);
				}
			}
		}
	}
	else
	{
		postion = Point(visibleSize.width + cDotsize.width, visibleSize.height - 150 - 20 - cDotsize.height);
		if (_cDotCnt == 5)
		{
			DetPt = Point(visibleSize.width / 2, postion.y);
		}
		else
		{
			for (int i = 5; i < _cDotCnt; i++)
			{
				auto pDot = this->getChildByTag(DOT_FIRST_TAG + i);
				auto pMove = MoveBy::create(0.8, ccp(-(cDotsize.width / 2), 0));
				auto eEO = EaseElasticOut::create(pMove);
				pDot->runAction(eEO);
				if (i == _cDotCnt - 1)
				{
					DetPt = Point((pDot->getPosition().x + cDotsize.width / 2), postion.y);
				}
			}
		}
	}
	cDot->setPosition(postion);
	this->addChild(cDot);
	cDot->setTag(DOT_FIRST_TAG + _cDotCnt);
	auto mAction = MoveTo::create(1.5, DetPt);
	auto eEO = EaseElasticOut::create(mAction);
	cDot->runAction(eEO);
	if (_soundBtn)
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sounds/color.wav");
	_cDotCnt++;
}

bool GameL1::onContactBegin(PhysicsContact& contact)
{
	auto sprite1 = contact.getShapeA()->getBody()->getNode();
	auto sprite2 = contact.getShapeB()->getBody()->getNode();

	auto step = _player == sprite1 ? sprite2 : sprite1;

	auto Ifcont = contact.getContactData();

	auto stepPosition = _StepLayer->convertToWorldSpace(step->getPosition());
	auto playerPosition = convertToWorldSpace(_player->getPosition());

	_contactStepCnt++;

	if (Ifcont->normal.y > 0)
	{
		if (!_player->playerState(Player::PS_BACK))
		{
			auto postion = convertToNodeSpace(Point(0, stepPosition.y + step->getContentSize().height / 2));
			_player->run(postion.y);
		}
		_treadStepCnt++;
	}
	else if (Ifcont->normal.x < 0)
	{
		//С���鱻̨���赲
		auto backPos = stepPosition.x - step->getContentSize().width / 2 - _player->getContentSize().width / 2;
		auto postion = convertToNodeSpace(Point(backPos, playerPosition.y));
		_player->setPosition(postion);
		_player->ContactBack(_StepLayer->getMoveDur(), Point(-200, 0));
	}

	return true;
}

//�Ӵ������¼��ص�
void GameL1::onContactSeperate(PhysicsContact& contact)
{
	auto Ifcont = contact.getContactData();
	if (!Ifcont)
	{
		return;
	}
	if (Ifcont->normal.y > 0)
		_treadStepCnt--;
	_contactStepCnt--;

	if (_treadStepCnt == 0)
	{
		if (_player->getJumpCnt() == 0 && !_player->playerState(Player::PS_JUMP))
		{
			_player->stopAllActions();
			_player->setPlayerState(Player::PS_JUMP);
			_player->setJumpCnt(1);
			_player->jumpActionCallBack();
		}
		//	if (!_player->playerState(Player::PS_JUMP))
		//		_player->DownAction();				//�޸�
	}
	if (_contactStepCnt == 0)
	{
		if (_player->playerState(Player::PS_BACK)){
			_player->stopBack();
		}
	}
}

void GameL1::setPosition(const Point &pos)
{
	auto sPos = pos - this->getPosition();								//�ƶ�λ��
	_player->setPosition(_player->getPosition() + sPos);				//�ƶ�С����
	_StepLayer->setPosition(_StepLayer->getPosition() + pos);			//�ƶ�̨��
}

GAME_STATE GameL1::getGameState()
{
	return _gameCurrentState;
}

void GameL1::setGameState(GAME_STATE gs)
{
	_gameCurrentState = gs;
}

void GameL1::playerInit(float dt)
{
	_player->DownRotated();				//ת��
	_player->DownAction();				//������
}

void GameL1::doPause(Ref* pSender, TouchEventType type)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	switch (type)
		case TouchEventType::TOUCH_EVENT_ENDED:
	{
		Size size = Director::sharedDirector()->getWinSize();
		RenderTexture* texture = RenderTexture::create((int)size.width, (int)size.height);
		texture->setPosition(Point(size.width / 2, size.height / 2));
		texture->begin();
		Director::sharedDirector()->getRunningScene()->visit();
		texture->end();

		//����Ϸ������ͣ��ѹ�볡����ջ�����л���GamePause����
		Director::sharedDirector()->pushScene(GamePause::createScene(texture));
	}
}