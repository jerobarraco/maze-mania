#include "animation.h"
#include <stdio.h>
#include <errno.h>
#include "utils/Files.h"
#include "menu/btnback.h"

//const unsigned short AN_C_LINE=1024;

VAItem *Animation::loadItems(const char *path, const bool load){
	VAItem *i = NULL;
	if(load){
		i = new VAItem();
		CCLOG("Loading animation '%s'", path);
	}else{
		CCLOG("UNLoading animation '%s'", path);
	}
	char *fpath = fullPath(path);
	if (!fpath) return i;
	unsigned long sz = 0;
	char *buf = (char*) cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fpath, "r", &sz);
	//&sz because parameter for reference are too mainstream
	//same reason for the unsigned char
	if(sz==0){
		CCLOG(" - Failed un/loading animation");
		return i;
	}
	//char line[AN_C_LINE];
	char * st = buf;//trick
	char * line = NULL;
	char * end=buf+sz-1;
	AItem *fr;
	unsigned long len;
	while( (line=strtok(st, "\n"))!=NULL){//will it blend?//notice asignation
		st = NULL;//to trick the strtok
		//maldita incosistencia!!!!
		if(line == NULL || line>=end || line[0]==0 || line[0]=='#' || line[0]=='\n' ){//no idea why if you leave 2 enters at the end of the file it makes this error
			//#CCLOG(" - empty line, continuying");
			continue;
		}
		len = strlen(line);
		if(load){
			fr = new AItem(line);
			fr->retain();
			i->push_back(fr);
		}else{
			AItem::unload(line);
		}
		st = line+len+1;
		if (st >= end) break;//importante para evitar memory corruption
	}

	delete []buf;
	return i;
}

void Animation::unload(const char *f){
	CCLOG("Trying to unload animation '%s'", f);
	//to save space and because the loading part is so difficult, i use the same function, loadItems.
	Animation::loadItems(f, false);
}

void Animation::unload(){
	CCLOG("Unloading all animations!");
	const char * anims[] = {
		files::help::anims[0], files::help::anims[1], files::help::anims[2], files::help::anims[3],
		files::help::anims[4], files::help::anims[5], files::help::anims[6],
		files::help::a_title, files::intro::a_logo, files::start_menu::a_logo, files::menu::a_title,
		files::gameover::a_winner, files::gameover::a_cleared, files::gameover::a_finished,
		files::gameover::a_players[0], files::gameover::a_players[1], files::gameover::a_players[2],
		files::gameover::a_players[3], files::gameover::a_players[4], files::gameover::a_players[5],
	};
	us cant = 20;
	for (us i =0; i<cant; i++){
		Animation::unload(anims[i]);
	}
}

void Animation::go(){
	if(!_items) return;
	/*For(it, *_items){
		(*it)->go();
		this->addChild(*it);
	}*/

	for (VAItem::iterator it = _items->begin(); it != _items->end(); it++){
		this->addChild(*it);
		((AItem*)*it)->go();
	}
}

Animation::Animation(const char *f, const bool start):CCNodeRGBA(){//RGBA should allow for fade but doesn't...
	//doesnt set autorelease!!!
	//_frame = 0;
	this->init();
	this->autorelease();
	_items = Animation::loadItems(f);
	if(start) go();
}

Animation::~Animation(){
	CCLOG("Animation done");
	for (VAItem::iterator it = _items->begin(); it != _items->end(); it++){
		((AItem*)*it)->release();
	}
	/*For(it, *_items){
		(*it)->release();
	}*/

	_items->clear();
	delete _items;
	this->removeAllChildren();
}

void Animation::setOpacity(GLubyte opaque){//a test to see if it can fade, it doesnt
	USING_NS_CC;
	cocos2d::CCNodeRGBA::setOpacity(opaque);//not needed
	CCArray* childs = NULL;
	CCObject* child = NULL;
	childs = this->getChildren();
	if (childs) {
		CCARRAY_FOREACH(childs, child){
			CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(child);
			if (pRGBAProtocol){
				//this makes fading possible
				//because it sets the new opacity relative to its opacity
				pRGBAProtocol->setCascadeOpacityEnabled(true);
				pRGBAProtocol->updateDisplayedOpacity(opaque);
			}
		}
	}
}

void Animation::onEnter(){
	cocos2d::CCNodeRGBA::onEnter();
	for (VAItem::iterator it = _items->begin(); it != _items->end(); it++){
		((AItem*)*it)->go();
	}
	/*For(it, *_items){
		(*it)->go();
	}*/
}

void Animation::onExit(){
	cocos2d::CCNodeRGBA::onExit();
	for (VAItem::iterator it = _items->begin(); it != _items->end(); it++){
		((AItem*)*it)->pause();
	}
	/*For(it, *_items){
		(*it)->pause();
	}*/

}
