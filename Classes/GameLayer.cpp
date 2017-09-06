#include "GameLayer.h"
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "Bangs.h"
#include "SimpleAudioEngine.h"
#include "GamePause.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;

bool GameL::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//设置初始状态
	_gameCurrentState = GS_READY;
	_cDotCnt = 0;
	_treadStepCnt = 0;
	_contactStepCnt = 0;

	//加载UI(the Start animation)
	auto StartUI = dynamic_cast<Layout *>(GUIReader::shareReader()->widgetFromJsonFile("Start/StartupUI_1.ExportJson"));
	this->addChild(StartUI, 3, 8);			//StartUp Tag is 8
	_uiIn = ActionManagerEx::getInstance()->playActionByName("StartupUI_1.ExportJson", "In");

	////加载暂停UI
	//auto pauseUI = GUIReader::getInstance()->widgetFromJsonFile("PauseBtn/PauseBtn_1.ExportJson");
	//pauseUI->setPosition(Point::ZERO);
	//pauseUI->setTouchEnabled(false);
	//this->addChild(pauseUI,3,9);

	////获取控件对象
	//auto _pauseBtn = static_cast<Button*>(Helper::seekWidgetByName(pauseUI, "pauseBtn"));
	//_pauseBtn->addTouchEventListener(this, toucheventselector(GameL::doPause));

	//Size visibleSize = Director::getInstance()->getVisibleSize();
	//MenuItemImage* pauseBtn = MenuItemImage::create(
	//	"pause@2x.png",
	//	"pause.png",
	//	this,
	//	menu_selector(GameL::doPause));
	//Menu* pause = Menu::create(pauseBtn, NULL);
	//pause->setPosition(visibleSize.width - 20, visibleSize.height - 20);
	//this->addChild(pause);

	//获取按钮
	auto doItBtn = static_cast<Button *>(StartUI->getChildByName("Begin"));
	auto helpBtn = static_cast<Button *>(StartUI->getChildByName("help"));
	auto soundBtn = static_cast<Button *>(StartUI->getChildByName("sound"));
	auto scoreBtn = static_cast<Button *>(StartUI->getChildByName("score"));

	//设置按钮触摸事件
	doItBtn->addTouchEventListener(this, toucheventselector(GameL::doItBtnFunc));
	helpBtn->addTouchEventListener(this, toucheventselector(GameL::helpBtnFunc));
	soundBtn->addTouchEventListener(this, toucheventselector(GameL::soundBtnFunc));
	scoreBtn->addTouchEventListener(this, toucheventselector(GameL::scoreBtnFunc));
	
	//初始化Steps
	_StepLayer = StepsLayer::create();
	_StepLayer->setPosition(Point::ZERO);
	_StepLayer->StepsMove(2);
	this->addChild(_StepLayer);
	
	//初始化主角
	_player = Player::create();
	auto playerBody = PhysicsBody::createBox(_player->getContentSize());
	playerBody->setCategoryBitmask(1);    // 0001
	playerBody->setContactTestBitmask(0xFFFFFFFF); // 0001
	playerBody->setCollisionBitmask(1);   // 0001
	playerBody->setGravityEnable(false);	// 取消重力
	_player->setPhysicsBody(playerBody);
	_player->setPosition(300,800);
	this->addChild(_player, 5);
	this->scheduleOnce(schedule_selector(GameL::playerInit), 1);

	//音乐
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( "music/bg.wav" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/bg.wav", true);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/color.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/jump1.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/jump2.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/pop.wav");

	//单点触控事件监听
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);				// 吞没事件
	touchListener->onTouchBegan = CC_CALLBACK_2(GameL::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameL::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//物理碰撞监听
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameL::onContactBegin, this);
	contactListener->onContactSeperate = CC_CALLBACK_1(GameL::onContactSeperate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();		//判断游戏是否结束

	return true;
}

void GameL::gameOverCB(float dt)
{

	ActionManagerEx::destroyInstance();
	auto reScene = TransitionProgressRadialCW::create(0.6, HelloWorld::createScene()); //场景逆时针消失到开始场景
	Director::getInstance()->replaceScene(reScene);

}

