/*
 * Item.h
 *
 *  Created on: Apr 10, 2014
 *      Author: nande
 */

#ifndef ITEM_H_
#define ITEM_H_
#include "cocos2d.h"

USING_NS_CC;

class Player;
class Board;
class Item: public CCSprite {
	bool active;
public:
	Board * b;
	int xi,yi;
	virtual void activate(Player *p){
		//overwrite me
	}
	Item(Board *b, int xi=0, int yi=0);
	virtual ~Item();
	virtual void setPaused(bool pause=true);
	static void unload();
};

#endif /* ITEM_H_ */
