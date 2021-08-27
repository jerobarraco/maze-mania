/*
 * DevMenu.cpp
 *  Created on: 04/04/2014
 */
#include "SimpleAudioEngine.h"
#include "DevMenu.h"
#include "utils/events.h"
#include "utils/Files.h"
#include "menu/spinbox.h"
#include "menu/overlay.h"
#include "scenes/game.h"

#include "utils/Utils.h"
#include "anim/animation.h"

static cus GAME_TIMERS = 10;
static cus PLAYER_SPEEDS = 10;
static cus DM_SPMAX = 10;
static cus MODE4THING[]={5,8,7,0,3,4,6,2,1,9};
/*
 * char *t = fullPath( GLOBALS.audio? files::menu::music[1] : files::menu::music[0]);
	CCSprite *sprite = CCSprite::create(t);*/
void DevMenu::onSPBChanged(void *self, void *sender, void *param){
	//CCLOG("SPbchanged new val is %d", *(int*)param);
	((DevMenu*)self)->doWaveChange((Spinbox*) sender, *(int*)param);
}

void DevMenu::doWaveChange(const Spinbox *spb, const int &val){
	//todo checkear cual esta puesto y setear el modo segun eso
	if(spb!=this->spins[T_CANT]) return;

	this->myOptions.setForWave(val);
	for (us i = T_INI; i<T_CANT; i++){
		this->spins[i]->setValue(round(POS4LVL(i, val)*DM_SPMAX));
	}

	myOptions.modes[myOptions.M_WAVE] = val==0?false:true;

}

DevMenu::DevMenu():CCNode(){//automatically sets the autorelease
	this->init();
	this->setAnchorPoint(ccp(0.5f, 0.5f));
	this->setPosition(CCPointZero); // CCPointZero
	//this->loadTextures();
	bPlay = bSkin = NULL;
	for (us i=T_INI; i<=T_CANT; i++){
		spins[i] = NULL;
		bModes[i] = NULL;
	}
	this->CreateMenu();
}

DevMenu::~DevMenu() {
	CCLog("main menu dead");
	EVUNSUB(e_spbchanged)
	for (us i =T_INI; i<=T_CANT; i++){//cuidado con la verif, el wave es uno mas. o sea que la condicion tiene que ser i<=T_CANT o i<T_CANT+1
		RELEASE(spins[i]);
		RELEASE(bModes[i]);
	}
	RELEASE(menu)
	this->removeAllChildren();
	//todo hacer retain y release aca de los otros objetos
}

