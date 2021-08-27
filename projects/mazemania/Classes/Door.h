/*
 * Door.h
 *
 *  Created on: 27/03/2014
 *      Author: nande
 */

#ifndef DOOR_H_
#define DOOR_H_
#include "TouchSprite.h"
#include "cocos2d.h"
enum DState {DS_OPEN=0, DS_CLOSED, DS_WALL};
USING_NS_CC;
class Door:public TouchSprite {
	bool active;
public:
	DState state;
	int type;
	bool vertical;
	int xi, yi;
	void swapState(float f=0);
	void setPaused(bool pause);
	bool onTouch();
	static void unload();
	static void load();
	void setState(DState nstate);
	void setType(int type);
	Door(bool vertical, int type, int xi, int yi);
	virtual ~Door();
};

#endif /* DOOR_H_ */
