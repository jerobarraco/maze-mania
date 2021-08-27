/*
 * Intro.h
 *
 *  Created on: 02/10/2014
 *      Author: Administrador
 */

#ifndef INTRO_H_
#define INTRO_H_

#include "cocos2d.h"
#include "loader.h"
USING_NS_CC;

const unsigned short AB_C_SOCIAL = 4;
class Intro: public LoadLayer{
	CCMenuItemImage *bPlay, *bAudio, *bAbout, *bExit;
public:
	Intro():LoadLayer(true){
		bPlay = NULL;
		bAudio = NULL;
		bAbout = NULL;
		bExit = NULL;
	}
	void load();
	static void unload();
	void onPlay(CCObject *sender);
	void playBGM();
	void onAudio(CCObject *sender);
	void onAbout(CCObject *sender);
	void onExit(CCObject *sender);
	~Intro();
	void keyBackClicked(void);
};

#endif /* INTRO_H_ */
