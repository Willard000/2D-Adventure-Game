#include "Timer.h"

Timer::Timer() :
	_time			( 0 ),
	_prev_time		( SDL_GetTicks() ),
	_is_paused		( true )
{}

Timer::Timer(Uint32 time) :
	_time			( time ),
	_prev_time		( SDL_GetTicks() ),
	_is_paused		( false )

{}

void Timer::set(Uint32 time) {
	_time = time;
	_is_paused = false;
}

void Timer::pause() {
	_is_paused = !_is_paused;
}

bool Timer::update() {
	if (!_is_paused && ((SDL_GetTicks() - _prev_time) > _time)) {
		_prev_time = SDL_GetTicks();
		return true;
	}
	return false;
}