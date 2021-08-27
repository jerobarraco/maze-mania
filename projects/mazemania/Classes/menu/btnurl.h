#ifndef BTNURL_H
#define BTNURL_H
#include "cocos2d.h"
class BtnUrl:public cocos2d::CCMenuItemImage{
	char *_url;
public:
	void onClick(CCObject* pSender);
	BtnUrl(const char *url, const char *tx);
	~BtnUrl();
};

#endif // BTNURL_H
