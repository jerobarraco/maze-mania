#ifndef GAME_H
#define GAME_H

#include "cocos2d.h"

#include "Board.h"
#include "GameOverMenu.h"
#include "scenes/loader.h"

/*This class wraps the board and stuff, so it can be safely included in the other scenes and renders fine
 * (and works fine too)
 * it can be used as composition or aggregation to other classes,
 * or it can be wrapped in a CCScene by calling sceneForLayer in utils.h
*/

enum GState {GS_DEMO=0, GS_PLAYING, GS_PAUSED, GS_GOVER};

class Game : public LoadLayer{
	//ya hay una clase game en cocos2d asi que hay que tener cuidado
	Board *b;
	GameOverMenu *gom;
	GState gstate;
	Event *e_game_over, *e_pause, *e_resume, *e_resume_app, *e_restart_wave;

	
public:
	void nextWave();
	void restartWave();
	static void onRestartWave(void *self, void *sender, void* param);
	static void onGameOver(void *self, void *sender, void* param);
	static void onPause(void *self, void *sender, void* param);
	static void onResume(void *self, void *sender, void* param);
	static void onResumeApp(void *self, void *sender, void* param);
	static void unload();

	void setPaused(bool paused, int team=GO_PAUSE);
	void load();
	Game();
	~Game();
	void keyBackClicked(void);
	void showGameOver(int team);
};

#endif // GAME_H
