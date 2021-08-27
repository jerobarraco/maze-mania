/*
 * Utils.cpp
 *
 *  Created on: 27/03/2014
 *      Author: nande @ moongate
 */
#include "Utils.h"

Options OPTIONS;
Globals GLOBALS;

cocos2d::CCScene *sceneForLayer(cocos2d::CCLayer *lyr){
// there's no 'id' in cpp, so we recommend returning the class instance pointer
	//This is not a nice code, but is a good way to reuse the game object and not having to create a whole class for a simple scene
	//when we DO need a scene object (only case is when we need the onTransitionDidFinished) we can create a subclass
	//but anyway is better to NOT have anything on a scene object because is hard to reuse
	cocos2d::CCScene *sc = cocos2d::CCScene::create();
	sc->addChild(lyr);
	return sc;
}

char _buffitos[20];//20 chars ought to be enough for any number -- Bill Gates
char *itos(const int & i){
	snprintf(_buffitos, sizeof(_buffitos), "%d", i);
	return _buffitos;
}

Options::Options(){
	reset(false);
}

void Options::reset(bool base_only){
	//base_only=true (the default) makes this clear only the base parameters that changes from wave to wave
	//base_only=false resets all the attributes, like cplayer, points, wave, modes and penabled
	//debe ir al principio para permitir ser pisado
	for (us i = 0; i<20; i++)
		overflow[i] = 54;

	//por si se ponen directamente desde el devmenu por defecto arrancan desde el máximo.
	//TODO en vez de usar un boton de activado/desactivado habria que poner una dificultad

	points = 0;
	max_bombs = round(VAL4POS(T_BOMBS, 0));
	max_boosts = round(VAL4POS(T_BOOST, 0));
	max_ipoints = round(VAL4POS(T_IPOINTS, 0));
	units = round(VAL4POS(T_SWARM, 0));
	max_points = round(VAL4POS(T_MXPOINTS, 0));
	t_player = VAL4POS(T_SPEED, 0);//dur at level 0
	t_door = VAL4POS(T_DOOR, 0);
	t_game = VAL4POS(T_TIMER, 0);
	//VERY MUY IMPORTANT!!!!
	for (us i = 0; i<M_CANT; i++){
		if (!(base_only && i == M_WAVE)){//in base only, the wave should not be resetted
			modes[i] = false;
		}
	}
	if(base_only) return;

	modes[M_DEMO] = true;
	cplayers = 0;
	wave = 0;
	for (us i= 0; i<PLAYER_MAX; i++)
		penabled[i] = false;
}

void Options::setForWave(unsigned int pwave){
	/*
		automatically sets the difficulty for a certain level or wave
		if wave is omitted, the current one is used, in that case the difficulty won't be incremented
		just the params will be set

		wave mode is not set automatically!!
		also some attributes are not resetted (see this->reset)
	*/

	this->wave = pwave;
	if (wave>=LV_CANT){wave=LV_CANT-1;}
	//reset only the stuff we are going to replace anyway
	this->reset(true);

	//notice we dont set anything to false

	float mval;//value for that mode

	//yes i could put all this into a for, but this way is faster and not every one is the same
	//TIMER
	mval = POS4LVL(T_TIMER, wave);
	if(mval>0){
		modes[M_TIMER] = true;
		this->t_game = VAL4POS(T_TIMER, mval);
	}

	//AUTODOOR
	mval = POS4LVL(T_DOOR, wave);
	if(mval>0){
		modes[M_ADOORS] = true;
		this->t_door = round(VAL4POS(T_DOOR, mval));
	}

	//SPEED
	this->t_player = VAL4LVL(T_SPEED, wave);

	//MAXPOINTS (WaveTimer)
	this->max_points = round(VAL4LVL(T_MXPOINTS, wave));
	//wave mode is not set automatically!!

	//POINT ITEMS
	this->max_ipoints = VAL4LVL(T_IPOINTS, wave);

	//GOAL
	this->modes[M_GOAL] = VAL4LVL(T_GOAL, wave)>0;

	//BOOST
	this->max_boosts = round(VAL4LVL(T_BOOST, wave));

	//SWARM
	this->units = round(VAL4LVL(T_SWARM, wave));

	//BOMBS
	this->max_bombs = round(VAL4LVL(T_BOMBS, wave));
}