//每帧判断游戏是否结束
void GameL::update(float delta)
{

	auto pPt = convertToWorldSpace(_player->getPosition());
	auto pSize = _player->getContentSize();

	if(pPt.x+pSize.width/2<0 || pPt.y+pSize.height/2<0)
	{
		this->stopActionByTag(VIBRATION_TAG);
		auto mD = MoveBy::create(0.05, ccp(0, -80));
		auto eso1 = EaseSineOut::create(mD);
		auto mU = MoveBy::create(0.05, ccp(0, 80));
		auto esi1 = EaseSineIn::create(mU);
		auto actionSeq = Sequence::create(eso1, esi1, NULL);
		actionSeq->setTag(VIBRATION_TAG);
		this->runAction(actionSeq);


		_gameCurrentState = GS_OVER;
		_player->stopAllActions();
		_StepLayer->stopAllActions();
		auto moveDown = MoveBy::create(3,ccp(0,-120));	//下移
		_StepLayer->runAction(moveDown);
		this->unscheduleUpdate();
		//放礼花
		auto fire = Bangs::create();
		this->addChild(fire,4);
		auto firePos = _player->getPosition();
		fire->setPosition(firePos);
		fire->fireworks(Rect(0, 500, 300, 500));

		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();//取消所有监听

		if (_soundBtn)		//如果开了声音
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sounds/pop.wav");

		//礼花结束回到开始场景
		this->scheduleOnce(schedule_selector(GameL::gameOverCB), 3);
		return;
	}
}

void GameL::doItBtnFunc(Ref* pSender, TouchEventType type)
{
	auto doitBt = (	Button *)pSender;
	if(type==TOUCH_EVENT_BEGAN)
	{
		doitBt->setOpacity(130);
	}
	else if(type==TOUCH_EVENT_ENDED)
	{
		doitBt->setOpacity(80);		
		_uiOut = ActionManagerEx::getInstance()->playActionByName("StartupUI_1.ExportJson","Out");
		this->scheduleOnce(schedule_selector(GameL::uiOutActionDoneCallBack1), 0.2);
		auto layerUp = MoveBy::create(0.5,Point(0,100));	//台阶上升100
		auto esoUp = EaseSineOut::create(layerUp);
		this->runAction(esoUp);

		//加载暂停UI
		auto pauseUI = GUIReader::getInstance()->widgetFromJsonFile("PauseBtn/PauseBtn_1.ExportJson");
		pauseUI->setPosition(Point::ZERO);
		pauseUI->setTouchEnabled(false);
		this->addChild(pauseUI);

		//获取控件对象
		Button* pauseBtn = static_cast<Button*>(Helper::seekWidgetByName(pauseUI,"pauseBtn"));
		pauseBtn->addTouchEventListener(this, toucheventselector(GameL::doPause));
	}
}

void GameL::helpBtnFunc(Ref* pSender, TouchEventType type)
{
	auto helpBtn = (Button *)pSender;
	if (type == TOUCH_EVENT_BEGAN)
	{
		helpBtn->setOpacity(130);
	}
	else if (type == TOUCH_EVENT_ENDED)
	{
		helpBtn->setOpacity(80);
		_uiOut = ActionManagerEx::getInstance()->playActionByName("StartupUI_1.ExportJson", "Out");
		this->scheduleOnce(schedule_selector(GameL::uiOutActionDoneCallBack2), 0.2);

		auto HelpUI = dynamic_cast<Layout *>(GUIReader::shareReader()->widgetFromJsonFile("HelpAnimation/HelpAnimation_1.ExportJson"));
		HelpUI->setPosition(Point(480,512));
		HelpUI->setTouchEnabled(false);
		this->addChild(HelpUI, 1, 10);
		_Helpui = ActionManagerEx::getInstance()->playActionByName("HelpAnimation_1.ExportJson", "Animation0");
		this->scheduleOnce(schedule_selector(GameL::HelpUIEndedCB), 1);

		//加载暂停UI
		auto pauseUI = GUIReader::getInstance()->widgetFromJsonFile("PauseBtn/PauseBtn_1.ExportJson");
		pauseUI->setPosition(Point::ZERO);
		pauseUI->setTouchEnabled(false);
		this->addChild(pauseUI);


		//获取控件对象
		Button* pauseBtn = static_cast<Button*>(Helper::seekWidgetByName(pauseUI, "pauseBtn"));
		pauseBtn->addTouchEventListener(this, toucheventselector(GameL::doPause));

	}
}

void GameL::soundBtnFunc(Ref* pSender, TouchEventType type)
{
	//播不播音乐
	auto *soundBt = ( Button *)pSender;
	if(type == TOUCH_EVENT_ENDED)
	{
		soundBt->setBright(!soundBt->isBright());
		_soundBtn = !_soundBtn;
		if (!_soundBtn)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		}
		else
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/bg.wav", true);
		}
	}
}

void GameL::scoreBtnFunc(Ref* pSender, TouchEventType type)
{
	//暂时没实现
}

void GameL::uiOutActionDoneCallBack1(float dt){
	_uiIn->stop();
	_uiOut->stop();
	this->removeChildByTag(8);				//StartUpUp Tag=8
	_gameCurrentState = GS_BEGIN;
	_StepLayer->StepsMove(0.50);			//加快台阶移动速度
	AddCDot(_StepLayer->_colors[0]);			//加载第一个ColDot
}

