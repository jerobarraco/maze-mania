#ifndef WAVETIMER_H
#define WAVETIMER_H
#include "Item.h"
#include "utils/events.h"
#include "utils/Utils.h"

class WaveTimer : public Item{
	Event *e_wave_over, *e_points;
	bool done;
	float ppf;//points per frame
	us cur_step;
public:
	static void onPoint(void *self, void *sender, void *param);
	void step(int c);
//	void doStep();
	static void unload();
	void load();

	WaveTimer(us xi, us yi);
	~WaveTimer(){
		EVRELEASE(e_wave_over)
		EVUNSUB(e_points)
	}
};

#endif // WAVETIMER_H
