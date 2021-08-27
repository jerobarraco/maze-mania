//system
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
//cocos
#include "cocos2d.h"
#include "CCEGLView.h"
//local
#include "../Classes/AppDelegate.h"

USING_NS_CC;
static const bool z10=false;
enum DEVICE_TYPE {DT_Z10, DT_Q10, DT_PLAYBOOK, DT_MAX};

static const DEVICE_TYPE DEVICE = DT_MAX;

int main(int argc, char **argv){
	// create the application instance
	AppDelegate app;
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	unsigned int sx, sy;
	switch (DEVICE){
		case DT_Z10:
			sx = 1280;
			sy = 768;
			break;
		case DT_Q10:
			sx = 720;
			sy = 720;
			break;
		case DT_PLAYBOOK:
			sx = 1024;
			sy = 600;
			break;
		case DT_MAX:
			sx = 1280;
			sy = 720;
			break;
	};
	eglView->setFrameSize(sx, sy);
	return CCApplication::sharedApplication()->run();
}
