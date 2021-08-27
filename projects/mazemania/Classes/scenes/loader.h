#ifndef LOADER_H
#define LOADER_H
#include "cocos2d.h"
#include "utils/events.h"

//loadable layer
class LoadLayer : public cocos2d::CCLayer{
	Event* estep;
public:
	LoadLayer(bool catcher=true);
	inline virtual void load(){
		loadStart(1);
		loadStep(1);
		loadStop();
	}
	void loadStart(int steps){ EM.get(events::loader::START)->fire(this, &steps, true); }
	void loadStep(int csteps=1){
		if (!estep){
			estep = EM.get(events::loader::STEP);
		}
		estep->fire(this, &csteps);
	}
	void loadStop(){
		EVRELEASE(estep)
		EM.get(events::loader::STOP)->fire(this, NULL, true);
	}
	virtual ~LoadLayer(){
		EVRELEASE(estep)
		CCLOG("loadlayer dying");
		this->removeAllChildren();
	}
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){return false;}//override me, cocos will CRASH if its not defined, WTF SERIOUSLY?!
};

//allows to sublcass a scene, or wrap a layer that supports loading
class LoadScene : public cocos2d::CCScene{
	LoadLayer *layer;
	void constructor();
public:
	LoadScene();
	LoadScene(LoadLayer* lyr);
	inline virtual void load(){
		CCLOG("loading the layer");
		if(layer) layer->load();
		CCLOG("loading the layer done");
	}
	virtual ~LoadScene();
};

//simple intermediate class that allows scenes to divide clearly the lifespan of (meaningfull) scenes
//so that preloading/unloading can work fine

class Loader : public cocos2d::CCScene{
	LoadScene *ls;
	Event *e_start, *e_step, *e_stop;
	cocos2d::CCSprite *sp;
	cocos2d::SEL_SCHEDULE sel;
public:
	int step, steps;
	static void onStart(void *self, void *sender, void *param);
	static void onStep(void *self, void *sender, void *param);
	static void onStop(void *self, void *sender, void *param);

	void doStep(int pstep);
	Loader(LoadScene* load_scene);
	void onEnterTransitionDidFinish();
	void loadAndSwap(float f);
	virtual ~Loader();
	static void transitionTo(LoadScene *scene){
		cocos2d::CCScene *loader = new Loader(scene);
		cocos2d::CCDirector::sharedDirector()->replaceScene(cocos2d::CCTransitionFade::create(0.3f, loader));
	}
	static void transitionTo(LoadLayer *ll){
		Loader::transitionTo(new LoadScene(ll));
	}
};
#endif // LOADER_H

