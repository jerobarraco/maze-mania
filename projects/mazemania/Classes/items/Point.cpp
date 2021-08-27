/*
 * Point.cpp
 *
 *  Created on: 24/09/2014
 *      Author: nande
 */
//#include <stdlib.h>     /* srand, rand */
#include "SimpleAudioEngine.h"
#include "Speed.h"
#include "utils/Utils.h"
#include "utils/Files.h"
#include "Point.h"

Point::Point(Board *b, int xi, int yi):Item(b, xi, yi){
	//this->ammount = (rand()/(float)RAND_MAX)>0.5;
	this->ammount = 50;
	preloadAudio(files::game::s_boosts[0]);

	char *t = fullPath(files::game::point);
	this->initWithFile(t);

	e_activated = EM.get(events::game::items::POINT);
}

void Point::activate(Player* p){
	play(files::game::s_point);
	p->score += this->ammount;
	CCLog("POINT ITEM! Player->score: %d", p->score);
	CCFiniteTimeAction *ac_scale = CCScaleTo::create(0.5, 1.5);
	CCFiniteTimeAction *ac_release = CCCallFunc::create(this, callfunc_selector(Point::removeFromParent));
	this->runAction(CCFadeOut::create(0.5));
	this->runAction(CCSequence::create(ac_scale, ac_release, NULL));

	e_activated->fire(this, &this->ammount);
}

void Point::unload(){
	unloadSFX(files::game::s_boosts[0]);
}
