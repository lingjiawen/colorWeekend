//#ifndef _STEPSHELP_H_
//#define _STEPSHELP_H_
//
//#include "cocos2d.h"
//#include "Step.h"
//#include <vector>
//#include "player.h"
//#include "cocostudio/CocoStudio.h"
//#include "Colors.h"
//#include "Tag.h"
//using namespace std;
//
//
//class StepsLayer :public cocos2d::Layer
//{
//public:
//
//	CREATE_FUNC(StepsLayer);
//	virtual bool init();
//
//	Step * AddStep(StepAttr &BA, cocos2d::Point Pt);				//根据属性和坐标添加台阶
//	Step * AddStepTail(StepAttr &blockAttri);						//根据属性往容器中添加台阶
//	void CleanupStep();												//从容器第一个开始清除已移出台阶
//	void StepsMove(float Dur);										//台阶移动函数
//	float getMoveDur() { return _stepMoveDur; }
//
//	Colors _colors;														//台阶们的颜色
//	int _colorIndex;													//台阶索引
//
//	virtual void setPosition(const cocos2d::Point &ps);					//reset setPosition
//
//
//protected:
//	void AddColorTruth(float dt);					//增加可踩颜色
//
//private:
//	vector<Step *> Steps_;							//台阶们
//	vector<StepAttr> _stepsAttribute;				//台阶们的属性
//	int _stepIndex;									//台阶索引
//
//	float _stepMoveDur;								//台阶移动时间
//
//	void InitStepsAttr();							//初始化台阶属性
//	void StepMoveCB(cocos2d::Node *Step);
//};
//
//
//#endif