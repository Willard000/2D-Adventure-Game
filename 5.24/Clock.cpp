#include "Clock.h"

#include "FileReader.h"

Clock::Clock() {
	FileReader file(FILE_PATH);
	_cap = _CLOCK_FPS;
	if (file.exists(FILE_CLOCK_FPS)) _cap = file.get_int(FILE_CLOCK_FPS);

	_ms = 1000.0 / _cap;
	_isLimit = _cap > 0 ? true : false;
	_frames = 0;
	_time = 0.0;
	_updateTicks = 0;
	_fms = 0.0;
	_previousTicks = SDL_GetTicks();
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