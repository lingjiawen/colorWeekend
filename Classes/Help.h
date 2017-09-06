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
//	Step * AddStep(StepAttr &BA, cocos2d::Point Pt);				//�������Ժ��������̨��
//	Step * AddStepTail(StepAttr &blockAttri);						//�������������������̨��
//	void CleanupStep();												//��������һ����ʼ������Ƴ�̨��
//	void StepsMove(float Dur);										//̨���ƶ�����
//	float getMoveDur() { return _stepMoveDur; }
//
//	Colors _colors;														//̨���ǵ���ɫ
//	int _colorIndex;													//̨������
//
//	virtual void setPosition(const cocos2d::Point &ps);					//reset setPosition
//
//
//protected:
//	void AddColorTruth(float dt);					//���ӿɲ���ɫ
//
//private:
//	vector<Step *> Steps_;							//̨����
//	vector<StepAttr> _stepsAttribute;				//̨���ǵ�����
//	int _stepIndex;									//̨������
//
//	float _stepMoveDur;								//̨���ƶ�ʱ��
//
//	void InitStepsAttr();							//��ʼ��̨������
//	void StepMoveCB(cocos2d::Node *Step);
//};
//
//
//#endif