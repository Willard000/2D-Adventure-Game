#include "Timer.h"

Timer::Timer() :
	_time			( 0 ),
	_clock			( 0 ),
	_prev_time		( (int)SDL_GetTicks() ),
	_is_paused		( true )
{}

Timer::Timer(int time) :
	_time			( time ),
	_clock			( 0 ),
	_prev_time		( (int)SDL_GetTicks() ),
	_is_paused		( false )

{}

Timer::Timer(const Timer &rhs) :
	_time			( rhs._time ),
	_clock			( rhs._clock ),
	_prev_time		( rhs._prev_time ),
	_is_paused		( rhs._is_paused )
{}

void Timer::set(int time) {
	_time = time;
	_is_paused = false;
}

void Timer::pause() {
	if (_is_paused) {
		_prev_time = (int)SDL_GetTicks() - _clock;
	}
	else {
		_clock = (int)SDL_GetTicks() - _prev_time;
	}
	_is_paused = !_is_paused;
}

void Timer::reset() {
	_prev_time = (int)SDL_GetTicks();
}

void Timer::offset(int time) {
	_prev_time -= time;
}

bool Timer::update() {
	if (_time > -1 && !_is_paused && (((int)SDL_GetTicks() - _prev_time) > _time)) {
		_prev_time = (int)SDL_GetTicks();
		return true;
	}
	return false;
}

int Timer::get_time() {
	return _time;
}

int Timer::get_clock() {
	if (!_is_paused) {
		_clock = (int)SDL_GetTicks() - _prev_time;
	}
	return _clock;
}