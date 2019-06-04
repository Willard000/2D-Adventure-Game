#include "Clock.h"

Clock::Clock() {
	_frames = 0;
	_isLimit = true;
	_time = 0.0,
	_lastTime = clock(),
	_fms = 0.0,
	_cap = 60.0,
	_ms = 1000.0 / _cap;
	_previous = hrclk::clock_hrc::now(),
	_last = hrclk::clock_hrc::now();
}

Clock::Clock(double cap)
	: Clock() {
	_cap = cap;
	_ms = 1000.0 / _cap;
	if (cap < 0) {
		_isLimit = false;
	}
}

bool Clock::update() {
	_time = std::chrono::duration_cast<std::chrono::duration<double>>(hrclk::clock_hrc::now() - _previous).count() * 1000.0;
	_previous = hrclk::clock_hrc::now();

	_frames++;

	if (_isLimit && (_time < _ms)) {
		Uint32 delay = (Uint32)(_ms - _time);
		if (delay > 0) {
			SDL_Delay(delay);
			_time = std::chrono::duration_cast<std::chrono::duration<double>>(hrclk::clock_hrc::now() - _previous).count() * 1000.0;
			_previous = hrclk::clock_hrc::now();
		}
	}

	if ((clock() - _lastTime) > UPDATE_INTERVAL) {
		_fms = 1000.0 / double(_frames);
		_frames = 0;
		_lastTime += UPDATE_INTERVAL;
		return true;
	}

	return false;
}

void Clock::limit(bool limit) {
	_isLimit = limit;
}

void Clock::setCap(double cap) {
	_cap = cap;
	_ms = 1000.0 / _cap;
}