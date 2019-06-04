#include "Timer.h"

Timer::Timer() {
	_time = 0;
	_prevTime = clock();
	_isPaused = true;
}

Timer::Timer(int time) {
	_time = time;
	_prevTime = clock();
	_isPaused = false;
}

void Timer::set(int time) {
	_time = time;
	_isPaused = false;
}

void Timer::pause() {
	_isPaused = !_isPaused;
}

bool Timer::update() {
	if (!_isPaused && (clock() - _prevTime > _time)) {
		_prevTime = clock();
		return true;
	}
	return false;
}