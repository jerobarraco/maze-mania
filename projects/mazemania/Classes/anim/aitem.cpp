#include <string.h> //strcpy
#include <cstdlib>

#include "cocos2d.h"
//#include "CCActionInterval.h"
#include "aitem.h"
#include "utils/Files.h"
//#define TOKANDCHECK(_p, _d) _p = strtok(NULL, _d); if(_p==NULL || *_p=='\n') return;
#define TOKANDCHECK(_p, _d, _r) _p = strtok(NULL, _d); if(_p==NULL || *_p=='\n' || *_p ==0) return _r;

const char del[]=",";

FrameDef readFrame(char *&p){//by reference ftw
	FrameDef f;
	//TOKANDCHECK(p, del, f);
	if(*p=='-'){
		f.sfx[0]=0;
	}else{
		strcpy(f.sfx, p);
	}
	TOKANDCHECK(p, del, f);
	f.t = atof(p);
	TOKANDCHECK(p, del, f);
	f.x = atoi(p);
	TOKANDCHECK(p, del, f);
	f.y = atoi(p);
	TOKANDCHECK(p, del, f);
	f.s = atof(p);
	TOKANDCHECK(p, del, f);
	f.a = atof(p);
	TOKANDCHECK(p, del, f);
	f.r = atof(p);
	return f;
}

void AItem::doFrame(){
	us fs = _fdefs.size();
	if(_frame >= fs) {
		if (loop && fs>1){
			_frame = 1; //it loops from frame 1 (skipping frame 0)
		}else{
			return;
		}
	}
	FrameDef f = _fdefs.at(_frame);

	if(f.sfx[0] != 0){
		play(f.sfx);
	}

	USING_NS_CC;
	CCSequence* ret = CCSequence::create(
		CCSpawn::create(
			CCMoveTo::create(f.t, ccp(f.x, f.y)),
			CCScaleTo::create(f.t, f.s, f.s),
			CCFadeTo::create(f.t, (GLubyte)(f.a*255)),//parenthesis are important like, a lot
			_frame==0?NULL:CCRotateBy::create(f.t, f.r),
			NULL
		),
		CCCallFunc::create(this, callfunc_selector(AItem::doFrame)),
		NULL
	);
	this->runAction(ret);
	_frame ++;
}

void AItem::reset(){
	_frame = 0;
	_state = AS_STOPPED;
	if (_fdefs.size()>0){
		FrameDef f = _fdefs.at(0);
		this->setPosition(ccp(f.x, f.y));
		this->setOpacity(f.a*255);
		this->setRotation(f.r);
	}
}

void AItem::unload(char *l){
	//itll modify your line! l=line of script (not texture)
	//char line[512];
	//strcpy(line, l);//to be able to use const char as param
	l = strtok(l, del);
	if(l == NULL || l[0]==0 || l[0]=='\n' || l[0]==1 ) return;
	unloadTX(l);
}

AItem::AItem(char *line):CCSprite(){//notice the by reference
	//strtok destroys the line but i dont care, that's why memory creation is left as a responsability by the callee
	//so it creates it only if necesary

	char *end = line+strlen(line)-1;
	char *p = strtok(line, del);
	//delay = atof(p);
	//p = strtok(NULL, del);
	strcpy(tx, p);
	this->initWithFile(fullPath(tx));//important for sprites
	this->autorelease();
	p = strtok(NULL, del);
	loop = atoi(p)!=0;
//	unsigned short len;
	while (p != NULL){
		p = strtok(NULL, del);//because strtok sucks too
		if( p >= end || p==NULL || p[0]==0 || p[0]=='\n' || p[0]=='#') break;//python i miss you :( clearer (no strlen should be needed
		_fdefs.push_back(readFrame(p));
		//if( p==NULL || (len = strlen(p) )==0 || p[len-1]=='\n') break;//python i miss you :( (notice the asignation of len inside the if!)
		//if( p==NULL || p[0]==0 || p[strlen(p)-1]=='\n') break;//python i miss you :( clearer (notice strlen can only happen if p!=NULL (because FYou thats why))
	}

	reset();
}

void AItem::go(){
	if (_state == AS_PLAYING)
		return; //no chance, this would implode
	if (_state == AS_PAUSED) {
		resumeSchedulerAndActions();
	}
	if (_state == AS_STOPPED){
		doFrame();
	}
	_state = AS_PLAYING;
}

void AItem::stop(){
	if(_state != AS_PLAYING) return;
	unscheduleAllSelectors();
	this->reset();
	_state = AS_STOPPED;//reset sets stopped but nothing like being explicitely redundant
}

void AItem::pause(){
	if(_state != AS_PLAYING) return;
	pauseSchedulerAndActions();
	_state = AS_PAUSED;
}

void AItem::onEnter(){
	cocos2d::CCSprite::onEnter();
	if(_state == AS_PAUSED){
		this->go();
	}
}

void AItem::onExit(){
	cocos2d::CCSprite::onEnter();
	if(_state == AS_PLAYING){
		this->stop();//cant pause on exit
	}
}
