/*
 * BtnPause.h
 *
 *  Created on: 22/04/2014
 *      Author: Administrador
 */

#ifndef BTNPAUSE_H_
#define BTNPAUSE_H_

#include "cocos2d.h"

#include "TouchSprite.h"
#include "utils/events.h"

USING_NS_CC;

class BtnPause: public TouchSprite {
public:
	Event* ev;
	BtnPause();
	bool onTouch();
	~BtnPause();
};

#endif /* BTNPAUSE_H_ */
