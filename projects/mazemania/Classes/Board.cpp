/*
 * Board.cpp
 *
 *  Created on: 27/03/2014
 *      Author: Moongate
 */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
/*on randoms:
 * v1 = rand() % 100;         // v1 in the range 0 to 99
v2 = rand() % 100 + 1;     // v2 in the range 1 to 100
v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014
rand Return Value
 An integer value between 0 and RAND_MAX.
 */
#include "misc_nodes/CCClippingNode.h"
#include "SimpleAudioEngine.h"

#include "Board.h"
#include "utils/Utils.h"
#include "utils/Files.h"
#include "Door.h"
#include "Player.h"
#include "items/Goal.h"
#include "items/Bomb.h"
#include "items/GameTimer.h"
#include "items/wavetimer.h"
#include "items/Speed.h"
#include "items/Point.h"

void Board::initRandomPos(){
	//este codigo va dedicado a todos los fans de c++ ahi afuera
	int tiles = tw*th*2;
	int *random_pos_prev = new int[tiles];
	//initialize the random shiet
	srand (time(NULL));
	int c = 0;
	int sw = (tw)*2, sh=(th)*2;

	for (int i = 1; i< sw; i+=2){
		for (int j = 1; j< sh; j+=2){
			random_pos_prev[c++] = i;
			random_pos_prev[c++] = j;
		}
	}

	if (c_random_pos>0){
		delete[] random_pos;
	}

	c_random_pos = c;
	if(c_random_pos>0){
		random_pos = new int[c_random_pos];
	}
	last_random =0;
	//now lets shuffle it
	//--- Shuffle elements by randomly exchanging each with one other.
	//modified to shuffle and add elements to the other list
	for (int i=0; i<c-2; i+=2) {
		int r = i + (rand() % (c-i)) ; // Random remaining position.
		if (r%2!=0) r--;//makes it odd (par)
		/*int tempa = random_pos_prev[i];
		 int tempb = random_pos_prev[i+1];
		 random_pos_prev[i] = random_pos_prev[r];
		 random_pos_prev[i+1] = random_pos_prev[r+1];
		 random_pos_prev[r] = tempa;
		 random_pos_prev[r+1] = tempb;*/

		//pass a random item to the list
		random_pos[i] = random_pos_prev[r];
		random_pos[i+1] = random_pos_prev[r+1];
		//now pass the exchanged item to the remaining section of the prev list
		//, no need to keep the prev item because we wont touch it anymore
		random_pos_prev[r] = random_pos_prev[i];
		random_pos_prev[r+1] = random_pos_prev[i+1];
	}
	delete[] random_pos_prev;
}

void Board::getRandomPos(int &xi, int &yi, bool noCenter){
	//if (c_random_pos<=0) return;

	unsigned short c =0;
	while(c <c_random_pos){
		xi = random_pos[last_random++];
		yi = random_pos[last_random++];
		last_random %= c_random_pos;
		if(!(noCenter && isOnCenter(xi, yi)))
			return;
		c+=2;
	}
	CCLog("There was no random pos available!");
	//si hay error
	xi = yi=1;
}

//offsets from center //left top right bottom

//// 3*3 square
static const short _c_offs[4] =  {-3, 3, 3, -3};
static const unsigned short _c_cdoor = 12;
static const short _cdoors[_c_cdoor][2]={
{_c_offs[0]+1, _c_offs[1]  }, {0, _c_offs[1]}, {_c_offs[2]-1, _c_offs[1]  },
{_c_offs[0]  , _c_offs[1]-1},                  {_c_offs[2]  , _c_offs[1]-1},
{_c_offs[0]  ,            0},                  {_c_offs[2]  ,            0},
{_c_offs[0]  , _c_offs[3]+1},                  {_c_offs[2]  , _c_offs[3]+1},
{_c_offs[0]+1, _c_offs[3]  }, {0, _c_offs[3]}, {_c_offs[2]-1, _c_offs[3]  },
};//puntos donde encierran las puertas


