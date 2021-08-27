#ifndef FRAME_H
#define FRAME_H
#include "cocos2d.h"

//todo rename to item or something like that
struct FrameDef{
	short x,y;
	float a,r,t,s;
	char sfx[128];
};

typedef std::vector<FrameDef> VFD;
class AItem:public cocos2d::CCSprite{
public:
	enum AState {AS_STOPPED, AS_PLAYING, AS_PAUSED};//because FYou thats why
private:
	AState _state;
	bool loop;
	VFD _fdefs;
	unsigned short _frame;
	void doFrame();
	void reset();
public:
	char tx[128];
	// each skin can define its own anim, the anim can define its own frames with its own items with its own textures
	static void unload(char *l);
	AItem(char *line);
	//returns the actions for in (enter)
	//cocos2d::CCFiniteTimeAction *acIn(float t = -1);
	void go();
	void stop();//full esta de prueba nomas
	void pause();
	void onEnter();
	void onExit();
};

#endif // FRAME_H
