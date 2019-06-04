#include <SDL.h>
#include <chrono>

#ifndef CLOCK_H
#define CLOCK_H

namespace hrclk {
	typedef std::chrono::high_resolution_clock::time_point clock_tp;
	typedef std::chrono::high_resolution_clock clock_hrc;

}

class Clock {
public:
	Clock();
	Clock(double cap);

	bool update();

	void limit(bool limit);
	void setCap(double cap);

	double getTime() { return _time; }
	double getFMS() { return _fms; }
private:
	const double UPDATE_INTERVAL = 1000.0;  // 1 second
	int _frames = 0;
	bool _isLimit = true;
	double _time, _lastTime, _fms, _cap, _ms;
	hrclk::clock_tp	_previous, _last;
};

#endif