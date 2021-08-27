/*
 * Bomb.h
 *
 *  Created on: Apr 11, 2014
 *      Author: nande
 */

#ifndef BOMB_H_
#define BOMB_H_

#include "cocos2d.h"

#include "Item.h" //parece que importando esto ya esta el board y player.... bien c++ ¿?¿?¿?¿??¿
#include "utils/events.h"

USING_NS_CC;

class Bomb: public Item {	
public:
	Event *e_activated;
	Bomb(Board*b, int xi, int yi);
	void activate(Player *p);
	virtual ~Bomb();
	static void unload();
};

#endif /* BOMB_H_ */
