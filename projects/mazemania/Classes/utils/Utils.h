/*
 * Utils.hpp
 *
 *  Created on: 27/03/2014
 *      Author: nande
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_
#include <string.h>
//string HA!
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//if defined(WIN32) && defined(_WINDOWS)
	static double round(double d){
		return floor(d + 0.5);
	}
#endif

#define For(iter, iterable) for(typeof((iterable).begin()) iter = (iterable).begin(); iter != (iterable).end(); ++iter)

//does the same as CC_SAFE_DELETE_NULL but cleaner and also doesnt depends on cocos, and is shorter
//Safely releases an object and nullifies it
#define RELEASE(_p) if (_p) { _p->release(); _p = NULL; }

//the same but also removes it from the parent
//order is important, it might NOT be in the parent, in that case, removingItFromParent first has no effect
//but if it doesn't have a parent and we release it first, it'll get deleted, and tryinǵ to remove it
//will result in a sigsegv
#define RERELEASE(_p) if (_p) { _p->removeFromParent(); _p->release(); _p = NULL; }

//experimental for levels
//increments _p by _val, if p ends greater than _max, it gets cropped
#define INCTILL(_p, _val, _max, _min) _p+=_val; if (_p>_max) { _p= _max; }else if (_p<_min) { _p= _min; }

//types.h (estandard) @151 typedef unsigned short int ushort;
//ushort es mas chico, consume menos memoria, pero ojo que cuenta de 0 hasta 65,535
//Unsigned 16-bit integer
//el ushort no esta definido en blackberry
typedef unsigned short us;
typedef const us cus;

typedef const char *pcc;

/* configuration */
enum GameOverType {GO_WAVEOVER =-4, GO_GAMEOVER=-3, GO_TIMEOUT=-2, GO_PAUSE = -1};

//directions
enum Direction {RIGHT=0, UP, LEFT, DOWN, CENTER};
inline Direction rotateDirection(Direction d, int by){
	//values less than -4 are undefined
	if(d == CENTER){
		return CENTER;
	}
	return (Direction) ((d+by+4)%4);//+4 to handle negative directions correctly
}

/*SPRITE*/
cus SPRITE_RAD = 60;
static const cocos2d::CCPoint DIR_POS [] = {
	ccp(SPRITE_RAD*2, SPRITE_RAD), //RIGHT
	ccp(SPRITE_RAD, SPRITE_RAD*2), //UP
	ccp(0, SPRITE_RAD), //LEFT
	ccp(SPRITE_RAD, 0), //DOWN
	ccp(SPRITE_RAD, SPRITE_RAD) //CENTER
};

//deprecated, use M_CANT (in the enum), if it results better, change any other like that
//static const unsigned short C_MODES = 6; //goal, bomb, swarm, timer, boost, wave
//deprecated, use LV_CANT
//static cus C_WAVES = 30; //if you change me you have to change DevMenu menu creation too
static cus PLAYER_MAX = 6; //if you change me you have to change DevMenu menu creation too

class Globals{
	//inmutable stuff or stuff that doesnt need to change on a per game basis and that NEEDS to be defined with a
	//reasonable value
public:
	us px_width, px_height, off_x, off_y;
	us cent_x, cent_y;
	us skin;
	bool audio;
};

class Options {
public:
	enum Mode {M_GOAL=0, M_TIMER, M_ADOORS, M_DEMO, M_WAVE, M_CANT};//M_CANT is cuantity
	//enum Level{LV_ZERO=0, LV_EASY=0, LV_WZERO=5, LV_NORMAL=12, LV_LUNATIC=24, LV_CANT=30};
	enum Level{LV_ZERO=0, LV_EASY=0, LV_WZERO=0, LV_NORMAL=4, LV_LUNATIC=8, LV_CANT=10};

	us cplayers, units, max_bombs, max_boosts, max_ipoints;
	float t_player, t_game, t_door;
	unsigned int wave, points, max_points;
	//bool is_demo;//is_demo makes the board act in demo mode (no pause and no win/lose)
	bool penabled[PLAYER_MAX];
	bool modes[M_CANT]; //goal, bomb, swarm, timer, boost, wave//todo quitar
	int overflow[20];
	//LV_CANT is the quantity of levels, max = cant-1 //if you change me you have to change DevMenu menu creation too
	Options();
	void setForWave(unsigned int wave=0);
	void reset(bool base_only=true);
};

extern Options OPTIONS;
extern Globals GLOBALS;
//Globals are evil... well c++ aint no saint

//not really needed
cocos2d::CCScene *sceneForLayer(cocos2d::CCLayer *lyr);

char *itos(const int &i);

enum thing {T_INI=0, T_SPEED=0, T_MXPOINTS, T_IPOINTS, T_GOAL, T_TIMER, T_BOOST, T_DOOR, T_SWARM, T_BOMBS, T_CANT};
//array of things
//static const unsigned short _things[] = {T_POINT, };
//wave on wich it turns on
//static const unsigned short _turn_on_wave[] = { };
//level for each thing on each wave
//TODO speed and timer should have more granularity
//Levels should be more granular and standard ie
//0 means NO/OFF, and range is 1..10 no exceptions