void DevMenu::CreateMenu(){
	//TODO pasar options como parametro
	/*CCDrawNode* rectNode = CCDrawNode::create();
	rectNode->setAnchorPoint(ccp(0.5, 0.5));
	CCPoint verts[4];
	verts[0] = ccp(0, 0);
	verts[1] = ccp(0, GLOBALS.px_height);
	verts[2] = ccp(GLOBALS.px_width, GLOBALS.px_height);
	verts[3] = ccp(GLOBALS.px_width, 0);

	SkinDef *sd = skinDef();
	rectNode->drawPolygon(verts, 4, sd->overlay, 1.0, sd->boardBorder) ;//drawPolygon(verts, 4, ccc4f(1.0,1.0,1.0,0.5), 1.0, ccc4f(1.0,1.0,1.0,1.0)) ;
	rectNode->setPosition(CCPointZero);
	this->addChild(rectNode,-1);*/
	this->addChild(new Overlay(), -1);

	Animation *title = new Animation(files::menu::a_title);
	title->setPosition(ccp(GLOBALS.cent_x, GLOBALS.cent_y+250));
	this->addChild(title);

	char *tx; //textura

	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	// TODO usah el globals.cent
	CCPoint center = ccp(//centro HORIZONTAL!(no vertical)
		origin.x + (GLOBALS.px_width/2),
		origin.y
	);
	CCPoint pos = center + ccp(-((3*80)+40), + 100);

	CCSprite* spr_players = CCSprite::create(fullPath(files::menu::players_icon));
	spr_players->setPosition(pos); //add 20px spacing
	pos.x += 90; //80+10
	addChild(spr_players);
	for (us i = 0 ; i<PLAYER_MAX ; i++){
		tx = fullPath(files::menu::pjIms[i]);
		bPlayer[i] = CCMenuItemImage::create(tx, tx, this, menu_selector(DevMenu::onMenuPlayer));
		bPlayer[i]->setPosition(pos);
		pos = pos + ccp(80, 0);
	}

	// Play MenuItem
	//pos = ccp(OPTIONS.px_width-100, OPTIONS.px_height-100);
	//pos.x+=10;
	tx = fullPath(files::menu::play);
	bPlay = CCMenuItemImage::create(tx, tx, this, menu_selector(DevMenu::onMenuPlay));
	bPlay->setPosition(pos);
	bPlay->setEnabled(false);
	bPlay->setOpacity(128);

	tx = fullPath(files::menu::menu);
	CCMenuItemImage *bMenu = CCMenuItemImage::create(tx, tx, this, menu_selector(DevMenu::onBackToMenu));
	bMenu->setAnchorPoint(ccp(0, 0));
	bMenu->setPosition(ccp(20, GLOBALS.px_height-100));

	tx = fullPath(files::menu::skin);
	bSkin = CCMenuItemImage::create(tx, tx, this, menu_selector(DevMenu::onSkin));
	bSkin->setAnchorPoint(ccp(0, 0));
	bSkin->setPosition(ccp(GLOBALS.px_width-20-64, GLOBALS.px_height-100));

	// agregar los items creados al menú
	menu = CCMenu::create(
			//bAbout,
			bMenu, bPlay,
			bPlayer[0], bPlayer[1], bPlayer[2], bPlayer[3], bPlayer[4], bPlayer[5],
			bSkin,
			NULL);
	menu->setPosition(CCPointZero);
	menu->retain();
	this->addChild(menu, 1);

	// Menu Mode
	pos = center + ccp(-((80*3)+40), 460);
	tx= fullPath(files::menu::mode);
	CCSprite* spr_modes = CCSprite::create(tx);
	spr_modes->setPosition(pos);
	addChild(spr_modes);
	pos.x+=90;
	us txn;
	for (us i = 0 ; i<=T_CANT; i++){
		txn = MODE4THING[i];
		tx = fullPath(i==0?files::menu::modes_on[txn] : files::menu::modes[txn]);
		bModes[i] = CCMenuItemImage::create(tx, tx, this, menu_selector(DevMenu::onMenuMode));
		bModes[i]->setPosition(pos);
		bModes[i]->retain();
		menu->addChild(bModes[i]);
		pos = pos + ccp(80, 0); //has not implemented operator+= (y)
		if (i == 5){
			pos = pos + ccp(-400, -100);
		}
		//bModes[i]->setOpacity(128);
	}
	//Spinboxes
	pos = center + ccp(0, 260); //center + ccp(0, 320);
	//T_SPEED, T_MXPOINTS, T_IPOINTS, T_GOAL, T_TIMER, T_BOOST, T_DOOR, T_SWARM, T_BOMBS, T_CANT};
	int mx;
	for (us i = T_INI; i<T_CANT; i++){
		mx = i==T_GOAL? 1: DM_SPMAX; //max, is 1 in case of goal, the rest is DM_SPMAX
		spins[i] = new Spinbox(menu, pos, mx);
		spins[i]->retain();
		spins[i]->removeFromParent();
	}
	spins[T_SPEED]->addTo(menu);

	spins[T_CANT] = new Spinbox(menu, pos, Options::LV_CANT-1);//wave spinbox is the only different
	spins[T_CANT]->retain();
	spins[T_CANT]->removeFromParent();
	e_spbchanged = EM.subscribe(this, events::com::SPB_CHANGED, DevMenu::onSPBChanged);
}

void DevMenu::onSkin(CCObject* pSender){
	playSndBtn();
	int nskin = GLOBALS.skin +1;
	EM.get(events::app::RELOAD)->fire(this, &nskin, true);
}

void DevMenu::onMenuPlayer(CCObject* pSender){
	//cocos2d::CCMenuItemImage* sender = (cocos2d::CCMenuItemImage*) pSender;
	playSndBtn();
	bool any_selected = false;
	for (us i = 0; i<PLAYER_MAX; i++){
		if(pSender == bPlayer[i]){
			bool on = !myOptions.penabled[i];
			myOptions.penabled[i] = on;
			//bPlayer[i]->setOpacity(255);
			char* tx = fullPath( on ? files::menu::pjImsOn[i] : files::menu::pjIms[i]);
			CCSprite *sprite = CCSprite::create(tx);
			bPlayer[i]->setNormalImage(sprite);
			//OPTIONS.cplayers = i+1;
			myOptions.cplayers += on?1:-1;
			any_selected = any_selected || on; //if on == true -> anyselected = true
		}else{
			any_selected = any_selected || myOptions.penabled[i]; //if enabled any = true
		}
	}
	if(bPlay){
		bPlay->setEnabled(any_selected);
		bPlay->setOpacity(any_selected?255:128);
	}
}

