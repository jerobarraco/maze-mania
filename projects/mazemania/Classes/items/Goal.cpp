/*
 * Goal.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: nande
 */
#include "SimpleAudioEngine.h"

#include "utils/Files.h"
#include "items/Goal.h"

Goal::Goal(Board*b, int xi, int yi):Item(b, xi, yi){
	preloadAudio(files::game::s_goal);

	char *t = fullPath(files::game::goal);
	this->initWithFile(t);
	e_game_over = EM.get(events::game::GAMEOVER);
	e_points = EM.get(events::game::items::POINT);
}

void Goal::activate(Player *p){
	play(files::game::s_goal);

	CCFiniteTimeAction *ac_scale = CCScaleTo::create(0.5, 1.5);
	CCFiniteTimeAction *ac_release = CCCallFunc::create(this, callfunc_selector(Goal::removeFromParent));
	this->runAction(CCFadeOut::create(0.5));
	this->runAction(CCSequence::create(ac_scale, ac_release, NULL));

	if(OPTIONS.modes[OPTIONS.M_WAVE]){
		int fp=(OPTIONS.max_points/2);//para que no sea demasiado zarpado
		e_points->fire(this, &fp);
	}else{
		e_game_over->fire(this, &p->team);
	}
}

Goal::~Goal(){
	EVRELEASE(e_game_over)
	EVRELEASE(e_points)
}

void Goal::unload(){
	unloadSFX(files::game::s_goal);
}
