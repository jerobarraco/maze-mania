/*
 * StartMenu.cpp
 *
 *  Created on: 10/07/2014
 */

#include <StartMenu.h>
#include "SimpleAudioEngine.h"

#include "DevMenu.h"
#include "utils/events.h"
#include "utils/Files.h"
#include "menu/btnevent.h"
#include "menu/overlay.h"
#include "anim/animation.h"

static cus SM_BTNYBASE = 250;

void StartMenu::unload(){
	unloadTX(files::intro::logo);
	unloadTX(files::menu::music[0]);
	unloadTX(files::menu::music[1]);
	unloadTX(files::start_menu::back);
	unloadTX(files::menu::config);
	for (us i = 0; i< SM_C_DIF; i++){
		unloadTX(files::start_menu::difficulties[i]);
	}
	for (us i=0; i< PLAYER_MAX; i++){
		unloadTX(files::start_menu::pjIms[i]);
		unloadTX(files::start_menu::pjImsOn[i]);
	}
}

StartMenu::StartMenu():CCNode(){
	this->init();
	bAudio=bConfig=bIntro = NULL;
	this->setAnchorPoint(ccp(0.5f, 0.5f));
	this->setPosition(CCPointZero); //CCPointZero
	//this->loadTextures();
	this->CreateMenu();
	this->playBGM();
}

StartMenu::~StartMenu(){
	CCLog("start menu dead");
	//notar que los objetos no tienen retain
	this->removeAllChildren();//todo ver si es necesario esto
}

//todo rename a load
void StartMenu::CreateMenu(){
	for (us i = 0; i<SM_C_DIF; i++){
		bDifficulty[i] = NULL;
	}

	for (us i = 0; i<PLAYER_MAX; i++){
		bPlayer[i] = NULL;
	}

	BtnEvent *be = new BtnEvent(fullPath(files::start_menu::help), events::scenes::HELP);
	be->setPosition(ccp(GLOBALS.cent_x, GLOBALS.px_height-50));
	this->addChild(be);

	CreateDifficultyBtns();
	CreatePlayerBtns();
	CreateConfigBtns();

	// Logo animado
	Animation *logo = new Animation(files::start_menu::a_logo);
	logo->setPosition(ccp(GLOBALS.cent_x, GLOBALS.cent_y+100));
	this->addChild(logo);

	/* // Logo
	CCSprite* logo = CCSprite::create(fullPath(files::intro::logo));
	logo->setAnchorPoint(ccp(0.5, 0.5));
	int lx = GLOBALS.cent_x;
	int ly = ((GLOBALS.px_height - SM_BTNYBASE)/2)+SM_BTNYBASE;//GLOBALS.px_height -150;
	logo->setPosition(ccp(lx, ly+50));
	this->addChild(logo);
	CCFiniteTimeAction *ac_move = CCMoveTo::create(1, ccp(lx, ly));
	CCFiniteTimeAction *ac_fade = CCFadeIn::create(1);
	logo->runAction(ac_move);
	logo->runAction(ac_fade);
	*/	

	/*CCDrawNode *rectNode = CCDrawNode::create();
	rectNode->setAnchorPoint(ccp(0.5, 0.5));
	CCPoint verts[4];
	verts[0] = ccp(0, 0);
	verts[1] = ccp(0, GLOBALS.px_height);
	verts[2] = ccp(GLOBALS.px_width, GLOBALS.px_height);
	verts[3] = ccp(GLOBALS.px_width, 0);
	rectNode->drawPolygon(verts, 4, ccc4f(0.0,0.0,0.0,0.5), 1.0, ccc4f(1.0,1.0,1.0,1.0)) ;
	rectNode->setPosition(CCPointZero);
	this->addChild(rectNode,-1);*/
	this->addChild(new Overlay(), -1);

	CCMenu* menu = CCMenu::create(
		bDifficulty[0], bDifficulty[1], bDifficulty[2], bDifficulty[3],
		bPlayer[0], bPlayer[1], bPlayer[2], bPlayer[3], bPlayer[4], bPlayer[5],
		bAudio, bConfig, bIntro,
		NULL
	);

	menu->setPosition(CCPointZero);
	this->addChild(menu, 1);
}

void StartMenu::CreateDifficultyBtns(){
	CCPoint pos = ccp(GLOBALS.cent_x+175, SM_BTNYBASE);
	char * tx;
	// Play buttons
	for (us i = 0; i< SM_C_DIF; i++){
		tx = fullPath(files::start_menu::difficulties[i]);
		bDifficulty[i] = CCMenuItemImage::create(tx, tx, this, menu_selector(StartMenu::onGameDifficulty));
		bDifficulty[i]->setPosition(pos);
		bDifficulty[i]->setEnabled(false);
		bDifficulty[i]->setOpacity(128);
		pos = pos - ccp(0, 68); //ccp(0,80);
	}
}

void StartMenu::CreatePlayerBtns(){
	//CCPoint pos = ccp(myOptions.cent_x,myOptions.cent_y);
	//CCPoint pos = ccp(0,0);
	CCPoint pos = ccp(GLOBALS.cent_x-200, SM_BTNYBASE);
	CCPoint offs[] = { ccp(0, 0), ccp(100, 0), ccp(-50, -100), ccp(150, -100), ccp(0, -200), ccp(100, -200) };
	// Play buttons
	char *t;
	for (us i=0; i< PLAYER_MAX; i++){
		t = fullPath(files::start_menu::pjIms[i]);
		bPlayer[i] = CCMenuItemImage::create(t, t, this, menu_selector(StartMenu::onPlayer));
		bPlayer[i]->setPosition(pos+offs[i]);
	}
}

