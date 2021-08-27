#ifndef EXIT_H
#define EXIT_H
#include "cocos2d.h"

class ExitScene:public cocos2d::CCScene{
public:
	ExitScene();
	void onEnterTransitionDidFinish();
};

#endif // EXIT_H
