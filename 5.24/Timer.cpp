#include "Timer.h"

Timer::Timer() {
	_time = 0;
	_prevTime = SDL_GetTicks();
	_isPaused = true;
}

Timer::Timer(Uint32 time) {
	_time = time;
	_prevTime = SDL_GetTicks();
	_isPaused = false;
}

void Timer::set(Uint32 time) {
	_time = time;
	_isPaused = false;
}

void Timer::pause() {
	_isPaused = !_isPaused;
}

bool Timer::update() {
	if (!_isPaused && ((SDL_GetTicks() - _prevTime) > _time)) {
		_prevTime = SDL_GetTicks();
		return true;
	}
	return false;
}