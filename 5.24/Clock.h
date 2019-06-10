#include <SDL.h>

#ifndef CLOCK_H
#define CLOCK_H

#define UPDATE_INTERVAL 1000

class Clock {
public:
	Clock();
	Clock(int cap);

	bool update();

	void limit(bool limit);
	void setCap(int cap);

	double getTime() { return _time; }
	double getFMS() { return _fms; }
private:
	void updateTime();
private:
	int _frames, _cap;
	double _fms, _ms, _time;
	Uint32 _ticks, _previousTicks, _updateTicks;
	bool _isLimit;
};

#endif
