#include <stdlib.h>     /* srand, rand */
#include "SimpleAudioEngine.h"
#include "Speed.h"
#include "utils/Utils.h"
#include "utils/Files.h"

Speed::Speed(Board *b, int xi, int yi):Item(b, xi, yi){
	up = (rand()/(float)RAND_MAX)>0.5;
	/*factor = 0.25+ ((rand()/(float)RAND_MAX)*1.5);
	this->type = (factor<=1.0) ? SPDT_UP: SPDT_DOWN;*/
	this->type = up ? SPDT_UP: SPDT_DOWN;
	this->factor = up? 0.5 : 1.5;

	preloadAudio(files::game::s_boosts[(int)type]);

	char *t = fullPath(files::game::boosts[(int)type]);
	this->initWithFile(t);

	e_activated = EM.get(events::game::items::BOOST);
}

void Speed::activate(Player *p){
	play(files::game::s_boosts[(int)type]);

	p->setSpeedFactor(factor);

	float s = this->up? 1.5 : 0.5;
	CCFiniteTimeAction *ac_scale = CCScaleTo::create(0.5, s);
	CCFiniteTimeAction *ac_release = CCCallFunc::create(this, callfunc_selector(Speed::removeFromParent));
	this->runAction(CCFadeOut::create(0.5));
	this->runAction(CCSequence::create(ac_scale, ac_release, NULL));
	//issue #160, no deberia pasar
	//[self runAction:[CCSequence actions:self.actions, newZoomAction,nil]];
	/*this->runAction(CCSpawn::create(
			this->actions,
			CCFadeOut::create(0.5),
			CCSequence::create(ac_scale, ac_release, NULL)
		)
	);*/
	e_activated->fire(this, p);
}

void Speed::unload(){
	for (int i = SPDT_UP; i<=SPDT_DOWN; i++){
		unloadSFX(files::game::s_boosts[i]);
	}
}
