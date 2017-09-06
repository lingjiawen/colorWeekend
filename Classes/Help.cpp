//
//#include "StepsLayer.h"
//#include "RanFunc.h"
//#include "GameLayer.h"
//
//USING_NS_CC;
//namespace rf = RanFunc;		//�������
//
//bool StepsLayer::init()
//{
//	_colorIndex = 0;
//	_stepIndex = 0;
//	_colors.randomColors();			//��11����ɫ������ң�����_ColorsKinds������
//	InitStepsAttr();			//��ʼ��̨�����ԣ�y,collar,allow?)
//	Size visibleSize = Director::getInstance()->getVisibleSize();
//	this->setTag(STEPS_LAYER_TAG);
//	StepAttr sa(0, _colors[0], true);
//	auto FirstStep = AddStep(sa, ccp(0, 0));
//	Steps_.push_back(FirstStep);
//	Point TailPoint;
//	do
//	{
//		TailPoint = L_STEP->getPosition();
//		AddStepTail(sa);
//	} while (TailPoint.x<visibleSize.width);
//
//	return true;
//}
//
//Step * StepsLayer::AddStep(StepAttr &SA, cocos2d::Point Pt)
//{
//	auto StepSprite = Step::create();
//	auto StepSize = StepSprite->getContentSize();
//	StepSprite->setPosition(Pt.x + StepSize.width / 2, Pt.y);
//	StepSprite->setAttributes(SA);
//	if (SA._allow)
//	{
//
//		auto body = PhysicsBody::createEdgeBox(StepSprite->getContentSize());
//		body->setCategoryBitmask(1);    // 0001
//		body->setContactTestBitmask(1); // 0001
//		body->setCollisionBitmask(1);   // 0001
//		StepSprite->setPhysicsBody(body);
//	}
//
//	this->addChild(StepSprite);
//	return StepSprite;
//}
//
//Step * StepsLayer::AddStepTail(StepAttr & stepAttri)
//{
//	Point TailStepPoint = L_STEP->getPosition();
//	auto StepSize = L_STEP->getContentSize();
//	Point StepPoint = Point(TailStepPoint.x + StepSize.width / 2, stepAttri._bh);
//	auto StepSprite = AddStep(stepAttri, StepPoint);
//	Steps_.push_back(StepSprite);
//	return StepSprite;
//}
//
//void StepsLayer::CleanupStep()
//{
//
//	auto Cnt = Steps_.size();
//	for (int i = 0; i < Cnt; i++)
//	{
//		auto Pt = convertToWorldSpace(Steps_[i]->getPosition());
//		auto Sz = Steps_[i]->getContentSize();
//		if (Pt.x < -Sz.width)
//		{
//			this->removeChild(Steps_[i]);
//			Steps_.erase(Steps_.begin());
//		}
//		else
//		{
//			break;
//		}
//	}
//}
//
//
//void StepsLayer::StepsMove(float Duration)
//{
//	this->stopActionByTag(STEP_MOVE_TAG);
//	_stepMoveDur = Duration;
//	auto StepsMove = MoveBy::create(_stepMoveDur, Point(-200, 0));		//200Ϊһ��̨�׵Ŀ��
//	auto StepsActionSeque = Sequence::create(StepsMove,
//		CallFuncN::create(CC_CALLBACK_1(StepsLayer::StepMoveCB, this)), NULL);
//	auto StepsMoveForever = RepeatForever::create(StepsActionSeque);
//	StepsMoveForever->setTag(STEP_MOVE_TAG);
//	this->runAction(StepsMoveForever);
//}
//
//void StepsLayer::InitStepsAttr()			//��ʼ��150��̨�׵�����
//{
//	INIT_RANDOM_ENGINE;
//	for (int i = 0; i < 10; i++)
//	{
//		for (int j = 0; j < 15; j++)
//		{
//			Color3B color;
//			float y = 0;
//			bool allow;
//			int stepIdx = i * 15 + j;
//			if (stepIdx < 2)			//��ʼ��ǰ����̨��Ϊ��ʼ��ɫ
//			{
//				color = _colors[0];
//				allow = true;
//			}
//			else
//			{
//				auto s1 = _stepsAttribute[stepIdx - 2]._allow;
//				auto s2 = _stepsAttribute[stepIdx - 1]._allow;
//
//				if (!s1 && !s2)			//ǰ��̨�׶����ܲ�
//				{
//					allow = true;
//				}
//				else if (s1 && s2)		//ǰ��̨�׶��ܲ�
//				{
//					allow = rf::rn(0, 6) < 3;
//				}
//				else if ((s2 && !s1) || (!s2 && s1))	 //ǰ��̨��ֻ��һ���ܲ�
//				{
//					allow = rf::rn(0, 1);
//				}
//
//
//				if (allow)
//				{
//					color = _colors[rf::rn(0, i)];
//				}
//				else
//				{
//					color = _colors[rf::rn(i + 1, _colors.cnt() - 1)];
//				}
//
//				y = rf::rn(-110, 110);
//				//y=������
//			}
//			_stepsAttribute.push_back(StepAttr(y, color, allow));
//		}
//	}
//}
//
//void StepsLayer::StepMoveCB(cocos2d::Node *Step)
//{
//	auto pa = (GameL *)this->getParent();
//	if (pa->getGameState() == GS_READY || pa->getGameState() == GS_HELP)
//	{
//		this->AddStepTail(_stepsAttribute[0]);
//	}
//	else if (pa->getGameState() == GS_BEGIN)
//	{
//		this->AddStepTail(_stepsAttribute[_stepIndex++]);
//		if (_stepIndex < _stepsAttribute.size())
//		{
//			if (_stepIndex % 15 == 0)
//			{
//				_colorIndex++;
//				this->scheduleOnce(schedule_selector(StepsLayer::AddColorTruth), 1.2);
//
//			}
//		}
//		else
//		{					//��Ϸ����
//			pa->setGameState(GS_END);
//		}
//	
//	}
//
//	else if (pa->getGameState() == GS_END)
//	{
//
//		//���ض�����û����
//
//	}
//	CleanupStep();
//}
//
//void StepsLayer::AddColorTruth(float dt)
//{
//	auto parent = (GameL *)this->getParent();
//	parent->AddCDot(_colors[_colorIndex]);
//}
//
//void StepsLayer::setPosition(const cocos2d::Point &ps)
//{
//	auto P = ps - this->getPosition();		//��ȡsprite����
//	for (int i = 0; i<Steps_.size(); i++)		//�ƶ�̨��
//	{
//		auto p = Steps_[i]->getPosition() + P;
//		Steps_[i]->setPosition(p);
//	}
//}