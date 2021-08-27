/*
 * Player.h
 *
 *  Created on: 27/03/2014
 *      Author: nande
 */

#ifndef PLAYER_H_
#define PLAYER_H_
#include "cocos2d.h"

#include "Board.h"
#include "utils/Utils.h"
#include "utils/events.h"

USING_NS_CC;

class Player:public CCSprite {
	Event *e_player_died;
	Board *b;
public:
	int score;
	Direction direction, prev;
	bool alive, blocked, active;
	int team, xi, yi, nxi, nyi;
	float speedf;
	void setPaused(bool pause);
	void die();
	bool atCenter();
	void checkDoor();
	void checkStep();
	void checkCenter();
	void setCoord(int xi, int yi);
	void setSpeedFactor(const float f);
	Player(Board *board, int team=0, int xi=0, int yi=0, Direction dir=RIGHT);
	virtual ~Player();
	static void onGameOver(void *self, void *sender, void*param );
	static void onUnpause(void *self, void *sender, void *param);
};

#endif /* PLAYER_H_ */
