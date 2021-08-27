#include "loader.h"
#include "cocos2d.h"
#include "utils/Files.h"
#include "utils/Utils.h"
//#include <stdlib.h>     //for using the function sleep
//#include <unistd.h>

USING_NS_CC;//actually needed to use the scheduler

void Loader::onStart(void *self, void *sender, void *param){
	((Loader *)self)->steps += *(int*)param;
	CCLOG("Loader started %i -> %i", *(int*)param, ((Loader *)self)->steps);
	//the clever += allows to have several loadable stuff one inside the other without the parent knowing how
	//much the child must load.. example, game can be used by itself, or inside main screen, which in turn needs to load more stuff
	((Loader*)self)->doStep(0);//updates position
}

void Loader::onStep(void *self, void *sender, void *param){
	((Loader*)self)->doStep(
		param == NULL? 1 : *(int*)param
	);//in case there's no param, take a 1 step
}

void Loader::onStop(void *self, void *sender, void *param){
	CCLOG("Loader stopped");
}

void Loader::doStep(int pstep){
	step += pstep;
	CCLOG("Loader stepped %i -> %i", pstep, step);
	int x;
	if(this->steps == 0){
		x = 0;
	}else{
		x = (GLOBALS.px_width / this->steps)*step;
	}
	this->sp->setPosition(ccp(x, GLOBALS.cent_y-50));
	cocos2d::CCDirector::sharedDirector()->drawScene();
	//cocos2d::CCDirector::sharedDirector()->scheduleOnce((SEL_SCHEDULE)(&cocos2d::CCDirector::drawScene), 2);
//	CCDelayTime *d = CCDelayTime::create(2.0f);
	//CCCallFunc *ac_release = CCCallFunc::create(cocos2d::CCDirector::sharedDirector(), callfunc_selector(cocos2d::CCDirector::drawScene));
	//this->runAction(CCSequence::create(d, ac_release, NULL));
	//sleep(1000);
}

Loader::Loader(LoadScene *load_scene):cocos2d::CCScene(){
	this->init();
	this->autorelease();
	step = steps = 0;
	//schedule_selector needs the using_ns_cc;
	sel = NULL;
	CCLOG("a new loader with a loadscene");
	ls = load_scene;
	ls->retain();
	e_start = EM.subscribe(this, events::loader::START, Loader::onStart);
	e_step = EM.subscribe(this, events::loader::STEP, Loader::onStep);
	e_stop = EM.subscribe(this, events::loader::STOP, Loader::onStop);

	char *t = fullPath(files::com::loading);//this is the loading logo, use whatever you want
	CCSprite *s = CCSprite::create(t);
	s->setPosition(ccp(GLOBALS.cent_x, GLOBALS.cent_y));
	this->addChild(s);

	t = fullPath(files::game::players[rand()%PLAYER_MAX]);//use the image you want here as a progress indicator
	sp = CCSprite::create(t);
	sp->retain();
	sp->setPosition(ccp(50, GLOBALS.cent_y-50));
	this->addChild(sp);
}

void Loader::onEnterTransitionDidFinish(){
	//other scene should be destroyed by now, we start loading
	//sel = schedule_selector(Loader::loadAndSwap);
	CCLOG("transition ended, scheduling");
	
	this->scheduleOnce(schedule_selector(Loader::loadAndSwap), 0.2f);
	//cocos2d::CCFiniteTimeAction *ac_scale = CCScaleTo::create(0.5, 1.5);
	//CCDelayTime *d = CCDelayTime::create(2.0f);
	//CCCallFunc *ac_release = CCCallFunc::create(this, callfunc_selector(Loader::loadAndSwap));
	//this->runAction(CCSequence::create(d, ac_release, NULL));
	//this->scheduleOnce((SEL_SCHEDULE)&(Loader::loadAndSwap)sel, 2);
	//we really need this, read loadAndSwap();
	
	
}

void Loader::loadAndSwap(float f){
	this->unschedule(schedule_selector(Loader::loadAndSwap));//lol this is the black magic that allows us to not crash AND update the screen from a method that
	//is the result of a schedule, otherway, itll crash in the schedule loop when this finishes.
	//this->pauseSchedulerAndActions();
	//this function is made so the loading and swapping can be scheduled
	//that, in time, is needed to let cocos kill the other classes and unload stuff as well as allow another transition.
	//in other case the transition will fail
	CCLOG("loading scene");
	//forces loading (and creation)
	ls->load();
	CCLOG("scene loading ended, transitioning");
	//sets the new scene after the loading
	cocos2d::CCDirector::sharedDirector()->replaceScene(
		cocos2d::CCTransitionFade::create(0.2f, ls)
	);
}

Loader::~Loader(){
	CCLOG("loader died");
	EVUNSUB(e_start)
	EVUNSUB(e_step)
	EVUNSUB(e_stop)

	RELEASE(ls)
	RELEASE(sp)
	this->removeAllChildren();
}

void LoadScene::constructor(){
	cocos2d::CCScene();
	layer = NULL;
	this->init();
	this->autorelease();
}
//*1 no, you cant call an overloaded constructor from another one, thanks c++ http://stackoverflow.com/a/308318/260242
LoadScene::LoadScene(){
	constructor();//*1
}

LoadScene::LoadScene(LoadLayer *lyr){
	constructor();//*1
	CCLOG("a new loadescene with a loadlayer");
	layer = lyr;
	layer->retain();
	this->addChild(layer);
}

LoadScene::~LoadScene(){
	CCLOG("loadscene dying");
	if (layer)
		layer->release();
	layer = NULL;
	this->removeAllChildren();
}

LoadLayer::LoadLayer(bool catcher){
	CCLOG("loadlayer constructor");
	estep = NULL;
	this->init();
	this->autorelease();
	if (catcher){
		setAccelerometerEnabled(false);
		setTouchEnabled(true);
		setKeypadEnabled(true);
		setTouchMode(kCCTouchesOneByOne);//it is important, this way it calls the ccTouchBegan instead of the ccTouchesBegan (which kinda sucks)
		CCLog("Layer Options keypad:%d , accel:%d, touch:%d", isKeypadEnabled(), isAccelerometerEnabled(), isTouchEnabled() );
	}
}
