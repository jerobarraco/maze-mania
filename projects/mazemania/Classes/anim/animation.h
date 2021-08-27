#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "cocos2d.h"
#include "anim/aitem.h"

typedef std::vector<AItem*> VAItem;
class Animation:public cocos2d::CCNodeRGBA{
	VAItem *_items;
	static VAItem *loadItems(const char *path, const bool load=true);
public:
	static void unload(const char *f);//this will be really slow, read on aitem::unload
	static void unload();//this is SLOW! avoid calling it!
	void go();

	Animation(const char *f, const bool start=true);
	~Animation();
	//void setOpacity(GLubyte opaque);
	void setOpacity(GLubyte opaque);
	void onEnter();
	void onExit();
};

#endif // ANIMATION_H