/* ======== 30 waves =========
static const double _mode_prog[][Options::LV_CANT] = {
	
	//speed
	{0.1905, 0.2149, 0.1905, 0.3488, 0.5115, 0.5553, 0.507, 0.4616, 0.487, 0.5811, 0.6886, 0.7473, 0.7318, 0.668, 0.6139, 0.6214, 0.7016, 0.8156, 0.8977, 0.8989, 0.8228, 0.7273, 0.684, 0.7195, 0.7892, 0.8206, 0.8017, 0.7948, 0.8014, 0.9999},
	//max points (wave timer)
	{0.11, 0.36, 0.43, 0.57, 0.7, 0.75, 0.73, 0.68, 0.68, 0.71, 0.77, 0.82, 0.82, 0.8, 0.77, 0.76, 0.79, 0.84, 0.89, 0.9, 0.85, 0.77, 0.7, 0.68, 0.71, 0.74, 0.75, 0.8, 1, 1},
	//frutas
	{0.2124, 0.3505, 0.4516, 0.5836, 0.6908, 0.7352, 0.723, 0.6868, 0.6601, 0.6616, 0.6912, 0.7365, 0.7822, 0.8183, 0.8435, 0.8631, 0.8838, 0.9069, 0.9255, 0.9252, 0.8921, 0.8224, 0.7314, 0.6554, 0.6396, 0.7119, 0.8509, 0.9722, 0.9852, 1.0069},
	//goal
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	//timer
	{0, 0, 0, 0, 0, 0, 0, 0.2, 0.37, 0.55, 0.69, 0.74, 0.72, 0.66, 0.59, 0.56, 0.57, 0.6, 0.62, 0.62, 0.59, 0.55, 0.54, 0.57, 0.64, 0.69, 0.7, 0.72, 0.88, 1,},
	//boost
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.21, 0.43, 0.63, 0.75, 0.76, 0.66, 0.53, 0.44, 0.46, 0.57, 0.68, 0.72, 0.65, 0.58, 0.67, 0.91, 0.96, 1},
	//autodoor
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.13, 0.26, 0.42, 0.57, 0.69, 0.76, 0.77, 0.76, 0.78, 0.86, 0.96, 1.01, 1},
	//swarm
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.26, 0.41, 0.46, 0.49, 0.61, 0.75, 0.82, 0.99, 1},
	//bombs
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.19, 0.42, 0.58, 0.61, 0.8, 1}
};
===========================
*/


static const double _mode_prog[][Options::LV_CANT] = {
	//speed	
	{0.1905, 0.3488, 0.507, 0.5811, 0.7318, 0.6214, 0.8977, 0.7273, 0.8206, 1},
	//max points (wave timer)
	{0.11, 0.57, 0.73, 0.71, 0.82, 0.76, 0.89, 0.77, 0.75, 1},
	//frutas
	{0.2124, 0.5836, 0.723, 0.6616, 0.7822, 0.8631, 0.9255, 0.7314, 0.8509, 1},
	//goal
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	//timer
	{0, 0, 0.2, 0.69, 0.66, 0.57, 0.62, 0.54, 0.7, 1},
	//boost
	{0, 0, 0, 0.21, 0.75, 0.53, 0.57, 0.65, 0.91, 1},
	//autodoor
	{0, 0, 0, 0, 0.13, 0.57, 0.69, 0.77, 0.86, 1},
	//swarm
	{0, 0, 0, 0, 0, 0.26, 0.41, 0.61, 0.82, 1},
	//bombs
	{0, 0, 0, 0, 0, 0, 0.19, 0.42, 0.61, 1}
};


//lo que son cantidad se pueden poner el inicio a 0
//lo que son duraciones no, a esas en setForWave hay que checkear primero el POSATLVL y recien ahi sacar el VALATPOS, para las otras el VALATLVL basta
//hacerlo de otra forma deforma los resultados
static const double _mode_pars[][2]={
	{1.2, -1.05},//{1.5, -1.35},//pspeed (dur) ini, dif(fin-ini)//player speed
	{0, 1000},//wave timer (puntos)
	{10, -7},//frutas (cantidad) (1=100pts)
	{0, 1},//goal (cantidad)
	{210, -110},//{110, -90},//timer (duración)
	{0, 6},//boost (cantidad)
	{10, -8.7},//auto-door (delay segundos)
	{1, 3},//swarm (cantidad)
	{0, 6}//bombas (cantidad)
};

//linear interpolation, the difficulty progress at a level
#define POS4LVL(_thing, _lvl) _mode_prog[_thing][_lvl]

//lineal interpolation of the difficulty at certain progression (0..1)
#define VAL4POS(_thing, _prog) _mode_pars[_thing][0]+(_mode_pars[_thing][1]*_prog)

//lineal interpolation of the value at certain level
#define VAL4LVL(_thing, _lvl) VAL4POS(_thing, POS4LVL(_thing, _lvl))

#endif /* UTILS_HPP_ */
