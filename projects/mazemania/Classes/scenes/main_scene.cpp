#include "CCDirector.h"//director
#include "misc_nodes/CCClippingNode.h"
#include "SimpleAudioEngine.h"

#include "scenes/main_scene.h"
#include "scenes/game.h"
#include "utils/Files.h"


USING_NS_CC;

void MainScene::unload(){
	StartMenu::unload();
	DevMenu::unload();
}

void MainScene::onShowDevMenu(void *self, void *sender, void *param){
	((MainScene*)self)->showDevMenu();
}

void MainScene::onShowStartMenu(void *self, void *sender, void *param){
	((MainScene*) self)->backToMainMenu();
}

void MainScene::showDevMenu(){
	//killScreens(); //hay que estar seguro de no llamarlo desde el devmenu o tenemos leak
	mstate = MS_DEV;

	devMenu->setPosition(ccp(0, GLOBALS.px_height));
	CCMoveTo *moveDMenu = CCMoveTo::create(0.3f, ccp(0, 0));
	devMenu->runAction(moveDMenu);
	this->addChild(devMenu, 10);

	CCMoveTo *moveSMenu = CCMoveTo::create(0.3f, ccp(0, -GLOBALS.px_height));
	startMenu->runAction(moveSMenu);
}

void MainScene::killScreens(){
	this->removeChild(devMenu);
	// El StartMenu no se debe destruir xq se reutiliza siempre.
	//this->removeChild(startMenu);
}

void MainScene::keyBackClicked(){
	if(mstate == MS_START){
		EM.get(events::scenes::INTRO)->fire(this, NULL, true);
	}else{
		this->backToMainMenu();
	}
}

void MainScene::load(){
	for (us i=0; i < 20; i++){
		OPTIONS.overflow[i] = 54;
	}
	////events
	///	internal
	e_devMenu = EM.subscribe(this, events::menu::DEV_MENU, MainScene::onShowDevMenu);
	e_startMenu = EM.subscribe(this, events::menu::START_MENU, MainScene::onShowStartMenu);
	//load
	this->loadStart(3);

	mstate = MS_START;

	this->loadStep();
	devMenu = new DevMenu();
	devMenu->retain();

	this->loadStep();
	LoadLayer *g = new Game();
	g->load();
	this->addChild(g);

	this->loadStep();
	startMenu = new StartMenu();
	startMenu->retain();
	this->addChild(startMenu, 10);
	this->loadStop();
}

void MainScene::backToMainMenu(){
	//TODO limpiar
	//this->addChild(devMenu, 10);
	//this->addChild(startMenu, 10);

	if(devMenu){
		CCMoveTo *moveDMenu = CCMoveTo::create(0.5, ccp(0, GLOBALS.px_height));
		devMenu->runAction(moveDMenu);
	}

	//startMenu->setPositionY(-GLOBALS.px_height);
	CCMoveTo *moveSMenu = CCMoveTo::create(0.5, ccp(0, 0));
	CCFiniteTimeAction *ac_killscreens = CCCallFunc::create(this, callfunc_selector(MainScene::killScreens));
	startMenu->runAction(CCSequence::create(moveSMenu, ac_killscreens, NULL));
}

MainScene::MainScene(){
	e_devMenu=NULL;
	e_startMenu=NULL;
	devMenu=NULL;
	startMenu=NULL;
}

MainScene::~MainScene(){
	killScreens();
	CCLOG("main scene is dead");

	EVUNSUB(e_devMenu)
	EVUNSUB(e_startMenu)
	RELEASE(devMenu)
	RELEASE(startMenu)
	this->removeAllChildren();
}
