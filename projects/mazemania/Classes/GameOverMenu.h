/*
 * GameOverMenu.h
 *
 *  Created on: 09/04/2014
 *      Author: Administrador
 */

#ifndef GAMEOVERMENU_H_
#define GAMEOVERMENU_H_

#include "cocos2d.h"

#include "utils/Utils.h"
#include "utils/events.h"
#include "menu/BtnUnpause.h"
#include "menu/btnevent.h"
#include "scenes/help.h"

USING_NS_CC;

cus C_LABELTXS=3;
cus C_WAVELBLS=2;
//todo mover a menu/
class GameOverMenu: public cocos2d::CCNode{
	CCTexture2D *label_txs[C_LABELTXS];
	CCSprite *label;
	Animation *a_label, *player;
	CCSprite *wave_lbls[C_WAVELBLS]; //time, points, clicks *2 (label+label para el value)
	CCLabelBMFont *wave_vals[C_WAVELBLS]; //time, points, clicks *2 (label+label para el value)

	BtnUnpause *btnResume;
	BtnEvent *btnHelp;
	Event *e_help, *e_hback;
public:
	Help *help;
	void showHelp();
	static void onHideHelp(void *self, void *sender, void *param);
	static void onShowHelp(void *self, void *sender, void *param);
	GameOverMenu();
	virtual ~GameOverMenu();
	static void unload();
	void load();
	void show(int winner);
	//void onReplay(CCObject *o);
	//void onBackToMenu(CCObject *o);
};

#endif /* GAMEOVERMENU_H_ */
