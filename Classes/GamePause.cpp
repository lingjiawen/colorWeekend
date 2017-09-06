#include "GamePause.h"
#include "ui\CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "HelloWorldScene.h"
#include "HelloWorldScene1.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;

Scene* GamePause::createScene(RenderTexture* sqr)
{

	Scene* scene = Scene::create();
	GamePause* layer = GamePause::create();
	scene->addChild(layer);
	auto _spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
	_spr->setPosition(Point(480, 512));
	_spr->setFlipY(true);
	_spr->setOpacity(100);
	scene->addChild(_spr);

	return scene;		
}

bool GamePause::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	//Sprite* _spr = Sprite::create("cloudBottom.png");
	//_spr->setPosition(Point(50, 50));
	////_spr->setFlipY(true);
	//////_spr->setColor(cocos2d::ccGRAY);
	//this->addChild(_spr);

	auto pauseUI = dynamic_cast<Layout *>(GUIReader::shareReader()->widgetFromJsonFile("pauseAmination/pauseAmination_1.ExportJson"));
	this->addChild(pauseUI);
	_uiIn = ActionManagerEx::getInstance()->playActionByName("pauseAmination_1.ExportJson", "In");

	//获取按钮
	Button* pausePlay = static_cast<Button*>(Helper::seekWidgetByName(pauseUI,"pausePlay"));
	Button* pauseHome = static_cast<Button*>(Helper::seekWidgetByName(pauseUI, "pauseHome"));
	Button* pauseRestart = static_cast<Button*>(Helper::seekWidgetByName(pauseUI, "pauseRestart"));

	//添加对象监听
	pausePlay->addTouchEventListener(this, toucheventselector(GamePause::PlayBtn));
	pauseHome->addTouchEventListener(this, toucheventselector(GamePause::HomeBtn));
	pauseRestart->addTouchEventListener(this, toucheventselector(GamePause::RestartBtn));

	return true;
}

void GamePause::PlayBtn(Ref* pSender, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		_uiOut = ActionManagerEx::getInstance()->playActionByName("pauseAmination_1.ExportJson", "Out");
		this->scheduleOnce(schedule_selector(GamePause::PlayTouchEndedCB), 0.2);
	}
	default:
		break;
	}
}

void GamePause::HomeBtn(Ref* pSender, TouchEventType type)
{
		switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		{
			_uiOut = ActionManagerEx::getInstance()->playActionByName("pauseAmination_1.ExportJson", "Out");
			this->scheduleOnce(schedule_selector(GamePause::HomeTouchEndedCB), 0.2);
		}
	default:
		break;
	}
}

void GamePause::RestartBtn(Ref* pSender, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		{
			_uiOut = ActionManagerEx::getInstance()->playActionByName("pauseAmination_1.ExportJson", "Out");
			this->scheduleOnce(schedule_selector(GamePause::RestartTouchEndedCB), 0.2);
		}
	default:
		break;
	}
}

void GamePause::PlayTouchEndedCB(float dt)
{
	_uiOut->stop();
	_uiIn->stop();
	ActionManagerEx::destroyInstance();
	Director::sharedDirector()->popScene();
}

void GamePause::HomeTouchEndedCB(float dt)
{
	_uiOut->stop();
	_uiIn->stop();
	ActionManagerEx::destroyInstance();
	Director::sharedDirector()->popScene();
	auto reScene = TransitionProgressRadialCW::create(0.6, HelloWorld::createScene()); //本场景逆时针消失到另一场景
	Director::getInstance()->replaceScene(reScene);
}

void GamePause::RestartTouchEndedCB(float dt)
{
	_uiOut->stop();
	_uiIn->stop();
	ActionManagerEx::destroyInstance();
	Director::sharedDirector()->popScene();

	Director::getInstance()->replaceScene(TransitionProgressHorizontal::create(0.1f,HelloWorld1::createScene()));
}