#include "player.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"

USING_NS_CC;
bool Player::init()
{
	this->initWithFile("playerTop@2x.png", Rect(8,8, 60, 60));	//���ǳߴ�Ϊ8,8,60,60
	return true;
}

void Player::DownAction()			//��ʼ���ǵ�����
{
	if(_playerState == PS_JUMP || _playerState == PS_JUMPBACK){
		return;
	}
	if(_playerState!=PS_DOWN){

		auto point = convertToWorldSpace(this->getPosition());
		auto height = point.y+70;
		auto moveAction = MoveBy::create(height/1000,Point(0,-height));
		auto movesei = EaseSineIn::create(moveAction);
		this->runAction(movesei);
		_playerState = PS_DOWN;
	}
}

void Player::DownRotated()						//����360���ת
{
	auto Rotate = RotateBy::create(0.8,360);
	auto RotateForever = RepeatForever::create(Rotate);
	RotateForever->setTag(ROTATE_TAG);						//������תTag
	this->runAction(RotateForever);
}

void Player::run(float y)
{
	if(_playerState == PS_JUMP){
			return;
	}
	if(_playerState!=PS_WALK){
		this->stopAllActions();
		this->setRotation(0);
		auto size = this->getContentSize();
		auto pt = this->getPosition();
		this->setPosition(Point(pt.x, y+size.height/2));
		_jumpCnt=0;
		_playerState=PS_WALK;
	}
}

void Player::ContactBack(float duration, const Point& deltaPosition)
{
	if(_playerState2!=PS_BACK){
		auto moveAction = MoveBy::create(duration,deltaPosition);
		auto moveForever = RepeatForever::create(moveAction);
		moveForever->setTag(PS_BACK);
		this->runAction(moveForever);
		_playerState2=PS_BACK;
	}
}

bool Player::playerState( PLAYER_STATE ps )
{
	if (ps == PS_BACK) 
	{
		return (_playerState2 == ps);
	} 
	else 
	{
		return(_playerState==ps);
	}
}

bool Player::runJumpAction(bool touchBegin) 
{

	if (_jumpCnt == 0 && _playerState != PS_WALK)
	{
		return false;
	}

	//˵�õ���������Σ���
	if (_jumpCnt >= 2) 
	{
		return false;
	}
	if (!touchBegin)
		return false;
	if (_playerState == PS_JUMP || _playerState == PS_JUMPBACK) 
	{
		this->stopActionByTag(ROTATE_TAG);		//��ת��
		this->stopActionByTag(JUMP_TAG);		//������
		this->stopActionByTag(BACK_TAG);		//������
	}
	_jumpCnt++;							//����һ��
	_playerState = PS_JUMP;

	DownRotated();					//���˾�ת����
	_jumpAction = PlayerJump::create(0.3f, Point(0, 240), 60, 1);
	auto jumpSeq = Sequence::create(
		_jumpAction,
		CallFunc::create(this, callfunc_selector(Player::jumpActionCallBack)),
		NULL);
	jumpSeq->setTag(JUMP_TAG);				//��ԾTagΪ12
	this->runAction(jumpSeq);


	//�ӵ�����
	auto gl = (GameL *)this->getParent();
	if(gl->soundIsOn()){
		if (_jumpCnt == 1) {
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sounds/jump1.wav");
		} else if (_jumpCnt == 2) {
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sounds/jump2.wav");
		}
	}
	return true;
}

void Player::jumpActionCallBack()
{
	//_jumpAction->retain();
	auto pos = this->getParent()->convertToWorldSpace(this->getPosition());
	auto height = pos.y+100;
	auto downAction = MoveBy::create(height/1300,ccp(0,-height));
	auto esiDown = EaseSineIn::create(downAction);
	esiDown->setTag(BACK_TAG);
	this->runAction(esiDown);
	_playerState = PS_JUMPBACK;
}

PlayerJump * PlayerJump::create(float duration, const Point& position, float height, int jumps)
{

	PlayerJump *jumpBy = new PlayerJump();
	jumpBy->initWithDuration(duration, position, height, jumps);
	jumpBy->autorelease();

	return jumpBy;
}

int Player::getJumpCnt()
{
	return _jumpCnt;
}

void Player::setJumpCnt(int jc)
{
	_jumpCnt = jc;
}

void Player::setPlayerState(PLAYER_STATE ps)
{
	_playerState = ps;
}