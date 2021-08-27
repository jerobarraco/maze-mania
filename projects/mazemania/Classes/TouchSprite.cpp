/*
 * TouchSprite.cpp
 *
 *  Created on: 07/02/2014
 *      Author: nande
 */

#include "TouchSprite.h"
USING_NS_CC;

TouchSprite::TouchSprite(int prio, bool swallows, int hitbox)
		:CCSprite(), CCTargetedTouchDelegate(){
	//deprecated use the other
	this->autorelease();
	this->swallows = swallows;//1313
	this->prio = prio;
	this->hitbox = hitbox;
	//cocos2d::CCLog("touch initialized");
}

TouchSprite::TouchSprite(const char *tx, const int prio, const bool swallows)
	:CCSprite(), CCTargetedTouchDelegate(){
	this->initWithFile(tx);
	this->autorelease();
	this->swallows = swallows;//1313
	this->prio = prio;
	int width = this->getContentSize().width;
	int height = this->getContentSize().height;
	this->hitbox = width>height?width:height;//max
}

void TouchSprite::onEnter(){
	// before 2.0:
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
	// since 2.0:
	cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, this->prio, this->swallows);
	CCSprite::onEnter();
}

void TouchSprite::onExit(){
	// before 2.0:
	// CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	// since 2.0:
	cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}

bool TouchSprite::onTouch(){
	//override me
	return false;//returning true will swallow the event
}

bool TouchSprite::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event){
	//CCLOG("%s", "check touched");
	//TODO add the posibility to do this check on touchend for onUp button click
	if(this->touchsMe(touch)){
			return this->onTouch();
	}
	return false;
}

void TouchSprite::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event){
    //lol
}

void TouchSprite::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event){
	//CCLOG("btn_pause UnTouched");
	//moar lol (read todo on ccTouchBegan)
}

bool TouchSprite::touchsMe(cocos2d::CCTouch* touch){
	//optionally override me too
	CCPoint p;
	// convert the touch object to a position in our cocos2d space
	//cocos2d::CCLog("++++++++prev x:%f, y:%f", touch->getLocation().x, touch->getLocation().y);
	p = this->getParent()->convertTouchToNodeSpace(touch);
	//cocos2d::CCLog("++++++++after x:%f, y:%f", p.x, p.y);
	float d = ccpDistance(this->getPosition(), p);
	//CCLog("dis %f", d);
	return ( d < hitbox);
	// this works also , notice the lousy rad on the distance.
	// 	float d = ccpDistance( ccp(this->rad, this->rad), this->convertTouchToNodeSpace(touch));
	//	return ( d< this->rad);
}