void StartMenu::CreateConfigBtns(){
	//CCPoint pos = ccp(GLOBALS.cent_x-150, GLOBALS.px_height*0.85);
	//CCPoint pos = ccp(GLOBALS.cent_x+80, GLOBALS.px_height*0.93);
	CCPoint pos = ccp(GLOBALS.px_width - 100, GLOBALS.px_height -50);
	char *t;
	int m = GLOBALS.audio?1:0;

	t = fullPath(files::menu::music[m]);
	bAudio = CCMenuItemImage::create(t, t, this, menu_selector(StartMenu::onAudio));
	bAudio->setAnchorPoint(ccp(0.5,0.5));
	bAudio->setPosition(pos);

	pos = ccp(100, GLOBALS.px_height -50);
	t = fullPath(files::start_menu::back);
	bIntro = CCMenuItemImage::create(t, t, this, menu_selector(StartMenu::onIntro));
	bIntro->setAnchorPoint(ccp(0.5,0.5));
	bIntro->setPosition(pos);

	pos = ccp(GLOBALS.cent_x-150, SM_BTNYBASE-100);//pos players
	t = fullPath(files::menu::config);
	bConfig = CCMenuItemImage::create(t, t, this, menu_selector(StartMenu::onConfig));
	bConfig->setAnchorPoint(ccp(0.5, 0.5));
	bConfig->setPosition(pos);
}

void StartMenu::playBGM(){
	play(files::menu::m_bgm, false, true, false);
}

void StartMenu::onGameDifficulty(CCObject * pSender){
	//TODO: guardar valores de cada dificultad en algun lado, si es un texto externo mejor
	//ver appdelegate default
	playSndBtn();

	//reset stuff

	//esto es importante porque el jugador puede apretar los diferentes botones de dificultad varias veces
	//y se le acumularian los efectos
	//update: eso es mentira, apenas toco el boton se va a jugar, y se destruye la instancia de
	//start_menu, la cual se recreará cuando se llame de nuevo, asi que no hay de que preocuparse
	//myOptions.reset();
	//myOptions.wave = 0;//wave inicial
	//myOptions.points = 0;//puntaje inicial
	//myOptions.modes[M_WAVE] = false; //just in case, as wave is the only to set this true we can put false here

	if (pSender == bDifficulty[0]){
		myOptions.setForWave(Options::LV_EASY);
	}else if (pSender == bDifficulty[1]){ // Easy
		myOptions.setForWave(Options::LV_NORMAL);
	}else if (pSender == bDifficulty[2]){ // Medium
		myOptions.setForWave(Options::LV_LUNATIC);
	}else if (pSender == bDifficulty[3]){	// Hard
		myOptions.setForWave(Options::LV_WZERO);
		myOptions.modes[myOptions.M_WAVE] = true; //importante colocarlo antes
	};
	myOptions.modes[myOptions.M_DEMO] = false;//make it for real!
	EM.get(events::scenes::PLAY)->fire(this, &myOptions, true);
}

void StartMenu::onPlayer(CCObject * pSender){
	playSndBtn();
	//bool any_selected = false;
	for (us i = 0; i<PLAYER_MAX; i++){
		if(pSender == bPlayer[i]){
			bool on = !myOptions.penabled[i];
			myOptions.penabled[i] = on;
			char *t = fullPath( on ? files::start_menu::pjImsOn[i] : files::start_menu::pjIms[i]);
			CCSprite *sprite = CCSprite::create(t);
			bPlayer[i]->setNormalImage(sprite);
			myOptions.cplayers += on?1:-1;
			//any_selected = any_selected || on; //if on == true -> anyselected = true
		}else{
			//any_selected = any_selected || myOptions.penabled[i]; //if enabled any = true
		}
	}

	for (us i = 0; i<SM_C_DIF; i++){
		CCMenuItemImage *btn = bDifficulty[i];
		if(btn){
			btn->setEnabled(false);
			btn->setOpacity(128);
		}
	}
	if (myOptions.cplayers ==1)	{
		CCMenuItemImage *btn = bDifficulty[3];
		btn->setEnabled(true);
		btn->setOpacity(255);
	}

	if (myOptions.cplayers >= 2){
		for (us i=0; i<4; i++){
			CCMenuItemImage *btn = bDifficulty[i];
			btn->setEnabled(true);
			btn->setOpacity(255);
		}
	}
}

void StartMenu::onConfig(CCObject * pSender){
	playSndBtn();
	EM.get(events::menu::DEV_MENU)->fire(this, NULL, true);
}

void StartMenu::onAudio(CCObject * pSender){
	playSndBtn();
	//Sound off_on
	GLOBALS.audio = !GLOBALS.audio;
	char *t = fullPath( GLOBALS.audio? files::menu::music[1] : files::menu::music[0]);
	CCSprite *sprite = CCSprite::create(t);
	bAudio->setNormalImage(sprite);
	if(!GLOBALS.audio){
		pauseAudio();
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}else{
		resumeAudio();
		playBGM();
	}
}
void StartMenu::onIntro(CCObject * pSender){
	playSndBtn();
	EM.get(events::scenes::INTRO)->fire(this, NULL, true);
}
