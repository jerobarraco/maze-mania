#ifndef ABOUT_H
#define ABOUT_H
#include "cocos2d.h"
#include "loader.h"
#include "menu/btnurl.h"

USING_NS_CC;

class About : public LoadLayer{
	CCSprite* slide;
	CCPoint base_point, top_point;
public:
	static void unload();
	About():LoadLayer(true){slide=NULL;}
	void load();
	void onBack(CCObject *sender);
	void scroll();
	~About();
	void keyBackClicked();
};

#endif // ABOUT_H
