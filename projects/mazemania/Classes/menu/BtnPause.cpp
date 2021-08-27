/*
 * BtnPause.cpp
 *
 *  Created on: 22/04/2014
 *      Author: Administrador
 */

#include "BtnPause.h"

#include "utils/Utils.h"
#include "utils/Files.h"
USING_NS_CC;

static const short BP_HITBOX = SPRITE_RAD*0.75;

BtnPause::BtnPause():TouchSprite(fullPath(files::com::pause)){
	this->setAnchorPoint(ccp(0.5, 0.5));
	ev = EM.get(events::game::PAUSE);
}

BtnPause::~BtnPause(){
	EVRELEASE(ev)
}

bool BtnPause::onTouch(){
	CCLOG("btn_paused touched");
	ev->fire(this, NULL);
	return true;
}
