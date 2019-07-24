#include <SDL.h>

#ifndef TIMER_H
#define TIMER_H

class Timer {
public:
	Timer();
	Timer(int time);
	void set(int time);
	void pause();
	void reset();

	bool update();

	int get_time();
private:
	int _time;
	Uint32 _prev_time;
	bool _is_paused;
};

#endif