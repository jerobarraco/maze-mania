#include "btnback.h"
#include "utils/Files.h"

BtnBack::BtnBack(bool sub)
	:TouchSprite(fullPath(files::start_menu::back)){
	ev_back = EM.get(sub?events::menu::BACK_SUB:events::menu::BACK);
}

BtnBack::~BtnBack(){
	EVRELEASE(ev_back);
}

bool BtnBack::onTouch(){
	CCLOG("btn_back touched");
	//playSndBtn();
	ev_back->fire(this, NULL);
	return true;
}
