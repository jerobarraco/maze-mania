/*
 * Point.h
 *
 *  Created on: 24/09/2014
 *      Author: nande
 */

#ifndef POINT_H_
#define POINT_H_
#include "utils/events.h"
#include "Board.h"
#include "Player.h"
#include "Item.h"

class Point: public Item{
	Event *e_activated;
public:
	int ammount;
	Point(Board *b, int xi, int yi);
	~Point(){
		EVRELEASE(e_activated)
	};
	void activate(Player *p);
	static void unload();
};

#endif /* POINT_H_ */