/*//// 3*2 rectangle (thanks playbook)
static const short _c_offs[4] =  {-3, 3, 3, -3};
static const unsigned short _c_cdoor = 12;
static const short _cdoors[_c_cdoor][2]={
{_c_offs[0]+1, _c_offs[1]  }, {0, _c_offs[1]}, {_c_offs[2]-1, _c_offs[1]  },
{_c_offs[0]  , _c_offs[1]-1},                  {_c_offs[2]  , _c_offs[1]-1},
{_c_offs[0]  ,            0},                  {_c_offs[2]  ,            0},
{_c_offs[0]  , _c_offs[3]+1},                  {_c_offs[2]  , _c_offs[3]+1},
{_c_offs[0]+1, _c_offs[3]  }, {0, _c_offs[3]}, {_c_offs[2]-1, _c_offs[3]  },
};//puntos donde encierran las puertas
*/
/*
		_c_offs	@0x417c3c	short [4]
			[0]	-3	short
			[1]	3	short
			[2]	3	short
			[3]	-3	short
		_cdoors	@0x6231e0	short [12][2]
			[0]	@0x6231e0	short [2]
				[0]	-2	short
				[1]	3	short
			[1]	@0x6231e4	short [2]
				[0]	0	short
				[1]	3	short
			[2]	@0x6231e8	short [2]
				[0]	2	short
				[1]	3	short
			[3]	@0x6231ec	short [2]
				[0]	-3	short
				[1]	2	short
			[4]	@0x6231f0	short [2]
				[0]	3	short
				[1]	2	short
			[5]	@0x6231f4	short [2]
				[0]	-3	short
				[1]	0	short
			[6]	@0x6231f8	short [2]
				[0]	3	short
				[1]	0	short
			[7]	@0x6231fc	short [2]
				[0]	-3	short
				[1]	-2	short
			[8]	@0x623200	short [2]
				[0]	3	short
				[1]	-2	short
			[9]	@0x623204	short [2]
				[0]	-2	short
				[1]	-3	short
			[10]	@0x623208	short [2]
				[0]	0	short
				[1]	-3	short
			[11]	@0x62320c	short [2]
				[0]	2	short
				[1]	-3	short
*/

bool Board::isOnCenter(const int xi, const int yi){
	//this is only ON inside. that means the doors on the border of the center
	//are not counted as in
	//works well with items and doors inside center
	if (
			(xi>ccx+_c_offs[0]) && (xi<ccx+_c_offs[2])
			&&(yi>ccy+_c_offs[3]) && (yi<ccy+_c_offs[1])
		){//dont close doors in the center
		return true;
	}
	return false;
}

bool Board::shouldCloseDoor(const int xi, const int yi){
	//set the center doors to closed

	//the doors inside the center box cant be closed
	//dont close doors in the center
	if (isOnCenter(xi, yi)){
		return false;
	}

	//random closing
	//i do this test here because its faster than the for below
	if(rand()%100>70){//dante, dejo a tu discresion elegir el porcentaje correcto
		//30% chance of getting closed
		return true;
	}

	//all this pointer crap is just to iterate this array faster, depends on the fact that the array is declared as [][]
	//welcome to hell++
	short *cdp = (short*)(_cdoors);//pointer to closed doors
	unsigned short c =0;
	int px, py;
	while (c<_c_cdoor){//iterates over array and compares points
		px = *(cdp++);//done separatedly because the if can skip one condition evaluation so it does not advance the pointer
		py = *(cdp++);
		if ((xi == ccx+px) && (yi == ccy+py)){
			return true;
		}
		c++;
	}
	return false;
}
const int CB_SPTURNS=4;//bomb, boost, point,
void Board::spawnStuff(float f){
	//esto anda pero no se si tan bien como yo quisiera,. debug needed
	//spawnea cosas, esto es llamado por un timer
	//cada vez que se llama espawnea algo segun la cuenta.
	if (!active) return;

	//lo mismo que spawn_turns = ++spawn_turns % CB_SPTURNS pero mas rapido y entendible
	spawn_turn++;
	if(spawn_turn>=CB_SPTURNS){
		spawn_turn =0;
	}

	switch (spawn_turn){
		case 0:
			if(c_spawn_boost>0){
				c_spawn_boost--;
				addBoost();
				break;//notar que el break esta acá adentro
			}else{
				c_spawn_boost=0;//o sea que si no spawnea nada ira al proximo caso
			}
		case 1:
			if(c_spawn_bombs>0){
				c_spawn_bombs--;
				addBomb();
				break;
			}else{
				c_spawn_bombs=0;
			}
		case 2:
			if (c_spawn_points>0){
				c_spawn_points--;
				addPointItem();
				break;
			}
			else{
				c_spawn_points=0;
			}
		default: break; //lol
	}
}

