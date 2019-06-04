#include <time.h>

#ifndef TIMER_H
#define TIMER_H

class Timer {
public:
	Timer();
	Timer(int time);
	void set(int time);
	void pause();

	bool update();

private:
	int _time, _prevTime;
	bool _isPaused;
};

#endif