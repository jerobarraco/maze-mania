#include "wavetimer.h"
#include "utils/Files.h"


static cus WT_MAXTX = 9;
static us _frames =1;//divide by zero avoid
static CCTexture2D *WT_TXS[WT_MAXTX] = {NULL};//should initialize every item to null, exactly as we wanted (//)read unload)

void WaveTimer::onPoint(void *self, void *sender, void *param){
	((WaveTimer*)self)->step(*(int*)param);
}

void WaveTimer::step(int c){
	if(done) return;

	OPTIONS.points += c;

	us step = OPTIONS.points/ppf;

	if (step>=_frames) step = _frames-1;

	if(cur_step!=step){
		cur_step=step;
		this->setTexture(WT_TXS[cur_step]);
		this->setScale(1.0);
		CCScaleTo *scale1 = CCScaleTo::create(0.1f, 1.25);
		CCScaleTo *scale2 = CCScaleTo::create(0.2f, 1.0);

		this->runAction(CCSequence::createWithTwoActions(scale1, scale2));
	}

	if (OPTIONS.points >= OPTIONS.max_points){
		done = true;
		int w = GO_WAVEOVER;
		e_wave_over->fire(this, &w);
		//wave over triggers game over, which pauses everything, item subclass handles pausing, it should pause
		//the scheduler here
	}
}

/*void WaveTimer::doStep(){//esto es para probar despues se saca cuando esten listos los items
	int p = 10;
	//cada item debe lanzar el suyo, con sus puntos como param, puede ser negativo
	e_points->fire(this, &p);
}*/

void WaveTimer::unload(){
	unloadTXCache(WT_MAXTX, files::game::w_timers, WT_TXS);
}

void WaveTimer::load(){
	//estas vars simples ya las inicialice en el constructor porque asi debe ser
	//pero las pongo aca por si algun dia es necesario resetear la clase en "caliente" (sin recrear el objeto)
	_frames = skinDef()->c_timer;
	preloadTXCache(_frames, files::game::w_timers, WT_TXS);

	ppf = (float)OPTIONS.max_points/(float)(_frames);
	done = false;
	//float delay = 0.05;
	//this->schedule(schedule_selector(WaveTimer::doStep), delay, 1000, delay );
	//to fire
	e_wave_over = EM.get(events::game::GAMEOVER);
	//to listen
	e_points = EM.subscribe(this, events::game::items::POINT, WaveTimer::onPoint);
	CCLOG("new wave timer wave:%d points:%d ", OPTIONS.wave, OPTIONS.max_points);
}

WaveTimer::WaveTimer(us xi, us yi):Item(b, xi, yi){
	this->initWithFile(fullPath(files::game::w_timers[0]));//necesario
	cur_step = 0;
	done = false;
	e_wave_over = NULL;
	e_points = NULL;
	this->load();
}