void DevMenu::onMenuMode(CCObject* pSender){
	playSndBtn();
	//thing cosas [] = { T_GOAL, T_BOMBS, T_SWARM, T_TIMER, T_BOOST, T_DOOR , T_CANT};ç
	//todo cambiar el orden de los botones para que siga el orden de los things? (el orden de los things es dificl de cambiar)
	Spinbox *sp;
	bool in;
	char *tx;
	us txn;
	for (us i = 0; i<=T_CANT; i++){
		sp = spins[i];
		sp->removeFromParent();
		in = pSender == bModes[i];
		if(in){
			sp->addTo(menu);
		}
		txn = MODE4THING[i];
		tx = fullPath( in? files::menu::modes_on[txn] : files::menu::modes[txn]);
		bModes[i]->setNormalImage(CCSprite::create(tx));
	}
}

void DevMenu::onMenuPlay(CCObject* pSender){
	//sobre el reset de options
	//int wn = spins[T_CANT]->value();

	//se fuerza a poner la velocidad de los timers
	//todo agregar los otros
	//myOptions.t_player = VAL4POS(T_SPEED, spins[T_SPEED]->value()/DM_SPMAX);
	myOptions.units = VAL4POS(T_SWARM, spins[T_SWARM]->value()/DM_SPMAX);
	myOptions.max_bombs = VAL4POS(T_BOMBS, spins[T_BOMBS]->value()/DM_SPMAX);
	myOptions.max_boosts = VAL4POS(T_BOOST, spins[T_BOOST]->value()/DM_SPMAX);
	myOptions.max_points = VAL4POS(T_MXPOINTS, spins[T_MXPOINTS]->value()/DM_SPMAX);
	myOptions.max_ipoints = VAL4POS(T_IPOINTS, spins[T_IPOINTS]->value()/DM_SPMAX);
	myOptions.t_player = VAL4POS(T_SPEED, spins[T_SPEED]->value()/DM_SPMAX);
	myOptions.modes[myOptions.M_GOAL] = VAL4POS(T_GOAL, spins[T_GOAL]->value()/spins[T_GOAL]->maximum()) >0;
	float pos;
	pos = spins[T_DOOR]->value()/DM_SPMAX;
	myOptions.modes[myOptions.M_ADOORS] = pos>0.0f;
	myOptions.t_door = VAL4POS(T_DOOR, pos);

	pos = spins[T_TIMER]->value()/DM_SPMAX;
	myOptions.modes[myOptions.M_TIMER] = pos>0.0f;
	myOptions.t_game = VAL4POS(T_TIMER, pos);

	playSndBtn();
	myOptions.modes[myOptions.M_DEMO] = false; //lets play for real
	EM.get(events::scenes::PLAY)->fire(this, &myOptions, true);
}

void DevMenu::onBackToMenu(CCObject* pSender){
	playSndBtn();
	EM.get(events::menu::START_MENU)->fire(this, NULL, true);
}

void DevMenu::unload(){
	Spinbox::unload();
	//todo unload mis texturas
	unloadTX(files::menu::player);
	unloadTX(files::menu::pjIms[0]);
	unloadTX(files::menu::pjIms[1]);
	unloadTX(files::menu::play);
	unloadTX(files::menu::mode);
	unloadTX(files::menu::menu);
	unloadTX(files::menu::skin);
	for (us i = 0 ; i<files::menu::C_MODES ; i++){
		unloadTX(files::menu::modes[i]);
	}
}
/*
 * Deprecated, solo con fines informativos (por si lo necesitamos)
void DevMenu::onMenuPlayerSpeed(CCObject* pSender){
	playSndBtn();
	bool in = true;
	for (us i = 0; i<PLAYER_SPEEDS; i++){
		//bPSpeed[i]->setOpacity(in?255:128);
		char* tx = fullPath( in? files::menu::speeds[i] : files::menu::speeds_off[i]);
		CCSprite *sprite = CCSprite::create(tx);
		bPSpeed[i]->setNormalImage(sprite);
		if(pSender == bPSpeed[i]){
			myOptions.pspeed = i;
			in = false;
		}
	}
}


void DevMenu::onMenuGTime(CCObject* pSender){
	playSndBtn();
	bool in = true;
	for (int i = 0; i<GAME_TIMERS; i++){
		//bGTimer[i]->setOpacity(in?255:128);
		char* tx = fullPath( in? files::menu::speeds[i] : files::menu::speeds_off[i]);
		CCSprite *sprite = CCSprite::create(tx);
		bGTimer[i]->setNormalImage(sprite);
		if(pSender == bGTimer[i]){
			in = false;
			myOptions.gtimer = i;
		}
	}
}*/
