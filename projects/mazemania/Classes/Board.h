/*
 * Board.h
 *
 *  Created on: 27/03/2014
 *      Author: nande
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "cocos2d.h"

#include "menu/BtnPause.h"

#include "Door.h"
#include "items/Item.h"
#include "utils/events.h"

USING_NS_CC;
class Player;//c++ you suck
class Item;

class Board:public CCNode {
	CCArray *doors, *pjs, *items;
	CCPoint verts[4];

	//holds the positions free in random, its a vector of XYXYXYXYXY data should be taken as pairs
	//declared as int* because c++ is so shit i cant nullify an int[] so i dont know if its allocated
	//also i cant even do random_pos = new..
	int *random_pos;
	int c_random_pos, last_random, c_spawn_bombs, c_spawn_boost, c_spawn_points;
	unsigned short spawn_turn;
	void initRandomPos();
	void getRandomPos(int &xi, int &yi, bool noCenter=false);
	void initDoors();
	bool shouldCloseDoor(const int xi, const int yi);
	
	BtnPause *btnPause;
	Event *e_game_over, *e_player_died, *e_game_unpaused, *e_on_boost, *e_on_bomb, *e_on_point;
public:
	void spawnStuff(float f=0.0f);// esto se usa en el scheduler asi que lo pongo aca en public
	int cplayers, ctotunits;
	//tw y th hacen referencia a la cantidad de tiles(puntos), no su tamaño
	//por cada tile hay 2 puntos (puerta y centro) y otro más compartido por otro tile
	unsigned short tw, th; //size in TILES
	unsigned short w, h; //size in PIXELS
	unsigned short ccx, ccy; //center x and y in PLAYER COORDS
	bool active;

	static void onUnpaused(void* self, void* sender, void* param);
	static void onGameOver(void* self, void *sender, void* winner);
	static void onBoostActivated(void* self, void *sender, void* param);
	static void onPointActivated(void* self, void* sender, void* param);
	static void onBombActivated(void* self, void *sender, void* param);
	static void unload();

	void setPaused(bool pause);
	void checkPlayers();
	void checkItem(Player *p, int xi, int yi);
	Player *lastAlive();
	Item *itemAt(int xi, int yi);
	Door *doorAt(int xi, int yi);
	Board();
	virtual ~Board();
	void load();
	void restart();
	void clean();
	void addBoost();
	void addBomb(bool noCenter=false);
	void addPointItem();
	void resetBoost(Item *sender);
	void resetBomb(Item *sender);
	void resetPoint(Item *sender);
	void addDoor(bool vertical, CCPoint pos, int xi, int yi, int type=0);
	CCPoint coordToPos(int xi, int yi);
	bool isOnCenter(const int xi, const int yi);
};

#endif /* BOARD_H_ */
