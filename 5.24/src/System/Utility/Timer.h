#include <SDL.h>

#ifndef TIMER_H
#define TIMER_H

class Timer {
public:
	Timer();
	Timer(int time);
	Timer(const Timer &rhs);

	void set(int time);
	void pause();
	void reset();
	void offset(int time);

	bool update();

	bool get_pause() { return _is_paused; }

	int get_time();
	int get_clock();
private:
	int _time;
	int _prev_time;
	int _clock;
	bool _is_paused;
};

#endif