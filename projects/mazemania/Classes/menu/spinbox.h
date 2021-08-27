#ifndef SPINBOX_H
#define SPINBOX_H
#include "cocos2d.h"
#include "utils/events.h"
class Spinbox: public cocos2d::CCMenuItem{
	cocos2d::CCMenuItemImage *more, *less;
	cocos2d::CCMenuItemLabel *lbl;
	int _min, _max, val, step;
	Event* e_changed;
public:
	//TODO deshabilitar los botones cuando se llegue a uno de los máximos
	//TODO usar parent nomas, cocos es  muy choto como para hacer subclassing
	//el parent porque ahora ando muy apurado como para que esta clase herede de
	//ccmenuitem (sin lo cual crashea obvio) y probablemente haya que implementar que pase el
	//touch a los hijos cosa que no tengo tiempo de hacer
	Spinbox(cocos2d::CCMenu *parent, cocos2d::CCPoint pos, int pmax=100, int pmin=0, int step=1);

	~Spinbox();
	void change(CCObject* pSender);
	int value();
	int maximum();//el vivo de visual studio tiene un macro "max" den minuscula, pero no una funcion round
	void setValue(const int &pVal);
	void removeFromParent();
	void addTo(cocos2d::CCMenu *p);
	static void unload();
};

#endif // SPINBOX_H
