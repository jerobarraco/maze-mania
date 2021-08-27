/*
 * Item.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: nande
 */

#include "Item.h"
#include "Board.h"
#include "utils/Utils.h"

//TODO cambiar el coordToPos a un global asi no dependemos mas del board
Item::Item(Board*b, int xi, int yi):CCSprite(){
	//items can preload fx (not mandatory) upon creation but should not unload it on destruction, because another instance can be created
	this->autorelease();
	this->xi =xi;
	this->yi=yi;
	this->b = b;
	this->setPosition(this->b->coordToPos(this->xi, this->yi));//todo quitar esto, pasar a board y quitar referencia a board
	this->active = true;
}

Item::~Item() {
//	CCLOG("item dead");
}

void Item::setPaused(bool pause){
	//si esta activo y se quiere despausar o viceversa (o sea si quiero pausar y ya esta pausado) no hago nada
	if (this->active != pause){
		CCLog("logic error, pausing a paused or resuming a resumed item");
		return;
	}
	this->active = !pause;
	if(pause){
		pauseSchedulerAndActions();
		CCLog("item paused");
	}else{
		resumeSchedulerAndActions();
		CCLog("item unpaused");
	}
}

void Item::unload(){
//here it should unload the resources it uses, used because each item can load different assests
//and we unload the because not all the possible boards have the same possible items
}
