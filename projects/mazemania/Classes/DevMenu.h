/*
 * DevMenu.h
 *
 *  Created on: 04/04/2014
 *      Author: Administrador
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "utils/Utils.h"
#include "cocos2d.h"
USING_NS_CC;

#include "utils/events.h"
#include "menu/spinbox.h"

class DevMenu : public CCNode {
	CCMenuItemImage *bSkin, *bPlay;
	//void loadTextures();
	CCMenuItemImage *bPlayer[PLAYER_MAX];
	//CCMenuItemImage *bPSpeed[PLAYER_SPEEDS];
	//CCMenuItemImage *bGTimer[GAME_TIMERS];
	CCMenuItemImage *bModes[T_CANT+1];
	Event *e_spbchanged;
	CCMenu *menu;
	Spinbox *spins[T_CANT+1];//+1 for wave
	static void onSPBChanged(void* self, void* sender, void* param);
	void doWaveChange(const Spinbox *spb, const int &val);
public:
	Options myOptions;

	DevMenu();
	virtual ~DevMenu();

	void CreateMenu();//todo rename a load ¿?

	void onSkin(CCObject* pSender);
	void onMenuPlayer(CCObject* pSender);
	void onMenuMode(CCObject* pSender);
	void onMenuPlay(CCObject* pSender);
	void onBackToMenu(CCObject* pSender);
	//void onMenuPlayerSpeed(CCObject* pSender);
	//void onMenuGTime(CCObject* pSender);
	static void unload();
};

#endif /* MAINMENU_H_ */
