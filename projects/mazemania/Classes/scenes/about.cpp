#include "about.h"
#include "utils/events.h"
#include "utils/Files.h"
#include "utils/Utils.h"
#include "scenes/game.h"
#include "cocos2d.h"

void About::unload(){
	unloadTX(files::menu::menu);
	unloadTX(files::about::credits);
	unloadTX(files::about::moongate);
	unloadTX(files::about::graphics);
	unloadTX(files::about::music);
	unloadTX(files::about::twitter);
	unloadTX(files::about::facebook);
	unloadTX(files::about::diaspora);
	unloadTX(files::about::patreon);
}

void About::load(){
	this->loadStart(6);
	this->loadStep();
	play(files::about::m_bgm, false, true, false);

	this->loadStep();
	LoadLayer *g = new Game();
	g->load();
	this->addChild(g);

	char *t;
	CCPoint pos = ccp(50, GLOBALS.px_height-50);
	short dif = (768 - GLOBALS.px_width);

	this->loadStep();
	//foreground
	base_point = ccp((768-dif)/2, -1600/2);// TODO usar cent_x
	top_point = base_point+ccp(0, 1600*2);
	t= fullPath(files::about::credits);
	slide = CCSprite::create(t);
	slide->setAnchorPoint(ccp(0.5, 0.5));
	slide->setPosition(ccp(GLOBALS.cent_x, 0));//todo getSpriteSize.x
	slide->retain();
	addChild(slide);

	this->loadStep();
	//back button
	t= fullPath(files::menu::menu);
	CCMenuItemImage *bBack = CCMenuItemImage::create(t, t, this, menu_selector(About::onBack));
	//bBack->setAnchorPoint(ccp(0, 0));
	bBack->setPosition(pos);

	//menu
	this->loadStep();
	CCMenu* menu = CCMenu::create(bBack, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 10);

	this->loadStep();
	// menu buttons
	//moongate
	const char *txs[] = {files::about::moongate, files::about::graphics, files::about::music,
	files::about::twitter, files::about::facebook, files::about::diaspora, files::about::patreon };
	const char *urls[] = {
		//moon, graphic, music
		"http://www.moongate.com.ar", skinDef()->graphicsURL, skinDef()->musicURL,
		//twitter, fb, diasp, patreon
		"https://twitter.com/moongateis", "https://www.facebook.com/MoonGateInteractive",
		"http://diasp.de/u/moongate", "http://patreon.com/MoongateIS"
	};
	CCMenuItemImage *btmp;
	for (us i =0; i<7; i++){
		btmp = new BtnUrl(urls[i], fullPath(txs[i]));
		pos.y -= 80;
		btmp->setPosition(pos);
		menu->addChild(btmp);
	}

	//starts the scrolling (sets the fg position)
	scroll();
	this->loadStop();
}

void About::onBack(CCObject *sender){
	keyBackClicked();
}

void About::scroll(){
	slide->setPosition(base_point);
	CCFiniteTimeAction* ac_move = CCMoveTo::create(40, top_point);
	CCFiniteTimeAction* ac_move_done = CCCallFunc::create(this, callfunc_selector(About::scroll));
	slide->runAction(CCSequence::create(ac_move, ac_move_done, NULL));
}

About::~About(){
	CCLOG("about dying");
	RELEASE(slide)
	this->removeAllChildren();
}

void About::keyBackClicked(){
	playSndBtn();
	EM.get(events::scenes::INTRO)->fire(this, NULL, true);
}