void Board::onBoostActivated(void *self, void *sender, void *param){
	//todo mover todo esto de la animación al item mismo
	((Board*)self)->resetBoost((Item*)sender);
}

void Board::onBombActivated(void *self, void *sender, void *param){
	((Board*)self)->resetBomb((Item*)sender);
}

void Board::onPointActivated(void *self, void *sender, void *param){
	((Board*)self)->resetPoint((Item*)sender);
}

void Board::setPaused(bool pause){
	//pausa o despausa el juego
	//CCArray* childs = this->getChildren();
	//if((this->active && ! pause) || !this->active && pause)
	if (this->active != pause)//si esta activo y se quiere despausar o viceversa (o sea si quiero pausar y ya esta pausado) no hago nada
		return;

	this->active = !pause;
	if(pause){
		pauseAudio();
		if(this->btnPause){
			this->removeChild(this->btnPause);
		}
	}else{
		if(this->btnPause){//el btnPause se crea solo si no es demo
			this->addChild(this->btnPause);
		}
	}


	CCObject* child;
	Item *i;
	CCARRAY_FOREACH(this->items, child){
		i = (Item*) child;
		i->setPaused(pause);
	}

	Player *p;
	CCARRAY_FOREACH(this->pjs, child){
		p = (Player*) child;
		p->setPaused(pause);
	}
	Door *d;
	CCARRAY_FOREACH(this->doors, child){
		d = (Door*) child;
		d->setPaused(pause);
	}
}

void Board::checkPlayers(){
	//due to the complexity of the winning /lose conditions the best place to check it all is in one funciton
	//probably this will get bloated and solw but prolly faster than having it separated in several functions
	/*End Game Conditions
	Race Mode:
		Win: When the first unit reaches the goal
		Lose: - (goal doesnt makes everyone lose)

	Swarm Mode:
		Win : -
		Lose : -

	Timer:
		Win: - (Timer doesnt make anyone win)
		Lose: When time runs out. Everybody loses for -> Timeout

	Bomb:
		Win:
			Si + de 1 Team:
				solo 1 team vivo
				y no es wave
		Lose:
			Si 1 team (Alone play) (con o sin swarm)
				0 unidades vivas -> gameover
	*/
	//First lets check who's alive
	int teams_alive=0;
	CCObject *o;
	Player *pj, *last=NULL;
	CCARRAY_FOREACH(this->pjs, o){
		pj = (Player*) o;
		if (pj->alive){
			if(last){
				if(last->team != pj->team){
					teams_alive ++;//lets assume that all swarm units are created, stored and iterated in order of team
					pj = last;
				}
			}else{
				last = pj;
				teams_alive = 1;
			}
		}
	}

	//now the situation
	int winner;
	if(teams_alive==0){
		//he died (of bombing or other stuff) and was playing against himself or with others but everybody died, lol
		//tams_alive == 0 implies units == 0 too
		winner = GO_GAMEOVER;
		e_game_over->fire(this, &winner);
	}else if((teams_alive == 1) && (cplayers>1) && !OPTIONS.modes[OPTIONS.M_WAVE]){
		//playing vs and only one team remains, but it's not wave
		//if only one team is alive, it won
		e_game_over->fire(this, &last->team);
	}
}

//TODO esto no esta declarado adentro de board
static void onPlayDied(void *self, void* sender, void* player){
//	Player* p = (Player*) player;
	Board* diz = (Board*) self;

	//CCLog("El sender %li me dice que el jugador %d murio", (long) sender, p->team	);
	diz->checkPlayers();
	//diz->removeChild((Item*) player);//will remove himself after the animation
	//should i remove it from player array?
}

Player* Board::lastAlive(){
	//returns a player only if its the last one alive, else returns null
	CCObject *o;
	Player *pj, *last=NULL;

	CCARRAY_FOREACH(this->pjs, o){
		pj = (Player*) o;
		if (pj->alive){
			if(last){
				if(last->team != pj->team){
					return NULL; //not the only one
				}
			}else{
				last = pj;
			}
		}
	}
	return last;
	//if last == null -> everybody's dead lol
}

Item* Board::itemAt(int xi, int yi){
	CCObject *o;
	Item * i;
	CCARRAY_FOREACH(this->items, o){
		i = (Item*) o;
		if(i->xi==xi && i->yi ==yi){
			return i;
		}
	}
	return NULL;
}

void Board::checkItem(Player *p, int xi, int yi){
	Item * it = this->itemAt(xi, yi);
	if(!it) return;
	it->activate(p);
}

