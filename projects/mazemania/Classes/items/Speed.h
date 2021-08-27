#ifndef SPEED_H
#define SPEED_H
#include "Item.h"
#include "Player.h"
#include "utils/events.h"

enum SPD_TYPE {SPDT_UP=0, SPDT_DOWN};

class Speed : public Item{
	Event *e_activated;
	SPD_TYPE type;
	float factor;
public:
	bool up;//TODO o quitamos up o quitamos SPD_TYPE

	Speed(Board*b, int xi, int yi);
	void activate(Player *p);
	~Speed(){
		EVRELEASE(e_activated)
	}
	static void unload();
};

#endif // SPEED_H
