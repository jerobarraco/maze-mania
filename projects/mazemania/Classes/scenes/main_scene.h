#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "StartMenu.h"
#include "DevMenu.h"
#include "utils/Utils.h"
#include "utils/events.h"
#include "scenes/loader.h"
/*
 * See game.h to know why, but you can use it as a scene using sceneForLayer on utils.h
 */
enum MState {MS_START=0, MS_DEV};

class MainScene : public LoadLayer{
	Event *e_devMenu, *e_startMenu;
	StartMenu *startMenu;
	DevMenu *devMenu;
	MState mstate;
public:
	static void unload();
	static void onShowDevMenu(void *self, void *sender, void* param);
	static void onShowStartMenu(void *self, void *sender, void* param);

	void showDevMenu();
	void backToMainMenu();
	void killScreens();
	void keyBackClicked();
	void load();
	MainScene();
	~MainScene();
};

#endif // __HELLOWORLD_SCENE_H__
