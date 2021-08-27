/*
 * Event.h
 *
 *  Created on: Apr 14, 2014
 *      Author: nande
 * Copyright Jerónimo Barraco Mármol GPLv3
 */

#ifndef EVENT_H_
#define EVENT_H_
#include <vector>
#include <map>
#include <string>

using namespace std;

/* I tried using an approach to be able to make callbacks into bound methods, but it sucked a lot
 * because i still need to store the "this" pointer at some structure (subscriber) and also to be able to
 * call a bounded method i need it to belong to a certain class (really c++¿) so basically i needed an extra
 * class just to be able to do (class->*cb)( ) which sucked like a black hole (seriously what's up with that syntax!)
 * it also prevented me to create callbacks to static methods..
 * so i rather just do Board* diz = (Board*) sender; and fuck the world
 * also it went against the K.I.S.S. principle.
 */
typedef void (*Callback)(void *self, void *sender, void *param);

struct Subscriber{
	Callback cb;
	void* self;
};

typedef std::vector<Subscriber* > SVec;

class EventManager; //forward
class Event {
	int refcount;
	SVec subs;//criptions
	std::string name;
	EventManager* em;
	void constructor(); //Because Suck++
public:
	Event(EventManager *evm, std::string name);
	Event(const Event &other);//for map
	Event& operator=(const Event &other);//for crappy map
	void subscribe(void* self, Callback cb);
	void unsubscribe(void *self);
	void fire(void * sender, void* param, const bool &release=false);
	void clear();
	friend bool operator<(const Event &a, const Event& b){
		return !! (int)(&a<&b);//i dont really care on sorting, why should i?;
	}
	Event();
	~Event(){
		clear();
	}

	//needed only on fires, subscribe already retains, not thread safe
	void retain(){
		this->refcount++;
	}
	//needed only on fires, unsubscribe already releases, not thread safe
	void release();
};

typedef std::map<std::string, Event*> EMap;

class EventManager{
	friend class Event;
	EMap events;
	void removeEvent(Event* e);
public:
	Event *subscribe(void* self, std::string name, Callback cb);
	void unsubscribe(void* self, std::string name);
	Event *get(std::string name, const bool &retain=true);
	void clear();
	~EventManager();
};


extern EventManager EM;
/*
EventManager EM;
void PlayerDied(void* self, void* sender, void* p){
    cout << "self " << (long)self << " sender " << sender << " p " << (char*)p << endl;
};
int main(){
  Event* ev = EM.get("PlayerDied");
  int a = 60;
  char *c = "c";
  ev->subscribe(NULL, PlayerDied);
  cout <<" probando"<< endl;
  //"dios ayudanos plz.jpg"
  ev->fire((void*)1, c);
}
*/

//example of an event without event manager
//extern Event playerDied;

typedef const char *pcc;
namespace events{
	namespace com{
		static pcc SPB_CHANGED = "sboxchanged";//parameter: int new value //re qt
	}
	namespace menu{
		static pcc START_MENU = "showstartmenu";
		static pcc DEV_MENU = "showdevmenu";
		//static pcc ABOUT = "showabout";
		static pcc BACK = "back";
		static pcc BACK_SUB = "back_sub";
	}
	namespace game{
		static pcc HELP = "showhelp";
		static pcc GAMEOVER = "gameover";
		static pcc PAUSE = "pause";
		static pcc PLAYER_DIED = "playerdied";
		static pcc RESUME = "resume";
		static pcc RESTART_WAVE = "restartwave";
		namespace items {
			static pcc BOOST = "item_activated_boost";
			static pcc BOMB = "item_activated_bomb";
			static pcc POINT = "item_activated_point";
		}
	}
	namespace scenes{
		static pcc INTRO = "intro";
		static pcc MENU = "menu";
		static pcc PLAY = "play";//parameter, Options object
		static pcc ABOUT = "about";
		static pcc HELP = "help";
	}
	namespace app{
		static pcc QUIT = "app_quit";
		static pcc RESUME = "app_resume";
		static pcc RELOAD = "app_reload";//parameter, int:new skin index(0:pixelmode, 1:sherman (read files.h))
	}
	namespace loader{
		static pcc START = "load_start";//parameter, total steps to have
		static pcc STEP = "load_step"; //parameter, ammount of step ADVANCED (increments a count)
		static pcc STOP = "load_stop";
	}
}

#define EVUNSUB(_e) if(_e){ _e->unsubscribe(this); _e=NULL;}
#define EVRELEASE(_e) if(_e){ _e->release(); _e=NULL;}
#endif /* EVENT_H_ */
