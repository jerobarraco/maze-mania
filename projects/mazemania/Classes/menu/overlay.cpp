#include "overlay.h"
#include "cocos2d.h"
#include "utils/Files.h"

//Overlay::Overlay():cocos2d::CCDrawNode(){
//	this->init();
//	this->autorelease();
//	SkinDef*sd = skinDef();
//	this->setAnchorPoint(ccp(0.5, 0.5));
//	cocos2d::CCPoint verts[4];
//	verts[0] = ccp(0, 0);
//	verts[1] = ccp(0, GLOBALS.px_height);
//	verts[2] = ccp(GLOBALS.px_width, GLOBALS.px_height);
//	verts[3] = ccp(GLOBALS.px_width, 0);
//	this->drawPolygon(verts, 4, sd->overlay, 1.0, sd->boardBorder);
//}
Overlay::Overlay(const bool &swallow):cocos2d::CCLayerColor(){//cocos2d::CCDrawNode(){
	_swallow = swallow;
	SkinDef *sd = skinDef();
	cocos2d::ccColor4B ccc;
	ccc.a = sd->overlay.a*255;//fuck you cocos
	ccc.r = sd->overlay.r*255;
	ccc.g = sd->overlay.g*255;
	ccc.b = sd->overlay.b*255;

	this->initWithColor(ccc);
	this->setAnchorPoint(ccp(0.5, 0.5));
	this->autorelease();

	setTouchEnabled(_swallow);
	//setTouchPriority(-10000);
	setTouchMode(cocos2d::kCCTouchesOneByOne);//yes it IS important

	//setTouchPriority(-1);
}

bool Overlay::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	//om nom nom
	return true;

}
