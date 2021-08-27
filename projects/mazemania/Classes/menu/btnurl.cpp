#include "btnurl.h"
#include "utils/Files.h"
USING_NS_CC;//necesario para el menu_selector ¬¬
void BtnUrl::onClick(CCObject *pSender){
	cocos2d::CCApplication::openURL(_url);
}

BtnUrl::BtnUrl(const char *url, const char *tx):
	cocos2d::CCMenuItemImage(){
	initWithNormalImage(tx, tx, tx, this, menu_selector(BtnUrl::onClick));
	this->autorelease();
	_url = new char[strlen(url)+1];
	_url[0]=0;
	strcpy(_url, url);
}

BtnUrl::~BtnUrl(){
	delete[] _url;
}
