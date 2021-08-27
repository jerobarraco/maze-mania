#include "exit.h"
#include "scenes/game.h"
ExitScene::ExitScene(){}

void ExitScene::onEnterTransitionDidFinish(){
	CCLOG("termino la entrada de la trasicion de exit");
	Game::unload();
	cocos2d::CCScene::onEnterTransitionDidFinish();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
		cocos2d::CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	#else
		cocos2d::CCDirector::sharedDirector()->end();
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
		#endif
	#endif
}

