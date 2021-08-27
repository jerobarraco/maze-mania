/*
 * Player.cpp
 *
 *  Created on: 27/03/2014
 *      Author: nande
 */
#include <math.h>//si no anda usar "math.h" en vez de <math.h>

#include "Player.h"
#include "utils/Files.h"

void Player::setPaused(bool pause){
	if(pause){
		if (alive){//alive para que no corte las animaciones del die
			active = false;
			pauseSchedulerAndActions();
		}
	}else{
		active = alive;
		resumeSchedulerAndActions();
	}
}

void Player::die(){
	//this->setPaused(true); //this will pause the other actions too
	this->stopAllActions(); //i could use stopActionByTag but when i create the action i need to do setTag

	alive = false;

	e_player_died->fire(this, this);

	CCFiniteTimeAction *ac_scale = CCScaleTo::create(0.3f, 1.8f);
	CCFiniteTimeAction *ac_tint = CCTintTo::create(0.3f, 128, 128, 128); //tenia rojo pero iba a quedar muy shockeante (??)

	CCFiniteTimeAction *ac_release = CCCallFunc::create(this, callfunc_selector(Player::removeFromParent));
	this->runAction(CCFadeTo::create(0.3f, 0)); //tenia rojo pero iba a quedar muy shockeante (??)
	this->runAction(ac_tint);
	this->runAction(CCSequence::create(ac_scale, ac_release, NULL));
	//CCFiniteTimeAction *ac_fade = CCFadeOut::create(0.5);
	//CCFiniteTimeAction *ac_release = CCCallFunc::create(sender, callfunc_selector(Item::removeFromParent));
	//sender->runAction(CCSequence::create(ac_fade, ac_release, NULL));
}

bool Player::atCenter(){
	return (xi %2 == 1) && (yi %2 == 1);
}

void Player::checkCenter(){
	if(blocked){
		//if the last
		this->direction = rotateDirection(this->direction, -1);
	}else{
		//keep walking fordward, so , i do nothing
	}
	blocked = false;//reset
}

void Player::checkDoor(){
	Door *d = this->b->doorAt(this->xi, this->yi);
	if (!d) return;
	//CCLog("I have a door and its %d,%d", d->xi, d->yi);
	if(d->state == DS_CLOSED || d->type == DS_WALL){
		//CCLog(" - Door's closed");
		this->direction = rotateDirection(this->direction, 2);
		blocked = true;
	}
}

void Player::setCoord(int xi, int yi){
	this->nxi = this->xi = xi;
	this->nyi = this->yi = yi;
	this->setPosition(this->b->coordToPos(xi, yi));
}

void Player::setSpeedFactor(const float f){
	this->speedf = f;

	//animate the factor
	bool up = f<1.0;
	CCFiniteTimeAction *ac;
	if (up){
		bool hor = this->direction == LEFT || this->direction == RIGHT;
		float sx = hor? 1.5:0.8, sy = hor? 0.8:1.5;
		ac = CCSequence::create(CCScaleTo::create(0.25, sx, sy), CCScaleTo::create(0.25, 1, 1), NULL) ;
	}else{
		ac = CCRotateBy::create(0.75, 1080);
	}
	this->runAction(ac);
}

void Player::checkStep(){
	if (!this->active){
		return;
	}
	xi = nxi;
	yi = nyi;
	//very important to activate the items before moving and stuff
	this->b->checkItem(this, xi, yi);
	if (!(this->b->active && this->alive)){
		return;
	}

	if(this->atCenter()){
		this->checkCenter();
	}else{
		this->checkDoor();
	};

	switch (direction){
		case RIGHT:
			nxi++;
			//the commented code is cool but criptic and also doesnt deal with some issue
			//nxi %= this->b->tw;//wraps :)
			if ( nxi > this->b->tw*2){
				//wraps :)
				nxi = 1;
				xi = 0;//teleports him down
				this->setPosition(this->b->coordToPos(xi, yi));
			}
			break;
		case LEFT:
			nxi--;
			if ( nxi < 0){
				nxi = (this->b->tw*2)-1;
				xi = (this->b->tw*2);//teleports him
				this->setPosition(this->b->coordToPos(xi, yi));
			}
			break;
		case UP:
			nyi++;
			if ( nyi > this->b->th*2){
				//wraps :)
				nyi = 1;
				yi = 0;//teleports him down
				this->setPosition(this->b->coordToPos(xi, yi));
			}
			break;
		case DOWN:
			nyi--;
			if ( nyi < 0){
				nyi = (this->b->th*2)-1;
				yi = (this->b->th*2);//teleports him
				this->setPosition(this->b->coordToPos(xi, yi));
			}
			break;
		case CENTER: break;
	}

	CCPoint pos = this->b->coordToPos(nxi, nyi);

	/* Velocidades random por paso
	float random = (float) rand() / (float) RAND_MAX;
	float b = random / 10;
	float ps = PSPEEDS_DUR[OPTIONS.pspeed] * (0.9+b);
	CCFiniteTimeAction* ac_move = CCMoveTo::create(ps , pos);
	*/
	//apply speed factor
	//double speed = PSPEEDS_DUR[OPTIONS.pspeed] * speedf;
	double speed = OPTIONS.t_player* speedf;
	double dif = 1.0 -speedf;
	double am = (dif<0) ? -dif: dif;//absolute dif
	if(am>0.01){
		if(speedf>1.0f){
			//speedf -= 1-am;
			speedf -= 0.1f;
		}else{
			speedf += 0.1f;
		}
	}else{
		speedf = 1.0f;
	}


	CCFiniteTimeAction* ac_move = CCMoveTo::create(speed, pos);
	CCFiniteTimeAction* ac_move_done = CCCallFunc::create(this, callfunc_selector(Player::checkStep) );
	runAction( CCSequence::create(ac_move, ac_move_done, NULL) );
}

//CCSprite does NOT set autorelease,
Player::Player(Board *board, int team, int xi, int yi, Direction dir):CCSprite() {
	this->team = team;
	this->b = board;
	blocked = false;
	active=alive = true;
	this->speedf = 1.0;
	this->score = 0;

	char *t = fullPath(files::game::players[team]);
	this->initWithFile(t);
	this->autorelease();
	this->prev = CENTER;
	this->direction = dir;
	//this->setScale(0.8);
	//int rDir = rand() %4;
	//this->direction = (Direction)rDir;
	this->setCoord(xi, yi);
	this->checkStep();

	e_player_died = EM.get(events::game::PLAYER_DIED);
}

Player::~Player() {
	EVRELEASE(e_player_died)
}
