#include "Timer.h"

Timer::Timer() :
	_time			( 0 ),
	_prev_time		( SDL_GetTicks() ),
	_is_paused		( true )
{}

Timer::Timer(int time) :
	_time			( time ),
	_prev_time		( SDL_GetTicks() ),
	_is_paused		( false )

{}

void Timer::set(int time) {
	_time = time;
	_is_paused = false;
}

void Timer::pause() {
	_is_paused = !_is_paused;
}

void Timer::reset() {
	_prev_time = SDL_GetTicks();
}

bool Timer::update() {
	if (_time > -1 && !_is_paused && ((int)(SDL_GetTicks() - _prev_time) > _time)) {
		_prev_time = SDL_GetTicks();
		return true;
	}
	return false;
}

int Timer::get_time() {
	return _time;
}