Door* Board::doorAt(int xi, int yi){
	CCObject *o;
	Door * d;
	//TODO OPTIMIZE (later)
	//this is slow and ugly and sucks but as the doors arent layed out symetrically there's no easy way to make a SIMLPE
	//calculation. also optimization is not the priority now
	CCARRAY_FOREACH(this->doors, o){
		d = (Door*) o;
		if(d->xi==xi && d->yi ==yi){
			return d;
		}
	}
	return NULL;
}

Board::Board():CCNode(){
	this->init();//does nothing but is good
	this->setAnchorPoint(ccp(0.5f, 0.5f));
	this->autorelease();
	//this->initWithFile("BG.png");
	btnPause = NULL;

	active=false;
	spawn_turn=w=tw=h=th=cplayers= 0;
	doors=pjs=items= NULL;
	c_random_pos = 0;//means its not allocated

	// Events
	//subscribed
	e_player_died = EM.subscribe(this, events::game::PLAYER_DIED, onPlayDied);
	e_on_boost = EM.subscribe(this, events::game::items::BOOST, onBoostActivated);
	e_on_bomb = EM.subscribe(this, events::game::items::BOMB, onBombActivated);
	e_on_point = EM.subscribe(this, events::game::items::POINT, onPointActivated);
	//to fire
	e_game_over = EM.get(events::game::GAMEOVER);
	//jebus powered!
	this->load();
}

void Board::clean(){
	active = false;
	unschedule(schedule_selector(Board::spawnStuff));
	RELEASE(doors)
	RELEASE(pjs)
	RELEASE(items)
	RELEASE(btnPause)
	this->removeAllChildren();

	if(c_random_pos>0){
		delete[] random_pos;
		random_pos = NULL;
		c_random_pos =0;
	}
}

void Board::restart(){
	this->clean();
	this->load();
}

