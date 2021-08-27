/*
 * TouchSprite.h
 *
 *  Created on: 07/02/2014
 *      Author: nande
 */

#ifndef TOUCHSPRITE_H_
#define TOUCHSPRITE_H_
#include "cocos2d.h"

class TouchSprite :
		public cocos2d::CCSprite,
		public cocos2d::CCTargetedTouchDelegate {
	bool swallows;
	int prio;
	int hitbox;
public:
	//override this
	virtual bool onTouch();
	//you can but override this but you dont need to
	virtual bool touchsMe(cocos2d::CCTouch* touch);

	TouchSprite(int prio= 0, bool swallows=true, int hitbox=0);
	TouchSprite(const char *tx, const int prio= 0, const bool swallows=true);

	virtual ~TouchSprite(){}

	virtual void onEnter();
	virtual void onExit();

	bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
};

#endif /* TOUCHSPRITE_H_ */
