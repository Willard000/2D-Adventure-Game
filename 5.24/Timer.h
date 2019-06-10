#include <SDL.h>

#ifndef TIMER_H
#define TIMER_H

class Timer {
public:
	Timer();
	Timer(Uint32 time);
	void set(Uint32 time);
	void pause();

	bool update();

private:
	Uint32 _time, _prevTime;
	bool _isPaused;
};

#endif