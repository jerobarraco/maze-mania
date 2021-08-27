#ifndef BTNSCENE_H
#define BTNSCENE_H
#include "TouchSprite.h"
#include "utils/events.h"
class BtnEvent : public TouchSprite{
	Event* e;
	void *_p;
public:
	bool onTouch();
	BtnEvent(const char *tx, const char *ev, void *p=NULL);
	~BtnEvent();
};

#endif // BTNSCENE_H
