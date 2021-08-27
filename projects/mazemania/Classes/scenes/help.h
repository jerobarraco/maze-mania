#ifndef HELP_H_
#define HELP_H_

#include "cocos2d.h"
#include "scenes/loader.h"
#include "menu/btnback.h"
#include "anim/animation.h"

USING_NS_CC;
const unsigned short H_C_BTN = 7;
class Help: public LoadLayer{
	//ToDo: usar una constante
	CCMenuItemImage *bItems[H_C_BTN]; //ToDo: Cambiar items. Por ahora lleva los mismos botones que el dev menu
	//CCMenuItemImage *bBack;
	bool _alone;
	Animation *_a;
public:
	void keyBackClicked();
	static void onBack(void* self, void* sender, void* param);
	static void onBackSub(void* self, void* sender, void* param);
	void killSub();
	Help(bool alone=true);
	virtual ~Help();

	void load();
	static void unload();

	void onBItem(CCObject *sender);
//	bool ccTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//void onBack(CCObject *sender);
};
#endif /* HELP_H_ */