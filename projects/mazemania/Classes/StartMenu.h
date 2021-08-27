/*
 * StartMenu.h
 *
 *  Created on: 10/07/2014
 *      Author: Administrador
 */

#ifndef STARTMENU_H_
#define STARTMENU_H_

#include "cocos2d.h"

#include "utils/Utils.h"
USING_NS_CC;

static const unsigned short SM_C_DIF = 4; //TODO las constantes estas que estan relacionadas con los archivos deberian estar en file directamente
class StartMenu: public CCNode {
	CCMenuItemImage *bDifficulty[SM_C_DIF];
	CCMenuItemImage *bPlayer[PLAYER_MAX];
	CCMenuItemImage *bAudio, *bConfig, *bIntro;
	Options myOptions;
	//void loadTextures();//unused
public:
	static void unload();
	StartMenu();
	virtual ~StartMenu();
	void CreateMenu();

	void CreateDifficultyBtns();
	void CreatePlayerBtns();
	void CreateSocialBtns();
	void CreateConfigBtns();
	void playBGM();

	void onAudio(CCObject *pSender);
	void onIntro(CCObject *pSender);
	void onGameDifficulty(CCObject *pSender);
	void onPlayer(CCObject *pSender);
	void onConfig(CCObject *pSender);
};

#endif /* STARTMENU_H_ */
