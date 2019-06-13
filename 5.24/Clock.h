#include <SDL.h>

#ifndef CLOCK_H
#define CLOCK_H

#define UPDATE_INTERVAL 1000

#define FILE_CLOCK_FPS "iclock_fps"

class Clock {
public:
	Clock();

	bool update();

	void limit(bool limit);
	void setCap(int cap);

	double getTime() { return _time; }
	double getFMS() { return _fms; }
private:
	void updateTime();
private:
	const char *FILE_PATH = "Data/system.txt";
	const int _CLOCK_FPS = 60;
	int _frames, _cap;
	double _fms, _ms, _time;
	Uint32 _ticks, _previousTicks, _updateTicks;
	bool _isLimit;
};

#endif
