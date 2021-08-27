#include "btnevent.h"
#include "utils/Files.h"

bool BtnEvent::onTouch(){
	playSndBtn();
	e->fire(this, _p);
	return true;
}

BtnEvent::BtnEvent(const char *tx, const char *ev, void *p)
	:TouchSprite(tx){
	e = EM.get(ev);
	_p = p;
}

BtnEvent::~BtnEvent(){
	EVRELEASE(e);
}
