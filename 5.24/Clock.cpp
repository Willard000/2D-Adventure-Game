#include "Clock.h"

#include "FileReader.h"

#define FILE_CLOCK_FPS "iclock_fps"

Clock::Clock() :
	_cap				(_CLOCK_FPS ),
	_ms					( 1000.0 / _cap ),
	_is_limit			( _cap > 0 ? true : false ),
	_frames				( 0 ),
	_time				( 0.0 ),
	_update_ticks		( 0 ),
	_fms				( 0.0 ),
	_previous_ticks		( SDL_GetTicks() )
{
	FileReader file(_FILE_PATH);
	if (file.exists(FILE_CLOCK_FPS)) {
		_cap = file.get_int(FILE_CLOCK_FPS);
		_ms = 1000.0 / _cap;
		_is_limit = _cap > 0 ? true : false;
	}
}

bool Clock::update(Uint32 interval) {
	update_time();
	_frames++;

	if (_is_limit && (_time < _ms)) {
		double delay = _ms - _time;
		if (delay > 0.0) {
			SDL_Delay(Uint32(delay));
			update_time();
		}
	}

	if (_previous_ticks >= _update_ticks) {
		_fms = 1000.0 / double(_frames);
		_frames = 0;
		_update_ticks += interval;
		return true;
	}

	return false;
}

void Clock::limit(bool limit) {
	_is_limit = limit;
}

void Clock::set_cap(int cap) {
	_cap = cap;
	_ms = 1000.0 / _cap;
}

void Clock::update_time() {
	_ticks = SDL_GetTicks() - _previous_ticks;
	_time = double(_ticks / 1000.0);
	_previous_ticks = SDL_GetTicks();
}

// Time format (00:00:00:000)
// days : hours : minutes : seconds : miliseconds
std::string Clock::get_display_time() {
	double miliseconds = SDL_GetTicks() / 1000.0;
	miliseconds = miliseconds - floor(miliseconds);
	miliseconds *= 1000.0;

	int ticks = (int)(SDL_GetTicks() / 1000.0);
	int seconds = ticks % 60;
	int minutes = int(ticks / 60) % 60;
	int hours = int(ticks / 3600) % 24;
	int days = int(ticks / 86400);

	std::string time = "";

	time += (days > 0) ? std::to_string(days) + ":" : "";
	time += ((hours < 10) ? "0" : "") + std::to_string(hours) + ":";
	time += ((minutes < 10) ? "0" : "") + std::to_string(minutes) + ":";
	time += ((seconds < 10) ? "0" : "") + std::to_string(seconds) + ":";
	time += std::to_string((int)miliseconds);

	return time;
}