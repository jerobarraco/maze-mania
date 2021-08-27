/*
 * Door.cpp
 *
 *  Created on: 27/03/2014
 *      Author: nande
 */
#include "SimpleAudioEngine.h"

#include "Door.h"
#include "utils/Utils.h"
#include "utils/events.h"
#include "utils/Files.h"

USING_NS_CC;

static const short D_C_TXS = 3;
static CCTexture2D *D_TXS [D_C_TXS] = {NULL};
static const short D_HITBOX = SPRITE_RAD*0.75;

void Door::load(){
	/* load textures */
	preloadTXCache(D_C_TXS, files::game::doors, D_TXS);
}

//todo crear un unload que sea estatico
void Door::unload(){
	unloadSFX(files::game::s_doors[0]);
	unloadSFX(files::game::s_doors[1]);
	unloadTXCache(D_C_TXS, files::game::doors, D_TXS);
}

void Door::setState(DState nstate){
	if (nstate>D_C_TXS) return;
	this->setTexture(D_TXS[nstate]);
	this->state = nstate;
}

void Door::setType(int t){
	this->type = t;
	this->initWithFile(fullPath(files::game::doors[type]));
	this->setAnchorPoint(ccp(0.5, 0.5));
}

Door::Door(bool vertical = true, int type = 0, int xi=0, int yi=0):TouchSprite(0, true, D_HITBOX) {
	this->xi = xi;
	this->yi = yi;
	this->vertical = vertical;
	this->type = 0;
	this->active = true;
	this->state = DS_OPEN;
	this->load();
	this->setType(type);
	this->setState(type==2?DS_WALL:DS_OPEN);
	this->setRotation(vertical?0:90);
	//this->autorelease(); set by touchsprite
}

Door::~Door() {}

void Door::swapState(float f){
	bool is_open = this->state == DS_OPEN;
	DState ns = is_open?DS_CLOSED:DS_OPEN;
	this->setState(ns);
	play(files::game::s_doors[is_open?0:1]);//este deberia estar en setState?
}

void Door::setPaused(bool pause){
	this->active = !pause;
	if(pause){
		pauseSchedulerAndActions();
	}else{
		resumeSchedulerAndActions();
	}
}

bool Door::onTouch(){
	//CCLOG("%s", "door touched");
	if(!this->active) return true;//swallow anyway to not have slowdowns, if i do want to allow other touchs i need to put this to false
	if (this->type == DS_WALL){
		return false;
	}
	swapState();

	if(OPTIONS.modes[Options::M_ADOORS])
		this->scheduleOnce(schedule_selector(Door::swapState), OPTIONS.t_door);

	/*if (ac_rotate->isDone() || (ac_rotate->getElapsed()==0)){
		this->runAction(ac_rotate);
		Direction nd = rotateDirection(this->direction, -1);
		CCLog("direction from %d to %d", this->direction , nd);
		this->direction = nd;
		};*/
	//this->setRotation(this->getRotation()+5);
	return true;
}
