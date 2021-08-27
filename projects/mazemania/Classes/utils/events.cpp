/*
 * Event.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: nande
 * Copyright Jerónimo Barraco Mármol GPLv3 
 */

#include "utils/events.h"
//Event playerDied;

EventManager EM;
//Event playerDied;

void Event::constructor(){
	refcount = 0;
	name = "";
	em = NULL;
}

Event::Event(){
	constructor();
}

Event::Event(EventManager *evm, string name){
	constructor();
	this->name = name;
	this->em = evm;
}

Event::Event(const Event &other){//:Event() can't delegate the constructor on qnx compiler
	constructor();
	*this=other;
	//todo check if this has something to do with refcount
}

Event& Event::operator=(const Event &other){//TODO does this breaks refcounts?
	clear();
	for (unsigned int i = 0 ; i<other.subs.size(); i++){
		Subscriber *s = other.subs[i];
		subscribe(s->self, s->cb);
	};
	name = other.name;
	em = other.em;
	return *this;
}

void Event::subscribe(void* self, Callback cb) {
	//Listener should call subscribe, and unsubscribe on deletion
	Subscriber *s = new Subscriber();
	s->cb = cb;
	s->self = self;
	subs.push_back(s);
	this->retain();//thread unsafe
}

void Event::unsubscribe(void *self){
	//clients should call this upon destruction
	SVec::iterator it;
	Subscriber *s;
	for (it = subs.begin(); it != subs.end(); it++){
		s = *it;
		if(self){
			if (s->self == self){
				subs.erase(it);
				this->release();
				return;
			};
		};
	}
}

void Event::fire(void * sender, void* param, const bool &release) {
	/*senders calls this to fire an event, senders do not need to hold reference to the event to call this method
	 * but for performance reasons it would be wise. In that case it should retain a reference using retain
	 */
	Subscriber *s;
	SVec::iterator it;
	//this->retain();//in case all the listeners remove themselves while an event was fired
	//not actually needed because the iterators are invalidated
	for (it = subs.begin(); it != subs.end() && (*it != NULL); it++){
		//*it != NULL because iterators are invalidated if the list is modified while iterating
		s = *it;
		s->cb(s->self, sender, param);
	}
	//dangerous stuff
	if(release) this->release();
}

void Event::clear(){
	//can be called to remove all the subscribers. the subscribers still need to call unsubscribe because a subscriber could hold a reference to this
	SVec::iterator it;
	Subscriber *s;
	for (it = subs.begin(); it != subs.end(); it++){
		s = *it;
		delete s;
	};
	subs.clear();
}

void Event::release(){
	this->refcount--;
	if(this->refcount<=0){//Adiue cruel world~~~ ;_;
		if(em){
			em->removeEvent(this);
		}
		delete this;
	}
}

/* Event Manager */
Event* EventManager::subscribe(void* self, std::string name, Callback cb){
	Event *e = this->get(name, false);//dont retain because subscribe will do it
	if (!e) return NULL;
	e->subscribe(self, cb);
	return e;
}

void EventManager::unsubscribe(void *self, string name){
	//test, is slower than holding reference to the event
	this->get(name, false)->unsubscribe(self);
}

Event* EventManager::get(std::string name, const bool &retain){
	//retain automatically retains the event
	//i dont remember why i added it, proably to allow to do
	//EM.get(xxx)->fire(); and avoid getting an error
	//but i dont remember why, maybe to avoid writing ev->retain after getting the event...
	//you could get and fire an event without retaining it (i think (cant remember))
	//because if a subscriber unsubscribe in the firing process, the iterator gets invalidated and hence it exists
	//you could use EM.get(xxx)->fire(, true); or EM.get(xxx)->fire() but dont ever put true in the second,
	//it might fail if it had one subscriber that unsubscribed and the event get deleted
	Event *ev = NULL;
	//ev = events[name];//using this we get a segmentation fault :)
	try{
		EMap::iterator pair = events.find(name);
		if(pair != events.end()){
			ev = pair->second;
		}
		//at doesnt exists and [] fails
	}catch(...){
	 //doesnt matters
	}
	if(!ev){//handles error and non-existence
		ev = new Event(this, name);
		events.insert(std::pair<std::string, Event*> (name, ev));
	}
	//getting an event will always retain it, you should manually unsubscribe or releas
	//unless explecitly not retained (case of subscribe)
	if(retain) ev->retain();
	return ev;
}

void EventManager::clear(){
	Event *e;
	EMap::iterator it;
	for (it=events.begin(); it!=events.end(); ++it){
		e = it->second;
		delete e;
	}
	events.clear();
}

EventManager::~EventManager(){
	clear();
}

void EventManager::removeEvent(Event* e){
	EMap::iterator it;
	for (it = events.begin(); it != events.end(); it++){
		if(e == it->second){
			events.erase(it);
			break;
		}
	}
}
