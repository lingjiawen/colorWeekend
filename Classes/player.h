#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
USING_NS_CC;

class PlayerJump:public JumpBy
{
public:
	static PlayerJump * create(float duration, const Point& position, float height, int jumps);
	Point GetDelta(){return _delta;}
};

class Player: public Sprite 
{
public:
	enum PLAYER_STATE 
	{
		PS_UNKNOW,
		PS_WALK,
		PS_BACK,
		PS_JUMP,
		PS_JUMPBACK,
		PS_DOWN
	};

	Player():
		_playerState(PS_UNKNOW),
		_playerState2(PS_UNKNOW),
		_jumpCnt(0) {}

	virtual bool init();
	CREATE_FUNC(Player);

	void DownAction();
	void DownRotated();
	void run(float y);
	void ContactBack(float duration, const Point& deltaPosition);
	bool runJumpAction(bool touchBegin);
	void jumpActionCallBack();
	int getJumpCnt();
	void setJumpCnt(int jc);
	void setPlayerState(PLAYER_STATE ps);
	void stopBack() 
	{
		this->stopActionByTag(PS_BACK);
		_playerState2 = PS_UNKNOW;
	}
	bool playerState(PLAYER_STATE ps);

protected:
	virtual void setRotationSkewX(float rotationX){}
	virtual void setRotationSkewY(float rotationY)

	{
		cocos2d::Node::setRotation(rotationY);
		if (! _recursiveDirty) {            
			_recursiveDirty = true;         
			setDirty(true);                 
			if (!_children.empty())         
				setDirtyRecursively(true);  
		}                               
	}
private:
	PLAYER_STATE _playerState;
	PLAYER_STATE _playerState2;
	int _jumpCnt;
	PlayerJump *_jumpAction;
	
};


#endif