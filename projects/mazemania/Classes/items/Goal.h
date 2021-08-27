/*
 * Goal.h
 *
 *  Created on: Apr 10, 2014
 *      Author: nande
 */

#ifndef GOAL_H_
#define GOAL_H_

#include "cocos2d.h"

#include "Item.h"
#include "Player.h"
#include "Board.h"
#include "utils/events.h"

USING_NS_CC;

class Goal: public Item {
	Event *e_game_over, *e_points;
public:
	Goal(Board*b, int xi, int yi);
	void activate(Player *p);
	virtual ~Goal();
	static void unload();
};

#endif /* GOAL_H_ */
