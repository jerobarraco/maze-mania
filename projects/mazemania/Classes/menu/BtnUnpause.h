/*
 * BtnUnpause.h
 *
 *  Created on: 22/04/2014
 *      Author: Administrador
 */

#ifndef BTNUNPAUSE_H_
#define BTNUNPAUSE_H_

#include "cocos2d.h"

#include "TouchSprite.h"
#include "utils/events.h"

USING_NS_CC;
//Transformar en un btnevent
class BtnUnpause: public TouchSprite {
public:
	BtnUnpause();
	bool onTouch();
};

#endif /* BTNUNPAUSE_H_ */
