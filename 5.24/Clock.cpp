#include "Clock.h"

Clock::Clock() {
	_frames = 0;
	_isLimit = true;
	_time = 0.0;
	_updateTicks = 0;
	_fms = 0.0;
	_cap = 60;
	_ms = 1000.0 / _cap;
	_previousTicks = SDL_GetTicks();
}

Clock::Clock(int cap)
	: Clock() {
	_cap = cap;
	_ms = 1000.0 / _cap;
	if (cap <= 0) {
		_isLimit = false;
	}
}

bool Clock::update() {
	updateTime();
	_frames++;

	if (_isLimit && (_time < _ms)) {
		double delay = _ms - _time;
		if (delay > 0.0) {
			SDL_Delay(Uint32(delay));
			updateTime();
		}
	}

	if (_previousTicks >= _updateTicks) {
		_fms = 1000.0 / double(_frames);
		_frames = 0;
		_updateTicks += UPDATE_INTERVAL;
		return true;
	}

	return false;
}

void Clock::limit(bool limit) {
	_isLimit = limit;
}

void Clock::setCap(int cap) {
	_cap = cap;
	_ms = 1000.0 / _cap;
}

void Clock::updateTime() {
	_ticks = SDL_GetTicks() - _previousTicks;
	_time = double(_ticks / 1000.0);
	_previousTicks = SDL_GetTicks();
}