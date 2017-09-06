#ifndef _TAG_H_
#define _TAG_H_

#define ROTATE_TAG    11
#define JUMP_TAG      12
#define BACK_TAG      13
#define STEP_MOVE_TAG 30
#define STEPS_LAYER_TAG 61
#define BLOCK_TAG 62
#define VIBRATION_TAG 101
#define DOT_FIRST_TAG 500

#define L_STEP (Steps_[Steps_.size()-1])	//最后一个颜台阶
#define NS_RF namespace rf = RanFunc


enum GAME_STATE
{
	GS_READY,
	GS_HELP,
	GS_BEGIN,
	GS_OVER,
	GS_END
};

#endif