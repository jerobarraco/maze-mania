/*
 * GameTimer.cpp
 *
 *  Created on: 15/04/2014
 *      Author: Administrador
 */

#include "items/GameTimer.h"
#include "utils/Utils.h"
#include "utils/Files.h"

static cus GT_MAXTX = 9;
static CCTexture2D *GT_TXS[GT_MAXTX] = {NULL};
static us _frames =1;//to avoid divide by zero
void GameTimer::load(){
	_frames = skinDef()->c_timer;
	preloadTXCache(_frames, files::game::timers, GT_TXS);

	step = (OPTIONS.t_game / _frames);//60fps
	this->setState(state);
	//Se llama una sola vez. (funcionObjetivo, delayFRAMES);
	this->schedule(schedule_selector(GameTimer::timeOut), step, _frames-1, step );
	e_game_over = EM.get(events::game::GAMEOVER);
}

void GameTimer::setState(cus nstate){
	if (nstate >= _frames) return;
	this->setTexture(GT_TXS[nstate]);
	this->state = nstate;
}

void GameTimer::timeOut(float f){
	CCLog("Time Out!!");
	//necesario hacerlo asi para evitar que se pase de rango pero también detectar que se paso
	int pstate = state +1;
	if(pstate<_frames){
		CCScaleTo *scale1 = CCScaleTo::create(0.2f, 1.25);
		CCScaleTo *scale2 = CCScaleTo::create(0.3f, 1.0);

		this->runAction(CCSequence::createWithTwoActions(scale1, scale2));
		play(files::game::s_timer);
		this->setState(pstate);
	}else{
		CCLog("Time out completo!!");
		play(files::game::s_timeout);
		int winner = GO_TIMEOUT;
		e_game_over->fire(this, &winner);
		this->state =0; //prevents multiple launchs
	}
}

void GameTimer::unload(){
	unloadSFX(files::game::s_timer);
	unloadSFX(files::game::s_timeout);
	//notice we set to NULL to ALL txs not just to _frames
	unloadTXCache(GT_MAXTX, files::game::timers, GT_TXS);
}

GameTimer::GameTimer(int xi, int yi):Item(b, xi, yi){
	state = 0;
	char *t = fullPath(files::game::timers[0]);
	this->initWithFile(t);//necesario
	//this->init();
	this->load();//load sets frames
}
