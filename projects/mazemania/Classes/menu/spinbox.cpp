#include "spinbox.h"
#include "cocos2d.h"
USING_NS_CC;
#include "utils/Files.h"


Spinbox::Spinbox(cocos2d::CCMenu *parent, CCPoint pos, int pmax, int pmin, int step):cocos2d::CCMenuItem(){
	less = more = NULL;
	this->_max = pmax;
	this->_min = pmin;
	this->val = pmin;
	this->step = step;
	this->init();
	this->autorelease();
	//this->initWithTarget(this, menu_selector(Spinbox::change) );
	lbl = CCMenuItemLabel::create(CCLabelBMFont::create(itos(val), fullPath(files::com::f_com)));
	lbl->setAnchorPoint(ccp(0.5, 0.5));
	lbl->setPosition(pos);
	lbl->retain();

	char *tx;
	//tx= fullPath(files::menu::play);
	tx= fullPath(files::menu::less);
	less = CCMenuItemImage::create(tx, tx, this, menu_selector(Spinbox::change));
	less->retain();
	//less->setRotation(180);
	less->setPosition(pos+ccp(-70, 0));

	tx = fullPath(files::menu::more);
	more = CCMenuItemImage::create(tx, tx, this, menu_selector(Spinbox::change));
	more->retain();
	more->setPosition(pos+ccp(70, 0));
	addTo(parent);

	//this->initWithTarget(this,menu_selector(Spinbox::change) );
	//this->setContentSize(CCSize(200, 100));
	e_changed = EM.get(events::com::SPB_CHANGED);
}

Spinbox::~Spinbox(){
	RELEASE(less)
	RELEASE(more)
	RELEASE(lbl)
	EVRELEASE(e_changed)
}

void Spinbox::change(CCObject *pSender){
	playSndBtn();//only plays sound if the change is triggered by tapping into one of the "buttons"
	this->setValue(
		this->val +
		(pSender == less?
			-this->step:
			this->step)
	);
}

int Spinbox::value(){
	return val;
}

int Spinbox::maximum(){
	return _max;
}

void Spinbox::setValue(const int &pVal){
	int nval = pVal;
	if(nval<_min) nval = _min;
	if(nval>_max) nval = _max;
	if(val==nval) return; //lol
	val = nval;
	lbl->setString(itos(val));
	e_changed->fire(this, &nval);
}

void Spinbox::removeFromParent(){
	lbl->removeFromParent();
	more->removeFromParent();
	less->removeFromParent();
}

void Spinbox::addTo(CCMenu *p){
	p->addChild(lbl);
	p->addChild(more);
	p->addChild(less);
	lbl->setOpacity(0);
	more->setOpacity(0);
	less->setOpacity(0);
	lbl->runAction(CCFadeIn::create(0.5));
	more->runAction(CCFadeIn::create(0.5));
	less->runAction(CCFadeIn::create(0.5));
}

void Spinbox::unload(){
	unloadTX(files::menu::more);
	unloadTX(files::menu::less);
	//todo unload bitmap font ¿?
}

