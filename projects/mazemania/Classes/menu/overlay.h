#ifndef OVERLAY_H
#define OVERLAY_H
#include "cocos2d.h"

//class Overlay : public cocos2d::CCDrawNode{
class Overlay : public cocos2d::CCLayerColor{
	bool _swallow;
public:
	Overlay(const bool &swallow=false);
	bool ccTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
};

#endif // OVERLAY_H
