/*
 * GameTimer.h
 *
 *  Created on: 15/04/2014
 *      Author: Administrador
 */

#ifndef GAMETIMER_H_
#define GAMETIMER_H_

#include "cocos2d.h"

#include "Item.h"
#include "utils/events.h"
#include "utils/Utils.h"

USING_NS_CC;

class GameTimer: public Item {
	Event *e_game_over;
	float step;
	us state;
public:
	static void unload();
	void load();
	GameTimer(int xi, int yi);
	void setState(cus i);
	void timeOut(float f=0);
	virtual ~GameTimer(){
		EVRELEASE(e_game_over)
	}
};

#endif /* GAMETIMER_H_ */
