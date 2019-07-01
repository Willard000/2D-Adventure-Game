#include <SDL.h>

#include <string>

#ifndef CLOCK_H
#define CLOCK_H

#define UPDATE_INTERVAL 1000

class Clock {
public:
	Clock();

	bool update(Uint32 interval = UPDATE_INTERVAL);

	void limit(bool limit);
	void set_cap(int cap);

	double get_time() { return _time; }
	double get_fms() { return _fms; }

	std::string get_display_time();
private:
	void update_time();
private:
	const char *_FILE_PATH = "Data/system.txt";
	const int _CLOCK_FPS = 60;
	int _frames, _cap;
	double _fms, _ms, _time;
	Uint32 _ticks, _previous_ticks, _update_ticks;
	bool _is_limit;
};

#endif
