#ifndef BTNBACK_H
#define BTNBACK_H
#include "TouchSprite.h"
#include "utils/events.h"

//TODO quitar, no es necesario, es un btnevent
class BtnBack:public TouchSprite{
	Event* ev_back;
public:
	BtnBack(bool sub=false);
	~BtnBack();
	bool onTouch();
};

#endif // BTNBACK_H
