/*
 * Bomb.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: nande
 */

#include "items/Bomb.h"
#include "Player.h"
#include "Board.h"
#include "utils/Files.h"

void Bomb::activate(Player *p){
	CCLog("un player cago %i", (long) p);
	play(files::game::s_bomb);

	if(p){
		p->die();
	}

	this->runAction( CCFadeOut::create(0.5) );
	this->runAction( CCTintTo::create(0.3f, 255, 0, 0) );

	CCFiniteTimeAction *ac_scale = CCScaleTo::create(0.3f, 1.5);
	CCFiniteTimeAction *ac_release = CCCallFunc::create(this, callfunc_selector(Bomb::removeFromParent));
	this->runAction(CCSequence::create(ac_scale, ac_release, NULL));

	e_activated->fire(this, p);
}

Bomb::~Bomb(){
	EVRELEASE(e_activated)
}

void Bomb::unload(){
	unloadSFX(files::game::s_bomb);
	unloadTX(files::game::bomb);
}

Bomb::Bomb(Board*b, int xi, int yi):Item(b, xi, yi){
	preloadAudio(files::game::s_bomb);

	char *t = fullPath(files::game::bomb);
	this->initWithFile(t);
	e_activated = EM.get(events::game::items::BOMB);
}
