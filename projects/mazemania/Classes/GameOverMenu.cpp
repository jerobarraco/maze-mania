/*
 * GameOverMenu.cpp
 *
 *  Created on: 09/04/2014
 *      Author: Administrador
 */
#include "SimpleAudioEngine.h"

#include "utils/Files.h"
#include "GameOverMenu.h"
#include "menu/BtnUnpause.h"
#include "menu/overlay.h"
#include "scenes/game.h"

const char *_wtxs[] = {files::gameover::wave, files::gameover::points, files::gameover::time, files::gameover::clicks};
const char *_txs[C_LABELTXS] = {files::gameover::pause, files::gameover::timeout, files::gameover::gameover};
const char *_lbanims[] = {files::gameover::a_winner, files::gameover::a_cleared, files::gameover::a_finished};

void GameOverMenu::onShowHelp(void *self, void *sender, void *param){
	GameOverMenu* diz = (GameOverMenu*) self;
	diz->addChild(diz->help, 50);
	CCFadeIn *fi = CCFadeIn::create(0.5f);
	diz->help->runAction(fi);
}

void GameOverMenu::onHideHelp(void *self, void *sender, void *param){
	GameOverMenu* diz = (GameOverMenu*) self;
	diz->removeChild(diz->help);
}

GameOverMenu::GameOverMenu():CCNode(){
	this->init();
	this->autorelease();//must be set manually
	this->setAnchorPoint(ccp(0.5f, 0.5f));
	this->setPosition(CCPointZero); //CCPointZero
	btnResume = NULL;
	btnHelp = NULL;
	help = NULL;
	label = NULL;
	player = NULL;
	a_label = NULL;
	e_help = e_hback = NULL;
	for (us i=0; i<C_LABELTXS; i++){
		label_txs[i] = NULL;
	}
	for (us i=0; i<C_WAVELBLS; i++){
		wave_lbls[i] = NULL;
		wave_vals[i] = NULL;
	}
	this->load();
}

GameOverMenu::~GameOverMenu() {
	CCLOG("game over menu dying");
	EVRELEASE(e_help);
	EVRELEASE(e_hback);
	RELEASE(label)
	RELEASE(player)
	RELEASE(a_label)
	//RELEASE(player)
	RELEASE(btnHelp);
	RELEASE(help);
	for (us i=0; i<C_LABELTXS; i++){
		RELEASE(label_txs[i])
	}

	for (us i=0; i<C_WAVELBLS;i++){
		RERELEASE(wave_lbls[i])
		RERELEASE(wave_vals[i])
	}

	this->removeAllChildren();
}

void GameOverMenu::unload(){
	unloadTX(files::gameover::cleared);
	unloadTX(files::gameover::bg);
	unloadTX(files::menu::help);
	//TODO agregar fonts
	for (us i=0; i<C_WAVELBLS; i++){
		unloadTX(_wtxs[i]);
	}
	for (us i=0; i<C_LABELTXS; i++){
		unloadTX(_txs[i]);
	}
}

void GameOverMenu::load(){
	CCLOG("Loading gom");
	int steps = 9;
	EM.get(events::loader::START)->fire(this, &steps, true);
	Event *estep =  EM.get(events::loader::STEP);

	estep->fire(this, NULL);
	//the following preload helps to avoid the hippuc when the match finishes.
	//as the cocos2d engine SUCKS there's no way to unload the audio.
	//I have no idea but i think it could eat up the ram if i preload too much stuff... but i have no idea yet
	//Anyway this is the only one that i preload, and i preload it only if "it's playing" the game,
	//that way there's a high chance that the gameover screen will show, playing this music,
	//and after that another music will play, unloading this.
	//BUT if the player pauses, and then goes to the menu, chances are this will still be preloaded... too bad
	//as is the only preloaded audio, the damage is not too much
	//TODO update to cocos 3 or implement another engine
	preloadAudio(files::game::m_gameover, false);

	estep->fire(this, NULL);
	//preload labels
	for (us i=0; i<C_LABELTXS; i++){
		label_txs[i] = preloadTX(_txs[i]);
		label_txs[i]->retain();
	}

	estep->fire(this, NULL);
	//preconstruimos los labels y labels de valores para los waves
	if(OPTIONS.modes[OPTIONS.M_WAVE]){
		CCPoint pos = ccp(GLOBALS.cent_x, GLOBALS.cent_y+25);
		CCPoint vpos = pos;
		vpos.x += 20;
		vpos.y += 8;

		CCSprite *lbl = NULL;
		CCLabelBMFont *lval = NULL;

		for (us i = 0; i<C_WAVELBLS; i++){
			//label label
			lbl = CCSprite::create(fullPath(_wtxs[i]));
			lbl->setAnchorPoint(ccp(0.5,0.0));
			lbl->setPosition(pos);
			lbl->retain();
			wave_lbls[i] = lbl;

			//label value
			lval = CCLabelBMFont::create("0", fullPath(files::com::f_com));
			lval->setAnchorPoint(ccp(0.0, 0.0));
			lval->setPosition(vpos);
			lval->retain();
			wave_vals[i] = lval;

			pos.y -= 75;
			vpos.y -= 75;
		}
	}

	CCPoint center = ccp(GLOBALS.cent_x, GLOBALS.cent_y);

	//nodes
	estep->fire(this, NULL);
	this->addChild(new Overlay(), -1);

	//background
	CCSprite *bg = CCSprite::create(fullPath(files::gameover::bg));
	addChild(bg);
	bg->setPosition(center);//has the anchor on center right?

	estep->fire(this, NULL);
	btnResume = new BtnUnpause();
	btnResume->setPosition(center+ccp(0, -140));//(center + ccp(-140, -140));
	btnResume->retain();

	estep->fire(this, NULL);
	btnHelp = new BtnEvent(fullPath(files::menu::help), events::game::HELP);
	btnHelp->setPosition(ccp(GLOBALS.cent_x+0, GLOBALS.cent_y-25));//beware, it uses the same place as the player button when it wins, only use this button on pause
	btnHelp->retain();

	estep->fire(this, NULL);
	help = new Help(false);
	help->retain();
	help->load();

	estep->fire(this, NULL);
	label = CCSprite::create(fullPath(_txs[0]));//we have to create it with some texture in it
	label->retain();
	label->setPosition(center+ccp(0, 120));
	addChild(label);

	estep->fire(this, NULL);
	//buttons

	BtnEvent *be = new BtnEvent(fullPath(files::menu::replay), events::game::RESTART_WAVE);
	be->setPosition(center + ccp(140, -140));
	this->addChild(be);

	be = new BtnEvent(fullPath(files::menu::menu), events::scenes::MENU);
	be->setPosition(center + ccp(-140, -140));
	this->addChild(be);

	e_help = EM.subscribe(this, events::game::HELP, GameOverMenu::onShowHelp);
	e_hback = EM.subscribe(this, events::menu::BACK, GameOverMenu::onHideHelp);
	EVRELEASE(estep)
	EM.get(events::loader::STOP)->fire(this, NULL, true);
}

