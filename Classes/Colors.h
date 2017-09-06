#ifndef _COLORS_H_
#define _COLORS_H_

#include "cocos2d.h"
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>

class Color {
public:
	Color();
	Color(GLubyte r, GLubyte g, GLubyte b);
	Color& operator= (const Color& other);
	const cocos2d::Color3B getCCColor();
    GLubyte _r;
    GLubyte _g;
    GLubyte _b;
};


class Colors {
public:
	Colors();
	void randomColors();
	unsigned cnt();	
	const cocos2d::Color3B operator[](int index);
private:
	std::vector<Color> _ColorsKinds;
};


#endif