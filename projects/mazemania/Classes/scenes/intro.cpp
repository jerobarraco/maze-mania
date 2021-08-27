#include "SimpleAudioEngine.h"

#include "intro.h"
#include "utils/Files.h"
#include "utils/Utils.h"
#include "utils/events.h"
#include "anim/animation.h"


Intro::~Intro(){
	CCLOG("Intro dying");
	RELEASE(bPlay)
	RELEASE(bAudio)
	RELEASE(bAbout)
	RELEASE(bExit)
	this->removeAllChildren();
}

void Intro::keyBackClicked(){
	onExit(NULL);
}

void Intro::unload(){
	unloadTX(files::intro::bg);
	unloadTX(files::intro::play);
	unloadTX(files::intro::logo);
	unloadTX(files::game::players[1]);
}

void Intro::load(){
	this->loadStart(8);
	this->loadStep();
	playBGM();

	char *tx;
	CCPoint pos;

	// Background
	this->loadStep();
	tx = fullPath(files::intro::bg);
	CCSprite *bg = CCSprite::create(tx);
	pos = ccp(GLOBALS.cent_x, GLOBALS.cent_y);//CCPointZero;
	bg->setPosition(pos);
	bg->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(bg);

	// Logo
	this->loadStep();
	Animation *a = new Animation(files::intro::a_logo);
	a->setPosition(ccp(GLOBALS.cent_x, GLOBALS.cent_y));
	this->addChild(a);
	/*tx = fullPath(files::intro::logo);
	CCSprite *logo = CCSprite::create(tx);
	pos = ccp(GLOBALS.cent_x, GLOBALS.px_height -200);
	logo->setPosition(pos);
	logo->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(logo);
	CCFiniteTimeAction *ac_move = CCMoveTo::create(1, pos+ccp(0, -50));
	CCFiniteTimeAction *ac_fade = CCFadeIn::create(1);
	logo->runAction(ac_move);
	logo->runAction(ac_fade);
	// the munyequito
	CCSprite *muny = CCSprite::create(fullPath(files::game::players[1]));
	muny->setPosition(pos+ccp(300, 150));
	muny->setAnchorPoint(ccp(0.5, 0.5));
	muny->setScale(2);
	muny->setRotation(10);
	this->addChild(muny);
	ac_move = CCMoveTo::create(1, pos+ccp(200, 50));
	ac_fade = CCFadeIn::create(1);
	muny->runAction(ac_move);
	muny->runAction(ac_fade);*/

	// =======MenuItems
	// Play
	this->loadStep();
	tx = fullPath(files::intro::play);
	bPlay = CCMenuItemImage::create(tx, tx, this, menu_selector(Intro::onPlay));
	pos = ccp(GLOBALS.cent_x, 100);
	bPlay->setPosition(pos);
	bPlay->setAnchorPoint(ccp(0.5,0.5));
	bPlay->retain();

	// Exit
	this->loadStep();
	tx = fullPath(files::menu::exit);
	bExit = CCMenuItemImage::create(tx, tx, this, menu_selector(Intro::onExit));
	pos = ccp(100, GLOBALS.px_height -50);
	bExit->setPosition(pos);
	bExit->setAnchorPoint(ccp(0.5,0.5));
	bExit->retain();

	// About
	this->loadStep();
	tx = fullPath(files::menu::about);
	bAbout = CCMenuItemImage::create(tx, tx, this, menu_selector(Intro::onAbout));
	pos = ccp(GLOBALS.cent_x, GLOBALS.px_height -50);
	bAbout->setPosition(pos);
	bAbout->setAnchorPoint(ccp(0.5,0.5));
	bAbout->retain();

	// Audio
	this->loadStep();
	unsigned int m = GLOBALS.audio?1:0;
	tx = fullPath(files::menu::music[m]);
	bAudio = CCMenuItemImage::create(tx, tx, this, menu_selector(Intro::onAudio));
	pos = ccp(GLOBALS.px_width - 100, GLOBALS.px_height -50);
	bAudio->setPosition(pos);
	bAudio->setAnchorPoint(ccp(0.5,0.5));
	bAudio->retain();

	// CCMenu
	this->loadStep();
	CCMenu *menu = CCMenu::create(bPlay, bAudio, bAbout, bExit,
		NULL);
	menu->setPosition(CCPointZero);

	this->addChild(menu, 10);
	this->loadStop();
}

void Intro::onPlay(CCObject *sender){
	playSndBtn();
	EM.get(events::scenes::MENU)->fire(this, NULL, true);
}

void Intro::playBGM(){
	play(files::about::m_bgm, false, true, false);
}

void Intro::onAudio(CCObject *sender){
	playSndBtn();
	GLOBALS.audio = !GLOBALS.audio;
	char *t = fullPath( GLOBALS.audio? files::menu::music[1] : files::menu::music[0]);
	CCSprite *sprite = CCSprite::create(t);
	bAudio->setNormalImage(sprite);

	if(!GLOBALS.audio){
		pauseAudio();
	}else{
		resumeAudio();
		playBGM();
	}
}

void Intro::onAbout(CCObject *sender){
	playSndBtn();
	EM.get(events::scenes::ABOUT)->fire(this, NULL, true);
}

void Intro::onExit(CCObject *sender){
	playSndBtn();
	EM.get(events::app::QUIT)->fire(this, NULL, true);
}
