#include "cocos2d.h"
#include "help.h"
#include "scenes/game.h"
#include "utils/Files.h"
#include "utils/Utils.h"
#include "utils/events.h"
#include "anim/animation.h"
#include "menu/btnevent.h"
#include "menu/overlay.h"

//see files::help::anims
pcc BTN_TXS[] = {files::menu::modes[0], files::menu::modes[1], files::menu::modes[3],
	files::menu::modes[4], files::menu::modes[6], files::menu::modes[9], files::menu::player};

pcc BTN_TXS_ON[] = {files::menu::modes_on[0], files::menu::modes_on[1], files::menu::modes_on[3],
	files::menu::modes_on[4], files::menu::modes_on[6], files::menu::modes_on[9], files::menu::player_on};

void Help::keyBackClicked(){
	if (this->_alone){
		if(this->_a){
			Help::onBackSub(this, this, NULL);
		}else{
			Help::onBack(this, this, NULL);
		}
	}
}

void Help::onBack(void *self, void *sender, void *param){
	EM.get(events::scenes::MENU, false)->fire(NULL, NULL, false);
}

void Help::onBackSub(void *self, void *sender, void *param){
	((Help*)self)->killSub();
}

void Help::killSub(){
	if (!_a) return;

	_a->runAction(CCSequence::create( CCFadeTo::create(0.5, 0), CCRemoveSelf::create(),	NULL));
	_a = NULL;
	for (int i=0; i<H_C_BTN; i++)	{
		bItems[i]->setNormalImage(CCSprite::create(fullPath(BTN_TXS[i])));
	}
}

Help::Help(bool alone):LoadLayer(true){
	_alone = alone;
	_a = NULL;
}

void Help::unload(){
	unloadTX(files::help::title);
	for (us i=0; i<H_C_BTN; i++){
		unloadTX(BTN_TXS[i]);
		unloadTX(BTN_TXS_ON[i]);
	}
	//animations are so (supossedly) slow to unload that they need to be unloaded in the main place (appdelegate::onReload)
}

void Help::load(){
	this->loadStart(6);
	this->loadStep();
	char *t;
	if(_alone){
		//alone indica que se maneje solo
		// deseable si funciona como escena o algo asi
		EM.subscribe(this, events::menu::BACK, Help::onBack);
		//ahora que lo pienso no necesito retenerlo si me subscribo, eso lo retiene solito *1
		Game *g = new Game();
		g->load();
		this->addChild(g, -2);
	}

	this->loadStep();
	this->addChild(new Overlay(!_alone), -1);//if its not alone, block the input to the background

	this->loadStep();
	//CCSprite *title = CCSprite::create(fullPath(files::help::title));
	Animation *title = new Animation(files::help::a_title);
	title->setPosition(ccp(GLOBALS.cent_x, GLOBALS.cent_y+150));
	this->addChild(title);

	this->loadStep();
	CCMenu *menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	this->loadStep();
	CCPoint pos = ccp(GLOBALS.cent_x, GLOBALS.cent_y+100);

	for (int i=0; i<H_C_BTN;i++){
		if (i%4== 0){
			//pos = pos + ccp(-(90*i), -100);
			int quedan = H_C_BTN-i>=4?4:H_C_BTN-i;
			pos.x = GLOBALS.cent_x-(90*(quedan-1)/2);//90*4/2
			pos.y -= 100;
		}
		t = fullPath(BTN_TXS[i]);
		bItems[i] = CCMenuItemImage::create(t, t, this, menu_selector(Help::onBItem));
		bItems[i]->retain();
		bItems[i]->setPosition(pos);
		menu->addChild(bItems[i]);
		pos.x += 90;
	}

	this->loadStep();

	BtnEvent *bBack = new BtnEvent(fullPath(files::start_menu::back), events::menu::BACK);
	bBack->setPosition(ccp(50, GLOBALS.px_height-50));
	this->addChild(bBack);

	EM.subscribe(this, events::menu::BACK_SUB, Help::onBackSub);
	this->loadStop();
}

void Help::onBItem(CCObject *sender){
	if (_a) return ; //bad dog no biscuit
	for (int i=0; i<H_C_BTN; i++)	{
		if (sender == bItems[i]){
			//cambiar sprite a _on
			bItems[i]->setNormalImage(CCSprite::create(fullPath(BTN_TXS_ON[i])));
			//crear animacion
			_a = new Animation(files::help::anims[i]);
			_a->setPosition(ccp(GLOBALS.cent_x, GLOBALS.cent_y));
			Overlay *ov = new Overlay(true);
			ov->setPosition(ccp(-GLOBALS.cent_x, -GLOBALS.cent_y));
			_a->addChild(ov, -1);
			BtnEvent *b = new BtnEvent(fullPath(files::start_menu::back),events::menu::BACK_SUB);
			//b->setPosition(ccp(GLOBALS.px_width-40, 40));
			b->setPosition(ccp(GLOBALS.cent_x-40, -GLOBALS.cent_y+40));
			_a->addChild(b, 200);
			this->addChild(_a, 200);//notar asignacion a _a
		}else{
			//desactivar anterior
			bItems[i]->setNormalImage(CCSprite::create(fullPath(BTN_TXS[i])));
		}
	}
}

Help::~Help(){
	EM.unsubscribe(this, events::menu::BACK_SUB);
	//*1 y al final lo unsubscribo, eso le hace release
	//y si this no esta subscripto, no hace nada igual
	//igual ojo porque es mas lento
	CCLOG("Help is dead");
	for (int i=0; i<H_C_BTN; i++){
		RELEASE(bItems[i]);
	}
	EM.get(events::menu::BACK, false)->unsubscribe(this);
	removeAllChildren();
}