static us B_ZITEM = 10;
static us B_ZPJ = 300;//300 items, doors, corners
void Board::load(){
	int steps = 11;
	EM.get(events::loader::START)->fire(this, &steps, true);
	Event *estep = EM.get(events::loader::STEP);

	// Preload SFX
	estep->fire(this, NULL);
	for (us i = 0; i<2; i++){
		preloadAudio(files::game::s_doors[i]);
	}

	#ifdef DEBUG
	//TODO esto es para depurar quitarlo despues
	for (int i = 0; i<20;i++){
		if(OPTIONS.overflow[i]!=54){
			CCLog("There has been an overflow in options!!!!");
			break;
		}
	}
	#endif

	estep->fire(this, NULL);
	active = true;
	doors = new CCArray(50);
	doors->autorelease();
	doors->retain();

	pjs = new CCArray(50);
	pjs->autorelease();
	pjs->retain();

	items = new CCArray(50);
	items->autorelease();
	items->retain();

	int SPS = SPRITE_RAD *2;
	this->tw = GLOBALS.px_width/SPS;
	this->th = GLOBALS.px_height/SPS;
	this->w = tw * SPS;
	this->h = th * SPS;
	// the center
	this->ccx=tw;
	this->ccy=th;
	if (ccx%2 == 0){
		ccx++;
	}
	if(ccy%2 == 0){
		ccy--;
	}

	estep->fire(this, NULL);
	initRandomPos();

	estep->fire(this, NULL);
	// Background
	CCDrawNode* rectNode = CCDrawNode::create();
	rectNode->setAnchorPoint(ccp(0.5, 0.5));
	verts[0] = ccp(0, 0);
	verts[1] = ccp(0, h);
	verts[2] = ccp(w, h);
	verts[3] = ccp(w, 0);
	//TODO mover esto a un archivo de configuracion

	//cocos2d::ccColor4F cBorder;
	SkinDef *sd = skinDef();
	rectNode->drawPolygon(verts, 4, sd->boardBG, 1.0, sd->boardBorder);
	rectNode->setPosition(CCPointZero);
	this->addChild(rectNode);

	estep->fire(this, NULL);
	//Doors
	//pos is in coords, not pixels!
	CCPoint pos = ccp(0, SPRITE_RAD), horoff=ccp(SPRITE_RAD, +SPRITE_RAD);
	/*Doors*/
	int xi=0, yi=0;//temporary points
	for (us j =0; j< tw; j++){
		yi = 1;
		for (us i =0; i<th; i++){
			//horizontal
			if(i<th-1)
				addDoor(false, pos + horoff, xi+1, yi+1);

			if ( j>0 )
				addDoor(true, pos, xi, yi);

			pos = pos + ccp(0, SPS);
			yi += 2;
		}
		pos = ccp(pos.x+SPS, SPRITE_RAD);
		xi+=2;
	}
	#ifdef DEBUG
		this->addDoor(false, this->coordToPos(1,0), 1, 0, DS_WALL);
		this->addDoor(false, this->coordToPos(3,0), 3, 0, DS_WALL);
		this->addDoor(false, this->coordToPos(5,0), 5, 0, DS_WALL);
	#endif
	doors->reduceMemoryFootprint();

	estep->fire(this, NULL);
	//Corners
	CCSprite * sp;
	int corrad = 16;
	CCPoint pts[]= {
		verts[0]+ccp(corrad, corrad), verts[1]+ccp(corrad, -corrad),
		verts[2]+ccp(-corrad, -corrad), verts[3]+ccp(-corrad, corrad)
	};

	for(us i = 0; i< 4; i++){
		sp = cocos2d::CCSprite::create(fullPath(files::game::corner));
		sp->setPosition(pts[i]);// PORQUE TIENE EL ANCLA AL CENTRO?!
		//sp->setAnchorPoint(ccp(0,0));el anchor define posicion y rotacion, que fome
		sp->setRotation(90*i);
		addChild(sp);
	}


	///////////
	///Items///
	///////////

	estep->fire(this, NULL);
	//Goal
	Item *tmpi = NULL;
	if (OPTIONS.modes[OPTIONS.M_GOAL]){
		getRandomPos(xi, yi, true);
		tmpi = new Goal(this, xi, yi);
		items->addObject(tmpi);
		this->addChild(tmpi);
	}

	estep->fire(this, NULL);
	//BOOSTS
	c_spawn_boost = OPTIONS.max_boosts;

	//BOMBS
	c_spawn_bombs = OPTIONS.max_bombs;
	/*
	estep->fire(this, NULL);
	//descomentar para respawear tutti al principio.
	 c_spawn_bombs =0;
	if (OPTIONS.modes[M_BOMB]){
		//int cBomb= rand() %4;
		//cBomb = 100;//test
		c_spawn_bombs = OPTIONS.max_bombs;
		int cBomb = 4;
		for (int i=0; i< cBomb; i++){
			//tmpi = new Bomb(this, tw+3, 1);
			this->addBomb(true);
		}
	}*/
	//Points
	c_spawn_points = OPTIONS.modes[OPTIONS.M_WAVE]?OPTIONS.max_ipoints:0;
	/*estep->fire(this, NULL);

	c_spawn_points =0;
	if (OPTIONS.modes[M_WAVE]){
		c_spawn_points =  C;

		int cPoints = 4;
		for (int i=0; i< cPoints; i++){
			this->addPointItem();
		}
	}*/

	//estep->fire(this, NULL);
	//TIMER
	if (OPTIONS.modes[OPTIONS.M_TIMER]){
		tmpi = new GameTimer(ccx-1, ccy+1); // ""centro"
		this->addChild(tmpi);
		items->addObject(tmpi);
	}

	estep->fire(this, NULL);
	if (OPTIONS.modes[OPTIONS.M_WAVE]){
		//WaveTimer
		tmpi =new WaveTimer(ccx+1, ccy+1);//al lado del timer
		this->addChild(tmpi);
		this->items->addObject(tmpi);//this allows to be paused
		//(if it creates problems (because items are checked for collisions) move to its own array (with the game timer)
	}

	items->reduceMemoryFootprint();
	// END ITEMS //

	estep->fire(this, NULL);
	//if its really playing and not just a demo board
	if(!OPTIONS.modes[Options::M_DEMO]){
		play(files::game::m_bgm[0], false, true, false);

		//Pause Button
		btnPause = new BtnPause();
		//CCPoint posi = this->coordToPos(tw*2, th*2);
		//posi = posi + ccp(-SPRITE_RAD+40, -SPRITE_RAD+40);//to align to sprite bottom (40 = 80/2)
		//btnPause->setPosition(posi);

		pos = ccp(25, this->h-25);

		//pos = ccp(SPRITE_RAD+40, -SPRITE_RAD+40);//to align to sprite bottom (40 = 80/2)
		btnPause->setPosition(pos);
		btnPause->retain();
		this->addChild(btnPause);
	}

	estep->fire(this, NULL);
	// Players
	int units = OPTIONS.units;
	//TODO calcular cplayers desde el options penabled
	cplayers = OPTIONS.cplayers;
	ctotunits = OPTIONS.cplayers*units;

	int teams[PLAYER_MAX]= {0, 1, 2 ,3 ,4 ,5};
	int tti =0;
	for (us i = 0; i<PLAYER_MAX; i++){
		if(OPTIONS.penabled[i]){
			teams[tti++] = i; //stores the enabled team so i dont have to modify the for below
		}
	}

	//int dir = 0;
	Direction dir;
	int puntos[6][2] = {
		{0,-2}, {0,2}, {-2,-2},
		{-2,2}, {2,-2},{2,2}
	};
	Direction dirs[6] = {
		RIGHT, LEFT, RIGHT, LEFT, RIGHT, LEFT
	};
	for (us i = 0 ; i< cplayers; i++){
		dir = dirs[i];
		for (int j = 0; j<units; j++){
			xi = ccx + puntos[i][0];
			yi = ccy + puntos[i][1];
			Player *p = new Player(this, teams[i], xi, yi, rotateDirection(dir, j));
			//para que cuando son muchos no se superpongan todos, horrible
			//p->setSpeedFactor(1-(0.15*j));
			p->speedf = 1.0-(0.15*j);//seteamos el speedf diréctamente para evitar que se anime.
			pjs->addObject(p);
			this->addChild(p, B_ZPJ-(i*units+j));
			//para que se muestren arriba de las bombas siempre
			//y el primero quede arriba de los demas
		}
	}

	pjs->reduceMemoryFootprint();
	this->schedule(schedule_selector(Board::spawnStuff), 2);

	EVRELEASE(estep)
	EM.get(events::loader::STOP)->fire(this, NULL, true);
}

