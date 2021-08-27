#include "game.h"
#include "actions/CCActionEase.h"
#include "utils/Utils.h"
#include "utils/Files.h"

void Game::onRestartWave(void *self, void *sender, void *param){
	((Game*)self)->restartWave();
}

void Game::restartWave(){
	this->gstate = GS_GOVER;
	OPTIONS.wave--;
	this->setPaused(false);
}

void Game::nextWave(){
	OPTIONS.setForWave(OPTIONS.wave+1);
	OPTIONS.points = 0;//puedo como no resetear los puntos
	CCLog("Wave started:%d", OPTIONS.wave);
	b->restart();
}

void Game::onGameOver(void *self, void *sender, void *param){
	((Game*)self)->setPaused(true, *(int*)param);
	//((Game*)self)->doGameOver(*(int*)param);
}

void Game::onPause(void *self, void *sender, void *param){
	((Game*)self)->setPaused(true);
}

void Game::onResume(void *self, void *sender, void *param){
	((Game*)self)->setPaused(false);
}

void Game::onResumeApp(void *self, void *sender, void *param){
	//fix para el fix del bug en android
	//cuando la app se resume la musica se re-reproduce, si estabamos pausados, repausamos la musica
	Game* dis = (Game*)self;
	if(dis->gstate == GS_PAUSED){
		pauseAudio();
	}
}

void Game::unload(){
	//this is slow and dangerous, and not really necesary, so we wont use it by now
	Board::unload();
	GameOverMenu::unload();
}

void Game::setPaused(bool pause, int team){
	//sets the paused state and shows the game over menu if its paused
	//team is optional, GO_PAUSE by default, if different it means there has been a gameOver

	//si esta activo y se quiere despausar o viceversa (o sea si quiero pausar y ya esta pausado) no hago nada
	//comentado para probar.
	//if (this->active != pause)
	//	return;

	//pausar el board al principio trae menos problemas,
	//sobre todo porque el nextWave resetea el board,
	//y si despues le hacemos un setPause(false) daria problemas. lo cual no se da a la inversa :)

	b->setPaused(pause);

	if(pause){
		showGameOver(team);
		gstate = team == GO_PAUSE? GS_PAUSED: GS_GOVER;
	}else{
		if (gstate == GS_PAUSED){
			resumeAudio();
		}else{
			//esto detecta si estamos resumiendo desde un wave_over o de un pause
			//aprovecha el hecho de que cuando hay un gameover el boton de resume no se muestra. pero cuidado,
			//de confundirse los dos, habrá que crear el GS_WAVEOVER que seria malo porque para saber si se termino habria
			//que comprobar los dos estados, quizas mejor otro flaǵ
			if(gstate == GS_GOVER){
				nextWave();
			}
		}
		gstate = GS_PLAYING;
		//quitar gom
		if (gom){//chau gom
			CCFiniteTimeAction *ac_move = CCMoveTo::create(0.3f, ccp(0.0, GLOBALS.px_height));
			CCFiniteTimeAction *ac_remove = CCCallFunc::create(gom, callfunc_selector(GameOverMenu::removeFromParent));
			gom->runAction(CCSequence::create(ac_move, ac_remove, NULL));
		}
	}
}

Game::Game():LoadLayer(true){
	gom = NULL;
	e_restart_wave = e_game_over = e_resume = e_pause = e_resume_app = NULL;
	gstate = GS_DEMO;
	b = NULL;
}