void GameL::uiOutActionDoneCallBack2(float dt){
	_uiIn->stop();
	_uiOut->stop();
	this->removeChildByTag(8);				//StartUpUp Tag=8
	_gameCurrentState =GS_HELP;
	_StepLayer->StepsMove(0.50);			//加快台阶移动速度
	AddCDot(_StepLayer->_colors[0]);			//加载第一个ColDot
}

bool GameL::onTouchBegan(Touch *touch, Event *unused_event) {
	if (_gameCurrentState == GS_BEGIN||_gameCurrentState== GS_HELP) 
	{
		auto vibration = _player->runJumpAction(true);
		if (vibration&&_gameCurrentState == GS_BEGIN)
		{
			//震动效果
			_StepLayer->stopActionByTag(VIBRATION_TAG);
			auto moveDown = MoveBy::create(0.2,ccp(0,-8));
			auto eso = EaseSineOut::create(moveDown);
			auto moveUp = MoveBy::create(0.2,ccp(0,8));
			auto esi = EaseSineIn::create(moveUp);
			auto actionSeq = Sequence::create(eso,esi,NULL);
			actionSeq->setTag(VIBRATION_TAG);
			_StepLayer->runAction(actionSeq);
		}
	}
	return true;
}

void GameL::onTouchEnded(Touch *touch, Event *unused_event) {}

void GameL::AddCDot(const Color3B &color)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto cDot = Sprite::create("colorDot@2x.png");
	auto cDotsize = cDot->getContentSize();
	cDot->setColor(color);
	Point postion,DetPt;
	//五个一行
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

//void GameL::AddHelpDot(const Color3B &color)
//{
//	Size visibleSize = Director::getInstance()->getVisibleSize();
//	auto cDot = Sprite::create("colorDot@2x.png");
//	auto cDotsize = cDot->getContentSize();
//	cDot->setColor(color);
//	Point postion, DetPt;
//	postion = Point(visibleSize.width + cDotsize.width, visibleSize.height - 150);
//	if (_cDotCnt == 0)
//	{
//		DetPt = Point(visibleSize.width / 2, postion.y);
//	}
//	else
//	{
//		for (int i = 0; i < _cDotCnt; i++)
//		{
//			auto pDot = this->getChildByTag(DOT_FIRST_TAG + i);
//			auto pMove = MoveBy::create(0.6, Point(-(cDotsize.width / 2), 0));
//			auto eEO = EaseElasticOut::create(pMove);
//			pDot->runAction(eEO);
//			if (i == _cDotCnt - 1)
//			{
//				DetPt = Point((pDot->getPosition().x + cDotsize.width / 2), postion.y);
//			}
//		}
//	}
//	cDot->setPosition(postion);
//	this->addChild(cDot);
//	cDot->setTag(DOT_FIRST_TAG + _cDotCnt);
//	auto mAction = MoveTo::create(1.5, DetPt);
//	auto eEO = EaseElasticOut::create(mAction);
//	cDot->runAction(eEO);
//	if (_soundBtn)
//		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sounds/color.wav");
//	_cDotCnt++;
//}

bool GameL::onContactBegin(PhysicsContact& contact) 
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
		//小方块被台阶阻挡
		auto backPos = stepPosition.x - step->getContentSize().width / 2 - _player->getContentSize().width / 2;
		auto postion = convertToNodeSpace(Point(backPos, playerPosition.y));
		_player->setPosition(postion);
		_player->ContactBack(_StepLayer->getMoveDur(), Point(-200, 0));
	}

	return true;
}

//接触分离事件回调
void GameL::onContactSeperate(PhysicsContact& contact) 
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
	//		_player->DownAction();				//修改
	}
	if (_contactStepCnt == 0)
	{
		if (_player->playerState(Player::PS_BACK)){
			_player->stopBack();
		}
	}
}


void GameL::setPosition(const Point &pos)
{
	auto sPos = pos - this->getPosition();								//移动位移
	_player->setPosition(_player->getPosition() + sPos);				//移动小主角
	_StepLayer->setPosition(_StepLayer->getPosition() + pos);			//移动台阶
}

GAME_STATE GameL::getGameState()
{
	return _gameCurrentState;
}

	void GameL::setGameState(GAME_STATE gs)
{
	_gameCurrentState = gs;
}

void GameL::playerInit(float dt)
{
	_player->DownRotated();				//转着
	_player->DownAction();				//掉下来
}

void GameL::doPause(Ref* pSender, TouchEventType type)
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

		//将游戏界面暂停，压入场景堆栈。并切换到GamePause界面
		Director::sharedDirector()->pushScene(GamePause::createScene(texture));
	}
}

void GameL::HelpUIEndedCB(float dt)
{
	_Helpui->stop();
}