void Board::unload(){
	//this function is to FORCE the unload of certain stuff,
	//use with care! unloading too much will cause the game to run slower
	//mostly usefull when changing the skin

	Door::unload(); //unload it for the skin change
	Bomb::unload();
	Goal::unload();
	Speed::unload();
	Point::unload();
	GameTimer::unload();
	WaveTimer::unload();
	//unload gameover music, lol yea, when cocos implement it
}

void Board::addDoor(bool vertical, CCPoint pos, int xi, int yi, int type){
	Door * d = new Door(vertical, type, xi, yi);
	d->setPosition(pos);
	this->addChild(d);
	doors->addObject(d);
	if(shouldCloseDoor(xi, yi) && type==0){
		d->setState(DS_CLOSED);
	}
}

void Board::addBoost(){
	int xi, yi;
	getRandomPos(xi, yi);
	Speed* tmpi = new Speed(this, xi, yi);
	tmpi->setPosition(this->coordToPos(xi, yi));
	items->addObject(tmpi);
	this->addChild(tmpi);
}

void Board::addBomb(bool noCenter){
	int xi, yi;
	getRandomPos(xi, yi, noCenter);
	Item *tmpi = new Bomb(this, xi, yi);
	items->addObject(tmpi);
	this->addChild(tmpi, 5);//para que se muestren atras de los pjs siempre
}

void Board::addPointItem(){
	int xi, yi;
	getRandomPos(xi, yi);
	Point* tmpi = new Point(this, xi, yi);
	tmpi->setPosition(this->coordToPos(xi, yi));
	items->addObject(tmpi);
	this->addChild(tmpi);
}

void Board::resetBoost(Item *sender){
	items->removeObject(sender);
	c_spawn_boost++;
}

void Board::resetBomb(Item *sender){
	items->removeObject(sender);
	c_spawn_bombs++;
}

void Board::resetPoint(Item *sender){
	items->removeObject(sender);
	c_spawn_points++;
}

Board::~Board(){
	CCLOG("board dying");
	this->clean();
	//no need to re register the events so we keep them until this class dies
	EVUNSUB(e_player_died)
	EVUNSUB(e_on_boost)
	EVUNSUB(e_on_bomb)
	EVUNSUB(e_on_point)
	EVRELEASE(e_game_over)
}

CCPoint Board::coordToPos(int xi, int yi){
	//CCPoint off=ccp(SPRITE_RAD, +SPRITE_RAD); unneded
	return ccp(xi*SPRITE_RAD, yi*SPRITE_RAD);
}