void Game::load(){
	this->loadStart(4);

	this->loadStep();
	if(OPTIONS.modes[OPTIONS.M_DEMO]){
		gstate = GS_DEMO;
	}else{
		e_game_over = EM.subscribe(this, events::game::GAMEOVER, Game::onGameOver);
		e_pause = EM.subscribe(this, events::game::PAUSE, Game::onPause);
		e_resume = EM.subscribe(this, events::game::RESUME, Game::onResume);
		e_restart_wave = EM.subscribe(this, events::game::RESTART_WAVE, Game::onRestartWave);
		e_resume_app = EM.subscribe(this, events::app::RESUME, Game::onResumeApp);
		gstate = GS_PLAYING;
		gom = new GameOverMenu();
		gom->retain();
	}

	this->loadStep();
	CCSprite *backk = CCSprite::create(fullPath(files::game::bg));
	//TODO debería estar centrado al centro de la pantalla
	//backk->setAnchorPoint(ccp(0.5, 0.5));
	backk->setAnchorPoint(CCPointZero);
	backk->setPosition(CCPointZero);
	this->addChild(backk);

	this->loadStep();
	b = new Board();
	b->retain();

	this->loadStep();
	//TODO Limpiar el codigo de aca
	CCPoint verts[4];
	verts[0] = ccp(0, 0);
	verts[1] = ccp(0, b->h);
	verts[2] = ccp(b->w, b->h);
	verts[3] = ccp(b->w, 0);

	CCClippingNode *clip = CCClippingNode::create();
	//clip->setInverted(true);
	clip->setAlphaThreshold(0.0f);
	this->addChild(clip);

	CCNode *nodef = CCNode::create();//todo usar overlay
	CCDrawNode *cliprect = CCDrawNode::create();
	cliprect->setAnchorPoint(ccp(0.5, 0.5));
	cliprect->drawPolygon(verts, 4, ccc4f(0.92156863, 0.64313725, 0.35686275, 0.75), 1.0, ccc4f(1.0,1.0,1.0, 1.0));
	cliprect->setPosition(CCPointZero);
	nodef->addChild(cliprect);
	clip->setStencil(nodef);
	clip->addChild(b);

	clip->setPosition(ccp(
		(GLOBALS.px_width - b->w)/2,//todo usar cent_*
		(GLOBALS.px_height - b->h)/2
	));

	this->loadStop();
}

Game::~Game(){
	CCLOG("game is dead");
	EVUNSUB(e_game_over)
	EVUNSUB(e_pause)
	EVUNSUB(e_resume)
	EVUNSUB(e_restart_wave)
	EVUNSUB(e_resume_app)

	RELEASE(b)

	RERELEASE(gom)//re because it can not be attached to this (the parent)
	removeAllChildren();
}

void Game::keyBackClicked(){
	//TODO lo mas probable es que esto pase casi todo a la otra escena (gamescene)
	CCLog("back button! kill me! state is %d", gstate);
	switch(gstate){
		case GS_GOVER:
			EM.get(events::scenes::MENU)->fire(this, NULL, true);
			break;
		case GS_PAUSED:
			EM.get(events::game::RESUME)->fire(this, NULL, true);
			break;
		case GS_PLAYING:
			EM.get(events::game::PAUSE)->fire(this, NULL, true);
			break;
		/*case GS_DEMO: not managed by us then, we are dummies*/
		default: break;
	}
}

void Game::showGameOver(int team){
	/*switch(team){
		case GO_PAUSE:
			gstate = GS_PAUSED;
			break;
		default:
			gstate = GS_GOVER;
			break;
	}
	 no need for this because they're all gameovers
	case GO_GAMEOVER:
	case GO_TIMEOUT:
		gstate = GS_GOVER;
		break;*/
	if(gstate != GS_PLAYING){ //TODO #101 esto esta pasando porque el wave timer no se pausa
		CCLOG("ERROR! ya estabamos en gameover!");
		return;
	}

	#ifdef DEBUG
	if(!gom){//debug code, when we are sure this is safe, remove this
		CCLOG("Error! no gom created! This is outrageous!");
		gom = new GameOverMenu();
		gom->retain();
	}
	#endif
	this->removeChild(gom);//just in case. shouldn't happen at all
	this->addChild(gom, 120);
	gom->show(team);

	// en caso que la condicion en el setPause(false) de problemas aca es un buen lugar para poner el flag
	//if(team == GO_WAVEOVER) {wave_over=true;}

	CCMoveTo *ccMove = CCMoveTo::create(1, ccp(0.0, 0.0));
	CCEaseBounceOut *ease;
	ease = CCEaseBounceOut::create(ccMove);
	gom->setPositionY(GLOBALS.px_height);
	gom->runAction(ease);
	CCLog("game over state:%d team:%d waven:%d", gstate, team, OPTIONS.wave);
}
