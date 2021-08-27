#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

#include "utils/events.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class AppDelegate : private cocos2d::CCApplication{
	cocos2d::CCScene *pScene;
	Event *e_quit, *e_play, *e_menu, *e_about, *e_intro, *e_reload;
public:
	static void onReload(void* self, void* sender, void* param);
	static void onIntro(void* self, void* sender, void* param);
	static void onAbout(void* self, void* sender, void* param);
	static void onHelp(void* self, void* sender, void* param);
	static void onCloseApp(void* self, void* sender, void* param);
	static void onPlay(void* self, void* sender, void* param);
	static void onMenu(void* self, void* sender, void* param);

	AppDelegate();
	virtual ~AppDelegate();

	/**
	@brief    Implement CCDirector and CCScene init code here.
	@return true    Initialize success, app continue.
	@return false   Initialize failed, app terminate.
	*/
	virtual bool applicationDidFinishLaunching();

	/**
	@brief  The function be called when the application enter background
	@param  the pointer of the application
	*/
	virtual void applicationDidEnterBackground();

	/**
	@brief  The function be called when the application enter foreground
	@param  the pointer of the application
	*/
	virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_