void GameOverMenu::show(int winner){
	//in case winner==GO_WAVEDONE wave_n is the wave number

	// QUITAR
	//importante primero quitar las cosas. recordar que el gom puede mostrarse varias veces,
	//o sea que puede haber quedado con cosas anteriores
	this->removeChild(btnResume);//just in case
	this->removeChild(btnHelp);
	this->removeChild(label);

	//remove player if it was already shown (this happens for example in a replay)
	//we need to make the right thing
	RERELEASE(player)
	RERELEASE(a_label)

	// LABELS
	//lo hacemos en este orden porque no siempre los vamos a querer re-agregar
	//(lo cual dispara un assert y hace que todo crashee
	//(porque para los chinos es mejor crashear que imprimir un warning y retornar))
	for (us i=0; i<C_WAVELBLS;i++){
		if(wave_lbls[i]){
			//quitarlos primero
			removeChild(wave_lbls[i]);
			removeChild(wave_vals[i]);
			if (winner==GO_WAVEOVER){
				//y agregarlos solo de ser necesario
				addChild(wave_lbls[i]);
				addChild(wave_vals[i]);
			}
		}
	}

	//music and main label
	bool music = true;
	short txn = 0;
	//TODO cambiar todos a animaciones
	switch(winner){
		case GO_PAUSE:
			//UnPause Button
			this->addChild(btnResume);
			this->addChild(btnHelp);
			//notar que el boton btnHelp aparece SOLO cuando se pausa, por eso ocupa el lugar del centro, donde sale el winner. cuidado con eso!
			//TODO hacer que el boton de ayuda aparezca siempre?
			txn = 0;
			music = false;
			break;
		case GO_TIMEOUT:
			txn = 1;
			break;
		case GO_GAMEOVER:
			txn = 2;
			break;
		case GO_WAVEOVER:
			CCLOG("Wave %d finished", OPTIONS.wave);
			txn = -3;//termino el wave

			if(OPTIONS.wave < OPTIONS.LV_CANT-1){//si NO terminó el último wave
				txn = -2;
				this->addChild(btnResume);//este boton NO debe estar en game_over ni time_out
			}
			//int wavenum = OPTIONS.wave - OPTIONS.LV_WZERO +1;//si defino la variable tengo que crear un scope y no me dan ganas http://stackoverflow.com/a/5685578/260242
			wave_vals[0]->setString(itos(OPTIONS.wave - OPTIONS.LV_WZERO +1));
			wave_vals[1]->setString(itos(OPTIONS.points));
			break;
		default://somebody actually won
			txn = -1;
			//create player image and store it to remove it on next show
			player = new Animation(files::gameover::a_players[winner]);
			//player = CCSprite::create(fullPath(files::menu::pjImsOn[winner]));//usa los ON para realzar más
			player->setPosition(ccp(GLOBALS.cent_x+0, GLOBALS.cent_y-25));
			//player->retain();//so we can remove it if show gets called again
			addChild(player);
			player->retain();
	}

	if(txn>=0){
		label->setTexture(label_txs[txn]);
		this->addChild(label);
	}else{
		a_label = new Animation(_lbanims[-(txn+1)]);
		a_label->retain();
		a_label->setPosition(ccp(GLOBALS.cent_x, GLOBALS.cent_y+120));
		this->addChild(a_label);
	}

	if (music){
		// Musica GameOver
		//el pause del audio se maneja en el board, que esta subscripto a ambos eventos
		play(files::game::m_gameover, false, false);
	}
}
