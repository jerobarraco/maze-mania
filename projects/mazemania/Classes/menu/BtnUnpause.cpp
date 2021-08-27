/*
 * BtnUnpause.cpp
 *
 *  Created on: 22/04/2014
 *      Author: Administrador
 */

#include "BtnUnpause.h"
#include "utils/Utils.h"
#include "utils/Files.h"

USING_NS_CC;

static const short BU_HITBOX = SPRITE_RAD*0.75;

BtnUnpause::BtnUnpause():TouchSprite(fullPath(files::gameover::resume)){
	this->setAnchorPoint(ccp(0.5, 0.5));
}

bool BtnUnpause::onTouch(){
	EM.get(events::game::RESUME)->fire(this, NULL, true);
	return true;
}
