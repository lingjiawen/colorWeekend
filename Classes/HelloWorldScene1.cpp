#pragma once
#include "HelloWorldScene1.h"
#include "RanFunc.h"

USING_NS_CC;
namespace rf = RanFunc;


Scene* HelloWorld1::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();  //创建物理引擎
	// 'layer' is an autorelease object
	auto layer = HelloWorld1::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld1::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	//设置背景颜色
	auto layer = LayerColor::create(/*ccc4(149, 233, 208, 0xff)*/ccc4(135, 206, 250, 255), visibleSize.width, visibleSize.height);
	this->addChild(layer, 0);

	//云彩动画
	this->schedule(schedule_selector(HelloWorld1::CloudRun), 8.0f, kRepeatForever, 0.88f);

	InitGameLayer1();

	return true;
}


//云彩动画
void HelloWorld1::CloudRun(float dt)
{
	INIT_RANDOM_ENGINE;

	auto CloudSprite = Sprite::create("cloudBottom@2x.png");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Size CloudSize = CloudSprite->getContentSize();
	CloudSprite->setPosition(
		Point(visibleSize.width + CloudSize.width / 2,
		visibleSize.height * (rf::rn(6, 8) / 10)));
	auto CloudSpriteTop = Sprite::create("cloudTop@2x.png");
	CloudSpriteTop->setPosition(CloudSprite->getPosition());
	CloudSprite->addChild(CloudSpriteTop, 1);


	this->addChild(CloudSprite, 1);
	auto CloudRot = RotateBy::create(80,
		rf::rn(0, 1) ? 360 : -360);
	auto CloudRotForever = RepeatForever::create(CloudRot);
	auto CloudMove = MoveBy::create(30, Point(-(visibleSize.width + CloudSize.width), 0));
	auto CloudMoveSeque = Sequence::create(CloudMove,
		CallFuncN::create(CC_CALLBACK_1(HelloWorld1::CloudRunCB, this)), NULL);
	auto CloudAction = Spawn::create(CloudRotForever, CloudMoveSeque, NULL);
	CloudSprite->runAction(CloudAction);
}

void HelloWorld1::CloudRunCB(cocos2d::Node *sprite)
{
	this->removeChild(sprite);
}

void HelloWorld1::InitGameLayer1() {

	_gameLayer1 = GameL1::create();
	_gameLayer1->setPosition(Point(0, 0));
	this->addChild(_gameLayer1, 2);
}
