#pragma once
#include "HelloWorldScene.h"
#include "RanFunc.h"

USING_NS_CC;
namespace rf = RanFunc;


Scene* HelloWorld::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();  //创建物理引擎
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
	
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    //设置背景颜色
    auto layer = LayerColor::create(/*ccc4(149, 233, 208, 0xff)*/ccc4(135,206,250,255), visibleSize.width, visibleSize.height);
    this->addChild(layer, 0);

    //云彩动画
	this->schedule(schedule_selector(HelloWorld::CloudRun), 8.0f, kRepeatForever, 0.88f);

	InitGameLayer();

    return true;
}

//云彩动画
void HelloWorld::CloudRun(float dt)
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
	CloudSprite->addChild(CloudSpriteTop,1);


    this->addChild(CloudSprite, 1);
    auto CloudRot = RotateBy::create(80,
                                          rf::rn(0, 1) ? 360 : -360);
	auto CloudRotForever = RepeatForever::create(CloudRot);
    auto CloudMove = MoveBy::create(30, Point(-(visibleSize.width + CloudSize.width), 0));
	auto CloudMoveSeque = Sequence::create(CloudMove,
		CallFuncN::create(CC_CALLBACK_1(HelloWorld::CloudRunCB, this)), NULL);
	auto CloudAction = Spawn::create(CloudRotForever, CloudMoveSeque, NULL);
	CloudSprite->runAction(CloudAction);
}

void HelloWorld::CloudRunCB(cocos2d::Node *sprite) 
{
    this->removeChild(sprite);
}

void HelloWorld::InitGameLayer() {
    
	_gameLayer = GameL::create();
	_gameLayer->setPosition(Point(0,0));
	this->addChild(_gameLayer, 2);
}
