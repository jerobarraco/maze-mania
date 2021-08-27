#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "AppDelegate.h"

#include "utils/Utils.h"
#include "utils/Files.h"

#include "scenes/main_scene.h"
#include "scenes/exit.h"
#include "scenes/game.h"
#include "scenes/about.h"
#include "scenes/loader.h"
#include "scenes/intro.h"
#include "scenes/help.h"

#include "anim/animation.h"

USING_NS_CC;

static Options defaults;

void setDefaults(){
	memcpy(&OPTIONS, &defaults, sizeof(Options));
}

void AppDelegate::onReload(void *self, void *sender, void *param){
	Intro::unload();
	About::unload();
	MainScene::unload();
	Game::unload();
	Help::unload();
	Animation::unload();//SSlow (super slow)//animations are so (supossedly) slow to unload that they need to be unloaded in the main place (appdelegate::onReload)

	int nskin = param == NULL?GLOBALS.skin: *((int*)param);
	GLOBALS.skin = nskin;
	if (GLOBALS.skin > C_SKINS-1){
		GLOBALS.skin = 0;
	}
	EM.get(events::scenes::MENU)->fire(NULL, NULL, true);
}

void AppDelegate::onIntro(void *self, void *sender, void *param){
	Loader::transitionTo(new Intro());
}

void AppDelegate::onAbout(void *self, void *sender, void *param){
	Loader::transitionTo(new About());
}

void AppDelegate::onHelp(void *self, void *sender, void *param){
	Loader::transitionTo(new Help());
}

void AppDelegate::onCloseApp(void* self, void* sender, void* param){
	//this kills the app used mostly on android
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, new ExitScene() ));
}

void AppDelegate::onMenu(void *self, void *sender, void *param){
	setDefaults();
	Loader::transitionTo(new MainScene());
}

void AppDelegate::onPlay(void *self, void *sender, void *param){
	Options *o = (Options*) param;
	if(o){
		memcpy(&OPTIONS, o, sizeof(Options));
	}
	OPTIONS.modes[OPTIONS.M_DEMO] = false;//set as playing//por las dudas
	Loader::transitionTo(new Game());
}

AppDelegate::AppDelegate(){}

AppDelegate::~AppDelegate(){
	destroyAudio();
	EVUNSUB(e_intro)
	EVUNSUB(e_quit)
	EVUNSUB(e_play)
	EVUNSUB(e_menu)
	EVUNSUB(e_about)
	//im testing this, is slower but needs less code, and one less duplicated pointer
	EM.unsubscribe(this, events::scenes::HELP);
	EVUNSUB(e_reload)
}

bool AppDelegate::applicationDidFinishLaunching() {
	srand(time(NULL));
	// initialize director
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

	pDirector->setOpenGLView(pEGLView);

	//events
	e_intro = EM.subscribe(this, events::scenes::INTRO, AppDelegate::onIntro);
	e_play = EM.subscribe(this, events::scenes::PLAY, AppDelegate::onPlay);
	e_menu = EM.subscribe(this, events::scenes::MENU, AppDelegate::onMenu);
	e_about = EM.subscribe(this, events::scenes::ABOUT, AppDelegate::onAbout);
	EM.subscribe(this, events::scenes::HELP, AppDelegate::onHelp);
	e_quit = EM.subscribe(this, events::app::QUIT, AppDelegate::onCloseApp);
	e_reload = EM.subscribe(this, events::app::RELOAD, AppDelegate::onReload);

	// suff
	//CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize(); //unused
	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);
	CCSize visibleSize = pDirector->getVisibleSize();

	GLOBALS.px_height = visibleSize.width;
	GLOBALS.px_width = visibleSize.width;
	GLOBALS.px_height = visibleSize.height;
	GLOBALS.cent_x = GLOBALS.px_width/2;
	GLOBALS.cent_y = GLOBALS.px_height/2;
	GLOBALS.off_x = 0;
	GLOBALS.off_y = 0;
	GLOBALS.skin = 0;//0: pixel_mode, 1: sherman
	GLOBALS.audio = true;
	//defaults.cplayers = PLAYER_MAX;
	//for (us i = 0; i<PLAYER_MAX;i++)
	//defaults.players[i] = true;
	// turn on display FPS
	#ifdef DEBUG
		pDirector->setDisplayStats(true);//disabled because of blackberry store maybe rejects it
		GLOBALS.audio = false;
	#endif
	defaults.cplayers = 6; //para que se muestren en el fondo
	setDefaults();

	//create a scene. it's an autorelease object
	#ifdef DEBUG
		//pScene = new Loader(new LoadScene(new MainScene()));//la primer escena tiene que ser runWithScene
		pScene = new Loader(new LoadScene(new Help(true)));//la primer escena tiene que ser runWithScene
	#else
		pScene = new Loader(new LoadScene(new Intro()));
	#endif
	//EM.get(EV_MENU)->fire(this, NULL);
	//run
	pDirector->runWithScene(pScene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call, it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	// if you use SimpleAudioEngine, it must be pause
	//la escena tiene un layer, ese layer es el mainScreen, es por ahora la mejor manera que encuentro de pausar las cosas
	//sin tener que reimplementar un scene. Otra forma es usar un evento , pero seria un overkill porque probablemente
	//no habran muchas cosas que necesiten hacer algo al pausar la app.
	//TODO independizar de las escenas
	//TODO crear un evento de ser necesiario
	CCDirector::sharedDirector()->stopAnimation();
	//maybe i should just call pause here.. right?
	CCLOG("pausing app");
	pauseAudio();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	//CCLOG("resumming app, gstate is %i", this->gstate);
	replayBGM();//this sucks but luckly this function gets called for android only (or it should)
	EM.get(events::app::RESUME)->fire(this, NULL, true);
	CCDirector::sharedDirector()->startAnimation();
}
